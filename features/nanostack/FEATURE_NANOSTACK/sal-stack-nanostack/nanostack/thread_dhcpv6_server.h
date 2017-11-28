/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
