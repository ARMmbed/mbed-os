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

/* rpl_downward.c deals with management of DAO targets, DAO parents, and
 * downward routes within an instance.
 *
 * rpl_domain_t is accessible, but not normally manipulated - all routines in
 * this file works on a specific instance.
 *
 * rpl_instance_t, rpl_dodag_t, rpl_dodag_version_t, rpl_neighbour_t are all accessible.
 *
 * The rpl_dao_target_t type overloads different needs:
 *   1) Non-storing root storage, which needs to record transit addresses.
 *   2) Published targets - published either for ourselves or on behalf of
 *      connected hosts. Sent to either DAO parents or DODAG root, depending
 *      on mode.
 *   3) Storing-mode non-owned targets - those we've received from DAO
 *      children, and need to forward on.
 *
 * Flags settings:
 *
 *   1)  Root non-storing storage:  root=Y   published=N  own=N
 *   2)  Published:                 root=N   published=Y  own=Y/N
 *   3)  Storing storage:           root=N   published=N  own=N
 *
 *
 * We follow the Path Control logic as follows - we can have up to 8 DAO
 * Parents, and they are assigned specific Path Control bits, rather than
 * a whole group of parents being assigned to a whole Path Control subfield
 * as in RFC 6550. Assignments are derived automatically from the preference
 * field set by the objective function, limited by the Path Control Size. There
 * is no specific mechanism for the objective function to control DAO parents
 * separately from DIO parents.
 *
 * Examples:
 *   Parent prefs      Path Control assignments
 *   1                 11111111
 *
 *   1,2               11000000,00111111
 *
 *   1,1               10000000,01111111
 *
 *   1,1,2,3,3         10000000,01000000,00110000,00001000,00000111
 *
 *   1,1,2             10000000,01000000,00111111
 *
 *   1,1,1             (treated as 1,1,2)
 *
 * If a parent is removed in Storing mode, we send it a No-Path for all
 * targets. (9.8.4)
 *
 * If a totally new parent is added to the DAO parent set, we can't really
 * do anything if we don't own the target (Storing mode only). I guess we could
 * send it an update if it is in a disjoint position?
 *
 * If we own the target (as we always will Non-Storing), we can trigger a path
 * sequence update to change parents - addition or removal.
 *
 * Targets contain 8-bit bitfields, which indicates which path control bits have
 * been registered. Parent choice happens at the instant of registration - we
 * just AND together parent and target path control bits.
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
#include "ip6string.h"

#include "Common_Protocols/icmpv6.h"
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

#ifdef HAVE_RPL_ROOT
static void rpl_downward_topo_sort_invalidate(rpl_instance_t *instance);
#endif

#define DEFAULT_DAO_DELAY 10 /* *100ms ticks = 1s */

//#define MINIMUM_DAO_TARGET_REFRESH (5*60) /* seconds */

/* Bit <n> of the PC mask */
#define PCBIT(n) (UINT8_C(0x80) >> (n))

/* The PCS mask */
#define PCSMASK(pcs) ((uint8_t)(0x100 - PCBIT(pcs)))


/*
 *                                 0 1 2 3 4 5 6 7
 *                                +-+-+-+-+-+-+-+-+
 *                                |PC1|PC2|PC3|PC4|
 *                                +-+-+-+-+-+-+-+-+
 *
 *          Figure 27: Path Control Preference Subfield Encoding
 */
void rpl_downward_convert_dodag_preferences_to_dao_path_control(rpl_dodag_t *dodag)
{
    if (!dodag || rpl_dodag_mop(dodag) == RPL_MODE_NO_DOWNWARD) {
        return;
    }

    rpl_instance_t *instance = dodag->instance;
    uint8_t pcs = dodag->config.path_control_size;
    uint_fast8_t bit = 0;

    rpl_neighbour_t *last = NULL;
    rpl_neighbour_t *neighbour = ns_list_get_first(&instance->candidate_neighbours);
    while (neighbour && neighbour->dodag_parent && bit <= pcs) {
        rpl_neighbour_t *next = ns_list_get_next(&instance->candidate_neighbours, neighbour);
        /* Terminate when we hit a non-DODAG parent */
        if (next && !next->dodag_parent) {
            next = NULL;
        }

        /* If non-storing, neighbours if they don't have a known global address */
        if (!neighbour->have_global_address && rpl_dodag_mop(dodag) == RPL_MODE_NON_STORING) {
            neighbour = next;
            continue;
        }

        neighbour->dao_path_control = PCBIT(bit++);
        if (bit <= pcs && next && next->dodag_pref > neighbour->dodag_pref && (bit & 1)) {
            /* Next neighbour is less-preferred, and would join us in the second
             * bit of our subfield. Expand this neighbour to use the second bit,
             * so next will be in the next subfield.
             */
            neighbour->dao_path_control |= PCBIT(bit++);
        }
        last = neighbour;
        neighbour = next;
    }

    /* If we didn't fill the Path Control, expand the final neighbour */
    if (last) {
        while (bit <= pcs) {
            last->dao_path_control |= PCBIT(bit++);
        }
    }
}

static void rpl_downward_target_refresh(rpl_dao_target_t *target)
{
    target->need_seq_inc = true;
    target->info.non_root.pc_assigned = 0;
    target->info.non_root.pc_assigning = 0;
    target->info.non_root.pc_to_retry = 0;
    target->info.non_root.path_lifetime = 0;
}

static bool rpl_instance_parent_selection_ready(rpl_instance_t *instance)
{
    rpl_neighbour_t *neighbour = ns_list_get_first(&instance->candidate_neighbours);
    if (neighbour && neighbour->dodag_parent &&  neighbour->dao_path_control) {
        //We have a Primary parent with Dao patha control
        return true;
    }
    return false;
}

void rpl_downward_neighbour_gone(rpl_instance_t *instance, rpl_neighbour_t *neighbour)
{
    if (neighbour->dao_path_control == 0) {
        return;
    }
    neighbour->old_dao_path_control = neighbour->dao_path_control;
    neighbour->dao_path_control = 0;
    rpl_downward_process_dao_parent_changes(instance);
}

void rpl_downward_process_dao_parent_changes(rpl_instance_t *instance)
{
    uint8_t mop = rpl_instance_mop(instance);
    bool storing;

    switch (mop) {
        case RPL_MODE_NON_STORING:
            storing = false;
            break;
        case RPL_MODE_STORING:
        case RPL_MODE_STORING_MULTICAST:
            storing = true;
            break;
        default:
            return;
    }

    bool bits_removed = false;
    uint8_t bits_added = 0;
    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        if (neighbour->old_dao_path_control != neighbour->dao_path_control) {
            if (neighbour->old_dao_path_control & ~ neighbour->dao_path_control) {
                bits_removed = true;
                break;
            } else {
                /* May as well resend all bits for this parent, not just new */
                bits_added |= neighbour->dao_path_control;
            }
        }
    }
    tr_debug("removed=%x, added=%x", bits_removed, bits_added);
    if (!(bits_removed || bits_added)) {
        return;
    }

    if (storing) {
        /* XXX more complicated - No-Paths to people losing stuff, etc.
         * Need to think a bit about what each parent would have had.
         * Different handling for stuff we own and stuff we don't. Can
         * probably actually unify somewhat, but needs thought.
         */
    } else {
        ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
            if (target->published && target->own) {
                if (bits_removed) {
                    /* Simple - just increment Path Sequence and trigger DAO to root */
                    rpl_downward_target_refresh(target);
                } else {
                    /* Make sure we send the newly-added bits (would previously have been assigned to no-one) */
                    target->info.non_root.pc_assigned &= ~bits_added;
                }
            }
        }
        //GENERATE PARENT Update event
        rpl_control_event(instance->domain, RPL_EVENT_DAO_PARENT_SWITCH);
        rpl_instance_dao_trigger(instance, 0);
    }
}

rpl_dao_target_t *rpl_create_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len, bool root)
{
    rpl_dao_target_t *target = rpl_alloc(sizeof(rpl_dao_target_t));
    if (!target) {
        tr_warn("RPL DAO overflow (target=%s)", trace_ipv6_prefix(prefix, prefix_len));
        return NULL;
    }
    memset(target, 0, sizeof * target);
    bitcopy0(target->prefix, prefix, prefix_len);
    target->instance = instance;
    target->prefix_len = prefix_len;
    target->path_sequence = rpl_seq_init();
    target->root = root;
#ifdef HAVE_RPL_ROOT
    if (root) {
        ns_list_init(&target->info.root.transits);
    }
    rpl_downward_topo_sort_invalidate(instance);
#endif

    ns_list_add_to_end(&instance->dao_targets, target);
    return target;
}

void rpl_delete_dao_target(rpl_instance_t *instance, rpl_dao_target_t *target)
{
    /* XXX For each notified parent, send a No-Path DAO */

    /* TODO - should send a No-Path to root */

    ns_list_remove(&instance->dao_targets, target);

#ifdef HAVE_RPL_ROOT
    if (target->root) {
        ns_list_foreach_safe(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
            ns_list_remove(&target->info.root.transits, transit);
            rpl_free(transit, sizeof * transit);
        }
        ipv6_route_table_remove_info(-1, ROUTE_RPL_DAO_SR, target);
        rpl_downward_topo_sort_invalidate(target->instance);
    }
#endif
    rpl_free(target, sizeof * target);
}

rpl_dao_target_t *rpl_instance_lookup_published_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->published && target->prefix_len == prefix_len &&
                bitsequal(target->prefix, prefix, prefix_len)) {
            return target;
        }
    }
    return NULL;
}

rpl_dao_target_t *rpl_instance_lookup_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->prefix_len == prefix_len &&
                bitsequal(target->prefix, prefix, prefix_len)) {
            return target;
        }
    }
    return NULL;
}

rpl_dao_target_t *rpl_instance_match_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    rpl_dao_target_t *longest = NULL;
    int_fast16_t longest_len = -1;

    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->prefix_len >= longest_len && target->prefix_len <= prefix_len &&
                bitsequal(target->prefix, prefix, target->prefix_len)) {
            longest = target;
            longest_len = target->prefix_len;
            if (longest_len == 128) {
                break;
            }
        }
    }
    return longest;
}
void rpl_instance_delete_published_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len)
{
    rpl_dao_target_t *target = rpl_instance_lookup_published_dao_target(instance, prefix, prefix_len);
    if (target) {
        rpl_delete_dao_target(instance, target);
    }
}

void rpl_instance_publish_dao_target(rpl_instance_t *instance, const uint8_t *prefix, uint8_t prefix_len, uint32_t valid_lifetime, bool own, bool want_descriptor, uint32_t descriptor)
{
    rpl_dao_target_t *target = rpl_instance_lookup_published_dao_target(instance, prefix, prefix_len);
    if (target) {
        int diff = target->lifetime > valid_lifetime ? target->lifetime - valid_lifetime : valid_lifetime - target->lifetime;
        target->lifetime = valid_lifetime;
        if (!own && diff > 60) {
            /* For non-owned targets, publish triggers a refresh */
            rpl_downward_target_refresh(target);
            rpl_instance_dao_trigger(instance, 0);
        }
        return;
    }
    target = rpl_create_dao_target(instance, prefix, prefix_len, false);
    if (!target) {
        tr_warn("Failed to publish DAO target %s", trace_ipv6_prefix(prefix, prefix_len));
        return;
    }
    target->interface_id = -1;
    target->published = true;
    target->own = own;
    target->external = !own;
    target->lifetime = valid_lifetime;
    if (own) {
        target->info.non_root.refresh_timer = 0; /* Auto-refresh */
    } else {
        target->info.non_root.refresh_timer = 0xFFFFFFFF; /* Do not refresh - require republishing */
    }
    target->descriptor_present = want_descriptor;
    target->descriptor = descriptor;
    target->path_control = 0xFF; /* Use as much path control as we can (PCS limits) */
    target->response_wait_time = 0;
    target->active_confirmation_state = false;
    target->trig_confirmation_state = true;
    //Activate allways registration
    instance->pending_neighbour_confirmation = rpl_policy_parent_confirmation_requested();
    tr_debug("New Target %s", trace_ipv6(target->prefix));
    /* Path lifetime left as 0 for now - will be filled in on transmission, along with refresh timer */
    rpl_instance_dao_trigger(instance, 0);

}

void rpl_instance_dao_trigger(rpl_instance_t *instance, uint16_t delay)
{
    if (delay == 0) {
        delay = randLIB_randomise_base(DEFAULT_DAO_DELAY, 0x4000, 0xC000);
    }
    if (instance->delay_dao_timer == 0 || instance->delay_dao_timer > delay) {
        instance->delay_dao_timer = delay;
        //tr_debug("DAO trigger %" PRIu16, delay);
    }
}

/*
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |   Type = 0x05 | Option Length |     Flags     | Prefix Length |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   +                                                               +
 *   |                Target Prefix (Variable Length)                |
 *   .                                                               .
 *   .                                                               .
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *            Figure 25: Format of the RPL Target Option
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |   Type = 0x09 |Opt Length = 4 |           Descriptor
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *          Descriptor (cont.)       |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *       Figure 30: Format of the RPL Target Descriptor Option
 */
static uint8_t *rpl_downward_write_target(uint8_t *ptr, rpl_dao_target_t *target)
{
    uint8_t byte_len = (target->prefix_len + 7u) / 8u;
    *ptr++ = RPL_TARGET_OPTION;
    *ptr++ = 2 + byte_len;
    *ptr++ = 0; // flags
    *ptr++ = target->prefix_len;
    bitcopy0(ptr, target->prefix, target->prefix_len);
    ptr += byte_len;

    if (target->descriptor_present) {
        *ptr++ = RPL_TARGET_DESC_OPTION;
        *ptr++ = 4;
        ptr = common_write_32_bit(target->descriptor, ptr);
    }

    return ptr;
}
/*
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |   Type = 0x06 | Option Length |E|    Flags    | Path Control  |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   | Path Sequence | Path Lifetime |                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
 *   |                                                               |
 *   +                                                               +
 *   |                                                               |
 *   +                        Parent Address*                        +
 *   |                                                               |
 *   +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *        Figure 26: Format of the Transit Information Option
 *
 */
static uint8_t *rpl_downward_write_transit(uint8_t *ptr, rpl_dao_target_t *target, uint8_t path_control, const uint8_t *parent, bool no_path)
{
    *ptr++ = RPL_TRANSIT_OPTION;
    *ptr++ = parent ? 16 + 4 : 4;
    *ptr++ = target->external ? TRANSIT_FLAG_EXTERNAL : 0;
    *ptr++ = path_control;
    *ptr++ = target->path_sequence;
    *ptr++ = no_path ? 0 : target->info.non_root.path_lifetime;
    if (parent) {
        ptr = (uint8_t *) memcpy(ptr, parent, 16) + 16;
    }

    return ptr;
}

static bool rpl_instance_clear_target_pc_to_retry(rpl_instance_t *instance)
{
    bool had_retry = false;
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (!target->root && target->info.non_root.pc_to_retry) {
            target->info.non_root.pc_to_retry = 0;
            had_retry = true;
        }
    }

    return had_retry;
}

/* Find a target that needs updating. For storing mode, after first is found,
 * subsequent must be for the same parent.
 */
static rpl_dao_target_t *rpl_instance_choose_target_to_assign(rpl_instance_t *instance, bool storing, rpl_neighbour_t **parent, uint8_t *path_control, rpl_dao_target_t *t1)
{
retry:
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->root || target == t1) {
            continue;
        }

        /* Check if this target has any unassigned path control bits */
        uint8_t unassigned_pc = target->path_control & ~(target->info.non_root.pc_assigned | target->info.non_root.pc_assigning | target->info.non_root.pc_to_retry);
        if (!unassigned_pc) {
            continue;
        }

        /* If we are looking for a follow-up target to share transit, transit data must match */
        if (t1) {
            uint8_t target_seq = target->path_sequence;
            if (target->need_seq_inc) {
                target_seq = rpl_seq_inc(target_seq);
            }
            if (target->external != t1->external || target->own != t1->own ||
                    target_seq != t1->path_sequence ||
                    target->info.non_root.path_lifetime != t1->info.non_root.path_lifetime) {
                continue;
            }
        }

        /* unassigned_pc is the total path control needing assignment */
        if (!storing) {
            /* In non-storing mode, any number of targets+transits can be output to root in 1 DAO */
            /* Leave unassigned_pc as the entire path control across all parents */
        } else if (*parent == NULL) {
            /* In storing mode, need to choose a parent, if we haven't already */
            /* This is the very first target for the DAO - we now choose a parent that wants
             * the bits, and output that parent's path control so we use it for future targets.
             */
            ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
                if (neighbour->dao_path_control & unassigned_pc) {
                    unassigned_pc &= neighbour->dao_path_control;
                    *path_control = unassigned_pc;
                    *parent = neighbour;
                    return target;
                }
            }
        } else {
            /* This is not the first target - we must be picking subsequent
             * targets with unassigned bits for the parent we chose last time.
             */
            unassigned_pc &= (*parent)->dao_path_control;
            if (!unassigned_pc) {
                continue;
            }
        }

        /* If looking for a follow-up target, final path control must match */
        if (t1) {
            if (unassigned_pc != *path_control) {
                continue;
            }
        } else {
            *path_control = unassigned_pc;
        }
        return target;
    }

    if (!t1) {
        /* If found nothing on initial search, it would appear we're done. However,
         * we were skipping "to_retry" assignments. Clear to_retry flags, and if
         * there were any, retry. We currently retry indefinitely (but with
         * exponential backoff).
         */
        if (rpl_instance_clear_target_pc_to_retry(instance)) {
            if (instance->dao_attempt < 255) {
                ++instance->dao_attempt;
            }
            tr_warn("DAO retry, attempt %d", instance->dao_attempt);
            goto retry;
        }
    }
    return NULL;
}

static void rpl_downward_reset_assigning(rpl_instance_t *instance, uint8_t pcs_mask)
{
    uint8_t parent_mask = 0;
    /* Check to see if we're short of parent mask coverage. If so, remove bits from the mask */
    ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
        parent_mask |= neighbour->dao_path_control;
    }
    pcs_mask &= parent_mask;

    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        target->info.non_root.pc_assigning = 0;
        /* For ease of processing, we allow target path control to have bits
         * beyond the mask. In this pass, we quietly mark those bits as
         * "assigned", as if we've sent them to an invisible parent.
         */
        target->info.non_root.pc_assigned |= (target->path_control & ~pcs_mask);
        target->info.non_root.pc_to_retry &= pcs_mask;
    }
}

static void rpl_instance_unconfirm_parent_info(rpl_instance_t *instance)
{
    ns_list_foreach_safe(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        if (n->dao_path_control == 0) {
            continue;
        }
        n->confirmed = false;

    }
}

static rpl_dao_target_t *rpl_instance_get_pending_target_confirmation_for_address(rpl_instance_t *instance, const uint8_t address[16])
{
    ns_list_foreach_safe(rpl_dao_target_t, n, &instance->dao_targets) {
        if (memcmp(n->prefix, address, 16) == 0) {
            return n;
        }
    }
    return NULL;
}

static rpl_dao_target_t *rpl_instance_get_pending_target_confirmation(rpl_instance_t *instance)
{
    ns_list_foreach_safe(rpl_dao_target_t, n, &instance->dao_targets) {
        if (!n->trig_confirmation_state) {
            continue;
        }
        n->trig_confirmation_state = false;
        n->active_confirmation_state = true;
        instance->wait_response = NULL;
        rpl_instance_unconfirm_parent_info(instance);
        return n;
    }
    return NULL;
}

void rpl_instance_send_address_registration(rpl_instance_t *instance, const uint8_t addr[16])
{
    if (!rpl_instance_parent_selection_ready(instance)) {
        return;
    }


    if (addr) {
        rpl_dao_target_t *target = rpl_instance_get_pending_target_confirmation_for_address(instance, addr);
        if (!target) {
            return;
        }

        if (instance->pending_neighbour_confirmation && (target->active_confirmation_state || target->trig_confirmation_state)) {
            return;
        }
        target->trig_confirmation_state = true;
    } else if (!instance->pending_neighbour_confirmation) {
        ns_list_foreach_safe(rpl_dao_target_t, n, &instance->dao_targets) {
            n->trig_confirmation_state = true;
        }
    }
    instance->pending_neighbour_confirmation = true;
}


/* We are optimised for sending updates to existing targets to current parents;
 * we track the state of what information DAO parents have, and manage the
 * updates together with message coalescing and ack tracking.
 *
 * No-Paths are handled separately, and not tracked - we spit out No-Paths
 * at the moment targets or parents are deleted, 1 attempt only.
 *
 * This function only has to deals with updating our active state.
 */
void rpl_instance_send_dao_update(rpl_instance_t *instance)
{
    instance->delay_dao_timer = 0;

    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    if (!dodag || rpl_dodag_am_root(dodag)) {
        return;
    }

    uint8_t mop = rpl_dodag_mop(dodag);
    bool storing;

    switch (mop) {
        case RPL_MODE_NON_STORING:
            storing = false;
            break;
        case RPL_MODE_STORING:
        case RPL_MODE_STORING_MULTICAST:
            storing = true;
            break;
        default:
            return;
    }

    //Verify that no pending address registartion to parent
    if (instance->pending_neighbour_confirmation) {
        rpl_instance_dao_trigger(instance, 6 * 10);
        return;
    }

    if (instance->dao_in_transit) {
        // Force current DAO timeout to be cut short, then
        // when it times out, it will re-evaluate the situation,
        // and come back back here.
        uint16_t delay = rpl_policy_initial_dao_ack_wait(instance->domain, mop);
        if (instance->dao_retry_timer > delay) {
            instance->dao_retry_timer = delay;
        }
        return;
    }

    if (rpl_policy_dao_retry_count() > 0 && instance->dao_attempt >= rpl_policy_dao_retry_count()) {
        // Check if recovery logic is started
        // after half the retries are done we remove the primary parent
        tr_info("DAO remove primary parent");
        rpl_neighbour_t *neighbour = ns_list_get_first(&instance->candidate_neighbours);
        if (neighbour) {
            rpl_delete_neighbour(instance, neighbour);
        }
        // Set parameters to restart
        instance->dao_in_transit = false;
        instance->dao_attempt = 0;
        instance->dao_retry_timer = 0;
        instance->delay_dao_timer = 0;
        return;
    }

    /* Which parent this DAO will be for if storing */
    rpl_neighbour_t *parent = NULL;

    /* Need our address to publish DAOs of attached hosts */
    const uint8_t *our_addr = NULL;
    if (!storing) {
        ns_list_foreach(rpl_dao_target_t, t, &instance->dao_targets) {
            if (t->published && t->own && t->prefix_len == 128) {
                our_addr = t->prefix;
                break;
            }
        }
    }

    /* To avoid packet overflow, we set a fairly conservative limit of how
     * many targets to put in a message.
     *
     * For storing mode, format is:
     *  Base (4-20)
     *  //Target (4-20, typically 20)
     *  |\Descriptor (0-6)
     *  |(^Repeat if more targets with same transit info)
     *  \ Transit (6)
     *  (^ Repeat if more targets for same parent, with different transit info)
     *
     * For non-storing mode, format is:
     *  Base
     *  //Target (4-20, typically 20)
     *  |\Descriptor (0-6)
     *  |(^Repeat if more targets with same transit info)
     *  | Transit for parent 1 (22)
     *  | Transit for parent 2 (22)
     *  \ Transit...           (22) (max 8 = 176 per target set)
     *  (^ Repeat if more targets with different transit info/parents)
     */


    uint8_t *opts = ns_dyn_mem_temporary_alloc(1280);
    if (!opts) {
        rpl_instance_dao_trigger(instance, 0);
        return;
    }
    uint8_t *ptr = opts;

    rpl_downward_reset_assigning(instance, PCSMASK(dodag->config.path_control_size));

    ns_list_foreach(rpl_dao_target_t, t, &instance->dao_targets) {
        /* Self-published targets can defer path lifetime choice */
        if (t->info.non_root.path_lifetime == 0) {
            uint32_t lifetime = t->lifetime;
            const rpl_dodag_conf_t *conf = rpl_dodag_get_config(dodag);
            uint16_t unit = conf->lifetime_unit;
            uint8_t def = conf->default_lifetime;
            if (lifetime != 0xFFFFFFFF) {
                lifetime = (lifetime + unit - 1) / unit;
            }
            if (lifetime > def) {
                lifetime = def;
            }
            t->info.non_root.path_lifetime = lifetime;
        }
    }


    /* We keep going until size exceeds 768. This gives plenty of slop to fit
     * in a 1280-byte packet.
     */
    while (ptr < opts + 768) {
        uint8_t path_control;
        /* Find a target that needs an update - after the first, it must be for the current parent */
        rpl_dao_target_t *target = rpl_instance_choose_target_to_assign(instance, storing, &parent, &path_control, NULL);
        if (!target) {
            break;
        }

        /* Stupid error case - can't publish non-owned addresses if we don't know our own address */
        if (!storing && !target->own && !our_addr) {
            target->info.non_root.pc_assigned |= path_control;
            continue;
        }

        if (target->need_seq_inc) {
            target->path_sequence = rpl_seq_inc(target->path_sequence);
            target->need_seq_inc = false;
        }

        ptr = rpl_downward_write_target(ptr, target);
        target->info.non_root.pc_assigning = path_control;

        /* Then find more targets with the same transit info - can compress */
        while (ptr < opts + 768) {
            rpl_dao_target_t *t2 = rpl_instance_choose_target_to_assign(instance, storing, &parent, &path_control, target);
            if (!t2) {
                break;
            }
            t2->path_sequence = target->path_sequence;
            t2->need_seq_inc = false;
            t2->info.non_root.pc_assigning = path_control;
            ptr = rpl_downward_write_target(ptr, t2);
        }

        /* Then output the transit information for the original target */
        if (storing) {
            /* Just one transit info */
            ptr = rpl_downward_write_transit(ptr, target, path_control, NULL, false);
        } else if (target->own) {
            /* One transit info for each DAO parent */
            ns_list_foreach(rpl_neighbour_t, neighbour, &instance->candidate_neighbours) {
                if (neighbour->dao_path_control & path_control) {
                    ptr = rpl_downward_write_transit(ptr, target, neighbour->dao_path_control & path_control, neighbour->global_address, false);
                }
            }
        } else {
            /* Attached host - single transit is us */
            ptr = rpl_downward_write_transit(ptr, target, path_control, our_addr, false);
        }
    }

    if (ptr == opts) {
        /* Had nothing... */
        ns_dyn_mem_free(opts);
        return;
    }

    const uint8_t *dst;
    protocol_interface_info_entry_t *cur;
    if (storing) {
        dst = parent->ll_address;
        cur = protocol_stack_interface_info_get_by_id(parent->interface_id);
    } else {
        dst = dodag->id;
        cur = NULL;
    }

    if (instance->dao_attempt > 0) {
        // Start informing problem in routing. This will cause us to select secondary routes when sending the DAO
        tr_info("DAO reachability problem");
        protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_rpl_domain(instance->domain, -1);
        if (interface) {
            ipv6_neighbour_reachability_problem(dst, interface->id);
        }
    }

    bool need_ack = rpl_control_transmit_dao(instance->domain, cur, instance, instance->id, instance->dao_sequence, dodag->id, opts, ptr - opts, dst);
    ns_dyn_mem_free(opts);

    instance->dao_sequence_in_transit = instance->dao_sequence;
    instance->dao_sequence = rpl_seq_inc(instance->dao_sequence);
    instance->requested_dao_ack = need_ack;
    instance->dao_in_transit = true;
    if (instance->dao_attempt < 16) {
        uint32_t t = (uint32_t) rpl_policy_initial_dao_ack_wait(instance->domain, mop) << instance->dao_attempt;
        t = randLIB_randomise_base(t, 0x4000, 0xC000);
        instance->dao_retry_timer = t <= 0xffff ? t : 0xffff;
    } else {
        instance->dao_retry_timer = 0xffff;
    }
    tr_info("DAO tx %u seq attempts %u retry-timer %u", instance->dao_sequence_in_transit, instance->dao_attempt, instance->dao_retry_timer);
}

#if 0
/* This only sends a specific No-Path for one target, to one neighbour, in storing mode */
void rpl_instance_send_dao_no_path(rpl_instance_t *instance, rpl_dao_target_t *target, rpl_neighbour_t *neighbour)
{
    //rpl_control_transmit(domain, NULL, ICMPV6_CODE_RPL_DAO, buf, dst);
    uint8_t dodagid = rpl_instance_id_is_local(instance->id) ? RPL_DAO_FLAG_DODAGID : 0;
    uint8_t base_size = dodagid ? 4 + 16 : 4;
    uint16_t opts_size = 20 + 6 + 6; /* Worst case - Target/Desc/Transit */
    buffer_t *buf = buffer_get(base_size + opts_size);
    if (!buf) {
        /* Retrigger? */
        return;
    }
    uint8_t *ptr = buffer_data_pointer(buf);

    ptr[0] = instance->id;
    ptr[1] = dodagid;
    ptr[2] = 0;
    ptr[3] = instance->dao_sequence = rpl_seq_inc(instance->dao_sequence);
    if (dodagid) {
        memcpy(ptr + 4, dodag->id, 16);
        ptr += 16;
    }
    ptr = rpl_downward_write_target(ptr, target);
    ptr = rpl_downward_write_transit(ptr, target, path_control ?, NULL, false);
    memcpy(buf + base_size, ptr, opts_size);
    const uint8_t *dst;
    if (storing) {
        dst = get_parent_ll_address(parent_idx);
    } else {
        dst = dodag->id;
    }
    rpl_control_transmit(domain, NULL, ICMPV6_CODE_RPL_DAO, buf, neighbour->ll_address);
}
#endif

#ifdef HAVE_RPL_ROOT
static uint_fast8_t rpl_downward_path_control_to_preference(uint8_t pc)
{
    if (pc >= 0x40) {
        return 1;
    } else if (pc >= 0x10) {
        return 2;
    } else if (pc >= 0x04) {
        return 3;
    } else {
        return 4;
    }
}

static rpl_dao_root_transit_t *rpl_downward_add_root_transit(rpl_dao_target_t *target, const uint8_t parent[16], uint8_t path_control)
{
    //rpl_dao_root_transit_t *old_first = ns_list_get_first(&target->info.root.transits);
    rpl_dao_root_transit_t *transit = NULL;
    ns_list_foreach(rpl_dao_root_transit_t, t, &target->info.root.transits) {
        if (addr_ipv6_equal(t->transit, parent)) {
            ns_list_remove(&target->info.root.transits, t);
            transit = t;
            /* Updating existing transit - invalidates costs only */
            rpl_downward_paths_invalidate(target->instance);
            break;
        }
    }

    if (!transit) {
        transit = rpl_alloc(sizeof(rpl_dao_root_transit_t));
        if (!transit) {
            tr_warn("RPL DAO overflow (target=%s,transit=%s)", trace_ipv6_prefix(target->prefix, target->prefix_len), trace_ipv6(parent));
            goto out;
        }
        transit->path_control = 0;
        /* A new transit invalidates the topo sort */
        rpl_downward_topo_sort_invalidate(target->instance);
    }

    transit->target = target;
    transit->path_control |= path_control;
    /* Initial transit cost is 1-4, depending on preference in Path Control.
     * Source routing errors from intermediate nodes may increase this. For
     * directly connected nodes, rpl_downward_compute_paths asks policy
     * to modify according to ETX (or whatever).
     */
    transit->cost = rpl_downward_path_control_to_preference(transit->path_control);

    memcpy(transit->transit, parent, 16);
    ns_list_add_to_end(&target->info.root.transits, transit);

out:
    return ns_list_get_first(&target->info.root.transits);
}

static rpl_dao_target_t *rpl_downward_delete_root_transit(rpl_dao_target_t *target, rpl_dao_root_transit_t *transit)
{
    ns_list_remove(&target->info.root.transits, transit);
    rpl_free(transit, sizeof * transit);
    if (ns_list_is_empty(&target->info.root.transits)) {
        rpl_delete_dao_target(target->instance, target);
        return NULL;
    }

    rpl_downward_topo_sort_invalidate(target->instance);
    return target;
}

void rpl_downward_transit_error(rpl_instance_t *instance, const uint8_t *target_addr, const uint8_t *transit_addr)
{
    ns_list_foreach_safe(rpl_dao_target_t, target, &instance->dao_targets) {
        if (!target->root) {
            continue;
        }
        if (!bitsequal(target_addr, target->prefix, target->prefix_len)) {
            continue;
        }
        ns_list_foreach_safe(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
            if (addr_ipv6_equal(transit_addr, transit->transit)) {
                /* 4 bit Path Control gives us an initial 1-4 cost. We then add something for every error. */
                /* This should make lowest path cost algorithm cycle through alternatives */
                /* When the DAO is refreshed, the cost is reset, so we forget accumulated errors. */
                if (transit->cost < 0xFFFC) {
                    transit->cost += 4;
                } else {
                    transit->cost = 0xFFFF;
                }
                rpl_downward_paths_invalidate(instance);
                instance->srh_error_count++;
                if (rpl_policy_dao_trigger_after_srh_error(instance->domain, (protocol_core_monotonic_time - instance->last_dao_trigger_time) / 10, instance->srh_error_count, ns_list_count(&instance->dao_targets))) {
                    rpl_instance_increment_dtsn(instance);
                }
                break;
            }
        }
    }
}
#endif // HAVE_RPL_ROOT

#ifdef HAVE_RPL_DAO_HANDLING
static bool rpl_downward_process_targets_for_transit(rpl_dodag_t *dodag, bool storing, const uint8_t *src, int8_t interface_id, const uint8_t *target_start, const uint8_t *target_end, const uint8_t *transit_opt, bool *new_info, uint8_t *status)
{
    (void) status;
    const uint8_t *parent;
    /* Check transit length */
    if (storing) {
        if (transit_opt[1] != 4) {
            return false;
        }
        parent = NULL;
    } else {
        if (transit_opt[1] != 20) {
            return false;
        }
        parent = transit_opt + 6;
    }
    bool external = transit_opt[2] & TRANSIT_FLAG_EXTERNAL;
    uint8_t path_control = transit_opt[3];
    uint8_t path_sequence = transit_opt[4];
    uint8_t path_lifetime = transit_opt[5];

    uint32_t lifetime;
    if (path_lifetime == 0xFF) {
        lifetime = 0xFFFFFFFF;
    } else {
        lifetime = (uint32_t) path_lifetime * dodag->config.lifetime_unit;
    }

    rpl_dao_target_t *last_target = NULL;
    while (target_start < target_end) {
        switch (target_start[0]) {
            case RPL_TARGET_OPTION: {
                last_target = NULL;
                uint8_t prefix_len = target_start[3];
                if (prefix_len > 128 || prefix_len > (target_start[1] - 2) * 8) {
                    return false;
                }
                const uint8_t *prefix = target_start + 4;
                rpl_dao_target_t *target = rpl_instance_lookup_dao_target(dodag->instance, prefix, prefix_len);
                if (target) {
                    /* Ignore DAOs for targets we're publishing ourselves */
                    if (target->published) {
                        break;
                    }
                    /* No-Paths are special: version number isn't significant */
                    if (path_lifetime == 0) {
                        tr_info("No-Path %s->%s", parent ? trace_ipv6(parent) : "", trace_ipv6_prefix(prefix, prefix_len));
                        if (storing) {
                            ipv6_route_delete_with_info(prefix, prefix_len, interface_id, src, ROUTE_RPL_DAO, target, 0);
                            /* If we have no DAO routes left for this target, kill it (we don't track who sends individual
                             * DAOs in our target database - the only record we have is in the system routing table
                             */
                            if (!ipv6_route_lookup_with_info(prefix, prefix_len, interface_id, NULL, ROUTE_RPL_DAO, target, 0)) {
                                rpl_delete_dao_target(dodag->instance, target);
                                *new_info = true;
                            }
                        } else {
                            ns_list_foreach(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
                                if (addr_ipv6_equal(transit->transit, parent)) {
                                    target = rpl_downward_delete_root_transit(target, transit);
                                    *new_info = true;
                                    break;
                                }
                            }
                        }
                        /* No more processing on this target - go to next option */
                        break;
                    }
                    /* A real path. Compare path sequence. */
                    rpl_cmp_t seq_cmp = rpl_seq_compare(path_sequence, target->path_sequence);

                    bool accept;
                    if (storing) {
                        /* For storing, follow the letter of spec. Can't afford for old route propagation to happen. */
                        accept = seq_cmp & (RPL_CMP_GREATER | RPL_CMP_EQUAL);
                    } else {
                        /* Lollipop counters don't necessarily work brilliantly after reboot - the path
                         * sequence causes more problems than it solves for non-storing modes, where it's
                         * the actual target owner sending the info. We don't have to worry about the
                         * network storing stale data. So we're more flexible.
                         */
                        if (path_sequence >= 128) {
                            /* Always accept anything with sequence in the lollipop counter restart region */
                            accept = true;
                            /* Also, we always refresh lifetime, even if sequence number is the same as stored */
                            target->lifetime = lifetime;
                        } else if (external) {
                            /* ZigBee IP requires external targets to use latest info and ignore sequence -
                             * publishers can't really keep them in sync. We go along with this.
                             */
                            accept = true;
                            target->lifetime = lifetime;
                        } else {
                            accept = seq_cmp & (RPL_CMP_GREATER | RPL_CMP_EQUAL);
                        }
                    }
                    if (!accept) {
                        tr_info("Ignoring stale path %s->%s (seq=%d vs %d)", parent ? trace_ipv6(parent) : "", trace_ipv6_prefix(prefix, prefix_len), path_sequence, target->path_sequence);
                        break;
                    }
                    /* If path sequence is different, we clear existing transits for this target */
                    if (!(seq_cmp & RPL_CMP_EQUAL)) {
                        if (target->root) {
                            ns_list_foreach_safe(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
                                ns_list_remove(&target->info.root.transits, transit);
                                rpl_free(transit, sizeof * transit);
                            }
                        }
                        if (storing) {
                            ipv6_route_table_remove_info(-1, ROUTE_RPL_DAO, target);
                        }
                        target->path_control = 0;
                        target->path_sequence = path_sequence;
                        target->lifetime = lifetime;
                        *new_info = true;
                    }
                    /* Then we proceed to add this transit to the target below */
                } else if (path_lifetime != 0) {
                    target = rpl_create_dao_target(dodag->instance, prefix, prefix_len, !storing);
                    if (target) {
                        target->path_sequence = path_sequence;
                        target->lifetime = lifetime;
                    }
                }

                /* No-Paths don't reach here - we break out above */
                if (target) {
                    if (path_control & ~ target->path_control) {
                        target->path_control |= path_control;
                        *new_info = true;
                    }
                    target->external = external;
                    target->interface_id = interface_id;
                    if (storing) {
                        target->info.non_root.path_lifetime = path_lifetime;
                    }

                    if (storing) {
                        /* In Storing mode, add a route immediately */
                        ipv6_route_add_with_info(prefix, prefix_len, interface_id, src, ROUTE_RPL_DAO, target, 0, target->lifetime, 0);
                    } else {
                        rpl_dao_root_transit_t *transit = rpl_downward_add_root_transit(target, parent, path_control);
#if 0
                        /* In Non-Storing mode, add the transit to the target, and we'll re-evaluate system routes later */
                        ipv6_route_table_remove_info(-1, ROUTE_RPL_DAO_SR, target);
                        if (transit_opt) {
                            if (protocol_interface_address_compare(parent) == 0) {
                                /* If we're transit, it's on-link */
                                ipv6_route_add_with_info(prefix, prefix_len, interface_id, NULL, ROUTE_RPL_DAO_SR, target, 0, target->lifetime, 0);
                            } else {
                                ipv6_route_add_with_info(prefix, prefix_len, interface_id, parent, ROUTE_RPL_DAO_SR, target, 0, target->lifetime, 0);
                            }
                        }
#else
                        if (transit) {
                            ipv6_route_add_with_info(prefix, prefix_len, interface_id, ADDR_UNSPECIFIED, ROUTE_RPL_DAO_SR, target, 0, target->lifetime, 0);
                        }
#endif
                    }
                }

                last_target = target;
                break;
            }
            case RPL_TARGET_DESC_OPTION:
                if (target_start[1] == 4 && last_target) {
                    last_target->descriptor_present = true;
                    last_target->descriptor = common_read_32_bit(target_start + 2);
                }
                break;
            case RPL_PAD1_OPTION:
                target_start++;
                continue;
        }
        target_start += 2 + target_start[1];
    }

    return true;
}
#endif // HAVE_RPL_DAO_HANDLING

#ifdef HAVE_RPL_ROOT
/* Link the graph ready for routing. "Canonical" database information stores
 * transits per target - in effect edges from children to parents.
 * For our path finding we need to match transits by prefix - eg all 2002::xx
 * transits might go via one 2002::/64 target - and we want to turn it around
 * so that our edges point from parents to children.
 *
 * This fills in (from scratch):
 *
 *     transit::parent   :-> matched target, or NULL for disconnected/DODAG root (us)
 *
 *     target::connected :=  true for targets directly connected to DODAG root
 *
 *     target::info.root.children
 * and instance::root_children    := list of transits with this target/root as parent
 *
 *     target::info.root.cost := number of transits connected to parent targets (connections to root not included)
 */
static void rpl_downward_link_transits_to_targets(rpl_instance_t *instance)
{
    ns_list_init(&instance->root_children);
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        target->info.root.cost = 0;
        target->connected = false;
        ns_list_init(&target->info.root.children);
    }
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        ns_list_foreach(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
            if (protocol_interface_address_compare(transit->transit) == 0) {
                /* It points to us (the DODAG root) - mark this with NULL */
                transit->parent = NULL;
                target->connected = true;
                /* Links to the root don't count as incoming transits */
                ns_list_add_to_end(&instance->root_children, transit);
            } else {
                transit->parent = rpl_instance_match_dao_target(instance, transit->transit, 128);
                if (transit->parent) {
                    target->info.root.cost++;
                    ns_list_add_to_end(&transit->parent->info.root.children, transit);
                }
            }
        }
    }
}

/* Subroutine for topo sort - an edge has been removed, either because the
 * parent is moved to the sorted list, or we're breaking a loop. Update
 * the "incoming edges" count in the child, and maybe move it to the top nodes list.
 */
static void rpl_downward_topo_sort_edge_removed(rpl_dao_root_transit_t *transit, rpl_dao_target_list_t *graph, rpl_dao_target_list_t *top_nodes)
{
    rpl_dao_target_t *child = transit->target;

    if (child->info.root.cost > 0) {
        child->info.root.cost--;
    } else {
        tr_err("topo sort count error");
    }
    /* If this child has no more incoming, move it onto the "top nodes" list */
    if (child->info.root.cost == 0) {
        ns_list_remove(graph, child);
        ns_list_add_to_end(top_nodes, child);
    }
}

static void rpl_downward_topo_sort_break_loop(rpl_dao_target_list_t *graph, rpl_dao_target_list_t *top_nodes)
{
    rpl_dao_root_transit_t *kill_transit = NULL;

    /* Don't expect this to happen much, so not particularly optimised - we kill 1 transit to break the loop */
    /* First choose a target with the most transits - we want to delete spare ones. */
    ns_list_foreach(rpl_dao_target_t, target, graph) {
        ns_list_foreach(rpl_dao_root_transit_t, transit, &target->info.root.children) {
            if (!kill_transit) {
                goto kill_candidate;
            }
            /* Prefer to delete transits to already-connected targets. In a
             * simple scenario with one loop, for example:
             *
             *     Root--->A--->B--->C--->D
             *                   ^------ /
             *
             * the initial pass will prune to:
             *
             *                  B--->C--->D
             *                   ^-------/
             *
             * and only B will be marked "connected". D->B is the link to
             * kill to get everyone else connected. (Deleting B->C would
             * leave C unconnected. Deleting C->D would leave D unconnected.)
             *
             * With alternate paths like:
             *
             *              /--------v
             *     Root--->A--->B--->C--->D
             *                   ^------ /
             *
             * it would prune to
             *
             *                  B--->C--->D
             *                   ^------ /
             *
             * but this time both B and C would be connected. Deleting
             * either D->B or B->C would result in valid acyclic graphs, although
             * an optimal link might be lost.
             *
             *              /--------v
             *     Root--->A--->B--->C--->D
             *
             *              /--------v
             *     Root--->A--->B    C--->D
             *                   ^-------/
             */
            if (!kill_transit->target->connected && transit->target->connected) {
                goto kill_candidate;
            } else if (kill_transit->target->connected && !transit->target->connected) {
                continue;
            }

            /* Prefer to kill a higher-cost link */
            if (kill_transit->cost < transit->cost) {
                goto kill_candidate;
            } else if (kill_transit->cost > transit->cost) {
                continue;
            }
kill_candidate:
            kill_transit = transit;
        }
    }

    /* Hopefully we killed found transit to a connected node to kill. */
    /* If we didn't, it means we're on an isolated island, so we're dooomed anyway... */

    /* This removes it from our routing graph, but not from the master database */
    ns_list_remove(&kill_transit->parent->info.root.children, kill_transit);
    rpl_downward_topo_sort_edge_removed(kill_transit, graph, top_nodes);
}

/* Topologically sort instance->dao_targets - closest to border router placed at start */
static void rpl_downward_topo_sort(rpl_instance_t *instance)
{
    if (instance->root_topo_sort_valid) {
        return;
    }

    rpl_downward_link_transits_to_targets(instance);

    rpl_dao_target_list_t sorted = NS_LIST_INIT(sorted);
    rpl_dao_target_list_t top_nodes = NS_LIST_INIT(top_nodes);

    /* Find all the topmost targets - most should have been marked "connected", ie they
     * have exactly 1 link to the DODAG root. Some may be disconnected. */
    ns_list_foreach_safe(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->info.root.cost == 0) {
            ns_list_remove(&instance->dao_targets, target);
            ns_list_add_to_end(&top_nodes, target);
        }
    }

retry_after_loop_break:
    ;
    /* Run the sort - targets are pulled off 'instance->dao_targets', and placed onto 'sorted' */

    rpl_dao_target_t *target;
    while ((target = ns_list_get_first(&top_nodes)) != NULL) {
        /* Take any topmost node, place on the end of the sorted list */
        ns_list_remove(&top_nodes, target);
        ns_list_add_to_end(&sorted, target);

        /* Decrement incoming link count on all children */
        ns_list_foreach(rpl_dao_root_transit_t, transit, &target->info.root.children) {
            rpl_downward_topo_sort_edge_removed(transit, &instance->dao_targets, &top_nodes);
            /* If this node is connected, the child is connected */
            if (target->connected) {
                transit->target->connected = true;
            }

        }
    }

    if (!ns_list_is_empty(&instance->dao_targets)) {
        /* Didn't manage to empty the graph, so we have a loop - not a DAG */
        do {
            rpl_downward_topo_sort_break_loop(&instance->dao_targets, &top_nodes);
        } while (ns_list_is_empty(&top_nodes));
        goto retry_after_loop_break;
    }

    /* Transfer sorted list back onto instance (dao_targets must be empty at this point) */
    ns_list_concatenate(&instance->dao_targets, &sorted);
    instance->root_topo_sort_valid = true;
}

/* Call when topo sort may have changed (or child links broken) - this invalidates everything */
static void rpl_downward_topo_sort_invalidate(rpl_instance_t *instance)
{
    instance->root_topo_sort_valid = false;
    rpl_downward_paths_invalidate(instance);
}

static void rpl_downward_update_path_cost_to_children(rpl_dao_root_transit_children_list_t *children, uint16_t parent_cost)
{
    ns_list_foreach(rpl_dao_root_transit_t, transit, children) {
        rpl_dao_target_t *child = transit->target;
        uint16_t transit_cost = transit->cost;
        /* For directly-connected paths, modify for ETX or similar */
        if (parent_cost == 0 && child->prefix_len == 128) {
            transit_cost = rpl_policy_modify_downward_cost_to_root_neighbour(child->instance->domain, child->interface_id, child->prefix, transit->cost);
        }
        if (child->info.root.cost > parent_cost + transit_cost) {
            /* Note new best cost to child, and make this transit the child's first/best */
            child->info.root.cost = parent_cost + transit_cost;
            if (transit != ns_list_get_first(&child->info.root.transits)) {
                ns_list_remove(&child->info.root.transits, transit);
                ns_list_add_to_start(&child->info.root.transits, transit);
            }
        }
    }
}

void rpl_downward_compute_paths(rpl_instance_t *instance)
{
    if (instance->root_paths_valid) {
        return;
    }

    /* First get targets into a topological sort - also breaks loops */
    rpl_downward_topo_sort(instance);

    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        target->info.root.cost = 0xFFFFFFFF;
    }

    /* First process the direct root->child links, giving inital link costs */
    rpl_downward_update_path_cost_to_children(&instance->root_children, 0);

    /* Now process every node in turn */
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        /* Update connected flag - could be invalid due to node removal that didn't redo topo sort */
        target->connected = target->info.root.cost != 0xFFFFFFFF;
        if (target->connected) {
            rpl_downward_update_path_cost_to_children(&target->info.root.children, target->info.root.cost);
        }
    }

    instance->root_paths_valid = true;
}

/* Called when path costs may have changed (but not topo sort) */
void rpl_downward_paths_invalidate(rpl_instance_t *instance)
{
    instance->root_paths_valid = false;
    rpl_data_sr_invalidate();
}
#endif // HAVE_RPL_ROOT

#ifdef HAVE_RPL_DAO_HANDLING
bool rpl_instance_dao_received(rpl_instance_t *instance, const uint8_t src[16], int8_t interface_id, bool multicast, const uint8_t *opts, uint16_t opts_len, uint8_t *status)
{
    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    if (!dodag) {
        return false;
    }
    if (multicast) {
        /* We don't handle multicast DAOs at all yet */
        return false;
    }
    bool storing;
    switch (rpl_dodag_mop(dodag)) {
        default:
        case RPL_MODE_NO_DOWNWARD:
            return false;
        case RPL_MODE_NON_STORING:
            if (!rpl_dodag_am_root(dodag) || addr_is_ipv6_link_local(src)) {
                return false;
            }
            storing = false;
            break;
        case RPL_MODE_STORING:
        case RPL_MODE_STORING_MULTICAST:
            if (instance->current_rank == RPL_RANK_INFINITE || !addr_is_ipv6_link_local(src)) {
                return false;
            }
            storing = true;
            break;
    }

    const uint8_t *start = opts, *end = opts + opts_len;

    /* Basic format is ("group of targets", "group of transits"), repeated.
     * All the transits apply to all the preceding targets. So parsing is:
     *    1) When we see next target, and target group is closed, remember start of target group, open end.
     *    2) When we see a transit, mark end of target group if not already closed.
     *    3) Then for any transit, process the entire target group.
     */
    const uint8_t *target_start = NULL, *target_end = opts;

    bool new_info = false;
    *status = 0;

    while (start < end) {
        switch (start[0]) {
            case RPL_TARGET_OPTION:
                if (target_end) {
                    target_start = start;
                    target_end = NULL;
                }
                break;
            case RPL_TRANSIT_OPTION:
                if (target_start) {
                    if (!target_end) {
                        target_end = start;
                    }
                    rpl_downward_process_targets_for_transit(dodag, storing, src, interface_id, target_start, target_end, start, &new_info, status);
                } else {
                    tr_warn("Transit without Targets");
                }
                break;
            case RPL_PAD1_OPTION:
                start++;
                continue;
        }
        start += 2 + start[1];
    }

    if (new_info && storing) {
        rpl_instance_dao_trigger(instance, 0);
    }

    return true;
}
#endif // HAVE_RPL_DAO_HANDLING

void rpl_instance_dao_acked(rpl_instance_t *instance, const uint8_t src[16], int8_t interface_id, uint8_t dao_sequence, uint8_t status)
{
    if (!instance->dao_in_transit || dao_sequence != instance->dao_sequence_in_transit) {
        return;
    }

    if (src) {
        ipv6_neighbour_reachability_confirmation(src, interface_id);
    }

    bool retry = false;
    if (status == 0) {
        tr_debug("DAO-ACK RX"); /* Some tests rely on this debug */
    } else {
        if (src) {
            tr_warn("DAO rejection from %s: status=%d", trace_ipv6(src), status);
        } else {
            tr_warn("DAO timeout");
            retry = true;
        }
    }

    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    const rpl_dodag_conf_t *conf = dodag ? rpl_dodag_get_config(dodag) : NULL;
    instance->dao_in_transit = false;
    instance->dao_retry_timer = 0;
    if (!retry) {
        instance->dao_attempt = 0;
    }

    bool more_to_do = false;
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->root) {
            continue;
        }
        tr_debug("tgt %s - pc %02x, assigned %02x, assigning %02x, life %02x, rfr %"PRIu32,
                 trace_ipv6_prefix(target->prefix, target->prefix_len),
                 target->path_control,
                 target->info.non_root.pc_assigned,
                 target->info.non_root.pc_assigning,
                 target->info.non_root.path_lifetime,
                 target->info.non_root.refresh_timer);
        target->info.non_root.pc_assigning &= target->path_control;
        if (target->info.non_root.pc_assigning) {
            if (!retry) {
                target->info.non_root.pc_assigned |= target->info.non_root.pc_assigning;
            } else {
                target->info.non_root.pc_to_retry |= target->info.non_root.pc_assigning;
            }
            target->info.non_root.pc_assigning = 0;
        }
        target->info.non_root.pc_assigned &= target->path_control;
        if (target->info.non_root.pc_assigned != target->path_control) {
            more_to_do = true;
        } else {
            if (target->published && target->info.non_root.refresh_timer == 0) {
                uint32_t t;
                if (conf && target->info.non_root.path_lifetime != 0xFF) {
                    t = (uint32_t) target->info.non_root.path_lifetime * conf->lifetime_unit;
                    /* Refresh between 1/2 and 3/4 of lifetime */
                    t = randLIB_randomise_base(t, 0x4000, 0x6000);
                } else {
                    t = 0xFFFFFFFF;
                }
#ifdef MINIMUM_DAO_TARGET_REFRESH
                if (t > MINIMUM_DAO_TARGET_REFRESH) {
                    t = randLIB_randomise_base(MINIMUM_DAO_TARGET_REFRESH, 0x7333, 0x8CCD); /* +/- 10% */
                }
#endif
                target->info.non_root.refresh_timer = t;
                tr_debug("set rfr to %"PRIu32, t);
            }
        }
    }

    if (more_to_do) {
        rpl_instance_dao_trigger(instance, 1);
    } else {
        rpl_control_event(instance->domain, RPL_EVENT_DAO_DONE);
    }
}

void rpl_instance_dao_request(struct rpl_instance *instance, struct rpl_neighbour *neighbour)
{
    uint8_t pc_mask = neighbour ? neighbour->dao_path_control : 0xFF;

    if (!pc_mask) {
        return;
    }

    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {
        if (!target->root) {
            target->info.non_root.pc_assigned &= ~pc_mask;
            target->info.non_root.pc_to_retry &= ~pc_mask;
        }
    }

    rpl_instance_dao_trigger(instance, 0);
}


void rpl_downward_dao_slow_timer(rpl_instance_t *instance, uint16_t seconds)
{
    if (!instance) {
        return;
    }
    /* Process lifetimes on all targets for expiry */
    ns_list_foreach_safe(rpl_dao_target_t, target, &instance->dao_targets) {
        if (target->lifetime == 0xFFFFFFFF) {
            continue;
        }
        if (target->lifetime > seconds) {
            target->lifetime -= seconds;
            continue;
        }
        rpl_delete_dao_target(instance, target);
    }

    /* Perform target auto-refresh for published targets */
    ns_list_foreach_safe(rpl_dao_target_t, target, &instance->dao_targets) {
        if (!target->published) {
            continue;
        }
        if (target->info.non_root.refresh_timer == 0xFFFFFFFF || target->info.non_root.refresh_timer == 0) {
            continue;
        }
        if (target->info.non_root.refresh_timer > seconds) {
            target->info.non_root.refresh_timer -= seconds;
            continue;
        }
        /* Refresh the DAO target */
        target->info.non_root.refresh_timer = 0;
        rpl_downward_target_refresh(target);
        rpl_instance_dao_trigger(instance, 0);
    }
}

void rpl_downward_dao_timer(rpl_instance_t *instance, uint16_t ticks)
{
    if (instance->dao_retry_timer) {
        if (instance->dao_retry_timer > ticks) {
            instance->dao_retry_timer -= ticks;
        } else {
            instance->dao_retry_timer = 0;
            if (!instance->requested_dao_ack) {
                /* Act as if ACK arrived at first retry point (gives them
                 * time to send a failure).
                 */
                rpl_instance_dao_acked(instance, NULL, -1, instance->dao_sequence_in_transit, 0);
            } else {
                rpl_instance_dao_acked(instance, NULL, -1, instance->dao_sequence_in_transit, 0xFF);
            }
        }
    }

    if (instance->delay_dao_timer) {
        if (instance->delay_dao_timer > ticks) {
            instance->delay_dao_timer -= ticks;
        } else {
            instance->delay_dao_timer = 0;
            rpl_instance_send_dao_update(instance);
        }
    }
}

void rpl_downward_print_instance(rpl_instance_t *instance, route_print_fn_t *print_fn)
{
    if (ns_list_is_empty(&instance->dao_targets)) {
        return;
    }
    print_fn("DAO Targets:");
    if (rpl_instance_am_root(instance)) {
        rpl_downward_compute_paths(instance);
    }
    ns_list_foreach(rpl_dao_target_t, target, &instance->dao_targets) {

        char str_buf[44];
        ip6_prefix_tos(target->prefix, target->prefix_len, str_buf);
#ifdef HAVE_RPL_ROOT
        if (target->root) {
            print_fn("  %-40s %02x seq=%d%s cost=%"PRIu32"%s%s%s",
                     str_buf,
                     target->path_control, target->path_sequence, target->need_seq_inc ? "+" : "",
                     target->info.root.cost,
                     target->published ? " (pub)" : "",
                     target->external ? " (E)" : "",
                     target->connected ? "" : " (disconnected)");
            ns_list_foreach(rpl_dao_root_transit_t, transit, &target->info.root.transits) {
                // Reuse str_buf as it's no longer needed and it's large enough for ROUTE_PRINT_ADDR_STR_FORMAT.
                print_fn("    ->%-36s %02x cost=%"PRIu16, ROUTE_PRINT_ADDR_STR_FORMAT(str_buf, transit->transit), transit->path_control, transit->cost);
            }
        } else
#endif
        {
            print_fn("  %-40s %02x seq=%d%s%s%s",
                     str_buf,
                     target->path_control, target->path_sequence, target->need_seq_inc ? "+" : "",
                     target->published ? " (pub)" : "",
                     target->external ? " (E)" : "");
        }
    }
}

rpl_dao_target_t *rpl_instance_get_active_target_confirmation(rpl_instance_t *instance)
{
    ns_list_foreach_safe(rpl_dao_target_t, n, &instance->dao_targets) {
        if (!n->active_confirmation_state) {
            continue;
        }
        return n;

    }
    return NULL;
}

static rpl_neighbour_t *rpl_instance_get_unconfirmed_parent_info(rpl_instance_t *instance)
{
    ns_list_foreach_safe(rpl_neighbour_t, n, &instance->candidate_neighbours) {
        if (n->dao_path_control != 0 && !n->confirmed) {
            return n;
        }

    }
    return NULL;
}

static bool rpl_instance_push_address_registration(protocol_interface_info_entry_t *interface, rpl_neighbour_t *neighbour, if_address_entry_t *addr)
{
    aro_t aro;

    aro.status = ARO_SUCCESS;
    aro.present = true;
    aro.lifetime = (addr->valid_lifetime / 60) + 1;
    memcpy(aro.eui64, interface->mac, 8);

    buffer_t *buf = icmpv6_build_ns(interface, neighbour->ll_address, addr->address, true, false, &aro);
    if (!buf) {
        return false;
    }
    tr_debug("Send ARO %s to %s", trace_ipv6(addr->address), trace_ipv6(neighbour->ll_address));
    protocol_push(buf);
    return true;
}
static if_address_entry_t *rpl_interface_addr_get(protocol_interface_info_entry_t *interface, const uint8_t addr[16])
{
    ns_list_foreach(if_address_entry_t, entry, &interface->ip_addresses) {
        if (memcmp(entry->address, addr, 16) == 0) {
            return entry;
        }
    }
    return NULL;
}

static void rpl_instance_address_registration_cancel(rpl_instance_t *instance)
{
    ns_list_foreach_safe(rpl_dao_target_t, n, &instance->dao_targets) {
        n->active_confirmation_state = false;
        n->trig_confirmation_state = false;
        n->response_wait_time = 0;
    }

    instance->wait_response = NULL;
    instance->pending_neighbour_confirmation = false;
    instance->delay_dao_timer = 0;
}

void rpl_instance_parent_address_reg_timer_update(rpl_instance_t *instance, uint16_t seconds)
{
    if (!instance->pending_neighbour_confirmation) {
        return; //No need validate any confirmation
    }

    //Verify that we have selected parent and it have a dao path control
    if (!rpl_instance_parent_selection_ready(instance)) {
        rpl_instance_address_registration_cancel(instance);
        return;
    }

    //Get Pendig active target
    rpl_dao_target_t *dao_target = rpl_instance_get_active_target_confirmation(instance);
    if (!dao_target) {
        dao_target = rpl_instance_get_pending_target_confirmation(instance);
        if (!dao_target) {
            instance->pending_neighbour_confirmation = false;
            return;
        }

        tr_debug("Register Address to parent %s", trace_ipv6(dao_target->prefix));
    }

    if (instance->wait_response) {
        if (seconds < dao_target->response_wait_time) {
            //Must Wait response time untill finish
            dao_target->response_wait_time -= seconds;
            return;
        }
        dao_target->response_wait_time = 0;
        instance->wait_response = NULL;
    }

    //Get Next Parent for confirmation
    rpl_neighbour_t *neighbour = rpl_instance_get_unconfirmed_parent_info(instance);
    if (!neighbour) {
        dao_target->active_confirmation_state = false;
        return;
    }

    //Get address and buffer
    protocol_interface_info_entry_t *interface = protocol_stack_interface_info_get_by_id(neighbour->interface_id);
    if (!interface) {
        rpl_instance_address_registration_cancel(instance);
        return;
    }

    if_address_entry_t *address = rpl_interface_addr_get(interface, dao_target->prefix);
    if (!address) {
        rpl_instance_address_registration_cancel(instance);
        return;
    }


    if (rpl_instance_push_address_registration(interface, neighbour, address)) {
        instance->wait_response = neighbour;
        dao_target->response_wait_time = 5;
    }

}

void rpl_instance_address_registration_done(protocol_interface_info_entry_t *interface, rpl_instance_t *instance, rpl_neighbour_t *neighbour, uint8_t status)
{

    if (!instance->pending_neighbour_confirmation) {
        return;
    }

    rpl_dao_target_t *dao_target = rpl_instance_get_active_target_confirmation(instance);
    if (!dao_target || instance->wait_response != neighbour) {
        return;
    }

    tr_debug("Address %s register to %s", trace_ipv6(dao_target->prefix), trace_ipv6(neighbour->ll_address));

    if (status == SOCKET_TX_DONE) {
        /* State_timer is 1/10 s. Set renewal to 75-85% of lifetime */
        if_address_entry_t *address = rpl_interface_addr_get(interface, dao_target->prefix);
        if (address && address->source != ADDR_SOURCE_DHCP) {
            address->state_timer = (address->preferred_lifetime * randLIB_get_random_in_range(75, 85) / 10);
        }
        neighbour->confirmed = true;
        dao_target->response_wait_time = 6;
    } else {
        tr_error("Address registration failed");
        rpl_delete_neighbour(instance, neighbour);
        rpl_instance_address_registration_cancel(instance);
    }
}

#endif /* HAVE_RPL */
