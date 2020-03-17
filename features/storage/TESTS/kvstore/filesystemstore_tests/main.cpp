/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "rtos/Thread.h"
#include "mbed_trace.h"
#include "mbed_error.h"
#include "BlockDevice.h"
#include "FileSystem.h"
#include "FileSystemStore.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>

#if !SECURESTORE_ENABLED
#error [NOT_SUPPORTED] SecureStore need to be enabled for this test
#else

#define FSST_TEST_NUM_OF_THREADS 5

static const int heap_alloc_threshold_size = 4096;

using namespace utest::v1;
using namespace mbed;

BlockDevice *bd = BlockDevice::get_default_instance();

typedef struct {
    int thread_num;
    FileSystemStore *fsst;
} thread_data_t;

static void test_set_thread_job(void *data)
{
    char kv_value[12] = {"valuevalue"};
    char kv_key[6] = {"key"};
    char thread_str[3] = {0};
    int err = 0;

    thread_data_t *thread_data = (thread_data_t *)data;
    int thread_num = thread_data->thread_num;
    FileSystemStore *thread_fsst = (FileSystemStore *)(thread_data->fsst);

    utest_printf("\n Thread %d Started\n", thread_num);
    sprintf(thread_str, "%d", thread_num);
    strcat(kv_value, thread_str);
    strcat(kv_key, thread_str);
    err = thread_fsst->set(kv_key, kv_value, strlen(kv_value) + 1, 0);

    TEST_ASSERT_EQUAL(0, err);
}

void test_file_system_store_functionality_unit_test()
{
    utest_printf("Test FileSystemStore Functionality APIs..\n");
    TEST_SKIP_UNLESS(bd != NULL);

    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");
    delete[] dummy;

    char kv_value1[64] = {"value1value1value1value1value1value1"};
    char kv_key1[16] = {"key1"};
    char kv_value2[64] = {"value2value2value2value2value2value2"};
    char kv_key2[16] = {"key2"};
    char kv_value3[64] = {"valui3valui3"};
    char kv_key3[16] = {"kei3"};
    char kv_value5[64] = {"setonce5555"};
    char kv_key5[16] = {"key5"};
    char kv_buf[64] = {0};
    char kv_name[16] = {0};
    int i_ind = 0;
    int err = 0;
    size_t actual_size = 0;

    err = bd->init();
    TEST_ASSERT_EQUAL(0, err);

    FileSystem *fs = FileSystem::get_default_instance();

    err = fs->mount(bd);
    if (err) {
        err = fs->reformat(bd);
        TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    }

    FileSystemStore *fsst = new FileSystemStore(fs);

    err = fsst->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Set key1 */
    err = fsst->set(kv_key1, kv_value1, 64, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Set key2 */
    err = fsst->set(kv_key2, kv_value2, strlen(kv_value2), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->get(kv_key2, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL(0, strcmp(kv_buf, kv_value2));
    TEST_ASSERT_EQUAL(strlen(kv_value2), (int)actual_size);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Set key3 */
    err = fsst->set(kv_key3, kv_value3, 12, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    KVStore::info_t kv_info;
    err = fsst->get_info(kv_key3, &kv_info);
    TEST_ASSERT_EQUAL(((int)kv_info.flags), 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Set key5 WRITE_ONCE Twice */
    err = fsst->set(kv_key5, kv_value5, 10, mbed::KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->set(kv_key5, kv_value3, 10, mbed::KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, err);

    err = fsst->get_info(kv_key5, &kv_info);
    TEST_ASSERT_EQUAL(((int)kv_info.flags), mbed::KVStore::WRITE_ONCE_FLAG);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Verify value remains of first set */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key5, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL(0, strncmp(kv_buf, kv_value5, 10));
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Non existing File get fails */
    err = fsst->get("key4", kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);

    KVStore::iterator_t kv_it;
    err = fsst->iterator_open(&kv_it, NULL);
    TEST_ASSERT_EQUAL(0, err);
    i_ind = 0;
    while (fsst->iterator_next(kv_it, kv_name, 16) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
    }
    TEST_ASSERT_EQUAL(i_ind, 4); /* 4 Files : key1, key2, kei3, key5 */
    fsst->iterator_close(kv_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Remove Write Once key5 - should fail */
    err = fsst->remove(kv_key5);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, err);

    /* Verify key5 still remains and has the same value */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key5, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL(0, strncmp(kv_buf, kv_value5, 10));
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    fsst->iterator_open(&kv_it, "key");
    TEST_ASSERT_EQUAL(0, err);
    i_ind = 0;
    while (fsst->iterator_next(kv_it, kv_name, 16) != MBED_ERROR_ITEM_NOT_FOUND) {
        i_ind++;
        TEST_ASSERT_EQUAL(0, strncmp(kv_name, "key", strlen("key")));
    }
    TEST_ASSERT_EQUAL(i_ind, 3); /* 3 Files with prefix 'key' : key1, key2, key5 */
    fsst->iterator_close(kv_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Verify double Remove kei3: first succeed, second fails */
    err = fsst->remove(kv_key3);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    err = fsst->remove(kv_key3);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);

    /* Verify after removing kei3 that get value/info fail */
    err = fsst->get(kv_key3, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);
    err = fsst->get_info(kv_key3, &kv_info);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);

    /* Reset - Verify key2 exists before reset, and not found after reset  */
    err = fsst->get(kv_key2, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    err = fsst->reset();
    err = fsst->get(kv_key2, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);

    /* Verify that even Write-Once key5 is not found after reset */
    err = fsst->get(kv_key5, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, err);

    /* Verify that key5 Write-Once can be set again after Reset*/
    err = fsst->set(kv_key5, kv_value5, 10, 0x1);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    err = bd->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
}



void test_file_system_store_edge_cases()
{
    utest_printf("Test FileSystemStore Edge Cases..\n");
    TEST_SKIP_UNLESS(bd != NULL);

    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");
    delete[] dummy;

    KVStore::info_t kv_info;
    KVStore::iterator_t kv_it;
    char kv_value1[64] = {"value1value1value1value1value1value1"};
    char kv_key1[16] = {"key1"};
    char kv_value2[64] = {"value2value2value2value2value2value2"};
    char kv_buf[64] = {0};
    char kv_name[16] = {0};


    int err = bd->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    FileSystem *fs = FileSystem::get_default_instance();

    err = fs->mount(bd);
    if (err) {
        err = fs->reformat(bd);
        TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    }

    FileSystemStore *fsst = new FileSystemStore(fs);

    err = fsst->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    size_t actual_size = 0;

    /*********************************/
    /*********** Unit Test ***********/
    /*********************************/
    /* Fail Set - key NULL */
    err = fsst->set(NULL, kv_value1, 64, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Set - Key length exceeds max */
    err = fsst->set(NULL, kv_value1, KVStore::MAX_KEY_SIZE + 10, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Set - NULL Buffer and size larger than 0 */
    err = fsst->set(kv_key1, NULL, 64, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* OK Set - NULL Buffer and Size is 0 */
    err = fsst->set(kv_key1, kv_value1, 0, 0);
    TEST_ASSERT_EQUAL(0, err);

    /* OK Set - Set Key1 twice and get returns second value */
    err = fsst->set(kv_key1, kv_value1, 64, 0);
    err = fsst->set(kv_key1, kv_value2, strlen(kv_value2), 0);
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_EQUAL(0, strcmp(kv_buf, kv_value2));
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Get - NULL Key */
    memset(kv_buf, 0, 64);
    err = fsst->get(NULL, kv_buf, 64, &actual_size, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* OK Get - NULL buffer , size=0 */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, NULL, 0, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Get - NULL buffer , size>0 */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, NULL, 64, &actual_size, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* OK Get - buffer is smaller than value's actual size */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, kv_buf, 8, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Get - offset larger than file size */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, kv_buf, 8, &actual_size, 128);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Remove - NULL Key */
    err = fsst->remove(NULL);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Remove - Key not found */
    err = fsst->remove("key4");
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Get_Info - NULL Key */
    err = fsst->get_info(NULL, &kv_info);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* OK Get_Info - NULL info pointer - verifies whether a key exists */
    err = fsst->get_info(kv_key1, NULL);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Get_Info - Key not found */
    err = fsst->get_info("key4", &kv_info);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Iteraor Open - NULL it */
    err = fsst->iterator_open(NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Iteraor Next - key size is 0  */
    err = fsst->iterator_open(&kv_it, NULL);
    err = fsst->iterator_next(kv_it, kv_name, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);
    err = fsst->iterator_close(kv_it);

    /* OK Iteraor Next - empty folder, returns not found  */
    err = fsst->reset();
    err = fsst->iterator_open(&kv_it, NULL);
    err = fsst->iterator_next(kv_it, kv_name, 16);
    TEST_ASSERT_NOT_EQUAL(0, err);
    err = fsst->iterator_close(kv_it);

    /* OK Iteraor Next - 1 File in folder, first returns ok, second returns not found  */
    err = fsst->set(kv_key1, kv_value1, 64, 0);
    err = fsst->iterator_open(&kv_it, NULL);
    err = fsst->iterator_next(kv_it, kv_name, 16);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    err = fsst->iterator_next(kv_it, kv_name, 16);
    TEST_ASSERT_NOT_EQUAL(0, err);
    err = fsst->iterator_close(kv_it);

    /* OK Iteraor Close - close after open*/
    err = fsst->iterator_open(&kv_it, NULL);
    err = fsst->iterator_close(kv_it);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Set_Start - NULL handle */
    err = fsst->set_start(NULL, "key1", 64, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Set_Start - NULL key */
    KVStore::set_handle_t handle;
    err = fsst->set_start(&handle, NULL, 64, 0);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* OK Set_Finalize - finalize after start, size 0 */
    err = fsst->set_start(&handle, "key1", 0, 0);
    err = fsst->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    /* Fail Set_Add_Data - NULL handle */
    err = fsst->set_add_data(NULL, "setvalue1", 10);
    TEST_ASSERT_NOT_EQUAL(0, err);

    /* Fail Set_Add_Data - NULL value */
    err = fsst->set_start(&handle, "key1", 0, 0);
    err = fsst->set_add_data(handle, NULL, 10);
    TEST_ASSERT_NOT_EQUAL(0, err);
    err = fsst->set_finalize(handle);

    /* OK Set_Add_Data - value size 0 */
    err = fsst->set_start(&handle, "key1", 10, 0);
    err = fsst->set_add_data(handle, "abcde12345", 10);
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, kv_buf, 10, &actual_size, 0);
    err = fsst->set_add_data(handle, "abcde12345", 0);
    err = fsst->set_finalize(handle);

    /* OK Get after Finalize */
    memset(kv_buf, 0, 64);
    err = fsst->get(kv_key1, kv_buf, 12, &actual_size, 0);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, strcmp(kv_buf, "abcde12345"));

    /* FAIL SET_Add_Data - exceed final size  */
    err = fsst->set_start(&handle, "key1", 10, 0);
    err = fsst->set_add_data(handle, "abcde12345", 5);
    err = fsst->set_add_data(handle, "abcde12345", 10);
    TEST_ASSERT_NOT_EQUAL(0, err);
    err = fsst->set_add_data(handle, "abcde12345", 5);
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    err = fsst->set_finalize(handle);

    /* FAIL SET_Add_Data - final size smaller than set at start */
    err = fsst->set_start(&handle, "key1", 10, 0);
    err = fsst->set_add_data(handle, "abcde12345", 5);
    err = fsst->set_add_data(handle, "abcde12345", 3);
    err = fsst->set_finalize(handle);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_INVALID_SIZE, err);

    err = fsst->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = bd->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
}

void test_file_system_store_multi_threads()
{
    utest_printf("\nTest Multi Threaded FileSystemStore Set Starts..\n");
    TEST_SKIP_UNLESS(bd != NULL);

    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");
    delete[] dummy;

    char kv_buf[64] = {0};
    int err = bd->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    FileSystem *fs = FileSystem::get_default_instance();

    err = fs->mount(bd);

    if (err) {
        err = fs->reformat(bd);
        TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
    }

    FileSystemStore *fsst = new FileSystemStore(fs);

    err = fsst->init();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    thread_data_t thread_data[3];

    /* Thread Access Test Starts */
    rtos::Thread set_thread[FSST_TEST_NUM_OF_THREADS];

    int i_ind = 0;

    for (i_ind = 0; i_ind < FSST_TEST_NUM_OF_THREADS; i_ind++) {
        thread_data[i_ind].fsst = fsst;
        thread_data[i_ind].thread_num = i_ind + 1;
        set_thread[i_ind].start(callback(test_set_thread_job, &(thread_data[i_ind])));
    }

    for (i_ind = 0; i_ind < FSST_TEST_NUM_OF_THREADS; i_ind++) {
        set_thread[i_ind].join();
    }


    char kv_value[12] = {"valuevalue"};
    char kv_key[6] = {"key"};
    char thread_str[FSST_TEST_NUM_OF_THREADS] = {0};

    size_t actual_size = 0;

    for (i_ind = 1; i_ind < (FSST_TEST_NUM_OF_THREADS + 1); i_ind++) {
        memset(kv_buf, 0, 64);
        sprintf(thread_str, "%d", i_ind);
        strcpy(&kv_value[10], thread_str);
        strcpy(&kv_key[3], thread_str);
        err = fsst->get(kv_key, kv_buf, 12, &actual_size, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
        TEST_ASSERT_EQUAL(0, strcmp(kv_value, kv_buf));
    }

    err = fsst->reset();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = fsst->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);

    err = bd->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(0, err);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing functionality APIs unit test", test_file_system_store_functionality_unit_test, greentea_failure_handler),
    Case("Testing Edge Cases", test_file_system_store_edge_cases, greentea_failure_handler),
    Case("Testing Multi Threads Set", test_file_system_store_multi_threads, greentea_failure_handler)
};

Specification specification(test_setup, cases);


int main()
{
    mbed_trace_init();
    utest_printf("MAIN STARTS\n");
    return !Harness::run(specification);
}

#endif //!SECURESTORE_ENABLED
#endif // !defined(MBED_CONF_RTOS_PRESENT)
