/* @file flush.cpp
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
 * Test cases to flush KVs in the CFSTORE using the drv->Flush() interface.
 */
#if defined __MBED__ && ! defined TOOLCHAIN_GCC_ARM


#include <stdio.h>
int main()
{
    printf("Not implemented for ARM toolchain\n");
}


#else



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifdef TARGET_LIKE_FRDM_K64F_GCC
#include <mbed-drivers/mbed.h>
#endif

#include "cfstore_config.h"
#include "../../../source/cfstore_test.h"
#include "../../../source/cfstore_debug.h"
#include <flash-abstraction/Driver_Common.h>
#include <configuration-store/configuration_store.h>
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

using namespace utest::v1;

/*
 * Defines
 */
#define CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE                256
#define cfstore_flush_fsm_null                          NULL
#define CFSTORE_FLUSH_CASE_TIMEOUT_MS                   10000
#define CFSTORE_FLUSH_FSM_LOOPS                         20

#ifdef CFSTORE_DEBUG
#define CFSTORE_FLUSH_GREENTEA_TIMEOUT_S     360
#else
#define CFSTORE_FLUSH_GREENTEA_TIMEOUT_S     60
#endif

/*
 * Globals
 *
 * cfstore_flush_utest_msg_g
 *  buffer for storing TEST_ASSERT_xxx_MESSAGE messages
 */
char cfstore_flush_utest_msg_g[CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE];

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.flush.box1");
UVISOR_BOX_CONFIG(cfstore_flush_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */




#ifdef TARGET_LIKE_X86_LINUX_NATIVE

/** @brief  basic Flush() test
 *
 * @return  status code
 *          ARM_DRIVER_OK, the test passed successfully
 *          ret != ARM_DRIVER_OK, the test failed with the return code
 *          supplying more details
 */
static int32_t cfstore_flush_test_01_x86_sync(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_LOG("cfstore_flush_test_01: Start%s", "\r\n");
    ret = drv->Initialize(NULL, NULL);
    if(ret != ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Initialize() call failed (ret=%" PRId32 ").\r\n", __func__, ret);
        goto out0;
    }
    ret = drv->Flush();
    if(ret != ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Flush() call failed (ret=%" PRId32 ").\r\n", __func__, ret);
    }
    ret = drv->Uninitialize();
    if(ret != ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Initialize() call failed to Uninitialise(ret=%" PRId32 ").\r\n", __func__, ret);
        goto out0;
    }
 out0:
    if(ret == ARM_DRIVER_OK){
        CFSTORE_LOG("cfstore_flush_test_01: End: Success%s", "\r\n");
    } else {
        CFSTORE_LOG("cfstore_flush_test_01: End: Failed%s", "\r\n");
    }
    return ret;
}
#endif /* TARGET_LIKE_X86_LINUX_NATIVE */

/* KV data for test_03 */
static cfstore_kv_data_t cfstore_flush_test_02_kv_data[] = {
        { "com.arm.mbed.configurationstore.test.flush.cfstoreflushtest02", "1"},
        /*          1         2         3         4         5         6        7  */
        /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        { NULL, NULL},
};


/* async test version */

/* @brief  test fsm states and events */
typedef enum cfstore_flush_fsm_state_t {
    cfstore_flush_fsm_state_stopped = 0,
    cfstore_flush_fsm_state_initializing,
    cfstore_flush_fsm_state_flushing,
    cfstore_flush_fsm_state_uninitializing,
    cfstore_flush_fsm_state_max
} cfstore_flush_fsm_state_t;

/* @brief  test fsm events */
typedef enum cfstore_flush_fsm_event_t {
    cfstore_flush_fsm_event_init_done = 0,
    cfstore_flush_fsm_event_flush_done,
    cfstore_flush_fsm_event_uninit_done,
    cfstore_flush_fsm_event_max,
} cfstore_flush_fsm_event_t;

typedef void (*cfstore_flush_fsm_handler)(void* ctx);

typedef struct cfstore_fsm_t
{
    cfstore_flush_fsm_state_t state;
    cfstore_flush_fsm_event_t event;
} cfstore_fsm_t;

typedef struct cfstore_flush_ctx_t
{
    int32_t loops_done;
    cfstore_fsm_t fsm;
    int32_t status;
    ARM_CFSTORE_OPCODE cmd_code;
} cfstore_flush_ctx_t;


/*
 * Globals
 */
static cfstore_flush_ctx_t cfstore_flush_ctx_g;

#ifdef CFSTORE_DEBUG
static const char* cfstore_flush_state_str[] =
{
    "stopped",
    "initializing",
    "flushing",
    "uninitializing",
    "unknown"
};

static const char* cfstore_flush_event_str[] =
{
    "init_done",
    "flush_done",
    "uninit_done",
    "unknown"
};
#endif

/*
 * Forward decl
 */
static void cfstore_flush_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_flush_fsm_event_t event, void* context);
static void cfstore_flush_fsm_state_set(cfstore_fsm_t* fsm, cfstore_flush_fsm_state_t new_state, void* ctx);
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


/* @brief   cfstore asynchronous callback handler */
void cfstore_flush_test_01_callback(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *client_context, ARM_CFSTORE_HANDLE handle)
{
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) client_context;

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d (%s) ctx=%p, handle=%p\r\n", __func__, (int) status, (int) cmd_code, cfstore_test_opcode_str[cmd_code], ctx, handle);
    (void) handle;
    switch(cmd_code)
    {
    case CFSTORE_OPCODE_INITIALIZE:
        ctx->fsm.event = cfstore_flush_fsm_event_init_done;
        break;
    case CFSTORE_OPCODE_FLUSH:
        ctx->fsm.event = cfstore_flush_fsm_event_flush_done;
        break;
    case CFSTORE_OPCODE_UNINITIALIZE:
        ctx->fsm.event = cfstore_flush_fsm_event_uninit_done;
        break;
    case CFSTORE_OPCODE_CLOSE:
    case CFSTORE_OPCODE_CREATE:
    case CFSTORE_OPCODE_DELETE:
    case CFSTORE_OPCODE_FIND:
    case CFSTORE_OPCODE_GET_KEY_NAME:
    case CFSTORE_OPCODE_GET_STATUS:
    case CFSTORE_OPCODE_GET_VALUE_LEN:
    case CFSTORE_OPCODE_OPEN:
    case CFSTORE_OPCODE_POWER_CONTROL:
    case CFSTORE_OPCODE_READ:
    case CFSTORE_OPCODE_RSEEK:
    case CFSTORE_OPCODE_WRITE:
    default:
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:WARN: received asynchronous notification for opcode=%d (%s) when api call should have been synchronous", __func__, cmd_code, cmd_code < CFSTORE_OPCODE_MAX ? cfstore_test_opcode_str[cmd_code] : "unknown");
        CFSTORE_DBGLOG("%s:WARN: received asynchronous notification for opcode=%d (%s) when api call should have been synchronous", __func__, cmd_code, cmd_code < CFSTORE_OPCODE_MAX ? cfstore_test_opcode_str[cmd_code] : "unknown");
        /* TEST_ASSERT_MESSAGE(false, cfstore_flush_utest_msg_g) */
        return;
    }
    ctx->status = status;
    ctx->cmd_code = cmd_code;
    cfstore_flush_fsm_state_handle_event(&ctx->fsm, ctx->fsm.event, client_context);
    return;
}

/* @brief  fsm handler called on entry to stopping state */
static void cfstore_flush_fsm_stop_on_entry(void* context)
{
    (void) context;
    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    Harness::validate_callback();
    return;
}

/* @brief  fsm handler called on entry to initializing state */
static void cfstore_flush_fsm_init_on_entry(void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;
    const ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered: callback=%p, ctx=%p\r\n", __func__, cfstore_flush_test_01_callback, ctx);
    ret = drv->Initialize(cfstore_flush_test_01_callback, ctx);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize CFSTORE (ret=%" PRId32 ")\r\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    CFSTORE_DBGLOG("%s:debug: ret=%" PRId32 "\r\n", __func__, ret);
    return;
}

/* brief    callback handler when in state initializing */
static void cfstore_flush_fsm_initializing(void* context)
{
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_FENTRYLOG("%s:entered: status = %d\r\n", __func__, (int) ctx->status);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in initializing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flush_fsm_state_initializing, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, CFSTORE_OPCODE_INITIALIZE);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == CFSTORE_OPCODE_INITIALIZE, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%" PRId32 "\r\n", __func__, ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    /* only change state if status >= 0*/
    if(ctx->status >= 0){
        cfstore_flush_fsm_state_set(&ctx->fsm, cfstore_flush_fsm_state_flushing, ctx);
    }
    return;
}

/* static void cfstore_flush_fsm_init_on_exit(void* context){ (void) context;} */


/* @brief  fsm handler called on entry to flushing state */
static void cfstore_flush_fsm_flush_on_entry(void* context)
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
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    memset(&kdesc, 0, sizeof(kdesc));

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in flushing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flush_fsm_state_flushing, cfstore_flush_utest_msg_g);
    /* try to read key; should not be found */
    ret = cfstore_test_kv_is_found(cfstore_flush_test_02_kv_data->key_name, &bfound);
    if(ret != ARM_DRIVER_OK && ret != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_test_kv_is_found() call failed (ret=%" PRId32 ").\r\n", __func__, ret);
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
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error:1: failed to write kv data (ret=%" PRId32 ").\r\n", __func__, ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
        /* flush to flash */
        ret = drv->Flush();
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to flush data to cfstore flash (ret=%" PRId32 ").\r\n", __func__, ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
        CFSTORE_LOG("FLUSH: Success pending for new KV creation (name=%s, value=%s)\n", cfstore_flush_test_02_kv_data->key_name, cfstore_flush_test_02_kv_data->value)
    } else {
        /*read the value, increment by 1 and write value back */
        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        ret = cfstore_test_read(cfstore_flush_test_02_kv_data->key_name, value, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to read kv data (ret=%" PRId32 ").\r\n", __func__, ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);

        ivalue = atoi(value);
        CFSTORE_LOG("FLUSH: Read KV from flash (name=%s, value=%" PRId32 ")\n", cfstore_flush_test_02_kv_data->key_name, ivalue);
        /* increment value */
        ++ivalue;
        snprintf(value, CFSTORE_KEY_NAME_MAX_LENGTH+1, "%" PRId32 "", ivalue);
        len = strlen(value);
        ret = cfstore_test_write(cfstore_flush_test_02_kv_data->key_name, value, &len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to write kv data (ret=%" PRId32 ").\r\n", __func__, ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);

        /* flush to flash */
        ret = drv->Flush();
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to flush data to cfstore flash (ret=%" PRId32 ").\r\n", __func__, ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
        CFSTORE_LOG("FLUSH: Success pending for new KV value to flash (name=%s, value=%" PRId32 ")\n", cfstore_flush_test_02_kv_data->key_name, ivalue);
    }
    return;
}


/* brief    callback handler when in state flushing */
static void cfstore_flush_fsm_flushing(void* context)
{
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in flushing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flush_fsm_state_flushing, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, CFSTORE_OPCODE_FLUSH);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == CFSTORE_OPCODE_FLUSH, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%" PRId32 "\r\n", __func__, ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    /* only change state if status >= 0*/
    if(ctx->status >= 0){
        CFSTORE_LOG("FLUSH: Successfully flushed data to flash.%s", "\n");
        cfstore_flush_fsm_state_set(&ctx->fsm, cfstore_flush_fsm_state_uninitializing, ctx);
    }
    return;
}

/* static void cfstore_flush_fsm_flush_on_exit(void* context){ (void) context;} */


/* @brief  fsm handler called on entry to uninitializing state */
static void cfstore_flush_fsm_uninit_on_entry(void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;
    const ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in uninitializing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flush_fsm_state_uninitializing, cfstore_flush_utest_msg_g);
    ret = drv->Uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to uninitialize CFSTORE (ret=%" PRId32 ")\r\n", __func__, ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    return;
}

/* brief    callback handler when in state uninitializing */
static void cfstore_flush_fsm_uninitializing(void* context)
{
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_DBGLOG("%s:ctx->status=%" PRId32 ", ctx->loops_done=%" PRId32 "\r\n", __func__, ctx->status, ctx->loops_done);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in uninitializing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flush_fsm_state_uninitializing, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, CFSTORE_OPCODE_UNINITIALIZE);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == CFSTORE_OPCODE_UNINITIALIZE, cfstore_flush_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%" PRId32 "\r\n", __func__, ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= ARM_DRIVER_OK, cfstore_flush_utest_msg_g);
    /* only change state if status >= 0*/
    if(ctx->status >= ARM_DRIVER_OK){
        ++ctx->loops_done;
        if(ctx->loops_done < CFSTORE_FLUSH_FSM_LOOPS){
            cfstore_flush_fsm_state_set(&ctx->fsm, cfstore_flush_fsm_state_initializing, ctx);
        } else {
            /* move to init state */
            cfstore_flush_fsm_state_set(&ctx->fsm, cfstore_flush_fsm_state_stopped, ctx);
        }
    }
    return;
}

/* static void cfstore_flush_fsm_uninit_on_exit(void* context) {(void)context;} */



/* handler functions while in state */
static cfstore_flush_fsm_handler cfstore_flush_fsm[cfstore_flush_fsm_state_max][cfstore_flush_fsm_event_max] =
{
/* state\event:         init_done                           flush_done                      unint_done */
/* stopped          */  {cfstore_flush_fsm_null,            cfstore_flush_fsm_null,         cfstore_flush_fsm_null              },
/* initialising     */  {cfstore_flush_fsm_initializing,    cfstore_flush_fsm_null,         cfstore_flush_fsm_null              },
/* flushing         */  {cfstore_flush_fsm_null,            cfstore_flush_fsm_flushing,     cfstore_flush_fsm_null              },
/* uninitializing   */  {cfstore_flush_fsm_null,            cfstore_flush_fsm_null,         cfstore_flush_fsm_uninitializing    }
};


/* handler functions for entering the state*/
static cfstore_flush_fsm_handler cfstore_flush_fsm_on_entry[cfstore_flush_fsm_state_max] =
{
    cfstore_flush_fsm_stop_on_entry,
    cfstore_flush_fsm_init_on_entry,
    cfstore_flush_fsm_flush_on_entry,
    cfstore_flush_fsm_uninit_on_entry
};

/* handler functions for exiting state, currently none used */
static cfstore_flush_fsm_handler cfstore_flush_fsm_on_exit[cfstore_flush_fsm_state_max] =
{
    cfstore_flush_fsm_null,
    cfstore_flush_fsm_null,
    cfstore_flush_fsm_null,
    cfstore_flush_fsm_null
};


/* @brief   inject event into fsm */
static void cfstore_flush_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_flush_fsm_event_t event, void* context)
{
    cfstore_flush_ctx_t* ctx = (cfstore_flush_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, state=(%s), event=%d (%s), ctx=%p\r\n", __func__, fsm, cfstore_flush_state_str[fsm->state], (int) event, cfstore_flush_event_str[event], ctx);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flush_utest_msg_g, CFSTORE_FLUSH_UTEST_MSG_BUF_SIZE, "%s:Error: invalid event (%d)\r\n", __func__, (int) event);
    TEST_ASSERT_MESSAGE(event < cfstore_flush_fsm_event_max, cfstore_flush_utest_msg_g);
    fsm->event = event;
    if(cfstore_flush_fsm[fsm->state][fsm->event] != NULL){
        cfstore_flush_fsm[fsm->state][fsm->event](ctx);
    }

    /* do not clear context data set by caller as it may be used later
     *  fsm->event = cfstore_flush_fsm_event_max;
     *  ctx->status = 0;
     *  ctx->cmd_code =  (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);
     */
    return;
}


/* @brief   function to move to new fsm state, calling state exit function for old state and entry function for new state */
static void cfstore_flush_fsm_state_set(cfstore_fsm_t* fsm, cfstore_flush_fsm_state_t new_state, void* ctx)
{
    #ifdef CFSTORE_DEBUG
    cfstore_flush_fsm_state_t old_state = fsm->state;
    #endif

    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, ctx=%p\r\n", __func__, fsm, ctx);
    #ifdef CFSTORE_DEBUG
    CFSTORE_DBGLOG("%s:FSM:REQ RX:%s:%s\r\n", __func__, cfstore_flush_state_str[fsm->state], cfstore_flush_state_str[new_state]);
    #endif
    TEST_ASSERT_MESSAGE(fsm != NULL, "fsm is not a valid pointer");
    TEST_ASSERT_MESSAGE(new_state < cfstore_flush_fsm_state_max, "new_state is not a valid state");
    TEST_ASSERT_MESSAGE(ctx != NULL, "ctx is not a valid pointer");
    TEST_ASSERT_MESSAGE(fsm->state < cfstore_flush_fsm_state_max, "fsm->state is not a valid state");

    if(cfstore_flush_fsm_on_exit[fsm->state] != NULL){
        cfstore_flush_fsm_on_exit[fsm->state](ctx);
    }
    fsm->state = new_state;
    if(cfstore_flush_fsm_on_entry[new_state] != NULL){
        cfstore_flush_fsm_on_entry[new_state](ctx);
    }
    #ifdef CFSTORE_DEBUG
    CFSTORE_DBGLOG("%s:FSM:REQ DONE fsm=%p, fsm->state (old_state)=%s, new_state=%s, ctx=%p\r\n", __func__, fsm, cfstore_flush_state_str[old_state], cfstore_flush_state_str[new_state], ctx);
    #endif
    return;
}


/* @brief   asynchronous test 01 */
static control_t cfstore_flush_test_02_k64f(void)
{
    cfstore_flush_ctx_t* ctx = cfstore_flush_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    cfstore_flush_ctx_init(ctx);
    cfstore_flush_fsm_state_set(&ctx->fsm, cfstore_flush_fsm_state_initializing, ctx);
    return CaseTimeout(CFSTORE_FLUSH_GREENTEA_TIMEOUT_S*1000);
}

/* report whether built/configured for flash sync or async mode */
static control_t cfstore_flush_test_00(const size_t call_count)
{
    (void) call_count;
    CFSTORE_LOG("INITIALIZING: caps.asynchronous_ops=%lu\n", cfstore_driver.GetCapabilities().asynchronous_ops);
    return CaseNext;
}


utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(CFSTORE_FLUSH_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
        Case("FLUSH_test_00", cfstore_flush_test_00),
        Case("FLUSH_test_01", cfstore_flush_test_02_k64f),
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
