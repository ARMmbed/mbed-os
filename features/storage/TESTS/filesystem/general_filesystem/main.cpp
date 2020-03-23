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

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include "BlockDevice.h"
#include "FileSystem.h"

#include <stdlib.h>
#include "LittleFileSystem.h"

#if COMPONENT_SPIF
#include "SPIFBlockDevice.h"
#elif COMPONENT_QSPIF
#include "QSPIFBlockDevice.h"
#elif COMPONENT_OSPIF
#include "OSPIFBlockDevice.h"
#elif COMPONENT_SD
#include "SDBlockDevice.h"
#else
#error [NOT_SUPPORTED] storage test not supported on this platform
#endif

using namespace utest::v1;
using namespace mbed;

static const size_t small_buf_size = 10;
static const size_t medium_buf_size = 250;
static const size_t large_buf_size = 1200;
static const size_t test_files = 2;

FILE *fd[test_files];

BlockDevice *bd = BlockDevice::get_default_instance();
FileSystem  *fs = FileSystem::get_default_instance();
const char *bd_type;

/*----------------help functions------------------*/

static void init()
{
    int res = bd->init();
    TEST_ASSERT_EQUAL(0, res);

    res = fs->reformat(bd);
    TEST_ASSERT_EQUAL(0, res);
}

static void deinit()
{
    int res = bd->deinit();
    TEST_ASSERT_EQUAL(0, res);

    res = fs->reformat(bd);
    TEST_ASSERT_EQUAL(0, res);

    res = fs->unmount();
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fopen()------------------*/

//init the blockdevice and reformat the filesystem
static void bd_init_fs_reformat()
{
    bd_type = bd->get_type();
    init();
}

//fopen path without fs prefix
static void FS_fopen_path_not_valid()
{
    int res = !((fd[0] = fopen("filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);

}

//fopen empty file name with r mode
static void FS_fopen_empty_path_r_mode()
{
    int res = !((fd[0] = fopen("/default/" "", "rb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen empty file name with w mode
static void FS_fopen_empty_path_w_mode()
{
    int res = !((fd[0] = fopen("/default/" "", "wb")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen empty mode
static void FS_fopen_invalid_mode()
{
    int res = !((fd[0] = fopen("/default/" "Invalid_mode", "")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with valid flowSystemStorage
static void FS_fopen_supported_wb_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with append mode
static void FS_fopen_supported_a_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "a")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with read mode
static void FS_fopen_supported_r_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with append update mode
static void FS_fopen_supported_a_update_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "a+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with read update mode
static void FS_fopen_supported_r_update_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "r+")) != NULL);
    TEST_ASSERT_EQUAL(1, res);
}

//fopen with write update mode
static void FS_fopen_supported_w_update_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "w+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with read update create, check contents exist
static void FS_fopen_read_update_create()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = "";

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, read_sz);
    TEST_ASSERT_EQUAL_STRING_LEN(write_buf, read_buf, small_buf_size);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fopen with write update create, check contents doesnt exist
static void FS_fopen_write_update_create()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = "";

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "w+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}
/*----------------fclose()------------------*/

//fclose valid flow
static void FS_fclose_valid_flow()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fwrite()------------------*/

//fwrite with nmemb zero
static void FS_fwrite_nmemb_zero()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite valid flow
static void FS_fwrite_valid_flow()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fwrite to fopen mode r
static void FS_fwrite_with_fopen_r_mode()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fread()------------------*/
#if !defined(__MICROLIB)
//fread with size zero
static void FS_fread_size_zero()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, 0, small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}
#endif

//fread with nmemb zero
static void FS_fread_nmemb_zero()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), 0, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread to fopen mode w
static void FS_fread_with_fopen_w_mode()
{
    char buffer[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread to fwrite file
static void FS_fread_to_fwrite_file()
{
    char read_buf[small_buf_size] = {};
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread empty file
static void FS_fread_empty_file()
{
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread valid flow small file
static void FS_fread_valid_flow_small_file()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}


//fread valid flow medium file
static void FS_fread_valid_flow_medium_file()
{
    char write_buf[medium_buf_size] = { 1 };
    char read_buf[medium_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread valid flow large file
static void FS_fread_valid_flow_large_file()
{
    char write_buf[large_buf_size] = { 1 };
    char read_buf[large_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fread valid flow small file read more than write
static void FS_fread_valid_flow_small_file_read_more_than_write()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size + 10] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fgetc()------------------*/

//fgetc to an empty file
static void FS_fgetc_empty_file()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgetc valid flow
static void FS_fgetc_valid_flow()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};
    int ch = 0;
    unsigned int i = 0;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 0; (i < (sizeof(read_buf) - 1) && ((ch = fgetc(fd[0])) != EOF) && (ch != '\n')); i++) {
        read_buf[i] = ch;
    }

    read_buf[i] = '\0';
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgetc to fopen mode w
static void FS_fgetc_with_fopen_w_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fgets()------------------*/

//fgets to an empty file
static void FS_fgets_empty_file()
{
    char buffer[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(buffer, sizeof(buffer), fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgets with buffer null with zero len of buffer string
static void FS_fgets_null_buffer_zero_len()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(NULL, 0, fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgets with buffer null
static void FS_fgets_null_buffer()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(NULL, small_buf_size, fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgets valid flow
static void FS_fgets_valid_flow()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NOT_NULL(fgets(read_buf, sizeof(read_buf), fd[0]));
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgets up to new line character
static void FS_fgets_new_line()
{
    char write_buf[small_buf_size] = "good_day";
    char read_buf[small_buf_size] = {};

    write_buf[4] = '\n';

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NOT_NULL(fgets(read_buf, sizeof(read_buf), fd[0]));
    TEST_ASSERT_EQUAL_STRING("good\n", read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fgets to fopen mode w
static void FS_fgets_with_fopen_w_mode()
{
    char buffer[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_NULL(fgets(buffer, sizeof(buffer), fd[0]));

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fflush()------------------*/

#if !defined(__MICROLIB)
//fflush with null
static void FS_fflush_null_stream()
{
    int res = fflush(NULL);
    TEST_ASSERT_EQUAL(0, res);
}
#endif

//fflush valid flow
static void FS_fflush_valid_flow()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}


//fflush twice
static void FS_fflush_twice()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(buffer, sizeof(char), small_buf_size, fd[0]);
    TEST_ASSERT_EQUAL(small_buf_size, write_sz);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fflush(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fputc()------------------*/

//fputc valid flow
static void FS_fputc_valid_flow()
{
    int write_ch = 10, read_ch = 0;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputc(write_ch, fd[0]);
    TEST_ASSERT_EQUAL(write_ch, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    read_ch = fgetc(fd[0]);
    TEST_ASSERT_EQUAL(write_ch, read_ch);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fputc with file open for read mode
static void FS_fputc_in_read_mode()
{
    int write_ch = 10;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputc(write_ch, fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fputs()------------------*/

//fputs valid flow
static void FS_fputs_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputs(write_buf, fd[0]);
    TEST_ASSERT_NOT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf) - 1, read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fputs with file open for read mode
static void FS_fputs_in_read_mode()
{
    char buffer[small_buf_size] = "good_day";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fputs(buffer, fd[0]);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fseek()------------------*/

//fseek empty file, SEEK_SET, offset 0
static void FS_fseek_empty_file_seek_set()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_SET, offset 0
static void FS_fseek_non_empty_file_seek_set()
{
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek empty file, SEEK_SET, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_set()
{
    char read_buf[small_buf_size] = "";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 10, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);
#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_SET, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_set()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = "";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek empty file, SEEK_CUR, offset 0
static void FS_fseek_empty_file_seek_cur()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_CUR, offset 0
static void FS_fseek_non_empty_file_seek_cur()
{
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek empty file, SEEK_CUR, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_cur()
{
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 1, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_CUR, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_cur()
{
    char read_buf[small_buf_size] = {};
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek empty file, SEEK_END, offset 0
static void FS_fseek_empty_file_seek_end()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_END, offset 0
static void FS_fseek_non_empty_file_seek_end()
{
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(write_sz, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek empty file, SEEK_END, offset 1 - beyond end of file
static void FS_fseek_beyond_empty_file_seek_end()
{
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 1, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_END, offset data_size + 1 - beyond end of file
static void FS_fseek_beyond_non_empty_file_seek_end()
{
    char read_buf[small_buf_size] = {};
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], sizeof(write_buf) + 1, SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(0, read_sz);

#if !defined(__MICROLIB)
    // feof() always returns 0 because the EOF indicator is not supported by Microlib
    res = feof(fd[0]);
    TEST_ASSERT_NOT_EQUAL(0, res);
#endif

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fseek non empty file, SEEK_END, offset negative
static void FS_fseek_negative_non_empty_file_seek_end()
{
    char write_buf[small_buf_size] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], -(sizeof(write_buf)), SEEK_END);
    TEST_ASSERT_EQUAL(0, res);

    int pos = ftell(fd[0]);
    TEST_ASSERT_EQUAL(0, pos);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fgetpos()------------------*/

//fgetpos rewrite file and check data
static void FS_fgetpos_rewrite_check_data()
{
    char write_buf[small_buf_size] = "123456789";
    char rewrite_buf[small_buf_size] = "987654321";
    char read_buf[small_buf_size] = {};
    fpos_t pos;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fgetpos(fd[0], &pos);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fsetpos(fd[0], &pos);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(rewrite_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fscanf()------------------*/

//fscanf valid flow
static void FS_fscanf_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};
    int num = 0;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d %s", 123, write_buf);
    TEST_ASSERT_EQUAL(3 + sizeof(write_buf), res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_INT(123, num);

    res = fscanf(fd[0], "%s", read_buf);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fscanf empty file
static void FS_fscanf_empty_file()
{
    int num = 0;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fscanf more fields than exist
static void FS_fscanf_more_fields_than_exist()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};
    int num = 0;

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d %s", 123, write_buf);
    TEST_ASSERT_EQUAL(3 + sizeof(write_buf), res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_INT(123, num);

    res = fscanf(fd[0], "%s", read_buf);
    TEST_ASSERT_EQUAL(1, res);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fscanf(fd[0], "%d", &num);
    TEST_ASSERT_EQUAL(EOF, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------fprintf()------------------*/

//fprintf in mode r
static void FS_fprintf_read_mode()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fprintf(fd[0], "%d", 123);
    TEST_ASSERT_NOT_EQUAL(1, res);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------freopen()------------------*/

//freopen point to the same file with two file handler
static void FS_freopen_point_to_same_file()
{
    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[1] = freopen("/default/" "new_file_name", "wb", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    TEST_ASSERT_EQUAL(fd[0], fd[1]);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//freopen valid flow
static void FS_freopen_valid_flow()
{
    char write_buf[small_buf_size] = "123456789";
    char read_buf[small_buf_size] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "wb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = !((fd[1] = freopen("/default/" "filename", "rb", fd[0])) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

/*----------------general------------------*/

//create a 1 byte file
static void FS_fopen_write_one_byte_file()
{
    char write_buf = 1;
    char read_buf[1] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(&write_buf, sizeof(char), 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL(write_buf, read_buf[0]);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//create a 2 bytes file
static void FS_fopen_write_two_byte_file()
{
    char write_buf[2] = "1";
    char read_buf[2] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//create a 5 bytes file
static void FS_fopen_write_five_byte_file()
{
    char write_buf[5] = "1234";
    char read_buf[5] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//create a 15 bytes file
static void FS_fopen_write_fifteen_byte_file()
{
    char write_buf[15] = "12345678901234";
    char read_buf[15] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//create a 5000 bytes file
static void FS_fopen_write_five_Kbyte_file()
{
    int buf_sz = 5000;
    char *write_buf = (char *)malloc(buf_sz);
    char read_buf[10] = {};
    char check_buf[10] = "123456789";

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    memcpy(write_buf, check_buf, sizeof(check_buf) - 1);
    int write_sz = fwrite(write_buf, sizeof(char), buf_sz, fd[0]);
    TEST_ASSERT_EQUAL(buf_sz, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "rb+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(check_buf) - 1, read_sz);
    TEST_ASSERT_EQUAL_STRING(check_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//rewrite file begining
static void FS_fseek_rewrite_non_empty_file_begining()
{
    char write_buf[15] = "12345678901234";
    char rewrite_buf[6] = "abcde";
    char check_buf[15] = "abcde678901234";
    char read_buf[15] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf) - 1, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(read_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(check_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//rewrite file middle
static void FS_fseek_rewrite_non_empty_file_middle()
{
    char write_buf[15] = "12345678901234";
    char rewrite_buf[6] = "abcde";
    char check_buf[15] = "12345abcde1234";
    char read_buf[15] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 5, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf) - 1, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(check_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//rewrite file end
static void FS_fseek_rewrite_non_empty_file_end()
{
    char write_buf[15] = "12345678901234";
    char rewrite_buf[6] = "abcde";
    char check_buf[15] = "123456789abcde";
    char read_buf[15] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 9, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(rewrite_buf) - 1, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    res = fseek(fd[0], 0, SEEK_SET);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(check_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//append buffer to empty file
static void FS_append_empty_file()
{
    char write_buf[17] = "1234567890123456";
    char read_buf[17] = {};

    int res = !((fd[0] = fopen("/default/" "filename", "a")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), read_sz);
    TEST_ASSERT_EQUAL_STRING(write_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//append buffer to non empty file
static void FS_append_non_empty_file()
{
    char write_buf[17] = "1234567890123456";
    char rewrite_buf[17] = "abcdefghijklmnop";
    char read_buf[34] = {};
    char check_buf[34] = "1234567890123456abcdefghijklmnop";

    int res = !((fd[0] = fopen("/default/" "filename", "a")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    int write_sz = fwrite(write_buf, sizeof(char), sizeof(write_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf) - 1, write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "a+")) != NULL);
    TEST_ASSERT_EQUAL(0, res);
#if defined(__MICROLIB)
    // Microlib does not support opening a file in an append mode.
    fseek(fd[0], 0L, SEEK_END);
#endif
    write_sz = fwrite(rewrite_buf, sizeof(char), sizeof(rewrite_buf), fd[0]);
    TEST_ASSERT_EQUAL(sizeof(write_buf), write_sz);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    memcpy(check_buf, write_buf, sizeof(write_buf) - 1);
    memcpy(check_buf + sizeof(write_buf) - 1, rewrite_buf, sizeof(rewrite_buf));

    int read_sz = fread(read_buf, sizeof(char), sizeof(read_buf) - 1, fd[0]);
    TEST_ASSERT_EQUAL(sizeof(check_buf) - 1, read_sz);
    TEST_ASSERT_EQUAL_STRING(check_buf, read_buf);

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fill write_buf buffer with random data, read back the data from the file
static void FS_write_read_random_data()
{
    char write_buf[medium_buf_size] = {};
    unsigned int i;

    // Fill write_buf buffer with random data and write the data into the file
    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 0; i < medium_buf_size; i++) {
        write_buf[i] = rand() % 0XFF;
        res = fprintf(fd[0], "%c", write_buf[i]);
        TEST_ASSERT_EQUAL(1, res);
    }

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    // Read back the data from the file and store them in data_read
    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 0; i < medium_buf_size; i++) {
        uint8_t data = fgetc(fd[0]);
        TEST_ASSERT_EQUAL(write_buf[i], data);
    }

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//fill write_buf buffer with random data, read back the data from the file
static void FS_fill_data_and_seek()
{
    unsigned int i, j;

    int res = !((fd[0] = fopen("/default/" "filename", "w")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 0; i < 256; i++) {
        res = fputc(i, fd[0]);
        TEST_ASSERT_EQUAL(i, res);
    }

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = !((fd[0] = fopen("/default/" "filename", "r")) != NULL);
    TEST_ASSERT_EQUAL(0, res);

    for (i = 1; i <= 255; i++) {

        res = fseek(fd[0], (long) - i, SEEK_END);
        TEST_ASSERT_EQUAL(0, res);

        j = getc(fd[0]);
        TEST_ASSERT_EQUAL(256 - i, j);

        res = fseek(fd[0], (long)i, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);

        j = getc(fd[0]);
        TEST_ASSERT_EQUAL(i, j);

        res = fseek(fd[0], (long)i, SEEK_SET);
        TEST_ASSERT_EQUAL(0, res);

        res = fseek(fd[0], (long)(i >= 128 ? -128 : 128), SEEK_CUR);
        TEST_ASSERT_EQUAL(0, res);

        j = getc(fd[0]);
        TEST_ASSERT_EQUAL((i >= 128 ? i - 128 : i + 128), j);
    }

    res = fclose(fd[0]);
    TEST_ASSERT_EQUAL(0, res);

    res = remove("/default/" "filename");
    TEST_ASSERT_EQUAL(0, res);
}

//deinit the blockdevice and unmount the filesystem
static void bd_deinit_fs_unmount()
{
    deinit();
}

/*----------------setup------------------*/

Case cases[] = {
    Case("bd_init_fs_reformat", bd_init_fs_reformat),

    Case("FS_fopen_path_not_valid", FS_fopen_path_not_valid),
    Case("FS_fopen_empty_path_r_mode", FS_fopen_empty_path_r_mode),
    Case("FS_fopen_empty_path_w_mode", FS_fopen_empty_path_w_mode),
    Case("FS_fopen_invalid_mode", FS_fopen_invalid_mode),
    Case("FS_fopen_supported_wb_mode", FS_fopen_supported_wb_mode),
    Case("FS_fopen_supported_a_mode", FS_fopen_supported_a_mode),
    Case("FS_fopen_supported_r_mode", FS_fopen_supported_r_mode),
    Case("FS_fopen_supported_a_update_mode", FS_fopen_supported_a_update_mode),
    Case("FS_fopen_supported_r_update_mode", FS_fopen_supported_r_update_mode),
    Case("FS_fopen_supported_w_update_mode", FS_fopen_supported_w_update_mode),
    Case("FS_fopen_read_update_create", FS_fopen_read_update_create),
    Case("FS_fopen_write_update_create", FS_fopen_write_update_create),

    Case("FS_fclose_valid_flow", FS_fclose_valid_flow),

    Case("FS_fwrite_nmemb_zero", FS_fwrite_nmemb_zero),
    Case("FS_fwrite_valid_flow", FS_fwrite_valid_flow),
    Case("FS_fwrite_with_fopen_r_mode", FS_fwrite_with_fopen_r_mode),
#if !defined(__MICROLIB)
    Case("FS_fread_size_zero", FS_fread_size_zero),
#endif
    Case("FS_fread_nmemb_zero", FS_fread_nmemb_zero),
    Case("FS_fread_with_fopen_w_mode", FS_fread_with_fopen_w_mode),
    Case("FS_fread_to_fwrite_file", FS_fread_to_fwrite_file),
    Case("FS_fread_empty_file", FS_fread_empty_file),
    Case("FS_fread_valid_flow_small_file", FS_fread_valid_flow_small_file),
    Case("FS_fread_valid_flow_medium_file", FS_fread_valid_flow_medium_file),
    Case("FS_fread_valid_flow_large_file", FS_fread_valid_flow_large_file),
    Case("FS_fread_valid_flow_small_file_read_more_than_write", FS_fread_valid_flow_small_file_read_more_than_write),

    Case("FS_fgetc_empty_file", FS_fgetc_empty_file),
    Case("FS_fgetc_valid_flow", FS_fgetc_valid_flow),
    Case("FS_fgetc_with_fopen_w_mode", FS_fgetc_with_fopen_w_mode),

    Case("FS_fgets_empty_file", FS_fgets_empty_file),
    Case("FS_fgets_null_buffer_zero_len", FS_fgets_null_buffer_zero_len),
    Case("FS_fgets_null_buffer", FS_fgets_null_buffer),
    Case("FS_fgets_valid_flow", FS_fgets_valid_flow),
    Case("FS_fgets_new_line", FS_fgets_new_line),
    Case("FS_fgets_with_fopen_w_mode", FS_fgets_with_fopen_w_mode),
#if !defined(__MICROLIB)
    Case("FS_fflush_null_stream", FS_fflush_null_stream),
#endif
    Case("FS_fflush_valid_flow", FS_fflush_valid_flow),
    Case("FS_fflush_twice", FS_fflush_twice),

    Case("FS_fputc_valid_flow", FS_fputc_valid_flow),
    Case("FS_fputc_in_read_mode", FS_fputc_in_read_mode),

    Case("FS_fputs_valid_flow", FS_fputs_valid_flow),
    Case("FS_fputs_in_read_mode", FS_fputs_in_read_mode),

    Case("FS_fseek_empty_file_seek_set", FS_fseek_empty_file_seek_set),
    Case("FS_fseek_non_empty_file_seek_set", FS_fseek_non_empty_file_seek_set),
    Case("FS_fseek_empty_file_seek_set", FS_fseek_beyond_empty_file_seek_set),
    Case("FS_fseek_beyond_non_empty_file_seek_set", FS_fseek_beyond_non_empty_file_seek_set),
    Case("FS_fseek_empty_file_seek_cur", FS_fseek_empty_file_seek_cur),
    Case("FS_fseek_non_empty_file_seek_cur", FS_fseek_non_empty_file_seek_cur),
    Case("FS_fseek_empty_file_seek_cur", FS_fseek_beyond_empty_file_seek_cur),
    Case("FS_fseek_beyond_non_empty_file_seek_cur", FS_fseek_beyond_non_empty_file_seek_cur),
    Case("FS_fseek_empty_file_seek_end", FS_fseek_empty_file_seek_end),
    Case("FS_fseek_non_empty_file_seek_end", FS_fseek_non_empty_file_seek_end),
    Case("FS_fseek_empty_file_seek_end", FS_fseek_beyond_empty_file_seek_end),
    Case("FS_fseek_beyond_non_empty_file_seek_end", FS_fseek_beyond_non_empty_file_seek_end),
    Case("FS_fseek_negative_non_empty_file_seek_end", FS_fseek_negative_non_empty_file_seek_end),

    Case("FS_fgetpos_rewrite_check_data", FS_fgetpos_rewrite_check_data),

    Case("FS_fscanf_valid_flow", FS_fscanf_valid_flow),
    Case("FS_fscanf_empty_file", FS_fscanf_empty_file),
    Case("FS_fscanf_more_fields_than_exist", FS_fscanf_more_fields_than_exist),

    Case("FS_fprintf_read_mode", FS_fprintf_read_mode),

    Case("FS_freopen_point_to_same_file", FS_freopen_point_to_same_file),
    Case("FS_freopen_valid_flow", FS_freopen_valid_flow),

    Case("FS_fopen_write_one_byte_file", FS_fopen_write_one_byte_file),
    Case("FS_fopen_write_two_byte_file", FS_fopen_write_two_byte_file),
    Case("FS_fopen_write_five_byte_file", FS_fopen_write_five_byte_file),
    Case("FS_fopen_write_fifteen_byte_file", FS_fopen_write_fifteen_byte_file),
    Case("FS_fopen_write_five_Kbyte_file", FS_fopen_write_five_Kbyte_file),

    Case("FS_fseek_rewrite_non_empty_file_begining", FS_fseek_rewrite_non_empty_file_begining),
    Case("FS_fseek_rewrite_non_empty_file_middle", FS_fseek_rewrite_non_empty_file_middle),
    Case("FS_fseek_rewrite_non_empty_file_end", FS_fseek_rewrite_non_empty_file_end),

    Case("FS_append_empty_file", FS_append_empty_file),
    Case("FS_append_non_empty_file", FS_append_non_empty_file),

    Case("FS_write_read_random_data", FS_write_read_random_data),
    Case("FS_fill_data_and_seek", FS_fill_data_and_seek),

    Case("bd_deinit_fs_unmount", bd_deinit_fs_unmount),
};


utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}

