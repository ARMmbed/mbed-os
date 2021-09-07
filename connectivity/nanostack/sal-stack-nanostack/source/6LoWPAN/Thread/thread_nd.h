/*
 * Copyright (c) 2014-2018, Pelion and affiliates.
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

/*
 * \file thread_nd.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_ND_H_
#define THREAD_ND_H_

#ifdef HAVE_THREAD_NEIGHBOR_DISCOVERY

struct ipv6_neighbour_cache;
struct ipv6_neighbour;
struct protocol_interface_info_entry;

int thread_nd_service_activate(int8_t interfaceId);
int thread_nd_service_disable(int8_t interfaceId);
void thread_nd_service_delete(int8_t interfaceId);
int thread_nd_client_service_activate(int8_t interfaceId);

bool thread_nd_ns_transmit(struct protocol_interface_info_entry *cur, struct ipv6_neighbour *entry,  bool unicast, uint8_t seq);
buffer_t *thread_nd_snoop(protocol_interface_info_entry_t *cur, buffer_t *buf, const sockaddr_t *ll_dst, const sockaddr_t *ll_src, bool *bounce);
buffer_t *thread_nd_special_forwarding(struct protocol_interface_info_entry *cur, buffer_t *buf, const sockaddr_t *ll_src, bool *bounce);
buffer_t *thread_nd_icmp_handler(struct protocol_interface_info_entry *cur, buffer_t *buf, bool *bounce);
int thread_nd_map_anycast_address(protocol_interface_info_entry_t *cur, uint16_t *addr16);
void thread_nd_address_remove(protocol_interface_info_entry_t *cur_interface, addrtype_t ll_type, const uint8_t *ll_address);
void thread_nd_flush_neighbour_cache_for_short_addr(struct protocol_interface_info_entry *cur, uint16_t flushee, bool children);

int thread_nd_address_registration(struct protocol_interface_info_entry *cur, const uint8_t *ipv6Address, uint16_t mac16, uint16_t panId, const uint8_t *mac64, bool *new_neighbour_created);

#else //HAVE_THREAD_NEIGHBOR_DISCOVERY

NS_DUMMY_DEFINITIONS_OK

#define thread_nd_service_activate(interfaceId)
#define thread_nd_service_disable(interfaceId)
#define thread_nd_service_delete(interfaceId)
#define thread_nd_client_service_activate(interfaceId)
#define thread_nd_address_remove(cur_interface, l_type, ll_address)

#define thread_nd_map_anycast_address(cur, addr16)

#define thread_nd_flush_neighbour_cache_for_short_addr(cur, flushee, children)

#define thread_nd_address_registration(cur, ipv6Address, mac16, panId, mac64)

/* Functions below assigned as function pointers to core only, so just make those pointers untyped NULL */
#define thread_nd_ns_transmit NULL
#define thread_nd_snoop NULL
#define thread_nd_special_forwarding NULL
#define thread_nd_icmp_handler NULL

#endif //HAVE_THREAD_NEIGHBOR_DISCOVERY

#endif /* THREAD_ND_H_ */
