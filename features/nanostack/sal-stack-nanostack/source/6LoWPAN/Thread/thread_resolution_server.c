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
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_resolution_server.h"

#define TRACE_GROUP "TRsS"

#include "coap_service_api.h"

#include "net_interface.h"
#include "socket_api.h"
#include "thread_config.h"
#include "thread_tmfcop_lib.h"
#include "thread_management_if.h"
#include "thread_commissioning_if.h"


typedef struct thread_management_server {
    int8_t interface_id;
    int8_t coap_service_id;
    thread_resolution_server_addr_query_cb *query_cb_ptr;
    ns_list_link_t link;
} thread_resolution_server_t;

static NS_LIST_DEFINE(instance_list, thread_resolution_server_t, link);

static thread_resolution_server_t *thread_resolution_server_find(int8_t interface_id)
{
    ns_list_foreach(thread_resolution_server_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

static thread_resolution_server_t *thread_management_find_by_service(int8_t service_id)
{
    ns_list_foreach(thread_resolution_server_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

/**
 * Address query
 * uri = tn/d/aq
 */
static int thread_resolution_server_query_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_resolution_server_t *this = thread_management_find_by_service(service_id);
    uint8_t payload[2 + 16 + 2 + 2 + 2 + 8 + 2 + 4];
    uint8_t *target_ip_ptr;
    uint8_t mlEID[8];
    bool proxy;
    uint16_t rloc16;
    uint16_t requestor_rloc;
    uint32_t last_transaction_time = 0;
    uint8_t *ptr;
    (void)source_port;

    /* Address queries should be a multicast, non-confirmed POST. Basic CoAP
     * protocol dictates that we should respond in some way if it's not multicast,
     * but we currently can't distinguish. (RFC 7390 says "A server may choose
     * not to respond to an IP multicast request if there's nothing useful to
     * respond back", but that doesn't extend to unicast). So we just drop in
     * all cases of non-resolution. People shouldn't(TM) be sending unicast
     * queries anyway...
     *
     * Note that even when we have an answer, we don't actually send a CoAP
     * response to the POST, but we instead trigger a new unicast confirmed
     * POST to the sender.
     */

    if (!this || !this->query_cb_ptr) {
        /* XXX "Internal Server Error" response? */
        return -1;
    }

    if (16 > thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &target_ip_ptr)) {
        /* XXX "Bad Request" response? */
        return -1;
    }

    requestor_rloc = common_read_16_bit(source_address + 14);

    tr_debug("Thread address query %s", trace_ipv6(target_ip_ptr));

    int ret = this->query_cb_ptr(this->interface_id, target_ip_ptr, &requestor_rloc, &rloc16, &proxy, &last_transaction_time, mlEID);
    if (ret < 0) {
        /* XXX "Forbidden" response? */
        return -1;
    }

    tr_debug("Thread query locator %x", rloc16);

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, target_ip_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, rloc16);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, mlEID);
    if (proxy) {
        tr_debug("    proxy: last transaction %" PRIu32, last_transaction_time);
        ptr = thread_tmfcop_tlv_data_write_uint32(ptr, TMFCOP_TLV_LAST_TRANSACTION_TIME, last_transaction_time);
    }

    // XXX "Accepted" response?

    /* We don't require a response, so we don't specify a callback. Library
     * should retry itself until it gets an ACK.
     */
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_ADDRESS_SHORT,
                              source_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_ADDRESS_NOTIFICATION, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, NULL);

    return -1;// 0 is expecting response to the message as this is different behaviour need to handle it
}

/**
 * Public interface functions
 */
int thread_resolution_server_init(int8_t interface_id, thread_resolution_server_addr_query_cb *query_cb)
{
    thread_resolution_server_t *this = thread_resolution_server_find(interface_id);
    if (this) {
        return -1;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_resolution_server_t));
    if (!this) {
        return -2;
    }

    this->interface_id = interface_id;
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    if (this->coap_service_id < 0) {
        tr_warn("Thread resolution init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
    this->query_cb_ptr = query_cb;
    ns_list_add_to_start(&instance_list, this);

    coap_service_register_uri(this->coap_service_id, THREAD_URI_ADDRESS_QUERY_REQUEST, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_resolution_server_query_cb);

    return 0;
}

void thread_resolution_server_delete(int8_t interface_id)
{
    thread_resolution_server_t *this = thread_resolution_server_find(interface_id);
    if (!this) {
        return;
    }

    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_ADDRESS_QUERY_REQUEST);

    coap_service_delete(this->coap_service_id);
    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
}

#endif //HAVE_THREAD_NEIGHBOR_DISCOVERY

