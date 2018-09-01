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

void test_seek_tests()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = MBED_TEST_FILESYSTEM::format(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = mkdir("/fs/" "hello", 0777);
        TEST_ASSERT_EQUAL(0, res);
        for (int i = 0; i < 132; i++) {
            sprintf((char*)buffer, "/fs/" "hello/kitty%d", i);
            res = !((fd[0] = fopen((char*)buffer,
                    "ab")) != NULL);
            TEST_ASSERT_EQUAL(0, res);
    
            size = strlen("kittycatcat");
            memcpy(buffer, "kittycatcat", size);
            for (int j = 0; j < 132; j++) {
                fwrite(buffer, 1, size, fd[0]);
            }
            res = fclose(fd[0]);
            TEST_ASSERT_EQUAL(0, res);
        }
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_simple_dir_seek()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((dd[0] = opendir("/fs/" "hello")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        int i;
        for (i = 0; i < 4; i++) {
            sprintf((char*)buffer, "kitty%d", i);
            res = ((ed = readdir(dd[0])) != NULL);
            TEST_ASSERT_EQUAL(1, res);
            res = strcmp(ed->d_name, (char*)buffer);
            TEST_ASSERT_EQUAL(0, res);
            pos = telldir(dd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
    
        seekdir(dd[0], pos);
        sprintf((char*)buffer, "kitty%d", i);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
    
        rewinddir(dd[0]);
        sprintf((char*)buffer, "kitty%d", 0);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
    
        seekdir(dd[0], pos);
        sprintf((char*)buffer, "kitty%d", i);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_large_dir_seek()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((dd[0] = opendir("/fs/" "hello")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        int i;
        for (i = 0; i < 128; i++) {
            sprintf((char*)buffer, "kitty%d", i);
            res = ((ed = readdir(dd[0])) != NULL);
            TEST_ASSERT_EQUAL(1, res);
            res = strcmp(ed->d_name, (char*)buffer);
            TEST_ASSERT_EQUAL(0, res);
            pos = telldir(dd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
    
        seekdir(dd[0], pos);
        sprintf((char*)buffer, "kitty%d", i);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
    
        rewinddir(dd[0]);
        sprintf((char*)buffer, "kitty%d", 0);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, ".");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, "..");
        TEST_ASSERT_EQUAL(0, res);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
    
        seekdir(dd[0], pos);
        sprintf((char*)buffer, "kitty%d", i);
        res = ((ed = readdir(dd[0])) != NULL);
        TEST_ASSERT_EQUAL(1, res);
        res = strcmp(ed->d_name, (char*)buffer);
        TEST_ASSERT_EQUAL(0, res);
        res = closedir(dd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_simple_file_seek()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        size = strlen("kittycatcat");
        for (int i = 0; i < 4; i++) {
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = memcmp(buffer, "kittycatcat", size);
            TEST_ASSERT_EQUAL(0, res);
            pos = ftell(fd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        rewind(fd[0]);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        res = fseek(fd[0], 0, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_large_file_seek()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "rb")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        size = strlen("kittycatcat");
        for (int i = 0; i < 128; i++) {
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = memcmp(buffer, "kittycatcat", size);
            TEST_ASSERT_EQUAL(0, res);
            pos = ftell(fd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        rewind(fd[0]);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        res = fseek(fd[0], 0, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_simple_file_seek_and_write()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "r+b")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        size = strlen("kittycatcat");
        for (int i = 0; i < 4; i++) {
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = memcmp(buffer, "kittycatcat", size);
            TEST_ASSERT_EQUAL(0, res);
            pos = ftell(fd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
    
        memcpy(buffer, "doggodogdog", size);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fwrite(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "doggodogdog", size);
        TEST_ASSERT_EQUAL(0, res);
    
        rewind(fd[0]);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "doggodogdog", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        res = fseek(fd[0], 0, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_large_file_seek_and_write()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "r+b")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        off_t pos;
        size = strlen("kittycatcat");
        for (int i = 0; i < 128; i++) {
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            if (i != 4) {
                res = memcmp(buffer, "kittycatcat", size);
                TEST_ASSERT_EQUAL(0, res);
            }
            pos = ftell(fd[0]);
        }
        res = pos >= 0;
        TEST_ASSERT_EQUAL(1, res);
    
        memcpy(buffer, "doggodogdog", size);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fwrite(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "doggodogdog", size);
        TEST_ASSERT_EQUAL(0, res);
    
        rewind(fd[0]);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], pos, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "doggodogdog", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], -size, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "kittycatcat", size);
        TEST_ASSERT_EQUAL(0, res);
    
        res = fseek(fd[0], 0, SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_boundary_seek_and_write()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "r+b")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        size = strlen("hedgehoghog");
        const off_t offsets[] = {512, 1020, 513, 1021, 511, 1019};
    
        for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]); i++) {
            off_t off = offsets[i];
            memcpy(buffer, "hedgehoghog", size);
            res = fseek(fd[0], off, SEEK_SET);
            TEST_ASSERT_EQUAL(0, res);
            res = fwrite(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = fseek(fd[0], off, SEEK_SET);
            TEST_ASSERT_EQUAL(0, res);
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = memcmp(buffer, "hedgehoghog", size);
            TEST_ASSERT_EQUAL(0, res);
            res = fseek(fd[0], 0, SEEK_SET);
            TEST_ASSERT_EQUAL(0, res);
            res = fread(buffer, 1, size, fd[0]);
            TEST_ASSERT_EQUAL(size, res);
            res = memcmp(buffer, "kittycatcat", size);
            TEST_ASSERT_EQUAL(0, res);
            res = fflush(fd[0]);
            TEST_ASSERT_EQUAL(0, res);
        }
        res = fclose(fd[0]);
        TEST_ASSERT_EQUAL(0, res);
        res = fs.unmount();
        TEST_ASSERT_EQUAL(0, res);
    }

    res = bd.deinit();
    TEST_ASSERT_EQUAL(0, res);
}

void test_out_of_bounds_seek()
{
    int res = bd.init();
    TEST_ASSERT_EQUAL(0, res);

    {
        res = fs.mount(&bd);
        TEST_ASSERT_EQUAL(0, res);
        res = !((fd[0] = fopen("/fs/" "hello/kitty42", "r+b")) != NULL);
        TEST_ASSERT_EQUAL(0, res);
    
        size = strlen("kittycatcat");
        res = fseek(fd[0], 0, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);
        res = ftell(fd[0]);
        TEST_ASSERT_EQUAL(132*size, res);
        res = fseek(fd[0], (132+4)*size,
                SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(0, res);
    
        memcpy(buffer, "porcupineee", size);
        res = fwrite(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = fseek(fd[0], (132+4)*size,
                SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "porcupineee", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fseek(fd[0], 132*size,
                SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);
        res = fread(buffer, 1, size, fd[0]);
        TEST_ASSERT_EQUAL(size, res);
        res = memcmp(buffer, "\0\0\0\0\0\0\0\0\0\0\0", size);
        TEST_ASSERT_EQUAL(0, res);
        res = fclose(fd[0]);
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
    Case("Seek tests", test_seek_tests),
    Case("Simple dir seek", test_simple_dir_seek),
    Case("Large dir seek", test_large_dir_seek),
    Case("Simple file seek", test_simple_file_seek),
    Case("Large file seek", test_large_file_seek),
    Case("Simple file seek and write", test_simple_file_seek_and_write),
    Case("Large file seek and write", test_large_file_seek_and_write),
    Case("Boundary seek and write", test_boundary_seek_and_write),
    Case("Out-of-bounds seek", test_out_of_bounds_seek),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
