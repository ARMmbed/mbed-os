/*
 * Copyright (c) 2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <ns_trace.h>
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "common_functions.h"

#ifdef HAVE_DHCPV6
#include "dhcp_service_api.h"
#include "dhcpv6_client_api.h"
#include "libDHCPv6/libDHCPv6.h"
#include "NWK_INTERFACE/Include/protocol.h"

#define TRACE_GROUP "DHP"

typedef struct {
    dhcp_client_global_adress_cb *global_address_cb;
    uint16_t service_instance;
    uint16_t relay_instance;
    uint8_t libDhcp_instance;
    int8_t interface;
} dhcp_client_class_t;

static dhcp_client_class_t dhcp_client;

void dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr);


void dhcp_client_init(int8_t interface)
{
    // No support for multible instances yet.
    dhcp_client.service_instance = dhcp_service_init(interface, DHCP_INSTANCE_CLIENT, NULL);
    dhcp_client.interface = interface;
    dhcp_client.libDhcp_instance = libdhcpv6_nonTemporal_entry_get_unique_instance_id();

    return;
}

void dhcp_relay_agent_enable(int8_t interface, uint8_t border_router_address[static 16])
{
    dhcp_client.relay_instance = dhcp_service_init(interface, DHCP_INTANCE_RELAY_AGENT, NULL);
    dhcp_service_relay_instance_enable(dhcp_client.relay_instance, border_router_address);
}

void dhcp_client_delete(int8_t interface)
{
    protocol_interface_info_entry_t *cur = NULL;
    dhcpv6_client_server_data_t *srv_data_ptr;

    dhcp_service_delete(dhcp_client.service_instance);


    cur = protocol_stack_interface_info_get_by_id(interface);

    if (!cur) {
        return;
    }

    do {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_instance(dhcp_client.libDhcp_instance);
        if (srv_data_ptr != NULL) {
            tr_debug("Free DHCPv6 Client\n");
            dhcp_service_req_remove(srv_data_ptr->transActionId);// remove all pending retransmissions
            addr_delete(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);
            libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        }
    } while (srv_data_ptr != NULL);
    dhcp_client.service_instance = 0;
    return;
}


void dhcpv6_client_send_error_cb(dhcpv6_client_server_data_t *srv_data_ptr)
{
    if (srv_data_ptr != NULL) {

        // error for Global address
        if (dhcp_client.global_address_cb != NULL) {
            dhcp_client.global_address_cb(dhcp_client.interface, srv_data_ptr->server_address, srv_data_ptr->iaNontemporalAddress.addressPrefix, false);
        }
    }
}


/* solication responce received for either global address or routter id assignment */
int dhcp_solicit_resp_cb(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len)
{
    dhcp_ia_non_temporal_params_t dhcp_ia_non_temporal_params;
    dhcp_link_options_params_t clientId;
    dhcp_link_options_params_t serverId;
    dhcp_link_options_params_t interfaceId;
    dhcpv6_client_server_data_t *srv_data_ptr = NULL;
    (void)instance_id;

    srv_data_ptr = ptr;

    if (srv_data_ptr == NULL) {
        tr_error("server data not found");
        goto error_exit;
    }

    // Validate message
    if (msg_name != DHCPV6_REPLY_TYPE) {
        tr_error("invalid response");
        goto error_exit;
    }

    if (libdhcpv6_reply_message_option_validate(&clientId, &serverId, &dhcp_ia_non_temporal_params, msg_ptr, msg_len) != 0) {
        tr_error("Sol Not include all Options");
        goto error_exit;
    }

    if (libdhcpv6_nonTemporal_entry_get_by_iaid(dhcp_ia_non_temporal_params.iaId) != srv_data_ptr) {
        /* Validate server data availability */
        tr_error("Valid instance not found");
        goto error_exit;
    }

    if (srv_data_ptr->IAID != dhcp_ia_non_temporal_params.iaId) {
        tr_error("Wrong IAID");
        goto error_exit;
    }

    interfaceId.linkID = srv_data_ptr->clientId;
    interfaceId.linkType = srv_data_ptr->clientLinkIdType;
    if (libdhcpv6_compare_DUID(&interfaceId, &clientId) != 0) {
        tr_error("Not Valid Client Id");
        goto error_exit;
    }

    memcpy(srv_data_ptr->iaNontemporalAddress.addressPrefix, dhcp_ia_non_temporal_params.nonTemporalAddress, 16);
    srv_data_ptr->iaNontemporalAddress.preferredTime = dhcp_ia_non_temporal_params.preferredValidLifeTime;
    srv_data_ptr->iaNontemporalAddress.validLifetime = dhcp_ia_non_temporal_params.validLifeTime;
    memcpy(srv_data_ptr->serverLinkId, serverId.linkID, 8);
    srv_data_ptr->serverLinkType = serverId.linkType;
    srv_data_ptr->T0 = dhcp_ia_non_temporal_params.T0;
    srv_data_ptr->T1  = dhcp_ia_non_temporal_params.T1;
    srv_data_ptr->iaNonTemporalStructValid = true;

    dhcpv6_client_set_address(dhcp_client.interface, srv_data_ptr);


    if (dhcp_client.global_address_cb) {
        dhcp_client.global_address_cb(dhcp_client.interface, srv_data_ptr->server_address, srv_data_ptr->iaNontemporalAddress.addressPrefix, true);
    }
    return RET_MSG_ACCEPTED;
error_exit:
    dhcpv6_client_send_error_cb(srv_data_ptr);
    return RET_MSG_ACCEPTED;
}

int dhcp_client_get_global_address(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], uint8_t mac64[static 8], dhcp_client_global_adress_cb *error_cb)
{
    dhcpv6_solication_base_packet_s solPacket = {0};
    dhcpv6_ia_non_temporal_address_s nonTemporalAddress = {0};
    uint8_t *payload_ptr;
    uint32_t payload_len;
    dhcpv6_client_server_data_t *srv_data_ptr;

    if (mac64 == NULL || prefix == NULL || dhcp_addr == NULL) {
        tr_error("Invalid parameters");
        return -1;
    }

    srv_data_ptr = libdhcvp6_nontemporalAddress_server_data_allocate(interface, dhcp_client.libDhcp_instance, mac64, DHCPV6_DUID_HARDWARE_EUI64_TYPE, prefix, dhcp_addr);
    if (!srv_data_ptr) {
        tr_error("OOM srv_data_ptr");
        return -1;
    }

    payload_len = libdhcpv6_solication_message_length(DHCPV6_DUID_HARDWARE_EUI64_TYPE, true, 0);
    payload_ptr = ns_dyn_mem_temporary_alloc(payload_len);
    if (!payload_ptr) {
        libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        tr_error("OOM payload_ptr");
        return -1;
    }

    dhcp_client.global_address_cb = error_cb; //TODO Only supporting one instance globaly if we need more for different instances needs code.
    srv_data_ptr->GlobalAddress = true;
    // Build solicit
    solPacket.clientDUID.linkID = mac64;
    solPacket.clientDUID.linkType = DHCPV6_DUID_HARDWARE_EUI64_TYPE;
    solPacket.iaID = srv_data_ptr->IAID;
    solPacket.messageType = DHCPV6_SOLICATION_TYPE;
    solPacket.transActionId = libdhcpv6_txid_get();
    /*Non Temporal Address */
    nonTemporalAddress.requestedAddress = prefix;
    libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &solPacket, &nonTemporalAddress, NULL);

    // send solicit
    srv_data_ptr->transActionId = dhcp_service_send_req(dhcp_client.service_instance, 0, srv_data_ptr, dhcp_addr, payload_ptr, payload_len, dhcp_solicit_resp_cb);
    if (srv_data_ptr->transActionId == 0) {
        ns_dyn_mem_free(payload_ptr);
        libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        return -1;
    }

    return 0;
}

void dhcp_client_global_address_renew(int8_t interface)
{
    (void)interface;
    return;
}

void dhcp_client_global_address_delete(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16])
{
    protocol_interface_info_entry_t *cur;
    dhcpv6_client_server_data_t *srv_data_ptr;
    (void) dhcp_addr;

    srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface, prefix);
    cur = protocol_stack_interface_info_get_by_id(interface);

    do {
        if (cur == NULL || srv_data_ptr == NULL) {
            return;
        }
        dhcp_service_req_remove(srv_data_ptr->transActionId);// remove all pending retransmissions
        tr_debug("Deleting address: %s", trace_ipv6(srv_data_ptr->iaNontemporalAddress.addressPrefix));

        addr_delete(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);

        libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface, prefix);
    } while (srv_data_ptr);

    return;
}

void dhcpv6_renew(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{
    dhcpv6_ia_non_temporal_address_s nonTemporalAddress = {0};
    dhcp_link_options_params_t serverLink;
    uint8_t *payload_ptr;
    uint32_t payload_len;
    dhcpv6_client_server_data_t *srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface->id, addr->address);

    if (srv_data_ptr == NULL) {
        tr_warn("Dhcp address lost");
        return ;
    }
    if (reason == ADDR_CALLBACK_INVALIDATED) {
        dhcp_service_req_remove(srv_data_ptr->transActionId);//stop retransmissions of renew
        libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        tr_warn("Dhcp address lost");
        return;
    }

    if (reason != ADDR_CALLBACK_TIMER) {
        return;
    }

    payload_len = libdhcpv6_address_request_message_len(srv_data_ptr->clientLinkIdType, srv_data_ptr->serverLinkType, 0);
    payload_ptr = ns_dyn_mem_temporary_alloc(payload_len);
    if (payload_ptr == NULL) {
        addr->state_timer = 200; //Retry after 20 seconds
        tr_error("Out of memory");
        return ;
    }
    dhcpv6_solication_base_packet_s packetReq = {
        .messageType = DHCPV6_RENEW_TYPE,
        .clientDUID.linkID = srv_data_ptr->clientId,
        .clientDUID.linkType = srv_data_ptr->clientLinkIdType,
        .requestedOptionCnt = 0,
        .iaID = srv_data_ptr->IAID,
        .timerT0 = srv_data_ptr->T0,
        .timerT1 = srv_data_ptr->T1,
        .requestedOptionList = NULL,
    };

    // Set Address information
    nonTemporalAddress.requestedAddress = srv_data_ptr->iaNontemporalAddress.addressPrefix;
    nonTemporalAddress.preferredLifeTime = srv_data_ptr->iaNontemporalAddress.preferredTime;
    nonTemporalAddress.validLifeTime = srv_data_ptr->iaNontemporalAddress.validLifetime;
    serverLink.linkID = srv_data_ptr->serverLinkId;
    serverLink.linkType = srv_data_ptr->serverLinkType;
    libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &packetReq, &nonTemporalAddress, &serverLink);
    // send solicit
    srv_data_ptr->transActionId = dhcp_service_send_req(dhcp_client.service_instance, 0, srv_data_ptr, srv_data_ptr->server_address, payload_ptr, payload_len, dhcp_solicit_resp_cb);
    if (srv_data_ptr->transActionId == 0) {
        ns_dyn_mem_free(payload_ptr);
        addr->state_timer = 200; //Retry after 20 seconds
        tr_error("DHCP renew send failed");
    }
}

void dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr)
{
    protocol_interface_info_entry_t *cur = NULL;
    if_address_entry_t *address_entry = NULL;
    uint32_t renewTimer;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }
    renewTimer = libdhcpv6_renew_time_define(srv_data_ptr);

    address_entry = addr_get_entry(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);
    if (address_entry == NULL) {
        // create new
        address_entry = addr_add(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix, 64, ADDR_SOURCE_DHCP, srv_data_ptr->iaNontemporalAddress.validLifetime, srv_data_ptr->iaNontemporalAddress.preferredTime, false);
    } else {
        addr_set_valid_lifetime(cur, address_entry, srv_data_ptr->iaNontemporalAddress.validLifetime);
        addr_set_preferred_lifetime(cur, address_entry, srv_data_ptr->iaNontemporalAddress.preferredTime);
    }

    if (address_entry == NULL) {
        tr_error("Address add failed");
        return;
    }
    if (renewTimer) {
        // translate seconds to 100ms ticks
        if (renewTimer  <  0xffffffff / 10) {
            renewTimer *= 10;
        } else {
            renewTimer = 0xfffffffe;
        }
    }
    address_entry->state_timer = renewTimer;
    address_entry->cb = dhcpv6_renew;
}

#endif
