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
/*
 * icmpv6_radv.c
 *
 * ICMPv6 Router Advertisement transmission
 *
 * This file handles all RS reception, and deals with the timing of all RAs.
 * The actual contents of the RAs are still handled separately by
 * nd_router_object.c for 6LoWPAN and protocol_ipv6.c for Ethernet.
 *
 * We handle multicast transmission - once initially scheduled, these will
 * automatically be repeated, and will be accelerated by RS reception if
 * rtr_adv_unicast_to_rs is false.
 *
 * If rtr_adv_unicast_to_rs is true, then each RS gets a unicast RA response,
 * without affecting any scheduled multicast transmissions.
 *
 * For 6LR operation, with RFC 6775 multihop distribution, we allow the system
 * to queue multiple transmissions to each destination - 1 per border router
 * (ABRO 6LBR). These are independently scheduled and rate limited; 1 RS
 * will lead to multiple independently randomised responses, 1 per ABRO, and
 * per-ABRO multicast transmissions will be independently randomly scheduled.
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "ns_list.h"
#include "randLIB.h"
#include <string.h>
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/ND/nd_router_object.h" // for nd_ra_timing()
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_radv.h"

#ifdef RADV_TX

#define TRACE_GROUP "RAdv"

typedef struct icmp_queued_ra {
    uint8_t         addr[16];           /* destination address */
    uint8_t         abro[16];           /* RFC 6775 ABRO 6LBR address (or ADDR_UNSPECIFIED if no ABRO) */
    bool            rs_triggered;       /* was queued by an RS */
    uint16_t        ticks;              /* ticks until transmission (relative to last list entry) */
    protocol_interface_info_entry_t *interface;
    ns_list_link_t  link;
} icmp_queued_ra_t;

static NS_LIST_DEFINE(icmp_ra_queue, icmp_queued_ra_t, link);

void icmpv6_radv_init(protocol_interface_info_entry_t *cur)
{
    /* Initialise basic static config */
    cur->adv_send_advertisements = false;
    cur->max_ra_delay_time = 5;
    cur->min_delay_between_ras = 30;
    cur->min_rtr_adv_interval = 2000; // 3 minutes 20 seconds (max/3)
    cur->max_rtr_adv_interval = 6000; // 10 minutes
    cur->max_initial_rtr_adv_interval =  160; // 16 seconds
    cur->max_initial_rtr_advertisements = 3;
    cur->adv_link_mtu = 0;
    cur->adv_cur_hop_limit = 0;
    cur->adv_reachable_time = 0;
    cur->adv_retrans_timer = 0;
    cur->rtr_adv_unicast_to_rs = false;
    cur->rtr_adv_flags = 0;
    cur->adv_copy_heard_flags = false;

    /* Initialise timing info for local RAs */
    cur->ra_timing.rtr_adv_last_send_time = protocol_core_monotonic_time - 0x10000;
    cur->ra_timing.initial_rtr_adv_count = 0;
}

/* Locate the timing info for a given source - we currently have a split in that
 * specified ABROs get their timing info stored over in the associated nd_router_t,
 * and if there's no ABRO, it's in the interface structure.
 *
 * When/if the "advert contents" gets centralised, this info can be stored there
 * too.
 */
static ipv6_ra_timing_t *icmpv6_ra_timing_lookup(protocol_interface_info_entry_t *cur, const uint8_t abro[16])
{
    if (addr_is_ipv6_unspecified(abro)) {
        return &cur->ra_timing;
    }

    return nd_ra_timing(abro);
}

/* Queue an RA - on entry new_ra->ticks is ticks from now; we need to
 * insert it into the list which has relative ticks, so it gets decremented
 * by the ticks of all preceding queue members. */
void icmpv6_queue_ra(icmp_queued_ra_t *new_ra)
{
    ns_list_foreach(icmp_queued_ra_t, ra, &icmp_ra_queue) {
        if (ra->ticks > new_ra->ticks) {
            ns_list_add_before(&icmp_ra_queue, ra, new_ra);
            ra->ticks -= new_ra->ticks;
            return;
        }
        new_ra->ticks -= ra->ticks;
    }

    ns_list_add_to_end(&icmp_ra_queue, new_ra);
}

void icmpv6_unqueue_ra(icmp_queued_ra_t *ra)
{
    icmp_queued_ra_t *before = ns_list_get_next(&icmp_ra_queue, ra);

    if (before) {
        before->ticks += ra->ticks;
    }

    ns_list_remove(&icmp_ra_queue, ra);
}

/* If there's an advert already queued to the specified destination for the specified ABRO, return it, and its scheduled time */
static icmp_queued_ra_t *icmpv6_queued_ra_lookup(const uint8_t *dest_addr, const uint8_t *abro, int8_t interface_id, uint16_t *abstime_out)
{
    uint16_t abstime = 0;
    ns_list_foreach(icmp_queued_ra_t, ra, &icmp_ra_queue) {
        abstime += ra->ticks;
        if (interface_id == ra->interface->id && addr_ipv6_equal(dest_addr, ra->addr) && addr_ipv6_equal(abro, ra->abro)) {
            if (abstime_out) {
                *abstime_out = abstime;
            }
            return ra;
        }
    }

    return NULL;
}

/* Trigger a single RA from an RS - must be called multiple times if we have multiple ABROs */
void icmpv6_trigger_ra_from_rs(protocol_interface_info_entry_t *cur, const uint8_t dest[16], const uint8_t abro[16])
{
    uint16_t scheduled;

    /* Check if we've already scheduled an RA to this destination for this ABRO */
    icmp_queued_ra_t *ra = icmpv6_queued_ra_lookup(dest, abro, cur->id, &scheduled);

    /* Delay "0" means next tick, ie somewhere between 0 and 100ms, so "(0, 4)"
     * gives us [0ms..500ms).
     */
    uint16_t delay = randLIB_get_random_in_range(0, cur->max_ra_delay_time - 1);
    if (ra) {
        /* If we've already handled an RS for this destination, or we'd be
         * delaying an already-scheduled RA, ignore this RS.
         */
        if (ra->rs_triggered || delay >= scheduled) {
            return;
        }
        /* Unqueue, and we'll requeue for an earlier time */
        icmpv6_unqueue_ra(ra);
    } else {
        ra = ns_dyn_mem_alloc(sizeof(icmp_queued_ra_t));
        if (!ra) {
            return;
        }
        memcpy(ra->addr, dest, 16);
        memcpy(ra->abro, abro, 16);
        ra->interface = cur;
    }

    ra->rs_triggered = true;

    /* Rate-limit multicasts - independently for each ABRO */
    if (dest == ADDR_LINK_LOCAL_ALL_NODES) {
        ipv6_ra_timing_t *t = icmpv6_ra_timing_lookup(cur, abro);
        uint32_t time_since_last_ra;
        time_since_last_ra = protocol_core_monotonic_time - t->rtr_adv_last_send_time;

        if (time_since_last_ra < cur->min_delay_between_ras) {
            delay += cur->min_delay_between_ras - time_since_last_ra;
        }
    } else {
        delay = 1;
    }
    ra->ticks = delay;

    icmpv6_queue_ra(ra);
}

buffer_t *icmpv6_rs_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    const uint8_t *sllao;

    if (buf->options.hop_limit != 255 || buf->options.code != 0) {
        return buffer_free(buf);
    }

    if (!icmpv6_options_well_formed_in_buffer(buf, 4)) {
        tr_debug("Malformed RS");
        return buffer_free(buf);
    }

    sllao = icmpv6_find_option_in_buffer(buf, 4, ICMPV6_OPT_SRC_LL_ADDR, 0);

    if (addr_is_ipv6_unspecified(buf->src_sa.address) && sllao) {
        return buffer_free(buf);
    }

    if (!cur->adv_send_advertisements) {
        return buffer_free(buf);
    }

    ipv6_neighbour_t *neighbour;

    if (sllao && cur->if_llao_parse(cur, sllao, &buf->dst_sa)) {
        neighbour = ipv6_neighbour_update_unsolicited(&cur->ipv6_neighbour_cache, buf->src_sa.address, buf->dst_sa.addr_type, buf->dst_sa.address);
    } else {
        neighbour = ipv6_neighbour_lookup(&cur->ipv6_neighbour_cache, buf->src_sa.address);
    }

    if (neighbour && neighbour->is_router) {
        ipv6_router_gone(&cur->ipv6_neighbour_cache, neighbour);
    }

    const uint8_t *dest;
    dest = cur->rtr_adv_unicast_to_rs ? buf->src_sa.address : ADDR_LINK_LOCAL_ALL_NODES;

    /* Yuck - unify later */
    if (cur->nwk_id == IF_6LoWPAN) {
        /* This triggers 1 RA per ABRO (nd_router_t) */
        nd_trigger_ras_from_rs(dest, cur);
    } else {
        /* Just trigger 1 RA without ABRO */
        icmpv6_trigger_ra_from_rs(cur, dest, ADDR_UNSPECIFIED);
    }

    return buffer_free(buf);
}

/* (Re)start multicast router advertisements for a given ABRO, or unspecified. adv_send_advertisements must be set */
void icmpv6_restart_router_advertisements(protocol_interface_info_entry_t *cur, const uint8_t abro[16])
{
    icmp_queued_ra_t *ra;

    if (!cur->adv_send_advertisements) {
        return;
    }

    ipv6_ra_timing_t *t = icmpv6_ra_timing_lookup(cur, abro);
    if (!t) {
        return;
    }

    ra = icmpv6_queued_ra_lookup(ADDR_LINK_LOCAL_ALL_NODES, abro, cur->id, NULL);
    if (ra) {
        icmpv6_unqueue_ra(ra);
    } else {
        ra = ns_dyn_mem_alloc(sizeof(icmp_queued_ra_t));
        if (!ra) {
            return;
        }

        memcpy(ra->addr, ADDR_LINK_LOCAL_ALL_NODES, 16);
        memcpy(ra->abro, abro, 16);
        ra->rs_triggered = false;
        ra->interface = cur;
        /* For a new transmission, if this is 0, we don't send anything initially,
         * but we still want randomness; on the other hand there's no point
         * having a minimum delay. So let's do it like this - equal random range,
         * but starting immediately.
         */
        if (cur->max_initial_rtr_advertisements == 0) {
            ra->ticks = randLIB_get_random_in_range(0, cur->max_rtr_adv_interval - cur->min_rtr_adv_interval);
        }
    }

    /* If we are retriggering "initial" adverts, should allow some jitter in case
     * we're doing it in response to a multicast update.
     */
    if (cur->max_initial_rtr_advertisements != 0) {
        ra->ticks = randLIB_get_random_in_range(0, cur->max_initial_rtr_adv_interval);
    }

    t->initial_rtr_adv_count = cur->max_initial_rtr_advertisements;

    /* And enforce the rate limiting, if somehow we cancelled and restarted this TX */
    uint16_t time_since_last = protocol_core_monotonic_time - t->rtr_adv_last_send_time;
    if (time_since_last < cur->min_delay_between_ras) {
        ra->ticks += cur->min_delay_between_ras - time_since_last;
    }

    icmpv6_queue_ra(ra);
}

/* Cancel scheduled router advertisements, either for a given ABRO (real or ADDR_UNSPECIFIED), or any ABRO (NULL) */
void icmpv6_stop_router_advertisements(protocol_interface_info_entry_t *cur, const uint8_t *abro)
{
    ns_list_foreach_safe(icmp_queued_ra_t, ra, &icmp_ra_queue) {
        if (ra->interface == cur) {
            /* Match ABRO if specified */
            if (abro && !addr_ipv6_equal(abro, ra->abro)) {
                continue;
            }

            icmpv6_unqueue_ra(ra);
            ns_dyn_mem_free(ra);
        }
    }
}

/* Actually send an RA - have to refer to protocol_ipv6.c and nd_router_object.c
 * at the moment, as the information is stored differently.
 */
static void icmpv6_send_ra(protocol_interface_info_entry_t *cur, const uint8_t *dest, const uint8_t *abro)
{
    if (cur->nwk_id == IF_6LoWPAN) {
        nd_ra_build_by_abro(abro, dest, cur);
    } else {
        ipv6_nd_ra_advert(cur, dest);
    }
}

void icmpv6_radv_timer(uint16_t ticks)
{
    /* This initialises to empty (on every call) */
    NS_LIST_DEFINE(to_requeue, icmp_queued_ra_t, link);

    /* Ticks are relative in this queue - break once all ticks are consumed */
    ns_list_foreach_safe(icmp_queued_ra_t, ra, &icmp_ra_queue) {
        if (ra->ticks > ticks) {
            /* Next entry doesn't fire yet - just decrease its time and exit */
            ra->ticks -= ticks;
            break;
        }

        /* Have a firing entry - note that once ticks reaches 0, we can still
         * consume multiple simultaneous entries with ra->ticks == 0, so
         * we don't stop as soon as ticks hits 0. */
        ticks -= ra->ticks;

        /* Just remove, not "unqueue" here, as we're in the process of adjusting ticks */
        ns_list_remove(&icmp_ra_queue, ra);

        ipv6_ra_timing_t *t = icmpv6_ra_timing_lookup(ra->interface, ra->abro);
        if (!t) {
            ns_dyn_mem_free(ra);
        } else {
            /* Safety check - make sure we shut down okay if this gets flipped off */
            if (ra->interface->adv_send_advertisements) {
                icmpv6_send_ra(ra->interface, ra->addr, ra->abro);
                t->rtr_adv_last_send_time = protocol_core_monotonic_time;
            }

            /* Multicast adverts get automatically rescheduled */
            if (addr_is_ipv6_multicast(ra->addr) && ra->interface->adv_send_advertisements) {
                /* reschedule - for safe list handling, stash and reinsert after the main loop */
                ra->ticks = randLIB_get_random_in_range(ra->interface->min_rtr_adv_interval, ra->interface->max_rtr_adv_interval);
                if (t->initial_rtr_adv_count && --t->initial_rtr_adv_count) {
                    uint16_t max = ra->interface->max_initial_rtr_adv_interval;
                    if (ra->ticks > max) {
                        ra->ticks = max;
                    }
                }
                ra->rs_triggered = false;
                ns_list_add_to_end(&to_requeue, ra);
            } else {
                ns_dyn_mem_free(ra);
            }
        }
    }

    ns_list_foreach_safe(icmp_queued_ra_t, ra, &to_requeue) {
        ns_list_remove(&to_requeue, ra);
        icmpv6_queue_ra(ra);
    }
}

#endif /* RADV_TX */
