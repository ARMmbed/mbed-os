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
#endif

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


// tests

void test_directory_tests()
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

void test_root_directory()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "/");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_directory_creation()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("potato", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_file_creation()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].open(&fs, "burito", O_CREAT | O_WRONLY);
        TEST_ASSERT_EQUAL(0, res);
        res = file[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_directory_iteration()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "/");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "potato");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "burito");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_directory_failures()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("potato", 0777);
        TEST_ASSERT_EQUAL(-EEXIST, res);
        res = dir[0].open(&fs, "tomato");
        TEST_ASSERT_EQUAL(-ENOENT, res);
        res = dir[0].open(&fs, "burito");
        TEST_ASSERT_EQUAL(-ENOTDIR, res);
        res = file[0].open(&fs, "tomato", O_RDONLY);
        TEST_ASSERT_EQUAL(-ENOENT, res);
        res = file[0].open(&fs, "potato", O_RDONLY);
        TEST_ASSERT_EQUAL(-EISDIR, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_nested_directories()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("potato/baked", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("potato/sweet", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("potato/fried", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "potato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "baked");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "fried");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_multi_block_directory()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("cactus", 0777);
        TEST_ASSERT_EQUAL(0, res);
        for (int i = 0; i < 128; i++) {
            sprintf((char*)buffer, "cactus/test%d", i);
            res = fs.mkdir((char*)buffer, 0777);
            TEST_ASSERT_EQUAL(0, res);
        }
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "cactus");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        for (int i = 0; i < 128; i++) {
            sprintf((char*)buffer, "test%d", i);
            res = dir[0].read(&ent);
            TEST_ASSERT_EQUAL(1, res);
            res = strcmp(ent.d_name, (char*)buffer);
            TEST_ASSERT_EQUAL(0, res);
        }
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_directory_remove()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.remove("potato");
        TEST_ASSERT_EQUAL(-ENOTEMPTY, res);
        res = fs.remove("potato/sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.remove("potato/baked");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.remove("potato/fried");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "potato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.remove("potato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "/");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "burito");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "cactus");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "/");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "burito");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "cactus");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_directory_rename()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("coldpotato", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("coldpotato/baked", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("coldpotato/sweet", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("coldpotato/fried", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("coldpotato", "hotpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "hotpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "baked");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "fried");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("warmpotato", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("warmpotato/mushy", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("hotpotato", "warmpotato");
        TEST_ASSERT_EQUAL(-ENOTEMPTY, res);
        res = fs.remove("warmpotato/mushy");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("hotpotato", "warmpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "warmpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "baked");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "fried");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mkdir("coldpotato", 0777);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("warmpotato/baked", "coldpotato/baked");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("warmpotato/sweet", "coldpotato/sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.rename("warmpotato/fried", "coldpotato/fried");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.remove("coldpotato");
        TEST_ASSERT_EQUAL(-ENOTEMPTY, res);
        res = fs.remove("warmpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].open(&fs, "coldpotato");
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "baked");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "sweet");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ent.d_name, "fried");
        TEST_ASSERT_EQUAL(0, res);
        res = ent.d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = dir[0].read(&ent);
        TEST_ASSERT_EQUAL(0, res);
        res = dir[0].close();
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
    Case("Directory tests", test_directory_tests),
    Case("Root directory", test_root_directory),
    Case("Directory creation", test_directory_creation),
    Case("File creation", test_file_creation),
    Case("Directory iteration", test_directory_iteration),
    Case("Directory failures", test_directory_failures),
    Case("Nested directories", test_nested_directories),
    Case("Multi-block directory", test_multi_block_directory),
    Case("Directory remove", test_directory_remove),
    Case("Directory rename", test_directory_rename),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
