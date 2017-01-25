/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#if !DEVICE_FLASH
    #error [NOT_SUPPORTED] Flash API not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "mbed.h"
#include "flash_api.h"
#include "flash_data.h"

using namespace utest::v1;

/*
    return values to be checked are documented at:
        http://arm-software.github.io/CMSIS_5/Pack/html/algorithmFunc.html#Verify
*/

#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x)& ~((a) - 1))
#endif

void flash_init_test()
{
    flash_t test_flash;
    int32_t ret = flash_init(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    ret = flash_free(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

// Erase sector, write one page, erase sector and write new data
void flash_program_page_test()
{
    flash_t test_flash;
    int32_t ret = flash_init(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    uint32_t test_size = flash_get_page_size(&test_flash);
    uint8_t *data = new uint8_t[test_size];
    for (uint32_t i = 0; i < test_size; i++) {
        data[i] = 0xCE;
    }

    // the one before the last page in the system
    uint32_t address = (test_flash.target_config->flash_start + test_flash.target_config->flash_size) - (2*test_size);

    // sector size might not be same as page size
    uint32_t erase_sector_boundary = ALIGN_DOWN(address, flash_get_sector_size(&test_flash, address));
    ret = flash_erase_sector(&test_flash, erase_sector_boundary);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    ret = flash_program_page(&test_flash, address, data, test_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    uint8_t *data_flashed = (uint8_t *)address;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, test_size);

    // sector size might not be same as page size
    erase_sector_boundary = ALIGN_DOWN(address, flash_get_sector_size(&test_flash, address));
    ret = flash_erase_sector(&test_flash, erase_sector_boundary);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // write another data to be certain we are re-flashing
    for (uint32_t i = 0; i < test_size; i++) {
        data[i] = 0xAC;
    }
    ret = flash_program_page(&test_flash, address, data, test_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, test_size);

    ret = flash_free(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    delete[] data;
}

void flash_erase_sector_test()
{
    flash_t test_flash;
    int32_t ret = flash_init(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    uint32_t sector_size = 0x1000;
    uint32_t address = (test_flash.target_config->flash_start + test_flash.target_config->flash_size) - (4*sector_size);
    // sector size might not be same as page size
    uint32_t erase_sector_boundary = ALIGN_DOWN(address, flash_get_sector_size(&test_flash, address));
    ret = flash_erase_sector(&test_flash, erase_sector_boundary);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    ret = flash_free(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flash_erase_sector_error_test()
{
    flash_t test_flash;
    int32_t ret = flash_init(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // most common sector size to get an sector address 
    uint32_t sector_size = 0x1000;
    uint32_t address = (test_flash.target_config->flash_start + test_flash.target_config->flash_size) - (4*sector_size);
    uint32_t erase_sector_boundary = ALIGN_DOWN(address, flash_get_sector_size(&test_flash, address));

    // unaligned address
    erase_sector_boundary += 1;
    ret = flash_erase_sector(&test_flash, erase_sector_boundary);
    TEST_ASSERT_EQUAL_INT32(ret, -1);

    ret = flash_free(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flash_program_page_error_test()
{
    flash_t test_flash;
    int32_t ret = flash_init(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);


    uint32_t test_size = flash_get_page_size(&test_flash);
    // the one before the last page in the system
    uint32_t address = (test_flash.target_config->flash_start + test_flash.target_config->flash_size) - (2*test_size);

    // sector size might not be same as page size
    uint32_t erase_sector_boundary = ALIGN_DOWN(address, flash_get_sector_size(&test_flash, address));
    ret = flash_erase_sector(&test_flash, erase_sector_boundary);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // we store the current data, and verify later they have not changed
    uint8_t *data = new uint8_t[test_size];
    uint8_t *previous_data = new uint8_t[test_size];
    uint8_t *current_data = (uint8_t *)address;
    for (uint32_t i = 0; i < test_size; i++) {
        previous_data[i] = *current_data;
        data[i] = 0xCE;
        current_data++;
    }

    address += 1UL;
    ret = flash_program_page(&test_flash, address, data, test_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(previous_data, current_data, test_size);

    ret = flash_free(&test_flash);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    delete[] data;
    delete[] previous_data;
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Flash - init", flash_init_test, greentea_failure_handler),
    Case("Flash - erase sector", flash_erase_sector_test, greentea_failure_handler),
    Case("Flash - program page", flash_program_page_test, greentea_failure_handler),
    Case("Flash - erase sector errors", flash_erase_sector_error_test, greentea_failure_handler),
    Case("Flash - program page errors", flash_program_page_error_test, greentea_failure_handler),

};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
