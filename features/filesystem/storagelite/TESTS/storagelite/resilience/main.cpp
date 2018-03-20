/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "StorageLite.h"
#include "Timer.h"
#include "HeapBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#if !STORAGELITE_ENABLED
#error [NOT_SUPPORTED] StorageLite needs to be enabled for this test
#endif

// FIXME: This test should use a persistent block device (i.e. MBED_TEST_BLOCKDEVICE and not
// MBED_TEST_SIM_BLOCKDEVICE). However, MBED_TEST_BLOCKDEVICE is currently not supported due to
// build issues, so test should only work in local mode (LOCAL_TEST). MBED_TEST_SIM_BLOCKDEVICE is still
// used in order to filter out inappropriate boards (little RAM etc.).
#if !defined(MBED_TEST_SIM_BLOCKDEVICE) && !defined(LOCAL_TEST)
#error [NOT_SUPPORTED] StorageLite test not supported on this platform
#endif


#define TEST_ASSERT_OR_EXIT(condition)              \
        TEST_ASSERT(condition); if (!(condition)) {error("Assert failed");}

#define TEST_ASSERT_EQUAL_OR_EXIT(expected, actual) \
        TEST_ASSERT_EQUAL(expected, actual); if ((int64_t)(expected) != (int64_t)(actual)) {error("Assert failed");}


// Define this if you want to run the test locally (requires SPIF/SD block device)
#undef LOCAL_TEST

#ifndef MBED_TEST_SIM_BLOCKDEVICE
#define MBED_TEST_SIM_BLOCKDEVICE SPIFBlockDevice
// FIXME: use proper SPI defines here
#define MBED_TEST_SIM_BLOCKDEVICE_DECL SPIFBlockDevice bd(D11, D12, D13, D8);
#endif

#ifdef LOCAL_TEST
#define TEST_SPIF
#include "SPIFBlockDevice.h"

#else
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_SIM_BLOCKDEVICE)
#endif

#ifndef MBED_TEST_SIM_BLOCKDEVICE_DECL
#define MBED_TEST_SIM_BLOCKDEVICE_DECL MBED_TEST_SIM_BLOCKDEVICE bd(64 * 512, 1, 1, 512)
#endif

MBED_TEST_SIM_BLOCKDEVICE_DECL;

using namespace utest::v1;

const char *iter_file_name = "iter_file";
const size_t num_files = 128;

typedef enum {
    CMD_STATUS_PASS,
    CMD_STATUS_FAIL,
    CMD_STATUS_CONTINUE,
    CMD_STATUS_ERROR
} cmd_status_t;

static StorageLite stlite;

static void test_init()
{
    int result = stlite.init(&bd, num_files);
    TEST_ASSERT_EQUAL_OR_EXIT(0, result);
}

static void test_format()
{
    int result = stlite.reset();
    TEST_ASSERT_EQUAL_OR_EXIT(0, result);
    int num_iters = 0;
    result = stlite.set(iter_file_name, strlen(iter_file_name), &num_iters, sizeof(num_iters), 0);
    TEST_ASSERT_EQUAL_OR_EXIT(0, result);
}

static void test_run(bool verify)
{
    uint8_t *file_name, *get_buf, *set_buf;
    size_t file_name_size = 16;
    size_t data_size = 32;
    size_t actual_data_size;
    int result;
    int curr_iter, start_iter, num_iters;
    int file_ind;

    file_name = new uint8_t[file_name_size];
    get_buf = new uint8_t[data_size];
    set_buf = new uint8_t[data_size];

    result = stlite.get(iter_file_name, strlen(iter_file_name), &num_iters, sizeof(num_iters), actual_data_size);
    TEST_ASSERT_EQUAL_OR_EXIT(0, result);

    if (verify) {
        start_iter = 0;
        printf("Verifying iteration %d\n", num_iters);
    } else {
        num_iters++;
        printf("Writing iteration %d\n", num_iters);
        result = stlite.set(iter_file_name, strlen(iter_file_name), &num_iters, sizeof(num_iters), 0);
        TEST_ASSERT_EQUAL_OR_EXIT(0, result);
        start_iter = num_iters - 1;
    }

    for (;;) {
        for (curr_iter = start_iter; curr_iter < num_iters; curr_iter++) {
            for (file_ind = 0; file_ind < curr_iter; file_ind++) {
                sprintf((char *) file_name, "file%d", curr_iter * 256 + file_ind);
                file_name_size = strlen((char *) file_name);
                memset(set_buf, curr_iter, data_size / 2);
                memset(set_buf + data_size / 2, file_ind, data_size / 2);
                bool exists = true;
                if ((file_ind % 3) == 1) {
                    result = stlite.file_exists(file_name, file_name_size);
                    if (result == STORAGELITE_NOT_FOUND) {
                        exists = false;
                    } else {
                        TEST_ASSERT_EQUAL_OR_EXIT(STORAGELITE_SUCCESS, result);
                    }
                }
                if (verify) {
                    if (!exists) {
                        continue;
                    }
                    result = stlite.get(file_name, file_name_size, get_buf, data_size, actual_data_size);
                    TEST_ASSERT_EQUAL_OR_EXIT(STORAGELITE_SUCCESS, result);
                    TEST_ASSERT_EQUAL_OR_EXIT(data_size, actual_data_size);
                    TEST_ASSERT_EQUAL_OR_EXIT(0, memcmp(set_buf, get_buf, data_size));
                } else {
                    if (exists && ((file_ind % 3) == 1)) {
                        result = stlite.remove(file_name, file_name_size);
                    } else {
                        uint32_t flags = ((rand() % 2) & StorageLite::encrypt_flag) |
                                         ((rand() % 2) & StorageLite::update_backup_flag) |
                                         ((rand() % 2) & StorageLite::rollback_protect_flag);
                        result = stlite.set(file_name, file_name_size, set_buf, data_size, flags);
                    }
                    TEST_ASSERT_EQUAL_OR_EXIT(STORAGELITE_SUCCESS, result);
                }
            }
        }
        if (verify) {
            break;
        }
    }

    delete[] file_name;
    delete[] get_buf;
    delete[] set_buf;
}

static cmd_status_t handle_command(const char *key, const char *value)
{
    if (strcmp(key, "format") == 0) {
        test_init();
        test_format();
        greentea_send_kv("format_done", 1);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "init") == 0) {
        test_init();
        greentea_send_kv("init_done", 1);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "run") == 0) {
        test_run(true);
        test_run(false);
        return CMD_STATUS_CONTINUE;

    } else if (strcmp(key, "exit") == 0) {
        if (strcmp(value, "pass") != 0) {
            return CMD_STATUS_FAIL;
        }
        test_init();
        test_run(true);
        return CMD_STATUS_PASS;

    } else {
        return CMD_STATUS_ERROR;
    }
}

int main()
{
    GREENTEA_SETUP(120, "storagelite_reset");

    char key[10 + 1] = {};
    char value[128 + 1] = {};

    greentea_send_kv("start", 1);

    // Handshake with host
    cmd_status_t cmd_status = CMD_STATUS_CONTINUE;
    while (CMD_STATUS_CONTINUE == cmd_status) {
        memset(key, 0, sizeof(key));
        memset(value, 0, sizeof(value));
        greentea_parse_kv(key, value, sizeof(key) - 1, sizeof(value) - 1);
        cmd_status = handle_command(key, value);
    }

    GREENTEA_TESTSUITE_RESULT(CMD_STATUS_PASS == cmd_status);
}
