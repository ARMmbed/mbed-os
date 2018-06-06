/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifdef HAVE_THREAD_ROUTER

#include <string.h>
#include "ns_types.h"
#include <nsdynmemLIB.h>
#include <ns_list.h>
#include "ns_trace.h"
#include "eventOS_event_timer.h"
#include "randLIB.h"
#include "common_functions.h"
#include "thread_border_router_api.h"
#include "thread_bbr_api.h"
#include "Common_Protocols/ipv6_constants.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_config.h"
#include "6LoWPAN/Thread/thread_constants.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_extension.h"
#include "6LoWPAN/Thread/thread_extension_bbr.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_border_router_api_internal.h"
#include "6LoWPAN/Thread/thread_mdns.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "coap_service_api.h"
#include "thread_management_server.h"
#include "socket_api.h"
#include "coap_service_api.h"
#include "Common_Protocols/icmpv6.h"

#define TRACE_GROUP "tBBR"

/*
 * Border router instance data.
 */
typedef struct {
    uint8_t commissioner_address[16];
    uint8_t bbr_prefix[8];
    uint16_t commissioner_pet_request_msg_id;
    uint32_t br_delay_timer;
    uint32_t br_delete_timer;
    uint32_t router_upgrade_delay_timer;
    uint16_t commissioner_timer;/* Commissioner parameter */
    uint16_t commissioner_port; /* source port of commissioner border router */
    uint16_t joiner_router_rloc;
    uint8_t br_count;
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t coap_extension_virtual_service_id;
    int8_t br_service_id;
    int8_t backbone_interface_id;
    int8_t udp_proxy_socket; /* socket to relay messages between BA and nodes */
    bool br_info_published:1;
    bool br_hosted:1;
    bool routing_enabled:1;
    bool commissioner_connected:1;
    ns_list_link_t link;
} thread_bbr_t;

/* Neighbor discovery options according to RFC6106 (rfc4861) */
#define RFC6106_RECURSIVE_DNS_SERVER_OPTION     25
#define RFC6106_DNS_SEARCH_LIST_OPTION          31
static NS_LIST_DEFINE(bbr_instance_list, thread_bbr_t, link);


static thread_bbr_t *thread_bbr_find_by_interface(int8_t interface_id)
{
    thread_bbr_t *this = NULL;
    ns_list_foreach(thread_bbr_t, cur_br, &bbr_instance_list) {
        if (cur_br->interface_id == interface_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static thread_bbr_t *thread_border_router_find_by_service(int8_t service_id)
{
    thread_bbr_t *this = NULL;
    ns_list_foreach(thread_bbr_t, cur_br, &bbr_instance_list) {
        if (cur_br->coap_service_id == service_id || cur_br->br_service_id == service_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static thread_bbr_t *thread_border_router_find_by_udp_proxy_recv_socket_id(int8_t socket_id)
{
    thread_bbr_t *this = NULL;
    ns_list_foreach(thread_bbr_t, cur_br, &bbr_instance_list) {
        if (cur_br->udp_proxy_socket == socket_id) {
            this = cur_br;
            break;
        }
    }
    return this;
}

static void thread_border_router_commissioner_info_clear(thread_bbr_t *this)
{
    this->commissioner_timer = 0;
    coap_service_close_secure_connection(this->br_service_id, this->commissioner_address, this->commissioner_port);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_RELAY_RECEIVE);
    this->commissioner_connected = false;
}

static int thread_border_agent_tmf_get_request_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;

    thread_bbr_t *this = thread_border_router_find_by_service(service_id);

    if (!this) {
        return -1;
    }

    return thread_management_server_tmf_get_request_handler(this->interface_id, service_id, request_ptr);
}

static int thread_border_router_relay_transmit_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t destination_address[16];
    uint16_t shortAddress;
    (void)source_address;
    (void)source_port;

    tr_debug("border router relay transmit");
    thci_trace("brCommissionerDataRelayedInbound");

    if (!this) {
        return -1;
    }
    ;
    if ( thread_management_get_ml_prefix_112(this->interface_id,destination_address) != 0 ||
         2 > thread_meshcop_tlv_data_get_uint16(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_JOINER_ROUTER_LOCATOR,&shortAddress)) {
        tr_warn("No joiner router address");
        return -1;
    }

    common_write_16_bit(shortAddress, &destination_address[14]);

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, THREAD_MANAGEMENT_PORT,
        COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_RELAY_TRANSMIT, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, NULL);
    return -1;
}

static int br_commissioner_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    int ret = -1;
    (void)address;
    (void)port;

    tr_info("brCommissionerDtlsSessionStarted");
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    if(this){
        link_configuration_s *linkConfiguration = thread_joiner_application_get_config(this->interface_id);
        if( linkConfiguration ){
            memcpy(pw, linkConfiguration->PSKc, 16 );
            *pw_len = 16;
            ret = 0;
        }else{
            *pw_len = 0;
        }
//        ret = coap_service_security_key_set( service_id, address, port, this->PSKc_ptr, this->PSKc_len );
    }
    return ret;
}

static int br_commissioner_security_done_cb(int8_t service_id, uint8_t address[16], uint8_t keyblock[static 40])
{
    (void)service_id;
    (void)address;
    (void)keyblock;
    thci_trace("brCommissionerAccepted");
    return 0;
}

static int thread_border_router_relay_receive_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    (void) source_address;
    (void) source_port;
    tr_debug("border router relay receive");
    thci_trace("brCommissionerDataRelayedOutbound");
    if (!this)
        return -1;

    coap_service_request_send(this->br_service_id, COAP_REQUEST_OPTIONS_NONE, this->commissioner_address, this->commissioner_port,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_RELAY_RECEIVE, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, NULL);
    return -1;// no response for relay
}

/**
 * Thread border router petition
 * uri = tn/mc/la
 */
static int thread_border_router_leader_petition_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t *ptr;
    (void)source_address;
    (void)source_port;

    if (!response_ptr) {
        tr_warn("invalid params");
        return -1;
    }

    thci_trace("BR recv petition Resp data: %s",trace_array(response_ptr->payload_ptr, response_ptr->payload_len));
    //tr_debug("border router leader response");
    if (!this) {
        tr_warn("commissioner service missing!");
        return -1;
    }

    if (1 <= thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &ptr) && *ptr == 1) {
        // commissioning petition successfull
        if(this->commissioner_connected == false) {
            tr_debug("enabling native commissioner");
            coap_service_register_uri(this->coap_service_id, THREAD_URI_RELAY_RECEIVE, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_border_router_relay_receive_cb);

        }
        this->commissioner_connected = true;
    } else {
        tr_debug("disabling native commissioner");
        this->commissioner_connected = false;
    }

    coap_service_response_send_by_msg_id(this->br_service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, this->commissioner_pet_request_msg_id, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, response_ptr->payload_ptr, response_ptr->payload_len);
    this->commissioner_pet_request_msg_id = 0;
    if(!this->commissioner_connected){
        // Commissioner rejected by leader
        thread_border_router_commissioner_info_clear(this);
    }

    return 0;
}
static int thread_border_router_leader_message_resp_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    (void)source_address;
    (void)source_port;

    if (!response_ptr || !this) {
        tr_warn("invalid params");
        return -1;
    }

    thci_trace("BR recv Resp data: %s",trace_array(response_ptr->payload_ptr, response_ptr->payload_len));

    coap_service_response_send_by_msg_id(this->br_service_id, COAP_REQUEST_OPTIONS_SECURE_BYPASS, this->commissioner_pet_request_msg_id, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, response_ptr->payload_ptr, response_ptr->payload_len);
    this->commissioner_pet_request_msg_id = 0;

    return 0;
}

/* UDP_TX.ntf c/tx
 * Handle message originating from Commissioner.
 * */
static int thread_border_router_udp_proxy_transmit_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    int ret_val = 0;
    uint8_t *udp_data_ptr, *ipv6_addr_ptr;
    uint16_t udp_data_len, ipv6_addr_len, encapsulation_payload_len;
    uint16_t dest_port;
    uint8_t *encapsulation_payload;
    ns_address_t ns_source_addr, ns_dest_addr;
    int16_t sock_status;
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);

    (void) source_address;
    (void) source_port;

    tr_debug("Recv UDP_TX.ntf: %s", trace_array(request_ptr->payload_ptr, request_ptr->payload_len));

    if (!this) {
        return -1;
    }

    udp_data_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_UDP_ENCAPSULATION, &udp_data_ptr);
    ipv6_addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_IPV6_ADDRESS, &ipv6_addr_ptr);

    if (udp_data_len == 0 || ipv6_addr_len != 16) {
        tr_err("UDP_TX.ntf invalid message");
        return -1;
    }

    // Find source and destination ports from the encapsulation message
    // source port is not used as we are already using ephemeral port in udp_proxy socket.
    dest_port = common_read_16_bit(udp_data_ptr + 2);

    // Get UDP payload
    encapsulation_payload = udp_data_ptr + 4;
    encapsulation_payload_len = udp_data_len - 4;

    // Set source parameters
    if (thread_management_get_commissioner_address(this->interface_id, ns_source_addr.address, 0) < 0) {
        tr_error("Failed to get commissioner ALOC");
        return -1;
    }
    // tr_debug("commissioner ALOC: %s", trace_ipv6(ns_source_addr.address));
    ns_source_addr.identifier = 0; // Use ephemeral port instead of src_port
    ns_source_addr.type = ADDRESS_IPV6;

    if (this->udp_proxy_socket < 0) {
        tr_error("UDP proxy socket not open!");
        return -1;
    }

    /* Bind source to Commissioner ALOC */
    ret_val = socket_bind(this->udp_proxy_socket, &ns_source_addr);
    if (ret_val < 0) {
        tr_error("UDP_TX socket bind2 failed %d", ret_val);
        ret_val = -1;
    }

    // Set destination parameters
    ns_dest_addr.identifier = dest_port;
    ns_dest_addr.type = ADDRESS_IPV6;
    memcpy(ns_dest_addr.address, ipv6_addr_ptr, 16);

    tr_debug("push TMF msg to: %s, sock=%d", trace_ipv6(ns_dest_addr.address), this->udp_proxy_socket);
    sock_status = socket_sendto(this->udp_proxy_socket, &ns_dest_addr, encapsulation_payload, encapsulation_payload_len);
    if (sock_status < 0) {
        tr_error("UDP Proxy socket write failed %d", sock_status);
        ret_val = -1;
    }

    return -1;
}

/*
 * Handle messages to commissioner.
 * Create UDP_RX.ntf TMF message and send it to commissioner.
 */
static void thread_border_router_udp_proxy_tmf_message_receive(int8_t socket_id, ns_address_t *ns_address, uint8_t *tmf_data, int16_t tmf_data_len)
{
    uint8_t *payload_ptr, *ptr;
    uint16_t payload_len;
    uint16_t dest_port = THREAD_MANAGEMENT_PORT;

    tr_debug("UDP_RX tmf from %s, port=%d", trace_ipv6(ns_address->address), ns_address->identifier);

    thread_bbr_t *this = thread_border_router_find_by_udp_proxy_recv_socket_id(socket_id);
    if (!this) {
        tr_error("BA instance not found!");
        return;
    }

    payload_len =  (2 + 2 + 2 + 2 + tmf_data_len) + (2 + THREAD_IPV6_ADDRESS_TLV_LENGTH);

    payload_ptr = ptr = ns_dyn_mem_alloc(payload_len);
    if (!payload_ptr) {
        tr_error("UDP_RX.ntf alloc failed!");
        return;
    }

    /* IPv6 Address TLV */
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_IPV6_ADDRESS, THREAD_IPV6_ADDRESS_TLV_LENGTH, ns_address->address);

    /* UDP Encapsulation TLV */
    *ptr++ = MESHCOP_TLV_UDP_ENCAPSULATION;
    *ptr++ = 0xff;
    ptr = common_write_16_bit(2 + 2 + tmf_data_len, ptr); // Length = source port + dest port + TMF message
    ptr = common_write_16_bit(ns_address->identifier, ptr);  //source port
    ptr = common_write_16_bit(dest_port, ptr);  // destination port
    memcpy(ptr, tmf_data, tmf_data_len);

    tr_debug("send to: %s, port=%d", trace_ipv6(this->commissioner_address), this->commissioner_port);
    tr_debug("UDP_RX.ntf: %s", trace_array(payload_ptr, payload_len));
    coap_service_request_send(this->br_service_id, COAP_REQUEST_OPTIONS_NONE, this->commissioner_address, this->commissioner_port,
            COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_UDP_RECVEIVE_NOTIFICATION, COAP_CT_OCTET_STREAM, payload_ptr, payload_len, NULL);

    ns_dyn_mem_free(payload_ptr);

    return;
}

/*
 * UDP_Proxy receive socket callback.
 * This method receives TMF messages from devices in Thread network and sends them to commissioner
 */
static void thread_border_router_udp_proxy_socket_recv_callback(void *socket_cb)
{
    socket_callback_t *socket_callback = (socket_callback_t *)socket_cb;
    ns_address_t ns_addr;
    int16_t length;

    if (socket_callback->event_type == SOCKET_DATA) {
        if (socket_callback->d_len > 0) {
            uint8_t *payload = (uint8_t *) ns_dyn_mem_alloc(socket_callback->d_len);
            if (!payload) {
                tr_error("buffer allocation failed");
                return;
            }

            length = socket_read(socket_callback->socket_id, &ns_addr, payload, socket_callback->d_len);
            if (length > 0) {
                thread_border_router_udp_proxy_tmf_message_receive(socket_callback->socket_id, &ns_addr, payload, length);
            }
            ns_dyn_mem_free(payload);
        }
    }
}

static int thread_border_petition_to_leader_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t destination_address[16];
    char *uri_ptr;

    tr_debug("border router petition to leader");

    if (!this) {
        return -1;
    }
    if (0 != thread_management_get_leader_address(this->interface_id, destination_address)) {
        tr_debug("No leader");
        return -1;
    }
    if (strncmp(THREAD_URI_COMMISSIONER_PETITION, (const char *)request_ptr->uri_path_ptr, request_ptr->uri_path_len) == 0) {
        uri_ptr = THREAD_URI_LEADER_PETITION;
    } else if (strncmp(THREAD_URI_COMMISSIONER_KEEP_ALIVE,(const char *)request_ptr->uri_path_ptr,request_ptr->uri_path_len) == 0) {
        uri_ptr = THREAD_URI_LEADER_KEEP_ALIVE;
    } else {
        return -1;
    }

    // Update commissioner timeout for deleting the commissioner session if there is no messages.
    this->commissioner_timer = THREAD_COMMISSIONER_KEEP_ALIVE_INTERVAL/1000 + 10;
    //TODO simple relaying is enough
    memcpy(this->commissioner_address, source_address, 16);
    this->commissioner_port = source_port;
    this->commissioner_pet_request_msg_id = request_ptr->msg_id;//TODO one message at a time causes problems
    thci_trace("BR recv uri:%.*s data: %s",request_ptr->uri_path_len,request_ptr->uri_path_ptr,trace_array(request_ptr->payload_ptr, request_ptr->payload_len));
    //tr_debug("relay data %s",trace_array(request_ptr->payload_ptr, request_ptr->payload_len));

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, thread_border_router_leader_petition_resp_cb);

    return 0;
}
static int thread_border_relay_to_leader_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    thread_bbr_t *this = thread_border_router_find_by_service(service_id);
    uint8_t destination_address[16];
    char uri_ptr[10];

    tr_error("border router relay to leader using OLD specification");

    if (!this || request_ptr->uri_path_len > 10) {
        return -1;
    }
    if (0 != thread_management_get_leader_address(this->interface_id, destination_address)) {
        tr_debug("No leader");
        return -1;
    }
    //buffer length is limited to 10 characters
    memset(uri_ptr,0,10);
    memcpy(uri_ptr,(const char *)request_ptr->uri_path_ptr,request_ptr->uri_path_len);

    // Update commissioner timeout for deleting the commissioner session if there is no messages.
    this->commissioner_timer = THREAD_COMMISSIONER_KEEP_ALIVE_INTERVAL/1000 + 10;
    //TODO simple relaying is enough
    memcpy(this->commissioner_address, source_address, 16);
    this->commissioner_port = source_port;
    this->commissioner_pet_request_msg_id = request_ptr->msg_id;//TODO one message at a time causes problems
    thci_trace("BR recv uri:%.*s data: %s",request_ptr->uri_path_len,request_ptr->uri_path_ptr,trace_array(request_ptr->payload_ptr, request_ptr->payload_len));

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, THREAD_MANAGEMENT_PORT,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, request_ptr->payload_ptr, request_ptr->payload_len, thread_border_router_leader_message_resp_cb);

    return -1;
}

#ifdef HAVE_THREAD_BORDER_ROUTER
static bool thread_bbr_i_host_prefix(struct protocol_interface_info_entry *cur, uint8_t prefix_ptr[8], uint8_t *br_count, bool *i_am_lowest)
{
    bool i_host_this_prefix = false;
    uint16_t rloc16 = mac_helper_mac16_address_get(cur);
    uint16_t lowest_rloc16 = 0xfffe;

    if (br_count) {
        *br_count = 0;
    }
    if (i_am_lowest) {
        *i_am_lowest = false;
    }
    if (!prefix_ptr) {
        return false;
    }

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, prefix, &cur->thread_info->networkDataStorage.localPrefixList) {

        if (prefix->servicesPrefixLen != 64 ||
            memcmp(prefix_ptr, prefix->servicesPrefix, 8) != 0) {
            continue;
        }

        ns_list_foreach(thread_network_server_data_entry_t, br, &prefix->borderRouterList) {
            if (br->routerID == 0xfffe) {
                continue;
            }
            if (!br->P_default_route) {
                continue;
            }
            if (lowest_rloc16 > br->routerID) {
                lowest_rloc16 = br->routerID;
            }
            if (br_count) {
                (*br_count)++;
            }
            if (rloc16 == br->routerID) {
                i_host_this_prefix = true;
            }
        }
    }
    //tr_info("br: prefix %s, brs:%d %s",trace_array(prefix_ptr,8), *br_count, i_host_this_prefix?"I host":"");
    if (i_am_lowest) {
        *i_am_lowest = (lowest_rloc16 == rloc16);
    }
    return i_host_this_prefix;
}

static void thread_bbr_network_data_remove(thread_bbr_t *this)
{
    tr_info("br: remove default route from network");
    thread_border_router_prefix_delete(this->interface_id, this->bbr_prefix, 64);
    DHCPv6_server_service_delete(this->interface_id,this->bbr_prefix,true);
    memset(this->bbr_prefix,0,8);
    this->br_info_published = false;
}

static void thread_bbr_network_data_send(thread_bbr_t *this, uint8_t prefix[8], uint8_t eui64[8])
{
    thread_border_router_info_t br_info = { 0 };

    tr_info("br: publish default route to network");

    // delete old prefix
    memset(this->bbr_prefix,0,8);
    // create new prefix
    if (DHCPv6_server_service_init(this->interface_id, prefix, eui64, DHCPV6_DUID_HARDWARE_EUI64_TYPE) != 0) {
        tr_warn("DHCP server alloc fail");
        // set 20 seconds delay before next process
        this->br_delay_timer = 20;
        return;
    }
    memcpy(this->bbr_prefix,prefix,8);

    DHCPv6_server_service_set_address_validlifetime(this->interface_id, this->bbr_prefix, THREAD_MIN_PREFIX_LIFETIME);

    br_info.P_default_route = true;
    br_info.P_dhcp = true;
    br_info.P_on_mesh = true;
    br_info.stableData = true;
    thread_border_router_prefix_add(this->interface_id, this->bbr_prefix, 64, &br_info);
    thread_border_router_publish(this->interface_id);
    this->br_info_published = true;
}

static void thread_bbr_routing_enable(thread_bbr_t *this)
{
    if (this->routing_enabled) {
        return;
    }
    tr_info("br: enable routing");
    // Start multicast proxying
    multicast_fwd_set_forwarding(this->interface_id, true);
    this->routing_enabled = true;
}

static void thread_bbr_routing_disable(thread_bbr_t *this)
{
    if (!this->routing_enabled) {
        return;
    }
    tr_info("br: disable routing");
    // Stop multicast proxying
    //multicast_fwd_set_proxy_upstream(-1);
    // By default allow sitelocal multicast to enter mesh
    multicast_fwd_set_forwarding(this->interface_id, false);
    this->routing_enabled = false;
}

static void thread_bbr_status_check(thread_bbr_t *this, uint32_t seconds)
{
    uint8_t global_address[16];
    uint8_t *bbr_prefix_ptr = NULL;
    bool br_lowest_host;

    if (this->br_delay_timer) {
        if (this->br_delay_timer > seconds) {
            this->br_delay_timer -= seconds;
            return;
        }
        this->br_delay_timer = 0;
    }

    if (arm_net_address_get(this->backbone_interface_id, ADDR_IPV6_GP, global_address) == 0) {
        bbr_prefix_ptr = global_address;
    }

    if ( this->br_info_published &&
         (!bbr_prefix_ptr || memcmp(this->bbr_prefix,bbr_prefix_ptr,8) != 0)) {
        // Address is changed or address removed
        // remove the old prefix and read the status of the new prefix
        tr_info("br: Address changed or not valid stop routing");
        thread_bbr_network_data_remove(this);
        thread_bbr_routing_disable(this);
        thread_border_router_publish(this->interface_id);
    }
    // Check if network data as border router is possible or modified
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    this->br_hosted = thread_bbr_i_host_prefix(cur, bbr_prefix_ptr,&this->br_count, &br_lowest_host);

    if (!this->br_info_published && bbr_prefix_ptr && this->br_count == 0) {
        // publish global route either no border routers or our info has changed
        tr_info("br: start and publish br info");
        thread_bbr_network_data_send(this, bbr_prefix_ptr, &global_address[8]);
    }

    // Check from network data are we currently BR or not and change routing state
    if (this->br_hosted) {
        thread_bbr_routing_enable(this);
    } else {
        thread_bbr_routing_disable(this);
    }

    // Check if we can abort the deletion
    if (this->br_count == 1) {
        if (bbr_prefix_ptr) {
            // Everything is ok cancel clearing if it was running as other BR is removed
            this->br_delete_timer = 0;
        }
    }

    if (this->br_delete_timer) {
        if (this->br_delete_timer > seconds) {
            this->br_delete_timer -= seconds;
        } else {
            // Delete timer was set and we need to remove our subscription.
            thread_bbr_network_data_remove(this);
            thread_border_router_publish(this->interface_id);
            this->br_delete_timer = 0;
        }
    } else {
        // Check states when we need to remove our BR from network
        if (this->br_hosted && this->br_count > 1) {
            // Race condition More border routers than there should trigger random delay to remove BR
            // our implementation prefers lowest RLOC to to stay to reduce problem time
            if (br_lowest_host) {
                this->br_delete_timer = randLIB_get_random_in_range(20,60);
            } else {
                this->br_delete_timer = randLIB_get_random_in_range(5,10);
            }
            tr_info("br: too many BRs start remove jitter:%"PRIu32, this->br_delete_timer);
            return;
        }
        if (this->br_info_published && !bbr_prefix_ptr ) {
            // Need to disable ND proxy will give a 20 second delay for it We could also disable routing immediately
            this->br_delete_timer = 20;
            tr_info("br: can not be border router need to remove after: %"PRIu32, this->br_delete_timer);
            return;
        }
    }
}

static bool thread_bbr_activated(thread_bbr_t *this, uint32_t seconds)
{
    protocol_interface_info_entry_t *cur;

    if (this->backbone_interface_id < 0) {
        return false;
    }

    cur = protocol_stack_interface_info_get_by_id(this->interface_id);

    if (!cur || !cur->thread_info) {
        return false;
    }
    // Check if we are router If we are not router upgrade to router.
    // If upgrade fails then start as child router because network is full
    if (thread_am_router(cur)) {
        return true;
    }

    if (cur->thread_info->routerIdReqCoapID) {
        // Router id reguest pending we need to wait for response
        return false;
    }
    if(this->router_upgrade_delay_timer) {
        if (this->router_upgrade_delay_timer > seconds) {
            this->router_upgrade_delay_timer -= seconds;
        } else {
            this->router_upgrade_delay_timer = 0;
        }
        // Delay timer running no actions can be made, so we will become router as child
        return true;
    }
    // We will try to become router. This is done only in 120 seconds intervals if failed
    thread_router_bootstrap_router_id_request(cur, THREAD_BBR_ROUTER_ID_REQUEST_STATUS);
    this->router_upgrade_delay_timer = 120;
    return false;
}

bool thread_bbr_routing_enabled(protocol_interface_info_entry_t *cur)
{
    thread_bbr_t *this = thread_bbr_find_by_interface(cur->thread_info->interface_id);

    if (!this) {
        return false;
    }
    return this->routing_enabled;
}

void thread_bbr_network_data_update_notify(protocol_interface_info_entry_t *cur)
{
    thread_mdns_network_data_update_notify();
    thread_extension_bbr_route_update(cur);
}
#endif /* HAVE_THREAD_BORDER_ROUTER*/

static void thread_bbr_udp_proxy_service_stop(int8_t interface_id)
{
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);

    if (!this) {
        tr_error("Failed to find BA instance");
        return;
    }

    socket_close(this->udp_proxy_socket);
    this->udp_proxy_socket = -1;
}

int thread_bbr_commissioner_proxy_service_update(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;
    ns_address_t ns_source_addr;
    int ret_val;

    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!this || !cur) {
        tr_error("Failed to find BBR instance");
        ret_val = -1;
        goto return_fail;
    }

    if (!cur->thread_info->registered_commissioner.commissioner_valid) {
        // commissioner not enabled
        if (this->udp_proxy_socket != -1) {
            thread_bbr_udp_proxy_service_stop(interface_id);
        }
        return 0;
    }

    if (this->udp_proxy_socket != -1) {
        // commissioner is valid and UDP service is already running
        return 0;
    }

    // Set source parameters, if commissioner is available
    ret_val = thread_management_get_commissioner_address(this->interface_id, &ns_source_addr.address[0], 0);
    if ( ret_val < 0) {
        tr_error("Failed to get commissioner ALOC %d", ret_val);
        ret_val = -1;
        goto return_fail;
    }

    this->udp_proxy_socket = socket_open(SOCKET_UDP, 0, thread_border_router_udp_proxy_socket_recv_callback);
    if (this->udp_proxy_socket < 0) {
        tr_error("socket allocation failed!");
        ret_val = -2;
        goto return_fail;
    }

    /* register to handle UDP_TX.nft */
    coap_service_register_uri(this->br_service_id, THREAD_URI_UDP_TRANSMIT_NOTIFICATION, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_router_udp_proxy_transmit_cb);

    return 0;

    return_fail:
        thread_bbr_udp_proxy_service_stop(interface_id);
        return ret_val;
}
int8_t thread_bbr_init(int8_t interface_id, uint16_t external_commisssioner_port)
{
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (this) {
        return 0;
    }

    tr_debug("thread_border_router_init if=%d", interface_id);

    this = ns_dyn_mem_alloc(sizeof(thread_bbr_t));
    if (!this) {
        return -2;
    }
    this->commissioner_pet_request_msg_id = 0;
    this->commissioner_connected = false;
    this->commissioner_port = 0;
    this->interface_id = interface_id;
    this->udp_proxy_socket = -1;
    this->commissioner_timer = 0;
    this->backbone_interface_id = -1;
    this->br_delay_timer = 0;
    this->router_upgrade_delay_timer = 0;
    this->br_delete_timer = 0;
    this->br_info_published = false;
    this->routing_enabled = false;

    memset(this->bbr_prefix,0,8);
    this->joiner_router_rloc = 0xffff;
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    if (this->coap_service_id < 0) {
        tr_warn("Thread border router coap init failed");
        ns_dyn_mem_free(this);
        return -3;
    }

    this->br_service_id = coap_service_initialize(this->interface_id, external_commisssioner_port, COAP_SERVICE_OPTIONS_SECURE | COAP_SERVICE_OPTIONS_SECURE_BYPASS, br_commissioner_security_start_cb, br_commissioner_security_done_cb);
    //TODO this needs secure bypass option HACK made
    if (this->br_service_id < 0) {
        tr_warn("Thread border router br-service init failed");
        coap_service_delete(this->coap_service_id);
        ns_dyn_mem_free(this);
        return -4;
    }

    // Border Agent handles MGMT_GET, MGMT_ACTIVE_GET, and MGMT_PENDING_GET directly
    coap_service_register_uri(this->br_service_id, THREAD_URI_MANAGEMENT_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_agent_tmf_get_request_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_ACTIVE_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_agent_tmf_get_request_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_PENDING_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_agent_tmf_get_request_cb);

    // TODO, these URI's should be available in BA, if they are asked from BA by Native/External commissioner */
    /*
    coap_service_register_uri(this->br_service_id, THREAD_URI_ACTIVE_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_relay_to_leader_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_PENDING_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_relay_to_leader_cb);
    */

    // Border router URIs for native and external commissioner
    coap_service_register_uri(this->br_service_id, THREAD_URI_RELAY_TRANSMIT, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_border_router_relay_transmit_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_COMMISSIONER_PETITION, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_petition_to_leader_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_COMMISSIONER_KEEP_ALIVE, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_petition_to_leader_cb);

    // These messages should not be forwarded according to new specification
    coap_service_register_uri(this->br_service_id, THREAD_URI_COMMISSIONER_SET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_relay_to_leader_cb);
    coap_service_register_uri(this->br_service_id, THREAD_URI_COMMISSIONER_GET, COAP_SERVICE_ACCESS_GET_ALLOWED, thread_border_relay_to_leader_cb);

    ns_list_add_to_start(&bbr_instance_list, this);
    return 0;
}

void thread_bbr_delete(int8_t interface_id)
{
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return;
    }

    thread_bbr_stop(interface_id);
    coap_service_delete(this->coap_service_id);
    coap_service_delete(this->br_service_id);

    ns_list_remove(&bbr_instance_list, this);
    ns_dyn_mem_free(this);
}

void thread_bbr_seconds_timer(int8_t interface_id, uint32_t seconds)
{
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this) {
        return;
    }

    if (this->commissioner_timer) {
        if (this->commissioner_timer > seconds) {
            this->commissioner_timer -= seconds;
        } else {
            //Clear commissioner session from the border router
            tr_info("Timing out the commissioner");
            thread_border_router_commissioner_info_clear(this);
        }
    }

#ifdef HAVE_THREAD_BORDER_ROUTER

    // check if Border router can be active
    if (thread_bbr_activated(this,seconds)) {
        // Run the BBR SM
        thread_bbr_status_check(this,seconds);
    }

    if (!thread_extension_version_check(thread_version)) {
        return;
    }
    thread_extension_bbr_seconds_timer(interface_id, seconds);

#endif
}
#endif // HAVE_THREAD_ROUTER

#ifdef HAVE_THREAD_BORDER_ROUTER
static int thread_bbr_na_send(int8_t interface_id, const uint8_t target[static 16])
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    buffer_t *buffer = icmpv6_build_na(cur, false, true, true, target, NULL, ADDR_UNSPECIFIED);
    protocol_push(buffer);
    return 0;

}
int thread_bbr_nd_entry_add (int8_t interface_id, const uint8_t *addr_data_ptr,  uint32_t lifetime, void *info, const uint8_t *mleid_ptr) {
    (void) mleid_ptr;
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    if (!this || this->backbone_interface_id < 0) {
        tr_err("bbr not ready");
        return -1;
    }
    ipv6_route_t *route = ipv6_route_add_with_info(addr_data_ptr, 128, interface_id, NULL, ROUTE_THREAD_PROXIED_HOST, info, 0, lifetime, 0);
    // We are using route info field to store sequence number
    if (!route) {
        // Direct route to host allows ND proxying to work
        tr_err("out of resources");
        return -2;
    }
    // send NA
    thread_bbr_na_send(this->backbone_interface_id, addr_data_ptr);

    return 0;
}

int thread_bbr_nd_entry_find(int8_t interface_id, const uint8_t *addr_data_ptr) {
    ipv6_route_t *route = ipv6_route_choose_next_hop(addr_data_ptr, interface_id, NULL);
    if (!route || route->prefix_len < 128 || !route->on_link || route->info.source != ROUTE_THREAD_PROXIED_HOST ) {
        //Not found
        return -1;
    }
    //TODO get information to route to parameters eq mleid, timeout
    return 0;
}

int thread_bbr_proxy_state_update(int8_t caller_interface_id , int8_t handler_interface_id, bool status)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(handler_interface_id);
    (void) caller_interface_id;
    if (!cur) {
        tr_error("No Interface");
        return -1;
    }
    // Route prefix is variable-length, so need to zero pad for ip6tos
    bool weHostServiceAlso = false;
    bool validToLearOnMeshRoute;
    uint16_t routerId;
    routerId = cur->mac_parameters->mac_short_address;
    thread_network_data_cache_entry_t *networkData;
    networkData = &cur->thread_info->networkDataStorage;
    validToLearOnMeshRoute = thread_on_mesh_route_possible_add(cur->thread_info->thread_device_mode);

    tr_debug("Proxy update");

    ns_list_foreach(thread_network_data_prefix_cache_entry_t, curPrefix, &networkData->localPrefixList) {

        weHostServiceAlso = thread_nd_hosted_by_this_routerid(routerId, &curPrefix->routeList);

        if (weHostServiceAlso) {
            ipv6_route_add(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, NULL, ROUTE_THREAD, 0xffffffff, 0);
        }

        weHostServiceAlso = thread_nd_hosted_by_this_routerid(routerId, &curPrefix->borderRouterList);

        ns_list_foreach(thread_network_server_data_entry_t, curRoute, &curPrefix->borderRouterList) {
            if (thread_nd_on_mesh_address_valid(curRoute)) {
                if (validToLearOnMeshRoute) {
                    if (curRoute->P_dhcp && weHostServiceAlso) {
                        if (status) {
                            ipv6_route_delete(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, NULL, ROUTE_THREAD);
                        } else {
                            ipv6_route_add(curPrefix->servicesPrefix, curPrefix->servicesPrefixLen, cur->id, NULL, ROUTE_THREAD, 0xffffffff, 0);
                        }

                    }
                }
            }
        }
    }
    return 0;
}
#endif

/*Public API control*/
int thread_bbr_start(int8_t interface_id, int8_t backbone_interface_id)
{
    (void) interface_id;
    (void) backbone_interface_id;
#ifdef HAVE_THREAD_BORDER_ROUTER
    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);
    link_configuration_s *link_configuration_ptr = thread_joiner_application_get_config(interface_id);
    uint8_t *extended_random_mac = thread_joiner_application_random_mac_get(interface_id);
    char service_name[30] = {0};
    char *ptr;

    if(!this || !link_configuration_ptr || backbone_interface_id < 0) {
        return -1;
    }

    tr_info("Thread BBR start if:%d, bb_if:%d",interface_id, backbone_interface_id);

    this->backbone_interface_id = backbone_interface_id;
    ptr = service_name;
    *ptr++ = 'a' + extended_random_mac[0] % 26;
    *ptr++ = 'a' + extended_random_mac[1] % 26;
    *ptr++ = 'a' + extended_random_mac[2] % 26;
    *ptr++ = 'a' + extended_random_mac[3] % 26;
    memcpy(ptr,"-ARM-",5);
    ptr += 5;
    memcpy(ptr,link_configuration_ptr->name,16);

    // Start mdns service
    thread_mdns_start(this->interface_id, this->backbone_interface_id, service_name);
    multicast_fwd_set_proxy_upstream(this->backbone_interface_id);
    multicast_fwd_full_for_scope(this->interface_id, 0);
    multicast_fwd_full_for_scope(this->backbone_interface_id, 0);
    // By default multicast forwarding is not enabled as it causes multicast loops
    multicast_fwd_set_forwarding(this->interface_id, false);

    thread_extension_bbr_init(interface_id,backbone_interface_id);
    return 0;
#else
    return -1;
#endif // HAVE_THREAD_BORDER_ROUTER
}

int thread_bbr_timeout_set(int8_t interface_id, uint32_t timeout_a, uint32_t timeout_b, uint32_t delay)
{
    (void) interface_id;
    (void) timeout_a;
    (void) timeout_b;
    (void) delay;
#ifdef HAVE_THREAD_BORDER_ROUTER
    thread_extension_bbr_timeout_set(interface_id, timeout_a, timeout_b, delay);
return 0;
#else
    return -1;
#endif // HAVE_THREAD_BORDER_ROUTER
}

int thread_bbr_validation_interface_address_set(int8_t interface_id, const uint8_t *addr_ptr, uint16_t port)
{
    (void) interface_id;
    (void) addr_ptr;
    (void) port;
#ifdef HAVE_THREAD_BORDER_ROUTER
    return thread_extension_bbr_address_set(interface_id, addr_ptr, port);
#else
    return -1;
#endif // HAVE_THREAD_BORDER_ROUTER
}



void thread_bbr_stop(int8_t interface_id)
{
    (void) interface_id;
#ifdef HAVE_THREAD_BORDER_ROUTER

    thread_bbr_t *this = thread_bbr_find_by_interface(interface_id);

    if(!this) {
        return;
    }
    thread_extension_bbr_delete(interface_id);
    thread_bbr_network_data_remove(this);
    thread_bbr_routing_disable(this);
    thread_border_router_publish(interface_id);
    thread_mdns_stop();
    this->backbone_interface_id = -1;

#else
    return;
#endif // HAVE_THREAD_BORDER_ROUTER

}
