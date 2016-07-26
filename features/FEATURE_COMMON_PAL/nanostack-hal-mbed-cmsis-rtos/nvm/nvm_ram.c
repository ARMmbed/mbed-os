/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved.
 */

#ifdef MBED_CONF_NANOSTACK_HAL_NVM_CFSTORE
#  if MBED_CONF_NANOSTACK_HAL_NVM_CFSTORE
#    define IGNORE_SIMULATED_NVM_STORAGE
#  else
#    undef IGNORE_SIMULATED_NVM_STORAGE
#  endif
#endif

/*
 * Define flag IGNORE_SIMULATED_NVM_STORAGE to ignore usage of simulated NVM and use 
 * platform specific NVM instead.
 */
#ifndef IGNORE_SIMULATED_NVM_STORAGE

#include <stdlib.h>
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "nsdynmemLIB.h"
#include "eventOS_event_timer.h"
#include "platform/arm_hal_nvm.h"

#include "ns_trace.h"
#define TRACE_GROUP "rnvm"

/**
* NVM to RAM/heap, targeted for testing.
* Key/values are stored to ns_list. Otherwise functionality should be similar to
* nanostack configuration-store adaptation.
*/

#define NVM_RAM_STANDARD_MALLOC
#ifdef NVM_RAM_STANDARD_MALLOC
#define ALLOC malloc
#define FREE free
#else
#define ALLOC ns_dyn_mem_alloc
#define FREE ns_dyn_mem_free
#endif

/*
 * Data entry stored to NVM
 */
typedef struct {
    char *key;
    uint8_t *data;
    uint32_t data_len;
    ns_list_link_t link;
} nvm_data_entry_t;

/*
 * Client request to NVM
 */
typedef struct {
    nvm_callback *client_cb;            // callback provided by client
    void *client_context;               // context provided by client
    void *client_buffer;             // buffer provided by client
    uint16_t *client_buffer_len;        // buffer length provided by client
    platform_nvm_status client_status;  // status to be returned to client
    ns_list_link_t link;
} nvm_client_req_t;

/*
 * NVM context
 */
typedef struct {
    timeout_t *callback_timer;              // timer handle for informing client
    bool is_initialized;
} nvm_context_t;

static NS_LIST_DEFINE(nvm_entry_list, nvm_data_entry_t, link);
static NS_LIST_DEFINE(nvm_client_req_list, nvm_client_req_t, link);

static nvm_context_t *nvm_context_ptr = NULL;

static void nvm_ram_timer_start(void *ctx);
static void nvm_ram_free_entry(nvm_data_entry_t *entry);


static platform_nvm_status create_client_request(nvm_callback *callback, void *context, void *buf, uint16_t *buf_len, platform_nvm_status status)
{
    nvm_client_req_t *nvm_client_req_ptr;
    nvm_client_req_ptr = ALLOC(sizeof(nvm_client_req_t));
    if (!nvm_client_req_ptr) {
        return PLATFORM_NVM_ERROR;
    }
    nvm_client_req_ptr->client_cb = callback;
    nvm_client_req_ptr->client_context = context;
    nvm_client_req_ptr->client_buffer = buf;
    nvm_client_req_ptr->client_buffer_len = buf_len;
    nvm_client_req_ptr->client_status = status;
    ns_list_add_to_end(&nvm_client_req_list, nvm_client_req_ptr);

    if (nvm_context_ptr->callback_timer == NULL) {
        nvm_ram_timer_start(nvm_client_req_ptr);
    }

    return PLATFORM_NVM_OK;
}

platform_nvm_status platform_nvm_init(nvm_callback *callback, void *context)
{
    if (nvm_context_ptr == NULL) {
        nvm_context_ptr = ALLOC(sizeof(nvm_context_t));
        if (!nvm_context_ptr) {
            return PLATFORM_NVM_ERROR;
        }
        nvm_context_ptr->callback_timer = NULL;
        nvm_context_ptr->is_initialized = true;
        ns_list_init(&nvm_entry_list);
        ns_list_init(&nvm_client_req_list);
    } else {
        if (nvm_context_ptr->is_initialized == true) {
            return PLATFORM_NVM_ERROR;
        }
    }

    return create_client_request(callback, context, NULL, NULL, PLATFORM_NVM_OK);
}

platform_nvm_status platform_nvm_finalize(nvm_callback *callback, void *context)
{
    platform_nvm_status ret;
    if (nvm_context_ptr->is_initialized == false) {
        return PLATFORM_NVM_ERROR;
    }

    ret = create_client_request(callback, context, NULL, NULL, PLATFORM_NVM_OK);
    if (ret == PLATFORM_NVM_OK) {
        nvm_context_ptr->is_initialized = false;
    }

    return ret;
}

platform_nvm_status platform_nvm_key_create(nvm_callback *callback, const char *key_name, uint16_t value_len, uint32_t flags, void *context)
{
    (void)flags;
    tr_debug("platform_nvm_key_create() %s len=%d", key_name, (int)value_len);

    ns_list_foreach(nvm_data_entry_t, current_entry, &nvm_entry_list) {
        if (strcmp(current_entry->key, key_name)==0) {
            // resizing existing key
            ns_list_remove(&nvm_entry_list, current_entry);
            nvm_ram_free_entry(current_entry);
            break;
        }
    }

    nvm_data_entry_t *entry = ALLOC(sizeof(nvm_data_entry_t));
    if (!entry) {
        return PLATFORM_NVM_ERROR;
    }
    memset(entry, 0, sizeof(nvm_data_entry_t));
    size_t key_len = strlen(key_name) + 1;
    entry->key = ALLOC(key_len);
    if (!entry->key) {
        FREE(entry);
        return PLATFORM_NVM_ERROR;
    }
    memcpy(entry->key, key_name, key_len);
    entry->data_len = value_len;
    entry->data = ALLOC(value_len);
    if (!entry->data) {
        FREE(entry->key);
        FREE(entry);
        return PLATFORM_NVM_ERROR;
    }

    ns_list_add_to_end(&nvm_entry_list, entry);

    return create_client_request(callback, context, NULL, NULL, PLATFORM_NVM_OK);
}

platform_nvm_status platform_nvm_key_delete(nvm_callback *callback, const char *key_name, void *context)
{
    platform_nvm_status client_status = PLATFORM_NVM_KEY_NOT_FOUND;
    tr_debug("platform_nvm_key_delete() %s", key_name);

    ns_list_foreach(nvm_data_entry_t, current_entry, &nvm_entry_list) {
        if (strcmp(current_entry->key, key_name)==0) {
            client_status = PLATFORM_NVM_OK;
            ns_list_remove(&nvm_entry_list, current_entry);
            nvm_ram_free_entry(current_entry);
            break;
        }
    }

    return create_client_request(callback, context, NULL, NULL, client_status);
}

platform_nvm_status platform_nvm_write(nvm_callback *callback, const char *key_name, const void *data, uint16_t *data_len, void *context)
{
    platform_nvm_status client_status = PLATFORM_NVM_KEY_NOT_FOUND;
    tr_debug("platform_nvm_write() %s len=%d", key_name, (int)*data_len);

    ns_list_foreach(nvm_data_entry_t, current_entry, &nvm_entry_list) {
        if (strcmp(current_entry->key, key_name)==0) {
            if (current_entry->data_len >= *data_len) {
                memcpy(current_entry->data, data, *data_len);
            } else {
                memcpy(current_entry->data, data, current_entry->data_len);
                *data_len = current_entry->data_len;
            }
            client_status = PLATFORM_NVM_OK;
            break;
        }
    }

    return create_client_request(callback, context, (void*)data, data_len, client_status);
}

platform_nvm_status platform_nvm_read(nvm_callback *callback, const char *key_name, void *buf, uint16_t *buf_len, void *context)
{
    platform_nvm_status client_status = PLATFORM_NVM_KEY_NOT_FOUND;
    tr_debug("platform_nvm_read() %s len=%d", key_name, (int)*buf_len);

    ns_list_foreach(nvm_data_entry_t, current_entry, &nvm_entry_list) {
        if (strcmp(current_entry->key, key_name)==0) {
            if (*buf_len >= current_entry->data_len) {
                memcpy(buf, current_entry->data, current_entry->data_len);
                *buf_len = current_entry->data_len;
            } else {
                memcpy(buf, current_entry->data, *buf_len);
            }
            client_status = PLATFORM_NVM_OK;
            break;
        }
    }

    return create_client_request(callback, context, buf, buf_len, client_status);
}

platform_nvm_status platform_nvm_flush(nvm_callback *callback, void *context)
{
    tr_debug("platform_nvm_flush()");
    return create_client_request(callback, context, NULL, NULL, PLATFORM_NVM_OK);
}

static void nvm_ram_timer_cb(void *args)
{
    nvm_client_req_t *nvm_client_req_ptr = (nvm_client_req_t *)args;
    nvm_client_req_ptr->client_cb(nvm_client_req_ptr->client_status, nvm_client_req_ptr->client_context);
    ns_list_remove(&nvm_client_req_list, nvm_client_req_ptr);
    FREE(nvm_client_req_ptr);

    nvm_context_ptr->callback_timer = NULL;

    if (!ns_list_is_empty(&nvm_client_req_list)) {
        // there are more client requests to process
        nvm_client_req_ptr = ns_list_get_first(&nvm_client_req_list);
        nvm_ram_timer_start(nvm_client_req_ptr);
    }
}

static void nvm_ram_timer_start(void *ctx)
{
    nvm_context_ptr->callback_timer = eventOS_timeout_ms(nvm_ram_timer_cb, 50, ctx);
}

static void nvm_ram_free_entry(nvm_data_entry_t *entry)
{
    FREE(entry->key);
    FREE(entry->data);
    FREE(entry);
}

#endif /* IGNORE_SIMULATED_NVM_STORAGE */
