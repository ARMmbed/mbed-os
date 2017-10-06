/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */

#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "ns_list.h"
#include "platform/arm_hal_nvm.h"
#include "ns_nvm_helper.h"

#define TRACE_GROUP "nnvm"

/* NVM operations */
#define NS_NVM_NONE         0x00
#define NS_NVM_INIT         0x01
#define NS_NVM_KEY_CREATE   0x02
#define NS_NVM_KEY_READ     0x03
#define NS_NVM_KEY_WRITE    0x04
#define NS_NVM_FLUSH        0x05
#define NS_NVM_KEY_DELETE   0x06

typedef struct {
    ns_nvm_callback *callback;
    const char *client_key_name;
    void *client_context;
    int operation;
    uint8_t *buffer;
    uint16_t *buffer_len;
    void *original_request;
    ns_list_link_t link;
} ns_nvm_request_t;

static bool ns_nvm_initialized = false;
static bool ns_nvm_operation_in_progress = false;

static ns_nvm_request_t *ns_nvm_create_request(ns_nvm_callback *callback, void *context, const char *key_name, uint8_t *buf, uint16_t *buf_len, uint8_t operation);
static int ns_nvm_operation_start(ns_nvm_request_t *request);
static int ns_nvm_operation_continue(ns_nvm_request_t *request, bool free_request);
static void ns_nvm_operation_end(ns_nvm_request_t *ns_nvm_request_ptr, int client_retval);

static NS_LIST_DEFINE(ns_nvm_request_list, ns_nvm_request_t, link);

/*
 * Callback from platform NVM adaptation
 */
void ns_nvm_callback_func(platform_nvm_status status, void *args)
{
    ns_nvm_request_t *ns_nvm_request_ptr = (ns_nvm_request_t*)args;
    int client_retval = NS_NVM_OK;

    if (status == PLATFORM_NVM_ERROR) {
        client_retval = NS_NVM_ERROR;
    } else if (status == PLATFORM_NVM_KEY_NOT_FOUND) {
        client_retval = NS_NVM_DATA_NOT_FOUND;
    }

    switch(ns_nvm_request_ptr->operation) {
        case NS_NVM_INIT:
            ns_nvm_operation_continue(ns_nvm_request_ptr->original_request, true);
            ns_dyn_mem_free(ns_nvm_request_ptr);
            break;
        case NS_NVM_FLUSH:
        case NS_NVM_KEY_READ:
            ns_nvm_operation_end(ns_nvm_request_ptr, client_retval);
            break;
        case NS_NVM_KEY_CREATE:
            if (status == PLATFORM_NVM_OK) {
                ns_nvm_request_ptr->operation = NS_NVM_KEY_WRITE;
                platform_nvm_write(ns_nvm_callback_func, ns_nvm_request_ptr->client_key_name, ns_nvm_request_ptr->buffer, ns_nvm_request_ptr->buffer_len, ns_nvm_request_ptr);
            } else {
                ns_nvm_operation_end(ns_nvm_request_ptr, client_retval);
            }
            break;
        case NS_NVM_KEY_DELETE:
        case NS_NVM_KEY_WRITE:
            if (status == PLATFORM_NVM_OK) {
                // write ok, flush the changes
                ns_nvm_request_ptr->operation = NS_NVM_FLUSH;
                platform_nvm_flush(ns_nvm_callback_func, ns_nvm_request_ptr);
            } else {
                // write failed, inform client
                ns_nvm_operation_end(ns_nvm_request_ptr, client_retval);
            }
            break;
    }
}

int ns_nvm_key_delete(ns_nvm_callback *callback, const char *key_name, void *context)
{
    if (!callback || !key_name) {
        return NS_NVM_ERROR;
    }
    ns_nvm_request_t *ns_nvm_request_ptr = ns_nvm_create_request(callback, context, key_name, NULL, NULL, NS_NVM_KEY_DELETE);
    return ns_nvm_operation_start(ns_nvm_request_ptr);
}

int ns_nvm_data_read(ns_nvm_callback *callback, const char *key_name, uint8_t *buf, uint16_t *buf_len, void *context)
{
    if (!callback || !key_name || !buf || !buf_len) {
        return NS_NVM_ERROR;
    }
    ns_nvm_request_t *ns_nvm_request_ptr = ns_nvm_create_request(callback, context, key_name, buf, buf_len, NS_NVM_KEY_READ);
    return ns_nvm_operation_start(ns_nvm_request_ptr);
}

int ns_nvm_data_write(ns_nvm_callback *callback, const char *key_name, uint8_t *buf, uint16_t *buf_len, void *context)
{
    if (!callback || !key_name || !buf || !buf_len) {
        return NS_NVM_ERROR;
    }
    ns_nvm_request_t *ns_nvm_request_ptr = ns_nvm_create_request(callback, context, key_name, buf, buf_len, NS_NVM_KEY_WRITE);
    return ns_nvm_operation_start(ns_nvm_request_ptr);
}

static int ns_nvm_operation_start(ns_nvm_request_t *nvm_request)
{
    int ret = NS_NVM_OK;
    platform_nvm_status pnvm_status;

    if (!nvm_request) {
        return NS_NVM_MEMORY;
    }
    if (ns_nvm_initialized == true) {
        // NVM already initialized, continue directly
        if (!ns_nvm_operation_in_progress) {
            ret = ns_nvm_operation_continue(nvm_request, true);
        } else {
            // add request to list and handle when existing calls has been handled.
            ns_list_add_to_end(&ns_nvm_request_list, nvm_request);
        }
    } else {
        ns_nvm_request_t *ns_nvm_request_ptr = ns_nvm_create_request(NULL, NULL, NULL, NULL, NULL, NS_NVM_INIT);
        if (!ns_nvm_request_ptr) {
            ns_dyn_mem_free(nvm_request);
            ns_dyn_mem_free(ns_nvm_request_ptr);
            return NS_NVM_MEMORY;
        }
        ns_nvm_request_ptr->original_request = nvm_request;
        pnvm_status = platform_nvm_init(ns_nvm_callback_func, ns_nvm_request_ptr);
        if (pnvm_status != PLATFORM_NVM_OK) {
            ns_dyn_mem_free(nvm_request);
            ns_dyn_mem_free(ns_nvm_request_ptr);
            return NS_NVM_ERROR;
        }
        ns_list_init(&ns_nvm_request_list);
        ns_nvm_initialized = true;
        ns_nvm_operation_in_progress = true;
    }
    return ret;
}

static ns_nvm_request_t *ns_nvm_create_request(ns_nvm_callback *callback, void *context, const char *key_name, uint8_t *buf, uint16_t *buf_len, uint8_t operation)
{
    ns_nvm_request_t *ns_nvm_request_ptr = ns_dyn_mem_temporary_alloc(sizeof(ns_nvm_request_t));
    if (!ns_nvm_request_ptr) {
        return NULL;
    }
    ns_nvm_request_ptr->client_context = context;
    ns_nvm_request_ptr->callback = callback;
    ns_nvm_request_ptr->client_key_name = key_name;
    ns_nvm_request_ptr->operation = operation;
    ns_nvm_request_ptr->buffer = buf;
    ns_nvm_request_ptr->buffer_len = buf_len;

    return ns_nvm_request_ptr;
}

static int ns_nvm_operation_continue(ns_nvm_request_t *request, bool free_request)
{
    platform_nvm_status ret = PLATFORM_NVM_OK;

    ns_nvm_operation_in_progress = true;
    switch(request->operation) {
        case NS_NVM_KEY_WRITE:
            request->operation = NS_NVM_KEY_CREATE;
            ret = platform_nvm_key_create(ns_nvm_callback_func, request->client_key_name, *request->buffer_len, 0, request);
            break;
        case NS_NVM_KEY_READ:
            ret = platform_nvm_read(ns_nvm_callback_func, request->client_key_name, request->buffer, request->buffer_len, request);
            break;
        case NS_NVM_KEY_DELETE:
            ret = platform_nvm_key_delete(ns_nvm_callback_func, request->client_key_name, request);
            break;
    }

    if (ret != PLATFORM_NVM_OK) {
        if (free_request == true) {
            // free request if requested
            ns_dyn_mem_free(request);
        }
        ns_nvm_operation_in_progress = false;
        return NS_NVM_ERROR;
    }

    return NS_NVM_OK;
}

static void ns_nvm_operation_end(ns_nvm_request_t *ns_nvm_request_ptr, int client_retval)
{
    ns_nvm_request_ptr->callback(client_retval, ns_nvm_request_ptr->client_context);
    ns_dyn_mem_free(ns_nvm_request_ptr);
    ns_nvm_operation_in_progress = false;

    ns_list_foreach_safe(ns_nvm_request_t, pending_req, &ns_nvm_request_list) {
        // there are pending requests to be processed
        ns_list_remove(&ns_nvm_request_list, pending_req);
        int ret = ns_nvm_operation_continue(pending_req, false);
        if (ret != NS_NVM_OK) {
            ns_nvm_operation_end(pending_req, ret);
        } else {
            break;
        }
    }
}
