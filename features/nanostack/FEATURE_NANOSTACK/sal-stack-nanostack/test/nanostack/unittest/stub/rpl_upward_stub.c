/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/* rpl_upward.c deals with management of the DODAG and upward routes within an
 * instance.
 *
 * rpl_domain_t is accessible, but not normally manipulated - all routines in
 * this file works on a specific instance.
 *
 * rpl_instance_t, rpl_dodag_t, rpl_dodag_version_t, rpl_neighbour_t are all accessible.
 */

#include "nsconfig.h"

#ifdef HAVE_RPL
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include <string.h>

#include "net_interface.h"

#include "Core/include/address.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Service_Libs/Trickle/trickle.h"
#include "ipv6_stack/ipv6_routing_table.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "Common_Protocols/ipv6_resolution.h"

#include "RPL/rpl_protocol.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_control.h"
#include "RPL/rpl_objective.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_structures.h"

#include "net_rpl.h"

#define TRACE_GROUP "rplu"

/* Rank comparison, and DAGRank(rank) */
uint16_t nrpl_dag_rank(const rpl_dodag_t *dodag, uint16_t rank)
{
    return 0;
}

/* Silly function needed because RPL HbH option includes dagrank directly */
rpl_cmp_t rpl_rank_compare_dagrank_rank(const rpl_dodag_t *dodag, uint16_t dag_rank_a, uint16_t b)
{
    return 0;
}

rpl_cmp_t rpl_rank_compare(const rpl_dodag_t *dodag, uint16_t a, uint16_t b)
{
    return 0;
}

/* Given a Rank, round up to the next higher integral rank */
uint16_t rpl_rank_next_level(const rpl_dodag_t *dodag, uint16_t a)
{
    return 0;
}

/* Given a Rank, return the maximum Rank with equal DAGRank */
uint16_t rpl_rank_max_at_level(const rpl_dodag_t *dodag, uint16_t a)
{
    return 0;
}

/* Add two ranks, checking for overflow */
uint16_t rpl_rank_add(uint16_t a, uint16_t b)
{
    return 0;
}

/* Subtract two ranks, checking for overflow */
uint16_t rpl_rank_sub(uint16_t a, uint16_t b)
{
    return 0;
}

uint8_t rpl_seq_init(void)
{
    return 0;
}

uint8_t rpl_seq_inc(uint8_t seq)
{
    return 0;
}

rpl_cmp_t rpl_seq_compare(uint8_t a, uint8_t b)
{
    return RPL_CMP_EQUAL;
}

void rpl_instance_set_dodag_version(rpl_instance_t *instance, rpl_dodag_version_t *version, uint16_t rank)
{
}

rpl_dodag_version_t *rpl_instance_current_dodag_version(const rpl_instance_t *instance)
{
    return NULL;
}

rpl_dodag_t *rpl_instance_current_dodag(const rpl_instance_t *instance)
{
    return NULL;
}

bool rpl_instance_am_root(const rpl_instance_t *instance)
{
    return false;
}

uint8_t rpl_instance_mop(const rpl_instance_t *instance)
{
    return 0;
}

rpl_neighbour_t *rpl_instance_preferred_parent(const rpl_instance_t *instance)
{
    return NULL;
}

rpl_dodag_version_t *rpl_instance_predicate_match(rpl_instance_t *instance, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, uint8_t version_num)
{
    return NULL;
}

void rpl_instance_inconsistency(rpl_instance_t *instance)
{
}

void rpl_instance_consistent_rx(rpl_instance_t *instance)
{
}

void rpl_instance_increment_dtsn(rpl_instance_t *instance)
{
}

void rpl_instance_poison(rpl_instance_t *instance, uint8_t count)
{
}

void rpl_instance_force_leaf(rpl_instance_t *instance)
{
}

void rpl_instance_trigger_parent_selection(rpl_instance_t *instance, uint16_t delay)
{
}

rpl_dodag_t *rpl_lookup_dodag(const rpl_instance_t *instance, const uint8_t *dodagid)
{
    return NULL;
}

rpl_dodag_version_t *rpl_lookup_dodag_version(const rpl_dodag_t *dodag, uint8_t version_num)
{
    return NULL;
}

rpl_dodag_version_t *rpl_lookup_dodag_and_version(const rpl_instance_t *instance, const uint8_t *dodagid, uint8_t version_num)
{
    return NULL;
}

rpl_neighbour_t *rpl_lookup_neighbour_by_ll_address(const rpl_instance_t *instance, const uint8_t *addr, int8_t if_id)
{
    return NULL;
}

rpl_neighbour_t *rpl_create_neighbour(rpl_dodag_version_t *version, const uint8_t *addr, int8_t if_id, uint8_t g_mop_prf, uint8_t dtsn)
{
    return NULL;
}

void rpl_delete_neighbour(rpl_instance_t *instance, rpl_neighbour_t *neighbour)
{
}

const uint8_t *rpl_neighbour_ll_address(const rpl_neighbour_t *neighbour)
{
    return NULL;
}

const uint8_t *rpl_neighbour_global_address(const rpl_neighbour_t *neighbour)
{
    return NULL;
}

void rpl_neighbour_update_global_address(rpl_neighbour_t *neighbour, const uint8_t *addr)
{
}

void rpl_neighbour_update_dodag_version(rpl_neighbour_t *neighbour, rpl_dodag_version_t *version, uint16_t rank, uint8_t g_mop_prf)
{
}

bool rpl_neighbour_update_dtsn(rpl_neighbour_t *neighbour, uint8_t dtsn)
{
    return false;
}

rpl_instance_t *rpl_neighbour_instance(const rpl_neighbour_t *neighbour)
{
    return NULL;
}

rpl_dodag_version_t *rpl_create_dodag_version(rpl_dodag_t *dodag, uint8_t version_num)
{
    return NULL;
}

void rpl_delete_dodag_version(rpl_dodag_version_t *version)
{
}

bool rpl_dodag_version_is_current(const rpl_dodag_version_t *version)
{
    return false;
}

bool rpl_dodag_version_rank_indicates_possible_sub_dodag(const rpl_dodag_version_t *version, uint16_t rank)
{
    return false;
}

rpl_cmp_t rpl_dodag_version_compare(const rpl_dodag_version_t *a, const rpl_dodag_version_t *b)
{
    return RPL_CMP_UNORDERED;
}

void rpl_dodag_version_limit_greediness(rpl_dodag_version_t *version, uint16_t rank)
{
}

void rpl_dodag_version_raise_greediness(rpl_dodag_version_t *version, uint16_t pref_rank)
{
}

rpl_dodag_t *rpl_create_dodag(rpl_instance_t *instance, const uint8_t *dodagid, uint8_t g_mop_prf)
{
    return NULL;
}

void rpl_delete_dodag(rpl_dodag_t *dodag)
{
}

uint8_t rpl_dodag_mop(const rpl_dodag_t *dodag)
{
    return 0;
}

bool rpl_dodag_update_config(rpl_dodag_t *dodag, const rpl_dodag_conf_t *conf, const uint8_t *src, bool *become_leaf)
{
    return false;
}

/* If we're currently a member of this DODAG, kick the DIO timers */
void rpl_dodag_inconsistency(rpl_dodag_t *dodag)
{
}

void rpl_dodag_increment_dtsn(rpl_dodag_t *dodag)
{
}

void rpl_dodag_set_pref(rpl_dodag_t *dodag, uint8_t pref)
{
}

rpl_cmp_t rpl_dodag_pref_compare(const rpl_dodag_t *a, const rpl_dodag_t *b)
{
    return RPL_CMP_LESS;
}

void rpl_dodag_set_root(rpl_dodag_t *dodag, bool root)
{
}

bool rpl_dodag_am_root(const rpl_dodag_t *dodag)
{
}

void rpl_dodag_set_leaf(rpl_dodag_t *dodag, bool leaf)
{

}

bool rpl_dodag_is_current(const rpl_dodag_t *dodag)
{
    return false;
}

const rpl_dodag_conf_t *rpl_dodag_get_config(const rpl_dodag_t *dodag)
{
    return NULL;
}

uint8_t rpl_dodag_get_version_number_as_root(const rpl_dodag_t *dodag)
{
    return 0;
}

void rpl_dodag_set_version_number_as_root(rpl_dodag_t *dodag, uint8_t number)
{
}

const rpl_dio_route_list_t *rpl_dodag_get_route_list(const rpl_dodag_t *dodag)
{
    return NULL;
}

rpl_dio_route_t *rpl_dodag_update_dio_route(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age)
{
    return NULL;
}

void rpl_dodag_delete_dio_route(rpl_dodag_t *dodag, rpl_dio_route_t *route)
{
}

const prefix_list_t *rpl_dodag_get_prefix_list(const rpl_dodag_t *dodag)
{
    return NULL;
}

prefix_entry_t *rpl_dodag_update_dio_prefix(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool publish, bool age)
{
    return NULL;
}

void rpl_dodag_delete_dio_prefix(rpl_dodag_t *dodag, prefix_entry_t *prefix)
{
}

rpl_instance_t *rpl_lookup_instance(const rpl_domain_t *domain, uint8_t instance_id, const uint8_t *addr)
{
    return NULL;
}

rpl_instance_t *rpl_create_instance(rpl_domain_t *domain, uint8_t instance_id)
{
    return NULL;
}

void rpl_delete_instance(rpl_instance_t *instance)
{
}

/* Purge one item from an instance */
bool rpl_instance_purge(rpl_instance_t *instance)
{
    return false;
}

void rpl_instance_neighbours_changed(rpl_instance_t *instance)
{
}

/* Called when a DIO has been received */
void rpl_dodag_update_implicit_system_routes(rpl_dodag_t *dodag, rpl_neighbour_t *parent)
{
}

void rpl_instance_run_parent_selection(rpl_instance_t *instance)
{
}

void rpl_instance_remove_interface(rpl_instance_t *instance, int8_t if_id)
{
}

/* Trigger DIO transmission - all interfaces multicast if addr+cur are NULL, else unicast */
void rpl_instance_dio_trigger(rpl_instance_t *instance, protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
}

void rpl_instance_set_local_repair(rpl_instance_t *instance, bool repair)
{
}

bool rpl_instance_local_repair(const rpl_instance_t *instance)
{
    return false;
}

uint16_t rpl_instance_current_rank(const rpl_instance_t *instance)
{
    return 0;
}

void rpl_instance_slow_timer(rpl_instance_t *instance, uint16_t seconds)
{
}

void rpl_upward_dio_timer(rpl_instance_t *instance, uint16_t ticks)
{
}

void rpl_upward_print_neighbour(const rpl_neighbour_t *neighbour, route_print_fn_t *print_fn)
{
}

void rpl_upward_print_neighbours_in_version(const rpl_neighbour_list_t *list, const rpl_dodag_version_t *version, route_print_fn_t *print_fn)
{
}

void rpl_upward_print_dodag(rpl_instance_t *instance, rpl_dodag_t *dodag, route_print_fn_t *print_fn)
{
}

void rpl_upward_print_instance(rpl_instance_t *instance, route_print_fn_t *print_fn)
{
}

/* Backwards-compatibility implementation of net_rpl.h API designed for old implementation */
bool rpl_upward_read_dodag_info(const rpl_instance_t *instance, rpl_dodag_info_t *dodag_info)
{
    return false;
}

uint16_t rpl_upward_read_dao_target_list_size(const rpl_instance_t *instance)
{
    return 0;
}

#endif /* HAVE_RPL */
