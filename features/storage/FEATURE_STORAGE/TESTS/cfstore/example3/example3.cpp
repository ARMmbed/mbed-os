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

/** @file example3.cpp Test case to demonstrate each API function works correctly.
 *
 * \par Example 3 Notes
 *
 * Example3 is a synchronous mode example for creating key-values in the persistent storage.
 *
 * The flash-journal synchronous mode example test does the following CFSTORE operations:
 * - initialises
 * - creates a key-value pair (KV).
 * - writes the data for the KV
 * - closes KV.
 * - flushes the KV to flash
 * - opens KV for reading.
 * - reads KV and checks the value blob was the same as previously written.
 * - closes KV.
 * - finds a KV (there is only 1 to find).
 * - for the KV returned, get the key name.
 * - for the KV returned, get the value length.
 * - for the KV returned, delete the KV.
 * - find another KV (which fails as there are no more keys to find).
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
 *
 * \par How to Build Example3 as a Stand-alone Application
 *
 * This example can be build as a stand-alone application as follows:
 * - Create a new mbed application using the `mbed new .` command.
 * - Copy this file example3.cpp from the to the top level application directory and rename the file to main.cpp.
 * - Build the application with `mbed compile -v -m <target> -t <toolchain> -DCFSTORE_EXAMPLE3_APP` e.g. `mbed compile -v -m K64F -t GCC_ARM -DCFSTORE_EXAMPLE3_APP`.
 *
 */
#include "mbed.h"
#ifndef CFSTORE_EXAMPLE3_APP
/* when built as Configuration-Store example, include greentea support otherwise omit */
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#else   // CFSTORE_EXAMPLE3_APP
/* map utest types for building as stand alone example */
#define control_t   void
#define CaseNext
#endif  // CFSTORE_EXAMPLE3_APP

#include "cfstore_config.h"
#include "cfstore_test.h"
#include "configuration_store.h"

#ifdef YOTTA_CFG_CONFIG_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CONFIG_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CFSTORE_EXAMPLE3_APP
using namespace utest::v1;
#endif


/// @cond CFSTORE_DOXYGEN_DISABLE
#define CFSTORE_EX1_TEST_ASSERT(Expr)                       if (!(Expr)) { printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}
#define CFSTORE_EX1_TEST_ASSERT_EQUAL(expected, actual)     if ((expected) != (actual)) {printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}
#define CFSTORE_EX1_TEST_ASSERT_NOT_EQUAL(expected, actual) if ((expected) == (actual)) {printf("%s:%u: assertion failure\r\n", __FUNCTION__, __LINE__); while (1) ;}

#define CFSTORE_EX1_TEST_ASSERT_MSG(Expr, _fmt, ...)      \
    do                                                    \
    {                                                     \
        if (!(Expr))                                      \
        {                                                 \
            printf(_fmt, __VA_ARGS__);                    \
            while (1) ;                                   \
        }                                                 \
    }while(0);

#define CFSTORE_EX1_LOG(_fmt, ...)                        \
  do                                                      \
  {                                                       \
        printf(_fmt, __VA_ARGS__);                        \
  }while(0);


const char* cfstore_ex3_opcode_str[] =
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

const char* cfstore_ex3_kv_name = "basement.medicine.pavement.government.trenchcoat.off.cough.off.kid.did.when.again.alleyway.friend.cap.pen.dollarbills.ten.foot.soot.put.but.anyway.say.May.DA.kid.did.toes.bows.those.hose.nose.clothes.man.blows.well.well";
const char* cfstore_ex3_kv_value = "TheRollingStone";
#define CFSTORE_EX1_RSEEK_OFFSET    10   /* offset to S of Stone */

typedef struct cfstore_example3_ctx_t
{
    ARM_CFSTORE_CAPABILITIES caps;
    uint8_t hkey[CFSTORE_HANDLE_BUFSIZE];
    uint8_t hkey_next_buf[CFSTORE_HANDLE_BUFSIZE];
    uint8_t hkey_prev_buf[CFSTORE_HANDLE_BUFSIZE];
    ARM_CFSTORE_HANDLE hkey_next;
    ARM_CFSTORE_HANDLE hkey_prev;
    ARM_CFSTORE_SIZE len;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_FMODE flags;
    char value[CFSTORE_KEY_NAME_MAX_LENGTH+1];
} cfstore_example3_ctx_t;

static cfstore_example3_ctx_t cfstore_example3_ctx_g;

extern ARM_CFSTORE_DRIVER cfstore_driver;
ARM_CFSTORE_DRIVER *cfstore_drv = &cfstore_driver;
/// @endcond


static void cfstore_ex3_test_01(cfstore_example3_ctx_t* ctx)
{
    int32_t ret;

	CFSTORE_EX1_LOG("INITIALIZING%s", "\r\n");
	ret = cfstore_drv->Initialize(NULL, NULL);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Initialize() should return ret >= 0 for async/synch modes(ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("CREATING%s", "\r\n");
	memset(&ctx->kdesc, 0, sizeof(ARM_CFSTORE_KEYDESC));
	ctx->kdesc.drl = ARM_RETENTION_NVM;
	ctx->len = strlen(cfstore_ex3_kv_value);
	ret = cfstore_drv->Create(cfstore_ex3_kv_name, ctx->len, &ctx->kdesc, ctx->hkey);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Create() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("WRITING%s", "\r\n");
	ctx->len = strlen(cfstore_ex3_kv_value);
	ret = cfstore_drv->Write(ctx->hkey, cfstore_ex3_kv_value, &ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Write() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) strlen(cfstore_ex3_kv_value), "%s:Error: Write() number of octets written (i.e. completion status (%ld)) != strlen(ctx->value)(%ld)\r\n", __func__, ret, (int32_t) strlen(cfstore_ex3_kv_value));
	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) ctx->len, "%s:Error: Write() number of octets written (i.e. completion status (%ld)) != updated value of len parameter (%ld)\r\n", __func__, ret, (int32_t) ctx->len);

	CFSTORE_EX1_LOG("CLOSING1%s", "\r\n");
	ret = cfstore_drv->Close(ctx->hkey);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Close() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("FLUSHING1%s", "\r\n");
	ret = cfstore_drv->Flush();
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Flush() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("OPENING%s", "\r\n");
	memset(&ctx->flags, 0, sizeof(ctx->flags));
	memset(&ctx->hkey, 0, CFSTORE_HANDLE_BUFSIZE);
	ret = cfstore_drv->Open(cfstore_ex3_kv_name, ctx->flags, ctx->hkey);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Open() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("READING1%s", "\r\n");
	ctx->len = CFSTORE_KEY_NAME_MAX_LENGTH;
	memset(ctx->value, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
	ret = cfstore_drv->Read(ctx->hkey, ctx->value, &ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Read() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) strlen(cfstore_ex3_kv_value), "%s:Error: Read() number of octets read (i.e. completion status (%ld)) != strlen(ctx->value)(%ld)\r\n", __func__, ret, (int32_t) strlen(cfstore_ex3_kv_value));
	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) ctx->len, "%s:Error: Read() number of octets read (i.e. completion status (%ld)) != updated value of len parameter (%ld)\r\n", __func__, ret, (int32_t) ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(strncmp(ctx->value, cfstore_ex3_kv_value, strlen(cfstore_ex3_kv_value)) == 0, "%s:Error: the read value (%s) is not as expected (%s)\r\n", __func__, ctx->value, cfstore_ex3_kv_value);

	CFSTORE_EX1_LOG("RSEEKING%s", "\r\n");
	ret = cfstore_drv->Rseek(ctx->hkey, CFSTORE_EX1_RSEEK_OFFSET);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Rseek() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("READING2%s", "\r\n");
	ctx->len = CFSTORE_KEY_NAME_MAX_LENGTH;
	memset(ctx->value, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
	ret = cfstore_drv->Read(ctx->hkey, ctx->value, &ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Read() failed (ret=%ld)\r\n", __func__, ret);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) strlen(&cfstore_ex3_kv_value[CFSTORE_EX1_RSEEK_OFFSET]), "%s:Error: Read() number of octets read (i.e. completion status (%ld)) != strlen(ctx->value)(%ld)\r\n", __func__, ret, (int32_t) strlen(&cfstore_ex3_kv_value[CFSTORE_EX1_RSEEK_OFFSET]));
	CFSTORE_EX1_TEST_ASSERT_MSG(ret == (int32_t) ctx->len, "%s:Error: Read() number of octets read (i.e. completion status (%ld)) != updated value of len parameter (%ld)\r\n", __func__, ret, (int32_t) ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(strncmp(ctx->value, &cfstore_ex3_kv_value[CFSTORE_EX1_RSEEK_OFFSET], strlen(&cfstore_ex3_kv_value[CFSTORE_EX1_RSEEK_OFFSET])) == 0, "%s:Error: the read value (%s) is not as expected (%s)\r\n", __func__, ctx->value, &cfstore_ex3_kv_value[CFSTORE_EX1_RSEEK_OFFSET]);

	CFSTORE_EX1_LOG("CLOSING2%s", "\r\n");
	ret = cfstore_drv->Close(ctx->hkey);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Close() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("FINDING1%s", "\r\n");
	ret = cfstore_drv->Find("*", ctx->hkey_next, ctx->hkey_prev);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Find() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("GETTING_KEY_NAME%s", "\r\n");
	ctx->len = CFSTORE_KEY_NAME_MAX_LENGTH;
	memset(ctx->value, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
	ret = cfstore_drv->GetKeyName(ctx->hkey_prev, ctx->value, (uint8_t*) &ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: GetKeyName() failed (ret=%ld)\r\n", __func__, ret);
	CFSTORE_EX1_TEST_ASSERT_MSG( ((int32_t) ctx->len == ((int32_t) strlen(cfstore_ex3_kv_name)+1)), "%s:Error: GetKeyName() updated value of len parameter (%ld) != strlen(cfstore_ex3_kv_name) (%ld) (\r\n", __func__, (int32_t) ctx->len, (int32_t) strlen(cfstore_ex3_kv_name));
	CFSTORE_EX1_TEST_ASSERT_MSG(strncmp(ctx->value, cfstore_ex3_kv_name, strlen(cfstore_ex3_kv_name)) == 0, "%s:Error: the key name (%s) is not as expected (%s)\r\n", __func__, ctx->value, cfstore_ex3_kv_name);

	CFSTORE_EX1_LOG("GETTING_VALUE_LEN%s", "\r\n");
	ctx->len = CFSTORE_KEY_NAME_MAX_LENGTH;
	ret = cfstore_drv->GetValueLen(ctx->hkey_prev, &ctx->len);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: GetValueLen() failed (ret=%ld)\r\n", __func__, ret);
	CFSTORE_EX1_TEST_ASSERT_MSG((int32_t) ctx->len == (int32_t) strlen(cfstore_ex3_kv_value), "%s:Error: GetValueLen() updated value of len parameter (%ld) != strlen(cfstore_ex3_kv_value)(%ld) \r\n", __func__, (int32_t) ctx->len, (int32_t) strlen(cfstore_ex3_kv_value));

	CFSTORE_EX1_LOG("DELETING%s", "\r\n");
	ret = cfstore_drv->Delete(ctx->hkey_prev);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Close() failed (ret=%ld)\r\n", __func__, ret);
	CFSTORE_HANDLE_SWAP(ctx->hkey_prev, ctx->hkey_next);

	CFSTORE_EX1_LOG("FINDING2%s", "\r\n");
	ret = cfstore_drv->Find("*", ctx->hkey_next, ctx->hkey_prev);
	CFSTORE_EX1_TEST_ASSERT_MSG(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND, "%s:Error: Find() failed to return expected value of ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("FLUSHING2%s", "\r\n");
	ret = cfstore_drv->Flush();
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error:2: Flush() failed (ret=%ld)\r\n", __func__, ret);

	CFSTORE_EX1_LOG("UNINITIALIZING%s", "\r\n");
	ret = cfstore_drv->Uninitialize();
	CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: Uninitialize() should return ret >= 0 for synch mode(ret=%ld)\r\n", __func__, ret);
	CFSTORE_EX1_LOG("***************%s", "\r\n");
	CFSTORE_EX1_LOG("*** SUCCESS ***%s", "\r\n");
	CFSTORE_EX1_LOG("***************%s", "\r\n");
	return;
}

static control_t cfstore_example3_app_start(const size_t call_count)
{
    cfstore_example3_ctx_t* ctx = &cfstore_example3_ctx_g;

    (void) call_count;

    /* initialise the context */
    memset(ctx, 0, sizeof(cfstore_example3_ctx_t));
    ctx->hkey_next = ctx->hkey_next_buf;
    ctx->hkey_prev = ctx->hkey_prev_buf;
    ctx->caps = cfstore_drv->GetCapabilities();
    CFSTORE_EX1_LOG("%s:INITIALIZING: caps.asynchronous_ops=%lu\n", __func__, ctx->caps.asynchronous_ops);
    if(ctx->caps.asynchronous_ops == 1){
    	/* This is a sync mode only test. If this test is not built for sync mode, then skip testing return true
    	 * This means the test will conveniently pass when run in CI as part of async mode testing */
        CFSTORE_EX1_LOG("*** Skipping test as binary built for flash journal async mode, and this test is sync-only%s", "\n");
        return CaseNext;
    }
    cfstore_ex3_test_01(ctx);
    return CaseNext;
}

#ifndef CFSTORE_EXAMPLE3_APP
/* when built as Configuration-Store example, include greentea support otherwise omit */

/* report whether built/configured for flash sync or async mode */
static control_t cfstore_example3_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_EX1_TEST_ASSERT_MSG(ret >= ARM_DRIVER_OK, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
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
        Case("EXAMPLE3_test_00", cfstore_example3_test_00),
        Case("EXAMPLE3_test_01_start", cfstore_example3_app_start),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
/// @endcond


#else   // CFSTORE_EXAMPLE3_APP

// stand alone Configuration-Store-Example
void app_start(int argc __unused, char** argv __unused)
{
    cfstore_example3_app_start(0);
}


#endif // CFSTORE_EXAMPLE3_APP
