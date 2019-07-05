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

#include "mbed.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "common_defines_test.h"
#include "file_test.h"

#if !INTEGRATION_TESTS
#error [NOT_SUPPORTED] integration tests not enabled for this target
#endif

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

void file_fn(size_t buffer)
{
    uint32_t thread_id = core_util_atomic_incr_u32(&thread_counter, 1);
    char filename[255] = { 0 };
    snprintf(filename, 255, "mbed-file-test-%d.txt", thread_id);
    file_test_write(filename, 0, story, sizeof(story), buffer);
    file_test_read(filename, 0, story, sizeof(story), buffer);
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
void file_2kb_fn()
{
    return file_fn(2048);
}
void file_4kb_fn()
{
    return file_fn(4096);
}

static control_t file_2_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    t1.start(file_4b_fn);
    t2.start(file_256b_fn);
    t1.join();
    t2.join();

    return CaseNext;
}

static control_t file_3_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    Thread t3;
    t1.start(file_256b_fn);
    t2.start(file_1kb_fn);
    t3.start(file_4kb_fn);
    t1.join();
    t2.join();
    t3.join();

    return CaseNext;
}

static control_t file_4_threads(const size_t call_count)
{
    thread_counter = 0;

    Thread t1;
    Thread t2;
    Thread t3;
    Thread t4;
    t1.start(file_256b_fn);
    t2.start(file_256b_fn);
    t3.start(file_1kb_fn);
    t4.start(file_2kb_fn);
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " format", test_format),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 2 files, buff 4b/256b", file_2_threads),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 3 files, buff 256b/1kb/4kb", file_3_threads),
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
