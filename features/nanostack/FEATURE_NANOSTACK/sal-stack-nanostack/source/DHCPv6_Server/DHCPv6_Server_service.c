/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

/*
 * \file DHCPv6_Server_service.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#ifdef HAVE_DHCPV6_SERVER
#include <string.h>
#include <ns_types.h>
#include <ns_trace.h>
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include <nsdynmemLIB.h>
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "DHCPv6_Server/DHCPv6_server_service.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_bbr_api_internal.h"
#include "Common_Protocols/icmpv6.h"
#include "dhcp_service_api.h"

#define TRACE_GROUP "dhcp"

#define DHCPV6_GUA_IF "dhcp"
#define DHCPV6_SERVER_SERVICE_TASKLET_INIT      1
#define DHCPV6_SERVER_SERVICE_TIMER             2

#define DHCPV6_SERVER_SERVICE_TIMER_ID          1

#define DHCPV6_TIMER_UPDATE_PERIOD_IN_SECONDS   10

typedef struct dhcpv6_gua_response {
    uint16_t    responseLength;
    uint8_t     *responsePtr;
} dhcpv6_gua_response_t;

static int8_t dhcpv6_service_tasklet = -1;

static arm_event_storage_t *dhcp_timer_storage = NULL;

static void DHCPV6_server_service_remove_GUA_from_neighcache(protocol_interface_info_entry_t *cur, uint8_t *targetAddress);


static bool DHCP_server_service_timer_start(void)
{
    if(!dhcp_timer_storage) {
        arm_event_s event = {
                .receiver = dhcpv6_service_tasklet,
                .sender = 0,
                .event_id = DHCPV6_SERVER_SERVICE_TIMER_ID,
                .data_ptr = NULL,
                .event_type = DHCPV6_SERVER_SERVICE_TIMER,
                .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        };

        dhcp_timer_storage  = eventOS_event_timer_request_every(&event, eventOS_event_timer_ms_to_ticks(DHCPV6_TIMER_UPDATE_PERIOD_IN_SECONDS * 1000));
        if (!dhcp_timer_storage) {
            tr_error("Dhcp server timer start fail");
            return false;
        }
    }
    return true;
}

static void DHCP_server_service_timer_stop(void)
{
    if (dhcp_timer_storage && libdhcpv6_gua_server_list_empty() ) {
        eventOS_cancel(dhcp_timer_storage);
        dhcp_timer_storage = NULL;
    }
}

int DHCPv6_server_respond_client(dhcpv6_gua_server_entry_s *serverBase, dhcpv6_reply_packet_s *replyPacket, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, dhcpv6_gua_response_t *response, bool allocateNew)
{
    dhcpv6_alloacted_address_entry_t *dhcp_allocated_address;
    dhcpv6_ia_non_temporal_address_s nonTemporalAddress;
    bool address_allocated = false;
    dhcp_allocated_address = libdhcpv6_address_allocated_list_scan(serverBase, replyPacket->clientDUID.linkID, replyPacket->clientDUID.linkType, dhcp_ia_non_temporal_params->iaId, dhcp_ia_non_temporal_params->T0, dhcp_ia_non_temporal_params->T1, allocateNew);
    if (dhcp_allocated_address) {
        address_allocated = true;
        nonTemporalAddress.requestedAddress = dhcp_allocated_address->nonTemporalAddress;
        nonTemporalAddress.validLifeTime = dhcp_allocated_address->lifetime;
        nonTemporalAddress.preferredLifeTime = dhcp_allocated_address->preferredLifetime;

        // If this is solicit from existing address, flush ND cache.
        if (allocateNew) {
            // coverity[returned_null] for ignoring protocol_stack_interface_info_get_by_id NULL return
            DHCPV6_server_service_remove_GUA_from_neighcache(protocol_stack_interface_info_get_by_id(serverBase->interfaceId), nonTemporalAddress.requestedAddress);
        }
        if (thread_bbr_nd_entry_add(serverBase->interfaceId,dhcp_allocated_address->nonTemporalAddress, nonTemporalAddress.validLifeTime, serverBase->guaPrefix, NULL) == -1) {
            // No nanostack BBR present we will put entry for application implemented BBR
            ipv6_route_t *route = ipv6_route_add_with_info(dhcp_allocated_address->nonTemporalAddress, 128, serverBase->interfaceId, NULL, ROUTE_THREAD_PROXIED_HOST,serverBase->guaPrefix,0, nonTemporalAddress.validLifeTime, 0);
            if (!route) {
                address_allocated = false;
                libdhcpv6_address_rm_from_allocated_list(serverBase,dhcp_allocated_address->nonTemporalAddress);
            }


        }
    }

    response->responseLength = libdhcpv6_address_reply_message_len(replyPacket->clientDUID.linkType, replyPacket->serverDUID.linkType, 0, replyPacket->rapidCommit, address_allocated);
    response->responsePtr = ns_dyn_mem_temporary_alloc(response->responseLength);
    if (response->responsePtr) {
        if (address_allocated) {
            libdhcpv6_reply_message_write(response->responsePtr, replyPacket, &nonTemporalAddress, NULL);
        } else {
            libdhcpv6_reply_message_write(response->responsePtr, replyPacket, NULL, NULL);
        }
        return 0;
    }
    return -1;
}


int DHCPV6_server_service_request_handler(uint16_t instance_id, uint32_t msg_tr_id, uint8_t message_type, uint8_t *msg_ptr, uint16_t msg_len)
{
    int retVal = RET_MSG_NOT_MINE;
    dhcp_ia_non_temporal_params_t dhcp_ia_non_temporal_params;
    dhcpv6_reply_packet_s replyPacket;
    dhcpv6_gua_response_t responseBuf;
    dhcpv6_gua_server_entry_s *serverBase;

    tr_debug("GUA Handler");
    switch (message_type) {
        case DHCPV6_SOLICATION_TYPE:
            if (libdhcpv6_solication_message_options_validate(msg_ptr, msg_len, &replyPacket.clientDUID, &dhcp_ia_non_temporal_params) == 0) {
                //Start Build Response
                serverBase = libdhcpv6_server_data_get_by_prefix_and_socketinstance(instance_id, dhcp_ia_non_temporal_params.nonTemporalAddress);
                if (serverBase) {
                    //Here Allocate address
                    replyPacket.rapidCommit = libdhcpv6_rapid_commit_option_at_packet(msg_ptr, msg_len);
                    replyPacket.serverDUID.linkID = serverBase->serverDUID;
                    replyPacket.serverDUID.linkType = serverBase->serverLinkType;
                    replyPacket.T0 = dhcp_ia_non_temporal_params.T0;
                    replyPacket.T1 = dhcp_ia_non_temporal_params.T1;
                    replyPacket.iaId = dhcp_ia_non_temporal_params.iaId;
                    replyPacket.transaction_ID = msg_tr_id;
                    //Check First Current list
                    if (DHCPv6_server_respond_client(serverBase, &replyPacket, &dhcp_ia_non_temporal_params, &responseBuf, true) == 0) {
                        //Respond
                        dhcp_service_send_resp(msg_tr_id, TX_OPT_NONE, responseBuf.responsePtr, responseBuf.responseLength);
                        ns_dyn_mem_free(responseBuf.responsePtr);
                        retVal = RET_MSG_ACCEPTED;
                    }

                }
            }
            break;

        case DHCPV6_RELEASE_TYPE:

            break;

        case DHCPV6_RENEW_TYPE:
            if (libdhcpv6_renew_message_options_validate(msg_ptr, msg_len, &replyPacket.clientDUID, &replyPacket.serverDUID, &dhcp_ia_non_temporal_params) == 0) {
                // Discover SERVER
                serverBase = libdhcpv6_server_data_get_by_prefix_and_socketinstance(instance_id, dhcp_ia_non_temporal_params.nonTemporalAddress);
                if (serverBase) {
                    dhcp_link_options_params_t serverInfoDui;
                    serverInfoDui.linkID = serverBase->serverDUID;
                    serverInfoDui.linkType = serverBase->serverLinkType;
                    if (libdhcpv6_compare_DUID(&serverInfoDui, &replyPacket.serverDUID) == 0) {
                        replyPacket.rapidCommit = libdhcpv6_rapid_commit_option_at_packet(msg_ptr, msg_len);
                        replyPacket.T0 = dhcp_ia_non_temporal_params.T0;
                        replyPacket.T1 = dhcp_ia_non_temporal_params.T1;
                        replyPacket.iaId = dhcp_ia_non_temporal_params.iaId;
                        replyPacket.transaction_ID = msg_tr_id;
                        //Check First Current list
                        if (DHCPv6_server_respond_client(serverBase, &replyPacket, &dhcp_ia_non_temporal_params, &responseBuf, false) == 0) {
                            //Respond
                            dhcp_service_send_resp(msg_tr_id, TX_OPT_NONE, responseBuf.responsePtr, responseBuf.responseLength);
                            ns_dyn_mem_free(responseBuf.responsePtr);
                            retVal = RET_MSG_ACCEPTED;
                        }
                    }

                }
            }
            break;

        default:
            tr_warn("UnSupported MessageId: %02x", message_type);
    }
    return retVal;
}


void DHCPv6_server_service_tasklet(arm_event_s *event)
{
    if (event->event_type == DHCPV6_SERVER_SERVICE_TASKLET_INIT) {
        //We should define peridiocally timer service!!
    } else if (event->event_type == DHCPV6_SERVER_SERVICE_TIMER) {
        libdhcpv6_gua_servers_time_update(DHCPV6_TIMER_UPDATE_PERIOD_IN_SECONDS);
    }
}

static void DHCPV6_server_service_remove_GUA_from_neighcache(protocol_interface_info_entry_t *cur, uint8_t *targetAddress)
{
    ipv6_neighbour_t *neighbour_entry;

    neighbour_entry = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, targetAddress);
    if (neighbour_entry) {
        tr_debug("Remove from neigh Cache: %s", tr_ipv6(targetAddress));
        ipv6_neighbour_entry_remove(&cur->ipv6_neighbour_cache, neighbour_entry);
    }
}

void DHCPv6_server_service_address_preferred_timeout_handler(int8_t interfaceId, uint8_t *targetAddress)
{
    tr_warn("Address Preferred Timeout");
    protocol_interface_info_entry_t *cur = 0;
    //allocate Socket Service

    cur = protocol_stack_interface_info_get_by_id(interfaceId);
    if (cur) {
        ipv6_route_delete(targetAddress, 128, interfaceId, NULL, ROUTE_THREAD_PROXIED_HOST);
        DHCPV6_server_service_remove_GUA_from_neighcache(cur, targetAddress);
    }
}

static int8_t dhcpv6_server_service_tasklet_generated(void)
{
    if (dhcpv6_service_tasklet == -1) {
        dhcpv6_service_tasklet = eventOS_event_handler_create(DHCPv6_server_service_tasklet, DHCPV6_SERVER_SERVICE_TASKLET_INIT);
    }

    return dhcpv6_service_tasklet;
}

/* Initialize dhcp Global address server.
 *
 * This instance needs to bee initialized once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this thread instance.
 * /param guaPrefix Global prefix /64
 * /param serverDUID Server Device ID (64-bit MAC)
 * /param serverDUIDType
 *
 */
int DHCPv6_server_service_init(int8_t interface, uint8_t guaPrefix[static 16], uint8_t serverDUID[static 8], uint16_t serverDUIDType)
{
    int retVal = -1;
    uint16_t socketInstance;
    protocol_interface_info_entry_t *cur;
    (void)serverDUID;
    (void)serverDUIDType;
    //allocate Socket Service
    socketInstance  = dhcp_service_init(interface, DHCP_INSTANCE_SERVER, DHCPV6_server_service_request_handler);
    cur = protocol_stack_interface_info_get_by_id(interface);
    if (cur) {
        if (dhcpv6_server_service_tasklet_generated() < 0) {
            retVal = -2;
        } else if (!DHCP_server_service_timer_start()) {
            retVal = -2;
        } else {
            //allocate server
            dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_gua_server_allocate(guaPrefix, interface, cur->mac, DHCPV6_DUID_HARDWARE_EUI64_TYPE, DHCPv6_server_service_address_preferred_timeout_handler);
            if (serverInfo) {
                //if_address_entry_t *def_address = NULL;
                uint8_t temp[16];
                uint8_t *ptr = temp;
                //define address & Route advert's
                memcpy(ptr, guaPrefix, 8);
                ptr += 8;
                memcpy(ptr, cur->iid_slaac, 8);

                serverInfo->socketInstance_id = socketInstance;
                socketInstance = 0;
                //Generate Address for current interface
                retVal = 0;
            }
        }
    }
    if (socketInstance > 0) {
        dhcp_service_delete(socketInstance);
        DHCP_server_service_timer_stop();
    }
    return retVal;
}

void DHCPv6_server_service_timeout_cb(uint32_t timeUpdateInSeconds)
{
    libdhcpv6_gua_servers_time_update(timeUpdateInSeconds);
}

void DHCPv6_GUA64_ML64_route_control(int8_t interfaceId, uint8_t *allocatedGuaAddress, uint8_t *clientEUID64, uint8_t *meshLocalPrefix, bool deleteMapping)
{
    uint8_t ml64[16];
    uint8_t *ptr = ml64;
    memcpy(ptr, meshLocalPrefix, 8);
    ptr += 8;
    memcpy(ptr, clientEUID64, 8);
    *ptr ^= 2;
    //Generate Route Info
    if (deleteMapping) {
        ipv6_route_delete(allocatedGuaAddress, 128, interfaceId, ml64, ROUTE_STATIC);
    } else if (ipv6_route_add(allocatedGuaAddress, 128, interfaceId, ml64, ROUTE_STATIC, 0xffffffff, 0) == 0) {
        tr_debug("Route ADD OK");
    } else {
        tr_warn("Route Add fail");
    }

}

/* Delete dhcp thread dhcp router ID server.
 *
 * When this is called it close selected service and free all allocated memory.
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param delete_gua_addresses Whether or not assigned addresses with the prefix should be removed from the interface.
 */
void DHCPv6_server_service_delete(int8_t interface, uint8_t guaPrefix[static 16], bool delete_gua_addresses)
{
    protocol_interface_info_entry_t *curPtr = 0;
    //allocate Socket Service

    curPtr = protocol_stack_interface_info_get_by_id(interface);
    if (curPtr) {
        dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
        if (serverInfo) {
            ns_list_foreach_safe(dhcpv6_alloacted_address_entry_t, cur, &serverInfo->allocatedAddressList) {
                //Delete Server data base
                DHCPV6_server_service_remove_GUA_from_neighcache(curPtr, cur->nonTemporalAddress);
            }
            // Clean all /128 'Thread Proxy' routes to self and others added when acting as a DHCP server
            ipv6_route_table_remove_info(curPtr->id, ROUTE_THREAD_PROXIED_HOST,serverInfo->guaPrefix);
            dhcp_service_delete(serverInfo->socketInstance_id);
        }

        if (delete_gua_addresses) {
            protocol_core_dhcpv6_allocated_address_remove(curPtr, guaPrefix);
        }

        libdhcpv6_gua_server_free_by_prefix_and_interfaceid(guaPrefix, interface);

        DHCP_server_service_timer_stop();
    }
}

/* Control GUA address for client by DUI.Default value is true
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param mode true trig autonous mode, false define address by default suffics + client id
 */
int DHCPv6_server_service_set_address_autonous_flag(int8_t interface, uint8_t guaPrefix[static 16], bool mode)
{
    int retVal = -1;
    dhcpv6_gua_server_entry_s *serverInfo;

    serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (serverInfo) {
        serverInfo->enableAddressAutonous = mode;
        retVal = 0;
    }

    return retVal;
}

/* Enable or disable GUA64 Address mapping to ML64
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param mode
 *  /param meshLocalPrefix mesh local prefix for generate ML6 from client EUID64
 */
int DHCPv6_server_service_set_gua_address_mapping(int8_t interface, uint8_t guaPrefix[static 16], bool mode, uint8_t meshLocalPrefix[8])
{
    int retVal = -1;
    dhcpv6_gua_server_entry_s *serverInfo;

    serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (serverInfo) {
        if (mode == true && meshLocalPrefix == NULL) {
            retVal = -2;
        } else {
            serverInfo->enableAddressMapping = mode;
            if (meshLocalPrefix) {
                memcpy(serverInfo->meshLocalPrefix, meshLocalPrefix, 8);
            }
            retVal = 0;
        }
    }

    return retVal;
}

/* SET max accepted clients to server, Default is 200
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param maxClientCount
 */
int DHCPv6_server_service_set_max_clients_accepts_count(int8_t interface, uint8_t guaPrefix[static 16], uint32_t maxClientCount)
{
    int retVal = -1;
    dhcpv6_gua_server_entry_s *serverInfo;
    if (maxClientCount == 0) {
        return -2;
    } else {
        serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
        if (serverInfo) {
            serverInfo->maxSuppertedClients = maxClientCount;
            retVal = 0;
        }
    }
    return retVal;
}

/** SET Address Valid Lifetime parameter for allocated address, Default is 7200 seconds
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param validLifeTimne in seconds
 */
int DHCPv6_server_service_set_address_validlifetime(int8_t interface, uint8_t guaPrefix[static 16], uint32_t validLifeTimne)
{
    int retVal = -1;
    dhcpv6_gua_server_entry_s *serverInfo;
    if (validLifeTimne < 120) {
        retVal = -2;
    } else {
        serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
        if (serverInfo) {
            serverInfo->validLifetime = validLifeTimne;
            retVal = 0;
        }
    }
    return retVal;
}

int DHCPv6_server_service_gua_target_mac_check(int8_t interfaceId, const uint8_t *targetGUA, uint8_t *targetEUI64)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, targetGUA);
    if (serverInfo) {
        dhcpv6_alloacted_address_entry_t *entry = libdhcpv6_address_get_from_allocated_list(serverInfo, targetGUA);
        if (entry) {
            if (entry->preferredLifetime && (entry->linkType == DHCPV6_DUID_HARDWARE_EUI64_TYPE)) {
                memcpy(targetEUI64, entry->linkId, 8);
                return 0;
            }
        }
    }
    return -1;
}
#else

int DHCPv6_server_service_init(int8_t interface, uint8_t guaPrefix[static 16], uint8_t serverDUID[static 8], uint16_t serverDUIDType)
{
    (void) interface;
    (void) guaPrefix;
    (void) serverDUID;
    (void) serverDUIDType;

    return -1;
}
void DHCPv6_server_service_delete(int8_t interface, uint8_t guaPrefix[static 16], bool delete_gua_addresses)
{
    (void) interface;
    (void) guaPrefix;
    (void) delete_gua_addresses;
}
void DHCPv6_server_service_timeout_cb(uint32_t timeUpdateInSeconds)
{
    (void) timeUpdateInSeconds;
}
int DHCPv6_server_service_set_address_autonous_flag(int8_t interface, uint8_t guaPrefix[static 16], bool mode)
{
    (void) interface;
    (void) guaPrefix;
    (void) mode;

    return -1;
}
int DHCPv6_server_service_set_gua_address_mapping(int8_t interface, uint8_t guaPrefix[static 16], bool mode, uint8_t meshLocalPrefix[8])
{
    (void) interface;
    (void) guaPrefix;
    (void) mode;
    (void) meshLocalPrefix;

    return -1;
}
int DHCPv6_server_service_set_max_clients_accepts_count(int8_t interface, uint8_t guaPrefix[static 16], uint32_t maxClientCount)
{
    (void) interface;
    (void) guaPrefix;
    (void) maxClientCount;

    return -1;
}
int DHCPv6_server_service_set_address_validlifetime(int8_t interface, uint8_t guaPrefix[static 16], uint32_t validLifeTimne)
{
    (void) interface;
    (void) guaPrefix;
    (void) validLifeTimne;

    return -1;
}
int DHCPv6_server_service_gua_target_mac_check(int8_t interfaceId, const uint8_t *targetGUA, uint8_t *targetEUI64)
{
    (void) interfaceId;
    (void) targetGUA;
    (void) targetEUI64;

    return -1;
}

#endif
