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

/** @file open.cpp Test cases to open KVs in the CFSTORE using the drv->Open() interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "cfstore_config.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
#include "Driver_Common.h"
#include "configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "cfstore_utest.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /*rand()*/
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_open_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.open.box1");
UVISOR_BOX_CONFIG(cfstore_open_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/// @cond CFSTORE_DOXYGEN_DISABLE
#ifdef CFSTORE_DEBUG
#define CFSTORE_OPEN_GREENTEA_TIMEOUT_S     3000
#else
#define CFSTORE_OPEN_GREENTEA_TIMEOUT_S     1000
#endif
/// @endcond


/* support functions */

/*
 * open tests that focus on testing cfstore_open()
 * cfstore_handle_t cfstore_open(const char* key_name, char* data, ARM_CFSTORE_SIZE* len, cfstore_key_desc_t* kdesc)
 */

/* KV data for test_01 */
static cfstore_kv_data_t cfstore_open_test_01_kv_data[] = {
        { "yotta.hello-world.animal{wobbly-dog}{foot}frontLeft", "missing"},
        { NULL, NULL},
};


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_open_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}

/** @brief
 * Basic open test which does the following:
 * - creates KV with default rw perms and writes some data to the value blob.
 * - closes the newly created KV.
 * - opens the KV with the default permissions (r-only)
 * - reads the KV data and checks its the same as the previously created data.
 * - closes the opened key
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_open_test_01_end(const size_t call_count)
{
    char* read_buf;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    cfstore_kv_data_t *node;
    ARM_CFSTORE_FMODE flags;

    CFSTORE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    node = cfstore_open_test_01_kv_data;
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));

    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    CFSTORE_DBGLOG("%s:About to create new node (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    ret = drv->Create(node->key_name, strlen(node->value), &kdesc, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    CFSTORE_DBGLOG("%s:length of KV=%d (key_name=\"%s\", value=\"%s\")\n", __func__, (int) len, node->key_name, node->value);
    len = strlen(node->value);
    ret = drv->Write(hkey, (char*) node->value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write full value data (key_name=\"%s\", value=\"%s\"), len=%d, (ret=%d)\n", __func__, node->key_name, node->value, (int) len, (int) ret);
    TEST_ASSERT_MESSAGE(len == strlen(node->value), cfstore_open_utest_msg_g);

    CFSTORE_DBGLOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);
    ret = drv->Close(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to close handle (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    /* now open the newly created key */
    ret = drv->Open(node->key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    len = strlen(node->value) + 1;
    read_buf = (char*) malloc(len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to allocated read buffer \n", __func__);
    TEST_ASSERT_MESSAGE(read_buf != NULL, cfstore_open_utest_msg_g);

    CFSTORE_DBGLOG("Opened KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);
    memset(read_buf, 0, len);
    ret = drv->Read(hkey, read_buf, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    /* check read data is as expected */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: read value data (%s) != KV value data (key_name=\"%s\", value=\"%s\")\n", __func__, read_buf, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, cfstore_open_utest_msg_g);

    if(read_buf){
        free(read_buf);
    }
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}

static cfstore_kv_data_t cfstore_open_test_02_data[] = {
        CFSTORE_INIT_1_TABLE_MID_NODE,
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
control_t cfstore_open_test_02_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_open_test_02_data[0].value);
    ret = cfstore_test_create(cfstore_open_test_02_data[0].key_name, (char*) cfstore_open_test_02_data[0].value, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    /* by default, owner of key opens with read-only permissions*/
    ret = drv->Open(cfstore_open_test_02_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\")(ret=%d)\n", __func__, cfstore_open_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    len = strlen(cfstore_open_test_02_data[0].value);
    ret = drv->Write(hkey, cfstore_open_test_02_data[0].value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: call to Write() succeeded when should have failed (key_name=\"%s\")(ret=%d).\n", __func__, cfstore_open_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_READ_ONLY, cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
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
control_t cfstore_open_test_03_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_open_test_02_data[0].value);
    ret = cfstore_test_create(cfstore_open_test_02_data[0].key_name, (char*) cfstore_open_test_02_data[0].value, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    /* opens with read-write permissions*/
    flags.read = true;
    flags.write = true;
    ret = drv->Open(cfstore_open_test_02_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\")(ret=%d)\n", __func__, cfstore_open_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    len = strlen(cfstore_open_test_02_data[0].value);
    ret = drv->Write(hkey, cfstore_open_test_02_data[0].value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: call to Write() failed when should have succeeded (key_name=\"%s\")(ret=%d).\n", __func__, cfstore_open_test_02_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}



/** @brief  test to call cfstore_open() with a key_name string that exceeds
 *          the maximum length
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_open_test_04_end(const size_t call_count)
{
    char kv_name_good[CFSTORE_KEY_NAME_MAX_LENGTH+1]; /* extra char for terminating null */
    char kv_name_bad[CFSTORE_KEY_NAME_MAX_LENGTH+2];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(kv_name_good, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    memset(kv_name_bad, 0, CFSTORE_KEY_NAME_MAX_LENGTH+2);
    memset(&kdesc, 0, sizeof(kdesc));
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));

    len = CFSTORE_KEY_NAME_MAX_LENGTH;
    ret = cfstore_test_kv_name_gen(kv_name_good, len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name_good.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name_good is not the correct length (len=%d, expected=%d).\n", __func__, (int) strlen(kv_name_good), (int) len);
    TEST_ASSERT_MESSAGE(strlen(kv_name_good) == CFSTORE_KEY_NAME_MAX_LENGTH, cfstore_open_utest_msg_g);

    ret = cfstore_test_create(kv_name_good, kv_name_good, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store for kv_name_good(ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    ret = cfstore_test_kv_name_gen(kv_name_bad, len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name_bad.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name_bad is not the correct length (len=%d, expected=%d).\n", __func__, (int) strlen(kv_name_bad), (int) len);
    TEST_ASSERT_MESSAGE(strlen(kv_name_bad) == CFSTORE_KEY_NAME_MAX_LENGTH+1, cfstore_open_utest_msg_g);

    memset(&kdesc, 0, sizeof(kdesc));
    ret = cfstore_test_create(kv_name_bad, kv_name_bad, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store for kv_name_bad when should have failed(ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
typedef struct cfstore_open_kv_name_ascii_node {
    uint32_t code;
    uint32_t f_allowed : 1;
} cfstore_open_kv_name_ascii_node;
/// @endcond

static const uint32_t cfstore_open_kv_name_ascii_table_code_sentinel_g = 256;

/*@brief    table recording ascii character codes permitted in kv names */
static cfstore_open_kv_name_ascii_node cfstore_open_kv_name_ascii_table[] =
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
        {cfstore_open_kv_name_ascii_table_code_sentinel_g, false},       /* sentinel */
};

/// @cond CFSTORE_DOXYGEN_DISABLE
enum cfstore_open_kv_name_pos {
    cfstore_open_kv_name_pos_start = 0x0,
    cfstore_open_kv_name_pos_mid,
    cfstore_open_kv_name_pos_end,
    cfstore_open_kv_name_pos_max
};
/// @endcond

/** @brief  test to call cfstore_open() with key_name that in includes
 *          illegal characters
 *          - the character(s) can be at the beginning of the key_name
 *          - the character(s) can be at the end of the key_name
 *          - the character(s) can be somewhere within the key_name string
 *          - a max-length string of random characters (legal and illegal)
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_open_test_05_end(const size_t call_count)
{
    bool f_allowed = false;
    char kv_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    uint32_t j = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
    ARM_CFSTORE_KEYDESC kdesc;
    cfstore_open_kv_name_ascii_node* node = NULL;
    uint32_t pos;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

#ifdef CFSTORE_DEBUG
    /* symbol only used why debug is enabled */
    const char* pos_str = NULL;
#endif

    /* create bad keyname strings with invalid character code at start of keyname */
    node = cfstore_open_kv_name_ascii_table;
    while(node->code !=  cfstore_open_kv_name_ascii_table_code_sentinel_g)
    {
        /* loop over range */
        for(j = node->code; j < (node+1)->code; j++)
        {
            /* set the start, mid, last character of the name to the test char code */
            for(pos = (uint32_t) cfstore_open_kv_name_pos_start; pos < (uint32_t) cfstore_open_kv_name_pos_max; pos++)
            {
                name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
                memset(kv_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
                memset(&kdesc, 0, sizeof(kdesc));
                kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

                ret = cfstore_test_kv_name_gen(kv_name, name_len);
                CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name.\n", __func__);
                TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , cfstore_open_utest_msg_g);
                CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: kv_name incorrect length (len=%d, expected= %d).\n", __func__, (int) strlen(kv_name), (int) name_len);
                TEST_ASSERT_MESSAGE(strlen(kv_name) == name_len, cfstore_open_utest_msg_g);

                /* overwrite a char at the pos start, mid, end of the kv_name with an ascii char code (both illegal and legal)*/
                switch(pos)
                {
                case cfstore_open_kv_name_pos_start:
                    kv_name[0] = (char) j;
                    break;
                case cfstore_open_kv_name_pos_mid:
                    /* create bad keyname strings with invalid character code in the middle of keyname */
                    kv_name[name_len/2] = (char) j;
                    break;
                case cfstore_open_kv_name_pos_end:
                    /* create bad keyname strings with invalid character code at end of keyname */
                    kv_name[name_len-1] = (char) j;
                    break;
                default:
                    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unexpected value of pos (pos=%d).\n", __func__, (int) pos);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
                    break;
                }

#ifdef CFSTORE_DEBUG
                /* processing only required when debug trace enabled */
                switch(pos)
                {
                case cfstore_open_kv_name_pos_start:
                    pos_str = "start";
                    break;
                case cfstore_open_kv_name_pos_mid:
                    pos_str = "middle";
                    break;
                case cfstore_open_kv_name_pos_end:
                    pos_str = "end";
                    break;
                default:
                    break;
                }
#endif
                ret = cfstore_test_create(kv_name, kv_name, &name_len, &kdesc);

                /* special cases */
                switch(j)
                {
                case 0 :
				case 46 :
                    switch(pos)
                    {
                    /* for code = 0 (null terminator). permitted at mid and end of string */
                    /* for code = 46 ('.'). permitted at mid and end of string but not at start */
                    case cfstore_open_kv_name_pos_start:
                        f_allowed = false;
                        break;
                    case cfstore_open_kv_name_pos_mid:
                    case cfstore_open_kv_name_pos_end:
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
                    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store when kv_name contains valid characters (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
                    /* revert CFSTORE_LOG for more trace */
                    CFSTORE_DBGLOG("Successfully created a KV with valid keyname containing ascii character code %d (%c) at the %s of the keyname.\n", (int) j, (int) j, pos_str);
                    CFSTORE_LOG("%c", '.');

                    ret = cfstore_test_delete(kv_name);
                    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to delete KV previously created (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
                }
                else
                {   /*node->f_allowed == false => not allowed to create kv name with ascii code */
                    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains an invalid character (code=%d, ret=%d).\n", __func__, (int) j, (int) ret);
                    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_open_utest_msg_g);
                    /* revert CFSTORE_LOG for more trace */
                    CFSTORE_DBGLOG("Successfully failed to create a KV with an invalid keyname containing ascii character code %d at the %s of the keyname.\n", (int) j, pos_str);
                    CFSTORE_LOG("%c", '.');
                }
            }
        }
        node++;
    }

    CFSTORE_LOG("%c", '\n');
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}


static const char cfstore_open_ascii_illegal_buf_g[] = "!\"�$%&'()*+,./:;<=>?@[\\]^_`{|}~"; /* 31 chars */

/** @brief  test to call cfstore_open() with key_name that in includes
 *          illegal characters
 *          - a max-length string of random illegal characters only
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_open_test_06_end(const size_t call_count)
{
    char kv_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    size_t i = 0;
    uint32_t pos = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
    ARM_CFSTORE_KEYDESC kdesc;
    size_t buf_data_max = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* create bad keyname strings with invalid character code at start of keyname */
    buf_data_max = strlen(cfstore_open_ascii_illegal_buf_g);
    name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
    memset(kv_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

    /* generate a kv name of illegal chars*/
    for(i = 0; i < name_len; i++)
    {
        pos = rand() % (buf_data_max+1);
        kv_name[i] = cfstore_open_ascii_illegal_buf_g[pos];
    }

    ret = cfstore_test_create(kv_name, kv_name, &name_len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains invalid characters (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}

/** @brief  test to call cfstore_open() with key_name that in includes
 *          illegal characters
 *          - a max-length string of random characters (legal and illegal)
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_open_test_07_end(const size_t call_count)
{
    char kv_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];    /* extra char for terminating null */
    size_t i = 0;
    int32_t ret = ARM_DRIVER_OK;
    size_t name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
    ARM_CFSTORE_KEYDESC kdesc;
    size_t buf_data_max = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* create bad keyname strings with invalid character code at start of keyname */
    buf_data_max = strlen(cfstore_open_ascii_illegal_buf_g);
    name_len = CFSTORE_KEY_NAME_MAX_LENGTH;
    memset(kv_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;

    ret = cfstore_test_kv_name_gen(kv_name, name_len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to generate kv_name.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK , cfstore_open_utest_msg_g);

    /* pepper the illegal chars across the string*/
    for(i++; i < buf_data_max; i++){
        kv_name[rand() % (name_len+1)] = cfstore_open_ascii_illegal_buf_g[i];
    }
    ret = cfstore_test_create(kv_name, kv_name, &name_len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: created KV in store when kv_name contains invalid characters (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_open_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_open_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, cfstore_open_utest_msg_g);
    return CaseNext;
}

/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(CFSTORE_OPEN_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("OPEN_test_00", cfstore_open_test_00),
        Case("OPEN_test_01_start", cfstore_utest_default_start),
        Case("OPEN_test_01_end", cfstore_open_test_01_end),
        Case("OPEN_test_02_start", cfstore_utest_default_start),
        Case("OPEN_test_02_end", cfstore_open_test_02_end),
        Case("OPEN_test_03_start", cfstore_utest_default_start),
        Case("OPEN_test_03_end", cfstore_open_test_03_end),
        Case("OPEN_test_04_start", cfstore_utest_default_start),
        Case("OPEN_test_04_end", cfstore_open_test_04_end),
        Case("OPEN_test_05_start", cfstore_utest_default_start),
        Case("OPEN_test_05_end", cfstore_open_test_05_end),
        Case("OPEN_test_06_start", cfstore_utest_default_start),
        Case("OPEN_test_06_end", cfstore_open_test_06_end),
        Case("OPEN_test_07_start", cfstore_utest_default_start),
        Case("OPEN_test_07_end", cfstore_open_test_07_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
