/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*
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

#include "TDBStore.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "mbed_error.h"
#include "Timer.h"
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

using namespace mbed;
using namespace utest::v1;

#undef TEST_SPIF
#undef TEST_SD
#undef TEST_FLASHIAP

#ifdef TEST_SPIF
#include "SPIFBlockDevice.h"
SPIFBlockDevice bd(MBED_CONF_SPIF_DRIVER_SPI_MOSI, MBED_CONF_SPIF_DRIVER_SPI_MISO,
                   MBED_CONF_SPIF_DRIVER_SPI_CLK, MBED_CONF_SPIF_DRIVER_SPI_CS);
SlicingBlockDevice flash_bd(&bd, 0, 16 * 4096);

#elif defined(TEST_SD)
#include "SDBlockDevice.h"
SDBlockDevice bd(MBED_CONF_SD_SPI_MOSI, MBED_CONF_SD_SPI_MISO,
                 MBED_CONF_SD_SPI_CLK, MBED_CONF_SD_SPI_CS);
SlicingBlockDevice slice_bd(&bd, 0, 512 * 512);
FlashSimBlockDevice flash_bd(&slice_bd);

#elif defined(TEST_FLASHIAP)
#include "FlashIAPBlockDevice.h"
FlashIAPBlockDevice flash_bd(0xF0000, 0x10000);

#else
#define USE_HEAP_BD 1
HeapBlockDevice bd(8 * 4096, 1, 1, 4096);
FlashSimBlockDevice flash_bd(&bd);
#endif

static const int heap_alloc_threshold_size = 4096;

typedef struct {
    size_t size;
    size_t read_size;
    size_t prog_size;
    size_t erase_size;
} bd_params_t;

static const char *const key1      = "key1";
static const char *const key1_val1 = "val1";
static const char *const key2      = "name_of_key2";
static const char *const key2_val1 = "val3";
static const char *const key2_val2 = "val2 of key 2";
static const char *const key2_val3 = "Val1 value of key 2            ";
static const char *const key3      = "This_is_the_name_of_key3";
static const char *const key3_val1 = "Data value of key 3 is the following";
static const char *const key4      = "This_is_the_name_of_key4";
static const char *const key4_val1 = "Is this the value of key 4?";
static const char *const key4_val2 = "What the hell is the value of key 4, god damn it!";
static const char *const key5      = "This_is_the_real_name_of_Key5";
static const char *const key5_val1 = "Key 5 value that should definitely be written";
static const char *const key5_val2 = "Key 5 value that should definitely not be written";
static const char *const res_val1  = "This should be saved as the reserved data";
static const char *const res_val2  = "This should surely not be saved as the reserved data";

static void white_box_test()
{
    bd_params_t bd_params[] = {
        {8192,     1,  16, 4096}, // Standard
        {4096 * 4, 1,   1, 4096}, // K82F like
        {8192,    64, 128, 2048}, // Awkward read and program sizes, both larger than header size
        {2048,     1,   4,  128}, // Small sector and total sizes
        {0,        0,   0,    0}, // Place holder for real non volatile device (if defined)
    };

    int num_bds = sizeof(bd_params) / sizeof(bd_params_t);
    uint8_t get_buf[128];
    size_t actual_data_size;
    int result;
    mbed::Timer timer;
    int elapsed;
    KVStore::info_t info;

#ifndef USE_HEAP_BD
    flash_bd.init();
    bd_params[num_bds - 1].size       = flash_bd.size();
    bd_params[num_bds - 1].read_size  = flash_bd.get_read_size();
    bd_params[num_bds - 1].prog_size  = flash_bd.get_program_size();
    bd_params[num_bds - 1].erase_size = flash_bd.get_erase_size();
    flash_bd.deinit();
#endif

    timer.start();
    for (int bd_num = 0; bd_num < num_bds; bd_num++) {

        uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
        TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");

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
        } else {
            test_bd = &flash_sim_bd;
            // We need to skip the test if we don't have enough memory for the heap block device.
            // However, this device allocates the erase units on the fly, so "erase" it via the flash
            // simulator. A failure here means we haven't got enough memory.
            flash_sim_bd.init();
            result = flash_sim_bd.erase(0, flash_sim_bd.size());
            TEST_SKIP_UNLESS_MESSAGE(!result, "Not enough heap to run test");
            flash_sim_bd.deinit();
        }

        delete[] dummy;

        TDBStore *tdbs = new TDBStore(test_bd);

        timer.reset();
        result = tdbs->init();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        elapsed = timer.read_ms();
        printf("Elapsed time for init %d ms\n", elapsed);

        timer.reset();
        result = tdbs->reset();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        elapsed = timer.read_ms();
        printf("Elapsed time for reset is %d ms\n", elapsed);

        result = tdbs->reserved_data_get(get_buf, strlen(res_val1));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = tdbs->reserved_data_set(res_val1, strlen(res_val1));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->reserved_data_set(res_val2, strlen(res_val2));
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_FAILED, result);

        result = tdbs->set(key1, key1_val1, strlen(key1_val1), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->set(key2, key2_val1, strlen(key2_val1), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->set(key2, key2_val2, strlen(key2_val2), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        timer.reset();
        result = tdbs->set(key2, key2_val3, strlen(key2_val3), 0);
        elapsed = timer.read_ms();
        printf("Elapsed time for set is %d ms\n", elapsed);

        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->set(key3, key3_val1, strlen(key3_val1), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->get(key3, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key3_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key3_val1, get_buf, strlen(key3_val1));

        KVStore::set_handle_t handle;
        result = tdbs->set_start(&handle, key4, 15, 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->set_add_data(handle, key4_val2, 10);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        result = tdbs->set_add_data(handle, key4_val2 + 10, 5);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        result = tdbs->set_finalize(handle);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->get(key4, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(15, actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key4_val2, get_buf, actual_data_size);

        result = tdbs->get(key4, get_buf, 7, &actual_data_size, 4);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(7, actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key4_val2 + 4, get_buf, actual_data_size);

        for (int j = 0; j < 2; j++) {
            result = tdbs->set(key4, key4_val1, strlen(key4_val1), 0);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            result = tdbs->set(key4, key4_val2, strlen(key4_val2), 0);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        }

        result = tdbs->remove(key3);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->remove(key3);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = tdbs->get_info(key5, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

        result = tdbs->set(key5, key5_val1, strlen(key5_val1), KVStore::WRITE_ONCE_FLAG);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        result = tdbs->set(key5, key5_val2, strlen(key5_val2), 0);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, result);

        result = tdbs->remove(key5);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_WRITE_PROTECTED, result);

        result = tdbs->get_info(key5, &info);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key5_val1), info.size);
        TEST_ASSERT_EQUAL(KVStore::WRITE_ONCE_FLAG, info.flags);

        result = tdbs->get(key5, get_buf, sizeof(get_buf), &actual_data_size);
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(strlen(key5_val1), actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(key5_val1, get_buf, strlen(key5_val1));

        for (int i = 0; i < 2; i++) {
            printf("%s deinit/init\n", i ? "After" : "Before");
            result = tdbs->get(key1, get_buf, sizeof(get_buf), &actual_data_size);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(key1_val1), actual_data_size);
            TEST_ASSERT_EQUAL_STRING_LEN(key1_val1, get_buf, strlen(key1_val1));

            timer.reset();
            result = tdbs->get(key2, get_buf, sizeof(get_buf), &actual_data_size);
            elapsed = timer.read_ms();
            printf("Elapsed time for get is %d ms\n", elapsed);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(key2_val3), actual_data_size);
            TEST_ASSERT_EQUAL_STRING_LEN(key2_val3, get_buf, strlen(key2_val3));

            result = tdbs->get(key3, get_buf, sizeof(get_buf), &actual_data_size);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

            result = tdbs->get(key4, get_buf, sizeof(get_buf), &actual_data_size);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(key4_val2), actual_data_size);
            TEST_ASSERT_EQUAL_STRING_LEN(key4_val2, get_buf, strlen(key4_val2));

            result = tdbs->get(key5, get_buf, sizeof(get_buf), &actual_data_size);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            TEST_ASSERT_EQUAL(strlen(key5_val1), actual_data_size);
            TEST_ASSERT_EQUAL_STRING_LEN(key5_val1, get_buf, strlen(key5_val1));

            KVStore::iterator_t it;
            char *char_get_buf = reinterpret_cast <char *>(get_buf);

            result = tdbs->iterator_open(&it, "This");
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            result = tdbs->iterator_next(it, char_get_buf, sizeof(get_buf));
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            bool got_key4 = !strcmp(key4, char_get_buf);
            bool got_key5 = !strcmp(key5, char_get_buf);
            TEST_ASSERT_EQUAL(true, got_key4 || got_key5);

            result = tdbs->iterator_next(it, char_get_buf, sizeof(get_buf));
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            if (got_key4) {
                TEST_ASSERT_EQUAL_STRING(key5, char_get_buf);
            } else {
                TEST_ASSERT_EQUAL_STRING(key4, char_get_buf);
            }

            result = tdbs->iterator_next(it, (char *)get_buf, sizeof(get_buf));
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_ERROR_ITEM_NOT_FOUND, result);

            result = tdbs->iterator_close(it);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            result = tdbs->reserved_data_get(get_buf, strlen(res_val1));
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
            TEST_ASSERT_EQUAL_STRING_LEN(res_val1, get_buf, strlen(res_val1));

            result = tdbs->deinit();
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

            timer.reset();
            result = tdbs->init();
            elapsed = timer.read_ms();
            printf("Elapsed time for init is %d ms\n", elapsed);
            TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        }

        result = tdbs->deinit();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

        delete tdbs;
    }
}


static void multi_set_test()
{

    char *key;
    uint8_t *get_buf, *set_buf;
    size_t key_size = 32;
    size_t data_size = 512;
    size_t num_keys = 16;
    size_t set_iters = 3;
    size_t actual_data_size;
    int result;
    mbed::Timer timer;
    int elapsed;
    size_t i;
    uint8_t key_ind;

    timer.start();

    uint8_t *dummy = new (std::nothrow) uint8_t[heap_alloc_threshold_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough heap to run test");

#ifdef USE_HEAP_BD
    // We need to skip the test if we don't have enough memory for the heap block device.
    // However, this device allocates the erase units on the fly, so "erase" it via the flash
    // simulator. A failure here means we haven't got enough memory.
    flash_bd.init();
    result = flash_bd.erase(0, flash_bd.size());
    TEST_SKIP_UNLESS_MESSAGE(!result, "Not enough heap to run test");
    flash_bd.deinit();
#endif

    delete[] dummy;

    TDBStore *tdbs = new TDBStore(&flash_bd);

    timer.reset();
    result = tdbs->init();
    elapsed = timer.read_ms();
    printf("Elapsed time for initial init is %d ms\n", elapsed);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    key = new char[key_size + 1];
    get_buf = new uint8_t[data_size];
    set_buf = new uint8_t[data_size];

    srand(1);
    for (i = 0; i < key_size; i++) {
        // Alphabet characters only
        key[i] = 'a' + rand() % ('z' - 'a' + 1);
    }
    key[key_size] = '\0';

    for (i = 0; i < data_size; i++) {
        set_buf[i] = rand() % 256;
    }

    int max_set_time = 0, total_set_time = 0;
    int max_get_time = 0, total_get_time = 0;

    timer.reset();
    result = tdbs->reset();
    elapsed = timer.read_ms();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    for (i = 0; i < set_iters; i++) {
        for (key_ind = 0; key_ind < num_keys; key_ind++) {
            key[0] = 'A' + key_ind;
            set_buf[0] = key_ind * (i + 1);
            timer.reset();
            result = tdbs->set(key, set_buf, data_size, 0);
            elapsed = timer.read_ms();
            TEST_ASSERT_EQUAL_ERROR_CODE(0, result);
            if (elapsed > max_set_time) {
                max_set_time = elapsed;
            }
            total_set_time += elapsed;
        }
    }

    for (key_ind = 0; key_ind < num_keys; key_ind++) {
        key[0] = 'A' + key_ind;
        set_buf[0] = key_ind * set_iters;
        timer.reset();
        result = tdbs->get(key, get_buf, data_size, &actual_data_size);
        elapsed = timer.read_ms();
        TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);
        TEST_ASSERT_EQUAL(data_size, actual_data_size);
        TEST_ASSERT_EQUAL_STRING_LEN(set_buf, get_buf, data_size);
        if (elapsed > max_get_time) {
            max_get_time = elapsed;
        }
        total_get_time += elapsed;
    }

    printf("set time: Total (%d * %d times) - %d ms, Average - %d ms, Max - %d ms\n",
           set_iters, num_keys, total_set_time,
           total_set_time / (set_iters * num_keys), max_set_time);
    printf("get time: Total (%d times)      - %d ms, Average - %d ms, Max - %d ms\n",
           num_keys, total_get_time,
           total_get_time / num_keys, max_get_time);

    result = tdbs->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    timer.reset();
    result = tdbs->init();
    elapsed = timer.read_ms();
    printf("Elapsed time for init is %d ms\n", elapsed);
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    result = tdbs->deinit();
    TEST_ASSERT_EQUAL_ERROR_CODE(MBED_SUCCESS, result);

    delete[] key;
    delete[] get_buf;
    delete[] set_buf;

    delete tdbs;
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("TDBStore: White box test",     white_box_test,    greentea_failure_handler),
    Case("TDBStore: Multiple set test",  multi_set_test,    greentea_failure_handler),
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
