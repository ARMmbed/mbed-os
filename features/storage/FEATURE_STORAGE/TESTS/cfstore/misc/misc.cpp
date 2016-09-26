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

/** @file misc.cpp Test cases for miscellaneous API drv->Xxx() functions.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "cfstore_config.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
#include "Driver_Common.h"
#include "cfstore_config.h"
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
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_misc_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* Create the main box ACL list for the application.
 * The main ACL gets inherited by all the other boxes
 */
CFSTORE_UVISOR_MAIN_ACL(cfstore_acl_uvisor_box_misc_g);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, cfstore_acl_uvisor_box_misc_g);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_misc_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseNext;
}


/** @brief  basic PowerControl() test
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_misc_test_00_start(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_POWER_STATE state = ARM_POWER_OFF;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    /* try setting the power control state before initialised, which should fail */
    ret = drv->PowerControl(state);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Eror: PowerControl() call should have failed as CFSTORE not initialised, but the call succeeded\r\n", __func__);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

    ret = drv->Initialize(cfstore_utest_default_callback, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseTimeout(CFSTORE_UTEST_DEFAULT_TIMEOUT_MS);
}

static control_t cfstore_misc_test_00_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_POWER_STATE state = ARM_POWER_OFF;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    while(state <= ARM_POWER_FULL)
    {
        ret = drv->PowerControl(state);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: PowerControl() call failed\r\n", __func__);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
        state = (ARM_POWER_STATE)((uint32_t) state + 1);
    }
    /*try invalid value which should fail*/
    ret = drv->PowerControl((ARM_POWER_STATE ) 0xffffffff);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:ERror: PowerControl() did not fail with bad value 0xffffffff (not as expected)\r\n", __func__);
    TEST_ASSERT_MESSAGE(ret < ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseNext;
}


/** @brief  basic GetVersion() test
 *
 */
control_t cfstore_misc_test_01(const size_t call_count)
{
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_DRIVER_VERSION version;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    memset(&version, 0, sizeof(version));

    version = drv->GetVersion();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:GetVersion() failed to return correct API version.\r\n", __func__);
    TEST_ASSERT_MESSAGE(version.api == ARM_CFSTORE_API_VERSION, cfstore_misc_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:GetVersion() to return correct driver version.\r\n", __func__);
    TEST_ASSERT_MESSAGE(version.drv == ARM_CFSTORE_DRV_VERSION, cfstore_misc_utest_msg_g);
    return CaseNext;
}


/* KV data for test_03 */
static cfstore_kv_data_t cfstore_misc_test_03_kv_data[] = {
        /*          1         2         3         4         5         6        7  */
        /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        { "The.principles.of.least.action.in.quantum.mechanics", "DoctoralThesis"},
        { "Space.Time.Approach.to.Quantum.Electrodynamic", " PhysicalReview766)"},
        { "An.Operator.Calculus.Having.Applications.in.Quantum.Electrodynamics", "PhysicalReview84)"},
        { NULL, NULL},
};


/** @brief  basic GetKeyName() test
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_misc_test_02_end(const size_t call_count)
{
    uint8_t key_name_len = 0;
    char key_name_buf[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    (void) call_count;
    memset(key_name_buf, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));

    ret = cfstore_test_create_table(cfstore_misc_test_03_kv_data);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to create keys from table.\r\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

    node = cfstore_misc_test_03_kv_data;
    while(node->key_name != NULL)
    {
        CFSTORE_DBGLOG("%s:About to open KV (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        ret = drv->Open(node->key_name, flags, hkey);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\r\n", __func__, node->key_name, node->value, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

        key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        memset(key_name_buf, 0, key_name_len);
        drv->GetKeyName(hkey, key_name_buf, &key_name_len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to GetKeyName() (key_name (expected)=\"%s\", key_name (returned)=\"%s\", value=\"%s\"), len return=%d, len expected=%d\r\n", __func__, node->key_name, key_name_buf, node->value, (int) key_name_len, (int) strlen(node->key_name));
        TEST_ASSERT_MESSAGE(key_name_len == strlen(node->key_name)+1, cfstore_misc_utest_msg_g);

        /* revert to CFSTORE_LOG if more trace required */
        CFSTORE_DBGLOG("GetKeyName() successfully reported key_name (key_name=\"%s\")\r\n", key_name_buf);
        ret = drv->Close(hkey);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to close key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
        node++;
    }

    cfstore_test_delete_all();
    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseNext;
}


/** @brief  basic GetValueLen() test
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_misc_test_03_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;
    /* dont set any flags to get default settings */
    memset(&flags, 0, sizeof(flags));

    ret = cfstore_test_create_table(cfstore_misc_test_03_kv_data);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: unable to create keys from table.\r\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

    node = cfstore_misc_test_03_kv_data;
    while(node->key_name != NULL)
    {
        CFSTORE_DBGLOG("%s:About to open KV (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        ret = drv->Open(node->key_name, flags, hkey);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\r\n", __func__, node->key_name, node->value, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);

        drv->GetValueLen(hkey, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Errro: GetValueLen() retrieve incorrect length of value blob(expected=%d, returned=%d)\r\n", __func__, (int) strlen(node->value), (int) len);
        TEST_ASSERT_MESSAGE(len == strlen(node->value), cfstore_misc_utest_msg_g);
        /* revert to CFSTORE_LOG if more trace required */
        CFSTORE_DBGLOG("GetValueLen() successfully reported correct value blob length%s", "\r\n");

        ret = drv->Close(hkey);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Failed to close key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
        node++;
    }
    cfstore_test_delete_all();
    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseNext;
}


/** @brief  basic GetStatus() test
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_misc_test_04_start(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_STATUS status;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    status = drv->GetStatus();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: GetStatus() before initialisation should have reported error, but reported no error.\r\n", __func__);
    TEST_ASSERT_MESSAGE(status.error == 1, cfstore_misc_utest_msg_g);

    ret = drv->Initialize(cfstore_utest_default_callback, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
    return CaseTimeout(CFSTORE_UTEST_DEFAULT_TIMEOUT_MS);
}

/** @brief  basic GetStatus() test
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_misc_test_04_end(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_STATUS status;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) call_count;

    status = drv->GetStatus();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: GetStatus() but reported error.\r\n", __func__);
    TEST_ASSERT_MESSAGE(status.error == 0, cfstore_misc_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: GetStatus() reported operation in progress.\r\n", __func__);
    TEST_ASSERT_MESSAGE(status.in_progress == 0, cfstore_misc_utest_msg_g);

    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_misc_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_misc_utest_msg_g);
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
        Case("MISC_test_00", cfstore_misc_test_00),
        Case("MISC_test_00_start", cfstore_misc_test_00_start),
        Case("MISC_test_00_end", cfstore_misc_test_00_end),
        Case("MISC_test_01", cfstore_misc_test_01),
        Case("MISC_test_02_start", cfstore_utest_default_start),
        Case("MISC_test_02_end", cfstore_misc_test_02_end),
        Case("MISC_test_03_start", cfstore_utest_default_start),
        Case("MISC_test_03_end", cfstore_misc_test_03_end),
        Case("MISC_test_04_start", cfstore_misc_test_04_start),
        Case("MISC_test_05_end", cfstore_misc_test_04_end),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
