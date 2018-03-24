/*
 * Copyright (c) 2018-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "USBTester.h"

#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#error [NOT_SUPPORTED] USB Device not supported for this target
#endif

using namespace utest::v1;

void control_basic_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};
    char str[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        sprintf (str, "%s %d %d", serial.get_serial_desc_string(), vendor_id, product_id);
        greentea_send_kv("control_basic_test", str);
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

void control_stall_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        greentea_send_kv("control_stall_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

void control_sizes_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        greentea_send_kv("control_sizes_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

void control_stress_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        greentea_send_kv("control_stress_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

void device_reset_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    greentea_send_kv("reset_support", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    if (strcmp(_value, "false") != 0) {

        USBTester serial(vendor_id, product_id, product_release, true);

        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        serial.clear_reset_count();
        // Wait for host before terminating
        while(serial.get_reset_count() == 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while(!serial.configured());

        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        serial.clear_reset_count();
        // Wait for host before terminating
        while(serial.get_reset_count() == 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while(!serial.configured());

        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        serial.clear_reset_count();
        // Wait for host before terminating
        while(serial.get_reset_count() == 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while(!serial.configured());

        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


void device_soft_reconnection_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};
    const uint32_t reconnect_try_count = 3;

    {
        USBTester serial(vendor_id, product_id, product_release, true);

        greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        for(int i = 0; i < reconnect_try_count; i++) {
            serial.disconnect();
            // If disconnect() + connect() occur too fast the reset event will be dropped.
            // At a minimum there should be a 200us delay between disconnect and connect.
            // To be on the safe side I would recommend a 1ms delay, so the host controller
            // has an entire USB frame to detect the disconnect.
            wait_ms(1);
            serial.connect();
            greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
            // Wait for host before terminating
            greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
            TEST_ASSERT_EQUAL_STRING("pass", _key);
        }

        serial.disconnect();
        wait_ms(1);
        serial.connect();
        serial.disconnect();
        wait_ms(1);
        serial.connect();
        serial.disconnect();
        wait_ms(1);
        serial.connect();
        greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

void device_suspend_resume_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        greentea_send_kv("device_suspend_resume_test", serial.get_serial_desc_string());
        printf("[1] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
        serial.clear_suspend_count();
        serial.clear_resume_count();
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        printf("[2] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
        TEST_ASSERT_EQUAL_STRING("pass", _key);
        wait_ms(5000);
        printf("[3] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
    }
}

void repeated_construction_destruction_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
    }

    wait_ms(1);
    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
    }

    wait_ms(1);
    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
    }

    wait_ms(1);
    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
        greentea_send_kv("repeated_construction_destruction_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }

    wait_ms(1);
    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
        greentea_send_kv("repeated_construction_destruction_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }

    wait_ms(1);
    {
        USBTester serial(vendor_id, product_id, product_release, true);
        TEST_ASSERT_EQUAL(true, serial.configured());
        wait_ms(1);
        greentea_send_kv("repeated_construction_destruction_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

Case cases[] = {
    Case("usb control basic test", control_basic_test),
    Case("usb control stall test", control_stall_test),
    Case("usb control sizes test", control_sizes_test),
    Case("usb control stress test", control_stress_test),
    Case("usb device reset test", device_reset_test),
    Case("usb soft reconnection test", device_soft_reconnection_test),
    Case("usb device suspend/resume test", device_suspend_resume_test),
    Case("usb repeated construction destruction test", repeated_construction_destruction_test)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "pyusb_basic");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
