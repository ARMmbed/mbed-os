/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "SecureStore.h"
#include "TDBStore.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "mbed_error.h"
#include "Timer.h"
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "DeviceKey.h"

#if !SECURESTORE_ENABLED
#error [NOT_SUPPORTED] SecureStore need to be enabled for this test
#else

using namespace mbed;

const size_t ul_bd_size  = 8 * 4096;
const size_t rbp_bd_size = 4 * 4096;

static const int heap_alloc_threshold_size = 4096;

#undef TEST_SPIF
#undef TEST_FSSTORE_UL
#undef NO_RBP_MODE

#ifdef TEST_SPIF
#include "SPIFBlockDevice.h"
SPIFBlockDevice flash_bd(MBED_CONF_SPIF_DRIVER_SPI_MOSI, MBED_CONF_SPIF_DRIVER_SPI_MISO,
                         MBED_CONF_SPIF_DRIVER_SPI_CLK, MBED_CONF_SPIF_DRIVER_SPI_CS);
SlicingBlockDevice ul_bd(&flash_bd, 0, ul_bd_size);
SlicingBlockDevice rbp_bd(&flash_bd, ul_bd_size, ul_bd_size + rbp_bd_size);
#else
HeapBlockDevice bd(ul_bd_size + rbp_bd_size, 1, 1, 4096);
FlashSimBlockDevice flash_bd(&bd);
SlicingBlockDevice ul_bd(&flash_bd, 0, ul_bd_size);
SlicingBlockDevice rbp_bd(&flash_bd, ul_bd_size, ul_bd_size + rbp_bd_size);
#endif

#ifdef TEST_FSSTORE_UL
#include "LittleFileSystem.h"
#include "FileSystemStore.h"
#endif

using namespace utest::v1;

static const char *const key1      = "key1";
static const char *const key1_val1 = "val1";
static const char *const key2      = "name_of_key2";
static const char *const key2_val1 = "val3";
static const char *const key2_val2 = "val2 of key 2";
static const char *const key2_val3 = "Val1 value of key 2            ";
static const char *const key3      = "This_is_the_name_of_key3";
static const char *const key3_val1 = "Data value of key 3 is the following";
static const char *const key3_val2 = "Unfollow";
static const char *const key4      = "This_is_the_name_of_key4";
static const char *const key4_val1 = "Is this the value of key 4?";
static const char *const key4_val2 = "What the hell is the value of key 4, god damn it!";
static const char *const key5      = "This_is_the_real_name_of_Key5";
static const char *const key5_val1 = "Key 5 value that should definitely be written";
static const char *const key5_val2 = "Key 5 value that should definitely not be written";
static const char *const key6      = "Key6_name";
static const char *const key6_val1 = "Value 1 of key6";
static const char *const key6_val2 = "Value 2 of key6. That's it.";
static const char *const key7      = "Key7";
static const char *const key7_val1 = "7 is a lucky number";

static void white_box_test()
{
    uint8_t get_buf[256];
    size_t actual_data_size;
    int result;
    mbed::Timer timer;
    int elapsed;
    KVStore::info_t info;

    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");

    // We need to skip the test if we don't have enough memory for the heap block device.
    // However, this device allocates the erase units on the fly, so "erase" it via the flash
    // simulator. A failure here means we haven't got enough memory.
    flash_bd.init();
    result = flash_bd.erase(0, flash_bd.size());
    TEST_SKIP_UNLESS_MESSAGE(!result, "Not enough heap to run test");
    flash_bd.deinit();

    delete[] dummy;

#ifdef TEST_FSSTORE_UL
    LittleFileSystem *fs = new LittleFileSystem("fs", &ul_bd);

    result = fs->mount(&ul_bd);

    if (result) {
        result = fs->reformat(&ul_bd);
        TEST_ASSERT_EQUAL(0, result);
    }

    FileSystemStore *ul_kv = new FileSystemStore(fs);
#else
    TDBStore *ul_kv = new TDBStore(&ul_bd);
#endif

#ifdef NO_RBP_MODE
    TDBStore *rbp_kv = 0;
#else
    TDBStore *rbp_kv = new TDBStore(&rbp_bd);
#endif

    SecureStore *sec_kv = new SecureStore(ul_kv, rbp_kv);

    for (int i = 0; i < 2; i++) {
        timer.reset();
        result = sec_kv->init();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        elapsed = timer.read_ms();
        printf("Elapsed time for init %d ms\n", elapsed);

        timer.reset();
        result = sec_kv->reset();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
#if DEVICEKEY_ENABLED
        DeviceKey::get_instance().generate_root_of_trust();
#endif
        elapsed = timer.read_ms();
        printf("Elapsed time for reset is %d ms\n", elapsed);

        result = sec_kv->set(key1, key1_val1, strlen(key1_val1), KVStore::REQUIRE_CONFIDENTIALITY_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->set(key2, key2_val1, strlen(key2_val1), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->set(key2, key2_val2, strlen(key2_val2), KVStore::REQUIRE_CONFIDENTIALITY_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->get(key2, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key2_val2), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key2_val2, get_buf, strlen(key2_val2));

        timer.reset();
        result = sec_kv->set(key2, key2_val3, strlen(key2_val3), KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
        elapsed = timer.read_ms();
        printf("Elapsed time for set is %d ms\n", elapsed);

        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->set(key3, key3_val1, strlen(key3_val1),
                             KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->set(key3, key3_val2, strlen(key3_val2), KVStore::REQUIRE_CONFIDENTIALITY_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_ARGUMENT, result);

        result = sec_kv->get(key3, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key3_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key3_val1, get_buf, strlen(key3_val1));

        for (int j = 0; j < 2; j++) {
            result = sec_kv->set(key4, key4_val1, strlen(key4_val1), KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            result = sec_kv->set(key4, key4_val2, strlen(key4_val2), KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        }

        result = sec_kv->get_info(key3, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG, info.flags);
        TEST_ASSERT_EQUAL(strlen(key3_val1), info.size);

        result = ul_kv->get_info(key3, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(0, info.flags);

#ifndef NO_RBP_MODE
        result = rbp_kv->get_info(key3, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
#endif

        result = sec_kv->remove(key3);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->remove(key3);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = ul_kv->get_info(key3, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

#ifndef NO_RBP_MODE
        result = rbp_kv->get_info(key3, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);
#endif

        result = sec_kv->get_info(key5, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = sec_kv->set(key5, key5_val1, strlen(key5_val1),
                             KVStore::REQUIRE_REPLAY_PROTECTION_FLAG | KVStore::WRITE_ONCE_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

#ifndef NO_RBP_MODE
        result = rbp_kv->get_info(key5, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(KVStore::WRITE_ONCE_FLAG, info.flags);
#endif

        result = sec_kv->set(key5, key5_val2, strlen(key5_val2),
                             KVStore::REQUIRE_REPLAY_PROTECTION_FLAG | KVStore::WRITE_ONCE_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, result);

        result = sec_kv->remove(key5);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, result);

        result = sec_kv->get(key5, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key5_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key5_val1, get_buf, strlen(key5_val1));

        result = sec_kv->get(key1, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key1_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key1_val1, get_buf, strlen(key1_val1));

        timer.reset();
        result = sec_kv->get(key2, get_buf, sizeof(get_buf), &actual_data_size);
        elapsed = timer.read_ms();
        printf("Elapsed time for get is %d ms\n", elapsed);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key2_val3), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key2_val3, get_buf, strlen(key2_val3));

        result = sec_kv->get(key4, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key4_val2), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key4_val2, get_buf, strlen(key4_val2));

        result = sec_kv->get(key4, get_buf, 7, &actual_data_size, 30);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(7, actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key4_val2 + 30, get_buf, 7);

        result = sec_kv->get(key5, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key5_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key5_val1, get_buf, strlen(key5_val1));

        KVStore::iterator_t it;
        char *char_get_buf = reinterpret_cast <char *>(get_buf);

        result = sec_kv->iterator_open(&it, "This");
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->iterator_next(it, char_get_buf, sizeof(get_buf));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        bool got_key4 = !strcmp(key4, char_get_buf);
        bool got_key5 = !strcmp(key5, char_get_buf);
        TEST_ASSERT_EQUAL(true, got_key4 || got_key5);

        result = sec_kv->iterator_next(it, char_get_buf, sizeof(get_buf));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        if (got_key4) {
            TEST_ASSERT_EQUAL_STRING(key5, char_get_buf);
        } else {
            TEST_ASSERT_EQUAL_STRING(key4, char_get_buf);
        }

        result = sec_kv->iterator_next(it, (char *)get_buf, sizeof(get_buf));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = sec_kv->iterator_close(it);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->deinit();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        timer.reset();
        result = sec_kv->init();
        elapsed = timer.read_ms();
        printf("Elapsed time for init is %d ms\n", elapsed);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->get(key4, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key4_val2), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key4_val2, get_buf, strlen(key4_val2));

        result = sec_kv->set(key6, key6_val1, strlen(key6_val1),
                             KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

#ifndef NO_RBP_MODE
        // Simulate a rollback attack
        char attack_buf[sizeof(get_buf)];
        size_t attack_size;
        result = ul_kv->get(key6, attack_buf, sizeof(attack_buf), &attack_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->set(key6, key6_val2, strlen(key6_val2),
                             KVStore::REQUIRE_CONFIDENTIALITY_FLAG | KVStore::REQUIRE_REPLAY_PROTECTION_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = ul_kv->set(key6, attack_buf, attack_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->get_info(key6, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_RBP_AUTHENTICATION_FAILED, result);

        // Make sure encrypted data is truly encrypted
        result = rbp_kv->get_info(key6, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        int cmac_size = info.size;
        uint8_t *cmac = new uint8_t[cmac_size];

        result = sec_kv->set(key7, key7_val1, strlen(key7_val1), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = ul_kv->get(key7, attack_buf, sizeof(attack_buf), &attack_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        int data_offset = attack_size - cmac_size - strlen(key7_val1);
        TEST_ASSERT_EQUAL(0, strncmp(key7_val1, attack_buf + data_offset, strlen(key7_val1)));

        result = sec_kv->set(key7, key7_val1, strlen(key7_val1), KVStore::REQUIRE_CONFIDENTIALITY_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = ul_kv->get(key7, attack_buf, sizeof(attack_buf), &attack_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_NOT_EQUAL(0, strncmp(key7_val1, attack_buf + data_offset, strlen(key7_val1)));

        // Simulate a wrong CMAC
        result = ul_kv->get(key7, attack_buf, attack_size - cmac_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        result = ul_kv->get(key7, cmac, cmac_size, &actual_data_size, attack_size - cmac_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        cmac[0]++;

        KVStore::set_handle_t handle;
        result = ul_kv->set_start(&handle, key7, attack_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = ul_kv->set_add_data(handle, attack_buf, attack_size - cmac_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        result = ul_kv->set_add_data(handle, cmac, cmac_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        result = ul_kv->set_finalize(handle);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = ul_kv->get(key7, attack_buf, sizeof(attack_buf), &attack_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = sec_kv->get_info(key7, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_AUTHENTICATION_FAILED, result);

        delete[] cmac;
#endif

        result = sec_kv->deinit();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
    }

    delete sec_kv;
    delete ul_kv;
    delete rbp_kv;

#ifdef TEST_FSSTORE_UL
    delete fs;
#endif
}

#if 0
static void multi_set_test()
{
    char *key;
    uint8_t *get_buf, *set_buf;
    size_t key_size = 32;
    size_t data_size = 512;
    size_t num_keys = 16;
    size_t set_iters = 3;
    size_t actual_data_size;
    int result;
    mbed::Timer timer;
    int elapsed;
    size_t i;
    uint8_t key_ind;

    timer.start();
#if !defined(TEST_SPIF) && !defined(TEST_SD)
    HeapBlockDevice heap_bd(4096 * 64, 1,  1, 4096);
    FlashSimBlockDevice flash_bd(&heap_bd);
#endif

    // TODO: Fix
    KVStore *kvs = new TDBStore(&flash_bd);

    timer.reset();
    result = kvs->init();
    elapsed = timer.read_ms();
    printf("Elapsed time for initial init is %d ms\n", elapsed);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    key = new char[key_size + 1];
    get_buf = new uint8_t[data_size];
    set_buf = new uint8_t[data_size];

    srand(1);
    for (i = 0; i < key_size; i++) {
        // printable characters only
        key[i] = rand() % 223 + 32;
    }
    key[key_size] = '\0';

    for (i = 0; i < data_size; i++) {
        set_buf[i] = rand() % 256;
    }

    int max_set_time = 0, total_set_time = 0;
    int max_get_time = 0, total_get_time = 0;

    timer.reset();
    result = kvs->reset();
    elapsed = timer.read_ms();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    for (i = 0; i < set_iters; i++) {
        for (key_ind = 0; key_ind < num_keys; key_ind++) {
            key[0] = '0' + key_ind;
            set_buf[0] = key_ind * (i + 1);
            timer.reset();
            result = kvs->set(key, set_buf, data_size, 0);
            elapsed = timer.read_ms();
            TEST_ASSERT_EQUAL(0, result);
            if (elapsed > max_set_time) {
                max_set_time = elapsed;
            }
            total_set_time += elapsed;
        }
    }

    for (key_ind = 0; key_ind < num_keys; key_ind++) {
        key[0] = '0' + key_ind;
        set_buf[0] = key_ind * set_iters;
        timer.reset();
        result = kvs->get(key, get_buf, data_size, &actual_data_size);
        elapsed = timer.read_ms();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(data_size, actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(set_buf, get_buf, data_size);
        if (elapsed > max_get_time) {
            max_get_time = elapsed;
        }
        total_get_time += elapsed;
    }

    printf("set time: Total (%d * %d times) - %d ms, Average - %d ms, Max - %d ms\n",
           set_iters, num_keys, total_set_time,
           total_set_time / (set_iters * num_keys), max_set_time);
    printf("get time: Total (%d times)      - %d ms, Average - %d ms, Max - %d ms\n",
           num_keys, total_get_time,
           total_get_time / num_keys, max_get_time);

    result = kvs->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    timer.reset();
    result = kvs->init();
    elapsed = timer.read_ms();
    printf("Elapsed time for init is %d ms\n", elapsed);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    result = kvs->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    delete[] key;
    delete[] get_buf;
    delete[] set_buf;

    delete kvs;
}
#endif



utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("SecureStore: White box test",     white_box_test,    greentea_failure_handler),
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

#endif // !SECURESTORE_ENABLED
