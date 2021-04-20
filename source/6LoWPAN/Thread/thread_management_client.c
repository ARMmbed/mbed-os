/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#ifdef HAVE_THREAD
#include <ns_types.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "thread_tmfcop_lib.h"

#include "coap_service_api.h"

#include "thread_common.h"
#include "thread_config.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_constants.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "NWK_INTERFACE/Include/protocol.h"

#define TRACE_GROUP TRACE_GROUP_THREAD_MANAGEMENT_CLIENT

typedef struct thread_management {
    thread_management_client_router_id_cb *router_id_cb_ptr;
    thread_management_client_router_id_cb *router_id_release_cb_ptr;
    thread_management_client_network_data_set_cb *network_data_set_cb_ptr;
    thread_management_client_network_data_set_cb *neighbor_discovery_cb_ptr;
    int8_t interface_id;
    int8_t coap_service_id; // COAP service ID from Management server
    ns_list_link_t link;
} thread_management_t;

static NS_LIST_DEFINE(instance_list, thread_management_t, link);

static thread_management_t *thread_management_find(int8_t interface_id)
{
    thread_management_t *this = NULL;
    ns_list_foreach(thread_management_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static thread_management_t *thread_management_find_by_service(int8_t service_id)
{
    thread_management_t *this = NULL;
    ns_list_foreach(thread_management_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static int thread_management_client_get_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    uint8_t *router_id_mask_ptr;
    uint16_t router_id;
    uint8_t *ptr;
    int8_t status = 0;
    (void)source_address;
    (void)source_port;

    tr_debug("Thread router id client cb");
    if (!this) {
        return -1;
    }

    if ((!response_ptr) || 9 > thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_ROUTER_MASK, &router_id_mask_ptr) ||
            2 > thread_meshcop_tlv_data_get_uint16(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_RLOC16, &router_id) ||
            1 > thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_STATUS, &ptr) || *ptr != 0) {
        // Failure in message
        status = -1;
        router_id_mask_ptr = NULL;
        router_id = 0xfffe;
    }

    if (this->router_id_cb_ptr) {
        this->router_id_cb_ptr(this->interface_id, status, router_id, router_id_mask_ptr);
    }

    return 0;
}

static int thread_management_client_release_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    (void) source_address;
    (void) source_port;
    (void) response_ptr;
    if (this && this->router_id_release_cb_ptr) {
        this->router_id_release_cb_ptr(this->interface_id, 0, 0xfffe, NULL);
    }
    return 0;
}

/**
 * Public Api functions
 */
void thread_management_client_init(int8_t interface_id)
{
    thread_management_t *this = thread_management_find(interface_id);
    if (this) {
        return;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_management_t));

    if (this) {
        this->network_data_set_cb_ptr = NULL;
        this->router_id_release_cb_ptr = NULL;
        this->neighbor_discovery_cb_ptr = NULL;
        this->router_id_cb_ptr = NULL;
        this->interface_id = interface_id;
        //TODO: Check if to use ephemeral port here

        this->coap_service_id = thread_management_server_service_id_get(interface_id);
        if (this->coap_service_id < 0) {
            tr_error("Failed to init COAP service");
        }
        ns_list_add_to_start(&instance_list, this);
    }
    return;
}

void thread_management_client_delete(int8_t interface_id)
{
    thread_management_t *this = thread_management_find(interface_id);
    if (!this) {
        return;
    }

    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
    return;
}

int thread_management_client_router_id_get(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb, uint8_t status)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint8_t payload[30];//!< max length 4 + 2, 4 + 8,  4 + 8,
    uint8_t destination[16];
    uint8_t *ptr;
    protocol_interface_info_entry_t *cur;

    if (!this || !mac || !id_cb) {
        return -1;
    }

    if (0 != thread_management_get_leader_address(interface_id, destination)) {
        return -2;
    }

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !cur->thread_info) {
        return -3;
    }

    this->router_id_cb_ptr = id_cb;
    ptr = payload;
    if (router_id < 0xfffe) {
        ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, router_id);
    }
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_MAC_ADDRESS, 8, mac);

    tr_debug("thread router id get id: %d, mac %s", router_id, trace_array(mac, 8));

    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, status);

    return coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination, THREAD_MANAGEMENT_PORT,
                                     COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ROUTER_ID_ASSIGNMENT, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_client_get_cb);
}

int thread_management_client_router_id_release(int8_t interface_id, uint8_t mac[8], uint16_t router_id, thread_management_client_router_id_cb *id_cb)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint8_t payload[30];//!< max length 4 + 2, 4 + 8,  4 + 8,
    uint8_t destination[16];
    uint8_t *ptr;

    if (!this || !mac || router_id >= 0xfffe) {
        return -1;
    }

    if (0 != thread_management_get_leader_address(interface_id, destination)) {
        return -2;
    }

    this->router_id_release_cb_ptr = id_cb;
    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, router_id);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_MAC_ADDRESS, 8, mac);

    tr_debug("thread router id release id: %d, mac %s", router_id, trace_array(mac, 8));

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ROUTER_ID_RELEASE, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_client_release_cb);
    return 0;
}

static int thread_management_client_register_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    (void) source_address;
    (void) source_port;
    if (!this) {
        return -1;
    }

    if (this->network_data_set_cb_ptr) {
        if (response_ptr) {
            // If we get response status is OK
            this->network_data_set_cb_ptr(this->interface_id, 0, response_ptr->payload_ptr, response_ptr->payload_len);
        } else {
            this->network_data_set_cb_ptr(this->interface_id, -1, NULL, 0);
        }
    }

    return 0;
}

int thread_management_client_network_data_register(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len, thread_management_client_network_data_set_cb *set_cb)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint8_t destination[16];

    if (!this) {
        return -1;
    }

    if (thread_management_get_leader_address(interface_id, destination)) {
        return -2;
    }

    this->network_data_set_cb_ptr = set_cb;
    tr_debug("thread network data send to %s", trace_ipv6(destination));

    return coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination, THREAD_MANAGEMENT_PORT,
                                     COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_NETWORK_DATA, COAP_CT_OCTET_STREAM, data_ptr, data_len, thread_management_client_register_cb);
}

int thread_management_client_network_data_unregister(int8_t interface_id, uint16_t rloc16)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint8_t payload[4];
    uint8_t *ptr;
    uint8_t destination[16];

    if (!this) {
        return -1;
    }

    if (thread_management_get_leader_address(interface_id, destination)) {
        return -2;
    }
    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, rloc16);

    tr_debug("thread network data unregister");

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_NETWORK_DATA, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_client_register_cb);
    return 0;
}

static int thread_management_client_neighbor_discovery_data_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    uint8_t *nd_data_options_ptr = NULL;
    int8_t status = -1;
    uint16_t nd_data_options_len = 0;
    (void) source_address;
    (void) source_port;
    if (!this) {
        return -1;
    }

    if (response_ptr) {
        status = 0; // If we get response status is OK
        nd_data_options_len = thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_ND_DATA, &nd_data_options_ptr);
    }

    if (this->neighbor_discovery_cb_ptr) {
        this->neighbor_discovery_cb_ptr(this->interface_id, status, nd_data_options_ptr, nd_data_options_len);
    }

    return 0;
}

int thread_management_client_get_interface_id_by_service_id(int8_t service_id)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    if (!this) {
        return -1;
    }
    return this->interface_id;
}

int thread_management_client_neighbor_discovery_data_request(int8_t interface_id, const uint8_t destination[16],  const uint8_t *options, uint8_t options_len, thread_management_client_network_data_set_cb *set_cb)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint8_t payload[2 + options_len]; //type + length + num of options
    uint8_t *ptr = &payload[2];
    uint8_t payload_len;

    if (!this || !options || options_len == 0) {
        return -1;
    }

    this->neighbor_discovery_cb_ptr = set_cb;
    tr_debug("thread neighbor discovery data request");

    memcpy(ptr, options, options_len);
    thread_tmfcop_tlv_data_write_header(payload, TMFCOP_TLV_ND_OPTION, options_len);
    payload_len = 2 + options_len; //type + length + options

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_NEIGHBOR_DISCOVERY_DATA_REQ, COAP_CT_OCTET_STREAM, payload, payload_len, thread_management_client_neighbor_discovery_data_cb);
    return 0;
}

static int thread_management_client_active_set_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    (void)source_address;
    (void)source_port;
    uint8_t state;

    tr_debug("Receiving active set response from leader");

    if (!this) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur || !cur->thread_info) {
        return -2;
    }

    if (!response_ptr || response_ptr->coap_status == COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED || !response_ptr->payload_ptr) {
        tr_error("active set failed");
        goto getout;
    }
    if (1 > thread_meshcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &state) || state != 1) {
        tr_error("active set rejected");
        goto getout;
    }
    // TODO this is problematic we can not advertise before leader distributes the data
    // Current approach is to wait for leader
    // Spec says we could form own partition or just learn the older ones
    // We could then just clear timestamp from active set and learn new one.
    // thread_active_configuration_dataset_query_done(cur);
    return 0;

getout:

    return 0;
}

int8_t thread_management_client_active_set(int8_t interface_id, uint8_t *dst_address)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint16_t response_len;
    uint8_t *response_ptr, *ptr;
    tr_debug("Sending active set to leader");

    response_len = thread_joiner_application_next_active_config_length(interface_id);
    if (!this || response_len < 1) {
        return -1;
    }

    response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        tr_warn("Out of mem");
        return -2;
    }
    ptr = thread_joiner_application_next_active_config_write(interface_id, response_ptr);

    if (0 == coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, dst_address, THREAD_MANAGEMENT_PORT,
                                       COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ACTIVE_SET, COAP_CT_OCTET_STREAM, response_ptr, (uint16_t)(ptr - response_ptr), thread_management_client_active_set_response_cb)) {
        tr_warn("request send failed");
    }
    ns_dyn_mem_free(response_ptr);
    return 0;
}


static int thread_management_client_pending_set_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    (void)source_address;
    (void)source_port;
    uint8_t state;

    tr_debug("Receiving pending set response from leader");

    if (!this) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur || !cur->thread_info) {
        return -2;
    }

    if (!response_ptr || response_ptr->coap_status == COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED || !response_ptr->payload_ptr) {
        tr_error("pending set failed");
        goto getout;
    }
    if (1 > thread_meshcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &state) || state != 1) {
        tr_error("pending set rejected");
        goto getout;
    }

    return 0;

getout:
    return 0;
}

int8_t thread_management_client_pending_set(int8_t interface_id, uint8_t *dst_address)
{
    thread_management_t *this = thread_management_find(interface_id);
    uint16_t response_len;
    uint8_t *response_ptr, *ptr;
    tr_debug("Sending Pending set to leader");

    if (!this) {
        return -1;
    }

    response_len = thread_joiner_application_next_pending_config_length(interface_id);

    response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        tr_warn("Out of mem");
        return -2;
    }
    ptr = thread_joiner_application_next_pending_config_build(interface_id, response_ptr);

    if (0 == coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, dst_address, THREAD_MANAGEMENT_PORT,
                                       COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_PENDING_SET, COAP_CT_OCTET_STREAM, response_ptr, (uint16_t)(ptr - response_ptr), thread_management_client_pending_set_response_cb)) {
        tr_warn("request send failed");
    }
    ns_dyn_mem_free(response_ptr);
    return 0;
}

static int thread_management_client_provision_request_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_t *this = thread_management_find_by_service(service_id);
    (void)source_address;
    (void)source_port;
    uint8_t state;

    tr_debug("Thread provisioning Received");

    if (!this) {
        return 0;
    }

    thread_joiner_application_provisioning_set(this->interface_id, PROVISIONING_STATUS_NOT_DONE);

    // CoAP request timed out
    if (!response_ptr) {
        return 0;
    }

    if (response_ptr->msg_code != COAP_MSG_CODE_RESPONSE_CHANGED) {
        tr_error("Provisioning failed");
        thread_joiner_application_provisioning_set(this->interface_id, PROVISIONING_STATUS_REJECTED);
        return 0;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur || !cur->thread_info || 1 > thread_meshcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &state)) {
        tr_error("provisioning response failed");
        return 0;
    }
    tr_debug("Thread provisioning status %d", state);

    if (state == 1) {
        thread_joiner_application_provisioning_set(this->interface_id, PROVISIONING_STATUS_DONE);
    } else {
        thread_joiner_application_provisioning_set(this->interface_id, PROVISIONING_STATUS_REJECTED);
    }

    return 0;
}

int thread_management_client_provision_request(int8_t interface_id, uint8_t *dst_address, uint16_t port)
{
    thread_management_t *this;
    uint8_t *ptr, *data_ptr;
    uint16_t length;

    if (thread_joiner_application_provisioning_get(interface_id) == PROVISIONING_STATUS_ONGOING) {
        // wait for response to to the message
        return 0;
    }

    tr_debug("Thread provisioning start send");
    this = thread_management_find(interface_id);
    device_configuration_s *device_configuration_ptr = thread_joiner_application_get_device_config(interface_id);

    if (!this || !device_configuration_ptr) {
        return -1;
    }
    length = thread_joiner_application_device_configuration_length(device_configuration_ptr);
    length += 3;// State

    data_ptr = ptr = ns_dyn_mem_alloc(length);
    if (!ptr) {
        tr_error("Failed to start Commissioning");
        return -1;
    }
    ptr = thread_joiner_application_device_configuration_build(ptr, device_configuration_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 1);


    thread_joiner_application_provisioning_set(interface_id, PROVISIONING_STATUS_ONGOING);

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, dst_address, port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_JOINER_APPLICATION_REQUEST, COAP_CT_OCTET_STREAM,
                              data_ptr, ptr - data_ptr, thread_management_client_provision_request_cb);

    ns_dyn_mem_free(data_ptr);

    return 0;
}

static int thread_management_client_proactive_an_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;
    (void) source_address;
    (void) source_port;
    (void) response_ptr;
    return 0;
}

void thread_management_client_proactive_an(int8_t interface_id, const uint8_t address[16], const uint16_t rloc, const uint8_t ml_eid[8], const uint8_t dst_addr[16])
{
    thread_management_t *this = thread_management_find(interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!this || !cur) {
        tr_warn("No resolution client or protocol info!");
        return;
    }

    tr_debug("Thread resolution client; proactive address notification");

    uint8_t payload[2 + 16 + 2 + 2 + 2 + 8];
    uint8_t *ptr;

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, address);
    ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, rloc);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, ml_eid);

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE,
                              dst_addr, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_ADDRESS_NOTIFICATION, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, thread_management_client_proactive_an_cb);
}
void thread_management_client_coap_message_delete(int8_t interface_id, uint16_t coap_message_id)
{
    thread_management_t *this = thread_management_find(interface_id);

    if (!this) {
        return;
    }

    coap_service_request_delete(this->coap_service_id, coap_message_id);
}

void thread_management_client_old_partition_data_clean(int8_t interface_id)
{
    thread_management_t *this = thread_management_find(interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!this || !cur) {
        return;
    }

    cur->thread_info->localServerDataBase.publish_coap_req_id = 0;
    cur->thread_info->routerIdRequested = false;
    coap_service_request_delete_by_service_id(this->coap_service_id);
}

#ifdef HAVE_THREAD_V2
void thread_management_client_addr_ntf_send(int8_t interface_id, uint8_t *destination_address, const uint8_t *addr_data_ptr, uint8_t bbr_status)
{
    uint8_t payload[16 + 1]; // Target eid + Status
    uint8_t *ptr;
    uint8_t br_ml_addr[16];
    uint8_t seq;
    uint32_t delay_timer;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return;
    }

    if (!thread_bootstrap_is_domain_prefix(cur, addr_data_ptr)) {
        return;
    }

    if (0 != thread_common_primary_bbr_get(cur, br_ml_addr, &seq, &delay_timer, NULL)) {
        // No pBBR present
        return;
    }

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, addr_data_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, bbr_status);

    coap_service_request_send(thread_management_server_service_id_get(cur->id), COAP_REQUEST_OPTIONS_ADDRESS_SHORT,
                              destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_BBR_DOMAIN_ADDRESS_NOTIFICATION, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, NULL);
    return;
}
static int thread_bootstrap_mlr_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;
    (void) source_address;
    (void) source_port;
    (void) response_ptr;

    tr_debug("Thread MLR callback");
    return 0;
}

int thread_management_client_mlr_req_send(int8_t interface_id, const uint8_t br_addr[16], const uint8_t *address, uint8_t address_len)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !br_addr || !address) {
        return -1;
    }
    tr_debug("thread MLR.req send");

    coap_service_request_send(thread_management_server_service_id_get(cur->id), COAP_REQUEST_OPTIONS_NONE, br_addr, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_MCAST_LISTENER_REPORT, COAP_CT_OCTET_STREAM, address, address_len, thread_bootstrap_mlr_cb);
    return 0;
}
#endif
#endif
