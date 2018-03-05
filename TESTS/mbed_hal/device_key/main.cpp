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
#include "device_key_api.h"

#ifndef DEVICE_DEVKEY
#error [NOT_SUPPORTED] DEVICE_KEY needs to be enabled for this test
#endif

using namespace utest::v1;

#define DEVICE_KEY_16BYTE 16
#define DEVICE_KEY_32BYTE 32

/*
 * Test if the platform return a valid device key size
 */
void device_key_get_size_test()
{
    size_t len = device_key_get_size_in_bytes();
    TEST_ASSERT_FALSE_MESSAGE(DEVICE_KEY_16BYTE != len && DEVICE_KEY_32BYTE != len,
                              "Device key length is not 16 or 32 Byte long");
}

/*
 * Test that the platform return error when the buffer is to small
 */
void device_key_get_key_buffer_to_small_test()
{
    uint32_t buffer[(DEVICE_KEY_16BYTE / 2) / sizeof(uint32_t)];
    size_t len = DEVICE_KEY_16BYTE / 2;

    memset(buffer, 0, DEVICE_KEY_16BYTE / 2);
    int status = device_key_get_value(buffer, &len);
    TEST_ASSERT_EQUAL_INT32(DEVICEKEY_HAL_BUFFER_TOO_SMALL, status);
}

/*
 * Test if the platform return a device key buffer with
 * the correct length.
 */
void device_key_get_key_length_test()
{
    size_t expected = device_key_get_size_in_bytes();
    uint32_t buffer[(DEVICE_KEY_32BYTE * 2) / sizeof(uint32_t)];
    size_t len = (DEVICE_KEY_32BYTE * 2);

    memset(buffer, 0, sizeof(buffer));
    int status = device_key_get_value(buffer, &len);
    TEST_ASSERT_EQUAL_INT32(0, status);
    TEST_ASSERT_EQUAL_INT32(expected, len);
}

/*
 * Test if the platform returns the same key consistently.
 */
void device_key_check_consistency_key_test()
{
    uint32_t empty_buffer[DEVICE_KEY_32BYTE / sizeof(uint32_t)];
    uint32_t buffer1[DEVICE_KEY_32BYTE / sizeof(uint32_t)];
    uint32_t buffer2[DEVICE_KEY_32BYTE / sizeof(uint32_t)];
    size_t len1 = DEVICE_KEY_32BYTE;
    size_t len2 = DEVICE_KEY_32BYTE;

    memset(empty_buffer, 0, sizeof(empty_buffer));
    memset(buffer1, 0, sizeof(buffer1));
    int status = device_key_get_value(buffer1, &len1);
    TEST_ASSERT_EQUAL_INT32(0, status);
    bool is_empty = !memcmp(empty_buffer, buffer1, sizeof(buffer1));
    TEST_ASSERT_FALSE(is_empty);

    for (int i = 0; i < 100; i++) {
        memset(buffer2, 0, sizeof(buffer2));
        status = device_key_get_value(buffer2, &len2);
        TEST_ASSERT_EQUAL_INT32(0, status);
        TEST_ASSERT_EQUAL_INT32(len2, len1);
        TEST_ASSERT_EQUAL_INT32_ARRAY(buffer1, buffer2, len2 / sizeof(uint32_t));
        len2 = DEVICE_KEY_32BYTE;
    }
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
        Case("Device Key - get size",                device_key_get_size_test,                greentea_failure_handler),
        Case("Device Key - get key buffer to small", device_key_get_key_buffer_to_small_test, greentea_failure_handler),
        Case("Device Key - get key length",          device_key_get_key_length_test,          greentea_failure_handler),
        Case("Device Key - check consistency key",   device_key_check_consistency_key_test,   greentea_failure_handler)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return Harness::run(specification);
}

