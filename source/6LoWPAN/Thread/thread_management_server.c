/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_server.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "TMFs"

#include "eventOS_event_timer.h"
#include "coap_service_api.h"

#include "net_interface.h"
#include "socket_api.h"
#include "thread_common.h"
#include "thread_config.h"
#include "thread_tmfcop_lib.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "thread_management_internal.h"
#include "thread_commissioning_if.h"
#include "thread_joiner_application.h"
#include "thread_beacon.h"
#include "thread_bootstrap.h"
#include "thread_management_server.h"
#include "thread_management_client.h"
#include "thread_ccm.h"
#include "thread_nvm_store.h"
#include "mac_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Core/include/ns_address_internal.h"
#include "mlme.h"

#ifdef HAVE_THREAD

//#define TRACE_DEEP
#ifdef TRACE_DEEP
#define tr_deep   tr_debug
#else
#define tr_deep(...)
#endif

typedef struct scan_query {
    int8_t coap_service_id;
    uint8_t channel_mask[6]; //<!** first byte is channel page
    uint16_t channel_mask_len;
    uint8_t *energy_list_ptr;
    uint8_t energy_list_length;
    uint8_t scan_count;
    timeout_t *timer;
    uint8_t address[16];
    uint16_t port;
    uint16_t panid;
    uint8_t count;
    uint16_t period;
    uint16_t duration;
    bool panid_scan: 1;
    bool energy_scan: 1;
} scan_query_t;

typedef struct announce {
    uint8_t channel_mask[6]; //<!** first byte is channel page support only channel page 0
    uint16_t channel_mask_len;
    timeout_t *timer;
    uint8_t count;
    uint16_t period;
    uint16_t channel;
} announce_t;

typedef struct thread_management_server {
    scan_query_t *scan_ptr;
    announce_t *announce_ptr;
    timeout_t *join_ent_timer;
    uint8_t destination_address[16];
    uint8_t one_time_key[16];
    uint16_t relay_port_joiner;
    uint16_t external_commissioner_port;
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t listen_socket_joiner;
    bool joiner_router_enabled: 1;
    ns_list_link_t link;
} thread_management_server_t;

static NS_LIST_DEFINE(instance_list, thread_management_server_t, link);
void thread_energy_scan_timeout_cb(void *arg);

static bool thread_channel_mask_is_channel_set(uint8_t *mask_ptr, uint8_t channel)
{
    uint8_t n;
    uint8_t bit;

    n = (channel) / 8;
    bit = 1 << (7 - (channel) % 8);

    if (n > 5 || channel > 27) {
        return false;
    }
    if (mask_ptr[n + 2] & bit) {
        return true;
    }
    return false;
}


static uint8_t thread_channel_mask_count(uint8_t *mask_ptr)
{
    uint8_t n;
    uint8_t result = 0;
    uint32_t bits;

    bits = common_read_32_bit(mask_ptr + 2);
    bits = bits >> 5;// five lover bits are not used
    for (n = 0; n < 27; n++) {
        if ((bits & 1) == 1) {
            result++;
        }
        bits = bits >> 1;
    }
    tr_debug("Channel mask count = %d ", result);
    return result;
}

/*This method identifies the channels to be scanned based on the bits set in the channel mask
 * */
static uint8_t thread_channels_to_be_scanned(uint8_t *mask_ptr)
{
    uint8_t result = 0;
    uint8_t val = 1;
    uint8_t *ptr = mask_ptr + 2; // first two bytes do not contain the channels to be scanned
    uint8_t j = 0;
    while (j < 4) {

        // one channel for every bit that is set in the mask ptr
        for (int i = 0; i < 8 ; i++) {
            if (val & (*ptr)) {
                result++;
            }
            val = val << 1;
        }
        val = 1;
        ptr++;
        j++;
    }
    return result;
}

static thread_management_server_t *thread_management_server_find(int8_t interface_id)
{
    thread_management_server_t *this = NULL;
    ns_list_foreach(thread_management_server_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static thread_management_server_t *thread_management_find_by_service(int8_t service_id)
{
    thread_management_server_t *this = NULL;
    ns_list_foreach(thread_management_server_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static thread_management_server_t *thread_management_find_by_sckt_id(int8_t sckt_id)
{
    thread_management_server_t *this = NULL;
    ns_list_foreach(thread_management_server_t, cur_ptr, &instance_list) {
        if (cur_ptr->listen_socket_joiner == sckt_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static bool tlv_is_requested(uint8_t *tlv_list, uint16_t list_len, uint8_t tlv)
{
    if (!list_len || !tlv_list) {
        return true;
    }
    for (uint16_t n = 0; n < list_len; n++) {
        if (tlv_list[n] == tlv) {
            return true;
        }
    }
    return false;
}

static int thread_management_server_management_get_respond(int8_t interface_id, int8_t coap_service_id, sn_coap_hdr_s *request_ptr)
{
    uint8_t *ptr = NULL;
    int response_len;
    uint8_t *response_ptr = NULL;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    link_configuration_s *link_configuration;
    device_configuration_s *device_configuration;

    link_configuration = thread_joiner_application_get_config(interface_id);
    device_configuration = thread_joiner_application_get_device_config(interface_id);

    if (!link_configuration || !device_configuration) {
        return -1;
    }

    tr_debug("Recv MGMT_GET request");

    response_len = thread_joiner_application_device_configuration_length(device_configuration);
    response_ptr = ptr = ns_dyn_mem_alloc(response_len);
    if (!ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        tr_error("Out of resources");
        goto send_response;
    }
    ptr = thread_joiner_application_device_configuration_build(ptr, device_configuration);
    return_code = COAP_MSG_CODE_RESPONSE_CHANGED;

send_response:
    coap_service_response_send(coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_OCTET_STREAM, response_ptr, ptr - response_ptr);
    ns_dyn_mem_free(response_ptr);
    return 0;
}

static int thread_management_server_active_get_respond(uint8_t interface_id, int8_t coap_service_id, sn_coap_hdr_s *request_ptr)
{
    link_configuration_s *link_configuration;
    uint8_t *request_tlv_ptr = NULL;
    uint8_t *request_tlv_copy = NULL;
    uint16_t request_tlv_len;
    uint8_t *ptr = NULL;
    uint8_t *payload_ptr = NULL;
    uint8_t *response_ptr = NULL;
    uint8_t error_msg[3];
    int response_len;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_RESPONSE_CHANGED;

    link_configuration = thread_joiner_application_get_config(interface_id);
    if (!link_configuration) {
        return -1;
    }

    tr_debug("Recv MGMT_ACTIVE_GET request");

    request_tlv_len = thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_GET, &request_tlv_ptr);

    if (!request_tlv_len) {
        request_tlv_copy = request_tlv_ptr = thread_joiner_application_active_config_tlv_list_get(interface_id, &request_tlv_len);
    }

    if (!request_tlv_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }

    if (request_tlv_len && !(link_configuration->securityPolicy & SECURITY_POLICY_OUT_OF_BAND_COMMISSIONING_ALLOWED)) {
        request_tlv_len = thread_meshcop_tlv_list_remove(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_NETWORK_MASTER_KEY);
        request_tlv_len = thread_meshcop_tlv_list_remove(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_SECURITY_POLICY);
    }

    if (!request_tlv_len) {
        goto send_response;
    }

    response_len = thread_joiner_application_active_config_length(interface_id, request_tlv_ptr, request_tlv_len, NULL, 0);

    payload_ptr = ptr = error_msg;
    if (response_len < 1) {
        //Error in message is responded with Thread status or if we have access rights problem
        goto send_response;
    }
    payload_ptr = ptr = response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }
    memset(response_ptr, 0, response_len);

    ptr = thread_joiner_application_active_config_write(interface_id, ptr, request_tlv_ptr, request_tlv_len, NULL, 0);

send_response:
    coap_service_response_send(coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr);
    ns_dyn_mem_free(response_ptr);
    ns_dyn_mem_free(request_tlv_copy);
    return 0;
}

static int thread_management_server_pending_get_respond(int8_t interface_id, int8_t coap_service_id, sn_coap_hdr_s *request_ptr)
{
    link_configuration_s *link_configuration;
    uint8_t *request_tlv_ptr = NULL;
    uint8_t *request_tlv_copy = NULL;
    uint16_t request_tlv_len;
    uint8_t *ptr;
    uint8_t *payload_ptr;
    uint8_t *response_ptr = NULL;
    uint8_t error_msg[3];
    int response_len;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_RESPONSE_CHANGED;

    tr_debug("Recv MGMT_PENDING_GET request");
    payload_ptr = ptr = error_msg;
    link_configuration = thread_joiner_application_get_config(interface_id);

    if (!link_configuration) {
        return_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto send_response;
    }
    if (!thread_joiner_application_pending_config_exists(interface_id)) {
        goto send_response;
    }

    request_tlv_len = thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_GET, &request_tlv_ptr);

    if (!request_tlv_len) {
        request_tlv_copy = request_tlv_ptr = thread_joiner_application_pending_config_tlv_list_get(interface_id, &request_tlv_len);
    }

    if (!request_tlv_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }

    if (request_tlv_len && !(link_configuration->securityPolicy & SECURITY_POLICY_OUT_OF_BAND_COMMISSIONING_ALLOWED)) {
        request_tlv_len = thread_meshcop_tlv_list_remove(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_NETWORK_MASTER_KEY);
        request_tlv_len = thread_meshcop_tlv_list_remove(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_SECURITY_POLICY);
    }

    if (!request_tlv_len) {
        goto send_response;
    }

    response_len = thread_joiner_application_pending_config_length(interface_id, request_tlv_ptr, request_tlv_len, NULL, 0);

    payload_ptr = ptr = response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }
    memset(response_ptr, 0, response_len);

    ptr = thread_joiner_application_pending_config_build(interface_id, ptr, request_tlv_ptr, request_tlv_len, NULL, 0);

send_response:
    coap_service_response_send(coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr);
    ns_dyn_mem_free(response_ptr);
    ns_dyn_mem_free(request_tlv_copy);
    return 0;
}

/**
 * Thread management GET command callback.
 * Handle messages:
 * -MGMT_GET (uri = /c/mg),
 * -MGMT_ACTIVE_GET (uri = /c/ag),
 * -MGMT_PENDING_GET (uri = /c/pg)
 */
static int thread_management_server_get_command_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_port;
    (void) source_address;

    thread_management_server_t *this = thread_management_find_by_service(service_id);

    if (!this) {
        return -1;
    }

    return thread_management_server_tmf_get_request_handler(this->interface_id, service_id, request_ptr);

}

static int thread_management_server_commissioner_get_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;
    protocol_interface_info_entry_t *cur;
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    uint8_t response_msg[2 + 2 + 2 + 2 + 2 + 16 + 2 + 2];
    uint8_t *request_tlv_ptr = NULL;
    uint16_t request_tlv_len;
    uint8_t *ptr;
    uint8_t *payload_ptr = NULL;
    if (!this) {
        return -1;
    }
    tr_debug("Thread management commission get request");
    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }
    payload_ptr = ptr = response_msg;

    if (!cur->thread_info->registered_commissioner.commissioner_valid) {
        //Error in message is responded with Thread status or if we have access rights problem
        tr_warn("No registered commissioner");
        ptr = thread_tmfcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
        goto send_response;
    }
    uint16_t border_router_locator = common_read_16_bit(&cur->thread_info->registered_commissioner.border_router_address[14]);
    request_tlv_len = thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_GET, &request_tlv_ptr);

    if (tlv_is_requested(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_BORDER_ROUTER_LOCATOR)) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_BORDER_ROUTER_LOCATOR, border_router_locator);
    }

    if (tlv_is_requested(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID)) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, cur->thread_info->registered_commissioner.session_id);
    }

    if (tlv_is_requested(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_STEERING_DATA)) {
        ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_STEERING_DATA, cur->thread_info->registered_commissioner.steering_data_len, cur->thread_info->registered_commissioner.steering_data);
    }

    if (payload_ptr == ptr) {
        tr_warn("No TLVs found");
        ptr = thread_tmfcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
        goto send_response;
    }
send_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr);
    return 0;
}

#ifdef HAVE_THREAD_V2

static int thread_management_server_mtd_dua_ntf_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    (void) service_id;
    (void) source_address;
    (void) source_port;
    (void) response_ptr;
    uint16_t addr_len;
    uint8_t *addr_data_ptr;
    uint8_t dua_status = THREAD_ST_DUA_SUCCESS;
    tr_debug("Thread MTD n/dn callback");

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_client_get_interface_id_by_service_id(service_id));
    if (!cur) {
        return -1;
    }
    addr_len = thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &addr_data_ptr);

    if (addr_len < 16) {
        tr_warn("Invalid target eid in DUA.ntf cb message");
        return -2;
    }

    thread_tmfcop_tlv_data_get_uint8(response_ptr->payload_ptr, response_ptr->payload_len, TMFCOP_TLV_STATUS, &dua_status);

    if (dua_status == THREAD_ST_DUA_DUPLICATE) {
        // generate new dua address
        cur->dad_failures++;
        thread_bootstrap_dua_address_generate(cur, addr_data_ptr, 64);
    }

    return 0;
}

static void thread_management_server_reset_timeout_cb(void *arg)
{
    protocol_interface_info_entry_t *cur = (protocol_interface_info_entry_t *)arg;

    if (!cur) {
        return;
    }

    // Delete all domain stuff and start discovery.
    thread_ccm_network_certificate_set(cur, NULL, 0);
    thread_ccm_network_private_key_set(cur, NULL, 0);
    thread_nvm_store_mleid_rloc_map_remove();
    thread_nvm_store_link_info_clear();
    thread_joiner_application_link_configuration_delete(cur->id);
    thread_bootstrap_connection_error(cur->id, CON_ERROR_NETWORK_KICK, NULL);
}

/*
 * Commissioner requests to leave this domain
*/
static int thread_management_server_reset_req_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    uint8_t payload[3] = {0};
    uint8_t *ptr = payload;
    uint8_t *signature_ptr = NULL;
    uint16_t session_id = 0;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_server_interface_id_get(service_id));

    if (!cur || !request_ptr) {
        return -1;
    }

    tr_debug("Received MGMT_RESET.req");

    // Verify request TLV's: Commissioner Session ID TLV -  Commissioner Token TLV (optional) - Commissioner Signature TLV
    thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id);
    thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMM_SIGNATURE, &signature_ptr);

    if ((session_id != thread_info(cur)->registered_commissioner.session_id) /*|| (signature_ptr == NULL)*/) {  // todo: signature may not come in this early phase of implementors
        tr_debug("Request parse failed");
        ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
    }

    // Downgrade if router
    if (thread_am_router(cur)) {
        thread_bootstrap_attached_downgrade_router(cur);
    }

    // Delete all data and start reattach
    // Get some time to send response and keep the Commissioner happy
    cur->thread_info->ccm_info->reset_timeout = eventOS_timeout_ms(thread_management_server_reset_timeout_cb, 5000, cur);

    // Send response
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_NONE, payload, ptr - payload);

    return 0;
}


/*
 * Commissioner requests to get new certificate from Registrar, but to stay in the same domain
 */
static int thread_management_server_reenroll_req_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    uint8_t pbbr_addr[16] = {0};
    uint8_t status_tlv[3] = {0};
    uint8_t *ptr = status_tlv;
    uint8_t *signature_ptr = NULL;
    uint16_t session_id = 0;
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_server_interface_id_get(service_id));

    if (!cur || !request_ptr) {
        return -1;
    }

    tr_debug("Received MGMT_REENROLL.req");

    // Verify request TLV's: Commissioner Session ID TLV -  Commissioner Token TLV (optional) - Commissioner Signature TLV
    thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id);
    thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMM_SIGNATURE, &signature_ptr);

    if ((session_id != thread_info(cur)->registered_commissioner.session_id) ||/* (signature_ptr == NULL) || */
            thread_common_primary_bbr_get(cur, pbbr_addr, NULL, NULL, NULL)) {
        tr_debug("Request parse failed");
        ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 0xff);
        goto send_response;
    }

    thread_ccm_reenrollment_start(cur, service_id, pbbr_addr);

send_response:

    // send response
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_NONE, status_tlv, ptr - status_tlv);
    return 0;
}
static void thread_management_server_ccm_register(int8_t interface_id, int8_t coap_service_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info->ccm_info) {
        return;
    }
    cur->thread_info->ccm_info->coap_service_id = coap_service_id;
    coap_service_register_uri(thread_info(cur)->ccm_info->coap_service_id, THREAD_URI_RESET_REQ, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_reset_req_cb);
    coap_service_register_uri(thread_info(cur)->ccm_info->coap_service_id, THREAD_URI_REENROLL_REQ, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_reenroll_req_cb);
}
static void thread_management_server_mtd_service_register(protocol_interface_info_entry_t *cur)
{
    coap_service_register_uri(thread_management_server_service_id_get(cur->id), THREAD_URI_BBR_DOMAIN_ADDRESS_NOTIFICATION, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_mtd_dua_ntf_cb);
}

#ifdef HAVE_THREAD_ROUTER

static int thread_management_server_ccm_relay_tx_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    ns_address_t destination_address = { .address = { 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    uint8_t *udp_data_ptr;
    uint16_t udp_data_len;
    uint8_t *iid_ptr;
    uint8_t iid_len;
    uint16_t port;
    uint8_t port_len;
    int8_t socket_id;
    (void)source_address;
    (void)source_port;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(thread_management_server_interface_id_get(service_id));
    if (!cur) {
        return -1;
    }
    iid_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_IID, &iid_ptr);
    port_len = thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_UDP_PORT, &port);
    udp_data_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ENCAPSULATION, &udp_data_ptr);
    // unwrap message and send to joiner socket.
    if (8 > iid_len || 2 > port_len || 0 == udp_data_len) {
        tr_err("Relay TX invalid message iid:%d, port:%d data_len:%d", iid_len, port_len, udp_data_len);
        return -1;
    }
    if (strncmp(THREAD_URI_BBR_NMK_TX_NTF, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        socket_id = cur->thread_info->ccm_info->listen_socket_nmkp;
    } else if (strncmp(THREAD_URI_BBR_TRI_TX_NTF, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        socket_id = cur->thread_info->ccm_info->listen_socket_ae;
    } else {
        return -1;
    }

    memcpy(&destination_address.address[8], iid_ptr, 8);
    destination_address.identifier = port;
    destination_address.type = ADDRESS_IPV6;
    int8_t err = socket_sendto(socket_id, &destination_address, udp_data_ptr, udp_data_len);
    if (err < 0) {
        tr_err("Relay TX sendto failed %d", err);
    }

    tr_debug("Relay TX sendto addr:%s port:%d, length:%d", trace_ipv6(destination_address.address), port, udp_data_len);

    return -1; // OK no response sent
}

static void thread_management_server_relay_socket_cb(void *cb_res)
{
    socket_callback_t *sckt_data = 0;
    ns_address_t source_address;
    uint8_t relay_destination_address[16];
    uint8_t *data_ptr = NULL;
    uint16_t data_len = 0;
    uint8_t *ptr;
    uint8_t *payload_ptr;
    uint16_t payload_len;
    char *destination_uri_ptr = THREAD_URI_RELAY_RECEIVE;
    sckt_data = cb_res;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(sckt_data->interface_id);

    if (!cur) {
        return;
    }
    if (sckt_data->socket_id == cur->thread_info->ccm_info->listen_socket_nmkp) {
        destination_uri_ptr = THREAD_URI_BBR_NMK_RX_NTF;
    } else if (sckt_data->socket_id == cur->thread_info->ccm_info->listen_socket_ae) {
        destination_uri_ptr = THREAD_URI_BBR_TRI_RX_NTF;
    } else {
        return;
    }


    if (sckt_data->event_type == SOCKET_DATA && sckt_data->d_len > 0) {
        data_ptr = ns_dyn_mem_alloc(sckt_data->d_len);
        if (!data_ptr) {
            tr_err("Out of memory");
            return;
        }
        data_len = socket_read(sckt_data->socket_id, &source_address, data_ptr, sckt_data->d_len);
    }
    if (!data_ptr || data_len < 1) {
        tr_err("No data received");
        return;
    }

    thread_addr_write_mesh_local_16(relay_destination_address, 0xfc38, cur->thread_info);

    payload_len = 4 + data_len + 4 + 8 + 4 + 2 + 4 + 2; //Joiner DTLS Encapsulation TLV  Joiner UDP Port TLV Joiner IID TLV Joiner Router Locator TLV
    payload_ptr = ns_dyn_mem_alloc(payload_len);

    if (!payload_ptr) {
        tr_err("Out of memory");
        ns_dyn_mem_free(data_ptr);
        return;
    }
    tr_debug("Relay TX recvfrom addr: %s, port:%d len:%d", trace_ipv6(source_address.address), source_address.identifier, data_len);
    thci_trace("joinerrouterJoinerDataRelayedInbound");
    ptr = payload_ptr;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ENCAPSULATION, data_len, data_ptr);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_IID, 8, &source_address.address[8]);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_UDP_PORT, source_address.identifier);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, mac_helper_mac16_address_get(cur));

    coap_service_request_send(cur->thread_info->ccm_info->coap_service_id, COAP_REQUEST_OPTIONS_NONE, relay_destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, destination_uri_ptr, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);
    ns_dyn_mem_free(payload_ptr);
    ns_dyn_mem_free(data_ptr);
}

static void thread_management_server_joiner_router_ae_deinit(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->ccm_info->relay_port_ae > 0) {
        tr_debug("deinit AE");
        coap_service_unregister_uri(cur->thread_info->ccm_info->coap_service_id, THREAD_URI_BBR_TRI_TX_NTF);
        socket_close(cur->thread_info->ccm_info->listen_socket_ae);
        cur->thread_info->ccm_info->listen_socket_ae = -1;
        cur->thread_info->ccm_info->relay_port_ae = 0;
    }
}

static void thread_management_server_joiner_router_nmkp_deinit(protocol_interface_info_entry_t *cur)
{
    if (cur->thread_info->ccm_info->relay_port_nmkp > 0) {
        tr_debug("deinit NMKP");
        coap_service_unregister_uri(cur->thread_info->ccm_info->coap_service_id, THREAD_URI_BBR_NMK_TX_NTF);
        socket_close(cur->thread_info->ccm_info->listen_socket_nmkp);
        cur->thread_info->ccm_info->listen_socket_nmkp = -1;
        cur->thread_info->ccm_info->relay_port_nmkp = 0;
    }
}

static int thread_management_server_ccm_joiner_router_init(protocol_interface_info_entry_t *cur)
{

    int8_t securityLinkLayer = 0;

    if (!cur->thread_info->ccm_info || thread_info(cur)->version < THREAD_VERSION_1_2) {
        return -1;
    }

    if (0 != thread_common_primary_bbr_get(cur, NULL, NULL, NULL, NULL)) {
        // Need to disable Joiner router either because port changed or moving to disabled
        thread_management_server_joiner_router_ae_deinit(cur);
        thread_management_server_joiner_router_nmkp_deinit(cur);
        // Joiner router should be disabled
        return 0;
    }

    // Is this a CCM network?
    uint16_t securityPolicy = thread_joiner_application_security_policy_get(cur->id);
    if (securityPolicy & THREAD_SECURITY_POLICY_CCM_DISABLED) {
        // Not a CCM network, de-initialize
        thread_management_server_joiner_router_ae_deinit(cur);
        thread_management_server_joiner_router_nmkp_deinit(cur);
        return 0;
    }

    if (thread_ccm_network_certificate_available(cur) == false) {
        // No domain certificate available
        return 0;
    }

    if (!(securityPolicy & THREAD_SECURITY_POLICY_AE_DISABLED)) {
        if (cur->thread_info->ccm_info->listen_socket_ae < 0) {
            // Start AE relay
            cur->thread_info->ccm_info->listen_socket_ae = socket_open(SOCKET_UDP, THREAD_DEFAULT_AE_PORT, thread_management_server_relay_socket_cb);
            if (cur->thread_info->ccm_info->listen_socket_ae >= 0) {
                cur->thread_info->ccm_info->relay_port_ae = THREAD_DEFAULT_AE_PORT;
                socket_setsockopt(cur->thread_info->ccm_info->listen_socket_ae, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
                // The regular TX is usable from joiner router, because it is stateless, but it neds to be forced on
                coap_service_register_uri(cur->thread_info->ccm_info->coap_service_id, THREAD_URI_BBR_TRI_TX_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_ccm_relay_tx_cb);
            } else {
                tr_warn("Joiner AE failed");
                cur->thread_info->ccm_info->relay_port_ae = 0;
            }
        }
    } else {
        thread_management_server_joiner_router_ae_deinit(cur);
    }

    if (!(securityPolicy & THREAD_SECURITY_POLICY_NMP_DISABLED)) {
        if (cur->thread_info->ccm_info->listen_socket_nmkp < 0) {
            // Start nmkp relay
            cur->thread_info->ccm_info->listen_socket_nmkp = socket_open(SOCKET_UDP, THREAD_DEFAULT_NMKP_PORT, thread_management_server_relay_socket_cb);
            if (cur->thread_info->ccm_info->listen_socket_nmkp >= 0) {
                cur->thread_info->ccm_info->relay_port_nmkp = THREAD_DEFAULT_NMKP_PORT;
                socket_setsockopt(cur->thread_info->ccm_info->listen_socket_nmkp, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
                // The regular TX is usable from joiner router, because it is stateless, but it neds to be forced on
                coap_service_register_uri(cur->thread_info->ccm_info->coap_service_id, THREAD_URI_BBR_NMK_TX_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_ccm_relay_tx_cb);
            } else {
                tr_warn("Joiner NMKP failed");
                cur->thread_info->ccm_info->relay_port_nmkp = 0;
            }
        }
    } else {
        thread_management_server_joiner_router_nmkp_deinit(cur);
    }

    tr_info("init commercial joiner router ae:%d nmkp:%d", cur->thread_info->ccm_info->relay_port_ae, cur->thread_info->ccm_info->relay_port_nmkp);

    return 0;
}
#endif // HAVE_THREAD_ROUTER

#else
#define thread_management_server_ccm_register(interface_id, coap_service_id) ((void)0)
#endif // HAVE_THREAD_V2

#ifdef HAVE_THREAD_V2
/* Public APIs */

int thread_management_server_ccm_service_init(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur || !cur->thread_info->ccm_info || !cur->thread_info->ccm_info->coap_service_id) {
        return -1;
    }

    if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE ||
            cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE) {
        thread_management_server_mtd_service_register(cur);
#ifdef HAVE_THREAD_ROUTER
    } else if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER) {
        thread_management_server_ccm_joiner_router_init(cur);
#endif
    }

    return 0;
}
#endif // HAVE_THREAD_V2

static int thread_start_mac_with_link_configuration(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration)
{
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));
    /*Enable RF interface */
    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
    } else {
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, false);
    }

    mac_helper_default_security_level_set(cur, SEC_ENC_MIC32);
    mac_helper_default_security_key_id_mode_set(cur, MAC_KEY_ID_MODE_IDX);

    cur->mac_parameters->mac_channel = linkConfiguration->rfChannel;
    cur->mac_parameters->pan_id = linkConfiguration->panId;
    cur->mac_parameters->mac_channel = linkConfiguration->rfChannel;

    start_req.PANId = linkConfiguration->panId;
    start_req.LogicalChannel = linkConfiguration->rfChannel;
    start_req.ChannelPage = 0;
    start_req.BeaconOrder = 0x0f;
    start_req.SuperframeOrder = 0x0f;

    cur->interface_mode = INTERFACE_UP;
    thread_discovery_responser_enable(cur->id, false);
    if (cur->mac_api) {
        cur->mac_api->mlme_req(cur->mac_api, MLME_START, (void *)&start_req);
    }
    if (cur->thread_info->sleepy_host_poll_time != 0) {
        mac_data_poll_host_mode_set(cur, NET_HOST_SLOW_POLL_MODE, cur->thread_info->sleepy_host_poll_time);
    } else {
        mac_data_poll_init(cur);
    }

    return 0;
}


static void thread_panid_conflict_timeout_cb(void *arg)
{
    uint8_t payload[12];// 2+6 + 2+2
    uint8_t *ptr;
    thread_management_server_t *this = arg;
    if (!this || !this->scan_ptr) {
        tr_error("panid conflict scan ptr missing");
        return;
    }

    this->scan_ptr->timer = NULL;

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_PANID, this->scan_ptr->panid);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_CHANNEL_MASK, this->scan_ptr->channel_mask_len, this->scan_ptr->channel_mask);
    //Send pan id conflict coap
    coap_service_request_send(this->scan_ptr->coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->scan_ptr->address, this->scan_ptr->port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_PANID_CONFLICT, COAP_CT_OCTET_STREAM, payload, ptr - payload, NULL);
    ns_dyn_mem_free(this->scan_ptr);
    this->scan_ptr = NULL;
}

//style = 0 means thread style, style = anything else means zigbee style
// for thread style the bit is set from left to right and for zigbee style it is set from right to left
static void set_channel_mask(uint8_t *channel_mask, uint8_t channel_number, uint8_t style)
{
    uint8_t byte_position;
    uint8_t bit_position;
    if (0 == style) {
        byte_position = channel_number / 8;
        bit_position = 7 - (channel_number % 8);
        channel_mask[byte_position + 2] |= (1 << bit_position);
        return;
    } else {
        byte_position = 3 - (channel_number / 8);
        bit_position = channel_number % 8;
        channel_mask[byte_position + 2] |= (1 << bit_position);
        return;
    }
}
static uint32_t reverse_bits(uint32_t num)
{
    uint32_t NO_OF_BITS = sizeof(num) * 8;
    uint32_t reversed_value = 0, i, temp;
    for (i = 0; i < NO_OF_BITS; i++) {
        temp = (num & (1 << i));
        if (temp) {
            reversed_value |= (1 << ((NO_OF_BITS - 1) - i));
        }
    }
    return reversed_value;
}
static void thread_panid_scan_response(int8_t if_id, const mlme_scan_conf_t *conf)
{
    bool conflict_occured = false;
    nwk_scan_params_t *scan_parameters_ptr;
    nwk_pan_descriptor_t *result;
    protocol_interface_info_entry_t *interface;
    link_configuration_s *linkConfiguration;

    if (conf->ScanType != MAC_ACTIVE_SCAN) {
        tr_error("Not active scan");
        return;
    }

    interface = protocol_stack_interface_info_get_by_id(if_id);
    if (!interface) {
        tr_error("Mac scan confirm:Unknow Interface");
        return;
    }

    linkConfiguration = thread_joiner_application_get_config(if_id);
    if (!linkConfiguration) {
        return;
    }

    scan_parameters_ptr = &interface->mac_parameters->nwk_scan_params; //mac_mlme_get_scan_params(interface);
    if (!scan_parameters_ptr || !scan_parameters_ptr->nwk_response_info || !conf->ResultListSize) {
        tr_debug("Mac scan confirm:No Beacons");
        thread_start_mac_with_link_configuration(interface, linkConfiguration);
        return;
    }
    scan_parameters_ptr->active_scan_active = false;

    thread_management_server_t *this = thread_management_server_find(if_id);

    if (!this) {
        return;
    }

    result = scan_parameters_ptr->nwk_response_info;
    // reset all channel masks
    this->scan_ptr->channel_mask[2] = 0x00;
    this->scan_ptr->channel_mask[3] = 0x00;
    this->scan_ptr->channel_mask[4] = 0x00;
    this->scan_ptr->channel_mask[5] = 0x00;
    do {
        tr_debug("Mac scan confirm:scanning results");
        if (result->pan_descriptor->CoordPANId == this->scan_ptr->panid) { //if pan id matches then send a conflict message
            tr_debug("Same pan id was found on channel %d", result->pan_descriptor->LogicalChannel);
            set_channel_mask(this->scan_ptr->channel_mask, result->pan_descriptor->LogicalChannel, 0);
            conflict_occured = true;
        }
        result = result->next;
    } while (result);
    if (conflict_occured) {
        tr_debug("conflict occured");
        this->scan_ptr->timer = eventOS_timeout_ms(thread_panid_conflict_timeout_cb, 2000, this);
    }
    thread_start_mac_with_link_configuration(interface, linkConfiguration);
    //TODO if no conflict scan again after delay seconds
}
static int thread_management_server_energy_scan_response_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    // Dummy response handler needed as otherwise retransmissions dont work in coap service.
    (void)service_id;
    (void)source_address;
    (void)source_port;
    (void)response_ptr;
    return 0;
}

static void thread_energy_scan_coap(thread_management_server_t *arg)
{
    uint8_t *ptr;
    thread_management_server_t *this = arg;
    link_configuration_s *linkConfiguration;
    protocol_interface_info_entry_t *interface;

    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if (!linkConfiguration) {
        return;
    }

    interface = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!interface) {
        tr_error("Mac scan confirm:Unknow Interface");
        return;
    }

    thread_start_mac_with_link_configuration(interface, linkConfiguration);
    uint8_t channel_count = thread_channels_to_be_scanned(this->scan_ptr->channel_mask);
    tr_debug("energy scan result mask %s, result %s, count %d", trace_array(this->scan_ptr->channel_mask, 6),
             trace_array(this->scan_ptr->energy_list_ptr, channel_count * this->scan_ptr->count), channel_count);


    uint8_t *payload_ptr = ns_dyn_mem_alloc(2 + 6 + 2 + channel_count * this->scan_ptr->count);
    if (!payload_ptr) {
        tr_error("out of resources");
        return;
    }
    ptr = payload_ptr;

    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_CHANNEL_MASK, this->scan_ptr->channel_mask_len, this->scan_ptr->channel_mask);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_ENERGY_LIST, channel_count * this->scan_ptr->count, this->scan_ptr->energy_list_ptr);

    coap_service_request_send(this->scan_ptr->coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->scan_ptr->address, this->scan_ptr->port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ED_REPORT, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, thread_management_server_energy_scan_response_resp_cb);
    ns_dyn_mem_free(payload_ptr);
    ns_dyn_mem_free(this->scan_ptr->energy_list_ptr);
    ns_dyn_mem_free(this->scan_ptr);
    this->scan_ptr = NULL;
}

static void energy_scan_confirm_cb(int8_t if_id, const mlme_scan_conf_t *conf)
{
    if (conf->ScanType != MAC_ED_SCAN_TYPE) {
        tr_error("Not energy scan");
        return;
    }

    if (conf->ResultListSize < 1) {
        tr_error("No scan responses");
        return;
    }

    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(if_id);
    if (!interface) {
        tr_error("could not get interface");
        return;
    }

    thread_management_server_t *this = thread_management_server_find(if_id);
    if (!this) {
        tr_error("could not find thread management server");
        return;
    }
    // reduce the scan_count by one since one scan has been performed and results are also obtained
    this->scan_ptr->scan_count--;

    for (int i = 0; i < conf->ResultListSize; i++) {
        *this->scan_ptr->energy_list_ptr++ = conf->ED_values[i];
    }

    link_configuration_s *linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if (!linkConfiguration) {
        return;
    }

    //the energy list has now the response_size set of responses so increase the length of energy_list_length
    this->scan_ptr->energy_list_length += conf->ResultListSize;

    // if all scans have been completed then, move the energy_list_ptr back to the beginning
    if (this->scan_ptr->scan_count == 0) {
        this->scan_ptr->energy_list_ptr -= this->scan_ptr->energy_list_length;
        thread_energy_scan_coap(this);
    } else {
        // if all scans have not been completed, enable RF, wait for scan period and call energy scan method again
        thread_start_mac_with_link_configuration(interface, linkConfiguration);
        if (this->scan_ptr->timer) {
            eventOS_timeout_cancel(this->scan_ptr->timer);
        }
        this->scan_ptr->timer = eventOS_timeout_ms(thread_energy_scan_timeout_cb, this->scan_ptr->period, this);
    }
}

void thread_energy_scan_timeout_cb(void *arg)
{
    link_configuration_s *linkConfiguration;
    thread_management_server_t *this = arg;
    if (!this || !this->scan_ptr || !this->scan_ptr->energy_list_ptr) {
        tr_error("Invalid query");
        return;
    }

    this->scan_ptr->timer = NULL;

    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if (!linkConfiguration) {
        return;
    }

    protocol_interface_info_entry_t *s;
    s = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!s) {
        return;
    }

    uint32_t channel_mask = 0;
    channel_mask = (this->scan_ptr->channel_mask[2] << 24) | (this->scan_ptr->channel_mask[3] << 16) | (this->scan_ptr->channel_mask[4] << 8) | (this->scan_ptr->channel_mask[5]);
    //Modify reversed_mask after the right way to interpret channel mask is obtained
    uint32_t reversed_mask = reverse_bits(channel_mask);
    channel_mask = reversed_mask;
    s->mac_parameters->nwk_scan_params.stack_chan_list.channel_mask[0] = channel_mask;
    //Channel page is 0 for thread
    s->mac_parameters->nwk_scan_params.stack_chan_list.channel_page = CHANNEL_PAGE_0;

    // Convert duration in ms to MAC exponent value
    uint8_t duration_n;
    if (this->scan_ptr->duration <= (CHANNEL_PAGE_0_SUPERFRAME_DURATION * 2)) {
        duration_n = 0;
    } else {
        duration_n = thread_log2_aprx((this->scan_ptr->duration / CHANNEL_PAGE_0_SUPERFRAME_DURATION) - 1);
    }

    // 10 == 15.7s
    // 11 == 31.5s
    // 12 == 62.9s
    // 13 == 125.8s (maximum when duration is 65535 and result of log2 is rounded up)
    // 14 == 251.6s (not possible)
    tr_debug("Start Energy scan duration:%d", duration_n);
    mac_data_poll_disable(s);
    mlme_scan_t req;
    mac_create_scan_request(MAC_ED_SCAN_TYPE, &s->mac_parameters->nwk_scan_params.stack_chan_list, duration_n, &req);
    if (s->mac_api) {
        s->scan_cb = energy_scan_confirm_cb;
        s->mac_api->mlme_req(s->mac_api, MLME_SCAN, &req);
    }
}


static void thread_panid_scan_timeout_cb(void *arg)
{
    thread_management_server_t *this = arg;
    if (!this || !this->scan_ptr) {
        return;
    }

    this->scan_ptr->timer = NULL;


    protocol_interface_info_entry_t *s;
    s = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!s || !s->mac_api) {
        return;
    }

    uint32_t channel_mask = 0;
    channel_mask = (this->scan_ptr->channel_mask[2] << 24) | (this->scan_ptr->channel_mask[3] << 16) | (this->scan_ptr->channel_mask[4] << 8) | (this->scan_ptr->channel_mask[5]);
    //Modify reversed_mask after the right way to interpret channel mask is obtained
    uint32_t reversed_mask = reverse_bits(channel_mask);
    channel_mask = reversed_mask;
    s->mac_parameters->nwk_scan_params.stack_chan_list.channel_mask[0] = channel_mask;
    //Channel page is 0 for thread
    s->mac_parameters->nwk_scan_params.stack_chan_list.channel_page = CHANNEL_PAGE_0;
    mac_data_poll_disable(s);
    mlme_scan_t req;
    mac_create_scan_request(MAC_ACTIVE_SCAN, &s->mac_parameters->nwk_scan_params.stack_chan_list, 5, &req);
    /*
    Before commencing an active or passive scan, the MAC sub-layer shall store the value of macPANId and
    then set it to 0xffff for the duration of the scan. This enables the receive filter to accept all beacons rather
    than just the beacons from its current PAN, as described in 5.1.6.2. On completion of the scan, the MAC
    sub-layer shall restore the value of macPANId to the value stored before the scan began.
     */
    mac_helper_panid_set(s, 0xffff);

    s->scan_cb = thread_panid_scan_response;
    s->mac_parameters->nwk_scan_params.active_scan_active = true;
    s->mac_api->mlme_req(s->mac_api, MLME_SCAN, &req);
}


/**
 * Thread PANID scan request
 */
static int thread_management_server_panid_query_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    uint16_t session_id;
    uint16_t panid;
    uint8_t *mask_ptr;
    uint8_t mask_len;

    if (!this) {
        return -1;
    }
    tr_debug("thread management panid query");
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        tr_warn("Missing Session id TLV");
        goto error_exit;
    }
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PANID, &panid)) {
        tr_warn("Missing PANID TLV");
        goto error_exit;
    }
    mask_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL_MASK, &mask_ptr);
    if (mask_len < 6) {
        tr_warn("Missing channel mask TLV");
        goto error_exit;
    }
    tr_info("PANID TLV %02x, session_id %d, Channel mask TLV %s", panid, session_id, trace_array(mask_ptr, 5));

    if (!this->scan_ptr) {
        this->scan_ptr = ns_dyn_mem_alloc(sizeof(scan_query_t));
        memset(this->scan_ptr, 0, sizeof(scan_query_t));
    } else {
        eventOS_timeout_cancel(this->scan_ptr->timer);
        this->scan_ptr->timer = NULL;
    }
    if (!this->scan_ptr) {
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }
    this->scan_ptr->timer = eventOS_timeout_ms(thread_panid_scan_timeout_cb, 500, this);// Delay for the confirm response message
    if (!this->scan_ptr->timer) {
        ns_dyn_mem_free(this->scan_ptr);
        this->scan_ptr = NULL;
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }

    this->scan_ptr->coap_service_id = service_id;
    memcpy(this->scan_ptr->channel_mask, mask_ptr, mask_len);
    this->scan_ptr->channel_mask_len = mask_len;
    this->scan_ptr->port = source_port;
    memcpy(this->scan_ptr->address, source_address, 16);
    this->scan_ptr->panid = panid;
    this->scan_ptr->panid_scan = true;

    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
        return 0;
    }
    return -1;
error_exit:
    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, NULL, 0);
        return 0;
    }

    return -1;
}
/**
 * Thread PANID scan request
 */
static int thread_management_server_energy_scan_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    uint16_t period;
    uint16_t session_id;
    uint8_t count;
    uint16_t duration;
    uint8_t *mask_ptr;
    uint8_t mask_len;


    if (!this) {
        return -1;
    }
    tr_debug("thread management energy scan");
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PERIOD, &period)) {
        tr_warn("Missing PERIOD TLV");
        goto error_exit;
    }
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        tr_warn("Missing Session id TLV");
        goto error_exit;
    }
    if (1 > thread_meshcop_tlv_data_get_uint8(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COUNT, &count)) {
        tr_warn("Missing COUNT TLV");
        goto error_exit;
    }
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_DURATION, &duration)) {
        tr_warn("Missing DURATION TLV");
        goto error_exit;
    }
    mask_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL_MASK, &mask_ptr);
    if (mask_len < 6) {
        tr_warn("Missing channel mask TLV");
        goto error_exit;
    }
    tr_info("Channel mask TLV %s, period %d, count %d, duration %d", trace_array(mask_ptr, mask_len), period, count, duration);

    if (count < 1  || thread_channel_mask_count(mask_ptr) < 1) {
        // Sanity checks
        response_code = COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE;
        goto error_exit;
    }
    if (!this->scan_ptr) {
        this->scan_ptr = ns_dyn_mem_alloc(sizeof(scan_query_t));
        memset(this->scan_ptr, 0, sizeof(scan_query_t));
    } else {
        eventOS_timeout_cancel(this->scan_ptr->timer);
        this->scan_ptr->timer = NULL;
    }
    if (!this->scan_ptr) {
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }
    uint16_t channel_count = thread_channels_to_be_scanned(mask_ptr);

    this->scan_ptr->energy_list_length = 0;
    this->scan_ptr->scan_count = count;

    // allocate memory for the energy scan results
    this->scan_ptr->energy_list_ptr = ns_dyn_mem_temporary_alloc(channel_count * this->scan_ptr->scan_count);
    if (!this->scan_ptr->energy_list_ptr) {
        response_code = COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE;
        tr_debug("Exiting after no energy list ptr was allocated");
        goto error_exit;
    }

    memset(this->scan_ptr->energy_list_ptr, 0, (channel_count * this->scan_ptr->scan_count));

    this->scan_ptr->timer = eventOS_timeout_ms(thread_energy_scan_timeout_cb, 500, this);

    if (!this->scan_ptr->timer) {
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }

    this->scan_ptr->coap_service_id = service_id;
    memcpy(this->scan_ptr->channel_mask, mask_ptr, mask_len);
    this->scan_ptr->channel_mask_len = mask_len;
    this->scan_ptr->port = source_port;
    memcpy(this->scan_ptr->address, source_address, 16);
    this->scan_ptr->count = count;
    this->scan_ptr->duration = duration;
    this->scan_ptr->period = period;
    this->scan_ptr->energy_scan = true;


    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
        return 0;
    }
    return -1;
error_exit:
    if (this->scan_ptr) {
        ns_dyn_mem_free(this->scan_ptr->energy_list_ptr);
        ns_dyn_mem_free(this->scan_ptr);
    }
    this->scan_ptr = NULL;

    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, NULL, 0);
        return 0;
    }

    return -1;
}

static void thread_announce_timeout_cb(void *arg)
{
    link_configuration_s *linkConfiguration;
    thread_management_server_t *this = arg;
    protocol_interface_info_entry_t *cur;

    if (!this || !this->announce_ptr) {
        return;
    }

    this->announce_ptr->timer = NULL;

    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if (!cur || !linkConfiguration) {
        return;
    }
    while (this->announce_ptr->channel < 27) {
        if (thread_channel_mask_is_channel_set(this->announce_ptr->channel_mask, this->announce_ptr->channel)) {
            break;
        }
        this->announce_ptr->channel++;
    }
    if (this->announce_ptr->channel > 26) {
        tr_debug("Announce done");
        ns_dyn_mem_free(this->announce_ptr);
        this->announce_ptr = NULL;
        return;
    }
    tr_debug("Announce to channel %d", this->announce_ptr->channel);

    thread_bootstrap_announcement_start(cur, this->announce_ptr->channel_mask[0], this->announce_ptr->channel, this->announce_ptr->count, this->announce_ptr->period);

    this->announce_ptr->channel++; // Next time we start the next channel
    this->announce_ptr->timer = eventOS_timeout_ms(thread_announce_timeout_cb, 5000, this);
    if (!this->announce_ptr->timer) {
        tr_warn("Announce failed");
        ns_dyn_mem_free(this->announce_ptr);
        this->announce_ptr = NULL;
        return;
    }
}

static int thread_management_server_announce_begin_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    link_configuration_s *linkConfiguration;
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
    uint16_t period;
    uint16_t session_id;
    uint8_t count;
    uint8_t *mask_ptr;
    uint8_t mask_len;

    (void)source_address;
    (void)source_port;

    if (!this) {
        return -1;
    }
    linkConfiguration = thread_joiner_application_get_config(this->interface_id);
    if (!linkConfiguration) {
        return -1;
    }

    tr_debug("thread management announce begin");
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        tr_warn("Missing Session id TLV");
        goto error_exit;
    }
    if (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_PERIOD, &period)) {
        tr_warn("Missing PERIOD TLV");
        goto error_exit;
    }
    if (1 > thread_meshcop_tlv_data_get_uint8(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_COUNT, &count)) {
        tr_warn("Missing COUNT TLV");
        goto error_exit;
    }
    mask_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_CHANNEL_MASK, &mask_ptr);
    if (mask_len < 6) {
        tr_warn("Missing channel mask TLV");
        goto error_exit;
    }
    // TODO validity checks session id must be validated
    tr_info("start announcing session id %d, mask TLV %s, period %d, count %d", session_id, trace_array(mask_ptr, mask_len), period, count);

    if (!this->announce_ptr) {
        this->announce_ptr = ns_dyn_mem_alloc(sizeof(announce_t));
    } else {
        eventOS_timeout_cancel(this->announce_ptr->timer);
        this->announce_ptr->timer = NULL;
    }
    if (!this->announce_ptr) {
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }
    this->announce_ptr->timer = eventOS_timeout_ms(thread_announce_timeout_cb, 500, this);
    if (!this->announce_ptr->timer) {
        ns_dyn_mem_free(this->announce_ptr);
        response_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto error_exit;
    }
    memcpy(this->announce_ptr->channel_mask, mask_ptr, mask_len);
    this->announce_ptr->channel_mask_len = mask_len;
    this->announce_ptr->count = count;
    this->announce_ptr->period = period;
    this->announce_ptr->channel = 0;
// Set own information to announce
    response_code = COAP_MSG_CODE_RESPONSE_CHANGED;

error_exit:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, NULL, 0);
    return 0;
}

static int coap_msg_prevalidate_cb(int8_t local_interface_id, uint8_t local_address[static 16], uint16_t local_port, int8_t recv_interface_id, uint8_t source_address[static 16], uint16_t source_port, char *coap_uri)
{
    protocol_interface_info_entry_t *cur_local, *cur_source;
    uint_fast8_t addr_scope;

    (void) source_address;
    (void) source_port;
    (void) coap_uri;

    cur_local = protocol_stack_interface_info_get_by_id(local_interface_id);

    if (!cur_local) {
        tr_error("No interface for %d", local_interface_id);
        return -1;
    }

    if (local_port != THREAD_MANAGEMENT_PORT) {
        // Message not sent to THREAD_MANAGEMENT_PORT, let it come through
        tr_deep("Message %s port %d is not mgmt port", coap_uri, local_port);
        return 0;
    }

    // check message source address
    if (!thread_management_server_source_address_check(local_interface_id, source_address)) {
        tr_deep("Drop CoAP msg %s from %s", coap_uri, trace_ipv6(source_address));
        return 3;
    }

    /* check our local address scope */
    addr_scope = addr_ipv6_scope(local_address, cur_local);
    if (addr_scope > IPV6_SCOPE_REALM_LOCAL) {
        tr_deep("Drop CoAP msg %s to %s due %d", coap_uri, trace_ipv6(local_address), addr_scope);
        return 1;
    }

    if (local_interface_id != recv_interface_id) {
        // message received from different interface
        cur_source = protocol_stack_interface_info_get_by_id(recv_interface_id);
        if (!cur_source) {
            tr_deep("No cur for if %d", recv_interface_id);
            return -1;
        }
        addr_scope = addr_ipv6_scope(source_address, cur_source);
        if (addr_scope < IPV6_SCOPE_REALM_LOCAL) {
            tr_deep("Drop CoAP msg %s from %s to %s due %d", coap_uri, trace_ipv6(source_address), trace_ipv6(local_address), addr_scope);
            return 2;
        }
    }

    return 0;
}

/**
 * Public interface functions
 */
int thread_management_server_init(int8_t interface_id)
{

    thread_management_server_t *this = thread_management_server_find(interface_id);
    if (this) {
        return 0;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_management_server_t));
    if (!this) {
        return -2;
    }

    this->joiner_router_enabled = false;
    this->interface_id = interface_id;
    this->listen_socket_joiner = -1;
    this->relay_port_joiner = 0;
    this->scan_ptr = NULL;
    this->announce_ptr = NULL;
    this->join_ent_timer = NULL;
    memset(this->destination_address, 0, 16);
    memset(this->one_time_key, 0, 16);
    this->external_commissioner_port = THREAD_COMMISSIONING_PORT;

#ifdef HAVE_THREAD_ROUTER
    if (thread_border_router_init(this->interface_id) != 0) {
        ns_dyn_mem_free(this);
        return -5;
    }

    if (thread_bbr_init(this->interface_id, this->external_commissioner_port) != 0) {
        ns_dyn_mem_free(this);
        return -5;
    }
#endif
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    if (this->coap_service_id < 0) {
        tr_error("Thread management init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
    coap_service_msg_prevalidate_callback_set(THREAD_MANAGEMENT_PORT, coap_msg_prevalidate_cb);
#ifdef HAVE_THREAD_ROUTER
    if (thread_leader_service_init(interface_id, this->coap_service_id) != 0) {
        tr_error("Thread leader service init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
#endif
    thread_management_server_ccm_register(interface_id, this->coap_service_id);
    // All thread devices
    coap_service_register_uri(this->coap_service_id, THREAD_URI_MANAGEMENT_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_management_server_get_command_cb);

    // Full functioning devices(Router,reed,fed)
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ACTIVE_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_management_server_get_command_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_PENDING_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_management_server_get_command_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_COMMISSIONER_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_management_server_commissioner_get_cb);

    coap_service_register_uri(this->coap_service_id, THREAD_URI_PANID_QUERY, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_panid_query_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_ED_SCAN, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_energy_scan_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_MANAGEMENT_ANNOUNCE_BEGIN, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_announce_begin_cb);

    ns_list_add_to_start(&instance_list, this);
    return 0;
}

void thread_management_server_delete(int8_t interface_id)
{
    thread_management_server_t *this = thread_management_server_find(interface_id);
    if (!this) {
        return;
    }

    thread_leader_service_delete(interface_id);

    if (this->joiner_router_enabled) {
        thread_management_server_joiner_router_deinit(interface_id);
    }
    //TODO move uri deletion to delete
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_MANAGEMENT_GET);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_MANAGEMENT_SET);
    coap_service_delete(this->coap_service_id);

    ns_list_remove(&instance_list, this);
    if (this->announce_ptr) {
        if (this->announce_ptr->timer) {
            eventOS_timeout_cancel(this->announce_ptr->timer);
        }
        ns_dyn_mem_free(this->announce_ptr);
    }
    if (this->scan_ptr) {
        if (this->scan_ptr->timer) {
            eventOS_timeout_cancel(this->scan_ptr->timer);
        }
        if (this->scan_ptr->energy_list_ptr) {
            ns_dyn_mem_free(this->scan_ptr->energy_list_ptr);
        }
        ns_dyn_mem_free(this->scan_ptr);
    }

    ns_dyn_mem_free(this);

    thread_border_router_delete(interface_id);
    thread_bbr_delete(interface_id);
    return;
}

int8_t thread_management_server_service_id_get(int8_t interface_id)
{
    thread_management_server_t *this = thread_management_server_find(interface_id);
    if (!this) {
        return -1;
    }
    return this->coap_service_id;
}

int8_t thread_management_server_interface_id_get(int8_t coap_service_id)
{
    thread_management_server_t *this = thread_management_find_by_service(coap_service_id);
    if (!this) {
        return -1;
    }
    return this->interface_id;
}

#ifdef THREAD_THCI_SUPPORT
static uint8_t Joiner_iid[8];
#endif

static int thread_management_server_entrust_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    (void)source_address;
    (void)source_port;

    if (response_ptr) {
        thci_trace("Device - Joiner Router|Direction - recv|IID - %s|Type - JOIN_ent.resp|Length - %d|Payload - %s", trace_array(Joiner_iid, 8), response_ptr->payload_len, trace_array(response_ptr->payload_ptr, response_ptr->payload_len));
    } else {
        thci_trace("Device - Joiner Router|Direction - recv|IID - %s|Type - JOIN_ent.resp|Payload - NULL", Joiner_iid);
    }
    tr_debug("Joiner entrust response received");
    if (!this) {
        tr_warn("commissioner service missing!");
        return -1;
    }

    thread_commissioning_if_pairwise_key_del(this->interface_id, &source_address[8]);

    return 0;
}
static int thread_management_server_entrust_send(thread_management_server_t *this, uint8_t destination_address[16], uint8_t one_time_key[16])
{
    uint8_t *response_ptr;
    uint16_t response_len;
    uint8_t *ptr;
    link_configuration_s *link_configuration_ptr;
    //int ret;
    if (!this || !destination_address || !one_time_key) {
        tr_err("entrust send fail");
        return -1;
    }

    tr_debug("Joiner_entrust_send Pairwise key %s address %s", trace_array(one_time_key, 16), trace_ipv6(destination_address));

    link_configuration_ptr = thread_management_configuration_get(this->interface_id);
    if (!link_configuration_ptr) {
        tr_err("Entrust sending failed no configuration");
        return -2;
    }
    int ret;
    uint8_t options = COAP_REQUEST_OPTIONS_NONE;
    ret = thread_commissioning_if_pairwise_key_add(this->interface_id, 10000, &destination_address[8], one_time_key);

    if (ret) {
        tr_err("Pairwise key set failed");
        return -3;
    }
    response_len = 6 + thread_joiner_application_active_config_length(this->interface_id, entrust_dataset_tlvs, entrust_dataset_tlvs_size(), NULL, 0);

    ptr = response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        tr_warn("Out of mem");
        return -2;
    }
    ptr = thread_joiner_application_active_config_write(this->interface_id, ptr, entrust_dataset_tlvs, entrust_dataset_tlvs_size(), NULL, 0);
    ptr = thread_meshcop_tlv_data_write_uint32(ptr, MESHCOP_TLV_NETWORK_KEY_SEQUENCE, link_configuration_ptr->key_sequence);

    thci_trace("joinerrouterJoinerAccepted");
    /*We must null out the master secret*/
#ifdef THREAD_THCI_SUPPORT
    uint8_t *master_secret_ptr;
    uint8_t *pskc_ptr;
    if (thread_meshcop_tlv_find(response_ptr, ptr - response_ptr, MESHCOP_TLV_NETWORK_MASTER_KEY, &master_secret_ptr) >= 16) {
        memset(master_secret_ptr, 0, 16);
    }
    if (thread_meshcop_tlv_find(response_ptr, ptr - response_ptr, MESHCOP_TLV_PSKC, &pskc_ptr) >= 16) {
        memset(pskc_ptr, 0, 16);
    }

    memcpy(Joiner_iid, &destination_address[8], 8);
    thci_trace("Device - Joiner Router|Direction - sent|IID - %s|Type - JOIN_ent.req|Length - %d|Payload - %s", trace_array(Joiner_iid, 8), (int)(ptr - response_ptr), trace_array(response_ptr, ptr - response_ptr));
    if (master_secret_ptr) {
        memcpy(master_secret_ptr, link_configuration_ptr->master_key, 16);
    }
    if (pskc_ptr) {
        memcpy(pskc_ptr, link_configuration_ptr->PSKc, 16);
    }
#endif
    coap_service_request_send(this->coap_service_id, options, destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_JOINER_ENTRUST, COAP_CT_OCTET_STREAM, response_ptr, ptr - response_ptr, thread_management_server_entrust_resp_cb);
    ns_dyn_mem_free(response_ptr);
    return 0;
}

static void thread_join_ent_timeout_cb(void *arg)
{
    thread_management_server_t *this = arg;
    if (!this || !this->join_ent_timer) {
        return;
    }

    this->join_ent_timer = NULL;
    thread_management_server_entrust_send(this, this->destination_address, this->one_time_key);
    return;
}

void joiner_router_recv_commission_msg(void *cb_res)
{
    socket_callback_t *sckt_data = 0;
    ns_address_t source_address;
    uint8_t rloc16[16];
    uint8_t relay_destination_address[16];
    uint8_t *data_ptr = NULL;
    uint16_t data_len = 0;
    uint8_t *ptr;
    uint8_t *payload_ptr;
    uint16_t payload_len;
    char *destination_uri_ptr = THREAD_URI_RELAY_RECEIVE;
    sckt_data = cb_res;
    int ret MAYBE_UNUSED = -1;

    thread_management_server_t *this = thread_management_find_by_sckt_id(sckt_data->socket_id);

    if (!this) {
        return;
    }

    if (0 !=  thread_management_get_ml16_address(this->interface_id, rloc16)) {
        return;
    }
    // Specification issue needs hack to make one concurrent relay working
    if (0 != (ret = thread_commissioning_if_border_router_locator_get(this->interface_id, relay_destination_address))) {
        tr_err("invalid commissioner address ret %d", ret);
        return;
    }

    if (sckt_data->event_type == SOCKET_DATA && sckt_data->d_len > 0) {
        data_ptr = ns_dyn_mem_alloc(sckt_data->d_len);
        if (!data_ptr) {
            tr_err("Out of memory");
            return;
        }
        data_len = socket_read(sckt_data->socket_id, &source_address, data_ptr, sckt_data->d_len);
    }
    if (!data_ptr || data_len < 1) {
        tr_err("No data received");
        return;
    }

    payload_len = 4 + data_len + 4 + 8 + 4 + 2 + 4 + 2; //Joiner DTLS Encapsulation TLV  Joiner UDP Port TLV Joiner IID TLV Joiner Router Locator TLV
    payload_ptr = ns_dyn_mem_alloc(payload_len);

    if (!payload_ptr) {
        tr_err("Out of memory");
        ns_dyn_mem_free(data_ptr);
        return;
    }
    tr_debug("Relay TX recvfrom addr: %s, port:%d len:%d", trace_ipv6(source_address.address), source_address.identifier, data_len);
    thci_trace("joinerrouterJoinerDataRelayedInbound");
    ptr = payload_ptr;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ENCAPSULATION, data_len, data_ptr);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_IID, 8, &source_address.address[8]);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_UDP_PORT, source_address.identifier);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, 2, &rloc16[14]);

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, relay_destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, destination_uri_ptr, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);
    ns_dyn_mem_free(payload_ptr);
    ns_dyn_mem_free(data_ptr);
}
static int thread_management_server_relay_tx_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_management_server_t *this = thread_management_find_by_service(service_id);
    ns_address_t destination_address = { .address = { 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
    uint8_t *kek_ptr;
    uint8_t *udp_data_ptr;
    uint16_t udp_data_len;
    uint8_t *iid_ptr;
    uint16_t port;
    (void)source_address;
    (void)source_port;

    if (!this) {
        return -1;
    }

    // unwrap message and send to joiner socket.
    if (8 > thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_IID, &iid_ptr) ||
            2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_UDP_PORT, &port) ||
            0 == (udp_data_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ENCAPSULATION, &udp_data_ptr))
       ) {
        tr_err("Relay TX invalid message");
        return -1;
    }
    memcpy(&destination_address.address[8], iid_ptr, 8);
    destination_address.identifier = port;
    destination_address.type = ADDRESS_IPV6;
    int8_t err = socket_sendto(this->listen_socket_joiner, &destination_address, udp_data_ptr, udp_data_len);
    if (err < 0) {
        tr_err("Relay TX sendto failed %d", err);
    }

    //if KEK present set pairwise key to joiner and send entrust method
    if (0 < thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ROUTER_KEK, &kek_ptr)) {
        // KEK present in relay set pairwise key and send entrust
        tr_debug("Kek received");
        if (this->join_ent_timer) {
            eventOS_timeout_cancel(this->join_ent_timer);
            thread_management_server_entrust_send(this, this->destination_address, this->one_time_key);
        }
        memcpy(this->destination_address, destination_address.address, 16);
        memcpy(this->one_time_key, kek_ptr, 16);
        this->join_ent_timer = eventOS_timeout_ms(thread_join_ent_timeout_cb, THREAD_DELAY_JOIN_ENT, this);
    }
    tr_debug("Relay TX sendto addr:%s port:%d, length:%d", trace_ipv6(destination_address.address), port, udp_data_len);
    thci_trace("joinerrouterJoinerDataRelayedOutbound");

    return -1; // OK no response sent
}

int thread_management_server_joiner_router_init(int8_t interface_id)
{
    thread_management_server_t *this = thread_management_server_find(interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    int8_t securityLinkLayer = 0;
    bool enable = false;

    if (!this || !cur) {
        return -1;
    }

    if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len > 0) {
        // Thread 1.1 commissioner is present
        enable = true;
    }
    if (this->joiner_router_enabled == enable &&
            this->relay_port_joiner == thread_joiner_port) {
        // Joiner router is in correct state
        return 0;
    }
    if (this->joiner_router_enabled) {
        // Need to disable Joiner router either because port changed or moving to disabled
        thread_management_server_joiner_router_deinit(interface_id);
    }
    if (!enable) {
        // Joiner router should be disabled
        tr_info("Joiner router Disable joining");
        return 0;
    }

    tr_info("Joiner router Enable joining");

    if (cur->thread_info->registered_commissioner.commissioner_valid && cur->thread_info->registered_commissioner.steering_data_len > 0) {
        // Thread 1.1 commissioner is present
        this->relay_port_joiner = thread_joiner_port;

        this->listen_socket_joiner = socket_open(SOCKET_UDP, this->relay_port_joiner, joiner_router_recv_commission_msg);
        if (this->listen_socket_joiner < 0) {
            // Try other ports
            while ((this->listen_socket_joiner < 0) && (this->relay_port_joiner < thread_joiner_port + 10)) {
                // We try 10 ports after default port
                this->relay_port_joiner++;
                this->listen_socket_joiner = socket_open(SOCKET_UDP, this->relay_port_joiner, joiner_router_recv_commission_msg);
            }
        }
        if (this->listen_socket_joiner < 0) {
            this->joiner_router_enabled = false;
            this->relay_port_joiner = 0;
            tr_error("Joiner router init failed");
            return -2;
        }
        socket_setsockopt(this->listen_socket_joiner, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &securityLinkLayer, sizeof(int8_t));
        tr_debug("init joiner router port:%d", this->relay_port_joiner);
    }

    coap_service_register_uri(this->coap_service_id, THREAD_URI_RELAY_TRANSMIT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_server_relay_tx_cb);
    this->joiner_router_enabled = true;
    return 0;
}

void thread_management_server_joiner_router_deinit(int8_t interface_id)
{
    thread_management_server_t *this = thread_management_server_find(interface_id);

    if (!this) {
        return;
    }

    tr_debug("deinit joiner router");
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_RELAY_TRANSMIT);
    socket_close(this->listen_socket_joiner);
    this->listen_socket_joiner = -1;
    this->relay_port_joiner = 0;
    this->joiner_router_enabled = false;
    return;
}

int thread_management_server_commisoner_data_get(int8_t interface_id, thread_management_server_data_t *server_data)
{
    thread_management_server_t *this = thread_management_server_find(interface_id);

    if (!this) {
        return -1;
    }

    server_data->joiner_router_enabled = this->joiner_router_enabled;
    server_data->joiner_router_port = this->relay_port_joiner;
    server_data->commissioner_port = this->external_commissioner_port;
    return 0;
}

bool thread_management_server_source_address_check(int8_t interface_id, uint8_t source_address[16])
{
    link_configuration_s *linkConfiguration;

    if (memcmp(ADDR_LINK_LOCAL_PREFIX, source_address, 8) == 0) {
        // Source address is from Link local address
        return true;
    }

    linkConfiguration = thread_joiner_application_get_config(interface_id);
    if (!linkConfiguration) {
        tr_error("No link cfg for if %d", interface_id);
        return false;
    }

    if (memcmp(source_address, linkConfiguration->mesh_local_ula_prefix, 8) == 0 &&
            memcmp(source_address + 8, ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
        // Source address is RLOC or ALOC
    } else if (memcmp(source_address, linkConfiguration->mesh_local_ula_prefix, 8) == 0) {
        // Source address is ML64 TODO this should check that destination address is ALOC or RLOC CoaP Service does not support
    } else {
        tr_deep("Message out of thread network; ML prefix: %s, src addr: %s",
                trace_ipv6_prefix(linkConfiguration->mesh_local_ula_prefix, 64),
                trace_ipv6(source_address));
        return false;
    }

    return true;
}

int thread_management_server_tmf_get_request_handler(int8_t interface_id, int8_t coap_service_id, struct sn_coap_hdr_ *request_ptr)
{
    int ret_val = -1;

    if (strncmp(THREAD_URI_ACTIVE_GET, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        ret_val = thread_management_server_active_get_respond(interface_id, coap_service_id, request_ptr);
    } else if (strncmp(THREAD_URI_PENDING_GET, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        ret_val = thread_management_server_pending_get_respond(interface_id, coap_service_id, request_ptr);
    } else if (strncmp(THREAD_URI_MANAGEMENT_GET, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        ret_val = thread_management_server_management_get_respond(interface_id, coap_service_id, request_ptr);
    } else {
        // unrecognized message
        tr_warn("Unrecognized tmf msg");
    }

    return ret_val;
}

#else // HAVE_THREAD

int thread_management_server_init(int8_t interface_id)
{
    (void) interface_id;
    return 0;
}

void thread_management_server_delete(int8_t interface_id)
{
    (void) interface_id;
}

int thread_management_server_joiner_router_init(int8_t interface_id)
{
    (void)interface_id;
    return 0;
}

void thread_management_server_joiner_router_deinit(int8_t interface_id)
{
    (void) interface_id;
}

int thread_management_server_commisoner_data_get(int8_t interface_id, thread_management_server_data_t *server_data)
{
    (void) interface_id;
    (void) server_data;
    return -1;
}

bool thread_management_server_source_address_check(int8_t interface_id, uint8_t source_address[16])
{
    (void)interface_id;
    (void)source_address;
    return false;
}

int thread_management_server_tmf_get_request_handler(int8_t interface_id, int8_t coap_service_id, struct sn_coap_hdr_ *request_ptr)
{
    (void)interface_id;
    (void)coap_service_id;
    (void)request_ptr;
    return -1;
}
#endif //HAVE_THREAD
