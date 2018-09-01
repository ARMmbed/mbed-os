/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
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

#include "nvstore.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "mbed_wait_api.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "mbed_stats.h"

#if !NVSTORE_ENABLED
#error [NOT_SUPPORTED] NVSTORE needs to be enabled for this test
#endif

using namespace utest::v1;

static const uint16_t max_test_keys = 20;

static const size_t basic_func_max_data_size = 128;

static const int thr_test_num_buffs = 5;
static const int thr_test_num_secs = 5;
static const int thr_test_max_data_size = 32;
static const int thr_test_num_threads = 3;

#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
static const int thr_test_min_stack_size = 1280;
static const int thr_test_max_stack_size = 1280;
#else
static const int thr_test_min_stack_size = 768;
static const int thr_test_max_stack_size = 1024;
#endif

typedef struct {
    uint8_t *buffs[max_test_keys][thr_test_num_buffs];
    uint16_t sizes[max_test_keys][thr_test_num_buffs];
    uint16_t max_keys;
    bool stop_threads;
} thread_test_data_t;

static thread_test_data_t *thr_test_data;

static const int race_test_num_threads = 4;
static const int race_test_key = 1;
static const int race_test_data_size = 128;
static const int race_test_min_stack_size = 768;
static const int race_test_max_stack_size = 1024;

static bool nvstore_overlaps_code = false;

static void gen_random(uint8_t *s, int len)
{
    for (int i = 0; i < len; ++i) {
        s[i] = rand() % 256;
    }
}

static void nvstore_basic_functionality_test()
{

    uint16_t actual_len_bytes = 0;
    NVStore &nvstore = NVStore::get_instance();
    uint16_t key;

    uint8_t *nvstore_testing_buf_set = new uint8_t[basic_func_max_data_size];
    uint8_t *nvstore_testing_buf_get = new uint8_t[basic_func_max_data_size];

    int result;

    printf("NVStore areas:\n");
    for (uint8_t area = 0; area < NVSTORE_NUM_AREAS; area++) {
        uint32_t area_address;
        size_t area_size;
        nvstore.get_area_params(area, area_address, area_size);
        printf("Area %d: address 0x%08lx, size %d (0x%x)\n", area, area_address, area_size, area_size);
        if (area_address < FLASHIAP_ROM_END) {
            nvstore_overlaps_code = true;
        }
        TEST_SKIP_UNLESS_MESSAGE(!nvstore_overlaps_code, "Test skipped. NVStore region overlaps code.");
    }

    gen_random(nvstore_testing_buf_set, basic_func_max_data_size);

    uint16_t max_possible_keys = nvstore.get_max_possible_keys();
    TEST_SKIP_UNLESS_MESSAGE(max_test_keys < max_possible_keys,
                             "Not enough possible keys for test. Test skipped.");

    nvstore.set_max_keys(max_test_keys);
    TEST_ASSERT_EQUAL(max_test_keys, nvstore.get_max_keys());

    result = nvstore.reset();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    printf("Max keys %d (out of %d possible ones)\n", nvstore.get_max_keys(), max_possible_keys);

    result = nvstore.set(5, 18, nvstore_testing_buf_set);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    result = nvstore.get(5, 22, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(18, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nvstore_testing_buf_set, nvstore_testing_buf_get, 15);
    result = nvstore.remove(5);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.get(5, 20, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);

    result = nvstore.set(11, 0, NULL);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(9, 20, NULL);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(7, 0, nvstore_testing_buf_set);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(10, 64, nvstore_testing_buf_set);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(13, 3, &(nvstore_testing_buf_set[1]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(15, 15, &(nvstore_testing_buf_set[2]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(64, 15, &(nvstore_testing_buf_set[2]));
    TEST_ASSERT_EQUAL(NVSTORE_BAD_VALUE, result);
    result = nvstore.set(9, 20, &(nvstore_testing_buf_set[3]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set_once(19, 12, &(nvstore_testing_buf_set[2]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(19, 10, &(nvstore_testing_buf_set[3]));
    TEST_ASSERT_EQUAL(NVSTORE_ALREADY_EXISTS, result);

    result = nvstore.allocate_key(key, 3);
    TEST_ASSERT_EQUAL(NVSTORE_NUM_PREDEFINED_KEYS, key);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(NVSTORE_NUM_PREDEFINED_KEYS, 17, &(nvstore_testing_buf_set[3]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    result = nvstore.allocate_key(key, 3);
    TEST_ASSERT_EQUAL(NVSTORE_NUM_PREDEFINED_KEYS + 1, key);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(NVSTORE_NUM_PREDEFINED_KEYS + 1, 17, &(nvstore_testing_buf_set[3]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    // Make sure set items are also gotten OK after reset
    result = nvstore.deinit();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.init();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    result = nvstore.get(14, 20, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);
    result = nvstore.get(7, 0, NULL, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);
    result = nvstore.get(7, 15, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);
    result = nvstore.get(7, 0, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);
    result = nvstore.get(9, 0, NULL, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_BUFF_TOO_SMALL, result);
    result = nvstore.get(9, 150, NULL, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_BUFF_TOO_SMALL, result);
    result = nvstore.get(10, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(64, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nvstore_testing_buf_set, nvstore_testing_buf_get, 64);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(17, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&nvstore_testing_buf_set[3], nvstore_testing_buf_get, 17);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS + 1, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(17, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&nvstore_testing_buf_set[3], nvstore_testing_buf_get, 17);

    result = nvstore.free_all_keys_by_owner(3);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS + 1, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);

    result = nvstore.get(10, 65, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(64, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nvstore_testing_buf_set, nvstore_testing_buf_get, 64);

    result = nvstore.get(64, 20, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_BAD_VALUE, result);
    result = nvstore.get(9, 20, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(20, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[3]), nvstore_testing_buf_get, 20);

    result = nvstore.get(9, 21, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(20, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[3]), nvstore_testing_buf_get, 20);

    result = nvstore.get(9, 19, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_BUFF_TOO_SMALL, result);
    result = nvstore.get(13, 3, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(3, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[1]), nvstore_testing_buf_get, 3);
    result = nvstore.get_item_size(13, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(3, actual_len_bytes);

    result = nvstore.get(13, 4, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(3, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[1]), nvstore_testing_buf_get, 3);

    result = nvstore.get(13, 2, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_BUFF_TOO_SMALL, result);
    result = nvstore.init();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    // check all the expected keys
    actual_len_bytes = 0;
    result = nvstore.get(10, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(64, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nvstore_testing_buf_set, nvstore_testing_buf_get, 64);

    actual_len_bytes = 0;
    result = nvstore.get(11, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);

    actual_len_bytes = 0;
    result = nvstore.get(13, 3, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(3, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[1]), nvstore_testing_buf_get, 3);

    actual_len_bytes = 0;
    result = nvstore.get(9, 20, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(20, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[3]), nvstore_testing_buf_get, 20);

    actual_len_bytes = 0;
    result = nvstore.get(7, 0, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);

    actual_len_bytes = 0;
    result = nvstore.get(15, 15, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(15, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[2]), nvstore_testing_buf_get, 15);

    actual_len_bytes = 0;
    result = nvstore.get(19, 12, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(12, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[2]), nvstore_testing_buf_get, 12);

    // change the data for all keys
    result = nvstore.set(10, 15, &(nvstore_testing_buf_set[4]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(11, 27, &(nvstore_testing_buf_set[5]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(13, 7, &(nvstore_testing_buf_set[6]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(9, 0, &(nvstore_testing_buf_set[7]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(7, 48, &(nvstore_testing_buf_set[8]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(14, 89, &(nvstore_testing_buf_set[9]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    result = nvstore.set(15, 53, &(nvstore_testing_buf_set[10]));
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    actual_len_bytes = 0;
    result = nvstore.get(10, 15, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(15, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[4]), nvstore_testing_buf_get, 15);

    actual_len_bytes = 0;
    result = nvstore.get(11, 27, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(27, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[5]), nvstore_testing_buf_get, 27);

    actual_len_bytes = 0;
    result = nvstore.get(13, 7, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(7, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[6]), nvstore_testing_buf_get, 7);

    actual_len_bytes = 0;
    result = nvstore.get(9, 0, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);

    actual_len_bytes = 0;
    result = nvstore.get(7, 48, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(48, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[8]), nvstore_testing_buf_get, 48);

    actual_len_bytes = 0;
    result = nvstore.get(14, 89, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(89, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[9]), nvstore_testing_buf_get, 89);

    actual_len_bytes = 0;
    result = nvstore.get(15, 53, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(53, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[10]), nvstore_testing_buf_get, 53);

    result = nvstore.deinit();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);

    actual_len_bytes = 0;
    result = nvstore.get(10, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(15, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[4]), nvstore_testing_buf_get, 15);

    actual_len_bytes = 0;
    result = nvstore.get(11, 27, nvstore_testing_buf_get, actual_len_bytes); // no care about the buf and len values
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(27, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[5]), nvstore_testing_buf_get, 27);

    actual_len_bytes = 0;
    result = nvstore.get(13, 7, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(7, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[6]), nvstore_testing_buf_get, 7);

    actual_len_bytes = 0;
    result = nvstore.get(9, 0, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(0, actual_len_bytes);

    actual_len_bytes = 0;
    result = nvstore.get(7, 48, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(48, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[8]), nvstore_testing_buf_get, 48);

    actual_len_bytes = 0;
    result = nvstore.get(14, 89, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(89, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[9]), nvstore_testing_buf_get, 89);

    actual_len_bytes = 0;
    result = nvstore.get(15, 53, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, result);
    TEST_ASSERT_EQUAL(53, actual_len_bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(&(nvstore_testing_buf_set[10]), nvstore_testing_buf_get, 53);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);

    result = nvstore.get(NVSTORE_NUM_PREDEFINED_KEYS + 1, 64, nvstore_testing_buf_get, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_NOT_FOUND, result);

    delete[] nvstore_testing_buf_set;
    delete[] nvstore_testing_buf_get;
}

// This function calculates the stack size that needs to be allocated per thread in
// the multi-thread tests. Given minimal and maximal stack sizes, and based on the heap
// stats (automatically enabled in CI), the function checks whether each thread has at least
// the minimal stack size, otherwise it reduces the number of threads (something that may happen
// on low memory boards).
static void calc_thread_stack_size(int &num_threads, uint32_t min_size, uint32_t max_size,
                                   uint32_t &stack_size)
{
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);

    // reserved size (along with all other fields in heap stats) will be zero if
    // app is compiled without heap stats (typically local builds)
    if (!heap_stats.reserved_size) {
        stack_size = max_size;
        printf("Heap stats disabled in this build, so test may fail due to insufficient heap size\n");
        printf("If this happens, please build the test with heap stats enabled (-DMBED_HEAP_STATS_ENABLED=1)\n");
        return;
    }

    NVStore &nvstore = NVStore::get_instance();
    int page_size = nvstore.size() / nvstore.get_max_possible_keys();
    // Check if we can allocate enough stack size (per thread) for the current number of threads
    while (num_threads) {
        stack_size = (heap_stats.reserved_size - heap_stats.current_size) / num_threads - sizeof(rtos::Thread) - page_size;

        stack_size = std::min(stack_size, max_size);
        if (stack_size >= min_size) {
            return;
        }

        // Got here - stack not sufficient per thread. Reduce number of threads
        num_threads--;
    }
}

static void thread_test_check_key(uint16_t key)
{
    uint8_t get_buff[thr_test_max_data_size];
    int ret;
    uint16_t actual_len_bytes;
    NVStore &nvstore = NVStore::get_instance();

    ret = nvstore.get(key, thr_test_max_data_size, get_buff, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
    TEST_ASSERT_NOT_EQUAL(0, actual_len_bytes);

    for (int i = 0; i < thr_test_num_buffs; i++) {
        if (thr_test_data->sizes[key][i] != actual_len_bytes) {
            continue;
        }

        if (!memcmp(thr_test_data->buffs[key][i], get_buff, actual_len_bytes)) {
            return;
        }
    }

    // Got here - always assert
    TEST_ASSERT(0);

}

#ifdef MBED_CONF_RTOS_PRESENT
static void thread_test_worker()
{
    int ret;
    int buf_num, is_set;
    uint16_t key;
    NVStore &nvstore = NVStore::get_instance();

    while (!thr_test_data->stop_threads) {
        key = rand() % thr_test_data->max_keys;
        is_set = rand() % 10;
        if (is_set) {
            buf_num = rand() % thr_test_num_buffs;
            ret = nvstore.set(key, thr_test_data->sizes[key][buf_num], thr_test_data->buffs[key][buf_num]);
            TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
        } else {
            thread_test_check_key(key);
        }

        wait_ms(1);
    }
}
#endif

static void nvstore_multi_thread_test()
{
#ifdef MBED_CONF_RTOS_PRESENT
    int i;
    int num_threads = thr_test_num_threads;
    uint32_t stack_size;
    uint16_t size;
    uint16_t key;
    int ret;

    rtos::Thread **threads = new rtos::Thread*[num_threads];

    NVStore &nvstore = NVStore::get_instance();

    TEST_SKIP_UNLESS_MESSAGE(!nvstore_overlaps_code, "Test skipped. NVStore region overlaps code.");

    ret = nvstore.reset();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);

    thr_test_data = new thread_test_data_t;
    thr_test_data->max_keys = max_test_keys / 2;
    uint16_t max_possible_keys = nvstore.get_max_possible_keys();
    TEST_SKIP_UNLESS_MESSAGE(thr_test_data->max_keys < max_possible_keys,
                             "Not enough possible keys for test. Test skipped.");

    thr_test_data->stop_threads = false;
    for (key = 0; key < thr_test_data->max_keys; key++) {
        for (i = 0; i < thr_test_num_buffs; i++) {
            size = 1 + rand() % thr_test_max_data_size;
            thr_test_data->sizes[key][i] = size;
            thr_test_data->buffs[key][i] = new uint8_t[size + 1];
            gen_random(thr_test_data->buffs[key][i], size);
        }
        ret = nvstore.set(key, thr_test_data->sizes[key][0], thr_test_data->buffs[key][0]);
        TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
    }

    calc_thread_stack_size(num_threads, thr_test_min_stack_size, thr_test_max_stack_size, stack_size);
    if (!num_threads) {
        printf("Not enough heap space to run test. Test skipped\n");
        goto end;
    }

    for (i = 0; i < num_threads; i++) {
        threads[i] = new rtos::Thread((osPriority_t)((int)osPriorityBelowNormal - num_threads + i), stack_size);
        threads[i]->start(callback(thread_test_worker));
    }

    wait_ms(thr_test_num_secs * 1000);
    thr_test_data->stop_threads = true;

    wait_ms(1000);

    for (i = 0; i < num_threads; i++) {
        delete threads[i];
    }

    delete[] threads;

    ret = nvstore.deinit();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);

    ret = nvstore.init();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);

    for (key = 0; key < thr_test_data->max_keys; key++) {
        thread_test_check_key(key);
    }

end:
    for (key = 0; key < thr_test_data->max_keys; key++) {
        for (i = 0; i < thr_test_num_buffs; i++) {
            delete[] thr_test_data->buffs[key][i];
        }
    }

    delete thr_test_data;

    nvstore.reset();

#endif
}


static void race_test_worker(void *buf)
{
    int ret;
    NVStore &nvstore = NVStore::get_instance();

    ret = nvstore.set(race_test_key, race_test_data_size, buf);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
}

static void nvstore_race_test()
{
#ifdef MBED_CONF_RTOS_PRESENT
    int i;
    uint32_t stack_size;
    uint16_t initial_buf_size;
    int ret;
    rtos::Thread *threads[race_test_num_threads];
    uint8_t *get_buff, *buffs[race_test_num_threads];
    int num_threads = race_test_num_threads;
    uint16_t actual_len_bytes;

    TEST_SKIP_UNLESS_MESSAGE(!nvstore_overlaps_code, "Test skipped. NVStore region overlaps code.");

    NVStore &nvstore = NVStore::get_instance();

    ret = nvstore.reset();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);

    initial_buf_size = std::min((nvstore.size() - race_test_data_size) / 2, (size_t) race_test_data_size);
    uint8_t *initial_buf = new uint8_t[initial_buf_size];
    int num_sets = (nvstore.size() - race_test_data_size) / initial_buf_size;
    for (i = 0; i < num_sets; i++) {
        ret = nvstore.set(0, initial_buf_size, initial_buf);
        TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
    }
    delete[] initial_buf;

    get_buff = new uint8_t[race_test_data_size];

    calc_thread_stack_size(num_threads, race_test_min_stack_size, race_test_max_stack_size, stack_size);
    if (!num_threads) {
        printf("Not enough heap space to run test. Test skipped\n");
        goto end;
    }

    for (i = 0; i < num_threads; i++) {
        buffs[i] = new uint8_t[race_test_data_size];
        gen_random(buffs[i], race_test_data_size);
    }

    for (i = 0; i < num_threads; i++) {
        threads[i] = new rtos::Thread((osPriority_t)((int)osPriorityBelowNormal - num_threads + i), stack_size);
        threads[i]->start(callback(race_test_worker, (void *) buffs[i]));
        threads[i]->join();
    }

    ret = nvstore.get(race_test_key, race_test_data_size, get_buff, actual_len_bytes);
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, ret);
    TEST_ASSERT_EQUAL(race_test_data_size, actual_len_bytes);

    for (i = 0; i < num_threads; i++) {
        if (!memcmp(buffs[i], get_buff, actual_len_bytes)) {
            break;
        }
    }
    TEST_ASSERT_NOT_EQUAL(num_threads, i);

    for (i = 0; i < num_threads; i++) {
        delete threads[i];
        delete[] buffs[i];
    }

end:
    delete[] get_buff;
#endif
}



utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("NVStore: Basic functionality",  nvstore_basic_functionality_test, greentea_failure_handler),
    Case("NVStore: Race test",            nvstore_race_test,                greentea_failure_handler),
    Case("NVStore: Multiple thread test", nvstore_multi_thread_test,        greentea_failure_handler),
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
