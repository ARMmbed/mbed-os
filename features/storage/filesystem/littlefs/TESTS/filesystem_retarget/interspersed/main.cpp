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


// tests

void test_parallel_tests()
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

void test_parallel_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "a", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[1] = fopen("/fs/" "b", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[2] = fopen("/fs/" "c", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[3] = fopen("/fs/" "d", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 10; i++) {
            res = fwrite((const void *)"a", 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"b", 1, 1, fd[1]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"c", 1, 1, fd[2]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"d", 1, 1, fd[3]);
            TEST_ASSERT_EQUAL(1, res);
        }

        fclose(fd[0]);
        fclose(fd[1]);
        fclose(fd[2]);
        fclose(fd[3]);
        res = !((dd[0] = opendir("/fs/" "/")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "a");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "b");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "c");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "d");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "a", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[1] = fopen("/fs/" "b", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[2] = fopen("/fs/" "c", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[3] = fopen("/fs/" "d", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 10; i++) {
            res = fread(buffer, 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('a', res);
            res = fread(buffer, 1, 1, fd[1]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('b', res);
            res = fread(buffer, 1, 1, fd[2]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('c', res);
            res = fread(buffer, 1, 1, fd[3]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('d', res);
        }

        fclose(fd[0]);
        fclose(fd[1]);
        fclose(fd[2]);
        fclose(fd[3]);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_parallel_remove_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "e", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 5; i++) {
            res = fwrite((const void *)"e", 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
        }
        res = remove("/fs/" "a");
        TEST_ASSERT_EQUAL(0, res);
        res = remove("/fs/" "b");
        TEST_ASSERT_EQUAL(0, res);
        res = remove("/fs/" "c");
        TEST_ASSERT_EQUAL(0, res);
        res = remove("/fs/" "d");
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 5; i++) {
            res = fwrite((const void *)"e", 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
        }

        fclose(fd[0]);
        res = !((dd[0] = opendir("/fs/" "/")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "e");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "e", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 10; i++) {
            res = fread(buffer, 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('e', res);
        }

        fclose(fd[0]);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_remove_inconveniently_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "e", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[1] = fopen("/fs/" "f", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[2] = fopen("/fs/" "g", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 5; i++) {
            res = fwrite((const void *)"e", 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"f", 1, 1, fd[1]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"g", 1, 1, fd[2]);
            TEST_ASSERT_EQUAL(1, res);
        }
        res = remove("/fs/" "f");
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 5; i++) {
            res = fwrite((const void *)"e", 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"f", 1, 1, fd[1]);
            TEST_ASSERT_EQUAL(1, res);
            res = fwrite((const void *)"g", 1, 1, fd[2]);
            TEST_ASSERT_EQUAL(1, res);
        }

        fclose(fd[0]);
        fclose(fd[1]);
        fclose(fd[2]);
        res = !((dd[0] = opendir("/fs/" "/")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_DIR, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "e");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "g");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "e", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[1] = fopen("/fs/" "g", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);

        for (int i = 0; i < 10; i++) {
            res = fread(buffer, 1, 1, fd[0]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('e', res);
            res = fread(buffer, 1, 1, fd[1]);
            TEST_ASSERT_EQUAL(1, res);
            res = buffer[0];
            TEST_ASSERT_EQUAL('g', res);
        }

        fclose(fd[0]);
        fclose(fd[1]);
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
    Case("Parallel tests", test_parallel_tests),
    Case("Parallel file test", test_parallel_file_test),
    Case("Parallel remove file test", test_parallel_remove_file_test),
    Case("Remove inconveniently test", test_remove_inconveniently_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // MBED_TEST_BLOCKDEVICE
