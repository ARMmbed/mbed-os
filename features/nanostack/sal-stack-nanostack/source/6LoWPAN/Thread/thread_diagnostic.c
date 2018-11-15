/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "net_interface.h"
#include "thread_management_if.h"
#include "thread_management_server.h"
#include "thread_common.h"
#include "thread_joiner_application.h"
#include "thread_leader_service.h"
#include "thread_router_bootstrap.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"
#include "MLE/mle.h"
#include "6LoWPAN/Thread/thread_router_bootstrap.h"
#include "thread_config.h"
#include "thread_network_data_storage.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "thread_diagcop_lib.h"
#include "common_functions.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "mac_api.h"


#define TRACE_GROUP "TdiaC"


#include "coap_service_api.h"


typedef struct thread_diagnostic_command {
    int8_t interface_id;
    int8_t coap_service_id;
    ns_list_link_t link;
} thread_diagnostic_command_t;

static NS_LIST_DEFINE(instance_list, thread_diagnostic_command_t, link);

static thread_diagnostic_command_t *thread_diagnostic_command_find(int8_t interface_id)
{
    thread_diagnostic_command_t *this = NULL;
    ns_list_foreach(thread_diagnostic_command_t, cur_ptr, &instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static thread_diagnostic_command_t *thread_diagnostic_find_by_service(int8_t service_id)
{
    thread_diagnostic_command_t *this = NULL;
    ns_list_foreach(thread_diagnostic_command_t, cur_ptr, &instance_list) {
        if (cur_ptr->coap_service_id == service_id) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static uint8_t *thread_diagnostic_child_table_tlv_build(uint8_t *data_ptr, protocol_interface_info_entry_t *cur)
{
    uint8_t child_count = 0;
    uint8_t calculated_timeout;

    mac_neighbor_table_list_t *mac_table_list = &mac_neighbor_info(cur)->neighbour_list;

    child_count = thread_router_bootstrap_child_count_get(cur);

    *data_ptr++ = DIAGCOP_TLV_CHILD_TABLE;  // Type
    *data_ptr++ = (3 * child_count);        // Length

    ns_list_foreach(mac_neighbor_table_entry_t, cur_entry, mac_table_list) {
        if (thread_router_addr_from_addr(cur_entry->mac16) == mac_helper_mac16_address_get(cur)) {
            /* |0|1|2|3|4|5|6|7|8|9|0|1|2|3|4|5|6|7|8|9|0|1|2|3| */
            /* |Timeout  |Rsv|  Child ID       |      Mode     | */
            calculated_timeout = thread_log2_aprx(cur_entry->link_lifetime - 1) + 4;
            uint8_t mode = 0;
            mode |= mle_mode_write_from_mac_entry(cur_entry);
            mode |= thread_neighbor_class_mode_write_from_entry(&cur->thread_info->neighbor_class, cur_entry->index);
            tr_debug("Write child table TLV entry: %d - %d - %d", calculated_timeout, cur_entry->mac16, mode);
            *data_ptr = 0x00; //reserved bytes to zero
            *data_ptr = calculated_timeout << 3;

            if (cur_entry->mac16 & 0x0100) {
                *data_ptr = *data_ptr | 0x01;
            }
            data_ptr++;
            *data_ptr++ = (uint8_t)(cur_entry->mac16 & 0x00ff);
            *data_ptr++ = mode;
        }
    }

    return data_ptr;
}

uint8_t thread_diag_mode_get_by_interface_ptr(protocol_interface_info_entry_t *cur)
{
    uint8_t mle_mode = 0;
    if (!thread_info(cur)) {
        return 0;
    }
    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        mle_mode |= MLE_RX_ON_IDLE;
    }

    if (thread_info(cur)->requestFullNetworkData) {
        mle_mode |= (MLE_THREAD_REQ_FULL_DATA_SET);
    }

    /* We always send secured data requests */
    mle_mode |= MLE_THREAD_SECURED_DATA_REQUEST;

    switch (thread_info(cur)->thread_device_mode) {
        case THREAD_DEVICE_MODE_ROUTER:
        case THREAD_DEVICE_MODE_FULL_END_DEVICE:
            mle_mode |= MLE_FFD_DEV;
            break;

        default:
            break;
    }


    return mle_mode;
}

static int thread_diagnostic_configuration_calc(protocol_interface_info_entry_t *cur, uint8_t *tlv_list, uint16_t list_len)
{
    int payload_len = 0;
    uint16_t address_count = 0;

    if (!tlv_list || list_len < 1) {
        return 0;
    }

    while (list_len --) {
        switch (*tlv_list) {
            case DIAGCOP_TLV_EXTENDED_MAC_ADDRESS:
                payload_len += 2 + 8;
                break;

            case DIAGCOP_TLV_ADDRESS16:
                payload_len += 2 + 2;
                break;

            case DIAGCOP_TLV_MODE:
                payload_len += 2 + 1;
                break;

            case DIAGCOP_TLV_TIMEOUT:
                if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE ||
                        cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE) {
                    payload_len += 2 + 4;
                }
                break;

            case DIAGCOP_TLV_CONNECTIVITY:
                payload_len += 2 + 10;
                break;

            case DIAGCOP_TLV_ROUTE64:
                payload_len += thread_route_option_size(cur);
                break;

            case DIAGCOP_TLV_LEADER_DATA:
                payload_len += 2 + 8; // TLV header + uint16 pan id
                break;

            case DIAGCOP_TLV_NETWORK_DATA:
                payload_len += 2; // TLV header
                payload_len += thread_network_data_tlv_size(cur, 1);
                break;

            case DIAGCOP_TLV_IPV6_ADDRESS_LIST:
                arm_net_interface_address_list_size(cur->id, &address_count);
                payload_len += 2 + (address_count * 16);
                break;

            case DIAGCOP_TLV_MAC_COUNTERS:
                payload_len += 2 + 36;
                break;
            case DIAGCOP_TLV_BATTERY_LEVEL:
                payload_len += 2 + 1;
                break;

            case DIAGCOP_TLV_SUPPLY_VOLTAGE:
                payload_len += 2 + 2;
                break;

            case DIAGCOP_TLV_CHILD_TABLE:
                /* Value length = Type + Length + 3 * child count */
                payload_len += 2 + (3 * thread_router_bootstrap_child_count_get(cur));
                break;

            case DIAGCOP_TLV_CHANNEL_PAGES:
                payload_len += 2 + 1;
                break;

            case DIAGCOP_TLV_MAX_CHILD_TIMEOUT:
                payload_len += 2 + 4;
                break;

            default:
                // todo: Other TLV's not supported atm
                break;
        }

        tlv_list++;
    }
    return payload_len;
}

static uint8_t *thread_diagnostic_get_build(protocol_interface_info_entry_t *cur, uint8_t *response_ptr, uint8_t *tlv_list, uint16_t list_len)
{

    if (!tlv_list || list_len < 1) {
        // Request all
        return response_ptr;
    }
    if (!thread_info(cur)) {
        return response_ptr;
    }
    // the following are some tlvs that need to be implemented correctly, this is only dummy data for the moment
    uint8_t dummy_data[36] = {0};
    uint8_t *ptr;
    int written_address_count = 0;
    uint16_t ipv6_address_count = 0;
    uint32_t max_child_timeout = 0;
    uint8_t extended_address[8] = {0};

    arm_net_interface_address_list_size(cur->id, &ipv6_address_count);

    // 16 bytes for each ipv6 address
    uint8_t ipv6_address_list[ipv6_address_count * 16];

    tr_debug("tlv list length %d", list_len);
    while (list_len --) {
        switch (*tlv_list) {

            case DIAGCOP_TLV_EXTENDED_MAC_ADDRESS:
                if (cur->mac_api) {
                    //Read dynamicaly generated current extented address from MAC.
                    cur->mac_api->mac64_get(cur->mac_api, MAC_EXTENDED_DYNAMIC, extended_address);
                }
                response_ptr = thread_diagcop_tlv_data_write(response_ptr, DIAGCOP_TLV_EXTENDED_MAC_ADDRESS, 8, extended_address);
                break;

            case DIAGCOP_TLV_ADDRESS16:
                response_ptr = thread_diagcop_tlv_data_write_uint16(response_ptr, DIAGCOP_TLV_ADDRESS16, mac_helper_mac16_address_get(cur));
                break;

            case DIAGCOP_TLV_MODE:
                response_ptr = thread_diagcop_tlv_data_write_uint8(response_ptr, DIAGCOP_TLV_MODE, thread_diag_mode_get_by_interface_ptr(cur));
                break;

            case DIAGCOP_TLV_TIMEOUT:
                //must be sleeping poll rate
                if (cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE ||
                        cur->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE) {
                    response_ptr = thread_diagcop_tlv_data_write_uint32(response_ptr, DIAGCOP_TLV_TIMEOUT, cur->thread_info->host_link_timeout);
                }
                break;

            case DIAGCOP_TLV_CONNECTIVITY:
                ptr = response_ptr;
                response_ptr = thread_connectivity_tlv_write(response_ptr, cur, 0x0f);
                if (ptr != response_ptr) {
                    ptr[0] = DIAGCOP_TLV_CONNECTIVITY;
                }
                break;

            case DIAGCOP_TLV_ROUTE64:
                ptr = response_ptr;
                response_ptr = thread_route_option_write(cur, response_ptr);
                if (ptr != response_ptr) {
                    ptr[0] = DIAGCOP_TLV_ROUTE64;
                }
                break;

            case DIAGCOP_TLV_LEADER_DATA:
                ptr = response_ptr;
                response_ptr = thread_leader_data_tlv_write(response_ptr, cur);
                if (ptr != response_ptr) {
                    ptr[0] = DIAGCOP_TLV_LEADER_DATA;
                }
                break;

            case DIAGCOP_TLV_NETWORK_DATA:
                ptr = response_ptr;
                response_ptr = thread_network_data_tlv_write(cur, response_ptr, true);
                if (ptr != response_ptr) {
                    ptr[0] = DIAGCOP_TLV_NETWORK_DATA;
                }
                break;

            case DIAGCOP_TLV_IPV6_ADDRESS_LIST:
                arm_net_address_list_get(cur->id, ipv6_address_count * 16, ipv6_address_list, &written_address_count);
                response_ptr = thread_diagcop_tlv_data_write(response_ptr, DIAGCOP_TLV_IPV6_ADDRESS_LIST, ipv6_address_count * 16, ipv6_address_list);
                break;

            case DIAGCOP_TLV_MAC_COUNTERS:
                /* The following elements from [RFC 2863] are included in this order:
                 * ifInUnknownProtos (4), ifInErrors (4), ifOutErrors (4), ifInUcastPkts (4),
                 * ifInBroadcastPkts (4), ifInDiscards (4), ifOutUcastPkts (4), ifOutBroadcastPkts (4), ifOutDiscards (4) */
                response_ptr = thread_diagcop_tlv_data_write(response_ptr, DIAGCOP_TLV_MAC_COUNTERS, 36, dummy_data);
                break;

            case DIAGCOP_TLV_BATTERY_LEVEL:
                response_ptr = thread_diagcop_tlv_data_write_uint8(response_ptr, DIAGCOP_TLV_BATTERY_LEVEL, 0);
                break;

            case DIAGCOP_TLV_SUPPLY_VOLTAGE:
                response_ptr = thread_diagcop_tlv_data_write_uint16(response_ptr, DIAGCOP_TLV_SUPPLY_VOLTAGE, 0);
                break;

            case DIAGCOP_TLV_CHILD_TABLE:
                if (thread_router_bootstrap_child_count_get(cur)) {
                    response_ptr = thread_diagnostic_child_table_tlv_build(response_ptr, cur);
                }
                break;

            case DIAGCOP_TLV_CHANNEL_PAGES:
                // Only supporting channel page 0
                response_ptr = thread_diagcop_tlv_data_write_uint8(response_ptr, DIAGCOP_TLV_CHANNEL_PAGES, 0);
                break;

            case DIAGCOP_TLV_MAX_CHILD_TIMEOUT:
                if (thread_router_bootstrap_child_max_timeout_get(cur, &max_child_timeout) == 0) {
                    response_ptr = thread_diagcop_tlv_data_write_uint32(response_ptr, DIAGCOP_TLV_MAX_CHILD_TIMEOUT, max_child_timeout);
                }
                break;

            default:

                break;
        }

        tlv_list++;
    }
    return response_ptr;
}
/**
 * Thread diagnostics request d/dg
 */
static int thread_diagnostic_command_request_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    thread_diagnostic_command_t *this = thread_diagnostic_find_by_service(service_id);
    protocol_interface_info_entry_t *cur;
    uint8_t *ptr = NULL;
    uint8_t *request_tlv_ptr = NULL;
    uint16_t request_tlv_len;
    int response_len;
    uint8_t *response_ptr = NULL;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;

    if (!this) {//check if there is request
        return -1;
    }
    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return -1;
    }

    tr_debug("Thread diagnostic command get request");

    request_tlv_len = thread_diagcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, DIAGCOP_TLV_TYPE_LIST, &request_tlv_ptr);

    // the following function calculates the total memory that is needed to be allocated for response.
    // If the request_tlv_len is 0 then the memory allocated is for all the diagnostic command tlvs
    response_len = thread_diagnostic_configuration_calc(cur, request_tlv_ptr, request_tlv_len);
    if (response_len < 1) {
        if (request_tlv_len > 0) {
            // TLV was ommitted but ok request. respond with ok status
            goto send_response;
        }
        goto failure;
    }
    ptr = response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto failure;
    }

    ptr = thread_diagnostic_get_build(cur, ptr, request_tlv_ptr,
                                      request_tlv_len);

send_response:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE,
                               request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_OCTET_STREAM,
                               response_ptr, ptr - response_ptr);
    ns_dyn_mem_free(response_ptr);
    return 0;

failure:
    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_NONE, NULL, 0);
    return 0;
}
/**
 * Thread diagnostics reset d/dr
 */
static int thread_diagnostic_command_reset_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    (void) source_address;
    (void) source_port;

    thread_diagnostic_command_t *this = thread_diagnostic_find_by_service(service_id);
    if (!this) {//check if there is request
        return -1;
    }

    tr_debug("Thread diagnostic command reset request");

    coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE,
                               request_ptr, COAP_MSG_CODE_RESPONSE_CHANGED, COAP_CT_TEXT_PLAIN, NULL, 0);
    return 0;
}

/**
 * Thread diagnostics query d/dq
 */
static int thread_diagnostic_command_query_cb(int8_t service_id, uint8_t source_address[16], uint16_t source_port, sn_coap_hdr_s *request_ptr)
{
    uint8_t address_copy[16];
    protocol_interface_info_entry_t *cur;
    uint8_t *response_ptr = NULL;
    uint8_t *ptr = NULL;
    uint8_t *request_tlv_ptr;
    uint16_t response_len;
    uint16_t request_tlv_len;
    sn_coap_msg_code_e return_code = COAP_MSG_CODE_EMPTY;
    int8_t response = -1;

    /* Source_address is freed when calling coap_service_response_send().
     * Anyhow, We need to use it when sending request, so let's make a copy! */
    memcpy(address_copy, source_address, 16);

    thread_diagnostic_command_t *this = thread_diagnostic_find_by_service(service_id);
    if (!this) {//check if there is request
        return -1;
    }
    cur = protocol_stack_interface_info_get_by_id(this->interface_id);
    if (!cur) {
        return -1;
    }

    tr_debug("Thread diagnostic command query request from %s:%d", trace_array(source_address, 16), source_port);
    // build response
    request_tlv_len = thread_diagcop_tlv_find(request_ptr->payload_ptr, request_ptr->payload_len, DIAGCOP_TLV_TYPE_LIST, &request_tlv_ptr);

    // the following function calculates the total memory that is needed to be allocated for response.
    // If the request_tlv_len is 0 then the memory allocated is for all the diagnostic command tlvs
    response_len = thread_diagnostic_configuration_calc(cur, request_tlv_ptr, request_tlv_len);
    if (response_len < 1) {
        if (request_tlv_len > 0) {
            // TLV was ommitted but ok request. respond with ok status
            goto send_response;
        }
        return_code = COAP_MSG_CODE_RESPONSE_BAD_REQUEST;
        goto failure;
    }
    ptr = response_ptr = ns_dyn_mem_alloc(response_len);
    if (!response_ptr) {
        return_code = COAP_MSG_CODE_RESPONSE_INTERNAL_SERVER_ERROR;
        goto failure;
    }

    ptr = thread_diagnostic_get_build(cur, response_ptr, request_tlv_ptr, request_tlv_len);

    /* Send ACK to confirmable request */
    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_NONE, NULL, 0);
        response = 0;
    }

    /* Send reply to d/da */
send_response:
    coap_service_request_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, address_copy, source_port, COAP_MSG_TYPE_CONFIRMABLE, COAP_MSG_CODE_REQUEST_POST,
                              THREAD_URI_DIAGNOSTIC_ANSWER, COAP_CT_OCTET_STREAM, response_ptr, ptr - response_ptr, NULL);

    ns_dyn_mem_free(response_ptr);

    return response;

failure:
    if (request_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        coap_service_response_send(this->coap_service_id, COAP_REQUEST_OPTIONS_NONE, request_ptr, return_code, COAP_CT_NONE, NULL, 0);
        response = 0;
    }
    return response;
}

/**
 * Public interface functions
 */
int thread_diagnostic_init(int8_t interface_id)
{

    thread_diagnostic_command_t *this = thread_diagnostic_command_find(interface_id);
    if (this) {
        return 0;
    }

    this = ns_dyn_mem_alloc(sizeof(thread_diagnostic_command_t));
    if (!this) {
        return -2;
    }

    this->interface_id = interface_id;

    this->coap_service_id = thread_management_server_service_id_get(interface_id);
    if (this->coap_service_id < 0) {
        tr_error("Thread diagnostic init failed");
        ns_dyn_mem_free(this);
        return -3;
    }
    /**
     * Thread Management uri registration
     */
    coap_service_register_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_REQUEST, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_diagnostic_command_request_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_RESET, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_diagnostic_command_reset_cb);
    coap_service_register_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_QUERY, COAP_SERVICE_ACCESS_POST_ALLOWED, thread_diagnostic_command_query_cb);
    ns_list_add_to_start(&instance_list, this);

    return 0;
}

int thread_diagnostic_delete(int8_t interface_id)
{
    thread_diagnostic_command_t *this = thread_diagnostic_command_find(interface_id);

    if (!this) {
        return -1;
    }
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_REQUEST);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_RESET);
    coap_service_unregister_uri(this->coap_service_id, THREAD_URI_DIAGNOSTIC_QUERY);
    ns_list_remove(&instance_list, this);
    ns_dyn_mem_free(this);
    return 0;
}

#endif
