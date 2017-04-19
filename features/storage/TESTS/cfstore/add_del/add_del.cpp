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
 */

/** @file add_del.cpp Test cases to add and delete key-value pairs in the CFSTORE.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "cfstore_config.h"
#include "Driver_Common.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include "configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "cfstore_utest.h"

#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_add_del_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* Create the main box ACL list for the application.
 * The main ACL gets inherited by all the other boxes
 */
CFSTORE_UVISOR_MAIN_ACL(cfstore_acl_uvisor_box_add_del_g);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, cfstore_acl_uvisor_box_add_del_g);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

static cfstore_kv_data_t cfstore_add_del_test_07_data[] = {
        CFSTORE_INIT_1_TABLE_MID_NODE,
        { NULL, NULL},
};


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_add_del_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
    return CaseNext;
}

/** @brief
 *
 * This test case does the following:
 * - creates a KV.
 * - deletes the KV.
 * - checks that the deleted KV can no longer be found in the store.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_add_del_test_01_end(const size_t call_count)
{
    bool bfound = false;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));
    memset(&flags, 0, sizeof(flags));

    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    len = strlen(cfstore_add_del_test_07_data[0].value);

    ret = cfstore_test_create(cfstore_add_del_test_07_data[0].key_name, (char*) cfstore_add_del_test_07_data[0].value, &len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create KV in store (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);

    /* now delete KV*/
    ret = drv->Open(cfstore_add_del_test_07_data[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to Open() (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);

    if(hkey != NULL){
        ret = drv->Delete(hkey);
        drv->Close(hkey);
        hkey = NULL;
    }
    /* check that the KV has been deleted */
    /* revert to CFSTORE_LOG if more trace required */
    CFSTORE_DBGLOG("LOG: WARNING: About to look for non-existent key (key_name=%s) (which will generate internal trace reporting errors if debug trace enabled).\n", cfstore_add_del_test_07_data[0].key_name);
    ret = cfstore_test_kv_is_found(cfstore_add_del_test_07_data[0].key_name, &bfound);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error failed to delete a key (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_add_del_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Test failed: found KV that was previously deleted (key_name=%s)\n", __func__, cfstore_add_del_test_07_data[0].key_name);
    TEST_ASSERT_MESSAGE(bfound == false, cfstore_add_del_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
    return CaseNext;
}


static cfstore_kv_data_t cfstore_add_del_test_08_data[] = {
        CFSTORE_INIT_1_TABLE_HEAD,
        CFSTORE_INIT_1_TABLE_MID_NODE,
        CFSTORE_INIT_1_TABLE_TAIL,
        { NULL, NULL},
};


/** @brief
 *
 * This test case adds a small number of KVs (~3), and then delete them.
 * - add key(s)
 * - delete key(s)
 * - make sure can't find key in cfstore
 * - loop over the above a number of times.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_add_del_test_02_end(const size_t call_count)
{
    bool bResult = true;   // We'll do "&=" cumulative checking.
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_KEYDESC kdesc;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));

    /* create  */
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    node = cfstore_add_del_test_08_data;
    while(node->key_name != NULL)
    {
        len = strlen(node->value);
        ret = cfstore_test_create(node->key_name, (char*) node->value, &len, &kdesc);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to create kv (key_name=%s.\n", __func__, node->key_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
        /* revert CFSTORE_LOG for more trace */
        CFSTORE_DBGLOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\n", node->key_name, node->value);
        node++;
    }

    /* test delete all */
    ret = cfstore_test_delete_all();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to delete all KVs.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);

    /* check there are no KVs present as expected */
    node = cfstore_add_del_test_08_data;
    while(node->key_name != NULL)
    {
        ret = cfstore_test_kv_is_found(node->key_name, &bResult);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: found key when should not be present.\n", __func__);
        TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND && bResult == false, cfstore_add_del_utest_msg_g);
        /* revert CFSTORE_LOG for more trace */
        CFSTORE_DBGLOG("Found KV successfully (key_name=\"%s\")\n", node->key_name);
        node++;
    }
    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
    return CaseNext;
}

/** @brief
 *
 * This test case adds ~50 KVs, and then delete entries at the start,
 * middle and end of list.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_add_del_test_03_end(const size_t call_count)
{
    bool bfound = false;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_FMODE flags;
    cfstore_kv_data_t *node;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&flags, 0, sizeof(flags));

    ret = cfstore_test_init_1();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialise cfstore area with entries\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);

    /* delete some keys */
    node = cfstore_add_del_test_08_data;
    while(node->key_name != NULL)
    {
        CFSTORE_DBGLOG("%s:about to delete key (key_name=%s).\n", __func__, node->key_name);
        cfstore_test_delete(node->key_name);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error failed to delete a key (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
        /* revert CFSTORE_LOG for more trace */
        CFSTORE_DBGLOG("Deleted KV successfully (key_name=\"%s\")\n", node->key_name);
        node++;
    }
    /* check the keys have been deleted */
    node = cfstore_add_del_test_08_data;
    while(node->key_name != NULL)
    {
        ret = cfstore_test_kv_is_found(node->key_name, &bfound);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to delete a key (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, cfstore_add_del_utest_msg_g);

        CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Test failed: found KV that was previously deleted (key_name=%s)\n", __func__, node->key_name);
        TEST_ASSERT_MESSAGE(bfound == false, cfstore_add_del_utest_msg_g);
        node++;
    }

    /* clean up by deleting all remaining KVs. this is not part of the test */
    ret = cfstore_test_delete_all();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error failed to delete a all KVs (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_add_del_utest_msg_g);
    return CaseNext;
}


/** @brief
 *
 * This test case is as per test_03 but using delete_all() on all init_1 data.
 * This test case is yet to be implemented.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_add_del_test_04(const size_t call_count)
{
    (void) call_count;
    /*todo: implement test */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_add_del_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_add_del_utest_msg_g);
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
        Case("ADD_DEL_test_00", cfstore_add_del_test_00),
        Case("ADD_DEL_test_01_start", cfstore_utest_default_start),
        Case("ADD_DEL_test_01_end", cfstore_add_del_test_01_end),
        Case("ADD_DEL_test_02_start", cfstore_utest_default_start),
        Case("ADD_DEL_test_02_end", cfstore_add_del_test_02_end),
        Case("ADD_DEL_test_03_start", cfstore_utest_default_start),
        Case("ADD_DEL_test_03_end", cfstore_add_del_test_03_end),
        Case("ADD_DEL_test_04", cfstore_add_del_test_04),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
