/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include "common.h"
#include <stdio.h>
#include "drivers/DigitalOut.h"

#if I2C_DEBUG_PIN_MASTER==1 || I2C_DEBUG_PIN_SLAVE==1

static mbed::DigitalOut test_pin(D8);

void test_pin_init()
{
    test_pin.write(0);
}


void test_pin_toggle(int count)
{
    int cv = test_pin.read();
    for (int i = 0; i < count; i++) {
        test_pin.write(cv == 0 ? 1 : 0);
    }
    test_pin.write(cv == 0 ? 0 : 1);
}


bool test_check(bool condition, const char *condition_str, const char *file, int line)
{
    if (!condition) {
        printf("assertion failed: %s  %s:%d\r\n", condition_str, file, line);
        return false;
    } else {
        return true;
    }
}

#endif // I2C_DEBUG_PIN_MASTER==1 || I2C_DEBUG_PIN_SLAVE==1

bool test_check_message(bool condition, const char *condition_str, const char *message, const char *file, int line)
{
    if (!condition) {
        printf("assertion failed: %s  %s  %s:%d\r\n", condition_str, message, file, line);
        return false;
    } else {
        return true;
    }
}

bool test_check_equal_int(int32_t expected, int32_t actual, const char *file, int line)
{
    if (expected != actual) {
        printf("assertion failed: %d != %d  %s:%d\r\n", expected, actual, file, line);
        return false;
    } else {
        return true;
    }
}

bool test_check_equal_uint(uint32_t expected, uint32_t actual, const char *file, int line)
{
    if (expected != actual) {
        printf("assertion failed: %u != %u  %s:%d\r\n", expected, actual, file, line);
        return false;
    } else {
        return true;
    }
}

bool test_check_uint_within(uint32_t min, uint32_t max, uint32_t actual, const char *file, int line)
{
    if (actual < min || actual > max) {
        printf("assertion failed: %u not in range (%u,%u) %s:%d\r\n", actual, min, max, file, line);
        return false;
    } else {
        return true;
    }
}
