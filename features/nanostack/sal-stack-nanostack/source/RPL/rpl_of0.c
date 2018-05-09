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
#include "net_interface.h"

#include "NWK_INTERFACE/Include/protocol_abstract.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Service_Libs/etx/etx.h"

#include "RPL/rpl_protocol.h"
#include "RPL/rpl_objective.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_structures.h"
#include "RPL/rpl_of0.h"

#define TRACE_GROUP "rpl0"

static void rpl_of0_parent_selection(rpl_instance_t *instance);
static uint16_t rpl_of0_rank_through_neighbour(const rpl_neighbour_t *neighbour);
static bool rpl_of0_neighbour_acceptable(const rpl_instance_t *instance, const rpl_neighbour_t *neighbour);

static rpl_objective_t rpl_of0 = {
    .ocp = RPL_OCP_OF0,
    .parent_selection = rpl_of0_parent_selection,
    .path_cost = rpl_of0_rank_through_neighbour,
    .neighbour_acceptable = rpl_of0_neighbour_acceptable,
};

#define DEFAULT_STEP_OF_RANK 3
#define MINIMUM_STEP_OF_RANK 1
#define MAXIMUM_STEP_OF_RANK 9
#define SUITABLE_STEP_OF_RANK 8
#define DEFAULT_RANK_STRETCH 0
#define MAXIMUM_RANK_STRETCH 5
#define DEFAULT_RANK_FACTOR 1
#define MINIMUM_RANK_FACTOR 1
#define MAXIMUM_RANK_FACTOR 4

typedef struct rpl_of0_params {
    uint8_t stretch_of_rank;
    uint8_t rank_factor;
} rpl_of0_params;

void rpl_of0_init(void)
{
    rpl_objective_register(&rpl_of0);
}

static uint8_t rpl_of0_step_of_rank(const rpl_neighbour_t *neighbour)
{
    uint16_t etx88 = ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, neighbour->interface_id, NULL, neighbour->ll_address, etx_read);
    if (etx88 < 0x100) { /* "Unknown" */
        return DEFAULT_STEP_OF_RANK;
    }

#if MINIMUM_STEP_OF_RANK != 1 || MAXIMUM_STEP_OF_RANK != 9
#error "This assumes step 1-9"
#endif
    /* So 0x100 = 1, 0x101-0x107=2, ... 0x201-0x400=8, 0x401-0xFFFE = 9, 0xFFFF = 10 */
    static const uint16_t etx_thresholds[] = {
        0x100, 0x108, 0x110, 0x120, 0x140, 0x180, 0x200, 0x400, 0xFFFE, 0xFFFF
    };

    uint8_t step = 1;
    while (etx88 > etx_thresholds[step - 1]) {
        step++;
    }
    return step;
}

static bool rpl_of0_neighbour_acceptable(const rpl_instance_t *instance, const rpl_neighbour_t *neighbour)
{
    (void)instance;

    return rpl_of0_step_of_rank(neighbour) <= SUITABLE_STEP_OF_RANK;
}

static uint16_t rpl_of0_rank_increase(const rpl_neighbour_t *neighbour)
{
    uint8_t step = rpl_of0_step_of_rank(neighbour);
    if (step > MAXIMUM_STEP_OF_RANK) {
        return 0xFFFF;
    }
    const rpl_domain_t *domain = neighbour->dodag_version->dodag->instance->domain;
    uint_fast8_t rank_factor = rpl_policy_of0_rank_factor(domain);
    return (rank_factor * step) * neighbour->dodag_version->dodag->config.min_hop_rank_increase;
}

static uint16_t rpl_of0_rank_through_neighbour(const rpl_neighbour_t *neighbour)
{
    return rpl_rank_add(neighbour->rank, rpl_of0_rank_increase(neighbour));
}

/* Given a preferred parent, we are only permitted to stretch our above the
 * path cost through that parent by a certain (policy) amount to accommodate a
 * bigger parent set.
 */
static uint16_t rpl_of0_max_stretched_rank(const rpl_neighbour_t *pref_parent)
{
    uint16_t base_rank = rpl_of0_rank_through_neighbour(pref_parent);
    rpl_dodag_t *dodag = pref_parent->dodag_version->dodag;
    uint16_t max_stretch = rpl_policy_of0_stretch_of_rank(dodag->instance->domain) * dodag->config.min_hop_rank_increase;
    uint16_t max_stretched = rpl_rank_add(base_rank, max_stretch);
    if (max_stretched > pref_parent->dodag_version->greediness_rank_limit) {
        max_stretched = pref_parent->dodag_version->greediness_rank_limit;
    }
    return max_stretched;
}

/* RFC 6552 4.2.2. Selection of the Backup Feasible Successor
 * This implementation can be called multiple times to select multiple successors, and can be used
 * to check the backup for a potential parent.
 */
static rpl_neighbour_t *rpl_of0_select_backup_parent(rpl_instance_t *instance, rpl_neighbour_t *pref, uint16_t max_rank)
{
    rpl_neighbour_t *best = NULL;
    uint8_t best_step = 0xFF;

    ns_list_foreach(rpl_neighbour_t, c, &instance->candidate_neighbours) {
        /* 1. New backup must not be (potential) preferred parent or already be a parent */
        if (c == pref || c->dodag_parent) {
            continue;
        }

        /* 2. Backup must be in the same DODAG version or later */
        rpl_dodag_t *dodag = c->dodag_version->dodag;
        if (dodag != pref->dodag_version->dodag) {
            continue;
        }
        rpl_cmp_t cmp = rpl_seq_compare(c->dodag_version->number, pref->dodag_version->number);
        if (!(cmp & (RPL_CMP_GREATER | RPL_CMP_EQUAL))) {
            continue;
        }

        /* 3. It must have lower DAGRank than our maximum permitted (we haven´t
         *    necessarily chosen our rank yet, but we do have an upper bound).
         *    NB neither RPL core nor OF0 requires our rank to indicate
         *    /step in rank/ through other parents - they just need to have
         *    /lesser/ rank.
         */
        cmp = rpl_rank_compare(dodag, c->rank, max_rank);
        if (!(cmp & RPL_CMP_LESS)) {
            continue;
        }

        uint8_t step = rpl_of0_step_of_rank(c);
        /* Ignore totally unreachable */
        if (step > MAXIMUM_STEP_OF_RANK) {
             continue;
        }

        if (!best) {
            goto new_best;
        }

        /* 4. Prefer lesser rank */
        cmp = rpl_rank_compare(dodag, c->rank, best->rank);
        if (cmp & RPL_CMP_LESS) {
            goto new_best;
        } else if (cmp & RPL_CMP_GREATER) {
            continue;
        }

        /* 5. Prefer suitable link quality */
        if (step <= SUITABLE_STEP_OF_RANK && best_step > SUITABLE_STEP_OF_RANK) {
            goto new_best;
        } else if (step > SUITABLE_STEP_OF_RANK && best_step <= SUITABLE_STEP_OF_RANK) {
            continue;
        }

        /* 6. Prefer certain interfaces (not implemented) */

        /* 7. Prefer router previously in use */
        if (c->was_dodag_parent && !best->was_dodag_parent) {
            goto new_best;
        } else if (!c->was_dodag_parent && best->was_dodag_parent) {
            continue;
        }

        /* If it's a tie at this point, prefer the first in the list, which will
         * retain any previous parent ordering.
         */
        continue;

    new_best:
        best = c;
        best_step = step;
    }

    return best;
}

/* RFC 6552 4.2.1. Selection of the Preferred Parent */
/* As part of its work, it can also return the first backup if desired. */
static rpl_neighbour_t *rpl_of0_select_preferred_parent(rpl_instance_t *instance, const rpl_neighbour_t *prev_preferred, rpl_neighbour_t **backup_out, uint16_t *rank_out)
{
    rpl_neighbour_t *best = NULL;
    uint16_t best_rank = RPL_RANK_INFINITE;
    uint8_t best_step = 0xFF;
    rpl_neighbour_t *best_backup = NULL;

    ns_list_foreach(rpl_neighbour_t, c, &instance->candidate_neighbours) {
        rpl_dodag_t *dodag = c->dodag_version->dodag;
        uint8_t step = rpl_of0_step_of_rank(c);
        uint16_t new_rank = rpl_of0_rank_through_neighbour(c);
        rpl_cmp_t cmp;

        /* Ignore totally unreachable */
        if (step > MAXIMUM_STEP_OF_RANK) {
            continue;
        }

        /* 1. Selection mustn't cause our Rank to increase excessively */
        if (new_rank > c->dodag_version->hard_rank_limit) {
            new_rank = RPL_RANK_INFINITE;
        }

        rpl_neighbour_t *c_backup = rpl_of0_select_backup_parent(instance, c, rpl_of0_max_stretched_rank(c));

        if (!best) {
            goto new_best;
        }

        /* 2. Ignore totally unreachable, then prefer suitable connectivity- ignore totally unre */
        if (step <= SUITABLE_STEP_OF_RANK && best_step > SUITABLE_STEP_OF_RANK) {
            goto new_best;
        } else if (step > SUITABLE_STEP_OF_RANK && best_step <= SUITABLE_STEP_OF_RANK) {
            continue;
        }

        /* 3. Prefer certain interfaces (not implemented) */

        /* 4. Go by DODAG preference, if policy says this supersedes grounded check */
        cmp = rpl_dodag_pref_compare(dodag, best->dodag_version->dodag);
        if (rpl_policy_of0_dodag_preference_supersedes_grounded(instance->domain)) {
            if (cmp == RPL_CMP_GREATER) {
                goto new_best;
            } else if (cmp == RPL_CMP_LESS) {
                continue;
            }
        }

        /* 5. Prefer connection to a grounded DODAG */
        if ((dodag->g_mop_prf & RPL_GROUNDED) != (best->dodag_version->dodag->g_mop_prf & RPL_GROUNDED)) {
            if (dodag->g_mop_prf & RPL_GROUNDED) {
                goto new_best;
            } else {
                continue;
            }
        }

        /* 6. Go by DODAG preference (if we didn't already at 4) - comparison result still in cmp */
        if (cmp == RPL_CMP_GREATER) {
            goto new_best;
        } else if (cmp == RPL_CMP_LESS) {
            continue;
        }

        /* 7. Prefer newer DODAG Version if same DODAG */
        if (dodag == best->dodag_version->dodag) {
            cmp = rpl_seq_compare(c->dodag_version->number, best->dodag_version->number);
            if (cmp & RPL_CMP_GREATER) {
                goto new_best;
            } else if (cmp & RPL_CMP_LESS) {
                continue;
            }
        }

        /* 8. Prefer lesser resulting rank (note there should be no fractional part for OF0) */
        cmp = rpl_rank_compare(dodag, new_rank, best_rank);
        if (cmp & RPL_CMP_LESS) {
            goto new_best;
        } else if (cmp & RPL_CMP_GREATER) {
            continue;
        }

        /* 9. Prefer parent that gives us a backup parent */
        if (c_backup && !best_backup) {
            goto new_best;
        } else if (!c_backup && best_backup) {
            continue;
        }

        /* 10. Stick with previous preferred parent */
        if (c == prev_preferred) {
            goto new_best;
        } else if (best == prev_preferred) {
            continue;
        }

        /* 11. Prefer parent that most recently sent a DIO */
        if (c->dio_timestamp != best->dio_timestamp) {
            if (common_serial_number_greater_32(c->dio_timestamp, best->dio_timestamp)) {
                goto new_best;
            } else {
                continue;
            }
        }

    new_best:
        best_backup = c_backup;
        best_rank = new_rank;
        best_step = step;
        best = c;
    }

    if (rank_out) {
        *rank_out = best_rank;
    }

    if (backup_out) {
        *backup_out = best_backup;
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
static void rpl_of0_parent_selection(rpl_instance_t *instance)
{
    rpl_neighbour_t *old_pref_parent = rpl_instance_preferred_parent(instance);
    rpl_neighbour_t *backup;
    uint16_t rank;
    rpl_neighbour_t *pref_parent = rpl_of0_select_preferred_parent(instance, old_pref_parent, &backup, &rank);
    uint8_t last_pref;
    rpl_neighbour_list_t NS_LIST_NAME_INIT(parent_list);
    rpl_dodag_version_t *version;
    rpl_dodag_t *dodag;

    if (!pref_parent) {
        tr_debug("No pref_parent (of0) -> set RPL_RANK_INFINITE");
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

    /* Note that we only stretch to accommodate CURRENT candidate backups - not
     * ones that appear later. Thus nodes entering a version earlier may end up
     * with lower rank and less stretch. Losing our only backup may mean we
     * lower our rank, and can't increase again to accommodate the backup.
     *
     * This may need revisiting.
     */
    uint_fast8_t more_successors = rpl_policy_of0_max_backup_successors(instance->domain);
    uint16_t last_cost = rank;
    uint16_t max_stretched_rank = rpl_of0_max_stretched_rank(pref_parent);
    while (backup) {
        /* Stretch rank to accommodate this backup */
        if (rpl_rank_compare(dodag, backup->rank, rank) & (RPL_CMP_GREATER|RPL_CMP_EQUAL)) {
            rank = rpl_rank_next_level(dodag, backup->rank);
            if (rank != RPL_RANK_INFINITE && rank > version->greediness_rank_limit) {
                tr_error("Rank excess during stretch %"PRIu16" > %"PRIu16, rank, version->greediness_rank_limit);
                rank = version->greediness_rank_limit;
                break;
            }
        }
        /* Indicate preference levels - we require exact cost match for equal preference */
        uint16_t backup_cost = rpl_of0_rank_through_neighbour(backup);
        if (backup_cost != last_cost && last_pref < 15) {
            last_pref++;
            last_cost = backup_cost;
        }
        backup->dodag_parent = true;
        backup->dodag_pref = last_pref;
        ns_list_remove(&instance->candidate_neighbours, backup);
        ns_list_add_to_end(&parent_list, backup);
        if (--more_successors) {
            backup = rpl_of0_select_backup_parent(instance, pref_parent, max_stretched_rank);
        } else {
            backup = NULL;
        }
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
