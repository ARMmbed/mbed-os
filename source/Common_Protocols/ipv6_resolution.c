/*
 * Copyright (c) 2015-2019, Arm Limited and affiliates.
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
#include "Core/include/ns_socket.h"
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
#include "Common_Protocols/tcp.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "Service_Libs/etx/etx.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"

#define TRACE_GROUP "ip6r"

#ifndef RESOLUTION_QUEUE_LIMIT
#define RESOLUTION_QUEUE_LIMIT 2
#endif

void ipv6_interface_resolve_send_ns(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry, bool unicast, uint_fast8_t seq)
{
    protocol_interface_info_entry_t *cur_interface = NS_CONTAINER_OF(cache, protocol_interface_info_entry_t, ipv6_neighbour_cache);

    if (cur_interface->if_ns_transmit) {
        /* Thread uses DHCP Leasequery (!) instead of NS for address resolution */
        /* We still allow unicast NUD probes using NS, although I expect them to be disabled */
        if (cur_interface->if_ns_transmit(cur_interface, entry, unicast, seq)) {
            return;
        }
    }

#ifdef HAVE_IPV6_ND
    tr_debug("Sending %s NS for: %s",
             (unicast ? "unicast" : "multicast"), trace_ipv6(entry->ip_address));

    buffer_t *prompting_packet = ns_list_get_first(&entry->queue);
    buffer_t *buf = icmpv6_build_ns(cur_interface, entry->ip_address,
                                    prompting_packet ? prompting_packet->src_sa.address : NULL,
                                    unicast, false, NULL);
    protocol_push(buf);
#else
    tr_error("No NS handler for interface %d", cur_interface->id);
#endif
}

/* Entry has already been removed from cache, and is about to be freed. Hence entry->queue can't change while we process it */
void ipv6_interface_resolution_failed(ipv6_neighbour_cache_t *cache, ipv6_neighbour_t *entry)
{
    protocol_interface_info_entry_t *cur_interface = NS_CONTAINER_OF(cache, protocol_interface_info_entry_t, ipv6_neighbour_cache);

    tr_warn("LL addr of %s not found", trace_ipv6(entry->ip_address));
    ns_list_foreach_safe(buffer_t, buf, &entry->queue) {
        ns_list_remove(&entry->queue, buf);
        uint8_t code;
        if (buf->options.ip_extflags & IPEXT_SRH_RPL) {
            /* Note that as the border router we loopback SRH errors to ourselves if the first hop doesn't resolve */
            code = ICMPV6_CODE_DST_UNREACH_SRC_RTE_HDR_ERR;
        } else {
            code = ICMPV6_CODE_DST_UNREACH_ADDR_UNREACH;
        }
        /* XXX note no special handling for our own socket transmissions,
         * unlike original case. If we want this, we should do it in ICMP
         * RX handling, so we get events for external errors.
         */
        buf = socket_tx_buffer_event(buf, SOCKET_NO_ROUTE);
        if (buf) {
            buf = icmpv6_error(buf, cur_interface, ICMPV6_TYPE_ERROR_DESTINATION_UNREACH, code, 0);
            protocol_push(buf);
        }
    }
}

/* Silly bit of interface glue - ipv6_routing_table.c doesn't know about interface structures,
 * but it needs to be able to get from the interface id in the Routing Table and/or
 * Destination Cache to the relevant Neighbour Cache
 */
ipv6_neighbour_cache_t *ipv6_neighbour_cache_by_interface_id(int8_t interface_id)
{
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(interface_id);

    return interface ? &interface->ipv6_neighbour_cache : NULL;
}

void ipv6_send_queued(ipv6_neighbour_t *entry)
{
    ns_list_foreach_safe(buffer_t, buf, &entry->queue) {
        ns_list_remove(&entry->queue, buf);
        tr_debug("Destination solved");
        protocol_push(buf);
    }
}

static void ipv6_trigger_resolve_query(protocol_interface_info_entry_t *cur_interface, buffer_t *buf, ipv6_neighbour_t *n)
{
    if (n->state != IP_NEIGHBOUR_NEW && n->state != IP_NEIGHBOUR_INCOMPLETE) {
        tr_debug("ipv6_resolve_query");
        buffer_free(buf);
        return;
    }

    uint_fast16_t count = ns_list_count(&n->queue);
    while (count >= RESOLUTION_QUEUE_LIMIT) {
        buffer_t *b = ns_list_get_first(&n->queue);
        ns_list_remove(&n->queue, b);
        socket_tx_buffer_event_and_free(b, SOCKET_NO_ROUTE);
        count--;
    }
    tr_debug("Queueing for: %s", trace_ipv6(n->ip_address));
    ns_list_add_to_end(&n->queue, buf);

    if (n->state == IP_NEIGHBOUR_NEW) {
        /* Start NS timers, send first NS */
        ipv6_neighbour_set_state(&cur_interface->ipv6_neighbour_cache, n, IP_NEIGHBOUR_INCOMPLETE);
        ipv6_interface_resolve_send_ns(&cur_interface->ipv6_neighbour_cache, n, false, 0);
    }

}


/* Given a buffer with IP next-hop address and outgoing interface, find the
 * neighbour entry, and if complete, write the link-layer address into the buffer
 * destination, and return the Neighbour Cache entry.
 * If we have an incomplete Neighbour Cache entry, start address resolution
 * and queue the buffer, returning NULL.
 */
ipv6_neighbour_t *ipv6_interface_resolve_new(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    buffer_routing_info_t *route = ipv6_buffer_route(buf);
    if (!route) {
        tr_warn("XXX ipv6_interface_resolve no route!");
        // Can this happen? How did it get to this interface in the first place?
        // If it can happen, send ICMP Destination Unreachable
        buffer_free(buf);
        return NULL;
    }
    ipv6_neighbour_t *n = ipv6_neighbour_lookup_or_create(&cur->ipv6_neighbour_cache, route->route_info.next_hop_addr);
    if (!n) {
        // If it can happen, send ICMP Destination Unreachable
        tr_warn("No heap for address resolve");
        buffer_free(buf);
        return NULL;
    }

    if (n->state == IP_NEIGHBOUR_NEW || n->state == IP_NEIGHBOUR_INCOMPLETE) {
        addrtype_t ll_type;
        const uint8_t *ll_addr;

        if (cur->if_map_ip_to_link_addr &&
                cur->if_map_ip_to_link_addr(cur, route->route_info.next_hop_addr, &ll_type, &ll_addr)) {
            ipv6_neighbour_update_from_na(&cur->ipv6_neighbour_cache, n, NA_O, ll_type, ll_addr);
        }
    }

    if (n->state == IP_NEIGHBOUR_NEW || n->state == IP_NEIGHBOUR_INCOMPLETE) {
        ipv6_trigger_resolve_query(cur, buf, n);
        return NULL;
    }

    buf->dst_sa.addr_type = n->ll_type;
    memcpy(buf->dst_sa.address, n->ll_address, addr_len_from_type(n->ll_type));

    /* Optimisation trick - if security bypass is set, this is presumably some
     * sort of MLE-type link management packet. Not worth sending extra NS/NA
     * noise for these.
     */
    if (!(buf->options.ll_security_bypass_tx && addr_is_ipv6_link_local(route->route_info.next_hop_addr))) {
        n = ipv6_neighbour_used(&cur->ipv6_neighbour_cache, n);
    }
    return n;
}

/* Attempt a mapping from current information (neighbour cache, hard mappings) */
bool ipv6_map_ip_to_ll(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *n, const uint8_t ip_addr[16], addrtype_t *ll_type, const uint8_t **ll_addr_out)
{
    if (!n) {
        n = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, ip_addr);
    }
    if (n && !(n->state == IP_NEIGHBOUR_NEW || n->state == IP_NEIGHBOUR_INCOMPLETE)) {
        *ll_type = n->ll_type;
        *ll_addr_out = n->ll_address;
        return true;
    }

    if (cur->if_map_ip_to_link_addr &&
            cur->if_map_ip_to_link_addr(cur, ip_addr, ll_type, ll_addr_out)) {
        return true;
    }

    return false;
}

/* Attempt a mapping from current information (neighbour cache, hard mappings) */
bool ipv6_map_ll_to_ip_link_local(protocol_interface_info_entry_t *cur, addrtype_t ll_type, const uint8_t *ll_addr, uint8_t ip_addr_out[16])
{
    if (cur->if_map_link_addr_to_ip &&
            cur->if_map_link_addr_to_ip(cur, ll_type, ll_addr, ip_addr_out)) {
        return true;
    }

    ns_list_foreach(ipv6_neighbour_t, n, &cur->ipv6_neighbour_cache.list) {
        if (ipv6_neighbour_ll_addr_match(n, ll_type, ll_addr) && addr_is_ipv6_link_local(n->ip_address)) {
            memcpy(ip_addr_out, n->ip_address, 16);
            return true;
        }
    }

    return false;
}

/* To comply with ETX returns 0xFFFF when neighbor doesn't exist and 0 when neighbor is currently unknown. */
uint16_t ipv6_map_ip_to_ll_and_call_ll_addr_handler(protocol_interface_info_entry_t *cur, int8_t interface_id, ipv6_neighbour_t *n, const uint8_t ipaddr[16], ll_addr_handler_t *ll_addr_handler_ptr)
{
    addrtype_t ll_type;
    const uint8_t *ll_addr;

    if (!cur) {
        cur = protocol_stack_interface_info_get_by_id(interface_id);
        if (!cur) {
            return 0xFFFF;
        }
    }

    if (!ipv6_map_ip_to_ll(cur, n, ipaddr, &ll_type, &ll_addr)) {
        return 0;
    }

    return ll_addr_handler_ptr(cur->id, ll_type, ll_addr);
}
