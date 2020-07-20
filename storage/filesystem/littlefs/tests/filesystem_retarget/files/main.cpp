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

void test_file_tests()
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

void test_simple_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        size = strlen("Hello World!\n");
        memcpy(wbuffer, "Hello World!\n", size);
        res = fwrite(wbuffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        size = strlen("Hello World!\n");
        res = fread(rbuffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(rbuffer, wbuffer, size);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_small_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = 32;
        size_t chunk = 31;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "smallavacado", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            for (size_t b = 0; b < chunk; b++) {
                buffer[b] = rand() & 0xff;
            }
            res = fwrite(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 32;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "smallavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_medium_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = 8192;
        size_t chunk = 31;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "mediumavacado", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            for (size_t b = 0; b < chunk; b++) {
                buffer[b] = rand() & 0xff;
            }
            res = fwrite(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 8192;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "mediumavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_large_file_test()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = 262144;
        size_t chunk = 31;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "largeavacado", "wb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            for (size_t b = 0; b < chunk; b++) {
                buffer[b] = rand() & 0xff;
            }
            res = fwrite(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 262144;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "largeavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_non_overlap_check()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        size_t size = 32;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "smallavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 8192;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "mediumavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    {
        size_t size = 262144;
        size_t chunk = 29;
        srand(0);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "largeavacado", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        for (size_t i = 0; i < size; i += chunk) {
            chunk = (chunk < size - i) ? chunk : size - i;
            res = fread(buffer, 1, chunk, fd[0]);
            TEST_ASSERT_EQUAL(chunk, res);
            for (size_t b = 0; b < chunk && i + b < size; b++) {
                res = buffer[b];
                TEST_ASSERT_EQUAL(rand() & 0xff, res);
            }
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_dir_check()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((dd[0] = opendir("/fs/" "/")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "hello");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "smallavacado");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "mediumavacado");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "largeavacado");
        TEST_ASSERT_EQUAL(0, res);
        res = ed->d_type;
        TEST_ASSERT_EQUAL(DT_REG, res);

        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
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
    Case("File tests", test_file_tests),
    Case("Simple file test", test_simple_file_test),
    Case("Small file test", test_small_file_test),
    Case("Medium file test", test_medium_file_test),
    Case("Large file test", test_large_file_test),
    Case("Non-overlap check", test_non_overlap_check),
    Case("Dir check", test_dir_check),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // MBED_TEST_BLOCKDEVICE
