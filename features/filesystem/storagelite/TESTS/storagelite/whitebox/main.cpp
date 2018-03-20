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

#include "StorageLite.h"
#include "StorageLiteFS.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "Timer.h"
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "mbed_retarget.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

// Define this if you want to run the test locally (requires SPIF/SD block device)
#undef LOCAL_TEST

#if !STORAGELITE_ENABLED
#error [NOT_SUPPORTED] StorageLite needs to be enabled for this test
#endif

// MBED_TEST_SIM_BLOCKDEVICE is used here only to filter out inappropriate boards (little RAM etc.)
#if !defined(MBED_TEST_SIM_BLOCKDEVICE) && !defined(LOCAL_TEST)
#error [NOT_SUPPORTED] StorageLite test not supported on this platform
#endif

#ifdef LOCAL_TEST

#define TEST_SPIF
#undef TEST_SD

#ifdef TEST_SPIF
#include "SPIFBlockDevice.h"
#ifdef TARGET_K82F
SPIFBlockDevice bd(PTE2, PTE4, PTE1, PTE5);
#else
SPIFBlockDevice bd(D11, D12, D13, D8);
#endif
SlicingBlockDevice flash_bd(&bd, 0, 64 * 4096);

#elif defined(TEST_SD)
#include "SDBlockDevice.h"
SDBlockDevice bd(PTE3, PTE1, PTE2, PTE4);
SlicingBlockDevice slice_bd(&bd, 0, 512 * 512);
FlashSimBlockDevice flash_bd(&slice_bd);

#else
// Satisfy compiler
HeapBlockDevice bd(512, 1, 1, 4);
FlashSimBlockDevice flash_bd(&bd);
#endif

#else
// Satisfy compiler
HeapBlockDevice bd(512, 1, 1, 4);
FlashSimBlockDevice flash_bd(&bd);
#endif

using namespace utest::v1;

typedef struct {
    size_t size;
    size_t read_size;
    size_t prog_size;
    size_t erase_size;
} bd_params_t;


static const char * const file1_name = "file1";
static const char * const file1_val1 = "val1";
static const char * const file2_name = "name of file2";
static const char * const file2_val1 = "val3";
static const char * const file2_val2 = "val2 of file 2";
static const char * const file2_val3 = "Val1 value of file 2            ";
static const char * const file3_name = "This is the name of file3";
static const char * const file3_val1 = "Data value of file 3 is the following";
static const char * const file4_name = "This is the name of file4";
static const char * const file4_val1 = "Is this the value of file 4?";
static const char * const file4_val2 = "What the hell is the value of file 4, god damn it!";


static void white_box_test()
{
    bd_params_t bd_params[] = {
        {8192,     1,  16, 4096}, // Standard
        {4096 * 4, 1,   1, 4096}, // K82F like
        {8192,    64, 128, 2048}, // Awkward read and program sizes, both larger than header size
        {2048,     1,   4,  128}, // Small sector and total sizes
        {0,        0,   0,    0}, // Place holder for real non volatile device
    };

    int num_bds = sizeof(bd_params) / sizeof(bd_params_t);
    uint8_t get_buf[256];
    size_t actual_name_size;
    size_t actual_data_size;
    int result;
    uint32_t handle;
    mbed::Timer timer;
    int elapsed;

#if defined(TEST_SPIF) || defined(TEST_SD)
    flash_bd.init();
    bd_params[num_bds - 1].size       = flash_bd.size();
    bd_params[num_bds - 1].read_size  = flash_bd.get_read_size();
    bd_params[num_bds - 1].prog_size  = flash_bd.get_program_size();
    bd_params[num_bds - 1].erase_size = flash_bd.get_erase_size();
    flash_bd.deinit();
#endif

    timer.start();
    for (int bd_num = 0; bd_num < num_bds; bd_num++) {
        bd_params_t *bdp = &bd_params[bd_num];
        if (!bdp->size) {
            break;
        }
        printf("\n\nBD #%d: size %d, read %d, prog %d, erase %d\n",
               bd_num, bdp->size, bdp->read_size, bdp->prog_size, bdp->erase_size);
        HeapBlockDevice heap_bd(bdp->size, bdp->read_size, bdp->prog_size, bdp->erase_size);
        FlashSimBlockDevice flash_sim_bd(&heap_bd);
        BlockDevice *test_bd;
        if (bd_num == num_bds - 1) {
            test_bd = &flash_bd;
            // Required for deinit
            flash_sim_bd.init();
        } else {
            test_bd = &flash_sim_bd;
        }

        StorageLite stlite;

        timer.reset();
        result = stlite.init(test_bd, 128);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        elapsed = timer.read_ms();
        printf("Elapsed time for init %d ms\n", elapsed);

        printf("StorageLite size is %u bytes\n", stlite.size());

        timer.reset();
        result = stlite.reset();
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        elapsed = timer.read_ms();
        printf("Elapsed time for reset is %d ms\n", elapsed);

        result = stlite.set(file1_name, file1_val1,
                            strlen(file1_val1), StorageLite::encrypt_flag);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        result = stlite.set(file2_name, file2_val1,
                            strlen(file2_val1), StorageLite::update_backup_flag);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        result = stlite.set(file2_name, strlen(file2_name), file2_val2,
                            strlen(file2_val2), StorageLite::update_backup_flag | StorageLite::rollback_protect_flag);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        timer.reset();
        result = stlite.set(file2_name, file2_val3,
                            strlen(file2_val3), StorageLite::encrypt_flag | StorageLite::rollback_protect_flag);
        elapsed = timer.read_ms();
        printf("Elapsed time for set is %d ms\n", elapsed);

        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        result = stlite.set(file3_name, file3_val1,
                            strlen(file3_val1), StorageLite::update_backup_flag);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        result = stlite.get(file3_name, get_buf,
                            sizeof(get_buf), actual_data_size);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(file3_val1), actual_data_size);
        TEST_ASSERT_EQUAL(0, memcmp(file3_val1, get_buf, strlen(file3_val1)));

        for (int j = 0; j < 2; j++) {
            result = stlite.set(file4_name, file4_val1,
                                strlen(file4_val1), StorageLite::encrypt_flag | StorageLite::update_backup_flag);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

            result = stlite.set(file4_name, strlen(file4_name), file4_val2,
                                strlen(file4_val2), StorageLite::encrypt_flag);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        }

        result = stlite.remove(file3_name);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        for (int i = 0; i < 2; i++) {
            printf("%s deinit/init\n", i ? "After" : "Before");
            result = stlite.get(file1_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file1_val1), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file1_val1, get_buf, strlen(file1_val1)));

            timer.reset();
            result = stlite.get(file2_name, strlen(file2_name), get_buf,
                                sizeof(get_buf), actual_data_size);
            elapsed = timer.read_ms();
            printf("Elapsed time for get is %d ms\n", elapsed);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file2_val3), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file2_val3, get_buf, strlen(file2_val3)));

            result = stlite.get(file3_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, result);

            result = stlite.get(file4_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file4_val2), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file4_val2, get_buf, strlen(file4_val2)));

            result = stlite.get_first_file(get_buf, sizeof(get_buf), actual_name_size, handle);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file1_name), actual_name_size);
            TEST_ASSERT_EQUAL(0, memcmp(file1_name, get_buf, strlen(file1_name)));

            result = stlite.get_next_file(get_buf, sizeof(get_buf), actual_name_size, handle);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file2_name), actual_name_size);
            TEST_ASSERT_EQUAL(0, memcmp(file2_name, get_buf, strlen(file2_name)));

            result = stlite.get_next_file(get_buf, sizeof(get_buf), actual_name_size, handle);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file4_name), actual_name_size);
            TEST_ASSERT_EQUAL(0, memcmp(file4_name, get_buf, strlen(file4_name)));

            result = stlite.get_next_file(get_buf, sizeof(get_buf), actual_name_size, handle);
            TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, result);

            result = stlite.deinit();
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

            timer.reset();
            result = stlite.init(test_bd);
            elapsed = timer.read_ms();
            printf("Elapsed time for init is %d ms\n", elapsed);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        }

        timer.reset();
        result = stlite.factory_reset();
        elapsed = timer.read_ms();
        printf("Elapsed time for factory reset is %d ms\n", elapsed);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        printf("Factory reset Set/get tests\n");
        for (int i = 0; i < 2; i++) {
            printf("%s deinit/init\n", i ? "After" : "Before");
            result = stlite.get(file1_name, strlen(file1_name), get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, result);

            result = stlite.get(file2_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file2_val2), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file2_val2, get_buf, strlen(file2_val2)));

            result = stlite.get(file3_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file3_val1), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file3_val1, get_buf, strlen(file3_val1)));

            result = stlite.get(file4_name, get_buf,
                                sizeof(get_buf), actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(file4_val1), actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(file4_val1, get_buf, strlen(file4_val1)));

            result = stlite.deinit();
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

            result = stlite.init(test_bd);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        }

        result = stlite.deinit();
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
    }
}

static void white_box_fs_test()
{
    uint8_t get_buf[256];
    int result;
    mbed::File file;

#if !defined(TEST_SPIF) && !defined(TEST_SD)
    HeapBlockDevice heap_bd(4096 * 4, 1,  1, 4096);
    FlashSimBlockDevice flash_bd(&heap_bd);
#endif

    StorageLite stlite;

    result = stlite.init(&flash_bd);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);

    result = stlitefs.mount(&flash_bd);
    TEST_ASSERT_EQUAL(0, result);

    result = stlitefs.reformat(&flash_bd);
    TEST_ASSERT_EQUAL(0, result);

    result = file.open(&stlitefs, file1_name, O_WRONLY);
    TEST_ASSERT_EQUAL(0, result);

    result = file.write(file1_val1, strlen(file1_val1));
    TEST_ASSERT_EQUAL(strlen(file1_val1), result);

    result = file.close();
    TEST_ASSERT_EQUAL(0, result);

    result = file.open(&stlitefs, file2_name, O_WRONLY);
    TEST_ASSERT_EQUAL(0, result);

    result = file.write(file2_val1, strlen(file2_val1));
    TEST_ASSERT_EQUAL(strlen(file2_val1), result);

    result = file.write(file2_val2, strlen(file2_val2));
    TEST_ASSERT_EQUAL(-ENOSYS, result);

    result = file.close();
    TEST_ASSERT_EQUAL(0, result);

    result = stlitefs.remove(file2_name);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

    for (int i = 0; i < 2; i++) {
        printf("%s unmount/mount\n", i ? "After" : "Before");

        size_t actual_data_size;
        result = stlite.get(file1_name, strlen(file1_name), get_buf,
                            sizeof(get_buf), actual_data_size);
        TEST_ASSERT_EQUAL(0, result);
        TEST_ASSERT_EQUAL(strlen(file1_val1), actual_data_size);
        TEST_ASSERT_EQUAL(0, memcmp(file1_val1, get_buf, strlen(file1_val1)));

        result = file.open(&stlitefs, file1_name, O_RDONLY);
        TEST_ASSERT_EQUAL(0, result);

        result = file.read(get_buf, sizeof(get_buf));
        TEST_ASSERT_EQUAL(strlen(file1_val1), result);
        TEST_ASSERT_EQUAL(0, memcmp(file1_val1, get_buf, strlen(file1_val1)));

        result = file.close();
        TEST_ASSERT_EQUAL(0, result);

        result = stlite.get(file2_name, strlen(file2_name), get_buf,
                            sizeof(get_buf), actual_data_size);
        TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, result);

        result = file.open(&stlitefs, file2_name, O_RDONLY);
        TEST_ASSERT_EQUAL(-ENOENT, result);

        result = stlitefs.unmount();
        TEST_ASSERT_EQUAL(0, result);

        result = stlitefs.mount(&flash_bd);
        TEST_ASSERT_EQUAL(0, result);
    }

    result = stlitefs.reformat(&flash_bd);
    TEST_ASSERT_EQUAL(0, result);
    for (int i = 0; i < 2; i++) {
        printf("%s unmount/mount\n", i ? "After" : "Before");

        result = file.open(&stlitefs, file1_name, O_RDONLY);
        TEST_ASSERT_EQUAL(-ENOENT, result);

        result = stlitefs.unmount();
        TEST_ASSERT_EQUAL(0, result);

        result = stlitefs.mount(&flash_bd);
        TEST_ASSERT_EQUAL(0, result);
    }
}

static void multi_set_test()
{
    char *file_name;
    uint8_t *get_buf, *set_buf;
    size_t file_name_size = 32;
    size_t data_size = 512;
    size_t num_files = 32;
    size_t set_iters = 3;
    size_t actual_data_size;
    int result;
    mbed::Timer timer;
    mbed::File file;
    int elapsed;
    size_t i;
    uint32_t flags;
    uint8_t file_ind;

    timer.start();
#if !defined(TEST_SPIF) && !defined(TEST_SD)
    HeapBlockDevice heap_bd(4096 * 64, 1,  1, 4096);
    FlashSimBlockDevice flash_bd(&heap_bd);
#endif

    StorageLite stlite;

    timer.reset();
    result = stlite.init(&flash_bd, 128);
    elapsed = timer.read_ms();
    printf("Elapsed time for initial init is %d ms\n", elapsed);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);

    file_name = new char[file_name_size + 1];
    get_buf = new uint8_t[data_size];
    set_buf = new uint8_t[data_size];

    srand(1);
    for (i = 0; i < file_name_size; i++) {
        // printable characters only
        file_name[i] = rand() % 223 + 32;
    }
    file_name[file_name_size] = '\0';

    for (i = 0; i < data_size; i++) {
        set_buf[i] = rand() % 256;
    }

    for (int st_type = 0; st_type < 2; st_type++) {
        int max_set_time = 0, total_set_time = 0;
        int max_get_time = 0, total_get_time = 0;

        timer.reset();
        if (st_type) {
            result = stlitefs.reformat(&flash_bd);
        } else {
            result = stlite.reset();
        }
        elapsed = timer.read_ms();
        printf("Elapsed time for %s is %d ms\n", st_type ? "reformat" : "reset", elapsed);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        for (i = 0; i < set_iters; i++) {
            for (file_ind = 0; file_ind < num_files; file_ind++) {
                file_name[0] = '0' + file_ind;
                set_buf[0] = file_ind * (i + 1);
                timer.reset();
                if (st_type) {
                    result = file.open(&stlitefs, (const char *) file_name, O_WRONLY);
                    TEST_ASSERT_EQUAL(0, result);

                    actual_data_size = file.write(set_buf, data_size);

                    result = file.close();
                } else {
                    flags = StorageLite::encrypt_flag;
                    if (i == (file_ind % set_iters)) {
                        flags |= StorageLite::update_backup_flag;
                    }
                    if (rand() % 2) {
                        flags |= StorageLite::rollback_protect_flag;
                    }
                    result = stlite.set(file_name, set_buf, data_size, flags);
                }
                elapsed = timer.read_ms();
                TEST_ASSERT_EQUAL(0, result);
                if (elapsed > max_set_time) {
                    max_set_time = elapsed;
                }
                total_set_time += elapsed;
            }
        }

        for (file_ind = 0; file_ind < num_files; file_ind++) {
            file_name[0] = '0' + file_ind;
            set_buf[0] = file_ind * set_iters;
            timer.reset();
            if (st_type) {
                result = file.open(&stlitefs, (const char *) file_name, O_RDONLY);
                TEST_ASSERT_EQUAL(0, result);

                actual_data_size = file.read(get_buf, data_size);

                result = file.close();
            } else {
                result = stlite.get(file_name, get_buf, data_size, actual_data_size);
            }
            elapsed = timer.read_ms();
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(data_size, actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(set_buf, get_buf, data_size));
            if (elapsed > max_get_time) {
                max_get_time = elapsed;
            }
            total_get_time += elapsed;
        }

        printf("%s time: Total (%d * %d times) - %d ms, Average - %d ms, Max - %d ms\n",
               st_type ? "open+write+close" : "set", set_iters, num_files, total_set_time,
               total_set_time / (set_iters * num_files), max_set_time);
        printf("%s time: Total (%d times)      - %d ms, Average - %d ms, Max - %d ms\n",
               st_type ? "open+read+close" : "get", num_files, total_get_time,
               total_get_time / num_files, max_get_time);

        if (st_type) {
            result = stlitefs.unmount();
        } else {
            result = stlite.deinit();
        }
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        timer.reset();
        if (st_type) {
            result = stlitefs.mount(&flash_bd);
        } else {
            result = stlite.init(&flash_bd);
        }
        elapsed = timer.read_ms();
        printf("Elapsed time for %s is %d ms\n", st_type ? "mount" : "init", elapsed);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        if (st_type) {
            continue;
        }

        timer.reset();
        result = stlite.factory_reset();
        elapsed = timer.read_ms();
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        printf("Elapsed time for factory reset is %d ms\n", elapsed);

        for (file_ind = 0; file_ind < num_files; file_ind++) {
            file_name[0] = '0' + file_ind;
            set_buf[0] = file_ind * ((file_ind % set_iters) + 1);
            result = stlite.get(file_name, get_buf, data_size, actual_data_size);
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
            TEST_ASSERT_EQUAL(data_size, actual_data_size);
            TEST_ASSERT_EQUAL(0, memcmp(set_buf, get_buf, data_size));
        }

        result = stlite.deinit();
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
    }

    delete[] file_name;
    delete[] get_buf;
    delete[] set_buf;
}

static void error_inject_test()
{
    char *file_name;
    uint8_t *get_buf, *set_buf, *exists;
    size_t file_name_size = 32;
    size_t data_size = 128;
    size_t num_files = 63;
    size_t actual_data_size;
    int result;
    uint32_t flags;
    uint8_t set_iters = 3;
    uint8_t i, file_ind;

    // Don't use a non volatile BD here (won't work in this test)
    HeapBlockDevice bd(4096 * 32, 1,  1, 4096);
    FlashSimBlockDevice flash_bd(&bd);

    StorageLite stlite;

    result = stlite.init(&flash_bd, 128);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

    result = stlite.reset();
    TEST_ASSERT_EQUAL(0, result);

    file_name = new char[file_name_size + 1];
    get_buf = new uint8_t[data_size];
    set_buf = new uint8_t[data_size];
    exists = new uint8_t[num_files];

    memset(exists, 0, num_files);

    for (i = 0; i < set_iters; i++) {
        for (file_ind = 0; file_ind < num_files; file_ind++) {
            memset(file_name, '0' + file_ind, file_name_size);
            file_name[file_name_size + 1] = '\0';
            memset(set_buf, file_ind * i, data_size);
            flags = ((rand() % 2) & StorageLite::encrypt_flag) |
                    ((rand() % 2) & StorageLite::update_backup_flag) |
                    ((rand() % 2) & StorageLite::rollback_protect_flag);
            if ((file_ind != (num_files - 1)) && exists[file_ind] && !(rand() % 3)) {
                result = stlite.remove(file_name, file_name_size);
                exists[file_ind] = 0;
            } else {
                result = stlite.set(file_name, file_name_size, set_buf, data_size, flags);
                exists[file_ind] = 1;
            }
            TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
        }
    }

    for (uint8_t get_iter = 0; get_iter < 2; get_iter++) {
        for (file_ind = 0; file_ind < num_files; file_ind++) {
            memset(file_name, '0' + file_ind, file_name_size);
            file_name[file_name_size + 1] = '\0';
            if (file_ind == (num_files - 1)) {
                // last file will hold the previous version at the second iteration (after being crippled)
                memset(set_buf, file_ind * (set_iters - get_iter - 1), data_size);
            } else {
                memset(set_buf, file_ind * (set_iters - 1), data_size);
            }
            result = stlite.get(file_name, file_name_size, get_buf, data_size, actual_data_size);
            if (exists[file_ind]) {
                TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
                TEST_ASSERT_EQUAL(data_size, actual_data_size);
                TEST_ASSERT_EQUAL(0, memcmp(set_buf, get_buf, data_size));
            } else {
                TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, result);
            }
        }

        if (get_iter) {
            break;
        }

        // Cripple the last file
        size_t offset = stlite.size() - stlite.free_size() - 16;
        uint8_t x;
        bd.read(&x, offset, 1);
        x++;
        bd.program(&x, offset, 1);

        result = stlite.get(file_name, file_name_size, get_buf, data_size, actual_data_size);
        TEST_ASSERT_EQUAL(STORAGELITE_DATA_CORRUPT, result);

        result = stlite.deinit();
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);

        result = stlite.init(&flash_bd);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
    }

    delete[] file_name;
    delete[] get_buf;
    delete[] set_buf;
}



utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("StorageLite: White box test",     white_box_test,    greentea_failure_handler),
    Case("StorageLite: White box FS test",  white_box_fs_test, greentea_failure_handler),
    Case("StorageLite: Multiple set test",  multi_set_test,    greentea_failure_handler),
    Case("StorageLite: Error inject test",  error_inject_test, greentea_failure_handler),
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
