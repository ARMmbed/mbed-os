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
 *
 *
 */

/** @file find.cpp Test cases to find KVs in the CFSTORE using the drv->Find() interface.
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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_find_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.find.box1");
UVISOR_BOX_CONFIG(cfstore_find_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_find_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    return CaseNext;
}

/** @brief  test to call cfstore_find() with a key_name string that exceeds
 *          the maximum length
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_01(const size_t call_count)
{
    (void) call_count;
    /*todo: implement test */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_find_utest_msg_g);
    return CaseNext;
}


/** @brief	test to call cfstore_find() with key_name that in includes
 *         	illegal characters
 *          - the character can be at the beginning of the key_name
 *          - the character can be at the end of the key_name
 *          - the character can be somewhere within the key_name string
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_02(const size_t call_count)
{
    /*todo: implement test
     *
     * specify the allowable characters in a set.
     * e.g. "0123456789ABCDEFGHIJKLMNOPQRSTUVQXYZabcdefghijklmnopqrstuvwxyz.[]*"
     * and use this set as the sell of allowable character codes.
     * All other 0-255 values for character codes should not be found in the key_name string
     * but the function should be tested for what happens in that case.
     *
     * Some code may be common here with other functions requiring a key_name e.g. cfstore_find(
     */
    (void) call_count;
    /*todo: implement test */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_find_utest_msg_g);
    return CaseNext;
}


/** @brief  test to call cfstore_find() with key_name that in includes
 *          illegal characters
 *          - the character can be at the beginning of the key_name
 *          - the character can be at the end of the key_name
 *          - the character can be somewhere within the key_name string
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_03_end(const size_t call_count)
{
    char* read_buf = NULL;
    const uint8_t key_name_max_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    char key_name_buf[key_name_max_len];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_SIZE max_len = 0;
    cfstore_kv_data_t* node;
    cfstore_kv_data_t* client_node = cfstore_test_init_1_data;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(prev);
    ARM_CFSTORE_HANDLE_INIT(next);

    CFSTORE_DBGLOG("%s:entered\r\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    memset(key_name_buf, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);

    /*scan for max length of value blob*/
    node = client_node;
    while(node->key_name != NULL)
    {
        len = strlen(node->value);
        if(len > max_len){
            max_len = len;
        }
        node++;
    }
    max_len++;  /* space for a terminating null, if required */
    read_buf = (char*) malloc(max_len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to allocated read buffer \r\n", __func__);
    TEST_ASSERT_MESSAGE(read_buf != NULL, cfstore_find_utest_msg_g);

    ret = cfstore_test_init_1();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialise cfstore area with entries\r\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

    /* now find and read back the key values  */
    ret = ARM_DRIVER_ERROR;
    node = client_node;
    while(node->key_name != NULL)
    {
        CFSTORE_DBGLOG("%s:About to find node (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        ret = drv->Find(node->key_name, prev, next);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to find node (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Find failed to return valid key handle\r\n", __func__);
        TEST_ASSERT_MESSAGE(next != NULL, cfstore_find_utest_msg_g);

        ret = drv->GetValueLen(next, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

        if(len > 0) {
            ret = drv->Read(next, read_buf, &len);
            CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read value (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
            TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

            /* check read data is as expected */
            CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read value data (%s) != KV value data (key_name=\"%s\", value=\"%s\")\r\n", __func__, read_buf, node->key_name, node->value);
            TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, cfstore_find_utest_msg_g);

        }
        read_buf[len] = '\0';
        /* revert to CFSTORE_LOG if more trace required */
        CFSTORE_DBGLOG("Successfully found KV and read value data (key_name=\"%s\", value=\"%s\")\r\n", node->key_name, read_buf);
        memset(read_buf, 0, len);
        drv->Close(next);
        node++;
    }

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    return CaseNext;
}


/** @brief  TODO: write test that uses cfstore_find_test_04_kv_data to grow {key, value}
 *          from 1 char to 221 chars long.
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
/*
 * use this data:
 *  static cfstore_kv_data_t cfstore_find_test_04_kv_data[] = {
 *        { "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef", "abcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyzabcdefghjklmnopqrstuvwxyz"},
 *        { NULL, NULL},
 * };
 *
 */
control_t cfstore_find_test_04(const size_t call_count)
{
    /*todo: implement test
     *
     * */
    (void) call_count;
    /*todo: implement test */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_find_utest_msg_g);
    return CaseNext;
}


/**
 * @brief   function to test whether a key name is valid, can be added to the
 * 			cfstore and found by a wildcard string
 *
 * @param	key_name
 * 			name of the key to create in the store
 * @param	match
 * 			string to use to try and find key_name in the cfstore
 * @param	should_find
 * 			if true, then 'match' should find 'key_name' in the store
 * 			if false, then 'match' should not find 'key_name' in the store
 *
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret < ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
static bool cfstore_find_key_name_validate(const char *key_name, const char *match, bool should_find)
{
    bool bret = true;
    bool btest_status = false;
    char* test_data = (char*) "test_data";
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_KEYDESC kdesc;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&kdesc, 0, sizeof(kdesc));

    /* create  */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(test_data);
    ret = cfstore_test_create((const char*) key_name, test_data, &len, &kdesc);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to create kv (key_name=%s.\r\n", "cfstore_find_test_05_ex", key_name);
        return ret;
    }
    ret = cfstore_test_kv_is_found(match, &bret);
    if(ret < ARM_DRIVER_OK && ret != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
        CFSTORE_ERRLOG("%s:Error: cfstore_test_kv_is_found() failed.\r\n", "cfstore_find_test_05_ex");
        return ret;
    }
    /* dont not use any functions that require finding the created item as they may not work,
     * depending on the construction of the test key_name & match strings */
    if(should_find == bret)
    {
        CFSTORE_DBGLOG("%s:Success: Find() behaved as expected.\r\n", "cfstore_find_test_05_ex");
        btest_status = true;
    }
    else
    {
        CFSTORE_ERRLOG("cfstore_find_test_05_ex: Failed match on %s vs. %s\n",  key_name, match);
    }
    /*delete using the actual name */
    ret = cfstore_test_delete((const char*) key_name);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to delete kv (key_name=%s)(ret=%d).\r\n", "cfstore_find_test_05_ex", key_name, (int)ret);
    }
    return btest_status;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
typedef struct cfstore_find_key_name_validate_t {
    const char* key_name;
    const char* match;
    uint32_t f_allowed : 1;
} cfstore_find_key_name_validate_t;

cfstore_find_key_name_validate_t cfstore_find_test_05_data[] = {
        { "yotta.hello-world.animal{wobbly-dog}{foot}backLeft", "yotta.hello-world.animal{*}{foot}backLeft", true},
        { "yotta.hello-world.animal{wobbly-dog}{foot}backLeft", "yotta.hello-world.animal{wobbly-dog}{*}backLeft", true},
        { "yotta.hello-world.animal{wobbly-dog}{foot}backLeft", "yotta.hello-world.animal{wobbly-dog}{*}*", true},
        { "yotta.hello-world.animal{1}{foot}backLeft", "yotta.hello-world.animal{?}{foot}backLeft", false},
        { "xyz", "xyz", true},
        { "xyzijkXYZ", "XYZ", false},
        { "xyzijkXYZ", "*XYZ", true},
        { "xyzijkXYZ", "xyz*XYZ", true},
        { "xyzijkXYZ", "*yz*XYZ", true},
        { "xyzijkXYZ", "*ijk*", true},
        { NULL, NULL, false},
};
/// @endcond


/** @brief  test whether a key name in the above table are valid, can be added to the
 *          cfstore and found by a wildcard string
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_05_end(const size_t call_count)
{
    bool ret = false;
    int32_t ret32 = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    cfstore_find_key_name_validate_t* node = cfstore_find_test_05_data;

    (void) call_count;

    while(node->key_name != NULL)
    {
        ret = cfstore_find_key_name_validate(node->key_name, node->match, node->f_allowed);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: test failed (ret=%d)\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret == true, cfstore_find_utest_msg_g);
        node++;
    }

    ret32 = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret32 >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
#define CFSTORE_FIND_TEST_06_ENTRY_MATCH_03      { "0123456789abcdef0123456.yxxx.3", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_MATCH_05      { "0123456789abcdef0123456.yxxx.5", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_MATCH_07      { "0123456789abcdef0123456.yxxx.7", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_MATCH_09      { "0123456789abcdef0123456.yxxx.9", "abcdefghijklmnopqrstuvwxyz"}

#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_01      { "0123456789abcdef0123456.xxxx.1", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_02      { "0123456789abcdef0123456.xxxx.2", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_04      { "0123456789abcdef0123456.xxxx.4", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_06      { "0123456789abcdef0123456.xxxx.6", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_08      { "0123456789abcdef0123456.xxxx.8", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_10      { "0123456789abcdef0123456.xxxx.10", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_11      { "0123456789abcdef0123456.xxxx.11", "abcdefghijklmnopqrstuvwxyz"}

/* table 1: to initialise cfstore with CFSTORE_CREATE_TEST_01_TABLE_MID_ENTRY_01 */
static cfstore_kv_data_t cfstore_find_test_06_data[] = {
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_01,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_02,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_03,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_04,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_05,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_06,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_07,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_08,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_09,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_10,
        CFSTORE_FIND_TEST_06_ENTRY_NOMATCH_11,
        { NULL, NULL},
};

static cfstore_kv_data_t cfstore_find_test_06_data_match_results[] = {
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_03,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_05,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_07,
        CFSTORE_FIND_TEST_06_ENTRY_MATCH_09,
        { NULL, NULL},
};
/// @endcond


/**
 * @brief   test to use find to find at least 2 entries in the cfstore,
 *          but the query string doesnt match the last 2 entries in the
 *          store.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_06_end(const size_t call_count)
{
    const char* key_name_query = "0123456789abcdef0123456.y*";
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t find_count = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(next);
    ARM_CFSTORE_HANDLE_INIT(prev);
    cfstore_kv_data_t* node = NULL;

    ret = cfstore_test_create_table(cfstore_find_test_06_data);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to add cfstore_find_test_06_data table data (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

    while((ret = drv->Find(key_name_query, prev, next)) == ARM_DRIVER_OK)
    {
        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        ret = drv->GetKeyName(next, key_name, &len);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("Error: failed to get key name%s", "\n");
            break;
        }
        CFSTORE_LOG("%s:Found entry key_name=%s\n", __func__, key_name);
        node = cfstore_find_test_06_data_match_results;
        while(node->key_name != NULL){
            if(strncmp(node->key_name, key_name, CFSTORE_KEY_NAME_MAX_LENGTH) == 0){
                find_count++;
                break;
            }
            node++;
        }
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to find match in match table for %s.\n", __func__, key_name);
        TEST_ASSERT_MESSAGE(node->key_name != NULL, cfstore_find_utest_msg_g);

        CFSTORE_HANDLE_SWAP(prev, next);
    }

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: find_count=%d doesnt match the number of entries in match table = %d.\n", __func__, (int) find_count, (int) (sizeof(cfstore_find_test_06_data_match_results)/sizeof(cfstore_kv_data_t))-1);
    TEST_ASSERT_MESSAGE(find_count == (sizeof(cfstore_find_test_06_data_match_results)/sizeof(cfstore_kv_data_t))-1, cfstore_find_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: expected ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, but ret = %d.\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_find_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    return CaseNext;
}

/**
 * @brief   test case to check Find() with previous NULL pointer works
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_find_test_07_end(const size_t call_count)
{
    const char* key_name_query = "0123456789abcdef0123456.y*";
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t find_count = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(next);
    cfstore_kv_data_t* node = NULL;

    ret = cfstore_test_create_table(cfstore_find_test_06_data);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to add cfstore_find_test_06_data table data (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

    while(true)
    {

        ret = drv->Find(key_name_query, NULL, next);
        if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
            /* no more attributes found matching search criteria.*/
           break;
        }

        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Find() failed(ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        ret = drv->GetKeyName(next, key_name, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to get key name for next (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

        CFSTORE_LOG("%s:Found entry key_name=%s\n", __func__, key_name);
        node = cfstore_find_test_06_data_match_results;
        while(node->key_name != NULL){
            if(strncmp(node->key_name, key_name, CFSTORE_KEY_NAME_MAX_LENGTH) == 0){
                find_count++;
                break;
            }
            node++;
        }
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to find match in match table for %s.\n", __func__, key_name);
        TEST_ASSERT_MESSAGE(node->key_name != NULL, cfstore_find_utest_msg_g);

        /* delete the KV so it wont be found when queried is repeated*/
        ret = drv->Delete(next);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Delete() on next handled failed(ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);

        ret = drv->Close(next);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() on next handled failed(ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    }

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: find_count=%d doesnt match the number of entries in match table = %d.\n", __func__, (int) find_count, (int) (sizeof(cfstore_find_test_06_data_match_results)/sizeof(cfstore_kv_data_t))-1);
    TEST_ASSERT_MESSAGE(find_count == (sizeof(cfstore_find_test_06_data_match_results)/sizeof(cfstore_kv_data_t))-1, cfstore_find_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: expected ret == ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, but ret = %d.\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_find_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_find_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_find_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(400, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FIND_test_00", cfstore_find_test_00),
        Case("FIND_test_01", cfstore_find_test_01),
        Case("FIND_test_02", cfstore_find_test_02),
        Case("FIND_test_03_start", cfstore_utest_default_start),
        Case("FIND_test_03_end", cfstore_find_test_03_end),
        Case("FIND_test_04", cfstore_find_test_04),
        Case("FIND_test_05_start", cfstore_utest_default_start),
        Case("FIND_test_05_end", cfstore_find_test_05_end),
        Case("FIND_test_06_start", cfstore_utest_default_start),
        Case("FIND_test_06_end", cfstore_find_test_06_end),
        Case("FIND_test_07_start", cfstore_utest_default_start),
        Case("FIND_test_07_end", cfstore_find_test_07_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond

