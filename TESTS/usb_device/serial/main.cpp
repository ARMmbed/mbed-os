/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
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
#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#error [NOT_SUPPORTED] USB Device not supported for this target
#endif

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "mbed.h"
#include <stdlib.h>
#include "USBCDC.h"
#include "USBSerial.h"

#define USB_CDC_VID 0x1f00
#define USB_CDC_PID 0x2013
#define USB_SERIAL_VID 0x1f00
#define USB_SERIAL_PID 0x2012
#define USB_SN_MAX_LEN 128

#define MSG_KEY_PORT_OPEN_WAIT "port_open_wait"
#define MSG_KEY_PORT_OPEN_CLOSE "port_open_close"
#define MSG_KEY_SEND_BYTES_SINGLE "send_single"
#define MSG_KEY_SEND_BYTES_MULTIPLE "send_multiple"
#define MSG_KEY_LOOPBACK "loopback"

#define TX_BUFF_SIZE 32
#define RX_BUFF_SIZE 32
#define CDC_LOOPBACK_REPS 1200
#define SERIAL_LOOPBACK_REPS 100
#define USB_RECONNECT_DELAY_MS 200

// Additional delay necessary for Windows hosts
// to handle the reconnect operation correctly.
#define USB_DISCONNECT_DELAY_MS 1

using utest::v1::Case;
using utest::v1::Specification;
using utest::v1::Harness;

#define EF_SEND (1ul << 0)
EventFlags event_flags;

char *usb_desc2str(const uint8_t *usb_desc, char *str, size_t n)
{
    const size_t desc_size = usb_desc[0] - 2;
    const uint8_t *src = &usb_desc[2];
    size_t i, j;
    for (i = 0, j = 0; i < n && j < desc_size; i++, j += 2) {
        str[i] = src[j];
    }
    for (; i < n; i++) {
        str[i] = '\0';
    }
    return str;
}

/** Test CDC USB reconnect
 *
 * Given the host has successfully opened the port of a USB CDC device
 * When the USB device disconnects and connects again
 * Then the host is able to successfully open the port again
 */
void test_cdc_usb_reconnect()
{
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    // Connect the USB device.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, usb_cdc_sn);
    // Wait for the host to open the port.
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    wait_ms(USB_DISCONNECT_DELAY_MS);
    // Disconnect the USB device.
    usb_cdc.disconnect();
    TEST_ASSERT_FALSE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    wait_ms(USB_RECONNECT_DELAY_MS);
    // Connect the USB device again.
    usb_cdc.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_cdc.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_FALSE(usb_cdc.ready());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, usb_cdc_sn);
    // Wait for the host to open the port again.
    usb_cdc.wait_ready();
    TEST_ASSERT_TRUE(usb_cdc.configured());
    TEST_ASSERT_TRUE(usb_cdc.ready());

    wait_ms(USB_DISCONNECT_DELAY_MS);
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
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, usb_cdc_sn);
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
        wait_ms(1);
    }
    usb_cdc.disconnect();
}

void tx_thread_fun(USBCDC *usb_cdc)
{
    uint8_t buff_val = 0;
    uint8_t buff[TX_BUFF_SIZE] = { 0 };
    while (event_flags.get() & EF_SEND) {
        if (!usb_cdc->send(buff, TX_BUFF_SIZE)) {
            wait_ms(1);
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
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, usb_cdc_sn);
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
        wait_ms(1);
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
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_MULTIPLE, usb_cdc_sn);
    usb_cdc.wait_ready();
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
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
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_MULTIPLE, usb_cdc_sn);
    usb_cdc.wait_ready();
    Thread tx_thread;
    event_flags.set(EF_SEND);
    tx_thread.start(mbed::callback(tx_thread_fun, &usb_cdc));
    uint8_t buff[RX_BUFF_SIZE] = { 0 };
    uint8_t expected_buff[RX_BUFF_SIZE] = { 0 };
    for (int expected = 0xff; expected >= 0; expected--) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        memset(expected_buff, expected, RX_BUFF_SIZE);
        TEST_ASSERT(usb_cdc.receive(buff, RX_BUFF_SIZE, NULL));
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_buff, buff, RX_BUFF_SIZE);
    }
    event_flags.clear(EF_SEND);
    tx_thread.join();
    // Wait for the host to close its port.
    while (usb_cdc.ready()) {
        wait_ms(1);
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
    USBCDC usb_cdc(false, USB_CDC_VID, USB_CDC_PID);
    char usb_cdc_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_cdc.string_iserial_desc(), usb_cdc_sn, USB_SN_MAX_LEN);
    usb_cdc.connect();
    greentea_send_kv(MSG_KEY_LOOPBACK, usb_cdc_sn);
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
        wait_ms(1);
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
    USBSerial usb_serial(false, USB_SERIAL_VID, USB_SERIAL_PID);
    char usb_serial_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_serial.string_iserial_desc(), usb_serial_sn, USB_SN_MAX_LEN);
    TEST_ASSERT_FALSE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Connect the USB device.
    usb_serial.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_serial.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, usb_serial_sn);
    // Wait for the host to open the port.
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Disconnect the USB device.
    wait_ms(USB_DISCONNECT_DELAY_MS);
    usb_serial.disconnect();
    TEST_ASSERT_FALSE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    wait_ms(USB_RECONNECT_DELAY_MS);
    // Connect the USB device again.
    usb_serial.connect();
    // Wait for the USB enumeration to complete.
    while (!usb_serial.configured()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_WAIT, usb_serial_sn);
    // Wait for the host to open the port again.
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Disconnect the USB device again.
    wait_ms(USB_DISCONNECT_DELAY_MS);
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
    USBSerial usb_serial(false, USB_SERIAL_VID, USB_SERIAL_PID);
    char usb_serial_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_serial.string_iserial_desc(), usb_serial_sn, USB_SN_MAX_LEN);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_PORT_OPEN_CLOSE, usb_serial_sn);
    // Wait for the host to open the terminal.
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.ready());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Wait for the host to close the terminal.
    while (usb_serial.ready()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_FALSE(usb_serial.ready());
    TEST_ASSERT_FALSE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    greentea_send_kv(MSG_KEY_PORT_OPEN_CLOSE, usb_serial_sn);
    // Wait for the host to open the terminal again.
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
    TEST_ASSERT_TRUE(usb_serial.configured());
    TEST_ASSERT_TRUE(usb_serial.ready());
    TEST_ASSERT_TRUE(usb_serial.connected());
    TEST_ASSERT_EQUAL_INT(0, usb_serial.readable());

    // Wait for the host to close the terminal again.
    while (usb_serial.ready()) {
        wait_ms(1);
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
    USBSerial usb_serial(false, USB_SERIAL_VID, USB_SERIAL_PID);
    char usb_serial_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_serial.string_iserial_desc(), usb_serial_sn, USB_SN_MAX_LEN);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_SEND_BYTES_SINGLE, usb_serial_sn);
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
    for (int expected = 0xff; expected >= 0; expected--) {
        TEST_ASSERT_EQUAL_INT(expected, usb_serial.getc());
    }
    for (int expected = 0; expected <= 0xff; expected++) {
        TEST_ASSERT_EQUAL_INT(expected, usb_serial.getc());
    }
    // Wait for the host to close its port.
    while (usb_serial.ready()) {
        wait_ms(1);
    }
    usb_serial.disconnect();
}

/** Test Serial printf & scanf
 *
 * Given the USB Serial device connected to a host
 * When the device trensmits a formatted string with a random value
 *     using the printf method
 *     and the host sends it back to the device
 * Then the device can successfully read the value using scanf method
 *     and the value received is equal value sent
 */
void test_serial_printf_scanf()
{
    USBSerial usb_serial(false, USB_SERIAL_VID, USB_SERIAL_PID);
    char usb_serial_sn[USB_SN_MAX_LEN] = { };
    usb_desc2str(usb_serial.string_iserial_desc(), usb_serial_sn, USB_SN_MAX_LEN);
    usb_serial.connect();
    greentea_send_kv(MSG_KEY_LOOPBACK, usb_serial_sn);
    while (!usb_serial.connected()) {
        wait_ms(1);
    }
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
        wait_ms(1);
    }
    usb_serial.disconnect();
}

utest::v1::status_t testsuite_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(25, "usb_device_serial");
    srand((unsigned) ticker_read_us(get_us_ticker_data()));
    return utest::v1::greentea_test_setup_handler(number_of_cases);
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
};

Specification specification(testsuite_setup, cases);

int main()
{
    return !Harness::run(specification);
}
