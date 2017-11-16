/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include <nsdynmemLIB.h>

#ifndef UINT_FAST4_MAX
typedef uint_fast8_t    uint_fast4_t;
#define UINT_FAST4_MAX  UINT_FAST8_MAX
#endif

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

#define TRACE_GROUP "trou"

/* MLE Route Data bit assignments (draft-kelsey-thread-routing-00) */
#define ROUTE_DATA_OUT_MASK     0xC0
#define ROUTE_DATA_OUT_SHIFT    6
#define ROUTE_DATA_IN_MASK      0x30
#define ROUTE_DATA_IN_SHIFT     4
#define ROUTE_DATA_COST_MASK    0x0F

#define MAX_ROUTE_COST          0x0F

typedef uint_fast4_t thread_route_cost_t;
#define THREAD_COST_INFINITE    0

#define MAX_LINK_AGE 100*10     /* 100 seconds */

#define LINK_AGE_STATIC 0xFFFF  /* Magic number to indicate "never expire" */


bool router_id_sequence_is_greater(const thread_routing_info_t *routing, uint8_t seq)
{
    return false;
}

#define LINK_QUALITY_HYSTERESIS (1 << THREAD_LINK_MARGIN_SCALING)   /* 1dB */

bool thread_i_am_router(const protocol_interface_info_entry_t *cur)
{
    return false;
}

void thread_routing_update_id_set(protocol_interface_info_entry_t *cur, uint8_t seq, const uint8_t *id_mask)
{
}

int_fast8_t thread_routing_update_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t sender,
        uint8_t link_margin_db,
        uint8_t outgoing_link_margin_db)
{
    return 0;
}

int_fast8_t thread_routing_force_link_margin(protocol_interface_info_entry_t *cur,
        uint16_t addr,
        uint8_t link_margin_db)
{
    return 0;
}

int_fast8_t thread_routing_add_link(protocol_interface_info_entry_t *cur,
                                    uint16_t sender, uint8_t link_margin_db,
                                    uint8_t id_seq,
                                    const uint8_t *id_mask,
                                    const uint8_t *route_data,
                                    bool is_static)
{
    return 0;
}

int_fast8_t thread_routing_remove_link(protocol_interface_info_entry_t *cur,
                                       uint16_t sender)
{
    return 0;
}

uint8_t thread_routing_get_route_data_size(protocol_interface_info_entry_t *cur)
{
    return 0;
}

uint_fast8_t thread_routing_cost_get_by_router_id(thread_routing_info_t *routing , uint8_t routerId)
{
    return 0;
}

int_fast8_t thread_routing_get_route_data(protocol_interface_info_entry_t *cur,
        uint8_t *id_seq,
        uint8_t *id_mask,
        uint8_t *data,
        uint8_t *len_out)
{
    return 0;
}

void thread_routing_init(thread_routing_info_t *routing)
{
}

void thread_routing_free(thread_routing_info_t *routing)
{
}

void thread_routing_activate(thread_routing_info_t *routing)
{
}

void thread_routing_deactivate(thread_routing_info_t *routing)
{
}

/* ticks is in 100ms units, I think */
/* Return true if we want to send an MLE advertisement */
bool thread_routing_timer(thread_info_t *thread, uint8_t ticks)
{
    return false;
}

void thread_routing_leader_connection_validate(struct thread_info_s *thread, uint16_t disconnect_period)
{
}

void thread_routing_set_mesh_callbacks(protocol_interface_info_entry_t *cur)
{
}

uint_fast8_t thread_routing_count_neighbours_with_quality_or_better(thread_routing_info_t *routing, thread_link_quality_e quality)
{
    return 0;
}

uint_fast8_t thread_routing_count_active_routers(thread_routing_info_t *routing)
{
    return 0;
}

uint_fast8_t thread_routing_count_active_routers_from_mask(const uint8_t *id_mask)
{
    return 0;
}

void thread_routing_reset(thread_routing_info_t *routing)
{

}
