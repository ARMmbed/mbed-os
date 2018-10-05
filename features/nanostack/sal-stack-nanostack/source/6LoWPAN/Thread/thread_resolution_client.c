/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "nsconfig.h"
#ifdef HAVE_THREAD_NEIGHBOR_DISCOVERY
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "thread_tmfcop_lib.h"

#include "coap_service_api.h"

#include "thread_config.h"
#include "thread_resolution_client.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_RESOLUTION_CLIENT

#define ADDRESS_QUERY_TIMEOUT               3
#define ADDRESS_QUERY_INITIAL_RETRY_DELAY   15
#define ADDRESS_QUERY_MAX_RETRY_DELAY       (8*60*60) // 8 hours = 28800 seconds

#define ADDRESS_QUERY_SET_SIZE 8

/* Thread says queries go to this "all-routers" address - routers respond for
 * end nodes.
 */
static const uint8_t ADDR_MESH_LOCAL_ALL_ROUTERS[16] = { 0xff, 0x03, [15] = 0x02 }; // ff03::2

typedef struct address_query {
    /* Outgoing query information */
    uint8_t eid[16]; // IPv6 address
    uint16_t aq_timeout;
    uint16_t aq_retry_timeout;
    uint8_t aq_failures;
    /* Incoming notification information */
    uint8_t an_ml_eid[8];
    bool an_received;
    bool an_duplicate;
    uint16_t an_rloc;
    uint32_t an_last_transaction_time;
    ns_list_link_t link;
} address_query_t;

typedef struct thread_resolution_client {
    thread_resolution_client_response_cb *response_cb_ptr;
    thread_resolution_client_notification_cb *notification_cb_ptr;
    thread_resolution_client_error_cb *error_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id;
    NS_LIST_HEAD(address_query_t, link) queries;
    ns_list_link_t link;
} thread_resolution_client_t;

static NS_LIST_DEFINE(instance_list, thread_resolution_client_t, link);

static thread_resolution_client_t *thread_resolution_client_find(int8_t interface_id)
{
    ns_list_foreach(thread_resolution_client_t , cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

static thread_resolution_client_t *thread_resolution_client_find_by_service(int8_t service_id)
{
    ns_list_foreach(thread_resolution_client_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

static address_query_t *address_query_find(thread_resolution_client_t *this, const uint8_t eid[static 16])
{
    ns_list_foreach(address_query_t, query, &this->queries) {
        if (addr_ipv6_equal(query->eid, eid)) {
            return query;
        }
    }
    return NULL;
}

static void address_query_delete(thread_resolution_client_t *this, address_query_t *query)
{
    ns_list_remove(&this->queries, query);
    ns_dyn_mem_free(query);
}

static void address_query_timeout(thread_resolution_client_t *this, address_query_t *query)
{
    if (!query->an_received) {
        query->aq_retry_timeout = ADDRESS_QUERY_INITIAL_RETRY_DELAY;
        /* Spec says initial * 2^Failures, _after_ increment, but that's silly */
        for (uint8_t i = query->aq_failures; i; i--) {
            if (query->aq_retry_timeout < ADDRESS_QUERY_MAX_RETRY_DELAY / 2) {
                query->aq_retry_timeout *= 2;
            } else {
                query->aq_retry_timeout = ADDRESS_QUERY_MAX_RETRY_DELAY;
                break;
            }
        }
        query->aq_retry_timeout++; // +1 for timer vagaries

        if (query->aq_failures < UINT8_MAX) {
            query->aq_failures++;
        }
        tr_info("AQ failed (%d)", query->aq_failures);
        this->response_cb_ptr(this->interface_id, -1, query->eid, 0xFFFE, 0, NULL);
        return;
    }

    if (query->an_duplicate) {
        /* Note that error message contains ML-EID that we are *accepting*. All
         * *other* routers should invalidate, meaning it still makes sense
         * to process the result.
         */
        tr_warn("Duplicate address during resolution");
        thread_resolution_client_address_error(this->interface_id, ADDR_MESH_LOCAL_ALL_ROUTERS, query->eid, query->an_ml_eid);
    }

    /* No need to make the callback here - we did it immediately */
    //tr_info("Resolved %s %s -> %04x", trace_ipv6(query->eid), trace_array(query->an_ml_eid, 8), query->an_rloc);
    //this->response_cb_ptr(this->interface_id, 0, query->eid, query->an_rloc, query->an_last_transaction_time, query->an_ml_eid);
    address_query_delete(this, query);
}

/* Handle incoming "notification" POSTs */
static int thread_resolution_client_notification_post_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_resolution_client_t *this = thread_resolution_client_find_by_service(service_id);
    uint8_t *target_ip_addr = NULL;
    uint8_t *mleid = NULL;
    uint16_t target_rloc = 0xffff;
    uint32_t last_transaction_time = 0;
    sn_coap_msg_code_e coap_code;
    (void) source_address;
    (void) source_port;

    tr_debug("Thread resolution notification cb");
    if (!this) {
        coap_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto done;
    }

    /* Target EID, Locator and MLE-ID are compulsory */
    if (16 > thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &target_ip_addr) ||
            2 > thread_tmfcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_RLOC16, &target_rloc) ||
            8 > thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_ML_EID, &mleid)) {
        coap_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto done;
    }

    /* Last transaction is optional - I think defaulting to 0 will give the desired result when it's absent */
    thread_tmfcop_tlv_data_get_uint32(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_LAST_TRANSACTION_TIME, &last_transaction_time);

    address_query_t *query = address_query_find(this, target_ip_addr);
    if (query && query->aq_timeout > 0) {
        tr_info("a/an(solicited); target=%s mleid=%s rloc=%04x, ltt=%"PRIu32, trace_ipv6(target_ip_addr), trace_array(mleid, 8), target_rloc, last_transaction_time);
        /* A notification for an ongoing query */
        if (query->an_received && memcmp(query->an_ml_eid, mleid, 8)) {
            query->an_duplicate = true;
            tr_warn("DUP!");
        }

        if (!query->an_received || last_transaction_time <= query->an_last_transaction_time) {
            query->an_received = true;
            memcpy(query->an_ml_eid, mleid, 8);
            query->an_rloc = target_rloc;
            query->an_last_transaction_time = last_transaction_time;
            /* If we get "old then new" responses, we call the response twice. This
             * will likely mean the first transmission goes to the old address, then subsequent
             * will go to the new address. But better than waiting the full 3 seconds
             * to make the call.
             */
            tr_info("Resolved %s %s -> %04x", trace_ipv6(query->eid), trace_array(query->an_ml_eid, 8), query->an_rloc);
            this->response_cb_ptr(this->interface_id, 0, query->eid, query->an_rloc, query->an_last_transaction_time, query->an_ml_eid);
            tr_info("Accepted");
        } else {
            tr_info("Rejected");
        }
    } else {
        /* Not for an non-ongoing query, but delete any stale ones - no longer "failed" */
        if (query) {
            ns_list_remove(&this->queries, query);
            ns_dyn_mem_free(query);
        }
        tr_info("a/an(unsolicited); target=%s mleid=%s rloc=%04x", trace_ipv6(target_ip_addr), trace_array(mleid, 8), target_rloc);
        /* And tell the core */
        if (this->notification_cb_ptr) {
            this->notification_cb_ptr(this->interface_id, target_ip_addr, target_rloc, mleid);
        } else {
            coap_code = COAP_MSG_CODE_RESPONSE_FORBIDDEN;
            goto done;
        }
    }

    coap_code = COAP_MSG_CODE_RESPONSE_CHANGED;

done:
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_ADDRESS_SHORT, request_ptr, coap_code, COAP_CT_NONE, NULL, 0);
    return 0;
}

/* Handle incoming "error" POSTs */
static int thread_resolution_client_error_post_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_resolution_client_t *this = thread_resolution_client_find_by_service(service_id);
    uint8_t *target_ip_addr = NULL;
    uint8_t *mleid = NULL;
    sn_coap_msg_code_e coap_code;
    (void) source_address;
    (void) source_port;

    tr_debug("Thread resolution error cb");
    if (!this) {
        coap_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto done;
    }

    /* Target EID and ML-EID are compulsory */
    if (16 > thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &target_ip_addr) ||
            8 > thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_ML_EID, &mleid)) {
        coap_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto done;
    }

    if (this->error_cb_ptr) {
        this->error_cb_ptr(this->interface_id, target_ip_addr, mleid);
    } else {
        coap_code = COAP_MSG_CODE_RESPONSE_FORBIDDEN;
        goto done;
    }

    coap_code = COAP_MSG_CODE_RESPONSE_CHANGED;

done:
    /* If the CoAP message was sent as confirmable, send response */
    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_ADDRESS_SHORT, request_ptr, coap_code, COAP_CT_NONE, NULL, 0);
        return 0;
    }
    return -1;
}


/**
 * Public Api functions
 */
void thread_resolution_client_init(int8_t interface_id)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    if (this) {
        return;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_resolution_client_t));
    if (!this) {
        return;
    }

    this->interface_id = interface_id;
    this->notification_cb_ptr = NULL;
    this->error_cb_ptr = NULL;
    ns_list_init(&this->queries);
    //TODO: Check if to use ephemeral port here
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    ns_list_add_to_start(&instance_list, this);

    coap_service_register_uri(this->coap_service_id, THREAD_URI_ADDRESS_NOTIFICATION, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_resolution_client_notification_post_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ADDRESS_ERROR, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_resolution_client_error_post_cb);
}

void thread_resolution_client_delete(int8_t interface_id)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    if (!this) {
        return;
    }

    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_ADDRESS_NOTIFICATION);
    coap_service_delete(this->coap_service_id);
    ns_list_foreach_safe(address_query_t, query, &this->queries) {
        ns_list_remove(&this->queries, query);
        ns_dyn_mem_free(query);
    }
    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
}

void thread_resolution_client_set_notification_cb(int8_t interface_id, thread_resolution_client_notification_cb notification_cb)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    if (!this) {
        return;
    }

    this->notification_cb_ptr = notification_cb;
}

void thread_resolution_client_set_error_cb(int8_t interface_id, thread_resolution_client_error_cb error_cb)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    if (!this) {
        return;
    }

    this->error_cb_ptr = error_cb;
}

int thread_resolution_client_address_error(int8_t interface_id, const uint8_t dest_ip_addr[16], const uint8_t target_ip_addr[16], const uint8_t ml_eid[8])
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    sn_coap_msg_type_e msg_type = COAP_MSG_TYPE_CONFIRMABLE;
    uint8_t payload[2 + 16 + 2 + 8];
    uint8_t *ptr;
    uint8_t options;

    if (!this || !target_ip_addr || !dest_ip_addr || !ml_eid) {
        return -1;
    }

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, target_ip_addr);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, ml_eid);

    options = COAP_REQUEST_OPTIONS_ADDRESS_SHORT;
    if (addr_is_ipv6_multicast(dest_ip_addr)) {
        options |= COAP_REQUEST_OPTIONS_MULTICAST;
        msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    }

    tr_debug("TX thread address error: target %s, mle %s, dest %s", trace_ipv6(target_ip_addr) ,trace_array(ml_eid, 8), trace_ipv6(dest_ip_addr));

    /* We don't expect a response to this POST, so we don't specify a callback. */
    coap_service_request_send(this->coap_service_id, options,
                              dest_ip_addr, THREAD_MANAGEMENT_PORT,
                              msg_type, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_ADDRESS_ERROR, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, NULL);

    return 0;

}

int thread_resolution_client_resolve(int8_t interface_id, uint8_t ip_addr[16], thread_resolution_client_response_cb *resp_cb)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    uint8_t payload[2 + 16];
    uint8_t *ptr;

    if (!this || !ip_addr || !resp_cb) {
        return -1;
    }

    address_query_t *query = address_query_find(this, ip_addr);
    if (query) {
        if (query->aq_timeout != 0) {
            /* Already in progress */
            return 0;
        }
        if (query->aq_retry_timeout != 0) {
            /* Will not query - this will make the packet get dropped (with Address Unreachable) */
            return -1;
        }
        /* Otherwise both timeouts zero - fine to proceed */
        /* Remove so it can be readded to start of list */
        ns_list_remove(&this->queries, query);
    } else {
        /* Get a new set entry - periodic timer will clear up if we go above limit */
        query = ns_dyn_mem_alloc(sizeof *query);
        if (!query) {
            return -1;
        }
        memset(query, 0, sizeof *query);
        memcpy(query->eid, ip_addr, 16);
    }
    ns_list_add_to_start(&this->queries, query);

    query->aq_timeout = ADDRESS_QUERY_TIMEOUT + 1; // +1 to allow for timer vagaries

    this->response_cb_ptr = resp_cb;
    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, ip_addr);

    tr_debug("thread address query: target %s", trace_ipv6(ip_addr));

    /* We don't expect a response to this POST, so we don't specify a callback.
     * Instead, this POST may trigger an inbound POST, which will go to our
     * ADDRESS_QUERY_RESPONSE POST handler.
     */
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_MULTICAST | COAP_REQUEST_OPTIONS_ADDRESS_SHORT,
                              ADDR_MESH_LOCAL_ALL_ROUTERS, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_ADDRESS_QUERY_REQUEST, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, NULL);

    return 0;
}

void thread_resolution_client_timer(int8_t interface_id, uint16_t seconds)
{
    thread_resolution_client_t *this = thread_resolution_client_find(interface_id);
    if (!this) {
        return;
    }
    unsigned count = 0;
    /* Run timers on set entries */
    ns_list_foreach_safe(address_query_t, query, &this->queries) {
        ++count;
        if (query->aq_retry_timeout != 0) {
            if (query->aq_retry_timeout <= seconds) {
                query->aq_retry_timeout = 0;
            } else {
                query->aq_retry_timeout -= seconds;
            }
        }
        if (query->aq_timeout != 0) {
            if (query->aq_timeout <= seconds) {
                query->aq_timeout = 0;
                address_query_timeout(this, query);
            } else {
                query->aq_timeout -= seconds;
            }
        }
    }
    /* Remove oldest excess entries that are not ongoing */
    if (count > ADDRESS_QUERY_SET_SIZE) {
        ns_list_foreach_reverse_safe(address_query_t, query, &this->queries) {
            if (query->aq_timeout == 0) {
                address_query_delete(this, query);
                if (--count <= ADDRESS_QUERY_SET_SIZE) {
                    break;
                }
            }
        }
    }

}
#endif // HAVE_THREAD_NEIGHBOR_DISCOVERY
