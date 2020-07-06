/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>
#include <errno.h>

using namespace utest::v1;

// test configuration
#ifndef MBED_TEST_FILESYSTEM
#define MBED_TEST_FILESYSTEM LittleFileSystem
#endif

#ifndef MBED_TEST_FILESYSTEM_DECL
#define MBED_TEST_FILESYSTEM_DECL MBED_TEST_FILESYSTEM fs("fs")
#endif

#ifndef MBED_TEST_BLOCKDEVICE
#error [NOT_SUPPORTED] Non-volatile block device required
#else

#ifndef MBED_TEST_BLOCKDEVICE_DECL
#define MBED_TEST_BLOCKDEVICE_DECL MBED_TEST_BLOCKDEVICE bd
#endif

#ifndef MBED_TEST_FILES
#define MBED_TEST_FILES 4
#endif

#ifndef MBED_TEST_DIRS
#define MBED_TEST_DIRS 4
#endif

#ifndef MBED_TEST_BUFFER
#define MBED_TEST_BUFFER 8192
#endif

#ifndef MBED_TEST_TIMEOUT
#define MBED_TEST_TIMEOUT 480
#endif


// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_FILESYSTEM)
#include INCLUDE(MBED_TEST_BLOCKDEVICE)

MBED_TEST_FILESYSTEM_DECL;
MBED_TEST_BLOCKDEVICE_DECL;

Dir dir[MBED_TEST_DIRS];
File file[MBED_TEST_FILES];
DIR *dd[MBED_TEST_DIRS];
FILE *fd[MBED_TEST_FILES];
struct dirent ent;
struct dirent *ed;
size_t size;
uint8_t buffer[MBED_TEST_BUFFER];
uint8_t rbuffer[MBED_TEST_BUFFER];
uint8_t wbuffer[MBED_TEST_BUFFER];


// tests for integration level format operations

void test_format()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = MBED_TEST_FILESYSTEM::format(&bd);
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_mount()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_bad_mount()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = bd.erase(0, 2 * bd.get_erase_size());
        TEST_ASSERT_EQUAL(0, res);
        memset(buffer, 0, bd.get_program_size());
        for (int i = 0; i < 2 * bd.get_erase_size(); i += bd.get_program_size()) {
            res = bd.program(buffer, i, bd.get_program_size());
            TEST_ASSERT_EQUAL(0, res);
        }
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_NOT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_bad_mount_then_reformat()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_NOT_EQUAL(0, res);

        res = fs.reformat(&bd);
        TEST_ASSERT_EQUAL(0, res);

        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_good_mount_then_reformat()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);

        res = fs.reformat(&bd);
        TEST_ASSERT_EQUAL(0, res);

        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}


// test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(MBED_TEST_TIMEOUT, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test format", test_format),
    Case("Test mount", test_mount),
    Case("Test bad mount", test_bad_mount),
    Case("Test bad mount than reformat", test_bad_mount_then_reformat),
    Case("Test good mount than reformat", test_good_mount_then_reformat),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // MBED_TEST_BLOCKDEVICE
