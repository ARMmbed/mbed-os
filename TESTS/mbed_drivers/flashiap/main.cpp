
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

#if !DEVICE_FLASH
    #error [NOT_SUPPORTED] Flash API not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include <algorithm>

#include "mbed.h"

using namespace utest::v1;

void flashiap_init_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);
    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(0, ret);
}

void flashiap_program_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);

    // get the last sector size (flash size - 1)
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    uint32_t page_size = flash_device.get_page_size();
    TEST_ASSERT_NOT_EQUAL(0, sector_size);
    TEST_ASSERT_NOT_EQUAL(0, page_size);
    TEST_ASSERT_TRUE(sector_size % page_size == 0);
    uint32_t prog_size = std::max(page_size, (uint32_t)8);
    uint8_t *data = new uint8_t[prog_size + 2];
    for (uint32_t i = 0; i < prog_size + 2; i++) {
        data[i] = i;
    }

    // the one before the last sector in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);
    ret = flash_device.erase(address, sector_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);


    for (uint32_t i = 0; i < sector_size / prog_size; i++) {
        uint32_t prog_addr = address + i * prog_size;
        ret = flash_device.program(data, prog_addr, prog_size);
        TEST_ASSERT_EQUAL_INT32(0, ret);
    }

    uint8_t *data_flashed = new uint8_t[prog_size];
    for (uint32_t i = 0; i < sector_size / prog_size; i++) {
        uint32_t page_addr = address + i * prog_size;
        ret = flash_device.read(data_flashed, page_addr, prog_size);
        TEST_ASSERT_EQUAL_INT32(0, ret);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, prog_size);
    }

    // check programming of unaligned buffer and size
    ret = flash_device.erase(address, sector_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    ret = flash_device.program(data + 2, address, prog_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    ret = flash_device.read(data_flashed, address, prog_size - 1);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data + 2, data_flashed, prog_size - 1);

    delete[] data;
    delete[] data_flashed;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(0, ret);
}

void flashiap_cross_sector_program_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);

    uint32_t page_size = flash_device.get_page_size();

    // Erase last two sectors
    uint32_t address = flash_device.get_flash_start() + flash_device.get_flash_size();
    uint32_t sector_size, agg_size = 0;
    for (uint32_t i = 0; i < 2; i++) {
        sector_size = flash_device.get_sector_size(address - 1UL);
        TEST_ASSERT_NOT_EQUAL(0, sector_size);
        TEST_ASSERT_TRUE(sector_size % page_size == 0);
        agg_size += sector_size;
        address -= sector_size;
    }
    ret = flash_device.erase(address, agg_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    address += sector_size - page_size;
    uint32_t aligned_prog_size = 2 * page_size;
    uint32_t prog_size = aligned_prog_size;
    if (page_size > 1) {
        prog_size--;
    }
    uint8_t *data = new uint8_t[aligned_prog_size];
    for (uint32_t i = 0; i < prog_size; i++) {
        data[i] = rand() % 256;
    }
    for (uint32_t i = prog_size; i < aligned_prog_size; i++) {
        data[i] = 0xFF;
    }

    ret = flash_device.program(data, address, prog_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    uint8_t *data_flashed = new uint8_t[aligned_prog_size];
    ret = flash_device.read(data_flashed, address, aligned_prog_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, aligned_prog_size);

    delete[] data;
    delete[] data_flashed;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(0, ret);
}

void flashiap_program_error_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);

    // get the last sector size (flash size - 1)
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    uint32_t page_size = flash_device.get_page_size();
    TEST_ASSERT_NOT_EQUAL(0, sector_size);
    TEST_ASSERT_NOT_EQUAL(0, page_size);
    TEST_ASSERT_TRUE(sector_size % page_size == 0);
    const uint8_t test_value = 0xCE;
    uint8_t *data = new uint8_t[page_size];
    for (uint32_t i = 0; i < page_size; i++) {
        data[i] = test_value;
    }

    // the one before the last page in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);

    // unaligned address
    ret = flash_device.erase(address + 1, sector_size);
    TEST_ASSERT_EQUAL_INT32(-1, ret);
    if (flash_device.get_page_size() > 1) {
        ret = flash_device.program(data, address + 1, page_size);
        TEST_ASSERT_EQUAL_INT32(-1, ret);
    }

    delete[] data;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(0, ret);
}

Case cases[] = {
    Case("FlashIAP - init", flashiap_init_test),
    Case("FlashIAP - program", flashiap_program_test),
    Case("FlashIAP - program across sectors", flashiap_cross_sector_program_test),
    Case("FlashIAP - program errors", flashiap_program_error_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
