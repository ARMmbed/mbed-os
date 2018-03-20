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
 *
 * Note that RFC 4861 dictates that the Prefix List is checked first,
 * followed by the Default Router List. In simple host scenarios, the
 * longest-match routing table look-up achieves that, because on-link entries
 * from the Prefix List are longer than the ::/0 default routes.
 *
 * In more complex scenarios, we can have more-specific routes preferred over
 * more general on-link prefixes, eg the border router preferring a /128 RPL
 * DAO-SR route instead of the /64 on-link prefix for the Ethernet backbone.
 *
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "common_functions.h"
#include "ip6string.h"
#include "randLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "Core/include/address.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "nsdynmemLIB.h"
#include "Service_Libs/etx/etx.h"
#include "Common_Protocols/ipv6_resolution.h"
#include <stdarg.h>
#include <stdio.h>

#define TRACE_GROUP "rout"

#define NCACHE_GC_PERIOD    20  /* seconds */
#define DCACHE_GC_PERIOD    20  /* seconds */

static uint16_t current_max_cache = 64;

/* We track "lifetime" of garbage-collectible entries, resetting
 * when used. Entries with lifetime 0 are favoured
 * for garbage-collection. */
#define NCACHE_GC_AGE 600   /* 10 minutes (1s units - decremented every slow timer call) */
#define DCACHE_GC_AGE 30    /* 10 minutes (20s units - decremented once per periodic GC) */
#define DCACHE_GC_AGE_LL 6  /* 2 minutes for link-local destinations */

/* For probable routers, consider them unreachable if ETX is greater than this */
#define ETX_REACHABILITY_THRESHOLD 0x200    /* 8.8 fixed-point, so 2 */

static NS_LIST_DEFINE(ipv6_destination_cache, ipv6_destination_t, link);
static NS_LIST_DEFINE(ipv6_routing_table, ipv6_route_t, link);

static ipv6_destination_t *ipv6_destination_lookup(const uint8_t *address, int8_t interface_id);
static void ipv6_destination_cache_forget_router(ipv6_neighbour_cache_t *cache, const uint8_t neighbour_addr[16]);
static void ipv6_destination_cache_forget_neighbour(const ipv6_neighbour_t *neighbour);
static void ipv6_destination_release(ipv6_destination_t *dest);
static void ipv6_route_table_remove_router(int8_t interface_id, const uint8_t *addr, ipv6_route_src_t source);
static uint16_t total_metric(const ipv6_route_t *route);
static void trace_debug_print(const char *fmt, ...);
static uint8_t ipv6_route_table_count_source(int8_t interface_id, ipv6_route_src_t source);
static void ipv6_route_table_remove_last_one_from_source(int8_t interface_id, ipv6_route_src_t source);
static uint8_t ipv6_route_table_get_max_entries(int8_t interface_id, ipv6_route_src_t source);

static uint16_t dcache_gc_timer;

static uint16_t cache_long_term(bool is_destination)
{
    uint16_t value = current_max_cache/8;
    if (is_destination) {
        value*=2;
    }
    if (value < 4) {
        value = 4;
    }
    return value;
}

static uint16_t cache_short_term(bool is_destination)
{
    uint16_t value = current_max_cache/2;
    if (value < cache_long_term(is_destination)) {
        return cache_long_term(is_destination);
    }
    return value;
}

static uint32_t next_probe_time(ipv6_neighbour_cache_t *cache, uint_fast8_t retrans_num)
{
    uint32_t t = cache->retrans_timer;

    while (retrans_num--) {
        t *= BACKOFF_MULTIPLE;
        if (t > MAX_RETRANS_TIMER) {
            t = MAX_RETRANS_TIMER;
            break;
        }
    }

    return randLIB_randomise_base(t, 0x4000, 0xBFFF);
}

int8_t ipv6_neighbour_set_current_max_cache(uint16_t max_cache)
{
    if (max_cache < 4) {
        return -1;
    }
    current_max_cache = max_cache;
    return 0;
}

/* Called when we determine a neighbour is no longer a router */
void ipv6_router_gone(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
    tr_debug("Router gone: %s", trace_ipv6(entry->ip_address));
    entry->is_router = false;
    /* Only delete RA routes to satisfy RFC 4861. We should have a callback to
     * other route providers here - eg RPL might want to know, and delete from
     * the Candidate Neighbour set. But unfortunately our 6LoWPAN-ND routers do
     * currently send RS packets while running, which means this would break
     * stuff. We get spurious switches of IsRouter to false :(
     */
    ipv6_route_table_remove_router(cache->interface_id, entry->ip_address, ROUTE_RADV);
    /* The above will re-evaluate all destinations affected by the routing
     * change; the below is needed to also forget redirections to the router.
     */
    ipv6_destination_cache_forget_router(cache, entry->ip_address);
}

/* Called when a neighbour has apparently become reachable */
static void ipv6_neighbour_appeared(ipv6_neighbour_cache_t *cache, uint8_t address[static 16])
{
    (void)cache;
    (void)address;
}

/* Called when a neighbour has apparently become unreachable */
static void ipv6_neighbour_gone(ipv6_neighbour_cache_t *cache, uint8_t address[static 16])
{
    (void) cache;
    tr_debug("Lost contact with neighbour: %s", trace_ipv6(address));
    // We can keep trying to talk directly to that neighbour, but should
    // avoid using it any more as a router, if there's an alternative.
    ipv6_destination_cache_forget_router(cache, address);
}

void ipv6_neighbour_cache_init(ipv6_neighbour_cache_t *cache, int8_t interface_id)
{
    /* Init Double linked Routing Table */
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        ipv6_neighbour_entry_remove(cache, cur);
    }
    cache->gc_timer = NCACHE_GC_PERIOD;
    cache->retrans_timer = 1000;
    cache->max_ll_len = 0;
    cache->interface_id = interface_id;
    cache->recv_addr_reg = false;
    cache->send_addr_reg = false;
    cache->send_nud_probes = true;
    cache->recv_na_aro = false;
    cache->recv_ns_aro = false;
    cache->route_if_info.metric = 0;
    memset(cache->route_if_info.sources, 0, sizeof(cache->route_if_info.sources));
}

void ipv6_neighbour_cache_flush(ipv6_neighbour_cache_t *cache)
{
    /* Flush non-registered entries only */
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        if (cur->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
            ipv6_neighbour_entry_remove(cache, cur);
        }
    }
}


ipv6_neighbour_t *ipv6_neighbour_lookup(ipv6_neighbour_cache_t *cache, const uint8_t *address)
{
    ns_list_foreach(ipv6_neighbour_t, cur, &cache->list) {
        if (addr_ipv6_equal(cur->ip_address, address)) {
            return cur;
        }
    }

    return NULL;
}

ipv6_neighbour_t *ipv6_neighbour_lookup_by_interface_id(int8_t interface_id, const uint8_t *address)
{
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(interface_id);
    if (!ncache) {
        return NULL;
    }

    return ipv6_neighbour_lookup(ncache, address);
}


void ipv6_neighbour_entry_remove(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
    /* Remove entry from cache first - avoids weird garbage collection issues, like
     * it being pushed out while generating ICMP errors, or ICMP errors actually using
     * the entry.
     */
    ns_list_remove(&cache->list, entry);
    switch (entry->state) {
        case IP_NEIGHBOUR_NEW:
            break;
        case IP_NEIGHBOUR_INCOMPLETE:
            /* If the NCE is discarded, the queued packet must also be discarded */
            /* Handle this here to avoid leakage in the event that the NCE is */
            /* dropped by garbage collection rather than the expected timeout */
            ipv6_interface_resolution_failed(cache, entry);
            break;
        case IP_NEIGHBOUR_STALE:
        case IP_NEIGHBOUR_REACHABLE:
        case IP_NEIGHBOUR_DELAY:
        case IP_NEIGHBOUR_PROBE:
        case IP_NEIGHBOUR_UNREACHABLE:
            /* Destination cache no longer has direct pointers to neighbour cache,
             * so a NCE being deleted no longer necessarily needs any special
             * action. Neighbour GC needn't affect the Dest Cache.
             */
            // ipv6_neighbour_gone(cache, entry);
            break;
    }
    ipv6_destination_cache_forget_neighbour(entry);
    ns_dyn_mem_free(entry);
}

ipv6_neighbour_t *ipv6_neighbour_lookup_or_create(ipv6_neighbour_cache_t *cache, const uint8_t *address/*, bool tentative*/)
{
    uint_fast16_t count = 0;
    ipv6_neighbour_t *entry = NULL;

    ns_list_foreach(ipv6_neighbour_t, cur, &cache->list) {
        count++;
        if (addr_ipv6_equal(cur->ip_address, address)) {
            if (cur != ns_list_get_first(&cache->list)) {
                ns_list_remove(&cache->list, cur);
                ns_list_add_to_start(&cache->list, cur);
            }
            return cur;
        }
    }

    if (count >= current_max_cache) {
        entry = ns_list_get_last(&cache->list);
        ipv6_neighbour_entry_remove(cache, entry);
    }

    // Allocate new - note we have a basic size, plus enough for the LL address,
    // plus another 8 for the EUI-64 of registration (RFC 6775). Note that in
    // the protocols, the link-layer address and EUI-64 are distinct. The
    // neighbour may be using a short link-layer address, not its EUI-64.
    entry = ns_dyn_mem_alloc(sizeof(ipv6_neighbour_t) + cache->max_ll_len + (cache->recv_addr_reg ? 8 : 0));
    if (!entry) {
        tr_warn("No mem!");
        return NULL;
    }

    memcpy(entry->ip_address, address, 16);
    entry->is_router = false;
    entry->from_redirect = false;
    entry->state = IP_NEIGHBOUR_NEW;
    /*if (tentative && cache->reg_required)
        entry->type = IP_NEIGHBOUR_TENTATIVE;
    else*/
    entry->type = IP_NEIGHBOUR_GARBAGE_COLLECTIBLE;
    ns_list_init(&entry->queue);
    entry->timer = 0;
    entry->lifetime = 0;
    entry->retrans_count = 0;
    entry->ll_type = ADDR_NONE;
    if (cache->recv_addr_reg) {
        memset(ipv6_neighbour_eui64(cache, entry), 0, 8);
    }

    ns_list_add_to_start(&cache->list, entry);

    return entry;
}

ipv6_neighbour_t *ipv6_neighbour_lookup_or_create_by_interface_id(int8_t interface_id, const uint8_t *address/*, bool tentative*/)
{
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(interface_id);
    if (!ncache) {
        return NULL;
    }

    return ipv6_neighbour_lookup_or_create(ncache, address/*, tentative*/);
}

ipv6_neighbour_t *ipv6_neighbour_used(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
    /* Reset the GC life, if it's a GC entry */
    if (entry->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
        entry->lifetime = NCACHE_GC_AGE;
    }

    /* Move it to the front of the list */
    if (entry != ns_list_get_first(&cache->list)) {
        ns_list_remove(&cache->list, entry);
        ns_list_add_to_start(&cache->list, entry);
    }

    /* If the entry is stale, prepare delay timer for active NUD probe */
    if (entry->state == IP_NEIGHBOUR_STALE && cache->send_nud_probes) {
        ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_DELAY);
    }

    /* Special case for Registered Unreachable entries - restart the probe timer if stopped */
    else if (entry->state == IP_NEIGHBOUR_UNREACHABLE && entry->timer == 0) {
        entry->timer = next_probe_time(cache, entry->retrans_count);
    }

    return entry;
}

static bool ipv6_neighbour_state_is_probably_reachable(ip_neighbour_cache_state_t state)
{
    switch (state) {
        case IP_NEIGHBOUR_NEW:
        case IP_NEIGHBOUR_INCOMPLETE:
        case IP_NEIGHBOUR_UNREACHABLE:
            return false;
        case IP_NEIGHBOUR_REACHABLE:
        case IP_NEIGHBOUR_STALE:
        case IP_NEIGHBOUR_DELAY:
        case IP_NEIGHBOUR_PROBE:
            return true;
    }
    return false;
}

bool ipv6_neighbour_is_probably_reachable(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *n)
{
    if (!n) {
        return false;
    }
    if (!ipv6_neighbour_state_is_probably_reachable(n->state)) {
        return false;
    }
    uint16_t etx = ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, cache->interface_id, n, n->ip_address, etx_read);
    if (etx > ETX_REACHABILITY_THRESHOLD) {
        /* "Unknown" is signalled as low values, so will be return "true" */
        return false;
    }
    return true;
}

bool ipv6_neighbour_addr_is_probably_reachable(ipv6_neighbour_cache_t *cache, const uint8_t *address)
{
    return ipv6_neighbour_is_probably_reachable(cache, ipv6_neighbour_lookup(cache, address));
}

bool ipv6_neighbour_ll_addr_match(const ipv6_neighbour_t *entry, addrtype_t ll_type, const uint8_t *ll_address)
{
    return ll_type == entry->ll_type && memcmp(entry->ll_address, ll_address, addr_len_from_type(ll_type)) == 0;
}

static bool ipv6_neighbour_update_ll(ipv6_neighbour_t *entry, addrtype_t ll_type, const uint8_t *ll_address)
{
    uint8_t ll_len = addr_len_from_type(ll_type);

    /* Any new address info clears the "redirected" flag - redirect itself
     * sets it again after this is called.
     */
    entry->from_redirect = false;

    if (ll_type != entry->ll_type || memcmp(entry->ll_address, ll_address, ll_len)) {
        entry->ll_type = ll_type;
        memcpy(entry->ll_address, ll_address, ll_len);
        return true;
    }
    return false;
}

void ipv6_neighbour_invalidate_ll_addr(ipv6_neighbour_cache_t *cache, addrtype_t ll_type, const uint8_t *ll_address)
{
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        if (cur->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE && ipv6_neighbour_ll_addr_match(cur, ll_type, ll_address)) {
            ipv6_neighbour_entry_remove(cache, cur);
        }
    }
}

void ipv6_neighbour_delete_registered_by_eui64(ipv6_neighbour_cache_t *cache, const uint8_t *eui64)
{
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        if (cur->type != IP_NEIGHBOUR_GARBAGE_COLLECTIBLE && memcmp(ipv6_neighbour_eui64(cache, cur), eui64, 8) == 0) {
            ipv6_neighbour_entry_remove(cache, cur);
        }
    }
}

void ipv6_neighbour_set_state(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, ip_neighbour_cache_state_t state)
{
    if (!ipv6_neighbour_state_is_probably_reachable(entry->state) &&
            ipv6_neighbour_state_is_probably_reachable(state)) {
        /* A neighbour is becoming reachable - may affect destination cache */
        ipv6_neighbour_appeared(cache, entry->ip_address);
    }
    switch (state) {
        case IP_NEIGHBOUR_INCOMPLETE:
            entry->retrans_count = 0;
            entry->timer = cache->retrans_timer;
            break;
        case IP_NEIGHBOUR_STALE:
            entry->timer = 0;
            break;
        case IP_NEIGHBOUR_DELAY:
            entry->timer = DELAY_FIRST_PROBE_TIME;
            break;
        case IP_NEIGHBOUR_PROBE:
            entry->retrans_count = 0;
            entry->timer = next_probe_time(cache, 0);
            break;
        case IP_NEIGHBOUR_REACHABLE:
            entry->timer = cache->reachable_time;
            break;
        case IP_NEIGHBOUR_UNREACHABLE:
            /* Progress to this from PROBE - timers continue */
            ipv6_neighbour_gone(cache, entry->ip_address);
            break;
        default:
            entry->timer = 0;
            break;
    }
    entry->state = state;
}

/* Called when LL address information is received other than in an NA (NS source, RS source, RA source, Redirect target) */
void ipv6_neighbour_entry_update_unsolicited(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, addrtype_t type, const uint8_t *ll_address/*, bool tentative*/)
{
    bool modified_ll = ipv6_neighbour_update_ll(entry, type, ll_address);

    switch (entry->state) {
        case IP_NEIGHBOUR_NEW:
            ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
            break;
        case IP_NEIGHBOUR_INCOMPLETE:
            ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
            ipv6_send_queued(entry);
            break;
        default:
            if (modified_ll) {
                ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
            }
            break;
    }
}

ipv6_neighbour_t *ipv6_neighbour_update_unsolicited(ipv6_neighbour_cache_t *cache, const uint8_t *ip_address, addrtype_t type, const uint8_t *ll_address/*, bool tentative*/)
{
    ipv6_neighbour_t *entry = ipv6_neighbour_lookup_or_create(cache, ip_address/*, tentative*/);
    if (!entry) {
        return NULL;
    }

    ipv6_neighbour_entry_update_unsolicited(cache, entry, type, ll_address/*, tentative*/);

    return entry;
}

void ipv6_neighbour_update_from_na(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, uint8_t flags, addrtype_t ll_type, const uint8_t *ll_address)
{
    if (entry->state == IP_NEIGHBOUR_NEW || entry->state == IP_NEIGHBOUR_INCOMPLETE) {
        entry->is_router = flags & NA_R;
        if (ll_type == ADDR_NONE) {
            return;
        }

        ipv6_neighbour_update_ll(entry, ll_type, ll_address);
        if (flags & NA_S) {
            ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_REACHABLE);
        } else {
            ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
        }
        ipv6_send_queued(entry);
        return;
    }

    /* Already have a complete entry with known LL address */
    bool ll_addr_differs = ll_type != ADDR_NONE && !ipv6_neighbour_ll_addr_match(entry, ll_type, ll_address);

    if (ll_addr_differs) {
        if (flags & NA_O) {
            entry->ll_type = ll_type;
            memcpy(entry->ll_address, ll_address, addr_len_from_type(ll_type));
        } else {
            if (entry->state == IP_NEIGHBOUR_REACHABLE) {
                ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
            }
            return;
        }
    }

    if (flags & NA_S) {
        ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_REACHABLE);
    } else if (ll_addr_differs) {
        ipv6_neighbour_set_state(cache, entry, IP_NEIGHBOUR_STALE);
    }

    if (entry->is_router && !(flags & NA_R)) {
        ipv6_router_gone(cache, entry);
    }

    entry->is_router = flags & NA_R;
}

void ipv6_neighbour_reachability_confirmation(const uint8_t ip_address[static 16], int8_t interface_id)
{
    /* No point creating an entry if doesn't exist */
    ipv6_destination_t *dest = ipv6_destination_lookup(ip_address, interface_id);
    if (!dest) {
        return;
    }

    /* We can't be absolutely certain which next hop is working, but last_neighbour is our best guess */
    ipv6_neighbour_t *next_hop = dest->last_neighbour;
#if 0
    if (next_hop) {
        tr_debug("%s rconf: mark %s reachable", trace_ipv6(ip_address), trace_ipv6(next_hop->ip_address));
    } else {
        tr_debug("%s rconf: next hop unknown", trace_ipv6(ip_address));
    }
#endif
    if (!next_hop) {
        return;
    }

    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(dest->interface_id);
    if (!ncache) {
        return;
    }

    if (next_hop->state != IP_NEIGHBOUR_NEW && next_hop->state != IP_NEIGHBOUR_INCOMPLETE) {
        ipv6_neighbour_set_state(ncache, next_hop, IP_NEIGHBOUR_REACHABLE);
    }
}

/* RFC 4861 doesn't have this, but would seem sensible to at least nudge it out of REACHABLE state.
 * This doesn't add a new state machine transition, we just cut short the timer.
 * This should normally be called /before/ initiating a retransmit, so the
 * retransmit then triggers an immediate transition into DELAY state.
 */
void ipv6_neighbour_reachability_problem(const uint8_t ip_address[static 16], int8_t interface_id)
{
    /* No point creating an entry if doesn't exist */
    ipv6_destination_t *dest = ipv6_destination_lookup(ip_address, interface_id);
    if (!dest) {
        return;
    }

    /* We can't be absolutely certain which next hop has problems, but last_neighbour is our best guess */
    ipv6_neighbour_t *next_hop = dest->last_neighbour;
    if (!next_hop) {
        return;
    }

    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(dest->interface_id);
    if (!ncache) {
        return;
    }

    if (next_hop->state == IP_NEIGHBOUR_REACHABLE) {
        ipv6_neighbour_set_state(ncache, next_hop, IP_NEIGHBOUR_STALE);
    }
}

static const char *state_names[] = {
    [IP_NEIGHBOUR_NEW]          = "NEW",
    [IP_NEIGHBOUR_INCOMPLETE]   = "INCOMPLETE",
    [IP_NEIGHBOUR_STALE]        = "STALE",
    [IP_NEIGHBOUR_REACHABLE]    = "REACHABLE",
    [IP_NEIGHBOUR_DELAY]        = "DELAY",
    [IP_NEIGHBOUR_PROBE]        = "PROBE",
    [IP_NEIGHBOUR_UNREACHABLE]  = "UNREACHABLE",
};

static const char *type_names[] = {
    [IP_NEIGHBOUR_GARBAGE_COLLECTIBLE]  = "GC",
    [IP_NEIGHBOUR_REGISTERED]           = "REGISTERED",
    [IP_NEIGHBOUR_TENTATIVE]            = "TENTATIVE",
};

static void sprint_array(char *s, const uint8_t *ptr, uint_fast8_t len)
{
    if (len == 0) {
        *s = '\0';
        return;
    }

    for (uint_fast8_t i = 0; i < len; i++) {
        s += sprintf(s, "%02x:", *ptr++);
    }
    // Replace last ':' with '\0'
    *(s - 1) = '\0';
}

void ipv6_neighbour_cache_print(const ipv6_neighbour_cache_t *cache, route_print_fn_t *print_fn)
{
    print_fn("Neighbour Cache %d", cache->interface_id);
    print_fn("Reachable Time: %"PRIu32"   Retrans Timer: %"PRIu32"   MTU: %"PRIu16"", cache->reachable_time, cache->retrans_timer, cache->link_mtu);
    ns_list_foreach(const ipv6_neighbour_t, cur, &cache->list) {
        ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str);
        print_fn("%sIP Addr: %s", cur->is_router ? "Router " : "", ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, cur->ip_address));
        // Reusing addr_str for the array prints as it's no longer needed and 41 bytes is more than enough.
        sprint_array(addr_str, cur->ll_address, addr_len_from_type(cur->ll_type));
        print_fn("LL Addr: (%s %"PRIu32") %s", state_names[cur->state], cur->timer, addr_str);
        if (cache->recv_addr_reg && memcmp(ipv6_neighbour_eui64(cache, cur), ADDR_EUI64_ZERO, 8)) {
            sprint_array( addr_str, ipv6_neighbour_eui64(cache, cur), 8);
            print_fn("EUI-64:  (%s %"PRIu32") %s", type_names[cur->type], cur->lifetime, addr_str);
        } else if (cur->type != IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
            print_fn("         (%s %"PRIu32") [no EUI-64]", type_names[cur->type], cur->lifetime);
        }
    }
}

static void ipv6_neighbour_cache_gc_periodic(ipv6_neighbour_cache_t *cache)
{
    uint_fast16_t gc_count = 0;
    ns_list_foreach_safe(ipv6_neighbour_t, entry, &cache->list) {
        if (entry->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
            gc_count++;
        }
    }

    if (gc_count <= cache_long_term(false)) {
        return;
    }

    /* Removal strategy - to stay below MAX_SHORT_TERM, we will chuck any STALE entries */
    /* To stay below MAX_LONG_TERM, we will chuck old STALE entries */
    ns_list_foreach_reverse_safe(ipv6_neighbour_t, entry, &cache->list) {
        /* Expiration of non-GC entries handled in slow timer routine */
        if (entry->type != IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
            continue;
        }

        if (entry->state != IP_NEIGHBOUR_STALE && entry->state != IP_NEIGHBOUR_UNREACHABLE) {
            continue;
        }

        if (entry->lifetime == 0 || gc_count > cache_short_term(false)) {
            ipv6_neighbour_entry_remove(cache, entry);
            if (--gc_count <= cache_long_term(false)) {
                break;
            }
        }
    }
}

void ipv6_neighbour_cache_slow_timer(ipv6_neighbour_cache_t *cache, uint8_t seconds)
{
    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        if (cur->lifetime == 0 || cur->lifetime == 0xffffffff) {
            continue;
        }

        if (cur->lifetime > seconds) {
            cur->lifetime -= seconds;
            continue;
        }

        cur->lifetime = 0;

        /* Lifetime expired */
        switch (cur->type) {
            case IP_NEIGHBOUR_GARBAGE_COLLECTIBLE:
                /* No immediate action, but 0 lifetime is an input to the GC */
                break;

            case IP_NEIGHBOUR_TENTATIVE:
            case IP_NEIGHBOUR_REGISTERED:
                /* These are deleted as soon as lifetime expires */
                ipv6_neighbour_gone(cache, cur->ip_address);
                ipv6_neighbour_entry_remove(cache, cur);
                break;
        }
    }

    if (cache->gc_timer > seconds) {
        cache->gc_timer -= seconds;
        return;
    }

    cache->gc_timer = NCACHE_GC_PERIOD;
    //ipv6_neighbour_cache_print(cache);
    ipv6_neighbour_cache_gc_periodic(cache);
}

void ipv6_neighbour_cache_fast_timer(ipv6_neighbour_cache_t *cache, uint16_t ticks)
{
    uint32_t ms = (uint32_t) ticks * 100;

    ns_list_foreach_safe(ipv6_neighbour_t, cur, &cache->list) {
        if (cur->timer == 0) {
            continue;
        }

        if (cur->timer > ms) {
            cur->timer -= ms;
            continue;
        }

        cur->timer = 0;

        /* Timer expired */
        switch (cur->state) {
            case IP_NEIGHBOUR_NEW:
                /* Shouldn't happen */
                break;
            case IP_NEIGHBOUR_INCOMPLETE:
                if (++cur->retrans_count >= MAX_MULTICAST_SOLICIT) {
                    /* Should be safe for registration - Tentative/Registered entries can't be INCOMPLETE */
                    ipv6_neighbour_gone(cache, cur->ip_address);
                    ipv6_neighbour_entry_remove(cache, cur);
                } else {
                    ipv6_interface_resolve_send_ns(cache, cur, false, cur->retrans_count);
                    cur->timer = cache->retrans_timer;
                }
                break;
            case IP_NEIGHBOUR_STALE:
                /* Shouldn't happen */
                break;
            case IP_NEIGHBOUR_REACHABLE:
                ipv6_neighbour_set_state(cache, cur, IP_NEIGHBOUR_STALE);
                break;
            case IP_NEIGHBOUR_DELAY:
                ipv6_neighbour_set_state(cache, cur, IP_NEIGHBOUR_PROBE);
                ipv6_interface_resolve_send_ns(cache, cur, true, 0);
                break;
            case IP_NEIGHBOUR_PROBE:
                if (cur->retrans_count >= MARK_UNREACHABLE - 1) {
                    if (cur->from_redirect) {
                        ipv6_neighbour_gone(cache, cur->ip_address);
                        ipv6_neighbour_entry_remove(cache, cur);
                        break;
                    } else {
                        ipv6_neighbour_set_state(cache, cur, IP_NEIGHBOUR_UNREACHABLE);
                    }
                }
            /* no break */
            case IP_NEIGHBOUR_UNREACHABLE:
                if (cur->retrans_count < 0xFF) {
                    cur->retrans_count++;
                }

                if (cur->retrans_count >= MAX_UNICAST_SOLICIT && cur->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
                    ipv6_neighbour_entry_remove(cache, cur);
                } else {
                    ipv6_interface_resolve_send_ns(cache, cur, true, cur->retrans_count);
                    if (cur->retrans_count >= MAX_UNICAST_SOLICIT - 1) {
                        /* "Final" unicast probe */
                        if (cur->type == IP_NEIGHBOUR_GARBAGE_COLLECTIBLE) {
                            /* Only wait 1 initial retrans time for response to final probe - don't want backoff in this case */
                            cur->timer = cache->retrans_timer;
                        } else {
                            /* We're not going to remove this. Let's stop the timer. We'll restart to probe once more if it's used */
                            cur->timer = 0;
                        }
                    } else {
                        /* Backoff for the next probe */
                        cur->timer = next_probe_time(cache, cur->retrans_count);
                    }
                }
                break;
        }
    }
}

void ipv6_destination_cache_print(route_print_fn_t *print_fn)
{
    print_fn("Destination Cache:");
    ns_list_foreach(ipv6_destination_t, entry, &ipv6_destination_cache) {
        ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str);
        print_fn(" %s (life %u)", ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, entry->destination), entry->lifetime);
#ifdef HAVE_IPV6_ND
        if (entry->redirected) {
            print_fn("     Redirect %s%%%u", ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, entry->redirect_addr), entry->interface_id);
        }
#endif
#ifndef NO_IPV6_PMTUD
        print_fn("     PMTU %u (life %u)", entry->pmtu, entry->pmtu_lifetime);
#endif
    }
}

static ipv6_destination_t *ipv6_destination_lookup(const uint8_t *address, int8_t interface_id)
{
    bool is_ll = addr_is_ipv6_link_local(address);

    if (is_ll && interface_id == -1) {
        return NULL;
    }

    ns_list_foreach(ipv6_destination_t, cur, &ipv6_destination_cache) {
        if (!addr_ipv6_equal(cur->destination, address)) {
            continue;
        }
        /* For LL addresses, interface ID must also be compared */
        if (is_ll && cur->interface_id != interface_id) {
            continue;
        }

        return cur;
    }

    return NULL;
}

/* Unlike original version, this does NOT perform routing check - it's pure destination cache look-up
 *
 * We no longer attempt to cache route lookups in the destination cache, as
 * assumption that routing look-ups are keyed purely by destination is no longer
 * true. If necessary, a caching layer could be placed into
 * ipv6_route_choose_next_hop.
 *
 * Interface IDs are a little tricky here. Current situation is that we
 * require an interface ID for <=realm-local addresses, and it's ignored for
 * other addresses. That prevents us having multiple Destination Cache entries
 * for one global address.
 */
ipv6_destination_t *ipv6_destination_lookup_or_create(const uint8_t *address, int8_t interface_id)
{
    uint_fast16_t count = 0;
    ipv6_destination_t *entry = NULL;
    bool interface_specific = addr_ipv6_scope(address, NULL) <= IPV6_SCOPE_REALM_LOCAL;

    if (interface_specific && interface_id == -1) {
        return NULL;
    }

    /* Find any existing entry */
    ns_list_foreach(ipv6_destination_t, cur, &ipv6_destination_cache) {
        count++;
        if (!addr_ipv6_equal(cur->destination, address)) {
            continue;
        }
        /* For LL addresses, interface ID must also be compared */
        if (interface_specific && cur->interface_id != interface_id) {
            continue;
        }

        entry = cur;
        break;
    }


    if (!entry) {
        if (count > current_max_cache) {
            entry = ns_list_get_last(&ipv6_destination_cache);
            ns_list_remove(&ipv6_destination_cache, entry);
            ipv6_destination_release(entry);
        }

        /* If no entry, make one */
        entry = ns_dyn_mem_alloc(sizeof(ipv6_destination_t));
        if (!entry) {
            return NULL;
        }
        memcpy(entry->destination, address, 16);
        entry->refcount = 1;
#ifdef HAVE_IPV6_ND
        entry->redirected = false;
#endif
        entry->last_neighbour = NULL;
#ifndef NO_IPV6_PMTUD
        entry->pmtu = 0xffff;
        entry->pmtu_lifetime = 0;
#endif
#ifndef NO_IP_FRAGMENT_TX
        entry->fragment_id = randLIB_get_32bit();
#endif
        if (interface_specific) {
            entry->interface_id = interface_id;
        } else {
            entry->interface_id = -1;
        }
        ns_list_add_to_start(&ipv6_destination_cache, entry);
    } else if (entry != ns_list_get_first(&ipv6_destination_cache)) {
        /* If there was an entry, and it wasn't at the start, move it */
        ns_list_remove(&ipv6_destination_cache, entry);
        ns_list_add_to_start(&ipv6_destination_cache, entry);
    }

    if (addr_ipv6_scope(address, NULL) <= IPV6_SCOPE_LINK_LOCAL) {
        entry->lifetime = DCACHE_GC_AGE_LL;
    } else {
        entry->lifetime = DCACHE_GC_AGE;
    }

    return entry;
}


/* Force re-evaluation of next hop for all entries using the specified next hop as
 * a router. Will keep using it for direct comms.
 */
static void ipv6_destination_cache_forget_router(ipv6_neighbour_cache_t *ncache, const uint8_t neighbour_addr[static 16])
{
    ipv6_neighbour_t *neighbour = ipv6_neighbour_lookup(ncache, neighbour_addr);

    ns_list_foreach(ipv6_destination_t, entry, &ipv6_destination_cache) {
        if (entry->last_neighbour && entry->interface_id == ncache->interface_id && entry->last_neighbour == neighbour) {
            entry->last_neighbour = NULL;
        }
#ifdef HAVE_IPV6_ND
        if (entry->redirected && entry->interface_id == ncache->interface_id && addr_ipv6_equal(entry->redirect_addr, neighbour_addr)) {
            entry->redirected = false;
        }
#endif
    }
}

static void ipv6_destination_cache_forget_neighbour(const ipv6_neighbour_t *neighbour)
{
    ns_list_foreach(ipv6_destination_t, entry, &ipv6_destination_cache) {
        if (entry->last_neighbour == neighbour) {
            entry->last_neighbour = NULL;
        }
    }
}

#ifdef HAVE_IPV6_ND
void ipv6_destination_redirect(const uint8_t *dest_addr, const uint8_t *sender_addr, const uint8_t *redirect_addr, int8_t interface_id, addrtype_t ll_type, const uint8_t *ll_address)
{
    ipv6_destination_t *dest_entry = ipv6_destination_lookup_or_create(dest_addr, interface_id);
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(interface_id);
    bool to_router;

    if (!dest_entry || !ncache) {
        tr_warn("Redirect failure - no dest entry/ncache");
        return;
    }

    if (!dest_entry->last_neighbour || dest_entry->interface_id != interface_id || !addr_ipv6_equal(dest_entry->last_neighbour->ip_address, sender_addr)) {
        tr_warn("Redirect not sent from current next hop");
        return;
    }

    if (addr_ipv6_equal(redirect_addr, dest_addr)) {
        /* We're being told it is on-link */
        to_router = false;
    } else if (addr_is_ipv6_link_local(redirect_addr)) {
        /* We're being sent to a different router */
        to_router = true;
    } else {
        tr_debug("Invalid redirection: %s", trace_ipv6(redirect_addr));
        return;
    }

    // XXX need to consider invalidating/preserving other information?
    // Possibly not as we should only be handling this if not a router, so no
    // possibility of screwing up RPL. Although the "am I a router" check isn't
    // in place...
    dest_entry->redirected = true;
    memcpy(dest_entry->redirect_addr, redirect_addr, 16);

    ipv6_neighbour_t *ncache_entry = NULL;

    if (ll_type != ADDR_NONE) {
        ncache_entry = ipv6_neighbour_update_unsolicited(ncache, redirect_addr, ll_type, ll_address);
        if (ncache_entry) {
            ncache_entry->from_redirect = true;
        }
    }

    if (to_router) {
        if (!ncache_entry) {
            ncache_entry = ipv6_neighbour_lookup(ncache, redirect_addr);
        }

        if (ncache_entry) {
            ncache_entry->is_router = true;
        }
    }

    tr_debug("Redirection added");
    tr_debug("Iface %d destination: %s", interface_id, trace_ipv6(dest_addr));
    tr_debug("Old next hop: %s", trace_ipv6(sender_addr));
    tr_debug("New next hop: %s", trace_ipv6(redirect_addr));
}
#endif

static void ipv6_destination_release(ipv6_destination_t *dest)
{
    if (--dest->refcount == 0) {
        ns_dyn_mem_free(dest);
    }
}

static void ipv6_destination_cache_gc_periodic(void)
{
    uint_fast16_t gc_count = 0;
    ns_list_foreach_safe(ipv6_destination_t, entry, &ipv6_destination_cache) {
        if (entry->lifetime) {
            entry->lifetime--;
        }
        gc_count++;
#ifndef NO_IPV6_PMTUD
        /* Purge old PMTU values */
        if (entry->pmtu_lifetime) {
            if (entry->pmtu_lifetime <= DCACHE_GC_PERIOD) {
                tr_info("Resetting PMTU for: %s", trace_ipv6(entry->destination));
                entry->pmtu_lifetime = 0;
                uint16_t old_mtu = entry->pmtu;
                if (entry->interface_id >= 0) {
                    entry->pmtu = ipv6_neighbour_cache_by_interface_id(entry->interface_id)->link_mtu;
                } else {
                    entry->pmtu = 0xffff;
                }
                if (entry->pmtu != old_mtu) {
                //   socket_pmtu_changed(entry->destination, entry->interface_id, old_mtu, entry->pmtu);
                }
            } else {
                entry->pmtu_lifetime -= DCACHE_GC_PERIOD;
            }
        }
#endif
    }

    if (gc_count <= cache_long_term(true)) {
        return;
    }

    /* Cache is in most-recently-used-first order. GC strategy is to start from
     * the back, and reduce the size to "MAX_SHORT_TERM" every GC period,
     * deleting any entry. Timed-out entries will be deleted to keep it to
     * MAX_LONG_TERM.
     */
    ns_list_foreach_reverse_safe(ipv6_destination_t, entry, &ipv6_destination_cache) {
        if (entry->lifetime == 0 || gc_count > cache_short_term(true)) {
            ns_list_remove(&ipv6_destination_cache, entry);
            ipv6_destination_release(entry);
            if (--gc_count <= cache_long_term(true)) {
                break;
            }
        }
    }

}

void ipv6_destination_cache_timer(uint8_t seconds)
{
    dcache_gc_timer += seconds;

    if (dcache_gc_timer >= DCACHE_GC_PERIOD) {
        dcache_gc_timer -= DCACHE_GC_PERIOD;
        ipv6_destination_cache_gc_periodic();
        //ipv6_destination_cache_print(trace_debug_print);
        //ipv6_route_table_print(trace_debug_print);
    }
}

static const char *route_src_names[] = {
    [ROUTE_ANY]     = "?",
    [ROUTE_STATIC]  = "Static",
    [ROUTE_USER]    = "User",
    [ROUTE_LOOPBACK] = "Loopback",
    [ROUTE_RADV]    = "RAdv",
    [ROUTE_ARO]     = "ARO",
    [ROUTE_RPL_DAO] = "RPL DAO",
    [ROUTE_RPL_DAO_SR] = "RPL DAO SR",
    [ROUTE_RPL_SRH] = "RPL SRH",
    [ROUTE_RPL_DIO] = "RPL DIO",
    [ROUTE_RPL_ROOT] = "RPL Root",
    [ROUTE_RPL_INSTANCE] = "RPL Instance",
    [ROUTE_RPL_FWD_ERROR] = "RPL Fwd-Error",
    [ROUTE_MULTICAST] = "Multicast",
    [ROUTE_MPL]     = "MPL",
    [ROUTE_RIP]     = "RIP",
    [ROUTE_THREAD]  = "Thread",
    [ROUTE_THREAD_BORDER_ROUTER] = "Thread BR",
    [ROUTE_THREAD_PROXIED_HOST] = "Thread Proxy",
    [ROUTE_REDIRECT] = "Redirect",
};

/* Which types of routes get probed as per RFC 4191 */
/* (Others are assumed to be always reachable) */
static const bool ipv6_route_probing[ROUTE_MAX] = {
    [ROUTE_RADV] = true,
    [ROUTE_RPL_DAO] = true,
    [ROUTE_RPL_DIO] = true,
    [ROUTE_RPL_ROOT] = true,
    [ROUTE_RPL_INSTANCE] = true,
};

/* Which route types get minimum link MTU by default */
/* Makes life easier for tunnel-based systems like RPL */
static const bool ipv6_route_min_mtu[ROUTE_MAX] = {
    [ROUTE_RPL_DAO] = true,
    [ROUTE_RPL_DAO_SR] = true,
    [ROUTE_RPL_DIO] = true,
    [ROUTE_RPL_ROOT] = true,
    [ROUTE_RPL_INSTANCE] = true,
    [ROUTE_MPL] = true,
};

// Remember when a route source has deleted an entry - allows buffers still in
// event queue to have their route info invalidated.
static bool ipv6_route_source_invalidated[ROUTE_MAX];

static ipv6_route_predicate_fn_t *ipv6_route_predicate[ROUTE_MAX];
static ipv6_route_next_hop_fn_t *ipv6_route_next_hop_computation[ROUTE_MAX];

void ipv6_route_table_set_predicate_fn(ipv6_route_src_t src, ipv6_route_predicate_fn_t fn)
{
    ipv6_route_predicate[src] = fn;
}

void ipv6_route_table_set_next_hop_fn(ipv6_route_src_t src, ipv6_route_next_hop_fn_t fn)
{
    ipv6_route_next_hop_computation[src] = fn;
}

static void ipv6_route_print(const ipv6_route_t *route, route_print_fn_t *print_fn)
{
    // Route prefix is variable-length, so need to zero pad for ip6tos
    uint8_t addr[16] = { 0 };
    bitcopy(addr, route->prefix, route->prefix_len);
    ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str);
    if (route->lifetime != 0xFFFFFFFF) {
        print_fn(" %24s/%-3u if:%u src:'%s' id:%d lifetime:%"PRIu32,
               ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), route->prefix_len, route->info.interface_id,
               route_src_names[route->info.source], route->info.source_id, route->lifetime
              );
    } else {
        print_fn(" %24s/%-3u if:%u src:'%s' id:%d lifetime:infinite",
               ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), route->prefix_len, route->info.interface_id,
               route_src_names[route->info.source], route->info.source_id
              );
    }
    if (route->on_link) {
        print_fn("     On-link (met %d)", total_metric(route));
    } else {
        print_fn("     next-hop %s (met %d)", ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, route->info.next_hop_addr), total_metric(route));
    }
}

void ipv6_route_table_print(route_print_fn_t *print_fn)
{
    print_fn("Routing table:");
    ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
        ipv6_route_print(r, print_fn);
    }
}

/*
 * This function returns total effective metric, which is a combination
 * of 1) route metric, and 2) interface metric. Can be extended to include
 * protocol metric as well in the future.
 */
static uint16_t total_metric(const ipv6_route_t *route)
{
    ipv6_neighbour_cache_t *cache;
    uint16_t metric;

    metric = route->metric;
    cache = ipv6_neighbour_cache_by_interface_id(route->info.interface_id);

    if (cache) {
        metric += cache->route_if_info.metric;
    }

    return metric;
}

#ifdef FEA_TRACE_SUPPORT
void trace_debug_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vtracef(TRACE_LEVEL_DEBUG, TRACE_GROUP, fmt, ap);
    va_end(ap);
}
#endif

static void ipv6_route_entry_remove(ipv6_route_t *route)
{
    tr_debug("Deleted route:");
#ifdef FEA_TRACE_SUPPORT
    ipv6_route_print(route, trace_debug_print);
#endif
    if (protocol_core_buffers_in_event_queue > 0) {
        // Alert any buffers in the queue already routed by this source
        ipv6_route_source_invalidated[route->info.source] = true;
    }
    ns_list_remove(&ipv6_routing_table, route);
    ns_dyn_mem_free(route);
}

static bool ipv6_route_same_router(const ipv6_route_t *a, const ipv6_route_t *b)
{
    if (a == b) {
        return true;
    }
    return !a->on_link && !b->on_link &&
           a->info.interface_id == b->info.interface_id &&
           addr_ipv6_equal(a->info.next_hop_addr, b->info.next_hop_addr);
}

static void ipv6_route_probe(ipv6_route_t *route)
{
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(route->info.interface_id);
    if (!ncache || !ncache->send_nud_probes || route->probe_timer) {
        return;
    }

    ipv6_neighbour_t *n = ipv6_neighbour_lookup_or_create(ncache, route->info.next_hop_addr);
    if (!n) {
        return;
    }
    ipv6_interface_resolve_send_ns(ncache, n, true, 0);

    /* We need to limit to once per minute *per router* - so set the hold-off
     * timer for *all* routing entries to this router
     */
    ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
        if (ipv6_route_same_router(r, route)) {
            r->probe_timer = 60;
            r->probe = false;
        }
    }
}

/* Return true is a is better than b */
static bool ipv6_route_is_better(const ipv6_route_t *a, const ipv6_route_t *b)
{
    /* Prefer longer prefix */
    if (a->prefix_len < b->prefix_len) {
        return false;
    }

    if (a->prefix_len > b->prefix_len) {
        return true;
    }

    /* Prefer on-link */
    if (b->on_link && !a->on_link) {
        return false;
    }

    if (a->on_link && !b->on_link) {
        return true;
    }

    /* If prefixes exactly equal, tiebreak by metric */
    return total_metric(a) < total_metric(b);
}

/* Find the "best" route regardless of reachability, but respecting the skip flag and predicates */
static ipv6_route_t *ipv6_route_find_best(const uint8_t *addr, int8_t interface_id, ipv6_route_predicate_fn_t *predicate)
{
    ipv6_route_t *best = NULL;
    ns_list_foreach(ipv6_route_t, route, &ipv6_routing_table) {
        /* We mustn't be skipping this route */
        if (route->search_skip) {
            continue;
        }

        /* Interface must match, if caller specified */
        if (interface_id != -1 && interface_id != route->info.interface_id) {
            continue;
        }

        /* Prefix must match */
        if (!bitsequal(addr, route->prefix, route->prefix_len)) {
            continue;
        }

        /* Check the predicate for the route itself. This allows,
         * RPL "root" routes (the instance defaults) to be ignored in normal
         * lookup. Note that for caching to work properly, we require
         * the route predicate to produce "constant" results.
         */
        bool valid = true;
        if (ipv6_route_predicate[route->info.source]) {
            valid = ipv6_route_predicate[route->info.source](&route->info, valid);
        }

        /* Then the supplied search-specific predicate can override */
        if (predicate) {
            valid = predicate(&route->info, valid);
        }

        /* If blocked by either predicate, skip */
        if (!valid) {
            continue;
        }

        if (!best || ipv6_route_is_better(route, best)) {
            best = route;
        }
    }
    return best;
}

ipv6_route_t *ipv6_route_choose_next_hop(const uint8_t *dest, int8_t interface_id, ipv6_route_predicate_fn_t *predicate)
{
    ipv6_route_t *best = NULL;
    bool reachable = false;
    bool need_to_probe = false;

    ns_list_foreach(ipv6_route_t, route, &ipv6_routing_table) {
        route->search_skip = false;
    }

    /* Search algorithm from RFC 4191, S3.2:
     *
     * When a type C host does next-hop determination and consults its
     * Routing Table for an off-link destination, it searches its routing
     * table to find the route with the longest prefix that matches the
     * destination, using route preference values as a tie-breaker if
     * multiple matching routes have the same prefix length.  If the best
     * route points to a non-reachable router, this router is remembered for
     * the algorithm described in Section 3.5 below, and the next best route
     * is consulted.  This check is repeated until a matching route is found
     * that points to a reachable router, or no matching routes remain.
     *
     * Note that rather than having a separate on-link Prefix List, we have
     * on-link entries. These take precedence over default routes (by their
     * non-0 length), but not necessarily over more-specific routes. Therefore
     * it is possible that we consider a few non-reachable routers first, then
     * fall back to on-link. This behaviour may or may not be desired, depending
     * on the scenario. If not desired, the router entries should have their
     * "probing" flag set to false, so they always take precedence over
     * the on-link entry.
     *
     * There is currently no mechanism for an on-link entry to always take
     * precedence over a more-specific route, which is what would happen if
     * we really did have a separate Prefix List and Routing Table. One
     * possibility would be a special precedence flag.
     */
    for (;;) {
        ipv6_route_t *route = ipv6_route_find_best(dest, interface_id, predicate);
        if (!route) {
            break;
        }

        if (route->on_link) {
            reachable = true;
        } else {
            /* Some routes (eg RPL SR) compute next hop on demand */
            if (ipv6_route_next_hop_computation[route->info.source]) {
                if (!ipv6_route_next_hop_computation[route->info.source](dest, &route->info)) {
                    route->search_skip = true;
                    continue;
                }
            }

            ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(route->info.interface_id);
            if (!ncache) {
                tr_warn("Invalid interface ID in routing table!");
                route->search_skip = true;
                continue;
            }

            if (ncache->send_nud_probes && ipv6_route_probing[route->info.source]) {
                /* Going via a router - check reachability, as per RFC 4191.
                 * This only applies for certain routes (currently those from RAs) */
                reachable = ipv6_neighbour_addr_is_probably_reachable(ncache, route->info.next_hop_addr);
            } else {
                /* Can't probe, so have to assume router is reachable */
                reachable = true;
            }
        }

        if (reachable) {
            /* If router is reachable, we'll take it now */
            best = route;
            break;
        } else {
            /* Otherwise, note it, and look for other less-good reachable ones */
            route->search_skip = true;

            /* As we would have used it, probe to check for reachability */
            route->probe = need_to_probe = true;

            if (!best) {
                best = route;
            }
            continue;
        }
    }

    /* This is a bit icky - data structures are routes, but we need to probe
     * routers - a many->1 mapping. Probe flag is set on all routes we skipped;
     * but we don't want to probe the router we actually chose.
     */
    if (need_to_probe) {
        ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
            if (!r->probe) {
                continue;
            }
            r->probe = false;

            /* Note that best must be set if need_to_probe is */
            if (!ipv6_route_same_router(r, best) && ipv6_route_is_better(r, best)) {
                ipv6_route_probe(r);
            }
        }
    }

    if (best && !reachable) {
        /* We've chosen a non-reachable router; this means no routers were
         * reachable. Move it to the bottom of the list, so that next time
         * we do this, we try (and hence probe) another non-reachable router,
         * otherwise we'll never make progress. This satisfies the
         * round-robin requirement in RFC 4861 6.3.6.2, enhanced for RFC 4191.
         */
        ns_list_remove(&ipv6_routing_table, best);
        ns_list_add_to_end(&ipv6_routing_table, best);
    }

    return best;
}

ipv6_route_t *ipv6_route_lookup_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, int_fast16_t src_id)
{
    ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id && prefix_len == r->prefix_len && bitsequal(prefix, r->prefix, prefix_len)) {
            if (source != ROUTE_ANY) {
                if (source != r->info.source) {
                    continue;
                }
                if (info && info != r->info.info) {
                    continue;
                }
                if (src_id != -1 && src_id != r->info.source_id) {
                    continue;
                }
                if (info && ipv6_route_next_hop_computation[source]) {
                    /* No need to match the actual next hop - we assume info distinguishes */
                    return r;
                }
            }

            /* "next_hop" being NULL means on-link; this is a flag in the route entry, and r->next_hop can't be NULL */
            if ((next_hop && r->on_link) || (!next_hop && !r->on_link)) {
                continue;
            }

            if (next_hop && !r->on_link && !addr_ipv6_equal(next_hop, r->info.next_hop_addr)) {
                continue;
            }

            return r;
        }
    }

    return NULL;
}

#define PREF_TO_METRIC(pref) (128 - 64 * (pref))

uint8_t ipv6_route_pref_to_metric(int_fast8_t pref)
{
    if (pref <-1 || pref > +1) {
        pref = 0;
    }
    return PREF_TO_METRIC(pref);
}

ipv6_route_t *ipv6_route_add(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, uint32_t lifetime, int_fast8_t pref)
{
    return ipv6_route_add_with_info(prefix, prefix_len, interface_id, next_hop, source, NULL, 0, lifetime, pref);
}

ipv6_route_t *ipv6_route_add_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, uint8_t source_id, uint32_t lifetime, int_fast8_t pref)
{
    /* Only support -1, 0 and +1 prefs, as per RFC 4191 */
    if (pref < -1 || pref > +1) {
        return NULL;
    }

    return ipv6_route_add_metric(prefix, prefix_len, interface_id, next_hop, source, info,  source_id, lifetime, PREF_TO_METRIC(pref));
}

ipv6_route_t *ipv6_route_add_metric(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, uint8_t source_id, uint32_t lifetime, uint8_t metric)
{
    ipv6_route_t *route = NULL;
    enum { UNCHANGED, UPDATED, NEW } changed_info = UNCHANGED;

    // Maybe don't need this after all? We'll just assume that the next_hop is on-link
    // Thread certainly wants to use ULAs...
#if 0
    if (next_hop) {
        /* Currently we require that all routes must be to link-local addresses. */
        /* This simplifies all sorts of things - particularly that we can assume link-local addresses to be on-link. */
        /* It is needed to make Redirects and probes work too. */
        if (!addr_is_ipv6_link_local(next_hop)) {
            return NULL;
        }
    }
#endif


    /* Check for matching info, in which case it's an update */
    route = ipv6_route_lookup_with_info(prefix, prefix_len, interface_id, next_hop, source, info, source_id);

    /* 0 lifetime is a deletion request (common to all protocols) */
    if (lifetime == 0) {
        if (route) {
            tr_debug("Zero route lifetime");
            ipv6_route_entry_remove(route);
        }
        return NULL;
    }

    uint8_t max_entries = ipv6_route_table_get_max_entries(interface_id, source);
    if (max_entries > 0) {
        uint8_t entries = ipv6_route_table_count_source(interface_id, source);
        if (entries > max_entries) {
            ipv6_route_table_remove_last_one_from_source(interface_id, source);
        }
    }

    if (!route) { /* new route */
        uint_fast8_t prefix_bytes = (prefix_len + 7u) / 8u;
        route = ns_dyn_mem_alloc(sizeof(ipv6_route_t) + prefix_bytes);
        if (!route) {
            return NULL;
        }
        memset(route->prefix, 0, prefix_bytes);
        bitcopy(route->prefix, prefix, prefix_len);
        route->prefix_len = prefix_len;
        route->search_skip = false;
        route->probe = false;
        route->probe_timer = 0;
        route->lifetime = lifetime;
        route->metric = metric;
        route->info.source = source;
        route->info.info = info;
        route->info.source_id = source_id;
        route->info.interface_id = interface_id;
        route->info.pmtu = ipv6_route_min_mtu[source] ? IPV6_MIN_LINK_MTU : 0xFFFF;
        if (next_hop) {
            route->on_link = false;
            memcpy(route->info.next_hop_addr, next_hop, 16);
        } else {
            route->on_link = true;
            memset(route->info.next_hop_addr, 0, 16);
        }

        /* See ipv6_route_probe - all routing entries to the same router
         * want to share the same hold-off time, so search and copy.
         */
        if (next_hop) {
            ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
                if (ipv6_route_same_router(r, route)) {
                    route->probe_timer = r->probe_timer;
                    break;
                }
            }
        }

        /* Routing table will be resorted during use, thanks to probing. */
        /* Doesn't matter much where they start off, but put them at the */
        /* beginning so new routes tend to get tried first. */
        ns_list_add_to_start(&ipv6_routing_table, route);
        changed_info = NEW;
    } else { /* updating a route - only lifetime and metric can be changing */
        route->lifetime = lifetime;
        if (metric != route->metric) {
            route->metric = metric;
            changed_info = UPDATED;
        }
    }

    if (changed_info != UNCHANGED) {
        tr_debug("%s route:", changed_info == NEW ? "Added" : "Updated");
#ifdef FEA_TRACE_SUPPORT
        ipv6_route_print(route, trace_debug_print);
#endif
    }

    return route;
}

int_fast8_t ipv6_route_delete(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source)
{
    return ipv6_route_delete_with_info(prefix, prefix_len, interface_id, next_hop, source, NULL, 0);
}

int_fast8_t ipv6_route_delete_with_info(const uint8_t *prefix, uint8_t prefix_len, int8_t interface_id, const uint8_t *next_hop, ipv6_route_src_t source, void *info, int_fast16_t source_id)
{
    ipv6_route_t *route = ipv6_route_lookup_with_info(prefix, prefix_len, interface_id, next_hop, source, info, source_id);
    if (!route) {
        return -1;
    }

    ipv6_route_entry_remove(route);
    return 0;
}

void ipv6_route_table_remove_interface(int8_t interface_id)
{
    ns_list_foreach_safe(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id) {
            ipv6_route_entry_remove(r);
        }
    }
}

static void ipv6_route_table_remove_router(int8_t interface_id, const uint8_t *addr, ipv6_route_src_t source)
{
    ns_list_foreach_safe(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id && r->info.source == source && !r->on_link && addr_ipv6_equal(addr, r->info.next_hop_addr)) {
            ipv6_route_entry_remove(r);
        }
    }
}

/* Somewhat specialised - allow on-the-fly modification of metrics. Masking
 * allows for top "preference" bits to be preserved.
 */
void ipv6_route_table_modify_router_metric(int8_t interface_id, const uint8_t *addr, ipv6_route_src_t source, uint8_t keep, uint8_t toggle)
{
    ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id && r->info.source == source && !r->on_link && addr_ipv6_equal(addr, r->info.next_hop_addr)) {
            r->metric = (r->metric & keep) ^ toggle;
        }
    }

}

void ipv6_route_table_remove_info(int8_t interface_id, ipv6_route_src_t source, void *info)
{
    ns_list_foreach_safe(ipv6_route_t, r, &ipv6_routing_table) {
        if ((interface_id == -1 || interface_id == r->info.interface_id) && r->info.source == source && (info == NULL || r->info.info == info)) {
            ipv6_route_entry_remove(r);
        }
    }
}

static uint8_t ipv6_route_table_count_source(int8_t interface_id, ipv6_route_src_t source)
{
    uint8_t count = 0;
    ns_list_foreach(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id && r->info.source == source) {
            count++;
            if (count == 0xff) {
                break;
            }
        }
    }
    return count;
}

static void ipv6_route_table_remove_last_one_from_source(int8_t interface_id, ipv6_route_src_t source)
{
    // Removes last i.e. oldest entry */
    ns_list_foreach_reverse(ipv6_route_t, r, &ipv6_routing_table) {
        if (interface_id == r->info.interface_id && r->info.source == source) {
            ipv6_route_entry_remove(r);
            break;
        }
    }
}


void ipv6_route_table_ttl_update(uint16_t seconds)
{
    ns_list_foreach_safe(ipv6_route_t, r, &ipv6_routing_table) {
        if (r->probe_timer) {
            if (r->probe_timer > seconds) {
                r->probe_timer -= seconds;
            } else {
                r->probe_timer = 0;
            }
        }

        if (r->lifetime == 0xFFFFFFFF) {
            continue;
        }

        if (r->lifetime > seconds) {
            r->lifetime -= seconds;
            continue;
        }

        tr_debug("Route expired");
        ipv6_route_entry_remove(r);
    }
}

void ipv6_route_table_set_max_entries(int8_t interface_id, ipv6_route_src_t source, uint8_t max_entries)
{
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(interface_id);

    if (ncache) {
        ncache->route_if_info.sources[source] = max_entries;
    }
}

static uint8_t ipv6_route_table_get_max_entries(int8_t interface_id, ipv6_route_src_t source)
{
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(interface_id);

    if (ncache) {
        return ncache->route_if_info.sources[source];
    }

    return 0;
}

bool ipv6_route_table_source_was_invalidated(ipv6_route_src_t src)
{
    return ipv6_route_source_invalidated[src];
}

// Called when event queue is empty - no pending buffers so can clear invalidation flags.
void ipv6_route_table_source_invalidated_reset(void)
{
    memset(ipv6_route_source_invalidated, false, sizeof ipv6_route_source_invalidated);
}
