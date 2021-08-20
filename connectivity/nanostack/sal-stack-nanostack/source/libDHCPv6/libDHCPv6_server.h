/*
 * Copyright (c) 2014-2015, 2017-2021, Pelion and affiliates.
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

#include "libDHCPv6/libDHCPv6.h"

#define MAX_SUPPORTED_ADDRESS_LIST_SIZE 0x0000fffd
#define DHCP_ADDRESS_ID_START 2

typedef void (dhcp_address_prefer_remove_cb)(int8_t interfaceId, uint8_t *targetAddress, void *prefix_info);
typedef uint8_t *(dhcp_vendor_data_cb)(int8_t interfaceId, uint8_t *ptr, uint16_t *dhcp_vendor_data_len);


typedef struct dhcpv6_allocated_address_entry_s {
    uint8_t             linkId[8];  /*!< Services UL64 */
    uint32_t            iaID;
    uint32_t            T0;
    uint32_t            T1;
    uint32_t            preferredLifetime;
    uint32_t            lifetime;
    uint16_t            linkType;
    uint16_t            allocatedID;
    ns_list_link_t      link;               /*!< List link entry */
} dhcpv6_allocated_address_entry_t;

typedef struct dhcpv6_dns_server_data_s {
    uint8_t             server_address[16];
    uint8_t             *search_list;
    uint8_t             search_list_length;
    ns_list_link_t      link;               /*!< List link entry */
} dhcpv6_dns_server_data_t;

typedef struct dhcpv6_vendor_data_s {
    uint32_t            enterprise_number;
    dhcp_vendor_data_cb *vendor_data_cb;    // Additional vendor data that can be set for individual reply
    uint8_t             *vendor_data;
    uint16_t             vendor_data_length;
    ns_list_link_t      link;               /*!< List link entry */
} dhcpv6_vendor_data_t;

typedef struct dhcpv6_allocated_address_s {
    uint8_t             nonTemporalAddress[16];
    uint8_t             linkId[8];  /*!< Services UL64 */
    uint16_t            linkType;
    uint32_t            iaID;
    uint32_t            T0;
    uint32_t            T1;
    uint32_t            preferredLifetime;
    uint32_t            lifetime;
} dhcpv6_allocated_address_t;


typedef NS_LIST_HEAD(dhcpv6_allocated_address_entry_t, link) dhcpv6_allocated_address_list_t;
typedef NS_LIST_HEAD(dhcpv6_dns_server_data_t, link) dhcpv6_dns_server_list_t;
typedef NS_LIST_HEAD(dhcpv6_vendor_data_t, link) dhcpv6_vendor_data_list_t;

typedef struct dhcp_address_cache_update {
    uint8_t *allocatedAddress;
    bool    allocatedNewAddress;
    uint32_t validLifeTime;
} dhcp_address_cache_update_t;

typedef bool (dhcp_address_add_notify_cb)(int8_t interfaceId, dhcp_address_cache_update_t *address_info, void *route_src);

typedef struct dhcpv6_gua_server_entry_s {
    int8_t                          interfaceId;
    bool                            anonymousAddress: 1;
    bool                            disableAddressList: 1;
    uint16_t                        socketInstance_id;
    uint8_t                         guaPrefix[8];
    uint8_t                         serverDynamic_DUID_length;
    uint32_t                        maxSupportedClients;
    uint8_t                         clientIdDefaultSuffics[6];
    uint16_t                        firstUnusedId;  /*!< This is first unused Id */
    uint32_t                        validLifetime;
    dhcp_duid_options_params_t      serverDUID;
    uint8_t                         *serverDynamic_DUID;
    dhcp_address_prefer_remove_cb   *removeCb;
    dhcp_address_add_notify_cb *addCb;
    dhcpv6_allocated_address_list_t allocatedAddressList;
    dhcpv6_dns_server_list_t dnsServerList;
    dhcpv6_vendor_data_list_t vendorDataList;
    dhcpv6_allocated_address_t  tempAddressEntry;
    ns_list_link_t      link;                   /*!< List link entry */
} dhcpv6_gua_server_entry_s;

bool libdhcpv6_gua_server_list_empty(void);
dhcpv6_gua_server_entry_s *libdhcpv6_gua_server_allocate(uint8_t *prefix, int8_t interfaceId, uint8_t *serverDUID, uint16_t serverDUIDType);
int libdhcpv6_server_duid_set(dhcpv6_gua_server_entry_s *server_info, uint8_t *duid_ptr, uint16_t duid_type, uint8_t duid_length);
void libdhcpv6_gua_server_free_by_prefix_and_interfaceid(uint8_t *prefix, int8_t interfaceId);
void libdhcpv6_gua_servers_time_update(uint32_t timeUpdateInSeconds);
void libdhcpv6_allocated_address_write(uint8_t *ptr, dhcpv6_allocated_address_entry_t *address, dhcpv6_gua_server_entry_s *serverInfo);
void libdhcpv6_address_delete(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address);
dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_interfaceid(int8_t interfaceId, const uint8_t *prefixPtr);
dhcpv6_gua_server_entry_s *libdhcpv6_server_data_get_by_prefix_and_socketinstance(uint16_t socketInstance, uint8_t *prefixPtr);
dhcpv6_allocated_address_t *libdhcpv6_address_allocate(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *euid64, uint16_t linkType, uint32_t iaID, uint32_t T0, uint32_t T1, bool allocateNew);
dhcpv6_dns_server_data_t *libdhcpv6_dns_server_discover(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address);
dhcpv6_dns_server_data_t *libdhcpv6_dns_server_allocate(dhcpv6_gua_server_entry_s *serverInfo, const uint8_t *address);
dhcpv6_vendor_data_t *libdhcpv6_vendor_data_discover(dhcpv6_gua_server_entry_s *serverInfo, uint32_t enterprise_number);
dhcpv6_vendor_data_t *libdhcpv6_vendor_data_allocate(dhcpv6_gua_server_entry_s *serverInfo, uint32_t enterprise_number);
uint16_t libdhcpv6_dns_server_message_sizes(dhcpv6_gua_server_entry_s *serverInfo);
uint16_t libdhcpv6_vendor_data_message_sizes(dhcpv6_gua_server_entry_s *serverInfo);
uint8_t *libdhcpv6_dns_server_message_writes(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *ptr);
uint8_t *libdhcpv6_vendor_data_message_writes(dhcpv6_gua_server_entry_s *serverInfo, uint8_t *ptr);
#else
#define libdhcpv6_gua_server_list_empty() true
#define libdhcpv6_gua_server_allocate(prefix, interfaceId, serverDUID, serverDUIDType) NULL
#define libdhcpv6_server_duid_set(server_info, duid_ptr, duid_type, duid_length)
#define libdhcpv6_gua_server_free_by_prefix_and_interfaceid(prefix, interfaceId)
#define libdhcpv6_gua_servers_time_update(timeUpdateInSeconds)
#define libdhcpv6_allocated_address_write(ptr, address, serverInfo)
#define libdhcpv6_address_delete(serverInfo, address)
#define libdhcpv6_server_data_get_by_prefix_and_interfaceid(interfaceId, prefixPtr) NULL
#define libdhcpv6_server_data_get_by_prefix_and_socketinstance(socketInstance,prefixPtr) NULL
#define libdhcpv6_address_allocate(serverInfo, euid64, linkType, iaID, T0,  T1, allocateNew) NULL
#define libdhcpv6_dns_server_discover(serverInfo, address) NULL
#define libdhcpv6_dns_server_allocate(serverInfo, address) NULL
#define libdhcpv6_vendor_data_discover(serverInfo, enterprise_number) NULL
#define libdhcpv6_vendor_data_allocate(serverInfo, enterprise_number) NULL
#define libdhcpv6_dns_server_message_sizes(serverInfo) 0
#define libdhcpv6_vendor_data_message_sizes(serverInfo) 0
#define libdhcpv6_dns_server_message_writes(serverInfo, ptr) NULL
#define libdhcpv6_vendor_data_message_writes(serverInfo, ptr) NULL
#endif

#endif /* LIBDHCPV6_SERVER_H_ */
