/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#include "platform/NonCopyable.h"

#define BUFFER_SIZE_SYNC 30

/* Class to handle communication with the host part of the test. */
class BMComm: private mbed::NonCopyable<BMComm>
{

public:

    BMComm(PinName tx, PinName rx, int baud = 9600)
    {
        _serial = new mbed::RawSerial(tx, rx, baud);
    }

    ~BMComm()
    {
        delete _serial;
    }

    /* Function writes provided null string to the serial port. */
    void write_line(const char *str)
    {
        while (true) {
            _serial->putc(*str);
            if (*str == '\0') {
                break;
            }

            str++;
        };
    }

    /* Function reads null terminated string from the serial port. */
    int read_line(char *buffer)
    {
        int count = 0;
        while (true) {
            *buffer = (char) _serial->getc();
            count++;
            if (*buffer == '\0') {
                return count;
            }

            buffer += 1;
        }
    }

    /* Function perform synchronisation between both parts of the test (host and device). */
    void sync_host_dev()
    {
        char buffer[BUFFER_SIZE_SYNC];

        read_line(buffer);

        while (strCmp(buffer, (char*) "__sync") != 0) {
            wait_cycles(1000);
            read_line(buffer);
        }

        /* If we are here, then sync signal has been received from host.
         * Indicate that the device is ready for test.
         */
        write_line("__ready");
    }

private:

    /* Function which provides delay. */
    void wait_cycles(int volatile cycles)
    {
        while (cycles--)
            ;
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

    mbed::RawSerial * _serial;
};

