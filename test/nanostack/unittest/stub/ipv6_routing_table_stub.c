/*
 * Copyright (c) 2012-2017, Arm Limited and affiliates.
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
 * ipv6_routing_table.c
 *
 *  Implements IPv6 Neighbour Cache (RFC 4861), Destination Cache (RFC 4861),
 *  and Routing Table (RFC 4191, incorporating the RFC 4861 Prefix List)
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "common_functions.h"
#include "ip6string.h"
#include "randLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "net_interface.h"
#include "Core/include/address.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "Common_Protocols/icmpv6.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "nsdynmemLIB.h"

#ifndef FEA_TRACE_SUPPORT
#define TRACE_LEVEL_DEBUG 0
#endif
#define TRACE_GROUP "rout"

#define NCACHE_MAX_LONG_TERM    8
#define NCACHE_MAX_SHORT_TERM   32

#define DCACHE_MAX_LONG_TERM    16
#define DCACHE_MAX_SHORT_TERM   64

#define NCACHE_GC_PERIOD    20
#define DCACHE_GC_PERIOD    20

#define NCACHE_GC_AGE 600
#define DCACHE_GC_AGE 30

void ipv6_router_gone(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
}

int8_t ipv6_neighbour_set_current_max_cache(uint16_t max_cache)
{
    return 0;
}


void ipv6_neighbour_cache_init(ipv6_neighbour_cache_t *cache, int8_t interface_id)
{
}

void ipv6_neighbour_cache_flush(ipv6_neighbour_cache_t *cache)
{
}


ipv6_neighbour_t *ipv6_neighbour_lookup(ipv6_neighbour_cache_t *cache, const uint8_t *address)
{
    return NULL;
}

void ipv6_neighbour_entry_remove(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
}

ipv6_neighbour_t *ipv6_neighbour_lookup_or_create(ipv6_neighbour_cache_t *cache, const uint8_t *address/*, bool tentative*/)
{
    return NULL;
}

ipv6_neighbour_t *ipv6_neighbour_lookup_or_create_by_interface_id(int8_t interface_id, const uint8_t *address/*, bool tentative*/)
{
    return NULL;
}

ipv6_neighbour_t *ipv6_neighbour_used(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
    return NULL;
}

bool ipv6_neighbour_is_probably_reachable(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *n)
{
    return false;
}

bool ipv6_neighbour_addr_is_probably_reachable(ipv6_neighbour_cache_t *cache, const uint8_t *address)
{
    return false;
}

bool ipv6_neighbour_ll_addr_match(const ipv6_neighbour_t *entry, addrtype_t ll_type, const uint8_t *ll_address)
{
    return false;
}

void ipv6_neighbour_invalidate_ll_addr(ipv6_neighbour_cache_t *cache, addrtype_t ll_type, const uint8_t *ll_address)
{
}

void ipv6_neighbour_set_state(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, ip_neighbour_cache_state_t state)
{
}

/* Called when LL address information is received other than in an NA (NS source, RS source, RA source, Redirect target) */
void ipv6_neighbour_entry_update_unsolicited(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, addrtype_t type, const uint8_t *ll_address/*, bool tentative*/)
{
}

ipv6_neighbour_t *ipv6_neighbour_update_unsolicited(ipv6_neighbour_cache_t *cache, const uint8_t *ip_address, addrtype_t type, const uint8_t *ll_address/*, bool tentative*/)
{
    return NULL;
}

void ipv6_neighbour_update_from_na(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, uint8_t flags, addrtype_t ll_type, const uint8_t *ll_address)
{
}

void ipv6_neighbour_reachability_confirmation(const uint8_t ip_address[__static 16], int8_t interface_id)
{
}

void ipv6_neighbour_cache_print(const ipv6_neighbour_cache_t *cache, route_print_fn_t *print_fn)
{
}



void ipv6_neighbour_cache_slow_timer(ipv6_neighbour_cache_t *cache, uint8_t seconds)
{
}

void ipv6_neighbour_cache_fast_timer(ipv6_neighbour_cache_t *cache, uint16_t ticks)
{
}

void ipv6_destination_cache_print(route_print_fn_t *print_fn)
{
}

ipv6_destination_t *ipv6_destination_lookup_or_create(const uint8_t *address, int8_t interface_id)
{
    return NULL;
}

void ipv6_destination_redirect(const uint8_t *dest_addr, const uint8_t *sender_addr, const uint8_t *redirect_addr, int8_t interface_id, addrtype_t ll_type, const uint8_t *ll_address)
{
}

void ipv6_route_table_set_predicate_fn(ipv6_route_src_t src, ipv6_route_predicate_fn_t fn)
{
}

void ipv6_route_table_set_next_hop_fn(ipv6_route_src_t src, ipv6_route_next_hop_fn_t fn)
{
}

void ipv6_destination_release(ipv6_destination_t *dest)
{
}

void ipv6_destination_cache_timer(uint8_t seconds)
{
}

void ipv6_route_table_print(route_print_fn_t *print_fn)
{
}

ipv6_route_t *ipv6_route_choose_next_hop(const uint8_t *dest, int8_t interface_id, ipv6_route_predicate_fn_t *predicate)
{
    return NULL;
}

ipv6_route_t *ipv6_route_lookup_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, int_fast16_t src_id)
{
    return NULL;
}

uint8_t ipv6_route_pref_to_metric(int_fast8_t pref)
{
    return 0;
}

ipv6_route_t *ipv6_route_add(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, uint32_t lifetime, int_fast8_t pref)
{
    return NULL;
}

ipv6_route_t *ipv6_route_add_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, uint8_t source_id, uint32_t lifetime, int_fast8_t pref)
{
    return NULL;
}

ipv6_route_t *ipv6_route_add_metric(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, uint8_t source_id, uint32_t lifetime, uint8_t metric)
{
    return NULL;
}

int_fast8_t ipv6_route_delete(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source)
{
    return -1;
}

int_fast8_t ipv6_route_delete_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, int_fast16_t source_id)
{
    return -1;
}

void ipv6_route_table_remove_interface(int8_t interface_id)
{
}

void ipv6_route_table_modify_router_metric(int8_t interface_id, const uint8_t *addr, ipv6_route_src_t source, uint8_t keep, uint8_t toggle)
{
}

void ipv6_route_table_remove_info(int8_t interface_id, ipv6_route_src_t source, void *info)
{
}

void ipv6_route_table_ttl_update(uint16_t seconds)
{
}

void ipv6_neighbour_reachability_problem(const uint8_t ip_address[__static 16], int8_t interface_id)
{
}

void ipv6_route_table_set_max_entries(int8_t interface_id, ipv6_route_src_t source, uint8_t max_entries)
{
}

bool ipv6_route_table_source_was_invalidated(ipv6_route_src_t src)
{
    return false;
}

// Called when event queue is empty - no pending buffers so can clear invalidation flags.
void ipv6_route_table_source_invalidated_reset(void)
{

}
