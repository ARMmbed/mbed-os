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

#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#error [NOT_SUPPORTED] USB Device not supported for this target
#else

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"
#include <stdlib.h>
#include "usb_phy_api.h"
#include "USBHID.h"
#include "USBMouse.h"
#include "USBKeyboard.h"
#include "hal/us_ticker_api.h"

// Reuse the VID & PID from basic USB test.
#define USB_HID_VID 0x0d28
#define USB_HID_PID_GENERIC 0x0206
#define USB_HID_PID_KEYBOARD 0x0206
#define USB_HID_PID_MOUSE 0x0206
#define USB_HID_PID_GENERIC2 0x0007

#define MSG_VALUE_LEN 24
#define MSG_KEY_LEN 24
#define MSG_KEY_DEVICE_READY "dev_ready"
#define MSG_KEY_HOST_READY "host_ready"
#define MSG_KEY_SERIAL_NUMBER "usb_dev_sn"
#define MSG_KEY_TEST_GET_DESCRIPTOR_HID "test_get_desc_hid"
#define MSG_KEY_TEST_GET_DESCRIPTOR_CFG "test_get_desc_cfg"
#define MSG_KEY_TEST_REQUESTS "test_requests"
#define MSG_KEY_TEST_RAW_IO "test_raw_io"

#define MSG_KEY_TEST_CASE_FAILED "fail"
#define MSG_KEY_TEST_CASE_PASSED "pass"
#define MSG_VALUE_DUMMY "0"
#define MSG_VALUE_NOT_SUPPORTED "not_supported"

#define RAW_IO_REPS 16

#define USB_DEV_SN_LEN (32) // 32 hex digit UUID
#define NONASCII_CHAR ('?')
#define USB_DEV_SN_DESC_SIZE (USB_DEV_SN_LEN * 2 + 2)

const char *default_serial_num = "0123456789";
char usb_dev_sn[USB_DEV_SN_LEN + 1];

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

/**
 * Convert a USB string descriptor to C style ASCII
 *
 * The string placed in str is always null-terminated which may cause the
 * loss of data if n is to small. If the length of descriptor string is less
 * than n, additional null bytes are written to str.
 *
 * @param str output buffer for the ASCII string
 * @param usb_desc USB string descriptor
 * @param n size of str buffer
 * @returns number of non-null bytes returned in str or -1 on failure
 */
int usb_string_desc2ascii(char *str, const uint8_t *usb_desc, size_t n)
{
    if (str == NULL || usb_desc == NULL || n < 1) {
        return -1;
    }
    // bDescriptorType @ offset 1
    if (usb_desc[1] != STRING_DESCRIPTOR) {
        return -1;
    }
    // bLength @ offset 0
    const size_t bLength = usb_desc[0];
    if (bLength % 2 != 0) {
        return -1;
    }
    size_t s, d;
    for (s = 0, d = 2; s < n - 1 && d < bLength; s++, d += 2) {
        // handle non-ASCII characters
        if (usb_desc[d] > 0x7f || usb_desc[d + 1] != 0) {
            str[s] = NONASCII_CHAR;
        } else {
            str[s] = usb_desc[d];
        }
    }
    int str_len = s;
    for (; s < n; s++) {
        str[s] = '\0';
    }
    return str_len;
}

/**
 * Convert a C style ASCII to a USB string descriptor
 *
 * @param usb_desc output buffer for the USB string descriptor
 * @param str ASCII string
 * @param n size of usb_desc buffer, even number
 * @returns number of bytes returned in usb_desc or -1 on failure
 */
int ascii2usb_string_desc(uint8_t *usb_desc, const char *str, size_t n)
{
    if (str == NULL || usb_desc == NULL || n < 4) {
        return -1;
    }
    if (n % 2 != 0) {
        return -1;
    }
    size_t s, d;
    // set bString (@ offset 2 onwards) as a UNICODE UTF-16LE string
    memset(usb_desc, 0, n);
    for (s = 0, d = 2; str[s] != '\0' && d < n; s++, d += 2) {
        usb_desc[d] = str[s];
    }
    // set bLength @ offset 0
    usb_desc[0] = d;
    // set bDescriptorType @ offset 1
    usb_desc[1] = STRING_DESCRIPTOR;
    return d;
}

class TestUSBHID: public USBHID {
private:
    uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
public:
    TestUSBHID(uint16_t vendor_id, uint16_t product_id, const char *serial_number = default_serial_num, uint8_t output_report_length = 64, uint8_t input_report_length = 64) :
        USBHID(get_usb_phy(), output_report_length, input_report_length, vendor_id, product_id, 0x01)
    {
        init();
        int rc = ascii2usb_string_desc(_serial_num_descriptor, serial_number, USB_DEV_SN_DESC_SIZE);
        if (rc < 0) {
            ascii2usb_string_desc(_serial_num_descriptor, default_serial_num, USB_DEV_SN_DESC_SIZE);
        }
    }

    virtual ~TestUSBHID()
    {
        deinit();
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *) _serial_num_descriptor;
    }

    // Make this accessible for tests (public).
    using USBHID::report_desc_length;
};

class TestUSBMouse: public USBMouse {
private:
    uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
public:
    TestUSBMouse(uint16_t vendor_id, uint16_t product_id, const char *serial_number = default_serial_num) :
        USBMouse(get_usb_phy(), REL_MOUSE, vendor_id, product_id, 0x01)
    {
        init();
        int rc = ascii2usb_string_desc(_serial_num_descriptor, serial_number, USB_DEV_SN_DESC_SIZE);
        if (rc < 0) {
            ascii2usb_string_desc(_serial_num_descriptor, default_serial_num, USB_DEV_SN_DESC_SIZE);
        }
    }

    virtual ~TestUSBMouse()
    {
        deinit();
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *) _serial_num_descriptor;
    }

    // Make this accessible for tests (public).
    using USBHID::report_desc_length;
};

class TestUSBKeyboard: public USBKeyboard {
private:
    uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
public:
    TestUSBKeyboard(uint16_t vendor_id, uint16_t product_id, const char *serial_number = default_serial_num) :
        USBKeyboard(get_usb_phy(), vendor_id, product_id, 0x01)
    {
        init();
        int rc = ascii2usb_string_desc(_serial_num_descriptor, serial_number, USB_DEV_SN_DESC_SIZE);
        if (rc < 0) {
            ascii2usb_string_desc(_serial_num_descriptor, default_serial_num, USB_DEV_SN_DESC_SIZE);
        }
    }

    virtual ~TestUSBKeyboard()
    {
        deinit();
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *) _serial_num_descriptor;
    }

    // Make this accessible for tests (public).
    using USBHID::report_desc_length;
};

/** Test Get_Descriptor request with the HID class descriptors
 *
 * Given a USB HID class device connected to a host,
 * when the host issues the Get_Descriptor(HID) request,
 * then the device returns the HID descriptor.
 *
 * When the host issues the Get_Descriptor(Report) request,
 * then the device returns the Report descriptor
 * and the size of the descriptor is equal to USBHID::report_desc_length().
 *
 * Details in USB Device Class Definition for HID, v1.11, paragraph 7.1.
 */
template<typename T, uint16_t PID>
void test_get_hid_class_desc()
{
    T usb_hid(USB_HID_VID, PID, usb_dev_sn);
    usb_hid.connect();
    greentea_send_kv(MSG_KEY_TEST_GET_DESCRIPTOR_HID, MSG_VALUE_DUMMY);

    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);
    TEST_ASSERT_EQUAL_STRING(MSG_KEY_TEST_CASE_PASSED, key);
    uint16_t host_report_desc_len;
    int num_args = sscanf(value, "%04hx", &host_report_desc_len);
    TEST_ASSERT_MESSAGE(num_args != 0 && num_args != EOF, "Invalid data received from host.");
    TEST_ASSERT_EQUAL_UINT16(usb_hid.report_desc_length(), host_report_desc_len);
}

/** Test Get_Descriptor request with the Configuration descriptor
 *
 * Given a USB HID class device connected to a host,
 * when the host issues the Get_Descriptor(Configuration) request,
 * then the device returns the Configuration descriptor and a HID
 * descriptor for each HID interface.
 *
 * Details in USB Device Class Definition for HID, v1.11, paragraph 7.1.
 */
template<typename T, uint16_t PID>
void test_get_configuration_desc()
{
    T usb_hid(USB_HID_VID, PID, usb_dev_sn);
    usb_hid.connect();
    greentea_send_kv(MSG_KEY_TEST_GET_DESCRIPTOR_CFG, MSG_VALUE_DUMMY);

    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);
    TEST_ASSERT_EQUAL_STRING(MSG_KEY_TEST_CASE_PASSED, key);
}

/** Test HID class requests
 *
 * Given a USB HID class device connected to a host,
 * when the host issues a request specific to the HID class device type,
 * then the device returns valid data.
 *
 * Details in USB Device Class Definition for HID, v1.11,
 * paragraph 7.2 and Appendix G.
 */
template<typename T, uint16_t PID>
void test_class_requests()
{
    T usb_hid(USB_HID_VID, PID, usb_dev_sn);
    usb_hid.connect();
    greentea_send_kv(MSG_KEY_TEST_REQUESTS, MSG_VALUE_DUMMY);

    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);
    TEST_ASSERT_EQUAL_STRING(MSG_KEY_TEST_CASE_PASSED, key);
}

/** Test send & read
 *
 * Given a USB HID class device connected to a host,
 * when the device sends input reports with a random data to the host
 * and the host sends them back to the device,
 * then received output report data is equal to the input report data.
 */
template<uint8_t REPORT_SIZE> // Range [1, MAX_HID_REPORT_SIZE].
void test_generic_raw_io()
{
    TestUSBHID usb_hid(USB_HID_VID, USB_HID_PID_GENERIC2, usb_dev_sn, REPORT_SIZE, REPORT_SIZE);
    usb_hid.connect();
    greentea_send_kv(MSG_KEY_TEST_RAW_IO, REPORT_SIZE);

    // Wait for the host HID driver to complete setup.
    char key[MSG_KEY_LEN + 1] = { };
    char value[MSG_VALUE_LEN + 1] = { };
    greentea_parse_kv(key, value, MSG_KEY_LEN, MSG_VALUE_LEN);
    TEST_ASSERT_EQUAL_STRING(MSG_KEY_HOST_READY, key);
    if (strcmp(value, MSG_VALUE_NOT_SUPPORTED) == 0) {
        TEST_IGNORE_MESSAGE("Test case not supported by host plarform.");
        return;
    }

    // Report ID omitted here. There are no Report ID tags in the Report descriptor.
    HID_REPORT input_report = {};
    HID_REPORT output_report = {};
    for (size_t r = 0; r < RAW_IO_REPS; r++) {
        for (size_t i = 0; i < REPORT_SIZE; i++) {
            input_report.data[i] = (uint8_t)(rand() % 0x100);
        }
        input_report.length = REPORT_SIZE;
        output_report.length = 0;
        TEST_ASSERT(usb_hid.send(&input_report));
        TEST_ASSERT(usb_hid.read(&output_report));
        TEST_ASSERT_EQUAL_UINT32(input_report.length, output_report.length);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(input_report.data, output_report.data, REPORT_SIZE);
    }
}

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(45, "usb_device_hid");
    srand((unsigned) ticker_read_us(get_us_ticker_data()));

    utest::v1::status_t status = utest::v1::greentea_test_setup_handler(number_of_cases);
    if (status != utest::v1::STATUS_CONTINUE) {
        return status;
    }

    char key[MSG_KEY_LEN + 1] = { };
    char usb_dev_uuid[USB_DEV_SN_LEN + 1] = { };

    greentea_send_kv(MSG_KEY_DEVICE_READY, MSG_VALUE_DUMMY);
    greentea_parse_kv(key, usb_dev_uuid, MSG_KEY_LEN, USB_DEV_SN_LEN + 1);

    if (strcmp(key, MSG_KEY_SERIAL_NUMBER) != 0) {
        utest_printf("Invalid message key.\n");
        return utest::v1::STATUS_ABORT;
    }

    strncpy(usb_dev_sn, usb_dev_uuid, USB_DEV_SN_LEN + 1);
    return status;
}

Case cases[] = {
    Case("Configuration descriptor, generic", test_get_configuration_desc<TestUSBHID, USB_HID_PID_GENERIC>),
    Case("Configuration descriptor, keyboard", test_get_configuration_desc<TestUSBKeyboard, USB_HID_PID_KEYBOARD>),
    Case("Configuration descriptor, mouse", test_get_configuration_desc<TestUSBMouse, USB_HID_PID_MOUSE>),

    Case("HID class descriptors, generic", test_get_hid_class_desc<TestUSBHID, USB_HID_PID_GENERIC>),
    Case("HID class descriptors, keyboard", test_get_hid_class_desc<TestUSBKeyboard, USB_HID_PID_KEYBOARD>),
    Case("HID class descriptors, mouse", test_get_hid_class_desc<TestUSBMouse, USB_HID_PID_MOUSE>),

    // HID class requests not supported by Mbed
    // Case("HID class requests, generic", test_class_requests<TestUSBHID, USB_HID_PID_GENERIC>),
    // Case("HID class requests, keyboard", test_class_requests<TestUSBKeyboard, USB_HID_PID_KEYBOARD>),
    // Case("HID class requests, mouse", test_class_requests<TestUSBMouse, USB_HID_PID_MOUSE>),

    Case("Raw input/output, 1-byte reports", test_generic_raw_io<1>),
    Case("Raw input/output, 20-byte reports", test_generic_raw_io<20>),
    Case("Raw input/output, 64-byte reports", test_generic_raw_io<64>),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#endif // !defined(MBED_CONF_RTOS_PRESENT)
#endif // !defined(USB_DEVICE_TESTS)
