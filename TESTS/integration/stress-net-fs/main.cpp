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
#error [NOT_SUPPORTED] integration tests not enabled for this target
#elif !MBED_CONF_RTOS_PRESENT
#error [NOT_SUPPORTED] integration tests require RTOS
#else

#include "mbed.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "common_defines_fs_test.h"
#include "common_defines_net_test.h"
#include "download_test.h"
#include "file_test.h"
#include <string>


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

#define MAX_RETRIES 3
NetworkInterface *interface = NULL;

static control_t setup_network(const size_t call_count)
{
    interface = NetworkInterface::get_default_instance();
    TEST_ASSERT_NOT_NULL_MESSAGE(interface, "failed to initialize network");

    nsapi_error_t err = -1;
    for (int tries = 0; tries < MAX_RETRIES; tries++) {
        err = interface->connect();
        if (err == NSAPI_ERROR_OK) {
            break;
        } else {
            tr_error("[ERROR] Connecting to network. Retrying %d of %d...", tries, MAX_RETRIES);
        }
    }
    TEST_ASSERT_EQUAL(NSAPI_ERROR_OK, err);

    return CaseNext;
}

BlockDevice *bd = BlockDevice::get_default_instance();
SlicingBlockDevice sd(bd, 0, MBED_CONF_APP_TESTS_FS_SIZE);
#if TEST_USE_FILESYSTEM == FS_FAT
FATFileSystem fs("sd");
#else
LittleFileSystem fs("sd");
#endif

static control_t test_format(const size_t call_count)
{
    int format_err = fs.format(&sd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, format_err, "could not format block device");

    int mount_err = fs.mount(&sd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mount_err, "could not mount block device");

    return CaseNext;
}

static uint32_t thread_counter = 0;

void download_fn()
{
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    download_test(interface, story, sizeof(story), 256, thread_id);
}
void file_fn(size_t buffer)
{
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    char filename[255] = { 0 };
    snprintf(filename, 255, "mbed-file-test-%" PRIu32 ".txt", thread_id);
    file_test_write(filename, 0, story, sizeof(story), buffer);
    file_test_read(filename, 0, story, sizeof(story), buffer);
}
void file_1b_fn()
{
    return file_fn(1);
}
void file_4b_fn()
{
    return file_fn(4);
}
void file_256b_fn()
{
    return file_fn(256);
}
void file_1kb_fn()
{
    return file_fn(1024);
}

static control_t stress_1_thread(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    t1.start(download_fn);
    t1.join();
    t1.start(file_1kb_fn);
    t1.join();

    return CaseNext;
}

static control_t stress_2_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    t1.start(file_1kb_fn);
    ThisThread::sleep_for(1);
    t2.start(download_fn);
    t2.join();
    t1.join();

    return CaseNext;
}

static control_t stress_3_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    Thread t3;
    t1.start(file_256b_fn);
    t2.start(file_1kb_fn);
    ThisThread::sleep_for(1);
    t3.start(download_fn);
    t3.join();
    t2.join();
    t1.join();

    return CaseNext;
}

template <uint32_t size>
void test_malloc()
{

    void *bufferTest = NULL;
    TEST_ASSERT_MESSAGE(size > 0, "Size must not be zero for test");
    tr_info("Allocating %d bytes", (int)size);
    bufferTest = malloc(size);
    TEST_ASSERT(bufferTest != NULL);
    free(bufferTest);
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(12 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case(TEST_NETWORK_TYPE " network setup", setup_network),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " format", test_format),
    Case("Test memory allocation of 10 K bytes", test_malloc<TEST_MEMORY_SIZE_10K>),
    Case("Test memory allocation of 20 K bytes", test_malloc<TEST_MEMORY_SIZE_20K>),
    Case("Test memory allocation of 40 K bytes", test_malloc<TEST_MEMORY_SIZE_40K>),
    Case("Test memory allocation of 60 K bytes", test_malloc<TEST_MEMORY_SIZE_60K>),
#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != CELLULAR
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE "+" TEST_NETWORK_TYPE " 1 thread, dl, file seq.", stress_1_thread),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE "+" TEST_NETWORK_TYPE " 2 threads, dl, 1kb", stress_2_threads),
#endif
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE "+" TEST_NETWORK_TYPE " 3 threads, dl, 256b, 1kb", stress_3_threads),
};

Specification specification(greentea_setup, cases);

int main()
{
    //Create a thread to blink an LED and signal that the device is alive
#if !defined(MBED_CONF_APP_NO_LED)
    Ticker t;
    t.attach(led_thread, 0.5);
#endif

    return !Harness::run(specification);
}
#endif // !INTEGRATION_TESTS
