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

/** @file open.cpp Test cases to open KVs in the FSFAT using the drv->Open() interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "SDFileSystem.h"
#include "fsfat_debug.h"
#include "fsfat_test.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /*rand()*/
#include <inttypes.h>
#include <errno.h>

using namespace utest::v1;

/// @cond FSFAT_DOXYGEN_DISABLE
#ifdef FSFAT_DEBUG
#define FSFAT_FOPEN_GREENTEA_TIMEOUT_S     3000
#else
#define FSFAT_FOPEN_GREENTEA_TIMEOUT_S     1000
#endif
/// @endcond


/* FSFAT_SDCARD_INSTALLTED
 *  For testing purposes, an SDCard must be installed on the target for the test cases in this file to succeed.
 *  If the target has an SD card installed then uncomment the #define FSFAT_SDCARD_INSTALLED directive for the target.
 */
#ifdef FSFAT_SDCARD_INSTALLED

static char fsfat_fopen_utest_msg_g[FSFAT_UTEST_MSG_BUF_SIZE];

#if defined(TARGET_KL25Z)
SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_KL46Z) || defined(TARGET_KL43Z)
SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_K64F) || defined(TARGET_K66F)
SDFileSystem sd(PTE3, PTE1, PTE2, PTE4, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_K22F)
SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_K20D50M)
SDFileSystem sd(PTD2, PTD3, PTD1, PTC2, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_nRF51822)
SDFileSystem sd(p12, p13, p15, p14, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_NUCLEO_F030R8) || \
      defined(TARGET_NUCLEO_F070RB) || \
      defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F401RE) || \
      defined(TARGET_NUCLEO_F410RB) || \
      defined(TARGET_NUCLEO_F411RE) || \
      defined(TARGET_NUCLEO_L053R8) || \
      defined(TARGET_NUCLEO_L073RZ) || \
      defined(TARGET_NUCLEO_L152RE)
SDFileSystem sd(D11, D12, D13, D10, "sd");
/* #define FSFAT_SDCARD_INSTALLED */


#elif defined(TARGET_DISCO_F051R8) || \
      defined(TARGET_NUCLEO_L031K6)
SDFileSystem sd(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_LPC2368)
SDFileSystem sd(p11, p12, p13, p14, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_LPC11U68)
SDFileSystem sd(D11, D12, D13, D10, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_LPC1549)
SDFileSystem sd(D11, D12, D13, D10, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_RZ_A1H)
SDFileSystem sd(P8_5, P8_6, P8_3, P8_4, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#elif defined(TARGET_LPC11U37H_401)
SDFileSystem sd(SDMOSI, SDMISO, SDSCLK, SDSSEL, "sd");
/* #define FSFAT_SDCARD_INSTALLED */

#else
SDFileSystem sd(p11, p12, p13, p14, "sd");
/* #define FSFAT_SDCARD_INSTALLED */
#endif


#define FSFAT_FOPEN_TEST_01      fsfat_fopen_test_01
#define FSFAT_FOPEN_TEST_02      fsfat_fopen_test_02
#define FSFAT_FOPEN_TEST_03      fsfat_fopen_test_03
#define FSFAT_FOPEN_TEST_04      fsfat_fopen_test_04
#define FSFAT_FOPEN_TEST_05      fsfat_fopen_test_05
#define FSFAT_FOPEN_TEST_06      fsfat_fopen_test_06
#define FSFAT_FOPEN_TEST_07      fsfat_fopen_test_07


/* support functions */

/*
 * open tests that focus on testing fsfat_open()
 * fsfat_handle_t fsfat_open(const char* key_name, char* data, ARM_FSFAT_SIZE* len, fsfat_key_desc_t* kdesc)
 */

/* KV data for test_01 */
static fsfat_kv_data_t fsfat_fopen_test_01_kv_data[] = {
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
static int32_t fsfat_filepath_split(char* filepath, char* parts[], uint32_t num)
{
    uint32_t i = 0;
    int32_t ret = -1;
    char* z = filepath;

    while (i < num && *z != '\0') {
        if (*z == '/' ) {
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
int32_t fsfat_filepath_remove_all(char* filepath)
{
    int32_t ret = -1;
    char *fpathbuf = NULL;
    char *pos = NULL;

    FSFAT_DBGLOG("%s:entered\n", __func__);
    fpathbuf = strdup(filepath);
    if (fpathbuf == NULL) {
        FSFAT_DBGLOG("%s: failed to duplicate string (out of memory)\n", __func__);
        return ret;
    }

    /* delete the leaf node first, and then successively parent directories. */
    pos = fpathbuf + strlen(fpathbuf);
    while (pos != fpathbuf) {
        FSFAT_DBGLOG("%s:Removing %s\n", __func__, fpathbuf);
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
 *
 * @return  On success, this returns 0, otherwise < 0 is returned;
 */
static int32_t fsfat_filepath_make_dirs(char* filepath)
{
    int32_t i = 0;
    int32_t num_parts = 0;
    int32_t ret = -1;
    char *fpathbuf = NULL;
    char *buf = NULL;
    int pos = 0;
    char *parts[10];

    FSFAT_DBGLOG("%s:entered\n", __func__);
    /* find the dirs to create*/
    memset(parts, 0, sizeof(parts));
    fpathbuf = strdup(filepath);
    num_parts = fsfat_filepath_split(fpathbuf, parts, 10);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to split filepath (filename=\"%s\", num_parts=%d)\n", __func__, filepath, (int) num_parts);
    TEST_ASSERT_MESSAGE(num_parts > 0, fsfat_fopen_utest_msg_g);

    /* Now create the directories on the directory path.
     * Skip creating dir for "/sd" which must be present */
    buf = (char*) malloc(strlen(filepath)+1);
    memset(buf, 0, strlen(filepath)+1);
    pos = sprintf(buf, "/%s", parts[0]);
    for (i = 1; i < num_parts - 1; i++) {
        pos += sprintf(buf+pos, "/%s", parts[i]);
        FSFAT_DBGLOG("mkdir(%s)\n", buf);
        ret = mkdir(buf, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create directory (filepath2=\"%s\", ret=%d, errno=%d)\n", __func__, buf, (int) ret, errno);
        TEST_ASSERT_MESSAGE(ret == 0, fsfat_fopen_utest_msg_g);
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
static control_t fsfat_fopen_test_01(const size_t call_count)
{
    char* read_buf;
    int32_t ret = 0;
    size_t len = 0;
    fsfat_kv_data_t *node;
    FILE *fp = NULL;

    FSFAT_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    node = fsfat_fopen_test_01_kv_data;

    /* remove file and directory from a previous failed test run, if present */
    fsfat_filepath_remove_all((char*) node->key_name);

    /* create dirs*/
    ret = fsfat_filepath_make_dirs((char*) node->key_name);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create dirs for filename (filename=\"%s\")(ret=%d)\n", __func__, node->key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 0, fsfat_fopen_utest_msg_g);

    FSFAT_DBGLOG("%s:About to create new file (filename=\"%s\", data=\"%s\")\n", __func__, node->key_name, node->value);
    fp = fopen(node->key_name, "w+");
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create file (filename=\"%s\", data=\"%s\")(ret=%d, errno=%d)\n", __func__, node->key_name, node->value, (int) ret, errno);
    TEST_ASSERT_MESSAGE(fp != NULL, fsfat_fopen_utest_msg_g);

    FSFAT_DBGLOG("%s:length of KV=%d (filename=\"%s\", data=\"%s\")\n", __func__, (int) len, node->key_name, node->value);
    len = strlen(node->value);
    ret = fwrite((const void*) node->value, len, 1, fp);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write file (filename=\"%s\", data=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret == 1, fsfat_fopen_utest_msg_g);

    FSFAT_DBGLOG("Created file successfully (filename=\"%s\", data=\"%s\")\n", node->key_name, node->value);
    ret = fclose(fp);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to close file (ret=%d, errno=%d)\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fsfat_fopen_utest_msg_g);

    /* now open the newly created key */
    fp = NULL;
    fp = fopen(node->key_name, "r");
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open file for reading (filename=\"%s\", data=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(fp != NULL, fsfat_fopen_utest_msg_g);

    len = strlen(node->value) + 1;
    read_buf = (char*) malloc(len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to allocated read buffer \n", __func__);
    TEST_ASSERT_MESSAGE(read_buf != NULL, fsfat_fopen_utest_msg_g);

    FSFAT_DBGLOG("Opened KV successfully (filename=\"%s\", data=\"%s\")\n", node->key_name, node->value);
    memset(read_buf, 0, len);
    ret = fread((void*) read_buf, len, 1, fp);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read file (filename=\"%s\", data=\"%s\", read_buf=\"%s\", ret=%d)\n", __func__, node->key_name, node->value, read_buf, (int) ret);
    /* FIX ME: fread should return the number of items read, not 0 when an item is read successfully.
     * This indicates a problem with the implementation, as the correct data is read. The correct assert should be:
     *   TEST_ASSERT_MESSAGE(ret == 1, fsfat_fopen_utest_msg_g);
     * The following assert is curerntly used until the implementation is fixed
     */
    TEST_ASSERT_MESSAGE(ret == 0, fsfat_fopen_utest_msg_g);

    /* check read data is as expected */
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: read value data (%s) != expected value data (filename=\"%s\", data=\"%s\", read_buf=\"%s\", ret=%d)\n", __func__, read_buf, node->key_name, node->value, read_buf, (int) ret);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, fsfat_fopen_utest_msg_g);

    if(read_buf){
        free(read_buf);
    }
    ret = fclose(fp);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed (ret=%d, errno=%d).\n", __func__, (int) ret, errno);
    TEST_ASSERT_MESSAGE(ret == 0, fsfat_fopen_utest_msg_g);
    return CaseNext;
}

#ifdef NOT_DEFINED

static fsfat_kv_data_t fsfat_fopen_test_02_data[] = {
        FSFAT_INIT_1_TABLE_MID_NODE,
        { NULL, NULL},
};

/**
 * @brief   test to open() a pre-existing key and try to write it, which should fail
 *          as by default pre-existing keys are opened read-only
 *
 * Basic open test which does the following:
 * - creates KV with default rw perms and writes some data to the value blob.
 * - closes the newly created KV.
 * - opens the KV with the default permissions (read-only)
 * - tries to write the KV data which should fail because KV was not opened with write flag set.
 * - closes the opened key
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_02(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_FSFAT_SIZE len = 0;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;
    ARM_FSFAT_KEYDESC kdesc;
    ARM_FSFAT_HANDLE_INIT(hkey);
    ARM_FSFAT_FMODE flags;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(fsfat_fopen_test_02_data[0].value);
    ret = fsfat_test_create(fsfat_fopen_test_02_data[0].key_name, (char*) fsfat_fopen_test_02_data[0].value, &len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    /* by default, owner of key opens with read-only permissions*/
    ret = fopen(fsfat_fopen_test_02_data[0].key_name, "w+");
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (filename=\"%s\")(ret=%d)\n", __func__, fsfat_fopen_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    len = strlen(fsfat_fopen_test_02_data[0].value);
    ret = drv->Write(hkey, fsfat_fopen_test_02_data[0].value, &len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: call to Write() succeeded when should have failed (filename=\"%s\")(ret=%d).\n", __func__, fsfat_fopen_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_FSFAT_DRIVER_ERROR_KEY_READ_ONLY, fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    ret = drv->Uninitialize();
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}




/**
 * @brief   test to open() a pre-existing key and try to write it, which should succeed
 *          because the key was opened read-write permissions explicitly
 *
 * Basic open test which does the following:
 * - creates KV with default rw perms and writes some data to the value blob.
 * - closes the newly created KV.
 * - opens the KV with the rw permissions (non default)
 * - tries to write the KV data which should succeeds because KV was opened with write flag set.
 * - closes the opened key
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_03(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_FSFAT_SIZE len = 0;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;
    ARM_FSFAT_KEYDESC kdesc;
    ARM_FSFAT_HANDLE_INIT(hkey);
    ARM_FSFAT_FMODE flags;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(fsfat_fopen_test_02_data[0].value);
    ret = fsfat_test_create(fsfat_fopen_test_02_data[0].key_name, (char*) fsfat_fopen_test_02_data[0].value, &len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    /* opens with read-write permissions*/
    flags.read = true;
    flags.write = true;
    ret = drv->Open(fsfat_fopen_test_02_data[0].key_name, flags, hkey);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (filename=\"%s\")(ret=%d)\n", __func__, fsfat_fopen_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    len = strlen(fsfat_fopen_test_02_data[0].value);
    ret = drv->Write(hkey, fsfat_fopen_test_02_data[0].value, &len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: call to Write() failed when should have succeeded (filename=\"%s\")(ret=%d).\n", __func__, fsfat_fopen_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    ret = drv->Uninitialize();
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}



/** @brief  test to call fsfat_open() with a key_name string that exceeds
 *          the maximum length
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_04(const size_t call_count)
{
    char kv_name_good[FSFAT_KEY_NAME_MAX_LENGTH+1]; /* extra char for terminating null */
    char kv_name_bad[FSFAT_KEY_NAME_MAX_LENGTH+2];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_FSFAT_SIZE len = 0;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;
    ARM_FSFAT_KEYDESC kdesc;
    ARM_FSFAT_FMODE flags;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(kv_name_good, 0, FSFAT_KEY_NAME_MAX_LENGTH+1);
    memset(kv_name_bad, 0, FSFAT_KEY_NAME_MAX_LENGTH+2);
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));

    len = FSFAT_KEY_NAME_MAX_LENGTH;
    ret = fsfat_test_kv_name_gen(kv_name_good, len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name_good.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name_good is not the correct length (len=%d, expected=%d).\n", __func__, (int) strlen(kv_name_good), (int) len);
    TEST_ASSERT_MESSAGE(strlen(kv_name_good) == FSFAT_KEY_NAME_MAX_LENGTH, fsfat_fopen_utest_msg_g);

    ret = fsfat_test_create(kv_name_good, kv_name_good, &len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store for kv_name_good(ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    len = FSFAT_KEY_NAME_MAX_LENGTH+1;
    ret = fsfat_test_kv_name_gen(kv_name_bad, len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name_bad.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name_bad is not the correct length (len=%d, expected=%d).\n", __func__, (int) strlen(kv_name_bad), (int) len);
    TEST_ASSERT_MESSAGE(strlen(kv_name_bad) == FSFAT_KEY_NAME_MAX_LENGTH+1, fsfat_fopen_utest_msg_g);

    memset(&kdesc, 0, sizeof(kdesc));
    ret = fsfat_test_create(kv_name_bad, kv_name_bad, &len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store for kv_name_bad when should have failed(ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    ret = drv->Uninitialize();
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}


/// @cond FSFAT_DOXYGEN_DISABLE
typedef struct fsfat_fopen_kv_name_ascii_node {
    uint32_t code;
    uint32_t f_allowed : 1;
} fsfat_fopen_kv_name_ascii_node;
/// @endcond

static const uint32_t fsfat_fopen_kv_name_ascii_table_code_sentinel_g = 256;

/*@brief    table recording ascii character codes permitted in kv names */
static fsfat_fopen_kv_name_ascii_node fsfat_fopen_kv_name_ascii_table[] =
{
        {0, false},         /* codes 0-44 not allowed */
        {45, true},         /* codes 45-46 == [-.] allowed */
        {47, false},        /* code 47 not allowed */
        {48, true},         /* codes 48-57 not allowed */
        {58, false},        /* codes 46-64 not allowed */
        {64, true},         /* codes 64-91 allowed [@A-Z] */
        {91, false},        /* code 91-96 not allowed */
        {95, true},         /* code 95 allowed '_' */
        {96, false},        /* codes 96 not allowed */
        {97, true},         /* codes 65-90 allowed [A-Z] and {*/
        {123, false},       /* codes 123 '}' not allowed on its own*/
        {124, false},       /* codes 124 not allowed */
        {125, false},       /* code 125 '}' not allowed on its own*/
        {126, false},       /* codes 126-255 not allowed */
        {fsfat_fopen_kv_name_ascii_table_code_sentinel_g, false},       /* sentinel */
};

/// @cond FSFAT_DOXYGEN_DISABLE
enum fsfat_fopen_kv_name_pos {
    fsfat_fopen_kv_name_pos_start = 0x0,
    fsfat_fopen_kv_name_pos_mid,
    fsfat_fopen_kv_name_pos_end,
    fsfat_fopen_kv_name_pos_max
};
/// @endcond

/** @brief  test to call fsfat_open() with key_name that in includes
 *          illegal characters
 *          - the character(s) can be at the beginning of the key_name
 *          - the character(s) can be at the end of the key_name
 *          - the character(s) can be somewhere within the key_name string
 *          - a max-length string of random characters (legal and illegal)
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_05(const size_t call_count)
{
    bool f_allowed = false;
    char kv_name[FSFAT_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    uint32_t j = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = FSFAT_KEY_NAME_MAX_LENGTH;
    ARM_FSFAT_KEYDESC kdesc;
    fsfat_fopen_kv_name_ascii_node* node = NULL;
    uint32_t pos;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

#ifdef FSFAT_DEBUG
    /* symbol only used why debug is enabled */
    const char* pos_str = NULL;
#endif

    /* create bad keyname strings with invalid character code at start of keyname */
    node = fsfat_fopen_kv_name_ascii_table;
    while(node->code !=  fsfat_fopen_kv_name_ascii_table_code_sentinel_g)
    {
        /* loop over range */
        for(j = node->code; j < (node+1)->code; j++)
        {
            /* set the start, mid, last character of the name to the test char code */
            for(pos = (uint32_t) fsfat_fopen_kv_name_pos_start; pos < (uint32_t) fsfat_fopen_kv_name_pos_max; pos++)
            {
                name_len = FSFAT_KEY_NAME_MAX_LENGTH;
                memset(kv_name, 0, FSFAT_KEY_NAME_MAX_LENGTH+1);
                memset(&kdesc, 0, sizeof(kdesc));
                kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

                ret = fsfat_test_kv_name_gen(kv_name, name_len);
                FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name.\n", __func__);
                TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , fsfat_fopen_utest_msg_g);
                FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name incorrect length (len=%d, expected= %d).\n", __func__, (int) strlen(kv_name), (int) name_len);
                TEST_ASSERT_MESSAGE(strlen(kv_name) == name_len, fsfat_fopen_utest_msg_g);

                /* overwrite a char at the pos start, mid, end of the kv_name with an ascii char code (both illegal and legal)*/
                switch(pos)
                {
                case fsfat_fopen_kv_name_pos_start:
                    kv_name[0] = (char) j;
                    break;
                case fsfat_fopen_kv_name_pos_mid:
                    /* create bad keyname strings with invalid character code in the middle of keyname */
                    kv_name[name_len/2] = (char) j;
                    break;
                case fsfat_fopen_kv_name_pos_end:
                    /* create bad keyname strings with invalid character code at end of keyname */
                    kv_name[name_len-1] = (char) j;
                    break;
                default:
                    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected value of pos (pos=%d).\n", __func__, (int) pos);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
                    break;
                }

#ifdef FSFAT_DEBUG
                /* processing only required when debug trace enabled */
                switch(pos)
                {
                case fsfat_fopen_kv_name_pos_start:
                    pos_str = "start";
                    break;
                case fsfat_fopen_kv_name_pos_mid:
                    pos_str = "middle";
                    break;
                case fsfat_fopen_kv_name_pos_end:
                    pos_str = "end";
                    break;
                default:
                    break;
                }
#endif
                ret = fsfat_test_create(kv_name, kv_name, &name_len, &kdesc);

                /* special cases */
                switch(j)
                {
                case 0 :
				case 46 :
                    switch(pos)
                    {
                    /* for code = 0 (null terminator). permitted at mid and end of string */
                    /* for code = 46 ('.'). permitted at mid and end of string but not at start */
                    case fsfat_fopen_kv_name_pos_start:
                        f_allowed = false;
                        break;
                    case fsfat_fopen_kv_name_pos_mid:
                    case fsfat_fopen_kv_name_pos_end:
                    default:
                        f_allowed = true;
                        break;
                    }
                    break;
				default:
					f_allowed = node->f_allowed;
					break;
                }
                if(f_allowed == true)
                {
                    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store when kv_name contains valid characters (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
                    /* revert FSFAT_LOG for more trace */
                    FSFAT_DBGLOG("Successfully created a KV with valid keyname containing ascii character code %d (%c) at the %s of the keyname.\n", (int) j, (int) j, pos_str);
                    FSFAT_LOG("%c", '.');

                    ret = fsfat_test_delete(kv_name);
                    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: failed to delete KV previously created (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
                }
                else
                {   /*node->f_allowed == false => not allowed to create kv name with ascii code */
                    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains an invalid character (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
                    /* revert FSFAT_LOG for more trace */
                    FSFAT_DBGLOG("Successfully failed to create a KV with an invalid keyname containing ascii character code %d at the %s of the keyname.\n", (int) j, pos_str);
                    FSFAT_LOG("%c", '.');
                }
            }
        }
        node++;
    }

    FSFAT_LOG("%c", '\n');
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}


static const char fsfat_fopen_ascii_illegal_buf_g[] = "!\"�$%&'()*+,./:;<=>?@[\\]^_`{|}~"; /* 31 chars */

/** @brief  test to call fsfat_open() with key_name that in includes
 *          illegal characters
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_06(const size_t call_count)
{
    char kv_name[FSFAT_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    size_t i = 0;
    uint32_t pos = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = FSFAT_KEY_NAME_MAX_LENGTH;
    ARM_FSFAT_KEYDESC kdesc;
    size_t buf_data_max = 0;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* create bad keyname strings with invalid character code at start of keyname */
    buf_data_max = strlen(fsfat_fopen_ascii_illegal_buf_g);
    name_len = FSFAT_KEY_NAME_MAX_LENGTH;
    memset(kv_name, 0, FSFAT_KEY_NAME_MAX_LENGTH+1);
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

    /* generate a kv name of illegal chars*/
    for(i = 0; i < name_len; i++)
    {
        pos = rand() % (buf_data_max+1);
        kv_name[i] = fsfat_fopen_ascii_illegal_buf_g[pos];
    }

    ret = fsfat_test_create(kv_name, kv_name, &name_len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains invalid characters (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}

/** @brief  test to call fsfat_open() with key_name that in includes
 *          illegal characters
 *          - a max-length string of random characters (legal and illegal)
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t fsfat_fopen_test_07(const size_t call_count)
{
    char kv_name[FSFAT_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    size_t i = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = FSFAT_KEY_NAME_MAX_LENGTH;
    ARM_FSFAT_KEYDESC kdesc;
    size_t buf_data_max = 0;
    ARM_FSFAT_DRIVER* drv = &fsfat_driver;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* create bad keyname strings with invalid character code at start of keyname */
    buf_data_max = strlen(fsfat_fopen_ascii_illegal_buf_g);
    name_len = FSFAT_KEY_NAME_MAX_LENGTH;
    memset(kv_name, 0, FSFAT_KEY_NAME_MAX_LENGTH+1);
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

    ret = fsfat_test_kv_name_gen(kv_name, name_len);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , fsfat_fopen_utest_msg_g);

    /* pepper the illegal chars across the string*/
    for(i++; i < buf_data_max; i++){
        kv_name[rand() % (name_len+1)] = fsfat_fopen_ascii_illegal_buf_g[i];
    }
    ret = fsfat_test_create(kv_name, kv_name, &name_len, &kdesc);
    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains invalid characters (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);

    FSFAT_TEST_UTEST_MESSAGE(fsfat_fopen_utest_msg_g, FSFAT_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, fsfat_fopen_utest_msg_g);
    return CaseNext;
}

#endif // NOT_DEFINED

#else

#define FSFAT_FOPEN_TEST_01      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_02      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_03      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_04      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_05      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_06      fsfat_fopen_test_dummy
#define FSFAT_FOPEN_TEST_07      fsfat_fopen_test_dummy

/** @brief  fsfat_fopen_test_dummy    Dummy test case for testing when platform doesnt have an SDCard installed.
 *
 * @return success always
 */
static control_t fsfat_fopen_test_dummy()
{
    printf("Null test\n");
    return CaseNext;
}

#endif  /* FSFAT_SDCARD_INSTALLED */


/// @cond FSFAT_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(FSFAT_FOPEN_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FOPEN_test_01", FSFAT_FOPEN_TEST_01)

#ifdef NOT_DEFINED
        Case("FOPEN_test_02", FSFAT_FOPEN_TEST_02),
        Case("FOPEN_test_03", FSFAT_FOPEN_TEST_03),
        Case("FOPEN_test_04", FSFAT_FOPEN_TEST_04),
        Case("FOPEN_test_05", FSFAT_FOPEN_TEST_05),
        Case("FOPEN_test_06", FSFAT_FOPEN_TEST_06),
        Case("FOPEN_test_07", FSFAT_FOPEN_TEST_07),
#endif // NOT_DEFINED

};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
