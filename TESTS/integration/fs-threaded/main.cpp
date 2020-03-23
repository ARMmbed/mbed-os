/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

/*
 * Based on mbed-stress-test by Marcus Chang @ Arm Mbed - http://github.com/ARMmbed/mbed-stress-test
*/

#if !INTEGRATION_TESTS
#error [NOT_SUPPORTED] integration tests not enabled
#elif !MBED_CONF_RTOS_PRESENT
#error [NOT_SUPPORTED] integration tests require RTOS
#endif

#include "mbed.h"
#include "LittleFileSystem.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "common_defines_fs_test.h"
#include "file_test.h"

#ifdef MBED_CONF_APP_BASICS_TEST_FILENAME
#include MBED_CONF_APP_BASICS_TEST_FILENAME
#else
#include "sample.h"
#endif

#ifndef MBED_CONF_APP_TESTS_FS_SIZE
#define MBED_CONF_APP_TESTS_FS_SIZE (2*1024*1024)
#endif

using namespace utest::v1;

#if !defined(MBED_CONF_APP_NO_LED)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
void led_thread()
{
    led1 = !led1;
    led2 = !led1;
}
#endif

BlockDevice *bd = BlockDevice::get_default_instance();
SlicingBlockDevice sbd(bd, 0, MBED_CONF_APP_TESTS_FS_SIZE);
LittleFileSystem fs("sd");

constexpr char fname_prefix[] = "mbed-file-test-";
constexpr char fname_postfix[] = ".txt";

static uint32_t thread_counter = 0;

void print_memory_info()
{
#if defined MBED_HEAP_STATS_ENABLED &&  MBED_MEM_TRACING_ENABLED
    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    printf("Heap size: %lu / %lu bytes\r\n",
           heap_stats.current_size,
           heap_stats.reserved_size);
#endif
}

void file_fn(size_t *block_size)
{
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    char filename[255] = { 0 };
    snprintf(filename, 255, "%s%" PRIu32 "%s", fname_prefix, thread_id, fname_postfix);
    file_test_write(filename, 0, story, sizeof(story), *block_size);
    file_test_read(filename, 0, story, sizeof(story), *block_size);
    print_memory_info();
}

static control_t file_2_threads(const size_t call_count)
{
    size_t block_size1 = 4;
    size_t block_size2 = 256;

    Thread t1;
    Thread t2;
    t1.start(callback(file_fn, &block_size1));
    t2.start(callback(file_fn, &block_size2));
    t1.join();
    t2.join();

    return CaseNext;
}

static control_t file_4_threads(const size_t call_count)
{
    size_t block_size1 = 4;
    size_t block_size2 = 256;
    size_t block_size3 = 1024;
    size_t block_size4 = 4096;

    Thread t1;
    Thread t2;
    Thread t3;
    Thread t4;
    t1.start(callback(file_fn, &block_size1));
    t2.start(callback(file_fn, &block_size2));
    t3.start(callback(file_fn, &block_size3));
    t4.start(callback(file_fn, &block_size4));
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return CaseNext;
}

static control_t file_5_threads(const size_t call_count)
{
    size_t block_size1 = 4;
    size_t block_size2 = 256;
    size_t block_size3 = 1024;
    size_t block_size4 = 4096;
    size_t block_size5 = 4096;

    Thread t1;
    Thread t2;
    Thread t3;
    Thread t4;
    Thread t5;
    t1.start(callback(file_fn, &block_size1));
    t2.start(callback(file_fn, &block_size2));
    t3.start(callback(file_fn, &block_size3));
    t4.start(callback(file_fn, &block_size4));
    t5.start(callback(file_fn, &block_size5));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return CaseNext;
}

utest::v1::status_t test_setup_handler(const size_t number_of_cases)
{
    GREENTEA_SETUP(10 * 60, "default_auto");

    /* Format only once per each test run */
    int format_err = fs.format(&sbd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, format_err, "could not format block device");

    int mount_err = fs.mount(&sbd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mount_err, "could not mount block device");

    return greentea_test_setup_handler(number_of_cases);
}

void test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    int mount_err = fs.unmount();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mount_err, "could not unmount block device");

    greentea_test_teardown_handler(passed, failed, failure);
}

utest::v1::status_t case_setup_handler(const Case *const source, const size_t index_of_case)
{
    thread_counter = 0;

    return greentea_case_setup_handler(source, index_of_case);
}

utest::v1::status_t case_teardown_handler(const Case *const source,
                                          const size_t passed,
                                          const size_t failed,
                                          const failure_t reason)
{
    char filename[255] = { 0 };

    for (uint32_t idx = 1; idx <= thread_counter; idx++) {
        snprintf(filename, 255, "%s%" PRIu32 "%s", fname_prefix, idx, fname_postfix);
        int removal_err = fs.remove(filename);
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, removal_err, "could not remove file");
    }

    return greentea_case_teardown_handler(source, passed, failed, reason);
}



Case cases[] = {
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 2 files, block size 4B/256B",
         case_setup_handler,
         file_2_threads,
         case_teardown_handler),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 4 files, block size 4B/256B/1KiB/4KiB",
         case_setup_handler,
         file_4_threads,
         case_teardown_handler),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 5 files, block size 4B/256B/1KiB/4KiB/4KiB",
         case_setup_handler,
         file_5_threads,
         case_teardown_handler),
};

Specification specification(test_setup_handler, cases, test_teardown_handler);

int main()
{
    //Create a thread to blink an LED and signal that the device is alive
#if !defined(MBED_CONF_APP_NO_LED)
    Ticker t;
    t.attach(led_thread, 0.5);
#endif

    return !Harness::run(specification);
}
