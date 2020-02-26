/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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

static bool DHCP_server_service_timer_start(void)
{
    if (!dhcp_timer_storage) {
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
    if (dhcp_timer_storage && libdhcpv6_gua_server_list_empty()) {
        eventOS_cancel(dhcp_timer_storage);
        dhcp_timer_storage = NULL;
    }
}

int DHCPv6_server_respond_client(dhcpv6_gua_server_entry_s *serverBase, dhcpv6_reply_packet_s *replyPacket, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, dhcpv6_gua_response_t *response, bool allocateNew)
{
    dhcpv6_allocated_address_t *dhcp_allocated_address = NULL;
    dhcpv6_ia_non_temporal_address_s nonTemporalAddress;
    bool address_allocated = false;
    //Validate Client DUID
    dhcp_link_options_params_t clientDUID;
    if (libdhcpv6_get_link_address_from_duid(replyPacket->clientDUID.duid, replyPacket->clientDUID.duid_length, replyPacket->clientDUID.type, &clientDUID) == 0) {
        dhcp_allocated_address = libdhcpv6_address_allocated_list_scan(serverBase, clientDUID.link_id, clientDUID.link_type, dhcp_ia_non_temporal_params->iaId, dhcp_ia_non_temporal_params->T0, dhcp_ia_non_temporal_params->T1, allocateNew);
    }
    if (dhcp_allocated_address) {
        address_allocated = true;
        nonTemporalAddress.requestedAddress = dhcp_allocated_address->nonTemporalAddress;
        nonTemporalAddress.validLifeTime = dhcp_allocated_address->lifetime;
        nonTemporalAddress.preferredLifeTime = dhcp_allocated_address->preferredLifetime;

        if (serverBase->addCb) {
            dhcp_address_cache_update_t update_info;
            update_info.allocatedAddress = dhcp_allocated_address->nonTemporalAddress;
            update_info.allocatedNewAddress = allocateNew;
            update_info.validLifeTime = nonTemporalAddress.validLifeTime;

            if (!serverBase->addCb(serverBase->interfaceId, &update_info, serverBase->guaPrefix)) {
                address_allocated = false;
                libdhcpv6_address_rm_from_allocated_list(serverBase, dhcp_allocated_address->nonTemporalAddress);
            }
        }
    }

    response->responseLength = libdhcpv6_address_reply_message_len(replyPacket->clientDUID.duid_length, replyPacket->serverDUID.duid_length, 0, replyPacket->rapidCommit, address_allocated);
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
                    replyPacket.serverDUID = serverBase->serverDUID;
                    replyPacket.T0 = dhcp_ia_non_temporal_params.T0;
                    replyPacket.T1 = dhcp_ia_non_temporal_params.T1;
                    replyPacket.iaId = dhcp_ia_non_temporal_params.iaId;
                    replyPacket.transaction_ID = msg_tr_id;

                    tr_debug("Response dhcp sol %s clientDUID", trace_array(replyPacket.clientDUID.duid, replyPacket.clientDUID.duid_length));

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
                    if (libdhcpv6_compare_DUID(&serverBase->serverDUID, &replyPacket.serverDUID) == 0) {
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
    //allocate Socket Service
    socketInstance  = dhcp_service_init(interface, DHCP_INSTANCE_SERVER, DHCPV6_server_service_request_handler);
    cur = protocol_stack_interface_info_get_by_id(interface);
    if (!cur) {
        return -1;
    }

    if (dhcpv6_server_service_tasklet_generated() < 0) {
        retVal = -2;
    } else if (!DHCP_server_service_timer_start()) {
        retVal = -2;
    } else {
        //allocate server
        dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_gua_server_allocate(guaPrefix, interface, cur->mac, serverDUIDType);
        if (serverInfo) {
            serverInfo->socketInstance_id = socketInstance;
            socketInstance = 0;
            retVal = 0;
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

/* Delete dhcp thread dhcp router ID server.
 *
 * When this is called it close selected service and free all allocated memory.
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param delete_gua_addresses Whether or not assigned addresses with the prefix should be removed from the interface.
 */
void DHCPv6_server_service_delete(int8_t interface, uint8_t guaPrefix[static 8], bool delete_gua_addresses)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (serverInfo) {
        ns_list_foreach_safe(dhcpv6_allocated_address_entry_t, cur, &serverInfo->allocatedAddressList) {
            //Delete Server data base
            if (serverInfo->removeCb) {
                uint8_t allocated_address[16];
                libdhcpv6_allocated_address_write(allocated_address, cur, serverInfo);
                serverInfo->removeCb(interface, allocated_address, NULL);
            }
        }

        if (serverInfo->removeCb) {
            // Clean all /128 'Thread Proxy' routes to self and others added when acting as a DHCP server
            serverInfo->removeCb(interface, NULL, serverInfo->guaPrefix);
        }
        dhcp_service_delete(serverInfo->socketInstance_id);
    }

    if (delete_gua_addresses) {
        protocol_interface_info_entry_t *curPtr = protocol_stack_interface_info_get_by_id(interface);
        if (curPtr) {
            protocol_core_dhcpv6_allocated_address_remove(curPtr, guaPrefix);
        }
    }

    libdhcpv6_gua_server_free_by_prefix_and_interfaceid(guaPrefix, interface);

    DHCP_server_service_timer_stop();

}

/* Control GUA address for client by DUI.Default value is true
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param mode true trig autonous mode, false define address by default suffics + client id
 */
int DHCPv6_server_service_set_address_autonous_flag(int8_t interface, uint8_t guaPrefix[static 16], bool mode, bool autonomous_skip_list)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (!serverInfo) {
        return -1;

    }

    serverInfo->enableAddressAutonous = mode;
    if (mode) {
        serverInfo->disableAddressListAllocation = autonomous_skip_list;
    } else {
        serverInfo->disableAddressListAllocation = false;
    }

    return 0;
}

void DHCPv6_server_service_callback_set(int8_t interface, uint8_t guaPrefix[static 16], dhcp_address_prefer_remove_cb *remove_cb, dhcp_address_add_notify_cb *add_cb)
{
    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (!serverInfo) {
        return;
    }

    serverInfo->addCb = add_cb;
    serverInfo->removeCb = remove_cb;
}


int DHCPv6_server_service_duid_update(int8_t interface, uint8_t guaPrefix[static 16],  uint8_t *duid_ptr, uint16_t duid_type, uint8_t duid_length)
{

    //Validate length and type
    if (!libdhcpv6_duid_length_validate(duid_type, duid_length)) {
        return -1;
    }


    dhcpv6_gua_server_entry_s *serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (!serverInfo) {
        return -1;
    }

    return libdhcpv6_server_duid_set(serverInfo, duid_ptr, duid_type, duid_length);
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
    dhcpv6_gua_server_entry_s *serverInfo;
    if (maxClientCount == 0 || maxClientCount > MAX_SUPPORTED_ADDRESS_LIST_SIZE) {
        return -2;
    }
    serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (!serverInfo) {
        return -1;
    }

    serverInfo->maxSupportedClients = maxClientCount;

    return 0;
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
    dhcpv6_gua_server_entry_s *serverInfo;
    if (validLifeTimne < 120) {
        return -2;
    }
    serverInfo = libdhcpv6_server_data_get_by_prefix_and_interfaceid(interface, guaPrefix);
    if (!serverInfo) {
        return -1;
    }
    serverInfo->validLifetime = validLifeTimne;

    return 0;
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
void DHCPv6_server_service_delete(int8_t interface, uint8_t guaPrefix[static 8], bool delete_gua_addresses)
{
    (void) interface;
    (void) guaPrefix;
    (void) delete_gua_addresses;
}
void DHCPv6_server_service_timeout_cb(uint32_t timeUpdateInSeconds)
{
    (void) timeUpdateInSeconds;
}
int DHCPv6_server_service_set_address_autonous_flag(int8_t interface, uint8_t guaPrefix[static 16], bool mode, bool autonomous_skip_list)
{
    (void) interface;
    (void) guaPrefix;
    (void) mode;
    (void) autonomous_skip_list;

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

#endif
