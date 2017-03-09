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

/** @file flash.cpp Test cases to flush KVs in the CFSTORE using the Flash-Journal interface.
 *
 * Please consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "cfstore_config.h"
#include "cfstore_test.h"
#include "cfstore_debug.h"
#include "Driver_Common.h"
#include "configuration_store.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
#include "flash_journal_strategy_sequential.h"
#include "flash_journal.h"
#include "Driver_Common.h"
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

using namespace utest::v1;

/* Configure secure box. */
#ifdef YOTTA_CFG_CFSTORE_UVISOR
UVISOR_BOX_NAMESPACE("com.arm.mbed.cfstore.test.flash.box1");
UVISOR_BOX_CONFIG(cfstore_flash_box1, UVISOR_BOX_STACK_SIZE);
#endif /* YOTTA_CFG_CFSTORE_UVISOR */



/* shared code common to both sync and async test cases */

/*
 * Defines
 *
 */
/// @cond CFSTORE_DOXYGEN_DISABLE
#define CFSTORE_FREE                                    free
#define CFSTORE_MALLOC                                  malloc
#define CFSTORE_REALLOC                                 realloc
#define CFSTORE_FLASH_STACK_BUF_SIZE                    64
#define CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE    8
#define CFSTORE_TEST_DATA_KEYNAME                       "com.arm.mbed.configurationstore.test.flush.cfstoreflushtest02"
#define CFSTORE_TEST_DATA_KEYNAME_SHORT                 "com.arm"
#define CFSTORE_TEST_DATA_VALUE_INIT                    "\1"
#define CFSTORE_TEST_DATA_KEYNAME_SIZE                  (sizeof(CFSTORE_TEST_DATA_KEYNAME) - 1)
#define CFSTORE_TEST_DATA_VALUE_SIZE                    (sizeof(CFSTORE_TEST_DATA_VALUE_INIT) - 1)
#define CFSTORE_FLASH_UTEST_MSG_BUF_SIZE                256
#define CFSTORE_FLASH_MTD_ASYNC_OPS_ON                  1
#define CFSTORE_FLASH_MTD_ASYNC_OPS_OFF                 0
#define CFSTORE_FLASH_CASE_TIMEOUT_MS                   5000
/// @endcond

/*
 * Globals
 */

/// @cond CFSTORE_DOXYGEN_DISABLE
char cfstore_flash_utest_msg_g[CFSTORE_FLASH_UTEST_MSG_BUF_SIZE];
/// @endcond

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
uint16_t cfstore_flash_mtd_async_ops_g = 0;
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;


/* KV data for test_01 */
static cfstore_kv_data_t cfstore_flush_test_01_kv_data[] = {
        { CFSTORE_TEST_DATA_KEYNAME, CFSTORE_TEST_DATA_VALUE_INIT},
        { NULL, NULL},
};


/* @brief   key value header structure defining key_name length, value length
 * @note
 * 8 bytes long */
typedef struct cfstore_area_header_t
{
    uint32_t vlength;
    uint8_t klength;
    uint8_t perm_owner_read : 1;
    uint8_t perm_owner_write : 1;
    uint8_t perm_owner_execute : 1;
    uint8_t perm_other_read : 1;
    uint8_t perm_other_write : 1;
    uint8_t perm_other_execute : 1;
    uint8_t reserved : 2;
    uint8_t refcount;
    struct flags_t {
        uint8_t deleting : 1;
        uint8_t reserved : 7;
    } flags ;
} cfstore_area_header_t;


/* @brief   data structure for managing test data */
typedef struct cfstore_flash_data_blob_t {
    cfstore_area_header_t hdr;
    uint8_t data[CFSTORE_TEST_DATA_KEYNAME_SIZE + CFSTORE_TEST_DATA_VALUE_SIZE]; /* 61 bytes for key_name, 1 byte for value */
} cfstore_flash_data_blob_t;

/*
 * Defines
 *
 * CFSTORE_FLASH_AREA_SIZE_MIN
 *  valid sizes of areas should always be greater than the size of the header, and therefore
 *  greater than this value, which is defined as smaller than the header size
 */
#define CFSTORE_FLASH_AREA_SIZE_MIN                     (sizeof(cfstore_area_header_t) - 1)

/*
 * Shared implementation between sync and async tests
 */

/* print key name string from area where key_name is not null terminated*/
static void cfstore_dump_key_name(uint8_t* keyname, uint8_t len, const char* tag)
{
     char blob_data[CFSTORE_KEY_NAME_MAX_LENGTH];

     (void) tag;
     assert(keyname != NULL);
     assert(tag != NULL);
     assert(len > 0);
     memcpy(blob_data, keyname, len);
     blob_data[len] = '\0';
     CFSTORE_DBGLOG("%s:keyname=%s\r\n", tag, blob_data);
}

/* @brief  test fsm states and events */
typedef enum cfstore_flash_fsm_state_t {
    cfstore_flash_fsm_state_initializing = 0,
    cfstore_flash_fsm_state_reading,
    cfstore_flash_fsm_state_writing,
    cfstore_flash_fsm_state_committing,
    cfstore_flash_fsm_state_max
} cfstore_flash_fsm_state_t;

/* @brief  test fsm events */
typedef enum cfstore_flash_fsm_event_t {
    cfstore_flash_fsm_event_init_done = 0,
    cfstore_flash_fsm_event_read_done,
    cfstore_flash_fsm_event_write_done,
    cfstore_flash_fsm_event_commit_done,
    cfstore_flash_fsm_event_max,
} cfstore_flash_fsm_event_t;

typedef void (*cfstore_flash_fsm_handler)(void* ctx);

typedef struct cfstore_fsm_t
{
    cfstore_flash_fsm_state_t state;
    cfstore_flash_fsm_event_t event;
} cfstore_fsm_t;

typedef struct cfstore_flash_ctx_t
{
    uint8_t* area_0_head;
    uint8_t* area_0_tail;
    FlashJournal_t jrnl;
    uint64_t expected_blob_size;
    cfstore_fsm_t fsm;
    int32_t status;
    FlashJournal_OpCode_t cmd_code;
    uint64_t expected_read_size;
} cfstore_flash_ctx_t;

/*
 * Globals
 */
static cfstore_flash_ctx_t cfstore_flash_ctx_g;
static const char* cfstore_flash_opcode_str[] =
{
    "FLASH_JOURNAL_OPCODE_INITIALIZE",
    "FLASH_JOURNAL_OPCODE_GET_INFO",
    "FLASH_JOURNAL_OPCODE_READ_BLOB",
    "FLASH_JOURNAL_OPCODE_LOG_BLOB",
    "FLASH_JOURNAL_OPCODE_COMMIT",
    "FLASH_JOURNAL_OPCODE_RESET",
};

#ifdef CFSTORE_DEBUG
static const char* cfstore_flash_state_str[] =
{
    "initializing",
    "reading",
    "writing",
    "committing",
    "unknown"
};

static const char* cfstore_flash_event_str[] =
{
    "init_done",
    "read_done",
    "write_done",
    "commit_done",
    "unknown"
};
#endif /* CFSTORE_DEBUG */

/*
 * Forward decl
 */
static void cfstore_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_flash_fsm_event_t event, void* context);
static void cfstore_fsm_state_set(cfstore_fsm_t* fsm, cfstore_flash_fsm_state_t new_state, void* ctx);

/*
 * context related methods
 */

/* @brief   get a pointer to the global context data structure */
static cfstore_flash_ctx_t* cfstore_flash_ctx_get(void)
{
    return &cfstore_flash_ctx_g;
}


/* @brief   flash journal asynchronous callback handler */
void cfstore_flash_test_01_callback(int32_t status, FlashJournal_OpCode_t cmd_code)
{
    cfstore_flash_ctx_t* ctx = cfstore_flash_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d (%s)\r\n", __func__, (int) status, (int) cmd_code, cfstore_flash_opcode_str[cmd_code]);
    switch(cmd_code)
    {
    case FLASH_JOURNAL_OPCODE_INITIALIZE:
        ctx->fsm.event = cfstore_flash_fsm_event_init_done;
        break;
    case FLASH_JOURNAL_OPCODE_READ_BLOB:
        ctx->fsm.event = cfstore_flash_fsm_event_read_done;
        break;
    case FLASH_JOURNAL_OPCODE_LOG_BLOB:
        ctx->fsm.event = cfstore_flash_fsm_event_write_done;
        break;
    case FLASH_JOURNAL_OPCODE_COMMIT:
        ctx->fsm.event = cfstore_flash_fsm_event_commit_done;
        break;
    case FLASH_JOURNAL_OPCODE_GET_INFO:
    case FLASH_JOURNAL_OPCODE_RESET:
    default:
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: received asynchronous notification for opcode=%d (%s) when api call should have been synchronous", __func__, cmd_code, cmd_code <= FLASH_JOURNAL_OPCODE_RESET ? cfstore_flash_opcode_str[cmd_code] : "unknown");
        TEST_ASSERT_MESSAGE(false, cfstore_flash_utest_msg_g)
        return;
    }
    ctx->status = status;
    ctx->cmd_code = cmd_code;
    cfstore_fsm_state_handle_event(&ctx->fsm, ctx->fsm.event, (void*) ctx);
    return;
}


/* @brief  fsm handler called on entry to initializing state */
static void cfstore_flash_fsm_init_on_entry(void* context)
{
    /* round up cfstore_flash_data_blob_t to nearest k64f program unit size */
    const ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_K64F;
    FlashJournal_Info_t info;
    FlashJournal_Status_t status = JOURNAL_STATUS_ERROR;
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    /* check that the mtd is in synchronous mode */
    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    memset(&info, 0, sizeof(info));

    /* FlashJournal_initialize() is potentially asynchronous */
    status = (FlashJournal_Status_t) FlashJournal_initialize(&ctx->jrnl, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, cfstore_flash_test_01_callback);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to initialize flash journaling layer (status=%d)\r\n", __func__, status);
    TEST_ASSERT_MESSAGE(status >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);
    /* if status > 0, expect async callback, otherwise initialisation has been completed */
    if(status > 0) {
        cfstore_flash_test_01_callback(status, FLASH_JOURNAL_OPCODE_INITIALIZE);
    }
    return;
}

/* brief    callback handler when in state initializing */
static void cfstore_flash_fsm_initializing(void* context)
{
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in initializing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flash_fsm_state_initializing, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, FLASH_JOURNAL_OPCODE_INITIALIZE);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == FLASH_JOURNAL_OPCODE_INITIALIZE, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%d\r\n", __func__, (int) ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);
    /* only change state if status > 0*/
    if(ctx->status > 0){
        cfstore_fsm_state_set(&ctx->fsm, cfstore_flash_fsm_state_reading, ctx);
    }
}

/* static void cfstore_flash_fsm_init_on_exit(void* context){ (void) context;} */

/* brief    callback handler called when entering the reading state
 * note
 *  flash journal has initialised successfully. now
 */
static void cfstore_flash_fsm_read_on_entry(void* context)
{
    uint8_t* ptr = NULL;
    int32_t ret = 0;
    FlashJournal_Info_t info;
    FlashJournal_Status_t status = JOURNAL_STATUS_ERROR;
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_ASSERT(ctx != NULL);
    /* drv->GetInfo() is synchronous */
    status = FlashJournal_getInfo(&ctx->jrnl, &info);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: failed get journal info (status=%d)\r\n", __func__, (int) status);
    TEST_ASSERT_MESSAGE(status == JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);
    CFSTORE_DBGLOG("%s:FlashJournal_getInfo() done. info.sizeofJournaledBlob=%lu\r\n", __func__, (long unsigned int) info.sizeofJournaledBlob);

    if(info.sizeofJournaledBlob > 0)
    {
        /* setup the expected blob size for writing
         * This is a multiple of program unit so the write doesnt fail due to unaligned log */
        ctx->expected_blob_size = sizeof(cfstore_flash_data_blob_t);
        if(ctx->expected_blob_size % CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE > 0){
            ctx->expected_blob_size += (CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE - (sizeof(cfstore_flash_data_blob_t) % CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE));
        }
        /* test that a blob size is the expected size for flash data that has been written before */
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error:  info.sizeofJournaledBlob does not match expect size. expected_blob_size (%lu) != info.sizeofJournaledBlob (%lu)\r\n", __func__, (unsigned long int) ctx->expected_blob_size, (unsigned long int) info.sizeofJournaledBlob);
        TEST_ASSERT_EQUAL_INT64_MESSAGE(ctx->expected_blob_size, info.sizeofJournaledBlob, cfstore_flash_utest_msg_g);

        /* grow the area by the size of the stored blob */
        ptr = (uint8_t*) CFSTORE_REALLOC((void*) ctx->area_0_head, ctx->expected_blob_size);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:realloc failed flash blob (size=%d)\r\n", __func__, (int)info.sizeofJournaledBlob);
        TEST_ASSERT_MESSAGE(ptr != NULL, cfstore_flash_utest_msg_g);
        memset(ptr, 0, ctx->expected_blob_size);
        if(ptr != ctx->area_0_head){
            CFSTORE_DBGLOG("%s:cfstore_ctx_g.area_0_head pointer changed (cfstore_ctx_g.area_0_head=%p, ptr=%p)\r\n", __func__, ctx->area_0_head, ptr);
            ctx->area_0_head = ptr;
            ctx->area_0_tail = ctx->area_0_head + info.sizeofJournaledBlob;
        }
        ret = FlashJournal_read(&ctx->jrnl, (void*) ctx->area_0_head, info.sizeofJournaledBlob);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to read flash journal (ret=%d. info.sizeofJournaledBlob=%d)\r\n", __func__, (int) ret, (int) info.sizeofJournaledBlob);
        TEST_ASSERT_MESSAGE(ret >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);
        if(ret > 0){
            /* read has completed synchronously*/
            CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to read all data from flash journal (expected size=%lu, read=%d)\r\n", __func__, (unsigned long int)info.sizeofJournaledBlob, (int) ret);
            TEST_ASSERT_EQUAL_INT32_MESSAGE( (int32_t) info.sizeofJournaledBlob, ret, cfstore_flash_utest_msg_g);
            cfstore_flash_test_01_callback(ret, FLASH_JOURNAL_OPCODE_READ_BLOB);
        }
    } else {
        /* there is no blob, move to next state. need a +ve status value to indicate async completion
         * to the fsm reading state handler. use CFSTORE_FLASH_AREA_SIZE_MIN for this value */
        ctx->expected_blob_size = CFSTORE_FLASH_AREA_SIZE_MIN;
        status = (FlashJournal_Status_t) CFSTORE_FLASH_AREA_SIZE_MIN;
        cfstore_flash_test_01_callback(status, FLASH_JOURNAL_OPCODE_READ_BLOB);
    }
    return;
}

/* @brief  fsm handler when in reading state */
void cfstore_flash_fsm_reading(void* context)
{
    int32_t ret = 0;
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;
    cfstore_flash_data_blob_t *blob = NULL;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in reading state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flash_fsm_state_reading, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, FLASH_JOURNAL_OPCODE_READ_BLOB);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == FLASH_JOURNAL_OPCODE_READ_BLOB, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%d\r\n", __func__, (int) ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);

    if(ctx->status > 0)
    {
        if(ctx->status > (int32_t) CFSTORE_FLASH_AREA_SIZE_MIN)
        {
            /* check the correct amount of data was read, which is the status code */
            CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to read all data from flash journal (expected size=%lu, read=%d)\r\n", __func__, (unsigned long int) ctx->expected_blob_size, (int) ctx->status);
            /* ctx->status contains the status of the read that was completed */
            TEST_ASSERT_EQUAL_INT64_MESSAGE(ctx->expected_blob_size, (int32_t) ctx->status, cfstore_flash_utest_msg_g);
            if(ctx->area_0_head != NULL)
            {
                /* check the key_name read from flash is correct */
                blob = (cfstore_flash_data_blob_t*) ctx->area_0_head;
                cfstore_dump_key_name(blob->data, CFSTORE_TEST_DATA_KEYNAME_SIZE, __func__);
                ret = memcmp(blob->data, cfstore_flush_test_01_kv_data[0].key_name, strlen(cfstore_flush_test_01_kv_data[0].key_name));
                CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: incorrect key_name read from flash (expected 0 from memcpy(keyname, flash_data), actual was non-zero)", __func__);
                TEST_ASSERT_EQUAL_INT_MESSAGE(0, ret, cfstore_flash_utest_msg_g);
            }
        }
        cfstore_fsm_state_set(&ctx->fsm, cfstore_flash_fsm_state_writing, ctx);
    }
}

/* void cfstore_flash_fsm_read_on_exit(void* context){ (void) context;} */

/* @brief   on entry to writing state, update value */
void cfstore_flash_fsm_write_on_entry(void* context)
{
    uint8_t value = 0;
    int32_t ret = 0;
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;
    cfstore_flash_data_blob_t *blob = NULL;

    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    /* allocate memory for data if not already done so */
    if(ctx->area_0_head == NULL)
    {
        /* This is a multiple of program unit so the write doesnt fail due to unaligned log */
        ctx->expected_blob_size = sizeof(cfstore_flash_data_blob_t);
        if(ctx->expected_blob_size % CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE > 0){
            ctx->expected_blob_size += (CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE - (sizeof(cfstore_flash_data_blob_t) % CFSTORE_FLASH_K64F_CURRENT_PROGRAM_UNIT_SIZE));
        }
        ctx->area_0_head = (uint8_t*) CFSTORE_REALLOC((void*) ctx->area_0_head, ctx->expected_blob_size);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to allocate memory for context\r\n", __func__);
        TEST_ASSERT_MESSAGE(ctx->area_0_head != NULL, cfstore_flash_utest_msg_g);
        ctx->area_0_tail = ctx->area_0_head + ctx->expected_blob_size;
        memset(ctx->area_0_head, 0, ctx->expected_blob_size);
        /* setup data to write to flash */
        blob = (cfstore_flash_data_blob_t*) ctx->area_0_head;
        blob->hdr.klength = strlen(cfstore_flush_test_01_kv_data->key_name);
        blob->hdr.vlength= 1;
        blob->hdr.perm_owner_read = true;
        blob->hdr.perm_owner_write = true;
        blob->hdr.refcount = 1;
        memcpy((void*) blob->data, (const void*) cfstore_flush_test_01_kv_data->key_name, strlen(cfstore_flush_test_01_kv_data->key_name));
        memcpy((void*) &blob->data[CFSTORE_TEST_DATA_KEYNAME_SIZE], (const void*) cfstore_flush_test_01_kv_data->value, strlen(cfstore_flush_test_01_kv_data->value));
    }
    if(ctx->area_0_head != NULL)
    {
        /* data has been read */
        /* check the key_name read from flash is correct */
        blob = (cfstore_flash_data_blob_t*) ctx->area_0_head;
        value = (uint8_t) blob->data[CFSTORE_TEST_DATA_KEYNAME_SIZE];
        CFSTORE_DBGLOG("INFO: value read from flash = %u\r\n", value);
        /* update the value */
        value++;
        memcpy((void*) &blob->data[CFSTORE_TEST_DATA_KEYNAME_SIZE], (const void*) &value, sizeof(uint8_t));
    }

    ret = FlashJournal_log(&ctx->jrnl, (const void*) ctx->area_0_head, ctx->expected_blob_size);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: ret = JOURNAL_STATUS_SMALL_LOG_REQUEST, bailing out.", __func__);
    TEST_ASSERT_MESSAGE(ret != (int32_t) JOURNAL_STATUS_SMALL_LOG_REQUEST, cfstore_flash_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform log operation to flash journal (ret=%d)\r\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 0, cfstore_flash_utest_msg_g);
    if(ret > 0){
        /* write has completed synchronously*/
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to write all data from flash journal (expected size=%lu, read=%d)\r\n", __func__, (unsigned long int) ctx->expected_blob_size, (int) ret);
        TEST_ASSERT_EQUAL_INT32_MESSAGE(ret, (int32_t) ctx->expected_blob_size, cfstore_flash_utest_msg_g);
        cfstore_flash_test_01_callback(ret, FLASH_JOURNAL_OPCODE_LOG_BLOB);
    }
    /* wait for async completion handler*/
}

/* @brief  fsm handler when in reading state */
void cfstore_flash_fsm_writing(void* context)
{
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in writing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flash_fsm_state_writing, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, FLASH_JOURNAL_OPCODE_LOG_BLOB);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == FLASH_JOURNAL_OPCODE_LOG_BLOB, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%d\r\n", __func__, (int) ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);

    if(ctx->status > 0){
        /* check the correct amount of data was written */
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: unable to write all data from flash journal (expected size=%lu, read=%d)\r\n", __func__, (unsigned long int) ctx->expected_blob_size, (int) ctx->status);
        TEST_ASSERT_EQUAL_INT64_MESSAGE(ctx->expected_blob_size, ctx->status, cfstore_flash_utest_msg_g);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_flash_fsm_state_committing, ctx);
    }
    return;
}

/* void cfstore_flash_fsm_write_on_exit(void* ctx){(void) ctx;} */

/* @brief  fsm handler when entering committing state */
void cfstore_flash_fsm_commit_on_entry(void* context)
{
    int32_t ret = 0;
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:\r\n", __func__);
    ret = FlashJournal_commit(&ctx->jrnl);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform commit operation to flash journal (ret=%d)\r\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= (int32_t) JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);
    /* for flash-journal-strategy-sequential version >0.4.0, commit() return no longer reports size of commit block */
    if(ret > 0){
        Harness::validate_callback();
    }
    /* wait for async completion handler*/
    return;
}


/* @brief  fsm handler when in committing state */
void cfstore_flash_fsm_committing(void* context)
{
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: entered handler(%s) but not in committing state (fsm->state=%d)\r\n", __func__, __func__, (int) ctx->fsm.state);
    TEST_ASSERT_MESSAGE(ctx->fsm.state == cfstore_flash_fsm_state_committing, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: cmd_code code (%d) is not as expected (%d)\r\n", __func__, ctx->cmd_code, FLASH_JOURNAL_OPCODE_COMMIT);
    TEST_ASSERT_MESSAGE(ctx->cmd_code == FLASH_JOURNAL_OPCODE_COMMIT, cfstore_flash_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: status=%d\r\n", __func__, (int) ctx->status);
    TEST_ASSERT_MESSAGE(ctx->status >= JOURNAL_STATUS_OK, cfstore_flash_utest_msg_g);

    /* check the correct amount of data was written */
    /* for flash-journal-strategy-sequential version >0.4.0, commit() return no longer reports size of commit block */
    Harness::validate_callback();
}

/* @brief  fsm handler when exiting committing state */
void cfstore_flash_fsm_commit_on_exit(void* context)
{
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    /* test done. clean up*/
    if(ctx->area_0_head){
        CFSTORE_FREE(ctx->area_0_head);
        ctx->area_0_head = NULL;
    }
    Harness::validate_callback();
 }

#define cfstore_flash_fsm_null  NULL

/* handler functions while in state */
static cfstore_flash_fsm_handler cfstore_flash_fsm[cfstore_flash_fsm_state_max][cfstore_flash_fsm_event_max] =
{
/* state\event:     init_done                           read_done                   write_done                  commit_done */
/* initialising */  {cfstore_flash_fsm_initializing,    cfstore_flash_fsm_null,     cfstore_flash_fsm_null,     cfstore_flash_fsm_null          },
/* reading      */  {cfstore_flash_fsm_null,            cfstore_flash_fsm_reading,  cfstore_flash_fsm_null,     cfstore_flash_fsm_null          },
/* writing      */  {cfstore_flash_fsm_null,            cfstore_flash_fsm_null,     cfstore_flash_fsm_writing,  cfstore_flash_fsm_null          },
/* committing   */  {cfstore_flash_fsm_null,            cfstore_flash_fsm_null,     cfstore_flash_fsm_null,     cfstore_flash_fsm_committing    },
};


/* handler functions for entering the state*/
cfstore_flash_fsm_handler cfstore_flash_fsm_on_entry[cfstore_flash_fsm_state_max] =
{
    cfstore_flash_fsm_init_on_entry,
    cfstore_flash_fsm_read_on_entry,
    cfstore_flash_fsm_write_on_entry,
    cfstore_flash_fsm_commit_on_entry,
};

/* handler functions for exiting state, currently none used */
cfstore_flash_fsm_handler cfstore_flash_fsm_on_exit[cfstore_flash_fsm_state_max] =
{
    NULL, NULL, NULL, NULL,
};


/* @brief   inject event into fsm */
static void cfstore_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_flash_fsm_event_t event, void* context)
{
    cfstore_flash_ctx_t* ctx = (cfstore_flash_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, event=%d (%s), ctx=%p\r\n", __func__, fsm, (int) event, cfstore_flash_event_str[event], ctx);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_FLASH_UTEST_MSG_BUF_SIZE, "%s:Error: invalid event (%d)\r\n", __func__, (int) event);
    TEST_ASSERT_MESSAGE(event < cfstore_flash_fsm_event_max, cfstore_flash_utest_msg_g);
    fsm->event = event;
    if(cfstore_flash_fsm[fsm->state][fsm->event] != NULL){
        cfstore_flash_fsm[fsm->state][fsm->event](ctx);
    }

    /* do not clear context data set by caller as it may be used later
     *  fsm->event = cfstore_flash_fsm_event_max;
     *  ctx->status = 0;
     *  ctx->cmd_code =  (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);
     */
    return;
}


/* @brief   function to move to new fsm state, calling state exit function for old state and entry function for new state */
static void cfstore_fsm_state_set(cfstore_fsm_t* fsm, cfstore_flash_fsm_state_t new_state, void* ctx)
{
    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, new_state=%d, ctx=%p\r\n", __func__, fsm, (int) new_state, ctx);
    CFSTORE_DBGLOG("%s:FSM:REQ RX:%s:%s\r\n", __func__, cfstore_flash_state_str[fsm->state], cfstore_flash_state_str[new_state]);
    TEST_ASSERT_MESSAGE(fsm != NULL, "fsm is not a valid pointer");
    TEST_ASSERT_MESSAGE(new_state < cfstore_flash_fsm_state_max, "new_state is not a valid state");
    TEST_ASSERT_MESSAGE(ctx != NULL, "ctx is not a valid pointer");
    TEST_ASSERT_MESSAGE(fsm->state < cfstore_flash_fsm_state_max, "fsm->state is not a valid state");

    if(cfstore_flash_fsm_on_exit[fsm->state] != NULL){
        cfstore_flash_fsm_on_exit[fsm->state](ctx);
    }
    fsm->state = new_state;
    if(cfstore_flash_fsm_on_entry[new_state] != NULL){
        cfstore_flash_fsm_on_entry[new_state](ctx);
    }
    CFSTORE_DBGLOG("%s:FSM:REQ DONE:\r\n", __func__);
    return;
}

/* @brief   initialize global context data structure */
static void cfstore_flash_ctx_init(cfstore_flash_ctx_t* ctx)
{
    TEST_ASSERT_MESSAGE(ctx != NULL, "ctx is NULL");

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&cfstore_flash_ctx_g, 0, sizeof(cfstore_flash_ctx_g));
}


/* @brief   asynchronous test 01 */
static control_t cfstore_flash_journal_async_test_01(void)
{
    cfstore_flash_ctx_t* ctx = cfstore_flash_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: \r\n", __func__);
    cfstore_flash_mtd_async_ops_g = CFSTORE_FLASH_MTD_ASYNC_OPS_ON;
    cfstore_flash_ctx_init(ctx);
    cfstore_fsm_state_set(&ctx->fsm, cfstore_flash_fsm_state_initializing, ctx);

    /* allow time for work to be done */
    return CaseTimeout(CFSTORE_FLASH_CASE_TIMEOUT_MS);
}

#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */


/* report whether built/configured for flash sync or async mode */
static control_t cfstore_flash_test_00(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;

    (void) call_count;
    ret = cfstore_test_startup();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_flash_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: failed to perform test startup (ret=%d).\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_flash_utest_msg_g);

#ifndef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
    CFSTORE_LOG("INITIALIZING: BACKEND=SRAM. Skipping flash test%s", "\n");
#endif
    return CaseNext;
}

/// @cond CFSTORE_DOXYGEN_DISABLE
/* Specify all your test cases here */
Case cases[] = {
        Case("flash_journal_async_test_00", cfstore_flash_test_00),
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
        Case("flash_journal_async_test_01", cfstore_flash_journal_async_test_01),
#endif
};


utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(100, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);


int main()
{
    return !Harness::run(specification);
}
/// @endcond
