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

/** @file close.cpp Test cases to close KVs in the CFSTORE using the drv->Close() API function.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "cfstore_config.h"
#include "Driver_Common.h"
#include "configuration_store.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
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

static char cfstore_close_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.close.box1");
UVISOR_BOX_CONFIG(cfstore_close_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

/* KV data for test_01 */
static cfstore_kv_data_t cfstore_close_test_01_kv_data[] = {
        { "yotta.hello-world.animal{wobbly-dog}{foot}frontLeft", "first_data_"},
        { "yotta.hello-world.animal{wobbly-dog}{foot}frontLeft", "second_data"},
        { NULL, NULL},
};


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_close_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);
    return CaseNext;
}


/** @brief
 *
 * The is a basic test case which does the following:
 * - 01. create a key with handle hkey1
 * - 02. write data of hkey1
 * - 03. opens KV with 2nd handle, hkey2
 * - 04. read data with hkey2 and make sure its the same as that written with hkey1
 * - 05. write new data with hkey2
 * - 06. delete hkey2
 * - 07. close hkey2
 * - 08. read hkey1 and make sure the data is the newly written data i.e. the key hasnt
 *       been deleted yet
 * - 09. try to open KV and make sure unable to do so, as KV is deleting
 * - 10. close hkey1
 * - 11. try to open KV and make sure unable to do so because its now been deleted
 * - 12. create new KV with same key_name to make sure can re-create the key again.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_close_test_01_end(const size_t call_count)
{
    char read_buf[CFSTORE_KEY_NAME_MAX_LENGTH];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey1);
    ARM_CFSTORE_HANDLE_INIT(hkey2);
    cfstore_kv_data_t *node;
    ARM_CFSTORE_FMODE flags;

    CFSTORE_DBGLOG("%s:entered\n", __func__);
    (void) call_count;
    node = &cfstore_close_test_01_kv_data[0];
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));
    memset(read_buf, 0, CFSTORE_KEY_NAME_MAX_LENGTH);

    /* step 01 */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    CFSTORE_DBGLOG("%s:About to create new node (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    ret = drv->Create(node->key_name, strlen(node->value), &kdesc, hkey1);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);
    CFSTORE_DBGLOG("%s:length of KV=%d (key_name=\"%s\", value=\"%s\")\n", __func__, (int) len, node->key_name, node->value);

    /* step 02 */
    len = strlen(node->value);
    ret = drv->Write(hkey1, (char*) node->value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write full value data (key_name=\"%s\", value=\"%s\"), len=%d, (ret=%d)\n", __func__, node->key_name, node->value, (int) len, (int) ret);
    TEST_ASSERT_MESSAGE(len == strlen(node->value), cfstore_close_utest_msg_g);
    /* revert to CFSTORE_LOG if more trace required */
    CFSTORE_DBGLOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);

    /* step 03: Now open second handle while keeping first handle open */
    flags.read = true;
    flags.write = true;
    ret = drv->Open(node->key_name, flags, hkey2);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);
    len = strlen(node->value) + 1;
    /* revert to CFSTORE_LOG if more trace required */
    CFSTORE_DBGLOG("Opened KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);

    /* step 04 */
    ret = drv->Read(hkey2, read_buf, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read key (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    /* check read data is as expected */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: read value data (%s) != KV value data (key_name=\"%s\", value=\"%s\")\n", __func__, read_buf, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, cfstore_close_utest_msg_g);

    /* step 05 write new data using hkey2 */
    node = &cfstore_close_test_01_kv_data[1];
    len = strlen(node->value);
    ret = drv->Write(hkey2, (char*) node->value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key with 2nd handle (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write full value data (key_name=\"%s\", value=\"%s\"), len=%d, (ret=%d)\n", __func__, node->key_name, node->value, (int) len, (int) ret);
    TEST_ASSERT_MESSAGE(len == strlen(node->value), cfstore_close_utest_msg_g);
    /* revert to CFSTORE_LOG if more trace required */
    CFSTORE_DBGLOG("Wrote KV successfully with new data (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);

    /* step 06 delete hkey2 */
    ret = drv->Delete(hkey2);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to Delete KV with 2nd handle (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    /* step 07 close hkey2 */
    ret = drv->Close(hkey2);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to Close KV with 2nd handle (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    /* step 08 read hkey1 for updated data */
    len = strlen(node->value) + 1;
    memset(read_buf, 0, len);
    ret = drv->Read(hkey1, read_buf, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read key with hkey1 (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    /* check read data is as expected */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: read value data (%s) != KV value data (key_name=\"%s\", value=\"%s\")\n", __func__, read_buf, node->key_name, node->value);
    TEST_ASSERT_MESSAGE(strncmp(read_buf, node->value, strlen(node->value)) == 0, cfstore_close_utest_msg_g);

    /* step 09  */
    ret = drv->Open(node->key_name, flags, hkey2);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Should not be able to open pre-existing key in deleting state (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY_DELETING || ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_close_utest_msg_g);

    /* step 10 close hkey1 */
    ret = drv->Close(hkey1);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to Close KV with 1st handle (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    /* step 11 try to open KV and make sure unable to do so because its now been deleted */
    ret = drv->Open(node->key_name, flags, hkey1);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Should not be able to open key as it should not be present in the CFSTORE (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_close_utest_msg_g);

    /* step 12. create new KV with same key_name to make sure can re-create the key again. */
    node = &cfstore_close_test_01_kv_data[0];
    len = strlen(node->value);
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    CFSTORE_DBGLOG("%s:About to create new node (key_name=\"%s\", value=\"%s\")\n", __func__, node->key_name, node->value);
    ret = drv->Create(node->key_name, strlen(node->value), &kdesc, hkey1);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);
    CFSTORE_DBGLOG("%s:length of KV=%d (key_name=\"%s\", value=\"%s\")\n", __func__, (int) len, node->key_name, node->value);

    len = strlen(node->value);
    ret = drv->Write(hkey1, (char*) node->value, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")(ret=%d)\n", __func__, node->key_name, node->value, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write full value data (key_name=\"%s\", value=\"%s\"), len=%d, (ret=%d)\n", __func__, node->key_name, node->value, (int) len, (int) ret);
    TEST_ASSERT_MESSAGE(len == strlen(node->value), cfstore_close_utest_msg_g);

    /* revert to CFSTORE_LOG if more trace required */
    CFSTORE_DBGLOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_close_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_close_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("CLOSE_test_00", cfstore_close_test_00),
        Case("CLOSE_test_01_start", cfstore_utest_default_start),
        Case("CLOSE_test_01_end", cfstore_close_test_01_end)
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond


