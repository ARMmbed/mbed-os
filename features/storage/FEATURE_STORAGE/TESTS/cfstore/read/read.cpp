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

/** @file read.cpp Test cases to read KVs in the CFSTORE using the drv->Read() API call.
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
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_read_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.read.box1");
UVISOR_BOX_CONFIG(cfstore_read_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/* KV data for test_01 */
static cfstore_kv_data_t cfstore_read_test_01_kv_data[] = {
        CFSTORE_INIT_1_TABLE_MID_NODE,
        { NULL, NULL},
};

/* report whether built/configured for flash sync or async mode */
static control_t cfstore_read_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);
    return CaseNext;
}


/* @brief check char at offset is as expected */
static int32_t cfstore_read_seek_test(ARM_CFSTORE_HANDLE hkey, uint32_t offset, const char expected)
{
    ARM_CFSTORE_SIZE len = 1;
    char read_buf[1];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    ret = drv->Rseek(hkey, offset);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:failed to Rseek() to desired offset (offset=%d) (ret=%d).\n", __func__, (int) offset, (int) ret);
        goto out0;
    }
    ret = drv->Read(hkey, read_buf, &len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:failed to Read() (offset=%d)(ret=%d).\n", __func__, (int) offset, (int) ret);
        goto out0;
    }
    if(read_buf[0] != expected){
        ret = ARM_DRIVER_ERROR;
        goto out0;
    }
out0:
    return ret;
}

/** @brief
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_read_test_01_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    cfstore_test_rw_data_entry_t *node;
    ARM_CFSTORE_FMODE flags;

    CFSTORE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));

    /* create a key for reading */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_read_test_01_kv_data[0].value);
    ret = cfstore_test_create(cfstore_read_test_01_kv_data[0].key_name, (char*) cfstore_read_test_01_kv_data[0].value, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);

    /* now open the newly created key */
    ret = drv->Open(cfstore_read_test_01_kv_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, cfstore_read_test_01_kv_data[0].key_name, cfstore_read_test_01_kv_data[0].value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);

    /* seek back and forth in key and check the characters are as expected */
    node = cfstore_test_rw_data_table;
    while(node->offset != CFSTORE_TEST_RW_TABLE_SENTINEL)
    {
        ret = cfstore_read_seek_test(hkey, node->offset, node->rw_char);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Rseek() to offset (%d) didn't read expected char (\'%c\') (ret=%d)\n", __func__, (int) node->offset, node->rw_char, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);
        node++;
    }
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_read_utest_msg_g);
    return CaseNext;
}


/** @brief
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_read_test_02_end(const size_t call_count)
{
    (void) call_count;
    /*todo: implement test */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_read_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_read_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(200, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("READ_test_00", cfstore_read_test_00),
        Case("READ_test_01_start", cfstore_utest_default_start),
        Case("READ_test_01_end", cfstore_read_test_01_end),
        Case("READ_test_02_start", cfstore_utest_default_start),
        Case("READ_test_02_end", cfstore_read_test_02_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
