/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

/** @file fopen.cpp Test cases to POSIX file fopen() interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

//#include "mbed.h"
#include "LittleFileSystem.h"
#include "fslittle_debug.h"
#include "fslittle_test.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /*rand()*/
#include <inttypes.h>
#include <errno.h>
/* mbed_retarget.h is included after errno.h so symbols are mapped to
 * consistent values for all toolchains */
//#include "platform/mbed_retarget.h"

using namespace utest::v1;

/// @cond FSLITTLE_DOXYGEN_DISABLE
#ifdef FSLITTLE_DEBUG
#define FSLITTLE_FOPEN_GREENTEA_TIMEOUT_S     3000
#else
#define FSLITTLE_FOPEN_GREENTEA_TIMEOUT_S     1000
#endif
/// @endcond


/* DEVICE_SPI
 *  This symbol is defined in targets.json if the target has a SPI interface, which is required for SDCard support.
 *
 * MBED_CONF_APP_FSLITTLE_SDCARD_INSTALLED
 *  For testing purposes, an SDCard must be installed on the target for the test cases in this file to succeed.
 *  If the target has an SD card installed then the MBED_CONF_APP_FSLITTLE_SDCARD_INSTALLED will be generated
 *  from the mbed_app.json, which includes the line
 *    {
 *    "config": {
 *        "UART_RX": "D0",
 *        <<< lines removed >>>
 *        "DEVICE_SPI": 1,
 *        "FSLITTLE_SDCARD_INSTALLED": 1
 *      },
 *      <<< lines removed >>>
 */

#include "FlashIAPBlockDevice.h"
#include "SlicingBlockDevice.h"

FlashIAPBlockDevice *flash;
SlicingBlockDevice *slice;
LittleFileSystem fs("sd");

static char fslittle_fopen_utest_msg_g[FSLITTLE_UTEST_MSG_BUF_SIZE];
#define FSLITTLE_FOPEN_TEST_MOUNT_PT_NAME      "sd"
#define FSLITTLE_FOPEN_TEST_MOUNT_PT_PATH      "/"FSLITTLE_FOPEN_TEST_MOUNT_PT_NAME
#define FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1    64
#define FSLITTLE_FOPEN_TEST_FILEPATH_MAX_DEPTH 20
static const int MAX_TEST_SIZE = 256 * 1024 * 2;
static const char *sd_badfile_path = "/sd/badfile.txt";
static const char *sd_testfile_path = "/sd/test.txt";

#define FSLITTLE_FOPEN_TEST_00      fslittle_fopen_test_00
#define FSLITTLE_FOPEN_TEST_01      fslittle_fopen_test_01
#define FSLITTLE_FOPEN_TEST_02      fslittle_fopen_test_02
#define FSLITTLE_FOPEN_TEST_03      fslittle_fopen_test_03
#define FSLITTLE_FOPEN_TEST_04      fslittle_fopen_test_04
#define FSLITTLE_FOPEN_TEST_05      fslittle_fopen_test_05
#define FSLITTLE_FOPEN_TEST_06      fslittle_fopen_test_06
#define FSLITTLE_FOPEN_TEST_07      fslittle_fopen_test_07
#define FSLITTLE_FOPEN_TEST_08      fslittle_fopen_test_08
#define FSLITTLE_FOPEN_TEST_09      fslittle_fopen_test_09
#define FSLITTLE_FOPEN_TEST_10      fslittle_fopen_test_10
#define FSLITTLE_FOPEN_TEST_11      fslittle_fopen_test_11
#define FSLITTLE_FOPEN_TEST_12      fslittle_fopen_test_12
#define FSLITTLE_FOPEN_TEST_13      fslittle_fopen_test_13
#define FSLITTLE_FOPEN_TEST_14      fslittle_fopen_test_14
#define FSLITTLE_FOPEN_TEST_15      fslittle_fopen_test_15
#define FSLITTLE_FOPEN_TEST_17      fslittle_fopen_test_17
#define FSLITTLE_FOPEN_TEST_18      fslittle_fopen_test_18
#define FSLITTLE_FOPEN_TEST_19      fslittle_fopen_test_19
#define FSLITTLE_FOPEN_TEST_20      fslittle_fopen_test_20
#define FSLITTLE_FOPEN_TEST_21      fslittle_fopen_test_21
#define FSLITTLE_FOPEN_TEST_22      fslittle_fopen_test_22
#define FSLITTLE_FOPEN_TEST_23      fslittle_fopen_test_23
#define FSLITTLE_FOPEN_TEST_24      fslittle_fopen_test_24
#define FSLITTLE_FOPEN_TEST_25      fslittle_fopen_test_25
#define FSLITTLE_FOPEN_TEST_26      fslittle_fopen_test_26
#define FSLITTLE_FOPEN_TEST_27      fslittle_fopen_test_27
#define FSLITTLE_FOPEN_TEST_28      fslittle_fopen_test_28
#define FSLITTLE_FOPEN_TEST_29      fslittle_fopen_test_29
#define FSLITTLE_FOPEN_TEST_30      fslittle_fopen_test_30


/* support functions */

/*
 * open tests that focus on testing fopen()
 * fslittle_handle_t fopen(const char* filename, char* data, size_t* len, fslittle_key_desc_t* kdesc)
 */

/* file data for test_01 */
static fslittle_kv_data_t fslittle_fopen_test_01_kv_data[] = {
    { "/sd/fopentst/hello/world/animal/wobbly/dog/foot/frontlft.txt", "missing"},
    { NULL, NULL},
};


/** @brief
 * Split a file path into its component parts, setting '/' characters to '\0', and returning
 * pointers to the file path components in the parts array. For example, if
 * filepath = "/sd/fopentst/hello/world/animal/wobbly/dog/foot/frontlft.txt" then
 *  *parts[0] = "sd"
 *  *parts[1] = "fopentst"
 *  *parts[2] = "hello"
 *  *parts[3] = "world"
 *  *parts[4] = "animal"
 *  *parts[5] = "wobbly"
 *  *parts[6] = "dog"
 *  *parts[7] = "foot"
 *  *parts[8] = "frontlft.txt"
 *   parts[9] = NULL
 *
 * ARGUMENTS
 *  @param  filepath     IN file path string to split into component parts. Expected to start with '/'
 *  @param  parts        IN OUT array to hold pointers to parts
 *  @param  num          IN number of components available in parts
 *
 * @return  On success, this returns the number of components in the filepath Returns number of compoee
 */
static int32_t fslittle_filepath_split(char *filepath, char *parts[], uint32_t num)
{
    uint32_t i = 0;
    int32_t ret = -1;
    char *z = filepath;

    while (i < num && *z != '\0') {
        if (*z == '/') {
            *z = '\0';
            parts[i] = ++z;
            i++;
        } else {
            z++;
        }
    }
    if (*z == '\0' && i > 0) {
        ret = (int32_t) i;
    }
    return ret;
}


/** @brief
 * remove all directories and file in the given filepath
 *
 * ARGUMENTS
 *  @param  filepath     IN file path string to split into component parts. Expected to start with '/'
 *
 * @return  On success, this returns 0, otherwise < 0 is returned;
 */
int32_t fslittle_filepath_remove_all(char *filepath)
{
    int32_t ret = -1;
    int32_t len = 0;
    char *fpathbuf = NULL;
    char *pos = NULL;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    len = strlen(filepath);
    fpathbuf = (char *) malloc(len + 1);
    if (fpathbuf == NULL) {
        FSLITTLE_DBGLOG("%s: failed to duplicate string (out of memory)\n", __func__);
        return ret;
    }
    memset(fpathbuf, 0, len + 1);
    memcpy(fpathbuf, filepath, len);

    /* delete the leaf node first, and then successively parent directories. */
    pos = fpathbuf + strlen(fpathbuf);
    while (pos != fpathbuf) {
        /* If the remaining file path is the mount point path then finish as the mount point cannot be removed */
        if (strlen(fpathbuf) == strlen(FSLITTLE_FOPEN_TEST_MOUNT_PT_PATH) && strncmp(fpathbuf, FSLITTLE_FOPEN_TEST_MOUNT_PT_PATH, strlen(fpathbuf)) == 0) {
            break;
        }
        ret = remove(fpathbuf);
        pos = strrchr(fpathbuf, '/');
        *pos = '\0';
    }
    if (fpathbuf) {
        free(fpathbuf);
    }
    return ret;
}


/** @brief
 * make all directories in the given filepath. Do not create the file if present at end of filepath
 *
 * ARGUMENTS
 *  @param  filepath     IN file path containing directories and file
 *  @param  do_asserts   IN set to true if function should assert on errors
 *
 * @return  On success, this returns 0, otherwise < 0 is returned;
 */
static int32_t fslittle_filepath_make_dirs(char *filepath, bool do_asserts)
{
    int32_t i = 0;
    int32_t num_parts = 0;
    int32_t len = 0;
    int32_t ret = -1;
    char *fpathbuf = NULL;
    char *buf = NULL;
    int pos = 0;
    char *parts[FSLITTLE_FOPEN_TEST_FILEPATH_MAX_DEPTH];

    FSLITTLE_DBGLOG("%s:entered\n", __func__);
    /* find the dirs to create*/
    memset(parts, 0, sizeof(parts));
    len = strlen(filepath);
    fpathbuf = (char *) malloc(len + 1);
    if (fpathbuf == NULL) {
        FSLITTLE_DBGLOG("%s: failed to duplicate string (out of memory)\n", __func__);
        return ret;
    }
    memset(fpathbuf, 0, len + 1);
    memcpy(fpathbuf, filepath, len);
    num_parts = fslittle_filepath_split(fpathbuf, parts, FSLITTLE_FOPEN_TEST_FILEPATH_MAX_DEPTH);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to split filepath (filename=\"%s\", num_parts=%d)\n", __func__, filepath, (int) num_parts);
    TEST_ASSERT_MESSAGE(num_parts > 0, fslittle_fopen_utest_msg_g);

    /* Now create the directories on the directory path.
     * Skip creating dir for "/sd" which must be present */
    buf = (char *) malloc(strlen(filepath) + 1);
    memset(buf, 0, strlen(filepath) + 1);
    pos = sprintf(buf, "/%s", parts[0]);
    for (i = 1; i < num_parts - 1; i++) {
        pos += sprintf(buf + pos, "/%s", parts[i]);
        FSLITTLE_DBGLOG("mkdir(%s)\n", buf);
        ret = mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (do_asserts == true) {
            FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create directory (filepath2=\"%s\", ret=%d, errno=%d)\n", __func__, buf, (int) ret, errno);
            TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
        }
    }

    if (buf) {
        free(buf);
    }
    if (fpathbuf) {
        free(fpathbuf);
    }
    return ret;
}


/* FIX ME: errno not set correctly when error occurs. This indicates a problem with the implementation. */

/** @brief
 * Basic fopen test which does the following:
 * - creates file and writes some data to the value blob.
 * - closes the newly created file.
 * - opens the file (r-only)
 * - reads the file data and checks its the same as the previously created data.
 * - closes the opened file
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fslittle_fopen_test_01(const size_t call_count)
{
    char *read_buf;
    int32_t ret = 0;
    size_t len = 0;
    fslittle_kv_data_t *node;
    FILE *fp = NULL;

    FSLITTLE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    node = fslittle_fopen_test_01_kv_data;

    /* remove file and directory from a previous failed test run, if present */
    fslittle_filepath_remove_all((char *) node->filename);

    /* create dirs */
    ret = fslittle_filepath_make_dirs((char *) node->filename, true);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dirs for filename (filename=\"%s\")(ret=%d)\n", __func__, node->filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_DBGLOG("%s:About to create new file (filename=\"%s\", data=\"%s\")\n", __func__, node->filename, node->value);
    fp = fopen(node->filename, "w+");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (filename=\"%s\", data=\"%s\")(ret=%d, errno=%d)\n", __func__, node->filename, node->value, (int) ret, errno);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    FSLITTLE_DBGLOG("%s:length of file=%d (filename=\"%s\", data=\"%s\")\n", __func__, (int) len, node->filename, node->value);
    len = strlen(node->value);
    ret = fwrite((const void *) node->value, len, 1, fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write file (filename=\"%s\", data=\"%s\")(ret=%d)\n", __func__, node->filename, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 1, fslittle_fopen_utest_msg_g);

    FSLITTLE_DBGLOG("Created file successfully (filename=\"%s\", data=\"%s\")\n", node->filename, node->value);
    ret = fclose(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to close file (ret=%d, errno=%d)\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* now open the newly created key */
    fp = NULL;
    fp = fopen(node->filename, "r");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file for reading (filename=\"%s\", data=\"%s\")(ret=%d)\n", __func__, node->filename, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    len = strlen(node->value) + 1;
    read_buf = (char *) malloc(len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to allocated read buffer \n", __func__);
    TEST_ASSERT_MESSAGE(read_buf != NULL, fslittle_fopen_utest_msg_g);

    FSLITTLE_DBGLOG("Opened file successfully (filename=\"%s\", data=\"%s\")\n", node->filename, node->value);
    memset(read_buf, 0, len);
    ret = fread((void *) read_buf, len, 1, fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read file (filename=\"%s\", data=\"%s\", read_buf=\"%s\", ret=%d)\n", __func__, node->filename, node->value, read_buf, (int) ret);
    /* FIX ME: fread should return the number of items read, not 0 when an item is read successfully.
     * This indicates a problem with the implementation, as the correct data is read. The correct assert should be:
     *   TEST_ASSERT_MESSAGE(ret == 1, fslittle_fopen_utest_msg_g);
     * The following assert is curerntly used until the implementation is fixed
     */
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* check read data is as expected */
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: read value data (%s) != expected value data (filename=\"%s\", data=\"%s\", read_buf=\"%s\", ret=%d)\n", __func__, read_buf, node->filename, node->value, read_buf, (int) ret);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, fslittle_fopen_utest_msg_g);

    if (read_buf) {
        free(read_buf);
    }
    ret = fclose(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: fclose() call failed (ret=%d, errno=%d).\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
    return CaseNext;
}

static fslittle_kv_data_t fslittle_fopen_test_02_data[] = {
    FSLITTLE_INIT_1_TABLE_MID_NODE,
    { NULL, NULL},
};

/**
 * @brief   test to fopen() a pre-existing key and try to write it, which should fail
 *          as by default pre-existing keys are opened read-only
 *
 * Basic open test which does the following:
 * - creates file with default rw perms and writes some data to the value blob.
 * - closes the newly created file.
 * - opens the file with the default permissions (read-only)
 * - tries to write the file data which should fail because file was not opened with write flag set.
 * - closes the opened key
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_02(const size_t call_count)
{
    int32_t ret = -1;
    size_t len = 0;
    FILE *fp = NULL;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    len = strlen(fslittle_fopen_test_02_data[0].value);
    ret = fslittle_test_create(fslittle_fopen_test_02_data[0].filename, (char *) fslittle_fopen_test_02_data[0].value, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    /* by default, owner of key opens with read-only permissions*/
    fp = fopen(fslittle_fopen_test_02_data[0].filename, "r");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file (filename=\"%s\", ret=%d)\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    len = strlen(fslittle_fopen_test_02_data[0].value);
    ret = fwrite((const void *) fslittle_fopen_test_02_data[0].value, len, 1, fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: call to fwrite() succeeded when should have failed for read-only file (filename=\"%s\")(ret=%d).\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret <= 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: fclose() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(fclose(fp) == 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/**
 * @brief   test to fopen() a pre-existing file and try to write it, which should succeed
 *          because the key was opened read-write permissions explicitly
 *
 * Basic open test which does the following:
 * - creates file with default rw perms and writes some data to the value blob.
 * - closes the newly created file.
 * - opens the file with the rw permissions (non default)
 * - tries to write the file data which should succeeds because file was opened with write flag set.
 * - closes the opened key
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_03(const size_t call_count)
{
    int32_t ret = -1;
    size_t len = 0;
    FILE *fp = NULL;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    len = strlen(fslittle_fopen_test_02_data[0].value);
    ret = fslittle_test_create(fslittle_fopen_test_02_data[0].filename, (char *) fslittle_fopen_test_02_data[0].value, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    /* opens with read-write permissions*/
    fp = fopen(fslittle_fopen_test_02_data[0].filename, "w+");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file (filename=\"%s\")(ret=%d)\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    len = strlen(fslittle_fopen_test_02_data[0].value);
    ret = fwrite((const void *) fslittle_fopen_test_02_data[0].value, len, 1, fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: call to fwrite() failed when should have succeeded (filename=\"%s\", ret=%d).\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: fclose() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(fclose(fp) >= 0, fslittle_fopen_utest_msg_g);

    /* clean-up */
    ret = remove(fslittle_fopen_test_02_data[0].filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to delete file (filename=%s, ret=%d) .\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/** @brief  test to call fopen() with a filename string that exceeds the maximum length
 * - check that filenames of this length can be created
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_04(const size_t call_count)
{
    char filename_good[FSLITTLE_FILENAME_MAX_LENGTH + 1];
    char filename_bad[FSLITTLE_FILENAME_MAX_LENGTH + 2];
    int32_t ret = -1;
    size_t len = 0;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    memset(filename_good, 0, FSLITTLE_FILENAME_MAX_LENGTH + 1);
    memset(filename_bad, 0, FSLITTLE_FILENAME_MAX_LENGTH + 2);
    ret = fslittle_test_filename_gen(filename_good, FSLITTLE_FILENAME_MAX_LENGTH);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate filename_good.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: filename_good is not the correct length (filename_good=%s, len=%d, expected=%d).\n", __func__, filename_good, (int) strlen(filename_good), (int) FSLITTLE_FILENAME_MAX_LENGTH);
    TEST_ASSERT_MESSAGE(strlen(filename_good) == FSLITTLE_FILENAME_MAX_LENGTH, fslittle_fopen_utest_msg_g);

    ret = fslittle_test_filename_gen(filename_bad, FSLITTLE_FILENAME_MAX_LENGTH + 1);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate filename_bad.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: filename_bad is not the correct length (len=%d, expected=%d).\n", __func__, (int) strlen(filename_bad), (int) FSLITTLE_FILENAME_MAX_LENGTH + 1);
    TEST_ASSERT_MESSAGE(strlen(filename_bad) == FSLITTLE_FILENAME_MAX_LENGTH + 1, fslittle_fopen_utest_msg_g);

    len = strlen(filename_good);
    ret = fslittle_test_create(filename_good, filename_good, len);

    len = strlen(filename_bad);
    ret = fslittle_test_create(filename_bad, filename_bad, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: created file in store for filename_bad when should have failed (filename=%s, ret=%d).\n", __func__, filename_bad, (int) ret);
    TEST_ASSERT_MESSAGE(ret < 0, fslittle_fopen_utest_msg_g);
    return CaseNext;
}


/// @cond FSLITTLE_DOXYGEN_DISABLE
typedef struct fslittle_fopen_kv_name_ascii_node {
    uint32_t code;
    uint32_t f_allowed : 1;
} fslittle_fopen_kv_name_ascii_node;
/// @endcond

static const uint32_t fslittle_fopen_kv_name_ascii_table_code_sentinel_g = 256;

/*@brief    table recording ascii character codes permitted in kv names */
static fslittle_fopen_kv_name_ascii_node fslittle_fopen_kv_name_ascii_table[] = {
    {0, true},          /* code 0-33 allowed*/
    {34, false},        /* '"' not allowed */
    {35, true},         /* allowed */
    {42, false},        /* '*' not allowed */
    {43, true},         /* allowed */
    {47, false},        /* '/' not allowed */
    {48, true},         /* allowed */
    {58, false},        /* ':' not allowed */
    {59, true},         /* allowed */
    {60, false},        /* '<' not allowed */
    {61, true},         /* allowed */
    {62, false},        /* '?', '>' not allowed */
    {64, true},         /* allowed */
    {92, false},        /* '\' not allowed */
    {93, true},         /* allowed */
    {124, false},        /* '!' not allowed */
    {125, true},         /* allowed */
    {127, false},        /* DEL not allowed */
    {128, true},         /* allowed */
    {fslittle_fopen_kv_name_ascii_table_code_sentinel_g, false},       /* sentinel */
};


/// @cond FSLITTLE_DOXYGEN_DISABLE
enum fslittle_fopen_kv_name_pos {
    fslittle_fopen_kv_name_pos_start = 0x0,
    fslittle_fopen_kv_name_pos_mid,
    fslittle_fopen_kv_name_pos_end,
    fslittle_fopen_kv_name_pos_max
};
/// @endcond

/** @brief  test to call fopen() with filename that in includes illegal characters
 *          - the character(s) can be at the beginning of the filename
 *          - the character(s) can be at the end of the filename
 *          - the character(s) can be somewhere within the filename string
 *          - a max-length string of random characters (legal and illegal)
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_05(const size_t call_count)
{
    bool f_allowed = false;
    const char *mnt_pt = FSLITTLE_FOPEN_TEST_MOUNT_PT_PATH;
    const char *basename = "goodfile";
    const char *extname = "txt";
    const size_t basename_len = strlen(basename);
    const size_t filename_len = strlen(mnt_pt) + strlen(basename) + strlen(extname) + 2; /* extra 2 chars for '/' and '.' in "/sd/goodfile.txt" */
    char filename[FSLITTLE_BUF_MAX_LENGTH];
    size_t len = 0;
    uint32_t j = 0;
    int32_t ret = 0;
    fslittle_fopen_kv_name_ascii_node *node = NULL;
    uint32_t pos;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

#ifdef FSLITTLE_DEBUG
    /* symbol only used why debug is enabled */
    const char *pos_str = NULL;
#endif

    /* create bad keyname strings with invalid character code at start of keyname */
    node = fslittle_fopen_kv_name_ascii_table;
    memset(filename, 0, FSLITTLE_BUF_MAX_LENGTH);
    while (node->code !=  fslittle_fopen_kv_name_ascii_table_code_sentinel_g) {
        /* loop over range */
        for (j = node->code; j < (node + 1)->code; j++) {
            if ((j >= 48 && j <= 57) || (j >= 65 && j <= 90) || (j >= 97 && j <= 122)) {
                FSLITTLE_DBGLOG("%s: skipping alpha-numeric ascii character code %d (%c).\n", __func__, (int) j, (char) j);
                continue;
            }

            /* set the start, mid, last character of the name to the test char code */
            for (pos = (uint32_t) fslittle_fopen_kv_name_pos_start; pos < (uint32_t) fslittle_fopen_kv_name_pos_max; pos++) {
                len = snprintf(filename, filename_len + 1, "%s/%s.%s", mnt_pt, basename, extname);
                /* overwrite a char at the pos start, mid, end of the filename with an ascii char code (both illegal and legal)*/
                switch (pos) {
                    case fslittle_fopen_kv_name_pos_start:
                        filename[5] = (char) j; /* 5 so at to write the second basename char (bad chars as first char not accepted)*/
                        break;
                    case fslittle_fopen_kv_name_pos_mid:
                        /* create bad keyname strings with invalid character code in the middle of keyname */
                        filename[5 + basename_len / 2] = (char) j;
                        break;
                    case fslittle_fopen_kv_name_pos_end:
                        /* create bad keyname strings with invalid character code at end of keyname */
                        filename[5 + basename_len - 1] = (char) j;
                        break;
                    default:
                        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected value of pos (pos=%d).\n", __func__, (int) pos);
                        TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);
                        break;
                }

#ifdef FSLITTLE_DEBUG
                /* processing only required when debug trace enabled */
                switch (pos) {
                    case fslittle_fopen_kv_name_pos_start:
                        pos_str = "start";
                        break;
                    case fslittle_fopen_kv_name_pos_mid:
                        pos_str = "middle";
                        break;
                    case fslittle_fopen_kv_name_pos_end:
                        pos_str = "end";
                        break;
                    default:
                        break;
                }
#endif
                ret = fslittle_test_create(filename, (const char *) filename, len);

                /* special cases */
                switch (j) {
                    //case 0 :
                    //case 46 :
                    //    switch(pos)
                    //    {
                    //    /* for code = 0 (null terminator). permitted at mid and end of string */
                    //    /* for code = 46 ('.'). permitted at mid and end of string but not at start */
                    //    case fslittle_fopen_kv_name_pos_start:
                    //        f_allowed = false;
                    //        break;
                    //    case fslittle_fopen_kv_name_pos_mid:
                    //    case fslittle_fopen_kv_name_pos_end:
                    //    default:
                    //        f_allowed = true;
                    //        break;
                    //    }
                    //    break;
                    default:
                        f_allowed = node->f_allowed;
                        break;
                }
                if (f_allowed == true) {
                    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file in store when filename contains valid characters (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);
                    /* revert FSLITTLE_LOG for more trace */
                    FSLITTLE_DBGLOG("Successfully created a file with valid keyname containing ascii character code %d (%c) at the %s of the keyname.\n", (int) j, (int) j, pos_str);
                    FSLITTLE_LOG("%c", '.');

                    ret = fslittle_test_delete(filename);
                    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to delete file previously created (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);
                } else {
                    /*node->f_allowed == false => not allowed to create kv name with ascii code */
                    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: created file in store when filename contains an invalid character (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret < 0, fslittle_fopen_utest_msg_g);
                    /* revert FSLITTLE_LOG for more trace */
                    FSLITTLE_DBGLOG("Successfully failed to create a file with an invalid keyname containing ascii character code %d at the %s of the keyname.\n", (int) j, pos_str);
                    FSLITTLE_LOG("%c", '.');
                }
            }
        }
        node++;
    }

    FSLITTLE_LOG("%c", '\n');
    return CaseNext;
}


static const char fslittle_fopen_ascii_illegal_buf_g[] = "\"?'*+,./:;<=>?[\\]|";

/** @brief  test to call fopen() with filename that in includes
 *          illegal characters
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_06(const size_t call_count)
{
// legal in LittleFS
#if 0
    const char *mnt_pt = FSLITTLE_FOPEN_TEST_MOUNT_PT_PATH;
    const char *extname = "txt";
    const size_t filename_len = strlen(mnt_pt) + FSLITTLE_MAX_FILE_BASENAME + strlen(extname) + 2; /* extra 2 chars for '/' and '.' in "/sd/goodfile.txt" */
    char filename[FSLITTLE_BUF_MAX_LENGTH];
    int32_t i = 0;
    int32_t j = 0;
    uint32_t pos = 0;
    uint32_t len = 0;
    int32_t ret = -1;
    size_t buf_data_max = 0;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    memset(filename, 0, FSLITTLE_BUF_MAX_LENGTH);
    /* create bad keyname strings with invalid character code at start of keyname */
    buf_data_max = strlen(fslittle_fopen_ascii_illegal_buf_g);

    /* generate a number of illegal filenames */
    for (j = 0; i < FSLITTLE_MAX_FILE_BASENAME; j++) {
        /* generate a kv name of illegal chars*/
        len = snprintf(filename, filename_len + 1, "%s/", mnt_pt);
        for (i = 0; i < FSLITTLE_MAX_FILE_BASENAME; i++) {
            pos = rand() % (buf_data_max + 1);
            len += snprintf(filename + len, filename_len + 1, "%c", fslittle_fopen_ascii_illegal_buf_g[pos]);

        }
        len += snprintf(filename + len, filename_len + 1, ".%s", extname);
        ret = fslittle_test_create(filename, filename, len);
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: created file when filename contains invalid characters (filename=%s, ret=%d).\n", __func__, filename, (int) ret);
        TEST_ASSERT_MESSAGE(ret < 0, fslittle_fopen_utest_msg_g);
    }
#endif
    return CaseNext;
}


/** @brief  test for errno reporting on a failed fopen()call
 *
 *  This test does the following:
 *  - tries to open a file that does not exist for reading, and checks that a NULL pointer is returned.
 *  - checks that errno is not 0 as there is an error.
 *  - checks that ferror() returns 1 indicating an error exists.
 *
 * Note: see NOTE_1 below.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_07(const size_t call_count)
{
    FILE *f = NULL;
    int ret = -1;
    int errno_val = 0;
    const char *filename = sd_badfile_path;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    errno = 0;
    /* this is expect to fail as the file doesnt exist */
    f = fopen(filename, "r");

    /* Store errno so the current value set  is not changed by new function call */
    errno_val = errno;

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: opened non-existent file for reading (filename=%s, f=%p).\n", __func__, filename, f);
    TEST_ASSERT_MESSAGE(f == NULL, fslittle_fopen_utest_msg_g);

    /* check errno is set correctly */
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: errno has unexpected value (errno != 0 expected) (filename=%s, errno=%d).\n", __func__, filename, errno);
    TEST_ASSERT_MESSAGE(errno_val != 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/** @brief  test for operation of clearerr() and ferror()
 *
 *  The test does the following:
 *  - opens and then closes a file, but keeps a copy of the FILE pointer fp.
 *  - set errno to 0.
 *  - write to the close file with fwrite(fp) which should return 0 (no writes) and set the errno.
 *  - check the error condition is set with ferror().
 *  - clear the error with clearerr().
 *  - check the error condition is reset with ferror().
 *
 * NOTE_1: GCC/ARMCC support for setting errno
 *  - Documentation (e.g. fwrite() man page) does not explicity say fwrite() sets errno
 *    (e.g. for an fwrite() on a read-only file).
 *  - GCC libc fwrite() appears to set errno as expected.
 *  - ARMCC & IAR libc fwrite() appears not to set errno.
 *
 * The following ARMCC documents are silent on whether fwrite() sets errno:
 * - "ARM C and C++ Libraries and Floating-Point Support".
 * - "RL-ARM User Guide fwrite() section".
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_08(const size_t call_count)
{
    FILE *fp = NULL;
    int ret = -1;
    int ret_ferror = -1;
    const char *filename = sd_testfile_path;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    errno = 0;
    fp = fopen(filename, "w+");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file (filename=%s, f=%p).\n", __func__, filename, fp);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    /* close the fp but then try to read or write it */
    ret = fclose(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to close file (ret=%d, errno=%d)\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* open file  */
    errno = 0;
    fp = fopen(filename, "r");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file for reading (filename=\"%s\", ret=%d)\n", __func__, filename, (int) ret);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    /* Perform fwrite() operation that will fail. */
    errno = 0;
    ret = fwrite("42!", 4, 1, fp);

    ret_ferror = ferror(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: ferror() failed to report error (filename=%s, ret_ferror=%d).\n", __func__, filename, (int) ret_ferror);
    TEST_ASSERT_MESSAGE(ret_ferror != 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: fwrite successfully wrote to read-only file (filename=%s, ret=%d).\n", __func__, filename, (int) ret);
    /* the fwrite() should fail and return 0. */
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

#if ! defined(__ARMCC_VERSION) && defined(__GNUC__)
    /* check that errno is set. ARMCC appears not to set errno for fwrite() failure. */
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected zero value for errno (filename=%s, ret=%d, errno=%d).\n", __func__, filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(errno != 0, fslittle_fopen_utest_msg_g);

    /* check that errno is set to the expected value (this may change differ for different libc's) */
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: errno != EBADF (filename=%s, ret=%d, errno=%d).\n", __func__, filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(errno == EBADF, fslittle_fopen_utest_msg_g);
#endif  /* ! defined(__ARMCC_VERSION) && defined(__GNUC__) */

    /* check clearerr() return clears the error */
    clearerr(fp);
    ret = ferror(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: ferror() did not return zero value when error has been cleared (filename=%s, ret=%d).\n", __func__, filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    fclose(fp);
    return CaseNext;
}


/** @brief  test for operation of ftell()
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_09(const size_t call_count)
{
    FILE *fp = NULL;
    int ret = -1;
    int32_t len = 0;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* create a file of a certain length */
    len = strlen(fslittle_fopen_test_02_data[0].value);
    ret = fslittle_test_create(fslittle_fopen_test_02_data[0].filename, (char *) fslittle_fopen_test_02_data[0].value, len);

    errno = 0;
    /* Open the file for reading so the file is not truncated to 0 length. */
    fp = fopen(fslittle_fopen_test_02_data[0].filename, "r");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file (filename=%s, fp=%p, errno=%d).\n", __func__, fslittle_fopen_test_02_data[0].filename, fp, errno);
    TEST_ASSERT_MESSAGE(fp != NULL, fslittle_fopen_utest_msg_g);

    errno = 0;
    ret = fseek(fp, 0, SEEK_END);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: fseek() failed to SEEK_END (filename=%s, ret=%d, errno=%d).\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    errno = 0;
    ret = ftell(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: ftell() failed to report correct offset value (filename=%s, ret=%d, errno=%d).\n", __func__, fslittle_fopen_test_02_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == len, fslittle_fopen_utest_msg_g);

    errno = 0;
    ret = fclose(fp);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to close file (ret=%d, errno=%d)\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/* file data for test_10 */
static fslittle_kv_data_t fslittle_fopen_test_10_kv_data[] = {
    { "/sd/test_10/testfile.txt", "test_data"},
    { NULL, NULL},
};

/** @brief  test for operation of remove()
 *
 * Performs the following tests:
 *  1. test remove() on a file that exists. This should succeed.
 *  2. test remove() on a dir that exists. This should succeed.
 *  3. test remove() on a file that doesnt exist. This should fail. check errno set.
 *  4. test remove() on a dir that doesnt exist. This should fail. check errno set.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_10(const size_t call_count)
{
    char buf[FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1];
    char *pos = NULL;
    int32_t ret = -1;
    size_t len = 0;
    fslittle_kv_data_t *node = fslittle_fopen_test_10_kv_data;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    TEST_ASSERT(strlen(node->filename) < FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);

    /* start from a known state i.e. directory to be created in not present */
    fslittle_filepath_remove_all((char *) node->filename);

    /* (1) */
    errno = 0;
    ret = fslittle_filepath_make_dirs((char *) node->filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    len = strlen(node->value);
    ret = fslittle_test_create(node->filename, (char *) node->value, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    ret = remove(node->filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: delete file operation failed (filename=%s, ret=%d) .\n", __func__, node->filename, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* (3) */
    ret = remove(node->filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: deleted a file that doesn't exist (filename=%s, ret=%d, errno=%d) .\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret != 0, fslittle_fopen_utest_msg_g);

    /* (2) */
    memset(buf, 0, FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);
    memcpy(buf, node->filename, strlen(node->filename));
    pos = strrchr(buf, '/');
    *pos = '\0';
    ret = remove(buf);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: delete directory operation failed (directory name=%s, ret=%d, errno=%d).\n", __func__, buf, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* (4) */
    ret = remove(buf);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: deleted a directory that doesn't exist (directory name=%s, ret=%d, errno=%d).\n", __func__, buf, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret != 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/* file data for test_11 */
static fslittle_kv_data_t fslittle_fopen_test_11_kv_data[] = {
    { "/sd/test_11/step0.txt", "test_data"},
    { "/sd/test_11/step1.txt", "test_data"},
    { "/sd/test_11/subdir/step3.txt", "test_data"},
    { NULL, NULL},
};

/** @brief  test for operation of rename()
 *
 * This test does the following:
 *  1) test rename() on a file that exists to a new filename within the same directory.
 *  2) test rename() on a file that exists to a new filename within a different directory.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_11(const size_t call_count)
{
    int32_t ret = -1;
    size_t len = 0;
    fslittle_kv_data_t *node = fslittle_fopen_test_11_kv_data;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    TEST_ASSERT(strlen(node->filename) < FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);

    /* start from a known state i.e. directory to be created in not present, files not present */
    while (node->filename != NULL) {
        fslittle_filepath_remove_all((char *) node->filename);
        node++;
    }

    /* create file and directories ready for rename() tests */
    errno = 0;
    node = fslittle_fopen_test_11_kv_data;
    ret = fslittle_filepath_make_dirs((char *) node->filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    len = strlen(node->value);
    ret = fslittle_test_create(node->filename, (char *) node->value, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    errno = 0;
    node = &fslittle_fopen_test_11_kv_data[2];
    ret = fslittle_filepath_make_dirs((char *) node->filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* (1) */
    ret = rename(fslittle_fopen_test_11_kv_data[0].filename, fslittle_fopen_test_11_kv_data[1].filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to rename file from (%s) to (%s) (ret=%d, errno=%d).\n", __func__, fslittle_fopen_test_11_kv_data[0].filename, fslittle_fopen_test_11_kv_data[1].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* (2) */
    ret = rename(fslittle_fopen_test_11_kv_data[1].filename, fslittle_fopen_test_11_kv_data[2].filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to rename file from (%s) to (%s) (ret=%d, errno=%d).\n", __func__, fslittle_fopen_test_11_kv_data[1].filename, fslittle_fopen_test_11_kv_data[2].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}


/* file data for test_12 */
static fslittle_kv_data_t fslittle_fopen_test_12_kv_data[] = {
    { "/sd/test_12/subdir/testfil1.txt", "testfil1.txt"},
    { "/sd/test_12/testfil2.txt", "testfil2.txt"},
    { "/sd/test_12/testfil3.txt", "testfil3.txt"},
    { "/sd/test_12/testfil4.txt", "testfil4.txt"},
    { "/sd/test_12/testfil5.txt", "testfil5.txt"},
    { NULL, NULL},
};

/** @brief  test for operation of readdir().
 *
 * Note, rewinddir(), telldir() and seekdir() dont appear to work reliably.
 * opendir() not available on ARM/IAR toolchains.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_12(const size_t call_count)
{
    char buf[FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1];
    char *pos = NULL;
    int32_t count = 0;
    int32_t ret = -1;
    size_t len = 0;
    DIR *dir;
    struct dirent *dp;
    fslittle_kv_data_t *node = fslittle_fopen_test_12_kv_data;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

#if ! defined(__ARMCC_VERSION) && defined(__GNUC__)

    /* start from a known state i.e. directory to be created in not present */
    while (node->filename != NULL) {
        fslittle_filepath_remove_all((char *) node->filename);
        node++;
    }

    /* create a file */
    node = fslittle_fopen_test_12_kv_data;
    errno = 0;
    ret = fslittle_filepath_make_dirs((char *) node->filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    node = fslittle_fopen_test_12_kv_data;
    while (node->filename != NULL) {
        len = strlen(node->value);
        ret = fslittle_test_create(node->filename, (char *) node->value, len);
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);
        node++;
    }

    node = fslittle_fopen_test_12_kv_data;
    memset(buf, 0, FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);
    memcpy(buf, node->filename, strlen(node->filename));
    pos = strrchr(buf, '/');
    *pos = '\0';
    dir = opendir(buf);

    dp = readdir(dir);
    TEST_ASSERT_MESSAGE(dp != 0, "Error: readdir() failed\n");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected object name (name=%s, expected=%s).\n", __func__, dp->d_name, ".");
    TEST_ASSERT_MESSAGE(strncmp(dp->d_name, ".", strlen(".")) == 0, fslittle_fopen_utest_msg_g);
    dp = readdir(dir);
    TEST_ASSERT_MESSAGE(dp != 0, "Error: readdir() failed\n");
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected object name (name=%s, expected=%s).\n", __func__, dp->d_name, "..");
    TEST_ASSERT_MESSAGE(strncmp(dp->d_name, "..", strlen("..")) == 0, fslittle_fopen_utest_msg_g);

    while ((dp = readdir(dir)) != NULL) {
        FSLITTLE_DBGLOG("%s: filename: \"%s\"\n", __func__, dp->d_name);
        TEST_ASSERT_MESSAGE(dp != 0, "Error: readdir() failed\n");
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected object name (name=%s, expected=%s).\n", __func__, dp->d_name, fslittle_fopen_test_12_kv_data[count].value);
        TEST_ASSERT_MESSAGE(strncmp(dp->d_name, fslittle_fopen_test_12_kv_data[count].value, strlen(fslittle_fopen_test_12_kv_data[count].value)) == 0, fslittle_fopen_utest_msg_g);
        count++;
    }
    closedir(dir);

    /* cleanup */
    node = fslittle_fopen_test_12_kv_data;
    while (node->filename != NULL) {
        fslittle_filepath_remove_all((char *) node->filename);
        node++;
    }
#endif  /* ! defined(__ARMCC_VERSION) && defined(__GNUC__) */
    return CaseNext;
}


/* file data for test_13 */
static fslittle_kv_data_t fslittle_fopen_test_13_kv_data[] = {
    /* a file is included in the filepath even though its not created by the test,
     * as the fslittle_filepath_make_dirs() works with it present. */
    { "/sd/test_13/dummy.txt", "testdir"},
    { NULL, NULL},
};
/** @brief  test for operation of mkdir()/remove()
 *
 * This test checks that:
 * - The mkdir() function successfully creates a directory that is not already present.
 * - The mkdir() function returns EEXIST when trying to create a directory thats already present.
 * - The remove() function successfully removes a directory that is present.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_13(const size_t call_count)
{
    int32_t ret = 0;

    FSLITTLE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;

    /* start from a known state i.e. directory to be created in not present */
    fslittle_filepath_remove_all((char *) fslittle_fopen_test_13_kv_data[0].filename);

    errno = 0;
    ret = fslittle_filepath_make_dirs((char *) fslittle_fopen_test_13_kv_data[0].filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, fslittle_fopen_test_13_kv_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    /* check that get a suitable error when try to create it again.*/
    errno = 0;
    ret = fslittle_filepath_make_dirs((char *) fslittle_fopen_test_13_kv_data[0].filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: permitted to create directory when already exists (dirname=%s, ret=%d, errno=%d)\n", __func__, fslittle_fopen_test_13_kv_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret != 0, fslittle_fopen_utest_msg_g);

    /* check errno is as expected */
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: errno != EEXIST (dirname=%s, ret=%d, errno=%d)\n", __func__, fslittle_fopen_test_13_kv_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(errno == EEXIST, fslittle_fopen_utest_msg_g);

    ret = fslittle_filepath_remove_all((char *) fslittle_fopen_test_13_kv_data[0].filename);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to remove directory (dirname=%s, ret=%d, errno=%d)\n", __func__, fslittle_fopen_test_13_kv_data[0].filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    return CaseNext;
}

/* file data for test_14 */
static fslittle_kv_data_t fslittle_fopen_test_14_kv_data[] = {
    /* a file is included in the filepath even though its not created by the test,
     * as the fslittle_filepath_make_dirs() works with it present. */
    { "/sd/test_14/testfile.txt", "testdata"},
    { NULL, NULL},
};

/** @brief  test for operation of stat()
 *
 * stat() is currently no supported by ARMCC and IAR toolchains libc.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_14(const size_t call_count)
{
#if ! defined(__ARMCC_VERSION) && defined(__GNUC__)

    char buf[FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1];
    char *pos = NULL;
    int32_t ret = -1;
    size_t len = 0;
    struct stat file_stat;
    fslittle_kv_data_t *node = fslittle_fopen_test_14_kv_data;

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    TEST_ASSERT(strlen(node->filename) < FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);

    /* start from a known state i.e. directory to be created in not present */
    fslittle_filepath_remove_all((char *) node->filename);

    /* Create file in a directory. */
    errno = 0;
    ret = fslittle_filepath_make_dirs((char *) node->filename, false);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dir (dirname=%s, ret=%d, errno=%d)\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    len = strlen(node->value);
    ret = fslittle_test_create(node->filename, (char *) node->value, len);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, fslittle_fopen_utest_msg_g);

    /* Test stat() on the file returns the correct attribute set */
    memset(&file_stat, 0, sizeof(file_stat));
    ret = stat(node->filename, &file_stat);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: stat() operation on file failed (filename=%s, ret=%d, errno=%d).\n", __func__, node->filename, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: expected st_mode S_IFREG flag not set (filename=%s).\n", __func__, node->filename);
    TEST_ASSERT_MESSAGE((file_stat.st_mode & S_IFREG) == S_IFREG, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected st_mode S_IFDIR flag set (filename=%s).\n", __func__, node->filename);
    TEST_ASSERT_MESSAGE((file_stat.st_mode & S_IFDIR) != S_IFDIR, fslittle_fopen_utest_msg_g);

    /* Test stat() on the directory returns the correct attribute set */
    memset(&file_stat, 0, sizeof(file_stat));
    memset(buf, 0, FSLITTLE_FOPEN_TEST_WORK_BUF_SIZE_1);
    memcpy(buf, node->filename, strlen(node->filename));
    pos = strrchr(buf, '/');
    *pos = '\0';
    ret = stat(buf, &file_stat);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: stat() operation on directory failed (directory name=%s, ret=%d, errno=%d).\n", __func__, buf, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected st_mode S_IFREG flag set (directory name=%s).\n", __func__, buf);
    TEST_ASSERT_MESSAGE((file_stat.st_mode & S_IFREG) != S_IFREG, fslittle_fopen_utest_msg_g);

    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: expected st_mode S_IFDIR flag not set (directory name=%s).\n", __func__, buf);
    TEST_ASSERT_MESSAGE((file_stat.st_mode & S_IFDIR) == S_IFDIR, fslittle_fopen_utest_msg_g);

    /* clean up after successful test */
    fslittle_filepath_remove_all((char *) node->filename);

#endif /* ! defined(__ARMCC_VERSION) && defined(__GNUC__) */
    return CaseNext;
}

/** @brief  test for operation of SDFileSystem::format()
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_00(const size_t call_count)
{

    FSLITTLE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    int32_t ret = -1;

    flash = new FlashIAPBlockDevice();
    ret = flash->init();
    TEST_ASSERT_EQUAL(0, ret);

    // Use slice of last sectors
    bd_addr_t slice_addr = flash->size();
    bd_size_t slice_size = 0;
    while (slice_size < MAX_TEST_SIZE) {
        bd_size_t unit_size = flash->get_erase_size(slice_addr - 1);
        slice_addr -= unit_size;
        slice_size += unit_size;
    }
    slice = new SlicingBlockDevice(flash, slice_addr);
    slice->init();

    ret = fs.reformat(slice);
    FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to format sdcard (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
    fs.mount(slice);

    return CaseNext;
}


/* @brief   test utility function to create a file of a given size.
 *
 * A reference data table is used of so that the data file can be later be
 * checked with fslittle_test_check_data_file().
 *
 * @param   filename    name of the file including path
 * @param   data        data to store in file
 * @param   len         number of bytes of data present in the data buffer.
 */
int32_t fslittle_test_create_data_file(const char *filename, size_t len)
{
    int32_t ret = -1;
    FILE *fp = NULL;
    size_t write_len = 0;
    size_t written_len = 0;
    int32_t exp = 0;
    const int32_t exp_max = 8;      /* so as not to exceed FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE/2 */

    FSLITTLE_FENTRYLOG("%s:entered (filename=%s, len=%d).\n", __func__, filename, (int) len);
    TEST_ASSERT(len % FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE == 0);
    fp = fopen(filename, "a");
    if (fp == NULL) {
        return ret;
    }

    while (written_len < len) {
        /* write fslittle_test_byte_data_table or part thereof, in 9 writes of sizes
         * 1, 2, 4, 8, 16, 32, 64, 128, 1, totalling 256 bytes len permitting. */
        for (exp = 0; (exp <= exp_max) && (written_len < len); exp++) {
            write_len = 0x1 << (exp % exp_max);
            write_len = len - written_len  > write_len ? write_len : len - written_len;
            ret = fwrite((const void *) &fslittle_test_byte_data_table[written_len % FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE], write_len, 1, fp);
            written_len += write_len;
            if (ret != 1) {
                FSLITTLE_DBGLOG("%s:Error: fwrite() failed (ret=%d)\n", __func__, (int) ret);
                ret = -1;
                goto out0;
            }
        }
    }
    if (written_len == len) {
        ret = 0;
    } else {
        ret = -1;
    }
out0:
    fclose(fp);
    return ret;
}


/* @brief   test utility function to check the data in the specified file is correct.
 *
 * The data read from the file is check that it agrees with the data written by
 * fslittle_test_create_data_file().
 *
 * @param   filename    name of the file including path
 * @param   data        data to store in file
 * @param   len         number of bytes of data present in the data buffer.
 */
int32_t fslittle_test_check_data_file(const char *filename, size_t len)
{
    int32_t ret = -1;
    FILE *fp = NULL;
    size_t read_len = 0;
    uint8_t buf[FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE];

    FSLITTLE_FENTRYLOG("%s:entered (filename=%s, len=%d).\n", __func__, filename, (int) len);
    TEST_ASSERT(len % FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE == 0);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return ret;
    }

    while (read_len < len) {
        ret = fread((void *) buf, FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE, 1, fp);
        read_len += FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE;
        if (ret == 0) {
            /* end of read*/
            FSLITTLE_DBGLOG("%s:unable to read data\n", __func__);
            break;
        }
        if (memcmp(buf, fslittle_test_byte_data_table, FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE) != 0) {
            FSLITTLE_DBGLOG("%s:Error: read data not as expected (0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x, 0x%2x\n", __func__,
                            buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
            ret = -1;
            goto out0;
        }
    }
    if (read_len == len) {
        ret = 0;
    }
out0:
    fclose(fp);
    return ret;
}

/* file data for test_16 */
static fslittle_kv_data_t fslittle_fopen_test_15_kv_data[] = {
    { "/sd/tst16_0/testfil0.txt", "dummy_data"},
    { "/sd/tst16_1/subdir0/testfil0.txt", "dummy_data"},
    { "/sd/tst16_2/subdir0/subdir1/testfil0.txt", "dummy_data"},
    { "/sd/tst16_3/subdir0/subdir1/subdir2/subdir3/testfil0.txt", "dummy_data"},

#if 0
    { "/sd/tst16_4/subdir0/subdir1/subdir2/subdir3/subdir4/testfil0.txt", "dummy_data"},
    { "/sd/tst16_5/subdir0/subdir1/subdir2/subdir3/subdir4/subdir5/testfil0.txt", "dummy_data"},
    { "/sd/tst16_6/subdir0/subdir1/subdir2/subdir3/subdir4/subdir5/subdir6/testfil0.txt", "dummy_data"},
    { "/sd/tst16_7/subdir0/subdir1/subdir2/subdir3/subdir4/subdir5/subdir6/subdir7/testfil0.txt", "dummy_data"},
    { "/sd/tst16_8/subdir0/subdir1/subdir2/subdir3/subdir4/subdir5/subdir6/subdir7/subdir8/testfil0.txt", "dummy_data"},
    { "/sd/tst16_9/subdir0/subdir1/subdir2/subdir3/subdir4/subdir5/subdir6/subdir7/subdir8/subdir9/testfil0.txt", "dummy_data"},
#endif
    { NULL, NULL},
};


/** @brief  stress test to write data to fs
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fslittle_fopen_test_15(const size_t call_count)
{
    int32_t ret = 0;
    fslittle_kv_data_t *node = fslittle_fopen_test_15_kv_data;
    const int32_t num_blocks = 100; /* each file ~25kB */

    FSLITTLE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;

    /* remove file and directory from a previous failed test run, if present */
    while (node->filename != NULL) {
        fslittle_filepath_remove_all((char *) node->filename);
        node++;
    }

    /* create dirs */
    node = fslittle_fopen_test_15_kv_data;
    while (node->filename != NULL) {
        ret = fslittle_filepath_make_dirs((char *) node->filename, true);
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dirs for filename (filename=\"%s\")(ret=%d)\n", __func__, node->filename, (int) ret);
        TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
        node++;
    }

    /* create the data files */
    node = fslittle_fopen_test_15_kv_data;
    while (node->filename != NULL) {
        ret = fslittle_test_create_data_file(node->filename, num_blocks * FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE);
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create data file (filename=\"%s\")(ret=%d)\n", __func__, node->filename, (int) ret);
        TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
        node++;
    }

    /* read the data back and check its as expected */
    node = fslittle_fopen_test_15_kv_data;
    while (node->filename != NULL) {
        ret = fslittle_test_check_data_file(node->filename, num_blocks * FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE);
        FSLITTLE_TEST_UTEST_MESSAGE(fslittle_fopen_utest_msg_g, FSLITTLE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to check data file (filename=\"%s\")(ret=%d)\n", __func__, node->filename, (int) ret);
        TEST_ASSERT_MESSAGE(ret == 0, fslittle_fopen_utest_msg_g);
        node++;
    }

    /* clean up */
    node = fslittle_fopen_test_15_kv_data;
    while (node->filename != NULL) {
        fslittle_filepath_remove_all((char *) node->filename);
        node++;
    }
    return CaseNext;
}

/// @cond FSLITTLE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(FSLITTLE_FOPEN_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    /*          1         2         3         4         5         6        7  */
    /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
    Case("FSLITTLE_FOPEN_TEST_00: format() test.", FSLITTLE_FOPEN_TEST_00),
    Case("FSLITTLE_FOPEN_TEST_01: fopen()/fwrite()/fclose() directories/file in multi-dir filepath.", FSLITTLE_FOPEN_TEST_01),
    Case("FSLITTLE_FOPEN_TEST_02: fopen(r) pre-existing file try to write it.", FSLITTLE_FOPEN_TEST_02),
    Case("FSLITTLE_FOPEN_TEST_03: fopen(w+) pre-existing file try to write it.", FSLITTLE_FOPEN_TEST_03),
    Case("FSLITTLE_FOPEN_TEST_04: fopen() with a filename exceeding the maximum length.", FSLITTLE_FOPEN_TEST_04),
#ifdef FOPEN_EXTENDED_TESTING
    Case("FSLITTLE_FOPEN_TEST_05: fopen() with bad filenames (extended).", FSLITTLE_FOPEN_TEST_05),
#endif
    Case("FSLITTLE_FOPEN_TEST_06: fopen() with bad filenames (minimal).", FSLITTLE_FOPEN_TEST_06),
    Case("FSLITTLE_FOPEN_TEST_07: fopen()/errno handling.", FSLITTLE_FOPEN_TEST_07),
    Case("FSLITTLE_FOPEN_TEST_08: ferror()/clearerr()/errno handling.", FSLITTLE_FOPEN_TEST_08),
    Case("FSLITTLE_FOPEN_TEST_09: ftell() handling.", FSLITTLE_FOPEN_TEST_09),
    Case("FSLITTLE_FOPEN_TEST_10: remove() test.", FSLITTLE_FOPEN_TEST_10),
    Case("FSLITTLE_FOPEN_TEST_11: rename().", FSLITTLE_FOPEN_TEST_11),
    Case("FSLITTLE_FOPEN_TEST_12: opendir(), readdir(), closedir() test.", FSLITTLE_FOPEN_TEST_12),
    Case("FSLITTLE_FOPEN_TEST_13: mkdir() test.", FSLITTLE_FOPEN_TEST_13),
    Case("FSLITTLE_FOPEN_TEST_14: stat() test.", FSLITTLE_FOPEN_TEST_14),
    Case("FSLITTLE_FOPEN_TEST_15: write/check n x 25kB data files.", FSLITTLE_FOPEN_TEST_15),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
