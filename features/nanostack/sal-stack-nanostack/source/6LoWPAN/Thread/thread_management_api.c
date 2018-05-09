/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#include "ns_types.h"
#include "ns_list.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "common_functions.h"

#include "ns_trace.h"

/**
 * Thread includes
 * */
#include "thread_config.h"
#include "thread_meshcop_lib.h"
#include "thread_management_if.h"
#include "thread_management_api.h"
#include "thread_commissioning_api.h"
#include "thread_common.h"
#ifdef HAVE_THREAD
#define TRACE_GROUP TRACE_GROUP_THREAD_MANAGEMENT_API

#include "coap_service_api.h"

typedef struct management_session {
    uint8_t destination_address[16];
    uint8_t final_dest_address[16]; /* MGMT tmf message final destination */
    uint16_t destination_port;
    management_set_response_cb *set_response_cb_ptr;
    management_get_response_cb *get_response_cb_ptr;
    bool native_interface:1;
    int8_t instance_id;
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t coap_virtual_service_id;
    ns_list_link_t link;
} management_session_t;

static NS_LIST_DEFINE(instance_list, management_session_t, link);

/* management_session class handlers*/
static management_session_t *management_find(int8_t instance_id)
{
    management_session_t *this = NULL;
    ns_list_foreach(management_session_t, cur_ptr, &instance_list) {
        if (cur_ptr->instance_id == instance_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static management_session_t *commissioner_find_by_service(int8_t service_id)
{
    management_session_t *this = NULL;
    ns_list_foreach(management_session_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id || cur_ptr->coap_virtual_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

/** Callback functions
 *
 */
int thread_management_recv_set_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    management_session_t *this = commissioner_find_by_service(service_id);
    management_state_e state = MANAGEMENT_STATE_REJECT;
    uint8_t *ptr;
    (void) source_address;
    (void) source_port;

    /* Transaction failed */
    if (!response_ptr) {
        return -1;
    }

    tr_debug("Management Set response");

    if (!this) {
        return -1;
    }

    if (1 <= thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &ptr)) {
        // only handle success
        if (*ptr == 1) {
            state = MANAGEMENT_STATE_ACCEPT;
        } else if (*ptr == 0) {
            state = MANAGEMENT_STATE_PENDING;
        }
    }

    if (this->set_response_cb_ptr) {
        this->set_response_cb_ptr(this->interface_id, state);
    }
    return 0;
}

int thread_management_recv_get_response_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    management_session_t *this = commissioner_find_by_service(service_id);
    management_state_e state = MANAGEMENT_STATE_ACCEPT;// Default is accept if we get error it is rejected
    uint8_t *ptr;
    (void) source_address;
    (void) source_port;

    /* Transaction failed */
    if (!response_ptr) {
        return -1;
    }

    tr_debug("management get response");
    if (!this) {
        return -1;
    }

    if (1 <= thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &ptr)) {
        // only handle success
        if (*ptr == 1) {
            state = MANAGEMENT_STATE_ACCEPT;
        } else if (*ptr == 0) {
            state = MANAGEMENT_STATE_PENDING;
        } else {
		    state = MANAGEMENT_STATE_REJECT;
        }
    }

    if (this->get_response_cb_ptr) {
        this->get_response_cb_ptr(this->interface_id, state, response_ptr->payload_ptr, response_ptr->payload_len);
    }
    return 0;
}

static int thread_management_get_remote_addr(management_session_t *this)
{
    if (0 == thread_management_get_leader_aloc(this->interface_id, this->destination_address)) {
        tr_debug("on-mesh interface");
        this->destination_port = THREAD_MANAGEMENT_PORT;
        this->native_interface = false;
    } else if (0 == thread_commissioning_native_commissioner_get_connection_info(this->interface_id,
            this->destination_address, &this->destination_port)) {
        tr_debug("native interface: dest addr=%s, dest port=%d", trace_ipv6(this->destination_address), this->destination_port);
        this->native_interface = true;
    } else {
        tr_error("No remote address");
        return -4;
    }
    return 0;
}

static int thread_management_udp_proxy_receive_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    tr_debug("Recv UDP_RX.ntf");

    management_session_t *this = commissioner_find_by_service(service_id);
    uint8_t *udp_encapsulation_ptr, *udp_tmf_ptr;
    uint16_t udp_encapsulation_len, udp_tmf_len;
    uint8_t *ipv6_addr_ptr;
    uint16_t ipv6_addr_len;
    uint16_t dest_port;

    (void) source_port;

    if (!this || !source_address || !request_ptr) {
        return -1;    // goto error response
    }

    udp_encapsulation_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_UDP_ENCAPSULATION, &udp_encapsulation_ptr);
    ipv6_addr_len = thread_meshcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, MESHCOP_TLV_IPV6_ADDRESS, &ipv6_addr_ptr);

    if (udp_encapsulation_len == 0 || ipv6_addr_len < 16) {
        tr_warn("Corrupted UDP_RX.ntf received (%d, %d)", udp_encapsulation_len, ipv6_addr_len);
        return -1;
    }

    dest_port = common_read_16_bit(udp_encapsulation_ptr + 2);
    udp_tmf_len = udp_encapsulation_len - 4;
    udp_tmf_ptr = udp_encapsulation_ptr + 4;

    tr_debug("UDP_RX tmf: %s", trace_array(udp_tmf_ptr, udp_tmf_len));

    coap_service_virtual_socket_recv(this->coap_virtual_service_id, ipv6_addr_ptr, dest_port, udp_tmf_ptr, udp_tmf_len);

    return -1; // no response sent
}

/*
 * Create UDP_TX.ntf and send it to target device
 */
static int thread_management_udp_proxy_virtual_socket_send_cb(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, const uint8_t *data_ptr, uint16_t data_len)
{
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t payload_len;
    uint16_t source_port;

    management_session_t *this = commissioner_find_by_service(service_id);
    if (!this) {
        return -1;
    }

    tr_debug("UDP_TX.ntf tmf: %s", trace_array(data_ptr, data_len));
    if (!this || !destination_addr_ptr || !data_ptr) {
        return -1;
    }

    payload_len =  2 + THREAD_IPV6_ADDRESS_TLV_LENGTH + 4 + 4 + data_len; // MESHCOP_TLV_IPV6_ADDRESS + MESHCOP_TLV_UDP_ENCAPSULATION

    payload_ptr = ns_dyn_mem_alloc(payload_len);
    if (!payload_ptr) {
        return -3;
    }

    ptr = payload_ptr;

    /* MESHCOP_TLV_IPV6_ADDRESS */
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_IPV6_ADDRESS, THREAD_IPV6_ADDRESS_TLV_LENGTH, this->final_dest_address);

    /* MESHCOP_TLV_UDP_ENCAPSULATION */
    *ptr++ = MESHCOP_TLV_UDP_ENCAPSULATION;
    *ptr++ = 0xff;
    ptr = common_write_16_bit(2 + 2 + data_len, ptr); // length (Port x 2 + TMF message)
    source_port = randLIB_get_16bit(); // ephemeral port, 16-bit number
    ptr = common_write_16_bit(source_port, ptr); // source port,
    ptr = common_write_16_bit(THREAD_MANAGEMENT_PORT, ptr); // destination port
    memcpy(ptr, data_ptr, data_len);
    ptr += data_len;

    /* Send UDP_TX.ntf */
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, port,
            COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_UDP_TRANSMIT_NOTIFICATION, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);

    ns_dyn_mem_free(payload_ptr);

    return 0;
}

/**
 * Public methods
 *
 */
int thread_management_register(int8_t interface_id)
{
    int8_t instance_id = 1;
    management_session_t *this = ns_dyn_mem_alloc(sizeof(management_session_t));
    if (!this) {
        return 0;
    }
    while (management_find(instance_id)) {
        instance_id++;
    }
    this->instance_id = instance_id;
    this->interface_id = interface_id;
    this->set_response_cb_ptr = NULL;
    this->get_response_cb_ptr = NULL;

    if (thread_management_get_remote_addr(this)) {
        ns_dyn_mem_free(this);
        return -1;
    }

    ns_list_add_to_start(&instance_list, this);
    if(this->native_interface) {
        this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_COMMISSIONING_PORT, COAP_SERVICE_OPTIONS_SECURE | COAP_SERVICE_OPTIONS_SECURE_BYPASS, NULL, NULL);
        /* Register for UDP_RX.ntf */
        coap_service_register_uri(this->coap_service_id, THREAD_URI_UDP_RECVEIVE_NOTIFICATION, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_management_udp_proxy_receive_cb);
    } else {
        this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    }

    /* Create virtual service for Native/External Commissioner */
    this->coap_virtual_service_id = coap_service_initialize(this->interface_id, THREAD_COMMISSIONING_PORT, COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET, NULL, NULL);
    coap_service_virtual_socket_set_cb(this->coap_virtual_service_id, thread_management_udp_proxy_virtual_socket_send_cb);

    return instance_id;
}

int thread_management_unregister(int8_t instance_id)
{
    management_session_t *this = management_find(instance_id);
    if (!this) {
        return -1;
    }

    coap_service_delete(this->coap_service_id);

    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
    return 0;
}

int thread_management_set_security_policy(int8_t instance_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr)
{
    management_session_t *this = management_find(instance_id);
    uint8_t payload[7];/* 4 + 3 */
    uint8_t tlv[3];
    uint8_t *ptr;

    if (!this || rotation_time < 1) {
        return -1;
    }

    if (thread_management_get_remote_addr(this)) {
        return -2;
    }

    this->set_response_cb_ptr = cb_ptr;
    common_write_16_bit(rotation_time, tlv);
    tlv[2] = options;

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_SECURITY_POLICY, 3, tlv);

    tr_debug("thread management set security policy options:%d rotation time %d", options, rotation_time);
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE,this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_MANAGEMENT_SET, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_recv_set_response_cb);
    return 0;
}

int thread_management_set_steering_data(int8_t instance_id,uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr)
{
    management_session_t *this = management_find(instance_id);
    uint8_t payload[24];/* 4 + 16 + 4*/
    uint8_t *ptr;
    if (!this || steering_data_len > 16) {
        return -1;
    }

    if (thread_management_get_remote_addr(this)) {
        return -2;
    }

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur || !cur->thread_info) {
        return -3;
    }
    this->set_response_cb_ptr = cb_ptr;
    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_STEERING_DATA, steering_data_len, steering_data_ptr);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);

    tr_debug("thread management set steering data %s", trace_array(steering_data_ptr, steering_data_len));
    //default uri for thread version 1.1
    char *uri = THREAD_URI_COMMISSIONER_SET;

    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_recv_set_response_cb);

    return 0;
}

int thread_management_set_commissioning_data_timestamp(int8_t instance_id, uint64_t time, management_set_response_cb *cb_ptr)
{
    management_session_t *this = management_find(instance_id);
    uint8_t payload[12];/* 4 + 8 */
    uint8_t *ptr;

    if (!this) {
        return -1;
    }

    if (thread_management_get_remote_addr(this)) {
        return -2;
    }

    this->set_response_cb_ptr = cb_ptr;
    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_ACTIVE_TIME_STAMP, 8, (uint8_t *)&time); //TODO not network order?

    tr_debug("thread management set commissioning timestamp %"PRIu64, time);
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_MANAGEMENT_SET, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_recv_set_response_cb);
    return 0;
}

int thread_management_get(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr)
{
    management_session_t *this = management_find(instance_id);
    uint8_t payload[36];/* 4 + 32 */
    uint8_t *ptr;
    int8_t service_id;
    bool wrap_to_udp_tx = false; // messages to Border Agent are send directly without wrapping to UDP_TX

    if (!this || fields_count > 32 ) {
        return -1;
    }

    if (!uri_ptr) {
    	uri_ptr = THREAD_URI_ACTIVE_GET;
    }

    if(!dst_addr) {
        if (thread_management_get_remote_addr(this)) {
            return -2;
        }
        memcpy(this->final_dest_address, this->destination_address, 16);
    }
    else{
        if (this->native_interface) {
            memcpy(this->final_dest_address, dst_addr, 16);
            wrap_to_udp_tx = true;
        } else {
            memcpy(this->destination_address, dst_addr, 16);
            this->destination_port = THREAD_MANAGEMENT_PORT;
        }
    }

    this->get_response_cb_ptr = cb_ptr;
    ptr = payload;
    if (fields_count > 0 && fields_ptr) {
        ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_GET, fields_count, fields_ptr);
    }

    tr_debug("thread management info get TLVs: %s, uri: %s", trace_array(fields_ptr, fields_count), uri_ptr);
    tr_debug("dest_addr: %s", trace_ipv6(this->destination_address));

    if (this->native_interface && wrap_to_udp_tx) {
        service_id = this->coap_virtual_service_id;
    } else {
        service_id = this->coap_service_id;
    }

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, payload, ptr - payload, thread_management_recv_get_response_cb);
    return 0;
}

int thread_management_set(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *data_ptr, uint8_t data_len, management_set_response_cb *cb_ptr)
{
    management_session_t *this = management_find(instance_id);
    int8_t service_id;
    bool wrap_to_udp_tx = false;

    if (!this || !data_ptr || data_len < 2) {
        return -1;
    }
    if (uri_ptr == NULL){
    	uri_ptr = THREAD_URI_MANAGEMENT_SET;
    }

    if(!dst_addr){
        if (thread_management_get_remote_addr(this)) {
            return -2;
        }
        memcpy(this->final_dest_address, this->destination_address, 16);
    }
    else{
        if (this->native_interface) {
            // native commissioner sending to address, need to encapsulate
            memcpy(this->final_dest_address, dst_addr, 16);
            wrap_to_udp_tx = true;
        } else {
            memcpy(this->destination_address, dst_addr, 16);
            this->destination_port = THREAD_MANAGEMENT_PORT;
        }
    }

    this->set_response_cb_ptr = cb_ptr;

    tr_debug("thread management set");

    if (this->native_interface && wrap_to_udp_tx) {
        // for non-native commissioner use virtual service to encapsulate messaes
        service_id = this->coap_virtual_service_id;
    } else {
        // for on-mesh commissioner use real service
        service_id = this->coap_service_id;
    }

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, data_ptr, data_len, thread_management_recv_set_response_cb);
    return 0;
}
#else
int thread_management_register(int8_t interface_id) {
    (void)interface_id;
    return -1;
}

int thread_management_unregister(int8_t instance_id) {
    (void)instance_id;
    return -1;
}

int thread_management_set_security_policy(int8_t instance_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr) {
    (void)instance_id;
    (void) options;
    (void)rotation_time;
    (void)cb_ptr;
    return -1;
}

int thread_management_set_steering_data(int8_t instance_id, uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr) {
    (void)instance_id;
    (void) session_id;
    (void) steering_data_ptr;
    (void)steering_data_len;
    (void)cb_ptr;
    return -1;
}

int thread_management_set_commissioning_data_timestamp(int8_t instance_id, uint64_t time, management_set_response_cb *cb_ptr) {
    (void)instance_id;
    (void) time;
    (void)cb_ptr;
    return -1;
}

int thread_management_get(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *fields_ptr, uint8_t fields_count, management_get_response_cb *cb_ptr)
{
    (void) instance_id;
    (void) dst_addr;
    (void) uri_ptr;
    (void) fields_ptr;
    (void) fields_count;
    (void) cb_ptr;
    return -1;
}

int thread_management_set(int8_t instance_id, uint8_t dst_addr[static 16], char *uri_ptr, uint8_t *data_ptr, uint8_t data_len, management_set_response_cb *cb_ptr)
{
    (void) instance_id;
    (void) dst_addr;
    (void) uri_ptr;
    (void) data_ptr;
    (void) data_len;
    (void) cb_ptr;
    return -1;
}

#endif
