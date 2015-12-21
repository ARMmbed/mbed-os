/*
 * Copyright (c) 2012-2015 ARM Limited. All rights reserved.
 */
/*
 * ipv6_routing_table.c
 *
 *  Implements IPv6 Neighbour Cache (RFC 4861), Destination Cache (RFC 4861),
 *  and Routing Table (RFC 4191, incorporating the RFC 4861 Prefix List)
 */
#include "config.h"
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
#include "nsdynmemLIB.h"

#ifndef HAVE_DEBUG
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

void ipv6_neighbour_cache_print(const ipv6_neighbour_cache_t *cache, uint8_t dlevel)
{
}



void ipv6_neighbour_cache_slow_timer(ipv6_neighbour_cache_t *cache, uint8_t seconds)
{
}

void ipv6_neighbour_cache_fast_timer(ipv6_neighbour_cache_t *cache, uint16_t ticks)
{
}

void ipv6_destination_cache_print(uint8_t dlevel)
{
}

ipv6_destination_t *ipv6_destination_lookup_or_create(const uint8_t *address, int8_t interface_id)
{
    return NULL;
}

void ipv6_destination_redirect(const uint8_t *dest_addr, const uint8_t *sender_addr, const uint8_t *redirect_addr, int8_t interface_id, addrtype_t ll_type, const uint8_t *ll_address)
{
}

void ipv6_destination_release(ipv6_destination_t *dest)
{
}

void ipv6_destination_cache_timer(uint8_t seconds)
{
}

void ipv6_route_table_print(uint8_t dlevel)
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

ipv6_route_t *ipv6_route_add(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, uint32_t lifetime, int_fast8_t pref)
{
    return NULL;
}

ipv6_route_t *ipv6_route_add_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, uint8_t source_id, uint32_t lifetime, int_fast8_t pref)
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

void ipv6_route_table_remove_info(int8_t interface_id, ipv6_route_src_t source, void *info)
{
}

void ipv6_route_table_ttl_update(uint16_t seconds)
{
}
