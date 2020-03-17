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
static const int    num_of_threads = 3;

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
    // Clear out any stale data that might be left from a previous test.
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
        TEST_ASSERT((ul_bd_size + rbp_bd_size) < sec_bd->size());

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

/*----------------set()------------------*/

//bad params : key is null
static void set_key_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(NULL, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void set_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char key_max[KVStore::MAX_KEY_SIZE + 1] = {0};
    memset(key_max, '*', KVStore::MAX_KEY_SIZE);
    int res = kvstore->set(key_max, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer is null, non zero size
static void set_buffer_null_size_not_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, NULL, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : undefined flag
static void set_key_undefined_flags()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 16);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer full, size is 0
static void set_buffer_size_is_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set same key several times
static void set_same_key_several_time()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

static void test_thread_set(char *th_key)
{
    int res = kvstore->set((char *)th_key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get several keys multithreaded
static void set_several_keys_multithreaded()
{
    TEST_SKIP_UNLESS(kvstore != NULL);
    int i = 0, res = 0;
    rtos::Thread kvstore_thread[num_of_threads];
    osStatus threadStatus;

    kvstore_thread[0].start(callback(test_thread_set, (char *)keys[0]));
    kvstore_thread[1].start(callback(test_thread_set, (char *)keys[1]));
    kvstore_thread[2].start(callback(test_thread_set, (char *)keys[2]));


    for (i = 0; i < num_of_threads; i++) {
        threadStatus = kvstore_thread[i].join();
        if (threadStatus != 0) {
            utest_printf("\nthread %d join failed!", i + 1);
        }
    }

    for (i = 0; i < num_of_threads; i++) {
        res = kvstore->get(keys[i], buffer, buffer_size, &actual_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        TEST_ASSERT_EQUAL_STRING_LEN(buffer, data, data_size);
    }

    for (i = 0; i < num_of_threads; i++) {
        res = kvstore->remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
}

//set key "write once" and try to set it again
static void set_write_once_flag_try_set_twice()
{
    char buf[10];
    size_t len;
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, "ONCE", 5, KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->set(key, "TWICE", 6, KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, res);

    res = kvstore->get(key, buf, 10, &len);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL(len, 5);
    TEST_ASSERT_EQUAL_STRING_LEN(buf, "ONCE", 5);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key "write once" and try to remove it
static void set_write_once_flag_try_remove()
{
    char buf[20];
    size_t len;
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, "TO_BE_REMOVED", 14, KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buf, 20, &len);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL(len, 14);
    TEST_ASSERT_EQUAL_STRING_LEN(buf, "TO_BE_REMOVED", 14);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, res);

    res = kvstore->get(key, buf, 20, &len);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value one byte size
static void set_key_value_one_byte_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char data_one = 'a';
    int res = kvstore->set(key, &data_one, 1, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = strncmp(buffer, &data_one, 1);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, res);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value two byte size
static void set_key_value_two_byte_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char data_two[2] = "d";
    int res = kvstore->set(key, data_two, 2, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_two, 1);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value five byte size
static void set_key_value_five_byte_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char data_five[5] = "data";
    int res = kvstore->set(key, data_five, 5, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_five, 4);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value fifteen byte size
static void set_key_value_fifteen_byte_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char data_fif[15] = "data_is_everyt";
    int res = kvstore->set(key, data_fif, 15, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_fif, 14);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value seventeen byte size
static void set_key_value_seventeen_byte_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char data_fif[17] = "data_is_everythi";
    int res = kvstore->set(key, data_fif, 17, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_fif, 16);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set several different keys and retrieve them
static void set_several_key_value_sizes()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char name[7] = "name_";
    char c = 0;
    int i = 0, res = 0;

    name[6] = 0;

    for (i = 0; i < 5; i++) {
        c = i + 'a';
        name[5] = c;
        res = kvstore->set(name, name, sizeof(name), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    for (i = 0; i < 5; i++) {
        c = i + 'a';
        name[5] = c;
        res = kvstore->get(name, buffer, sizeof(buffer), &actual_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        TEST_ASSERT_EQUAL_STRING_LEN(name, buffer, sizeof(name));
        memset(buffer, 0, sizeof(buffer));

        res = kvstore->remove(name);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
}

//try to set several different unvalid key names
static void set_several_unvalid_key_names()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char name[7] = "name_";
    char unvalid[] = {'*', '?', ':', ';', '"', '|', ' ', '<', '>', '\\', '/'};
    int i = 0, res = 0;

    name[6] = 0;

    for (i = 0; i < 11; i++) {
        name[5] = unvalid[i];
        res = kvstore->set(name, name, sizeof(name), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
    }
}

//set key initialize kvstore and retrieve it
static void set_key_init_deinit()
{
    if (kv_setup == TDBStoreSet) {
        TEST_SKIP_MESSAGE("Unstable testcase. Skipped. Issue # IOTSTOR-978");
    }
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING(data, buffer);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key with ROLLBACK flag and retrieve it, set it again with no ROLBACK
static void Sec_set_key_rollback_set_again_no_rollback()
{
    char key_name[7] = "name";

    TEST_SKIP_UNLESS(kvstore != NULL);
    if (kv_setup != SecStoreSet) {
        return;
    }

    int res = kvstore->set(key_name, data, data_size, KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key_name, buffer, sizeof(buffer), &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(data, buffer, sizeof(data));
    memset(buffer, 0, sizeof(buffer));

    res = kvstore->set(key_name, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);

    res = kvstore->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key with ENCRYPT flag and retrieve it
static void Sec_set_key_encrypt()
{
    TEST_SKIP_UNLESS(kvstore != NULL);
    if (kv_setup != SecStoreSet) {
        return;
    }

    int res = kvstore->set(key, data, data_size, KVStore::REQUIRE_CONFIDENTIALITY_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, sizeof(buffer), &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(data, buffer, sizeof(data));

    res = kvstore->get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_ERROR_CODE(KVStore::REQUIRE_CONFIDENTIALITY_FLAG, info.flags);
    memset(buffer, 0, sizeof(buffer));

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------get()------------------*/

//bad params : key is null
static void get_key_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->get(NULL, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void get_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char key_max[KVStore::MAX_KEY_SIZE + 1] = {0};
    memset(key_max, '*', KVStore::MAX_KEY_SIZE);
    int res = kvstore->get(key_max, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer is null, non zero size
static void get_buffer_null_size_not_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->get(key, NULL, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//bad params : buffer full, size is 0
static void get_buffer_size_is_zero()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, NULL, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, 0, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//buffer_size smaller than data real size
static void get_buffer_size_smaller_than_data_real_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char big_data[25] = "data";

    int res = kvstore->set(key, big_data, sizeof(big_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, big_data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//buffer_size bigger than data real size
static void get_buffer_size_bigger_than_data_real_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char big_buffer[25] = {};
    res = kvstore->get(key, big_buffer, sizeof(big_buffer), &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(big_buffer, data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//offset bigger than data size
static void get_offset_bigger_than_data_size()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, data_size + 1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_SIZE, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get a non existing key
static void get_non_existing_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get a removed key
static void get_removed_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//set the same key twice and get latest data
static void get_key_that_was_set_twice()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_data[] = "new_data";
    res = kvstore->set(key, new_data, sizeof(new_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->get(key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, new_data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

static void test_thread_get(const void *th_key)
{
    int res = kvstore->get((char *)th_key, buffer, buffer_size, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get several keys multithreaded
static void get_several_keys_multithreaded()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    rtos::Thread kvstore_thread[num_of_threads];
    osStatus threadStatus;

    for (int i = 0; i < num_of_threads; i++) {
        int res = kvstore->set(keys[i], data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    kvstore_thread[0].start(callback(test_thread_get, "key1"));
    kvstore_thread[1].start(callback(test_thread_get, "key2"));
    kvstore_thread[2].start(callback(test_thread_get, "key3"));

    for (int i = 0; i < num_of_threads; i++) {
        threadStatus = kvstore_thread[i].join();
        if (threadStatus != 0) {
            utest_printf("\nthread %d join failed!", i + 1);
        }
    }

    for (int i = 0; i < num_of_threads; i++) {
        int res = kvstore->remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
}


/*----------------remove()------------------*/

//bad params : key is null
static void remove_key_null()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->remove(NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void remove_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char key_max[KVStore::MAX_KEY_SIZE + 1] = {0};
    memset(key_max, '*', KVStore::MAX_KEY_SIZE);
    int res = kvstore->remove(key_max);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//key doesn’t exist
static void remove_non_existing_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    char new_key[] = "remove_key";
    int res = kvstore->remove(new_key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//key already removed
static void remove_removed_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//key exist - valid flow
static void remove_existed_key()
{
    TEST_SKIP_UNLESS(kvstore != NULL);

    int res = kvstore->set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kvstore->remove(key);
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

    {"set_key_null", set_key_null, greentea_failure_handler},
    {"set_key_length_exceeds_max", set_key_length_exceeds_max, greentea_failure_handler},
    {"set_buffer_null_size_not_zero", set_buffer_null_size_not_zero, greentea_failure_handler},
    {"set_key_undefined_flags", set_key_undefined_flags, greentea_failure_handler},
    {"set_buffer_size_is_zero", set_buffer_size_is_zero, greentea_failure_handler},
    {"set_same_key_several_time", set_same_key_several_time, greentea_failure_handler},
    {"set_several_keys_multithreaded", set_several_keys_multithreaded, greentea_failure_handler},
    {"set_write_once_flag_try_set_twice", set_write_once_flag_try_set_twice, greentea_failure_handler},
    {"set_write_once_flag_try_remove", set_write_once_flag_try_remove, greentea_failure_handler},
    {"set_key_value_one_byte_size", set_key_value_one_byte_size, greentea_failure_handler},
    {"set_key_value_two_byte_size", set_key_value_two_byte_size, greentea_failure_handler},
    {"set_key_value_five_byte_size", set_key_value_five_byte_size, greentea_failure_handler},
    {"set_key_value_fifteen_byte_size", set_key_value_fifteen_byte_size, greentea_failure_handler},
    {"set_key_value_seventeen_byte_size", set_key_value_seventeen_byte_size, greentea_failure_handler},
    {"set_several_key_value_sizes", set_several_key_value_sizes, greentea_failure_handler},
    {"set_several_unvalid_key_names", set_several_unvalid_key_names, greentea_failure_handler},
    {"set_key_init_deinit", set_key_init_deinit, greentea_failure_handler},

    {"Sec_set_key_rollback_set_again_no_rollback", Sec_set_key_rollback_set_again_no_rollback, greentea_failure_handler},
    {"Sec_set_key_encrypt", Sec_set_key_encrypt, greentea_failure_handler},

    {"get_key_null", get_key_null, greentea_failure_handler},
    {"get_key_length_exceeds_max", get_key_length_exceeds_max, greentea_failure_handler},
    {"get_buffer_null_size_not_zero", get_buffer_null_size_not_zero, greentea_failure_handler},
    {"get_buffer_size_is_zero", get_buffer_size_is_zero, greentea_failure_handler},
    {"get_buffer_size_smaller_than_data_real_size", get_buffer_size_smaller_than_data_real_size, greentea_failure_handler},
    {"get_buffer_size_bigger_than_data_real_size", get_buffer_size_bigger_than_data_real_size, greentea_failure_handler},
    {"get_offset_bigger_than_data_size", get_offset_bigger_than_data_size, greentea_failure_handler},
    {"get_non_existing_key", get_non_existing_key, greentea_failure_handler},
    {"get_removed_key", get_removed_key, greentea_failure_handler},
    {"get_key_that_was_set_twice", get_key_that_was_set_twice, greentea_failure_handler},
    {"get_several_keys_multithreaded", get_several_keys_multithreaded, greentea_failure_handler},

    {"remove_key_null", remove_key_null, greentea_failure_handler},
    {"remove_key_length_exceeds_max", remove_key_length_exceeds_max, greentea_failure_handler},
    {"remove_non_existing_key", remove_non_existing_key, greentea_failure_handler},
    {"remove_removed_key", remove_removed_key, greentea_failure_handler},
    {"remove_existed_key", remove_existed_key, greentea_failure_handler},

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
