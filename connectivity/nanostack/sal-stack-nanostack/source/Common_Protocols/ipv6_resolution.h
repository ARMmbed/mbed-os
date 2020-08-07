/*
 * Copyright (c) 2015-2017, 2019, Arm Limited and affiliates.
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


#ifndef IPV6_RESOLUTION_H_
#define IPV6_RESOLUTION_H_

#include "Core/include/ns_address_internal.h"

struct ipv6_neighbour;
struct ipv6_neighbour_cache;
struct buffer;
struct protocol_interface_info_entry;

struct ipv6_neighbour *ipv6_interface_resolve_new(struct protocol_interface_info_entry *cur, struct buffer *buf);
void ipv6_interface_resolve_send_ns(struct ipv6_neighbour_cache *cache, struct ipv6_neighbour *entry, bool unicast, uint_fast8_t seq);
void ipv6_interface_resolution_failed(struct ipv6_neighbour_cache *cache, struct ipv6_neighbour *entry);
void ipv6_send_queued(struct ipv6_neighbour *neighbour);
struct ipv6_neighbour_cache *ipv6_neighbour_cache_by_interface_id(int8_t interface_id);
bool ipv6_map_ip_to_ll(struct protocol_interface_info_entry *cur, struct ipv6_neighbour *n, const uint8_t ip_addr[16], addrtype_t *ll_type, const uint8_t **ll_addr_out);
bool ipv6_map_ll_to_ip_link_local(struct protocol_interface_info_entry *cur, addrtype_t ll_type, const uint8_t *ll_addr, uint8_t ip_addr_out[16]);
typedef uint16_t (ll_addr_handler_t)(int8_t nwk_id, addrtype_t addr_type, const uint8_t *addr_ptr);
uint16_t ipv6_map_ip_to_ll_and_call_ll_addr_handler(struct protocol_interface_info_entry *cur, int8_t interface_id, struct ipv6_neighbour *n, const uint8_t ipaddr[16], ll_addr_handler_t *ll_addr_handler_ptr);

#endif /* IPV6_RESOLUTION_H_ */
