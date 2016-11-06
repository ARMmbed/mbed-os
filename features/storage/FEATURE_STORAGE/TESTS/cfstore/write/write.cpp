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

/** @file write.cpp Test cases to write KVs in the CFSTORE using the drv->Write() API call.
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

static char cfstore_write_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.write.box1");
UVISOR_BOX_CONFIG(cfstore_write_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/*
 * write tests
 * cfstore_handle_t cfstore_write()
 */

/* KV data for test_01 */
static cfstore_kv_data_t cfstore_write_test_01_kv_data[] = {
        CFSTORE_INIT_1_TABLE_MID_NODE,
        { NULL, NULL},
};


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_write_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);
    return CaseNext;
}

/** @brief  test to write many times to an open KV to test data is appended
 *          sequentially to the end of the value blob
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_write_test_01_end(const size_t call_count)
{
    char read_buf[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint32_t i = 0;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(read_buf, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));

    /* create an empty KV of the required length */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_write_test_01_kv_data[0].value);
    ret = cfstore_test_create(cfstore_write_test_01_kv_data[0].key_name, "one", &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create new KV (key_name=%s, ret=%d).\n", __func__, cfstore_write_test_01_kv_data[0].key_name, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);

    /* now open the newly created key and write repeated to created the string */
    flags.write = true;
    ret = drv->Open(cfstore_write_test_01_kv_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, cfstore_write_test_01_kv_data[0].key_name, cfstore_write_test_01_kv_data[0].value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);

    for(i = 0; i < strlen(cfstore_write_test_01_kv_data[0].value); i++)
    {
        len = 1;
        ret = drv->Write(hkey, &cfstore_write_test_01_kv_data[0].value[i], &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Write failed for char (\'%c\') (ret=%d)\n", __func__, cfstore_write_test_01_kv_data[0].value[i], (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);
    }
    /* check that the value created in the key is as expected*/
    len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    ret = drv->Read(hkey, read_buf, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read failed (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);

    /* check node key_names are identical */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: KV value (%s) is not as expected (%s).\n", __func__, read_buf, cfstore_write_test_01_kv_data[0].value);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, cfstore_write_test_01_kv_data[0].value, strlen(cfstore_write_test_01_kv_data[0].value)) == 0, cfstore_write_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Close(hkey) >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);

    cfstore_test_delete_all();
    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);
    return CaseNext;
}

/** @brief  test to write with a NULL buffer, which should fail gracefully
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_write_test_02_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));

    /* create an empty KV of the required length */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_write_test_01_kv_data[0].value);
    ret = cfstore_test_create(cfstore_write_test_01_kv_data[0].key_name, NULL, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error KV creation should have failed due to null write buffer but something else happended (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_INVALID_WRITE_BUFFER, cfstore_write_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_write_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_write_utest_msg_g);
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
        Case("WRITE_test_00", cfstore_write_test_00),
        Case("WRITE_test_01_start", cfstore_utest_default_start),
        Case("WRITE_test_01_end", cfstore_write_test_01_end),
        Case("WRITE_test_02_start", cfstore_utest_default_start),
        Case("WRITE_test_02_end", cfstore_write_test_02_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
