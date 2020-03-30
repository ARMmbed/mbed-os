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

static control_t test_block_size_1(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 1);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 1);

    return CaseNext;
}
static control_t test_block_size_4(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 4);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 4);

    return CaseNext;
}
static control_t test_block_size_16(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 16);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 16);

    return CaseNext;
}
static control_t test_block_size_64(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 64);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 64);

    return CaseNext;
}
static control_t test_block_size_256(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 256);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 256);

    return CaseNext;
}
static control_t test_block_size_1k(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 1024);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 1024);

    return CaseNext;
}
static control_t test_block_size_4k(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 4 * 1024);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 4 * 1024);

    return CaseNext;
}
static control_t test_block_size_16k(const size_t call_count)
{
    file_test_write("mbed-file-test-0.txt", 0, story, sizeof(story), 16 * 1024);
    file_test_read("mbed-file-test-0.txt", 0, story, sizeof(story), 16 * 1024);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(5 * 60, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " format", test_format),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff     1", test_block_size_1),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff     4", test_block_size_4),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff    16", test_block_size_16),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff    64", test_block_size_64),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff   256", test_block_size_256),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff  1024", test_block_size_1k),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff  4096", test_block_size_4k),
    Case(TEST_BLOCK_DEVICE_TYPE "+" TEST_FILESYSTEM_TYPE " 1 file, buff 16384", test_block_size_16k),
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
