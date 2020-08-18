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
#include "ip6string.h"

#include "net_interface.h"

#include "Core/include/ns_address_internal.h"
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

/* How many times to transmit/retransmit a zero-lifetime route */
#define RPL_MAX_FINAL_RTR_ADVERTISEMENTS 3

#define RPL_DEFAULT_DIO_INTERVAL_MIN 3
#define RPL_DEFAULT_DIO_INTERVAL_DOUBLINGS 20

#define RPL_DEFAULT_IMIN_TICKS (((1ull << RPL_DEFAULT_DIO_INTERVAL_MIN) + 99) / 100)
#define RPL_DEFAULT_IMAX_TICKS (((1ull << (RPL_DEFAULT_DIO_INTERVAL_MIN+RPL_DEFAULT_DIO_INTERVAL_DOUBLINGS)) + 99) / 100)

static NS_LIST_DEFINE(rpl_candidate_neighbour_set, rpl_neighbour_t, candidate_neighbour_link);

static void rpl_instance_remove_parents(rpl_instance_t *instance);
static void rpl_instance_remove_system_routes_through_parent(rpl_instance_t *instance, rpl_neighbour_t *parent);
static void rpl_dodag_update_system_route(rpl_dodag_t *dodag, rpl_dio_route_t *route);
static rpl_neighbour_t *rpl_instance_choose_worst_neighbour(const rpl_instance_t *instance);
static uint32_t rpl_dio_imax_time_calculate(uint16_t Imax, uint16_t fixed_point);

/* Rank comparison, and DAGRank(rank) */
uint16_t nrpl_dag_rank(const rpl_dodag_t *dodag, uint16_t rank)
{
    return rank == RPL_RANK_INFINITE ? rank : rank / dodag->config.min_hop_rank_increase;
}

uint16_t nrpl_rank(const rpl_dodag_t *dodag, uint16_t dag_rank)
{
    uint32_t rank = (uint32_t) dag_rank * dodag->config.min_hop_rank_increase;
    return rank < RPL_RANK_INFINITE ? rank : RPL_RANK_INFINITE;
}

/* Silly function needed because RPL HbH option includes dagrank directly */
rpl_cmp_t rpl_rank_compare_dagrank_rank(const rpl_dodag_t *dodag, uint16_t dag_rank_a, uint16_t b)
{
    /* Special infinity handling makes sure we're absolutely solid, but callers
     * do need to then consider the (faint) possibility of unordered.
     */
    if (dag_rank_a == RPL_RANK_INFINITE) {
        return b == RPL_RANK_INFINITE ? RPL_CMP_UNORDERED : RPL_CMP_GREATER;
    } else if (b == RPL_RANK_INFINITE) {
        return RPL_CMP_LESS;
    }

    if (!dodag) {
        return RPL_CMP_UNORDERED;
    }

    uint16_t dag_rank_b = nrpl_dag_rank(dodag, b);
    if (dag_rank_a < dag_rank_b) {
        return RPL_CMP_LESS;
    } else if (dag_rank_a == dag_rank_b) {
        return RPL_CMP_EQUAL;
    } else {
        return RPL_CMP_GREATER;
    }
}

rpl_cmp_t rpl_rank_compare(const rpl_dodag_t *dodag, uint16_t a, uint16_t b)
{
    uint16_t dag_rank_a = nrpl_dag_rank(dodag, a);
    return rpl_rank_compare_dagrank_rank(dodag, dag_rank_a, b);
}

/* Given a Rank, round up to the next higher integral rank */
uint16_t rpl_rank_next_level(const rpl_dodag_t *dodag, uint16_t a)
{
    uint16_t r = dodag->config.min_hop_rank_increase * (1 + nrpl_dag_rank(dodag, a));
    return r >= a ? r : RPL_RANK_INFINITE;
}

/* Given a Rank, return the maximum Rank with equal DAGRank */
uint16_t rpl_rank_max_at_level(const rpl_dodag_t *dodag, uint16_t a)
{
    uint16_t r = dodag->config.min_hop_rank_increase * (1 + nrpl_dag_rank(dodag, a)) - 1;
    return r >= a ? r : RPL_RANK_INFINITE;
}

/* Add two ranks, checking for overflow */
uint16_t rpl_rank_add(uint16_t a, uint16_t b)
{
    uint16_t r = a + b;
    return r >= a ? r : RPL_RANK_INFINITE;
}

/* Subtract two ranks, checking for overflow */
uint16_t rpl_rank_sub(uint16_t a, uint16_t b)
{
    uint16_t r = a - b;
    return r <= a ? r : 0;
}

/* Sequence counter operations (RFC 6550 S7.2) */
#define RPL_SEQUENCE_WINDOW 16

uint8_t rpl_seq_init(void)
{
    return 256 - RPL_SEQUENCE_WINDOW;
}

uint8_t rpl_seq_inc(uint8_t seq)
{
    return seq == 127 ? 0 : (uint8_t)(seq + 1);
}

rpl_cmp_t rpl_seq_compare(uint8_t a, uint8_t b)
{
    if (a == b) {
        return RPL_CMP_EQUAL;
    } else if (a >= 128 && b < 128) {
        if (256 + b - a <= RPL_SEQUENCE_WINDOW) {
            return RPL_CMP_LESS;
        } else {
            return RPL_CMP_GREATER;
        }
    } else if (b >= 128 && a < 128) {
        if (256 + a - b <= RPL_SEQUENCE_WINDOW) {
            return RPL_CMP_GREATER;
        } else {
            return RPL_CMP_LESS;
        }
    } else if (a < 128) { /* so both <= 127 */
        /* RFC 6550 description is wrong/misleading ("If the absolute magnitude
         * of difference between the two sequence counters is less than or
         * equal to SEQUENCE_WINDOW...)". This doesn't cover the wrap
         * at the end of the circular region - the difference has to
         * be computed modulo-128..
         */
        uint8_t diff = (a - b) & 127;
        if (diff <= RPL_SEQUENCE_WINDOW) {
            return RPL_CMP_GREATER;
        } else if (diff >= 128 - RPL_SEQUENCE_WINDOW) {
            return RPL_CMP_LESS;
        } else {
            return RPL_CMP_UNORDERED;
        }
    } else { /* both >= 128 */
        /* In this case, there's no wrap, so absolute difference being bigger
         * than SEQUENCE_WINDOW is unordered, as the RFC says (230 could be
         * newer than 250 due to reboot, or could be old).
         */
        uint8_t abs_diff = a > b ? a - b : b - a;
        if (abs_diff > RPL_SEQUENCE_WINDOW) {
            return RPL_CMP_UNORDERED;
        } else if (a > b) {
            return RPL_CMP_GREATER;
        } else {
            return RPL_CMP_LESS;
        }
    }
}

void rpl_instance_set_dodag_version(rpl_instance_t *instance, rpl_dodag_version_t *version, uint16_t rank)
{
    if (!version || rpl_dodag_am_leaf(version->dodag)) {
        if (!version) {
            tr_debug("No version -> set RPL_RANK_INFINITE");
        } else {
            tr_debug("Leaf -> set RPL_RANK_INFINITE");
        }
        rank = RPL_RANK_INFINITE;
    }

    instance->current_rank = rank;

    rpl_dodag_version_t *old_version = instance->current_dodag_version;
    if (old_version == version) {
        return;
    }

    instance->current_dodag_version = version;
    if (version) {
        version->dodag->used = true;
        if (version->dodag->root) {
            rpl_instance_remove_parents(instance);
        }

        /* Need to call trickle_start somewhere (to avoid uninitialised variables) - this is it */
        if (!old_version || old_version->dodag != version->dodag) {
            trickle_start(&instance->dio_timer, &version->dodag->dio_timer_params);
        }
    }

    /* Then changing dodag version is an inconsistency. We may be changing from non-NULL to NULL, in which case we use old parameters to do poison */
    trickle_inconsistent_heard(&instance->dio_timer, version ? &version->dodag->dio_timer_params : &old_version->dodag->dio_timer_params);
}

rpl_dodag_version_t *rpl_instance_current_dodag_version(const rpl_instance_t *instance)
{
    return instance->current_dodag_version;
}

rpl_dodag_t *rpl_instance_current_dodag(const rpl_instance_t *instance)
{
    return instance->current_dodag_version ? instance->current_dodag_version->dodag : NULL;
}

#ifdef HAVE_RPL_ROOT
bool rpl_instance_am_root(const rpl_instance_t *instance)
{
    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    return dodag ? rpl_dodag_am_root(dodag) : false;
}
#endif

uint8_t rpl_instance_mop(const rpl_instance_t *instance)
{
    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    /* MOP is supposed to be the same for all DODAGs, so take any */
    if (!dodag) {
        dodag = ns_list_get_first(&instance->dodags);
    }
    return dodag ? rpl_dodag_mop(dodag) : RPL_MODE_NO_DOWNWARD;
}

rpl_neighbour_t *rpl_instance_preferred_parent(const rpl_instance_t *instance)
{
    /* DODAG parents are first in the neighbour list, and preferred is first.
     * So if we have a preferred parent, it's the first entry, and its
     * dodag_parent (or was_dodag_parent) flag is set.
     */
    rpl_neighbour_t *neighbour = ns_list_get_first(&instance->candidate_neighbours);
    if (!neighbour || (!neighbour->dodag_parent && !neighbour->was_dodag_parent)) {
        return NULL;
    }
    return neighbour;
}

uint16_t rpl_instance_candidate_rank(const rpl_neighbour_t *candidate)
{
    return candidate->rank;
}

bool rpl_instance_possible_better_candidate(const rpl_instance_t *instance, rpl_neighbour_t *replacing, uint16_t candidate_rank, uint16_t etx)
{
    return instance->of->possible_better_candidate(instance, replacing, candidate_rank, etx);
}

/* If we're a member of a DODAG Version matching the predicate in this instance,
 * return it. Mainly used for handling DODAG Information Solicitations.
 */
rpl_dodag_version_t *rpl_instance_predicate_match(rpl_instance_t *instance, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, uint8_t version_num)
{
    rpl_dodag_version_t *dodag_version = instance->current_dodag_version;
    if (!dodag_version) {
        return NULL;
    }
    if ((pred & RPL_SOLINFO_PRED_INSTANCEID) && (instance->id != instance_id)) {
        return NULL;
    }
    if ((pred & RPL_SOLINFO_PRED_DODAGID) && !addr_ipv6_equal(dodag_version->dodag->id, dodagid)) {
        return NULL;
    }
    if ((pred & RPL_SOLINFO_PRED_VERSION) && dodag_version->number != version_num) {
        return NULL;
    }
    return dodag_version;
}

void rpl_instance_inconsistency(rpl_instance_t *instance)
{
    if (instance->current_dodag_version) {
        trickle_inconsistent_heard(&instance->dio_timer, &instance->current_dodag_version->dodag->dio_timer_params);
    }
}

void rpl_instance_consistent_rx(rpl_instance_t *instance)
{
    if (!instance->dio_not_consistent) {
        trickle_consistent_heard(&instance->dio_timer);
    }
}

void rpl_instance_increment_dtsn(rpl_instance_t *instance)
{
    instance->dtsn = rpl_seq_inc(instance->dtsn);
    instance->last_dao_trigger_time = protocol_core_monotonic_time;
    instance->srh_error_count = 0;
    /* Should a DTSN increment trigger DIOs, thus? */
    rpl_instance_inconsistency(instance);
}

void rpl_instance_poison(rpl_instance_t *instance, uint8_t count)
{
    if (instance->poison_count < count) {
        instance->poison_count = count;
    }
    rpl_instance_inconsistency(instance);
}

void rpl_instance_force_leaf(rpl_instance_t *instance)
{
    instance->current_rank = RPL_RANK_INFINITE;
}

void rpl_instance_trigger_parent_selection(rpl_instance_t *instance, uint16_t delay, rpl_dodag_t *dodag)
{
    /* When "improving", let us have a minimum trigger time based on Imin, for large networks */
    if (dodag) {
        //Convert imin 100ms tick to seconds
        uint16_t i_min_delay = dodag->dio_timer_params.Imin / 10;
        if (i_min_delay > delay) {
            delay = i_min_delay;
        }
    }
    if (instance->parent_selection_timer == 0 || instance->parent_selection_timer > delay) {
        instance->parent_selection_timer = randLIB_randomise_base(delay, 0x8000, 0x999A) /* Random between delay * 1.0-1.2 */;
        tr_debug("Timed parent triggered %u", instance->parent_selection_timer);
    }
}

static void rpl_instance_parent_selection_timer(rpl_instance_t *instance, uint16_t seconds)
{


    if (instance->parent_selection_timer > seconds) {
        instance->parent_selection_timer -= seconds;
    } else if (instance->parent_selection_timer != 0) {
        tr_debug("Timed parent selection");
        rpl_instance_run_parent_selection(instance);
    }
}

rpl_dodag_t *rpl_lookup_dodag(const rpl_instance_t *instance, const uint8_t *dodagid)
{
    ns_list_foreach(rpl_dodag_t, dodag, &instance->dodags) {
        if (addr_ipv6_equal(dodag->id, dodagid)) {
            dodag->timestamp = protocol_core_monotonic_time;
            return dodag;
        }
    }
    return NULL;
}

rpl_dodag_version_t *rpl_lookup_dodag_version(const rpl_dodag_t *dodag, uint8_t version_num)
{
    ns_list_foreach(rpl_dodag_version_t, dodag_version, &dodag->versions) {
        if (dodag_version->number == version_num) {
            return dodag_version;
        }
    }
    return NULL;
}

rpl_neighbour_t *rpl_lookup_neighbour_by_ll_address(const rpl_instance_t *instance, const uint8_t *addr, int8_t if_id)
{
    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->interface_id == if_id && addr_ipv6_equal(neighbour->ll_address, addr)) {
            return neighbour;
        }
    }
    return NULL;
}

rpl_neighbour_t *rpl_lookup_last_candidate_from_list(const rpl_instance_t *instance)
{
    rpl_neighbour_t *neighbour = rpl_instance_choose_worst_neighbour(instance);
    if (neighbour && neighbour->considered && !neighbour->dodag_parent) {
        return neighbour;
    }
    return NULL;
}

rpl_neighbour_t *rpl_create_neighbour(rpl_dodag_version_t *version, const uint8_t *addr, int8_t if_id, uint8_t g_mop_prf, uint8_t dtsn)
{
    /* Should gate higher-rank neighbours here - ignore higher-rank neighbours
     * unless in some sort of local repair.
     */

    rpl_neighbour_t *neighbour = rpl_alloc(sizeof(rpl_neighbour_t));
    if (!neighbour) {
        return NULL;
    }

    rpl_instance_t *instance = version->dodag->instance;
    neighbour->dodag_version = version;
    memcpy(neighbour->ll_address, addr, 16);
    memset(neighbour->global_address, 0, 16);
    neighbour->have_global_address = false;
    neighbour->dodag_parent = neighbour->was_dodag_parent = false;
    neighbour->considered = false;
    neighbour->interface_id = if_id;
    neighbour->rank = 0xffff;
    neighbour->g_mop_prf = g_mop_prf;
    neighbour->dtsn = dtsn;
    neighbour->dao_path_control = 0;
    neighbour->confirmed = 0;
    neighbour->addr_reg_failures = 0;

    /* Need to limit number of neighbours here - chucking worst neighbour */

    /* Neighbour list wants parents at the front of the list; slot new
     * people in after the parents. Then old neighbours get pushed to the end.
     */
    ns_list_foreach(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        if (!n->dodag_parent) {
            ns_list_add_before(&instance->candidate_neighbours, n, neighbour);
            return neighbour;
        }
    }
    ns_list_add_to_end(&instance->candidate_neighbours, neighbour);
    return neighbour;
}

void rpl_delete_neighbour(rpl_instance_t *instance, rpl_neighbour_t *neighbour)
{
    rpl_dao_target_t *dao_target = rpl_instance_get_active_target_confirmation(instance);
    if (dao_target && instance->wait_response == neighbour) {
        instance->wait_response = NULL;
    }

    rpl_downward_neighbour_gone(instance, neighbour);
    ns_list_remove(&instance->candidate_neighbours, neighbour);
    if (neighbour->dao_path_control) {
    }
    if (neighbour->dodag_parent) {
        rpl_instance_remove_system_routes_through_parent(instance, neighbour);
        rpl_instance_neighbours_changed(instance, NULL);
    }

    rpl_free(neighbour, sizeof * neighbour);
}

const uint8_t *rpl_neighbour_ll_address(const rpl_neighbour_t *neighbour)
{
    return neighbour->ll_address;
}

const uint8_t *rpl_neighbour_global_address(const rpl_neighbour_t *neighbour)
{
    return neighbour->have_global_address ? neighbour->global_address : NULL;
}

void rpl_neighbour_update_global_address(rpl_neighbour_t *neighbour, const uint8_t *addr)
{
    if (!addr_ipv6_equal(neighbour->global_address, addr)) {
        memcpy(neighbour->global_address, addr, 16);
        neighbour->have_global_address = true;
        //neighbour->down_change = true;
    }
}

void rpl_neighbour_update_dodag_version(rpl_neighbour_t *neighbour, rpl_dodag_version_t *version, uint16_t rank, uint8_t g_mop_prf)
{
    /* DODAG follows G/MOP/Prf of preferred parent if it isn't moving */
    if (g_mop_prf != version->dodag->g_mop_prf &&
            (rpl_dodag_version_compare(version, neighbour->dodag_version) & (RPL_CMP_GREATER | RPL_CMP_EQUAL)) &&
            neighbour == rpl_instance_preferred_parent(version->dodag->instance)) {
        version->dodag->g_mop_prf = g_mop_prf;
        rpl_dodag_inconsistency(version->dodag);
    }
    neighbour->g_mop_prf = g_mop_prf;
    neighbour->dodag_version = version;
    neighbour->rank = rank;
    neighbour->dio_timestamp = protocol_core_monotonic_time;
}

bool rpl_neighbour_update_dtsn(rpl_neighbour_t *neighbour, uint8_t dtsn)
{
    uint8_t old_dtsn = neighbour->dtsn;

    neighbour->dtsn = dtsn;

    return neighbour->dodag_parent && (rpl_seq_compare(dtsn, old_dtsn) & RPL_CMP_GREATER);
}

rpl_instance_t *rpl_neighbour_instance(const rpl_neighbour_t *neighbour)
{
    return neighbour->dodag_version->dodag->instance;
}

rpl_dodag_version_t *rpl_create_dodag_version(rpl_dodag_t *dodag, uint8_t version_num)
{
    rpl_dodag_version_t *version = rpl_alloc(sizeof(rpl_dodag_version_t));
    if (!version) {
        return NULL;
    }

    version->dodag = dodag;
    version->number = version_num;
    version->last_advertised_rank = RPL_RANK_INFINITE;
    version->lowest_advertised_rank = RPL_RANK_INFINITE;
    version->greediness_rank_limit = RPL_RANK_INFINITE;
    version->hard_rank_limit = RPL_RANK_INFINITE;

    if (!ns_list_is_empty(&dodag->versions)) {
        protocol_stats_update(STATS_RPL_GLOBAL_REPAIR, 1);
    }

    /* Maintain the version list newest first, taking care on ordering */
    bool inserted = false;
    ns_list_foreach_safe(rpl_dodag_version_t, v, &dodag->versions) {
        rpl_cmp_t cmp = rpl_seq_compare(version_num, v->number);
        if (cmp & (RPL_CMP_GREATER | RPL_CMP_UNORDERED)) {
            /* "Unordered" is treated as newest (as per RFC 6550 7.2 rule 4?) */
            ns_list_add_before(&dodag->versions, v, version);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        ns_list_add_to_end(&dodag->versions, version);
    }

    /* Now a clean-up to guarantee we have a completely comparable list.
     * Starting from the newest, check every other element is "less" than the
     * newest. If we hit one that isn't, chuck that, and all subsequent ones.
     */
    rpl_dodag_version_t *newest = ns_list_get_first(&dodag->versions);
    for (rpl_dodag_version_t *v = ns_list_get_next(&dodag->versions, newest); v; v = ns_list_get_next(&dodag->versions, v)) {
        if (rpl_seq_compare(v->number, newest->number) & RPL_CMP_LESS) {
            continue;
        } else {
            do {
                rpl_dodag_version_t *next = ns_list_get_next(&dodag->versions, v);
                rpl_delete_dodag_version(v);
                v = next;
            } while (v);
            break;
        }
    }



    return version;
}

void rpl_delete_dodag_version(rpl_dodag_version_t *version)
{
    rpl_dodag_t *dodag = version->dodag;
    rpl_instance_t *instance = dodag->instance;

    if (instance->current_dodag_version == version) {
        // Don't call rpl_instance_set_dodag_version(NULL) - that would pre-empt parent reselection,
        // triggering poison immediately.
        // Give parent selection a chance to select another version (but will it have any info on-hand?)
        instance->current_dodag_version = NULL;
        rpl_instance_trigger_parent_selection(instance, 5, NULL);
    }

    ns_list_foreach_safe(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->dodag_version == version) {
            rpl_delete_neighbour(instance, neighbour);
        }
    }
    ns_list_remove(&dodag->versions, version);
    rpl_free(version, sizeof(*version));
}

bool rpl_dodag_version_is_current(const rpl_dodag_version_t *version)
{
    return version->dodag->instance->current_dodag_version == version;
}

bool rpl_dodag_version_rank_indicates_possible_sub_dodag(const rpl_dodag_version_t *version, uint16_t rank)
{
    if (!rpl_dodag_version_is_current(version)) {
        return false;
    }
    rpl_cmp_t cmp = rpl_rank_compare(version->dodag, rank, version->dodag->instance->current_rank);
    return cmp & RPL_CMP_GREATER;
}

rpl_cmp_t rpl_dodag_version_compare(const rpl_dodag_version_t *a, const rpl_dodag_version_t *b)
{
    if (a == NULL || b == NULL) {
        return RPL_CMP_UNORDERED;
    }
    if (a == b) {
        return RPL_CMP_EQUAL;
    }
    if (a->dodag != b->dodag) {
        return RPL_CMP_UNORDERED;
    }
    return rpl_seq_compare(a->number, b->number);
}

void rpl_dodag_version_limit_greediness(rpl_dodag_version_t *version, uint16_t rank)
{
    /* Apply RPL greediness limit rule - after we've joined a DODAG version,
     * we don't increase rank unless required to by an existing parent.
     */
    if (rank != RPL_RANK_INFINITE && version->greediness_rank_limit == RPL_RANK_INFINITE) {
        version->greediness_rank_limit = rpl_rank_max_at_level(version->dodag, rank);
    }
}

void rpl_dodag_version_raise_greediness(rpl_dodag_version_t *version, uint16_t pref_rank)
{
    /* This can be called during parent selection, after preferred parent is chosen,
     * to potentially increase the greediness limit considering the new parent circumstance.
     * Eg, if our initial and current Rank was 1, our greediness limit would have
     * been 1.99. But if we've just had to increase Rank to 2 for an existing
     * parent, then we can raise the limit.
     */
    if (version->greediness_rank_limit < pref_rank) {
        version->greediness_rank_limit = rpl_rank_max_at_level(version->dodag, pref_rank);
    }
}

rpl_dodag_t *rpl_create_dodag(rpl_instance_t *instance, const uint8_t *dodagid, uint8_t g_mop_prf)
{
    rpl_dodag_t *dodag = rpl_alloc(sizeof(rpl_dodag_t));
    if (!dodag) {
        return NULL;
    }

    dodag->instance = instance;
    dodag->timestamp = protocol_core_monotonic_time;
    memcpy(dodag->id, dodagid, 16);
    dodag->leaf = false;
    dodag->root = false;
    dodag->was_root = false;
    dodag->have_config = false;
    dodag->used = false;
    dodag->g_mop_prf = g_mop_prf;
    dodag->new_config_advertisment_count = 0;
    // Default timer parameters and trickle start should never normally
    // be used - we would set the parameters from the DODAG Config and start
    // as we join a version. But initialising here catches odd cases where
    // we end up sending poison DIOs before we get any config.
    dodag->dio_timer_params.Imin = RPL_DEFAULT_IMIN_TICKS;
    dodag->dio_timer_params.Imax = (trickle_time_t)(RPL_DEFAULT_IMAX_TICKS < TRICKLE_TIME_MAX ? RPL_DEFAULT_IMAX_TICKS : TRICKLE_TIME_MAX);
    dodag->dio_timer_params.k = 10;
    trickle_start(&instance->dio_timer, &dodag->dio_timer_params);
    ns_list_init(&dodag->versions);
    ns_list_init(&dodag->routes);
    ns_list_init(&dodag->prefixes);

    ns_list_add_to_start(&instance->dodags, dodag);
    return dodag;
}

void rpl_delete_dodag(rpl_dodag_t *dodag)
{
    rpl_instance_t *instance = dodag->instance;
    ns_list_foreach_safe(rpl_dodag_version_t, version, &dodag->versions) {
        rpl_delete_dodag_version(version);
    }
    ns_list_foreach_safe(rpl_dio_route_t, route, &dodag->routes) {
        rpl_dodag_delete_dio_route(dodag, route);
    }
    ns_list_foreach_safe(prefix_entry_t, prefix, &dodag->prefixes) {
        rpl_dodag_delete_dio_prefix(dodag, prefix);
    }
    ns_list_remove(&instance->dodags, dodag);
    rpl_free(dodag, sizeof(*dodag));
}

void rpl_delete_dodag_root(rpl_dodag_t *dodag)
{
    // This should trigger immediate poison
    rpl_instance_set_dodag_version(dodag->instance, NULL, RPL_RANK_INFINITE);
    // Retain DODAG version info and just unset root flag
    // We have was_root still set which will drop adverts for this dodag.
    dodag->root = false;
}

/* Convert RPL configuration to generic trickle parameters. Returns true if
 * the value in the generic object has changed.
 */
static bool rpl_dodag_conf_convert_trickle_parameters(trickle_params_t *params_out, const rpl_dodag_conf_t *conf)
{
    /* Convert trickle parameters into 100ms ticks */
    uint32_t Imin_ms = conf->dio_interval_min < 32 ? (1ul << conf->dio_interval_min) : 0xfffffffful;
    uint32_t Imin_ticks = (Imin_ms + 99) / 100;
    uint32_t Imax_ms = (conf->dio_interval_min + conf->dio_interval_doublings) < 32 ?
                       (1ul << (conf->dio_interval_min + conf->dio_interval_doublings)) : 0xfffffffful;
    uint32_t Imax_ticks = (Imax_ms + 99) / 100;
    trickle_time_t Imin = (trickle_time_t)(Imin_ticks <= TRICKLE_TIME_MAX ?  Imin_ticks : TRICKLE_TIME_MAX);
    trickle_time_t Imax = (trickle_time_t)(Imax_ticks <= TRICKLE_TIME_MAX ?  Imax_ticks : TRICKLE_TIME_MAX);
    uint8_t k = conf->dio_redundancy_constant;
    if (params_out->Imin != Imin || params_out->Imax != Imax || params_out->k != k) {
        params_out->Imin = Imin;
        params_out->Imax = Imax;
        params_out->k = k;
        params_out->TimerExpirations = TRICKLE_EXPIRATIONS_INFINITE;
        return true;
    } else {
        return false;
    }
}

uint8_t rpl_dodag_mop(const rpl_dodag_t *dodag)
{
    return dodag->g_mop_prf & RPL_MODE_MASK;
}

bool rpl_dodag_update_config(rpl_dodag_t *dodag, const rpl_dodag_conf_t *conf, const uint8_t *src, bool *become_leaf)
{
    /* If already have config, don't update unless it's coming from preferred parent */
    if (dodag->have_config) {
        rpl_neighbour_t *parent = rpl_instance_preferred_parent(dodag->instance);
        if (parent && src && !addr_ipv6_equal(src, parent->ll_address)) {
            return true;
        }
    }
    dodag->config = *conf;
    bool restart_timer = rpl_dodag_conf_convert_trickle_parameters(&dodag->dio_timer_params, conf);
    dodag->have_config = true;
    if (restart_timer && rpl_instance_current_dodag(dodag->instance) == dodag) {
        /* They've changed the timing parameters for our currently-in-use trickle timer! */
        tr_warn("Trickle parameters changed");
        trickle_start(&dodag->instance->dio_timer, &dodag->dio_timer_params);
        dodag->new_config_advertisment_count = 0;
    }
    dodag->instance->of = rpl_objective_lookup(conf->objective_code_point);
    /* We could be a leaf of an unknown OCP. Still need an OF to choose parents */
    if (!dodag->instance->of) {
        dodag->instance->of = rpl_objective_lookup(RPL_OCP_OF0);
        if (!dodag->instance->of) {
            return false;
        }
        if (become_leaf) {
            *become_leaf = true;
        }
    }
    return true;
}

/* If we're currently a member of this DODAG, kick the DIO timers */
void rpl_dodag_inconsistency(rpl_dodag_t *dodag)
{
    if (rpl_instance_current_dodag(dodag->instance) == dodag) {
        trickle_inconsistent_heard(&dodag->instance->dio_timer, &dodag->dio_timer_params);
    }
}

void rpl_dodag_increment_dtsn(rpl_dodag_t *dodag)
{
    if (rpl_instance_current_dodag(dodag->instance) == dodag) {
        rpl_instance_increment_dtsn(dodag->instance);
    }
}

void rpl_dodag_set_pref(rpl_dodag_t *dodag, uint8_t pref)
{
    dodag->g_mop_prf &= ~RPL_DODAG_PREF_MASK;
    dodag->g_mop_prf |= pref;
}

rpl_cmp_t rpl_dodag_pref_compare(const rpl_dodag_t *a, const rpl_dodag_t *b)
{
    uint8_t pref_a, pref_b;
    pref_a = a->g_mop_prf & RPL_DODAG_PREF_MASK;
    pref_b = b->g_mop_prf & RPL_DODAG_PREF_MASK;
    if (pref_a == pref_b) {
        return RPL_CMP_EQUAL;
    } else if (pref_a < pref_b) {
        return RPL_CMP_LESS;
    } else {
        return RPL_CMP_GREATER;
    }
}

void rpl_dodag_set_root(rpl_dodag_t *dodag, bool root)
{
    if (root != dodag->root) {
        dodag->root = root;
        if (root) {
            rpl_instance_remove_parents(dodag->instance);
            dodag->was_root = true;
        } else {
            rpl_instance_run_parent_selection(dodag->instance);
        }
    }
}

#ifdef HAVE_RPL_ROOT
bool rpl_dodag_am_root(const rpl_dodag_t *dodag)
{
    return dodag->root;
}

bool rpl_dodag_was_root(const rpl_dodag_t *dodag)
{
    return dodag->was_root;
}
#endif

void rpl_dodag_set_leaf(rpl_dodag_t *dodag, bool leaf)
{
    dodag->leaf = leaf;
}

bool rpl_dodag_am_leaf(const rpl_dodag_t *dodag)
{
    return dodag->leaf || dodag->instance->domain->force_leaf;
}

bool rpl_dodag_is_current(const rpl_dodag_t *dodag)
{
    return dodag->instance->current_dodag_version && dodag->instance->current_dodag_version->dodag == dodag;
}

const rpl_dodag_conf_t *rpl_dodag_get_config(const rpl_dodag_t *dodag)
{
    return dodag->have_config ? &dodag->config : NULL;
}

uint8_t rpl_dodag_get_version_number_as_root(const rpl_dodag_t *dodag)
{
    return dodag->instance->current_dodag_version->number;
}

void rpl_dodag_set_version_number_as_root(rpl_dodag_t *dodag, uint8_t number)
{
    /* As root, unlike other nodes, we don't need to worry about maintaining
     * multiple rpl_dodag_version_t to track neighbours in each version and
     * our ranks in each version. We'll just quietly change the version number
     * of our only, always-current version.
     */
    rpl_dodag_version_t *version = dodag->instance->current_dodag_version;
    version->number = number;
    rpl_dodag_inconsistency(dodag);
#if defined FEA_TRACE_SUPPORT && defined EXTRA_CONSISTENCY_CHECKS
    /* Sanity check that the above assertion is true - as root we shouldn't have
     * any neighbours referring to this version.
     */
    ns_list_foreach(rpl_neighbour_t, n, &dodag->instance->candidate_neighbours) {
        if (n->dodag_version == version) {
            tr_err("Root DODAG version had neighbour");
            break;
        }
    }
#endif
    protocol_stats_update(STATS_RPL_GLOBAL_REPAIR, 1);
}

static bool rpl_dodag_in_use(const rpl_dodag_t *dodag)
{
    if (dodag->root) {
        return true;
    }

    if (!dodag->have_config) {
        return false;
    }

    rpl_instance_t *instance = dodag->instance;
    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->dodag_version->dodag == dodag) {
            return true;
        }
    }

    return false;
}

const rpl_dio_route_list_t *rpl_dodag_get_route_list(const rpl_dodag_t *dodag)
{
    return &dodag->routes;
}

rpl_dio_route_t *rpl_dodag_update_dio_route(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age)
{
    rpl_dio_route_t *route = NULL;
    bool update = false;

    /* First look for matching prefix */
    ns_list_foreach(rpl_dio_route_t, r, &dodag->routes) {
        if (r->prefix_len == prefix_len && bitsequal(r->prefix, prefix, prefix_len)) {
            route = r;
            break;
        }
    }

    /* If not found, create a new one */
    if (!route) {
        uint_fast8_t prefix_bytes = (prefix_len + 7u) / 8u;
        route = rpl_alloc(sizeof(rpl_dio_route_t) + prefix_bytes);
        if (!route) {
            return NULL;
        }

        route->prefix_len = prefix_len;
        bitcopy0(route->prefix, prefix, prefix_len);
        ns_list_add_to_end(&dodag->routes, route);
        update = true;
    } else {
        if (lifetime != route->lifetime || route->flags != flags) {
            update = true;
        }
    }

    /* Update other info */
    route->lifetime = lifetime;
    route->hold_count = lifetime ? 0 : RPL_MAX_FINAL_RTR_ADVERTISEMENTS;
    route->flags = flags;
    route->age = age;
    if (update) {
        rpl_dodag_update_system_route(dodag, route);
    }
    return route;
}

void rpl_dodag_delete_dio_route(rpl_dodag_t *dodag, rpl_dio_route_t *route)
{
    ns_list_remove(&dodag->routes, route);
    uint_fast8_t prefix_bytes = (route->prefix_len + 7u) / 8u;
    rpl_free(route, sizeof * route + prefix_bytes);
}

static void rpl_dodag_age_routes(rpl_dodag_t *dodag, uint16_t seconds)
{
    ns_list_foreach_safe(rpl_dio_route_t, route, &dodag->routes) {
        if (route->age && route->lifetime != 0xFFFFFFFF) {
            if (route->lifetime > seconds) {
                route->lifetime -= seconds;
            } else {
                route->lifetime = 0;
                if (route->hold_count == 0) {
                    rpl_dodag_delete_dio_route(dodag, route);
                }
            }
        }
    }
}

const prefix_list_t *rpl_dodag_get_prefix_list(const rpl_dodag_t *dodag)
{
    return &dodag->prefixes;
}

/* Called before updating all prefixes in a DIO */
void rpl_dodag_update_unpublished_dio_prefix_start(rpl_dodag_t *dodag)
{
    /* Clear age flags - will use as a marker for entries being in the DIO */
    ns_list_foreach(prefix_entry_t, entry, &dodag->prefixes) {
        if (!(entry->options & RPL_PIO_PUBLISHED)) {
            entry->options &= ~RPL_PIO_AGE;
        }
    }
}


/* Called after updating all prefixes in a DIO */
void rpl_dodag_update_unpublished_dio_prefix_finish(rpl_dodag_t *dodag)
{
    /* Any remaining non-published entries that don't have the age flag
     * set are not being sent by parent any more, so we should stop sending
     * too, except for the minimimum count requirement on 0 lifetime.
     */
    ns_list_foreach_safe(prefix_entry_t, entry, &dodag->prefixes) {
        if ((entry->options & (RPL_PIO_PUBLISHED | RPL_PIO_AGE | RPL_PIO_HOLD_MASK)) == 0) {
            rpl_dodag_delete_dio_prefix(dodag, entry);
        }
    }
}

prefix_entry_t *rpl_dodag_update_dio_prefix(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool publish, bool age)
{
    /* Don't let them set funny flags - we won't propagate them either.
     * Is not propagating them sensible? RFC isn't clear. Anyway, we use
     * the spare 5 flag bits internally...
     */
    flags &= (PIO_R | PIO_A | PIO_L);

    if (publish) {
        flags |= RPL_PIO_PUBLISHED;
    }

    if (age) {
        flags |= RPL_PIO_AGE;
    }

    prefix_entry_t *entry = icmpv6_prefix_add(&dodag->prefixes, prefix, prefix_len, lifetime, preftime, 0xff);
    /* icmpv6_prefix_add indicates a new entry by leaving options set to 0xFF */
    if (entry) {
        /* Newly-seen zero lifetimes should be advertised at least a few times -
         * count this down in the RPL_PIO_HOLD_COUNT field
         */
        if (lifetime == 0 && (entry->options == 0xFF || entry->lifetime != 0)) {
            flags |= RPL_MAX_FINAL_RTR_ADVERTISEMENTS;
        }
        entry->options = flags;
        entry->lifetime = lifetime;
        entry->preftime = preftime;
    }
    return entry;
}

void rpl_dodag_delete_dio_prefix(rpl_dodag_t *dodag, prefix_entry_t *prefix)
{
    rpl_instance_t *instance = dodag->instance;

    if (instance && instance->domain->prefix_cb) {
        instance->domain->prefix_cb(prefix, instance->domain->cb_handle, NULL);
    }

    ns_list_remove(&dodag->prefixes, prefix);
    ns_dyn_mem_free(prefix);
}

static void rpl_dodag_age_prefixes(rpl_dodag_t *dodag, uint16_t seconds)
{
    ns_list_foreach_safe(prefix_entry_t, prefix, &dodag->prefixes) {
        if (!(prefix->options & RPL_PIO_AGE)) {
            continue;
        }
        if (prefix->preftime != 0xFFFFFFFF) {
            if (prefix->preftime > seconds) {
                prefix->preftime -= seconds;
            } else {
                prefix->preftime = 0;
            }
        }
        if (prefix->lifetime != 0xFFFFFFFF) {
            if (prefix->lifetime > seconds) {
                prefix->lifetime -= seconds;
            } else {
                prefix->lifetime = 0;
                /* Only delete on timeout if we're publishing - otherwise we will
                 * keep advertising until we see our parent stop advertising it - deletion
                 * is handled in rpl_control_process_prefix_options.
                 */
                if ((prefix->options & (RPL_PIO_PUBLISHED | RPL_PIO_HOLD_MASK)) == RPL_PIO_PUBLISHED) {
                    rpl_dodag_delete_dio_prefix(dodag, prefix);
                }
            }
        }
    }
}

static void rpl_dodag_slow_timer(rpl_dodag_t *dodag, uint16_t seconds)
{
    rpl_dodag_age_routes(dodag, seconds);
    rpl_dodag_age_prefixes(dodag, seconds);
}


/* Look up a RPL instance by identifier. If the ID is local, addr must
 * identify the root - eg by providing either the source destination address of
 * a packet (depending on the 'O' flag).
 */
rpl_instance_t *rpl_lookup_instance(const rpl_domain_t *domain, uint8_t instance_id, const uint8_t *addr)
{
    if (rpl_instance_id_is_local(instance_id)) {
        instance_id &= ~RPL_INSTANCE_DEST;
    }

    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        /* First match the instance ID */
        if (instance->id != instance_id) {
            continue;
        }
        /* If it's a global ID, this is a match */
        if (rpl_instance_id_is_global(instance_id)) {
            return instance;
        }
        /* If it's a local ID, address must match */
        const rpl_dodag_t *dodag = ns_list_get_first(&instance->dodags);
        if (addr && dodag && addr_ipv6_equal(dodag->id, addr)) {
            return instance;
        }
    }
    return NULL;
}

rpl_instance_t *rpl_create_instance(rpl_domain_t *domain, uint8_t instance_id)
{
    rpl_instance_t *instance = rpl_alloc(sizeof(rpl_instance_t));
    if (!instance) {
        return NULL;
    }
    memset(instance, 0, sizeof(rpl_instance_t));
    ns_list_init(&instance->dodags);
    ns_list_init(&instance->candidate_neighbours);
    ns_list_init(&instance->dao_targets);
    instance->dtsn = rpl_seq_init();
    instance->last_dao_trigger_time = protocol_core_monotonic_time;
    instance->dao_sequence = rpl_seq_init();
    instance->id = instance_id;
    instance->domain = domain;

    ns_list_add_to_start(&domain->instances, instance);
    return instance;
}

void rpl_delete_instance(rpl_instance_t *instance)
{
    rpl_domain_t *domain = instance->domain;
    ns_list_foreach_safe(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        rpl_delete_neighbour(instance, neighbour);
    }
    ns_list_foreach_safe(rpl_dodag_t, dodag, &instance->dodags) {
        rpl_delete_dodag(dodag);
    }
    ns_list_foreach_safe(rpl_dao_target_t, target, &instance->dao_targets) {
        rpl_delete_dao_target(instance, target);
    }
    ns_list_remove(&domain->instances, instance);
    rpl_free(instance, sizeof * instance);
}

/* Don't purge a DODAG we've used unless it's been gone for 15 minutes */
#define DODAG_MIN_PURGE_AGE (15*60*10) // 15 minutes

/* Choose worst DODAG in an instance - will be purgeable */
static rpl_dodag_t *rpl_instance_choose_dodag_to_purge(const rpl_instance_t *instance)
{
    rpl_dodag_t *worst = NULL;

    ns_list_foreach_reverse(rpl_dodag_t, dodag, &instance->dodags) {
        uint32_t dodag_age = protocol_core_monotonic_time - dodag->timestamp;
        if (rpl_dodag_in_use(dodag)) {
            continue;
        }

        if (dodag_age < DODAG_MIN_PURGE_AGE && dodag->used) {
            continue;
        }

        if (!worst) {
            goto new_worst;
        }

        /* Prefer to purge least-recently-heard-from */
        uint32_t worst_age = protocol_core_monotonic_time - worst->timestamp;
        if (dodag_age <= worst_age) {
            continue;
        } else {
            goto new_worst;
        }

new_worst:
        worst = dodag;
        worst_age = dodag_age;
    }

    return worst;
}

/* Choose worst neighbour in an instance - may be a candidate for purging */
static rpl_neighbour_t *rpl_instance_choose_worst_neighbour(const rpl_instance_t *instance)
{
    rpl_neighbour_t *worst = NULL;
    bool worst_acceptable = false;
    bool worst_old = false;

    /* Parents will be first - loop backwards so we take non-parents first */
    ns_list_foreach_reverse(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        bool acceptable = instance->of->neighbour_acceptable(instance, neighbour);
        bool old;
        if (!neighbour->dodag_version) {
            old = true;
        } else {
            uint32_t age = protocol_core_monotonic_time - neighbour->dio_timestamp;
            uint32_t age_threshold = rpl_dio_imax_time_calculate(neighbour->dodag_version->dodag->dio_timer_params.Imax,
                                                                 rpl_policy_dio_validity_period(instance->domain));
            old = age > age_threshold;
        }
        if (!worst) {
            goto new_worst;
        }

        /* Stop if crossing from non-parents to parents - parents can't be worse */
        if (neighbour->dodag_parent && !worst->dodag_parent) {
            break;
        }

        /* Prefer neighbours with DODAG version */
        if (neighbour->dodag_version && !worst->dodag_version) {
            continue;
        } else if (!neighbour->dodag_version && worst->dodag_version) {
            goto new_worst;
        }

        /* Prefer to purge "unacceptable" neighbours according to OF */
        if (acceptable && !worst_acceptable) {
            continue;
        } else if (!acceptable && worst_acceptable) {
            goto new_worst;
        }

        /* Prefer to purge old neighbours */
        if (old && !worst_old) {
            continue;
        } else if (!old && worst_old) {
            goto new_worst;
        }

        /* Tiebreak by path cost, assuming we have dodag_version */
        if (neighbour->dodag_version && instance->of) {
            /* worst must also have version to reach this tiebreak */
            if (instance->of->path_cost(neighbour) <= instance->of->path_cost(worst)) {
                continue;
            }
        }
new_worst:
        worst = neighbour;
        worst_acceptable = acceptable;
        worst_old = old;
    }

    return worst;
}

/* Purge one item from an instance */
bool rpl_instance_purge(rpl_instance_t *instance)
{
    /* Purge this instance itself if no remaining neighbours or DODAGs
     * (time restrictions on those are sufficient - no need for extra)
     */
    if (ns_list_is_empty(&instance->candidate_neighbours) &&
            ns_list_is_empty(&instance->dodags)) {
        rpl_delete_instance(instance);
        return true;
    }

    /* May purge a DODAG if not used for some time, and no remaining neighbours */
    rpl_dodag_t *dodag = rpl_instance_choose_dodag_to_purge(instance);
    if (dodag) {
        rpl_delete_dodag(dodag);
        return true;
    }

    /* Otherwise, delete a non-parent neighbour we've considered at least once.
     * (We don't want to delete a new neighbour before it gets a chance to
     * become a new parent)
     */
    rpl_neighbour_t *neighbour = rpl_instance_choose_worst_neighbour(instance);
    if (neighbour && neighbour->considered && !neighbour->dodag_parent && neighbour->dao_path_control == 0) {
        tr_debug("Candidate Purge: Remove %s", trace_ipv6(neighbour->ll_address));
        rpl_delete_neighbour(instance, neighbour);
        return true;
    }
    return false;
}

void rpl_instance_neighbours_changed(rpl_instance_t *instance, rpl_dodag_t *dodag)
{
    instance->neighbours_changed = true;
    uint16_t delay = rpl_policy_dio_parent_selection_delay(instance->domain);

    rpl_instance_trigger_parent_selection(instance, delay, dodag);
}

static void rpl_instance_remove_parents(rpl_instance_t *instance)
{
    ns_list_foreach(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        n->dodag_parent = false;
    }
}

/* Convert RPL lifetime (8-bit in units) to core lifetime (32-bit seconds) */
static uint32_t rpl_lifetime(uint8_t lifetime, uint16_t lifetime_unit)
{
    return lifetime == 0xff ? 0xffffffff : lifetime * (uint32_t) lifetime_unit;
}

static uint32_t rpl_default_lifetime(rpl_dodag_t *dodag)
{
    return rpl_lifetime(dodag->config.default_lifetime, dodag->config.lifetime_unit);
}

/* Adjust a lifetime (in seconds) downwards to account for its age */
static uint32_t rpl_aged_lifetime(uint32_t lifetime, uint32_t timestamp)
{
    if (lifetime != 0xffffffff) {
        uint32_t age = (protocol_core_monotonic_time - timestamp) / 10;
        if (age < lifetime) {
            lifetime -= age;
        } else {
            lifetime = 0;
        }
    }
    return lifetime;
}

static void rpl_instance_update_system_dio_route(rpl_instance_t *instance, rpl_neighbour_t *parent, rpl_dio_route_t *route)
{
    int8_t pref;
    switch (route->flags & RA_PRF_MASK) {
        case RA_PRF_LOW:
            pref = -1;
            break;
        case RA_PRF_MEDIUM:
            pref = 0;
            break;
        case RA_PRF_HIGH:
            pref = 1;
            break;
        default:
            return;
    }

    uint8_t metric = ipv6_route_pref_to_metric(pref) + parent->dodag_pref;

    ipv6_route_add_metric(route->prefix, route->prefix_len, parent->interface_id, parent->ll_address, ROUTE_RPL_DIO, parent, instance->id, route->lifetime, metric);
}

/* Called when a DIO has been received */
void rpl_dodag_update_implicit_system_routes(rpl_dodag_t *dodag, rpl_neighbour_t *parent)
{
    if (!rpl_dodag_is_current(dodag) || !parent->dodag_parent) {
        return;
    }

    uint32_t default_lifetime = rpl_default_lifetime(dodag);
    uint8_t metric = IPV6_ROUTE_DEFAULT_METRIC + parent->dodag_pref;

    /* Always add the "root" default route - only used for per-instance lookup */
    ipv6_route_add_metric(NULL, 0, parent->interface_id, parent->ll_address, ROUTE_RPL_INSTANCE, parent, dodag->instance->id, default_lifetime, metric);

    /* Also add a specific route to the DODAGID */
    ipv6_route_add_metric(dodag->id, 128, parent->interface_id, parent->ll_address, ROUTE_RPL_ROOT, parent, dodag->instance->id, default_lifetime, metric);

    /* Check if we assume default route to DODAGID */
    if (!dodag->instance->domain->process_routes) {
        ipv6_route_add_metric(NULL, 0, parent->interface_id, parent->ll_address, ROUTE_RPL_ROOT, parent, dodag->instance->id, default_lifetime, metric);
    }
}

/* Called when a DIO RIO route has been updated (but not the parent list) */
static void rpl_dodag_update_system_route(rpl_dodag_t *dodag, rpl_dio_route_t *route)
{
    if (!rpl_dodag_is_current(dodag)) {
        return;
    }
    rpl_instance_t *instance = dodag->instance;

    if (!instance->domain->process_routes) {
        // We dont add actual routes and only create default route throuh DODAGID
        return;
    }

    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->dodag_parent) {
            rpl_instance_update_system_dio_route(instance, neighbour, route);
        }
    }
}

/* Called when a parent has been added/updated (but not the DIO route list) */
static void rpl_instance_update_system_routes_through_parent(rpl_instance_t *instance, rpl_neighbour_t *parent)
{
    rpl_dodag_t *dodag = parent->dodag_version->dodag;

    rpl_dodag_update_implicit_system_routes(dodag, parent);

    if (!instance->domain->process_routes) {
        // We dont add actual routes and only create default route through DODAGID
        return;
    }

    /* Then add the specific routes listed in the DIO as ROUTE_RPL_DIO */
    ns_list_foreach(rpl_dio_route_t, route, &dodag->routes) {
        rpl_instance_update_system_dio_route(instance, parent, route);
    }
}

static void rpl_instance_remove_system_routes_through_parent(rpl_instance_t *instance, rpl_neighbour_t *parent)
{
    (void)instance;

    ipv6_route_table_remove_info(parent->interface_id, ROUTE_RPL_INSTANCE, parent);
    ipv6_route_table_remove_info(parent->interface_id, ROUTE_RPL_DIO, parent);
    ipv6_route_table_remove_info(parent->interface_id, ROUTE_RPL_ROOT, parent);
}

static void trace_info_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vtracef(TRACE_LEVEL_INFO, TRACE_GROUP, fmt, ap);
    va_end(ap);
}

static uint32_t rpl_dio_imax_time_calculate(uint16_t Imax, uint16_t fixed_point)
{
    return (((uint32_t)Imax * fixed_point) / 0x0100);
}


void rpl_instance_run_parent_selection(rpl_instance_t *instance)
{
    bool parent_set_change = false;
    rpl_dodag_version_t *original_version = instance->current_dodag_version;
    uint16_t original_rank = instance->current_rank;

    instance->parent_selection_timer = rpl_policy_parent_selection_period(instance->domain);

    if (!instance->of) {
        return;
    }

    if (instance->current_dodag_version &&
            (instance->current_dodag_version->dodag->root || instance->current_dodag_version->dodag->was_root)) {
        return;
    }

    ns_list_foreach_safe(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        //Remove a Parent candidates which are not heared a long time ago and not slected ones
        if (!n->dodag_parent && (rpl_aged_lifetime(rpl_default_lifetime(n->dodag_version->dodag), n->dio_timestamp) == 0)) {
            tr_debug("Candidate timeout: Remove %s", trace_ipv6(n->ll_address));
            rpl_delete_neighbour(instance, n);
            continue;
        }
        n->old_dao_path_control = n->dao_path_control;
        n->dao_path_control = 0;
        n->was_dodag_parent = n->dodag_parent;
        n->dodag_parent = false;
        n->considered = true;
    }

    rpl_neighbour_t *original_preferred = rpl_instance_preferred_parent(instance);

    instance->of->parent_selection(instance);

    ns_list_foreach(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        if (n->was_dodag_parent != n->dodag_parent) {
            tr_info("%s parent %s", n->dodag_parent ? "Added" : "Removed", trace_ipv6(n->ll_address));
            parent_set_change = true;
            /* Remove routes through a deselected parent */
            if (!n->dodag_parent) {
                rpl_instance_remove_system_routes_through_parent(instance, n);
            }
        }
        /* Always re-run route update (in case of changed preference values) */
        if (n->dodag_parent) {
            rpl_instance_update_system_routes_through_parent(instance, n);
        }
        n->was_dodag_parent = false;
    }

    rpl_neighbour_t *preferred_parent = rpl_instance_preferred_parent(instance);

    if (preferred_parent) {
        rpl_control_disable_ra_routes(instance->domain);
    } else {
        rpl_instance_poison(instance, rpl_policy_repair_poison_count(instance->domain));
    }

    if (original_preferred != preferred_parent) {
        protocol_stats_update(STATS_RPL_PARENT_CHANGE, 1);
        if (preferred_parent) {
            tr_info("New preferred parent %s", trace_array(preferred_parent->ll_address, 16));
        }
    }

    // Sets new preferred parent
    if (preferred_parent) {
        ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, preferred_parent->interface_id, NULL, preferred_parent->ll_address,
                                                   protocol_6lowpan_neighbor_priority_set);
        // Sets secondary parent
        rpl_neighbour_t *sec_parent = ns_list_get_next(&instance->candidate_neighbours, preferred_parent);
        if (sec_parent && sec_parent->dodag_parent) {
            ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, sec_parent->interface_id, NULL, sec_parent->ll_address,
                                                       protocol_6lowpan_neighbor_second_priority_set);
        } else {
            protocol_6lowpan_neighbor_priority_clear_all(preferred_parent->interface_id, PRIORITY_2ND);
        }
    }

    //Control Local repair state
    if (preferred_parent) {
        // Always stop repair if we find a parent
        rpl_instance_set_local_repair(instance, false);
        //Validate time from last DIO

        uint32_t time_between_parent = protocol_core_monotonic_time - preferred_parent->dio_timestamp;
        uint32_t accepted_time = rpl_dio_imax_time_calculate(instance->current_dodag_version->dodag->dio_timer_params.Imax, rpl_policy_dio_validity_period(instance->domain));

        if (accepted_time < time_between_parent) {
            rpl_control_transmit_dis(instance->domain, NULL, RPL_SOLINFO_PRED_INSTANCEID, instance->id, NULL, 0, preferred_parent->ll_address);
        }

    } else if (original_preferred) {
        // Only start repair if we just lost a parent
        rpl_instance_set_local_repair(instance, true);
    } else {
        // !preferred_parent && !original_preferred - didn't have a parent,
        // still don't. Leave repair flag as-is (would be off on initial start
        // up, may be on if having problems mid-session).
    }

    if (rpl_instance_mop(instance) != RPL_MODE_NO_DOWNWARD) {
        rpl_downward_process_dao_parent_changes(instance);
    }

    /* Anyone who's not a parent can be pruned now (eg bad link cost) */
    ns_list_foreach_safe(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        if (n->dodag_parent) {
            continue;
        }
        if (!instance->of->neighbour_acceptable(instance, n)) {
            tr_debug("Candidate not acceptable: Remove %s", trace_ipv6(n->ll_address));
            rpl_delete_neighbour(instance, n);
        }
    }
    rpl_control_print(trace_info_print);
    /* Changing DODAG version is an inconsistency */
    if (original_version != instance->current_dodag_version) {
        //learn Routes an Prefixes
        if (preferred_parent && instance->current_dodag_version) {
            rpl_dodag_t *dodag = instance->current_dodag_version->dodag;
            protocol_interface_info_entry_t *rpl_interface = protocol_stack_interface_info_get_by_id(preferred_parent->interface_id);
            if (rpl_interface) {
                ns_list_foreach(prefix_entry_t, prefix, &dodag->prefixes) {
                    rpl_control_process_prefix_option(prefix, rpl_interface);
                    if (instance->domain->prefix_cb) {
                        instance->domain->prefix_cb(prefix, rpl_interface, preferred_parent->ll_address);
                    }
                }
            }
        }

        rpl_instance_inconsistency(instance);
        return;
    }

    /* Check for "consistent" indication as per RFC 6550 8.3 - if not
     * "consistent" by this definition, we reset Trickle consistency counter,
     * and do not process any more consistency events this interval.
     * See comments in rpl_control_dio_handler() for more info.
     */
    if (parent_set_change ||
            original_preferred != preferred_parent ||
            !(instance->current_dodag_version &&
              (rpl_rank_compare(instance->current_dodag_version->dodag, original_rank, instance->current_rank) & RPL_CMP_EQUAL))) {
        instance->dio_not_consistent = true;
        instance->dio_timer.c = 0;
    }
}

void rpl_instance_remove_interface(rpl_instance_t *instance, int8_t if_id)
{
    ns_list_foreach_safe(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->interface_id == if_id) {
            rpl_delete_neighbour(instance, neighbour);
        }
    }
}

/* Trigger DIO transmission - all interfaces multicast if addr+cur are NULL, else unicast */
void rpl_instance_dio_trigger(rpl_instance_t *instance, protocol_interface_info_entry_t *cur, const uint8_t *addr)
{
    uint16_t rank = instance->current_rank;
    rpl_dodag_version_t *dodag_version = instance->current_dodag_version;
    rpl_dodag_t *dodag;
    if (dodag_version) {
        dodag = dodag_version->dodag;
    } else if (instance->poison_count) {
        /* When poisoning, we can send using data from an arbitrary DODAG/version */
        dodag = ns_list_get_first(&instance->dodags);
        if (dodag) {
            dodag_version = ns_list_get_first(&dodag->versions);
        }
    } else {
        dodag = NULL;
    }

    if (!dodag || !dodag_version) {
        return;
    }

    if (instance->poison_count) {
        instance->poison_count--;
        rank = RPL_RANK_INFINITE;
        tr_debug("Poison count -> set RPL_RANK_INFINITE");
    }

    // Always send config in unicasts (as required), never in multicasts (optional)
    rpl_dodag_conf_t *conf;
    if (addr) {
        conf = &dodag->config;
    } else if (dodag->new_config_advertisment_count < rpl_policy_dio_multicast_config_advertisment_min_count()) {
        conf = &dodag->config;
        if (dodag->new_config_advertisment_count < 0xfe) {
            dodag->new_config_advertisment_count++;
        }
    } else {
        conf = NULL;
    }

    rpl_control_transmit_dio(instance->domain, cur, instance->id, dodag_version->number, rank, dodag->g_mop_prf, instance->dtsn, dodag, dodag->id, conf, addr);

    dodag_version->last_advertised_rank = rank;

    /* When we advertise a new lowest rank, need to re-evaluate our rank limits */
    if (rank < dodag_version->lowest_advertised_rank) {
        dodag_version->lowest_advertised_rank = rank;
#if 0
        // Standard RFC 6550 behaviour
        dodag_version->hard_rank_limit = rpl_rank_add(rank, dodag->config.dag_max_rank_increase);
#else
        // Round up hard limit - DAGRank interpretation. Contrary to wording of RFC 6550 8.2.2.4.3,
        // but needed to cope reasonably with Wi-SUN insisting on DAGMaxRankIncrease of 0.
        // Interpret that as a request to not increase DAGRank, rather than Rank.
        //
        // Example, if DAGMaxRankIncrease is 0, MinHopRankIncrease is 0x80, and our advertised
        // 0xC0, then we permit up to 0xFF, which doesn't increase DAGRank. If DAGMaxRankIncrease
        // is 0x80, then we permit can go form 0xC0 to 0x17F, increasing DAGRank by 1, even though
        // it's a Rank increase of 0xBF. Fractional parts of DAGMaxRankIncrease are ignored.
        uint16_t dagrank = nrpl_dag_rank(dodag, rank);
        uint16_t dagmaxinc = nrpl_dag_rank(dodag, dodag->config.dag_max_rank_increase);
        uint16_t dagmax = rpl_rank_add(dagrank, dagmaxinc);
        if (dagmax == RPL_RANK_INFINITE) {
            dodag_version->hard_rank_limit = RPL_RANK_INFINITE;
        } else {
            dodag_version->hard_rank_limit = nrpl_rank(dodag, 1 + dagmax) - 1;
        }
#endif
    }
    rpl_dodag_version_limit_greediness(dodag_version, rank);

    instance->last_advertised_dodag_version = dodag_version;
}

static void rpl_instance_dis_timer(rpl_instance_t *instance, uint16_t seconds)
{
    if (instance->repair_dis_timer > seconds) {
        instance->repair_dis_timer -= seconds;
    } else if (instance->repair_dis_timer != 0) {
        tr_debug("Timed repair DIS %d", instance->id);
        instance->repair_dis_timer = 0;
        instance->repair_dis_count++;
        rpl_control_transmit_dis(instance->domain, NULL, RPL_SOLINFO_PRED_INSTANCEID, instance->id, NULL, 0, NULL);
        if (instance->repair_dis_count < rpl_policy_repair_dis_count(instance->domain)) {
            uint16_t t = rpl_policy_repair_initial_dis_delay(instance->domain);
            uint16_t max = rpl_policy_repair_maximum_dis_interval(instance->domain);
            for (uint_fast8_t n = instance->repair_dis_count; n; n--) {
                if (t < 0x8000 && t < max) {
                    t <<= 1;
                } else {
                    t = max;
                    break;
                }
            }
            if (t > max) {
                t = max;
            }
            instance->repair_dis_timer = t;
        } else {
            rpl_control_event(instance->domain, RPL_EVENT_LOCAL_REPAIR_NO_MORE_DIS);
        }
    }
}

void rpl_instance_set_local_repair(rpl_instance_t *instance, bool repair)
{
    if (instance->local_repair == repair) {
        return;
    }

    instance->local_repair = repair;

    if (repair) {
        /* Notify RPL user to state switch */
        rpl_control_event(instance->domain, RPL_EVENT_LOCAL_REPAIR_START);
        protocol_stats_update(STATS_RPL_LOCAL_REPAIR, 1);
        instance->repair_dis_timer = rpl_policy_repair_initial_dis_delay(instance->domain);
        instance->repair_dis_count = 0;
    } else {
        instance->repair_dis_timer = 0;
    }

    /* When repair ends, eliminate all higher-rank neighbours (potential sub-DODAG) from table */
    if (!repair && instance->current_dodag_version) {
        ns_list_foreach_safe(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
            if (rpl_dodag_version_rank_indicates_possible_sub_dodag(neighbour->dodag_version, neighbour->rank)) {
                rpl_delete_neighbour(instance, neighbour);
            }
        }
    }
}

bool rpl_instance_local_repair(const rpl_instance_t *instance)
{
    return instance->local_repair;
}

uint16_t rpl_instance_current_rank(const rpl_instance_t *instance)
{
    return instance->current_rank;
}

bool rpl_instance_address_is_parent(rpl_instance_t *instance, const uint8_t *ipv6_addr)
{
    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (!neighbour->dodag_parent) {
            // list is ordered so first encounter of false means no more parents in list
            return false;
        }

        if (addr_ipv6_equal(neighbour->ll_address, ipv6_addr)) {
            return true;
        }
    }
    return false;
}

bool rpl_instance_address_is_candidate(rpl_instance_t *instance, const uint8_t *ipv6_addr, uint16_t candidate_amount)
{
    uint16_t list_compared = 0;

    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (addr_ipv6_equal(neighbour->ll_address, ipv6_addr)) {
            return true;
        }
        list_compared++;
        if (candidate_amount && list_compared >= candidate_amount) {
            return false;
        }

    }
    return false;
}

uint16_t rpl_instance_address_candidate_count(const rpl_instance_t *instance, bool selected_parents)
{
    uint16_t parent_list = 0;

    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {


        if (selected_parents) {
            if (neighbour->dodag_parent) {
                parent_list++;
            }
        } else {
            parent_list++;
        }
    }
    return parent_list;
}

void rpl_instance_neighbor_delete(rpl_instance_t *instance, const uint8_t *ipv6_addr)
{
    ns_list_foreach_safe(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (addr_ipv6_equal(neighbour->ll_address, ipv6_addr)) {
            rpl_delete_neighbour(instance, neighbour);
        }
    }
    return;
}

void rpl_instance_slow_timer(rpl_instance_t *instance, uint16_t seconds)
{
    ns_list_foreach(rpl_dodag_t, dodag, &instance->dodags) {
        rpl_dodag_slow_timer(dodag, seconds);
    }

    rpl_instance_parent_address_reg_timer_update(instance, seconds);
    rpl_instance_parent_selection_timer(instance, seconds);
    if (!rpl_instance_preferred_parent(instance)) {
        protocol_stats_update(STATS_RPL_TIME_NO_NEXT_HOP, 1);
        rpl_instance_dis_timer(instance, seconds);
    }
}


void rpl_upward_dio_timer(rpl_instance_t *instance, uint16_t ticks)
{
    rpl_dodag_version_t *dodag_version = instance->current_dodag_version;
    rpl_dodag_t *dodag;
    if (dodag_version) {
        dodag = dodag_version->dodag;
    } else if (instance->poison_count) {
        /* When poisoning, we can send using data from an arbitrary DODAG/version */
        dodag = ns_list_get_first(&instance->dodags);
        if (dodag) {
            dodag_version = ns_list_get_first(&dodag->versions);
        }
    } else {
        dodag = NULL;
    }

    if (!dodag || !dodag_version) {
        return;
    }

    /* Leaves don't send normal periodic DIOs */
    if (rpl_dodag_am_leaf(dodag) && !instance->poison_count) {
        return;
    }
    /* Delay sending first DIO if we are still potentially gathering info */
    /* Important to always send DIOs if we ever have sent any, so we can indicate problems to others */
    if (!instance->last_advertised_dodag_version && rpl_policy_parent_confirmation_requested()) {
        // We dont have any valid address in interface
        if (ns_list_count(&instance->dao_targets) == 0) {
            return;
        }
        /* Address registrations for parent ongoing*/
        if (instance->pending_neighbour_confirmation) {
            return;
        }
        /* If we are waiting for DAO or DAO registration is needed we dont send periodic DIOs */
        if (instance->dao_in_transit || instance->delay_dao_timer > 0) {
            return;
        }
    }
    if (trickle_timer(&instance->dio_timer, &dodag->dio_timer_params, ticks)) {
        instance->dio_not_consistent = false;
        rpl_instance_dio_trigger(instance, NULL, NULL);
    }
}

void rpl_upward_print_neighbour(const rpl_neighbour_t *neighbour, route_print_fn_t *print_fn)
{
    uint16_t path_cost;
    if (neighbour->dodag_version->dodag->instance->of) {
        path_cost = neighbour->dodag_version->dodag->instance->of->path_cost(neighbour);
    } else {
        path_cost = 0xFFFF;
    }

    ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str_ll);
    ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str_global);
    print_fn("   %2.0d%c%04x %04x %02x %s%%%d (%s)",
             neighbour->dodag_parent ? neighbour->dodag_pref + 1 : 0,
             neighbour->dodag_version && rpl_instance_preferred_parent(neighbour->dodag_version->dodag->instance) == neighbour ? '*' : ' ',
             neighbour->rank,
             path_cost,
             neighbour->dao_path_control,
             ROUTE_PRINT_ADDR_STR_FORMAT(addr_str_ll, neighbour->ll_address), neighbour->interface_id,
             ROUTE_PRINT_ADDR_STR_FORMAT(addr_str_global, neighbour->global_address));
}

void rpl_upward_print_neighbours_in_version(const rpl_neighbour_list_t *list, const rpl_dodag_version_t *version, route_print_fn_t *print_fn)
{
    ns_list_foreach(rpl_neighbour_t, neighbour, list) {
        if (neighbour->dodag_version == version) {
            rpl_upward_print_neighbour(neighbour, print_fn);
        }
    }
}

void rpl_upward_print_dodag(rpl_instance_t *instance, rpl_dodag_t *dodag, route_print_fn_t *print_fn)
{
    /* Summary */
    ROUTE_PRINT_ADDR_STR_BUFFER_INIT(addr_str);
    print_fn("DODAG %s", ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, dodag->id));
    print_fn("  G=%d MOP=%d Prf=%d", dodag->g_mop_prf & RPL_GROUNDED ? 1 : 0,
             (dodag->g_mop_prf & RPL_MODE_MASK) >> RPL_MODE_SHIFT,
             (dodag->g_mop_prf & RPL_DODAG_PREF_MASK));
    /* Routes */
    ns_list_foreach(rpl_dio_route_t, route, &dodag->routes) {
        uint8_t addr[16] = { 0 } ;
        int_fast8_t pref;
        switch (route->flags & RA_PRF_MASK) {
            case RA_PRF_LOW:
                pref = -1;
                break;
            case RA_PRF_MEDIUM:
                pref = 0;
                break;
            case RA_PRF_HIGH:
                pref = 1;
                break;
            default:
                pref = 2;
                break;
        }
        bitcopy(addr, route->prefix, route->prefix_len);
        if (route->lifetime == 0xFFFFFFFF) {
            print_fn("%24s/%-3u lifetime:infinite pref:%"PRIdFAST8,
                     ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), route->prefix_len, pref);

        } else {
            print_fn("%24s/%-3u lifetime:%"PRIu32" pref:%"PRIdFAST8,
                     ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), route->prefix_len, route->lifetime, pref);
        }
    }
    /* Prefixes */
    ns_list_foreach(prefix_entry_t, prefix, &dodag->prefixes) {
        uint8_t addr[16] = { 0 } ;
        bitcopy(addr, prefix->prefix, prefix->prefix_len);
        if (prefix->lifetime == 0xFFFFFFFF) {
            print_fn("%24s/%-3u lifetime:infinite flags:%c%c%c",
                     ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), prefix->prefix_len,
                     prefix->options & PIO_L ? 'L' : '-',
                     prefix->options & PIO_A ? 'A' : '-',
                     prefix->options & RPL_PIO_PUBLISHED ? '*' : ' '
                    );
        } else {
            print_fn("%24s/%-3u lifetime:%"PRIu32" flags:%c%c%c",
                     ROUTE_PRINT_ADDR_STR_FORMAT(addr_str, addr), prefix->prefix_len, prefix->lifetime,
                     prefix->options & PIO_L ? 'L' : '-',
                     prefix->options & PIO_A ? 'A' : '-',
                     prefix->options & RPL_PIO_PUBLISHED ? '*' : ' '
                    );
        }
    }
    /* Versions */
    ns_list_foreach(rpl_dodag_version_t, version, &dodag->versions) {
        print_fn("  Version %d", version->number);
        if (version == instance->current_dodag_version) {
            print_fn("  *Current version* Rank=%04x", instance->current_rank);
        }
        rpl_upward_print_neighbours_in_version(&instance->candidate_neighbours, version, print_fn);
    }
}

void rpl_upward_print_instance(rpl_instance_t *instance, route_print_fn_t *print_fn)
{
    print_fn("RPL Instance %d", instance->id);
    print_fn("---------------");
    ns_list_foreach(rpl_dodag_t, dodag, &instance->dodags) {
        rpl_upward_print_dodag(instance, dodag, print_fn);
    }
    if (instance->current_dodag_version) {
        const trickle_params_t *params = &instance->current_dodag_version->dodag->dio_timer_params;
        const trickle_t *timer = &instance->dio_timer;

        print_fn("DIO trickle Imin=%d, Imax=%d, k=%d", params->Imin, params->Imax, params->k);
        print_fn("            I=%d, now=%d, t=%d, c=%d", timer->I, timer->now, timer->t, timer->c);
    }
}

uint16_t rpl_upward_read_dao_target_list_size(const rpl_instance_t *instance, const uint8_t *target_prefix)
{

    if (target_prefix) {
        uint16_t registered_address_count = 0;
        ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
            if (bitsequal(target->prefix, target_prefix, 64)) {
                registered_address_count++;
            }
        }
        return registered_address_count;
    }

    return ns_list_count(&instance->dao_targets);
}

/* Backwards-compatibility implementation of net_rpl.h API designed for old implementation */
bool rpl_upward_read_dodag_info(const rpl_instance_t *instance, rpl_dodag_info_t *dodag_info)
{
    rpl_dodag_version_t *version = rpl_instance_current_dodag_version(instance);
    if (!version) {
        return false;
    }

    rpl_dodag_t *dodag = version->dodag;

    memcpy(dodag_info->dodag_id, dodag->id, 16);
    dodag_info->instance_id = instance->id;
    dodag_info->flags = dodag->g_mop_prf;
    dodag_info->version_num = version->number;
    dodag_info->DTSN = instance->dtsn;
    dodag_info->curent_rank = instance->current_rank;
    dodag_info->parent_flags = 0;
    dodag_info->dag_min_hop_rank_inc = dodag->config.min_hop_rank_increase;

    rpl_neighbour_t *pref_parent = rpl_instance_preferred_parent(instance);
    if (!pref_parent) {
        return true;
    }

    dodag_info->parent_flags |= RPL_PRIMARY_PARENT_SET;
    memcpy(dodag_info->primary_parent,
           pref_parent->have_global_address ? pref_parent->global_address
           : pref_parent->ll_address,
           16);
    dodag_info->primary_parent_rank = pref_parent->rank;

    rpl_neighbour_t *sec_parent = ns_list_get_next(&instance->candidate_neighbours, pref_parent);
    if (!sec_parent || !sec_parent->dodag_parent) {
        return true;
    }

    dodag_info->parent_flags |= RPL_SECONDARY_PARENT_SET;
    memcpy(dodag_info->secondary_parent,
           sec_parent->have_global_address ? sec_parent->global_address
           : sec_parent->ll_address,
           16);
    dodag_info->secondary_parent_rank = sec_parent->rank;

    return true;
}

bool rpl_upward_accept_prefix_update(const rpl_dodag_t *dodag_info, const rpl_neighbour_t *neighbour, const rpl_neighbour_t *pref_parent)
{
    //Accept allways from Pref parent or before it is selected
    if (!pref_parent || neighbour == pref_parent) {
        return true;
    }

    //Accept only same or higher version number
    if (rpl_dodag_version_compare(neighbour->dodag_version, pref_parent->dodag_version) & (RPL_CMP_EQUAL | RPL_CMP_GREATER)) {
        //Calculate Time between from last dio from parent and this neighbour
        //neighbour dio_timestamp >= pref_parent's, because it's a newly-received message
        uint32_t time_between_parent = neighbour->dio_timestamp - pref_parent->dio_timestamp;
        uint32_t accepted_time = rpl_dio_imax_time_calculate(dodag_info->dio_timer_params.Imax, 0x0200);
        //Accept prefix Update If Time from last DIO is more than 2 x Max
        if (accepted_time < time_between_parent) {
            return true;
        }
    }

    return false;
}

#endif /* HAVE_RPL */
