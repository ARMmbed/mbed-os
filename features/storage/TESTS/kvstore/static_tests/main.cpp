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

#if defined(MBED_CONF_RTOS_PRESENT)
#include "Thread.h"
#endif
#include "mbed_error.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "kvstore_global_api.h"
#include "DeviceKey.h"
#include <cstring>

using namespace utest::v1;
using namespace mbed;

static const char   data[] = "data";
static char         key_buf[20] = {};
static char         buffer[20] = {};
static const size_t data_size = 5;
static size_t       actual_size = 0;
static const size_t buffer_size = 20;
static const int    num_of_threads = 3;
static const char num_of_keys = 11;
#if defined(MBED_CONF_RTOS_PRESENT)
/* Forked 3 threads plus misc, so minimum (4 * OS_STACK_SIZE) heap are required. */
static const int heap_alloc_threshold_size = 4 * OS_STACK_SIZE;
#else
/* Bare metal does not require memory for threads, so use just minimum for test */
static const int heap_alloc_threshold_size = MBED_CONF_TARGET_BOOT_STACK_SIZE;
#endif
static const char *keys[num_of_keys] = { "key1", "key2", "key3", "key4", "key5", "key6", "key7", "key8", "key9",
                                         "key10", "key11"
                                       };

static int init_res = MBED_ERROR_NOT_READY;

kv_info_t info;
kv_iterator_t kvstore_it;

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

/*----------------helpers------------------*/

static char *def_kv = NULL;
static char *key = NULL;

static void parse_default_kv()
{
    const char *def_kv_temp = STR(MBED_CONF_STORAGE_DEFAULT_KV);
    def_kv = new char[3 + strlen(def_kv_temp)];
    sprintf(def_kv, "/%s/", def_kv_temp);

    key = new char[4 + strlen(def_kv)];
    sprintf(key, "%skey", def_kv);
}

/*----------------initialization------------------*/

//init the blockdevice
static void kvstore_init()
{
    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");
    delete[] dummy;

    parse_default_kv();

    init_res = kv_reset(def_kv);
    TEST_SKIP_UNLESS_MESSAGE(init_res != MBED_ERROR_UNSUPPORTED, "Unsupported configuration. Test skipped.");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, init_res);
#if DEVICEKEY_ENABLED
    DeviceKey::get_instance().generate_root_of_trust();
#endif
}

/*----------------set()------------------*/

//bad params : key is null
static void set_key_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(NULL, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void set_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(!init_res);
    char key_max[KV_MAX_KEY_LENGTH + 1] = {};
    memset(key_max, '*', KV_MAX_KEY_LENGTH);
    int res = kv_set(key_max, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer is null, non zero size
static void set_buffer_null_size_not_zero()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, NULL, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer full, size is 0
static void set_buffer_size_is_zero()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set same key several times
static void set_same_key_several_time()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

static void test_thread_set(char *th_key)
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set((char *)th_key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get several keys multithreaded
static void set_several_keys_multithreaded()
{
#if defined(MBED_CONF_RTOS_PRESENT)
    TEST_SKIP_UNLESS(!init_res);
    rtos::Thread kvstore_thread[num_of_threads];
    osStatus threadStatus;

    kvstore_thread[0].start(callback(test_thread_set, (char *) keys[0]));
    kvstore_thread[1].start(callback(test_thread_set, (char *) keys[1]));
    kvstore_thread[2].start(callback(test_thread_set, (char *) keys[2]));


    for (int i = 0; i < num_of_threads; i++) {
        threadStatus = kvstore_thread[i].join();
        if (threadStatus != 0) {
            utest_printf("\nthread %d join failed!", i + 1);
        }
    }

    for (int i = 0; i < num_of_threads; i++) {
        int res = kv_get(keys[i], buffer, buffer_size, &actual_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

        TEST_ASSERT_EQUAL_STRING_LEN(buffer, data, data_size);

        res = kv_remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
#endif
}

//set key "write once" and try to set it again
static void set_write_once_flag_try_set_twice()
{
    TEST_SKIP_UNLESS(!init_res);
    char write_once_key[] = "write_once_key";

    int res = kv_set(write_once_key, data, data_size, KV_WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_set(write_once_key, data, data_size, KV_WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, res);
}

//set key "write once" and try to remove it
static void set_write_once_flag_try_remove()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, KV_WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, res);

    res = kv_reset(def_kv);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
#if DEVICEKEY_ENABLED
    DeviceKey::get_instance().generate_root_of_trust();
#endif
}

//set key value one byte size
static void set_key_value_one_byte_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char data_one = 'a';
    int res = kv_set(key, &data_one, 1, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = strncmp(buffer, &data_one, 1);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, res);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value two byte size
static void set_key_value_two_byte_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char data_two[2] = "d";
    int res = kv_set(key, data_two, 2, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_two, 1);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value five byte size
static void set_key_value_five_byte_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char data_five[5] = "data";
    int res = kv_set(key, data_five, 5, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_five, 4);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value fifteen byte size
static void set_key_value_fifteen_byte_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char data_fif[15] = "data";
    int res = kv_set(key, data_fif, 15, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_fif, 14);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set key value seventeen byte size
static void set_key_value_seventeen_byte_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char data_fif[17] = "data_is_everythi";
    int res = kv_set(key, data_fif, 17, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, data_fif, 16);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//set several different keys and retrieve them
static void set_several_key_value_sizes()
{
    TEST_SKIP_UNLESS(!init_res);
    char name[7] = "name_";
    char c = 0;
    int i = 0, res = 0;

    name[6] = 0;

    for (i = 0; i < num_of_keys; i++) {
        c = i + 'a';
        name[5] = c;
        res = kv_set(name, name, sizeof(name), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    for (i = 0; i < num_of_keys; i++) {
        c = i + 'a';
        name[5] = c;
        res = kv_get(name, buffer, sizeof(buffer), &actual_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        TEST_ASSERT_EQUAL_STRING_LEN(name, buffer, sizeof(name));
        memset(buffer, 0, sizeof(buffer));

        res = kv_remove(name);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
}

//try to set several different unvalid key names
static void set_several_unvalid_key_names()
{
    TEST_SKIP_UNLESS(!init_res);
    char name[7] = "name_";
    char unvalid[] = {'*', '?', ':', ';', '"', '|', ' ', '<', '>', '\\', '/'};
    int i = 0, res = 0;

    name[6] = 0;

    for (i = 0; i < num_of_keys; i++) {
        name[5] = unvalid[i];
        res = kv_set(name, name, sizeof(name), 0);
        if (unvalid[i] != '/') {
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
        } else {
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
        }
    }
}

/*----------------get()------------------*/

//bad params : key is null
static void get_key_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_get(NULL, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void get_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(!init_res);
    char key_max[KV_MAX_KEY_LENGTH + 1] = {};
    memset(key_max, '*', KV_MAX_KEY_LENGTH);
    int res = kv_get(key_max, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : buffer is null, non zero size
static void get_buffer_null_size_not_zero()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_get(key, NULL, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//bad params : buffer full, size is 0
static void get_buffer_size_is_zero()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, NULL, 0, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, 0, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//buffer_size smaller than data real size
static void get_buffer_size_smaller_than_data_real_size()
{
    TEST_SKIP_UNLESS(!init_res);
    char big_data[25] = "data";

    int res = kv_set(key, big_data, sizeof(big_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, big_data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//buffer_size bigger than data real size
static void get_buffer_size_bigger_than_data_real_size()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char big_buffer[25] = {};
    res = kv_get(key, big_buffer, sizeof(big_buffer), &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(big_buffer, data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//get a non existing key
static void get_non_existing_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get a removed key
static void get_removed_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove("key");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//set the same key twice and get latest data
static void get_key_that_was_set_twice()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_data[] = "new_data";
    res = kv_set(key, new_data, sizeof(new_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get(key, buffer, buffer_size, &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING_LEN(buffer, new_data, &actual_size);
    memset(buffer, 0, buffer_size);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

static void test_thread_get(const void *th_key)
{
    TEST_SKIP_UNLESS(!init_res);
    char buffer[5] = {0};

    int res = kv_get((char *)th_key, buffer, sizeof(buffer), &actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING((char *)th_key, buffer);
}

//get several keys multithreaded
static void get_several_keys_multithreaded()
{
#if defined(MBED_CONF_RTOS_PRESENT)
    TEST_SKIP_UNLESS(!init_res);
    int i = 0, res = 0;
    rtos::Thread kvstore_thread[num_of_threads];
    osStatus threadStatus;

    for (i = 0; i < num_of_threads; i++) {
        res = kv_set(keys[i], keys[i], strlen(keys[i]) + 1, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    kvstore_thread[0].start(callback(test_thread_get, "key1"));
    kvstore_thread[1].start(callback(test_thread_get, "key2"));
    kvstore_thread[2].start(callback(test_thread_get, "key3"));

    for (i = 0; i < num_of_threads; i++) {
        threadStatus = kvstore_thread[i].join();
        if (threadStatus != 0) {
            utest_printf("\nthread %d join failed!", i + 1);
        }
    }

    for (i = 0; i < num_of_threads; i++) {
        res = kv_remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }
#endif
}

/*----------------remove()------------------*/

//bad params : key is null
static void remove_key_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_remove(NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void remove_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(!init_res);
    char key_max[KV_MAX_KEY_LENGTH + 1] = {};
    memset(key_max, '*', KV_MAX_KEY_LENGTH);
    int res = kv_remove(key_max);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//key doesn’t exist
static void remove_non_existing_key()
{
    TEST_SKIP_UNLESS(!init_res);
    char new_key[] = "remove_key";
    int res = kv_remove(new_key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//key already removed
static void remove_removed_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//key exist - valid flow
static void remove_existed_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------get_info()------------------*/

//bad params : key is null
static void get_info_key_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_get_info(NULL, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : key length over key max size
static void get_info_key_length_exceeds_max()
{
    TEST_SKIP_UNLESS(!init_res);
    char key_max[KV_MAX_KEY_LENGTH + 1] = {};
    memset(key_max, '*', KV_MAX_KEY_LENGTH);
    int res = kv_get_info(key_max, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

//bad params : &info is null
static void get_info_info_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_get_info(key, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get_info of non existing key
static void get_info_non_existing_key()
{
    TEST_SKIP_UNLESS(!init_res);
    char new_key[] = "get_info_key";
    int res = kv_get_info(new_key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get_info of removed key
static void get_info_removed_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove("key");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);
}

//get_info of existing key - valid flow
static void get_info_existed_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, KV_WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_ERROR_CODE(info.flags, KV_WRITE_ONCE_FLAG);

    res = kv_reset(def_kv);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
#if DEVICEKEY_ENABLED
    DeviceKey::get_instance().generate_root_of_trust();
#endif
}

//get_info of overwritten key
static void get_info_overwritten_key()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_data[] = "new_data";
    res = kv_set(key, new_data, sizeof(new_data), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_get_info(key, &info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_ERROR_CODE(info.size, sizeof(new_data));

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------iterator_open()------------------*/

//bad params : it is null
static void iterator_open_it_null()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_iterator_open(NULL, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, res);
}

/*----------------iterator_next()------------------*/

//key valid, key_size 0
static void iterator_next_key_size_zero()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with empty list
static void iterator_next_empty_list()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iterator_next for one key list
static void iterator_next_one_key_list()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with empty list (all keys removed)
static void iterator_next_empty_list_keys_removed()
{
    TEST_SKIP_UNLESS(!init_res);
    char new_key_1[] = "it_1";
    int res = kv_set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "it_2";
    res = kv_set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(new_key_1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(new_key_2);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with non matching prefix (empty list)
static void iterator_next_empty_list_non_matching_prefix()
{
    TEST_SKIP_UNLESS(!init_res);
    char new_key_1[] = "it_1";
    int res = kv_set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "it_2";
    res = kv_set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_open(&kvstore_it, "prefix");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(new_key_1);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(new_key_2);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next with several overwritten keys
static void iterator_next_several_overwritten_keys()
{
    TEST_SKIP_UNLESS(!init_res);
    for (int i = 0; i < num_of_keys; i++) {
        int res = kv_set(key, data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    int res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_next(kvstore_it, key_buf, sizeof(key_buf));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iterator_next for full list - check key names for validation
static void iterator_next_full_list()
{
    TEST_SKIP_UNLESS(!init_res);
    int i = 0;
    bool *key_found = new bool[num_of_keys];

    for (i = 0; i < num_of_keys; i++) {
        int res = kv_set(keys[i], data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
        key_found[i] = false;
    }

    int res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char temp_key[KV_MAX_KEY_LENGTH];

    for (i = 0; i < num_of_keys; i++) {
        res = kv_iterator_next(kvstore_it, temp_key, sizeof(temp_key));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

        res = kv_get(temp_key, buffer, buffer_size, &actual_size);
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

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    for (i = 0; i < num_of_keys; i++) {
        res = kv_remove(keys[i]);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    delete[] key_found;
}

//iterator_next for full path- check key names for validation
static void iterator_next_path_check()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_set(key, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_open(&kvstore_it, def_kv);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char temp_key[KV_MAX_KEY_LENGTH];

    res = kv_iterator_next(kvstore_it, temp_key, sizeof(temp_key));
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    TEST_ASSERT_EQUAL_STRING(key, temp_key);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_remove(key);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

//iteartor_next remove while iterating
static void iterator_next_remove_while_iterating()
{
    TEST_SKIP_UNLESS(!init_res);

    int i = 0, res = 0;

    for (i = 0; i < 3; i++) {
        int res = kv_set(keys[i], data, data_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    char new_key_1[] = "new_key_1";
    res = kv_set(new_key_1, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char new_key_2[] = "new_key_2";
    res = kv_set(new_key_2, data, data_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_open(&kvstore_it, "key");
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    char key[KV_MAX_KEY_LENGTH];

    while (1) {
        res = kv_iterator_next(kvstore_it, key, sizeof(key));
        if (res != MBED_SUCCESS) {
            break;
        }

        res = kv_remove(key);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    while (1) {
        res = kv_iterator_next(kvstore_it, key, sizeof(key));
        if (res != MBED_SUCCESS) {
            break;
        }

        TEST_ASSERT_EQUAL_STRING_LEN("new", key, 3);

        res = kv_remove(key);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
    }

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------iterator_close()------------------*/

//iterator_close right after iterator_open
static void iterator_close_right_after_iterator_open()
{
    TEST_SKIP_UNLESS(!init_res);
    int res = kv_iterator_open(&kvstore_it, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);

    res = kv_iterator_close(kvstore_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, res);
}

/*----------------setup------------------*/

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    UnityConcludeTest();
    return STATUS_CONTINUE;
}

Case cases[] = {

    Case("kvstore_init", kvstore_init), //must be first

    Case("set_key_null", set_key_null, greentea_failure_handler),
    Case("set_key_length_exceeds_max", set_key_length_exceeds_max, greentea_failure_handler),
    Case("set_buffer_null_size_not_zero", set_buffer_null_size_not_zero, greentea_failure_handler),
    Case("set_buffer_size_is_zero", set_buffer_size_is_zero, greentea_failure_handler),
    Case("set_same_key_several_time", set_same_key_several_time, greentea_failure_handler),
#if defined(MBED_CONF_RTOS_PRESENT)
    Case("set_several_keys_multithreaded", set_several_keys_multithreaded, greentea_failure_handler),
#endif
    Case("set_write_once_flag_try_set_twice", set_write_once_flag_try_set_twice, greentea_failure_handler),
    Case("set_write_once_flag_try_remove", set_write_once_flag_try_remove, greentea_failure_handler),
    Case("set_key_value_one_byte_size", set_key_value_one_byte_size, greentea_failure_handler),
    Case("set_key_value_two_byte_size", set_key_value_two_byte_size, greentea_failure_handler),
    Case("set_key_value_five_byte_size", set_key_value_five_byte_size, greentea_failure_handler),
    Case("set_key_value_fifteen_byte_size", set_key_value_fifteen_byte_size, greentea_failure_handler),
    Case("set_key_value_seventeen_byte_size", set_key_value_seventeen_byte_size, greentea_failure_handler),
    Case("set_several_key_value_sizes", set_several_key_value_sizes, greentea_failure_handler),
    Case("set_several_unvalid_key_names", set_several_unvalid_key_names, greentea_failure_handler),

    Case("get_key_null", get_key_null, greentea_failure_handler),
    Case("get_key_length_exceeds_max", get_key_length_exceeds_max, greentea_failure_handler),
    Case("get_buffer_null_size_not_zero", get_buffer_null_size_not_zero, greentea_failure_handler),
    Case("get_buffer_size_is_zero", get_buffer_size_is_zero, greentea_failure_handler),
    Case("get_buffer_size_smaller_than_data_real_size", get_buffer_size_smaller_than_data_real_size, greentea_failure_handler),
    Case("get_buffer_size_bigger_than_data_real_size", get_buffer_size_bigger_than_data_real_size, greentea_failure_handler),
    Case("get_non_existing_key", get_non_existing_key, greentea_failure_handler),
    Case("get_removed_key", get_removed_key, greentea_failure_handler),
    Case("get_key_that_was_set_twice", get_key_that_was_set_twice, greentea_failure_handler),
#if defined(MBED_CONF_RTOS_PRESENT)
    Case("get_several_keys_multithreaded", get_several_keys_multithreaded, greentea_failure_handler),
#endif
    Case("remove_key_null", remove_key_null, greentea_failure_handler),
    Case("remove_key_length_exceeds_max", remove_key_length_exceeds_max, greentea_failure_handler),
    Case("remove_non_existing_key", remove_non_existing_key, greentea_failure_handler),
    Case("remove_removed_key", remove_removed_key, greentea_failure_handler),
    Case("remove_existed_key", remove_existed_key, greentea_failure_handler),

    Case("get_info_key_null", get_info_key_null, greentea_failure_handler),
    Case("get_info_key_length_exceeds_max", get_info_key_length_exceeds_max, greentea_failure_handler),
    Case("get_info_info_null", get_info_info_null, greentea_failure_handler),
    Case("get_info_non_existing_key", get_info_non_existing_key, greentea_failure_handler),
    Case("get_info_removed_key", get_info_removed_key, greentea_failure_handler),
    Case("get_info_existed_key", get_info_existed_key, greentea_failure_handler),
    Case("get_info_overwritten_key", get_info_overwritten_key, greentea_failure_handler),

    Case("iterator_open_it_null", iterator_open_it_null, greentea_failure_handler),

    Case("iterator_next_key_size_zero", iterator_next_key_size_zero, greentea_failure_handler),
    Case("iterator_next_empty_list", iterator_next_empty_list, greentea_failure_handler),
    Case("iterator_next_one_key_list", iterator_next_one_key_list, greentea_failure_handler),
    Case("iterator_next_empty_list_keys_removed", iterator_next_empty_list_keys_removed, greentea_failure_handler),
    Case("iterator_next_empty_list_non_matching_prefix", iterator_next_empty_list_non_matching_prefix, greentea_failure_handler),
    Case("iterator_next_several_overwritten_keys", iterator_next_several_overwritten_keys, greentea_failure_handler),
    Case("iterator_next_full_list", iterator_next_full_list, greentea_failure_handler),
    Case("iterator_next_path_check", iterator_next_path_check, greentea_failure_handler),
    Case("iterator_next_remove_while_iterating", iterator_next_remove_while_iterating, greentea_failure_handler),

    Case("iterator_close_right_after_iterator_open", iterator_close_right_after_iterator_open, greentea_failure_handler),
};


utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
