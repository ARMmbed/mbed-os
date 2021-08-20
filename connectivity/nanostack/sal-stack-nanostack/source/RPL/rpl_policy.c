/*
 * Copyright (c) 2015-2021, Pelion and affiliates.
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
#include "randLIB.h"
#include "net_interface.h"

#include "Core/include/ns_address_internal.h"
#include "Service_Libs/etx/etx.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "ipv6_stack/ipv6_routing_table.h"

#include "RPL/rpl_objective.h"
#include "RPL/rpl_policy.h"

#define TRACE_GROUP "RPLy"

typedef enum {
    RPL_POLICY_FORWARD,
    RPL_POLICY_DROP
} rpl_forward_policy_t;

static bool rpl_policy_parent_confirmation_req = false;
static int8_t rpl_policy_dao_retry_count_conf = 0;
static int16_t rpl_policy_dao_initial_timeout_conf = 20; // Default is 2 seconds 100ms ticks
static uint16_t rpl_policy_dio_validity_period_hysteresis = 0x0180; //Fixed Point 1.5
static uint8_t rpl_policy_multicast_config_min_advertisment_count = 0;
static uint8_t rpl_policy_mrhof_parent_set_size_conf = 3; // default parent set size
static uint16_t rpl_policy_minimum_dao_target_refresh_conf = 0; // by default follow the configuration
static uint16_t rpl_policy_address_registration_timeout_value = 0; // Address registration timeouts in minutes 0 use address lifetime

static bool rpl_policy_force_tunnel_to_BR = false;

bool rpl_policy_force_tunnel(void)
{
    return rpl_policy_force_tunnel_to_BR;
}

void rpl_policy_force_tunnel_set(bool enable)
{
    rpl_policy_force_tunnel_to_BR = enable;
}
/* TODO - application API to control when to join new instances / DODAGs
 *
 * Eg, allow application to ignore local DODAGs, or specify known instance IDs,
 * select on something in the configuration...
 *
 * See RFC 6550 18.6. Policy
 */
bool rpl_policy_join_instance(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid)
{
    (void)domain;
    (void)instance_id;
    (void)dodagid;

    /* And that will do for now. */
    return true;
}

bool rpl_policy_join_dodag(rpl_domain_t *domain, uint8_t g_mop_prf, uint8_t instance_id, const uint8_t *addr)
{
    (void)domain;
    (void)g_mop_prf;
    (void)instance_id;
    (void)addr;

    /* And that will do for now. */
    return true;
}

bool rpl_policy_join_config(rpl_domain_t *domain, const rpl_dodag_conf_t *conf, bool *leaf_only)
{
    (void)domain;
    (void)conf;
    (void)leaf_only;

    /* Sanity checks */
    if (conf->min_hop_rank_increase == 0) {
        return false;
    }

    /* We don't support authentication */
    if (conf->authentication) {
        return false;
    }

    /* We're okay with DODAGs with unrecognised objectives - core will join as leaf using OF0 if we do nothing*/
#if 0
    /* To reject unknowns: */
    if (!rpl_objective_lookup(conf->objective_code_point)) {
        return false;
    }
#endif

    return true;
}


/*bool rpl_policy_join_unsupported_as_leaf(rpl_domain_t *domain)
{
    return true;
}*/

bool rpl_policy_request_dao_acks(const rpl_domain_t *domain, uint8_t mop)
{
    (void)domain;
    (void)mop;

    return true;
}

void rpl_policy_set_initial_dao_ack_wait(uint16_t timeout_in_ms)
{
    rpl_policy_dao_initial_timeout_conf = timeout_in_ms;
}

void rpl_policy_set_minimum_dao_target_refresh(uint16_t seconds)
{
    rpl_policy_minimum_dao_target_refresh_conf = seconds;
}

uint16_t rpl_policy_minimum_dao_target_refresh(void)
{
    return rpl_policy_minimum_dao_target_refresh_conf;
}

uint16_t rpl_policy_initial_dao_ack_wait(const rpl_domain_t *domain, uint8_t mop)
{
    (void)mop;
    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(domain->non_storing_downstream_interface);
    if (ncache) {
        //Check here RE trans timeout
        if (ncache->retrans_timer > 2000) {
            uint32_t reTransTimer = ncache->retrans_timer / 100;
            if (reTransTimer > 0x7fff) {
                return 0xffff;
            }
            return (uint16_t)reTransTimer * 2;
        }
    }

    return rpl_policy_dao_initial_timeout_conf;
}


void rpl_policy_set_dao_retry_count(uint8_t count)
{
    rpl_policy_dao_retry_count_conf = count;
}

int8_t rpl_policy_dao_retry_count()
{
    return rpl_policy_dao_retry_count_conf;
}

//Default Optimistic value is ETX < 2.5
static uint16_t rpl_policy_etx_full_forward = 0x280;
//Total Drop limit default is 8.0 etx
static uint16_t rpl_policy_etx_full_drop = 0x800;

//Scale 100 % to bigger are to get more random resolution
#define MAX_DROP_COMPARE (100 * 256)

int rpl_policy_forward_link_etx_threshold_set(uint16_t etx_full_forward, uint16_t etx_full_drop)
{
    if (etx_full_forward > etx_full_drop) {
        return -1;
    }

    rpl_policy_etx_full_forward = etx_full_forward;
    rpl_policy_etx_full_drop = etx_full_drop;

    return 0;
}

static rpl_forward_policy_t rpl_policy_link_forward_policy(uint16_t link_etx)
{
    if (link_etx >= rpl_policy_etx_full_drop) {
        return RPL_POLICY_DROP;
    }

    if (link_etx <= rpl_policy_etx_full_forward) {
        return RPL_POLICY_FORWARD;
    }

    // The multiplication could overflow 16-bit, even though the final result will be 16-bit
    uint16_t drop_prob = (uint32_t) MAX_DROP_COMPARE * (link_etx - rpl_policy_etx_full_forward) / (rpl_policy_etx_full_drop - rpl_policy_etx_full_forward);

    if (randLIB_get_random_in_range(0, 25599) < drop_prob) {
        return RPL_POLICY_DROP;
    }

    return RPL_POLICY_FORWARD;
}

/* Given the next-hop address from a source routing header, which interface,
 * if any, should we assume that next hop is on?
 */
#define ETX_SRH_THRESHOLD 0x400 /* 8.8 fixed-point, so 4 */
int8_t rpl_policy_srh_next_hop_interface(rpl_domain_t *domain, int8_t if_id, const uint8_t *next_hop)
{
    if (domain->non_storing_downstream_interface != -1) {
        if_id = domain->non_storing_downstream_interface;
    }

    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(if_id);
    ipv6_neighbour_t *n = ncache ? ipv6_neighbour_lookup(ncache, next_hop) : NULL;
    if (n && n->state == IP_NEIGHBOUR_UNREACHABLE) {
        tr_warn("Rejecting SRH to %s: neighbour unreachable", trace_ipv6(next_hop));
        goto reject;
    }

    uint16_t etx = ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, if_id, n, next_hop, etx_read);

    //Validate Link ETX and play poker game if ETX is over optimistic value
    if (rpl_policy_link_forward_policy(etx) == RPL_POLICY_DROP) {
        tr_warn("Rejecting SRH to %s: etx = %x", trace_ipv6(next_hop), etx);
        goto reject;
    }

    return if_id;

reject:
    /* If rejecting due to SRH or unreachable, we should do some sort of probe
     * to try to update state. This does the job. */
    if (n) {
        ipv6_neighbour_used(ncache, n);
    }
    return -1;
}

uint16_t rpl_policy_modify_downward_cost_to_root_neighbour(rpl_domain_t *domain, int8_t if_id, const uint8_t *next_hop, uint16_t cost)
{
    (void)domain;

    ipv6_neighbour_cache_t *ncache = ipv6_neighbour_cache_by_interface_id(if_id);
    ipv6_neighbour_t *n = ncache ? ipv6_neighbour_lookup(ncache, next_hop) : NULL;
    uint16_t etx = ipv6_map_ip_to_ll_and_call_ll_addr_handler(NULL, if_id, n, next_hop, etx_read);

    if (etx == 0xFFFF) {
        return cost + 256;
    }
    if (etx > ETX_SRH_THRESHOLD) {
        return cost + 8;
    }
    if (!ipv6_neighbour_is_probably_reachable(ncache, n)) {
        return cost + 4;
    }

    return cost;
}

uint16_t rpl_policy_parent_selection_period(rpl_domain_t *domain)
{
    (void)domain;

    return (10 * 60); /* seconds */
}

uint16_t rpl_policy_etx_hysteresis(rpl_domain_t *domain)
{
    (void)domain;

    return 0x0080; /* 8.8 fixed-point, so 0.5 */
}

uint16_t rpl_policy_dio_validity_period(rpl_domain_t *domain)
{
    (void)domain;

    return rpl_policy_dio_validity_period_hysteresis; /* Fixed Point */
}

void rpl_policy_set_dio_validity_period(rpl_domain_t *domain, uint16_t fixed_point)
{
    (void)domain;

    rpl_policy_dio_validity_period_hysteresis = fixed_point; /* Fixed Point */
}

uint16_t rpl_policy_etx_change_parent_selection_delay(rpl_domain_t *domain)
{
    (void)domain;

    return 30; /* seconds */
}

uint16_t rpl_policy_dio_parent_selection_delay(rpl_domain_t *domain)
{
    (void)domain;

    return 15; /* seconds */
}

uint16_t rpl_policy_repair_initial_dis_delay(rpl_domain_t *domain)
{
    (void)domain;

    return 10; /* seconds */
}

uint16_t rpl_policy_repair_maximum_dis_interval(rpl_domain_t *domain)
{
    (void)domain;

    return 60 * 60; /* seconds = 1 hour */
}

uint_fast8_t rpl_policy_repair_dis_count(rpl_domain_t *domain)
{
    (void)domain;

    return 6;
}

uint_fast8_t rpl_policy_repair_poison_count(rpl_domain_t *domain)
{
    (void)domain;

    return 2;
}

bool rpl_policy_dao_trigger_after_srh_error(rpl_domain_t *domain, uint32_t seconds_since_last_dao_trigger, uint16_t errors_since_last_dao_trigger, uint_fast16_t targets)
{
    (void)domain;
    (void)seconds_since_last_dao_trigger;

    /* Trigger a DAO refresh after a number of errors proportional to the network size */
    return errors_since_last_dao_trigger > 2 * targets;
}

bool rpl_policy_target_descriptor_for_own_address(rpl_domain_t *domain, const uint8_t addr[16], if_address_source_t source, void *data, uint32_t *descriptor_out)
{
    (void)domain;
    (void)addr;
    (void)source;
    (void)data;
    (void)descriptor_out;

    return false;
}

bool rpl_policy_target_descriptor_for_host_address(rpl_domain_t *domain, const uint8_t addr[16], uint32_t *descriptor_out)
{
    (void)domain;
    (void)addr;
    (void)descriptor_out;

    return false;
}

uint16_t rpl_policy_of0_stretch_of_rank(const rpl_domain_t *domain)
{
    (void)domain;

    return 0;
}

uint_fast8_t rpl_policy_of0_rank_factor(const rpl_domain_t *domain)
{
    (void)domain;

    return 1;
}

bool rpl_policy_of0_dodag_preference_supersedes_grounded(const rpl_domain_t *domain)
{
    (void)domain;

    return false;
}

uint_fast8_t rpl_policy_of0_max_backup_successors(const rpl_domain_t *domain)
{
    (void)domain;

    return 1;
}

void rpl_policy_set_mrhof_parent_set_size(uint8_t parent_set_size)
{
    rpl_policy_mrhof_parent_set_size_conf = parent_set_size;
}

uint_fast8_t rpl_policy_mrhof_parent_set_size(const rpl_domain_t *domain)
{
    (void)domain;

    return rpl_policy_mrhof_parent_set_size_conf;
}

uint16_t rpl_policy_mrhof_max_rank_stretch_for_extra_parents(const rpl_domain_t *domain)
{
    (void)domain;

    /* Conservative - will allow some backup, but not an extra hop */
    return 64;
}

uint16_t rpl_policy_mrhof_max_link_metric(const rpl_domain_t *domain)
{
    (void)domain;

    return 512;
}

uint16_t rpl_policy_mrhof_parent_switch_threshold(const rpl_domain_t *domain)
{
    (void)domain;

    return 192;
}

void rpl_policy_set_parent_confirmation_request(bool confirmation_requested)
{
    rpl_policy_parent_confirmation_req = confirmation_requested;
}


bool rpl_policy_parent_confirmation_requested(void)
{
    return rpl_policy_parent_confirmation_req;
}

uint8_t rpl_policy_dio_multicast_config_advertisment_min_count(void)
{
    return rpl_policy_multicast_config_min_advertisment_count;
}

void rpl_policy_set_dio_multicast_config_advertisment_min_count(uint8_t min_count)
{
    rpl_policy_multicast_config_min_advertisment_count = min_count;
}

uint16_t rpl_policy_address_registration_timeout()
{
    return rpl_policy_address_registration_timeout_value;
}

void rpl_policy_set_address_registration_timeout(uint16_t timeout_in_minutes)
{
    rpl_policy_address_registration_timeout_value = timeout_in_minutes;
}


#ifdef RPL_STRUCTURES_H_
#error "rpl_structures.h should not be included by rpl_policy.c"
#endif

#endif /* HAVE_RPL */
