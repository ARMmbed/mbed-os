/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#include "crc_api.h"

#if !DEVICE_CRC
#error [NOT_SUPPORTED] CRC not supported for this target
#else

using namespace utest::v1;

#define POLY_8BIT_MAXIM 0x31

#define UNSUPPORTED (-1)
#define POL_CNT (2)

const uint8_t input_data[] = "123456789";

typedef struct {
    const crc_mbed_config config_data;
    uint32_t expected_result;

} TEST_CASE;

/* We will allocate test case array on stack since memory limits on some boards
 * like NUCLEO_F070RB. */
static TEST_CASE *test_cases;
static uint32_t test_cases_size;

/* Test that hal_crc_is_supported() function returns true if given polynomial
 * is supported, false otherwise (at least one polynomial from the predefined list must be supported).  */
void crc_is_supported_test()
{
    /* Check if at least one crc polynomial/config is supported. */
    uint32_t num_of_supported_polynomials = 0;

    for (unsigned int i = 0; i < (test_cases_size / sizeof(TEST_CASE)); i++) {
        if (HAL_CRC_IS_SUPPORTED(test_cases[i].config_data.polynomial, test_cases[i].config_data.width)) {

            num_of_supported_polynomials++;
        }
    }

    TEST_ASSERT(num_of_supported_polynomials > 0);
}

/* Test that CRC module can be successfully configured, fed with data and the result can
 * be successfully obtained. */
void crc_calc_single_test()
{
    for (unsigned int i = 0; i < (test_cases_size / sizeof(TEST_CASE)); i++) {
        if (HAL_CRC_IS_SUPPORTED(test_cases[i].config_data.polynomial, test_cases[i].config_data.width)) {

            hal_crc_compute_partial_start(&test_cases[i].config_data);
            hal_crc_compute_partial((uint8_t *) input_data, strlen((const char *) input_data));
            const uint32_t crc = hal_crc_get_result();

            TEST_ASSERT_EQUAL(test_cases[i].expected_result, crc);
        }
    }
}

/* Test that hal_crc_compute_partial() function can be call multiple times in
 * succession in order to provide additional data to CRC module. */
void crc_calc_multi_test()
{
    for (unsigned int i = 0; i < (test_cases_size / sizeof(TEST_CASE)); i++) {
        if (HAL_CRC_IS_SUPPORTED(test_cases[i].config_data.polynomial, test_cases[i].config_data.width)) {

            const uint32_t first_part_bytes = 3;
            const uint32_t second_part_bytes = 1;
            const uint32_t third_part_bytes = strlen((const char *) input_data) - first_part_bytes
                                              - second_part_bytes;

            hal_crc_compute_partial_start(&test_cases[i].config_data);
            hal_crc_compute_partial((uint8_t *) input_data, first_part_bytes);
            hal_crc_compute_partial((uint8_t *)(input_data + first_part_bytes), second_part_bytes);
            hal_crc_compute_partial((uint8_t *)(input_data + first_part_bytes + second_part_bytes),
                                    third_part_bytes);
            const uint32_t crc = hal_crc_get_result();

            TEST_ASSERT_EQUAL(test_cases[i].expected_result, crc);
        }
    }
}

/* Test that calling hal_crc_compute_partial_start() without finalising the
 * CRC calculation overrides the current configuration. */
void crc_reconfigure_test()
{
    int pol_idx[POL_CNT] =
    { UNSUPPORTED, UNSUPPORTED };
    int pol_cnt = 0;
    const uint8_t dummy_input_data[] = "abcdefghijklmnopqrstuvwxyz";

    /* At least one configuration must be supported. If two are supported, then
     * re-initialize CRC module using different config. */
    for (unsigned int i = 0; i < (test_cases_size / sizeof(TEST_CASE)); i++) {

        /* Find two supported polynomials if possible. */
        if (HAL_CRC_IS_SUPPORTED(test_cases[i].config_data.polynomial, test_cases[i].config_data.width)) {
            if (pol_cnt == 0) {
                pol_idx[pol_cnt] = i;
                pol_cnt++;
            } else if (test_cases[pol_idx[0]].config_data.polynomial != test_cases[i].config_data.polynomial) {
                pol_idx[pol_cnt] = i;
                pol_cnt++;
            }

            if (pol_cnt == POL_CNT) {
                break;
            }
        }
    }

    pol_cnt = 0;

    /* Init CRC module and provide some data, but do not read the result. */
    hal_crc_compute_partial_start(&test_cases[pol_idx[pol_cnt]].config_data);
    hal_crc_compute_partial((uint8_t *) dummy_input_data, strlen((const char *) dummy_input_data));

    /* Change index only if more than one supported polynomial has been found. */
    if (pol_idx[POL_CNT - 1] != UNSUPPORTED) {
        pol_cnt++;
    }

    /* Now re-init CRC module and provide new data and check the result. */
    hal_crc_compute_partial_start(&test_cases[pol_idx[pol_cnt]].config_data);
    hal_crc_compute_partial((uint8_t *) input_data, strlen((const char *) input_data));
    const uint32_t crc = hal_crc_get_result();

    TEST_ASSERT_EQUAL(test_cases[pol_idx[pol_cnt]].expected_result, crc);
}

/* Test that hal_crc_compute_partial() does nothing if pointer to buffer is undefined or
 * data length is equal to 0. */
void crc_compute_partial_invalid_param_test()
{
    uint32_t crc = 0;

    /* At least one polynomial must be supported. */
    for (unsigned int i = 0; i < (test_cases_size / sizeof(TEST_CASE)); i++) {
        if (HAL_CRC_IS_SUPPORTED(test_cases[i].config_data.polynomial, test_cases[i].config_data.width)) {

            hal_crc_compute_partial_start(&test_cases[i].config_data);

            /* Call hal_crc_compute_partial() with invalid parameters. */
            hal_crc_compute_partial((uint8_t *) NULL, strlen((const char *) input_data));
            hal_crc_compute_partial((uint8_t *) input_data, 0);

            /* Now use valid parameters. */
            hal_crc_compute_partial((uint8_t *) input_data,
                                    strlen((const char *) input_data));

            crc = hal_crc_get_result();

            TEST_ASSERT_EQUAL(test_cases[i].expected_result, crc);

            break;
        }
    }
}

Case cases[] = {
    Case("test: supported polynomials.", crc_is_supported_test),
    Case("test: CRC calculation - single input.", crc_calc_single_test),
    Case("test: CRC calculation - multi input.", crc_calc_multi_test),
    Case("test: re-configure without getting the result.", crc_reconfigure_test),
    Case("test: hal_crc_compute_partial() - invalid parameters.", crc_compute_partial_invalid_param_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(30, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    // *INDENT-OFF*
    TEST_CASE local_test_cases[] = {
        /* Predefined polynomials. */
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x00000000, false, false}, 0x75       },
        { {POLY_7BIT_SD     ,  7, 0x0000007F, 0x00000000, false, false}, 0x50       },
        { {POLY_7BIT_SD     ,  7, 0x0000002B, 0x00000000, false, false}, 0x3A       },
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x0000007F, false, false}, 0x0A       },
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x0000002B, false, false}, 0x5E       },
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x00000000, true , false}, 0x52       },
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x00000000, false, true }, 0x57       },
        { {POLY_7BIT_SD     ,  7, 0x0000002B, 0x00000000, true , false}, 0x1D       },
        { {POLY_7BIT_SD     ,  7, 0x0000002B, 0x00000000, false, true }, 0x2E       },
        { {POLY_7BIT_SD     ,  7, 0x0000002B, 0x00000000, true , true }, 0x5C       },
        { {POLY_7BIT_SD     ,  7, 0x00000000, 0x0000002B, false, true }, 0x7C       },

        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x00000000, false, false}, 0xF4       },
        { {POLY_8BIT_CCITT  ,  8, 0x000000FF, 0x00000000, false, false}, 0xFB       },
        { {POLY_8BIT_CCITT  ,  8, 0x000000AB, 0x00000000, false, false}, 0x87       },
        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x000000FF, false, false}, 0x0B       },
        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x000000AB, false, false}, 0x5F       },
        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x00000000, true , false}, 0x04       },
        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x00000000, false, true }, 0x2F       },
        { {POLY_8BIT_CCITT  ,  8, 0x000000AB, 0x00000000, true,  false}, 0x77       },
        { {POLY_8BIT_CCITT  ,  8, 0x000000AB, 0x00000000, false, true }, 0xE1       },
        { {POLY_8BIT_CCITT  ,  8, 0x000000AB, 0x00000000, true,  true }, 0xEE       },
        { {POLY_8BIT_CCITT  ,  8, 0x00000000, 0x000000AB, false, true }, 0x84       },

        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x00000000, false, false}, 0x31C3     },
        { {POLY_16BIT_CCITT , 16, 0x0000FFFF, 0x00000000, false, false}, 0x29B1     },
        { {POLY_16BIT_CCITT , 16, 0x0000ABAB, 0x00000000, false, false}, 0x7D70     },
        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x0000FFFF, false, false}, 0xCE3C     },
        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x0000ABAB, false, false}, 0x9A68     },
        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x00000000, true , false}, 0x9184     },
        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x00000000, false, true }, 0xC38C     },
        { {POLY_16BIT_CCITT , 16, 0x0000ABAB, 0x00000000, true,  false}, 0xDD37     },
        { {POLY_16BIT_CCITT , 16, 0x0000ABAB, 0x00000000, false, true }, 0x0EBE     },
        { {POLY_16BIT_CCITT , 16, 0x0000ABAB, 0x00000000, true,  true }, 0xECBB     },
        { {POLY_16BIT_CCITT , 16, 0x00000000, 0x0000ABAB, false, true }, 0x6827     },

        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x00000000, false, false}, 0xFEE8     },
        { {POLY_16BIT_IBM   , 16, 0x0000FFFF, 0x00000000, false, false}, 0xAEE7     },
        { {POLY_16BIT_IBM   , 16, 0x0000ABAB, 0x00000000, false, false}, 0x0887     },
        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x0000FFFF, false, false}, 0x0117     },
        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x0000ABAB, false, false}, 0x5543     },
        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x00000000, true , false}, 0xBCDD     },
        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x00000000, false, true }, 0x177F     },
        { {POLY_16BIT_IBM   , 16, 0x0000ABAB, 0x00000000, true,  false}, 0x4AB2     },
        { {POLY_16BIT_IBM   , 16, 0x0000ABAB, 0x00000000, false, true }, 0xE110     },
        { {POLY_16BIT_IBM   , 16, 0x0000ABAB, 0x00000000, true,  true }, 0x4D52     },
        { {POLY_16BIT_IBM   , 16, 0x00000000, 0x0000ABAB, false, true }, 0xBCD4     },

        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0x00000000, false, false}, 0x89A1897F },
        { {POLY_32BIT_ANSI  , 32, 0xFFFFFFFF, 0x00000000, false, false}, 0x0376E6E7 },
        { {POLY_32BIT_ANSI  , 32, 0xABABABAB, 0x00000000, false, false}, 0x871A2FAA },
        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0xFFFFFFFF, false, false}, 0x765E7680 },
        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0xABABABAB, false, false}, 0x220A22D4 },
        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0x00000000, true , false}, 0x11B4BFB4 },
        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0x00000000, false, true }, 0xFE918591 },
        { {POLY_32BIT_ANSI  , 32, 0xABABABAB, 0x00000000, true,  false}, 0x1F0F1961 },
        { {POLY_32BIT_ANSI  , 32, 0xABABABAB, 0x00000000, false, true }, 0x55F458E1 },
        { {POLY_32BIT_ANSI  , 32, 0xABABABAB, 0x00000000, true,  true }, 0x8698F0F8 },
        { {POLY_32BIT_ANSI  , 32, 0x00000000, 0xABABABAB, false, true }, 0x553A2E3A },

        /* Not-predefined polynomials. */
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x00000000, false, false}, 0xA2       },
        { {POLY_8BIT_MAXIM  ,  8, 0x000000FF, 0x00000000, false, false}, 0xF7       },
        { {POLY_8BIT_MAXIM  ,  8, 0x000000AB, 0x00000000, false, false}, 0x71       },
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x000000FF, false, false}, 0x5D       },
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x000000AB, false, false}, 0x09       },
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x00000000, true , false}, 0x85       },
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x00000000, false, true }, 0x45       },
        { {POLY_8BIT_MAXIM  ,  8, 0x000000AB, 0x00000000, true,  false}, 0x56       },
        { {POLY_8BIT_MAXIM  ,  8, 0x000000AB, 0x00000000, false, true }, 0x8E       },
        { {POLY_8BIT_MAXIM  ,  8, 0x000000AB, 0x00000000, true,  true }, 0x6A       },
        { {POLY_8BIT_MAXIM  ,  8, 0x00000000, 0x000000AB, false, true }, 0xEE       },
    };
    // *INDENT-ON*

    test_cases = local_test_cases;
    test_cases_size = sizeof(local_test_cases);

    Harness::run(specification);
}

#endif // !DEVICE_CRC
