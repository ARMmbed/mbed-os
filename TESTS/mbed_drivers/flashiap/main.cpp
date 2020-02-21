
/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if !DEVICE_FLASH
#error [NOT_SUPPORTED] Flash API not supported for this target
#else

#include "utest/utest.h"
#include "utest/utest_print.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "FlashIAP.h"
#include "unity.h"
#include <algorithm>

#include "mbed.h"

// Debug available
#ifndef FLASHIAP_DEBUG
#define FLASHIAP_DEBUG      0
#endif

#if FLASHIAP_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

using namespace utest::v1;


void flashiap_init_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);

    uint32_t flash_start = flash_device.get_flash_start();
    uint32_t flash_size = flash_device.get_flash_size();
    utest_printf("Flash address: 0x%08x, size: %d\n", flash_start, flash_size);
    uint32_t address = flash_start;
    int num = 0;
    while (flash_size) {
        uint32_t sector_size = flash_device.get_sector_size(address);
        // Make sure all sectors sum up to the total flash size
        TEST_ASSERT(flash_size >= sector_size);
        DEBUG_PRINTF("\tsector %3d: address 0x%08x, size %8d\n", num++, address, sector_size);
        flash_size -= sector_size;
        address += sector_size;
    }

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

    // the one before the last sector in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);
    utest_printf("ROM ends at 0x%lx, test starts at 0x%lx\n", FLASHIAP_APP_ROM_END_ADDR, address);
    TEST_SKIP_UNLESS_MESSAGE(address >= FLASHIAP_APP_ROM_END_ADDR, "Test skipped. Test region overlaps code.");

    ret = flash_device.erase(address, sector_size);
    TEST_ASSERT_EQUAL_INT32(0, ret);

    uint8_t erase_val = flash_device.get_erase_value();
    memset(data, erase_val, prog_size);

    uint8_t *data_flashed = new uint8_t[prog_size];
    for (uint32_t i = 0; i < sector_size / prog_size; i++) {
        uint32_t page_addr = address + i * prog_size;
        ret = flash_device.read(data_flashed, page_addr, prog_size);
        TEST_ASSERT_EQUAL_INT32(0, ret);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, prog_size);
    }

    for (uint32_t i = 0; i < prog_size + 2; i++) {
        data[i] = i;
    }

    for (uint32_t i = 0; i < sector_size / prog_size; i++) {
        uint32_t prog_addr = address + i * prog_size;
        ret = flash_device.program(data, prog_addr, prog_size);
        TEST_ASSERT_EQUAL_INT32(0, ret);
    }

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
    uint8_t erase_value = flash_device.get_erase_value();

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
    utest_printf("ROM ends at 0x%lx, test starts at 0x%lx\n", FLASHIAP_APP_ROM_END_ADDR, address);
    TEST_SKIP_UNLESS_MESSAGE(address >= FLASHIAP_APP_ROM_END_ADDR, "Test skipped. Test region overlaps code.");
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
        data[i] = erase_value;
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
    utest_printf("ROM ends at 0x%lx, test starts at 0x%lx\n", FLASHIAP_APP_ROM_END_ADDR, address);
    TEST_SKIP_UNLESS_MESSAGE(address >= FLASHIAP_APP_ROM_END_ADDR, "Test skipped. Test region overlaps code.");
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

void flashiap_timing_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(0, ret);
    mbed::Timer timer;
    unsigned int num_write_sizes;
    unsigned int curr_time, byte_usec_ratio;
    unsigned int avg_erase_time = 0;
    unsigned int max_erase_time = 0, min_erase_time = (unsigned int) -1;
    const unsigned int max_writes = 128;
    const unsigned int max_write_sizes = 6;
    const unsigned int max_byte_usec_ratio = 200;

    uint32_t page_size = flash_device.get_page_size();
    uint32_t write_size = page_size;

    uint32_t end_address = flash_device.get_flash_start() + flash_device.get_flash_size();

    utest_printf("\nFlash timing:\n");
    uint32_t sector_size = flash_device.get_sector_size(end_address - 1UL);
    uint32_t base_address = end_address - sector_size;
    utest_printf("ROM ends at 0x%lx, test starts at 0x%lx\n", FLASHIAP_APP_ROM_END_ADDR, base_address);
    TEST_SKIP_UNLESS_MESSAGE(base_address >= FLASHIAP_APP_ROM_END_ADDR, "Test skipped. Test region overlaps code.");

    timer.start();
    for (num_write_sizes = 0; num_write_sizes < max_write_sizes; num_write_sizes++) {
        if (write_size > sector_size) {
            break;
        }
        uint8_t *buf = new (std::nothrow) uint8_t[write_size];
        if (!buf) {
            // Don't fail the test on lack of heap memory for the buffer
            break;
        }
        memset(buf, 0x5A, write_size);
        timer.reset();
        ret = flash_device.erase(base_address, sector_size);
        curr_time = timer.read_us();
        avg_erase_time += curr_time;
        TEST_ASSERT_EQUAL_INT32(0, ret);
        max_erase_time = std::max(max_erase_time, curr_time);
        min_erase_time = std::min(min_erase_time, curr_time);
        uint32_t address = base_address;
        unsigned int avg_write_time = 0;
        unsigned int max_write_time = 0, min_write_time = (unsigned int) -1;
        unsigned int num_writes;
        for (num_writes = 0; num_writes < max_writes; num_writes++) {
            if ((address + write_size) > end_address) {
                break;
            }
            timer.reset();
            ret = flash_device.program(buf, address, write_size);
            curr_time = timer.read_us();
            avg_write_time += curr_time;
            TEST_ASSERT_EQUAL_INT32(0, ret);
            max_write_time = std::max(max_write_time, curr_time);
            min_write_time = std::min(min_write_time, curr_time);
            address += write_size;
        }
        delete[] buf;
        avg_write_time /= num_writes;
        utest_printf("Write size %6u bytes: avg %10u, min %10u, max %10u (usec)\n",
                     write_size, avg_write_time, min_write_time, max_write_time);
        byte_usec_ratio = write_size / avg_write_time;
        TEST_ASSERT(byte_usec_ratio < max_byte_usec_ratio);
        write_size *= 4;
    }

    if (num_write_sizes) {
        avg_erase_time /= num_write_sizes;
        utest_printf("\nErase size %6u bytes: avg %10u, min %10u, max %10u (usec)\n\n",
                     sector_size, avg_erase_time, min_erase_time, max_erase_time);
        byte_usec_ratio = sector_size / avg_erase_time;
        TEST_ASSERT(byte_usec_ratio < max_byte_usec_ratio);
    }

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(0, ret);
}


Case cases[] = {
    Case("FlashIAP - init", flashiap_init_test),
    Case("FlashIAP - program", flashiap_program_test),
    Case("FlashIAP - program across sectors", flashiap_cross_sector_program_test),
    Case("FlashIAP - program errors", flashiap_program_error_test),
    Case("FlashIAP - timing", flashiap_timing_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !DEVICE_FLASH
