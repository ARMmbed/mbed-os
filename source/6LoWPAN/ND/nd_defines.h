/*
 * Copyright (c) 2014-2017, 2019, Pelion and affiliates.
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

#ifndef ND_DEFINES_H_
#define ND_DEFINES_H_

#include "Core/include/ns_address_internal.h"
#include "ns_list.h"
#include "6LoWPAN/IPHC_Decode/lowpan_context.h"
#include "Common_Protocols/icmpv6_prefix.h"

typedef enum {
    ND_READY = 0,
    ND_BR_READY = 1,
    ND_RS_UNCAST = 2,
    ND_RS_MULTICAST = 3,
} nd_obj_state;

#define nd_is_ready_state(state) ((state) == ND_READY || (state) == ND_BR_READY)
#define nd_is_bootstrap_state(state) (!nd_is_ready_state(state))

typedef enum {
    ND_ADDRESS_PRIMARY = 0,
    ND_ADDRESS_SECONDARY = 1,
} nd_multi_address_type;

typedef struct nd_router_next_hop {
    addrtype_t addrtype;
    uint8_t address[8];
    uint8_t LQI;
} nd_router_next_hop;

// This structure conflates a few things, confusingly. It's basically
// a "per-border-router"/"per-ABRO" (RFC 6775) structure.
// This is sensible for a 6LR, which has to store prefixes and contexts
// per-ABRO for relaying, and needs to time them out, but it's not the same
// structure needed as a host (or for the host part of the router). For a host,
// contexts and prefixes and addresses are needed per-interface, and they
// shouldn't time out just because an ABRO timed out.
// Also, because of this structure, we always end up requiring ABROs as a host,
// which isn't really necessary.
// This also isn't the RFC 4861 "Default Router List", alas.
// Note also that this is used both to describe a remote border router, and
// ourselves as a border router, with some confusing effects on lifetimes
// (we're in danger of timing ourselves out as a border router)
typedef struct nd_router {
    nwk_interface_id nwk_id;
    uint8_t border_router[16];
    uint8_t flags;
    uint16_t life_time;
    uint16_t nd_re_validate;
    nd_obj_state nd_state;
    uint16_t ns_forward_timer;
    uint16_t nd_timer;
    uint16_t mle_advert_timer;
    uint8_t mle_purge_timer;
    uint8_t nd_bootstrap_tick;
    uint8_t ns_retry;
    uint32_t abro_version_num;
    bool trig_address_reg;
    ipv6_ra_timing_t ra_timing;
    prefix_list_t prefix_list;
    lowpan_context_list_t context_list;
    nd_router_next_hop default_hop;
    nd_router_next_hop *secondaty_hop;

    ns_list_link_t link;

} nd_router_t;

/* XXX why isn't this a substructure of nd_router_t? or share one */
typedef struct nd_router_setup {
    uint16_t life_time;
    uint32_t abro_version_num;
    prefix_list_t prefix_list;
    lowpan_context_list_t context_list;
} nd_router_setup_t;


#endif /* ND_DEFINES_H_ */
