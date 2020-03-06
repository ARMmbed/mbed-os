/*
 * Copyright (c) 2018-2020, ARM Limited, All Rights Reserved
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

#if !USB_DEVICE_TESTS
#error [NOT_SUPPORTED] usb device tests not enabled
#else

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] USB stack and test cases require RTOS to run.
#else

#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "USBTester.h"
#include "USBEndpointTester.h"
#include "usb_phy_api.h"

// TODO
// suspend resume test: implement host side USB suspend/resume
// sync frame test: add test on isochronous endpoint

// Uncomment/remove this when host suspend_resume_test part will be implemented
//#define SUSPEND_RESUME_TEST_SUPPORTED

// If disconnect() + connect() occur too fast the reset event will be dropped.
// At a minimum there should be a 200us delay between disconnect and connect.
// To be on the safe side I would recommend a 1ms delay, so the host controller
// has an entire USB frame to detect the disconnect.
#define MIN_DISCONNECT_TIME_US  1000

#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#error [NOT_SUPPORTED] USB Device not supported for this target
#else


using namespace utest::v1;

// Print callback counters for endpoint tests
#define EP_DBG 0

static USBPhy *get_phy()
{
    return get_usb_phy();
}


/** Control basic tests

    Test device configuration/deconfiguration
    Given an initialized USB (HOST <---> DUT connection established)
    When device configuration is checked just after initialization
    Then get_configuration returns 1 (default configuration is set)
    When device is deconfigured
    Then get_configuration returns 0 (no configuration is set)
    When each from supported configurations is set
    Then the configuration is set correctly

    Test device interface setting
    Given an initialized USB (HOST <---> DUT connection established)
    When each altsetting from every supported configuration is set
    Then the interface altsetting is set correctly

    Test device/interface/endpoint status
    Given an initialized USB (HOST <---> DUT connection established)
    When device status is checked
    Then status is within allowed values (see status bits description below)
    When control endpoint status is checked
    Then control endpoint status is 0
    When status of each interface from every supported configuration is checked
    Then interface status is 0
    When status of each endpoint in every allowed device interface/configuration combination is checked
    Then endpoint status is 0 (not halted)

    Test set/clear feature on device/interface/endpoint
    Given an initialized USB (HOST <---> DUT connection established)
    When for each endpoint in every allowed interface/configuration combination the feature is set and then cleared
    Then selected feature is set/cleared accordingly

    Test device/configuration/interface/endpoint descriptors
    Given an initialized USB (HOST <---> DUT connection established)
    When device descriptor is read
    Then the descriptor content is valid
    When configuration descriptor is read
    Then the descriptor content is valid
    When interface descriptor is read
    Then the error is thrown since it is not directly accessible
    When endpoint descriptor is read
    Then the error is thrown since it is not directly accessible

    Test descriptor setting
    Given an initialized USB (HOST <---> DUT connection established)
    When device descriptor is to be set
    Then error is thrown since descriptor setting command is not supported by Mbed
*/
void control_basic_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};
    char str[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        sprintf(str, "%s %d %d", serial.get_serial_desc_string(), vendor_id, product_id);
        greentea_send_kv("control_basic_test", str);
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


/** Test control endpoint stall on invalid request

    Given an initialized USB (HOST <---> DUT connection established)
    When unsupported request to control endpoint is to be sent
    Then the endpoint is stalled and error is thrown

*/
void control_stall_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        greentea_send_kv("control_stall_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


/** Test various data sizes in control transfer

    Given an initialized USB (HOST <---> DUT connection established)
    When control data in each tested size is sent
    Then read data should match sent data

*/
void control_sizes_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        greentea_send_kv("control_sizes_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


/** Test various patterns of control transfers

    Given an initialized USB (HOST <---> DUT connection established)
    When stress control transfer with a data in stage is performed
    Then transfer ends with success
    When stress control transfer with a data out stage followed by a control transfer with a data in stage is performed
    Then transfer ends with success
    When stress control transfer with a data out stage is performed
    Then transfer ends with success

*/
void control_stress_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        greentea_send_kv("control_stress_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test data correctness for every OUT/IN endpoint pair
 *
 * Given a USB device with multiple OUT/IN endpoint pairs
 * When the host sends random payloads up to wMaxPacketSize in size
 *     to an OUT endpoint of the device,
 *     and then the device sends data back to host using an IN endpoint
 * Then data sent and received by host is equal for every endpoint pair
 */
void ep_test_data_correctness()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, false);
        greentea_send_kv("ep_test_data_correctness", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test endpoint halt for every OUT/IN endpoint pair
 *
 * Given a USB device with multiple OUT/IN endpoint pairs
 * When the host issues an endpoint halt control request at a random point
 *     of OUT or IN transfer
 * Then the endpoint is stalled and all further transfers fail
 */
void ep_test_halt()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, false);
        greentea_send_kv("ep_test_halt", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test simultaneous data transfers for multiple OUT/IN endpoint pairs
 *
 * Given a USB device with multiple OUT/IN endpoint pairs
 * When multiple OUT and IN endpoints are used to transfer random data in parallel
 * Then all transfers succeed
 *     and for every endpoint pair, data received by host equals data sent by host
 */
void ep_test_parallel_transfers()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, false);
        greentea_send_kv("ep_test_parallel_transfers", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test simultaneous data transfers in parallel with control transfers
 *
 * Given a USB device with multiple OUT/IN endpoint pairs
 * When multiple OUT and IN endpoints are used to transfer random data
 *     and control requests are processed in parallel
 * Then all transfers succeed
 *     and for every endpoint pair, data received by host equals data sent by host
 */
void ep_test_parallel_transfers_ctrl()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, false);
        greentea_send_kv("ep_test_parallel_transfers_ctrl", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test aborting data transfer for every OUT/IN endpoint pair
 *
 * Given a USB device with multiple OUT/IN endpoint pairs
 * When a device aborts an in progress data transfer
 * Then no more data is transmitted
 *     and endpoint buffer is correctly released on the device end
 */
void ep_test_abort()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, true);
        greentea_send_kv("ep_test_abort", serial.get_serial_desc_string());
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test data toggle reset for bulk OUT/IN endpoint pairs
 *
 * Given a USB device
 * When an interface is set
 * Then the data toggle bits for all endpoints are reset to DATA0
 * When clear feature is called for an endpoint that *IS NOT* stalled
 * Then the data toggle is reset to DATA0 for that endpoint
 * When clear halt is called for an endpoint that *IS* stalled
 * Then the data toggle is reset to DATA0 for that endpoint
 */
void ep_test_data_toggle()
{
    uint16_t vendor_id = 0x0d28;
    // Use a product ID different than that used in other tests,
    // to help Windows hosts use the correct configuration descriptor.
    uint16_t product_id = 0x0206;
    uint16_t product_release = 0x0001;
    char _key[11] = { };
    char _value[128] = { };

    {
        USBEndpointTester serial(get_phy(), vendor_id, product_id, product_release, false);
        greentea_send_kv("ep_test_data_toggle", serial.get_serial_desc_string());
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
#if EP_DBG
        ThisThread::sleep_for(100);
        printf("cnt_cb_set_conf = %lu\r\n", serial.get_cnt_cb_set_conf());
        printf("cnt_cb_set_intf = %lu\r\n", serial.get_cnt_cb_set_intf());
        printf("cnt_cb_bulk_out = %lu\r\n", serial.get_cnt_cb_bulk_out());
        printf("cnt_cb_bulk_in  = %lu\r\n", serial.get_cnt_cb_bulk_in());
        printf("cnt_cb_int_out  = %lu\r\n", serial.get_cnt_cb_int_out());
        printf("cnt_cb_int_in   = %lu\r\n", serial.get_cnt_cb_int_in());
        printf("cnt_cb_iso_out  = %lu\r\n", serial.get_cnt_cb_iso_out());
        printf("cnt_cb_iso_in   = %lu\r\n", serial.get_cnt_cb_iso_in());
#endif
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}

/** Test USB implementation against repeated reset

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is reset repeatedly
    Then the USB is operational with no errors

*/
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

        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        serial.clear_reset_count();
        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        while (serial.get_reset_count() == 0);
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while (!serial.configured());

        serial.clear_reset_count();
        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        while (serial.get_reset_count() == 0);
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while (!serial.configured());

        serial.clear_reset_count();
        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        while (serial.get_reset_count() == 0);
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        while (!serial.configured());

        greentea_send_kv("device_reset_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


/** Test USB implementation against repeated reconnection

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is disconnected and then connected repeatedly
    Then the USB is operational with no errors

*/
void device_soft_reconnection_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};
    const uint32_t reconnect_try_count = 3;

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);

        greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);

        for (int i = 0; i < reconnect_try_count; i++) {
            serial.disconnect();
            wait_us(MIN_DISCONNECT_TIME_US);
            serial.connect();
            greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
            // Wait for host before terminating
            greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
            TEST_ASSERT_EQUAL_STRING("pass", _key);
        }

        serial.disconnect();
        wait_us(MIN_DISCONNECT_TIME_US);
        serial.connect();
        serial.disconnect();
        wait_us(MIN_DISCONNECT_TIME_US);
        serial.connect();
        serial.disconnect();
        wait_us(MIN_DISCONNECT_TIME_US);
        serial.connect();
        greentea_send_kv("device_soft_reconnection_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }
}


#if SUSPEND_RESUME_TEST_SUPPORTED
/** Test USB implementation against repeated suspend and resume

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is suspended and then resumed repeatedly
    Then the USB is operational with no errors

*/
void device_suspend_resume_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        greentea_send_kv("device_suspend_resume_test", serial.get_serial_desc_string());
        printf("[1] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
        serial.clear_suspend_count();
        serial.clear_resume_count();
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        printf("[2] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
        TEST_ASSERT_EQUAL_STRING("pass", _key);
        ThisThread::sleep_for(5000);
        printf("[3] suspend_count: %d  resume_count: %d\n", serial.get_suspend_count(), serial.get_resume_count());
    }
}
#endif


/** Test USB implementation against repeated initialization and deinitialization

    Given an initialized USB (HOST <---> DUT connection established)
    When USB device is deinitialized and then initialized repeatedly
    Then the USB is operational with no errors

*/
void repeated_construction_destruction_test()
{
    uint16_t vendor_id = 0x0d28;
    uint16_t product_id = 0x0205;
    uint16_t product_release = 0x0001;
    char _key[11] = {};
    char _value[128] = {};

    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
    }

    wait_us(MIN_DISCONNECT_TIME_US);
    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
    }

    wait_us(MIN_DISCONNECT_TIME_US);
    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
    }

    wait_us(MIN_DISCONNECT_TIME_US);
    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
        greentea_send_kv("repeated_construction_destruction_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }

    wait_us(MIN_DISCONNECT_TIME_US);
    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
        greentea_send_kv("repeated_construction_destruction_test", serial.get_serial_desc_string());
        // Wait for host before terminating
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING("pass", _key);
    }

    wait_us(MIN_DISCONNECT_TIME_US);
    {
        USBTester serial(get_phy(), vendor_id, product_id, product_release);
        TEST_ASSERT_EQUAL(true, serial.configured());
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
#if SUSPEND_RESUME_TEST_SUPPORTED
    Case("usb device suspend/resume test", device_suspend_resume_test),
#endif
    Case("usb repeated construction destruction test", repeated_construction_destruction_test),
    Case("endpoint test data correctness", ep_test_data_correctness),
    Case("endpoint test halt", ep_test_halt),
    Case("endpoint test parallel transfers", ep_test_parallel_transfers),
    Case("endpoint test parallel transfers ctrl", ep_test_parallel_transfers_ctrl),
    Case("endpoint test abort", ep_test_abort),
    Case("endpoint test data toggle reset", ep_test_data_toggle)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(180, "pyusb_basic");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#endif // !defined(MBED_CONF_RTOS_PRESENT)
#endif // !defined(USB_DEVICE_TESTS)
