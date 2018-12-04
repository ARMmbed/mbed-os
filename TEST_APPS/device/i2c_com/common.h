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

#include "mbed.h"

#ifndef I2C_TEST_COMMON_H
#define I2C_TEST_COMMON_H


// uncomment this in order to test i2c driver layer
//#define TEST_I2C_DRIVER

#define I2C_DEBUG               0
#define I2C_DEBUG_PIN_SLAVE     0
#define I2C_DEBUG_PIN_MASTER    0

#define EMPTY_PARAM_INT (-1)


#define MASTER_1_ID     111
#define MASTER_2_ID     222

#define DEVICE_AS_MASTER   false
#define DEVICE_AS_SLAVE    true

#define I2C_ADDRESS_MASK_7BIT   (0x7F)
#define I2C_ADDRESS_MASK_10BIT  (0x3FF)
#define I2C_10BIT_FIRST_BYTE    (0xF0)

// 7bit addressing, allowed values from 0x8 to 0x77
#define I2C_7BIT_ADDRESS_MIN    (0x08)
#define I2C_7BIT_ADDRESS_MAX    (0x77)
#define I2C_7BIT_ADDRESS        (0x55)

#define MAKE_7BIT_SLAVE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)
#define MAKE_7BIT_READ_ADDRESS(addr)    (((I2C_ADDRESS_MASK_7BIT & addr) << 1) | 1)
#define MAKE_7BIT_WRITE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)
#define MAKE_10BIT_READ_ADDRESS(addr)   ((((I2C_10BIT_FIRST_BYTE | (((addr & I2C_ADDRESS_MASK_10BIT)) >> 7)) | 1) << 8) | (addr & 0xFF))
#define MAKE_10BIT_WRITE_ADDRESS(addr)  ((((I2C_10BIT_FIRST_BYTE | (((addr & I2C_ADDRESS_MASK_10BIT)) >> 7)) & ~1) << 8) | (addr & 0xFF))

void test_pin_init();
void test_pin_toggle(int count = 1);

#if I2C_DEBUG_PIN_SLAVE
#define SLAVE_PIN_TOGGLE(n) test_pin_toggle(n)
#else
#define SLAVE_PIN_TOGGLE(n) (void)0
#endif

#if I2C_DEBUG_PIN_MASTER
#define MASTER_PIN_TOGGLE(n) test_pin_toggle(n)
#else
#define MASTER_PIN_TOGGLE(n) (void)0
#endif

#if I2C_DEBUG
#define I2C_DEBUG_PRINTF(...)   printf(__VA_ARGS__)
#else
#define I2C_DEBUG_PRINTF(...)   (void)0
#endif

bool test_check(bool condition, const char *condition_str, const char *file, int line);
bool test_check_message(bool condition, const char *condition_str, const char *message, const char *file, int line);
bool test_check_equal_int(int32_t expected, int32_t actual, const char *file, int line);
bool test_check_equal_uint(uint32_t expected, uint32_t actual, const char *file, int line);
bool test_check_uint_within(uint32_t min, uint32_t max, uint32_t actual, const char *file, int line);

#define TEST_CHECK(condition)                       test_check(condition, #condition, __FILE__, __LINE__)
#define TEST_CHECK_MESSAGE(condition, message)      test_check_message(condition, #condition, message, __FILE__, __LINE__)
#define TEST_CHECK_EQUAL_INT(expected, actual)      test_check_equal_int(expected, actual, __FILE__, __LINE__)
#define TEST_CHECK_EQUAL_UINT(expected, actual)     test_check_equal_uint(expected, actual, __FILE__, __LINE__)
#define TEST_CHECK_UINT_WITHIN(min, max, actual)    test_check_uint_within(min, max, actual, __FILE__, __LINE__)

template<uint32_t SS>
class WorkerThread : public Thread {
    unsigned char stack_mem[SS];
public:
    WorkerThread(osPriority priority = osPriorityNormal) : Thread(priority, SS, stack_mem) { }
};

#endif
