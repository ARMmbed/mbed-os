/*
 * Copyright (c) 2014-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/**
 * \file thread_dhcpv6_server.h
 * \brief Thread specific DHCP server.
 *
 * \warning This api will be removed as there is no Thread specific modifications anymore.
 */

#ifndef THREAD_DHCPV6_SERVER_H_
#define THREAD_DHCPV6_SERVER_H_

#include "ns_types.h"

/**
 * Set DHCPV6 server for Thread GP data purpose
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix
 * \param max_client_cnt Max number of clients
 * \param stableData True if data is stable
 *
 * \return 0, Set OK
 * \return <0 Set Not OK
 */
int thread_dhcpv6_server_add(int8_t interface_id, uint8_t *prefix_ptr, uint16_t max_client_cnt, bool stableData);

/**
 * Modify dhcp address valid lifetime values default value is 1 day (86400)
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix (size must be 16 bytes)
 * \param valid_lifetime New valid life value given to new address valid values are (120 - 0xffffffff)
 *
 * \return 0, ADD OK
 * \return <0 Add Not OK
 */
int thread_dhcpv6_server_set_lifetime(int8_t interface_id, uint8_t *prefix_ptr, uint32_t valid_lifetime);

/**
 * Modify dhcp maximum client count.
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix  (size must be 16 bytes)
 * \param max_client_count maximum amount of clients valid values (1 - 0xffffffff)
 *
 * \return 0, ADD OK
 * \return <0 Add Not OK
 */
int thread_dhcpv6_server_set_max_client(int8_t interface_id, uint8_t *prefix_ptr, uint32_t max_client_count);

/**
 * Modify dhcp anonymous mode.
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix  (size must be 16 bytes)
 * \param anonymous true == addressing mode is anonymous, false == addressing mode uses mac address after prefix.
 *
 * \return 0, ADD OK
 * \return <0 Add Not OK
 */
int thread_dhcpv6_server_set_anonymous_addressing(int8_t interface_id, uint8_t *prefix_ptr, bool anonymous);

/**
 * Stop DHCPV6 server for Thread GP data purpose
 *
 * \param interface_id Network Interface
 * \param prefix_ptr pointer DHCPv6 Server Given Prefix
 *
 * \return 0, Stop OK
 * \return <0 Stop Not OK
 */
int thread_dhcpv6_server_delete(int8_t interface_id, uint8_t *prefix_ptr);

#endif /* THREAD_DHCPV6_SERVER_H_ */
