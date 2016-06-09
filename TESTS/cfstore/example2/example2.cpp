/** @file example2.cpp
 *
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


/* Overview of test:
 * - initialises cfstore
 * - creates a key called "com.arm.mbed.spv.assets.asset2.payload" with value blob length = 15 = strlen("Grumpy old man")+1.
 * - writes the data for the key to be "Grumpy old man"
 * - closes kv.
 * - opens kv for reading/writing
 * - reads the value blob and checks its == "Grumpy old man"
 * - writes the first 11 chars of the value blob to be "Grumpy man" plus a NULL;
 * - reads the value blob back and checks its as expected
 *
 * This test is coded so as to work in the following modes:
 * - flash sync mode i.e. with caps.asynchronous_ops == false
 * - flash async mode i.e. with caps.asynchronous_ops == true
 */
#if defined __MBED__ && ! defined TOOLCHAIN_GCC_ARM


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif

#include "cfstore_config.h"
#include <Driver_Common.h>
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include <configuration-store/configuration_store.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#include "cfstore_uvisor.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

using namespace utest::v1;

static control_t cfstore_example2_test_00(const size_t call_count)
{
    (void) call_count;
    printf("Not implemented for ARM toolchain\n");
    return CaseNext;
}


utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("EXAMPLE2_test_00", cfstore_example2_test_00),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}



#else


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif

#include "cfstore_config.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
//#include <flash-abstraction/Driver_Common.h>
#include <Driver_Common.h>
#include <configuration-store/configuration_store.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CONFIG_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CONFIG_UVISOR */


using namespace utest::v1;

static char cfstore_example2_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];

/* defines */
#define PvMemSet memset
#define PvStrLen strlen

/* report whether built/configured for flash sync or async mode */
static control_t cfstore_example2_test_00(const size_t call_count)
{
    (void) call_count;
    CFSTORE_LOG("INITIALIZING: caps.asynchronous_ops=%lu\n", cfstore_driver.GetCapabilities().asynchronous_ops);
    return CaseNext;
}

ARM_CFSTORE_DRIVER *drv = &cfstore_driver;
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
	cfsStatus = drv->Create(keyName, valueLength, keyDesc, hkey);
	TEST_ASSERT_EQUAL(ARM_DRIVER_OK, cfsStatus);

	valueLength = *dataLength;
	cfsStatus = drv->Write(hkey, data, &valueLength);
	/*
	 * (1) Note the following:
	 * - if cfsStatus > 0 then Write() has completed synchronously and returned the number of bytes written (irrespective of the caps.asynchronous_ops attribute).
	 * - if cfsStatus == ARM_DRIVER_OK then:
     *      - if caps.asynchronous_ops == true then the operation will be completed with registered client callback passed to Initialize().
     *      - if caps.asynchronous_ops == false then the operation has completed synchronously and no bytes were written.
     * - if cfsStatus < ARM_DRIVER_OK then an error has occurred
	 */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write key (rc=%" PRId32 ")\n", __func__, cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example2_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%sError: valueLength(%" PRId32 ") does not match the expected dataLength(%" PRId32 ")\n", __func__, (int32_t) valueLength, (int32_t) *dataLength);
    TEST_ASSERT_MESSAGE(*dataLength == valueLength, cfstore_example2_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%sError: Write() return value cfsStatus(%" PRId32 ") does not match the expected dataLength(%" PRId32 ")\n", __func__, cfsStatus, (int32_t) *dataLength);
    TEST_ASSERT_MESSAGE((int32_t) *dataLength == cfsStatus, cfstore_example2_utest_msg_g);

	drv->Close(hkey);
	/* CreateKeyValueStore() returns what was returned from Write(). See (1) above for description. */
	return cfsStatus;
}

static control_t cfstore_example2_test_01(const size_t call_count)
{
	int32_t cfsStatus;
	ARM_CFSTORE_HANDLE_INIT(hkey);
	ARM_CFSTORE_HANDLE_INIT(updatedKeyH);
	ARM_CFSTORE_FMODE flags;
	ARM_CFSTORE_KEYDESC kdesc;
	ARM_CFSTORE_SIZE valueLen;
	char* ptr = NULL;
	
	const char key[]   = "com.arm.mbed.spv.assets.asset2.payload";
	const char value[] = "Grumpy old man";

	(void) call_count;

	// It must not exceed the value_len field specified when the Key-Value pair was created
	const char newDataToWrite[] = "Grumpy man";
	
	char readBuf[CFSTORE_KEY_NAME_MAX_LENGTH + 1];
	ARM_CFSTORE_SIZE len = 0;

	// Write a key-value pair

	PvMemSet(&kdesc, 0, sizeof(kdesc));
	PvMemSet(&flags, 0, sizeof(flags));
	PvMemSet(readBuf, 0, CFSTORE_KEY_NAME_MAX_LENGTH + 1);

	kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
	/* The length supplied to Write() is the number of octets to store in the blob.
	 * Specifying the following value for valueLen will store the terminating null to
	 * a string, for example.
	 */
	valueLen = PvStrLen(value) + 1;

	cfsStatus = drv->Initialize(NULL, NULL);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Initialize() failed (cfsStatus=%" PRId32 ")\n", __func__, cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example2_utest_msg_g);
	
	cfsStatus = CreateKeyValueStore(key, value, &valueLen, &kdesc);

	/* CreateKeyValueStore() returns the number of characters written, which can vary between 0 and the supplied arg valueLen
	 * - in the case that this example is compiled for flash mode sync, CreateKeyValueStore(), on success should always return valueLen
	 * - in the case that this example is compiled for flash mode async, CreateKeyValueStore() on success may return a value 0 to valueLen
	 *   with async notification of the completed transaction.
	 */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%sError: valueLen(%" PRId32 ") does not match the expected returned value from CreateKeyValueStore(%" PRId32 ")\n", __func__, (int32_t) valueLen, cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus == (int32_t) valueLen, cfstore_example2_utest_msg_g);

	// Read key-value pair with 'Write' permission

	flags.read = true;
	flags.write = true;
	cfsStatus = drv->Open(key, flags, hkey);
	TEST_ASSERT_EQUAL(ARM_DRIVER_OK, cfsStatus);

	len = sizeof(readBuf);
	cfsStatus = drv->Read(hkey, readBuf, &len);
    /* Read() returns the number of characters read, which can vary between 0 and the size of the value blob, and the size of the supplied buffer  */
	CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read() returned value (%" PRId32 ") does not match the created length of the value blob(%" PRId32 ")\n", __func__, cfsStatus, (int32_t) PvStrLen(value) + 1);
    TEST_ASSERT_MESSAGE(cfsStatus == (int32_t) (PvStrLen(value) + 1), cfstore_example2_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read() returned len value (%" PRId32 ") does not match the created length of the value blob(%" PRId32 ")\n", __func__, (int32_t) len, (int32_t) PvStrLen(value) + 1);
    TEST_ASSERT_MESSAGE(len == PvStrLen(value) + 1, cfstore_example2_utest_msg_g);

    /* Note:
     * - original data = "Grumpy old man", which is 14+1 chars inc NULL
     * - New data = "Grumpy man" which is 10+1 chars inc NULL
     * - when the key "com.arm.mbed.spv.assets.asset2.payload"; was created, it was created with a value blob size of 14+1=15 chars.
     * - The new data is shorter that the old data so it will be accommodated in the value blob
     * - The size of the value blob will stay the same.
     */
	// Update the value and value length
    /* note len set to sizeof(newDataToWrite) which includes the terminating null of the string */
	len = sizeof(newDataToWrite);
	cfsStatus = drv->Write(hkey, newDataToWrite, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Write() returned cfsStatus value (%" PRId32 ") does not match the length of new data written(%" PRId32 ")\n", __func__, cfsStatus, (int32_t) sizeof(newDataToWrite));
    TEST_ASSERT_MESSAGE(cfsStatus == (int32_t) sizeof(newDataToWrite), cfstore_example2_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Write() returned len (%" PRId32 ") does not match the length of new data written(%" PRId32 ")\n", __func__, (int32_t) len, (int32_t) sizeof(newDataToWrite));
    TEST_ASSERT_MESSAGE((int32_t) len == (int32_t) sizeof(newDataToWrite), cfstore_example2_utest_msg_g);

	drv->Close(hkey);

	// Check that the value was updated
	flags.write = false;
	cfsStatus = drv->Open(key, flags, updatedKeyH);
	TEST_ASSERT_EQUAL(ARM_DRIVER_OK, cfsStatus);

	len = CFSTORE_KEY_NAME_MAX_LENGTH;
	PvMemSet(readBuf, 0, len);
	cfsStatus = drv->Read(updatedKeyH, readBuf, &len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read() returned value (%" PRId32 ") does not match the created length of the value blob(%" PRId32 ")\n", __func__, cfsStatus, (int32_t) PvStrLen(value) + 1);
    TEST_ASSERT_MESSAGE(cfsStatus == (int32_t) (PvStrLen(value) + 1), cfstore_example2_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read() returned len value (%" PRId32 ") does not match the created length of the value blob(%" PRId32 ")\n", __func__, (int32_t) len, (int32_t) PvStrLen(value) + 1);
    TEST_ASSERT_MESSAGE(len == (PvStrLen(value) + 1), cfstore_example2_utest_msg_g);

    /* convert any terminating nulls to '=' */
    while( (ptr = (char*) memchr(readBuf, 0, (PvStrLen(value) + 1))) != NULL)
    {
        *ptr = '=';
    }
    /* check the data is as expected */
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Read() returned unexpected string (%s) where nulls have been converted to '=' chars\n", __func__, readBuf);
    TEST_ASSERT_MESSAGE(strncmp(readBuf, "Grumpy man=man=", (PvStrLen(value) + 1)) == 0, cfstore_example2_utest_msg_g);

    CFSTORE_LOG("Success: New value of KV (%s) value blob (with nulls converted to '=') = (%s)\n", key, readBuf);

	drv->Close(updatedKeyH);
	
	cfsStatus = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_example2_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Uninitialize() failed (cfsStatus=%" PRId32 ")\n", __func__, cfsStatus);
    TEST_ASSERT_MESSAGE(cfsStatus >= ARM_DRIVER_OK, cfstore_example2_utest_msg_g);

	return CaseNext;
}



utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(400, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("EXAMPLE2_test_00", cfstore_example2_test_00),
        Case("EXAMPLE2_test_01", cfstore_example2_test_01),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

#if defined CFSTORE_CONFIG_MBED_OS_VERSION && CFSTORE_CONFIG_MBED_OS_VERSION == 3
/* mbedosV3*/
void app_start(int argc __unused, char** argv __unused)
{
    /* Run the test specification */
    Harness::run(specification);
}
#endif /* CFSTORE_CONFIG_MBED_OS_VERSION == 3 */

#if defined CFSTORE_CONFIG_MBED_OS_VERSION && CFSTORE_CONFIG_MBED_OS_VERSION == 4
/* mbedosV3++*/
int main()
{
    return !Harness::run(specification);
}
#endif /* CFSTORE_CONFIG_MBED_OS_VERSION == 4 */


#endif // __MBED__ && ! defined TOOLCHAIN_GCC_ARM
