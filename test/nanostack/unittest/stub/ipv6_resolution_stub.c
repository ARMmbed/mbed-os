/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "string.h"
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/ND/nd_router_object.h" // for gp_address_ functions - better place?
#include "ipv6_stack/ipv6_routing_table.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Common_Protocols/ipv6_resolution.h"
#ifndef NO_TCP
#include "Common_Protocols/tcp.h"
#endif
#include "Service_Libs/whiteboard/whiteboard.h"
#include "Service_Libs/etx/etx.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "randLIB.h"

#include "ipv6_resolution_stub.h"

#define TRACE_GROUP "ip6r"


ipv6_resolution_stub_def ipv6_resolution_stub;


void ipv6_interface_resolve_send_ns(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, bool unicast, uint_fast8_t seq)
{
}

void ipv6_interface_resolution_failed(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
}

/* Silly bit of interface glue - ipv6_routing_table.c doesn't know about interface structures,
 * but it needs to be able to get from the interface id in the Routing Table and/or
 * Destination Cache to the relevant Neighbour Cache
 */
ipv6_neighbour_cache_t *ipv6_neighbour_cache_by_interface_id(int8_t interface_id)
{
    ipv6_resolution_stub.uint8_input_value = interface_id;
    return ipv6_resolution_stub.ipv6_neighbour_cache;
}

void ipv6_send_queued(ipv6_neighbour_t *entry)
{
}


ipv6_neighbour_t *ipv6_interface_resolve_new(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    return NULL;
}

/* Attempt a mapping from current information (neighbour cache, hard mappings) */
bool ipv6_map_ip_to_ll(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *n, const uint8_t ip_addr[16], addrtype_t *ll_type, const uint8_t **ll_addr_out)
{
    return false;
}

/* Attempt a mapping from current information (neighbour cache, hard mappings) */
bool ipv6_map_ll_to_ip_link_local(protocol_interface_info_entry_t *cur, addrtype_t ll_type, const uint8_t *ll_addr, uint8_t ip_addr_out[16])
{
    return false;
}

/* To comply with ETX returns 0xFFFF when neighbor doesn't exist and 0 when neighbor is currently unknown. */
uint16_t ipv6_map_ip_to_ll_and_call_ll_addr_handler(protocol_interface_info_entry_t *cur, int8_t interface_id, ipv6_neighbour_t *n, const uint8_t ipaddr[16], ll_addr_handler_t *ll_addr_handler_ptr)
{
    return 0;
}
