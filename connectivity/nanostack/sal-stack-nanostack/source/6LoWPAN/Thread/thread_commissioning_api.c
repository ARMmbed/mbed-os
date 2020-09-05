/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
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

/*Nanostack includes*/
#include "nsconfig.h"

#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "common_functions.h"
#include "ns_sha256.h"

/*thread includes*/
#include "thread_config.h"
#include "thread_management_if.h"
#include "thread_meshcop_lib.h"
#include "thread_management_api.h"
#include "thread_commissioning_api.h"
#include "thread_beacon.h"
/*Private includes*/
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"// Get coordinator address

#define TRACE_GROUP TRACE_GROUP_THREAD_COMMISSIONING_API
#ifdef HAVE_THREAD

#include "coap_service_api.h"

typedef struct device {
    uint8_t PSKd[32];
    uint8_t kek[32];// shal libs needs 32 bytes kek is first 16
    uint8_t EUI64[8];
    uint8_t IID[8];
    thread_commissioning_joiner_finalisation_cb *joining_device_cb_ptr;
    uint16_t joiner_router_rloc;
    uint8_t PSKd_len;
    bool send_kek: 1;
    bool short_eui64: 1;/*< Use short format for bloom filter creation*/

    ns_list_link_t link;
} device_t;

typedef NS_LIST_HEAD(device_t, link) device_list_t;

typedef struct commissioner_entry {
    device_list_t device_list;
    uint8_t destination_address[16];
    uint8_t final_dest_address[16]; // Relay message final destination
    uint8_t leader_address[16]; // leader ALOC for contacting the leader
    uint8_t PSKc_ptr[16];
    thread_commissioning_status_cb *status_cb_ptr;
    uint16_t destination_port;
    uint16_t session_id;
    int8_t management_instance;
    int8_t interface_id;
    int8_t coap_service_id;
    int8_t coap_secure_service_id;
    int8_t coap_secure_virtual_service_id;
    int8_t coap_udp_proxy_service_id;
    bool registered: 1;
    bool native_commissioner: 1;

    ns_list_link_t link;
} commissioner_t;

static NS_LIST_DEFINE(instance_list, commissioner_t, link);

const uint8_t any_device[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/*
 * Commissioned device handlers;
 */
static device_t *device_create(commissioner_t *commissioner_ptr)
{
    device_t *device_ptr = ns_dyn_mem_alloc(sizeof(device_t));
    if (device_ptr) {
        memset(device_ptr, 0, sizeof(device_t));
        device_ptr->send_kek = false;
        device_ptr->joining_device_cb_ptr = NULL;
        ns_list_add_to_start(&commissioner_ptr->device_list, device_ptr);
    }
    return device_ptr;
}

static void device_delete(commissioner_t *commissioner_ptr, device_t *device_ptr)
{
    if (device_ptr) {
        ns_list_remove(&commissioner_ptr->device_list, device_ptr);
        ns_dyn_mem_free(device_ptr);
    }
    return;
}

static device_t *device_find(commissioner_t *commissioner_ptr, uint8_t EUI64[8])
{
    device_t *this = NULL;
    if (!commissioner_ptr || !EUI64) {
        return NULL;
    }

    ns_list_foreach(device_t, cur_ptr, &commissioner_ptr->device_list) {
        if (memcmp(cur_ptr->EUI64, EUI64, 8) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static device_t *device_find_by_iid(commissioner_t *commissioner_ptr, uint8_t IID[8])
{
    device_t *this = NULL;
    if (!IID) {
        return NULL;
    }
    ns_list_foreach(device_t, cur_ptr, &commissioner_ptr->device_list) {
        tr_debug("device iid %s and commissioner iid %s", trace_array(cur_ptr->IID, 8), trace_array(IID, 8));
        if (memcmp(cur_ptr->IID, IID, 8) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
#define device_get_first(commissioner) ns_list_get_first(&commissioner->device_list)


/* Commissioner class handlers*/
static commissioner_t *commissioner_find(int8_t interface_id)
{
    commissioner_t *this = NULL;
    ns_list_foreach(commissioner_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static commissioner_t *commissioner_find_by_service(int8_t service_id)
{
    commissioner_t *this = NULL;
    ns_list_foreach(commissioner_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id || cur_ptr->coap_secure_virtual_service_id == service_id || cur_ptr->coap_secure_service_id == service_id || cur_ptr->coap_udp_proxy_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}
static commissioner_t *commissioner_create(int8_t interface_id)
{
    commissioner_t *this = ns_dyn_mem_alloc(sizeof(commissioner_t));
    if (this) {
        memset(this, 0, sizeof(commissioner_t));
        this->interface_id = interface_id;
        this->registered = false;
        ns_list_init(&this->device_list);
        ns_list_add_to_start(&instance_list, this);
    }
    return this;
}

static void commissioner_delete(commissioner_t *this)
{
    if (this) {
        device_t *device_ptr = device_get_first(this);
        while (device_ptr != NULL) {
            device_delete(this, device_ptr);
            device_ptr = device_get_first(this);
        }
        ns_list_remove(&instance_list, this);
        ns_dyn_mem_free(this);
    }
    return;
}

/*Internal helper functions*/
static int commissioning_meshcop_map_state(commissioning_state_e state)
{
    if (state == COMMISSIONING_STATE_ACCEPT) {
        return 1;
    } else if (state == COMMISSIONING_STATE_PENDING) {
        return 0;
    }

    return -1;
}


static int commission_finalisation_resp_send(int8_t coap_service_id, device_t *device_ptr, sn_coap_hdr_s *request_ptr, commissioning_state_e state)
{
    commissioner_t *this = commissioner_find_by_service(coap_service_id);
    uint8_t payload[11];// 4 + 1 + 4 + 2
    uint8_t *ptr;

    if (!this || !request_ptr || !device_ptr) {
        return -1;
    }

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, (uint8_t)commissioning_meshcop_map_state(state));

    thci_trace("Device - Comm|Direction - sent|EUI - %s|Type - JOIN_FIN.resp|Length - %d|Payload - %s", trace_array(device_ptr->EUI64, 8), (int)(ptr - payload), trace_array(payload, ptr - payload));
    tr_debug("finalisation response send state:%d value:%d ", state, (uint8_t)commissioning_meshcop_map_state(state));
    coap_service_response_send(coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

/*
 * Callback functions
*/

static int thread_commissioning_active_get_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    commissioning_state_e state = COMMISSIONING_STATE_REJECT;// Default is accept if we get error it is rejected
    uint8_t *ptr;
    uint16_t len;
    (void) source_address;
    (void) source_port;

    /* Transaction failed */
    if (!response_ptr) {
        return -1;
    }

    tr_debug("management get response from comm module");
    if (!this) {
        return -2;
    }

    if ((len = thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_NETWORK_MESH_LOCAL_ULA, &ptr)) > 0) {
        state = COMMISSIONING_STATE_ACCEPT;
        tr_debug(" TLV ml prefix=%s\r\n", trace_array(ptr, len));
        memcpy(this->leader_address, ptr, 8);
        memcpy(this->leader_address + 8, ADDR_SHORT_ADR_SUFFIC, 6);
        common_write_16_bit(0xfc00, this->leader_address + 14);
    }

    if (this->status_cb_ptr) {
        this->status_cb_ptr(this->interface_id, this->session_id, state);
    }
    return 0;
}

static int commissioning_leader_petition_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    commissioning_state_e state = COMMISSIONING_STATE_REJECT;
    uint16_t session_id = 0;
    uint8_t *ptr = NULL;
    char *uri_ptr = THREAD_URI_ACTIVE_GET;
    (void) source_address;
    (void) source_port;

    tr_debug("Thread Petition response received service %d", service_id);
    if (!this) {
        return -1;
    }

    if (!response_ptr || !response_ptr->payload_ptr || response_ptr->payload_len < 1) {
        tr_debug("invalid response");
        thci_trace("commissionerPetitionError");
        goto user_response;
    }

    if ((2 <= thread_meshcop_tlv_data_get_uint16(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_COMMISSIONER_SESSION_ID, &session_id)) &&
            (1 <= thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &ptr) && *ptr == 1)) {
        state = COMMISSIONING_STATE_ACCEPT;
        thci_trace("commissionerPetitionAccepted");
        this->session_id = session_id;
        this->registered = true;
        //@TODO order keep alive timer for the message and start sending it periodically
    }
    tr_debug("petition response session_id: %d state:%d", session_id, state);

    // if registered and native commissioner send ACTIVE_GET to BBR to get mesh parameters
    // if not native set leader ALOC from stack
    if (this->native_commissioner) {
        coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                                  COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, NULL, 0, thread_commissioning_active_get_cb);
        return 0;
    } else {
        thread_management_get_leader_aloc(this->interface_id, this->leader_address);
    }

user_response:
    if (state == COMMISSIONING_STATE_REJECT) {
        thci_trace("commissionerPetitionRejected");
    }
    if (this->status_cb_ptr) {
        this->status_cb_ptr(this->interface_id, this->session_id, state);
    }
    return 0;
}

static int commissioning_keep_alive_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    uint8_t *ptr;
    (void) source_address;
    (void) source_port;

    tr_debug("Thread keep alive response received");
    if (!this) {
        tr_warn("commissioner service missing!");
        thci_trace("commissionerError");
        return -1;
    }
    if (!response_ptr) {
        tr_warn("commission ka response without ptr!");
        thci_trace("commissionerError");
        return -1;
    }

    if (1 <= thread_meshcop_tlv_find(response_ptr->payload_ptr, response_ptr->payload_len, MESHCOP_TLV_STATE, &ptr) && *ptr == 1) {
        // only handle success
        return 0;
    }

    this->session_id = 0;
    this->registered = false;
    if (this->status_cb_ptr) {
        this->status_cb_ptr(this->interface_id, this->session_id, COMMISSIONING_STATE_REJECT);
    }

    return -2;
}

static int commission_finalisation_req_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    uint8_t address[16];
    device_t *device_ptr;
    int ret = -1;// always reject
    (void) source_port;

    tr_debug("joiner finalisation recv addr %s", trace_ipv6(source_address));
    if (!this || !source_address || !request_ptr) {
        return -1;
    }

    device_ptr = device_find_by_iid(this, &source_address[8]);
    if (device_ptr) {
        ret = 0;    // accept even without application confirmation
    }
    thci_trace("Device - Comm|Direction - recv|EUI - %s|Type - JOIN_FIN.req|Length - %d|Payload - %s", trace_array(device_ptr->EUI64, 8), request_ptr->payload_len, trace_array(request_ptr->payload_ptr, request_ptr->payload_len));
    memcpy(address, source_address, 16);

    if (device_ptr) {
        if (device_ptr->joining_device_cb_ptr) {
            ret = device_ptr->joining_device_cb_ptr(this->interface_id, &source_address[8], request_ptr->payload_ptr, request_ptr->payload_len);
        }

        device_ptr->send_kek = true;
    }

    if (ret == 0) {
        thci_trace("commissionerJoinerAccepted");
    }

    commission_finalisation_resp_send(service_id, device_ptr, request_ptr, ret == 0 ? COMMISSIONING_STATE_ACCEPT : COMMISSIONING_STATE_REJECT);
    coap_service_close_secure_connection(service_id, address, source_port);

    return 0;
}
static int commission_application_provision_req_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    uint8_t payload[11];// 4 + 1 + 4 + 2
    uint8_t *ptr;

    (void) source_port;

    tr_debug("joiner application provisioning recv addr %s", trace_ipv6(source_address));
    if (!this || !source_address || !request_ptr) {
        return -1;
    }

    thci_trace("Device - Comm|Direction - recv|EUI - %s|Type - JOIN_APP.req|Length - %d|Payload - %s", trace_array(&source_address[8], 8), request_ptr->payload_len, trace_array(request_ptr->payload_ptr, request_ptr->payload_len));

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, 1);

    thci_trace("Device - Comm|Direction - sent|EUI - %s|Type - JOIN_APP.resp|Length - %d|Payload - %s", trace_array(&source_address[8], 8), (int)(ptr - payload), trace_array(payload, ptr - payload));
    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM, payload, ptr - payload);
    return 0;
}

static int commission_dataset_changed_notify_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void)source_address;
    (void)source_port;

    tr_debug("Dataset changed - notification received from: %s", trace_ipv6(source_address));
    commissioner_t *this = commissioner_find_by_service(service_id);

    if (!this) {
        return -1;
    }

    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_ACTIVE_GET, COAP_CT_OCTET_STREAM, NULL, 0, thread_commissioning_active_get_cb);

    coap_service_response_send(service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_NONE, NULL, 0);

    return 0;
}
static int thread_commission_udp_proxy_receive_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    tr_debug("Recv UDP_RX.ntf");

    commissioner_t *this = commissioner_find_by_service(service_id);
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

    coap_service_virtual_socket_recv(this->coap_udp_proxy_service_id, ipv6_addr_ptr, dest_port, udp_tmf_ptr, udp_tmf_len);

    return -1; // no response sent
}

static uint8_t *bloom_filter_calculate(uint8_t *bloom_filter_ptr, device_list_t device_list, int *steering_tlv_max_length)
{
    memset(bloom_filter_ptr, 0, *steering_tlv_max_length);
    ns_list_foreach(device_t, cur_ptr, &device_list) {
        if (memcmp(cur_ptr->EUI64, any_device, 8) != 0) {
            tr_debug("eui64 used on commissioning side = %s", trace_array(cur_ptr->EUI64, 8));
            cur_ptr->IID[0] |= 2; //Changed IID to MAC extended address for bloom filter calculation
            thread_beacon_calculate_bloom_filter(bloom_filter_ptr, *steering_tlv_max_length, cur_ptr->IID, 8);
            cur_ptr->IID[0] &= ~2;//Restore IID
        } else {
            bloom_filter_ptr[0] = 0xff;
            *steering_tlv_max_length = 1;
            break;
        }
    }

    return bloom_filter_ptr;
}
static int thread_commissioner_set_steering_data(commissioner_t *this, uint16_t session_id, uint8_t *steering_data_ptr, uint8_t steering_data_len)
{
    uint8_t payload[24];/* 4 + 16 + 4*/
    uint8_t *ptr;
    int8_t coap_service_id;
    if (!this || steering_data_len > 16) {
        return -1;
    }

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_STEERING_DATA, steering_data_len, steering_data_ptr);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, session_id);

    tr_debug("thread commissioner set steering data %s", trace_array(steering_data_ptr, steering_data_len));
    memcpy(this->final_dest_address, this->leader_address, 16);
    //default uri for thread version 1.1
    char *uri = THREAD_URI_COMMISSIONER_SET;

    if (this->native_commissioner) {
        coap_service_id = this->coap_udp_proxy_service_id;
    } else {
        coap_service_id = this->coap_service_id;
    }

    coap_service_request_send(coap_service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri, COAP_CT_OCTET_STREAM, payload, ptr - payload, NULL);

    return 0;
}

static int commission_steering_data_update(commissioner_t *this)
{
    //variable for steering tlv length - maximum possible length is 16 bytes - section 8.10.1.9 Max length
    int steering_tlv_length = 16;
    uint8_t bloom_filter_ptr[16];
    int ret;

    //bloom filter calculation function call
    bloom_filter_calculate(bloom_filter_ptr, this->device_list, &steering_tlv_length);
    tr_debug("Steering bloom set :%s", trace_array(bloom_filter_ptr, 16));
    ret = thread_commissioner_set_steering_data(this, this->session_id, bloom_filter_ptr, steering_tlv_length);
    if (ret) {
        tr_warn("Steering data set failed %d", ret);
        return -1;
    }

    return 0;
}

/*
 * These functions are the relay endpoint for commissioner device.
 * this can be either:
 * secure native commissioner
 * or unsecure native commissioner
 * */
static int commission_relay_rx_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    uint8_t joiner_address[16] = { 0xfe, 0x80 };
    uint8_t *joiner_iid_ptr;
    uint8_t *udp_ptr;
    uint16_t udp_len;
    uint16_t joiner_port;
    uint16_t joiner_router_rloc;
    device_t *device_ptr;
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
    tr_debug("Relay RX from (%x) addr %s, port %d, len:%d", joiner_router_rloc, trace_ipv6(joiner_address), joiner_port, udp_len);

    device_ptr = device_find_by_iid(this, joiner_iid_ptr);
    if (!device_ptr) {
        tr_warn("unknown device connected");
        //Interop HACK
        device_ptr = device_find(this, (uint8_t *)any_device);
        if (!device_ptr) {
            tr_warn("No catch all device added");
            return -1;
        }

        memcpy(device_ptr->IID, &joiner_address[8], 8);
        memcpy(device_ptr->EUI64, &joiner_address[8], 8); // This is wrong as we cannot reverse the SHA1 But ok For Any device
        device_ptr->EUI64[0] ^= 2;// We are searching for list of eui64 so need to flip
        if (commission_steering_data_update(this) != 0) {
            return -1;
        }
        tr_warn("catching device for iid:%s", trace_array(&joiner_address[8], 8));
    }
    device_ptr->joiner_router_rloc = joiner_router_rloc;
    coap_service_virtual_socket_recv(this->coap_secure_virtual_service_id, joiner_address, joiner_port, udp_ptr, udp_len);
    return -1; // no response sent
}

static int commission_virtual_socket_send_cb(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, const uint8_t *data_ptr, uint16_t data_len)
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    uint8_t destination_address[16];
    uint16_t destination_port;
    device_t *device_ptr;
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t payload_len;
    uint8_t destination_service_id;

    tr_debug("Relay RX send addr %s, port %d, len:%d", trace_ipv6(destination_addr_ptr), port, data_len);
    if (!this || !destination_addr_ptr || !data_ptr) {
        return -1;
    }

    device_ptr = device_find_by_iid(this, &destination_addr_ptr[8]);
    if (!device_ptr) {
        return -2;
    }
    payload_len =  4 + data_len + 4 + 8 + 4 + 2;

    if (device_ptr->send_kek) {
        payload_len += 4 + 16;
    }

    payload_ptr = ns_dyn_mem_alloc(payload_len);
    if (!payload_ptr) {
        return -3;
    }
    if (this->native_commissioner) {
        destination_service_id = this->coap_secure_service_id;
        memcpy(destination_address, this->destination_address, 16);
        destination_port = this->destination_port;
    } else {
        memset(destination_address, 0, 16);
        destination_service_id = this->coap_service_id;
        thread_management_get_ml_prefix(this->interface_id, destination_address);
        common_write_16_bit(0xfffe, &destination_address[11]);
        common_write_16_bit(device_ptr->joiner_router_rloc, &destination_address[14]);
        destination_port = THREAD_MANAGEMENT_PORT;
    }

    tr_debug("Relay destination %s:%d", trace_ipv6(destination_address), destination_port);

    ptr = payload_ptr;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ENCAPSULATION, data_len, data_ptr);
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_IID, 8, &destination_addr_ptr[8]);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_UDP_PORT, port);
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_JOINER_ROUTER_LOCATOR, device_ptr->joiner_router_rloc);

    if (device_ptr->send_kek) {
        tr_debug("Relay RX Send KEK");
        ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_JOINER_ROUTER_KEK, 16, device_ptr->kek);
        device_ptr->send_kek = false;
    }

    coap_service_request_send(destination_service_id, COAP_REQUEST_OPTIONS_NONE, destination_address, destination_port,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_RELAY_TRANSMIT, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);

    ns_dyn_mem_free(payload_ptr);

    return 0;
}

static int commissioning_security_done_cb(int8_t service_id, uint8_t address[static 16], uint8_t keyblock[static 40])
{
    commissioner_t *this = commissioner_find_by_service(service_id);
    device_t *device_ptr;

    if (!this) {
        return -1;
    }

    tr_debug("Commissioner security done addr %s keyblock %s", trace_ipv6(address), trace_array(keyblock, 40));
    device_ptr = device_find_by_iid(this, &address[8]);
    if (!device_ptr) {
        tr_debug("Commissioner device not found");
        thci_trace("commissionerBrError");
        return -1;
    }
    ns_sha256(keyblock, 40, device_ptr->kek);

    return 0;
}

static int joiner_commissioner_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    int ret = -1;
    (void)port;
    tr_info("commissionerJoinerDtlsSessionStarted");

    commissioner_t *this = commissioner_find_by_service(service_id);

    if (!this) {
        return ret;
    }

    device_t *device_ptr = device_find_by_iid(this, &address[8]);

    if (device_ptr) {
        memcpy(pw, device_ptr->PSKd, device_ptr->PSKd_len);
        *pw_len = device_ptr->PSKd_len;
        ret = 0;
//        ret = coap_service_security_key_set( service_id, address, port, device_ptr->PSKd, device_ptr->PSKd_len );
    }

    return ret;
}

static int commissioner_br_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t *pw, uint8_t *pw_len)
{
    int ret = -1;
    (void)address;
    (void)port;
    tr_info("commissionerBrDtlsSessionStarted");
    commissioner_t *this = commissioner_find_by_service(service_id);
    if (this) {
        memcpy(pw, this->PSKc_ptr, 16);
        *pw_len = 16;
        ret = 0;
//        ret = coap_service_security_key_set( service_id, address, port, this->PSKc_ptr, this->PSKc_len );
    }

    return ret;
}

static int commissioner_br_security_done_cb(int8_t service_id, uint8_t address[16], uint8_t keyblock[static 40])
{
    (void)service_id;
    (void)address;
    (void)keyblock;
    thci_trace("commissionerBrAccepted");
    thci_trace("commissionerPetitionStarted");
    return 0;
}

static int thread_commission_udp_proxy_virtual_socket_send_cb(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, const uint8_t *data_ptr, uint16_t data_len)
{
    (void) port;
    uint8_t *payload_ptr;
    uint8_t *ptr;
    uint16_t payload_len;
    uint16_t source_port;

    commissioner_t *this = commissioner_find_by_service(service_id);
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

    tr_debug("br_address %s final dest_address %s and port %d", trace_ipv6(this->destination_address),
             trace_ipv6(this->final_dest_address), this->destination_port);

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
    coap_service_request_send(this->coap_secure_service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_NON_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, THREAD_URI_UDP_TRANSMIT_NOTIFICATION, COAP_CT_OCTET_STREAM, payload_ptr, ptr - payload_ptr, NULL);

    ns_dyn_mem_free(payload_ptr);

    return 0;
}

/*
Public api functions
*/
int thread_commissioning_register(int8_t interface_id, uint8_t PSKc[static 16])
{
    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        this = commissioner_create(interface_id);
    }
    if (!this) {
        return -2;
    }
    memcpy(this->PSKc_ptr, PSKc, 16);
    if (this->registered) {
        return 0;
    }
    this->coap_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_NONE, NULL, NULL);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_RELAY_RECEIVE, COAP_SERVICE_ACCESS_POST_ALLOWED, commission_relay_rx_recv_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_JOINER_APPLICATION_REQUEST, COAP_SERVICE_ACCESS_POST_ALLOWED, commission_application_provision_req_recv_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_DATASET_CHANGED, COAP_SERVICE_ACCESS_POST_ALLOWED, commission_dataset_changed_notify_recv_cb);

    this->coap_secure_service_id = coap_service_initialize(this->interface_id, THREAD_COMMISSIONING_PORT, COAP_SERVICE_OPTIONS_SECURE | COAP_SERVICE_OPTIONS_SECURE_BYPASS, commissioner_br_security_start_cb, commissioner_br_security_done_cb);
    coap_service_register_uri(this->coap_secure_service_id, THREAD_URI_RELAY_RECEIVE, COAP_SERVICE_ACCESS_POST_ALLOWED, commission_relay_rx_recv_cb);
    coap_service_register_uri(this->coap_secure_service_id, THREAD_URI_UDP_RECVEIVE_NOTIFICATION, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_commission_udp_proxy_receive_cb);

    this->coap_secure_virtual_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_SECURE | COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET, joiner_commissioner_security_start_cb, commissioning_security_done_cb);
    coap_service_register_uri(this->coap_secure_virtual_service_id, THREAD_URI_JOINER_FINALIZATION, COAP_SERVICE_ACCESS_POST_ALLOWED, commission_finalisation_req_recv_cb);
    coap_service_virtual_socket_set_cb(this->coap_secure_virtual_service_id, commission_virtual_socket_send_cb);

    this->coap_udp_proxy_service_id = coap_service_initialize(this->interface_id, THREAD_MANAGEMENT_PORT, COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET, NULL, NULL);
    coap_service_virtual_socket_set_cb(this->coap_udp_proxy_service_id, thread_commission_udp_proxy_virtual_socket_send_cb);

    return 0;
}

int thread_commissioning_unregister(int8_t interface_id)
{
    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        return -1;
    }
    if (this->registered) {
        // Unregister the commissioner
        thread_commissioning_petition_keep_alive(this->interface_id, COMMISSIONING_STATE_REJECT);
    }

    coap_service_delete(this->coap_service_id);
    coap_service_delete(this->coap_secure_service_id);
    coap_service_delete(this->coap_secure_virtual_service_id);
    coap_service_delete(this->coap_udp_proxy_service_id);

    commissioner_delete(this);
    return 0;
}

int thread_commissioning_petition_start(int8_t interface_id, char *commissioner_id_ptr, thread_commissioning_status_cb *status_cb_ptr)
{
    commissioner_t *this;
    uint8_t payload[68];// max length for commissioner id is 64 + 4 byte header
    uint8_t commissioner_id_length;
    uint8_t service_id;
    uint8_t *ptr;
    char *uri_ptr;
    this = commissioner_find(interface_id);

    if (!this) {
        return -1;
    }
    if (!commissioner_id_ptr) {
        return -2;
    }
    commissioner_id_length = strlen(commissioner_id_ptr);

    if (commissioner_id_length > 64) {
        return -3;
    }

    if (this->native_commissioner) {
        uri_ptr = THREAD_URI_COMMISSIONER_PETITION;
        service_id = this->coap_secure_service_id;
    } else {
        uri_ptr = THREAD_URI_LEADER_PETITION;
        service_id = this->coap_service_id;
        thread_management_get_leader_aloc(this->interface_id, this->destination_address);
        this->destination_port = THREAD_MANAGEMENT_PORT;
    }

    this->status_cb_ptr = status_cb_ptr;
    ptr = payload;
    ptr = thread_meshcop_tlv_data_write(ptr, MESHCOP_TLV_COMMISSIONER_ID, commissioner_id_length, (uint8_t *) commissioner_id_ptr);

    tr_debug("Thread Petition send to %s:%d id %s ", trace_ipv6(this->destination_address), this->destination_port, commissioner_id_ptr);

    //TODO there must be way to set PSKc for request
    //TODO there must be way to make client transactions with security and no security
    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, payload, ptr - payload, commissioning_leader_petition_recv_cb);
    return 0;
}

int thread_commissioning_petition_keep_alive(int8_t interface_id, commissioning_state_e state)
{
    commissioner_t *this;
    uint8_t payload[7];
    uint8_t *ptr;
    uint8_t service_id;
    char *uri_ptr;
    /* length is
       state tlv 3
       session id tlv 4
    */
    this = commissioner_find(interface_id);

    if (!this) {
        return -1;
    }

    if (this->native_commissioner) {
        uri_ptr = THREAD_URI_COMMISSIONER_KEEP_ALIVE;
        service_id = this->coap_secure_service_id;
    } else {
        uri_ptr = THREAD_URI_LEADER_KEEP_ALIVE;
        service_id = this->coap_service_id;
        thread_management_get_leader_aloc(this->interface_id, this->destination_address);
    }

    ptr = payload;
    ptr = thread_meshcop_tlv_data_write_uint8(ptr, MESHCOP_TLV_STATE, (uint8_t)commissioning_meshcop_map_state(state));
    ptr = thread_meshcop_tlv_data_write_uint16(ptr, MESHCOP_TLV_COMMISSIONER_SESSION_ID, this->session_id);

    tr_debug("Commissioner keep alive send to %s:%d, session_id=%d, state=%d",
             trace_ipv6(this->destination_address), this->destination_port,
             this->session_id, state);
    thci_trace("commissionerKeepAliveSent");
    if (state != COMMISSIONING_STATE_ACCEPT) {
        this->session_id = 0;
    }
    coap_service_request_send(service_id, COAP_REQUEST_OPTIONS_NONE, this->destination_address, this->destination_port,
                              COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST, uri_ptr, COAP_CT_OCTET_STREAM, payload, ptr - payload, commissioning_keep_alive_recv_cb);
    return 0;
}

/*
 * PSKD (Pre-Shared Key -Device). A PSKD can be a minimum of six (6) uppercase alphanumeric characters long
 *
 * */
int thread_commissioning_device_add(int8_t interface_id, bool short_eui64, uint8_t EUI64[static 8], uint8_t *PSKd_ptr, uint8_t PSKd_len, thread_commissioning_joiner_finalisation_cb *joining_device_cb_ptr)
{
    commissioner_t *this;
    device_t *device_ptr = NULL;

    this = commissioner_find(interface_id);
    if (!this || PSKd_len < 1 || PSKd_len > 32 || !PSKd_ptr) {
        return -1;
    }

    if (memcmp(EUI64, any_device, 8) == 0) {
        // always add 'any' device to allow multiple 'any' devices
        device_ptr = device_create(this);
    }

    if (!device_ptr) {
        // try to find an existing device
        device_ptr = device_find(this, EUI64);
    }

    if (!device_ptr) {
        // create device
        device_ptr = device_create(this);
    }

    if (!device_ptr) {
        return -2;
    }

    memcpy(device_ptr->EUI64, EUI64, 8);
    if (memcmp(EUI64, any_device, 8) != 0) {
        ns_sha256_nbits(EUI64, 8, device_ptr->IID, 64);
        device_ptr->IID[0] &= ~2; //local administered bit is set in MAC and flipped in IID
    } else {
        memcpy(device_ptr->IID, any_device, 8);
    }

    memcpy(device_ptr->PSKd, PSKd_ptr, PSKd_len);
    device_ptr->PSKd_len = PSKd_len;
    device_ptr->short_eui64 = short_eui64;
    device_ptr->joining_device_cb_ptr = joining_device_cb_ptr;

    if (commission_steering_data_update(this) != 0) {
        return -2;
    }
    thci_trace("commissionerNwkDataSynced");
    return 0;
}

int thread_commissioning_device_delete(int8_t interface_id, uint8_t EUI64[static 8])
{
    commissioner_t *this = commissioner_find(interface_id);

    if (!this) {
        return -1;
    }

    tr_debug("delete commissioned device EUI64:%s", trace_array(EUI64, 8));
    device_delete(this, device_find(this, EUI64));

    if (commission_steering_data_update(this) != 0) {
        return -2;
    }
    return 0;
}


void *thread_commission_device_get_next(void *ptr, int8_t interface_id, bool *short_eui64, uint8_t EUI64[8], uint8_t PSKd[32], uint8_t *PSKd_len)
{
    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        return NULL;
    }
    device_t *cur_ptr = (device_t *)ptr;
    if (cur_ptr == NULL) {
        cur_ptr = (device_t *)ns_list_get_first(&this->device_list);
    } else {
        cur_ptr = (device_t *)ns_list_get_next(&this->device_list, cur_ptr);
    }
    if (!cur_ptr) {
        return NULL;
    }
    if (short_eui64) {
        *short_eui64 = cur_ptr->short_eui64;
    }
    if (EUI64) {
        memcpy(EUI64, cur_ptr->EUI64, 8);
    }
    if (PSKd) {
        memcpy(PSKd, cur_ptr->PSKd, 32);
    }
    if (PSKd_len) {
        *PSKd_len = cur_ptr->PSKd_len;
    }

    return cur_ptr;
}

int thread_commissioning_attach(int8_t interface_id, uint8_t *destination_address, uint16_t destination_port)
{
    tr_debug("start ethernet commissioner attach");
    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        return -1;
    }
    memcpy(this->destination_address, destination_address, 16);
    this->destination_port = destination_port;
    this->native_commissioner = true;
    return 0;

}

int thread_commissioning_native_commissioner_start(int8_t interface_id, thread_commissioning_native_select_cb *cb_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    tr_debug("start native commissioner scanning");
    if (!cur || !cur->thread_info) {
        return -1;
    }
    cur->thread_info->native_commissioner_link = NULL;
    cur->thread_info->native_commissioner_cb = cb_ptr;
    //TODO check if bootstrap is connected then we have to drop from old network and trigger scanning
    return 0;

}

int thread_commissioning_native_commissioner_stop(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    tr_debug("stop native commissioner scanning");
    if (!cur || !cur->thread_info) {
        return -1;
    }
    ns_dyn_mem_free(cur->thread_info->native_commissioner_link);
    cur->thread_info->native_commissioner_link = NULL;
    cur->thread_info->native_commissioner_cb = NULL;
    return 0;
}

int thread_commissioning_native_commissioner_connect(int8_t interface_id, thread_commissioning_link_configuration_s *link_ptr)
{
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    tr_debug("connect native commissioner");
    if (!cur || !cur->thread_info) {
        return -1;
    }
    cur->thread_info->native_commissioner_link = ns_dyn_mem_alloc(sizeof(thread_commissioning_link_configuration_s));
    if (!cur->thread_info->native_commissioner_link) {
        return -2;
    }
    *cur->thread_info->native_commissioner_link = *link_ptr;

    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        this = commissioner_create(interface_id);
    }
    if (!this) {
        return -3;
    }

    this->native_commissioner = true;
    memcpy(this->destination_address, link_ptr->destination_address, 16);
    this->destination_port = link_ptr->destination_port;
    //TODO check that we are scanning for networks and reset backup timers

    return 0;
}

int thread_commissioning_native_commissioner_get_connection_info(int8_t interface_id, uint8_t *address_ptr, uint16_t *port)
{
    commissioner_t *this = commissioner_find(interface_id);
    tr_debug("get native connection info");
    if (!this) {
        return -1;
    }

    memcpy(address_ptr, this->destination_address, 16);

    *port = this->destination_port;
    return 0;
}

int8_t thread_commissioning_get_management_id(int8_t interface_id)
{
    commissioner_t *this = commissioner_find(interface_id);
    if (!this) {
        return -1;
    }

    return this->management_instance;
}


#else
int thread_commissioning_register(int8_t interface_id, uint8_t PSKc[static 16])
{
    (void)interface_id;
    (void)PSKc;
    return -1;
}

int thread_commissioning_unregister(int8_t interface_id)
{
    (void)interface_id;
    return -1;
}

int thread_commissioning_device_add(int8_t interface_id, bool short_eui64, uint8_t EUI64[static 8], uint8_t *PSKd_ptr, uint8_t PSKd_len, thread_commissioning_joiner_finalisation_cb *joining_device_cb_ptr)
{
    (void)interface_id;
    (void)short_eui64;
    (void)EUI64;
    (void)PSKd_ptr;
    (void)PSKd_len;
    (void)joining_device_cb_ptr;
    return -1;
}

int thread_commissioning_device_delete(int8_t interface_id, uint8_t EUI64[8])
{
    (void)interface_id;
    (void)EUI64;
    return -1;
}
void *thread_commission_device_get_next(void *ptr, int8_t interface_id, bool *short_eui64, uint8_t EUI64[8], uint8_t PSKd[32], uint8_t *PSKd_len)
{
    (void)ptr;
    (void)interface_id;
    (void)short_eui64;
    (void)EUI64;
    (void)PSKd;
    (void)PSKd_len;
    return NULL;
}

int thread_commissioning_petition_keep_alive(int8_t interface_id, commissioning_state_e state)
{
    (void)interface_id;
    (void)state;
    return -1;
}

int thread_commissioning_petition_start(int8_t interface_id, char *commissioner_id_ptr, thread_commissioning_status_cb *status_cb_ptr)
{
    (void)interface_id;
    (void)commissioner_id_ptr;
    (void)status_cb_ptr;
    return -1;
}

int thread_commissioning_native_commissioner_get_connection_info(int8_t interface_id, uint8_t *address_ptr, uint16_t *port)
{
    (void)interface_id;
    (void)address_ptr;
    (void)port;
    return -1;
}

int8_t thread_commissioning_get_management_id(int8_t interface_id)
{
    (void)interface_id;
    return -1;
}

int thread_commissioning_native_commissioner_start(int8_t interface_id, thread_commissioning_native_select_cb *cb_ptr)
{
    (void)interface_id;
    (void)cb_ptr;
    return -1;
}

int thread_commissioning_native_commissioner_stop(int8_t interface_id)
{
    (void)interface_id;
    return -1;
}

int thread_commissioning_native_commissioner_connect(int8_t interface_id, thread_commissioning_link_configuration_s *link_ptr)
{
    (void)interface_id;
    (void)link_ptr;
    return -1;
}

int thread_commissioning_attach(int8_t interface_id, uint8_t *destination_address, uint16_t destination_port)
{
    (void)interface_id;
    (void)destination_address;
    (void)destination_port;
    return -1;
}

#endif
