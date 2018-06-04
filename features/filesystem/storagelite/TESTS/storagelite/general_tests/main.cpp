/* Copyright (c) 2017 ARM Limited
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
#include "HeapBlockDevice.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "Thread.h"
#endif
#include "nvstore.h"
#include "sha256.h"

// Define this if you want to run the test locally (requires SPIF/SD block device)
#undef LOCAL_TEST

#if !STORAGELITE_ENABLED
#error [NOT_SUPPORTED] StorageLite needs to be enabled for this test
#endif

// MBED_TEST_SIM_BLOCKDEVICE is used here only to filter out inappropriate boards (little RAM etc.)
#if !defined(MBED_TEST_SIM_BLOCKDEVICE) && !defined(LOCAL_TEST)
#error [NOT_SUPPORTED] StorageLite test not supported on this platform
#endif

using namespace utest::v1;

static const uint32_t data_buf_size        = 10;
static const uint16_t default_name_size    = 16;
static const uint8_t  default_name         = 1;
static const uint8_t  non_exist_file_name  = 2;
static const uint8_t  fr_file_name         = 3;
static const uint8_t  empty_file_name      = 4;

static const uint16_t name_max_size        = StorageLite::max_name_size;
static const uint32_t invalid_flags        = 0xFFFF;

static const size_t bd_size                = 8192;
static const size_t bd_erase_size          = 4096;
static const size_t bd_prog_size           = 16;
static const size_t bd_read_size           = 1;

StorageLite *stlite = NULL;
HeapBlockDevice bd(bd_size, bd_read_size, bd_prog_size, bd_erase_size);
FlashSimBlockDevice flash_bd(&bd);

static void terminated()
{
    int status = STORAGELITE_SUCCESS;

    stlite->reset();
    status = stlite->deinit();
    TEST_ASSERT_EQUAL_MESSAGE(STORAGELITE_SUCCESS, status, "StorageLite::deinit failed\n");

    delete stlite;
}

static void set_thread(uint8_t *file_name)
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->set(file_name, 1, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
}

/*------------------set()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_set_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(NULL, 0, data_buf, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_set_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(NULL, default_name_size, data_buf, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_set_name_len_zero_name_not_null()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, 0, data_buf, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_set_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, name_max_size + 1, data_buf, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : invalid flags
static void storagelite_set_invalid_flags()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, invalid_flags);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : buf_size is not 0 and buf is null
static void storagelite_set_buf_size_not_zero_buf_null()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->set(&default_name, default_name_size, NULL, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : buf_size is 0 and buf is not null
static void storagelite_set_buf_size_zero_buf_not_null()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//buf size is 0 and buf is null
static void storagelite_set_buf_size_zero_buf_null()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->set(&default_name, default_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//set two files with the same parameters
static void storagelite_set_two_files_same_params()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, 0);
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//multithreaded set, check with get all files were created succesfully
static void storagelite_set_multithreded()
{
    int status = STORAGELITE_SUCCESS;
    Thread T1, T2, T3;
    uint8_t i1 = 0;
    uint8_t i2 = 1;
    uint8_t i3 = 2;

    osStatus err = T1.start(callback(set_thread, &i1));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }
    
    err = T2.start(callback(set_thread, &i2));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }
    
    err = T3.start(callback(set_thread, &i3));
    if (err) {
        TEST_FAIL_MESSAGE("creating thread failed!");
    }

    err = T1.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }
    err = T2.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }
    err = T3.join();
    if (err) {
        TEST_FAIL_MESSAGE("joining thread failed!");
    }

    size_t actual_len_bytes = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        status = stlite->get(&i, 1, NULL, 0, actual_len_bytes);
        TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    }

    terminated();
}

/*------------------get()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_get_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(NULL, 0, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_get_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(NULL, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_get_name_len_zero_name_not_null()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, 0, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_get_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, name_max_size + 1, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : buf_size is not 0 and buf is null
static void storagelite_get_buf_size_not_zero_buf_null()
{
    int status = STORAGELITE_SUCCESS;
    size_t actual_len_bytes = 0;
    status = stlite->get(&default_name, default_name_size, NULL, data_buf_size, actual_len_bytes);
    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : buf_size is 0 and buf is not null
static void storagelite_get_not_empty_file_buf_size_zero_buf_null()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    status = stlite->get(&default_name, default_name_size, NULL, 0, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//bad params : buf_size is 0 and buf is null (when file req. is not empty)
static void storagelite_get_empty_file_buf_size_zero_buf_null()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->set(&empty_file_name, default_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&empty_file_name, default_name_size, NULL, 0, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//get file with valid params but with insufficient buf size
static void storagelite_get_buf_size_insufficient()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size / 2, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//get a non existing file
static void storagelite_get_non_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&non_exist_file_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get an existing file
static void storagelite_get_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(data_buf_size, actual_len_bytes);
    terminated();
}

//get a removed file
static void storagelite_get_removed_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//flags: set RollBack - file retrieved ok
static void storagelite_get_rollback_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, StorageLite::rollback_protect_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//flags: set RollBack - corrupt file’s CMAC record on Nvstore
static void storagelite_get_corrupt_rollback_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, StorageLite::rollback_protect_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    NVStore& nvstore = NVStore::get_instance();
    status = nvstore.reset();
    TEST_ASSERT_EQUAL(NVSTORE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_RB_PROTECT_ERROR, status);
    terminated();
}

//flags: set Encrypt - file retrieved ok
static void storagelite_get_encrypt_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, StorageLite::encrypt_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

/*------------------remove()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_remove_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(NULL, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_remove_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(NULL, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_remove_name_not_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_remove_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, name_max_size + 1);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//set a file and remove it
static void storagelite_remove_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//try to remove a non existing file
static void storagelite_remove_non_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&non_exist_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//try to remove a removed file
static void storagelite_remove_removed_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//set FR file, remove it and try to get it
static void storagelite_remove_fr_file_try_get()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&fr_file_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

/*------------------get_file_size()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_get_item_size_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(NULL, 0, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_get_item_size_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(NULL, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_get_item_size_name_not_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&default_name, 0, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_get_item_size_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&default_name, name_max_size + 1, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//get_item_size that doesn’t exist
static void storagelite_get_item_size_non_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&non_exist_file_name, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get_item_size that exists
static void storagelite_get_item_size_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&default_name, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(data_buf_size, actual_data_size);
    terminated();
}

//get_item_size that was removed
static void storagelite_get_item_size_removed_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&default_name, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get_item_size for empty file
static void storagelite_get_item_size_empty_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t empty_file_name[] = "empty_file";
    status = stlite->set(empty_file_name, default_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_data_size = data_buf_size;
    status = stlite->get_file_size(empty_file_name, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(0, actual_data_size);
    terminated();
}

//get_item_size for modified file
static void storagelite_get_item_size_modified_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size * 2] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size * 2, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_data_size = 0;
    status = stlite->get_file_size(&default_name, default_name_size, actual_data_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(data_buf_size * 2, actual_data_size);
    terminated();
}

//set FR file, remove it and try to get_item_size on it
static void storagelite_get_item_size_fr_file_try_get()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get_file_size(&fr_file_name, default_name_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

/*------------------file_exists()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_file_exists_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(NULL, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_file_exists_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(NULL, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_file_exists_name_not_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(&default_name, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_file_exists_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(&default_name, name_max_size + 1);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//search item that doesn’t exist
static void storagelite_file_exists_non_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(&non_exist_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//search item that exists
static void storagelite_file_exists_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->file_exists(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//search item that was removed
static void storagelite_file_exists_removed_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->file_exists(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//set FR file, remove it and try to item_exist it
static void storagelite_file_exists_fr_file_try_get()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->file_exists(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

/*------------------get_file_flags()------------------*/

//bad params : name is NULL and name_length is 0
static void storagelite_get_file_flags_name_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(NULL, 0, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name is NULL and name_length is not 0
static void storagelite_get_file_flags_name_null_name_len_not_zero()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(NULL, default_name_size, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is 0 and name blob is not NULL
static void storagelite_get_file_flags_name_not_null_name_len_zero()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(&default_name, 0, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//bad params : name_length is big not valid number
static void storagelite_get_file_flags_name_len_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(&default_name, name_max_size + 1, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//get_item_flags for a non existing file
static void storagelite_get_file_flags_non_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(&non_exist_file_name, default_name_size, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get_item_flags for a an existing file
static void storagelite_get_file_flags_existing_file()
{
    int status = STORAGELITE_SUCCESS;

    uint32_t flags = 0;
    status = stlite->get_file_flags(&default_name, default_name_size, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//get_item_flags for a removed file
static void storagelite_get_file_flags_removed_file()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->remove(&default_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    uint32_t flags = 0;
    status = stlite->get_file_flags(&default_name, default_name_size, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//set FR file, remove it and try to item_exist it
static void storagelite_get_file_flags_fr_file_try_get()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    uint32_t flags = 0;
    status = stlite->get_file_flags(&fr_file_name, default_name_size, flags);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

/*------------------get_first_file()------------------*/

//bad params : max_name_size is 0
static void storagelite_get_first_file_max_name_size_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    status = stlite->get_first_file(file_name, 0, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//bad params : max_name_size is bigger than allowed
static void storagelite_get_first_file_max_name_bigger_than_max()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size + 1;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//get_first when no file in storage
static void storagelite_get_first_file_no_file_in_storage()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {1};
    uint16_t file_name_buf_size = name_max_size;

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get_first with max_name_size smaller than files name size
static void storagelite_get_first_file_max_name_size_too_small()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = 1;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//valid flow : get_first with valid files
static void storagelite_get_first_file_valid_flow()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//get_first with max_name_size smaller than all files name size but one
static void storagelite_get_first_file_not_first()
{
    int status = STORAGELITE_SUCCESS;
    uint8_t long_file_name[default_name_size * 2] = {0};
    uint16_t long_file_name_size = default_name_size * 2;

    status = stlite->set(long_file_name, long_file_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->set(&default_name, default_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size + 1] = {0};
    uint16_t file_name_buf_size = default_name_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

/*------------------get_next_file()------------------*/

//bad params : max_name_size is 0
static void storagelite_get_next_file_max_name_size_zero()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    file_name_buf_size = 0;
    status = stlite->get_next_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//get_next when no file in storage
static void storagelite_get_next_file_no_file_in_storage()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->get_next_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//get_next with max_name_size smaller than files name size
static void storagelite_get_next_file_max_name_size_too_small()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    file_name_buf_size = 1;
    status = stlite->get_next_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BUFF_TOO_SMALL, status);
    terminated();
}

//valid flow : get_next with valid files
static void storagelite_get_next_file_valid_flow()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size] = {0};
    uint16_t file_name_buf_size = name_max_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->get_next_file(file_name, file_name_buf_size + 1, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//get_next with an invalid handle
static void storagelite_get_next_file_invalid_handle()
{
    int status = STORAGELITE_SUCCESS;

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size + 1] = {0};
    uint16_t file_name_buf_size = default_name_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    handle = 15;
    status = stlite->get_next_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_BAD_VALUE, status);
    terminated();
}

//get_next with max_name_size smaller than all files name size but one
static void storagelite_get_next_file_not_first()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t long_file_name[default_name_size * 2] = {0};
    uint16_t long_file_name_size = default_name_size * 2;

    status = stlite->set(long_file_name, long_file_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->set(&default_name, default_name_size, NULL, 0, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t file_name_size = 0;
    uint32_t handle = 0;
    uint8_t file_name[default_name_size + 1] = {0};
    uint16_t file_name_buf_size = default_name_size;
    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->get_first_file(file_name, file_name_buf_size, file_name_size, handle);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

/*------------------Factory Reset tests function------------------*/

//set FR file, after FR do get for the file
static void storagelite_factory_reset_get_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->factory_reset();

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&fr_file_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

//set files without FR, after FR do get for the file
static void storagelite_factory_reset_get_file_without_fr_flag()
{
    int status = STORAGELITE_SUCCESS;

    status = stlite->factory_reset();

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->get(&default_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_NOT_FOUND, status);
    terminated();
}

//set FR file, set the file again without FR, after FR run get on the file
static void storagelite_factory_reset_get_original_fr_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    uint8_t temp_data_buf[data_buf_size * 2] = {0};
    status = stlite->set(&fr_file_name, default_name_size, temp_data_buf, data_buf_size * 2, 0);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->factory_reset();

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&fr_file_name, default_name_size, temp_data_buf, data_buf_size * 2, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(data_buf_size, actual_len_bytes);
    terminated();
}

//set FR file, set the file again with FR, after FR run get on the file
static void storagelite_factory_reset_get_modified_fr_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    uint8_t temp_data_buf[data_buf_size * 2] = {0};
    status = stlite->set(&fr_file_name, default_name_size, temp_data_buf, data_buf_size * 2,
                         StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->factory_reset();

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&fr_file_name, default_name_size, temp_data_buf, data_buf_size * 2, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    TEST_ASSERT_EQUAL(data_buf_size * 2, actual_len_bytes);
    terminated();
}

//set FR file, remove it, after FR do get on the file
static void storagelite_factory_reset_get_removed_fr_file()
{
    int status = STORAGELITE_SUCCESS;

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&fr_file_name, default_name_size, data_buf, data_buf_size, StorageLite::update_backup_flag);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->remove(&fr_file_name, default_name_size);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    status = stlite->factory_reset();

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    size_t actual_len_bytes = 0;
    status = stlite->get(&fr_file_name, default_name_size, data_buf, data_buf_size, actual_len_bytes);

    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);
    terminated();
}

/*------------------handlers------------------*/

utest::v1::status_t setup_handler(const Case *const source, const size_t index_of_case)
{
    int status = STORAGELITE_SUCCESS;

    stlite = new StorageLite();

    status = stlite->init(&flash_bd);
    TEST_ASSERT_EQUAL_MESSAGE(STORAGELITE_SUCCESS, status, "StorageLite::init failed\n");

    return STATUS_CONTINUE;
}

utest::v1::status_t setup_handler_set_file(const Case *const source, const size_t index_of_case)
{
    int status = STORAGELITE_SUCCESS;

    stlite = new StorageLite();

    status = stlite->init(&flash_bd);
    TEST_ASSERT_EQUAL_MESSAGE(STORAGELITE_SUCCESS, status, "StorageLite::init failed\n");

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, 0);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    return STATUS_CONTINUE;
}

utest::v1::status_t setup_handler_set_multiple_files(const Case *const source, const size_t index_of_case)
{
    int status = STORAGELITE_SUCCESS;

    stlite = new StorageLite();

    status = stlite->init(&flash_bd);
    TEST_ASSERT_EQUAL_MESSAGE(STORAGELITE_SUCCESS, status, "StorageLite::init failed\n");

    uint8_t data_buf[data_buf_size] = {0};
    status = stlite->set(&default_name, default_name_size, data_buf, data_buf_size, 0);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    uint8_t new_file_name = default_name + 1;
    status = stlite->set(&new_file_name, default_name_size, data_buf, data_buf_size, 0);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, status);

    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    /*------------------set()------------------*/
    Case("storagelite_set_name_null_name_len zero",
         setup_handler, storagelite_set_name_null_name_len_zero),
    Case("storagelite_set_name_null_name_len_not_zero",
         setup_handler, storagelite_set_name_null_name_len_not_zero),
    Case("storagelite_set_name_len_zero_name_not_null",
         setup_handler, storagelite_set_name_len_zero_name_not_null),
    Case("storagelite_set_name_len_bigger_than_max",
         setup_handler, storagelite_set_name_len_bigger_than_max),
    Case("storagelite_set_invalid_flags",
         setup_handler, storagelite_set_invalid_flags),
    Case("storagelite_set_buf_size_not_zero_buf_null",
         setup_handler, storagelite_set_buf_size_not_zero_buf_null),
    Case("storagelite_set_buf_size_zero_buf_not_null",
         setup_handler, storagelite_set_buf_size_zero_buf_not_null),
    Case("storagelite_set_buf_size_zero_buf_null",
         setup_handler, storagelite_set_buf_size_zero_buf_null),
    Case("storagelite_set_two_files_same_params",
         setup_handler, storagelite_set_two_files_same_params),
    Case("storagelite_set_multithreded",
         setup_handler, storagelite_set_multithreded),
    /*------------------get()------------------*/
    Case("storagelite_get_name_null_name_len_zero",
         setup_handler_set_file, storagelite_get_name_null_name_len_zero),
    Case("storagelite_get_name_null_name_len_not_zero",
         setup_handler_set_file, storagelite_get_name_null_name_len_not_zero),
    Case("storagelite_get_name_len_zero_name_not_null",
         setup_handler_set_file, storagelite_get_name_len_zero_name_not_null),
    Case("storagelite_get_name_len_bigger_than_max",
         setup_handler_set_file, storagelite_get_name_len_bigger_than_max),
    Case("storagelite_get_buf_size_not_zero_buf_null",
         setup_handler_set_file, storagelite_get_buf_size_not_zero_buf_null),
    Case("storagelite_get_not_empty_file_buf_size_zero_buf_null",
         setup_handler_set_file, storagelite_get_not_empty_file_buf_size_zero_buf_null),
    Case("storagelite_get_empty_file_buf_size_zero_buf_null",
         setup_handler_set_file, storagelite_get_empty_file_buf_size_zero_buf_null),
    Case("storagelite_get_buf_size_insufficient",
         setup_handler_set_file, storagelite_get_buf_size_insufficient),
    Case("storagelite_get_non_existing_file",
         setup_handler_set_file, storagelite_get_non_existing_file),
    Case("storagelite_get_existing_file",
         setup_handler_set_file, storagelite_get_existing_file),
    Case("storagelite_get_removed_file",
         setup_handler_set_file, storagelite_get_removed_file),
    Case("storagelite_get_rollback_file",
         setup_handler, storagelite_get_rollback_file),
    Case("storagelite_get_corrupt_rollback_file",
         setup_handler, storagelite_get_corrupt_rollback_file),
    Case("storagelite_get_encrypt_file",
         setup_handler, storagelite_get_encrypt_file),
    /*------------------remove()------------------*/
    Case("storagelite_remove_name_null_name_len_zero",
         setup_handler_set_file, storagelite_remove_name_null_name_len_zero),
    Case("storagelite_remove_name_null_name_len_not_zero",
         setup_handler_set_file, storagelite_remove_name_null_name_len_not_zero),
    Case("storagelite_remove_name_not_null_name_len_zero",
         setup_handler_set_file, storagelite_remove_name_not_null_name_len_zero),
    Case("storagelite_remove_name_len_bigger_than_max",
         setup_handler_set_file, storagelite_remove_name_len_bigger_than_max),
    Case("storagelite_remove_existing_file",
         setup_handler_set_file, storagelite_remove_existing_file),
    Case("storagelite_remove_non_existing_file",
         setup_handler, storagelite_remove_non_existing_file),
    Case("storagelite_remove_removed_file",
         setup_handler_set_file, storagelite_remove_removed_file),
    Case("storagelite_remove_fr_file_try_get",
         setup_handler, storagelite_remove_fr_file_try_get),
    /*------------------get_item_size()------------------*/
    Case("storagelite_get_item_size_name_null_name_len_zero",
         setup_handler_set_file, storagelite_get_item_size_name_null_name_len_zero),
    Case("storagelite_get_item_size_name_null_name_len_not_zero",
         setup_handler_set_file, storagelite_get_item_size_name_null_name_len_not_zero),
    Case("storagelite_get_item_size_name_not_null_name_len_zero",
         setup_handler_set_file, storagelite_get_item_size_name_not_null_name_len_zero),
    Case("storagelite_get_item_size_name_len_bigger_than_max",
         setup_handler_set_file, storagelite_get_item_size_name_len_bigger_than_max),
    Case("storagelite_get_item_size_non_existing_file",
         setup_handler_set_file, storagelite_get_item_size_non_existing_file),
    Case("storagelite_get_item_size_existing_file",
         setup_handler_set_file, storagelite_get_item_size_existing_file),
    Case("storagelite_get_item_size_removed_file",
         setup_handler_set_file, storagelite_get_item_size_removed_file),
    Case("storagelite_get_item_size_empty_file",
         setup_handler, storagelite_get_item_size_empty_file),
    Case("storagelite_get_item_size_modified_file",
         setup_handler_set_file, storagelite_get_item_size_modified_file),
    Case("storagelite_get_item_size_fr_file_try_get",
         setup_handler_set_file, storagelite_get_item_size_fr_file_try_get),
    /*------------------file_exists()------------------*/
    Case("storagelite_file_exists_name_null_name_len_zero",
         setup_handler_set_file, storagelite_file_exists_name_null_name_len_zero),
    Case("storagelite_file_exists_name_null_name_len_not_zero",
         setup_handler_set_file, storagelite_file_exists_name_null_name_len_not_zero),
    Case("storagelite_file_exists_name_not_null_name_len_zero",
         setup_handler_set_file, storagelite_file_exists_name_not_null_name_len_zero),
    Case("storagelite_file_exists_name_len_bigger_than_max",
         setup_handler_set_file, storagelite_file_exists_name_len_bigger_than_max),
    Case("storagelite_file_exists_non_existing_file",
         setup_handler_set_file, storagelite_file_exists_non_existing_file),
    Case("storagelite_file_exists_existing_file",
         setup_handler_set_file, storagelite_file_exists_existing_file),
    Case("storagelite_file_exists_removed_file",
         setup_handler_set_file, storagelite_file_exists_removed_file),
    Case("storagelite_file_exists_fr_file_try_get",
         setup_handler_set_file, storagelite_file_exists_fr_file_try_get),
    /*------------------get_file_flags()------------------*/
    Case("storagelite_get_file_flags_name_null_name_len_zero",
         setup_handler_set_file, storagelite_get_file_flags_name_null_name_len_zero),
    Case("storagelite_get_file_flags_name_null_name_len_not_zero",
         setup_handler_set_file, storagelite_get_file_flags_name_null_name_len_not_zero),
    Case("storagelite_get_file_flags_name_not_null_name_len_zero",
         setup_handler_set_file, storagelite_get_file_flags_name_not_null_name_len_zero),
    Case("storagelite_get_file_flags_name_len_bigger_than_max",
         setup_handler_set_file, storagelite_get_file_flags_name_len_bigger_than_max),
    Case("storagelite_get_file_flags_non_existing_file",
         setup_handler_set_file, storagelite_get_file_flags_non_existing_file),
    Case("storagelite_get_file_flags_existing_file",
         setup_handler_set_file, storagelite_get_file_flags_existing_file),
    Case("storagelite_get_file_flags_removed_file",
         setup_handler_set_file, storagelite_get_file_flags_removed_file),
    Case("storagelite_get_file_flags_fr_file_try_get",
         setup_handler_set_file, storagelite_get_file_flags_fr_file_try_get),
    /*------------------get_first_file()------------------*/
    Case("storagelite_get_first_file_max_name_size_zero",
         setup_handler_set_file, storagelite_get_first_file_max_name_size_zero),
    Case("storagelite_get_first_file_max_name_bigger_than_max",
         setup_handler_set_file, storagelite_get_first_file_max_name_bigger_than_max),
    Case("storagelite_get_first_file_no_file_in_storage",
         setup_handler, storagelite_get_first_file_no_file_in_storage),
    Case("storagelite_get_first_file_max_name_size_too_small",
         setup_handler_set_file, storagelite_get_first_file_max_name_size_too_small),
    Case("storagelite_get_first_file_valid_flow",
         setup_handler_set_file, storagelite_get_first_file_valid_flow),
    Case("storagelite_get_first_file_not_first",
         setup_handler, storagelite_get_first_file_not_first),
    /*------------------get_next_file()------------------*/
    Case("storagelite_get_next_file_max_name_size_zero",
         setup_handler_set_multiple_files, storagelite_get_next_file_max_name_size_zero),
    Case("storagelite_get_next_file_no_file_in_storage",
         setup_handler_set_file, storagelite_get_next_file_no_file_in_storage),
    Case("storagelite_get_next_file_max_name_size_too_small",
         setup_handler_set_multiple_files, storagelite_get_next_file_max_name_size_too_small),
    Case("storagelite_get_next_file_valid_flow",
         setup_handler_set_multiple_files, storagelite_get_next_file_valid_flow),
    Case("storagelite_get_next_file_invalid_handle",
         setup_handler_set_multiple_files, storagelite_get_next_file_invalid_handle),
    Case("storagelite_get_next_file_not_first",
         setup_handler_set_multiple_files, storagelite_get_next_file_not_first),
    /*------------------factory_reset()------------------*/
    Case("storagelite_factory_reset_get_file",
         setup_handler, storagelite_factory_reset_get_file),
    Case("storagelite_factory_reset_get_file_without_fr_flag",
         setup_handler, storagelite_factory_reset_get_file_without_fr_flag),
    Case("storagelite_factory_reset_get_original_fr_file",
         setup_handler, storagelite_factory_reset_get_original_fr_file),
    Case("storagelite_factory_reset_get_modified_fr_file",
         setup_handler, storagelite_factory_reset_get_modified_fr_file),
    Case("storagelite_factory_reset_get_removed_fr_file",
         setup_handler, storagelite_factory_reset_get_removed_fr_file),
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
