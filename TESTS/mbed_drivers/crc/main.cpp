
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

using namespace utest::v1;

void test_supported_polynomials()
{
    char  test[] = "123456789";
    uint32_t crc;

    {
        MbedCRC<POLY_7BIT_SD, 7> ct;
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0xEA, crc);
    }
    {
        MbedCRC<POLY_8BIT_CCITT, 8> ct;
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0xF4, crc);
    }
    {
        MbedCRC<POLY_16BIT_CCITT, 16> ct;
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0x29B1, crc);
    }
    {
        MbedCRC<POLY_16BIT_IBM, 16> ct;
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0xBB3D, crc);
    }
    {
        MbedCRC<POLY_32BIT_ANSI, 32> ct;
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
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
        TEST_ASSERT_EQUAL(0, ct.compute_partial((void *)&test, 4, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial((void *)&test[4], 5, &crc));
        TEST_ASSERT_EQUAL(0, ct.compute_partial_stop(&crc));

        TEST_ASSERT_EQUAL(0x29B1, crc);
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
    TEST_ASSERT_EQUAL(0, crc7.compute((void *)test, 5, &crc));
    crc = (crc | 0x1 ) & 0xFF;
    TEST_ASSERT_EQUAL(0x95, crc);

    test[0] = 0x48;
    test[1] = 0x00;
    test[2] = 0x00;
    test[3] = 0x01;
    test[4] = 0xAA;
    TEST_ASSERT_EQUAL(0, crc7.compute((void *)test, 5, &crc));
    crc = (crc | 0x1 ) & 0xFF;
    TEST_ASSERT_EQUAL(0x87, crc);

    test[0] = 0x51;
    test[1] = 0x00;
    test[2] = 0x00;
    test[3] = 0x00;
    test[4] = 0x00;
    TEST_ASSERT_EQUAL(0, crc7.compute((void *)test, 5, &crc));
    crc = (crc | 0x1 ) & 0xFF;
    TEST_ASSERT_EQUAL(0x55, crc);

    MbedCRC<POLY_16BIT_CCITT, 16> crc16(0, 0, false, false);
    memset(test, 0xFF, 512);
    TEST_ASSERT_EQUAL(0, crc16.compute((void *)test, 512, &crc));
    TEST_ASSERT_EQUAL(0x7FA1, crc);
}

void test_any_polynomial()
{
    char  test[] = "123456789";
    uint32_t crc;
    {
        MbedCRC<0x3D65, 16> ct(0x0, 0xFFFF, 0, 0);
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0xC2B7, crc);
    }
    {
        MbedCRC<0x1EDC6F41, 32> ct(0xFFFFFFFF, 0xFFFFFFFF, 1, 1);
        TEST_ASSERT_EQUAL(0, ct.compute((void *)test, strlen((const char*)test), &crc));
        TEST_ASSERT_EQUAL(0xE3069283, crc);
    }
}

Case cases[] = {
    Case("Test supported polynomials", test_supported_polynomials),
    Case("Test partial CRC", test_partial_crc),
    Case("Test SD CRC polynomials", test_sd_crc),
    Case("Test not supported polynomials", test_any_polynomial)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(15, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
