/*
 * Copyright (c) 2019 Arm Limited and affiliates.
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
#if !DEVICE_SERIAL
#error [NOT_SUPPORTED] serial communication not supported for this target
#else

#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "platform/FileHandle.h"
#include "drivers/BufferedSerial.h"


using namespace utest::v1;


/**
 * Macros for setting console flow control.
 */
#define CONSOLE_FLOWCONTROL_RTS                                               1
#define CONSOLE_FLOWCONTROL_CTS                                               2
#define CONSOLE_FLOWCONTROL_RTSCTS                                            3
#define mbed_console_concat_(x) CONSOLE_FLOWCONTROL_##x
#define mbed_console_concat(x) mbed_console_concat_(x)
#define CONSOLE_FLOWCONTROL mbed_console_concat(MBED_CONF_TARGET_CONSOLE_UART_FLOW_CONTROL)


#define MSG_KEY_ECHO_MESSAGE                                     "echo_message"
#define MSG_VALUE_HELLO_WORLD                                   "Hello, world!"

#define EXPECTED_ECHOED_STRING "{{" MSG_KEY_ECHO_MESSAGE ";" MSG_VALUE_HELLO_WORLD "}}"
// The target is expected to transmit Greentea messages with \n (or \r\n) or they are not detected by the host
#define STRING_TO_SEND EXPECTED_ECHOED_STRING "\n"


static BufferedSerial buffered_serial_obj(
    USBTX, USBRX, MBED_CONF_PLATFORM_STDIO_BAUD_RATE
);


FileHandle *mbed::mbed_override_console(int fd)
{
    return &buffered_serial_obj;
}

static ssize_t buffered_serial_read(void *buffer, ssize_t length)
{
    if (length == 0) {
        return 0;
    }

    // Ignore the `\n` character previously sent to the host in the previous
    // key-value pair that may not have been removed from the FIFO.
    unsigned char *buf = static_cast<unsigned char *>(buffer);
    buffered_serial_obj.read(buf, 1);
    ssize_t i = (buf[0] == '{') ? 1 : 0;

    // Get the message sent by the host
    for (; i < length; i++) {
        TEST_ASSERT_EQUAL_UINT(1, buffered_serial_obj.read(buf + i, 1));
    }

    return length;
}


// Test that data sent using an UnbufferedSerial object is correctly sent.
// The test case sends a Greentea key-value pair message from the target to the
// host using an UnbufferedSerial object and expects the message
// to be echoed back by the host. The host response is received via the Greentea
// framework usual route using greentea_parse_kv(). Success is determined upon
// reception of the echoed message which indicates that the message was received
// by the host as it was sent by the target.
static void test_serial_write()
{
    char tx_msg[] = STRING_TO_SEND;

    TEST_ASSERT_EQUAL_UINT(
        strlen(tx_msg) + 1,
        buffered_serial_obj.write(tx_msg, strlen(tx_msg) + 1)
    );

    char rx_key[30] = {0};
    char rx_value[30] = {0};
    greentea_parse_kv(rx_key, rx_value, sizeof(rx_key), sizeof(rx_value));

    TEST_ASSERT_EQUAL_STRING(MSG_KEY_ECHO_MESSAGE, rx_key);
    TEST_ASSERT_EQUAL_STRING(MSG_VALUE_HELLO_WORLD, rx_value);
}


// Test that data received using an UnbufferedSerial object is correctly received.
// The test case sends a Greentea key-value pair message from the target to the
// host via the Greentea framework usual route using greentea_send_kv().
// It expects the message to be echoed back to the target. An UnbufferedSerial
// object is used to handle the received message. Succes is determined upon
// reception of a key-value pair matching the key-value pair sent by the target.
static void test_serial_read()
{
    greentea_send_kv(MSG_KEY_ECHO_MESSAGE, MSG_VALUE_HELLO_WORLD);

    char rx_msg[sizeof(EXPECTED_ECHOED_STRING)] = {0};
    // Exclude the null terminator which is not read
    ssize_t expected_rx_msg_length = sizeof(EXPECTED_ECHOED_STRING) - 1;

    buffered_serial_read(rx_msg, expected_rx_msg_length);

    TEST_ASSERT_EQUAL_STRING(EXPECTED_ECHOED_STRING, rx_msg);
}


utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(12, "serial_comms");

    return greentea_test_setup_handler(number_of_cases);
}


utest::v1::status_t greentea_failure_handler(
    const Case *const source, const failure_t reason
)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}


Case cases[] = {
    Case(
        "Bytes are correctly sent",
        test_serial_write, greentea_failure_handler
    ),
    Case(
        "Bytes are correctly received",
        test_serial_read, greentea_failure_handler
    ),
};


Specification specification(
    greentea_setup, cases, greentea_test_teardown_handler
);


int main()
{
#if   CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTS
    buffered_serial_obj.set_flow_control(
        SerialBase::RTS, STDIO_UART_RTS, NC
    );
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_CTS
    buffered_serial_obj.set_flow_control(
        SerialBase::CTS, NC, STDIO_UART_CTS
    );
#elif CONSOLE_FLOWCONTROL == CONSOLE_FLOWCONTROL_RTSCTS
    buffered_serial_obj.set_flow_control(
        SerialBase::RTSCTS, STDIO_UART_RTS, STDIO_UART_CTS
    );
#endif
    return !Harness::run(specification);
}

#endif // !DEVICE_SERIAL
