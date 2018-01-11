/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "drivers/RawSerial.h"

using namespace mbed;

/* UART TEST FOR GREEN TEA
 *
 * Greentea-client uses RawSerial class to handle UART communication.
 * RawSerial class must provide valid implementation of the following
 * methods (used by Greentea-client):
 *  - int RawSerial::getc()
 *  - int RawSerial::putc(int c).
 *
 * Greentea-client uses USB serial port for communication with the
 * following transmission parameters:
 *  - 8 data bits,
 *  - 1 stop bit,
 *  - none parity bit,
 *  - configurable baudrate.
 *
 * Greentea-client operates in pooling mode only.
 * Flow control is disabled.
 *
 * UART buffer size is limited to 120 bytes.
 *
 * TEST SCENARIO
 * This part of the test is executed on mbed board.
 *
 * After startup it will try to communicate via UART with host part of the test.
 * This part sends 4 messages with different length to the host and waits
 * for the response (host should respond with the same message).
 * Last message "PASSED" is send to inform that all responses from host were
 * successfully received.
 *
 * To perform this test with serial port baud rate different than default (9600),
 * call host test with appropriate --baudrate parameter.
 *
 */

/* Default baud rate if undefined. */
#ifndef BAUD_RATE
#define BAUD_RATE 9600
#endif

#define NUMBER_OF_TEST_STRINGS 5
#define NUMBER_OF_TEST_RETRIES 10

typedef enum
{
    PASSED, FAILED
} test_status;

/* Array of string to be transmitted. */
const char *test_strings[NUMBER_OF_TEST_STRINGS] =
        { "x",
          "mbed", "Green Tea UART bare metal test.",
          "Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message-Long Message--.",
          "PASSED" };

/* Function writes provided string to the serial port. */
void write_line(RawSerial &serial, const char *str)
{
    while (true) {
        serial.putc(*str);
        if (*str == '\0') {
            break;
        }

        str++;
    };
}

/* Function reads provided string from the serial port. */
int read_line(RawSerial &serial, char *buffer)
{
    int count = 0;
    while (true) {
        *buffer = (char) serial.getc();
        count++;
        if (*buffer == '\0') {
            return count;
        }

        buffer += 1;
    }
}

/* Function compares provided strings and returns 0 if are equal. */
int strCmp(const char string1[], char string2[])
{
    for (int i = 0;; i++) {
        if (string1[i] != string2[i]) {
            return string1[i] < string2[i] ? -1 : 1;
        }

        if (string1[i] == '\0') {
            return 0;
        }
    }
}

/* Function writes provided message to the serial port and
 * waits for the response from the Host. Host should
 * respond with the same message.
 *
 * If valid response has been received, then function returns
 * PASSED status, otherwise transmission is repeated.
 *
 * If max number of transmission attempts is reached, then
 * function returns FAILED status.
 *
 * */
test_status test_uart_communication(RawSerial &serial, const char * msg)
{
    char buffer[120];

    for (int i = 0; i < NUMBER_OF_TEST_RETRIES; i++) {
        write_line(serial, msg);
        read_line(serial, buffer);

        if (strCmp(msg, buffer) == 0) {
            return PASSED;
        }
    }

    return FAILED;
}

int main()
{
    RawSerial serial(USBTX, USBRX, BAUD_RATE);

    /* Start the test. */
    for (unsigned int i = 0; i < (sizeof(test_strings) / sizeof(test_strings[0])); i++) {
        if (test_uart_communication(serial, test_strings[i]) == FAILED) {
            break;
        }
    }

    while (true);
}

