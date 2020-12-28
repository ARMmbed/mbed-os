/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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
#include <ns_types.h>
#include <string.h>
#include <nsdynmemLIB.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "common_functions.h"

#include "thread_config.h"
#include "coap_service_api.h"
#include "thread_bbr_api.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_discovery.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "6LoWPAN/Thread/thread_resolution_client.h"
#include "6LoWPAN/Thread/thread_bbr_commercial.h"
#include "6LoWPAN/Thread/thread_ccm.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"


#if defined(HAVE_THREAD_V2) && defined(HAVE_THREAD_BORDER_ROUTER)
#define TRACE_GROUP "pBBR"

static uint8_t dua_response_status = 0xff;
static uint8_t dua_status_count = 1;
static uint8_t ba_response_status_count = 0;

/*
 * Border router instance data.
 */
typedef struct {
    uint8_t pbbr_multicast_address[16];
    uint8_t tri_address[16];
    uint8_t registrar_address[16];
    uint8_t domain_prefix[8];
    uint32_t mlr_timeout;
    uint32_t delay_timer;
    uint32_t dua_timeout;
    uint32_t dua_timer_ticks;
    uint16_t pbbr_port; /* primary BBR listening port */
    uint16_t tri_port; /* Thread Registrar Interface listening port */
    uint16_t joiner_router_rloc;
    uint8_t sequence_number;
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t coap_nmkp_virtual_service_id;
    int8_t backbone_interface_id;
    int8_t br_bb_service_id;
    bool pbbr_started: 1;
    ns_list_link_t link;
} thread_pbbr_t;

typedef struct {
    int8_t interface_id;
    uint8_t source_address[16];
    uint8_t ml_eid[8];
    uint8_t target_eid[16];
    uint16_t ttl;
    uint8_t dua_dad_repeat;
    ns_list_link_t link;
} duplicate_dua_tr_t;


static NS_LIST_DEFINE(pbbr_instance_list, thread_pbbr_t, link);
static NS_LIST_DEFINE(duplicate_dua_tr_list, duplicate_dua_tr_t, link);

#define THREAD_BBR_MLR_REGISTRATION_TIMEOUT       3600   //<* Default MLR timeout in seconds
#define THREAD_BBR_DUA_REGISTRATION_TIMEOUT       3600*24 //<* 24 hours and we refresh the sequence number
#define THREAD_BBR_DUA_REGISTRATION_DELAY         5000  // 5 seconds in ms
#define THREAD_BBR_BACKBONE_PORT                  5683  //<* Backbone border router
#define THREAD_BBR_DUA_DAD_QUERY_TIMEOUT          1     // wait period for Duplicate Address Detection
#define THREAD_BBR_DUA_DAD_REPEATS                2     // multicast repeated as part of DUA

static void thread_border_router_multicast_store_add(thread_pbbr_t *this, uint8_t *destination_addr_ptr);
static void thread_border_router_multicast_store_del(thread_pbbr_t *this, uint8_t *destination_addr_ptr);
static void thread_border_router_multicast_store_activate(thread_pbbr_t *this);

static int stringlen(const char *s, int n)
{
    char *end = memchr(s, 0, n);
    return end ? end - s : n;
}

static thread_pbbr_t *thread_bbr_find_by_interface(int8_t interface_id)
{
    thread_pbbr_t *this = NULL;
    ns_list_foreach(thread_pbbr_t, cur_br, &pbbr_instance_list) {
        if (cur_br->interface_id == interface_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static thread_pbbr_t *thread_border_router_find_by_service(int8_t service_id)
{
    thread_pbbr_t *this = NULL;
    ns_list_foreach(thread_pbbr_t, cur_br, &pbbr_instance_list) {
        if (cur_br->coap_service_id == service_id ||
                cur_br->br_bb_service_id == service_id || cur_br->coap_nmkp_virtual_service_id == service_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static duplicate_dua_tr_t *thread_border_router_dup_tr_create(int8_t interface_id, uint8_t *source_addr_ptr, uint8_t *target_eid_ptr, uint8_t *ml_eid_ptr)
{
    duplicate_dua_tr_t *this = ns_dyn_mem_alloc(sizeof(duplicate_dua_tr_t));

    if (!this) {
        return NULL;
    }
    ns_list_add_to_start(&duplicate_dua_tr_list, this);
    this->interface_id = interface_id;
    memcpy(this->target_eid, target_eid_ptr, 16);
    memcpy(this->source_address, source_addr_ptr, 16);
    memcpy(this->ml_eid, ml_eid_ptr, 8);
    this->dua_dad_repeat = THREAD_BBR_DUA_DAD_REPEATS;
    this->ttl = THREAD_BBR_DUA_DAD_QUERY_TIMEOUT;
    return this;
}

static void thread_border_router_dup_tr_delete(duplicate_dua_tr_t *this)
{
    if (!this) {
        return;
    }
    ns_list_remove(&duplicate_dua_tr_list, this);
    ns_dyn_mem_free(this);
    return;
}

static duplicate_dua_tr_t *thread_border_router_dup_tr_find(int8_t interface_id, uint8_t *target_eid_ptr)
{
    duplicate_dua_tr_t *this = NULL;
    ns_list_foreach(duplicate_dua_tr_t, cur_dup_tr, &duplicate_dua_tr_list) {
        if (cur_dup_tr->interface_id == interface_id && memcmp(cur_dup_tr->target_eid, target_eid_ptr, 16) == 0) {
            this = cur_dup_tr;
            break;
        }
    }
    return this;
}

/*
 * Target EID TLV
 * ML-EID TLV
 * Time Since Last Transaction TLV
 * DUA Registration Sequence Number TLV
 * Network Name TLV
 */
static int thread_border_router_bb_ans_send(thread_pbbr_t *this, uint8_t *destination_addr_ptr, uint8_t *target_eid_ptr, uint8_t *ml_eid_ptr, uint32_t last_transaction_time, uint8_t *network_name_ptr, uint16_t *rloc_ptr)
{
    uint8_t *payload_ptr, *ptr;
    sn_coap_msg_type_e coap_msg_type = COAP_MSG_TYPE_CONFIRMABLE;

    tr_debug("Thread BBR BB_ANS.ntf send");

    payload_ptr = ptr = ns_dyn_mem_alloc(64);
    if (!payload_ptr) {
        tr_error("BB_ANS.ntf alloc failed!");
        return -1;
    }

    ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, target_eid_ptr);
    ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, ml_eid_ptr);
    ptr = thread_meshcop_tlv_data_write_uint32(ptr, TMFCOP_TLV_LAST_TRANSACTION_TIME, last_transaction_time);
    ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_NETWORK_NAME, stringlen((char *)network_name_ptr, 16), network_name_ptr);
    if (rloc_ptr) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, *rloc_ptr);
    }

    if (addr_is_ipv6_multicast(destination_addr_ptr)) {
        coap_msg_type = COAP_MSG_TYPE_NON_CONFIRMABLE;
    }

    coap_service_request_send(this->br_bb_service_id, COAP_REQUEST_OPTIONS_NONE, destination_addr_ptr, this->pbbr_port,
                              coap_msg_type, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_BB_ANS_NTF, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);

    ns_dyn_mem_free(payload_ptr);

    return 0;
}

static void thread_border_router_bb_qry_send(thread_pbbr_t *this, const uint8_t *target_eid_ptr, uint16_t *rloc_ptr)
{
    uint8_t  *ptr;
    uint8_t payload[22];

    tr_info("Thread BBR BB_QRY.ntf send if:%d, service:%d", this->backbone_interface_id, this->br_bb_service_id);

    ptr = payload;
    if (target_eid_ptr) {
        ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, target_eid_ptr);
    }
    if (rloc_ptr) {
        ptr = thread_meshcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, *rloc_ptr);
    }

    /* UDP Encapsulation TLV */
    coap_service_request_send(this->br_bb_service_id, COAP_REQUEST_OPTIONS_NONE, this->pbbr_multicast_address, this->pbbr_port,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_BB_QRY_NTF, COAP_CT_OCTET_STREAM, payload, ptr - payload, NULL);

    return;
}
static int thread_pbbr_relay_from_tri_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t destination_address[16];
    uint16_t short_address;

    tr_info("Thread BBR relay from TRI to JR");

    if (!this) {
        return -1;
    }

    // security check? we should compare source address to TRI address

    if (thread_management_get_ml_prefix_112(this->interface_id, destination_address) != 0 ||
            2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, &short_address)) {
        tr_warn("No joiner router address");
        return -1;
    }
    common_write_16_bit(short_address, &destination_address[14]);

    //buffer length is limited to 10 characters

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_TRI_TX_NTF, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, NULL);

    return 0;
}

static int thread_pbbr_relay_to_tri_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    tr_info("Thread BBR relay to TRI");

    if (!this) {
        return -1;
    }

    coap_service_request_send(this->br_bb_service_id, COAP_REQUEST_OPTIONS_NONE, this->tri_address, this->tri_port,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_TRI_RX_NTF, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, NULL);

    return 0;
}

static int thread_pbbr_data_req_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    uint8_t payload_ptr[18] = {0};
    uint8_t *address_ptr;
    uint8_t *request_tlv_ptr;
    uint16_t request_tlv_len;
    uint8_t *ptr = payload_ptr;
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    if (!this || !request_ptr) {
        return -1;
    }

    tr_debug("MGMT_BBR_GET.req received");

    request_tlv_len = thread_tmfcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_GET, &request_tlv_ptr);

    if (0 == request_tlv_len) {
        //error handling
        return -1;
    }

    if (thread_meshcop_tlv_list_type_available(request_tlv_ptr, request_tlv_len, MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS)) {
        tr_debug("Registrar IPv6 address requested");
        // If registrar address is not set, return TRI address
        if (addr_is_ipv6_unspecified(this->registrar_address)) {
            address_ptr = this->tri_address;
        } else {
            address_ptr = this->registrar_address;
        }

        ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS, 16, address_ptr);
    }

    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr);

    return 0;
}

static int thread_pbbr_data_set_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    uint8_t response[3] = {0};
    uint8_t *ptr = response;
    int8_t response_code = -1;
    uint8_t *registrar_addr = NULL;
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    if (!request_ptr || !this) {
        return -1;
    }

    tr_debug("MGMT_BBR_SET.req received");

    if (16 == thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_REGISTRAR_IPV6_ADDRESS, &registrar_addr)) {
        memcpy(this->registrar_address, registrar_addr, 16);
        response_code = 1;
    }

    ptr = thread_meshcop_tlv_data_write_uint8(response, MESHCOP_TLV_STATE, response_code);

    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, response, ptr - response);

    return 0;
}

static int thread_pbbr_sec_data_set_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    uint8_t response[3] = {0};
    uint8_t *ptr = response;

    // This is not yet supported, thus return state TLV rejected
    ptr = thread_meshcop_tlv_data_write_uint8(response, MESHCOP_TLV_STATE, 0xff);

    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, response, ptr - response);

    return 0;
}

static int thread_pbbr_nmkp_relay_rx_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t joiner_address[16] = { 0xfe, 0x80 };
    uint8_t *joiner_iid_ptr;
    uint8_t *udp_ptr;
    uint16_t udp_len;
    uint16_t joiner_port;
    uint16_t joiner_router_rloc;
    (void) source_port;

    if (!this || !source_address || !request_ptr) {
        return -1;    // goto error response
    }

    if ((0 == (udp_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ENCAPSULATION, &udp_ptr))) ||
            (8 > thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_IID, &joiner_iid_ptr))  ||
            (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_UDP_PORT, &joiner_port)) ||
            (2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, &joiner_router_rloc))) {
        tr_warn("Corrupted relay packet received");
        return -1;
    }

    memcpy(&joiner_address[8], joiner_iid_ptr, 8);
    tr_debug("BBR Relay RX for NMKP from (%x) addr %s, port %d, len:%d", joiner_router_rloc, trace_ipv6(joiner_address), joiner_port, udp_len);

    this->joiner_router_rloc = joiner_router_rloc;
    coap_service_virtual_socket_recv(this->coap_nmkp_virtual_service_id, joiner_address, joiner_port, udp_ptr, udp_len);
    return 1; // no response sent
}

static int thread_pbbr_nmkp_virtual_socket_send_cb(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, const uint8_t *data_ptr, uint16_t data_len)
{
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t destination_address[16];
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t payload_len;

    tr_debug("virtual socket send to addr %s, port %d, len:%d", trace_ipv6(destination_addr_ptr), port, data_len);
    if (!this || !destination_addr_ptr || !data_ptr) {
        return -1;
    }

    payload_len =  4 + data_len + 4 + 8 + 4 + 2;
    payload_ptr = ns_dyn_mem_alloc(payload_len);
    if (!payload_ptr) {
        return -3;
    }
    thread_management_get_ml_prefix_112(this->interface_id, destination_address);
    common_write_16_bit(this->joiner_router_rloc, &destination_address[14]);

    tr_debug("Relay destination %s:%d", trace_ipv6(destination_address), THREAD_MANAGEMENT_PORT);
    ptr = payload_ptr;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ENCAPSULATION, data_len, data_ptr);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_IID, 8, &destination_addr_ptr[8]);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_UDP_PORT, port);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, this->joiner_router_rloc);

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_NMK_TX_NTF, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);

    ns_dyn_mem_free(payload_ptr);

    return 0;
}
static int thread_pbbr_pskd_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    device_configuration_s *configuration_ptr;
    (void)address;
    (void)port;

    if (!this) {
        return -1;
    }

    configuration_ptr = thread_joiner_application_get_device_config(this->interface_id);
    if (!configuration_ptr) {
        return -1;
    }
    tr_info("Thread BBR PSKd security start pskd:%s", trace_array(configuration_ptr->PSKd_ptr, configuration_ptr->PSKd_len));
    *pw_len = configuration_ptr->PSKd_len;
    memcpy(pw, configuration_ptr->PSKd_ptr, *pw_len);
    return 0;
}
static int thread_pbbr_nmkp_req_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    protocol_interface_info_entry_t *cur;
    uint8_t source_addr[16];
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t len;

    (void) source_port;

    tr_debug("Thread BBR NMKP recv addr %s", trace_ipv6(source_address));
    if (!this || !source_address || !request_ptr) {
        return 0;
    }
    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return -1;
    }
    len = 3 + 10; // state,timestamp
    len += thread_joiner_application_active_config_length(this->interface_id, NULL, 0, NULL, 0);
    // TODO implement processing
    payload_ptr = ns_dyn_mem_temporary_alloc(len);
    if (!payload_ptr) {
        tr_error("out of resources");
        return -1;
    }
    ptr = payload_ptr;
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 1);
    ptr = thread_joiner_application_active_config_write(this->interface_id, ptr, NULL, 0, NULL, 0);
    ptr = thread_active_timestamp_write(cur, ptr);

    memcpy(source_addr, source_address, 16);

    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr);
    ns_dyn_mem_free(payload_ptr);
    coap_service_close_secure_connection(service_id, source_addr, source_port);
    return 0;
}

static int thread_pbbr_bb_qry_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_port;
    uint16_t addr_len;
    uint8_t *addr_data_ptr;
    uint8_t *ml_eid_ptr;
    uint16_t rloc;
    uint16_t *rloc_ptr = NULL;
    uint32_t last_transaction_time;
    tr_info("Thread BBR BB_QRY.ntf Received");

    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    if (!this) {
        return -1;
    }

    link_configuration_s *link_configuration_ptr = thread_joiner_application_get_config(this->interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    protocol_interface_info_entry_t *backbone_if = protocol_stack_interface_info_get_by_id(this->backbone_interface_id);

    if (!link_configuration_ptr || !cur || !backbone_if) {
        return -1;
    }
    if (addr_interface_address_compare(backbone_if, source_address) == 0) {
        // Received from own address no need to process
        return -1;
    }

    addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &addr_data_ptr);
    if (addr_len < 16) {
        tr_warn("Invalid BB_QRY.ntf message");
        return -1;
    }

    // Test code for b/ba response override
    if (ba_response_status_count) {
        device_configuration_s *device_config = thread_joiner_application_get_device_config(this->interface_id);
        ml_eid_ptr = device_config->eui64;
        last_transaction_time = protocol_core_monotonic_time;
        ba_response_status_count--;
        goto send_response;
    }

    ipv6_route_t *route = ipv6_route_choose_next_hop(addr_data_ptr, this->interface_id, NULL);
    if (!route || route->prefix_len < 128 || !route->on_link || route->info.source != ROUTE_THREAD_PROXIED_DUA_HOST || !route->info.info) {
        //address not in mesh
        return -1;
    }
    if (thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_RLOC16, &rloc) > 1) {
        rloc_ptr = &rloc;
    }

    last_transaction_time = protocol_core_monotonic_time - ((thread_pbbr_dua_info_t *)route->info.info)->last_contact_time;
    ml_eid_ptr = ((thread_pbbr_dua_info_t *)route->info.info)->mleid_ptr;

send_response:
    // This address is valid in our MESH
    return thread_border_router_bb_ans_send(this, source_address, addr_data_ptr, ml_eid_ptr, last_transaction_time, link_configuration_ptr->name, rloc_ptr);
}

static void thread_pbbr_pro_bb_ntf_process(protocol_interface_info_entry_t *cur, uint8_t *network_name_ptr, uint8_t network_name_len, uint8_t *ml_eid_ptr, uint8_t *addr_data_ptr, uint32_t last_transaction_time)
{
    (void) network_name_ptr;
    (void) network_name_len;
    thread_pbbr_dua_info_t *mleid_dua_map;
    thread_pbbr_t *this = thread_bbr_find_by_interface(cur->id);
    if (!this) {
        return;
    }
    ipv6_route_t *route = ipv6_route_lookup_with_info(addr_data_ptr, 128, this->interface_id, NULL, ROUTE_THREAD_PROXIED_DUA_HOST, NULL, 0);
    if (!route || !route->info.info) {
        return;
    }
    mleid_dua_map = route->info.info;
    if (memcmp(mleid_dua_map->mleid_ptr, ml_eid_ptr, 8) == 0) {
        // matching ml-eid present in our dua map, device has roamed
        if (last_transaction_time <= protocol_core_monotonic_time - mleid_dua_map->last_contact_time) {
            // remove entry
            goto remove_entry;
        } else {
            thread_border_router_bb_ans_send(this, this->pbbr_multicast_address, addr_data_ptr, ml_eid_ptr, protocol_core_monotonic_time - mleid_dua_map->last_contact_time, thread_joiner_application_network_name_get(cur->id), NULL);
            return;
        }
    }

    // duplicate dua address detected
    // Address should be ML-prefix + MLeid TODO create spec issue
    uint8_t destination_address[16];
    thread_management_get_ml_prefix(cur->id, destination_address);
    memcpy(&destination_address[8], mleid_dua_map->mleid_ptr, 8);
    thread_resolution_client_address_error(cur->id, destination_address, addr_data_ptr, mleid_dua_map->mleid_ptr);

remove_entry:
    tr_info("Remove dua registration for %s", trace_ipv6(addr_data_ptr));
    ipv6_route_delete(addr_data_ptr, 128, cur->id, NULL, ROUTE_THREAD_PROXIED_DUA_HOST);
    return;
}

static int thread_pbbr_dua_duplicate_address_detection(int8_t service_id, uint8_t *addr_data_ptr, uint8_t *ml_eid_ptr)
{
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    if (!this) {
        return -1;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    duplicate_dua_tr_t *tr_ptr = thread_border_router_dup_tr_find(this->interface_id, addr_data_ptr);

    if (!tr_ptr) {
        return -1;
    }

    ipv6_route_t *route = ipv6_route_choose_next_hop(addr_data_ptr, this->interface_id, NULL);
    if (!route || route->prefix_len != 128 || !route->on_link || route->info.source != ROUTE_THREAD_PROXIED_DUA_HOST) {
        // Not found
        tr_debug("route not found");
        return -1;
    }

    // We have pending request and received answer
    if (memcmp(ml_eid_ptr, tr_ptr->ml_eid, 8) != 0) {
        // Different ml_eid but same address means duplicate address detected
        thread_resolution_client_address_error(this->interface_id, tr_ptr->source_address, tr_ptr->target_eid, ml_eid_ptr);
        ipv6_neighbour_t *neighbour_entry;
        neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, tr_ptr->target_eid);
        if (neighbour_entry) {
            tr_debug("Remove from neigh Cache: %s", tr_ipv6(tr_ptr->target_eid));
            ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
        }
        ipv6_route_delete(route->prefix, route->prefix_len, this->interface_id, NULL, ROUTE_THREAD_PROXIED_DUA_HOST);
        thread_border_router_dup_tr_delete(tr_ptr);
    }

    return 0;
}

static int thread_pbbr_bb_ans_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)service_id;
    (void)source_port;
    (void)request_ptr;
    uint16_t addr_len, ml_eid_len, last_transaction_time_len;
    uint8_t *addr_data_ptr;
    uint16_t rloc16;
    uint8_t destination_address[16] = {0};
    uint8_t *ml_eid_ptr;
    uint32_t last_transaction_time;
    uint8_t *network_name_ptr;
    uint8_t network_name_len;
    tr_info("Thread BBR BB_ANS.ntf receive");
    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);

    if (!this) {
        return -1;
    }
    link_configuration_s *link_configuration_ptr = thread_joiner_application_get_config(this->interface_id);
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    protocol_interface_info_entry_t *backbone_if = protocol_stack_interface_info_get_by_id(this->backbone_interface_id);

    if (!link_configuration_ptr || !cur || !backbone_if) {
        return -1;
    }
    if (addr_interface_address_compare(backbone_if, source_address) == 0) {
        // Received from own address no need to process
        return -1;
    }
    addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &addr_data_ptr);
    ml_eid_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_ML_EID, &ml_eid_ptr);
    last_transaction_time_len = thread_meshcop_tlv_data_get_uint32(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_LAST_TRANSACTION_TIME, &last_transaction_time);
    network_name_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_NETWORK_NAME, &network_name_ptr);

    if (addr_len < 16 || ml_eid_len < 8 || last_transaction_time_len < 4) {
        tr_err("Invalid message");
        // send confirmation with not acceptable status code
        if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
            coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_NOT_ACCEPTABLE, COAP_CT_OCTET_STREAM, NULL, 0);
        }
        return 0;
    }

    // when we process unicast b/ba req send confirmation
    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, NULL, 0);
    }

    if ((thread_pbbr_dua_duplicate_address_detection(service_id, addr_data_ptr, ml_eid_ptr) == 0)) {
        return 0;
    }

    // If rloc16 is present then a/an is sent to the thread device with the rloc
    if (thread_tmfcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_RLOC16, &rloc16) != 2) {
        thread_pbbr_pro_bb_ntf_process(cur, network_name_ptr, network_name_len, ml_eid_ptr, addr_data_ptr, last_transaction_time);
        return 0;
    }

    // form the destination address to which the a/an is sent
    thread_addr_write_mesh_local_16(destination_address, rloc16, cur->thread_info);

    // Address query pending send address notification, include tlvs: target eid, rloc16 tlv of bbr, mleid, and last transaction time
    uint8_t payload[16 + 2 + 8 + 4]; // Target eid + Rloc16 + MLEID + transaction time
    uint8_t *ptr;
    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, addr_data_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint16(ptr, TMFCOP_TLV_RLOC16, cur->thread_info->routerShortAddress);
    ptr = thread_tmfcop_tlv_data_write(ptr, TMFCOP_TLV_ML_EID, 8, ml_eid_ptr);
    ptr = thread_tmfcop_tlv_data_write_uint32(ptr, TMFCOP_TLV_LAST_TRANSACTION_TIME, last_transaction_time);

    // XXX "Accepted" response?

    /* We don't require a response, so we don't specify a callback. Library
     * should retry itself until it gets an ACK.
     */
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_ADDRESS_SHORT,
                              destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_ADDRESS_NOTIFICATION, COAP_CT_OCTET_STREAM,
                              payload, ptr - payload, NULL);
    return 0;
}
static int thread_bbr_commercial_bmlr_req_send(int8_t service_id, const uint8_t br_addr[16], const uint8_t *address_ptr, uint8_t addr_len, uint32_t timeout)
{
    uint8_t payload[2 + 16 + 2 + 4 + 2];
    uint8_t *ptr;

    if (!br_addr || !address_ptr) {
        return -1;
    }

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_IPV6_ADDRESS, addr_len, address_ptr);
    ptr = thread_meshcop_tlv_data_write_uint32(ptr, TMFCOP_TLV_TIMEOUT, timeout);

    tr_debug("thread BMLR.ntf send %s; timeout: %"PRIu32, trace_ipv6(address_ptr), timeout);

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, br_addr, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_BBR_BMLR_NTF, COAP_CT_OCTET_STREAM, payload, ptr - payload, NULL);
    return 0;
}

static int thread_bbr_commercial_mlr_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;

    protocol_interface_info_entry_t *cur;
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;
    uint16_t addr_len;
    uint16_t session_id;
    uint8_t *addr_data_ptr;
    uint8_t *invalid_addr_ptr = NULL;
    uint32_t timeout_value;
    bool commissioner = false;
    uint8_t bbr_rloc_addr[16];
    uint8_t bbr_status = THREAD_ST_DUA_SUCCESS;
    uint8_t payload[4 + 18];
    uint8_t *ptr;

    tr_info("Thread BBR MLR.ntf receive");

    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    if (!this) {
        return -1;
    }

    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return -1;
    }

    ptr = payload;

    // Set default value if not specified in message.
    timeout_value = this->mlr_timeout;

    addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_IPV6_ADDRESS, &addr_data_ptr);

    //check if commissioner
    if (2 <= thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) {
        // Session id present must be valid
        tr_info("message from commissioner");
        if (cur->thread_info->registered_commissioner.session_id != session_id) {
            tr_warn("Invalid commissioner session id");
            bbr_status = THREAD_ST_DUA_GENERAL_FAILURE;
            goto send_response;
        }
        thread_meshcop_tlv_data_get_uint32(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TIMEOUT, &timeout_value);
        commissioner = true;
    }

    if (!commissioner) {
        // check if we are not primary bbr respond status 5
        if (0 != thread_common_primary_bbr_get(cur, bbr_rloc_addr, NULL, NULL, NULL) ||
                !addr_get_entry(cur, bbr_rloc_addr)) {
            // Primary BBR not present or I am not BBR
            bbr_status = THREAD_ST_DUA_BBR_NOT_PRIMARY;
            goto send_response;
        }
    }


    int16_t remaining = addr_len;
    uint8_t *addr_ptr = addr_data_ptr;

    if (addr_len < 16 ||
            addr_len % 16 != 0) {
        // Address must be more than 16 and can be multible
        tr_err("Invalid /n/mr message");
        if (addr_len >= 16) {
            invalid_addr_ptr = addr_ptr;
        }
        bbr_status = THREAD_ST_DUA_INVALID;
        goto send_response;
    }

    while (remaining > 0) {
        //Go through all addresses
        if (!addr_is_ipv6_multicast(addr_ptr)) {
            tr_err("Invalid /n/mr not multicast address");
            bbr_status = THREAD_ST_DUA_INVALID;
            if (!invalid_addr_ptr) {
                invalid_addr_ptr = addr_ptr;
            }
        } else {
            if (timeout_value == 0) {
                //tr_debug("Multicast address: %s delete", trace_ipv6(addr_ptr));
                addr_multicast_fwd_remove(cur, addr_ptr);
                // TODO remove address from NVM
                thread_border_router_multicast_store_del(this, addr_ptr);
            } else {
                //tr_debug("Multicast address: %s Timeout value: %"PRIu32, trace_ipv6(addr_ptr),timeout_value);
                multicast_fwd_add(this->interface_id, addr_ptr, timeout_value);
                if (timeout_value == 0xffffffff) {
                    thread_border_router_multicast_store_add(this, addr_ptr);
                }
                // send BMLR.ntf message to backend
                thread_bbr_commercial_bmlr_req_send(this->br_bb_service_id, this->pbbr_multicast_address, addr_data_ptr, 16, timeout_value);
            }
        }
        addr_ptr += 16;
        remaining -= 16;
    }

send_response:

    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, bbr_status);
        if (invalid_addr_ptr) {
            ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_IPV6_ADDRESS, 16, invalid_addr_ptr);
        }
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
        return 0;
    }

    return -1;
}
static int thread_bbr_commercial_bmlr_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;
    (void)service_id;
    (void)request_ptr;

    tr_info("Thread BBR BMLR.ntf receive");

    return -1;
}

static int thread_bbr_commercial_dua_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_port;
    uint16_t addr_len, ml_eid_len;
    uint8_t *addr_data_ptr = NULL;
    uint8_t *ml_eid_ptr;
    uint8_t payload[2 + 16 + 2 + 2 + 1 + 8];
    uint8_t bbr_rloc_addr[16];
    uint8_t *ptr;
    bool entry_keep_alive = false;
    uint8_t bbr_status = THREAD_ST_DUA_SUCCESS;

    tr_info("Thread BBR DUA.req received");

    thread_pbbr_t *this = thread_border_router_find_by_service(service_id);
    sn_coap_msg_code_e response_code = COAP_MSG_CODE_RESPONSE_CHANGED;

    if (!this) {
        return -1;
    }
    link_configuration_s *link_configuration_ptr = thread_joiner_application_get_config(this->interface_id);
    if (!link_configuration_ptr) {
        return -1;
    }

    /*if (this->backbone_interface_id< 0){
        //backbone not valid
        tr_warn("Backbone link not available");
        bbr_status = THREAD_BBR_STATUS_BB_LINK_NOT_OPERATIONAL;
        goto send_response;
    }
    */
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return -1;
    }

    if (0 == thread_common_primary_bbr_get(cur, bbr_rloc_addr, NULL, NULL, NULL) &&
            !addr_get_entry(cur, bbr_rloc_addr)) {
        // Primary pBBR present and I am not the pBBR
        bbr_status = THREAD_ST_DUA_BBR_NOT_PRIMARY;
        goto send_response;
    }

    addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_TARGET_EID, &addr_data_ptr);
    ml_eid_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, TMFCOP_TLV_ML_EID, &ml_eid_ptr);

    if (addr_len < 16 || ml_eid_len < 8) {
        tr_err("Invalid /n/dr message");
        bbr_status = THREAD_ST_DUA_GENERAL_FAILURE;
        goto send_response;
    }

    if (!bitsequal(addr_data_ptr, this->domain_prefix, 64)) {
        // Prefix did not match
        tr_warn("Registration failed invalid address");
        bbr_status = THREAD_ST_DUA_INVALID;
        goto send_response;
    }

    tr_debug("DUA.req addr:%s ml_eid:%s", trace_array(addr_data_ptr, addr_len), trace_array(ml_eid_ptr, ml_eid_len));

    // Test code for dua response override
    if (dua_response_status != 0xff && dua_status_count) {
        bbr_status = dua_response_status;
        dua_status_count--;
        goto send_response;
    }

    entry_keep_alive = false;
    ipv6_route_t *route = ipv6_route_lookup_with_info(addr_data_ptr, 128, this->interface_id, NULL, ROUTE_THREAD_PROXIED_DUA_HOST, NULL, 0);

    // new device has generated duplicate dua
    if (route != NULL && route->info.info != NULL && memcmp(((thread_pbbr_dua_info_t *)route->info.info)->mleid_ptr, ml_eid_ptr, 8) != 0) {
        bbr_status = THREAD_ST_DUA_DUPLICATE;
        goto send_response;
    }
    if (route != NULL && route->info.info != NULL && memcmp(((thread_pbbr_dua_info_t *)route->info.info)->mleid_ptr, ml_eid_ptr, 8) == 0) {
        ((thread_pbbr_dua_info_t *)route->info.info)->last_contact_time = protocol_core_monotonic_time;
        entry_keep_alive = true;
    }
    if (thread_bbr_dua_entry_add(this->interface_id, addr_data_ptr, this->dua_timeout * 2, ml_eid_ptr) != 0) {
        bbr_status = THREAD_ST_DUA_NO_RESOURCES;
        goto send_response;
    }
    /*if (route && memcmp(route->info.next_hop_addr, ml_addr,16) != 0) {
        // MLEID not matching duplicate detected
        tr_warn("Registration failed duplicate detected");
        bbr_status = THREAD_BBR_STATUS_DUA_ALREADY_IN_USE;
        goto send_response;
    }
    */

    if (entry_keep_alive) {
        // send proactive BB_ans.ntf
        thread_border_router_bb_ans_send(this, this->pbbr_multicast_address, addr_data_ptr, ml_eid_ptr, 0, link_configuration_ptr->name, NULL);
    } else {
        // send BB.qry
        // TODO Create duplicate Transaction to wait answers from Backbone
        thread_border_router_dup_tr_create(this->interface_id, source_address, addr_data_ptr, ml_eid_ptr);
        thread_border_router_bb_qry_send(this, addr_data_ptr, NULL);

    }

send_response:

    ptr = payload;
    ptr = thread_tmfcop_tlv_data_write_uint8(ptr, TMFCOP_TLV_STATUS, bbr_status);
    if (addr_data_ptr) {
        ptr = thread_meshcop_tlv_data_write(ptr, TMFCOP_TLV_TARGET_EID, 16, addr_data_ptr);
    }
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, response_code, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}


/*POC for multicast NVM*/
#define MULTICAST_ADDRESS_STORE_AMOUNT 3

typedef struct {
    uint8_t addr[16];
} multicast_addr_t;
static multicast_addr_t multicast_store[MULTICAST_ADDRESS_STORE_AMOUNT] = {{{0}}};

static void thread_border_router_multicast_store_add(thread_pbbr_t *this, uint8_t *destination_addr_ptr)
{
    (void)this;
    tr_info("Store multicast address to NVM");
    for (int n = 0; n < MULTICAST_ADDRESS_STORE_AMOUNT; n++) {
        if (memcmp(multicast_store[n].addr, ADDR_UNSPECIFIED, 16) == 0) {
            memcpy(multicast_store[n].addr, destination_addr_ptr, 16);
            break;
        }
    }
}

static void thread_border_router_multicast_store_del(thread_pbbr_t *this, uint8_t *destination_addr_ptr)
{
    (void)this;
    tr_info("delete multicast address from NVM");
    for (int n = 0; n < MULTICAST_ADDRESS_STORE_AMOUNT; n++) {
        if (memcmp(multicast_store[n].addr, destination_addr_ptr, 16) == 0) {
            memcpy(multicast_store[n].addr, ADDR_UNSPECIFIED, 16);
        }
    }
}

static void thread_border_router_multicast_store_activate(thread_pbbr_t *this)
{
    (void)this;
    for (int n = 0; n < MULTICAST_ADDRESS_STORE_AMOUNT; n++) {
        if (memcmp(multicast_store[n].addr, ADDR_UNSPECIFIED, 16) != 0) {
            multicast_fwd_add(this->interface_id, multicast_store[n].addr, 0xffffffff);
            thread_bbr_commercial_bmlr_req_send(this->br_bb_service_id, this->pbbr_multicast_address, multicast_store[n].addr, 16, 0xffffffff);
        }
    }
}


static bool thread_bbr_commercial_downgrade_to_secondary(struct protocol_interface_info_entry *cur)
{
    uint16_t rloc16 = mac_helper_mac16_address_get(cur);
    uint16_t highest_rloc16 = rloc16;
    uint8_t highest_seq = 0;


    ns_list_foreach(thread_network_data_service_cache_entry_t, service, &cur->thread_info->networkDataStorage.service_list) {

        if (service->S_enterprise_number != THREAD_ENTERPRISE_NUMBER) {
            // Not Thread service
            continue;
        }
        if (service->S_service_data_length < 1 &&
                service->S_service_data[0] != THREAD_SERVICE_DATA_BBR) {
            //Not pBBR service
            continue;
        }

        ns_list_foreach(thread_network_data_service_server_entry_t, service_instance, &service->server_list) {
            if (service_instance->router_id > 0xfffe) {
                continue;
            }
            if (service_instance->server_data_length < 1) {
                continue;
            }
            if (service_instance->server_data[0] > highest_seq) {
                highest_seq = service_instance->server_data[0];
                highest_rloc16 = service_instance->router_id;
            }
        }
    }
    if (highest_rloc16 != rloc16) {
        // We are not highest serquence number we
        return true;
    }
    return false;
}


static int thread_border_router_pbbr_nw_data_register(thread_pbbr_t *this)
{
    // Create Primary BBR network data and send to leader
    uint8_t service_data[1];
    uint8_t server_data[7];
    uint8_t *ptr = server_data;

    service_data[0] = THREAD_SERVICE_DATA_BBR;
    *ptr++ = this->sequence_number;
    ptr = common_write_16_bit(this->delay_timer, ptr);
    ptr = common_write_32_bit(this->mlr_timeout, ptr);

    if (thread_border_router_service_add(this->interface_id, service_data,
                                         1, 0, THREAD_ENTERPRISE_NUMBER,
                                         server_data, 7, true) != 0) {
        tr_error("Adding service data failed!");
        return -1;
    }

    if (memcmp(this->domain_prefix, ADDR_UNSPECIFIED, 8) != 0) {
        thread_border_router_info_t br_info = {0};
        br_info.P_on_mesh = true;
        br_info.stableData = true;
        br_info.P_res1 = true;
        if (thread_border_router_prefix_add(this->interface_id, this->domain_prefix, 64, &br_info)) {
            tr_error("Adding domain prefix failed!");
            return -1;
        }
    }
    return 0;
}

static int thread_bbr_commercial_pbbr_stop(thread_pbbr_t *this)
{
    // Create Primary BBR network data and send to leader
    uint8_t service_data[1];

    service_data[0] = THREAD_SERVICE_DATA_BBR;
    thread_border_router_service_delete(this->interface_id, service_data, 1, THREAD_ENTERPRISE_NUMBER);
    thread_border_router_prefix_delete(this->interface_id, this->domain_prefix, 64);
    multicast_free_address(this->pbbr_multicast_address);
    multicast_fwd_full_for_scope(this->interface_id, IPV6_SCOPE_SITE_LOCAL);

    ipv6_route_table_remove_info(this->backbone_interface_id, ROUTE_THREAD_BBR, NULL);

    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_BBR_TRI_RX_NTF);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_BBR_NMK_RX_NTF);

    coap_service_delete(this->br_bb_service_id);
    this->br_bb_service_id = -1;

    coap_service_delete(this->coap_nmkp_virtual_service_id);
    this->coap_nmkp_virtual_service_id = -1;

    this->pbbr_started = false;
    return 0;
}

static int thread_bbr_commercial_pbbr_start(thread_pbbr_t *this)
{
    // Create Primary BBR network data and send to leader
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur || !cur->thread_info) {
        return -1;
    }

    this->sequence_number++;

    thread_border_router_pbbr_nw_data_register(this);
    // Start ticker for DUA refresh
    thread_border_router_publish(this->interface_id);
    this->dua_timer_ticks = this->dua_timeout;
    if (this->pbbr_started) {
        //If started only refresh network data
        return 0;
    }
    // Register to backbone CoAP service
    this->br_bb_service_id = coap_service_initialize(this->backbone_interface_id, this->pbbr_port, COAP_SERVICE_OPTIONS_SELECT_SOCKET_IF, NULL, NULL);
    if (this->br_bb_service_id < 0) {
        tr_error("pBBR Commercial service start failed");
        thread_bbr_commercial_pbbr_stop(this);
        return -1;
    }
    // Allow only filtered addresses to be forwarded
    multicast_fwd_full_for_scope(this->interface_id, 0x10);
    multicast_fwd_full_for_scope(this->backbone_interface_id, 0);

    // Generate pbbr multicast address
    memset(this->pbbr_multicast_address, 0, 16);
    this->pbbr_multicast_address[0] = 0xff;
    this->pbbr_multicast_address[1] = 0x32; //Thread specification says p and t bits are 1
    this->pbbr_multicast_address[2] = 0x00; //Reserved
    this->pbbr_multicast_address[3] = 0x40; //Prefix length 64 bits
    memcpy(&this->pbbr_multicast_address[4], this->domain_prefix, 8);
    this->pbbr_multicast_address[15] = 3;

    tr_info("pBBR service started (Commercial network)");
    // Register Primary BBR backbone multicast address
    multicast_add_address(this->pbbr_multicast_address, false);
    this->pbbr_started = true;
    thread_border_router_multicast_store_activate(this);

    // Register Backbone commercial features
    coap_service_register_uri(this->br_bb_service_id, THREAD_URI_BBR_BB_QRY_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_bb_qry_cb);
    coap_service_register_uri(this->br_bb_service_id, THREAD_URI_BBR_BB_ANS_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_bb_ans_cb);
    coap_service_register_uri(this->br_bb_service_id, THREAD_URI_TRI_TX_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_relay_from_tri_cb);
    coap_service_register_uri(this->br_bb_service_id, THREAD_URI_BBR_BMLR_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_bbr_commercial_bmlr_cb);

    // Register Mesh side relay URI
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_TRI_RX_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_relay_to_tri_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_NMK_RX_NTF, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_nmkp_relay_rx_recv_cb);

    // create secure service for Network master key provisioning
    this->coap_nmkp_virtual_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_SECURE | COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET, thread_pbbr_pskd_security_start_cb, NULL);
    // SET certificates
    thread_ccm_network_certificate_enable(cur, this->coap_nmkp_virtual_service_id);
    coap_service_register_uri(this->coap_nmkp_virtual_service_id, THREAD_URI_BBR_NMKP_REQ, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_nmkp_req_recv_cb);
    coap_service_virtual_socket_set_cb(this->coap_nmkp_virtual_service_id, thread_pbbr_nmkp_virtual_socket_send_cb);

    return 0;
}

int8_t thread_bbr_commercial_init(int8_t interface_id, int8_t backbone_interface_id)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (this) {
        return 0;
    }
    if (thread_version < THREAD_VERSION_1_2) {
        return 0;
    }
    tr_debug("thread_border_router_init if=%d", interface_id);

    this = ns_dyn_mem_alloc(sizeof(thread_pbbr_t));
    if (!this) {
        return -2;
    }
    memset(this, 0, sizeof(thread_pbbr_t));
    this->interface_id = interface_id;

    this->backbone_interface_id = backbone_interface_id;
    this->sequence_number = randLIB_get_8bit();
    this->mlr_timeout = THREAD_BBR_MLR_REGISTRATION_TIMEOUT;
    this->dua_timeout = THREAD_BBR_DUA_REGISTRATION_TIMEOUT;
    this->delay_timer = THREAD_BBR_DUA_REGISTRATION_DELAY;
    this->pbbr_started = false;
    memcpy(this->pbbr_multicast_address, ADDR_LINK_LOCAL_ALL_ROUTERS, 16);
    this->pbbr_port = THREAD_BBR_BACKBONE_PORT;
    memset(this->registrar_address, 0, 16);
    memcpy(this->tri_address, ADDR_LINK_LOCAL_ALL_ROUTERS, 16);
    this->tri_port = THREAD_BBR_BACKBONE_PORT;
    this->joiner_router_rloc = 0xffff;
    this->coap_nmkp_virtual_service_id = -1;
    this->br_bb_service_id = -1;
    memset(this->domain_prefix, 0, sizeof(this->domain_prefix));
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    if (this->coap_service_id < 0) {
        tr_warn("Thread border router coap init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
    ns_list_add_to_start(&pbbr_instance_list, this);

    // Set the partition weight to be 72
    thread_management_partition_weighting_set(this->interface_id, 72);
    // Register Mesh address registration URI
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_MCAST_LISTENER_REPORT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_bbr_commercial_mlr_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_DOMAIN_ADDRESS_REGISTRATION, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_bbr_commercial_dua_cb);
    // Register BBR data request URI
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_DATA_REQ, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_pbbr_data_req_recv_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_BBR_DATA_SET, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_data_set_recv_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_MGMT_SEC_PENDING_SET, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_sec_data_set_recv_cb);

    // add features to secure commissioner port
    int8_t commissioner_service_id = thread_bbr_get_commissioner_service(this->interface_id);
    coap_service_register_uri(commissioner_service_id, THREAD_URI_BBR_DATA_REQ, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_pbbr_data_req_recv_cb);
    coap_service_register_uri(commissioner_service_id, THREAD_URI_BBR_DATA_SET, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_data_set_recv_cb);
    coap_service_register_uri(commissioner_service_id, THREAD_URI_MGMT_SEC_PENDING_SET, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_pbbr_sec_data_set_recv_cb);
    coap_service_register_uri(commissioner_service_id, THREAD_URI_BBR_MCAST_LISTENER_REPORT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_bbr_commercial_mlr_cb);
    return 0;
}

void thread_bbr_commercial_delete(int8_t interface_id)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return;
    }
    thread_bbr_commercial_pbbr_stop(this);
    int8_t commissioner_service_id = thread_bbr_get_commissioner_service(this->interface_id);
    coap_service_unregister_uri(commissioner_service_id, THREAD_URI_BBR_DATA_REQ);
    coap_service_unregister_uri(commissioner_service_id, THREAD_URI_BBR_DATA_SET);
    coap_service_unregister_uri(commissioner_service_id, THREAD_URI_MGMT_SEC_PENDING_SET);
    coap_service_unregister_uri(commissioner_service_id, THREAD_URI_BBR_MCAST_LISTENER_REPORT);

    coap_service_delete(this->coap_service_id);

    ns_list_remove(&pbbr_instance_list, this);
    ns_dyn_mem_free(this);
}

bool thread_bbr_commercial_nd_query_process(protocol_interface_info_entry_t *cur, const uint8_t *target_addr, uint16_t rloc)
{
    if (thread_version < THREAD_VERSION_1_2) {
        return false;
    }
    thread_pbbr_t *this = thread_bbr_find_by_interface(cur->id);
    if (!this) {
        return false;
    }
    if (!this->pbbr_started) {
        return false;
    }
    // if we do not have DUA addressing enabled
    if (!bitsequal(this->domain_prefix, target_addr, 64)) {
        return false;
    }

    // SEND BB_QRY
    thread_border_router_bb_qry_send(this, target_addr, &rloc);
    return true;

}

static void thread_bbr_commercial_dad_process(protocol_interface_info_entry_t *cur, thread_pbbr_t *this, uint32_t seconds)
{
    if (!this) {
        return;
    }
    ns_list_foreach_safe(duplicate_dua_tr_t, cur_dup_tr, &duplicate_dua_tr_list) {
        if (cur_dup_tr->interface_id != cur->id) {
            continue;
        }
        if (cur_dup_tr->ttl > seconds) {
            cur_dup_tr->ttl -= seconds;
        } else {
            cur_dup_tr->dua_dad_repeat--;
            // repeat dad for one more time
            if (cur_dup_tr->dua_dad_repeat > 0) {
                cur_dup_tr->ttl = THREAD_BBR_DUA_DAD_QUERY_TIMEOUT;
                thread_border_router_bb_qry_send(this, cur_dup_tr->target_eid, NULL);
            } else {
                // dad completed
                // send PRO_BB.ntf and delete dad entry
                thread_border_router_bb_ans_send(this, this->pbbr_multicast_address, cur_dup_tr->target_eid, cur_dup_tr->ml_eid, 0, thread_joiner_application_network_name_get(cur->id), NULL);
                thread_bbr_na_send(this->backbone_interface_id, cur_dup_tr->target_eid);
                thread_border_router_dup_tr_delete(cur_dup_tr);
            }
        }
    }
}

void thread_bbr_commercial_route_update(protocol_interface_info_entry_t *cur)
{
    // if we have DUA prefix in settings
    //   if we are pbbr delete dua on-mesh route
    //   if we are pbbr add dua route to backbone
    // else
    // Remove all DUA routes using type
    uint8_t addr[16];
    if (0 != thread_common_primary_bbr_get(cur, addr, NULL, NULL, NULL)) {
        // BBR not present
        return;
    }
    if (!addr_is_assigned_to_interface(cur, addr)) {
        return;
    }

    thread_pbbr_t *this = thread_bbr_find_by_interface(cur->id);
    if (!this) {
        return;
    }

    // remove with info and add valid domain prefix again to back bone interface
    ipv6_route_table_remove_info(this->backbone_interface_id, ROUTE_THREAD_BBR, NULL);

    if (!addr_is_ipv6_unspecified(this->domain_prefix)) {
        // add dua route to backbone, delete dua from on-mesh route
        ipv6_route_add_with_info(this->domain_prefix, 64, this->backbone_interface_id, NULL, ROUTE_THREAD_BBR, NULL, 0, 0xffffffff, 0);
        tr_info("Hosting pBBR for DUA prefix");
        ipv6_route_delete(this->domain_prefix, 64, cur->id, NULL, ROUTE_THREAD);
    }
}

void thread_bbr_commercial_seconds_timer(int8_t interface_id, uint32_t seconds)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return;
    }
    if (thread_version < THREAD_VERSION_1_2) {
        return;
    }
    if (this->backbone_interface_id < 0) {
        // pbbr Not enabled
        return;
    }

    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return;
    }
    if (!this->pbbr_started) {
        // Check if I am valid to start as pBBR
        if (thread_attach_ready(cur) != 0) {
            //not yet attached
            return;
        }
        // Check if there is already pbbr if present dont enable
        if (0 == thread_common_primary_bbr_get(cur, NULL, NULL, NULL, NULL)) {
            // Primary bbr present in network I am secondary
            return;
        }
        if (!thread_am_router(cur)) {
            // I am not router
            return;
        }
        // Should we have backend routing check?
        // If we are leader we should send immediately even without it?
        //if (!thread_bbr_routing_enabled(cur)) {
        //    return;
        // }

        thread_bbr_commercial_pbbr_start(this);
    } else {
        if (thread_bbr_commercial_downgrade_to_secondary(cur)) {
            tr_info("pbbr downgraded");
            thread_bbr_commercial_pbbr_stop(this);
        }

    }
    // Check secondary state if we need to drop

    // Check pending dua registrations
    thread_bbr_commercial_dad_process(cur, this, seconds);

    if (this->dua_timer_ticks) {
        if (this->dua_timer_ticks > seconds) {
            this->dua_timer_ticks -= seconds;
        } else {
            //Clear commissioner session from the border router
            tr_info("Refreshing Primary BBR information");
            thread_bbr_commercial_pbbr_start(this);
        }
    }
    // If domain prefix is changed in configuration we need to restart the pbbr
}

int thread_bbr_commercial_timeout_set(int8_t interface_id, uint32_t timeout_a, uint32_t timeout_b, uint32_t delay)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return -1;
    }
    if (timeout_a) {
        this->mlr_timeout = timeout_a;
    }
    if (timeout_b) {
        this->dua_timeout = timeout_b;
    }
    if (delay) {
        this->delay_timer = delay;
    }
    if (this->backbone_interface_id < 0 || !this->pbbr_started) {
        // it is ok to change default values before the start of pbbr
        return 0;
    }
    thread_bbr_commercial_pbbr_start(this);
    return 0;
}

int thread_bbr_commercial_prefix_set(int8_t interface_id, uint8_t *prefix)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this || !prefix) {
        return -1;
    }
    thread_border_router_prefix_delete(this->interface_id, this->domain_prefix, 64);
    memcpy(this->domain_prefix, prefix, 8);
    if (!this->pbbr_started) {
        return 0;
    }

    if (memcmp(this->domain_prefix, ADDR_UNSPECIFIED, 8) != 0) {
        thread_border_router_info_t br_info = {0};
        br_info.P_on_mesh = true;
        br_info.stableData = true;
        br_info.P_res1 = true;
        if (thread_border_router_prefix_add(this->interface_id, this->domain_prefix, 64, &br_info)) {
            tr_error("Adding domain prefix failed!");
            return -1;
        }
    }

    thread_border_router_publish(this->interface_id);

    return 0;
}

int thread_bbr_commercial_address_set(int8_t interface_id, const uint8_t *addr_ptr, uint16_t port)
{

    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);

    if (!this) {
        return -1;
    }
    if (addr_ptr) {
        memcpy(this->tri_address, addr_ptr, 16);
    }
    if (port) {
        this->tri_port = port;
    }
    return 0;
}

int thread_bbr_commercial_sequence_number_set(int8_t interface_id, uint8_t seq_number)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return -1;
    }
    if (seq_number) {
        this->sequence_number = --seq_number;
    }
    if (this->backbone_interface_id < 0 || !this->pbbr_started) {
        // it is ok to change seq number before the start of pbbr
        return 0;
    }
    thread_bbr_commercial_pbbr_start(this);
    return 0;
}

void thread_bbr_commercial_old_partition_data_clean(int8_t interface_id)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (this) {
        coap_service_request_delete_by_service_id(this->coap_service_id);
    }
}

void thread_bbr_commercial_status_override_get(uint8_t *dua_status, uint8_t *dua_count, uint8_t *ba_failure_count)
{
    if (*dua_status) {
        *dua_status = dua_response_status;
    }
    if (*dua_count) {
        *dua_count = dua_status_count;
    }
    if (*ba_failure_count) {
        *ba_failure_count = ba_response_status_count;
    }
}

void thread_bbr_commercial_status_override_set(uint8_t dua_status, uint8_t dua_count, uint8_t ba_failure_count)
{
    dua_response_status = dua_status;
    dua_status_count = dua_count;
    ba_response_status_count = ba_failure_count;
}

void thread_bbr_commercial_mcast_fwd_check(int8_t interface_id, bool *multicast_fwd)
{
    thread_pbbr_t *this = thread_bbr_find_by_interface(interface_id);

    if (!this || !multicast_fwd) {
        return;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return;
    }

    if (this->pbbr_started) {
        //We are Primary BBR so we always forward multicast
        *multicast_fwd = true;
        return;
    }

    if (0 == thread_common_primary_bbr_get(cur, NULL, NULL, NULL, NULL)) {
        // We are secondary BBR we newer forward
        *multicast_fwd = false;
        return;
    }
    // No modification made
}

#endif //HAVE_THREAD_BORDER_ROUTER && HAVE_THREAD_V2
