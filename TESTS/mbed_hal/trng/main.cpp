/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*
* The test is based on the assumption that trng will generate random data, random so
* there will not be any similar patterns in it, that kind of data will be impossible to
* compress, if compression will occur the test will result in failure.
*
* The test is composed out of three parts:
* the first, generate a trng buffer and try to compress it, at the end of first part
* we will reset the device.
* In second part we will generate a trng buffer with a different buffer size and try to
* compress it.
* In the third part we will again generate a trng buffer to see that the same trng output
* is not generated as the stored trng buffer from part one (before reseting), the new trng data will
* be concatenated to the trng data from the first part and then try to compress it
* together, if there are similar patterns the compression will succeed.
*
* We need to store and load the first part data before and after reset, the mechanism
* we will use is the mbed greentea platform for sending and receving the data from the device
* to the host running the test and back, the problem with this mechanism is that it doesn't
* handle well certain characters, especially non ASCII ones, so we use the base64 algorithm
* to ensure all characters will be transmitted correctly.
*/

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "hal/trng_api.h"
#include "base64b.h"
#include "pithy.h"
#include <stdio.h>
#include <string.h>
#include "mbedtls/config.h"
#include "mbedtls/platform.h"

#if !DEVICE_TRNG
#error [NOT_SUPPORTED] TRNG API not supported for this target
#else

#define MSG_VALUE_DUMMY                 "0"
#define MSG_VALUE_LEN                   64
#define MSG_KEY_LEN                     32

#define BUFFER_LEN                      (MSG_VALUE_LEN/2)

#define MSG_TRNG_READY                  "ready"
#define MSG_TRNG_BUFFER                 "buffer"
#define MSG_TRNG_EXIT                   "exit"

#define MSG_TRNG_TEST_STEP1             "check_step1"
#define MSG_TRNG_TEST_STEP2             "check_step2"
#define MSG_TRNG_TEST_SUITE_ENDED       "Test_suite_ended"

#define RESULT_SUCCESS                  0

using namespace utest::v1;

#if (defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))
#include "entropy.h"
#include "entropy_poll.h"
#include "crypto.h"
#if !defined(MAX)
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* Calculating the minimum allowed entropy size in bytes */
#define MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE \
            MAX(MBEDTLS_ENTROPY_MIN_PLATFORM, MBEDTLS_ENTROPY_BLOCK_SIZE)

void inject_entropy_for_psa()
{
    if (psa_crypto_init() == PSA_ERROR_INSUFFICIENT_ENTROPY) {
        uint8_t seed[MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE] = {0};
        /* inject some a seed for test*/
        for (int i = 0; i < MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE; ++i) {
            seed[i] = i;
        }

        /* don't really care if this succeed this is just to make crypto init pass*/
        mbedtls_psa_inject_entropy(seed, MBEDTLS_PSA_INJECT_ENTROPY_MIN_SIZE);
    }
}
#endif // (defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))

static int fill_buffer_trng(uint8_t *buffer, trng_t *trng_obj, size_t trng_len)
{
    size_t temp_size = 0, output_length = 0;
    int trng_res = 0;
    uint8_t *temp_in_buf = buffer;

    trng_init(trng_obj);
    memset(buffer, 0, BUFFER_LEN);

    while (true) {
        trng_res = trng_get_bytes(trng_obj, temp_in_buf, trng_len - temp_size, &output_length);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, trng_res, "trng_get_bytes error!");
        temp_size += output_length;
        temp_in_buf += output_length;
        if (temp_size >= trng_len) {
            break;
        }
    }

    temp_in_buf = NULL;
    trng_free(trng_obj);
    return 0;
}

void print_array(uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        utest_printf("%02x", buffer[i]);
    }
    utest_printf("\n");
}

static void compress_and_compare(char *key, char *value)
{
    trng_t trng_obj;
    uint8_t *out_comp_buf, *buffer;
    uint8_t *input_buf, *temp_buf;
    size_t comp_sz = 0;
    unsigned int result = 0;

#define OUT_COMP_BUF_SIZE ((BUFFER_LEN * 5) + 32)
#define TEMP_BUF_SIZE (BUFFER_LEN * 2)

    out_comp_buf = new uint8_t[OUT_COMP_BUF_SIZE];
    buffer = new uint8_t[BUFFER_LEN];
    temp_buf = new uint8_t[BUFFER_LEN * 2];
    input_buf = new uint8_t[BUFFER_LEN * 4];

    /*At the begining of step 2 load trng buffer from step 1*/
    if (strcmp(key, MSG_TRNG_TEST_STEP2) == 0) {
        /*Using base64 to decode data sent from host*/
        uint32_t lengthWritten = 0;
        uint32_t charsProcessed = 0;
        result = trng_DecodeNBase64((const char *)value,
                                    MSG_VALUE_LEN,
                                    buffer,
                                    BUFFER_LEN,
                                    &lengthWritten,
                                    &charsProcessed);
        TEST_ASSERT_EQUAL(0, result);
        memcpy(input_buf, buffer, BUFFER_LEN);
    }

    if (strcmp(key, MSG_TRNG_TEST_STEP1) == 0) {
        /*Fill buffer with trng values*/
        result = fill_buffer_trng(buffer, &trng_obj, BUFFER_LEN);
        TEST_ASSERT_EQUAL(0, result);
        memcpy(input_buf, buffer, BUFFER_LEN);
    }
    /*pithy_Compress will try to compress the random data, if it succeeded it means the data is not really random*/
    else if (strcmp(key, MSG_TRNG_TEST_STEP2) == 0) {

        comp_sz = pithy_Compress((char *)buffer,
                                 BUFFER_LEN,
                                 (char *)out_comp_buf,
                                 OUT_COMP_BUF_SIZE,
                                 9);
        if (comp_sz <= BUFFER_LEN) {
            print_array(buffer, BUFFER_LEN);
        }
        TEST_ASSERT_MESSAGE(comp_sz > BUFFER_LEN,
                            "TRNG_TEST_STEP1: trng_get_bytes was able to compress thus not random");

        /*pithy_Compress will try to compress the random data with a different buffer size*/
        result = fill_buffer_trng(temp_buf, &trng_obj, TEMP_BUF_SIZE);
        TEST_ASSERT_EQUAL(0, result);

        comp_sz = pithy_Compress((char *)temp_buf,
                                 TEMP_BUF_SIZE,
                                 (char *)out_comp_buf,
                                 OUT_COMP_BUF_SIZE,
                                 9);
        if (comp_sz <= TEMP_BUF_SIZE) {
            print_array(temp_buf, TEMP_BUF_SIZE);
        }
        TEST_ASSERT_MESSAGE(comp_sz > TEMP_BUF_SIZE,
                            "TRNG_TEST_STEP2: trng_get_bytes was able to compress thus not random");

        memcpy(input_buf + BUFFER_LEN, temp_buf, TEMP_BUF_SIZE);
        /*pithy_Compress will try to compress the random data from before reset concatenated with new random data*/
        comp_sz = pithy_Compress((char *)input_buf,
                                 TEMP_BUF_SIZE + BUFFER_LEN,
                                 (char *)out_comp_buf,
                                 OUT_COMP_BUF_SIZE,
                                 9);
        if (comp_sz <= TEMP_BUF_SIZE + BUFFER_LEN) {
            print_array(input_buf, TEMP_BUF_SIZE + BUFFER_LEN);
        }
        TEST_ASSERT_MESSAGE(comp_sz > TEMP_BUF_SIZE + BUFFER_LEN,
                            "TRNG_TEST_STEP3: concatenated buffer after reset was able to compress thus not random");

        greentea_send_kv(MSG_TRNG_TEST_SUITE_ENDED, MSG_VALUE_DUMMY);
    }

    /*At the end of step 1 store trng buffer and reset the device*/
    if (strcmp(key, MSG_TRNG_TEST_STEP1) == 0) {
        int result = 0;
        /*Using base64 to encode data sending from host*/
        result = trng_EncodeBase64(buffer,
                                   BUFFER_LEN,
                                   (char *)out_comp_buf,
                                   OUT_COMP_BUF_SIZE);
        TEST_ASSERT_EQUAL(RESULT_SUCCESS, result);

        greentea_send_kv(MSG_TRNG_BUFFER, (const char *)out_comp_buf);
    }

    delete[] out_comp_buf;
    delete[] buffer;
    delete[] input_buf;
    delete[] temp_buf;
}

/*This method call first and second steps, it directs by the key received from the host*/
void trng_test()
{
    greentea_send_kv(MSG_TRNG_READY, MSG_VALUE_DUMMY);

    char key[MSG_KEY_LEN + 1] = { };
    char *value = new char[MSG_VALUE_LEN + 1];
    do {
        memset(key, 0, MSG_KEY_LEN + 1);
        memset(value, 0, MSG_VALUE_LEN + 1);

        greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);

        if (strcmp(key, MSG_TRNG_TEST_STEP1) == 0) {
            /*create trng data buffer and try to compress it, store it for later checks*/
            compress_and_compare(key, value);
        }

        if (strcmp(key, MSG_TRNG_TEST_STEP2) == 0) {
            /*create another trng data buffer and concatenate it to the stored trng data buffer
            try to compress them both*/
            compress_and_compare(key, value);
        }
    } while (strcmp(key, MSG_TRNG_EXIT) != 0);

    delete[] value;
}

Case cases[] = {
    Case("TRNG: trng_test", trng_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "trng_reset");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    int ret = 0;
#if defined(MBEDTLS_PLATFORM_C)
    ret = mbedtls_platform_setup(NULL);
#endif /* MBEDTLS_PLATFORM_C */
#if (defined(TARGET_PSA) && defined(COMPONENT_PSA_SRV_IPC) && defined(MBEDTLS_PSA_CRYPTO_C))
    inject_entropy_for_psa();
#endif
    ret = !Harness::run(specification);
#if defined(MBEDTLS_PLATFORM_C)
    mbedtls_platform_teardown(NULL);
#endif /* MBEDTLS_PLATFORM_C */
    return ret;
}

#endif // !DEVICE_TRNG

