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
#include "StorageLiteFS.h"
#include "FlashSimBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "HeapBlockDevice.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

// Define this if you want to run the test locally (requires SPIF/SD block device)
#undef LOCAL_TEST

#ifdef LOCAL_TEST
#undef TEST_SPIF
#ifdef TEST_SPIF
#include "SPIFBlockDevice.h"
#endif

#undef TEST_SD
#ifdef TEST_SD
#include "SDBlockDevice.h"
#endif
#endif

#if !STORAGELITE_ENABLED
#error [NOT_SUPPORTED] StorageLite needs to be enabled for this test
#endif

// MBED_TEST_SIM_BLOCKDEVICE is used here only to filter out inappropriate boards (little RAM etc.)
#if !defined(MBED_TEST_SIM_BLOCKDEVICE) && !defined(LOCAL_TEST)
#error [NOT_SUPPORTED] StorageLite test not supported on this platform
#endif

using namespace utest::v1;

static const size_t buf_size          = 10;
static const size_t test_buffer       = 8192;
static const size_t test_files        = 4;

FILE *fd[test_files];
uint8_t wbuffer[test_buffer];
uint8_t rbuffer[test_buffer];
uint8_t buffer[test_buffer];
StorageLite stlite;


#ifdef TEST_SPIF
#ifdef TARGET_K82F
SPIFBlockDevice bd(PTE2, PTE4, PTE1, PTE5);
#else
SPIFBlockDevice bd(D11, D12, D13, D8);
#endif
SlicingBlockDevice flash_bd(&bd, 0 * 4096, bd.size());
#elif defined(TEST_SD)
HeapBlockDevice bd(512 * 512, 16, 16, 512);
BufferedBlockDevice buf_bd(&bd);
FlashSimBlockDevice flash_bd(&buf_bd);
#endif

#if !defined(TEST_SPIF) && !defined(TEST_SD)
HeapBlockDevice bd(4096 * 4, 1,  1, 4096);
FlashSimBlockDevice flash_bd(&bd);
#endif

/*----------------help functions------------------*/

static void init()
{
    int result = STORAGELITE_SUCCESS;

    result = stlite.init(&flash_bd);
    TEST_ASSERT_EQUAL(STORAGELITE_SUCCESS, result);
}

static void fs_init(StorageLiteFS *stlitefs)
{
    int result = STORAGELITE_SUCCESS;

    result = stlitefs->mount(&flash_bd);
    TEST_ASSERT_EQUAL(0, result);

    result = stlitefs->reformat(&flash_bd);
    TEST_ASSERT_EQUAL(0, result);
}

static void open_write_file(FILE *fd, size_t buf_size)
{
    int res = !((fd = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd);
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fopen()------------------*/

//fopen path without stfs prefix
static void StorageLiteFS_fopen_path_not_valid()
{
    int res = !((fd[0] = fopen("hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen empty file name with r mode
static void StorageLiteFS_fopen_empty_path_r_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "", "r")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen empty file name with w mode
static void StorageLiteFS_fopen_empty_path_w_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);
}

//fopen empty mode
static void StorageLiteFS_fopen_invalid_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "invalid_mode", "")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with unsupported a mode
static void StorageLiteFS_fopen_unsupported_a_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "unsupported_mode", "a")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with unsupported a+ mode
static void StorageLiteFS_fopen_unsupported_a_plus_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "unsupported_mode", "a+")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with unsupported r+ mode
static void StorageLiteFS_fopen_unsupported_r_plus_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "unsupported_mode", "r+")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with unsupported w+ mode
static void StorageLiteFS_fopen_unsupported_w_plus_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "unsupported_mode", "w+")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with unsupported rb mode
static void StorageLiteFS_fopen_supported_rb_plus_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with unsupported wb mode
static void StorageLiteFS_fopen_supported_wb_plus_mode()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "unsupported_mode", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fclose()------------------*/

//fclose valid flow
static void StorageLiteFS_fclose_valid_flow()
{
    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}


//fwrite with size zero
static void StorageLiteFS_fwrite_size_zero()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, 0, buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite with nmemb zero
static void StorageLiteFS_fwrite_nmemb_zero()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite valid flow
static void StorageLiteFS_fwrite_valid_flow()
{
    char buffer[buf_size] = "good_day", rbuffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(rbuffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);
    TEST_ASSERT_EQUAL_STRING(buffer, rbuffer);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite to fopen mode r
static void StorageLiteFS_fwrite_with_fopen_r_mode()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite to closed file
static void StorageLiteFS_fwrite_closed_file()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);
}

//fwrite twice to the same file
static void StorageLiteFS_fwrite_twice_to_file()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(-1, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite twice to the same file
static void StorageLiteFS_fwrite_twice_separated_with_fclose()
{
    char buffer[buf_size] = "good_day";

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fread()------------------*/

//fread with size zero
static void StorageLiteFS_fread_size_zero()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, 0, buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread with nmemb zero
static void StorageLiteFS_fread_nmemb_zero()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread valid flow
static void StorageLiteFS_fread_valid_flow()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread after fwrite without fclose
static void StorageLiteFS_fread_fwrite_no_fclose()
{
    char buffer[buf_size] = "good_day", rbuffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(rbuffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread to fopen mode w
static void StorageLiteFS_fread_with_fopen_w_mode()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread to closed file
static void StorageLiteFS_fread_closed_file()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);
}

//fread twice from file
static void StorageLiteFS_fread_twice_from_file()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);

    read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}

//fread twice from file
static void StorageLiteFS_fread_twice_separated_with_fclose()
{
    char buffer[buf_size] = {};

    init();
    StorageLiteFS stlitefs("stfs", &stlite, StorageLite::encrypt_flag);
    fs_init(&stlitefs);

    open_write_file(fd[0], buf_size);

    int res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/stfs/" "hello", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    read_sz = fread(buffer, sizeof(char), buf_size, fd[0]);
    TEST_ASSERT_EQUAL(buf_size, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);
}


/*----------------setup------------------*/

utest::v1::status_t setup_init(const Case *const source, const size_t index_of_case, uint16_t max_bd_files)
{
    return STATUS_CONTINUE;
}

utest::v1::status_t tear_down_handler(const Case *const source, const size_t passed, const size_t failed,
                                      const failure_t reason)
{
    return STATUS_CONTINUE;
}

utest::v1::status_t failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("StorageLiteFS_fopen_path_not_valid", StorageLiteFS_fopen_path_not_valid),
    Case("StorageLiteFS_fopen_empty_path_r_mode", StorageLiteFS_fopen_empty_path_r_mode),
    Case("StorageLiteFS_fopen_empty_path_w_mode", StorageLiteFS_fopen_empty_path_w_mode),
    Case("StorageLiteFS_fopen_invalid_mode", StorageLiteFS_fopen_invalid_mode),
    Case("StorageLiteFS_fopen_unsupported_a_mode", StorageLiteFS_fopen_unsupported_a_mode),
    Case("StorageLiteFS_fopen_unsupported_a_plus_mode", StorageLiteFS_fopen_unsupported_a_plus_mode),
    Case("StorageLiteFS_fopen_unsupported_r_plus_mode", StorageLiteFS_fopen_unsupported_r_plus_mode),
    Case("StorageLiteFS_fopen_unsupported_w_plus_mode", StorageLiteFS_fopen_unsupported_w_plus_mode),
    Case("StorageLiteFS_fopen_supported_rb_plus_mode", StorageLiteFS_fopen_supported_rb_plus_mode),
    Case("StorageLiteFS_fopen_supported_wb_plus_mode", StorageLiteFS_fopen_supported_wb_plus_mode),

    Case("StorageLiteFS_fclose_valid_flow", StorageLiteFS_fclose_valid_flow),

    Case("StorageLiteFS_fwrite_size_zero", StorageLiteFS_fwrite_size_zero),
    Case("StorageLiteFS_fwrite_nmemb_zero", StorageLiteFS_fwrite_nmemb_zero),
    Case("StorageLiteFS_fwrite_valid_flow", StorageLiteFS_fwrite_valid_flow),
    Case("StorageLiteFS_fwrite_with_fopen_r_mode", StorageLiteFS_fwrite_with_fopen_r_mode),
    Case("StorageLiteFS_fwrite_closed_file", StorageLiteFS_fwrite_closed_file),
    Case("StorageLiteFS_fwrite_twice_to_file", StorageLiteFS_fwrite_twice_to_file),
    Case("StorageLiteFS_fwrite_twice_separated_with_fclose", StorageLiteFS_fwrite_twice_separated_with_fclose),

    Case("StorageLiteFS_fread_size_zero", StorageLiteFS_fread_size_zero),
    Case("StorageLiteFS_fread_nmemb_zero", StorageLiteFS_fread_nmemb_zero),
    Case("StorageLiteFS_fread_valid_flow", StorageLiteFS_fread_valid_flow),
    Case("StorageLiteFS_fread_fwrite_no_fclose", StorageLiteFS_fread_fwrite_no_fclose),
    Case("StorageLiteFS_fread_with_fopen_w_mode", StorageLiteFS_fread_with_fopen_w_mode),
    Case("StorageLiteFS_fread_closed_file", StorageLiteFS_fread_closed_file),
    Case("StorageLiteFS_fread_twice_from_file", StorageLiteFS_fread_twice_from_file),
    Case("StorageLiteFS_fread_twice_separated_with_fclose", StorageLiteFS_fread_twice_separated_with_fclose),
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
