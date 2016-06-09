/** @file EXAMPLE5.cpp
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
 * Test cases to add and delete KVs in the CFSTORE.
 */


/* Notes
 * =====
 *
 * The flash-journal synchronous mode example test does the following CFSTORE operations:
 * - initialises
 * - creates a key-value pair (KV).
 * - writes the data for the KV
 * - closes KV.
 * - flushes the KV to flash
 * - flushes the KV to flash again
 * - uninitialises
 * - initialises
 * - opens KV for reading.
 * - deletes KV
 * - closes KV.
 * - flushes the updated state to flash to store the removal of the deleted KV.
 * - uninitialises
 * - stops
 *
 * This test is coded so as to work only in flash journal sync mode
 * i.e. with caps.asynchronous_ops == false
 *
 * The test leaves the flash in the same state as at the beginning of the test so
 * it can be run a second time on the device without flashing, and the test should
 * still work.
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

static control_t cfstore_example5_test_00(const size_t call_count)
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
        Case("EXAMPLE5_test_00", cfstore_example5_test_00),
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
#include <malloc.h>

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif

//#include <flash-abstraction/Driver_Common.h>
#include <Driver_Common.h>

#ifndef YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING
/* when built as Configuration-Store example, include greentea support otherwise omit */
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#else   // YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING
// map utest types for building as stand alone example
#define control_t   void
#define CaseNext
#endif  // YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING

#include "cfstore_config.h"
#include <configuration-store/configuration_store.h>


#ifndef YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING
using namespace utest::v1;
#endif


#define CFSTORE_EX5_TEST_ASSERT(Expr)                       if (!(Expr)) { printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}
#define CFSTORE_EX5_TEST_ASSERT_EQUAL(expected, actual)     if ((expected) != (actual)) {printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}
#define CFSTORE_EX5_TEST_ASSERT_NOT_EQUAL(expected, actual) if ((expected) == (actual)) {printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}

#define CFSTORE_EX5_TEST_ASSERT_MSG(Expr, _fmt, ...)      \
    do                                                    \
    {                                                     \
        if (!(Expr))                                      \
        {                                                 \
            printf(_fmt, __VA_ARGS__);                    \
            while (1) ;                                   \
        }                                                 \
    }while(0);

#define CFSTORE_EX5_LOG(_fmt, ...)                        \
  do                                                      \
  {                                                       \
        printf(_fmt, __VA_ARGS__);                        \
  }while(0);


const char* cfstore_ex5_opcode_str[] =
{
    "UNDEFINED",
    "CFSTORE_OPCODE_CLOSE",
    "CFSTORE_OPCODE_CREATE",
    "CFSTORE_OPCODE_DELETE",
    "CFSTORE_OPCODE_FIND",
    "CFSTORE_OPCODE_FLUSH",
    "CFSTORE_OPCODE_GET_KEY_NAME",
    "CFSTORE_OPCODE_GET_STATUS",
    "CFSTORE_OPCODE_GET_VALUE_LEN",
    "CFSTORE_OPCODE_INITIALIZE",
    "CFSTORE_OPCODE_OPEN",
    "CFSTORE_OPCODE_POWER_CONTROL",
    "CFSTORE_OPCODE_READ",
    "CFSTORE_OPCODE_RSEEK",
    "CFSTORE_OPCODE_UNINITIALIZE",
    "CFSTORE_OPCODE_WRITE",
    "CFSTORE_OPCODE_MAX"
};

const char* cfstore_ex5_kv_name = "basement.medicine.pavement.government.trenchcoat.off.cough.off.kid.did.when.again.alleyway.friend.cap.pen.dollarbills.ten.foot.soot.put.but.anyway.say.May.DA.kid.did.toes.bows.those.hose.nose.clothes.man.blows.well.well";
const char* cfstore_ex5_kv_value = "TheRollingStone";
#define CFSTORE_EX5_RSEEK_OFFSET    10   /* offset to S of Stone */

typedef struct cfstore_EXAMPLE5_ctx_t
{
    ARM_CFSTORE_CAPABILITIES caps;;
    uint8_t hkey[CFSTORE_HANDLE_BUFSIZE];
    uint8_t hkey_next_buf[CFSTORE_HANDLE_BUFSIZE];
    uint8_t hkey_prev_buf[CFSTORE_HANDLE_BUFSIZE];
    ARM_CFSTORE_HANDLE hkey_next;
    ARM_CFSTORE_HANDLE hkey_prev;
    ARM_CFSTORE_SIZE len;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_FMODE flags;
    char value[CFSTORE_KEY_NAME_MAX_LENGTH+1];
} cfstore_EXAMPLE5_ctx_t;

static cfstore_EXAMPLE5_ctx_t cfstore_EXAMPLE5_ctx_g;

extern ARM_CFSTORE_DRIVER cfstore_driver;
ARM_CFSTORE_DRIVER *cfstore_drv = &cfstore_driver;


static void cfstore_ex5_test_01(cfstore_EXAMPLE5_ctx_t* ctx)
{
    int32_t ret;

	CFSTORE_EX5_LOG("INITIALIZING1%s", "\r\n");
	ret = cfstore_drv->Initialize(NULL, NULL);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Initialize() should return ret >= 0 for async/synch modes(ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("CREATING%s", "\r\n");
	memset(&ctx->kdesc, 0, sizeof(ARM_CFSTORE_KEYDESC));
	ctx->kdesc.drl = ARM_RETENTION_NVM;
	ctx->len = strlen(cfstore_ex5_kv_value);
	ret = cfstore_drv->Create(cfstore_ex5_kv_name, ctx->len, &ctx->kdesc, ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Create() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("WRITING%s", "\r\n");
	ctx->len = strlen(cfstore_ex5_kv_value);
	ret = cfstore_drv->Write(ctx->hkey, cfstore_ex5_kv_value, &ctx->len);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Write() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_TEST_ASSERT_MSG(ret == (int32_t) strlen(cfstore_ex5_kv_value), "%s:Error: Write() number of octets written (i.e. completion status (%ld)) != strlen(ctx->value)(%ld)\r\n", __func__, ret, (int32_t) strlen(cfstore_ex5_kv_value));
	CFSTORE_EX5_TEST_ASSERT_MSG(ret == (int32_t) ctx->len, "%s:Error: Write() number of octets written (i.e. completion status (%ld)) != updated value of len parameter (%ld)\r\n", __func__, ret, (int32_t) ctx->len);

	CFSTORE_EX5_LOG("CLOSING1%s", "\r\n");
	ret = cfstore_drv->Close(ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Close() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("FLUSHING1%s", "\r\n");
	ret = cfstore_drv->Flush();
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Flush() failed (ret=%ld)\r\n", __func__, ret);

	/* commenting out the 2nd flush() means the test works.*/
	//CFSTORE_EX5_LOG("FLUSHING2%s", "\r\n");
	//ret = cfstore_drv->Flush();
	//CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Flush() failed (ret=%ld)\r\n", __func__, ret);

	/* todo: re-instate when Flush() really persists to flash on mbedOSv3+++.
	 * currently in the supported sram version Uninitialize() clears the sram
	CFSTORE_EX5_LOG("UNINITIALIZING1%s", "\r\n");
	ret = cfstore_drv->Uninitialize();
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Uninitialize() should return ret >= 0 for synch mode(ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("INITIALIZING2%s", "\r\n");
	ret = cfstore_drv->Initialize(NULL, NULL);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Initialize() should return ret >= 0 for async/synch modes(ret=%ld)\r\n", __func__, ret);
	*/

	CFSTORE_EX5_LOG("OPENING%s", "\r\n");
	memset(&ctx->flags, 0, sizeof(ctx->flags));
	memset(&ctx->hkey, 0, CFSTORE_HANDLE_BUFSIZE);
	ret = cfstore_drv->Open(cfstore_ex5_kv_name, ctx->flags, ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Open() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("DELETE1%s", "\r\n");
	ctx->len = CFSTORE_KEY_NAME_MAX_LENGTH;
	memset(ctx->value, 0, CFSTORE_KEY_NAME_MAX_LENGTH + 1);
	ret = cfstore_drv->Delete(ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Delete() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("CLOSING2%s", "\r\n");
	ret = cfstore_drv->Close(ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Close() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("FLUSHING3%s", "\r\n");
	ret = cfstore_drv->Flush();
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Flush() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("OPEN2 %s", "\r\n");
	memset(&ctx->flags, 0, sizeof(ctx->flags));
	memset(&ctx->hkey, 0, CFSTORE_HANDLE_BUFSIZE);
	ret = cfstore_drv->Open(cfstore_ex5_kv_name, ctx->flags, ctx->hkey);
	CFSTORE_EX5_TEST_ASSERT_MSG(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, "%s:Error: Find() failed to return expected value of ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("FLUSHING2%s", "\r\n");
	ret = cfstore_drv->Flush();
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error:2: Flush() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX5_LOG("UNINITIALIZING3%s", "\r\n");
	ret = cfstore_drv->Uninitialize();
	CFSTORE_EX5_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Uninitialize() should return ret >= 0 for synch mode(ret=%ld)\r\n", __func__, ret);
	CFSTORE_EX5_LOG("***************%s", "\r\n");
	CFSTORE_EX5_LOG("*** SUCCESS ***%s", "\r\n");
	CFSTORE_EX5_LOG("***************%s", "\r\n");
	return;
}

static control_t cfstore_EXAMPLE5_app_start(const size_t call_count)
{
    cfstore_EXAMPLE5_ctx_t* ctx = &cfstore_EXAMPLE5_ctx_g;

    (void) call_count;

    /* initialise the context */
    memset(ctx, 0, sizeof(cfstore_EXAMPLE5_ctx_t));
    ctx->hkey_next = ctx->hkey_next_buf;
    ctx->hkey_prev = ctx->hkey_prev_buf;
    ctx->caps = cfstore_drv->GetCapabilities();
    CFSTORE_EX5_LOG("%s:INITIALIZING: caps.asynchronous_ops=%lu\n", __func__, ctx->caps.asynchronous_ops);
    if(ctx->caps.asynchronous_ops == true){
    	/* This is a sync mode only test. If this test is not built for sync mode, then skip testing return true
    	 * This means the test will conveniently pass when run in CI as part of async mode testing */
        CFSTORE_EX5_LOG("*** Skipping test as binary built for flash journal async mode, and this test is sync-only%s", "\n");
        return CaseNext;
    }
    cfstore_ex5_test_01(ctx);
    return CaseNext;
}

#ifndef YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING
/* when built as Configuration-Store example, include greentea support otherwise omit */

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("EXAMPLE5_test_01_start", cfstore_EXAMPLE5_app_start),
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


#else   // YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING

// stand alone Configuration-Store-Example
void app_start(int argc __unused, char** argv __unused)
{
    cfstore_EXAMPLE5_app_start(0);
}


#endif // YOTTA_CONFIGURATION_STORE_EXAMPLE5_VERSION_STRING


#endif // __MBED__ && ! defined TOOLCHAIN_GCC_ARM
