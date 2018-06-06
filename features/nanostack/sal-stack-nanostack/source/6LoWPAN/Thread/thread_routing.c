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
/*
 * Thread-specific routing functionality
 *
 * draft-kelsey-thread-routing-00
 */

#include "nsconfig.h"
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <randLIB.h>
#include <nsdynmemLIB.h>

#define THREAD_ROUTING_FN extern

#include  <net_thread_test.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MLE/mle.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_nd.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_leader_service.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "trou"

/* MLE Route Data bit assignments (draft-kelsey-thread-routing-00) */
#define ROUTE_DATA_OUT_MASK     0xC0
#define ROUTE_DATA_OUT_SHIFT    6
#define ROUTE_DATA_IN_MASK      0x30
#define ROUTE_DATA_IN_SHIFT     4
#define ROUTE_DATA_COST_MASK    0x0F
#define ROUTE_DATA_OURSELF      0x01

/*
 * MAX_LINK_AGE must be > 1.5 * trickle Imax, as that's the maximum spacing
 * between advert transmissions (assuming all peers have same Imax, as they
 * should)
 *
 * |---Imax---|---Imax---|  (Two Imax intervals, transmitting at Imax/2 in first
 *       t               t   and Imax-1 in second, so 1.5*Imax - 1 apart).
 */
#define MAX_LINK_AGE 100*10     /* 100 seconds */

#define LINK_AGE_STATIC 0xFFF  /* Magic number to indicate "never expire" */

#ifdef HAVE_THREAD_ROUTER

static trickle_params_t thread_mle_advert_trickle_params = {
    .Imin = 1 * 10,     /* 1 second; ticks are 100ms */
    .Imax = 32 * 10,    /* 32 seconds */
    .k = 0,             /* infinity - no consistency checking */
    .TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE
};

static bool thread_update_fast_route(thread_info_t *thread, thread_router_id_t dest);
static void thread_update_fast_route_table(thread_info_t *thread);

bool router_id_sequence_is_greater(const thread_routing_info_t *routing, uint8_t seq)
{
    return !routing->router_id_sequence_valid || common_serial_number_greater_8(seq, routing->router_id_sequence);
}

/*
 * Hysteresis for quality changes in dB. Note that in this implementation, given
 * the thresholds, this must be less than 2dB. At 1dB, quality will switch from
 * "2dB" to "BAD" only when link margin drops below 1dB.
 */
#define LINK_QUALITY_HYSTERESIS (1 << THREAD_LINK_MARGIN_SCALING)   /* 1dB */

static thread_link_margin_t link_quality_to_margin_lower_bound(thread_link_quality_e quality)
{
    switch (quality) {
        case QUALITY_20dB:
            return 20 << THREAD_LINK_MARGIN_SCALING;
        case QUALITY_10dB:
            return 10 << THREAD_LINK_MARGIN_SCALING;
        case QUALITY_2dB:
            return 2 << THREAD_LINK_MARGIN_SCALING;
        default:
            return 0;
    }
}

static thread_link_margin_t link_quality_to_margin_upper_bound(thread_link_quality_e quality)
{
    switch (quality) {
        case QUALITY_10dB:
            return 20 << THREAD_LINK_MARGIN_SCALING;
        case QUALITY_2dB:
            return 10 << THREAD_LINK_MARGIN_SCALING;
        case QUALITY_BAD:
            return 2 << THREAD_LINK_MARGIN_SCALING;
        default:
            return THREAD_LINK_MARGIN_MAX;
    }
}
static thread_link_quality_e link_margin_to_quality_with_hysteresis(thread_link_margin_t margin, thread_link_quality_e old_quality)
{
    thread_link_quality_e new_quality = thread_link_margin_to_quality(margin);

    if ((new_quality > old_quality && margin > link_quality_to_margin_upper_bound(old_quality) + LINK_QUALITY_HYSTERESIS) ||
            (new_quality < old_quality && margin < link_quality_to_margin_lower_bound(old_quality) - LINK_QUALITY_HYSTERESIS)) {
        return new_quality;
    } else {
        return old_quality;
    }
}

static bool cost_less(thread_route_cost_t a, thread_route_cost_t b)
{
    if (a == THREAD_COST_INFINITE) {
        return false;
    } else if (b == THREAD_COST_INFINITE) {
        return true;
    } else {
        return a < b;
    }
}

bool thread_i_am_router(const protocol_interface_info_entry_t *cur)
{
    return cur->thread_info && cur->mac_parameters && thread_is_router_addr(mac_helper_mac16_address_get(cur));
}


/* Look up a router in our list of neighbour routers - return NULL if not a neighbour */
static thread_router_link_t *thread_get_neighbour_router_by_id(thread_routing_info_t *routing, thread_router_id_t id)
{
    ns_list_foreach(thread_router_link_t, neighbour, &routing->link_set) {
        if (neighbour->router_id == id) {
            return neighbour;
        }
    }

    return NULL;
}

static inline thread_link_quality_e thread_quality_combine(thread_link_quality_e in, thread_link_quality_e out)
{
    thread_link_quality_e q;

    if (out < in) {
        q = out;
    } else {
        q = in;
    }

    return q;
}

/* Return the quality (worse of incoming and outgoing quality) for a neighbour router */
static inline thread_link_quality_e thread_neighbour_router_quality(const thread_router_link_t *neighbour)
{
    return thread_quality_combine(neighbour->incoming_quality, neighbour->outgoing_quality);
}


/* Return the quality for a neighbour router; return QUALITY_BAD if not a neighbour */
static thread_link_quality_e thread_get_neighbour_router_quality(thread_routing_info_t *routing, thread_router_id_t id)
{
    thread_router_link_t *neighbour = thread_get_neighbour_router_by_id(routing, id);
    if (!neighbour) {
        return QUALITY_BAD;
    }

    return thread_neighbour_router_quality(neighbour);
}

/* Return the routing cost for a neighbour router */
static thread_route_cost_t thread_neighbour_router_cost(const thread_router_link_t *neighbour)
{
    return thread_link_quality_to_cost(thread_neighbour_router_quality(neighbour));
}

/* Return the routing cost for a neighbour router; return THREAD_COST_INFINITE if
 * not a neighbour.
 */
static thread_route_cost_t thread_get_neighbour_router_cost(thread_routing_info_t *routing, thread_router_id_t id)
{
    return thread_link_quality_to_cost(thread_get_neighbour_router_quality(routing, id));
}

static thread_route_t *thread_get_route_entry_by_id(thread_routing_info_t *routing, thread_router_id_t id)
{
    ns_list_foreach(thread_route_t, r, &routing->route_set) {
        if (r->destination == id) {
            return r;
        }
    }

    return NULL;
}

static thread_route_t *thread_delete_route_entry_by_id(thread_info_t *thread, thread_router_id_t id)
{
    thread_route_t *r = thread_get_route_entry_by_id(&thread->routing, id);
    if (r) {
        ns_list_remove(&thread->routing.route_set, r);
        ns_dyn_mem_free(r);
        if (thread_update_fast_route(thread, id)) {
            trickle_inconsistent_heard(&thread->routing.mle_advert_timer, &thread_mle_advert_trickle_params);
        }
    }

    return NULL;
}

/* Routing function for Mesh layer */
static int_fast8_t thread_route_fn(
    protocol_interface_info_entry_t *cur,
    uint_fast8_t last_hop_addr_len, const uint8_t last_hop_addr[last_hop_addr_len],
    uint_fast8_t addr_len, const uint8_t dest_addr[addr_len],
    mesh_routing_route_response_t *resp)
{
    if (addr_len != 2) {
        return -1;
    }
    uint16_t mac16 = mac_helper_mac16_address_get(cur);
    thread_info_t *thread = cur->thread_info;
    if (!thread  || mac16 >= 0xfffe) {
        return -1;
    }


    if (!thread_is_router_addr(mac16)) {
        /* We're just a leaf - always send to our parent router */
        resp->addr_len = 2;
        common_write_16_bit(thread_router_addr_from_addr(mac16), resp->address);
        return 0;
    }

    uint16_t dest = common_read_16_bit(dest_addr);
    uint16_t dest_router_addr = thread_router_addr_from_addr(dest);
    if (dest_router_addr == mac16) {
        /* We're this device's parent - transmit direct to it */
        mle_neigh_table_entry_t *entry = mle_class_get_by_link_address(cur->id, dest_addr, ADDR_802_15_4_SHORT);
        if (!entry || (entry->mode & MLE_DEV_MASK) == MLE_RFD_DEV) {
            /* To cover some of draft-kelsey-thread-network-data-00, we send the
             * packet up to our own IP layer in the case where it's addressed to
             * an unrecognised child. The special IP forwarding rules can then
             * generate an ICMP message. Also catching reduced function device here.
             */
            resp->intercept = true;
            return 0;
        }
        resp->addr_len = 2;
        common_write_16_bit(dest, resp->address);
        return 0;
    }

    thread_router_id_t dest_router_id = thread_router_id_from_addr(dest_router_addr);
    if (thread->routing.fast_route_table[dest_router_id] >= N_THREAD_ROUTERS) {
        return -1;
    }

    thread_router_id_t next_hop_router_id = thread->routing.fast_route_table[dest_router_id];
    uint16_t next_hop_router_addr = thread_router_addr_from_id(next_hop_router_id);

    /* 2-hop loop detection required by Thread spec */
    if (last_hop_addr_len == 2 && next_hop_router_addr == common_read_16_bit(last_hop_addr)) {
        tr_debug("%d: two-hop loop detected", dest_router_id);
        thread_delete_route_entry_by_id(thread, dest_router_id);
        // Don't drop packet - we still forward, which gives other hop
        // a chance to detect the loop. If it comes back again, then we
        // won't have a route that time. (Spec isn't explicit here -
        // found this works better than dropping).
    }

    resp->addr_len = 2;
    common_write_16_bit(next_hop_router_addr, resp->address);
    return 0;
}

/* First hop function for Mesh layer */
static int_fast8_t thread_first_hop_fn(
    protocol_interface_info_entry_t *cur,
    uint_fast8_t addr_len, const uint8_t dest_addr[addr_len],
    mesh_routing_route_response_t *resp)
{
    /* Re-use the routing function */
    return thread_route_fn(cur, 0, NULL, addr_len, dest_addr, resp);
}

static thread_route_cost_t thread_compute_route_cost(thread_routing_info_t *routing, thread_router_id_t dest, thread_router_id_t *next_hop)
{
    /* Never attempt to route if router ID is known invalid */
    if (routing->fast_route_table[dest] == FAST_ROUTE_INVALID_ID) {
        return THREAD_COST_INFINITE;
    }

    /* Check cost for direct transmission */
    thread_route_cost_t cost_direct = thread_get_neighbour_router_cost(routing, dest);

    /* Then cost for multihop transmission */
    thread_route_cost_t cost_multihop = THREAD_COST_INFINITE;
    thread_route_t *route_entry = thread_get_route_entry_by_id(routing, dest);
    if (route_entry) {
        thread_route_cost_t next_hop_cost = thread_get_neighbour_router_cost(routing, route_entry->next_hop);
        cost_multihop = thread_link_cost_sum(route_entry->route_cost, next_hop_cost);
    }

    if (cost_direct == THREAD_COST_INFINITE && cost_multihop == THREAD_COST_INFINITE) {
        return THREAD_COST_INFINITE;
    } else {
        if (route_entry && cost_less(cost_multihop, cost_direct)) {
            if (next_hop) {
                *next_hop = route_entry->next_hop;
            }
            return cost_multihop;
        } else {
            if (next_hop) {
                *next_hop = dest;
            }
            return cost_direct;
        }
    }
}

/* Called by thread_routing.c as a result of updates to routing table - allows
 * leader to monitor a router being available (having a finite route cost).
 * Possible values for cost are 1-15, or 0 meaning infinite.
 */
static void thread_router_link_changed(thread_info_t *thread_info, uint8_t router_id, uint8_t cost, int8_t interface_id)
{
    /* Convert cost to scale to 0 (= cost 1) to 15 (= cost infinite), and place in bottom 4 bits of metric */
    uint8_t metric = ((unsigned) cost - 1) & 0xf;

    uint16_t router_addr_16 = thread_router_addr_from_id(router_id);
    uint8_t router_ip_addr[16];
    thread_addr_write_mesh_local_16(router_ip_addr, router_addr_16, thread_info);

    /* Leave upper (preference) bits of metric untouched */
    ipv6_route_table_modify_router_metric(interface_id, router_ip_addr, ROUTE_THREAD_BORDER_ROUTER, 0xf0, metric);

    /* Also tell the leader, who's monitoring which are available */
    thread_leader_service_router_state_changed(thread_info, router_id, cost != 0, interface_id);

}

static void set_fast_route_entry(thread_info_t *thread, thread_router_id_t dest, uint8_t value, thread_route_cost_t cost)
{
    thread->routing.fast_route_table[dest] = value;
    thread_router_link_changed(thread, dest, cost, thread->interface_id);
}

/* Returns true if a change relevant to MLE Trickle has occurred */
static bool thread_update_fast_route(thread_info_t *thread, thread_router_id_t dest)
{
    thread_routing_info_t *routing = &thread->routing;

    if (routing->fast_route_table[dest] == FAST_ROUTE_INVALID_ID) {
        return false;
    }

    /* Trickle only cares if routes come or go, not if they change */
    bool change = false;
    thread_router_id_t next_hop;
    thread_route_cost_t cost = thread_compute_route_cost(routing, dest, &next_hop);
    if (cost == THREAD_COST_INFINITE) {
        if (routing->fast_route_table[dest] != FAST_ROUTE_NO_ROUTE) {
            change = true;
        }

        set_fast_route_entry(thread, dest, FAST_ROUTE_NO_ROUTE, THREAD_COST_INFINITE);
    } else {
        if (routing->fast_route_table[dest] == FAST_ROUTE_NO_ROUTE) {
            change = true;
        }

        set_fast_route_entry(thread, dest, next_hop, cost);
    }

    return change;
}

/* Rescan the neighbour list to generate the overall routing table. "id_valid"
 * fields are always live - this updates the other fields.
 */
static void thread_update_fast_route_table(thread_info_t *thread)
{
    bool change = false;

    for (thread_router_id_t id = 0; id < N_THREAD_ROUTERS; id++) {
        change |= thread_update_fast_route(thread, id);
    }

    if (change) {
        trickle_inconsistent_heard(&thread->routing.mle_advert_timer, &thread_mle_advert_trickle_params);
    }
}

void thread_routing_update_id_set(protocol_interface_info_entry_t *cur, uint8_t seq, const uint8_t *id_mask)
{
    thread_info_t *thread = cur->thread_info;
    thread_routing_info_t *routing = &thread->routing;
    bool change = false;

    if (!router_id_sequence_is_greater(routing, seq)) {
        return;
    }
    routing->router_id_sequence = seq;
    routing->router_id_sequence_valid = true;

    for (thread_router_id_t i = 0; i < N_THREAD_ROUTERS; i++) {
        if (bit_test(id_mask, i)) {
            if (routing->fast_route_table[i] == FAST_ROUTE_INVALID_ID) {
                /* Won't have any route info for this new router ID, but may have
                 * some existing link quality data from a handshake. Set
                 * initial route to direct, if we think we can hear it.
                 */
                thread_route_cost_t cost = thread_get_neighbour_router_cost(routing, i);
                if (cost == THREAD_COST_INFINITE) {
                    set_fast_route_entry(thread, i, FAST_ROUTE_NO_ROUTE, THREAD_COST_INFINITE);
                } else {
                    set_fast_route_entry(thread, i, i, cost);
                    change = true;
                }
                tr_info("Add router (ID: %d)", i);
            }
        } else {
            if (routing->fast_route_table[i] != FAST_ROUTE_INVALID_ID) {
                if (routing->fast_route_table[i] != FAST_ROUTE_NO_ROUTE) {
                    change = true;
                }
                set_fast_route_entry(thread, i, FAST_ROUTE_INVALID_ID, THREAD_COST_INFINITE);
                tr_info("Remove router (ID: %d)", i);
                thread_nd_flush_neighbour_cache_for_short_addr(cur, thread_router_addr_from_id(i), true);
                thread_routing_remove_link(cur, thread_router_addr_from_id(i));
                thread_delete_route_entry_by_id(thread, i);
            }
        }
    }

    /* Transitions from invalid->finite or finite->invalid must kick timer */
    if (change) {
        trickle_inconsistent_heard(&routing->mle_advert_timer, &thread_mle_advert_trickle_params);
    }
}

void thread_routing_force_next_hop(protocol_interface_info_entry_t *cur, uint8_t id_seq, const uint8_t *id_mask, thread_router_id_t next_hop_id)
{
    /*
     * Called when we become a router, to make our original parent be
     * the initial path to all routers.
     *
     * Do this by creating and processing a fake advertisement from the
     * next hop. (We assume we have no existing routing information, so
     * it wins and ends up being our route to everywhere). Real routing
     * information, either from other routers or the one we're faking,
     * will then take over later.
     */
    uint8_t route_data[64];
    uint8_t *ptr = route_data;
    thread_router_id_t my_router_id = thread_router_id_from_addr(mac_helper_mac16_address_get(cur));

    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        if (!bit_test(id_mask, r)) {
            continue;
        }

        if (r == my_router_id) {
            // It has a bad link to us
            *ptr++ = (QUALITY_2dB << ROUTE_DATA_OUT_SHIFT) | (QUALITY_2dB << ROUTE_DATA_IN_SHIFT) | thread_link_quality_to_cost(QUALITY_2dB);
        } else if (r == next_hop_id) {
            // It reports itself correctly
            *ptr++ = ROUTE_DATA_OURSELF;
        } else {
            // It has a maximum-length route to every other router (and no direct link)
            *ptr++ = (QUALITY_BAD << ROUTE_DATA_OUT_SHIFT) | (QUALITY_BAD << ROUTE_DATA_IN_SHIFT) | (THREAD_MAX_ROUTE_COST - thread_link_quality_to_cost(QUALITY_2dB));
        }
    }

    if (thread_routing_add_link(cur, thread_router_addr_from_id(next_hop_id), 4 /* dB (bad) */,
                                id_seq, id_mask, route_data, false)) {
        tr_warn("Function thread_routing_force_next_hop() failed");
    }
}

/* We leave it to the caller to give us the dB link margin Thread wants.
 *
 * Getting that will be hardware dependent - new RF API needed...
 */

static thread_router_link_t *thread_routing_update_link_margin_internal(thread_info_t *thread,
        uint16_t sender,
        uint8_t link_margin_db)
{
    thread_router_id_t sender_id = thread_router_id_from_addr(sender);

    /* Find the sender in our Link Set - creating an entry if necessary */
    thread_router_link_t *neighbour = thread_get_neighbour_router_by_id(&thread->routing, sender_id);
    if (!neighbour) {
        neighbour = ns_dyn_mem_alloc(sizeof * neighbour);
        if (!neighbour) {
            return NULL;
        }
        neighbour->router_id = sender_id;
        neighbour->link_margin = link_margin_db << THREAD_LINK_MARGIN_SCALING;
        neighbour->incoming_quality = thread_link_margin_to_quality(neighbour->link_margin);
        neighbour->outgoing_quality = QUALITY_BAD;
        neighbour->outgoing_quality_known = false;
        neighbour->as_good = false;
        ns_list_add_to_end(&thread->routing.link_set, neighbour);
        trickle_inconsistent_heard(&thread->routing.mle_advert_timer, &thread_mle_advert_trickle_params); //Reset Trigle when learn new link
    } else {
        /* Exponentially weighted moving average, weighted by THREAD_LINK_MARGIN_SCALING */
        neighbour->link_margin = neighbour->link_margin + link_margin_db - (neighbour->link_margin >> THREAD_LINK_MARGIN_SCALING);
        neighbour->incoming_quality = link_margin_to_quality_with_hysteresis(neighbour->link_margin, (thread_link_quality_e) neighbour->incoming_quality);
    }
    neighbour->age = 0;

    return neighbour;
}

int_fast8_t thread_routing_update_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t sender,
        uint8_t link_margin_db,
        uint8_t outgoing_link_margin_db)
{
    thread_info_t *thread = cur->thread_info;
    /* Sanity check that the source is a Thread router */
    if (!thread || !thread_is_router_addr(sender)) {
        return -2;
    }

    tr_debug("New margin info for %04x: in=%d, out=%d", sender, link_margin_db, outgoing_link_margin_db);

    /* We record link quality info even if we're not currently a router - we can
     * use the info when we become one, and as part of the decision about
     * whether to become one.
     */

    thread_router_link_t *neighbour = thread_routing_update_link_margin_internal(thread, sender, link_margin_db);
    if (!neighbour) {
        return -3;
    }

    /* We use the outgoing link margin provided here only if we haven't ever
     * heard an actual Route TLV quality report from the neighbour. This is
     * intended for "secondary" info, derived from RSSI TLVs to bootstrap.
     */
    if (!neighbour->outgoing_quality_known) {
        neighbour->outgoing_quality = thread_link_margin_to_quality(outgoing_link_margin_db);
    }

    thread_update_fast_route_table(thread);

    return 0;
}

int_fast8_t thread_routing_force_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t addr,
        uint8_t link_margin_db)
{
    thread_info_t *thread = cur->thread_info;
    if (!thread || !cur->mac_parameters || !thread_is_router_addr(addr)) {
        return -2;
    }

    thread_router_id_t sender_id = thread_router_id_from_addr(addr);
    thread_router_link_t *neighbour = thread_get_neighbour_router_by_id(&thread->routing, sender_id);
    if (!neighbour) {
        return -3;
    }

    tr_debug("Forcing link margin for %04x: in=%d", addr, link_margin_db);

    neighbour->link_margin = link_margin_db << THREAD_LINK_MARGIN_SCALING;
    neighbour->incoming_quality = thread_link_margin_to_quality(neighbour->link_margin);

    thread_update_fast_route_table(thread);

    return 0;
}

int_fast8_t thread_routing_add_link(protocol_interface_info_entry_t *cur,
                                    uint16_t sender, uint8_t link_margin_db,
                                    uint8_t id_seq,
                                    const uint8_t *id_mask,
                                    const uint8_t *route_data,
                                    bool is_static)
{
    thread_info_t *thread = cur->thread_info;
    if (!thread) {
        return -2;
    }

    /* Update master Router ID Set */
    thread_routing_update_id_set(cur, id_seq, id_mask);

    /* Sanity check that the source is a Thread router */
    if (!thread_is_router_addr(sender)) {
        return -2;
    }

    /* Even if not currently a router, worth tracking incoming link margin in case we become one */
    thread_router_link_t *neighbour = thread_routing_update_link_margin_internal(thread, sender, link_margin_db);
    if (!neighbour) {
        return -3;
    }

    /* Now check that we're a router - if not, we don't bother with anything further */
    if (!thread_is_router_addr(mac_helper_mac16_address_get(cur))) {
        return 0;
    }

    thread_router_id_t sender_id = thread_router_id_from_addr(sender);
    thread_router_id_t my_router_id = thread_router_id_from_addr(mac_helper_mac16_address_get(cur));

    neighbour->age = is_static ? LINK_AGE_STATIC : 0;

    /* We have to check its quality report for us first - need to have
     * this information before making routing decisions.
     */
    const uint8_t *ptr = route_data;
    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        uint8_t byte;
        bool router_in_mask = bit_test(id_mask, r);

        if (router_in_mask) {
            byte = *ptr++;
        } else {
            byte = (QUALITY_BAD << ROUTE_DATA_OUT_SHIFT) | (QUALITY_BAD << ROUTE_DATA_IN_SHIFT) | THREAD_COST_INFINITE;
        }

        if (r == my_router_id) {
            neighbour->outgoing_quality_known = router_in_mask;
            neighbour->outgoing_quality = (byte & ROUTE_DATA_IN_MASK) >> ROUTE_DATA_IN_SHIFT;
            break;
        }
    }

    /* Compute Thread REED downgrade condition - does this router have as good or better quality links
     * to all Routers for which we have two-way link quality 2 (10dB) or better?
     */
    neighbour->as_good = true;
    ptr = route_data;
    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        uint8_t byte;
        bool router_in_mask = bit_test(id_mask, r);

        if (router_in_mask) {
            byte = *ptr++;
        } else {
            byte = (QUALITY_BAD << ROUTE_DATA_OUT_SHIFT) | (QUALITY_BAD << ROUTE_DATA_IN_SHIFT) | THREAD_COST_INFINITE;
        }

        if (r == sender_id) {
            continue;
        }

        thread_router_link_t *other_neighbour = thread_get_neighbour_router_by_id(&thread->routing, r);
        if (!other_neighbour) {
            continue;
        }

        thread_link_quality_e our_quality_to_other_neighbour = thread_neighbour_router_quality(other_neighbour);
        if (our_quality_to_other_neighbour < QUALITY_10dB) {
            continue;
        }
        thread_link_quality_e neighbours_incoming_quality_to_other_neighbour = (byte & ROUTE_DATA_IN_MASK) >> ROUTE_DATA_IN_SHIFT;
        thread_link_quality_e neighbours_outgoing_quality_to_other_neighbour = (byte & ROUTE_DATA_OUT_MASK) >> ROUTE_DATA_OUT_SHIFT;
        thread_link_quality_e neighbours_quality_to_other_neighbour = thread_quality_combine(neighbours_incoming_quality_to_other_neighbour,
                                                                                             neighbours_outgoing_quality_to_other_neighbour);
        if (neighbours_quality_to_other_neighbour < our_quality_to_other_neighbour) {
            neighbour->as_good = false;
            break;
        }
    }

    /* Now go through and update routes based on its data */
    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        uint8_t byte;

        /* If a router isn't in its ID set (but remains in ours), we treat
         * it as if it's saying "no route"+"quality bad".
         */
        if (bit_test(id_mask, r)) {
            byte = *route_data++;
        } else {
            byte = (QUALITY_BAD << ROUTE_DATA_OUT_SHIFT) | (QUALITY_BAD << ROUTE_DATA_IN_SHIFT) | THREAD_COST_INFINITE;
        }

        /* Only _after_ consuming route data do we skip invalid IDs */
        if (thread->routing.fast_route_table[r] == FAST_ROUTE_INVALID_ID) {
            continue;
        }

        if (r == sender_id || r == my_router_id) {
            continue;
        }

        thread_route_t *route_entry = thread_get_route_entry_by_id(&thread->routing, r);
        thread_route_cost_t cost_reported = byte & ROUTE_DATA_COST_MASK;
        if (cost_reported == 0) {
            /* If sender was our next hop to r, but it no longer has a route */
            if (route_entry && route_entry->next_hop == sender_id) {
                /* Really delete the route? I guess the model in the spec
                 * doesn't leave us with the state info to choose an alternative
                 * immediately, so we do have to wait for more adverts...
                 */
                tr_debug("Delete Route by Cost Report: NH=%d dest=%d", route_entry->next_hop, route_entry->destination);
                ns_list_remove(&thread->routing.route_set, route_entry);
                ns_dyn_mem_free(route_entry);
            }
        } else {
            /* If we have no existing multihop route, or if this is our
             * existing multihop route, or it's better than our existing one */
            if (route_entry == NULL ||
                    route_entry->next_hop == sender_id ||
                    cost_less(thread_link_cost_sum(thread_neighbour_router_cost(neighbour), cost_reported),
                              thread_link_cost_sum(thread_get_neighbour_router_cost(&thread->routing, route_entry->next_hop), route_entry->route_cost))) {
                if (!route_entry) {
                    route_entry = ns_dyn_mem_alloc(sizeof * route_entry);
                    if (!route_entry) {
                        continue;
                    }
                    route_entry->destination = r;
                    ns_list_add_to_end(&thread->routing.route_set, route_entry);
                } else {
                    tr_debug("Update Old %d D, %d NH, %d C", route_entry->destination, route_entry->next_hop, route_entry->route_cost);
                }
                route_entry->next_hop = sender_id;
                route_entry->route_cost = cost_reported;
            }
        }
    }

    thread_update_fast_route_table(thread);

    return 0;
}

static void delete_link(thread_info_t *thread, thread_router_link_t *link)
{
    thread_router_id_t id = link->router_id;
    thread_routing_info_t *routing = &thread->routing;

    tr_debug("delete_link: router: %x", thread_router_addr_from_id(link->router_id));

    /* Remove entry from the link set */
    ns_list_remove(&routing->link_set, link);
    ns_dyn_mem_free(link);

    /* Remove all routing entries for which that link was the next hop */
    ns_list_foreach_safe(thread_route_t, route_entry, &routing->route_set) {
        if (route_entry->next_hop == id) {
            ns_list_remove(&routing->route_set, route_entry);
            ns_dyn_mem_free(route_entry);
        }
    }
    thread_update_fast_route_table(thread);
}

int_fast8_t thread_routing_remove_link(protocol_interface_info_entry_t *cur,
                                       uint16_t sender)
{
    thread_info_t *thread = cur->thread_info;
    /* Sanity check that the source is a Thread router */
    if (!thread || !thread_is_router_addr(sender)) {
        return -2;
    }

    thread_router_id_t sender_id = thread_router_id_from_addr(sender);
    thread_router_link_t *neighbour = thread_get_neighbour_router_by_id(&thread->routing, sender_id);
    if (!neighbour) {
        return -1;
    }

    delete_link(thread, neighbour);

    return 0;
}

uint8_t thread_routing_get_route_data_size(protocol_interface_info_entry_t *cur)
{
    uint_fast8_t len = 0;
    thread_info_t *thread = cur->thread_info;
    if (!thread || !thread->routing.router_id_sequence_valid) {
        return 0;
    }

    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        if (thread->routing.fast_route_table[r] != FAST_ROUTE_INVALID_ID) {
            len++;
        }
    }

    return len;
}

uint_fast8_t thread_routing_cost_get_by_router_id(thread_routing_info_t *routing , uint8_t routerId)
{
    return thread_compute_route_cost(routing, routerId, NULL);
}

int_fast8_t thread_routing_get_route_data(protocol_interface_info_entry_t *cur,
        uint8_t *id_seq,
        uint8_t *id_mask,
        uint8_t *data,
        uint8_t *len_out)
{
    uint8_t len = 0;
    thread_info_t *thread = cur->thread_info;
    if (!thread || !thread->routing.router_id_sequence_valid) {
        return -1;
    }

    uint16_t mac16 = mac_helper_mac16_address_get(cur);

    *id_seq = thread->routing.router_id_sequence;
    memset(id_mask, 0, (N_THREAD_ROUTERS + 7) / 8);

    for (thread_router_id_t r = 0; r < N_THREAD_ROUTERS; r++) {
        uint8_t val;

        if (thread->routing.fast_route_table[r] == FAST_ROUTE_INVALID_ID) {
            continue;
        }

        bit_set(id_mask, r);

        if (thread_router_addr_from_id(r) == mac16) {
            val = ROUTE_DATA_OURSELF;
        } else {
            thread_router_link_t *link = thread_get_neighbour_router_by_id(&thread->routing, r);
            if (link) {
                val = (link->outgoing_quality << ROUTE_DATA_OUT_SHIFT)
                      | (link->incoming_quality << ROUTE_DATA_IN_SHIFT);
            } else {
                val = (QUALITY_BAD << ROUTE_DATA_OUT_SHIFT)
                      | (QUALITY_BAD << ROUTE_DATA_IN_SHIFT);
            }

            val |= thread_compute_route_cost(&thread->routing, r, NULL);
        }

        *data++ = val;
        len++;
    }

    *len_out = len;

    return 0;
}

void thread_routing_init(thread_routing_info_t *routing)
{
    ns_list_init(&routing->route_set);
    ns_list_init(&routing->link_set);
    thread_routing_reset(routing);
}

void thread_routing_reset(thread_routing_info_t *routing)
{
    thread_routing_free(routing);
    routing->router_id_sequence_valid = false;
    routing->networkIdTimeout = NETWORK_ID_TIMEOUT;
    memset(routing->fast_route_table, FAST_ROUTE_INVALID_ID, sizeof routing->fast_route_table);
    trickle_start(&routing->mle_advert_timer, &thread_mle_advert_trickle_params);
}

void thread_routing_free(thread_routing_info_t *routing)
{
    ns_list_foreach_safe(thread_route_t, route_entry, &routing->route_set) {
        ns_list_remove(&routing->route_set, route_entry);
        ns_dyn_mem_free(route_entry);
    }

    ns_list_foreach_safe(thread_router_link_t, link_entry, &routing->link_set) {
        ns_list_remove(&routing->link_set, link_entry);
        ns_dyn_mem_free(link_entry);
    }
}

void thread_routing_activate(thread_routing_info_t *routing)
{
    trickle_inconsistent_heard(&routing->mle_advert_timer, &thread_mle_advert_trickle_params);
    routing->activated = true;
    routing->networkFragmentationTimer = 0;
}

void thread_routing_deactivate(thread_routing_info_t *routing)
{
    thread_routing_reset(routing);
    routing->activated = false;
}

/* ticks is in 100ms units, I think */
/* Return true if we want to send an MLE advertisement */
bool thread_routing_timer(thread_info_t *thread, uint8_t ticks)
{
    thread_routing_info_t *routing = &thread->routing;

    if (!routing->activated) {
        return false;
    }

    ns_list_foreach_safe(thread_router_link_t, link, &routing->link_set) {
        if (link->age == LINK_AGE_STATIC) {
            continue;
        }

        link->age += ticks;
        if (link->age > MAX_LINK_AGE) {
            delete_link(thread, link);
        }
    }

    return trickle_timer(&routing->mle_advert_timer, &thread_mle_advert_trickle_params, ticks);
}

/* This is experimental and is testing if we can improve long topology networks to be more stable without
 * resetting the trickle every time we hear inconsistency.
 * This speeds up next advertisement when connection restored
 *
 * Solution proposal 2:
 * turn on consistency checking, treat leader data changes as inconsistent, and matching leader data
 * as consistent if we've sent an advertisement recently.
 * Needs specification update. Does cause more resetting of trickle.
 *
 */

//
//

static void thread_trickle_accelerate(trickle_t *t, const trickle_params_t *params, uint16_t ticks)
{
    trickle_time_t new_time = t->now + ticks;

    /* Catch overflow */
    if (new_time < t->now) {
        new_time = TRICKLE_TIME_MAX;
    }

    if (new_time < t->t) {
        // We must speed up this so that next trigger happens before this time
        t->now = t->t - new_time;
    }
    if (t->now > t->t) {
        // if now is larger than t move t to trigger event again during this period
        t->t = t->now + randLIB_get_random_in_range(0, params->Imin/2);
    }
}

// This functions speeds up next advertisement depending on the disconnect period to leader
void thread_routing_leader_connection_validate(thread_info_t *thread, uint16_t disconnect_period)
{
    thread_routing_info_t *routing = &thread->routing;

    if (!routing->activated) {
        return;
    }
    //If disconnect period is about 60 seconds make sure next advertisement happens before 70 second mark
    if (disconnect_period < NETWORK_ID_SPEEDUP) {
        return;
    }
    if (disconnect_period > NETWORK_ID_SPEEDUP_MAX ) {
        tr_debug("Leader restored:accelerate reset: %d",disconnect_period);
        trickle_inconsistent_heard(&routing->mle_advert_timer, &thread_mle_advert_trickle_params);
    } else {
        tr_debug("Leader restored:accelerate: %d",disconnect_period);
        thread_trickle_accelerate(&routing->mle_advert_timer, &thread_mle_advert_trickle_params, 100);// 10 second with 100ms tics
    }
}

static bool thread_forwardable_address_fn(const protocol_interface_info_entry_t *cur, addrtype_t addr_type, const uint8_t *address)
{
    (void) cur;
    if (addr_type != ADDR_802_15_4_SHORT) {
        return false;
    }

    if (addr_check_broadcast(address, addr_type) == 0) {
        return false;
    }

    return true;
}

static bool thread_address_map_fn(protocol_interface_info_entry_t *cur, addrtype_t *addr_type, uint8_t *address)
{
    /* Don't touch addresses other than short */
    if (*addr_type != ADDR_802_15_4_SHORT) {
        return true;
    }

    /* Anything with maximum router ID is anycast - others are normal unmapped unicast */
    uint16_t addr16 = common_read_16_bit(address + 2);
    if (thread_router_id_from_addr(addr16) != N_THREAD_ROUTERS - 1) {
        return true;
    }

    /* If the ND mapping function fails, drop the packet */
    if (thread_nd_map_anycast_address(cur, &addr16) < 0) {
        return false;
    }

    /* Update the address */
    common_write_16_bit(addr16, address + 2);
    return true;
}

static bool thread_header_needed_fn(const protocol_interface_info_entry_t *cur, const buffer_t *buf)
{
    /* Never mesh headers on broadcasts/multicasts */
    if (addr_check_broadcast(buf->dst_sa.address, buf->dst_sa.addr_type) == 0) {
        return false;
    }
    /* We have no use for long addresses in mesh headers */
    if (buf->dst_sa.addr_type != ADDR_802_15_4_SHORT || buf->src_sa.addr_type != ADDR_802_15_4_SHORT) {
        return false;
    }

    /* Don't need a mesh header if travelling between a router and its child */
    uint16_t src = common_read_16_bit(buf->src_sa.address + 2);
    uint16_t dst = common_read_16_bit(buf->dst_sa.address + 2);
    if (src == thread_router_addr_from_addr(dst) || dst == thread_router_addr_from_addr(src)) {
        return false;
    }

    /* Don't need a mesh header if we are routing directly to a neighbor router
       Possibly unsafe if routing info changes before it reaches the transmit stage? */
    if (thread_i_am_router(cur) && thread_is_router_addr(dst)) {
        thread_router_id_t id = thread_router_id_from_addr(dst);
        /* Direct routing is indicated by the next hop
           in fast_route_table being the router itself */
        if (cur->thread_info->routing.fast_route_table[id] == id) {
            return false;
        }
    }

    return true;
}


static mesh_callbacks_t thread_routing_mesh_callbacks = {
    .first_hop = thread_first_hop_fn,
    .route = thread_route_fn,
    .header_needed = thread_header_needed_fn,
    .forwardable_address = thread_forwardable_address_fn,
    .address_map = thread_address_map_fn,
};

void thread_routing_set_mesh_callbacks(protocol_interface_info_entry_t *cur)
{
    cur->mesh_callbacks = &thread_routing_mesh_callbacks;
    mesh_all_addresses_unicast(true);
}

/* Return value = number of neighbours with quality 10dB or better ("set M")
 * *as_good     = number of neighbours with as good or better quality links to all routers in set M
 *
 * (Note, these numbers are not calculated at exactly the same time, so could be a slight mismatch...)
 */
uint_fast8_t thread_routing_count_neighbours_for_downgrade(thread_routing_info_t *routing, uint_fast8_t *as_good)
{
    uint_fast8_t linkCnt = 0;
    *as_good = 0;
    thread_link_quality_e linkQuality;
    ns_list_foreach_safe(thread_router_link_t, link, &routing->link_set) {
        linkQuality = thread_neighbour_router_quality(link);
        if (linkQuality >= QUALITY_10dB) {
            linkCnt++;
        }
        if (link->as_good) {
            (*as_good)++;
        }
    }

    return linkCnt;
}

uint_fast8_t thread_routing_count_active_routers(thread_routing_info_t *routing)
{
    uint_fast8_t activeRouterCnt = 0;
    for (thread_router_id_t i = 0; i < N_THREAD_ROUTERS; i++) {

        if (routing->fast_route_table[i] != FAST_ROUTE_INVALID_ID) {
            activeRouterCnt++;
        }
    }

    return activeRouterCnt;
}

uint_fast8_t thread_routing_count_active_routers_from_mask(const uint8_t *id_mask)
{
    uint_fast8_t activeRouterCnt = 0;
    for (uint_fast8_t i = 0; i < 8; i++) {
        activeRouterCnt += common_count_bits(id_mask[i]);
    }
    return activeRouterCnt;
}

#endif //HAVE_THREAD_ROUTER
