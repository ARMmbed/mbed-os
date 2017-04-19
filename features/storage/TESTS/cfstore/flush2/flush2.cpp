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

/** @file flush2.cpp Test cases to flush KVs in the CFSTORE using the drv->Flush() interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "cfstore_config.h"
#include "configuration_store.h"
#include "greentea-client/test_env.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
#include "cfstore_utest.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

/// @cond CFSTORE_DOXYGEN_DISABLE
/*
 * Defines
 */
#define CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE                256
#define cfstore_flush_fsm_null                          NULL
#define CFSTORE_FLUSH_CASE_TIMEOUT_MS                   10000

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.flush2.box1");
UVISOR_BOX_CONFIG(cfstore_flush2_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/*
 * Globals
 *
 * cfstore_flush_utest_msg_g
 *  buffer for storing TEST_ASSERT_xxx_MESSAGE messages
 */
static char cfstore_flush_utest_msg_g[CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE];

/* KV data for test_03 */
static cfstore_kv_data_t cfstore_flush_test_02_kv_data[] = {
        { "com.arm.mbed.configurationstore.test.flush.cfstoreflushtest02", "1"},
        /*          1         2         3         4         5         6        7  */
        /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        { NULL, NULL},
};

/* async test version */

typedef struct cfstore_flush_ctx_t
{
    int32_t status;
    ARM_CFSTORE_OPCODE cmd_code;
} cfstore_flush_ctx_t;
/// @endcond

/*
 * Globals
 */
static cfstore_flush_ctx_t cfstore_flush_ctx_g;

/*
 * context related methods
 */

/* @brief   get a pointer to the global context data structure */
static cfstore_flush_ctx_t* cfstore_flush_ctx_get(void)
{
    return &cfstore_flush_ctx_g;
}

/* @brief   initialize global context data structure */
static void cfstore_flush_ctx_init(cfstore_flush_ctx_t* ctx)
{
    TEST_ASSERT_MESSAGE(ctx != NULL, "ctx is NULL");

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&cfstore_flush_ctx_g, 0, sizeof(cfstore_flush_ctx_g));
}


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_flush2_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    return CaseNext;
}

/**
 * @brief
 *
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t  cfstore_flush2_test_01_start(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_flush_ctx_t* ctx = cfstore_flush_ctx_get();
    const ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    (void) call_count;
    cfstore_flush_ctx_init(ctx);
    ret = drv->Initialize(cfstore_utest_default_callback, ctx);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%d)\r\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    return CaseTimeout(100000);
}


/**
 * @brief
 *
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_flush2_test_01_mid(const size_t call_count)
{

    bool bfound = false;
    int32_t ivalue = 0;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    const char* key_name_query = "*";
    char value[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    ARM_CFSTORE_SIZE len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    ARM_CFSTORE_HANDLE_INIT(next);
    ARM_CFSTORE_HANDLE_INIT(prev);
    ARM_CFSTORE_KEYDESC kdesc;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    (void) call_count;
    memset(&kdesc, 0, sizeof(kdesc));

    /* try to read key; should not be found */
    ret = cfstore_test_kv_is_found(cfstore_flush_test_02_kv_data->key_name, &bfound);
    if(ret != ARM_DRIVER_OK && ret != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_test_kv_is_found() call failed (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(false, cfstore_flush_utest_msg_g);
    }

    if(!bfound)
    {
        /* first time start up. nothing is stored in cfstore flash. check this is the case */
        while(drv->Find(key_name_query, prev, next) == ARM_DRIVER_OK)
        {
            CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: have found an entry in cfstore when none should be present", __func__);
            TEST_ASSERT_MESSAGE(false, cfstore_flush_utest_msg_g);
        }
        /* no entries found, which is correct.
         * store a value */
        len = strlen(cfstore_flush_test_02_kv_data->value);
        ret = cfstore_test_create(cfstore_flush_test_02_kv_data->key_name, cfstore_flush_test_02_kv_data->value, &len, &kdesc);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error:1: failed to write kv data (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
        /* flush to flash */
        ret = drv->Flush();
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to flush data to cfstore flash (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);

    } else {
        /*read the value, increment by 1 and write value back */
        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        ret = cfstore_test_read(cfstore_flush_test_02_kv_data->key_name, value, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read kv data (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);

        /* increment value */
        ivalue = atoi(value);
        ++ivalue;
        snprintf(value, CFSTORE_KEY_NAME_MAX_LENGTH+1, "%d", (int) ivalue);
        len = strlen(value);
        ret = cfstore_test_write(cfstore_flush_test_02_kv_data->key_name, value, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write kv data (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);

        /* flush to flash */
        ret = drv->Flush();
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to flush data to cfstore flash (ret=%d).\r\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    }
    return CaseTimeout(100000);
}

/**
 * @brief
 *
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t  cfstore_flush2_test_01_end(const size_t call_count)
{
    const ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    (void) call_count;
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() call failed.\n", __func__);
    TEST_ASSERT_MESSAGE(drv->Uninitialize() >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    return CaseNext;
}


/**
 * @brief   test to open(create) a very large value, write the data, close, then flush.
 *          for a N keys simultaneously.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
control_t cfstore_flush2_test_02(const size_t call_count)
{
    (void) call_count;
    //todo: implement test
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Warn: Not implemented\n", __func__);
    CFSTORE_DBGLOG("%s: WARN: requires implementation\n", __func__);
    TEST_ASSERT_MESSAGE(true, cfstore_flush_utest_msg_g);
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    // Call the default reporting function-
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
        Case("CFSTORE_FLUSH2_test_00", cfstore_flush2_test_00),
        Case("CFSTORE_FLUSH2_test_01_start", cfstore_flush2_test_01_start),
        Case("CFSTORE_FLUSH2_test_01_mid", cfstore_flush2_test_01_mid),
        Case("CFSTORE_FLUSH2_test_01_end", cfstore_flush2_test_01_end),
        Case("CFSTORE_FLUSH2_test_02_start", cfstore_utest_default_start),
        Case("CFSTORE_FLUSH2_test_02_end", cfstore_flush2_test_02),
};


// Declare your test specification with a custom setup handler
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
