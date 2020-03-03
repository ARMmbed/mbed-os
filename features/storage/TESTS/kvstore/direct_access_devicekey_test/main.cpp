/* Copyright (c) 2018 Arm Limited
*
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

#ifndef COMPONENT_FLASHIAP
#error [NOT_SUPPORTED] Target must have internal FlashIAP for this test
#else

#include "mbed.h"
#include <stdio.h>
#include <string.h>
#include "DeviceKey.h"
#include "KVStore.h"
#include "KVMap.h"
#include "kv_config.h"
#include "TDBStore.h"
#include "FlashIAP.h"
#include "FlashIAPBlockDevice.h"
#include "DirectAccessDevicekey.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>

using namespace utest::v1;
using namespace mbed;

#define TEST_DEVICEKEY_LENGTH 32

static inline uint32_t align_up(uint64_t val, uint64_t size)
{
    return (((val - 1) / size) + 1) * size;
}

static inline uint32_t align_down(uint64_t val, uint64_t size)
{
    return (((val) / size)) * size;
}

int  get_virtual_TDBStore_position(uint32_t conf_start_address, uint32_t conf_size,
                                   uint32_t *tdb_start_address, uint32_t *tdb_end_address)
{
    uint32_t bd_final_size = conf_size;
    uint32_t flash_end_address;
    uint32_t flash_start_address;
    uint32_t aligned_start_address;
    FlashIAP flash;

    int ret = flash.init();
    if (ret != 0) {
        return -1;
    }

    uint32_t flash_first_writable_sector_address = (uint32_t)(align_up(FLASHIAP_APP_ROM_END_ADDR,
                                                                       flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR)));

    //Get flash parameters before starting
    flash_start_address = flash.get_flash_start();
    flash_end_address = flash_start_address + flash.get_flash_size();

    bool request_default = false;
    if (conf_start_address == 0 && conf_size == 0) {
        request_default = true;
    }

    if (!request_default) {
        if (conf_start_address == 0) {
            conf_start_address = flash_end_address - conf_size;
        }
        aligned_start_address = align_down(conf_start_address, flash.get_sector_size(conf_start_address));
        if (conf_size == 0) {
            bd_final_size = (flash_end_address - aligned_start_address);
        }

        if ((conf_size == 0) || (conf_start_address == 0)) {
            if (aligned_start_address < flash_first_writable_sector_address) {
                flash.deinit();
                return -2;
            }
        }
    } else {
        bd_addr_t default_start;
        bd_size_t default_size;
        kv_get_default_flash_addresses(&default_start, &default_size);
        aligned_start_address = (uint32_t)default_start;
        bd_final_size = (uint32_t)default_size;
    }

    (*tdb_start_address) = aligned_start_address;
    (*tdb_end_address) = aligned_start_address + bd_final_size;

    flash.deinit();

    return 0;
}


void test_direct_access_to_devicekey_tdb_flashiap_default()
{
    utest_printf("Test Direct Access To DeviceKey Test Entire FlashIAP Default Address\n");

    uint32_t flash_bd_start_address;
    uint32_t flash_bd_end_address;

    int err = get_virtual_TDBStore_position(MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS, MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE, &flash_bd_start_address, &flash_bd_end_address);
    TEST_SKIP_UNLESS_MESSAGE(err != -2, "Test skipped. Not enough available space on Internal FlashIAP");
    TEST_ASSERT_EQUAL(0, err);
    uint32_t flash_bd_size = flash_bd_end_address - flash_bd_start_address;

    FlashIAPBlockDevice *flash_bd = new FlashIAPBlockDevice((bd_addr_t)flash_bd_start_address, (bd_size_t)flash_bd_size);
    TEST_ASSERT_NOT_EQUAL(NULL, flash_bd);

    TDBStore *tdb = new TDBStore(flash_bd);
    TEST_ASSERT_NOT_EQUAL(NULL, tdb);
    // Start by Init and Reset to TDBStore
    err = tdb->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = tdb->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    // Assign a dummy DeviceKey, and set via tdb
    uint8_t device_key_in[TEST_DEVICEKEY_LENGTH] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    err = tdb->reserved_data_set(device_key_in, TEST_DEVICEKEY_LENGTH);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);


    err = tdb->deinit();
    TEST_ASSERT_EQUAL(0, err);

    delete tdb;
    delete flash_bd;

    // Now use Direct Access To DeviceKey to retrieve it
    uint8_t device_key_out[TEST_DEVICEKEY_LENGTH] = {0};
    size_t actual_data_size = 0;

    err = direct_access_to_devicekey((uint32_t)flash_bd_start_address, (uint32_t)flash_bd_end_address, device_key_out,
                                     TEST_DEVICEKEY_LENGTH,
                                     &actual_data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    // Assert DeviceKey value and length
    TEST_ASSERT_EQUAL(actual_data_size, TEST_DEVICEKEY_LENGTH);
    for (int i_ind = 0; i_ind < TEST_DEVICEKEY_LENGTH; i_ind++) {
        TEST_ASSERT_EQUAL(device_key_in[i_ind], device_key_out[i_ind]);
    }

}

void test_direct_access_to_devicekey_tdb_last_two_sectors()
{
    utest_printf("Test Direct Access To DeviceKey Test Last Two Sectors\n");

    uint32_t flash_bd_start_address;
    uint32_t flash_bd_end_address;

    int err = get_virtual_TDBStore_position(0, 0, &flash_bd_start_address, &flash_bd_end_address);
    TEST_SKIP_UNLESS_MESSAGE(err != -2, "Test skipped. Not enough available space on Internal FlashIAP");
    TEST_ASSERT_EQUAL(0, err);

    uint32_t flash_bd_size = flash_bd_end_address - flash_bd_start_address;

    FlashIAPBlockDevice *flash_bd = new FlashIAPBlockDevice((bd_addr_t)flash_bd_start_address, (bd_size_t)flash_bd_size);
    flash_bd->init();
    uint32_t sector_addr = flash_bd->size();
    for (int i = 0; i < 2; i++) {
        uint32_t sector_size = flash_bd->get_erase_size(sector_addr - 1);
        uint32_t erase_prog_ratio = sector_size / flash_bd->get_program_size();
        if (erase_prog_ratio < 4) {
            delete flash_bd;
            TEST_SKIP_UNLESS_MESSAGE(false, "Test skipped. Flash program size doesn't support this test.");
        }
        sector_addr -= sector_size;
    }
    flash_bd->deinit();

    TDBStore *tdb = new TDBStore(flash_bd);
    // Start by Init and Reset to TDBStore
    err = tdb->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = tdb->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    // Assign a dummy DeviceKey, and set via tdb
    uint8_t device_key_in[TEST_DEVICEKEY_LENGTH] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    err = tdb->reserved_data_set(device_key_in, TEST_DEVICEKEY_LENGTH);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = tdb->deinit();
    TEST_ASSERT_EQUAL(0, err);

    delete tdb;
    delete flash_bd;

    // Now use Direct Access To DeviceKey to retrieve it
    uint8_t device_key_out[TEST_DEVICEKEY_LENGTH] = {0};
    size_t actual_data_size = 0;

    err = direct_access_to_devicekey((uint32_t)flash_bd_start_address, (uint32_t)flash_bd_end_address, device_key_out,
                                     TEST_DEVICEKEY_LENGTH,
                                     &actual_data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    // Assert DeviceKey value and length
    TEST_ASSERT_EQUAL(TEST_DEVICEKEY_LENGTH, actual_data_size);
    for (int i_ind = 0; i_ind < TEST_DEVICEKEY_LENGTH; i_ind++) {
        TEST_ASSERT_EQUAL(device_key_in[i_ind], device_key_out[i_ind]);
    }

}
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

void test_direct_access_to_device_inject_root()
{
    kv_init_storage_config();
    DeviceKey &devkey = DeviceKey::get_instance();
    uint32_t rkey[DEVICE_KEY_16BYTE / sizeof(uint32_t)];
    uint32_t key[DEVICE_KEY_16BYTE / sizeof(uint32_t)];
    KVMap &kv_map = KVMap::get_instance();
    KVStore *inner_store = kv_map.get_internal_kv_instance(NULL);
    TEST_SKIP_UNLESS_MESSAGE(inner_store != NULL, "Test skipped. No KVStore Internal");

    BlockDevice *flash_bd = kv_map.get_internal_blockdevice_instance("");
    TEST_ASSERT_NOT_EQUAL(NULL, flash_bd);

    int ret = inner_store->reset();
    TEST_ASSERT_EQUAL_INT(DEVICEKEY_SUCCESS, ret);

    memcpy(key, "abcdefghabcdefgh", sizeof(key));
    ret = devkey.device_inject_root_of_trust(key, DEVICE_KEY_16BYTE);
    TEST_ASSERT_EQUAL_INT(DEVICEKEY_SUCCESS, ret);

    // Now use Direct Access To DeviceKey to retrieve it
    uint32_t internal_start_address;
    uint32_t internal_rbp_size;
    if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "FILESYSTEM") == 0) {
        internal_start_address =  MBED_CONF_STORAGE_FILESYSTEM_INTERNAL_BASE_ADDRESS;
        internal_rbp_size =  MBED_CONF_STORAGE_FILESYSTEM_RBP_INTERNAL_SIZE;
    } else if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "TDB_EXTERNAL") == 0) {
        internal_start_address =  MBED_CONF_STORAGE_TDB_EXTERNAL_INTERNAL_BASE_ADDRESS;
        internal_rbp_size =  MBED_CONF_STORAGE_TDB_EXTERNAL_RBP_INTERNAL_SIZE;
    } else if (strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "TDB_INTERNAL") == 0 ||
               strcmp(STR(MBED_CONF_STORAGE_STORAGE_TYPE), "default") == 0) {
        internal_start_address =  MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_BASE_ADDRESS;
        internal_rbp_size =  MBED_CONF_STORAGE_TDB_INTERNAL_INTERNAL_SIZE;
    } else {
        TEST_SKIP_UNLESS_MESSAGE(false, "Test skipped. No KVStore Internal");
    }

    uint32_t tdb_st_add = 0;
    uint32_t tdb_end_add = 0;
    ret = get_virtual_TDBStore_position(internal_start_address, internal_rbp_size, &tdb_st_add, &tdb_end_add);
    TEST_SKIP_UNLESS_MESSAGE(ret != -2, "Test skipped. Not enough available space on Internal FlashIAP");
    TEST_ASSERT_EQUAL(0, ret);

    uint32_t expected_tdb_st_add = 0;
    uint32_t expected_tdb_end_add = 0;
    ret = get_expected_internal_TDBStore_position(&expected_tdb_st_add, &expected_tdb_end_add);
    if (ret ==  MBED_SUCCESS) {
        TEST_ASSERT_EQUAL(expected_tdb_st_add, tdb_st_add);
        TEST_ASSERT_EQUAL(expected_tdb_end_add, tdb_end_add);
    }

    memset(rkey, 0, sizeof(rkey));
    size_t actual_data_size = 0;
    ret = ((TDBStore *)inner_store)->reserved_data_get(rkey, DEVICE_KEY_16BYTE, &actual_data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, ret);
    TEST_ASSERT_EQUAL(actual_data_size, DEVICE_KEY_16BYTE);
    TEST_ASSERT_EQUAL_INT32_ARRAY(key, rkey, DEVICE_KEY_16BYTE / sizeof(uint32_t));

    memset(rkey, 0, sizeof(rkey));
    actual_data_size = 0;

    ret = direct_access_to_devicekey(tdb_st_add, tdb_end_add, rkey, DEVICE_KEY_16BYTE, &actual_data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, ret);
    /* Assert DeviceKey value and length */
    TEST_ASSERT_EQUAL(actual_data_size, DEVICE_KEY_16BYTE);
    TEST_ASSERT_EQUAL_INT32_ARRAY(key, rkey, DEVICE_KEY_16BYTE / sizeof(uint32_t));

    ret = inner_store->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, ret);
}

// Test setup
utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("Testing direct access to devicekey with tdb over flashiap default placement", test_direct_access_to_devicekey_tdb_flashiap_default, greentea_failure_handler),
    Case("Testing direct access to devicekey with tdb over last two sectors", test_direct_access_to_devicekey_tdb_last_two_sectors, greentea_failure_handler),
    Case("Testing direct access to injected devicekey ", test_direct_access_to_device_inject_root, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(120, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}

#endif // COMPONENT_FLASHIAP
