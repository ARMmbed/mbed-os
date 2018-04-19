/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "math.h"

#define THREAD_STACK_SIZE 1024

#if !DEVICE_SAI
#error [NOT_SUPPORTED] SAI not supported for this target
#endif

using namespace utest::v1;

/* Since boards on CI do not have wire SAI loop-back connection, some of the functional
 * tests can not be executed.
 * If you want to fully test SAI support please connect pins as follows:
 * - SAI_A_SD   <--> SAI_B_SD
 * - SAI_A_BCLK <--> SAI_B_BCLK
 * - SAI_A_WCLK <--> SAI_B_WCLK
 * and enable loop-back tests by setting LOOPBACK_CONNECTION to true.
 */
#define LOOPBACK_CONNECTION (false)

#define BUFFER_SIZE 100

typedef enum
{
    LOOPBACK_TEST_OK, LOOPBACK_TEST_SKIP, LOOPBACK_TEST_FAILURE
} loopback_test_tatus_t;

static uint32_t transmit_buffer[BUFFER_SIZE];
static uint32_t receive_buffer[BUFFER_SIZE];

/* Auxiliary function which sets the specified buffer using specified pattern. */
static void set_buffer(void * buffer, uint32_t size, char pattern)
{
    char* p_byte = (char*) buffer;

    while (size--) {
        *p_byte = pattern;
        p_byte++;
    }
}

/* Auxiliary function which compares two buffers and returns true if both are the same. */
static bool compare_buffers(const void *buffer1, const void *buffer2, uint32_t size)
{
    const unsigned char *p1 = (const unsigned char *) buffer1;
    const unsigned char *p2 = (const unsigned char *) buffer2;
    while (size--) {
        if (*p1 != *p2) {
            return false;
        } else {
            p1++;
            p2++;
        }
    }

    return true;
}

/* Number of formats which must be supported by SAI device.
 * This variable defines number of obligate formats in test_sai_formats array. */
const uint32_t required_formats_count = 5;

/* Array of tested formats. */
static sai_format_t test_sai_formats[] = {
// Formats which are requested for all SAI devices
    sai_mode_i2s16,
    sai_mode_i2s16w32,
    sai_mode_i2s32,
    sai_mode_pcm16l,
    sai_mode_pcm16s,
// Other formats

};

/* Test that SAI class is able to communicate in loop-back mode.
 *
 * Given is SAI Class and SAI loop-back connection.
 * When SAI object is configured as transmitter with the specified format and
 *      another SAI object is configured as receiver with the same format used.
 * Then transmitter can successfully transmit samples to the receiver.
 */
void SAI_transmission_test()
{
    loopback_test_tatus_t status;

    if (!LOOPBACK_CONNECTION) {
        TEST_IGNORE_MESSAGE("NO LOOPBACK CONNECTION - TEST SKIPPED!");
        return;
    }

    char message[50];

    for (uint32_t f_idx = 0; f_idx < (sizeof(test_sai_formats) / sizeof(sai_format_t)); f_idx++) {

        SAI sai_transmitter(SAI_A_MCLK,                // mclk
                SAI_A_BCLK,                // bclk
                SAI_A_WCLK,                // wclk
                SAI_A_SD,                  // SD
                &test_sai_formats[f_idx],  // format
                false,                     // is input
                0,                         // master clock
                false                      // internal mclk
                );

        SAI sai_receiver(SAI_B_MCLK,                 // mclk
                SAI_B_BCLK,                 // bclk
                SAI_B_WCLK,                 // wclk
                SAI_B_SD,                   // SD
                &test_sai_formats[f_idx],   // format
                true,                       // is input
                0,                          // master clock
                false                       // internal mclk
                );

        // we should check init status before the actual test - for now there is no such option
        sai_result_t transmitter_init_status = SAI_RESULT_OK;
        sai_result_t receiver_init_status = SAI_RESULT_OK;

        /* Continue test only if SAI device has been successfully configured. */
        if (transmitter_init_status == SAI_RESULT_OK && receiver_init_status == SAI_RESULT_OK) {

            const uint32_t data_mask = ((1 << test_sai_formats[f_idx].data_length) - 1);

            /* Set to unexpected. */
            set_buffer(receive_buffer, sizeof(receive_buffer), 0xFF);

            for (int i = 0; i < BUFFER_SIZE; i++) {
                transmit_buffer[i] = data_mask / 100 * i;
            }

            uint32_t write_cnt = 0;
            uint32_t read_cnt = 0;
            uint32_t sample;

            /* Send samples and read them back. */
            while (1) {
                if (write_cnt < BUFFER_SIZE) {
                    sample = transmit_buffer[write_cnt];
                    if (sai_transmitter.xfer(&sample) == true) {
                        write_cnt++;
                    }
                }

                if (sai_receiver.xfer(&sample) == true) {
                    receive_buffer[read_cnt] = sample;
                    read_cnt++;
                }

                if (read_cnt == BUFFER_SIZE) {
                    break;
                }
            }

            if (compare_buffers(transmit_buffer, receive_buffer, BUFFER_SIZE) == true) {
                status = LOOPBACK_TEST_OK;
            } else {
                status = LOOPBACK_TEST_FAILURE;
            }
        } else {
            status = LOOPBACK_TEST_SKIP;
        }

        /* First `required_formats_count` formats are obligate, so we expect that
         * communication tests passes. Others formats may be supported, so test can pass or be skipped.
         */
        sprintf(message, "format idx: %lu", f_idx);
        if (f_idx < required_formats_count) {
            TEST_ASSERT_EQUAL_MESSAGE(LOOPBACK_TEST_OK, status, message);
        } else {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(LOOPBACK_TEST_FAILURE, status, message);
        }
    }
}

/* Test that SAITransmitter class is able to send samples in loop-back mode.
 * Test that SAIReceiver class is able to receive samples in loop-back mode.
 *
 * Given are SAITransmitter and SAIReceiver Classes and SAI loop-back connection.
 * When SAI object is configured as transmitter with the specified format and
 *      another SAI object is configured as receiver with the same format used.
 * Then transmitter can successfully transmit samples to the receiver.
 */
void SAITransmitter_SAIReceiver_transmission_test()
{
    loopback_test_tatus_t status;

    if (!LOOPBACK_CONNECTION) {
        TEST_IGNORE_MESSAGE("NO LOOPBACK CONNECTION - TEST SKIPPED!");
        return;
    }

    char message[50];

    for (uint32_t f_idx = 0; f_idx < (sizeof(test_sai_formats) / sizeof(sai_format_t)); f_idx++) {

        SAITransmitter sai_transmitter(SAI_A_MCLK, SAI_A_BCLK, SAI_A_WCLK, SAI_A_SD, &test_sai_formats[f_idx]);
        SAIReceiver sai_receiver(SAI_B_MCLK, SAI_B_BCLK, SAI_B_WCLK, SAI_B_SD, &test_sai_formats[f_idx]);

        // we should check init status before the actual test - for now there is no such option
        sai_result_t transmitter_init_status = SAI_RESULT_OK;
        sai_result_t receiver_init_status = SAI_RESULT_OK;

        /* Continue test only if SAI device has been successfully configured. */
        if (transmitter_init_status == SAI_RESULT_OK && receiver_init_status == SAI_RESULT_OK) {

            const uint32_t data_mask = ((1 << test_sai_formats[f_idx].data_length) - 1);

            /* Set to unexpected. */
            set_buffer(receive_buffer, sizeof(receive_buffer), 0xFF);

            for (int i = 0; i < BUFFER_SIZE; i++) {
                transmit_buffer[i] = data_mask / 100 * i;
            }

            uint32_t write_cnt = 0;
            uint32_t read_cnt = 0;
            uint32_t sample;

            /* Send samples and read them back. */
            while (1) {
                if (write_cnt < BUFFER_SIZE) {
                    sample = transmit_buffer[write_cnt];
                    if (sai_transmitter.send(sample) == true) {
                        write_cnt++;
                    }
                }

                if (sai_receiver.receive(&sample) == true) {
                    receive_buffer[read_cnt] = sample;
                    read_cnt++;
                }

                if (read_cnt == BUFFER_SIZE) {
                    break;
                }
            }

            if (compare_buffers(transmit_buffer, receive_buffer, BUFFER_SIZE) == true) {
                status = LOOPBACK_TEST_OK;
            } else {
                status = LOOPBACK_TEST_FAILURE;
            }
        } else {
            status = LOOPBACK_TEST_SKIP;
        }

        /* First `required_formats_count` formats are obligate, so we expect that
         * communication tests passes. Others formats may be supported, so test can pass or be skipped.
         */
        sprintf(message, "format idx: %lu", f_idx);
        if (f_idx < required_formats_count) {
            TEST_ASSERT_EQUAL_MESSAGE(LOOPBACK_TEST_OK, status, message);
        } else {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(LOOPBACK_TEST_FAILURE, status, message);
        }
    }
}

Case cases[] = {
    Case("SAI - loopback communication test.", SAI_transmission_test),
    Case("SAITransmitter/SAIReceiver - loopback communication test.", SAITransmitter_SAIReceiver_transmission_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
