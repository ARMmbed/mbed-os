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
#include "USBCDC.h"
#include "USBSerial.h"
#include "hal/us_ticker_api.h"

#define USB_CDC_VID 0x1f00
#define USB_CDC_PID 0x2013
#define USB_SERIAL_VID 0x1f00
#define USB_SERIAL_PID 0x2012

#define MSG_KEY_LEN 24
#define MSG_VALUE_DUMMY "0"
#define MSG_KEY_DEVICE_READY "ready"
#define MSG_KEY_SERIAL_NUMBER "usb_dev_sn"
#define MSG_KEY_PORT_OPEN_WAIT "port_open_wait"
#define MSG_KEY_PORT_OPEN_CLOSE "port_open_close"
#define MSG_KEY_SEND_BYTES_SINGLE "send_single"
#define MSG_KEY_SEND_BYTES_MULTIPLE "send_multiple"
#define MSG_KEY_LOOPBACK "loopback"
#define MSG_KEY_CHANGE_LINE_CODING "change_lc"

#define TX_BUFF_SIZE 32
#define RX_BUFF_SIZE 32

// The size of every data chunk the host sends (for each value from a
// known sequence) during 'CDC RX multiple' test cases is
// HOST_RX_BUFF_SIZE_RATIO times the size of RX_BUFF_SIZE input buffer.
// This way the device has to correctly handle data bigger that its buffer.
#define HOST_RX_BUFF_SIZE_RATIO 64

// A DTR line is used to signal that the host has configured a terminal and
// is ready to transmit and receive data from the USB CDC/Serial device.
// When this test suite is run with the use of a Linux host, a workaround has
// to be used to overcome some platform specific DTR line behavior.
// Every time the serial port file descriptor is opened, the DTR line is
// asserted until the terminal attributes are set.
// As a consequence, the device receives a premature DTR signal with a
// duration of 200-500 us before the correct, long-lasting DTR signal set by
// the host-side test script. (tested on the Linux kernel 4.15.0)
//
// Online references:
// https://github.com/pyserial/pyserial/issues/124#issuecomment-227235402
//
// The solution is to wait for the first DTR spike, ignore it, and wait for
// the correct DTR signal again.
#define LINUX_HOST_DTR_FIX 1
#define LINUX_HOST_DTR_FIX_DELAY_MS 1

#define CDC_LOOPBACK_REPS 1200
#define SERIAL_LOOPBACK_REPS 100
#define USB_RECONNECT_DELAY_MS 1

#define LINE_CODING_STRLEN 13 // 6 + 2 + 1 + 1 + 3 * comma

#define USB_DEV_SN_LEN (32) // 32 hex digit UUID
#define NONASCII_CHAR ('?')
#define USB_DEV_SN_DESC_SIZE (USB_DEV_SN_LEN * 2 + 2)

const char *default_serial_num = "0123456789";
char usb_dev_sn[USB_DEV_SN_LEN + 1];

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

typedef struct LineCoding {
    // bits per second
    int baud;

    // 5, 6, 7, 8 or 16
    int bits;

    // 0 -- None,
    // 1 -- Odd,
    // 2 -- Even,
    // 3 -- Mark,
    // 4 -- Space
    int parity;

    // 0 -- 1 Stop bit,
    // 1 -- 1.5 Stop bits,
    // 2 -- 2 Stop bits
    int stop;

    int get_num_diffs(LineCoding const &other) const
    {
        int diffs = 0;
        if (baud != other.baud) {
            diffs++;
        }
        if (bits != other.bits) {
            diffs++;
        }
        if (parity != other.parity) {
            diffs++;
        }
        if (stop != other.stop) {
            diffs++;
        }
        return diffs;
    }
} line_coding_t;

line_coding_t default_lc = { 9600, 8, 0, 0 };

// There is no POSIX support for 1.5 stop bits.
// Do not set stop bits to 1.5 to keep tests compatible with all supported
// host systems.
line_coding_t test_codings[] = {
    { 9600, 5, 0, 2 },
    { 4800, 7, 2, 0 },
    { 19200, 8, 0, 2 },
    { 115200, 8, 0, 0 },
    { 38400, 8, 1, 0 },
    { 1200, 8, 0, 0 },
    { 19200, 8, 0, 0 },
    { 2400, 7, 2, 0 },
    { 9600, 8, 0, 0 },
    { 57600, 8, 0, 0 },
};

Mail<line_coding_t, 8> lc_mail;

#define EF_SEND (1ul << 0)
EventFlags event_flags;

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

class TestUSBCDC: public USBCDC {
private:
    uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
public:
    TestUSBCDC(uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001,
               const char *serial_number = default_serial_num) :
        USBCDC(get_usb_phy(), vendor_id, product_id, product_release)
    {
        init();
        int rc = ascii2usb_string_desc(_serial_num_descriptor, serial_number, USB_DEV_SN_DESC_SIZE);
        if (rc < 0) {
            ascii2usb_string_desc(_serial_num_descriptor, default_serial_num, USB_DEV_SN_DESC_SIZE);
        }
    }

    virtual ~TestUSBCDC()
    {
        deinit();
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *) _serial_num_descriptor;
    }
};

class TestUSBSerial: public USBSerial {
private:
    uint8_t _serial_num_descriptor[USB_DEV_SN_DESC_SIZE];
public:
    TestUSBSerial(uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001,
                  const char *serial_number = default_serial_num) :
        USBSerial(get_usb_phy(), vendor_id, product_id, product_release)
    {
        int rc = ascii2usb_string_desc(_serial_num_descriptor, serial_number, USB_DEV_SN_DESC_SIZE);
        if (rc < 0) {
            ascii2usb_string_desc(_serial_num_descriptor, default_serial_num, USB_DEV_SN_DESC_SIZE);
        }
    }

    virtual ~TestUSBSerial()
    {
        deinit();
    }

    virtual const uint8_t *string_iserial_desc()
    {
        return (const uint8_t *) _serial_num_descriptor;
    }
};

/** Test CDC USB reconnect
 *
 * Given the host has successfully opened the port of a USB CDC device
 * When the USB device disconnects and connects again
 * Then the host is able to successfully open the port again
 */
void test_cdc_usb_reconnect()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    // Connect the USB device.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, MSG_VALUE_DUMMY);
    // Wait for the host to open the port.
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    // Disconnect the USB device.
    usb_cdc.disconnect();
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    ThisThread::sleep_for(USB_RECONNECT_DELAY_MS);
    // Connect the USB device again.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, MSG_VALUE_DUMMY);
    // Wait for the host to open the port again.
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    // Disconnect the USB device again.
    usb_cdc.disconnect();
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());
}

/** Test CDC receive single bytes
 *
 * Given the USB CDC device connected to a host
 * When the host transmits a known sequence one byte at a time
 * Then every byte received by the device matches the sequence
 */
void test_cdc_rx_single_bytes()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    uint8_t buff = 0x01;
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_cdc.disconnect();
}

void tx_thread_fun(USBCDC *usb_cdc)
{
    uint8_t buff_val = 0;
    uint8_t buff[TX_BUFF_SIZE] = { 0 };
    while (event_flags.get() & EF_SEND) {
        if (!usb_cdc->send(buff, TX_BUFF_SIZE)) {
            ThisThread::sleep_for(1);
            continue;
        }
        buff_val++;
        memset(buff, buff_val, TX_BUFF_SIZE);
    }
}

/** Test CDC receive single bytes concurrently
 *
 * Given the USB CDC device connected to a host
 * When the host transmits a known sequence one byte at a time
 *     and at the same time the device transmits data to host
 * Then every byte received by the device matches the sequence
 */
void test_cdc_rx_single_bytes_concurrent()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    Thread tx_thread;
    event_flags.set(EF_SEND);
    tx_thread.start(mbed::callback(tx_thread_fun, &usb_cdc));
    uint8_t buff = 0x01;
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT(usb_cdc.receive(&buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(expected, buff);
    }
    event_flags.clear(EF_SEND);
    tx_thread.join();
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC receive multiple bytes
 *
 * Given the USB CDC device connected to a host
 * When the host transmits chunks of data following a known sequence
 * Then every chunk received by the device matches the sequence
 */
void test_cdc_rx_multiple_bytes()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_MULTIPLE, HOST_RX_BUFF_SIZE_RATIO);
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        for (int chunk = 0; chunk < HOST_RX_BUFF_SIZE_RATIO; chunk++) {
            memset(expected_buff, expected, RX_BUFF_SIZE);
            TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
        }
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        for (int chunk = 0; chunk < HOST_RX_BUFF_SIZE_RATIO; chunk++) {
            memset(expected_buff, expected, RX_BUFF_SIZE);
            TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
        }
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC receive multiple bytes concurrently
 *
 * Given the USB CDC device connected to a host
 * When the host transmits chunks of data following a known sequence
 *     and at the same time the device transmits data to host
 * Then every chunk received by the device matches the sequence
 */
void test_cdc_rx_multiple_bytes_concurrent()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_MULTIPLE, HOST_RX_BUFF_SIZE_RATIO);
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    Thread tx_thread;
    event_flags.set(EF_SEND);
    tx_thread.start(mbed::callback(tx_thread_fun, &usb_cdc));
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        for (int chunk = 0; chunk < HOST_RX_BUFF_SIZE_RATIO; chunk++) {
            memset(expected_buff, expected, RX_BUFF_SIZE);
            TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
        }
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        for (int chunk = 0; chunk < HOST_RX_BUFF_SIZE_RATIO; chunk++) {
            memset(expected_buff, expected, RX_BUFF_SIZE);
            TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
            TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
        }
    }
    event_flags.clear(EF_SEND);
    tx_thread.join();
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_cdc.disconnect();
}

/** Test CDC loopback
 *
 * Given the USB CDC device connected to a host
 * When the device transmits random bytes to host
 *     and the host transmits them back to the device
 * Then every byte received by the device is equal to byte preciously sent
 */
void test_cdc_loopback()
{
    TestUSBCDC usb_cdc(USB_CDC_VID, USB_CDC_PID, 1, usb_dev_sn);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_LOOPBACK, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_cdc.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_cdc.wait_ready();
    uint8_t rx_buff, tx_buff;
    for (int i = 0; i < CDC_LOOPBACK_REPS; i++) {
        tx_buff = (uint8_t)(rand() % 0x100);
        rx_buff = (uint8_t)(tx_buff + 1);
        TEST_ASSERT(usb_cdc.send(&tx_buff, 1));
        TEST_ASSERT(usb_cdc.receive(&rx_buff, 1, NULL));
        TEST_ASSERT_EQUAL_UINT8(tx_buff, rx_buff);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_cdc.disconnect();
}

/** Test Serial USB reconnect
 *
 * Given the host has successfully opened the port of a USB Serial device
 * When the USB device disconnects and connects again
 * Then the host is able to successfully open the port again
 */
void test_serial_usb_reconnect()
{
    TestUSBSerial usb_serial(USB_SERIAL_VID, USB_SERIAL_PID, 1, usb_dev_sn);
    TEST_ASSERT_FALSE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Connect the USB device.
    usb_serial.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_serial.configured()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, MSG_VALUE_DUMMY);
    // Wait for the host to open the port.
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    while (!usb_serial.connected()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Disconnect the USB device.
    usb_serial.disconnect();
    TEST_ASSERT_FALSE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    ThisThread::sleep_for(USB_RECONNECT_DELAY_MS);
    // Connect the USB device again.
    usb_serial.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_serial.configured()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, MSG_VALUE_DUMMY);
    // Wait for the host to open the port again.
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    while (!usb_serial.connected()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Disconnect the USB device again.
    usb_serial.disconnect();
    TEST_ASSERT_FALSE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());
}

/** Test Serial terminal reopen
 *
 * Given the host has successfully opened the port of a USB Serial device
 * When the host closes its port
 * Then the host is able to successfully open the port again
 */
void test_serial_term_reopen()
{
    TestUSBSerial usb_serial(USB_SERIAL_VID, USB_SERIAL_PID, 1, usb_dev_sn);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_PORT_OPEN_CLOSE, MSG_VALUE_DUMMY);
    // Wait for the host to open the terminal.
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    while (!usb_serial.connected()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.ready());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Wait for the host to close the terminal.
    while (usb_serial.ready()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.ready());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_CLOSE, MSG_VALUE_DUMMY);
    // Wait for the host to open the terminal again.
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    while (!usb_serial.connected()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.ready());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Wait for the host to close the terminal again.
    while (usb_serial.ready()) {
        ThisThread::sleep_for(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.ready());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    usb_serial.disconnect();
}

/** Test Serial getc
 *
 * Given the USB Serial device connected to a host
 * When the host transmits a known sequence one byte at a time
 * Then every byte received by the device matches the sequence
 */
void test_serial_getc()
{
    TestUSBSerial usb_serial(USB_SERIAL_VID, USB_SERIAL_PID, 1, usb_dev_sn);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_serial.wait_ready();
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT_EQUAL_INT(expected, usb_serial.getc());
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT_EQUAL_INT(expected, usb_serial.getc());
    }
    // Wait for the host to close its port.
    while (usb_serial.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_serial.disconnect();
}

/** Test Serial printf & scanf
 *
 * Given the USB Serial device connected to a host
 * When the device transmits a formatted string with a random value
 *     using the printf method
 *     and the host sends it back to the device
 * Then the device can successfully read the value using scanf method
 *     and the value received is equal value sent
 */
void test_serial_printf_scanf()
{
    TestUSBSerial usb_serial(USB_SERIAL_VID, USB_SERIAL_PID, 1, usb_dev_sn);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_LOOPBACK, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_serial.wait_ready();
    static const char fmt[] = "Formatted\nstring %i.";
    int tx_val, rx_val, rc;
    for (int i = 0; i < SERIAL_LOOPBACK_REPS; i++) {
        tx_val = rand();
        rx_val = tx_val + 1;
        rc = usb_serial.printf(fmt, tx_val);
        TEST_ASSERT(rc > 0);
        rc = usb_serial.scanf(fmt, &rx_val);
        TEST_ASSERT(rc == 1);
        TEST_ASSERT_EQUAL_INT(tx_val, rx_val);
    }
    // Wait for the host to close its port.
    while (usb_serial.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_serial.disconnect();
}

void line_coding_changed_cb(int baud, int bits, int parity, int stop)
{
    line_coding_t *lc = lc_mail.alloc();
    lc->baud = baud;
    lc->bits = bits;
    lc->parity = parity;
    lc->stop = stop;
    lc_mail.put(lc);
}

/** Test Serial / CDC line coding change
 *
 * Given the device transmits a set of line coding params to host
 * When the host updates serial port settings
 * Then line_coding_changed() callback is called
 *     and the line coding is set as expected
 */
void test_serial_line_coding_change()
{
    TestUSBSerial usb_serial(USB_SERIAL_VID, USB_SERIAL_PID, 1, usb_dev_sn);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_CHANGE_LINE_CODING, MSG_VALUE_DUMMY);
#if LINUX_HOST_DTR_FIX
    usb_serial.wait_ready();
    ThisThread::sleep_for(LINUX_HOST_DTR_FIX_DELAY_MS);
#endif
    usb_serial.wait_ready();
    usb_serial.attach(line_coding_changed_cb);
    size_t num_line_codings = sizeof test_codings / sizeof test_codings[0];
    line_coding_t *lc_prev = &default_lc;
    line_coding_t *lc_expected = NULL;
    line_coding_t *lc_actual = NULL;
    int num_expected_callbacks, rc;
    for (size_t i = 0; i < num_line_codings; i++) {
        lc_expected = &(test_codings[i]);
        num_expected_callbacks = lc_prev->get_num_diffs(*lc_expected);
        rc = usb_serial.printf("%06i,%02i,%01i,%01i", lc_expected->baud, lc_expected->bits, lc_expected->parity,
                               lc_expected->stop);
        TEST_ASSERT_EQUAL_INT(LINE_CODING_STRLEN, rc);
        // The pyserial Python module does not update all line coding params
        // at once. It updates params one by one instead, and since every
        // update is followed by port reconfiguration we get multiple
        // calls to line_coding_changed callback on the device.
        while (num_expected_callbacks > 0) {
            num_expected_callbacks--;
            osEvent event = lc_mail.get();
            TEST_ASSERT_EQUAL_UINT32(osEventMail, event.status);
            lc_actual = (line_coding_t *) event.value.p;
            if (lc_expected->get_num_diffs(*lc_actual) == 0) {
                break;
            } else if (num_expected_callbacks > 0) {
                // Discard lc_actual only if there is still a chance to get new
                // set of params.
                lc_mail.free(lc_actual);
            }
        }
        TEST_ASSERT_EQUAL_INT(lc_expected->baud, lc_actual->baud);
        TEST_ASSERT_EQUAL_INT(lc_expected->bits, lc_actual->bits);
        TEST_ASSERT_EQUAL_INT(lc_expected->parity, lc_actual->parity);
        TEST_ASSERT_EQUAL_INT(lc_expected->stop, lc_actual->stop);
        lc_mail.free(lc_actual);
        lc_prev = lc_expected;
    }
    // Wait for the host to close its port.
    while (usb_serial.ready()) {
        ThisThread::sleep_for(1);
    }
    usb_serial.disconnect();
}

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(45, "usb_device_serial");
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
    Case("CDC USB reconnect", test_cdc_usb_reconnect),
    Case("CDC RX single bytes", test_cdc_rx_single_bytes),
    Case("CDC RX single bytes concurrent", test_cdc_rx_single_bytes_concurrent),
    Case("CDC RX multiple bytes", test_cdc_rx_multiple_bytes),
    Case("CDC RX multiple bytes concurrent", test_cdc_rx_multiple_bytes_concurrent),
    Case("CDC loopback", test_cdc_loopback),
    Case("Serial USB reconnect", test_serial_usb_reconnect),
    Case("Serial terminal reopen", test_serial_term_reopen),
    Case("Serial getc", test_serial_getc),
    Case("Serial printf/scanf", test_serial_printf_scanf),
    Case("Serial line coding change", test_serial_line_coding_change),
};

Specification specification((utest::v1::test_setup_handler_t) testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#endif // !defined(MBED_CONF_RTOS_PRESENT)
#endif // !defined(USB_DEVICE_TESTS)
