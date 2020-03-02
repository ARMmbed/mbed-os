
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

using namespace utest::v1;

void test_supported_polynomials()
{
    const char test[] = "123456789";
    uint32_t crc;

    {
        MbedCRC<POLY_7BIT_SD, 7> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x75, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0x7F, 0, false, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x50, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0x2B, 0, false, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x3A, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0, 0x7F, false, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x0A, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0, 0x2B, false, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x5E, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0, 0, true, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x52, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0, 0, false, true);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x57, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0x2B, 0, true, false);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x1D, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0x2B, 0, false, true);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x2E, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0x2B, 0, true, true);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x5C, crc);
    }
    {
        MbedCRC<POLY_7BIT_SD, 7> ct(0, 0x2B, false, true);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x7C, crc);
    }
    {
        MbedCRC<POLY_8BIT_CCITT, 8> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xF4, crc);
    }
    {
        MbedCRC<POLY_16BIT_CCITT, 16> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0x29B1, crc);
    }
    {
        MbedCRC<POLY_16BIT_IBM, 16> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xBB3D, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
}

void test_partial_crc()
{
    char  test[] = "123456789";
    uint32_t crc;
    {
        MbedCRC<POLY_16BIT_CCITT, 16> ct;
        TEST_ASSERT_EQUAL(0, ct.compute_partial_start(&crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(test, 4, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(&test[4], 5, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));

        TEST_ASSERT_EQUAL(0x29B1, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32> ct;
        TEST_ASSERT_EQUAL(0, ct.compute_partial_start(&crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(test, 8, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(&test[8], 1, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));

        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32> ct(0xFFFFFFFF, 0, true, false);
        TEST_ASSERT_EQUAL(0, ct.compute_partial_start(&crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(test, 8, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32> ct(crc, 0xFFFFFFFF, true, true);
        TEST_ASSERT_EQUAL(0, ct.compute_partial_start(&crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial(&test[8], 1, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));

        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
}

void test_mode_limit()
{
    const char test[] = "123456789";
    uint32_t crc;

    {
        MbedCRC<POLY_32BIT_ANSI, 32, CrcMode::BITWISE> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32, CrcMode::TABLE> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32, CrcMode::HARDWARE> ct;
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xCBF43926, crc);
    }
}

void test_sd_crc()
{
    MbedCRC<POLY_7BIT_SD, 7> crc7;
    uint32_t crc;
    char test[512];

    test[0] = 0x40;
    test[1] = 0x00;
    test[2] = 0x00;
    test[3] = 0x00;
    test[4] = 0x00;
    TEST_ASSERT_EQUAL(0, crc7.compute(test, 5, &crc));
    crc = (crc << 1) | 0x1;
    TEST_ASSERT_EQUAL(0x95, crc);

    test[0] = 0x48;
    test[1] = 0x00;
    test[2] = 0x00;
    test[3] = 0x01;
    test[4] = 0xAA;
    TEST_ASSERT_EQUAL(0, crc7.compute(test, 5, &crc));
    crc = (crc << 1) | 0x1;
    TEST_ASSERT_EQUAL(0x87, crc);

    test[0] = 0x51;
    test[1] = 0x00;
    test[2] = 0x00;
    test[3] = 0x00;
    test[4] = 0x00;
    TEST_ASSERT_EQUAL(0, crc7.compute(test, 5, &crc));
    crc = (crc << 1) | 0x1;
    TEST_ASSERT_EQUAL(0x55, crc);

    MbedCRC<POLY_16BIT_CCITT, 16> crc16(0, 0, false, false);
    memset(test, 0xFF, 512);
    TEST_ASSERT_EQUAL(0, crc16.compute(test, 512, &crc));
    TEST_ASSERT_EQUAL(0x7FA1, crc);
}

void test_any_polynomial()
{
    char  test[] = "123456789";
    uint32_t crc;
    {
        MbedCRC<0x3D65, 16> ct(0x0, 0xFFFF, 0, 0);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xC2B7, crc);
    }
    {
        MbedCRC<0x1EDC6F41, 32> ct(0xFFFFFFFF, 0xFFFFFFFF, 1, 1);
        TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
        TEST_ASSERT_EQUAL(0xE3069283, crc);
    }
}

void test_thread(void)
{
    char  test[] = "123456789";
    uint32_t crc;
    MbedCRC<POLY_32BIT_ANSI, 32> ct;
    TEST_ASSERT_EQUAL(0, ct.compute(test, strlen(test), &crc));
    TEST_ASSERT_EQUAL(0xCBF43926, crc);
}

#if defined(MBED_CONF_RTOS_PRESENT)
void test_thread_safety()
{
    char  test[] = "123456789";
    uint32_t crc;

    MbedCRC<POLY_16BIT_IBM, 16> ct;

    TEST_ASSERT_EQUAL(0, ct.compute_partial_start(&crc));
    TEST_ASSERT_EQUAL(0, ct.compute_partial(test, 4, &crc));

    Thread t1(osPriorityNormal1, 380);
    t1.start(callback(test_thread));
    TEST_ASSERT_EQUAL(0, ct.compute_partial(&test[4], 5, &crc));
    TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));
    TEST_ASSERT_EQUAL(0xBB3D, crc);

    // Wait for the thread to finish
    t1.join();
}
#endif

Case cases[] = {
    Case("Test supported polynomials", test_supported_polynomials),
    Case("Test partial CRC", test_partial_crc),
    Case("Test mode-limited CRC", test_mode_limit),
    Case("Test SD CRC polynomials", test_sd_crc),
#if defined(MBED_CONF_RTOS_PRESENT)
    Case("Test thread safety", test_thread_safety),
#endif
    Case("Test not supported polynomials", test_any_polynomial)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
