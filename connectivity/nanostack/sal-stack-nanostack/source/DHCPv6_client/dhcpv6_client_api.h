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

#ifndef DHCPV6_CLIENT_API_H_
#define DHCPV6_CLIENT_API_H_
#include <ns_types.h>

/* DHCP client implementation.
 *
 * Responsibilities of this module are:
 * - handle Global address queries and refresh inside network.
 *
 */

/* Initialize dhcp client.
 *
 * This instance needs to bee initialized once for each network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this instance.
 * /param link_type DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE, DHCPV6_DUID_HARDWARE_EUI64_TYPE or DHCPV6_DUID_HARDWARE_EUI48_TYPE
 *
 */
void dhcp_client_init(int8_t interface, uint16_t link_type);

/* Set configurations for DHCP client
 *
 * /param interface Client Inteface ID
 * /param renew_uses_solicit Instead of renew message SOLICIT is used.
 * /param one_client_for_this_interface True Interface use oneinstance for allocate address
 * /param no_address_hint IAID use address at Solicit
 */
void dhcp_client_configure(int8_t interface, bool renew_uses_solicit, bool one_client_for_this_interface, bool no_address_hint);

/* Set Timeout parameters for SOLICIT transactions
 *
 * /param timeout SOLICIT timeout initial value. 0 means use defaults
 * /param max_rt SOLICIT timeout max value.
 * /param max_rc SOLICIT re-transmission count. 0 means infinite.
 */
void dhcp_client_solicit_timeout_set(int8_t interface, uint16_t timeout, uint16_t max_rt, uint8_t max_rc);

/* Delete dhcp client.
 *
 * When this is called all addressed assigned by this module are removed from stack.
 */
void dhcp_client_delete(int8_t interface);

/* Global address handler.
 *
 * This module updates the addresses from dhcp server and sets them in stack.
 * this module makes refresh of address when needed.
 *
 */


/* give dhcp server and prefix for global address assignment
 *
 * /param interface interface where address is got
 * /param dhcp_addr dhcp server ML16 address where address is registered.
 * /param prefix dhcp server ML16 address where address is registered.
 * /param error_cb error callback that is called if address cannot be created or becomes invalid.
 * /param register_status true if address registered.
 *
 */
typedef void (dhcp_client_global_adress_cb)(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status);

int dhcp_client_get_global_address(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], dhcp_client_global_adress_cb *error_cb);

/* Renew all leased adddresses might be used when short address changes
 *
 * /param interface interface where address is got
 */
void dhcp_client_global_address_renew(int8_t interface);

/* Delete address from device
 *  if prefix is NULL all are deleted
 *
 * /param interface interface where address is got
 * /param prefix dhcp server ML16 address where address is registered.
 *
 */
void dhcp_client_global_address_delete(int8_t interface, uint8_t *dhcp_addr, uint8_t prefix[static 16]);


void dhcp_relay_agent_enable(int8_t interface, uint8_t border_router_address[static 16]);

int dhcp_client_server_address_update(int8_t interface, uint8_t *prefix, uint8_t server_address[static 16]);



#endif /* DHCPV6_CLIENT_API_H_ */
