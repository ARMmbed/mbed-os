/** @file configuration_store.c
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
 */

// This file is deprecated so deprecation warnings when building it are silenced
#if   defined ( __CC_ARM )
#pragma diag_suppress 1361  // Deprecated declaration
#elif defined ( __GNUC__ )
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include "cfstore_config.h"
#include "cfstore_debug.h"
#include "cfstore_list.h"
#include "cfstore_fnmatch.h"
#include "configuration_store.h"

#if defined CFSTORE_CONFIG_MBED_OS_VERSION && CFSTORE_CONFIG_MBED_OS_VERSION == 3
#include <core-util/critical.h>
#endif /* CFSTORE_CONFIG_MBED_OS_VERSION == 3 */

#ifdef YOTTA_CFG_CFSTORE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
#include "cfstore_svm.h"
#include "flash_journal_strategy_sequential.h"
#include "flash_journal.h"
#include "Driver_Common.h"
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>


#ifdef CFSTORE_DEBUG
uint32_t cfstore_optDebug_g = 1;
uint32_t cfstore_optLogLevel_g = CFSTORE_LOG_NONE; /*CFSTORE_LOG_NONE|CFSTORE_LOG_ERR|CFSTORE_LOG_DEBUG|CFSTORE_LOG_FENTRY */
uint32_t cfstore_optLogTracepoint_g = CFSTORE_TP_NONE; /*CFSTORE_TP_NONE|CFSTORE_TP_CLOSE|CFSTORE_TP_CREATE|CFSTORE_TP_DELETE|CFSTORE_TP_FILE|CFSTORE_TP_FIND|CFSTORE_TP_FLUSH|CFSTORE_TP_INIT|CFSTORE_TP_OPEN|CFSTORE_TP_READ|CFSTORE_TP_WRITE|CFSTORE_TP_VERBOSE1|CFSTORE_TP_VERBOSE2|CFSTORE_TP_VERBOSE3|CFSTORE_TP_FENTRY; */
#endif


/*
 * Externs
 */
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
ARM_DRIVER_STORAGE *cfstore_storage_drv = &ARM_Driver_Storage_MTD_K64F;
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

struct _ARM_DRIVER_STORAGE cfstore_journal_mtd;

/*
 * Defines
 *
 * CFSTORE_FLASH_STACK_BUF_SIZE
 *  when performing flush, if the program_unit <= CFSTORE_FLASH_STACK_BUF_SIZE octets then a
 *  stack buffer is used to perform the tail write. Otherwise a buffer is malloced
 *
 * CFSTORE_FLASH_AREA_SIZE_MIN
 *  valid sizes of areas should always be greater than the size of the header, and therefore
 *  greater than this value, which is defined as smaller than the header size
 *
 * CFSTORE_FLASH_NUMSLOTS
 *  number of flash journal slots
 *
 * ARM_DRIVER_OK_DONE
 *   value that indicates an operation has been done i.e. a value > 0
 */
#define CFSTORE_KEY_NAME_CHARS_ACCEPTABLE           "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ{}.-_@"
#define CFSTORE_KEY_NAME_QUERY_CHARS_ACCEPTABLE     CFSTORE_KEY_NAME_CHARS_ACCEPTABLE"*"
#define CFSTORE_HKVT_REFCOUNT_MAX                   0xff
#define CFSTORE_LOCK_REFCOUNT_MAX                   0xffff
#define CFSTORE_FILE_CREATE_MODE_DEFAULT            (ARM_CFSTORE_FMODE)0
#define CFSTORE_FLASH_STACK_BUF_SIZE 	            64
#define CFSTORE_FLASH_AREA_SIZE_MIN                 (sizeof(cfstore_area_header_t) - 1)
#define CFSTORE_FLASH_NUMSLOTS                      4
#define cfstore_fsm_null                            NULL
#define CFSTORE_SENTINEL                            0x7fffffff
#define CFSTORE_CALLBACK_RET_CODE_DEFAULT           0x1
#define ARM_DRIVER_OK_DONE                          1

/*
 * Simple Types
 */
#define CFSTORE_LOCK uint32_t


/*
 * Structures
 */

/** @brief
 *
 * @param   key_permissions
 *          bottom 6 bits contain the ACLs-bits (owner read/write/execute,
 *           other read/write/execute). The remaining bits in this field are
 *          used for the Device Data Security Protection Features bit field,
 *          bits are low-active
 * @param   perm_owner_read
 *          if set => this KV is owner readable
 * @param   perm_owner_write
 *          if set => this KV is owner writable
 * @param   perm_owner_execute
 *          if set => this KV is owner executable
 * @param   perm_other_read
 *          if set => this KV is world readable
 * @param   perm_other_write
 *          if set => this KV is world writable
 * @param   perm_other_execute
 *          if set => this KV is world executable
 * @param   klength
 *          key name size including zero-padding
 * @param   vlength
 *          this value fragment length
 * @param   refcount
 *          Number of handles open on this hkvt
 *
 * @param   delete
 *          indicates this KV is being deleted
 */
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
        uint8_t delete : 1;
        uint8_t reserved : 7;
    } flags ;
} cfstore_area_header_t;


/* helper struct */
typedef struct cfstore_area_hkvt_t
{
    uint8_t *head;
    uint8_t *key;
    uint8_t *value;
    uint8_t *tail;
} cfstore_area_hkvt_t;


/* helper struct */
typedef struct cfstore_client_notify_data_t
{
    uint32_t opcode;
    int32_t status;
    ARM_CFSTORE_HANDLE handle;
} cfstore_client_notify_data_t;

/* @brief  test fsm states and events */
typedef enum cfstore_fsm_state_t {
    cfstore_fsm_state_stopped = 0,
    cfstore_fsm_state_initing,
    cfstore_fsm_state_reading,
    cfstore_fsm_state_logging,
    cfstore_fsm_state_committing,
    cfstore_fsm_state_resetting,
    cfstore_fsm_state_ready,        /* ready for next flash journal command to arise */
    cfstore_fsm_state_formatting,   /* flash formatting in progress */
    cfstore_fsm_state_max
} cfstore_fsm_state_t;

/* @brief  test fsm events */
typedef enum cfstore_fsm_event_t {
    cfstore_fsm_event_init_done = 0,
    cfstore_fsm_event_read_done,
    cfstore_fsm_event_log_done,
    cfstore_fsm_event_commit_req,
    cfstore_fsm_event_commit_done,
    cfstore_fsm_event_reset_done,
    cfstore_fsm_event_format_done,
    cfstore_fsm_event_max,
} cfstore_fsm_event_t;

typedef int32_t (*cfstore_fsm_handler)(void* ctx);

/* @brief   flash finite state machine helper function */
typedef struct cfstore_fsm_t
{
    cfstore_fsm_state_t state;
    cfstore_fsm_event_t event;
} cfstore_fsm_t;


#ifdef CFSTORE_DEBUG
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
/* strings used for debug trace */
static const char* cfstore_flash_opcode_str[] =
{
    "FLASH_JOURNAL_OPCODE_FORMAT",
    "FLASH_JOURNAL_OPCODE_INITIALIZE",
    "FLASH_JOURNAL_OPCODE_GET_INFO",
    "FLASH_JOURNAL_OPCODE_READ_BLOB",
    "FLASH_JOURNAL_OPCODE_LOG_BLOB",
    "FLASH_JOURNAL_OPCODE_COMMIT",
    "FLASH_JOURNAL_OPCODE_RESET",
};

static const char* cfstore_flash_state_str[] =
{
    "stopped",
    "initializing",
    "reading",
    "logging",
    "committing",
    "resetting",
    "ready",
    "formatting",
    "unknown"
};

static const char* cfstore_flash_event_str[] =
{
    "init_done",
    "read_done",
    "log_done",
    "commit_req",
    "commit_done",
    "reset_done",
    "format_done",
    "unknown"
};
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */
#endif /* CFSTORE_DEBUG */


/*
 * Forward decl
 */
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
static int32_t cfstore_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_fsm_event_t event, void* context);
static int32_t cfstore_fsm_state_set(cfstore_fsm_t* fsm, cfstore_fsm_state_t new_state, void* ctx);
#endif  /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */
static int32_t cfstore_get_key_name_ex(cfstore_area_hkvt_t *hkvt, char* key_name, uint8_t *key_name_len);


/* Walking Area HKVT's While Inserting   a New HKVT:
 * Implementation Note 1 [NOTE1]
 *
 * The implementation must address the following problem:
 * - client1 may be creating a new KV into area_0, which means inserting the
 *   header-key-value-tail data into area_0.
 * - concurrently, client2 (through a call to Find()) is walking KVs in area_0,
 *   and the walk has to be safe against the insertion of the new KV.
 *
 * This problem is addressed in the by using the cfstore_ctx_g.rw_lock to police
 * access to the area when making changes.
 * - Walking the KVs in area_0 is performed using the header structures,
 *   which contain key and value lengths required to find the start of the
 *   next hkvt. These must not change under the client.
 * - The Find() walk is terminated when the hkvt header pointer is found to
 *   point to cfstore_ctx_g.area_0_tail i.e. when this arises then the
 *   iterator knows its come to the end of the hkvt's in the area.
 *
 * Memory Management (todo: future support)
 * Implementation Note 2 [NOTE2]
 * CFSTORE supports using a client provisioned SRAM slab rather than using realloc() to allocated heap
 * memory. This has the following advantages:
 * - the client is in control of the memory allocation.
 * - realloc() cannot fail (e.g. due to memory leaks losing memory) as the sram has been preprovisioned.
 *   This makes the system more resilient.
 * The client specifes the sram slab in the following way:
 * - having target.json defined yotta_config.h symbol for CFSTORE_SRAM_START_ADDR, CFSTORE_SRAM_SIZE
 *   and #ifdef on these values to use that memory area for area_0 rather than using malloc.
 * - for the case where a client tries to create a KV which causes area_0 to exceed CFSTORE_SRAM_SIZE
 *   then the operation is failed.
 * - modify the API so that the client is responsible for allocating the memory the the CFSTORE internal
 *   data structures, with the size of the internal data structure exposed through a #define.
 *   The contents of the buffer are opaque to the client. The reasons for this are as follows:
 *   - to allow the cfstore implementation not to use malloc().
 *   - the memory allocation policy for allocating the memory of CFSTORE internal data structures
 *     can be decided and implemented by the client
 *   - for clients written in C++, its possible to have a static class with the memory for the
 *     internal context, and the static class memory area is given to CFSTORE for use, so it
 *     provides good C++ support.
 *   - The SRAM area can be allocated with the previous point, and the handle associated data
 *     structures i.e. cfstore_file_t, can be covered by the supplied buffers to those functions
 *     creating handles.
 * - currently neither target.json nor config.json allow a symbol in yotta_config.h to be defined
 *   for the current case of CFSTORE being a yotta module/library.
 *
 * UVISOR Integration (todo)
 * Implementation Note 3 [NOTE3]
 * Outstanding Questions:
 * - uvisor_ctx. Should all functions use this to access the global data context?
 *   - see cfstore_ctx_get() for an implementation
 *   - compile in cfstore_ctx_g only when not using uvisor
 * - how do you allocate heap memory objects with uvisor protections?
 *   - doesnt seem to be an api for this yet.
 *   - will be required for sram storage of KVs i.e. "the area".
 *   - will be required for file objects
 * - Q: is it safe to store the caller_box_id in the cfstore_file_t?
 *   A: no, because the cfstore_file_t is held in client controlled memory (opaque hkey)
 *      so the client can modify from under cfstore, breaching security if it was used
 *      by other cfstore methods.
 * - method for securing access:
 *   - create()/open() checks namespace etc, and then creates/opens cfstore_file_t
 *     and returns hkey (opaque cfstore_file_t) for subsequent use by api calls.
 *   - read/write/rseek etc check the kv pathname accessible via cfstore_file_t::head
 *     is within the callers namespace.
 *   - we are trusting the caller to be secure and not be malicious?
 *  - put "uvisor-lib" : "^2.0.0" in module.json. not necessary as mbed-drivers has this dep.
 *  - flash-journal change from using NVIC_Set/GetVector() to VIRQ_Set/GetVector()
 *
 */

/*
 * @brief   CS global context that maintains state
 *
 * @param   area_0_start
 *          pointer to start of malloc-ed memory block for containing area_0
 *
 * @param   area_0_head
 *          pointer to area_0 header struct within the memblock.
 *          - ((cfstore_area_header_t*) area_0)->refcount is the number of
 *            open handles in the whole of area_0.
 *          - accessed in app & intr context; hence needs CS protection.
 *
 * @param   area_0_tail
 *          pointer to address in the sram after the last byte of the last
 *          KV. Note there can be padding after the area_0_tail to align the
 *          sram area with flash program_unit (or 1 if SRAM only version)
 *          to facilitate reading/writing to flash.
 *          - accessed in app & intr context; hence needs CS protection.
 *
 * @param   area_0_len
 *          length of the area used for storing KVs, including padding to
 *          round to nearest program unit
 *
 * @param   rw_area0_lock
 *          lock used to make CS re-entrant e.g. only 1 flush operation can be
 *          performed at a time while no readers/writers have handles open
 *          to KVs. The lock is to protect access to the following:
 *          - cfstore_ctx_g.area_0_head/cfstore_ctx_g.area_0_tail. Realloc()
 *            in Delete() and Create() can cause these pointers to change.
 *
 * @param   client_notify_data
 *          fsm handler functions set a flag for a client notification call
 *          to be made after fsm handler functions have been completed. This
 *          block holds the client notification status data for the callback.
 *
 * @param   area_dirty_flag
 *          flag indicating that the area has been written and therefore is
 *          dirty with respect to the data persisted to flash.
 *
 * @expected_blob_size  expected_blob_size = area_0_tail - area_0_head + pad
 *          In the case of reading from flash into sram, this will be be size
 *          of the flash blob (rounded to a multiple program_unit if not
 *          already so).
 *          In the case of writing to flash, this the size of all the KV's
 *          plus padding so the sram blob size is a multiple of flash
 *          program_unit.
 *          - accessed in app & intr context; hence needs CS protection.
 */
typedef struct cfstore_ctx_t
{
    cfstore_list_node_t file_list;
    int32_t init_ref_count;
    CFSTORE_LOCK rw_area0_lock;
    ARM_POWER_STATE power_state;
    uint8_t *area_0_head;
    uint8_t *area_0_tail;
    size_t area_0_len;
    cfstore_fsm_t fsm;
    int32_t status;

    /* client notification data */
    void* client_context;
    ARM_CFSTORE_CALLBACK client_callback;
    cfstore_client_notify_data_t client_notify_data;

    /* flags */
    uint32_t client_callback_notify_flag : 1;
    uint32_t area_dirty_flag : 1;
    uint32_t f_reserved0 : 30;

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
    /* flash journal related data */
    FlashJournal_t jrnl;
    FlashJournal_Info_t info;
    FlashJournal_OpCode_t cmd_code;
    uint64_t expected_blob_size;
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */
} cfstore_ctx_t;


/*
 * @brief   file structure for KV, one per open file handle.
 *
 * @param   head
 *          pointer to head of KV
 *
 * @param   rlocation
 *          read location of rseek to move
 *
 * @param   read
 *          indicates file is readable,
 * @param   writable
 *          indicates file is readable,
 * @param   executable
 *          indicates file is readable,
 * @param 	uvisor_client_box_id
 * 			box id of caller using this file. set on create/open and thereafter used by other methods to check accesses.
 * 			Q: is it safe to store this here? Is it of any value? i.e. a client can change the value
 * 			after cfstore has set it so cfstore cant rely on it being secure.
 */
typedef struct cfstore_file_t
{
    cfstore_list_node_t node;
    uint32_t rlocation;
    uint32_t wlocation;
    uint8_t *head;
    ARM_CFSTORE_FMODE flags;
#ifdef YOTTA_CFG_CFSTORE_UVISOR
    // todo: add this into mix.
    //int uvisor_client_box_id;
#endif

} cfstore_file_t;

/* @brief   structure used to compose table for mapping flash journal error codes to cfstore error codes */
typedef struct cfstore_flash_journal_error_code_node
{
    int32_t flash_journal_error_code;
    int32_t cfstore_error_code;
} cfstore_flash_journal_error_code_node;


/*
 * Globals
 */
#ifndef CFSTORE_STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS
static ARM_CFSTORE_CAPABILITIES cfstore_caps_g = { .asynchronous_ops = 1, .uvisor_support_enabled = 0 };
#else
static ARM_CFSTORE_CAPABILITIES cfstore_caps_g = { .asynchronous_ops = CFSTORE_STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS, .uvisor_support_enabled = 0 };
#endif /* CFSTORE_STORAGE_DRIVER_CONFIG_HARDWARE_MTD_ASYNC_OPS */

static const ARM_DRIVER_VERSION cfstore_driver_version_g = { .api = ARM_CFSTORE_API_VERSION, .drv = ARM_CFSTORE_DRV_VERSION };

#ifndef YOTTA_CFG_CFSTORE_UVISOR
/* uvisor is not being used so instantiate a context */
static cfstore_ctx_t cfstore_ctx_g = {
        .file_list.next = NULL,
        .file_list.prev = NULL,
        .init_ref_count = 0,
        .rw_area0_lock = 0,
        .power_state = ARM_POWER_FULL,
        .area_0_head = NULL,
        .area_0_tail = NULL,
        .client_callback = NULL,
        .client_context = NULL,
        .f_reserved0 = 0,
};
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

#ifdef YOTTA_CFG_CFSTORE_UVISOR

/*
 * Configure the secure box compartment
 */
static const char const cfstore_uvisor_namespace_root_g[] = "com.arm.mbed.";
// UVISOR_BOX_NAMESPACE("com.arm.mbed.configuration-store");
// macro: static const char *const __uvisor_box_namespace = box_namespace
static const char *const __uvisor_box_namespace = "com.arm.mbed.configuration-store";

/* although the descriptor is empty, the main box descriptor is inherited and added to whats here. */
static const UvisorBoxAclItem cfstore_acl_list_g[] = {
    /* todo: this needs completing with correct data for the secure flash partition above the binary
     *
     0xabaadfood = start of secure flash in address map (flash journal partition
     0xbeefbeef = size in bytes of secure flash partition
     {(void *) 0xabaadfood, 0xbeefbeef, UVISOR_TACLDEF_PERIPH},
     */
     /* put reference to k64 subfamily reference manual and cmsis k64f target header as to where this comes from */
     {FTFE,    sizeof(*FTFE),    UVISOR_TACLDEF_PERIPH},
};

/* UVISOR_BOX_CONFIG_CTX(configuration_store, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 *
 * It would be better to use the following macro:
 * 	UVISOR_BOX_CONFIG(configuration_store, cfstore_acl_list_g, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 * rather than the unpacked macro code that follows.
 *
 * #define __UVISOR_BOX_CONFIG(box_name, acl_list, acl_list_count, stack_size, context_size) \
 *     \
 *     uint8_t __attribute__((section(".keep.uvisor.bss.boxes"), aligned(32))) \
 *         box_name ## _reserved[UVISOR_STACK_SIZE_ROUND(((UVISOR_MIN_STACK(stack_size) + (context_size))*8)/6)]; \
 *     \
 *     static const __attribute__((section(".keep.uvisor.cfgtbl"), aligned(4))) UvisorBoxConfig box_name ## _cfg = { \
 *         UVISOR_BOX_MAGIC, \
 *         UVISOR_BOX_VERSION, \
 *         UVISOR_MIN_STACK(stack_size), \
 *         context_size, \
 *         __uvisor_box_namespace, \
 *         acl_list, \
 *         acl_list_count \
 *     }; \
 *     \
 *     extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const box_name ## _cfg_ptr = &box_name ## _cfg;
 *
 * However, the macro currently generates warnings that need to be fixed i.e.
 * =====================================================================================================================================================================================
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:490:1: error: initializer element is not constant
 * UVISOR_BOX_CONFIG(configuration_store, cfstore_acl_list_g, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 * ^
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:490:1: error: (near initialization for 'configuration_store_cfg.box_namespace')
 * In file included from d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/yotta_modules/uvisor-lib/uvisor-lib/uvisor-lib.h:38:0,
 * 		from d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:27:
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:490:19: warning: 'configuration_store_cfg_ptr' initialized and declared 'extern'
 * UVISOR_BOX_CONFIG(configuration_store, cfstore_acl_list_g, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 *                    ^
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/yotta_modules/uvisor-lib/uvisor-lib/box_config.h:74:95: note: in definition of macro '__UVISOR_BOX_CONFIG'
 *      extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const box_name ## _cfg_ptr = &box_name ## _cfg;
 *                                                                                                     ^
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/yotta_modules/uvisor-lib/uvisor-lib/box_config.h:57:55: note: in expansion of macro '__UVISOR_BOX_CONFIG_CONTEXT'
 * #define __UVISOR_BOX_MACRO(_1, _2, _3, _4, NAME, ...) NAME
 *                                                        ^
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/yotta_modules/uvisor-lib/uvisor-lib/box_config.h:101:5: note: in expansion of macro 'UVISOR_BOX_CONFIG_ACL'
 *      UVISOR_BOX_CONFIG_ACL(__VA_ARGS__)
 *           ^
 * d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:490:1: note: in expansion of macro 'UVISOR_BOX_CONFIG'
 * UVISOR_BOX_CONFIG(configuration_store, cfstore_acl_list_g, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 * ^
 * ninja: build stopped: subcommand failed.
 * error: command ['ninja'] failed
 * =====================================================================================================================================================================================
 * The UVISOR_BOX_CONFIG() macro expands to include the following:
 * 		extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const configuration_store_cfg_ptr = &configuration_store_cfg;
 * The extern at the beginning of the line creates a warning when in a c file, and so needs to be removed/fixed.
 * There are also many other warnings from the macro expansion which need to be investigated further.
 *
 * todo: possible investigation: move configuration_store.c -> configuration_store.cpp
 */
uint8_t __attribute__((section(".keep.uvisor.bss.boxes"), aligned(32))) configuration_store_reserved[UVISOR_STACK_SIZE_ROUND(((UVISOR_MIN_STACK(UVISOR_BOX_STACK_SIZE) + (sizeof(cfstore_ctx_t)))*8)/6)];
static const __attribute__((section(".keep.uvisor.cfgtbl"), aligned(4))) UvisorBoxConfig configuration_store_cfg = {
	UVISOR_BOX_MAGIC,
	UVISOR_BOX_VERSION,
	UVISOR_MIN_STACK(UVISOR_BOX_STACK_SIZE),
	sizeof(cfstore_ctx_t),
	"com.arm.mbed.configuration-store", //problem using__uvisor_box_namespace defined above so inserting string directly here
	cfstore_acl_list_g,
	UVISOR_ARRAY_COUNT(cfstore_acl_list_g)
};

const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const configuration_store_cfg_ptr = &configuration_store_cfg;
UVISOR_EXTERN cfstore_ctx_t * const uvisor_ctx;

#endif /* YOTTA_CFG_CFSTORE_UVISOR */

/*
 * client notifier helper function
 */
static void cfstore_client_notify_data_init(cfstore_client_notify_data_t* data, uint32_t opcode, int32_t status, ARM_CFSTORE_HANDLE handle)
{
    memset(data, 0, sizeof(cfstore_client_notify_data_t));
    data->opcode = opcode;
    data->status = status;
    data->handle = handle;
}

/*
 * cfstore_ctx_t methods
 */

/* @brief   helper function to report whether the initialisation flag has been set in the cfstore_ctx_g */
static bool cfstore_ctx_is_initialised(cfstore_ctx_t* ctx)
{
    CFSTORE_ASSERT(ctx!= NULL);
    return ctx->init_ref_count > 0 ? true : false;
}

/* @brief   helper function to return a pointer to the global cfstore context. */
static inline cfstore_ctx_t* cfstore_ctx_get(void)
{
#ifdef YOTTA_CFG_CFSTORE_UVISOR
	/* use the secure cfstore_ctx_t struct allocated by uvisor for use */
	return (cfstore_ctx_t*) uvisor_ctx;
#else
	/* use the insecure statically allocated data struct */
	return &cfstore_ctx_g;
#endif
}

/** @brief   helper function to compute the total size of the KVs stored in the
 *           sram area in bytes.
 *
 * Note:
 *  - sram_area_size =  cfstore_ctx_get_kv_total_len() + padding
 *  - padding rounds up cfstore_ctx_get_kv_total_len() to
 *    be a multiple of flash program_unit size.
 */
static ARM_CFSTORE_SIZE cfstore_ctx_get_kv_total_len(void)
{
    ARM_CFSTORE_SIZE size = 0;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    size = (ARM_CFSTORE_SIZE) (ctx->area_0_tail - ctx->area_0_head);
    return size;
}

/* @brief   helper function to get the program_unit */
static inline uint32_t cfstore_ctx_get_program_unit(cfstore_ctx_t* ctx)
{
    CFSTORE_ASSERT(ctx!= NULL);
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
    return ctx->info.program_unit;
#else
    /* the program unit is 1 so byte aligned when no flash backend present */
    (void) ctx;
    return 1;
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

}

static inline void cfstore_ctx_client_notify(cfstore_ctx_t* ctx, cfstore_client_notify_data_t* data)
{
    CFSTORE_FENTRYLOG("%s:entered: ctx=%p, ctx->client_callback=%p, ctx->client_context=%p\n", __func__, ctx, ctx->client_callback, ctx->client_context);
    CFSTORE_TP(CFSTORE_TP_CALLBACK, "%s:data=%p, data->opcode=%d, data->status=%d, data->handle=%p\n", __func__, data, (int) data->opcode, (int) data->status, data->handle);
    if(ctx->client_callback){
        ctx->client_callback(data->status, (ARM_CFSTORE_OPCODE) data->opcode, ctx->client_context, data->handle);
    }
    return;
}

/*
 * CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR
 *  client can supply a SRAM slab address and size for
 *  CFSTORE internal use. This is a default addr
 *  for development use. Should be defined by client
 * CFSTORE_YOTTA_CFG_CFSTORE_SRAM_SIZE
 *  size of sram area. Should be define by client
 */
#ifndef CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR
/* if the client doesnt provide a memory slab then CFSTORE uses realloc internally*/

#ifndef CFSTORE_DEBUG
#define CFSTORE_FREE        free
#define CFSTORE_MALLOC      malloc
#define CFSTORE_REALLOC     realloc
#else

static uint32_t cfstore_malloc_size_g = 0;
#define CFSTORE_MALLOC      malloc

static void* CFSTORE_REALLOC(void *ptr, size_t size)
{
    void* mem;

    mem = realloc(ptr, size);
    CFSTORE_TP(CFSTORE_TP_MEM, "%s:ptr=%p, mem=%p, old_size=%u, new_size=%u.\n", __func__, ptr, mem, (int) cfstore_malloc_size_g, (int) size);
    cfstore_malloc_size_g = size;
    return mem;
}

static void CFSTORE_FREE(void *ptr)
{
    free(ptr);
    CFSTORE_TP(CFSTORE_TP_MEM, "%s:ptr=%p, old_size=%u, new_size=%u.\n", __func__, ptr, (int) cfstore_malloc_size_g, 0);
    cfstore_malloc_size_g = 0;
    return;
}
#endif /* CFSTORE_DEBUG */

/* memory tracking */

#else
#define CFSTORE_FREE        CFSTORE_ASSERT(0)
#define CFSTORE_MALLOC      CFSTORE_ASSERT(0)
#define CFSTORE_REALLOC     cfstore_realloc



/* function to realloc from a client provided memory slab
 * size = new size of area used by sram
 * ptr is always head of slab
 *
 * The cfstore_realloc() function changes the size of the memory
 * block pointed to by ptr to size bytes, backed by the client
 * provided memory slab. The contents will be unchanged in the
 * range from the start of the region up to the minimum of the
 * old and new sizes. If the new size is larger than the old size,
 * the added memory will not be initialized.
 *
 * ptr
 * ptr should be set to null on the first call to this function and
 * for size > 0 && size <= CFSTORE_YOTTA_CFG_CFSTORE_SRAM_SIZE
 * CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR will be returned.
 * On subsequent calls, ptr must have been returned by an earlier
 * call to this function.
 *
 * size
 * if size is equal to zero, and ptr is not NULL, then the call is
 * equivalent to reseting the memory area and NULL will be returned.
 */
void *cfstore_realloc(void *ptr, ARM_CFSTORE_SIZE size)
{
    static uint8_t *cfstore_sram_head = NULL;
    static uint8_t *cfstore_sram_tail = NULL;

    if(size > 0) {
        if(size <= CFSTORE_YOTTA_CFG_CFSTORE_SRAM_SIZE) {
            if(ptr == NULL) {
                memset(CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR, 0, CFSTORE_YOTTA_CFG_CFSTORE_SRAM_SIZE);
                cfstore_sram_head = CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR;
            }
            cfstore_sram_tail = cfstore_sram_head + size;
            return (void*) cfstore_sram_head;
        }
        /* requested size is too big so fail the operation by setting
         * head/tail to NULL */
    }
    /* size == 0 => reset */
    cfstore_sram_head = NULL;
    cfstore_sram_tail = NULL;
    return (void*) cfstore_sram_head;
}

#endif /* CFSTORE_YOTTA_CFG_CFSTORE_SRAM_ADDR */


#ifdef CFSTORE_TARGET_LIKE_X86_LINUX_NATIVE
static inline void cfstore_critical_section_lock(CFSTORE_LOCK* lock, const char* tag){ (void) tag; __sync_fetch_and_add(lock, 1); }
static inline void cfstore_critical_section_unlock(CFSTORE_LOCK* lock, const char* tag){(void) tag;  __sync_fetch_and_sub(lock, 1); }

static CFSTORE_INLINE int32_t cfstore_hkvt_refcount_dec(cfstore_area_hkvt_t* hkvt, uint8_t *refcount)
{
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;
    uint32_t __refcount;

    __refcount =__sync_fetch_and_sub(&hdr->refcount, 1);
    if(refcount) *refcount = __refcount;
    return ARM_DRIVER_OK;
}

static CFSTORE_INLINE int32_t cfstore_hkvt_refcount_inc(cfstore_area_hkvt_t* hkvt, uint8_t *refcount)
{
    int32_t ret = ARM_CFSTORE_DRIVER_ERROR_HANDLE_COUNT_MAX;
    uint32_t __refcount;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    if( (__refcount = __sync_fetch_and_add(&hdr->refcount, 1)) < CFSTORE_LOCK_REFCOUNT_MAX) {
        if(refcount) *refcount = __refcount;
        ret = ARM_DRIVER_OK;
    } else {
        /* maximum count reach, back down and return error*/
        __sync_fetch_and_sub(&hdr->refcount, 1);
    }
    return ret;
}


#else

/*
 * Platform Specific Function Implementations
 */

static inline void cfstore_critical_section_unlock(CFSTORE_LOCK* lock, const char* tag)
{
    (void) lock;
    (void) tag;
    CFSTORE_DBGLOG("%s:before critical_section_exit()(lock=%lu)\n", tag, *lock);
    (*lock)--;
    /* todo: put mbedosv3++ critical section exit here */
    CFSTORE_DBGLOG("%s:after critical_section_exit()(lock=%lu)\n", tag, *lock);
}

static inline void cfstore_critical_section_lock(CFSTORE_LOCK* lock, const char* tag)
{
    (void) lock;
    (void) tag;
    CFSTORE_DBGLOG("%s:before critical_section_enter()(lock=%lu)\n", tag, *lock);
    /* todo: put mbedosv3++ critical section enter here */
    (*lock)++;
    CFSTORE_DBGLOG("%s:after critical_section_enter()(lock=%lu)\n", tag, *lock);
}

static CFSTORE_INLINE int32_t cfstore_hkvt_refcount_dec(cfstore_area_hkvt_t* hkvt, uint8_t *refcount)
{
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    /* todo: put mbedosv3++ critical section enter here */
    hdr->refcount--;
    if(refcount) *refcount = hdr->refcount;
    /* todo: put mbedosv3++ critical section exit here */
    return ARM_DRIVER_OK;
}

static CFSTORE_INLINE int32_t cfstore_hkvt_refcount_inc(cfstore_area_hkvt_t* hkvt, uint8_t *refcount)
{
    int32_t ret = ARM_CFSTORE_DRIVER_ERROR_HANDLE_COUNT_MAX;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    /* todo: put mbedosv3++ critical section enter here */
    if(hdr->refcount < CFSTORE_HKVT_REFCOUNT_MAX)
    {
        hdr->refcount++;
        if(refcount) *refcount = hdr->refcount;
        ret = ARM_DRIVER_OK;
    }
    /* todo: put mbedosv3++ critical section exit here */
    return ret;
}

#endif /* CFSTORE_TARGET_LIKE_X86_LINUX_NATIVE */


/*
 * security/permissions helper functions
 */

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/**
 * @brief   check that a client (cfstore-uvisor client box) is the "owner" of the
 *          KV. Owner means the client that can create or created the KV. This is
 *          determined by the clients namespace and whether the KV path name falls
 *          within that name space
 * @param   key_name
 * 			the name of the KV being created.
 * 			the validation that the key_name is composed of permissible chars is
 * 			carried out before this function is called.
 * @note
 * Conceptually, cfstore supports the following KV path namespaces:
 * 	- com.arm.mbed.
 * 	- guids of the form xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx where x is a hex digit.
 *
 *  In the cfstore implementation, explicit checking of the structure of the
 *  namespace string is not required. Cfstore only need enforce that:
 *  	the root of the KV pathname == cfstore client uvisor namespace.
 */
static int32_t cfstore_uvisor_is_client_kv_owner(char* key_name, int32_t* cfstore_uvisor_box_id)
{
    int32_t calling_box_id;
    int32_t ret;
    /* We store the calling_box_namespace on our stack, lest somebody else modify it. */
    char calling_box_namespace[UVISOR_MAX_BOX_NAMESPACE_LENGTH];

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(calling_box_namespace, 0, sizeof(calling_box_namespace));
    /* Get the ID of the box that called this box through the most recent secure gateway. */
    calling_box_id = uvisor_box_id_caller();
    if(calling_box_id < 0){
        CFSTORE_ERRLOG("%s: Error: uvisor uvisor_box_id_caller() returned invalid id (calling_box_id=%d\n", __func__, (int) calling_box_id);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_BOX_ID;
    }
    if(cfstore_uvisor_box_id){
        *cfstore_uvisor_box_id = calling_box_id;
    }
    if(calling_box_id == 0){
        /* the cfstore uvisor client is the main box.
         * main box is not allowed to create a key as a client is only permitted to create KVs in their namespace. */
        CFSTORE_ERRLOG("%s: Error: uvisor box id identifies cfstore client cannot create KVs (calling_box_id=%d\n", __func__, (int) calling_box_id);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_BOX_ID;
    }
    /* Copy the name of the calling box to our stack. */
    ret = uvisor_box_namespace(calling_box_id, calling_box_namespace, sizeof(calling_box_namespace));
    if(ret < 0){
        /* error */
        CFSTORE_ERRLOG("%s: Error: unable to recover uvisor box namespace\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_NAMESPACE;
    }
    /* check the cfstore client uvisor box namespace is non-trivial */
    if(strlen(calling_box_namespace) == 0){
        CFSTORE_ERRLOG("%s: Error: uvisor box namespace is zero length\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_NAMESPACE;
    }
    /* check that the key name is within the root domain namespace */
    if(strncmp(calling_box_namespace, key_name, sizeof(calling_box_namespace)) != 0) {
        /* The key_name does not fall within the cfstore-uvisor client namespace and therefore the create is not allowed */
        CFSTORE_ERRLOG("%s: Error: key name (%s) is not permitted to be created within client uvisor box namespace (%s) of cfstore client\n", __func__, key_name, calling_box_namespace);
        return ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS;
    }
    /* We've passed all our checks, so we allow the calling box. */
    return ARM_DRIVER_OK;
}
#endif /* YOTTA_CFG_CFSTORE_UVISOR */

/**
 * @brief   check that the cfstore client (caller, which is a uvisor box)
 *          is only trying to access its own namespace.
 *
 * @note    This function is the cfstore equivalent of "is_calling_box_allowed"
 */
static int32_t cfstore_uvisor_security_context_prefix_check(const char* key_name)
{
    /*todo: implement : A client uvisor security context should exist with
     *      a security_prefix_name that matches the first part of the
     *      key_name. Make sure this is the case. */

	// if the caller is the main box then deny access, as only secure uvisor boxes
	// are permitted to access cfstore.

	// get box_id of caller
	// get namespace of caller
	// if the keyname is in the namespace then permit, otherwise deny

    (void) key_name;
    return ARM_DRIVER_OK;
}

/* @brief   check that a client (cfstore-uvisor client box) is the "owner" of the
 *          KV (wrapper). see cfstore_uvisor_is_client_kv_owner() for more details.
 */
static int32_t cfstore_is_client_kv_owner(const char* key_name, int32_t* cfstore_uvisor_box_id)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
/*
#ifdef YOTTA_CFG_CFSTORE_UVISOR
    return cfstore_uvisor_is_client_kv_owner(key_name, cfstore_uvisor_box_id);
#else
    return ARM_DRIVER_OK;
#endif
*/
    (void) key_name;
    (void) cfstore_uvisor_box_id;
    return ARM_DRIVER_OK;
}

/* @brief   helper function to determine whether this client can close a given KV */
static bool cfstore_is_kv_client_closable(cfstore_file_t* file)
{
    /* todo: integrate with uvisor to get boxId (security prefix name)
     * - check the kv key_name prefix matches the security context to determine whether client is
     *   allowed to close the given key_name.
     */
    /* until can implement this functionality, assume client can close KV */
    (void) file;
    return true;
}

/* @brief   helper function to determine whether this client can delete a given KV */
static bool cfstore_is_kv_client_deletable(cfstore_file_t* file)
{
    /* todo: integrate with uvisor to get boxId (security prefix name)
     * - check the kv key_name prefix matches the security context to determine whether client is
     *   allowed to delete the given key_name.
     */
    /* until can implement this functionality, assume client can delete KV */
    (void) file;
    return true;
}

#ifdef YOTTA_CFG_CFSTORE_UVISOR
/* @brief   helper function to determine whether this cfstore-uvisor client box can read a given KV */
static bool cfstore_is_kv_client_readable(cfstore_area_hkvt_t* hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;


    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if(ret == ARM_DRIVER_OK){
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_read;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_read;
    }
    return bret;
}

/* @brief   helper function to determine whether this client can write a given KV */
static bool cfstore_is_kv_client_writable(cfstore_area_hkvt_t* hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if(ret == ARM_DRIVER_OK){
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_write;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_write;
    }
    return bret;
}

/* @brief   helper function to determine whether this client can execute a given KV */
static bool cfstore_is_kv_client_executable(cfstore_area_hkvt_t* hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if(ret == ARM_DRIVER_OK){
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_execute;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_execute;
    }
    return bret;
}
#endif  /* YOTTA_CFG_CFSTORE_UVISOR */

/* @brief   helper function to determine whether this client can read a given KV */
static bool cfstore_is_kv_client_readable(cfstore_area_hkvt_t* hkvt)
{
    /* todo: integrate with uvisor to get boxId (security prefix name)
     * - check the kv key_name prefix matches the security context to determine whether client is
     *   owner or other.
     * - if(owner)
     *   {
     *      // client is owner of kv
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_owner_read == true) {
     *          return true;
     *      }
     *   } else {
     *      // client is other
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_other_read == true) {
     *          return true;
     *   }
     *   return false;
     */
    /* until can implement this functionality, assume client has read access to KV */
    (void) hkvt;
    return true;
}

/* @brief   helper function to determine whether this client can write a given KV */
static bool cfstore_is_kv_client_writable(cfstore_area_hkvt_t* hkvt)
{
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    /* todo: integrate with uvisor to get boxId (security prefix name)
     * - check the kv key_name prefix matches the security context to determine whether client is
     *   owner or other.
     * - if(owner)
     *   {
     *      // client is owner of kv
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_owner_write == true) {
     *          return true;
     *      }
     *   } else {
     *      // client is other
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_other_write == true) {
     *          return true;
     *   }
     *   return false;
     */
    /* until can implement this functionality, assume client has write access to KV */

    /* check that the owner has write permission */
    return hdr->perm_owner_write;
}

/* @brief   helper function to determine whether this client can execute a given KV */
static bool cfstore_is_kv_client_executable(cfstore_area_hkvt_t* hkvt)
{
    /* todo: integrate with uvisor to get boxId (security prefix name)
     * - check the kv key_name prefix matches the security context to determine whether client is
     *   owner or other.
     * - if(owner)
     *   {
     *      // client is owner of kv
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_owner_execute == true) {
     *          return true;
     *      }
     *   } else {
     *      // client is other
     *      if( ((cfstore_area_header_t*)(hkvt->head))->perm_other_execute == true) {
     *          return true;
     *   }
     *   return false;
     */
    /* until can implement this functionality, assume client has execute access to KV */
    (void) hkvt;
    return true;
}


/*
 * flags helper function
 */
static bool cfstore_acl_is_default(ARM_CFSTORE_ACCESS_CONTROL_LIST acl)
{
    if(     acl.perm_owner_read == false                &&
            acl.perm_owner_write == false               &&
            acl.perm_owner_execute == false             &&
            acl.perm_other_read == false                &&
            acl.perm_other_write == false               &&
            acl.perm_other_execute == false             )
    {
        /* flags are set to indicate "adopt some meaningful default behaviour" */
        return true;
    }
    return false;
}

/*
 * flags helper function
 */
static bool cfstore_flags_is_default(ARM_CFSTORE_FMODE flags)
{
    if( flags.read == 0             &&
        flags.write == 0            &&
        flags.continuous == 0       &&
        flags.flush_on_close == 0   &&
        flags.lazy_flush == 0       &&
        flags.storage_detect == 0   )
    {
        /* flags are set to indicate "adopt some meaningful default behaviour" */
        return true;
    }
    return false;
}

static CFSTORE_INLINE bool cfstore_hkvt_get_flags_delete(cfstore_area_hkvt_t *hkvt)
{
    return ((cfstore_area_header_t*) hkvt->head)->flags.delete;
}

static CFSTORE_INLINE void cfstore_hkvt_set_flags_delete(cfstore_area_hkvt_t *hkvt, bool flag)
{
    CFSTORE_ASSERT(hkvt != NULL);
    ((cfstore_area_header_t*) hkvt->head)->flags.delete = flag;
}


/*
 * struct cfstore_area_hkvt_t helper operations
 */
static CFSTORE_INLINE uint8_t cfstore_hkvt_get_key_len(cfstore_area_hkvt_t* hkvt)
{
    cfstore_area_header_t *header;
    CFSTORE_ASSERT(hkvt != NULL);
    header = (cfstore_area_header_t*) hkvt->head;
    return header->klength;
}

static CFSTORE_INLINE uint32_t cfstore_hkvt_get_value_len(cfstore_area_hkvt_t* hkvt)
{
    cfstore_area_header_t *header;
    CFSTORE_ASSERT(hkvt != NULL);
    header = (cfstore_area_header_t*) hkvt->head;
    return header->vlength;
}

static CFSTORE_INLINE ARM_CFSTORE_SIZE cfstore_hkvt_get_size(cfstore_area_hkvt_t* hkvt)
{
    ARM_CFSTORE_SIZE kv_size = 0;

    kv_size += sizeof(cfstore_area_header_t);
    kv_size += cfstore_hkvt_get_key_len(hkvt);
    kv_size += cfstore_hkvt_get_value_len(hkvt);
    return kv_size;
}

static CFSTORE_INLINE void cfstore_hkvt_init(cfstore_area_hkvt_t* hkvt)
{
    memset(hkvt, 0, sizeof(cfstore_area_hkvt_t));
}


static CFSTORE_INLINE bool cfstore_hkvt_is_valid(cfstore_area_hkvt_t *hkvt, uint8_t *area_0_tail)
{
    if(hkvt->head && hkvt->head != area_0_tail && hkvt->key && hkvt->value && hkvt->tail) {
        return true;
    }
    return false;
}

static CFSTORE_INLINE uint32_t cfstore_hkvt_set_value_len(cfstore_area_hkvt_t* hkvt, uint32_t value_len)
{
    uint32_t vlength;
    cfstore_area_header_t *hdr;
    CFSTORE_ASSERT(hkvt != NULL);
    hdr = (cfstore_area_header_t*) hkvt->head;
    vlength = hdr->vlength;
    hdr->vlength = value_len;
    return vlength;
}

/* @brief   helper function to detect if there are any KV's stored in the sram area */
static bool cfstore_area_has_hkvt(void)
{
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    /* head and tail pointer equal means there are no KVs stored */
    if(ctx->area_0_head == ctx->area_0_tail){
        /* there are no KV's stored*/
        return false;
    }
    return true;
}


/* @brief   helper function to get the first KV in the sram area */
static cfstore_area_hkvt_t cfstore_get_hkvt_from_head_ptr(uint8_t* head)
{
    cfstore_area_hkvt_t hkvt;

    CFSTORE_ASSERT(head != NULL);
    memset((void*) &hkvt, 0, sizeof(hkvt));
    hkvt.head = head;
    hkvt.key = hkvt.head + sizeof(cfstore_area_header_t);
    hkvt.value = hkvt.key + ((cfstore_area_header_t*) hkvt.head)->klength;
    hkvt.tail = hkvt.value + ((cfstore_area_header_t*) hkvt.head)->vlength;
    return hkvt;
}


/* @brief   helper function to convert a opaque handle to a struct cfstore_area_hkvt_t */
static cfstore_area_hkvt_t cfstore_get_hkvt(ARM_CFSTORE_HANDLE hkey)
{
    cfstore_file_t* file = (cfstore_file_t*) hkey;
    return cfstore_get_hkvt_from_head_ptr((uint8_t*) file->head);
}


/* @brief   helper function to convert a opaque handle to a struct cfstore_area_hkvt_t */
static int32_t cfstore_get_head_hkvt(cfstore_area_hkvt_t* hkvt)
{
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(hkvt != NULL);
    if(!cfstore_area_has_hkvt()){
        CFSTORE_TP(CFSTORE_TP_VERBOSE1, "%s:CFSTORE has no KVs\n", __func__);
        memset((void*) hkvt, 0, sizeof(cfstore_area_hkvt_t));
        return ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
    }

    CFSTORE_TP(CFSTORE_TP_VERBOSE1, "%s:CFSTORE has KVs\n", __func__);
    *hkvt = cfstore_get_hkvt_from_head_ptr(ctx->area_0_head);
    return ARM_DRIVER_OK;
}


/* @brief   helper function to walk the sram area from the previous hkvt to
 *          the next hkvt.
 * @param   prev
 *          pointer to previous hkvt. If null then the search is started
 *          from the beginning of the sram area.
 * @param   next
 *          pointer to next hkvt for which the pointers need calculating.
 */
static int32_t cfstore_get_next_hkvt(cfstore_area_hkvt_t* prev, cfstore_area_hkvt_t* next)
{
    cfstore_ctx_t* ctx = cfstore_ctx_get();

	CFSTORE_ASSERT(prev != NULL);
    CFSTORE_ASSERT(next != NULL);
    CFSTORE_ASSERT(prev->tail <= ctx->area_0_tail);

    if(prev->tail == ctx->area_0_tail){
        CFSTORE_TP(CFSTORE_TP_VERBOSE1, "%s:reached the end of the list. return NULL entry\n", __func__);
        memset((void*) next, 0, sizeof(cfstore_area_hkvt_t));
        return ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
    }
    /* use the prev tail pointer to find the next head pointer */
    *next = cfstore_get_hkvt_from_head_ptr((uint8_t*) prev->tail);
    return ARM_DRIVER_OK;
}


/*
 * Flash support functions
 */

static CFSTORE_INLINE void cfstore_hkvt_dump(cfstore_area_hkvt_t* hkvt, const char* tag);

/** @brief  Set the context tail pointer area_0_tail to point to the end of the
 *          last KV in the memory area.
 *
 * This function walks hkvt entries in the KV area to find the memory
 * address after the end of the last KV, and then sets the area tail pointer
 * area_0_tail to that address. The function therefore relies on the
 * head, key, value, tail fields being correct.
 *
 * Notes:
 * - This function should only be called after the memory area is loaded from
 *   flash and the area_0_tail pointer needs setting. The only way to do this
 *   (at the present time) is to walk the list of KVs, which is what this function
 *   does. The only other place the code sets area_0_tail is cfstore_realloc_ex(),
 *   and this state of affairs shouldnt change i.e. its unnecessary for
 *   other functions to change area_0_tail.
 * - When loading the area_0 image from falsh, cfstore_realloc_ex() is used
 *   to allocate the memory with ctx->expected_blob_size as the size. Thus
 *   area_0_tail will be initially set to
 *     area_0_tail = area_0_head + expected_blob_size       (1)
 *   and thereby may include padding used to align the area size to a
 *   flash program unit boundary. cfstore_flash_set_tail() is used to
 *   set area_0_tail correctly.
 */
static int32_t cfstore_flash_set_tail(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    uint8_t* ptr = NULL;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    uint8_t* tail = NULL;
    cfstore_area_hkvt_t hkvt;

    CFSTORE_FENTRYLOG("%s:entered: \n", __func__);
    CFSTORE_ASSERT(ctx != NULL);
    cfstore_hkvt_init(&hkvt);

    /* Check for cases where the tail pointer is already set correctly
     * e.g. where the area is of zero length */
    if(cfstore_ctx_get_kv_total_len() == 0) {
        /* tail pointer already set correctly */
        return ARM_DRIVER_OK;
    }
    ptr = ctx->area_0_head;
    tail = ctx->area_0_tail;
    while(ptr <= tail) {
        CFSTORE_FENTRYLOG("%s:ptr=%p, tail=%p: \n", __func__, ptr, tail);
        hkvt = cfstore_get_hkvt_from_head_ptr(ptr);
        if(cfstore_hkvt_is_valid(&hkvt, tail) == false) {
            CFSTORE_ERRLOG("%s:Error:found invalid hkvt entry in area\n", __func__);
            break;
        }
        cfstore_hkvt_dump(&hkvt, __func__);
        /* when the length between the hkvt.tail and tail
         * is less than the minimum KV length then we have found the last KV, and can set the
         * area_0_tail correctly to the end of the last KV. This works OK for the present support
         * (where flash_program_unit ~ sizeof(cfstore_area_header_t)) but may need
         * revisiting where flash_program_unit > sizeof(cfstore_area_header_t) */
        if((uint32_t)(tail - hkvt.tail) < sizeof(cfstore_area_header_t)){
            /* ptr is last KV in area as there isn't space for another header  */
            ctx->area_0_tail = hkvt.tail;
            ret = ARM_DRIVER_OK;
            break;
        }
        ptr = hkvt.tail;
    }
    return ret;
}


/** @brief  Function to realloc the SRAM area used to store KVs.
 *
 * This function consolidates the code needed to:
 * - realloc the memory
 * - when the start of the SRAM area moves, update data structures
 *   which point into SRAM area (e.g. open files cfstore_file_t head pointers).
 *
 * The function assumes:
 * - the cfstore_file_t::head pointers are valid i.e. point to the
 *   correct locations in the KV area for each file.
 *
 * @param   size
 * total KV size in bytes storage required. Note this does not include
 * padding to round up to the nearest multiple of flash program unit
 * as this is computed and added in this function.
 *
 * @param   allocated_size
 * total size in bytes that was allocated (value returned to caller).
 * This may be larger than the requested size due to rounding to align with a
 * flash program unit boundary.
 */
static int32_t cfstore_realloc_ex(ARM_CFSTORE_SIZE size, uint64_t *allocated_size)
{
    uint8_t* ptr = NULL;
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t len_diff = 0;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_file_t* file;
    cfstore_list_node_t* node;
    cfstore_list_node_t* file_list = &ctx->file_list;
    ARM_CFSTORE_SIZE total_kv_size = size;

    /* Switch on the size of the sram area to create:
     * - if size > 0 (but may be shrinking) then use REALLOC.
     * - if size == 0 then the area is being deleted so free the memory
     * Note:
     * - realloc can return NULL when the last KV is deleted
     * - It also appears that realloc can return non-zero ptr when size = 0.
     *   Hence for this case free() is used.
     */
    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    CFSTORE_TP(CFSTORE_TP_MEM, "%s:cfstore_ctx_g.area_0_head=%p, cfstore_ctx_g.area_0_tail=%p, cfstore_ctx_g.area_0_len=%d, size=%d, \n", __func__, ctx->area_0_head, ctx->area_0_tail, (int) ctx->area_0_len, (int) size);

    if(size > 0)
    {
        /* In the general case (size % program_unit > 0). The new area_0 size is
         * aligned to a flash program_unit boundary to facilitate r/w to flash
         * and so the memory realloc size is calculated to align, as follows */
        if(size % cfstore_ctx_get_program_unit(ctx) > 0){
            size += (cfstore_ctx_get_program_unit(ctx) - (size % cfstore_ctx_get_program_unit(ctx)));
        }

        ptr = (uint8_t*) CFSTORE_REALLOC((void*) ctx->area_0_head, size);
        if (ptr == NULL) {
            if (total_kv_size <= ctx->area_0_len) {
                /* Size is shrinking so a realloc failure is recoverable.
                 * Update ptr so it matches the previous head.
                 */
                ptr = ctx->area_0_head;
            }
        }
        if(ptr == NULL){
            CFSTORE_ERRLOG("%s:Error: unable to allocate memory (size=%d)\n", __func__, (int) size);
            /* realloc() has failed to allocate the required memory object. If previously
             * allocation has been made, the old memory object remains allocated. On error, the client
             * is expected to clean up including making a call to Uninitialize() which will free the
             * old memory object.
             */
            return ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY;
        }
        /* check realloc() hasn't move area in memory from cfstore_ctx_g.area_0_head */
        if(ptr != ctx->area_0_head){
            /* realloc() has moved the area in memory */
            CFSTORE_TP(CFSTORE_TP_MEM, "%s: realloc() has moved memory area and area_0_head ptr must change. old cfstore_ctx_g.area_0_head=%p, new head ptr=%p)\n", __func__, ctx->area_0_head, ptr);

            /* now have to walk the file list updating head pointers to point into the realloc-ed
             * To begin with, leave the relative position of the file pointers unaltered */
            node = file_list->next;
            while(node != file_list){
                file = (cfstore_file_t*) node;
                file->head = (uint8_t *) (file->head - ctx->area_0_head);
                file->head = (uint8_t *) ((int32_t) file->head + (int32_t) ptr);
                node = node->next;
            }
            ctx->area_0_head = ptr;
        }

        /* If the area is growing then zero the new space at the end of the area */
        len_diff = size - (int32_t) ctx->area_0_len;
        if(len_diff > 0) {
            memset(ptr + ctx->area_0_len, 0, len_diff);
        }
        /* Set area_0_tail to be the memory address after the end of the last KV in the memory area.
         * This is the only place that area_0_tail should be changed, apart from cfstore_flash_set_tail()
         * which is only called when attributes are loaded from flash.
         */
        ctx->area_0_len = size;
        ctx->area_0_tail = ptr + total_kv_size;
        if(allocated_size != NULL) {
            *allocated_size = size;
        }
    }
    else
    {
        /* size = 0 so delete the memory */
        CFSTORE_FREE((void*) ctx->area_0_head);
        ctx->area_0_head = NULL;
        ctx->area_0_tail = NULL;
        ctx->area_0_len = 0;
    }
    CFSTORE_TP(CFSTORE_TP_MEM, "%s:cfstore_ctx_g.area_0_head=%p, cfstore_ctx_g.area_0_tail=%p\n", __func__, ctx->area_0_head, ctx->area_0_tail);
    ret = ARM_DRIVER_OK;
    return ret;

}


#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED

/*
 * flash helper functions
 */

/* @brief   table for mapping flash journal error codes to equivalent cfstore error codes */
static cfstore_flash_journal_error_code_node cfstore_flash_journal_error_code_map[]=
{
    { JOURNAL_STATUS_OK,                                ARM_DRIVER_OK},
    { JOURNAL_STATUS_ERROR,                             ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_ERROR},
    { JOURNAL_STATUS_BUSY,                              ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_BUSY},
    { JOURNAL_STATUS_TIMEOUT,                           ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_TIMEOUT},
    { JOURNAL_STATUS_UNSUPPORTED,                       ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_UNSUPPORTED},
    { JOURNAL_STATUS_PARAMETER,                         ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_PARAMETER},
    { JOURNAL_STATUS_BOUNDED_CAPACITY,                  ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_BOUNDED_CAPACITY},
    { JOURNAL_STATUS_STORAGE_API_ERROR,                 ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_STORAGE_API_ERROR},
    { JOURNAL_STATUS_STORAGE_IO_ERROR,                  ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_STORAGE_IO_ERROR},
    { JOURNAL_STATUS_NOT_INITIALIZED,                   ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_NOT_INITIALIZED},
    { JOURNAL_STATUS_EMPTY,                             ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_EMPTY},
    { JOURNAL_STATUS_SMALL_LOG_REQUEST,                 ARM_CFSTORE_DRIVER_ERROR_JOURNAL_STATUS_SMALL_LOG_REQUEST},
    { CFSTORE_SENTINEL, CFSTORE_SENTINEL}
};

static int32_t cfstore_flash_map_error(int32_t flash_journal_status_code)
{
    cfstore_flash_journal_error_code_node* node = cfstore_flash_journal_error_code_map;

    while(node->flash_journal_error_code != (int32_t) CFSTORE_SENTINEL)
    {
        if(flash_journal_status_code == node->flash_journal_error_code)
        {
            return node->cfstore_error_code;
        }
    }
    return ARM_CFSTORE_DRIVER_ERROR_INTERNAL;
}


/* @brief   Callback registered with flash journal for async operation
 *          completion notifications.
 *
 * @note    The callback is called at interrupt context.
 *          The critical section to used police access to context variables
 *          modified by both the interrupt and application context processing.
 *          The interrupt context prevents application context from running and
 *          hence its only necessary to use the critical_section_xxx in the
 *          application execution context.
 *
 *          In flash journal async mode, when:
 *              - a FlashJournal_xxx() function has been invoked, and
 *              - before the async completion has been received and processed
 *          the application context code should alway co-ordinate access to
 *          context variables modified by interrupt and application context
 *          by use of the critical_section_xxx.
 */
static void cfstore_flash_journal_callback(int32_t status, FlashJournal_OpCode_t cmd_code)
{
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: status=%d, cmd_code=%d (%s)\n", __func__, (int) status, (int) cmd_code, cfstore_flash_opcode_str[cmd_code]);
    switch(cmd_code)
    {
    case FLASH_JOURNAL_OPCODE_FORMAT:
        ctx->fsm.event = cfstore_fsm_event_format_done;
        break;
    case FLASH_JOURNAL_OPCODE_INITIALIZE:
        ctx->fsm.event = cfstore_fsm_event_init_done;
        break;
    case FLASH_JOURNAL_OPCODE_READ_BLOB:
        ctx->fsm.event = cfstore_fsm_event_read_done;
        break;
    case FLASH_JOURNAL_OPCODE_LOG_BLOB:
        ctx->fsm.event = cfstore_fsm_event_log_done;
        break;
    case FLASH_JOURNAL_OPCODE_COMMIT:
        ctx->fsm.event = cfstore_fsm_event_commit_done;
        break;
    case FLASH_JOURNAL_OPCODE_RESET:
        ctx->fsm.event = cfstore_fsm_event_reset_done;
        break;
    case FLASH_JOURNAL_OPCODE_GET_INFO:
    default:
        CFSTORE_ERRLOG("%s:Error: notification of unsupported cmd_code event (status=%d, cmd_code=%d)\n", __func__, (int) status, (int) cmd_code);
        return;
    }
    ctx->status = status;
    ctx->cmd_code = cmd_code;
    cfstore_fsm_state_handle_event(&ctx->fsm, ctx->fsm.event, (void*) ctx);
    return;
}


/* @brief */
static int32_t cfstore_fsm_stop_on_entry(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    /* reset fsm state */
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx->fsm.state == cfstore_fsm_state_stopped);

    ctx->fsm.event = cfstore_fsm_event_max;
    ctx->cmd_code = (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);
    return ARM_DRIVER_OK;
}

/* static int32_t cfstore_fsm_stop_on_exit(void* context) {(void) context; }*/


/* @brief   fsm on entry function for the initing state
 * @note
 *  flash journal sync mode: (see async mode notes)
 *  flash journal async mode:
 *      This is typically called in app context (not intr context) for both flash
 *      journal sync and asyc modes. There are no outstanding async requests
 *      so it cannot be interrupted, and therefore doesnt need CS protection.
 */
static int32_t cfstore_fsm_init_on_entry(void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    ret = cfstore_svm_init(&cfstore_journal_mtd);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_DBGLOG("%s:Error: Unable to initialize storage volume manager\n", __func__);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_formatting, ctx);
        return ARM_DRIVER_OK;
    }

    ret = FlashJournal_initialize(&ctx->jrnl, (ARM_DRIVER_STORAGE *) &cfstore_journal_mtd, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, cfstore_flash_journal_callback);
    CFSTORE_TP(CFSTORE_TP_FSM, "%s:FlashJournal_initialize ret=%d\n", __func__, (int) ret);
    if(ret < ARM_DRIVER_OK){
        if(ret == JOURNAL_STATUS_NOT_FORMATTED) {
            CFSTORE_DBGLOG("%s:Error: flash not formatted\n", __func__);
            cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_formatting, ctx);
            return ARM_DRIVER_OK;
        }
        if(ret == JOURNAL_STATUS_METADATA_ERROR) {
            CFSTORE_ERRLOG("%s:Error: flash meta-data (CRC) error detected when initializing flash. Reformatting flash.\n", __func__);
            cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_formatting, ctx);
            return ARM_DRIVER_OK;
        }
        CFSTORE_ERRLOG("%s:Error: failed to initialize flash journaling layer (ret=%d)\n", __func__, (int) ret);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_stopped, ctx);
    }
    else if(ret > 0){
        /* operation completed synchronously*/
        cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_INITIALIZE);
    }
    return ret;
}


/* @brief   fsm initing state handler function
 * @note
 *  flash journal sync mode:
 *      CS protection not required as there are no callbacks.
 *  flash journal async mode:
 *      This is typically called at intr context (not app context) when flash
 *      journal invokes the callback handler for FLASH_JOURNAL_OPCODE_INITIALIZE
 *      Hence as running at intr level, no CS protection is required.
 */
static int32_t cfstore_fsm_initing(void* context)
{
    int32_t ret = ARM_DRIVER_OK;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx->fsm.state == cfstore_fsm_state_initing);
    CFSTORE_ASSERT(ctx->cmd_code == FLASH_JOURNAL_OPCODE_INITIALIZE);

    /* only change state if status > 0*/
    if(ctx->status > 0){
        ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_reading, ctx);
    } else if(ctx->status < 0) {
        CFSTORE_ERRLOG("%s:Error: failed to initialize flash journaling layer (ret=%d)\n", __func__, (int) ctx->status);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_stopped, ctx);
    }
    return ret;
}

/* static int32_t cfstore_fsm_init_on_exit(void* context) */


/* @brief   fsm on entry function for the reading state
 * @note
 *  flash journal sync mode:
 *      CS protection not required as there are no callbacks.
 *  flash journal async mode:
 *      This is typically called at intr context (not app context) when flash
 *      journal invokes the callback handler for FLASH_JOURNAL_OPCODE_INITIALIZE
 *      Hence as running at intr level, no CS protection is required.
 */
static int32_t cfstore_fsm_read_on_entry(void* context)
{
    int32_t ret = 0;
    FlashJournal_Status_t status = JOURNAL_STATUS_ERROR;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx != NULL);
    /* FlashJournal_getInfo() is synchronous */
    status = FlashJournal_getInfo(&ctx->jrnl, &ctx->info);
    if(status < JOURNAL_STATUS_OK){
        CFSTORE_TP(CFSTORE_TP_FSM, "%s:Error: failed get journal info (status=%d)\n", __func__, (int) status);
        /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
        ret = ARM_CFSTORE_DRIVER_ERROR_INTERNAL;
        goto out;
    }
    if(ctx->info.sizeofJournaledBlob > 0)
    {
        /* setup the expected blob size for writing */
        ctx->expected_blob_size = ctx->info.sizeofJournaledBlob;
        ret = cfstore_realloc_ex(ctx->expected_blob_size, &ctx->expected_blob_size);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: cfstore_realloc_ex() failed (ret=%d)\n", __func__, (int) ret);
            /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
            cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
            goto out;
        }
        ret = FlashJournal_read(&ctx->jrnl, (void*) ctx->area_0_head, ctx->info.sizeofJournaledBlob);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to initialize flash journaling layer (ret=%d)\n", __func__, (int) ret);
            /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
            cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
            goto out;
        } else if(ret > 0){
            /* read has completed synchronously*/
            CFSTORE_TP(CFSTORE_TP_FSM, "%s:debug:ret > 0: (ret=%d)\n", __func__, (int) ret);
            cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_READ_BLOB);
            ret = ctx->status;
            goto out;
        }
        /* keep lock and wait for async callback */
    } else {
        /* there is no blob, move to next state. need a +ve status value to indicate async completion
         * to the fsm reading state handler. use CFSTORE_FLASH_AREA_SIZE_MIN for this value */
        ctx->expected_blob_size = CFSTORE_FLASH_AREA_SIZE_MIN;
        status = (FlashJournal_Status_t) CFSTORE_FLASH_AREA_SIZE_MIN;
        cfstore_flash_journal_callback(status, FLASH_JOURNAL_OPCODE_READ_BLOB);
        ret = ctx->status;
        goto out;
    }
out:
    return ret;
}


/* @brief  fsm handler when in reading state */
static int32_t cfstore_fsm_reading(void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx->fsm.state == cfstore_fsm_state_reading);
    CFSTORE_ASSERT(ctx->cmd_code == FLASH_JOURNAL_OPCODE_READ_BLOB);
    if(ctx->status > 0)
    {
        if(ctx->status > (int32_t) CFSTORE_FLASH_AREA_SIZE_MIN)
        {
            CFSTORE_TP(CFSTORE_TP_FSM, "%s:debug:ctx->status > (int32_t) CFSTORE_FLASH_AREA_SIZE_MIN:\n", __func__);
            /* check the correct amount of data was read, which is the status code */
            if(ctx->status == (int32_t) ctx->expected_blob_size)
            {
                /* now have to allow for the fact that there may have been some padding
                 * at the end of the last _log() to flash, so the read back area may have
                 * padding at the end, and the tail_pointer needs to not point to the
                 * end where the padding is located, but to the end of the last KV.
                 */
                ret = cfstore_flash_set_tail();
                if(ret < ARM_DRIVER_OK){
                    CFSTORE_ERRLOG("%s:Error: cfstore_flash_set_tail() failed (ret=%d)\n", __func__, (int) ret);
                    /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
                    cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
                    memset(&ctx->info, 0, sizeof(ctx->info));
                    goto out;
                }
                ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
                if(ret < ARM_DRIVER_OK){
                    CFSTORE_ERRLOG("%s:Error: cfstore_fsm_state_set() failed (ret=%d)\n", __func__, (int) ret);
                    goto out;
                }
                ret = ctx->status;
            }
            else
            {
                CFSTORE_ERRLOG("%s:Error: read bytes (%d) does not equal requested read size (%d)\n", __func__, (int) ctx->status, (int) ctx->expected_blob_size);
                ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
                if(ret < ARM_DRIVER_OK){
                    /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
                    CFSTORE_ERRLOG("%s:Error: cfstore_fsm_state_set() failed (ret=%d)\n", __func__, (int) ret);
                    goto out;
                }
                ret = ctx->status;
            }
        }
        else
        {
            CFSTORE_TP(CFSTORE_TP_FSM, "%s:debug:ctx->status <= (int32_t) CFSTORE_FLASH_AREA_SIZE_MIN:\n", __func__);
            ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
            if(ret < ARM_DRIVER_OK){
                /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
                CFSTORE_ERRLOG("%s:Error: cfstore_fsm_state_set() failed (ret=%d)\n", __func__, (int) ret);
                goto out;
            }
            ret = ctx->status;
        }
    }
    else if(ctx->status < 0)
    {
        CFSTORE_TP(CFSTORE_TP_FSM, "%s:debug:ctx->status < 0:\n", __func__);
        ret = ctx->status;
    }
out:
    return ret;
}


static int32_t cfstore_fsm_read_on_exit(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    /* notify client of initialisation status */
    cfstore_client_notify_data_init(&ctx->client_notify_data, CFSTORE_OPCODE_INITIALIZE, ctx->status, NULL);
    ctx->client_callback_notify_flag = true;
    return ARM_DRIVER_OK;
}

/* int32_t cfstore_fsm_log_on_entry(void* context){ (void) context;} */

/* @brief   on entry to writing state, update value */
int32_t cfstore_fsm_log_on_entry(void* context)
{
    int32_t ret = 0;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;
    FlashJournal_Info_t info;
    FlashJournal_Status_t status = JOURNAL_STATUS_ERROR;

    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    memset(&info, 0, sizeof(info));

    status = FlashJournal_getInfo(&ctx->jrnl, &info);
    if(status < JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: failed get journal info (status=%d)\n", __func__, (int) status);
        /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
        return cfstore_flash_map_error(status);
    }
    /* compute the expected_blob_size = area_size plus the padding at the end of the area to align with program_unit*/
    ctx->expected_blob_size = cfstore_ctx_get_kv_total_len();
    if(ctx->expected_blob_size % info.program_unit > 0){
        ctx->expected_blob_size += (info.program_unit - (ctx->expected_blob_size % info.program_unit));
    }
    /* log the changes to flash even when the area has shrunk to 0, as its necessary to erase the flash */
    if(ctx->area_dirty_flag == true)
    {
        if(ctx->expected_blob_size > 0){
            CFSTORE_TP(CFSTORE_TP_FLUSH, "%s:logging: ctx->area_0_head=%p, ctx->expected_blob_size-%d\n", __func__, ctx->area_0_head, (int) ctx->expected_blob_size);
            ret = FlashJournal_log(&ctx->jrnl, (const void*) ctx->area_0_head, ctx->expected_blob_size);
            if(ret < JOURNAL_STATUS_OK){
                CFSTORE_ERRLOG("%s:Error: FlashJournal_commit() failed (ret=%d)\n", __func__, (int) ret);
                ret = cfstore_flash_map_error(status);
                /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
                cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
                goto out0;
            } else if(ret > 0){
                /* read has completed synchronously*/
                cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_LOG_BLOB);
                ret = ctx->status;
            }
            /* wait for async completion handler*/
        } else {
            /* expected_blob_size == 0
             * There are no entries in the cfstore (e.g. last entry has been deleted) and this needs
             * reflecting in the flash. A log is not required (as there is not data). Initiate the
             * commit which will zero the flash
             * */
            CFSTORE_TP(CFSTORE_TP_FLUSH, "%s:skip logging: initiate commit to erase flash\n", __func__);
            ret = JOURNAL_STATUS_OK;
            cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_LOG_BLOB);
        }
    }
    else
    {
        /* nothing to be logged so move back to ready state indicating success*/
        CFSTORE_TP(CFSTORE_TP_FLUSH, "%s:not logging: ctx->area_0_head=%p, ctx->expected_blob_size-=%d\n", __func__, ctx->area_0_head, (int) ctx->expected_blob_size);
        cfstore_flash_journal_callback(ctx->expected_blob_size, FLASH_JOURNAL_OPCODE_LOG_BLOB);
    }
out0:
    return ret;
}

/* @brief  fsm handler when in reading state */
static int32_t cfstore_fsm_logging(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:ctx->status=%ld\n", __func__, ctx->status);
    /* check the correct amount of data was written */
    if(ctx->status < JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: FlashJournal_log() failed (ret=%d)\n", __func__, (int) ctx->status);
        /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
        ctx->status = cfstore_flash_map_error(ctx->status);
    }
    else
    {   /* ctx->status >= 0 (status == 0 when everything is deleted) */
        if(ctx->status == (int32_t)ctx->expected_blob_size){
            /* move to the committing state to commit to flash*/
            ctx->status = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_committing, ctx);
        } else {
            CFSTORE_ERRLOG("%s:Error: FlashJournal_log() failed to log the expected number of bytes (ctx->expected_blob_size=%d, committed=%d)\n", __func__, (int) ctx->expected_blob_size, (int) ctx->status);
            ctx->status = ARM_DRIVER_ERROR;
        }
    }
    return ctx->status;
}


static int32_t cfstore_fsm_log_on_exit(void* context)
{
	(void) context;
    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    return ARM_DRIVER_OK;
}


/* @brief  fsm handler when entering committing state
 * @note
 * Its unnecessary to provide CS protection for the flashJouranl_commit() as the all the
 * _log() operations affecting the commit have been performed, and no more _log() operations
 * can happen until we're back in the ready state
 */
static int32_t cfstore_fsm_commit_on_entry(void* context)
{
    int32_t ret = JOURNAL_STATUS_OK;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    if(ctx->area_dirty_flag == true)
    {
		ret = FlashJournal_commit(&ctx->jrnl);
		CFSTORE_TP(CFSTORE_TP_FSM, "%s:debug: FlashJournal_commit() (ret=%d)\n", __func__, (int) ret);
		if(ret < JOURNAL_STATUS_OK){
			CFSTORE_ERRLOG("%s:Error: FlashJournal_commit() failed (ret=%d)\n", __func__, (int) ret);
			/* move to ready state. cfstore client is expected to Uninitialize() before further calls */
			cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
		} else if(ret > 0){
			/* read has completed synchronously*/
			cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_COMMIT);
			ret = ctx->status;
		}
    }
    else
    {
		/* a commit should not be made because there have been no flashJournal_log() calls since the last commit.
		 * If a _commit() call was made without any _log() calls then it would result in the flash being erased
		 * because flash journal essentially contains a mirror image of the configuration store sram area, which
		 * has to be *** FULLY*** repopulated before each _commit(). */
		cfstore_flash_journal_callback(ARM_DRIVER_OK_DONE, FLASH_JOURNAL_OPCODE_COMMIT);
		ret = ctx->status;
    }
	/* wait for async callback */
	CFSTORE_FENTRYLOG("%s:exiting: FlashJournal_commit() (ret=%d)\n", __func__, (int) ret);
    return ret;
}


/* @brief  fsm handler when in committing state
 * @note
 * Its unnecessary to provide CS protection for the flashJouranl_commit() as the all the
 * _log() operations affecting the commit have been performed, and no more _log() operations
 * can happen until we're back in the ready state
 */
static int32_t cfstore_fsm_committing(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx->fsm.state == cfstore_fsm_state_committing);
    CFSTORE_ASSERT(ctx->cmd_code == FLASH_JOURNAL_OPCODE_COMMIT);

    /* check the correct amount of data was written */
    if(ctx->status < JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: FlashJournal_commit() failed (ret=%d)\n", __func__, (int) ctx->status);
        /* move to ready state. cfstore client is expected to Uninitialize() before further calls */
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
        ctx->status = cfstore_flash_map_error(ctx->status);
    }
    else if(ctx->status == JOURNAL_STATUS_OK)
    {
        ctx->status = cfstore_flash_map_error(ctx->status);
    }
    else
    {   /* ctx->status > 0. for flash-journal-strategy-sequential version >0.4.0, commit() return no longer reports size of commit block */
        ctx->status = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_ready, ctx);
    }
    return ctx->status;
}

static int32_t cfstore_fsm_commit_on_exit(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered:\n", __func__);
    ctx->area_dirty_flag = false;
    /* notify client of commit status */
    cfstore_client_notify_data_init(&ctx->client_notify_data, CFSTORE_OPCODE_FLUSH, ctx->status, NULL);
    ctx->client_callback_notify_flag = true;
    return ARM_DRIVER_OK;
}

/* int32_t cfstore_fsm_reset_on_entry(void* context){ (void) context;} */
/* int32_t cfstore_fsm_resetting(void* context){ (void) context;} */
/* int32_t cfstore_fsm_reset_on_exit(void* context){ (void) context;} */


static int32_t cfstore_fsm_ready_on_commit_req(void* context)
{
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_logging, ctx);
}

/* int32_t cfstore_fsm_ready_on_entry(void* context){ (void) context;} */
/* int32_t cfstore_fsm_ready(void* context){ (void) context;} */
/* int32_t cfstore_fsm_ready_on_exit(void* context){ (void) context;} */


/** @brief  fsm handler when entering the formatting state
 */
static int32_t cfstore_fsm_format_on_entry(void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    ret = flashJournalStrategySequential_format((ARM_DRIVER_STORAGE *) &cfstore_journal_mtd, CFSTORE_FLASH_NUMSLOTS, cfstore_flash_journal_callback);
    CFSTORE_TP(CFSTORE_TP_FSM, "%s:flashJournalStrategySequential_format ret=%d\n", __func__, (int) ret);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to format flash (ret=%d)\n", __func__, (int) ret);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_stopped, ctx);
    }
    else if(ret > 0){
        /* operation completed synchronously*/
        cfstore_flash_journal_callback(ret, FLASH_JOURNAL_OPCODE_FORMAT);
    }
    return ret;
}

/** @brief  fsm handler when in formatting state
 */
int32_t cfstore_fsm_formatting(void* context)
{
    int32_t ret = ARM_DRIVER_OK;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(ctx->fsm.state == cfstore_fsm_state_formatting);
    CFSTORE_ASSERT(ctx->cmd_code == FLASH_JOURNAL_OPCODE_FORMAT);

    /* only change state if status > 0*/
    if(ctx->status > 0){
        ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_initing, ctx);
    } else if(ctx->status < 0) {
        CFSTORE_ERRLOG("%s:Error: failed to format flash (ret=%d)\n", __func__, (int) ctx->status);
        cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_stopped, ctx);
    }
    return ret;
}

/* int32_t cfstore_fsm_format_on_exit(void* context){ (void) context;} */


/* handler functions while in state */
static cfstore_fsm_handler cfstore_flash_fsm[cfstore_fsm_state_max][cfstore_fsm_event_max] =
{
/* state\event:   init_done               read_done             log_done              commit_req                            commit_done               reset_done                format_done, */
/* stopped    */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* init       */  {cfstore_fsm_initing,   cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* reading    */  {cfstore_fsm_null,      cfstore_fsm_reading,  cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* logging    */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_logging,  cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* committing */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_committing,   cfstore_fsm_null,     cfstore_fsm_null        },
/* resetting  */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* ready      */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_ready_on_commit_req,      cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_null        },
/* formatting */  {cfstore_fsm_null,      cfstore_fsm_null,     cfstore_fsm_null,     cfstore_fsm_null,                     cfstore_fsm_null,         cfstore_fsm_null,     cfstore_fsm_formatting  },
};

/* handler functions for entering the state*/
cfstore_fsm_handler cfstore_fsm_on_entry[cfstore_fsm_state_max] =
{
    cfstore_fsm_stop_on_entry,
    cfstore_fsm_init_on_entry,
    cfstore_fsm_read_on_entry,
    cfstore_fsm_log_on_entry,
    cfstore_fsm_commit_on_entry,
    cfstore_fsm_null,               /* cfstore_fsm_reset_on_entry */
    cfstore_fsm_null,               /* cfstore_fsm_ready_on_entry */
    cfstore_fsm_format_on_entry     /* cfstore_fsm_format_on_entry */
};

/* handler functions for exiting state, currently none used */
cfstore_fsm_handler cfstore_fsm_on_exit[cfstore_fsm_state_max] =
{
    cfstore_fsm_null,             /* cfstore_fsm_stop_on_exit */
    cfstore_fsm_null,             /* cfstore_fsm_init_on_exit */
    cfstore_fsm_read_on_exit,
    cfstore_fsm_log_on_exit,
    cfstore_fsm_commit_on_exit,
    cfstore_fsm_null,             /* cfstore_fsm_reset_on_exit */
    cfstore_fsm_null,             /* cfstore_fsm_ready_on_exit */
    cfstore_fsm_null              /* cfstore_fsm_format_on_exit */
};


/* @brief   inject event into fsm */
static int32_t cfstore_fsm_state_handle_event(cfstore_fsm_t* fsm, cfstore_fsm_event_t event, void* context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = (cfstore_ctx_t*) context;

    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, fsm->state=%d, event=%d (%s), ctx=%p\n", __func__, fsm, fsm->state, event, cfstore_flash_event_str[event], ctx);
    CFSTORE_ASSERT(event < cfstore_fsm_event_max);
    fsm->event = event;
    if(cfstore_flash_fsm[fsm->state][fsm->event] != NULL){
        ret = cfstore_flash_fsm[fsm->state][fsm->event](ctx);
        if(ret < ARM_DRIVER_OK){
            #ifdef CFSTORE_DEBUG
            CFSTORE_ERRLOG("%s:FSM:EVT:Error: cfstore_flash_fsm[%s][%s] failed\n", __func__, (char*) cfstore_flash_state_str[fsm->state], (char*) cfstore_flash_event_str[fsm->event]);
            #endif
            return ret;
        }
    }

    /* do not clear context data set by caller as it may be used later
     *  fsm->event = cfstore_fsm_event_max;
     *  ctx->status = 0;
     *  ctx->cmd_code =  (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);
     */
    return ret;
}


/* @brief   get the current state of the fsm */
static cfstore_fsm_state_t cfstore_fsm_state_get(cfstore_fsm_t* fsm)
{
    return fsm->state;
}

/* @brief   function to move to new fsm state, calling state exit function for old state and entry function for new state */
static int32_t cfstore_fsm_state_set(cfstore_fsm_t* fsm, cfstore_fsm_state_t new_state, void* ctx)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* context = (cfstore_ctx_t*) ctx;
    #ifdef CFSTORE_DEBUG
    cfstore_fsm_state_t old_state = fsm->state;
    #endif

    CFSTORE_FENTRYLOG("%s:entered: fsm=%p, ctx=%p\n", __func__, fsm, ctx);
    #ifdef CFSTORE_DEBUG
    CFSTORE_TP(CFSTORE_TP_FSM, "%s:FSM:REQ RX: fsm->state=%d (%s): new_state=%d (%s)\n", __func__, (int) fsm->state, cfstore_flash_state_str[fsm->state], (int) new_state, cfstore_flash_state_str[new_state]);
    #endif
    CFSTORE_ASSERT(fsm != NULL);
    CFSTORE_ASSERT(new_state < cfstore_fsm_state_max);
    CFSTORE_ASSERT(ctx != NULL);
    CFSTORE_ASSERT(fsm->state < cfstore_fsm_state_max);

    if(cfstore_fsm_on_exit[fsm->state] != NULL){
        ret = cfstore_fsm_on_exit[fsm->state](ctx);
        if(ret < ARM_DRIVER_OK){
            #ifdef CFSTORE_DEBUG
            CFSTORE_ERRLOG("%s:FSM:REQ RX:%s:%s:Error: cfstore_fsm_on_exit() failed\n", __func__, cfstore_flash_state_str[fsm->state], cfstore_flash_state_str[new_state]);
            #endif
            /* handling of the error is done in the on_exit() method, which best knows how the state to move to */
            return ret;
        }
    }
    fsm->state = new_state;
    if(cfstore_fsm_on_entry[new_state] != NULL){
        ret = cfstore_fsm_on_entry[new_state](ctx);
        if(ret < ARM_DRIVER_OK){
            #ifdef CFSTORE_DEBUG
            CFSTORE_TP(CFSTORE_TP_FSM, "%s:FSM:REQ RX: fsm->state=%d (%s): new_state=%d (%s): Error: cfstore_fsm_on_entry() failed (ret=%d)\n", __func__, (int) fsm->state, cfstore_flash_state_str[fsm->state], (int) new_state, cfstore_flash_state_str[new_state], (int) ret);
            #endif
            /* handling of the error is done in the on_entry() method, which best knows how the state to move to */
            return ret;
        }
    }
    if(context->client_callback_notify_flag == true)
    {
        cfstore_client_notify_data_t notify_data;

        CFSTORE_TP(CFSTORE_TP_FSM, "%s:doing client callback\n", __func__);

        /* only one set of client notify data is required as there can only be 1 outstanding flash journal async notificaion
         * at one time. */
        context->client_callback_notify_flag = false; /* prevents re-calling callback if this function gets called again */
        memcpy(&notify_data, &context->client_notify_data, sizeof(cfstore_client_notify_data_t));
        /* clear context state before initiating call */
        cfstore_client_notify_data_init(&context->client_notify_data, CFSTORE_OPCODE_MAX, ARM_DRIVER_ERROR, NULL);
        cfstore_ctx_client_notify(ctx, &notify_data);
    }
    CFSTORE_TP(CFSTORE_TP_FSM, "%s:FSM:REQ DONE: fsm->state=%d (%s): new_state=%d (%s)\n", __func__, (int) old_state, cfstore_flash_state_str[old_state], (int) new_state, cfstore_flash_state_str[new_state]);
    return ret;
}

static bool cfstore_flash_journal_is_async_op_pending(cfstore_ctx_t* ctx)
{
    CFSTORE_FENTRYLOG("%s:entered: fsm->state=%s\n", __func__, (char*) cfstore_flash_state_str[cfstore_fsm_state_get(&ctx->fsm)]);
    if(cfstore_fsm_state_get(&ctx->fsm) != cfstore_fsm_state_ready)
    {
        /* flash journal async operation is in progress */
        return true;
    }
    return false;
}

static int32_t cfstore_flash_init(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: \n", __func__);
    ctx->cmd_code = (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);
    ctx->expected_blob_size = 0;
    ctx->fsm.event = cfstore_fsm_event_max;
    ctx->fsm.state = cfstore_fsm_state_stopped;
    memset(&ctx->info, 0, sizeof(ctx->info));
    ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_initing, ctx);
    if(ret < 0){
        CFSTORE_DBGLOG("%s:Error: cfstore_fsm_state_set() failed\n", __func__);
        return ret;
    }
    return ret;
}


/* @brief   de-initialise the flash journal */
static int32_t cfstore_flash_deinit(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: fsm->state=%s\n", __func__, (char*) cfstore_flash_state_str[cfstore_fsm_state_get(&ctx->fsm)]);
    ret = cfstore_fsm_state_set(&ctx->fsm, cfstore_fsm_state_stopped, ctx);
    if(ret < 0){
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:Error: cfstore_fsm_state_set() failed\n", __func__);
    }
    return ret;
}

/*
static int32_t cfstore_flash_reset(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    ret = FlashJournal_reset(&ctx->jrnl);
    if(ret != JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: failed to reset flash journal (ret=%d)\n", __func__, (int) ret);
        goto out0;
    }
out0:
    return ret;
}
*/

static int32_t cfstore_flash_flush(cfstore_ctx_t* ctx)
{
    int32_t ret = ARM_DRIVER_OK;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    /* put the async completion code state variables into a known state */
    ctx->status = ARM_DRIVER_OK;
    ctx->cmd_code = (FlashJournal_OpCode_t)((int) FLASH_JOURNAL_OPCODE_RESET+1);

    /* cfstore_fsm_state_handle_event() is called at intr context via
     * cfstore_flash_journal_callback(), and hence calls from app context are
     * protected with CSs */
    cfstore_critical_section_lock(&ctx->rw_area0_lock, __func__);
    ret = cfstore_fsm_state_handle_event(&ctx->fsm, cfstore_fsm_event_commit_req, (void*) ctx);
    cfstore_critical_section_unlock(&ctx->rw_area0_lock, __func__);
    return ret;
}

#else /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

static bool cfstore_flash_journal_is_async_op_pending(cfstore_ctx_t* ctx) { CFSTORE_FENTRYLOG("%s:SRAM:entered:\n", __func__); (void) ctx; return false; }

/* @brief   generate the CFSTORE_OPCODE_INITIALIZE callback notification */
static int32_t cfstore_flash_init(void)
{
    cfstore_client_notify_data_t notify_data;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:SRAM:entered:\n", __func__);
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_INITIALIZE, ARM_DRIVER_OK, NULL);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ARM_DRIVER_OK;
}

static int32_t cfstore_flash_deinit(void){ CFSTORE_FENTRYLOG("%s:SRAM:entered:\n", __func__); return ARM_DRIVER_OK; }
/* static int32_t cfstore_flash_reset(void) { CFSTORE_FENTRYLOG("%s:SRAM:entered:\n", __func__); return ARM_DRIVER_OK; }*/
static int32_t cfstore_flash_flush(cfstore_ctx_t* ctx)
{
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:SRAM:entered:\n", __func__);
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_FLUSH, ARM_DRIVER_OK, NULL);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ARM_DRIVER_OK;
}

#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */


/** @brief  After a cfstore KV area memmove() operation, update the file pointers
 *          to reflect the new location in memory of KVs.
 *
 * @param   head
 * the position at which size_diff bytes have been inserted/deleted
 *
 * @param   size_diff
 * Change in size (size difference) of the KV memory area.
 *  - size_diff > 0 => increase in area, |size_diff| bytes have been inserted at head,
 *    and the previously following KVs shifted up to higher memory addresses
 *  - size_diff < 0 => decrease in area, |size_diff| bytes have been removed at head,
 *    and the previously following KVs shifted down to lower memory addresses
 * */
static int32_t cfstore_file_update(uint8_t* head, int32_t size_diff)
{
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_file_t* file;
    cfstore_list_node_t* node;
    cfstore_list_node_t* file_list = &ctx->file_list;

    CFSTORE_FENTRYLOG("%s:entered:(ctx->area_0_head=%p, ctx->area_0_tail=%p)\n", __func__, ctx->area_0_head, ctx->area_0_tail);

    /* walk the file list updating head pointers for the KVs that remain*/
    node = file_list->next;
    while(node != file_list){
        /* Any KV positioned later in the area than the deleted KV will require file head pointers updating.
         * If file's head pointer is beyond the deleted KV tail then the file->head needs to be updated
         * to reflect the memove
         */
        file = (cfstore_file_t*) node;
        if(file->head >= head){
            /* sign of sign_diff used to move file->head up/down in memory*/
            file->head += size_diff;
        }
        node = node->next;
    }
    return ARM_DRIVER_OK;
}


static int32_t cfstore_delete_ex(cfstore_area_hkvt_t* hkvt)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE kv_size = 0;
    ARM_CFSTORE_SIZE kv_total_size = 0;
    ARM_CFSTORE_SIZE realloc_size = 0;      /* size aligned to flash program_unit size */
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered:(ctx->area_0_head=%p, ctx->area_0_tail=%p)\n", __func__, ctx->area_0_head, ctx->area_0_tail);
    kv_size  = cfstore_hkvt_get_size(hkvt);
    kv_total_size = cfstore_ctx_get_kv_total_len();

    /* Note the following:
     *  1. memmove() above shifts the position of the KVs falling after the deleted KV to be at
     *     lower memory addresses. The code (A) updates the cfstore_file_t::head pointers for these KVs
     *     so they point to the new locations.
     *  2. The operation at 1. above has to happen before the realloc because realloc() can move the
     *     start of heap block to a new location, in which case all cfstore_file_t::head pointers
     *     need to be updated. cfstore_realloc() can only do this starting from a set of correct
     *     cfstore_file_t::head pointers i.e. after 1. has been completed.
     */
    memmove(hkvt->head, hkvt->tail, ctx->area_0_tail - hkvt->tail);
    /* zero the deleted KV memory */
    memset(ctx->area_0_tail-kv_size, 0, kv_size);

    /* The KV area has shrunk so a negative size_diff should be indicated to cfstore_file_update(). */
    ret = cfstore_file_update(hkvt->head, -1 * kv_size);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error:file update failed\n", __func__);
        goto out0;
    }

    /* setup the reallocation memory size. */
    realloc_size = kv_total_size - kv_size;
    ret = cfstore_realloc_ex(realloc_size, NULL);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error:realloc failed\n", __func__);
        goto out0;
    }
out0:
    return ret;
}


/*
 * File operations
 */

static cfstore_file_t* cfstore_file_get(ARM_CFSTORE_HANDLE hkey)
{
    return (cfstore_file_t*) hkey;
}

static cfstore_file_t* cfstore_file_create(cfstore_area_hkvt_t* hkvt, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey, cfstore_list_node_t *list_head)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_file_t* file = (cfstore_file_t*) hkey;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(file != NULL){
        memset(file, 0, sizeof(cfstore_file_t));
        CFSTORE_INIT_LIST_HEAD(&file->node);
        ret = cfstore_hkvt_refcount_inc(hkvt, NULL);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: cfstore_hkvt_refcount_inc() failed (ret=%d)\n", __func__, (int) ret);
            return NULL;
        }
        file->head = hkvt->head;
        file->flags.read = flags.read;
        file->flags.write = flags.write;
        if(list_head != NULL){
            cfstore_listAdd(list_head, &file->node, list_head);
        }
    }
    return file;
}

/* @brief   required to be in critical section when called. */
static int32_t cfstore_file_destroy(cfstore_file_t* file)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    uint8_t refcount = 0;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(file) {
        hkvt = cfstore_get_hkvt_from_head_ptr(file->head);
        CFSTORE_ASSERT(cfstore_hkvt_is_valid(&hkvt, cfstore_ctx_get()->area_0_tail) == true);
        ret = ARM_DRIVER_OK;
        cfstore_hkvt_refcount_dec(&hkvt, &refcount);
        CFSTORE_TP(CFSTORE_TP_FILE, "%s:refcount =%d file->head=%p\n", __func__, (int)refcount, file->head);
        if(refcount == 0){
            /* check for delete */
            CFSTORE_TP(CFSTORE_TP_FILE, "%s:checking delete flag\n", __func__);
            if(cfstore_hkvt_get_flags_delete(&hkvt)){
                ret = cfstore_delete_ex(&hkvt);
            }
        }
        /* reset client buffer to empty ready for reuse */
        /* delete the file even if not deleting the KV*/
        cfstore_listDel(&file->node);
        memset(file, 0, sizeof(cfstore_file_t));
    }
    return ret;
}


/**
 * @brief   check whether this is an valid buffer
 *
 * @param   hkey
 *          IN: The key handle to be validated
 *
 *          ctx
 *          IN: cfstore context block
 */
static bool cfstore_file_is_valid(ARM_CFSTORE_HANDLE hkey, cfstore_ctx_t* ctx)
{
    cfstore_file_t* file = cfstore_file_get(hkey);

    if(ctx->area_0_head != NULL && ctx->area_0_tail != NULL){
        if(file->head < ctx->area_0_head || file->head > ctx->area_0_tail){
            return 0;
        }
        return true;
    }
    return false;
}

/**
 * @brief   check whether this is an empty buffer, or whether it
 *          has valid data
 *
 * @param   hkey
 *          IN: The key handle to be validated
 *
 *          ctx
 *          IN: cfstore context block
 */
static bool cfstore_file_is_empty(ARM_CFSTORE_HANDLE hkey)
{
    ARM_CFSTORE_HANDLE_INIT(zero);
    if(hkey != NULL){
        return !memcmp(hkey, zero, CFSTORE_HANDLE_BUFSIZE);
    }
    return 0;
}


/* @brief  See definition in configuration_store.h for description. */
ARM_CFSTORE_CAPABILITIES cfstore_get_capabilities(void)
{
    /* getting capabilities doesn't change the sram area so this can happen independently of
     * an outstanding async operation. its unnecessary to check the fsm state */
    return cfstore_caps_g;
}


/* @brief   check the flags argument are supported */
static int32_t cfstore_validate_fmode_flags(ARM_CFSTORE_FMODE flags)
{
    if(flags.continuous){
        CFSTORE_ERRLOG("%s:Error:Continuous flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(flags.lazy_flush){
        CFSTORE_ERRLOG("%s:Error:Lazy flush flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(flags.flush_on_close){
        CFSTORE_ERRLOG("%s:Error:Flush on close flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(flags.storage_detect){
        CFSTORE_ERRLOG("%s:Error:Storage detect flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    return ARM_DRIVER_OK;
}


/* @brief   validate the client supplied opaque handle */
static CFSTORE_INLINE int32_t cfstore_validate_handle(ARM_CFSTORE_HANDLE hkey)
{
    if(hkey == NULL){
        return ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
    }
    return ARM_DRIVER_OK;
}

/* @brief   check the flash security features are valid (internal use only) */
static int32_t cfstore_validate_flash_security_features(const ARM_STORAGE_SECURITY_FEATURES *security)
{
    CFSTORE_ASSERT(security != NULL);

    if(security->acls){
        CFSTORE_ERRLOG("%s:Error: flash security features acls flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->internal_flash){
        CFSTORE_ERRLOG("%s:Error: flash security features internal_flash flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->rollback_protection){
        CFSTORE_ERRLOG("%s:Error: flash security features rollback_protection flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->tamper_proof){
        CFSTORE_ERRLOG("%s:Error: flash security features tamper_proof flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->board_level_attacks){
        CFSTORE_ERRLOG("%s:Error: flash security features board level attacks flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->software_attacks){
        CFSTORE_ERRLOG("%s:Error: flash security features device_software flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->chip_level_attacks){
        CFSTORE_ERRLOG("%s:Error: flash security features chip level attacks flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(security->side_channel_attacks){
        CFSTORE_ERRLOG("%s:Error: flash security features side channel attacks flag not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    return ARM_DRIVER_OK;
}

/* @brief   check the key descriptor are valid (internal use only) */
static int32_t cfstore_validate_flash_data_retention_level(const uint8_t drl)
{
    int32_t ret = ARM_DRIVER_ERROR;

    switch(drl)
    {
    case ARM_RETENTION_WHILE_DEVICE_ACTIVE :
    case ARM_RETENTION_ACROSS_SLEEP :
    case ARM_RETENTION_ACROSS_DEEP_SLEEP :
    case ARM_RETENTION_BATTERY_BACKED :
    case ARM_RETENTION_NVM :
        ret = ARM_DRIVER_OK;
        break;
    default:
        CFSTORE_ERRLOG("%s:Error: data retention level (%d) not supported.\n", __func__, drl);
        ret = ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
        break;

    }
    return ret;
}

/* @brief   check the access control list is valid (internal use only)  */
static int32_t cfstore_validate_access_control_list(const ARM_CFSTORE_ACCESS_CONTROL_LIST acl)
{
    if(acl.perm_owner_execute)
    {
        CFSTORE_ERRLOG("%s:Error: Access control list with permission owner execute set is not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    if(acl.perm_other_execute)
    {
        CFSTORE_ERRLOG("%s:Error: Access control list with permission other execute set is not supported.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_NOT_SUPPORTED;
    }
    return ARM_DRIVER_OK;
}

/* @brief   check the key descriptor is valid */
static int32_t cfstore_validate_key_desc(const ARM_CFSTORE_KEYDESC *kdesc)
{
    int32_t ret = ARM_DRIVER_ERROR;

    if(kdesc == NULL){
        return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_DESCRIPTOR;
    }
    ret = cfstore_validate_access_control_list(kdesc->acl);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    ret = cfstore_validate_flash_data_retention_level(kdesc->drl);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    ret = cfstore_validate_flash_security_features(&kdesc->security);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    ret = cfstore_validate_fmode_flags(kdesc->flags);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    return ARM_DRIVER_OK;
}

/**
 * @brief   check the key_len pointer is valid
 *
 * @param   hkey
 *          IN: The key handle to be validated
 */
static CFSTORE_INLINE int32_t cfstore_validate_len_ptr(ARM_CFSTORE_SIZE *len)
{
    if(len == NULL){
        return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_LEN;
    }
    return ARM_DRIVER_OK;
}

/* @brief	return a pointer to the next { or }, or NULL if not present */
static inline char* cfstore_validate_pos_next_brace(const char* pos)
{
	char* pos_open = strchr(pos, '{');
	char* pos_close = strchr(pos, '}');
	if(pos_open != NULL) {
		if(pos_close != NULL){
			return pos_open < pos_close ? pos_open : pos_close;
		}
		return pos_open;
	}
	return pos_close;
}


static int32_t cfstore_validate_key_name_ex(const char* key_name, const char* permissible)
{
    char* pos = NULL;
    int brace_count = 0;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_SIZE valid_len = 0;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(key_name != NULL){
        /* check the key_name is terminated by a 0 */
        pos = (char*) memchr(key_name, '\0', CFSTORE_KEY_NAME_MAX_LENGTH+1);
        if(pos == NULL){
            CFSTORE_ERRLOG("%s:key_name does not have terminating null.\n", __func__);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }
        /* check for zero length key_name*/
        if(strlen(key_name) == 0){
            CFSTORE_ERRLOG("%s:Error: invalid key_name.\n", __func__);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }
        /* check the key_name len is less than the max length (220) */
        len = strlen(key_name);
        if(len > CFSTORE_KEY_NAME_MAX_LENGTH){
            CFSTORE_ERRLOG("%s:key_name string is longer (%d) than the supported maximum (%d).\n", __func__, (int) len, (int) CFSTORE_KEY_NAME_MAX_LENGTH);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }
        /* check the key_name only contains permissible characters */
        valid_len = strspn(key_name, permissible);
        if(valid_len != len){
            CFSTORE_ERRLOG("%s:Invalid character (%c) found in key_name (key_name=%s).\n", __func__, key_name[valid_len], key_name);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }

        /*check there isnt a leading '.' on the kv name */
        if(key_name[0] == '.'){
            CFSTORE_ERRLOG("%s:Leading (.) character found in key_name (key_name=%s) is not allowed.\n", __func__, key_name);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }

        /* - check for matching '{' for each '}' present
         * - only check a string if either { or } are present
         *   i.e. dont process string without
         *   checking for existence of single brace, and checking for either { or } so
         *   that the case where } is the first brace is convered.
         * - start loop at first { or } char,  both {} covers case where } is the first brace
         * - (brace_count >=0 && brace_count <= 1) must always be true
         * - brace_count must == 0 at end of string
         */
        pos = cfstore_validate_pos_next_brace(key_name);
        while(pos != NULL && brace_count >= 0 && brace_count <= 1)
        {
        	switch(*pos)
        	{
        	case '{':
        		brace_count++;
        		break;
        	case '}':
        		brace_count--;
        		break;
        	default:
        		break;
        	}
        	pos++;
        	pos = cfstore_validate_pos_next_brace(pos);
        }
        if(brace_count != 0){
            CFSTORE_ERRLOG("%s: Unmatched brace found in key_name (count=%d.\n", __func__, brace_count);
            return ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        }
    }
    return ARM_DRIVER_OK;
}


/* @brief   check the key name is valid */
static int32_t cfstore_validate_key_name(const char* key_name)
{
    int32_t ret = ARM_DRIVER_ERROR;

    ret = cfstore_uvisor_security_context_prefix_check(key_name);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed uvisor security context check.\n", __func__);
        return ret;
    }
    return cfstore_validate_key_name_ex(key_name, CFSTORE_KEY_NAME_CHARS_ACCEPTABLE);
}

/* @brief   check the key name query is valid */
static int32_t cfstore_validate_key_name_query(const char* key_name_query)
{
    return cfstore_validate_key_name_ex(key_name_query, CFSTORE_KEY_NAME_QUERY_CHARS_ACCEPTABLE);
}


/**
 * @brief   check the value length field is valid
 *
 * @param   key_name
 *          IN: The key name string to be validated
 * @note    This will be replaced with the actual uvisor call, when available.
 */
static CFSTORE_INLINE int32_t cfstore_validate_value_len(ARM_CFSTORE_SIZE value_len)
{
    if(value_len <= CFSTORE_VALUE_SIZE_MAX) {
        return ARM_DRIVER_OK;
    }
    return ARM_CFSTORE_DRIVER_ERROR_VALUE_SIZE_TOO_LARGE;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_get_key_name_ex(cfstore_area_hkvt_t *hkvt, char* key_name, uint8_t *key_name_len)
{
    int32_t ret = ARM_DRIVER_OK;
    int32_t max_len = 0;

    max_len = cfstore_hkvt_get_key_len(hkvt) + 1;
    max_len = max_len <= *key_name_len ? max_len : *key_name_len;
    memcpy(key_name, (const char*) hkvt->key, max_len-1);
    key_name[max_len-1] = '\0';
    *key_name_len = max_len;
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_get_key_name(ARM_CFSTORE_HANDLE hkey, char* key_name, uint8_t *key_name_len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    cfstore_client_notify_data_t notify_data;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_ASSERT(key_name != NULL);
    CFSTORE_ASSERT(key_name_len != NULL);

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* getting a keyname doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    if(key_name == NULL){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_KEY_NAME;
        goto out0;
    }
    ret = cfstore_validate_len_ptr((ARM_CFSTORE_SIZE*)key_name_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key_name_len argument.\n", __func__);
        goto out0;
    }
    memset(&hkvt, 0, sizeof(hkvt));
    hkvt = cfstore_get_hkvt(hkey);
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    ret = cfstore_get_key_name_ex(&hkvt, key_name, key_name_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        goto out0;
    }
    ret = *key_name_len;
out0:
    /* GetKeyName() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_GET_KEY_NAME, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}

/* @brief  See definition in configuration_store.h for description. */
static ARM_CFSTORE_STATUS cfstore_get_status(void)
{
    ARM_CFSTORE_STATUS status;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    memset(&status, 0, sizeof(status));
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        status.error = true;
    }
    /* getting status doesnt change the sram area so this can happen independently of
     * an oustanding async operation. */
    if(cfstore_flash_journal_is_async_op_pending(ctx))
    {
        status.in_progress = true;
    }
    else
    {
        status.in_progress = false;
    }
    return status;
}

/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    int32_t ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
    cfstore_area_hkvt_t hkvt;
    cfstore_client_notify_data_t notify_data;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    CFSTORE_ASSERT(hkey != NULL);
    CFSTORE_ASSERT(value_len != NULL);

    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        goto out0;
    }
    /* getting a value len doesnt change the sram area so this can happen independently of
     * an outstanding async operation. its unnecessary to check the fsm state */
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    ret = cfstore_validate_len_ptr(value_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid value len argument.\n", __func__);
        goto out0;
    }
    hkvt = cfstore_get_hkvt(hkey);
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    *value_len = cfstore_hkvt_get_value_len(&hkvt);
    ret = (int32_t) *value_len;
out0:
    /* GetValueLen() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_GET_VALUE_LEN, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}

#ifdef CFSTORE_DEBUG

/* @brief   debug trace a struct cfstore_area_hkvt_t, providing values for key field. */
static CFSTORE_INLINE void cfstore_hkvt_dump(cfstore_area_hkvt_t* hkvt, const char* tag)
{
/* #define CFSTORE_HKVT_DUMP_ON */
#ifdef CFSTORE_HKVT_DUMP_ON
    char kname[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    char value[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint32_t klen = 0;
    uint32_t vlen = 0;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    memset(kname, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    memset(value, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
    klen = cfstore_hkvt_get_key_len(hkvt);
    vlen = cfstore_hkvt_get_value_len(hkvt);
    memcpy((void*)kname, (void*) hkvt->key, klen);
    memcpy((void*)value, (void*) hkvt->value, vlen);
    kname[klen] = '\0';
    value[vlen] = '\0';

    /* table column description
     * col 1: tag, descriptive string supplied by client to identify context of table dump
     * col 2: hkvt struct member that is to be reported i.e. head, key, value, tail
     * col 3: the value of the pointer described in col 2.
     * col 4: the value of the pointer described in col 3 as an offset from the start of the sram area
     * col 5: field specified data e.g. for header, the extracted key length, value_length.
     */
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:hkvt->head:%8p:%8p:klen=%08d:vlen=%08d:\n", tag, hkvt->head, (void*)(hkvt->head - ctx->area_0_head), (int) klen, (int) vlen);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:hkvt->key :%8p:%8p:%s\n", tag, hkvt->key, (void*)(hkvt->key - ctx->area_0_head), kname);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:hkvt->val :%8p:%8p:%s\n", tag, hkvt->value, (void*)(hkvt->value - ctx->area_0_head), value);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:hkvt->tail:%8p:%8p:\n", tag, hkvt->tail, (void*)(hkvt->tail - ctx->area_0_head));
    return;
#else
    (void) hkvt;
    (void) tag;

#endif /*  CFSTORE_HKVT_DUMP_ON */
}

static CFSTORE_INLINE void cfstore_flags_dump(ARM_CFSTORE_FMODE flag, const char* tag)
{
    int pos = 0;
    char flags[9];

    pos += snprintf(&flags[pos], 9, "%c", flag.continuous ? 'C' : 'c');
    pos += snprintf(&flags[pos], 9, "%c", flag.lazy_flush ? 'L' : 'l');
    pos += snprintf(&flags[pos], 9, "%c", flag.flush_on_close ? 'F' : 'f');
    pos += snprintf(&flags[pos], 9, "%c", flag.read ? 'R' : 'r');
    pos += snprintf(&flags[pos], 9, "%c", flag.write ? 'W' : 'w');
    pos += snprintf(&flags[pos], 9, "%c", flag.storage_detect ? 'S' : 's');
    pos += snprintf(&flags[pos], 9, "--");

    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:flags     :%s:(C=>continuous set, L=>lazy flush, F=>flush on close, R=>read, W=>write, S=>storage detect)\n", tag, flags);
    return;
}

static CFSTORE_INLINE void cfstore_file_dump(cfstore_file_t* file, const char* tag)
{
/*#define CFSTORE_FILE_DUMP_ON */
#ifdef CFSTORE_FILE_DUMP_ON
    cfstore_area_hkvt_t hkvt;

    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:*** Dumping File Contents : Start ***\n", tag);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:file==hkey:%p\n", tag, file);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:rloc/wloc :%08u/%08u:\n", tag, (unsigned int)  file->rlocation, (unsigned int)  file->wlocation);
    cfstore_flags_dump(file->flags, tag);
    hkvt = cfstore_get_hkvt((ARM_CFSTORE_HANDLE)file);
    cfstore_hkvt_dump(&hkvt, tag);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:*** Dumping File Contents : End ***\n", tag);
    return;
#else
    (void) file;
    (void) tag;

#endif /*  CFSTORE_FILE_DUMP_ON */
}

/* dump sram contents of cfstore in a useful manner for debugging */
static CFSTORE_INLINE void cfstore_dump_contents(const char* tag)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:*** Dumping CFSTORE Contents : Start ***\n", tag);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:cfstore_ctx_g.area_0_head=%8p\n", tag, ctx->area_0_head);
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:cfstore_ctx_g.area_0_tail=%8p\n", tag, ctx->area_0_tail);
    ret = cfstore_get_head_hkvt(&hkvt);
    if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
        CFSTORE_TP(CFSTORE_TP_VERBOSE1, "%s:CFSTORE has no KVs\n", tag);
        goto out0;
    } else if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: could not get head of list.\n", tag);
        goto out0;
    }
    while(cfstore_get_next_hkvt(&hkvt, &hkvt) != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND)
    {
        cfstore_hkvt_dump(&hkvt, tag);
    }
out0:
    CFSTORE_TP(CFSTORE_TP_VERBOSE3, "%s:*** Dumping CFSTORE Contents : End ***\n", tag);
    return;
}



#else
static CFSTORE_INLINE void cfstore_hkvt_dump(cfstore_area_hkvt_t* hkvt, const char* tag){ (void) hkvt; (void) tag; return; }
static CFSTORE_INLINE void cfstore_file_dump(cfstore_file_t* file, const char* tag){ (void) file; (void) tag;  return; }
static CFSTORE_INLINE void cfstore_dump_contents(const char* tag){ (void) tag;  return; }
static CFSTORE_INLINE void cfstore_flags_dump(ARM_CFSTORE_FMODE flag, const char* tag){ (void) flag; (void) tag;  return; }
#endif /*CFSTORE_DEBUG*/

/*
 * CS operations
 */

/* @brief  See definition in configuration_store.h for description. */
ARM_DRIVER_VERSION cfstore_get_version(void)
{
    /* getting version info doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    return cfstore_driver_version_g;
}


/*
 * CS API Key-Value operations
 */

/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_delete(ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_TP((CFSTORE_TP_DELETE|CFSTORE_TP_FENTRY), "%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_TP(CFSTORE_TP_DELETE, "%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* deleting a key will change the sram area while a logging/flushing operation is pending, which
     * should not happen while an async operation is outstanding */
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_DELETE, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
        goto out0;
    }
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    if(!cfstore_is_kv_client_deletable((cfstore_file_t*) hkey)){
        CFSTORE_ERRLOG("%s:Error: client is not permitted to delete KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS;
        goto out0;
    }
    hkvt = cfstore_get_hkvt(hkey);
    /* check its a valid hkvt */
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    /* set the delete flag so the delete occurs when the file is closed
     * no further handles will be returned to this key */
    cfstore_hkvt_set_flags_delete(&hkvt, true);

    /* set the dirty flag so the changes are persisted to backing store when flushed */
    ctx->area_dirty_flag = true;

out0:
    /* Delete() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_DELETE, ret, NULL);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/** @brief  Internal find function using hkvt's.
 *
 * @note
 * Not the following:
 *  - Any required locks should be taken before this function is called.
 *    This function does not affect refcount for underlying KVs.
 *  - The function assumes the arguments have been validated before calling this function
 *  - No acl policy is enforced by the function.
 *
 * @return  return_value
 *          On success (finding a KV matching the query) ARM_DRIVER_OK is
 *          returned. If a KV is not found matching the description then
 *          ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND is returned.
 */
static int32_t cfstore_find_ex(const char* key_name_query, cfstore_area_hkvt_t *prev, cfstore_area_hkvt_t *next)
{
    int32_t ret = ARM_DRIVER_ERROR;
    uint8_t next_key_len;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_TP((CFSTORE_TP_FIND|CFSTORE_TP_FENTRY), "%s:entered: key_name_query=\"%s\", prev=%p, next=%p\n", __func__, key_name_query, prev, next);
    if(prev == NULL){
        ret = cfstore_get_head_hkvt(next);
        /* CFSTORE_TP(CFSTORE_TP_FIND, "%s:next->head=%p, next->key=%p, next->value=%p, next->tail=%p, \n", __func__, next->head, next->key, next->value, next->tail); */
        if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:CFSTORE has no KVs\n", __func__);
            return ret;
        } else if(ret < ARM_DRIVER_OK) {
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:failed to find the first KV in area\n", __func__);
            return ret;
        }

        /* check for no KVs in the store => hkvt is not valid */
        if(!cfstore_hkvt_is_valid(next, ctx->area_0_tail)){
            /* no KVs in store */
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:hkvt is not valid\n", __func__);
            return ARM_DRIVER_OK;
        }

    } else {
        /* CFSTORE_TP(CFSTORE_TP_FIND, "%s:getting hkvt from prev\n", __func__);*/
        ret = cfstore_get_next_hkvt(prev, next);
        if(ret < ARM_DRIVER_OK){
            /* no more matching entries or error.
             * either way, return*/
            return ret;
        }
    }
    if(next->head == NULL){
        /* no entry*/
        CFSTORE_TP(CFSTORE_TP_FIND, "%s:No more entries found\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
    }
    /* CFSTORE_TP(CFSTORE_TP_FIND, "%s:cfstore_ctx_g.area_0_head=%p, cfstore_ctx_g.area_0_tail=%p\n", __func__, cfstore_ctx_g.area_0_head, cfstore_ctx_g.area_0_tail);*/
    cfstore_hkvt_dump(next, __func__);
    while(cfstore_hkvt_is_valid(next, ctx->area_0_tail))
    {
        /* CFSTORE_TP(CFSTORE_TP_FIND, "%s:next->head=%p, next->key=%p, next->value=%p, next->tail=%p, \n", __func__, next->head, next->key, next->value, next->tail); */
        cfstore_hkvt_dump(next, __func__);

        /* if this KV is deleting then proceed to the next item */
        if(cfstore_hkvt_get_flags_delete(next)){
            ret = cfstore_get_next_hkvt(next, next);
            if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
                CFSTORE_TP(CFSTORE_TP_FIND, "%s:No more KVs found\n", __func__);
                return ret;
            }
            continue;
        }
        /* if this KV is not readable by the client then proceed to the next item */
        if(!cfstore_is_kv_client_readable(next)){
            ret = cfstore_get_next_hkvt(next, next);
            if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
                CFSTORE_TP(CFSTORE_TP_FIND, "%s:No more KVs found\n", __func__);
                return ret;
            }
            continue;
        }
        /* check if this key_name matches the query */
        next_key_len = cfstore_hkvt_get_key_len(next);
        next_key_len++;
        cfstore_get_key_name_ex(next, key_name, &next_key_len);
        ret = cfstore_fnmatch(key_name_query, key_name, 0);
        if(ret == 0){
            /* found the entry in the store. return handle */
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:Found matching key (key_name_query = \"%s\", next->key = \"%s\"),next_key_len=%d\n", __func__, key_name_query, key_name, (int) next_key_len);
            cfstore_hkvt_dump(next, __func__);
            return ARM_DRIVER_OK;
        } else if(ret != CFSTORE_FNM_NOMATCH){
            CFSTORE_ERRLOG("%s:Error: cfstore_fnmatch() error (ret=%d).\n", __func__, (int) ret);
            return ARM_DRIVER_ERROR;
        }
        /* CFSTORE_FNM_NOMATCH => get the next hkvt if any */
        ret = cfstore_get_next_hkvt(next, next);
        if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:No more KVs found\n", __func__);
            return ret;
        }
    }
    return ARM_DRIVER_OK;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_find(const char* key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_len = 0;
    cfstore_area_hkvt_t hkvt_next;
    cfstore_area_hkvt_t hkvt_previous;
    cfstore_area_hkvt_t *phkvt_previous = NULL;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_FMODE fmode;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_ASSERT(next != NULL);
    CFSTORE_FENTRYLOG("%s:entered: key_name_query=\"%s\", previous=%p, next=%p\n", __func__, key_name_query, previous, next);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out1;
    }
    /* finding a key doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    ret = cfstore_validate_key_name_query(key_name_query);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key_name.\n", __func__);
        goto out1;
    }
    ret = cfstore_validate_handle(next);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid next argument.\n", __func__);
        goto out1;
    }
    /* note previous can be NULL if this is the first call the find */
    memset(&hkvt_next, 0, sizeof(hkvt_next));
    memset(&fmode, 0, sizeof(fmode));
    if(previous != NULL && cfstore_file_is_valid(previous, ctx)){
        ret = cfstore_validate_handle(previous);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
            goto out1;
        }
        phkvt_previous = &hkvt_previous;
        memset(phkvt_previous, 0, sizeof(hkvt_previous));
        hkvt_previous = cfstore_get_hkvt(previous);
        cfstore_hkvt_dump(&hkvt_previous, __func__);
        if(!cfstore_hkvt_is_valid(phkvt_previous, ctx->area_0_tail)){
            ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
            goto out1;
        }
    } else if(previous != NULL && !cfstore_file_is_empty(previous)){
        CFSTORE_TP(CFSTORE_TP_FIND, "%s:Invalid previous hkey buffer.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE_BUF;
        goto out1;
    }
    ret = cfstore_find_ex(key_name_query, phkvt_previous, &hkvt_next);
    if(ret < ARM_DRIVER_OK){
        /* either no more entries or error but either way, return */
        CFSTORE_TP(CFSTORE_TP_FIND, "%s:No more KVs found.\n", __func__);
        goto out2;
    }

    if(!cfstore_hkvt_is_valid(&hkvt_next, ctx->area_0_tail)){
        CFSTORE_TP(CFSTORE_TP_FIND, "%s:Did not find any matching KVs.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
        goto out2;
    }

    /* Have a valid kv. cfstore_find_ex() checked the client has
     * permission to read the KV, so dont have to perform this check again here. */

    /* return handle to client */
    cfstore_file_create(&hkvt_next, fmode, next, &ctx->file_list);
    ret = ARM_DRIVER_OK;
out2:
    /* previous handle is being returned to CFSTORE with this call so destroy file struct */
    if(previous != NULL && cfstore_file_is_valid(previous, ctx))
    {
        /* do not use ret in this stanza as will loose return state from above */
        /* CFSTORE_TP(CFSTORE_TP_FIND, "%s:about to destroy KV, previous=%p.\n", __func__, previous); */
        cfstore_file_dump((cfstore_file_t*) previous, __func__);

        key_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);

        cfstore_file_destroy(cfstore_file_get(previous));

        /* check hkvt is valid before trying to retrieve name*/
        if(!cfstore_hkvt_is_valid(&hkvt_next, ctx->area_0_tail)){
            goto out1;
        }
        if(cfstore_get_key_name_ex(&hkvt_next, key_name, &key_len) < ARM_DRIVER_OK){
            /* either no more entries or error but either way, return */
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:debug: cfstore_get_key_name_ex failed or no more kvs.\n", __func__);
            goto out1;
        }
        /* now get hkvt_next again based on the name to overcome the fact that the hkvt
         * may be invalid due to the possible deletion of the previous KV.x */
        if(cfstore_find_ex(key_name, NULL, &hkvt_next) < ARM_DRIVER_OK){
            /* either no more entries or error but either way, return */
            CFSTORE_TP(CFSTORE_TP_FIND, "%s:find failed key_name=%s ret=%d.\n", __func__, key_name, (int) ret);
            goto out1;
        }
        cfstore_hkvt_dump(&hkvt_next, __func__);
    }
out1:
    /* Find() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_FIND, ret, next);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  grow/shrink pre-existing KV.
 *
 * @note rw_lock must be held by the caller of this function rw_area0_lock */
static int32_t cfstore_recreate(const char* key_name, ARM_CFSTORE_SIZE value_len, ARM_CFSTORE_HANDLE hkey, cfstore_area_hkvt_t* hkvt)
{
    uint8_t* old_area_0_head = NULL;
    int32_t kv_size_diff = 0;
    int32_t ret = ARM_DRIVER_ERROR;
    size_t memmove_len = 0;
    ARM_CFSTORE_SIZE area_size = 0;
    ARM_CFSTORE_FMODE flags;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered: key_name=\"%s\", value_len=%d\n", __func__, key_name, (int) value_len);
    cfstore_dump_contents(__func__);
    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    flags.write = true;
    kv_size_diff = value_len - cfstore_hkvt_get_value_len(hkvt);
    if(kv_size_diff == 0){
        /* nothing more to do*/
        CFSTORE_TP(CFSTORE_TP_CREATE, "%s:new value length the same as the old\n", __func__);
        return ARM_DRIVER_OK;
    }

    /* grow the area by the size of the new KV */
    area_size = cfstore_ctx_get_kv_total_len();
    /* store the area_0_head, and move length for later updating hkvt if realloc moves KV area */
    old_area_0_head = ctx->area_0_head;
    memmove_len = ctx->area_0_tail - hkvt->tail;

    CFSTORE_TP(CFSTORE_TP_CREATE, "%s:cfstore_ctx_g.area_0_head=%p, cfstore_ctx_g.area_0_tail=%p\n", __func__, ctx->area_0_head, ctx->area_0_tail);
    CFSTORE_TP(CFSTORE_TP_CREATE, "%s:sizeof(header)=%d, sizeof(key)=%d, sizeof(value)=%d, kv_size_diff=%d, area_size=%d\n", __func__, (int) sizeof(cfstore_area_header_t),  (int)(strlen(key_name)), (int)value_len, (int) kv_size_diff, (int) area_size);
    if (kv_size_diff < 0){
        /* value blob size shrinking => do memmove() before realloc() which will free memory */
        memmove(hkvt->tail + kv_size_diff, hkvt->tail, memmove_len);
        ret = cfstore_file_update(hkvt->head, kv_size_diff);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error:file update failed\n", __func__);
            goto out0;
        }
    }

    ret = cfstore_realloc_ex(area_size + kv_size_diff, NULL);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error:file realloc failed\n", __func__);
        goto out0;
    }
    if(old_area_0_head != ctx->area_0_head){
        /* As realloc() has caused the memory to move, hkvt needs re-initialising */
        hkvt->head += ctx->area_0_head - old_area_0_head;
        hkvt->key += ctx->area_0_head - old_area_0_head;
        hkvt->value +=  ctx->area_0_head - old_area_0_head;
        hkvt->tail += ctx->area_0_head - old_area_0_head;
    }

    if(kv_size_diff > 0) {
        /* value blob size growing requires memmove() after realloc() */
        memmove(hkvt->tail+kv_size_diff, hkvt->tail, memmove_len);
        ret = cfstore_file_update(hkvt->head, kv_size_diff);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error:file update failed\n", __func__);
            goto out0;
        }
    }
    /* hkvt->head, hkvt->key and hkvt->value remain unchanged but hkvt->tail has moved. Update it.*/
    hkvt->tail = hkvt->tail + kv_size_diff;

    /* set the new value length in the header */
    cfstore_hkvt_set_value_len(hkvt, value_len);
    cfstore_file_create(hkvt, flags, hkey, &ctx->file_list);
    ctx->area_dirty_flag = true;

#ifdef CFSTORE_DEBUG
    cfstore_hkvt_dump(hkvt, __func__);
    cfstore_dump_contents(__func__);
#endif
    ret = ARM_DRIVER_OK;
out0:
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_create(const char* key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    bool b_acl_default = false;
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t cfstore_uvisor_box_id = 0;
    ARM_CFSTORE_SIZE area_size = 0;
    ARM_CFSTORE_SIZE kv_size = 0;
    ARM_CFSTORE_SIZE realloc_size = 0;
    cfstore_area_header_t* hdr;
    cfstore_area_hkvt_t hkvt;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    ARM_CFSTORE_FMODE flags;
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:entered: key_name=\"%s\", value_len=%d, kdesc=%p\n", __func__, key_name, (int)value_len, kdesc);
    CFSTORE_ASSERT(kdesc != NULL);
    CFSTORE_ASSERT(hkey != NULL);

    memset(&flags, 0, sizeof(flags));
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
    }
    /* creating a key cannot happen while a flashJournal_log() is pending as it would change the sram area being logged*/
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_CREATE, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
    }
    ret = cfstore_validate_key_name(key_name);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key_name (%s).\n", __func__, key_name);
        goto out0;
    }
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    ret = cfstore_validate_value_len(value_len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key_name.\n", __func__);
        goto out0;
    }
    /* check uvisor security */
    if(cfstore_is_client_kv_owner(key_name, &cfstore_uvisor_box_id) != ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: Client has insufficient permissions to create KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS;
        goto out0;
    }
    /* the cfstore (uvisor) client is the owner of the KV and therefore is permitted to created it */
    /* A null kdesc is permitted if client is growing/shrinking pre-existing key.
     * Hence, find if key_name pre-exists before validating kdesc */
    ret = cfstore_find_ex(key_name, NULL, &hkvt);
    if(ret < ARM_DRIVER_OK && ret != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND){
        CFSTORE_ERRLOG("%s:CFSTORE find() returned error (%d)\n", __func__, (int) ret);
        goto out1;
    }

    if(ret != ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND && cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        /* found pre-existing entry; */
        if(cfstore_hkvt_get_flags_delete(&hkvt)){
            CFSTORE_ERRLOG("%s:CFSTORE pre-existing KV with key_name=\"%s\" deleting\n", __func__, key_name);
            ret = ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY_DELETING;
            goto out1;
        }
        if(kdesc != NULL) {
            CFSTORE_ERRLOG("%s:CFSTORE contains pre-existing KV with key_name=\"%s\". Cannot create a new KV with the same name\n", __func__, key_name);
            ret = ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY;
            goto out1;
        }

        /* client is requesting to grow/shrink pre-existing key */
        ret = cfstore_recreate(key_name, value_len, hkey, &hkvt);
        goto out1;
    }
    /* not a valid hkvt implying the key_name wasnt found */

    /* create new key */
    ret = cfstore_validate_key_desc(kdesc);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key descriptor.\n", __func__);
        goto out1;
    }
    /* insert the KV into the area */
    kv_size = strlen(key_name);
    kv_size += value_len;
    kv_size += sizeof(cfstore_area_header_t);

    /* grow the area by the size of the new KV
     * In the general case the new ((area_size + kv_size) % program_unit > 0). The new area_size is
     * aligned to a program_unit boundary to facilitate r/w to flash and so the memory realloc size
     * is calculated to align, as follows */
    area_size = cfstore_ctx_get_kv_total_len();
    /* setup the reallocation memory size. */
    realloc_size = area_size + kv_size;
    ret = cfstore_realloc_ex(realloc_size, NULL);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error:file realloc failed\n", __func__);
        goto out1;
    }

    /* determine if should adopt a default behavior for acl permission setting */
    if(cfstore_acl_is_default(kdesc->acl)){
        /* set as read-write by default default */
        CFSTORE_TP(CFSTORE_TP_CREATE, "%s:Note: No ACL bits set. Adopting default permissions of owner read and write.\n", __func__);
        b_acl_default = true;
    }
    /* set the header up, then copy key_name into header */
    hdr = (cfstore_area_header_t*) (ctx->area_0_head + area_size);
    CFSTORE_FENTRYLOG("%s:hdr=%p\n", __func__, hdr);
    hdr->klength = (uint8_t) strlen(key_name);
    hdr->vlength = value_len;
    hdr->perm_owner_read = b_acl_default ? true : kdesc->acl.perm_owner_read;
    hdr->perm_owner_write = b_acl_default ? true : kdesc->acl.perm_owner_write;
    hdr->perm_owner_execute = kdesc->acl.perm_owner_execute;
    hdr->perm_other_read = kdesc->acl.perm_other_read;
    hdr->perm_other_write = kdesc->acl.perm_other_write;
    hdr->perm_other_execute = kdesc->acl.perm_other_execute;
    strncpy((char*)hdr + sizeof(cfstore_area_header_t), key_name, strlen(key_name));
    hkvt = cfstore_get_hkvt_from_head_ptr((uint8_t*) hdr);
    if(cfstore_flags_is_default(kdesc->flags)){
        /* set as read-only by default default */
        flags.read = true;
        flags.write = true;
    } else {
        flags.read = kdesc->flags.read;
        flags.write = kdesc->flags.write;
    }
    cfstore_file_create(&hkvt, flags, hkey, &ctx->file_list);
    ctx->area_dirty_flag = true;
    ret = ARM_DRIVER_OK;
out1:
    cfstore_hkvt_dump(&hkvt,  __func__);
out0:
    cfstore_dump_contents(__func__);
    /* Create() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_CREATE, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_open(const char* key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    cfstore_file_t *file = NULL;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    cfstore_flags_dump(flags, __func__);
    CFSTORE_ASSERT(key_name != NULL);
    CFSTORE_ASSERT(hkey != NULL);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out1;
    }
    ret = cfstore_validate_key_name(key_name);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid key_name.\n", __func__);
        goto out1;
    }
    ret = cfstore_validate_fmode_flags(flags);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid flags.\n", __func__);
        goto out1;
    }
    if(flags.write){
        /* opening a pre-existing key for writing can result in the sram area being changed, which
         * cannot happen while a flashJournal_xxx() async completion notification is outstanding */
        if(cfstore_flash_journal_is_async_op_pending(ctx)) {
            CFSTORE_TP(CFSTORE_TP_OPEN, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
            ret = ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
            goto out1;
        }
    }
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out1;
    }
    /* find the KV and return a handle */
    cfstore_hkvt_init(&hkvt);
    ret = cfstore_find_ex(key_name, NULL, &hkvt);
    if(ret < ARM_DRIVER_OK){
        /* either no more entries or error but either way, return */
        CFSTORE_TP(CFSTORE_TP_OPEN, "%s:debug: find failed or no more kvs.\n", __func__);
        goto out1;
    }
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail))
    {
        CFSTORE_ERRLOG("%s:Error: Could not find pre-existing key to open with key_name=(%s).\n", __func__, key_name);
        ret = ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND;
        goto out1;
    }
    /* if this KV is deleting then do not allow item to be opened */
    if(cfstore_hkvt_get_flags_delete(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: Pre-existing key key_name=(%s) is deleting.\n", __func__, key_name);
        ret = ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY_DELETING;
        goto out1;
    }
    /* key found, check permissions */
    if(cfstore_flags_is_default(flags)){
        /* set as read-only by default default */
        flags.read = true;
    }
    if(flags.read == true && !cfstore_is_kv_client_readable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: Client has no read access to KV (key_name=%s).\n", __func__, key_name);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_READ_ACCESS;
        goto out1;
    }
    if(flags.write == true && !cfstore_is_kv_client_writable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: Client has no write access to KV (key_name=%s).\n", __func__, key_name);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_WRITE_ACCESS;
        goto out1;
    }
    if(flags.execute == true && !cfstore_is_kv_client_executable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: Client has no execute access to KV (key_name=%s).\n", __func__, key_name);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_EXECUTE_ACCESS;
        goto out1;
    }
    /* return handle to client */
    file = cfstore_file_create(&hkvt, flags, hkey, &ctx->file_list);
    if(file) {
        cfstore_file_dump(file, __func__);
    } else {
        CFSTORE_ERRLOG("%s:Error: failed to create file (key_name=%s).\n", __func__, key_name);
    }
out1:
    /* Open() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_OPEN, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_close(ARM_CFSTORE_HANDLE hkey)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;
    cfstore_area_hkvt_t hkvt;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* closing a key can lead to its deletion, which cannot happening while there are pending
     * async operations outstanding */
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_CLOSE, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
        goto out0;
    }
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid hkey argument.\n", __func__);
        goto out0;
    }
    /* check the hkey is valid */
    hkvt = cfstore_get_hkvt(hkey);
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    if(!cfstore_is_kv_client_closable((cfstore_file_t*) hkey)){
        CFSTORE_ERRLOG("%s:Error: client is not permitted to close KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS;
        goto out0;
    }
    /* delete the file associated with this open handle */
    CFSTORE_TP(CFSTORE_TP_CLOSE, "%s:about to call cfstore_file_destroy().\n", __func__);
    cfstore_file_dump((cfstore_file_t*) hkey, __func__);
    ret = cfstore_file_destroy(cfstore_file_get(hkey));
out0:
    /* Close() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_CLOSE, ret, NULL);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE read_len = 0;
    cfstore_area_hkvt_t hkvt;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_file_t* file = cfstore_file_get(hkey);
    cfstore_client_notify_data_t notify_data;

    CFSTORE_ASSERT(data);
    CFSTORE_ASSERT(len);
    CFSTORE_FENTRYLOG("%s:entered, hkey=%p\n", __func__, hkey);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* reading KVs doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    if(data == NULL){
        CFSTORE_ERRLOG("%s:Error: invalid read data buffer.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_READ_BUFFER;
        goto out0;
    }
    ret = cfstore_validate_len_ptr(len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid len argument.\n", __func__);
        goto out0;
    }
    cfstore_hkvt_init(&hkvt);
    hkvt = cfstore_get_hkvt(hkey);
    /* check the hkey is valid */
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }

    if(!cfstore_is_kv_client_readable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: client does not have permission to read KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_READ_ACCESS;
        goto out0;
    }
    read_len = *len <= (cfstore_hkvt_get_value_len(&hkvt) - file->rlocation) ? *len : cfstore_hkvt_get_value_len(&hkvt) - file->rlocation;
    memcpy(data, hkvt.value + file->rlocation, read_len);
    file->rlocation += read_len;
    *len = read_len;
    ret = read_len;
out0:
    /* Read() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_READ, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE value_len = 0;
    cfstore_area_hkvt_t hkvt;
    cfstore_file_t* file = cfstore_file_get(hkey);
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:entered, hkey=%p\n", __func__, hkey);
    CFSTORE_ASSERT(hkey != NULL);
    CFSTORE_ASSERT(len != NULL);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* writing a key cannot happen while a flashJournal_xxx() async operation is pending */
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_WRITE, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
        goto out0;
    }
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        goto out0;
    }
    if(data == NULL){
        CFSTORE_ERRLOG("%s:Error: invalid write data buffer.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_WRITE_BUFFER;
        goto out0;
    }
    ret = cfstore_validate_len_ptr(len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid len argument.\n", __func__);
        goto out0;
    }
    ret = cfstore_validate_value_len(*len);
    if (ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:Error: invalid key_name.\n", __func__);
        goto out0;
    }
    /*check file has write permission set */
    if(!file->flags.write){
        CFSTORE_ERRLOG("%s:Error: KV is read-only.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_KEY_READ_ONLY;
        goto out0;
    }
    memset(&hkvt, 0, sizeof(hkvt));
    hkvt = cfstore_get_hkvt(hkey);
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:Error: ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    if(!cfstore_is_kv_client_writable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: client does not have permission to write KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_WRITE_ACCESS;
        goto out0;
    }
    value_len = (ARM_CFSTORE_SIZE) cfstore_hkvt_get_value_len(&hkvt);
    *len = *len < value_len ? *len: value_len;
    memcpy(hkvt.value + file->wlocation, data, *len);
    file->wlocation += *len;
    cfstore_hkvt_dump(&hkvt, __func__);
    ctx->area_dirty_flag = true;
    ret = *len;
out0:
    /* Write() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_WRITE, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_area_hkvt_t hkvt;
    cfstore_file_t* file = cfstore_file_get(hkey);
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
    }
    /* read-seeking KVs doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    ret = cfstore_validate_handle(hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: invalid handle.\n", __func__);
        return ret;
    }
    ret = cfstore_validate_value_len(offset);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: offset (%u) greater than maximum value blob size (%u).\n", __func__, (unsigned int) offset, CFSTORE_VALUE_SIZE_MAX);
        return ret;
    }
    if(!file->flags.read){
        CFSTORE_ERRLOG("%s:Error: KV is not readable.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_KEY_UNREADABLE;
        goto out0;

    }
    cfstore_hkvt_init(&hkvt);
    hkvt = cfstore_get_hkvt(hkey);
    if(!cfstore_hkvt_is_valid(&hkvt, ctx->area_0_tail)){
        CFSTORE_ERRLOG("%s:Error: ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_HANDLE;
        goto out0;
    }
    if(!cfstore_is_kv_client_readable(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: client does not have permission to read KV.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_PERM_NO_READ_ACCESS;
        goto out0;
    }
    /* check offset is in range */
    if(offset > cfstore_hkvt_get_value_len(&hkvt)){
        CFSTORE_ERRLOG("%s:Error: seeking beyond end of value.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_INVALID_SEEK;
        goto out0;

    }
    file->rlocation = offset;
    ret = (int32_t) offset;
out0:
    /* Rseek() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_RSEEK, ret, hkey);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_flush(void)
{
	int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();

	CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* only 1 flush operation can be outstanding so check whether one is already in progress */
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_FLUSH, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
    }
    ret = cfstore_flash_flush(ctx);
    if(ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:Error: cfstore_flash_flush() returned error (ret=%d).\n", __func__, (int) ret);
        goto out0;
    }
out0:
    return ret;
}

/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_initialise(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
	int ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
    ARM_STORAGE_CAPABILITIES storage_caps;
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

    CFSTORE_FENTRYLOG("%s:entered: callback=%p, client_context=%p, ref_count=%d\n", __func__, callback, client_context, (int) ctx->init_ref_count);
    /* init cfstore context the first time this method is called
     * note ctx->rw_area0_lock has already been initialised */

    /* CS protection required to get into the fsm into the initing state, without another client g*/
    cfstore_critical_section_lock(&ctx->rw_area0_lock, __func__);
    if(ctx->init_ref_count == 0)
    {
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:debug: first time init\n", __func__);
        /* perform first time initialisation */
        ctx->init_ref_count++;
        /* initially there is no memory allocated for the area */
        CFSTORE_INIT_LIST_HEAD(&ctx->file_list);
        /* ctx->rw_area0_lock initialisation is not required here as the lock is statically initialised to 0 */
        ctx->area_0_head = NULL;
        ctx->area_0_tail = NULL;

        CFSTORE_ASSERT(sizeof(cfstore_file_t) == CFSTORE_HANDLE_BUFSIZE);
        if(sizeof(cfstore_file_t) != CFSTORE_HANDLE_BUFSIZE){
            CFSTORE_ERRLOG("%s:Error: sizeof(cfstore_file_t)=(%d) != CFSTORE_HANDLE_BUFSIZE (%d)\n", __func__,(int) sizeof(cfstore_file_t), (int) CFSTORE_HANDLE_BUFSIZE);
            ret = ARM_CFSTORE_DRIVER_ERROR_INTERNAL;
            goto out0;
        }
        ctx->client_callback = callback;
        ctx->client_context = client_context;
        ctx->area_dirty_flag = false;
        ctx->client_callback_notify_flag = false;

        cfstore_client_notify_data_init(&ctx->client_notify_data, CFSTORE_OPCODE_MAX, ARM_DRIVER_ERROR, NULL);
        ctx->power_state = ARM_POWER_FULL;
        ctx->status = ARM_DRIVER_OK;

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
        /* set the cfstore async flag according to the storage driver mode */
        storage_caps = cfstore_storage_drv->GetCapabilities();
        cfstore_caps_g.asynchronous_ops = storage_caps.asynchronous_ops;
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

        ret = cfstore_flash_init();
        if(ret < ARM_DRIVER_OK) {
            CFSTORE_ERRLOG("%s:Error: failed to initialise flash layer\n", __func__);
            goto out0;
        }
    }
    else
    {
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:debug: n-th time init\n", __func__);
        /* initialisation already done so only increment the ref count */
        ctx->init_ref_count++;
        ret = ARM_DRIVER_OK;
    }
out0:
    /* if not initialised already, fsm now in the initing state so safe to come out of CS */
    cfstore_critical_section_unlock(&ctx->rw_area0_lock, __func__);
    CFSTORE_FENTRYLOG("%s:exiting: callback=%p, client_context=%p, ref_count=%d\n", __func__, callback, client_context, (int) ctx->init_ref_count);
    return ret;
}


/* @brief  See prototype definition in configuration_store.h for function description.
 *
 * @note    unitialising cfstore results in all entries that have not been flushed being lost
 */
static int32_t cfstore_uninitialise(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_STORAGE_CAPABILITIES caps;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_file_t* file;
    cfstore_list_node_t* node;
    cfstore_list_node_t* file_list = &ctx->file_list;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(&caps, 0, sizeof(caps));

    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out;
    }
    /* only uninitialise when there are no flash journal async operations pending*/
    if(cfstore_flash_journal_is_async_op_pending(ctx)) {
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:Debug: flash journal operation pending (awaiting asynchronous notification).\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_OPERATION_PENDING;
        goto out;
    }
    if(ctx->init_ref_count > 0) {
        ctx->init_ref_count--;
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:Debug: decremented init_ref_count (%d).\n", __func__, (int) ctx->init_ref_count);
    }
    if(ctx->init_ref_count == 0)
    {
        CFSTORE_TP(CFSTORE_TP_INIT, "%s:Debug: init_ref_count == 0 (%d) so uninitialising.\n", __func__, (int) ctx->init_ref_count);
        /* check file list is empty and if not, free the items */
        if(ctx->file_list.next != ctx->file_list.prev)
        {
            /* list is not empty. walk the list and close the files, cleaning up state */
            node = file_list->next;
            while(node != file_list){
                file = (cfstore_file_t*) node;
                cfstore_close((ARM_CFSTORE_HANDLE) file);
                node = node->next;
            }
        }
        ret = cfstore_flash_deinit();
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to uninitialise flash journal layer.\n", __func__);
            goto out;
        }
        if(ctx->area_0_head){
            CFSTORE_FREE(ctx->area_0_head);
            ctx->area_0_head = NULL;
            ctx->area_0_tail = NULL;
            ctx->area_0_len = 0;
        }
    }
out:
    /* notify client */
    cfstore_client_notify_data_init(&ctx->client_notify_data, CFSTORE_OPCODE_UNINITIALIZE, ret, NULL);
    cfstore_ctx_client_notify(ctx, &ctx->client_notify_data);
    return ret;
}


/* @brief  See definition in configuration_store.h for description. */
static int32_t cfstore_power_control(ARM_POWER_STATE state)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t* ctx = cfstore_ctx_get();
    cfstore_client_notify_data_t notify_data;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    if(!cfstore_ctx_is_initialised(ctx)) {
        CFSTORE_ERRLOG("%s:Error: CFSTORE is not initialised.\n", __func__);
        ret = ARM_CFSTORE_DRIVER_ERROR_UNINITIALISED;
        goto out0;
    }
    /* setting power state doesnt change the sram area so this can happen independently of
     * an oustanding async operation. its unnecessary to check the fsm state */
    if(state <= ARM_POWER_FULL){
        ctx->power_state = state;
        /* set return to a positive value*/
        ret = (int32_t) state;
    }
out0:
    /* PowerControl() always completes synchronously irrespective of flash mode, so indicate to caller */
    cfstore_client_notify_data_init(&notify_data, CFSTORE_OPCODE_POWER_CONTROL, ret, NULL);
    cfstore_ctx_client_notify(ctx, &notify_data);
    return ret;
}


#ifdef YOTTA_CFG_CFSTORE_UVISOR

/*
 * uvisor secure gateways for ARM_CFSTORE_DRIVER access methods.
 */

UVISOR_EXTERN int32_t __cfstore_uvisor_close(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_close(hkey);
}

static int32_t cfstore_uvisor_close(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_close, hkey);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_create(const char* key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_create(key_name, value_len, kdesc, hkey);
}

static int32_t cfstore_uvisor_create(const char* key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_create, key_name, value_len, kdesc, hkey);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_delete(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_delete(hkey);
}

static int32_t cfstore_uvisor_delete(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_delete, hkey);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_find(const char* key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_find(key_name_query, previous, next);
}

static int32_t cfstore_uvisor_find(const char* key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_find, key_name_query, previous, next);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_flush(int dummy)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	(void) dummy;
	return cfstore_flush();
}

static int32_t cfstore_uvisor_flush(void)
{
	int dummy = 0;

	CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_flush, dummy);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_get_key_name(ARM_CFSTORE_HANDLE hkey, char* key_name, uint8_t *key_name_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_get_key_name(hkey, key_name, key_name_len);
}

static int32_t cfstore_uvisor_get_key_name(ARM_CFSTORE_HANDLE hkey, char* key_name, uint8_t *key_name_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_get_key_name, hkey, key_name, key_name_len);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_get_value_len(hkey, value_len);
}

static int32_t cfstore_uvisor_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_get_value_len, hkey, value_len);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_initialize(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_initialise(callback, client_context);
}

static int32_t cfstore_uvisor_initialise(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_initialize, callback, client_context);
}

/* type to convert between ARM_CFSTORE_FMODE and uint32 for passing flags through secure gw */
typedef union cfstore_fmode_flags_t
{
	ARM_CFSTORE_FMODE flags;
	uint32_t val;
} cfstore_fmode_flags_t;

UVISOR_EXTERN int32_t __cfstore_uvisor_open(const char* key_name, uint32_t flags, ARM_CFSTORE_HANDLE hkey)
{
	cfstore_fmode_flags_t uflags;

	CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	uflags.val = flags;
	return cfstore_open(key_name, uflags.flags, hkey);
}

static int32_t cfstore_uvisor_open(const char* key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey)
{
	cfstore_fmode_flags_t uflags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    uflags.flags = flags;
	return secure_gateway(configuration_store, __cfstore_uvisor_open, key_name, uflags.val, hkey);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_read(hkey, data, len);
}

static int32_t cfstore_uvisor_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_read, hkey, data, len);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_rseek(hkey, offset);
}

static int32_t cfstore_uvisor_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_rseek, hkey, offset);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_uninitialise(int dummy)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	(void) dummy;
	return cfstore_uninitialise();
}

static int32_t cfstore_uvisor_uninitialize(void)
{
	int dummy = 0;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_uninitialise, dummy);
}

UVISOR_EXTERN int32_t __cfstore_uvisor_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return cfstore_write(hkey, data, len);
}

static int32_t cfstore_uvisor_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
	return secure_gateway(configuration_store, __cfstore_uvisor_write, hkey, data, len);
}


ARM_CFSTORE_DRIVER cfstore_driver =
{
        .Close = cfstore_uvisor_close,
        .Create = cfstore_uvisor_create,
        .Delete= cfstore_uvisor_delete,
        .Find = cfstore_uvisor_find,
        .Flush = cfstore_uvisor_flush,
        .GetCapabilities = cfstore_get_capabilities,
        .GetKeyName = cfstore_uvisor_get_key_name,
        .GetStatus = cfstore_get_status,
        .GetValueLen = cfstore_uvisor_get_value_len,
        .GetVersion = cfstore_get_version,
        .Initialize = cfstore_uvisor_initialise,
        .Open = cfstore_uvisor_open,
        .PowerControl = cfstore_power_control,
        .Read = cfstore_uvisor_read,
        .Rseek = cfstore_uvisor_rseek,
        .Uninitialize = cfstore_uvisor_uninitialize,
        .Write = cfstore_uvisor_write,
};

#else

/* non-uvisor interface */
ARM_CFSTORE_DRIVER cfstore_driver =
{
        .Close = cfstore_close,
        .Create = cfstore_create,
        .Delete= cfstore_delete,
        .Find = cfstore_find,
        .Flush = cfstore_flush,
        .GetCapabilities = cfstore_get_capabilities,
        .GetKeyName = cfstore_get_key_name,
        .GetStatus = cfstore_get_status,
        .GetValueLen = cfstore_get_value_len,
        .GetVersion = cfstore_get_version,
        .Initialize = cfstore_initialise,
        .Open = cfstore_open,
        .PowerControl = cfstore_power_control,
        .Read = cfstore_read,
        .Rseek = cfstore_rseek,
        .Uninitialize = cfstore_uninitialise,
        .Write = cfstore_write,
};

#endif /* YOTTA_CFG_CFSTORE_UVISOR */

