/*
 * Copyright (c) 2014-2015, 2017-2018, Arm Limited and affiliates.
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
 * \file libDHCPv6_server.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef LIBDHCPV6_SERVER_H_
#define LIBDHCPV6_SERVER_H_
#ifdef HAVE_DHCPV6_SERVER
#include "ns_list.h"

typedef void (dhcp_address_prefer_remove_cb)(int8_t interfaceId, uint8_t *targetAddress, void *prefix_info);

typedef struct dhcpv6_alloacted_address_entry_s {
    uint8_t             nonTemporalAddress[16];
    uint8_t             linkId[8];  /*!< Services UL64 */
    uint16_t            linkType;
    uint32_t            iaID;
    uint32_t            T0;
    uint32_t            T1;
    uint32_t            preferredLifetime;
    uint32_t            lifetime;
    ns_list_link_t      link;               /*!< List link entry */
} dhcpv6_alloacted_address_entry_t;

typedef NS_LIST_HEAD(dhcpv6_alloacted_address_entry_t, link) dhcpv6_alloacted_address_list_t;

typedef struct thread_dhcpv6_server_data_s {
    uint8_t           prefix[8];    /*!< Services Prefix */
    uint16_t          maxSuppertedClients;
    uint32_t          clientIdSequence;     /*!< Define  */
    dhcpv6_alloacted_address_list_t allocatedAddressList;
    ns_list_link_t      link;               /*!< List link entry */
} dhcpv6_server_data_entry_t;

typedef struct dhcp_address_cache_update {
    uint8_t *allocatedAddress;
    bool    allocatedNewAddress;
    uint32_t validLifeTime;
} dhcp_address_cache_update_t;

typedef bool (dhcp_address_add_notify_cb)(int8_t interfaceId, dhcp_address_cache_update_t *address_info, void *route_src);

typedef struct dhcpv6_gua_server_entry_s {
    int8_t      interfaceId;
    bool        enableAddressAutonous;
    uint16_t    socketInstance_id;
    uint8_t     guaPrefix[8];
    uint8_t     serverDUID[8];
    uint16_t    serverLinkType;
    uint32_t    maxSuppertedClients;
    uint32_t    clientIdDefaultSuffics;
    uint32_t    clientIdSequence;       /*!< Define  */
    uint32_t    validLifetime;
    dhcp_address_prefer_remove_cb *removeCb;
    dhcp_address_add_notify_cb *addCb;
    dhcpv6_alloacted_address_list_t allocatedAddressList;
    ns_list_link_t      link;                   /*!< List link entry */
} dhcpv6_gua_server_entry_s;

bool libdhcpv6_gua_server_list_empty(void);
dhcpv6_gua_server_entry_s *libdhcpv6_gua_server_allocate(uint8_t *prefix, int8_t interfaceId, uint8_t *serverDUID, uint16_t serverDUIDType);
void libdhcpv6_gua_server_free_by_prefix_and_interfaceid(uint8_t *prefix, int8_t interfaceId);
void libdhcpv6_gua_servers_time_update(uint32_t timeUpdateInSeconds);
void libdhcpv6_address_rm_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address);
dhcpv6_alloacted_address_entry_t *libdhcpv6_address_get_from_allocated_list(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address);
dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_interfaceid(int8_t interfaceId, const uint8_t *prefixPtr);
dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_socketinstance(uint16_t socketInstance, uint8_t *prefixPtr);
dhcpv6_alloacted_address_entry_t *libdhcpv6_address_allocated_list_scan(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *euid64, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew);
#else
#define libdhcpv6_gua_server_list_empty() true
#define libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefixPtr) NULL
#define libdhcpv6_server_data_get_by_prefix_and_socketinstance(socketInstance, prefixPtr) NULL
#define libdhcpv6_gua_server_allocate(prefix, interfaceId, serverDUID, serverDUIDType) NULL
#define libdhcpv6_gua_server_free_by_prefix_and_interfaceid(prefix, interfaceId) ((void)0)
#define libdhcpv6_gua_servers_time_update(timeUpdateInSeconds) ((void)0)
#define libdhcpv6_gua_server_free_by_interfaceid(interfaceId) ((void)0)
#define libdhcpv6_address_get_from_allocated_list(serverInfo, address) NULL
#define libdhcpv6_address_rm_from_allocated_list(serverInfo, address) ((void)0)
#define libdhcpv6_address_allocated_list_scan( serverInfo, linkId, linkType, iaID, T0, T1, allocateNew) NULL
#endif

#endif /* LIBDHCPV6_SERVER_H_ */
