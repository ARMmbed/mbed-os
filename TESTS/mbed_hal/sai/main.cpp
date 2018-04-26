/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
#include "sai_api.h"

#if !DEVICE_SAI
#error [NOT_SUPPORTED] SAI not supported for this target
#endif

using namespace utest::v1;

/* Since boards on CI do not have wire loop-back connection, some of the functional
 * tests can not be executed.
 * If you want to fully test SAI support please connect pins as follows:
 * - SAI_A_SD   <--> SAI_B_SD
 * - SAI_A_BCLK <--> SAI_B_BCLK
 * - SAI_A_WCLK <--> SAI_B_WCLK
 * and enable loop-back tests by setting LOOPBACK_CONNECTION to true.
 */
#define LOOPBACK_CONNECTION (false)

#define BUFFER_SIZE 100

#define SAMPLE_1 123
#define SAMPLE_2 124

typedef enum
{
    LOOPBACK_TEST_OK,
    LOOPBACK_TEST_SKIP,
    LOOPBACK_TEST_FAILURE
} loopback_test_tatus_t;

static uint32_t transmit_buffer[BUFFER_SIZE];
static uint32_t receive_buffer[BUFFER_SIZE];

/* Auxiliary function which inits SAI config structure. */
static void sai_config_create(sai_init_t *p_config, bool is_receiver, uint32_t sample_rate,
        const sai_format_t * p_format)
{
    if (is_receiver) {
        p_config->mclk = SAI_A_MCLK;
        p_config->sd = SAI_A_SD;
        p_config->bclk = SAI_A_BCLK;
        p_config->wclk = SAI_A_WCLK;
        p_config->is_receiver = true;
    } else {
        p_config->mclk = SAI_B_MCLK;
        p_config->sd = SAI_B_SD;
        p_config->bclk = SAI_B_BCLK;
        p_config->wclk = SAI_B_WCLK;
        p_config->is_receiver = false;
    }
    memcpy(&p_config->format, p_format, sizeof(p_config->format));
    p_config->sample_rate = sample_rate;
    p_config->mclk_source = SAI_CLOCK_SOURCE_INTERNAL;
    p_config->input_mclk_frequency = 0; // 0 means find it by yourself.
    p_config->output_mclk_frequency = p_config->format.word_length * p_config->sample_rate;
}

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
static sai_format_t test_sai_formats[] =
{
// Formats which are requested for all SAI devices
    sai_mode_i2s16,
    sai_mode_i2s16w32,
    sai_mode_i2s32,
    sai_mode_pcm16l,
    sai_mode_pcm16s,
// Other formats

};

/* Array of tested sample rates [Hz]. */
static uint32_t test_sample_rates[] =
{
    SAI_DEFAULT_SAMPLE_RATE,

};

/* Auxiliary function to test SAI loop-back communication using specified format and sample rate. */
static loopback_test_tatus_t sai_loopback_communication_test(sai_format_t *format, uint32_t sample_rate)
{
    sai_t sai_transmitter;
    sai_t sai_receiver;

    sai_init_t sai_transmitter_init = { };
    sai_init_t sai_receiver_init = { };

    sai_config_create(&sai_transmitter_init, false, SAI_DEFAULT_SAMPLE_RATE, format);
    sai_config_create(&sai_receiver_init, true, SAI_DEFAULT_SAMPLE_RATE, format);

    const sai_result_t transmitter_init_status = sai_init(&sai_transmitter, &sai_transmitter_init);
    const sai_result_t receiver_init_status = sai_init(&sai_receiver, &sai_receiver_init);

    /* Continue test only if SAI device has been successfully configured. */
    if (transmitter_init_status == SAI_RESULT_OK && receiver_init_status == SAI_RESULT_OK) {

        const uint32_t data_len = sai_receiver_init.format.data_length;
        const uint32_t data_mask = ((1 << data_len) - 1);

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
                if (sai_transfer(&sai_transmitter, &sample) == true) {
                    write_cnt++;
                }
            }

            if (sai_transfer(&sai_receiver, &sample) == true) {
                receive_buffer[read_cnt] = sample;
                read_cnt++;
            }

            if (read_cnt == BUFFER_SIZE) {
                break;
            }
        }

        sai_free(&sai_transmitter);
        sai_free(&sai_receiver);

        if (compare_buffers(transmit_buffer, receive_buffer, BUFFER_SIZE) == true) {
            return LOOPBACK_TEST_OK;
        } else {
            return LOOPBACK_TEST_FAILURE;
        }
    }

    if (transmitter_init_status == SAI_RESULT_OK) {
        sai_free(&sai_transmitter);
    }

    if (receiver_init_status == SAI_RESULT_OK) {
        sai_free(&sai_receiver);
    }

    return LOOPBACK_TEST_SKIP;
}

/* Test that SAI devices supports required formats. */
void sai_transmission_test()
{

    if (!LOOPBACK_CONNECTION) {
        TEST_IGNORE_MESSAGE("NO LOOPBACK CONNECTION - TEST SKIPPED!");
        return;
    }

    char message[50];

    for (uint32_t f_idx = 0; f_idx < (sizeof(test_sai_formats) / sizeof(sai_format_t)); f_idx++) {
        for (uint32_t sr_idx = 0; sr_idx < (sizeof(test_sample_rates) / sizeof(uint32_t)); sr_idx++) {
            const loopback_test_tatus_t result = sai_loopback_communication_test(&test_sai_formats[f_idx],
                    test_sample_rates[sr_idx]);

            /* First `required_formats_count` formats are obligate, so we expect that
             * communication tests passes. Others formats may be supported, so test can pass or be skipped.
             */
            sprintf(message, "format idx: %lu, sample rate idx: %lu", f_idx, sr_idx);
            if (f_idx < required_formats_count) {
                TEST_ASSERT_EQUAL_MESSAGE(LOOPBACK_TEST_OK, result, message);
            } else {
                TEST_ASSERT_NOT_EQUAL_MESSAGE(LOOPBACK_TEST_FAILURE, result, message);
            }
        }
    }
}

/* Test that sai_init() returns `SAI_RESULT_ALREADY_INITIALIZED` if SAI is to be re-initialised. */
void sai_reinit_test()
{
    sai_t sai_obj = { };
    sai_init_t sai_init_data = { };

    sai_config_create(&sai_init_data, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    /* Change config (format) and try to re-init. */
    sai_config_create(&sai_init_data, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16);

    TEST_ASSERT_EQUAL(SAI_RESULT_ALREADY_INITIALIZED, sai_init(&sai_obj, &sai_init_data));

    sai_free(&sai_obj);
}

/* Test that `sai_init` returns `SAI_RESULT_INVALID_PARAM` if at least one of the given parameters is
 * invalid. */
void sai_init_invalid_param_test()
{
    sai_t sai_obj;
    sai_init_t sai_init_data = { };

    /* Provide NULL pointers. */
    TEST_ASSERT_EQUAL(SAI_RESULT_INVALID_PARAM, sai_init(NULL, &sai_init_data));
    TEST_ASSERT_EQUAL(SAI_RESULT_INVALID_PARAM, sai_init(&sai_obj, NULL));

    /* Create invalid configuration: invalid sample rate. */
    sai_config_create(&sai_init_data, false, 0, &sai_mode_i2s16w32);
    TEST_ASSERT_EQUAL(SAI_RESULT_INVALID_PARAM, sai_init(&sai_obj, &sai_init_data));

    /* Create invalid configuration: invalid format (word length inconsistent with data length). */
    sai_config_create(&sai_init_data, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);
    sai_init_data.format.word_length = 16;
    sai_init_data.format.data_length = 32;
    TEST_ASSERT_EQUAL(SAI_RESULT_INVALID_PARAM, sai_init(&sai_obj, &sai_init_data));
}

/* Test that sai_free() handles invalid (undefined) parameter. */
void sai_free_invalid_param_test()
{
    /* Call free with invalid param and check if no exception is generated. */
    sai_free((sai_t *) NULL);

    TEST_ASSERT_TRUE(true);
}

/* Test that SAI can be successfully re-initialised after it has been freed. */
void sai_reinit_after_free_test()
{
    sai_t sai_obj = { };
    sai_init_t sai_init_data = { };

    sai_config_create(&sai_init_data, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    sai_free(&sai_obj);

    /* Change configuration. */
    sai_config_create(&sai_init_data, true, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    sai_free(&sai_obj);
}

/* Test that sai_transfer() of the SAI receiver returns false if pointer to
 * the `sai_t` object is NULL. */
void sai_transfer_receiver_invalid_param_test()
{
    sai_t sai_obj = { };
    sai_init_t sai_init_data = { };

    sai_config_create(&sai_init_data, true, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    uint32_t sample = 0xABABABAB;

    const bool result = sai_transfer((sai_t *) NULL, &sample);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(0xABABABAB, sample);

    sai_free(&sai_obj);
}

/* Test that `sai_transfer` of the SAI receiver returns false if there's no sample in the FIFO. */
void sai_transfer_receiver_empty_fifo_test()
{
    sai_t sai_obj = { };
    sai_init_t sai_init_data = { };

    sai_config_create(&sai_init_data, true, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    uint32_t sample = 0xABABABAB;

    /* Assume that no sample has been sent. */
    const bool result = sai_transfer(&sai_obj, &sample);

    TEST_ASSERT_FALSE(result);

    sai_free(&sai_obj);
}

/* Test that sai_transfer() of the SAI receiver pops 1 sample from the FIFO,
 * stores it to the address pointed by `psample`(if defined), and returns true. */
static void sai_transfer_receiver_psample_test()
{
    if (!LOOPBACK_CONNECTION) {
        TEST_IGNORE_MESSAGE("NO LOOPBACK CONNECTION - TEST SKIPPED!");
        return;
    }

    sai_t sai_transmitter;
    sai_t sai_receiver;

    sai_init_t sai_transmitter_init = { };
    sai_init_t sai_receiver_init = { };

    sai_config_create(&sai_transmitter_init, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);
    sai_config_create(&sai_receiver_init, true, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    const sai_result_t transmitter_init_status = sai_init(&sai_transmitter, &sai_transmitter_init);
    const sai_result_t receiver_init_status = sai_init(&sai_receiver, &sai_receiver_init);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, transmitter_init_status);
    TEST_ASSERT_EQUAL(SAI_RESULT_OK, receiver_init_status);

    uint32_t write_cnt = 0;
    uint32_t read_cnt = 0;
    uint32_t sample;

    /* Send SAMPLE_1 and SAMPLE_2 alternately. Try to read samples using
     * undefined and defined `psample` parameter.
     */
    while (1) {
        if (write_cnt < BUFFER_SIZE) {
            (write_cnt & 1) ? sample = SAMPLE_1 : sample = SAMPLE_2;
            if (sai_transfer(&sai_transmitter, &sample) == true) {
                write_cnt++;
            }
        }

        if (read_cnt & 1) {
            if (sai_transfer(&sai_receiver, NULL) == true) {
                read_cnt++;
                // SAMPLE_1 has been popped from receive FIFO.
            }
        } else {
            if (sai_transfer(&sai_receiver, &sample) == true) {
                read_cnt++;
                TEST_ASSERT_EQUAL(SAMPLE_2, sample);
            }
        }

        if (read_cnt == BUFFER_SIZE) {
            break;
        }
    }

    sai_free(&sai_transmitter);
    sai_free(&sai_receiver);
}

/* Test that sai_transfer() of the SAI transmitter returns false if pointer to
 * the `sai_t` object is NULL. */
void sai_transfer_transmitter_invalid_param_test()
{
    sai_t sai_obj = { };
    sai_init_t sai_init_data = { };

    sai_config_create(&sai_init_data, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, sai_init(&sai_obj, &sai_init_data));

    uint32_t sample = 0xABABABAB;

    const bool result = sai_transfer((sai_t *) NULL, &sample);

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(0xABABABAB, sample);

    sai_free(&sai_obj);
}

/* Test that sai_transfer() of the SAI transmitter pushes the pointed sample (or 0 if undefined)
 * to the FIFO and returns true. */
static void sai_transfer_transmitter_psample_test()
{
    if (!LOOPBACK_CONNECTION) {
        TEST_IGNORE_MESSAGE("NO LOOPBACK CONNECTION - TEST SKIPPED!");
        return;
    }

    sai_t sai_transmitter;
    sai_t sai_receiver;

    sai_init_t sai_transmitter_init = { };
    sai_init_t sai_receiver_init = { };

    sai_config_create(&sai_transmitter_init, false, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);
    sai_config_create(&sai_receiver_init, true, SAI_DEFAULT_SAMPLE_RATE, &sai_mode_i2s16w32);

    const sai_result_t transmitter_init_status = sai_init(&sai_transmitter, &sai_transmitter_init);
    const sai_result_t receiver_init_status = sai_init(&sai_receiver, &sai_receiver_init);

    TEST_ASSERT_EQUAL(SAI_RESULT_OK, transmitter_init_status);
    TEST_ASSERT_EQUAL(SAI_RESULT_OK, receiver_init_status);

    uint32_t write_cnt = 0;
    uint32_t read_cnt = 0;
    uint32_t sample = 0xFFFFFFFF;

    /* Send SAMPLE_1 and undefined sample alternately, read them back and
     * verify results.
     */
    while (1) {
        if (write_cnt < BUFFER_SIZE) {

            if (write_cnt & 1) {
                if (sai_transfer(&sai_transmitter, NULL) == true) {
                    write_cnt++;
                }
            } else {
                sample = SAMPLE_1;
                if (sai_transfer(&sai_transmitter, &sample) == true) {
                    write_cnt++;
                }
            }
        }

        if (sai_transfer(&sai_receiver, &sample) == true) {
            const uint32_t exp_result = (read_cnt & 1) ? 0 : SAMPLE_1;
            TEST_ASSERT_EQUAL(exp_result, sample);
            read_cnt++;
        }

        if (read_cnt == BUFFER_SIZE) {
            break;
        }
    }

    sai_free(&sai_transmitter);
    sai_free(&sai_receiver);
}

Case cases[] = {
    Case("sai_init() - invalid parameter.", sai_init_invalid_param_test),
    Case("sai_init() - re-init.", sai_reinit_test),
    Case("sai_free() - invalid parameter.", sai_free_invalid_param_test),
    Case("sai_free()/sai_init() - re-init after free.", sai_reinit_after_free_test),
    Case("receiver: sai_transfer() - invalid param", sai_transfer_receiver_invalid_param_test),
    Case("receiver: sai_transfer() - empty FIFO", sai_transfer_receiver_empty_fifo_test),
    Case("receiver: sai_transfer() - psample defined/undefined", sai_transfer_receiver_psample_test),
    Case("transmitter: sai_transfer() - invalid param", sai_transfer_transmitter_invalid_param_test),
    Case("transmitter: sai_transfer() - psample defined/undefined", sai_transfer_transmitter_psample_test),
    Case("SAI transmission test", sai_transmission_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
