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
#ifndef THREAD_ROUTING_H_
#define THREAD_ROUTING_H_

#include "NWK_INTERFACE/Include/protocol.h"
#include <ns_list.h>
#ifndef UINT_FAST12_MAX
typedef uint_fast16_t   uint_fast12_t;
#define UINT_FAST12_MAX UINT_FAST16_MAX
#endif

#include "Service_Libs/Trickle/trickle.h"

struct thread_info_s;

#define MAX_ROUTER_ID           62
#define N_THREAD_ROUTERS        64
#define THREAD_ROUTER_ID_BITS   6
#define THREAD_ROUTER_MASK      0xFC00
#define THREAD_ROUTER_SHIFT     10
#define THREAD_CHILD_MASK       0x03FF
#define THREAD_ROUTER_IDX       0

typedef uint_fast8_t thread_router_id_t;

#define THREAD_QUALITY_BITS     2
#define THREAD_COST_BITS        4
#define THREAD_COST_INFINITE    0
#define THREAD_MAX_ROUTE_COST   15

/* Scaled link margin */
#define THREAD_LINK_MARGIN_BITS     12
#define THREAD_LINK_MARGIN_SCALING  3   /* For 1/8 exponential moving average weighting */

#if THREAD_LINK_MARGIN_BITS == 12
typedef uint_fast12_t thread_link_margin_t;
#define THREAD_LINK_MARGIN_MAX UINT_FAST12_MAX
#endif

/* I want to use uint_fast4_t - create it if it doesn't exist */
#ifndef UINT_FAST4_MAX
typedef uint_fast8_t    uint_fast4_t;
#define UINT_FAST4_MAX  UINT_FAST8_MAX
#endif
typedef uint_fast4_t thread_route_cost_t;

typedef enum {
    QUALITY_20dB = 3,
    QUALITY_10dB = 2,
    QUALITY_2dB = 1,
    QUALITY_BAD = 0,
} thread_link_quality_e;

/* The draft-kelsey-thread-routing-00 Link Set */
/* XXX - probably don't need this - put it into mle_neigh_table? */
typedef struct thread_router_link_s {
    unsigned router_id          : THREAD_ROUTER_ID_BITS;
    unsigned incoming_quality   : THREAD_QUALITY_BITS;
    unsigned outgoing_quality   : THREAD_QUALITY_BITS;
    unsigned link_margin        : THREAD_LINK_MARGIN_BITS;
    unsigned age                : 12; /* 100ms ticks, so good for ~2 hours */
    bool outgoing_quality_known : 1;
    bool as_good                : 1;
    ns_list_link_t link;
} thread_router_link_t;

/* The draft-kelsey-thread-routing-00 Route Set */
typedef struct thread_route_s {
    unsigned destination        : THREAD_ROUTER_ID_BITS;
    unsigned next_hop           : THREAD_ROUTER_ID_BITS;
    unsigned route_cost         : THREAD_COST_BITS;     /* Cost according to next_hop - doesn't include our cost to next_hop */
    ns_list_link_t link;
} thread_route_t;

/* Fast routing table - also serves as the draft-kelsey-thread-routing-00 Router ID Set
 * Entries currently squeeze into 1 byte, making fast_route_table a 64-byte array
 *
 * "INVALID_ID" or not is always live info, as that's the master ID Set
 * storage. If ID is valid, the route decision is a cache generated from
 * route_set and link_set, and it needs to be kept up-to-date with
 * thread_update_fast_route_table() when those change.
 */
#define FAST_ROUTE_INVALID_ID   0xFF
#define FAST_ROUTE_NO_ROUTE     0xFE
#if N_THREAD_ROUTERS > 0xFD
#error "rework fast_route_table"
#endif

typedef struct thread_routing_info_s {
    NS_LIST_HEAD(thread_router_link_t, link) link_set;
    NS_LIST_HEAD(thread_route_t, link) route_set;
    uint_least8_t fast_route_table[N_THREAD_ROUTERS];
    bool activated;
    bool router_id_sequence_valid;
    uint8_t router_id_sequence;
    uint16_t networkFragmentationTimer;
    uint16_t networkIdTimeout; //!< timeout in seconds
    trickle_t mle_advert_timer;
}
thread_routing_info_t;

#ifdef HAVE_THREAD

/* See ns_types.h for explanation */
NS_INLINE thread_router_id_t thread_router_id_from_addr(uint16_t addr);
NS_INLINE uint16_t thread_router_addr_from_id(thread_router_id_t addr);
NS_INLINE uint16_t thread_router_addr_from_addr(uint16_t addr);
NS_INLINE bool thread_is_router_addr(uint16_t addr);
NS_INLINE bool thread_addr_is_child(uint16_t a, uint16_t b);
NS_INLINE bool thread_addr_is_equal_or_child(uint16_t a, uint16_t b);

#if defined NS_ALLOW_INLINING || defined THREAD_ROUTING_FN
#ifndef THREAD_ROUTING_FN
#define THREAD_ROUTING_FN NS_INLINE
#endif
THREAD_ROUTING_FN thread_router_id_t thread_router_id_from_addr(uint16_t addr)
{
    return addr >> THREAD_ROUTER_SHIFT;
}

THREAD_ROUTING_FN uint16_t thread_router_addr_from_id(thread_router_id_t addr)
{
    return (uint_fast16_t) addr << THREAD_ROUTER_SHIFT;
}

THREAD_ROUTING_FN uint16_t thread_router_addr_from_addr(uint16_t addr)
{
    return (addr & THREAD_ROUTER_MASK) | THREAD_ROUTER_IDX;
}

THREAD_ROUTING_FN bool thread_is_router_addr(uint16_t addr)
{
    return (addr & ~ THREAD_ROUTER_MASK) == THREAD_ROUTER_IDX;
}

/* Return true if b is a child of a */
THREAD_ROUTING_FN bool thread_addr_is_child(uint16_t a, uint16_t b)
{
    if (thread_is_router_addr(b)) {
        return false;
    }
    return thread_router_addr_from_addr(b) == a;
}

/* Return true if b is equal to a, or b is a child of a */
THREAD_ROUTING_FN bool thread_addr_is_equal_or_child(uint16_t a, uint16_t b)
{
    return b == a || thread_addr_is_child(a, b);
}
#endif /* NS_ALLOW_INLINING || defined THREAD_ROUTING_FN */
#endif // HAVE_THREAD

#ifdef HAVE_THREAD_ROUTER

bool thread_i_am_router(const protocol_interface_info_entry_t *cur);

void thread_routing_init(thread_routing_info_t *routing);
void thread_routing_reset(thread_routing_info_t *routing);
void thread_routing_free(thread_routing_info_t *routing);
void thread_routing_activate(thread_routing_info_t *routing);
void thread_routing_deactivate(thread_routing_info_t *routing);
bool thread_routing_timer(struct thread_info_s *thread, uint8_t ticks);
void thread_routing_leader_connection_validate(struct thread_info_s *thread, uint16_t disconnect_period);
void thread_routing_set_mesh_callbacks(protocol_interface_info_entry_t *cur);

uint_fast8_t thread_routing_cost_get_by_router_id(thread_routing_info_t *routing , uint8_t routerId);
bool router_id_sequence_is_greater(const thread_routing_info_t *routing, uint8_t seq);

uint_fast8_t thread_routing_count_neighbours_for_downgrade(thread_routing_info_t *routing, uint_fast8_t *as_good);
uint_fast8_t thread_routing_count_active_routers(thread_routing_info_t *routing);
uint_fast8_t thread_routing_count_active_routers_from_mask(const uint8_t *id_mask);

//Init
void thread_routing_update_id_set(protocol_interface_info_entry_t *cur, uint8_t seq, const uint8_t *id_mask);
void thread_routing_force_next_hop(protocol_interface_info_entry_t *cur, uint8_t id_seq, const uint8_t *id_mask, thread_router_id_t next_hop_id);

int_fast8_t thread_routing_update_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t sender,
        uint8_t link_margin_db,
        uint8_t outgoing_link_margin_db);
int_fast8_t thread_routing_force_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t addr,
        uint8_t link_margin_db);
int_fast8_t thread_routing_add_link(protocol_interface_info_entry_t *cur,
                                    uint16_t sender, uint8_t link_margin_db,
                                    uint8_t id_seq,
                                    const uint8_t *id_mask,
                                    const uint8_t *route_data,
                                    bool is_static);
int_fast8_t thread_routing_remove_link(protocol_interface_info_entry_t *cur,
                                       uint16_t sender);

uint8_t thread_routing_get_route_data_size(protocol_interface_info_entry_t *cur);

int_fast8_t thread_routing_get_route_data(protocol_interface_info_entry_t *cur,
        uint8_t *id_seq,
        uint8_t *id_mask,
        uint8_t *data,
        uint8_t *len_out);
#else // HAVE_THREAD_ROUTER

#define thread_i_am_router(cur) false
#define thread_routing_init(routing)
#define thread_routing_reset(routing)
#define thread_routing_free(routing)
#define thread_routing_activate(routing)
#define thread_routing_deactivate(routing)
#define thread_routing_timer(thread, ticks) false
#define thread_routing_leader_connection_validate(thread, disconnect_period)
#define thread_routing_set_mesh_callbacks(cur)
#define thread_routing_cost_get_by_router_id(routing, routerId) (0)
#define thread_routing_count_active_routers(routing) 0
#define thread_routing_count_active_routers_from_mask(id_mask) 0
#define thread_routing_update_id_set(cur, seq, id_mask)
#define thread_routing_update_link_margin(cur, sender, link_margin_db, outgoing_link_margin_db) (-3)
#define thread_routing_force_link_margin(cur, addr, link_margin_db) (-3)
#define thread_routing_add_link(cur, sender, link_margin_db, id_seq, id_mask, route_data, is_static) (-2)
#define thread_is_router_addr(addr) false
#define thread_routing_remove_link(cur, sender) 0
#define thread_routing_get_route_data_size(cur) 0
#define thread_routing_get_route_data(cur,id_seq,id_mask,data,len_out) (0)
#define thread_addr_is_equal_or_child(a, b) false
#define thread_addr_is_child(a, b) false

#endif // HAVE_THREAD_ROUTER

#endif /* THREAD_ROUTING_H_ */
