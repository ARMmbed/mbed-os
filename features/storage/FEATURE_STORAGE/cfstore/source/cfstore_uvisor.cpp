/** @file cfstore_uvisor.cpp
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

/* IOTSFW_2185:
 * - secure static strings in secure storage e.g. by placing in uvisor_ctx
 *   e.g. cfstore_flash_opcode_str, cfstore_flash_state_str
 *   or remove.
 * - secure global data in uvisor ctx e.g.:
 *   - cfstore_caps_g
 *   - cfstore_ctx_g
 *   - cfstore_uvisor_namespace_root_g
 *   - __uvisor_box_namespace
 *   - cfstore_acl_list_g
 *   - configuration_store_cfg
 *   - cfstore_malloc_size_g
 *   - cfstore_sram_head
 *   - cfstore_sram_tail
 *   - cfstore_flash_journal_error_code_map
 *   - cfstore_flash_fsm
 *   - cfstore_os_ctx_g
 * - store hkey buffers in uvisor allocated area so they're secure.
 * - convert to use uviosr sync rpc gateway
 */

#include <inttypes.h>

#ifdef YOTTA_CFG_CFSTORE_UVISOR

#include "uvisor-lib/uvisor-lib.h"

/*
 * Configure the secure box compartment
 */
static const char const cfstore_uvisor_namespace_root_g[] = "com.arm.mbed.";
/* See IOTSFW-2526: uvisor-acl support.
 * UVISOR_BOX_NAMESPACE("com.arm.mbed.configuration-store");
 * static const char *const __uvisor_box_namespace = box_namespace
 */
static const char *const __uvisor_box_namespace = "com.arm.mbed.configuration-store";

/* although the descriptor is empty, the main box descriptor is inherited and added to whats here. */
static const UvisorBoxAclItem cfstore_acl_list_g[] = {
    /* See IOTSFW-2526: uvisor-acl support.
     * this needs completing with correct data for the secure flash partition above the binary
     *
     * 0xabaadfood = start of secure flash in address map (flash journal partition
     * 0xbeefbeef = size in bytes of secure flash partition
     * {(void *) 0xabaadfood, 0xbeefbeef, UVISOR_TACLDEF_PERIPH},
     *
     * put reference to k64 subfamily reference manual and cmsis k64f target header as to where this comes from */
    {FTFE,    sizeof(*FTFE),    UVISOR_TACLDEF_PERIPH},
};

/* UVISOR_BOX_CONFIG_CTX(configuration_store, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
 *
 * It would be better to use the following macro:
 *  UVISOR_BOX_CONFIG(configuration_store, cfstore_acl_list_g, UVISOR_BOX_STACK_SIZE, cfstore_ctx_t);
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
 *      from d:/datastore/public/jobs/yr2016/2247/sdh_dev_10/configuration-store/source/configuration_store.c:27:
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
 *      extern const __attribute__((section(".keep.uvisor.cfgtbl_ptr"), aligned(4))) void * const configuration_store_cfg_ptr = &configuration_store_cfg;
 * The extern at the beginning of the line creates a warning when in a c file, and so needs to be removed/fixed.
 * There are also many other warnings from the macro expansion which need to be investigated further.
 *
 * See IOTSFW-2526: uvisor-acl support.
 * possible investigation: move configuration_store.c -> configuration_store.cpp
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


/*
 * security/permissions helper functions
 */

/**
 * @brief   check that a client (cfstore-uvisor client box) is the "owner" of the
 *          KV. Owner means the client that can create or created the KV. This is
 *          determined by the clients namespace and whether the KV path name falls
 *          within that name space
 * @param   key_name
 *          the name of the KV being created.
 *          the validation that the key_name is composed of permissible chars is
 *          carried out before this function is called.
 * @note
 * Conceptually, cfstore supports the following KV path namespaces:
 *  - com.arm.mbed.
 *  - guids of the form xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx where x is a hex digit.
 *
 *  In the cfstore implementation, explicit checking of the structure of the
 *  namespace string is not required. Cfstore only need enforce that:
 *      the root of the KV pathname == cfstore client uvisor namespace.
 */
static int32_t cfstore_uvisor_is_client_kv_owner(char *key_name, int32_t *cfstore_uvisor_box_id)
{
    int32_t calling_box_id;
    int32_t ret;
    /* We store the calling_box_namespace on our stack, lest somebody else modify it. */
    char calling_box_namespace[UVISOR_MAX_BOX_NAMESPACE_LENGTH];

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(calling_box_namespace, 0, sizeof(calling_box_namespace));
    /* Get the ID of the box that called this box through the most recent secure gateway. */
    calling_box_id = uvisor_box_id_caller();
    if (calling_box_id < 0) {
        CFSTORE_ERRLOG("%s: Error: uvisor uvisor_box_id_caller() returned invalid id (calling_box_id=%d\n", __func__, (int) calling_box_id);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_BOX_ID;
    }
    if (cfstore_uvisor_box_id) {
        *cfstore_uvisor_box_id = calling_box_id;
    }
    if (calling_box_id == 0) {
        /* the cfstore uvisor client is the main box.
         * main box is not allowed to create a key as a client is only permitted to create KVs in their namespace. */
        CFSTORE_ERRLOG("%s: Error: uvisor box id identifies cfstore client cannot create KVs (calling_box_id=%d\n", __func__, (int) calling_box_id);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_BOX_ID;
    }
    /* Copy the name of the calling box to our stack. */
    ret = uvisor_box_namespace(calling_box_id, calling_box_namespace, sizeof(calling_box_namespace));
    if (ret < 0) {
        /* error */
        CFSTORE_ERRLOG("%s: Error: unable to recover uvisor box namespace\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_NAMESPACE;
    }
    /* check the cfstore client uvisor box namespace is non-trivial */
    if (strlen(calling_box_namespace) == 0) {
        CFSTORE_ERRLOG("%s: Error: uvisor box namespace is zero length\n", __func__);
        return ARM_CFSTORE_DRIVER_ERROR_UVISOR_NAMESPACE;
    }
    /* check that the key name is within the root domain namespace */
    if (strncmp(calling_box_namespace, key_name, sizeof(calling_box_namespace)) != 0) {
        /* The key_name does not fall within the cfstore-uvisor client namespace and therefore the create is not allowed */
        CFSTORE_ERRLOG("%s: Error: key name (%s) is not permitted to be created within client uvisor box namespace (%s) of cfstore client\n", __func__, key_name, calling_box_namespace);
        return ARM_CFSTORE_DRIVER_ERROR_NO_PERMISSIONS;
    }
    /* We've passed all our checks, so we allow the calling box. */
    return ARM_DRIVER_OK;
}


/* @brief   helper function to determine whether this cfstore-uvisor client box can read a given KV */
static bool cfstore_is_kv_client_readable(cfstore_area_hkvt_t *hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;


    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if (ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if (ret == ARM_DRIVER_OK) {
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_read;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_read;
    }
    return bret;
}

/* @brief   helper function to determine whether this client can write a given KV */
static bool cfstore_is_kv_client_writable(cfstore_area_hkvt_t *hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if (ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if (ret == ARM_DRIVER_OK) {
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_write;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_write;
    }
    return bret;
}

/* @brief   helper function to determine whether this client can execute a given KV */
static bool cfstore_is_kv_client_executable(cfstore_area_hkvt_t *hkvt)
{
    bool bret = false;
    int32_t ret = ARM_DRIVER_ERROR;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t key_name_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    cfstore_area_header_t *hdr = (cfstore_area_header_t*) hkvt->head;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    memset(key_name, 0, key_name_len);
    ret = cfstore_get_key_name_ex(hkvt, key_name, &key_name_len);
    if (ret < ARM_DRIVER_OK) {
        CFSTORE_ERRLOG("%s:Error: cfstore_get_key_name_ex() returned error.\n", __func__);
        return bret;
    }
    ret = cfstore_is_client_kv_owner(key_name, NULL);
    if (ret == ARM_DRIVER_OK) {
        /* cfstore-usvisor client box is the "owner" of the key */
        bret = hdr->perm_owner_execute;
    } else {
        /* cfstore-usvisor client box is not the "owner" of the key i.e. is the "other" */
        bret = hdr->perm_other_execute;
    }
    return bret;
}


/*
 * uvisor secure gateways for ARM_CFSTORE_DRIVER access methods.
 */

/** @brief      extract opaque handle from caller hkey and
 *              for assignment to cfstore box hkey private pointer
 *
 *  IOTSFW_2185: check these macros work correctly.
 */
#define CFSTORE_UVISOR_HKEY_PRV_GET(_hkey_caller)                   ((void*) *(void *) (_hkey_caller))
#define CFSTORE_UVISOR_HKEY_PRV_SET(_hkey_caller, _hkey_prv)        (*(void *) (_hkey_caller) = (void) (_hkey_prv))


UVISOR_EXTERN int32_t cfstore_uvisor_gw_close(ARM_CFSTORE_HANDLE hkey)
{
    void* hkey_prv = NULL;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    hkey_prv = CFSTORE_UVISOR_HKEY_PRV_GET(hkey);
    return cfstore_close(hkey_prv);
}

static int32_t cfstore_uvisor_close(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_close, hkey);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_create(const char *key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    void* hkey_prv = NULL;
    int32_t ret = ARM_DRIVER_ERROR;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    /* IOTSFW_2185: uvisor integration prototype
     * Allocate private, secure storage buffer for config store.
     * This memory is not deleted in cfstore_uvisor_gw_delete() as
     * but internally when the last KV handle is closed, and
     * the file is deleted
     * maybe better to allocate this private buffer inside cfstore_create()?
     * need to store hkey so its available for the callback mechanism ?
     *
     * */
    hkey_prv = CFSTORE_MALLOC(CFSTORE_HANDLE_BUFSIZE);
    if (hkey_prv == NULL) {
        return ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY;
    }
    memset(hkey_prv, 0, CFSTORE_HANDLE_BUFSIZE);

    ret = cfstore_create(key_name, value_len, kdesc, hkey_prv);
    if (ret >= ARM_DRIVER_OK) {
        /* Store pointer to secure storage buffer in caller supplied hkey bufer
         * This is callers opaque handle */
        CFSTORE_UVISOR_HKEY_PRV_SET(hkey, hkey_prv);
    }
    return ret;
}

static int32_t cfstore_uvisor_create(const char *key_name, ARM_CFSTORE_SIZE value_len, const ARM_CFSTORE_KEYDESC* kdesc, ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_create, key_name, value_len, kdesc, hkey);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_delete(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_delete(CFSTORE_UVISOR_HKEY_PRV_GET(hkey));
}

static int32_t cfstore_uvisor_delete(ARM_CFSTORE_HANDLE hkey)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_delete, hkey);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_find(const char *key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    void* hkey_prv_previous = NULL;
    void* hkey_prv_next = NULL;
    int32_t ret = ARM_DRIVER_ERROR;
    bool hkey_prv_previous_alloced = false;
    bool hkey_prv_next_alloced = false;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    /* IOTSFW_2185: uvisor integration prototype.
     * See cfstore_uvisor_gw_create() for more details. */
    hkey_prv_previous = CFSTORE_UVISOR_HKEY_PRV_GET(previous);
    if (hkey_prv_previous == NULL) {
        hkey_prv_previous = CFSTORE_MALLOC(CFSTORE_HANDLE_BUFSIZE);
        if (hkey_prv_previous == NULL) {
            return ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY;
        }
        memset(hkey_prv_previous, 0, CFSTORE_HANDLE_BUFSIZE);
        hkey_prv_previous_alloced = true;
    }
    /* further validity checks on hkey_prv_previous will be made in cfstore_find() */
    hkey_prv_next = CFSTORE_UVISOR_HKEY_PRV_GET(next);
    if (hkey_prv_next == NULL) {
        hkey_prv_next = CFSTORE_MALLOC(CFSTORE_HANDLE_BUFSIZE);
        if (hkey_prv_next == NULL) {
            CFSTORE_FREE(hkey_prv_previous);
            return ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY;
        }
        memset(hkey_prv_next, 0, CFSTORE_HANDLE_BUFSIZE);
        hkey_prv_next_alloced = true;
    }
    ret = cfstore_find(key_name_query, hkey_prv_previous, hkey_prv_next);
    if (ret >= ARM_DRIVER_OK) {
        /* Store pointer to secure storage buffer in caller supplied hkey buffer
         * This is callers opaque handle */
        if (hkey_prv_previous_alloced == true) {
            CFSTORE_UVISOR_HKEY_PRV_SET(previous, hkey_prv_previous);
        }
        if (hkey_prv_next_alloced == true) {
            CFSTORE_UVISOR_HKEY_PRV_SET(next, hkey_prv_next);
        }
    } else {
        /* Deallocate any newly allocated secure hkey buffers */
        if (hkey_prv_previous_alloced == true) {
            CFSTORE_FREE(hkey_prv_previous);
        }
        if (hkey_prv_next_alloced == true) {
            CFSTORE_FREE(hkey_prv_next);
        }
    }
    /* IOTSFW_2185: what about case on walking the list when not found? */
    return ret;
}

static int32_t cfstore_uvisor_find(const char *key_name_query, const ARM_CFSTORE_HANDLE previous, ARM_CFSTORE_HANDLE next)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_find, key_name_query, previous, next);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_flush(int dummy)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) dummy;
    return cfstore_flush();
}

static int32_t cfstore_uvisor_flush(void)
{
    int dummy = 0;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_flush, dummy);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_get_key_name(ARM_CFSTORE_HANDLE hkey, char *key_name, uint8_t *key_name_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_get_key_name(CFSTORE_UVISOR_HKEY_PRV_GET(hkey), key_name, key_name_len);
}

static int32_t cfstore_uvisor_get_key_name(ARM_CFSTORE_HANDLE hkey, char *key_name, uint8_t *key_name_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_get_key_name, hkey, key_name, key_name_len);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_get_value_len(CFSTORE_UVISOR_HKEY_PRV_GET(hkey), value_len);
}

static int32_t cfstore_uvisor_get_value_len(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_SIZE *value_len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_get_value_len, hkey, value_len);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_initialize(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_ctx_t *ctx = cfstore_ctx_get();

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    /* this is something that needs synchronisation */

    /* underlying async behaviour */
    ret = cfstore_initialise(callback, client_context);
    return ret;
}

static int32_t cfstore_uvisor_initialise(ARM_CFSTORE_CALLBACK callback, void* client_context)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_initialize, callback, client_context);
}

/* type to convert between ARM_CFSTORE_FMODE and uint32 for passing flags through secure gw */
typedef union cfstore_fmode_flags_t {
    ARM_CFSTORE_FMODE flags;
    uint32_t val;
} cfstore_fmode_flags_t;

UVISOR_EXTERN int32_t cfstore_uvisor_gw_open(const char *key_name, uint32_t flags, ARM_CFSTORE_HANDLE hkey)
{
    void* hkey_prv = NULL;
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_fmode_flags_t uflags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    /* IOTSFW_2185: uvisor integration prototype.
     * See cfstore_uvisor_gw_create() for more details. */
    hkey_prv = CFSTORE_MALLOC(CFSTORE_HANDLE_BUFSIZE);
    if (hkey_prv == NULL) {
        return ARM_CFSTORE_DRIVER_ERROR_OUT_OF_MEMORY;
    }
    memset(hkey_prv, 0, CFSTORE_HANDLE_BUFSIZE);

    uflags.flags = flags;
    ret = cfstore_open(key_name, uflags.flags, hkey_prv);
    if (ret >= ARM_DRIVER_OK) {
        /* Store pointer to secure storage buffer in caller supplied hkey buffer
         * This is callers opaque handle */
        CFSTORE_UVISOR_HKEY_PRV_SET(hkey, hkey_prv);
    }
    return ret;
}

static int32_t cfstore_uvisor_open(const char *key_name, ARM_CFSTORE_FMODE flags, ARM_CFSTORE_HANDLE hkey)
{
    cfstore_fmode_flags_t uflags;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    uflags.flags = flags;
    return secure_gateway(configuration_store, cfstore_uvisor_gw_open, key_name, uflags.val, hkey);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_read(CFSTORE_UVISOR_HKEY_PRV_GET(hkey), data, len);
}

static int32_t cfstore_uvisor_read(ARM_CFSTORE_HANDLE hkey, void* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_read, hkey, data, len);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_rseek(CFSTORE_UVISOR_HKEY_PRV_GET(hkey), offset);
}

static int32_t cfstore_uvisor_rseek(ARM_CFSTORE_HANDLE hkey, ARM_CFSTORE_OFFSET offset)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_rseek, hkey, offset);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_uninitialise(int dummy)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    (void) dummy;
    return cfstore_uninitialise();
}

static int32_t cfstore_uvisor_uninitialize(void)
{
    int dummy = 0;

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_uninitialise, dummy);
}

UVISOR_EXTERN int32_t cfstore_uvisor_gw_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return cfstore_write(CFSTORE_UVISOR_HKEY_PRV_GET(hkey), data, len);
}

static int32_t cfstore_uvisor_write(ARM_CFSTORE_HANDLE hkey, const char* data, ARM_CFSTORE_SIZE* len)
{
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    return secure_gateway(configuration_store, cfstore_uvisor_gw_write, hkey, data, len);
}


ARM_CFSTORE_DRIVER cfstore_os_mbed_uvisor_driver = {
    .Close = cfstore_uvisor_close,
    .Create = cfstore_uvisor_create,
    .Delete= cfstore_uvisor_delete,
    .Find = cfstore_uvisor_find,
    .Flush = cfstore_uvisor_flush,                  // IOTSFW_2185: needs wrapping to make sync of c++ wrapper
    .GetCapabilities = cfstore_get_capabilities,
    .GetKeyName = cfstore_uvisor_get_key_name,
    .GetStatus = cfstore_get_status,
    .GetValueLen = cfstore_uvisor_get_value_len,
    .GetVersion = cfstore_get_version,
    .Initialize = cfstore_uvisor_initialise,        // IOTSFW_2185: needs wrapping to make sync of c++ wrapper
    .Open = cfstore_uvisor_open,
    .PowerControl = cfstore_power_control,
    .Read = cfstore_uvisor_read,
    .Rseek = cfstore_uvisor_rseek,
    .Uninitialize = cfstore_uvisor_uninitialize,
    .Write = cfstore_uvisor_write,
};

#endif /* YOTTA_CFG_CFSTORE_UVISOR */


/** @brief  main configuration store box thread
 *
 * In the case that uvisor is not enabled, this thread will be called from the
 * C++ wrapper. Otherwise, this will be the cfstore main box thread created
 * by uvisor
 *
 * */
int32_t cfstore_box_thread(void)
{
#ifdef YOTTA_CFG_CFSTORE_UVISOR_ENABLE

    /* The list of functions we are interested in handling RPC requests for */
    static const TFN_Ptr my_fn_array[] = {
        (TFN_Ptr) cfstore_uvisor_gw_close,
        (TFN_Ptr) cfstore_uvisor_gw_create,
        (TFN_Ptr) cfstore_uvisor_gw_delete,
        (TFN_Ptr) cfstore_uvisor_gw_find,
        (TFN_Ptr) cfstore_uvisor_gw_flush,
        (TFN_Ptr) cfstore_get_capabilities,
        (TFN_Ptr) cfstore_uvisor_gw_get_key_name,
        (TFN_Ptr) cfstore_get_status,
        (TFN_Ptr) cfstore_uvisor_gw_get_value_len,
        (TFN_Ptr) cfstore_get_version,
        (TFN_Ptr) cfstore_uvisor_gw_initialise,
        (TFN_Ptr) cfstore_uvisor_gw_open,
        (TFN_Ptr) cfstore_power_control,
        (TFN_Ptr) cfstore_uvisor_gw_read,
        (TFN_Ptr) cfstore_uvisor_gw_rseek,
        (TFN_Ptr) cfstore_uvisor_gw_uninitialize,
        (TFN_Ptr) cfstore_uvisor_gw_write
    };

    while (1) {
        int status;

        /* NOTE: This serializes all access to the number store! */
        status = rpc_fncall_waitfor(my_fn_array, 17, UVISOR_WAIT_FOREVER);

        if (status) {
            printf("Failure is not an option.\r\n");
            uvisor_error(USER_NOT_ALLOWED);
        }
    }

#endif
    return 0;
}

