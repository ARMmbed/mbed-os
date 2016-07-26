/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * NVM adaptation to Configuration Store that is storing Key-Value pairs to SRAM or flash.
 */

/*
 * Application needs to enable MBED_CONF_NANOSTACK_HAL_NVM_CFSTORE in its configuration in
 * order to use configuration-store. Application also needs to define storage area start
 * address in flash (CONFIG_HARDWARE_MTD_START_ADDR) and storage size (CONFIG_HARDWARE_MTD_SIZE).
 */
#if MBED_CONF_NANOSTACK_HAL_NVM_CFSTORE

#include <stdio.h>
#include <string.h>
#include <ns_types.h>
#include "configuration-store/configuration_store.h"
// #define HAVE_DEBUG
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "eventOS_event_timer.h"
#include "platform/arm_hal_nvm.h"

#define TRACE_GROUP "pnvm"

// Timeout for polling response from configuration store
#define NVM_CB_POLLING_TIMEOUT 50

// Check if synchronous mode is enabled
#define IS_SYNC_MODE(cs_ctx) ((cs_ctx)->capabilities.asynchronous_ops == 0)

// NVM internal states
typedef enum cfstore_state_t {
    NVM_STATE_NONE,
    NVM_STATE_INIT_DONE,
    NVM_STATE_CREATING,
    NVM_STATE_CREATE_DONE,
    NVM_STATE_OPENING,
    NVM_STATE_OPEN_DONE,
    NVM_STATE_WRITING,
    NVM_STATE_WRITE_DONE,
    NVM_STATE_CLOSING,
    NVM_STATE_CLOSE_DONE,
    NVM_STATE_FLUSHING,
    NVM_STATE_FLUSH_DONE,
    NVM_STATE_READING,
    NVM_STATE_READ_DONE,
    NVM_STATE_DELETING,
    NVM_STATE_DELETE_DONE,
    NVM_STATE_UNINITIALIZING,
    NVM_STATE_UNINIT_DONE
} nvm_state_e;

// NVM context
typedef struct cfstore_context_s {
    ARM_CFSTORE_CAPABILITIES capabilities;
    //TODO: Fix hkey length once CFSTORE_HANDLE_BUFSIZE becomes visible
    uint8_t hkey[/*CFSTORE_HANDLE_BUFSIZE*/40]; // Handle to the key in process
    ARM_CFSTORE_SIZE data_len;              // Data length cfstore is using
    nvm_state_e state;                      // current nvm_state_e
    timeout_t *callback_timer;              // timer handle for informing client
    nvm_callback *client_cb;                // callback provided by client
    void *client_context;                   // context provided by client
    platform_nvm_status client_status;      // status to be returned to client
    uint8_t *client_buf;                    // buffer provided by client
    uint16_t *client_buf_len;               // client buffer length
} cs_context_t;

ARM_CFSTORE_DRIVER *drv = &cfstore_driver;
static cs_context_t *cs_context_ptr = NULL;

// forward declarations
static bool nvm_write_internal(cs_context_t *cf_context);
static bool nvm_read_internal(cs_context_t *cf_context);
static bool nvm_delete_internal(cs_context_t *cf_context);
static bool nvm_close_internal(cs_context_t *cf_context);
static bool nvm_status_check(cs_context_t *cf_context);
static platform_nvm_status nvm_error_map(int32_t cs_error);
static void nvm_fsm_timer_start(void);
static void nvm_fsm_timer_cb(void *arg);

/**
 * Configuration store callback
 */
static void configuration_store_cb(int32_t status, ARM_CFSTORE_OPCODE cmd_code, void *ctx, ARM_CFSTORE_HANDLE handle)
{
    tr_debug("configuration_store_cb status=%d, cmd_code=%d, ctx=%x, hndl=%x", (int)status, (int)cmd_code, (unsigned int)ctx, (unsigned int)handle);
    cs_context_t *cf_context = (cs_context_t*)ctx;

    switch(cmd_code) {
        case CFSTORE_OPCODE_INITIALIZE:
            tr_debug("CFSTORE_OPCODE_INITIALIZE %d", (int)status);
            cf_context->state = NVM_STATE_INIT_DONE;
            cf_context->client_status = nvm_error_map(status);
            break;
        case CFSTORE_OPCODE_POWER_CONTROL:
            tr_debug("CFSTORE_OPCODE_POWER_CONTROL %d", (int)status);
            // do nothing for power control
            break;
        case CFSTORE_OPCODE_CREATE:
            tr_debug("CFSTORE_OPCODE_CREATE %d", (int)status);
            cf_context->client_status = nvm_error_map(status);
            cf_context->state = NVM_STATE_CREATE_DONE;
            if (status >= ARM_DRIVER_OK) {
                // key created successfully, close the key
                if (nvm_close_internal(cf_context) == false) {
                    // closing failed
                    // Ignore errors in close as closing recreated keys returns error
                    // cf_context->state = NVM_STATE_CLOSE_DONE;
                    // cf_context->client_status = PLATFORM_NVM_ERROR;
                } else {
                    // closing OK, wait for CFSTORE_OPCODE_CLOSE callback
                }
            }
            break;
        case CFSTORE_OPCODE_OPEN:
            tr_debug("CFSTORE_OPCODE_OPEN %d", (int)status);
            if (status < ARM_DRIVER_OK) {
                // opening failed, do not continue any further
                cf_context->client_status = nvm_error_map(status);
                cf_context->state = NVM_STATE_OPEN_DONE;
                break;
            }
            // proceed to client action read/write/delete
            if (cf_context->state == NVM_STATE_WRITING) {
                if (nvm_write_internal(cf_context) == false) {
                    /* reading failed set client_status */
                    cf_context->client_status = PLATFORM_NVM_ERROR;
                } else {
                    // writing OK, wait for CFSTORE_OPCODE_WRITE callback
                }
            } else if (cf_context->state == NVM_STATE_READING) {
                if (nvm_read_internal(cf_context) == false) {
                    /* reading failed set client_status */
                    cf_context->client_status = PLATFORM_NVM_ERROR;
                } else {
                    // reading in progress, wait for CFSTORE_OPCODE_READ callback
                }
            } else if (cf_context->state == NVM_STATE_DELETING) {
                if (nvm_delete_internal(cf_context) == false) {
                    /* reading failed set client_status */
                    cf_context->client_status = PLATFORM_NVM_ERROR;
                } else {
                    // deleting in progress, wait for CFSTORE_OPCODE_DELETE callback
                }
            }

            if (cf_context->client_status == PLATFORM_NVM_ERROR) {
                // read/write/delete operation failed, close the handle
                if (nvm_close_internal(cf_context) == false) {
                    cf_context->state = NVM_STATE_CLOSE_DONE;
                    cf_context->client_status = PLATFORM_NVM_ERROR;
                }
            }
            break;
        case CFSTORE_OPCODE_WRITE:
            tr_debug("CFSTORE_OPCODE_WRITE %d", (int)status);
            cf_context->state = NVM_STATE_WRITE_DONE;
            *cf_context->client_buf_len = cf_context->data_len;
            if (nvm_close_internal(cf_context) == false) {
                /* writing failed set status and start callback timer */
                cf_context->state = NVM_STATE_CLOSE_DONE;
                cf_context->client_status = PLATFORM_NVM_ERROR;
            } else {
                // closing OK, wait for CFSTORE_OPCODE_CLOSE callback
            }
            break;
        case CFSTORE_OPCODE_READ:
            tr_debug("CFSTORE_OPCODE_READ %d", (int)status);
            cf_context->state = NVM_STATE_READ_DONE;
            if (nvm_close_internal(cf_context) == false) {
                cf_context->state = NVM_STATE_CLOSE_DONE;
                cf_context->client_status = PLATFORM_NVM_ERROR;
            } else {
                // closing OK, wait for CFSTORE_OPCODE_CLOSE callback
                *cf_context->client_buf_len = (uint16_t)status; // save the bytes read
            }
            break;
        case CFSTORE_OPCODE_DELETE:
            tr_debug("CFSTORE_OPCODE_DELETE %d", (int)status);
            if (nvm_close_internal(cf_context) == false) {
                /* closing failed set client_status */
                cf_context->state = NVM_STATE_CLOSE_DONE;
                cf_context->client_status = PLATFORM_NVM_ERROR;
            } else {
                // closing OK, wait for CFSTORE_OPCODE_CLOSE callback
            }
            break;
        case CFSTORE_OPCODE_CLOSE:
            tr_debug("CFSTORE_OPCODE_CLOSE %d", (int)status);
            cf_context->state = NVM_STATE_CLOSE_DONE;
            // client_status is already set by read/write/delete operation, do not override it
            break;
        case CFSTORE_OPCODE_UNINITIALIZE:
            tr_debug("CFSTORE_OPCODE_UNINITIALIZE %d", (int)status);
            cf_context->state = NVM_STATE_UNINIT_DONE;
            cf_context->client_status = nvm_error_map(status);
            break;
        case CFSTORE_OPCODE_FLUSH:
            tr_debug("CFSTORE_OPCODE_FLUSH %d", (int)status);
            cf_context->state = NVM_STATE_FLUSH_DONE;
            cf_context->client_status = nvm_error_map(status);
            break;

        default:
            tr_debug("unhandled cmd_code %d", cmd_code);
            break;
    }

    return;
}

static int nvm_fsm_update(cs_context_t *cs_context)
{
    int ret_val = 0;

    tr_debug("nvm_fsm_update() state=%d", (int)cs_context->state);
    switch (cs_context->state)
    {
        case NVM_STATE_UNINIT_DONE:
            cs_context->client_cb(cs_context->client_status, cs_context->client_context);
            cs_context->state = NVM_STATE_NONE;
            if (cs_context->client_status == PLATFORM_NVM_OK) {
                ns_dyn_mem_free(cs_context_ptr);
                cs_context_ptr = NULL;
            }
            ret_val = 1;
            break;
        case NVM_STATE_INIT_DONE:
        case NVM_STATE_CREATE_DONE:
        case NVM_STATE_OPEN_DONE:
        case NVM_STATE_WRITE_DONE:
        case NVM_STATE_READ_DONE:
        case NVM_STATE_DELETE_DONE:
        case NVM_STATE_CLOSE_DONE:
        case NVM_STATE_FLUSH_DONE:
            cs_context->state = NVM_STATE_NONE;
            cs_context->client_cb(cs_context->client_status, cs_context->client_context);
            ret_val = 1;
            break;

        default:
            tr_error("unknown state %d", cs_context->state);
            break;
    }

    return ret_val;
}

/**
 * Initialize NVM
 */
platform_nvm_status platform_nvm_init(nvm_callback *callback, void *context)
{
    int32_t ret;

    tr_debug("platform_nvm_init()");

    if (callback == NULL || cs_context_ptr) {
        return PLATFORM_NVM_ERROR;
    }

    if (cs_context_ptr == NULL) {
        cs_context_ptr = ns_dyn_mem_alloc(sizeof(cs_context_t));
    }

    if (cs_context_ptr == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    memset(cs_context_ptr, 0, sizeof(cs_context_t));
    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;

    cs_context_ptr->capabilities = drv->GetCapabilities();

    tr_debug("mode: %s", IS_SYNC_MODE(cs_context_ptr) ? "sync": "async" );

    ret = drv->Initialize(configuration_store_cb, cs_context_ptr);
    if (ret < ARM_DRIVER_OK) {
        tr_error("initialisation failed %d", (int)ret);
        ns_dyn_mem_free(cs_context_ptr);
        cs_context_ptr = NULL;
        return PLATFORM_NVM_ERROR;
    }

    drv->PowerControl(ARM_POWER_FULL);

    // start timer to report initialization status back to client
    nvm_fsm_timer_start();
    return PLATFORM_NVM_OK;
}

/*
 * Deinitialize NVM.
 */
platform_nvm_status platform_nvm_finalize(nvm_callback *callback, void *context)
{
    int32_t ret;

    tr_debug("platform_nvm_deinit()");

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }

    if (callback == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->state = NVM_STATE_UNINITIALIZING;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;
    drv->PowerControl(ARM_POWER_OFF);
    ret = drv->Uninitialize();

    if (ret < ARM_DRIVER_OK) {
        tr_error("deinit failed %d", (int)ret);
        cs_context_ptr->state = NVM_STATE_UNINIT_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    nvm_fsm_timer_start();
    return PLATFORM_NVM_OK;
}

/*
 * Create key to NVM
 */
platform_nvm_status platform_nvm_key_create(nvm_callback *callback, const char *key_name, uint16_t value_len, uint32_t flags, void *context)
{
    int32_t ret;
    ARM_CFSTORE_KEYDESC keydesc;
    (void)flags;

    tr_debug("platform_nvm_key_create() %s len=%d", key_name, value_len);

    if (callback == NULL || key_name == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }

    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->state = NVM_STATE_CREATING;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;

    memset(&keydesc, 0, sizeof(ARM_CFSTORE_KEYDESC));
    keydesc.drl = ARM_RETENTION_NVM;

    ret = drv->Create(key_name, value_len, &keydesc, cs_context_ptr->hkey);
    if(ret < ARM_DRIVER_OK) {
        if (ret == ARM_CFSTORE_DRIVER_ERROR_PREEXISTING_KEY) {
            tr_debug("adjust value len to %d", value_len);
            ret = drv->Create(key_name, value_len, NULL, cs_context_ptr->hkey);
        }
    }

    if(ret < ARM_DRIVER_OK) {
        tr_error("Key creation failed %d", (int)ret);
        cs_context_ptr->state = NVM_STATE_CREATE_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    nvm_fsm_timer_start();

    return PLATFORM_NVM_OK;
}

/**
 * Delete key from NVM
 */
platform_nvm_status platform_nvm_key_delete(nvm_callback *callback, const char *key_name, void *context)
{
    int32_t ret;
    ARM_CFSTORE_FMODE flags;

    tr_debug("platform_nvm_key_delete() %s", key_name);

    if (callback == NULL || key_name == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }

    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;
    cs_context_ptr->state = NVM_STATE_DELETING;

    memset(&flags, 0, sizeof(ARM_CFSTORE_FMODE));
    flags.read = 1;
    flags.write = 1;
    ret = drv->Open(key_name, flags, cs_context_ptr->hkey);

    if(ret < ARM_DRIVER_OK) {
        tr_error("Key delete, open failed %d", (int)ret);
        cs_context_ptr->state = NVM_STATE_DELETE_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    // start callback timer in both asynch and synch mode
    nvm_fsm_timer_start();

    return PLATFORM_NVM_OK;
}

/**
 * Reading from NVM
 */
platform_nvm_status platform_nvm_read(nvm_callback *callback, const char *key_name, void *buf, uint16_t *buf_len, void *context)
{
    int32_t ret;
    ARM_CFSTORE_FMODE flags;

    tr_debug("platform_nvm_read()");

    if (callback == NULL || key_name == NULL || buf == NULL || buf_len == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }
    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->client_buf = buf;
    cs_context_ptr->client_buf_len = buf_len;
    cs_context_ptr->data_len = *buf_len;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;
    cs_context_ptr->state = NVM_STATE_READING;

    // Open handle for reading
    memset(&flags, 0, sizeof(ARM_CFSTORE_FMODE));
    flags.read = 1;
    ret = drv->Open(key_name, flags, cs_context_ptr->hkey);

    if(ret < ARM_DRIVER_OK) {
        tr_error("Read failed to open handle %d", (int)ret);
        cs_context_ptr->state = NVM_STATE_READ_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    // start callback timer in both async and synch mode
    nvm_fsm_timer_start();

    return PLATFORM_NVM_OK;
}

/**
 * Write to NVM.
 */
platform_nvm_status platform_nvm_write(nvm_callback *callback, const char *key_name, const void *data, uint16_t *data_len, void *context)
{
    int32_t ret;
    ARM_CFSTORE_FMODE flags;
    tr_debug("platform_nvm_write()");

    if (callback == NULL || key_name == NULL || data == NULL || data_len == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }
    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->client_buf = (void*)data;
    cs_context_ptr->client_buf_len = data_len;
    cs_context_ptr->data_len = *data_len;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;
    cs_context_ptr->state = NVM_STATE_WRITING;

    // Open handle for writing, execution continues in callback
    memset(&flags, 0, sizeof(ARM_CFSTORE_FMODE));
    flags.write = 1;
    ret = drv->Open(key_name, flags, cs_context_ptr->hkey);

    if(ret < ARM_DRIVER_OK) {
        tr_error("Write failed %d", (int)ret);
        cs_context_ptr->state = NVM_STATE_WRITE_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    // start callback timer in both asynch and synch mode
    nvm_fsm_timer_start();

    return PLATFORM_NVM_OK;
}

/**
 * Flush the NVM
 */
platform_nvm_status platform_nvm_flush(nvm_callback *callback, void *context)
{
    tr_debug("platform_nvm_flush()");

    int32_t ret;

    if (callback == NULL) {
        return PLATFORM_NVM_ERROR;
    }

    if (!nvm_status_check(cs_context_ptr)) {
        return PLATFORM_NVM_ERROR;
    }

    cs_context_ptr->client_cb = callback;
    cs_context_ptr->client_context = context;
    cs_context_ptr->client_status = PLATFORM_NVM_OK;
    cs_context_ptr->state = NVM_STATE_FLUSHING;

    ret = drv->Flush();

    if(ret < ARM_DRIVER_OK) {
        cs_context_ptr->state = NVM_STATE_FLUSH_DONE;
        cs_context_ptr->client_status = nvm_error_map(ret);
    }

    // start callback timer in both asynch and synch mode
    nvm_fsm_timer_start();

    return PLATFORM_NVM_OK;
}

static bool nvm_write_internal(cs_context_t *cf_context)
{
    int32_t ret;
    cf_context->state = NVM_STATE_WRITING;
    ret = drv->Write(cf_context->hkey, (const char*)cf_context->client_buf, &cf_context->data_len);

    if(ret >= ARM_DRIVER_OK) {
        return true;
    } else {
        tr_error("Write failed %d", (int)ret);
        return false;
    }
}

static bool nvm_read_internal(cs_context_t *cf_context)
{
    int32_t ret;
    cf_context->state = NVM_STATE_READING;
    ret = drv->Read(cf_context->hkey, (void*)cf_context->client_buf, &cf_context->data_len);

    if(ret >= ARM_DRIVER_OK) {
        return true;
    } else {
        tr_error("Read failed %d", (int)ret);
        return false;
    }
}

static bool nvm_delete_internal(cs_context_t *cf_context)
{
    int32_t ret;
    cf_context->state = NVM_STATE_DELETING;
    ret = drv->Delete(cf_context->hkey);

    if(ret >= ARM_DRIVER_OK) {
        return true;
    } else {
        tr_error("Delete failed %d", (int)ret);
        return false;
    }
}

static bool nvm_close_internal(cs_context_t *cf_context)
{
    int32_t ret;
    cf_context->state = NVM_STATE_CLOSING;
    ret = drv->Close(cf_context->hkey);

    if(ret >= ARM_DRIVER_OK) {
        return true;
    } else {
        tr_error("Close failed %d", (int)ret);
        return false;
    }
}

/*
 * Check NVM state before executing client action
 */
static bool nvm_status_check(cs_context_t *cf_context)
{
    if (!cs_context_ptr) {
        // not initialized
        tr_error("NVM not initialized");
        return false;
    }

    if (cf_context->state != NVM_STATE_NONE) {
        tr_error("NVM busy, operation in progress %d", cf_context->state);
        return false;
    }

    return true;
}

static platform_nvm_status nvm_error_map(int32_t cs_error)
{
    platform_nvm_status client_error;

    if (cs_error >= ARM_DRIVER_OK) {
        return PLATFORM_NVM_OK;
    }

    switch(cs_error) {
        case  ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND:
            client_error = PLATFORM_NVM_KEY_NOT_FOUND;
            break;
        default:
            client_error = PLATFORM_NVM_ERROR;
            break;
    }

    return client_error;
}

static void nvm_fsm_timer_cb(void *args)
{
    (void) args;
    switch(nvm_fsm_update(cs_context_ptr)) {
        case 0:
            // Nothing processed, restart timer
            tr_debug("nvm_fsm_timer_cb not handled event in () %d", (int)cs_context_ptr->state);
            nvm_fsm_timer_start();
            break;
        default:
            break;
    }
}

/*
 * Start timer for polling callback from
 */
static void nvm_fsm_timer_start(void)
{
    cs_context_ptr->callback_timer = eventOS_timeout_ms(nvm_fsm_timer_cb, NVM_CB_POLLING_TIMEOUT, NULL);
}

#endif /* MBED_CONF_NANOSTACK_HAL_NVM_CFSTORE */
