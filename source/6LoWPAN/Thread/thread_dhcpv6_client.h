/*
 * Copyright (c) 2014-2015, 2017, Arm Limited and affiliates.
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


#ifndef SOURCE_6LOWPAN_THREAD_THREAD_DHCPV6_CLIENT_H_
#define SOURCE_6LOWPAN_THREAD_THREAD_DHCPV6_CLIENT_H_

#include <ns_types.h>

/* Thread DHCP client implementation.
 *
 * Responsibilities of this module are:
 * - send router id address request and receive new router address and inform it to thread bootstrap.
 * - handle Global address queries and refresh inside thread network.
 *
 */

/* Initialize dhcp thread dhcp client.
 *
 * This instance needs to bee initialized once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this thread instance.
 *
 */

void thread_dhcp_client_init(int8_t interface);

/* Delete dhcp thread dhcp client.
 *
 * When this is called all addressed assigned by this module are removed from stack.
 */
void thread_dhcp_client_delete(int8_t interface);

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
 * /param mac64 64 bit mac address for identifieng client.
 * /param error_cb error callback that is called if address cannot be created or becomes invalid.
 * /param register_status true if address registered.
 *
 */
typedef void (thread_dhcp_client_global_adress_cb)(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], bool register_status);

int thread_dhcp_client_get_global_address(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16], uint8_t mac64[static 8], thread_dhcp_client_global_adress_cb *error_cb);

/* Renew all leased adddresses might be used when short address changes
 *
 * /param interface interface where address is got
 */
void thread_dhcp_client_global_address_renew(int8_t interface);

/* Delete address from device
 *  if prefix is NULL all are deleted
 *
 * /param interface interface where address is got
 * /param prefix dhcp server ML16 address where address is registered.
 *
 */
void thread_dhcp_client_global_address_delete(int8_t interface, uint8_t dhcp_addr[static 16], uint8_t prefix[static 16]);

#endif /* SOURCE_6LOWPAN_THREAD_THREAD_DHCPV6_CLIENT_H_ */
