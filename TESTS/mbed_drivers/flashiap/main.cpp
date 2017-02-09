
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

#include "mbed.h"

using namespace utest::v1;

void flashiap_init_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flashiap_program_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // get the last sector size (flash size - 1)
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    TEST_ASSERT_NOT_EQUAL(sector_size, 0);
    const uint8_t test_value = 0xCE;
    uint8_t *data = new uint8_t[sector_size];
    for (uint32_t i = 0; i < sector_size; i++) {
        data[i] = test_value;
    }

    // the one before the last page in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);
    ret = flash_device.erase(address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    ret = flash_device.program(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    uint8_t *data_flashed = new uint8_t[sector_size];
    ret = flash_device.read(data_flashed,  address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, sector_size);
    delete[] data;
    delete[] data_flashed;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flashiap_write_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // get the last sector size
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    TEST_ASSERT_NOT_EQUAL(sector_size, 0);
    const uint8_t test_value = 0xCE;
    uint8_t *data = new uint8_t[sector_size];
    for (uint32_t i = 0; i < sector_size; i++) {
        data[i] = test_value;
    }

    // the one before the last page in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);
    ret = flash_device.write(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    uint8_t *data_flashed = new uint8_t[sector_size];
    ret = flash_device.read(data_flashed,  address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, 0);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data, data_flashed, sector_size);
    delete[] data;
    delete[] data_flashed;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flashiap_program_error_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // get the last sector size (flash size - 1)
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    TEST_ASSERT_NOT_EQUAL(sector_size, 0);
    const uint8_t test_value = 0xCE;
    uint8_t *data = new uint8_t[sector_size];
    for (uint32_t i = 0; i < sector_size; i++) {
        data[i] = test_value;
    }

    // the one before the last page in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);

    // unaligned address
    address += 1;
    ret = flash_device.erase(address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);
    ret = flash_device.program(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);

    // unaligned sector size
    sector_size += 1;
    ret = flash_device.program(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);

    delete[] data;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

void flashiap_write_error_test()
{
    FlashIAP flash_device;
    uint32_t ret = flash_device.init();
    TEST_ASSERT_EQUAL_INT32(ret, 0);

    // get the last sector size
    uint32_t sector_size = flash_device.get_sector_size(flash_device.get_flash_start() + flash_device.get_flash_size() - 1UL);
    TEST_ASSERT_NOT_EQUAL(sector_size, 0);
    const uint8_t test_value = 0xCE;
    uint8_t *data = new uint8_t[sector_size];
    for (uint32_t i = 0; i < sector_size; i++) {
        data[i] = test_value;
    }

    // the one before the last page in the system
    uint32_t address = (flash_device.get_flash_start() + flash_device.get_flash_size()) - (sector_size);
    TEST_ASSERT_TRUE(address != 0UL);

    // unaligned address
    address += 1;
    ret = flash_device.write(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);

    // unaligned sector size
    address -= 1;
    sector_size += 1;
    ret = flash_device.write(data, address, sector_size);
    TEST_ASSERT_EQUAL_INT32(ret, -1);

    delete[] data;

    ret = flash_device.deinit();
    TEST_ASSERT_EQUAL_INT32(ret, 0);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("FlashIAP - init", flashiap_init_test, greentea_failure_handler),
    Case("FlashIAP - program", flashiap_program_test, greentea_failure_handler),
    Case("FlashIAP - write", flashiap_write_test, greentea_failure_handler),
    Case("FlashIAP - program errors", flashiap_program_error_test, greentea_failure_handler),
    Case("FlashIAP - write errors", flashiap_write_error_test, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
