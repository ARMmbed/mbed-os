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
#include "nsconfig.h"

#ifdef HAVE_RPL

#include "ns_types.h"
#include "ns_trace.h"
#include "common_functions.h"

#include "net_rpl.h"

#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Service_Libs/etx/etx.h"

#include "RPL/rpl_protocol.h"
#include "RPL/rpl_objective.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_structures.h"
#include "RPL/rpl_mrhof.h"

#define TRACE_GROUP "rplm"

/* Current implementation assumes ETX (and no metric container) - need to
 * ensure that we behave appropriately if a metric container is present.
 */

static void rpl_mrhof_parent_selection(rpl_instance_t *instance);
static uint16_t rpl_mrhof_path_cost_through_neighbour(const rpl_neighbour_t *neighbour);
static bool rpl_mrhof_neighbour_acceptable(const rpl_instance_t *instance, const rpl_neighbour_t *neighbour);

static rpl_objective_t rpl_mrhof = {
    .ocp = RPL_OCP_MRHOF,
    .parent_selection = rpl_mrhof_parent_selection,
    .path_cost = rpl_mrhof_path_cost_through_neighbour,
    .neighbour_acceptable = rpl_mrhof_neighbour_acceptable,
};

typedef struct rpl_of0_params {
    uint8_t stretch_of_rank;
    uint8_t rank_factor;
} rpl_of0_params;

void rpl_mrhof_init(void)
{
    rpl_objective_register(&rpl_mrhof);
}

/* Return ETX in RFC 6551 metric form (*128), 0xFFFF = infinite */
static uint16_t rpl_mrhof_etx(const rpl_neighbour_t *neighbour)
{
    uint16_t etx88 = ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, neighbour->interface_id, NULL, neighbour->ll_address, etx_read);
    switch (etx88) {
        case 0x0000: /* Unknown - assume poor */
            return 2 * 128;
        case 0x0001: /* Interface has no ETX - assume good */
            return 1 * 128;
        case 0xFFFF: /* Not associated */
            return RPL_RANK_INFINITE;
        default:
            return etx88 >> 1;
    }
}

static uint16_t rpl_mrhof_link_metric_to_neighbour(const rpl_neighbour_t *neighbour)
{
    return rpl_mrhof_etx(neighbour);
}

static uint16_t rpl_mrhof_path_cost_through_neighbour(const rpl_neighbour_t *neighbour)
{
    return rpl_rank_add(neighbour->rank, rpl_mrhof_link_metric_to_neighbour(neighbour));
}

static bool rpl_mrhof_neighbour_acceptable(const rpl_instance_t *instance, const rpl_neighbour_t *neighbour)
{
    return rpl_mrhof_link_metric_to_neighbour(neighbour) <= rpl_policy_mrhof_max_link_metric(instance->domain);
}


/* Given a preferred parent, we are only permitted to stretch our above the
 * path cost through that parent by a certain (policy) amount to accommodate a
 * bigger parent set.
 */
static uint16_t rpl_mrhof_max_stretched_rank(const rpl_dodag_version_t *version, uint16_t base_rank)
{
    uint16_t max_stretch = rpl_policy_mrhof_max_rank_stretch_for_extra_parents(version->dodag->instance->domain);
    uint16_t max_stretched = rpl_rank_add(base_rank, max_stretch);
    if (max_stretched > version->greediness_rank_limit) {
        max_stretched = version->greediness_rank_limit;
    }
    return max_stretched;
}



/* RFC 6552 4.2.2. Selection of the Backup Feasible Successor
 * This implementation can be called multiple times to select multiple successors, and can be used
 * to check the backup for a potential parent.
 */
static rpl_neighbour_t *rpl_mrhof_select_backup_parent(rpl_instance_t *instance, rpl_neighbour_t *pref, uint16_t *rank, uint16_t max_rank)
{
    rpl_neighbour_t *best = NULL;
    uint16_t best_rank = RPL_RANK_INFINITE;
    uint16_t best_path_cost = RPL_RANK_INFINITE;

    ns_list_foreach(rpl_neighbour_t, c, &instance->candidate_neighbours) {
        /* New backup must not be (potential) preferred parent or already be a parent */
        if (c == pref || c->dodag_parent) {
            continue;
        }

        /* Backup must be in the same DODAG version (keep it simple) */
        if (c->dodag_version != pref->dodag_version) {
        	continue;
        }

        rpl_dodag_t *dodag = c->dodag_version->dodag;
        if (dodag != pref->dodag_version->dodag) {
            continue;
        }

        /* Ignore high-link-metric neighbours */
        if (rpl_mrhof_link_metric_to_neighbour(c) > rpl_policy_mrhof_max_link_metric(instance->domain)) {
            continue;
        }

        /* It must not cause our rank to go up too much (rank affected by rules
         * 2 and 3 of RFC 6719 3.3.)
         */
        uint16_t path_cost = rpl_mrhof_path_cost_through_neighbour(c);
        uint16_t next_rank_rule2 = rpl_rank_next_level(dodag, c->rank);
        uint16_t path_cost_rule3 = rpl_rank_sub(path_cost, dodag->config.dag_max_rank_increase);
        uint16_t new_rank = next_rank_rule2 > path_cost_rule3 ? next_rank_rule2 : path_cost_rule3;
        if (new_rank > max_rank) {
            continue;
        }

        if (!best) {
            goto new_best;
        }

        /* Prefer lesser path cost */
        if (path_cost < best_path_cost) {
            goto new_best;
        } else if (path_cost > best_path_cost) {
            continue;
        }

        /* If it's a tie at this point, prefer the first in the list, which will
         * retain any previous parent ordering.
         */
        continue;

new_best:
        best = c;
        best_path_cost = path_cost;
        best_rank = new_rank;
    }

    if (best) {
        if (best_rank > *rank) {
            *rank = best_rank;
        }
    }

    return best;
}

/* RFC 6719 3.2.2. Selection of the parent with lowest path cost */
static rpl_neighbour_t *rpl_mrhof_select_best_parent(rpl_instance_t *instance, const rpl_neighbour_t *prev_preferred, uint16_t *rank_out)
{
    rpl_neighbour_t *best = NULL;
    uint16_t best_rank = RPL_RANK_INFINITE;
    uint16_t best_path_cost = RPL_RANK_INFINITE;
    uint16_t best_link_metric = RPL_RANK_INFINITE;
    uint16_t prev_preferred_path_cost = RPL_RANK_INFINITE;
    const uint16_t metric_threshold = rpl_policy_mrhof_max_link_metric(instance->domain);

    /* Previous preferred parent, if any, will be at the start of the list */
    /* We can use this to simplify some logic */
    if (prev_preferred) {
    	prev_preferred_path_cost = rpl_mrhof_path_cost_through_neighbour(prev_preferred);
    }

    ns_list_foreach(rpl_neighbour_t, c, &instance->candidate_neighbours) {
        rpl_dodag_t *dodag = c->dodag_version->dodag;

        /* Check link, and ignore totally unreachable neighbours */
        uint16_t link_metric = rpl_mrhof_link_metric_to_neighbour(c);
        if (link_metric == RPL_RANK_INFINITE) {
            continue;
        }

        /* For ETX, rank is the path cost, then make sure we increase by MinHopRankIncrease */
        /* (Normally, MinHopRankIncrease will be 0x80, so this would be superfluous) */
        uint16_t path_cost = rpl_mrhof_path_cost_through_neighbour(c);
        uint16_t new_rank = path_cost;
        uint16_t min_rank = rpl_rank_add(c->rank, dodag->config.min_hop_rank_increase);
        if (new_rank < min_rank) {
        	new_rank = min_rank;
        }

        if (new_rank > c->dodag_version->hard_rank_limit) {
            new_rank = RPL_RANK_INFINITE;
        }

        if (!best) {
            goto new_best;
        }

        /* Avoid using high-link-metric neighbours (but allow if we have no alternative) */
        if (link_metric <= metric_threshold && best_link_metric > metric_threshold) {
            goto new_best;
        } else if (link_metric > metric_threshold && best_link_metric <= metric_threshold) {
            continue;
        }

        /* MRHOF RFC is fuzzy about versions and DODAG selection; it seems
         * in many ways to assume we're choosing parents only within a
         * DODAG Version, whereas we also need to choose the DODAG and Version.
         *
         * Therefore we retain some of the logic for selecting DODAG from OF0.
         */

        /* Prefer connection to a grounded DODAG */
        if ((dodag->g_mop_prf & RPL_GROUNDED) != (best->dodag_version->dodag->g_mop_prf & RPL_GROUNDED)) {
            if (dodag->g_mop_prf & RPL_GROUNDED) {
                goto new_best;
            } else {
                continue;
            }
        }

        /* Go by DODAG preference */
        rpl_cmp_t cmp = rpl_dodag_pref_compare(dodag, best->dodag_version->dodag);
        if (cmp == RPL_CMP_GREATER) {
            goto new_best;
        } else if (cmp == RPL_CMP_LESS) {
            continue;
        }

        /* No explicit version preference - versions should flow naturally
         * from the root without one?
         */

        /* Hysteresis path cost test, if we are thinking of switching parents */
        if (best == prev_preferred) {
            /* Do not switch away from current parent, until threshold met */
        	/* Note that any current parent will be first in the list - we
        	 * don't need to worry about hitting it and giving it preference as
        	 * a candidate later.
        	 */
            if (rpl_rank_add(path_cost, rpl_policy_mrhof_parent_switch_threshold(instance->domain)) <= prev_preferred_path_cost) {
                goto new_best;
            } else {
                continue;
            }
        }

        /* Prefer lesser resulting path cost */
        if (path_cost < best_path_cost) {
        	goto new_best;
        } else if (path_cost > best_path_cost) {
        	continue;
        }

        /* Prefer parent that most recently sent a DIO */
        if (c->dio_timestamp != best->dio_timestamp) {
            if (common_serial_number_greater_32(c->dio_timestamp, best->dio_timestamp)) {
                goto new_best;
            } else {
                continue;
            }
        }


    new_best:
        best_rank = new_rank;
        best_path_cost = path_cost;
        best_link_metric = link_metric;
        best = c;
    }

    if (rank_out) {
        *rank_out = best_rank;
    }

    return best;
}



/*
 * Parent selection is a serious business. This runs periodically - it need not
 * be fast.
 *
 * Its job is to reorder the instance's candidate neighbour list, placing
 * members of the parent set at the front, in preference order. Those neighbours
 * must have dodag_parent set, and dodag_pref filled in.
 *
 * Before entry, "was_dodag_parent" is set to "dodag_parent", and "dodag_parent"
 * is cleared. "was_dodag_parent" must not be modified.
 *
 * Parent selection must not delete candidate neighbours.
 *
 * For each DODAGVersion, target_max_rank is the RPL core's desired maximum
 * This will be >= the current rank. This allows distinction between different
 * states in a DODAG:
 *
 * 1) Wanting to hold rank
 * 2) Wanting to hold DAGRank
 * 3) Willing to increase DAGRank, but not exceeding MaxDagRankIncrease
 * 4) Willing to take any rank (new version)
 *
 * instance->current_dodag_version and instance->current_rank should be updated
 * on exit.
 *
 * No other instance data structures should be modified - core will re-evaluate
 * by examining the reordered candidate list.
 */
static void rpl_mrhof_parent_selection(rpl_instance_t *instance)
{
    rpl_neighbour_t *old_pref_parent = rpl_instance_preferred_parent(instance);
    uint16_t rank;
    rpl_neighbour_t *pref_parent = rpl_mrhof_select_best_parent(instance, old_pref_parent, &rank);
    uint8_t last_pref;
    rpl_neighbour_list_t NS_LIST_NAME_INIT(parent_list);
    rpl_dodag_version_t *version;
    rpl_dodag_t *dodag;

    if (!pref_parent) {
        tr_debug("No pref_parent (mrhof) -> set RPL_RANK_INFINITE");
        rank = RPL_RANK_INFINITE;
        version = NULL;
        dodag = NULL;
        goto finish_up;
    }

    version = pref_parent->dodag_version;
    dodag = version->dodag;

    rpl_dodag_version_raise_greediness(version, rank);

    pref_parent->dodag_parent = true;
    pref_parent->dodag_pref = last_pref = 0;

    /* Move the preferred parent onto head of our empty parent list */
    ns_list_remove(&instance->candidate_neighbours, pref_parent);
    ns_list_add_to_end(&parent_list, pref_parent);

    uint_fast8_t more_successors = rpl_policy_mrhof_parent_set_size(instance->domain) - 1;
    uint16_t last_cost = rpl_mrhof_path_cost_through_neighbour(pref_parent);
    uint16_t max_rank = rpl_mrhof_max_stretched_rank(pref_parent->dodag_version, rank);
    while (more_successors--) {
        rpl_neighbour_t *backup = rpl_mrhof_select_backup_parent(instance, pref_parent, &rank, max_rank);
        if (!backup) {
        	break;
        }

        if (rank != RPL_RANK_INFINITE && rank > version->greediness_rank_limit) {
            tr_error("Rank excess during stretch %"PRIu16" > %"PRIu16, rank, version->greediness_rank_limit);
            rank = version->greediness_rank_limit;
            break;
        }

        /* Indicate preference levels, comparing path cost. If the preferred
         * parent has a higher cost than the backups, due to hysteresis, all
         * the others will end up with the same "preference" level.
         */
        uint16_t backup_cost = rpl_mrhof_path_cost_through_neighbour(backup);
        if (backup_cost > last_cost && last_pref < 15) {
            last_pref++;
            last_cost = backup_cost;
        }
        backup->dodag_parent = true;
        backup->dodag_pref = last_pref;
        ns_list_remove(&instance->candidate_neighbours, backup);
        ns_list_add_to_end(&parent_list, backup);
    }

    /* Two-step shuffle: move remaining unselected neighbours onto end of our list */
    ns_list_concatenate(&parent_list, &instance->candidate_neighbours);

    /* Then transfer whole list back to instance */
    ns_list_concatenate(&instance->candidate_neighbours, &parent_list);

finish_up:
    rpl_instance_set_dodag_version(instance, version, rank);

    /* Use default DAO path control */
    rpl_downward_convert_dodag_preferences_to_dao_path_control(dodag);
}

#endif /* HAVE_RPL */
