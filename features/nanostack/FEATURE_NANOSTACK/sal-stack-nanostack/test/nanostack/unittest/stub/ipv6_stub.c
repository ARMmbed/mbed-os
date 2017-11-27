/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#define _HAVE_IPV6

#ifdef _HAVE_IPV6

#include "ns_types.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "Common_Protocols/icmpv6.h"
//#include "6LoWPAN/Bootstraps/network_lib.h"
//#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "common_functions.h"

#define TRACE_GROUP_IPv6 "ipv6"

bool ipv6_new_layers_for_unicast;
bool ipv6_new_layers_for_lowpan_multicast;
bool ipv6_new_layers_for_ethernet_multicast;

bool ipv6_use_new_layers(buffer_t *buf, bool multicast)
{
    return false;
}

void ipv6_set_exthdr_provider(ipv6_route_src_t src, ipv6_exthdr_provider_fn_t *fn)
{
}


buffer_routing_info_t *ipv6_buffer_route_to(buffer_t *buf, const uint8_t *next_hop, protocol_interface_info_entry_t *next_if)
{

}

buffer_routing_info_t *ipv6_buffer_route(buffer_t *buf)
{
    return NULL;
}

buffer_t *ipv6_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *ipv6_up(buffer_t *buf)
{
    return NULL;
}

uint16_t ipv6_header_size_required(buffer_t *buf)
{
    return 0;
}

uint16_t ipv6_max_unfragmented_payload(buffer_t *buf, uint16_t mtu_limit)
{
    return 0;
}

uint16_t ipv6_mtu(buffer_t *buf)
{
    return 0;
}

buffer_t *ipv6_down_new(buffer_t *buf)
{
    return NULL;
}

buffer_t *ipv6_forwarding_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *ipv6_consider_forwarding_received_packet(buffer_t *buf, protocol_interface_info_entry_t *cur, const sockaddr_t *ll_src)
{
    return NULL;
}

buffer_t *ipv6_forwarding_up(buffer_t *buf)
{
    return NULL;
}

#endif /* _HAVE_IPV6 */

