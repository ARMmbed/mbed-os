/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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

#ifdef HAVE_RPL

#include <string.h>
#include "common_functions.h"
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"

#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/ipv6_routing_table.h"

#include "net_rpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_data.h"
#include "RPL/rpl_structures.h"

#define TRACE_GROUP "RPLd"

void rpl_downward_convert_dodag_preferences_to_dao_path_control(rpl_dodag_t *dodag)
{
}

void rpl_downward_neighbour_gone(rpl_instance_t *instance, rpl_neighbour_t *neighbour)
{
}

void rpl_downward_process_dao_parent_changes(rpl_instance_t *instance)
{
}

rpl_dao_target_t *rpl_create_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len, bool root)
{
    return NULL;
}

void rpl_delete_dao_target(rpl_instance_t *instance, rpl_dao_target_t *target)
{
}

rpl_dao_target_t *rpl_instance_lookup_published_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    return NULL;
}

rpl_dao_target_t *rpl_instance_lookup_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    return NULL;
}

rpl_dao_target_t *rpl_instance_match_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    return NULL;
}

void rpl_instance_delete_published_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
}

void rpl_instance_publish_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len, uint32_t valid_lifetime, bool own, bool want_descriptor, uint32_t descriptor)
{
}

void rpl_instance_dao_trigger(rpl_instance_t *instance, uint16_t delay)
{
}

void rpl_instance_send_dao_update(rpl_instance_t *instance)
{
}

rpl_dao_target_t *rpl_downward_delete_root_transit(rpl_dao_target_t *target, rpl_dao_root_transit_t *transit)
{
    return NULL;
}

void rpl_downward_transit_error(rpl_instance_t *instance, const uint8_t *target_addr, const uint8_t *transit_addr)
{
}

void rpl_downward_compute_paths(rpl_instance_t *instance)
{
}

void rpl_downward_paths_invalidate(rpl_instance_t *instance)
{
}

bool rpl_instance_dao_received(rpl_instance_t *instance, const uint8_t src[16], int8_t interface_id, bool multicast, const uint8_t *opts, uint16_t opts_len, uint8_t *status)
{
    return false;
}


void rpl_instance_dao_acked(rpl_instance_t *instance, const uint8_t src[16], int8_t interface_id, uint8_t dao_sequence, uint8_t status)
{
}

void rpl_instance_dao_request(struct rpl_instance *instance, struct rpl_neighbour *neighbour)
{
}


void rpl_downward_dao_slow_timer(rpl_instance_t *instance, uint16_t seconds)
{
}

void rpl_downward_dao_timer(rpl_instance_t *instance, uint16_t ticks)
{
}

void rpl_downward_print_instance(rpl_instance_t *instance, route_print_fn_t *print_fn)
{
}

#endif /* HAVE_RPL */
