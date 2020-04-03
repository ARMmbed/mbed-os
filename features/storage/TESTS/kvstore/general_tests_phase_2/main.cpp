/* Copyright (c) 2017 ARM Limited
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
#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Kvstore API test cases require a RTOS to run
#else

#include "SecureStore.h"
#include "TDBStore.h"
#include "Thread.h"
#include "mbed_error.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "FileSystemStore.h"
#include "DeviceKey.h"

using namespace utest::v1;
using namespace mbed;

#if !SECURESTORE_ENABLED
#error [NOT_SUPPORTED] SecureStore need to be enabled for this test
#else

static const char   data[] = "data";
static const char   key[] = "key";
static char         buffer[20] = {};
static const size_t data_size = 5;
static size_t       actual_size = 0;
static const size_t buffer_size = 20;
static const char   num_of_keys = 3;

static const char *keys[] = {"key1", "key2", "key3"};

KVStore::info_t info;
KVStore::iterator_t kvstore_it;

KVStore *kvstore = NULL;
FileSystem *fs = NULL;
BlockDevice *bd = NULL;
FlashSimBlockDevice *flash_bd = NULL;
SlicingBlockDevice *ul_bd = NULL, *rbp_bd = NULL;

enum kv_setup {
    TDBStoreSet = 0,
    FSStoreSet,
    SecStoreSet,
    NumKVs
};

static const char *kv_prefix[] = {"TDB_", "FS_", "SEC_"};

static int kv_setup = TDBStoreSet;

static const int heap_alloc_threshold_size = 4096;

static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

/*----------------initialization------------------*/

//init the blockdevice
static void kvstore_init()
{
    // This directly corresponds to the pages allocated for each of the SecureStore block devices
    // For the others it may not match exactly to the space that is used, but it is expected to
    // be a close enough approximation to act as a guideline for how much of the block device we
    // need to erase in order to ensure a stable initial condition.
    const size_t PAGES_ESTIMATE = 40;

    int res;
    size_t program_size, erase_size, ul_bd_size, rbp_bd_size;
    BlockDevice *sec_bd;

    res = bd->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, res);
    int erase_val = bd->get_erase_value();
    // Clear out any stale data that might be left from a previous test
    // Multiply by 2 because SecureStore requires two underlying block devices of this size
    size_t bytes_to_erase = align_up(2 * PAGES_ESTIMATE * bd->get_program_size(), bd->get_erase_size());

    bd->erase(0, bytes_to_erase);
    res = bd->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, res);

    if (kv_setup == TDBStoreSet) {
        if (erase_val == -1) {
            flash_bd = new FlashSimBlockDevice(bd);
            kvstore = new TDBStore(flash_bd);
        } else {
            kvstore = new TDBStore(bd);
        }
    }
    if (kv_setup == FSStoreSet) {
        fs = FileSystem::get_default_instance();
        TEST_SKIP_UNLESS(fs != NULL);
        res = fs->mount(bd);
        if (res) {
            res = fs->reformat(bd);
            TEST_ASSERT_EQUAL_ERROR_CODE(0, res);
        }
        kvstore = new FileSystemStore(fs);
    }

#if SECURESTORE_ENABLED
    if (kv_setup == SecStoreSet) {
        sec_bd = bd;
        if (erase_val == -1) {
            flash_bd = new FlashSimBlockDevice(bd);
            sec_bd = flash_bd;
        }
        res = sec_bd->init();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

        program_size  = sec_bd->get_program_size();
        erase_size = sec_bd->get_erase_size();
        // We must be able to hold at least 10 small keys (20 program sectors) and master record + internal data
        // but minimum of 2 erase sectors, so that the garbage collection way work
        ul_bd_size  = align_up(program_size * PAGES_ESTIMATE, erase_size * 2);
        rbp_bd_size = align_up(program_size * PAGES_ESTIMATE, erase_size * 2);

        res = sec_bd->deinit();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

        ul_bd = new SlicingBlockDevice(sec_bd, 0, ul_bd_size);
        rbp_bd = new SlicingBlockDevice(sec_bd, ul_bd_size, ul_bd_size + rbp_bd_size);

        TDBStore *ul_kv = new TDBStore(ul_bd);
        TDBStore *rbp_kv = new TDBStore(rbp_bd);
        kvstore = new SecureStore(ul_kv, rbp_kv);
    }
#endif

    TEST_SKIP_UNLESS(kvstore != NULL);

    res = kvstore->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
#if DEVICEKEY_ENABLED
    DeviceKey::get_instance().generate_root_of_trust();
#endif
}

//deinit the blockdevice
static void kvstore_deinit()
{
    int res = 0;

    TEST_SKIP_UNLESS(kvstore != NULL);

    int erase_val = bd->get_erase_value();

    res = kvstore->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    if (kv_setup == TDBStoreSet) {
        if (erase_val == -1) {
            delete flash_bd;
        }
    }
    if (kv_setup == FSStoreSet) {
        fs = FileSystem::get_default_instance();
        TEST_SKIP_UNLESS(fs != NULL);
        res = fs->unmount();
        TEST_ASSERT_EQUAL_ERROR_CODE(0, res);
    }
    if (kv_setup == SecStoreSet) {
        if (erase_val == -1) {
            delete flash_bd;
        }
        delete ul_bd;
        delete rbp_bd;
    }

    delete kvstore;
    kvstore = NULL;

    kv_setup++;
}

/*----------------get_info()------------------*/

//bad params : key is null
static void get_info_key_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->get_info(NULL, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void get_info_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char key_max[KVStore::MAX_KEY_SIZE + 1] = {0};
    memset(key_max, '*', KVStore::MAX_KEY_SIZE);
    int res = kvstore->get_info(key_max, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//get_info of non existing key
static void get_info_non_existing_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char new_key[] = "get_info_key";
    int res = kvstore->get_info(new_key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get_info of removed key
static void get_info_removed_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get_info of existing key - valid flow
static void get_info_existed_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_ERROR_CODE(info.flags, KVStore::WRITE_ONCE_FLAG);

    res = kvstore->get_info(key, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get_info(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get_info of overwritten key
static void get_info_overwritten_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_data[] = "new_data";
    res = kvstore->set(key, new_data, sizeof(new_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_ERROR_CODE(info.size, sizeof(new_data));

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------iterator_open()------------------*/

//bad params : it is null
static void iterator_open_it_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->iterator_open(NULL, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

/*----------------iterator_next()------------------*/

//key valid, key_size 0
static void iterator_next_key_size_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with empty list
static void iterator_next_empty_list()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key, sizeof(key));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iterator_next for one key list
static void iterator_next_one_key_list()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key_buf[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with empty list (all keys removed)
static void iterator_next_empty_list_keys_removed()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char new_key_1[] = "it_1";
    int res = kvstore->set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "it_2";
    res = kvstore->set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(new_key_1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(new_key_2);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key_buf[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with non matching prefix (empty list)
static void iterator_next_empty_list_non_matching_prefix()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char new_key_1[] = "it_1";
    int res = kvstore->set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "it_2";
    res = kvstore->set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_open(&kvstore_it, "Key*");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key_buf[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(new_key_1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(new_key_2);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with several overwritten keys
static void iterator_next_several_overwritten_keys()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    for (int i = 0; i < num_of_keys; i++) {
        int res = kvstore->set(key, data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    int res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key[KVStore::MAX_KEY_SIZE];

    res = kvstore->iterator_next(kvstore_it, key, sizeof(key));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_next(kvstore_it, key, sizeof(key));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iterator_next for full list - check key names for validation
static void iterator_next_full_list()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int i = 0;
    bool *key_found = new bool[num_of_keys];
    for (i = 0; i < num_of_keys; i++) {
        int res = kvstore->set(keys[i], data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        key_found[i] = false;
    }

    int res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char temp_key[KVStore::MAX_KEY_SIZE];

    for (i = 0; i < num_of_keys; i++) {
        res = kvstore->iterator_next(kvstore_it, temp_key, sizeof(temp_key));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

        res = kvstore->get(temp_key, buffer, buffer_size, &actual_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        int j;
        for (j = 0; j < num_of_keys; j++) {
            if (!key_found[j] && (!strcmp(keys[j], temp_key))) {
                key_found[j] = true;
                break;
            }
        }
        TEST_ASSERT_NOT_EQUAL(j, num_of_keys);
    }

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    for (i = 0; i < num_of_keys; i++) {
        int res = kvstore->remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
}

//iteartor_next remove while iterating
static void iterator_next_remove_while_iterating()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int i = 0, res = 0;

    for (i = 0; i < num_of_keys; i++) {
        int res = kvstore->set(keys[i], data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    char new_key_1[] = "new_key_1";
    res = kvstore->set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "new_key_2";
    res = kvstore->set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_open(&kvstore_it, "key");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key[KVStore::MAX_KEY_SIZE];

    while (1) {
        res = kvstore->iterator_next(kvstore_it, key, sizeof(key));
        if (res != MBED_SUCCESS) {
            break;
        }
        res = kvstore->remove(key);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    while (1) {
        res = kvstore->iterator_next(kvstore_it, key, sizeof(key));
        if (res != MBED_SUCCESS) {
            break;
        }
        TEST_ASSERT_EQUAL_STRING_LEN("new", key, 3);

        res = kvstore->remove(key);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    res = kvstore->iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------iterator_close()------------------*/

//iterator_close right after iterator_open
static void iterator_close_right_after_iterator_open()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->iterator_close(kvstore_it);
}

/*----------------set_start()------------------*/

//bad params : key is null
static void set_start_key_is_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    int res = kvstore->set_start(&handle, NULL, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key_size over max_size
static void set_start_key_size_exceeds_max_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    char key_max[KVStore::MAX_KEY_SIZE + 1] = {0};
    memset(key_max, '*', KVStore::MAX_KEY_SIZE);
    int res = kvstore->set_start(&handle, key_max, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//final_data_size is 0
static void set_start_final_data_size_is_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    int res = kvstore->set_start(&handle, key, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//final_data_size is smaller than actual data
static void set_start_final_data_size_is_smaller_than_real_data()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    size_t new_data_size = 20;
    int res = kvstore->set_start(&handle, key, new_data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, data, data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_SIZE, res);
}

//final_data_size is smaller than actual data
static void set_start_final_data_size_is_bigger_than_real_data()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    char new_data[] = "new_data_buffer";
    int res = kvstore->set_start(&handle, key, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, new_data, sizeof(new_data));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_SIZE, res);
}

/*----------------set_add_data()------------------*/

//bad params : value_data is null
static void set_add_data_value_data_is_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    int res = kvstore->set_start(&handle, key, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, NULL, sizeof(data_size));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//bad params : value_data is valid, data_size is 0
static void set_add_data_data_size_is_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    int res = kvstore->set_start(&handle, key, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, NULL, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//data_size is bigger than actual data
static void set_add_data_data_size_bigger_than_real_data()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    size_t new_data_size = 20;
    int res = kvstore->set_start(&handle, key, new_data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, data, new_data_size - 1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, data, data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_SIZE, res);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set different data_sizes chunks of data in the same transaction
static void set_add_data_set_different_data_size_in_same_transaction()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    char new_data[] = "new_data_tests";
    size_t new_data_size = 15;

    int res = kvstore->set_start(&handle, key, new_data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, new_data, new_data_size - 5);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, new_data, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_add_data(handle, new_data + (new_data_size - 5), 5);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    TEST_ASSERT_EQUAL_STRING(new_data, buffer);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value five Kbyte size
static void set_add_data_set_key_value_five_Kbytes()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    size_t new_data_size = 5000;
    char temp_buf[50] = {};
    char read_temp_buf[50] = {};
    unsigned int i = 0;

    int res = kvstore->set_start(&handle, key, new_data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    for (i = 0; i < (new_data_size / sizeof(temp_buf)); i++) {
        memset(temp_buf, '*', sizeof(temp_buf));
        res = kvstore->set_add_data(handle, temp_buf, sizeof(temp_buf));
    }

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, read_temp_buf, sizeof(read_temp_buf), &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    TEST_ASSERT_EQUAL_STRING_LEN(temp_buf, read_temp_buf, sizeof(temp_buf));

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set_add_data without set_start
static void set_add_data_without_set_start()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle = reinterpret_cast<KVStore::set_handle_t>(0);

    int res = kvstore->set_add_data(handle, data, data_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

/*----------------set_finalize()------------------*/

//set_finalize without set_start
static void set_finalize_without_set_start()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle = reinterpret_cast<KVStore::set_handle_t>(0);

    int res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

static void set_finalize_right_after_set_start()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    KVStore::set_handle_t handle;

    int res = kvstore->set_start(&handle, key, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------setup------------------*/

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

typedef struct {
    const char *description;
    const case_handler_t case_handler;
    const case_failure_handler_t failure_handler;
} template_case_t;

template_case_t template_cases[] = {

    {"kvstore_init", kvstore_init, greentea_failure_handler}, //must be first

    {"get_info_key_null", get_info_key_null, greentea_failure_handler},
    {"get_info_key_length_exceeds_max", get_info_key_length_exceeds_max, greentea_failure_handler},
    {"get_info_non_existing_key", get_info_non_existing_key, greentea_failure_handler},
    {"get_info_removed_key", get_info_removed_key, greentea_failure_handler},
    {"get_info_existed_key", get_info_existed_key, greentea_failure_handler},
    {"get_info_overwritten_key", get_info_overwritten_key, greentea_failure_handler},

    {"iterator_open_it_null", iterator_open_it_null, greentea_failure_handler},

    {"iterator_next_key_size_zero", iterator_next_key_size_zero, greentea_failure_handler},
    {"iterator_next_empty_list", iterator_next_empty_list, greentea_failure_handler},
    {"iterator_next_one_key_list", iterator_next_one_key_list, greentea_failure_handler},
    {"iterator_next_empty_list_keys_removed", iterator_next_empty_list_keys_removed, greentea_failure_handler},
    {"iterator_next_empty_list_non_matching_prefix", iterator_next_empty_list_non_matching_prefix, greentea_failure_handler},
    {"iterator_next_several_overwritten_keys", iterator_next_several_overwritten_keys, greentea_failure_handler},
    {"iterator_next_full_list", iterator_next_full_list, greentea_failure_handler},
    {"iterator_next_remove_while_iterating", iterator_next_remove_while_iterating, greentea_failure_handler},

    {"iterator_close_right_after_iterator_open", iterator_close_right_after_iterator_open, greentea_failure_handler},

    {"set_start_key_is_null", set_start_key_is_null, greentea_failure_handler},
    {"set_start_key_size_exceeds_max_size", set_start_key_size_exceeds_max_size, greentea_failure_handler},
    {"set_start_final_data_size_is_zero", set_start_final_data_size_is_zero, greentea_failure_handler},
    {"set_start_final_data_size_is_smaller_than_real_data", set_start_final_data_size_is_smaller_than_real_data, greentea_failure_handler},
    {"set_start_final_data_size_is_bigger_than_real_data", set_start_final_data_size_is_bigger_than_real_data, greentea_failure_handler},

    {"set_add_data_value_data_is_null", set_add_data_value_data_is_null, greentea_failure_handler},
    {"set_add_data_data_size_is_zero", set_add_data_data_size_is_zero, greentea_failure_handler},
    {"set_add_data_data_size_bigger_than_real_data", set_add_data_data_size_bigger_than_real_data, greentea_failure_handler},
    {"set_add_data_set_different_data_size_in_same_transaction", set_add_data_set_different_data_size_in_same_transaction, greentea_failure_handler},
    {"set_add_data_set_key_value_five_Kbytes", set_add_data_set_key_value_five_Kbytes, greentea_failure_handler},
    {"set_add_data_without_set_start", set_add_data_without_set_start, greentea_failure_handler},

    {"set_finalize_without_set_start", set_finalize_without_set_start, greentea_failure_handler},
    {"set_finalize_right_after_set_start", set_finalize_right_after_set_start, greentea_failure_handler},

    {"kvstore_deinit", kvstore_deinit, greentea_failure_handler},
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    return greentea_test_setup_handler(number_of_cases);
}

int main()
{
    GREENTEA_SETUP(3000, "default_auto");

    // Don't even start if conditions aren't appropriate for test run
    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    if (!dummy) {
        printf("Not enough heap memory to run test. Test skipped.\n");
        GREENTEA_TESTSUITE_RESULT(1);
        return 0;
    }
    delete[] dummy;

    bd = BlockDevice::get_default_instance();
    if (!bd) {
        printf("No default instance for this target. Test skipped.\n");
        GREENTEA_TESTSUITE_RESULT(1);
        return 0;
    }

    // We want to replicate our test cases to different KV types
    size_t num_cases = sizeof(template_cases) / sizeof(template_case_t);
    size_t total_num_cases = 0;

    void *raw_mem = new (std::nothrow) uint8_t[NumKVs * num_cases * sizeof(Case)];
    Case *cases = static_cast<Case *>(raw_mem);

    for (int kv = 0; kv < NumKVs; kv++) {
        for (size_t i = 0; i < num_cases; i++) {
            char desc[128], *desc_ptr;
            sprintf(desc, "%s%s", kv_prefix[kv], template_cases[i].description);
            desc_ptr = new char[strlen(desc) + 1];
            strcpy(desc_ptr, desc);
            new (&cases[total_num_cases]) Case((const char *) desc_ptr, template_cases[i].case_handler,
                                               template_cases[i].failure_handler);
            total_num_cases++;
        }
    }

    Specification specification(greentea_test_setup, cases, total_num_cases,
                                greentea_test_teardown_handler, default_handler);

    return !Harness::run(specification);
}

#endif //!SECURESTORE_ENABLED
#endif // !defined(MBED_CONF_RTOS_PRESENT)
