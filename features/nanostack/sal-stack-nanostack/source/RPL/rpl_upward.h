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

#ifndef RPL_UPWARD_H_
#define RPL_UPWARD_H_

#include "Common_Protocols/icmpv6_prefix.h"
#include "rpl_control.h"

/* Structures are abstract for outer control layer */
typedef struct rpl_neighbour rpl_neighbour_t;
typedef struct rpl_dodag_version rpl_dodag_version_t;
typedef struct rpl_dodag rpl_dodag_t;
typedef struct rpl_instance rpl_instance_t;

/* Internal flags used in prefix descriptors - disjoint from real L/A/R flags */
#define RPL_PIO_PUBLISHED   0x10
#define RPL_PIO_AGE         0x08
#define RPL_PIO_HOLD_MASK   0x07

/* Comparison functions return one of these values - as they are bits, you
 * can test for "greater-than-or-equal" by "if (cmp & (RPL_CMP_GREATER|RPL_CMP_EQUAL))".
 */
#define RPL_CMP_LESS      0x1
#define RPL_CMP_EQUAL     0x2
#define RPL_CMP_GREATER   0x4
#define RPL_CMP_UNORDERED 0x8
typedef uint_fast8_t rpl_cmp_t;

uint8_t rpl_seq_init(void);
uint8_t rpl_seq_inc(uint8_t seq);
rpl_cmp_t rpl_seq_compare(uint8_t a, uint8_t b);

uint16_t nrpl_dag_rank(const rpl_dodag_t *dodag, uint16_t rank);
rpl_cmp_t rpl_rank_compare(const rpl_dodag_t *dodag, uint16_t a, uint16_t b);
rpl_cmp_t rpl_rank_compare_dagrank_rank(const rpl_dodag_t *dodag, uint16_t dag_rank_a, uint16_t b);
uint16_t rpl_rank_add(uint16_t a, uint16_t b);
uint16_t rpl_rank_sub(uint16_t a, uint16_t b);
uint16_t rpl_rank_next_level(const rpl_dodag_t *dodag, uint16_t a);
uint16_t rpl_rank_max_at_level(const rpl_dodag_t *dodag, uint16_t a);

/* Internal APIs */
void rpl_upward_dio_timer(rpl_instance_t *instance, uint16_t ticks);

rpl_instance_t *rpl_lookup_instance(const rpl_domain_t *domain, uint8_t instance_id, const uint8_t *addr);
rpl_instance_t *rpl_create_instance(rpl_domain_t *domain, uint8_t instance_id);
void rpl_delete_instance(rpl_instance_t *instance);
bool rpl_instance_purge(rpl_instance_t *instance);
void rpl_instance_set_dodag_version(rpl_instance_t *instance, rpl_dodag_version_t *version, uint16_t rank);
rpl_dodag_t *rpl_instance_current_dodag(const rpl_instance_t *instance);
#ifdef HAVE_RPL_ROOT
bool rpl_instance_am_root(const rpl_instance_t *instance);
#else
#define rpl_instance_am_root(instance) false
#endif
uint8_t rpl_instance_mop(const rpl_instance_t *instance);
rpl_dodag_version_t *rpl_instance_current_dodag_version(const rpl_instance_t *instance);
rpl_neighbour_t *rpl_instance_preferred_parent(const rpl_instance_t *instance);
rpl_dodag_version_t *rpl_instance_predicate_match(rpl_instance_t *instance, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, uint8_t version_num);
void rpl_instance_inconsistency(rpl_instance_t *instance);
void rpl_instance_consistent_rx(rpl_instance_t *instance);
void rpl_instance_increment_dtsn(rpl_instance_t *instance);
void rpl_dodag_set_pref(rpl_dodag_t *dodag, uint8_t pref);
void rpl_instance_poison(rpl_instance_t *instance, uint8_t count);
void rpl_instance_force_leaf(rpl_instance_t *instance);
void rpl_instance_trigger_parent_selection(rpl_instance_t *instance, uint16_t delay);
void rpl_instance_remove_interface(rpl_instance_t *instance, int8_t if_id);
void rpl_instance_dio_trigger(rpl_instance_t *instance, struct protocol_interface_info_entry *cur, const uint8_t *addr);
void rpl_instance_set_local_repair(rpl_instance_t *instance, bool repair);
bool rpl_instance_local_repair(const rpl_instance_t *instance);
uint16_t rpl_instance_current_rank(const rpl_instance_t *instance);
void rpl_instance_slow_timer(rpl_instance_t *instance, uint16_t seconds);

rpl_dodag_t *rpl_lookup_dodag(const rpl_instance_t *instance, const uint8_t *dodagid);
rpl_dodag_t *rpl_create_dodag(rpl_instance_t *instance, const uint8_t *dodagid, uint8_t g_mop_prf);
void rpl_delete_dodag(rpl_dodag_t *dodag);
uint8_t rpl_dodag_mop(const rpl_dodag_t *dodag);
void rpl_dodag_set_root(rpl_dodag_t *dodag, bool root);
#ifdef HAVE_RPL_ROOT
bool rpl_dodag_am_root(const rpl_dodag_t *dodag);
#else
#define rpl_dodag_am_root(dodag) false
#endif
uint8_t rpl_dodag_get_version_number_as_root(const rpl_dodag_t *dodag);
void rpl_dodag_set_version_number_as_root(rpl_dodag_t *dodag, uint8_t number);
void rpl_dodag_set_leaf(rpl_dodag_t *dodag, bool leaf);
bool rpl_dodag_am_leaf(const rpl_dodag_t *dodag);
const rpl_dodag_conf_t *rpl_dodag_get_config(const rpl_dodag_t *dodag);
void rpl_dodag_inconsistency(rpl_dodag_t *dodag);
void rpl_dodag_increment_dtsn(rpl_dodag_t *dodag);
rpl_cmp_t rpl_dodag_pref_compare(const rpl_dodag_t *a, const rpl_dodag_t *b);
void rpl_dodag_update_implicit_system_routes(rpl_dodag_t *dodag, rpl_neighbour_t *parent);

rpl_dio_route_t *rpl_dodag_update_dio_route(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age);
void rpl_dodag_delete_dio_route(rpl_dodag_t *dodag, rpl_dio_route_t *route);
const rpl_dio_route_list_t *rpl_dodag_get_route_list(const rpl_dodag_t *dodag);

prefix_entry_t *rpl_dodag_update_dio_prefix(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool publish, bool age);
void rpl_dodag_delete_dio_prefix(rpl_dodag_t *dodag, prefix_entry_t *prefix);
const prefix_list_t *rpl_dodag_get_prefix_list(const rpl_dodag_t *dodag);

rpl_dodag_version_t *rpl_lookup_dodag_version(const rpl_dodag_t *dodag, uint8_t version_num);
rpl_dodag_version_t *rpl_create_dodag_version(rpl_dodag_t *dodag, uint8_t version_num);
void rpl_delete_dodag_version(rpl_dodag_version_t *version);
bool rpl_dodag_version_possible_subdodag(const rpl_dodag_version_t *version, uint16_t rank);
bool rpl_dodag_version_is_current(const rpl_dodag_version_t *version);
rpl_cmp_t rpl_dodag_version_compare(const rpl_dodag_version_t *a, const rpl_dodag_version_t *b);
void rpl_dodag_version_limit_greediness(rpl_dodag_version_t *version, uint16_t rank);
void rpl_dodag_version_raise_greediness(rpl_dodag_version_t *version, uint16_t pref_rank);
bool rpl_dodag_version_rank_indicates_possible_sub_dodag(const rpl_dodag_version_t *version, uint16_t rank);

rpl_neighbour_t *rpl_lookup_neighbour_by_ll_address(const rpl_instance_t *instance, const uint8_t *addr, int8_t if_id);
rpl_neighbour_t *rpl_create_neighbour(rpl_dodag_version_t *instance, const uint8_t *ll_addr, int8_t if_id, uint8_t g_mop_prf, uint8_t dtsn);
void rpl_delete_neighbour(rpl_instance_t *instance, rpl_neighbour_t *neighbour);
bool rpl_dodag_update_config(rpl_dodag_t *dodag, const rpl_dodag_conf_t *conf, const uint8_t *src, bool *become_leaf);
const uint8_t *rpl_neighbour_ll_address(const rpl_neighbour_t *neighbour);
const uint8_t *rpl_neighbour_global_address(const rpl_neighbour_t *neighbour);
void rpl_neighbour_update_global_address(rpl_neighbour_t *neighbour, const uint8_t *addr);
void rpl_neighbour_update_dodag_version(rpl_neighbour_t *neighbour, rpl_dodag_version_t *version, uint16_t rank, uint8_t g_mop_prf);
bool rpl_neighbour_update_dtsn(rpl_neighbour_t *neighbour, uint8_t dtsn);
rpl_instance_t *rpl_neighbour_instance(const rpl_neighbour_t *neighbour);

void rpl_instance_neighbours_changed(rpl_instance_t *instance);
void rpl_instance_run_parent_selection(rpl_instance_t *instance);

void rpl_upward_print_instance(rpl_instance_t *instance, route_print_fn_t *print_fn);

bool rpl_upward_read_dodag_info(const rpl_instance_t *instance, struct rpl_dodag_info_t *dodag_info);
uint16_t rpl_upward_read_dao_target_list_size(const rpl_instance_t *instance);
#endif /* RPL_UPWARD_H_ */
