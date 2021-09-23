/*
 * Copyright (c) 2014-2021, Pelion and affiliates.
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
 * \file DHCPv6_server_service.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef DHCPV6_SERVER_SERVICE_H_
#define DHCPV6_SERVER_SERVICE_H_
#ifdef HAVE_DHCPV6_SERVER
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"

/* Initialize dhcp Global address server.
 *
 * This instance needs to bee initialized once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this thread instance.
 * /param guaPrefix Global prefix /64
 * /param serverDUID Server Device ID (64-bit MAC)
 * /param serverDUIDType
 * /param maxClientCnt
 * /param enableAddressMapping
 *
 */
int DHCPv6_server_service_init(int8_t interface, uint8_t guaPrefix[static 16], uint8_t serverDUID[static 8], uint16_t serverDUIDType);


void DHCPv6_server_service_callback_set(int8_t interface, uint8_t guaPrefix[static 16], dhcp_address_prefer_remove_cb *remove_cb, dhcp_address_add_notify_cb *add_cb);

int DHCPv6_server_service_duid_update(int8_t interface, uint8_t guaPrefix[static 16],  uint8_t *duid_ptr, uint16_t duid_type, uint8_t duid_length);

/* Delete dhcp thread dhcp router ID server.
 *
 * When this is called it close selected service and free all allocated memory.
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param delete_gua_addresses Whether or not assigned addresses with the prefix should be removed from the interface.
 */
void DHCPv6_server_service_delete(int8_t interface, uint8_t guaPrefix[static 8], bool delete_gua_addresses);


void DHCPv6_server_service_timeout_cb(uint32_t timeUpdateInSeconds);

/* Control GUA address for client by DUI.Default value is true
 *
 * Anonymous and disable address list can optimize either
 * Using 16 bit suffix to optimize data amount in network
 * and having list of assigned addresses meaning larger RAM usage at border router
 *
 * or Using SLAAC type address generation and not have a list of addresses at Border router
 * -> Less RAM usage, but more bandwidth used
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param mode true assign addresses anonymously. false define address by Prefix + client id
 *  /param disable_address_list Dont keep track of assigned Addresses (Can't be used if anonymous)
 */
int DHCPv6_server_service_set_address_generation_anonymous(int8_t interface, uint8_t guaPrefix[static 16], bool mode, bool autonomous_skip_list);


/* SET max accepted clients to server, Default is 200
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param maxClientCount
 */
int DHCPv6_server_service_set_max_clients_accepts_count(int8_t interface, uint8_t guaPrefix[static 16], uint32_t maxClientCount);


/* SET Address Valid Lifetime parameter for allocated address, Default is 7200 seconds
 *
 *
 *  /param interface interface id of this thread instance.
 *  /param guaPrefix Prefix which will be removed
 *  /param validLifeTimne in seconds
 */
int DHCPv6_server_service_set_address_validlifetime(int8_t interface, uint8_t guaPrefix[static 16], uint32_t validLifeTimne);

int DHCPv6_server_service_set_dns_server(int8_t interface, uint8_t guaPrefix[static 16], uint8_t dns_server_address[static 16], uint8_t *dns_search_list_ptr, uint8_t dns_search_list_len);

int DHCPv6_server_service_set_vendor_data(int8_t interface, uint8_t guaPrefix[static 16], uint32_t enterprise_number, uint8_t *dhcp_vendor_data_ptr, uint16_t dhcp_vendor_data_len);

int DHCPv6_server_service_set_vendor_data_callback(int8_t interface, uint8_t guaPrefix[static 16], uint32_t enterprise_number, dhcp_vendor_data_cb *vendor_data_cb);

#else
#define DHCPv6_server_service_delete(interface, guaPrefix, delete_gua_addresses)
#endif
#endif /* DHCPV6_SERVER_SERVICE_H_ */
