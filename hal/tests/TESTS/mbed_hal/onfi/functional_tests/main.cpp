/* mbed Microcontroller Library
 * Copyright (c) 2022 ARM Limited
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

#if !DEVICE_ONFI
#error [NOT_SUPPORTED] Flash API not supported for this target
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "mbed.h"
#include "onfi_api.h"

using namespace utest::v1;

#define FLASH_PAGE_SIZE                     (2048)
#define ONFINAND_BLOCK_SIZE                 0x40000
#define DATA_0 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D0)
#define DATA_1 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D1)
#define DATA_2 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D2)
#define DATA_3 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D3)
#define DATA_4 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D4)
#define DATA_5 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D5)
#define DATA_6 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D6)
#define DATA_7 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_D7)
#define ADD_09 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_ADD09)
#define ADD_11 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_ADD11)
#define ADD_12 static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_ADD12)
#define BA_1   static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_BA1)
#define RDY    static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_RDY)
#define CSX    static_cast<PinName>(MBED_CONF_DRIVERS_ONFI_CSX)

uint8_t mem_writeBuffer[FLASH_PAGE_SIZE];
uint8_t mem_readBuffer[FLASH_PAGE_SIZE] = {0};

void flash_init_test()
{
    onfi_t test_onfi;
    int32_t ret = onfi_init(&test_onfi, DATA_0, DATA_1, DATA_2, DATA_3, DATA_4, DATA_5,
                            DATA_6, DATA_7, ADD_09, ADD_11, ADD_12, BA_1, RDY, CSX);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    ret = onfi_free(&test_onfi);
    TEST_ASSERT_EQUAL_INT32(0, ret);
}

void flash_erase_block_test()
{
    onfi_t test_onfi;
    int32_t ret = onfi_init(&test_onfi, DATA_0, DATA_1, DATA_2, DATA_3, DATA_4, DATA_5,
                            DATA_6, DATA_7, ADD_09, ADD_11, ADD_12, BA_1, RDY, CSX);
    uint32_t start_address = 0;
    TEST_ASSERT_EQUAL_INT32(0, ret);

    ret = onfi_erase_block(&test_onfi, start_address, ONFINAND_BLOCK_SIZE);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    ret = onfi_read(&test_onfi, start_address, mem_readBuffer, FLASH_PAGE_SIZE);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    for (uint32_t bytesIndex = 0; bytesIndex < FLASH_PAGE_SIZE; bytesIndex++) {
        if (mem_readBuffer[bytesIndex] != 0xFF) {
            utest_printf("\r\n***NAND Flash Erase block Check Failed!***\r\n");
        }
    }
}

void flash_write_read_test()
{
    onfi_t test_onfi;
    int32_t ret = onfi_init(&test_onfi, DATA_0, DATA_1, DATA_2, DATA_3, DATA_4, DATA_5,
                            DATA_6, DATA_7, ADD_09, ADD_11, ADD_12, BA_1, RDY, CSX);
    uint32_t start_address = 0;
    TEST_ASSERT_EQUAL_INT32(0, ret);

    ret = onfi_erase_block(&test_onfi, start_address, ONFINAND_BLOCK_SIZE);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    memset(mem_writeBuffer, 0xaa, sizeof(mem_writeBuffer));

    ret = onfi_write(&test_onfi, start_address, mem_writeBuffer, FLASH_PAGE_SIZE);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    ret = onfi_read(&test_onfi, start_address, mem_readBuffer, FLASH_PAGE_SIZE);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    if (memcmp(mem_writeBuffer, mem_readBuffer, FLASH_PAGE_SIZE) != 0) {
        utest_printf("\r\n***NAND Flash Read Failed!***\r\n");
    }
}

Case cases[] = {
    Case("Flash - init", flash_init_test),
    Case("Flash - erase block", flash_erase_block_test),
    Case("Flash - write read test", flash_write_read_test)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(200, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

void greentea_test_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown);

int main()
{
    Harness::run(specification);
}

#endif // !DEVICE_ONFI

