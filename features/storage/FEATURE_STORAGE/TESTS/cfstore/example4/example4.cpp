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

/** @file example4.cpp Test case to demonstrate a subset of the API functions each work correctly.
 *
 * Test case created from Issue 10 code supplied by Motti Gondabi. The code:
 * - creates a KV
 * - writes the KV
 * - closes the KV
 * - flushes the KV
 * - opens the KV.
 * - deletes the KV.
 * - flushes empty configuration store.
 *
 * The test case makes sure that the implementation can flush an empty configuration store
 * without causing errors. This has only been possible since flash-journal-strategy-sequential
 * v0.4.0.
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
#ifdef YOTTA_CFG_CONFIG_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CONFIG_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

using namespace utest::v1;

static char cfstore_example4_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* defines */
/// @cond CFSTORE_DOXYGEN_DISABLE
#define PvMemSet memset
#define PvStrLen strlen
#define PvKeyValue_t cfstore_kv_data_t

ARM_CFSTORE_DRIVER *gCfStoreDriver = &cfstore_driver;
/// @endcond

static control_t cfstore_example4_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_CAPABILITIES caps;;

    (void) call_count;

    /* initialise the context */
    caps = gCfStoreDriver->GetCapabilities();
    CFSTORE_LOG("%s:INITIALIZING: caps.asynchronous_ops=%lu\n", __func__, caps.asynchronous_ops);
    if(caps.asynchronous_ops == 1){
    	/* This is a sync mode only test. If this test is not built for sync mode, then skip testing return true
    	 * This means the test will conveniently pass when run in CI as part of async mode testing */
        CFSTORE_LOG("*** Skipping test as binary built for flash journal async mode, and this test is sync-only%s", "\n");
        return CaseNext;
    }
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);
    return CaseNext;
}


/* used for sync mode build only */
#if defined STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS && STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS==0

static const PvKeyValue_t testDataKeyValue[] = {
    { "com.arm.mbed.spv.assets.dtls", "This Is my DTLS Secret" },
    { "com.arm.mbed.spv.assets.asset1.payload", "The Rolling Stone" },
    { "com.arm.mbed.spv.assets.asset2.payload", "Grumpy old man" },
    { "com.arm.mbed.spv.assets.asset3.payload", "Delete this asset payload" },
};


static int32_t CreateKeyValueStore(
	const char *keyName,
	const char *data,
	ARM_CFSTORE_SIZE *dataLength,
	ARM_CFSTORE_KEYDESC *keyDesc)
{
	int32_t cfsStatus = ARM_DRIVER_ERROR;
	ARM_CFSTORE_SIZE valueLength = 0;
	ARM_CFSTORE_HANDLE_INIT(hkey);

	valueLength = *dataLength;
	cfsStatus = gCfStoreDriver->Create(keyName, valueLength, keyDesc, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Create() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	valueLength = *dataLength;
	cfsStatus = gCfStoreDriver->Write(hkey, data, &valueLength);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Write() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: valueLength != *dataLength\n", __func__);
    TEST_ASSERT_MESSAGE(valueLength == *dataLength, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Close(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Flush();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Flush() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

    return ARM_DRIVER_OK;
}


static control_t cfstore_example4_test_01(const size_t call_count)
{
	int32_t cfsStatus = ARM_DRIVER_ERROR;
	ARM_CFSTORE_KEYDESC kdesc;
	ARM_CFSTORE_SIZE valueLen;
	ARM_CFSTORE_FMODE flags;
	ARM_CFSTORE_HANDLE_INIT(hkey);

	(void) call_count;
	PvMemSet(&kdesc, 0, sizeof(kdesc));

	kdesc.drl = ARM_RETENTION_NVM;
	valueLen = PvStrLen(testDataKeyValue[0].value);

	cfsStatus = gCfStoreDriver->Initialize(NULL, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Initialize() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = CreateKeyValueStore(testDataKeyValue[0].key_name, testDataKeyValue[0].value, &valueLen, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CreateKeyValueStore() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	PvMemSet(&flags, 0, sizeof(flags));

	cfsStatus = gCfStoreDriver->Open(testDataKeyValue[0].key_name, flags, hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Open() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Delete(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Delete() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Close(hkey);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Close() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Flush();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Flush() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	cfsStatus = gCfStoreDriver->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example4_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() failed (cfsStatus=%d)\n", __func__, (int) cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example4_utest_msg_g);

	return CaseNext;
}
#endif // STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(400, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("EXAMPLE4_test_00", cfstore_example4_test_00),
#if defined STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS && STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS==0
        Case("EXAMPLE4_test_01", cfstore_example4_test_01),
#endif // STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond
