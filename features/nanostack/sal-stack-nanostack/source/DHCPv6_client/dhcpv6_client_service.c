/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
    uint16_t sol_timeout;
    uint16_t sol_max_rt;
    uint8_t sol_max_rc;
    uint8_t libDhcp_instance;
    int8_t interface;
    bool renew_uses_solicit: 1;
    bool one_instance_interface: 1;
    bool no_address_hint: 1;
} dhcp_client_class_t;

static dhcp_client_class_t dhcp_client;

static bool dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr);
void dhcpv6_renew(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason);


void dhcp_client_init(int8_t interface)
{
    // No support for multible instances yet.
    dhcp_client.service_instance = dhcp_service_init(interface, DHCP_INSTANCE_CLIENT, NULL);
    dhcp_client.interface = interface;
    dhcp_client.libDhcp_instance = libdhcpv6_nonTemporal_entry_get_unique_instance_id();
    dhcp_client.sol_timeout = 0;
    dhcp_client.sol_max_rt = 0;
    dhcp_client.sol_max_rc = 0;
    dhcp_client.renew_uses_solicit = false;
    dhcp_client.one_instance_interface = false;
    dhcp_client.no_address_hint = false;

    return;
}
void dhcp_client_configure(int8_t interface, bool renew_uses_solicit, bool one_client_for_this_interface, bool no_address_hint)
{
    // Set true if RENEW is not used and SOLICIT sent instead.
    (void)interface;
    dhcp_client.renew_uses_solicit = renew_uses_solicit;
    dhcp_client.one_instance_interface = one_client_for_this_interface;
    dhcp_client.no_address_hint = no_address_hint;
}

void dhcp_client_solicit_timeout_set(int8_t interface, uint16_t timeout, uint16_t max_rt, uint8_t max_rc)
{
    // Set the default retry values for SOLICIT and RENEW messages.
    (void)interface;
    dhcp_client.sol_timeout = timeout;
    dhcp_client.sol_max_rt = max_rt;
    dhcp_client.sol_max_rc = max_rc;

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
    uint8_t temporary_address[16];
    dhcp_service_delete(dhcp_client.service_instance);


    cur = protocol_stack_interface_info_get_by_id(interface);

    if (!cur) {
        return;
    }

    do {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_instance(dhcp_client.libDhcp_instance);
        if (srv_data_ptr != NULL) {
            tr_debug("Free DHCPv6 Client\n");
            memcpy(temporary_address, srv_data_ptr->iaNontemporalAddress.addressPrefix, 16);
            dhcp_service_req_remove(srv_data_ptr->transActionId);// remove all pending retransmissions
            libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
            addr_delete(cur, temporary_address);

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

    if (dhcp_client.one_instance_interface && memcmp(srv_data_ptr->iaNontemporalAddress.addressPrefix, dhcp_ia_non_temporal_params.nonTemporalAddress, 16)) {

        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(dhcp_client.interface);
        if (cur) {
            addr_deprecate(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);
        }
    }

    memcpy(srv_data_ptr->iaNontemporalAddress.addressPrefix, dhcp_ia_non_temporal_params.nonTemporalAddress, 16);
    srv_data_ptr->iaNontemporalAddress.preferredTime = dhcp_ia_non_temporal_params.preferredValidLifeTime;
    srv_data_ptr->iaNontemporalAddress.validLifetime = dhcp_ia_non_temporal_params.validLifeTime;
    memcpy(srv_data_ptr->serverLinkId, serverId.linkID, 8);
    srv_data_ptr->serverLinkType = serverId.linkType;
    srv_data_ptr->T0 = dhcp_ia_non_temporal_params.T0;
    srv_data_ptr->T1  = dhcp_ia_non_temporal_params.T1;


    bool status = dhcpv6_client_set_address(dhcp_client.interface, srv_data_ptr);


    if (dhcp_client.global_address_cb) {
        dhcp_client.global_address_cb(dhcp_client.interface, srv_data_ptr->server_address, srv_data_ptr->iaNontemporalAddress.addressPrefix, status);
    }
    return RET_MSG_ACCEPTED;
error_exit:
    dhcpv6_client_send_error_cb(srv_data_ptr);
    return RET_MSG_ACCEPTED;
}

int dhcp_client_get_global_address(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], uint8_t mac64[static 8], uint16_t link_type, dhcp_client_global_adress_cb *error_cb)
{
    dhcpv6_solication_base_packet_s solPacket = {0};

    uint8_t *payload_ptr;
    uint32_t payload_len;
    dhcpv6_client_server_data_t *srv_data_ptr;
    bool add_prefix;

    if (mac64 == NULL || dhcp_addr == NULL) {
        tr_error("Invalid parameters");
        return -1;
    }

    if (!prefix || dhcp_client.one_instance_interface) {
        //NULL Definition will only check That Interface is not generated
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_instance(dhcp_client.libDhcp_instance);
        if (srv_data_ptr) {
            //Already Created to same interface
            if (dhcp_client.one_instance_interface && prefix) {
                if (srv_data_ptr->iaNonTemporalStructValid) {
                    if (memcmp(srv_data_ptr->iaNontemporalAddress.addressPrefix, prefix, 8)) {
                        //Request new address direct from Server if prefix is new
                        srv_data_ptr->iaNonTemporalStructValid = false;
                        dhcpv6_renew(protocol_stack_interface_info_get_by_id(interface), NULL, ADDR_CALLBACK_TIMER);
                    }
                    return 0;
                } else if (dhcp_client_server_address_update(interface, prefix, dhcp_addr) == 0) {
                    //DHCP server address OK
                    return 0;
                }
            }
            return -1;
        }
    } else if (dhcp_client_server_address_update(interface, prefix, dhcp_addr) == 0) {
        //No need for allocate new
        return 0;
    }

    tr_debug("GEN new Dhcpv6 client %u", dhcp_client.libDhcp_instance);
    srv_data_ptr = libdhcvp6_nontemporalAddress_server_data_allocate(interface, dhcp_client.libDhcp_instance, mac64, link_type, prefix, dhcp_addr);

    if (!srv_data_ptr) {
        tr_error("OOM srv_data_ptr");
        return -1;
    }


    if (!prefix || dhcp_client.no_address_hint) {
        add_prefix = false;
    } else {
        add_prefix = prefix != NULL;
    }

    payload_len = libdhcpv6_solication_message_length(link_type, add_prefix, 0);


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
    solPacket.clientDUID.linkType = link_type;
    solPacket.iaID = srv_data_ptr->IAID;
    solPacket.messageType = DHCPV6_SOLICATION_TYPE;
    solPacket.transActionId = libdhcpv6_txid_get();
    /*Non Temporal Address */

    if (prefix && !dhcp_client.no_address_hint) {
        dhcpv6_ia_non_temporal_address_s nonTemporalAddress = {0};
        nonTemporalAddress.requestedAddress = prefix;
        libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &solPacket, &nonTemporalAddress, NULL);
    } else {
        libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &solPacket, NULL, NULL);
    }

    // send solicit
    srv_data_ptr->transActionId = dhcp_service_send_req(dhcp_client.service_instance, 0, srv_data_ptr, dhcp_addr, payload_ptr, payload_len, dhcp_solicit_resp_cb);
    if (srv_data_ptr->transActionId == 0) {
        ns_dyn_mem_free(payload_ptr);
        libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
        return -1;
    }
    srv_data_ptr->iaNonTemporalStructValid = false;
    if (dhcp_client.sol_timeout != 0) {
        // Default retry values are modified from specification update to message
        dhcp_service_set_retry_timers(srv_data_ptr->transActionId, dhcp_client.sol_timeout, dhcp_client.sol_max_rt, dhcp_client.sol_max_rc);
    }

    return 0;
}

int dhcp_client_server_address_update(int8_t interface, uint8_t *prefix, uint8_t server_address[static 16])
{
    dhcpv6_client_server_data_t *srv_data_ptr = NULL;

    if (dhcp_client.interface != interface) {
        tr_debug("Interface not match");
        return -1;
    }

    if (prefix) {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface, prefix);
    } else if (dhcp_client.one_instance_interface) {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_instance(dhcp_client.libDhcp_instance);
    }
    if (!srv_data_ptr) {
        return -1;
    }

    if (memcmp(srv_data_ptr->server_address, server_address, 16) == 0) {
        return 0;
    }

    memcpy(srv_data_ptr->server_address, server_address, 16);
    if (!srv_data_ptr->iaNonTemporalStructValid) {
        dhcp_service_update_server_address(srv_data_ptr->transActionId, server_address);
    }
    return 0;
}



void dhcp_client_global_address_renew(int8_t interface)
{
    (void)interface;
    return;
}

void dhcp_client_global_address_delete(int8_t interface, uint8_t *dhcp_addr, uint8_t prefix[static 16])
{
    protocol_interface_info_entry_t *cur;
    dhcpv6_client_server_data_t *srv_data_ptr;
    (void) dhcp_addr;

    srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface, prefix);
    cur = protocol_stack_interface_info_get_by_id(interface);

    if (cur == NULL || srv_data_ptr == NULL) {
        return;
    }

    dhcp_service_req_remove(srv_data_ptr->transActionId);// remove all pending retransmissions
    if (dhcp_client.one_instance_interface) {
        addr_deprecate(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);
    } else {
        addr_delete(cur, srv_data_ptr->iaNontemporalAddress.addressPrefix);
    }

    libdhcvp6_nontemporalAddress_server_data_free(srv_data_ptr);
}

void dhcpv6_renew(protocol_interface_info_entry_t *interface, if_address_entry_t *addr, if_address_callback_t reason)
{

    dhcp_link_options_params_t serverLink;
    uint8_t *payload_ptr;
    uint32_t payload_len;
    dhcpv6_client_server_data_t *srv_data_ptr;
    if (addr) {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_prefix(interface->id, addr->address);
    } else {
        srv_data_ptr = libdhcpv6_nonTemporal_entry_get_by_instance(dhcp_client.libDhcp_instance);
    }

    if (srv_data_ptr == NULL) {
        return;
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

    payload_len = libdhcpv6_address_request_message_len(srv_data_ptr->clientLinkIdType, srv_data_ptr->serverLinkType, 0, !dhcp_client.no_address_hint);
    payload_ptr = ns_dyn_mem_temporary_alloc(payload_len);
    if (payload_ptr == NULL) {
        if (addr) {
            addr->state_timer = 200; //Retry after 20 seconds
        }
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

    if (dhcp_client.renew_uses_solicit) {
        packetReq.messageType = DHCPV6_SOLICATION_TYPE;
    }


    serverLink.linkID = srv_data_ptr->serverLinkId;
    serverLink.linkType = srv_data_ptr->serverLinkType;
    if (dhcp_client.no_address_hint && dhcp_client.renew_uses_solicit) {
        packetReq.timerT0 = 0;
        packetReq.timerT1 = 0;
        libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &packetReq, NULL, &serverLink);
    } else {
        // Set Address information
        dhcpv6_ia_non_temporal_address_s nonTemporalAddress = {0};
        nonTemporalAddress.requestedAddress = srv_data_ptr->iaNontemporalAddress.addressPrefix;
        nonTemporalAddress.preferredLifeTime = srv_data_ptr->iaNontemporalAddress.preferredTime;
        nonTemporalAddress.validLifeTime = srv_data_ptr->iaNontemporalAddress.validLifetime;
        libdhcpv6_generic_nontemporal_address_message_write(payload_ptr, &packetReq, &nonTemporalAddress, &serverLink);
    }
    // send solicit
    uint8_t *server_address = dhcp_service_relay_global_addres_get(dhcp_client.relay_instance);
    if (!server_address) {
        server_address = srv_data_ptr->server_address;
    }

    srv_data_ptr->transActionId = dhcp_service_send_req(dhcp_client.service_instance, 0, srv_data_ptr, server_address, payload_ptr, payload_len, dhcp_solicit_resp_cb);
    if (srv_data_ptr->transActionId == 0) {
        ns_dyn_mem_free(payload_ptr);
        if (addr) {
            addr->state_timer = 200; //Retry after 20 seconds
        }
        tr_error("DHCP renew send failed");
    }
    if (packetReq.messageType == DHCPV6_SOLICATION_TYPE && dhcp_client.sol_timeout != 0) {
        // Default retry values are modified from specification update to message
        dhcp_service_set_retry_timers(srv_data_ptr->transActionId, dhcp_client.sol_timeout, dhcp_client.sol_max_rt, dhcp_client.sol_max_rc);
    }
    tr_info("DHCP renew send OK");
}

static bool dhcpv6_client_set_address(int8_t interface_id, dhcpv6_client_server_data_t *srv_data_ptr)
{
    protocol_interface_info_entry_t *cur = NULL;
    if_address_entry_t *address_entry = NULL;
    uint32_t renewTimer;

    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return false;
    }
    renewTimer = libdhcpv6_renew_time_define(srv_data_ptr);

    srv_data_ptr->iaNonTemporalStructValid = true;
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
        srv_data_ptr->iaNonTemporalStructValid = false;
        return false;
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
    return true;
}

#endif
