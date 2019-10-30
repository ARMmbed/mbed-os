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

/* rpl_control.c deals with packet handling, and management of domains. The
 * workings of individual instances are dealt elsewhere.
 *
 * rpl_domain_t is accessible.
 * rpl_instance_t, rpl_dodag_t, rpl_dodag_version_t, rpl_neighbout_t are all opaque
 */

/*
 * State machine still not fully developed - we're basically free-running,
 * with a simple kick to advance the bootstrap when we first get a DAO-ACK
 * on any instance.
 *
 * A larger set of events is needed, including some sort of policy interaction
 * to establish which instances "count" for triggering an overall system (or
 * domain) state machine.
 */
#include "nsconfig.h"

#ifdef HAVE_RPL

#include <string.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"

#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "Service_Libs/etx/etx.h" /* slight ick */

#include "net_rpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_control.h"

#define TRACE_GROUP "rplc"

const uint8_t ADDR_LINK_LOCAL_ALL_RPL_NODES[16] = { 0xff, 0x02, [15] = 0x1a };

/* Sensible default limits for a 6LoWPAN-ND node */
static size_t rpl_purge_threshold = 1 * 1024;
static size_t rpl_alloc_limit = 2 * 1024; // 0 means no limit

static size_t rpl_alloc_total;
#define RPL_ALLOC_OVERHEAD 8

static NS_LIST_DEFINE(rpl_domains, rpl_domain_t, link);

void rpl_control_set_memory_limits(size_t soft_limit, size_t hard_limit)
{
    rpl_purge_threshold = soft_limit;
    rpl_alloc_limit = hard_limit;
}

/* Send all RPL allocations through central point - gives a way of counting and
 * limiting RPL allocations. Allocation rejected if total would go above the
 * hard limit rpl_alloc_limit. This should normally be avoided by the timers
 * trying to keep allocations below the soft limit rpl_purge_threshold.
 */
void *rpl_realloc(void *p, uint16_t old_size, uint16_t new_size)
{
    if (old_size == new_size) {
        return p;
    }

    /* If increasing memory, check if we're going above the hard limit */
    if (rpl_alloc_limit != 0 && new_size > old_size) {
        uint16_t size_diff = p ? new_size - old_size : new_size + RPL_ALLOC_OVERHEAD;

        if (rpl_alloc_total + size_diff > rpl_alloc_limit) {
            protocol_stats_update(STATS_RPL_MEMORY_OVERFLOW, size_diff);
            return NULL;
        }
    }
    void *n = ns_dyn_mem_alloc(new_size);
    if (n) {
        /* rpl_free() below will subtract (old_size + RPL_ALLOC_OVERHEAD) if reallocing */
        rpl_alloc_total += (size_t) new_size + RPL_ALLOC_OVERHEAD;
        protocol_stats_update(STATS_RPL_MEMORY_ALLOC, new_size);
        if (p) {
            memcpy(n, p, old_size < new_size ? old_size : new_size);
        }
    } else {
        protocol_stats_update(STATS_RPL_MEMORY_OVERFLOW, new_size);
    }
    rpl_free(p, old_size);
    return n;
}

void *rpl_alloc(uint16_t size)
{
    return rpl_realloc(NULL, 0, size);
}

void rpl_free(void *p, uint16_t size)
{
    if (p) {
        rpl_alloc_total -= (size_t) size + RPL_ALLOC_OVERHEAD;
        protocol_stats_update(STATS_RPL_MEMORY_FREE, size);
    }
    ns_dyn_mem_free(p);
}

void rpl_control_event(struct rpl_domain *domain, rpl_event_t event)
{
    if (domain->callback) {
        domain->callback(event, domain->cb_handle);
    }
}


/* When we join a new instance, we need to publish existing addresses.
 * Later addresses additions/removals are handled by rpl_control_addr_notifier.
 */
static void rpl_control_publish_own_addresses(rpl_domain_t *domain, rpl_instance_t *instance)
{
    int8_t last_id = -1;
    protocol_interface_info_entry_t *cur;
    while ((cur = protocol_stack_interface_info_get_by_rpl_domain(domain, last_id)) != NULL) {
        ns_list_foreach(if_address_entry_t, addr, &cur->ip_addresses) {
            if (!addr->tentative && !addr_is_ipv6_link_local(addr->address)) {
                /* TODO - wouldn't need to publish address if within a prefix published by parent */
                uint32_t descriptor = 0;
                bool want_descriptor = rpl_policy_target_descriptor_for_own_address(domain, addr->address, addr->source, addr->data, &descriptor);
                rpl_instance_publish_dao_target(instance, addr->address, 128, addr->valid_lifetime, true, want_descriptor, descriptor);
            }
        }
        last_id = cur->id;
    }
}

void rpl_control_publish_host_address(rpl_domain_t *domain, const uint8_t addr[16], uint32_t lifetime)
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        if (!rpl_instance_am_root(instance)) {
            /* TODO - Wouldn't need to publish host address if within a published prefix */
            uint32_t descriptor = 0;
            bool want_descriptor = rpl_policy_target_descriptor_for_host_address(domain, addr, &descriptor);
            rpl_instance_publish_dao_target(instance, addr, 128, lifetime, false, want_descriptor, descriptor);
        }
    }
}

/* Is unpublish a word? */
void rpl_control_unpublish_address(rpl_domain_t *domain, const uint8_t addr[16])
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_instance_delete_published_dao_target(instance, addr, 128);
    }
}

void rpl_control_request_parent_link_confirmation(bool requested)
{
    rpl_policy_set_parent_confirmation_request(requested);
}

void rpl_control_set_dio_multicast_min_config_advertisment_count(uint8_t min_count)
{
    rpl_policy_set_dio_multicast_config_advertisment_min_count(min_count);
}

void rpl_control_set_dao_retry_count(uint8_t count)
{
    rpl_policy_set_dao_retry_count(count);
}

void rpl_control_set_initial_dao_ack_wait(uint16_t timeout_in_ms)
{
    rpl_policy_set_initial_dao_ack_wait(timeout_in_ms);
}

/* Send address registration to either specified address, or to non-registered address */
void rpl_control_register_address(protocol_interface_info_entry_t *interface, const uint8_t addr[16])
{
    if (!rpl_policy_parent_confirmation_requested()) {
        return;
    }
    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {
        rpl_instance_send_address_registration(instance, addr);
    }
}

void rpl_control_address_register_done(protocol_interface_info_entry_t *interface, const uint8_t ll_addr[16], uint8_t status)
{
    if (!interface->rpl_domain) {
        return;
    }
    if (!rpl_policy_parent_confirmation_requested()) {
        return;
    }

    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {
        rpl_neighbour_t *neighbour = rpl_lookup_neighbour_by_ll_address(instance, ll_addr, interface->id);
        if (neighbour) {
            rpl_instance_address_registration_done(interface, instance, neighbour, status);
        }
    }
}

bool rpl_control_is_dodag_parent(protocol_interface_info_entry_t *interface, const uint8_t ll_addr[16])
{
    if (!interface->rpl_domain) {
        return false;
    }
    // go through instances and parents and check if they match the address.
    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {
        if (rpl_instance_address_is_parent(instance, ll_addr)) {
            return true;
        }
    }
    return false;
}

bool rpl_control_is_dodag_parent_candidate(protocol_interface_info_entry_t *interface, const uint8_t ll_addr[16], uint16_t candidate_cmp_limiter)
{
    if (!interface->rpl_domain) {
        return false;
    }
    // go through instances and parents and check if they match the address.
    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {
        if (rpl_instance_address_is_candidate(instance, ll_addr, candidate_cmp_limiter)) {
            return true;
        }
    }
    return false;
}


uint16_t rpl_control_parent_candidate_list_size(protocol_interface_info_entry_t *interface, bool parent_list)
{
    if (!interface->rpl_domain) {
        return 0;
    }

    uint16_t parent_list_size = 0;

    // go through instances and parents and check if they match the address.
    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {
        uint16_t current_size = rpl_instance_address_candidate_count(instance, parent_list);
        if (current_size > parent_list_size) {
            parent_list_size = current_size;
        }
    }
    return parent_list_size;
}


void rpl_control_neighbor_delete(protocol_interface_info_entry_t *interface, const uint8_t ll_addr[16])
{
    if (!interface->rpl_domain) {
        return;
    }
    // go through instances and delete address.
    ns_list_foreach(struct rpl_instance, instance, &interface->rpl_domain->instances) {

        rpl_neighbour_t *neighbour = rpl_lookup_neighbour_by_ll_address(instance, ll_addr, interface->id);
        if (neighbour) {
            rpl_delete_neighbour(instance, neighbour);
        }
    }
}

/* Address changes need to trigger DAO target re-evaluation */
static void rpl_control_addr_notifier(struct protocol_interface_info_entry *interface, const if_address_entry_t *addr, if_address_callback_t reason)
{
    /* Don't care about link-local addresses */
    if (addr_is_ipv6_link_local(addr->address)) {
        return;
    }

    /* Only publish addresses on the domain attached to their interface */
    if (!interface->rpl_domain) {
        return;
    }

    switch (reason) {
        case ADDR_CALLBACK_DELETED:
            rpl_control_unpublish_address(interface->rpl_domain, addr->address);
            break;
        default:
            break;

    }
}

static void rpl_control_etx_change_callback(int8_t  nwk_id, uint16_t previous_etx, uint16_t current_etx, uint8_t attribute_index)
{
    (void)previous_etx;
    (void)current_etx;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(nwk_id);
    if (!cur || !cur->rpl_domain) {
        return;
    }
    rpl_domain_t *domain = cur->rpl_domain;
    uint16_t delay = rpl_policy_etx_change_parent_selection_delay(domain);
    tr_debug("Triggering parent selection due to ETX change on neigh index %u, etx %u", attribute_index, current_etx);

    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_instance_trigger_parent_selection(instance, delay);
        if (rpl_instance_am_root(instance)) {
            rpl_downward_paths_invalidate(instance);
        }
    }
}

/* Create a RPL domain - it can be associated with one or more interfaces */
rpl_domain_t *rpl_control_create_domain(void)
{
    rpl_domain_t *domain = rpl_alloc(sizeof(rpl_domain_t));
    if (!domain) {
        return NULL;
    }

    ns_list_init(&domain->instances);
    domain->non_storing_downstream_interface = -1;
    domain->callback = NULL;
    domain->cb_handle = NULL;
    domain->force_leaf = false;
    ns_list_add_to_start(&rpl_domains, domain);

    addr_notification_register(rpl_control_addr_notifier);

    return domain;
}

void rpl_control_delete_domain(rpl_domain_t *domain)
{
    ns_list_foreach_safe(rpl_instance_t, instance, &domain->instances) {
        rpl_delete_instance(instance);
    }
    ns_list_remove(&rpl_domains, domain);
    rpl_free(domain, sizeof * domain);
}

static void rpl_control_remove_interface_from_domain(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, bool free_instances)
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_instance_remove_interface(instance, cur->id);
    }

    ns_list_foreach(if_address_entry_t, addr, &cur->ip_addresses) {
        if (!addr_is_ipv6_link_local(addr->address)) {
            rpl_control_unpublish_address(domain, addr->address);
        }
    }

    if (free_instances) {
        ns_list_foreach_safe(rpl_instance_t, instance, &domain->instances) {
            rpl_delete_instance(instance);
        }
    }

    if (domain->non_storing_downstream_interface == cur->id) {
        domain->non_storing_downstream_interface = -1;
    }
}

void rpl_control_set_domain_on_interface(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, bool downstream)
{
    if (cur->rpl_domain != domain) {
        rpl_control_remove_domain_from_interface(cur);
        cur->rpl_domain = domain;
        addr_add_group(cur, ADDR_LINK_LOCAL_ALL_RPL_NODES);
    }

    if (downstream) {
        domain->non_storing_downstream_interface = cur->id;
    }
    /* This is a bit icky - why assume that our Objective Functions use ETX? */
    /* But this is the easiest place to add an interface registration */
    etx_value_change_callback_register(cur->nwk_id, cur->id,  rpl_policy_etx_hysteresis(domain), rpl_control_etx_change_callback);
}

void rpl_control_remove_domain_from_interface(protocol_interface_info_entry_t *cur)
{
    if (cur->rpl_domain) {
        rpl_control_remove_interface_from_domain(cur, cur->rpl_domain, false);
        addr_delete_group(cur, ADDR_LINK_LOCAL_ALL_RPL_NODES);
        cur->rpl_domain = NULL;
    }
}

void rpl_control_free_domain_instances_from_interface(protocol_interface_info_entry_t *cur)
{
    if (cur->rpl_domain) {
        rpl_control_remove_interface_from_domain(cur, cur->rpl_domain, true);
        addr_delete_group(cur, ADDR_LINK_LOCAL_ALL_RPL_NODES);
        cur->rpl_domain = NULL;
    }
}

void rpl_control_set_callback(rpl_domain_t *domain, rpl_domain_callback_t callback, rpl_prefix_callback_t prefix_learn_cb, rpl_new_parent_callback_t new_parent_add, void *cb_handle)
{
    domain->callback = callback;
    domain->prefix_cb = prefix_learn_cb;
    domain->cb_handle = cb_handle;
    domain->new_parent_add = new_parent_add;
}

/* To do - this should live somewhere nicer. Basically a bootstrap
 * thing to stop being a host as soon as we get our first RPL parent
 * (any instance?)
 */
void rpl_control_disable_ra_routes(struct rpl_domain *domain)
{
    int8_t last_id = -1;
    protocol_interface_info_entry_t *cur;
    while ((cur = protocol_stack_interface_info_get_by_rpl_domain(domain, last_id)) != NULL) {
        icmpv6_recv_ra_routes(cur, false);
        last_id = cur->id;
    }
}

bool rpl_control_have_dodag(rpl_domain_t *domain)
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        if (rpl_instance_current_dodag(instance)) {
            return true;
        }
    }
    return false;
}

typedef void rpl_control_predicate_loop_fn_t(rpl_instance_t *instance, rpl_dodag_version_t *version, void *arg);

/* Callbacks for rpl_control_predicate_loop */

static void rpl_loopfn_reset_dio_timer(rpl_instance_t *instance, rpl_dodag_version_t *dodag_version, void *handle)
{
    (void)dodag_version;
    (void)handle;

    rpl_instance_inconsistency(instance);
}

typedef struct rpl_loopfn_trigger_unicast_dio_arg {
    struct protocol_interface_info_entry *interface;
    const uint8_t *dst;
} rpl_loopfn_trigger_unicast_dio_arg_t;

static void rpl_loopfn_trigger_unicast_dio(rpl_instance_t *instance, rpl_dodag_version_t *dodag_version, void *handle)
{
    (void)dodag_version;

    rpl_loopfn_trigger_unicast_dio_arg_t *arg = handle;
    rpl_instance_dio_trigger(instance, arg->interface, arg->dst);
}

/* For each DODAG Version we're a member of, matching the predicates, call the
 * provided function. Mainly used for handling DODAG Information Solicitations.
 */
static void rpl_control_predicate_loop(rpl_domain_t *domain, rpl_control_predicate_loop_fn_t *fn, void *fn_arg, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, uint8_t version_num)
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_dodag_version_t *version = rpl_instance_predicate_match(instance, pred, instance_id, dodagid, version_num);
        if (version) {
            fn(instance, version, fn_arg);
        }
    }
}

/* Manipulation of roots */
rpl_dodag_t *rpl_control_create_dodag_root(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid, const rpl_dodag_conf_t *conf, uint16_t rank, uint8_t g_mop_prf)
{
    rpl_instance_t *instance = rpl_lookup_instance(domain, instance_id, dodagid);
    if (!instance) {
        instance = rpl_create_instance(domain, instance_id);
        if (!instance) {
            return NULL;
        }
    }

    rpl_dodag_t *dodag = rpl_lookup_dodag(instance, dodagid);
    if (dodag) {
        tr_error("Root DODAG already exists");
        return NULL;
    }
    dodag = rpl_create_dodag(instance, dodagid, g_mop_prf);
    if (!dodag) {
        tr_error("No mem for DODAG root");
        return NULL;
    }

    rpl_dodag_update_config(dodag, conf, NULL, NULL);
    rpl_dodag_set_root(dodag, true);
    rpl_dodag_version_t *version = rpl_create_dodag_version(dodag, rpl_seq_init());
    if (!version) {
        rpl_delete_dodag(dodag);
        tr_error("No mem for DODAG root");
        return NULL;
    }
    rpl_instance_set_dodag_version(instance, version, rank);

    return dodag;
}

void rpl_control_delete_dodag_root(rpl_domain_t *domain, rpl_dodag_t *dodag)
{
    (void)domain;

    rpl_delete_dodag_root(dodag);
}

void rpl_control_update_dodag_route(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age)
{
    /* Not clear if non-root nodes should be able to publish routes */
    if (rpl_dodag_am_root(dodag)) {
        rpl_dodag_update_dio_route(dodag, prefix, prefix_len, flags, lifetime, age);
    }
}

void rpl_control_update_dodag_prefix(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool age)
{
    /* Don't let them set weird flags. We do allow them to add prefixes if not
     * a root though - they might want this to add a L prefix?
     */
    flags &= (PIO_A | PIO_L);
    rpl_dodag_update_dio_prefix(dodag, prefix, prefix_len, flags, lifetime, preftime, /*publish=*/true, age);
}

void rpl_control_increment_dtsn(rpl_dodag_t *dodag)
{
    rpl_dodag_increment_dtsn(dodag);
    //rpl_dodag_inconsistency(dodag); currently implied by rpl_dodag_increment_dtsn
}

void rpl_control_increment_dodag_version(rpl_dodag_t *dodag)
{
    if (rpl_dodag_am_root(dodag)) {
        uint8_t new_version = rpl_seq_inc(rpl_dodag_get_version_number_as_root(dodag));
        rpl_dodag_set_version_number_as_root(dodag, new_version);
    }
}
void rpl_control_update_dodag_config(struct rpl_dodag *dodag, const rpl_dodag_conf_t *conf)
{

    if (rpl_dodag_am_root(dodag)) {
        rpl_dodag_update_config(dodag, conf, NULL, NULL);
    }
}


void rpl_control_set_dodag_pref(rpl_dodag_t *dodag, uint8_t pref)
{
    if (rpl_dodag_am_root(dodag)) {
        rpl_dodag_set_pref(dodag, pref);
    }
}

void rpl_control_poison(rpl_domain_t *domain, uint8_t count)
{
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_instance_poison(instance, count);
    }
}

void rpl_control_force_leaf(rpl_domain_t *domain, bool leaf)
{
    domain->force_leaf = leaf;
    if (leaf) {
        ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
            rpl_instance_force_leaf(instance);
        }
    }
}

/* Check whether the options section of a RPL control message is well-formed */
static bool rpl_control_options_well_formed(const uint8_t *dptr, uint_fast16_t dlen)
{
    while (dlen) {
        uint_fast16_t opt_len;

        if (dptr[0] == RPL_PAD1_OPTION) {
            opt_len = 1;
        } else {
            if (dlen < 2) {
                return false;
            }
            opt_len = 2 + dptr[1];
        }

        if (opt_len > dlen) {
            return false;
        }

        dptr += opt_len;
        dlen -= opt_len;
    }

    return true;
}

static bool rpl_control_options_well_formed_in_buffer(const buffer_t *buf, uint16_t offset)
{
    if (buffer_data_length(buf) < offset) {
        return false;
    }

    return rpl_control_options_well_formed(buffer_data_pointer(buf) + offset,
                                           buffer_data_length(buf) - offset);
}

/*
 * Search for the first option of the specified type (and optionally length).
 * Caller must have already checked the options are well-formed.
 * Returns pointer to type byte, so length is at +1, data at +2.
 */
static const uint8_t *rpl_control_find_option(const uint8_t *dptr, uint_fast16_t dlen, uint8_t option, uint8_t optlen)
{
    while (dlen) {
        uint8_t type = dptr[0];
        if (type == RPL_PAD1_OPTION) {
            dptr++, dlen--;
            continue;
        }
        uint_fast16_t len = dptr[1];
        if (type == option && (optlen == 0 || optlen == len)) {
            return dptr;
        }

        dlen -= 2 + len;
        dptr += 2 + len;
    }
    return NULL;
}

static const uint8_t *rpl_control_find_option_in_buffer(const buffer_t *buf, uint_fast16_t offset, uint8_t option, uint8_t optlen)
{
    return rpl_control_find_option(buffer_data_pointer(buf) + offset,
                                   buffer_data_length(buf) - offset,
                                   option, optlen);
}

/*  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Type = 0x04 |Opt Length = 14| Flags |A| PCS | DIOIntDoubl.  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |  DIOIntMin.   |   DIORedun.   |        MaxRankIncrease        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |      MinHopRankIncrease       |              OCP              |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Reserved    | Def. Lifetime |      Lifetime Unit            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *      Figure 24: Format of the DODAG Configuration Option
 */
static const uint8_t *rpl_control_read_conf(rpl_dodag_conf_t *conf_out, const uint8_t *opt)
{
    conf_out->authentication = opt[2] & 0x08;
    conf_out->path_control_size = opt[2] & 0x07;
    conf_out->dio_interval_doublings = opt[3];
    conf_out->dio_interval_min = opt[4];
    conf_out->dio_redundancy_constant = opt[5];
    conf_out->dag_max_rank_increase = common_read_16_bit(opt + 6);
    conf_out->min_hop_rank_increase = common_read_16_bit(opt + 8);
    conf_out->objective_code_point = common_read_16_bit(opt + 10);
    conf_out->default_lifetime = opt[13];
    conf_out->lifetime_unit = common_read_16_bit(opt + 14);
    return opt + 16;
}

static uint8_t *rpl_control_write_conf(uint8_t *opt_out, const rpl_dodag_conf_t *conf)
{
    opt_out[0] = RPL_DODAG_CONF_OPTION;
    opt_out[1] = 14;
    opt_out[2] = conf->authentication ? RPL_CONF_FLAG_AUTH : 0;
    opt_out[2] |= conf->path_control_size;
    opt_out[3] = conf->dio_interval_doublings;
    opt_out[4] = conf->dio_interval_min;
    opt_out[5] = conf->dio_redundancy_constant;
    common_write_16_bit(conf->dag_max_rank_increase, opt_out + 6);
    common_write_16_bit(conf->min_hop_rank_increase, opt_out + 8);
    common_write_16_bit(conf->objective_code_point, opt_out + 10);
    opt_out[12] = 0; // reserved
    opt_out[13] = conf->default_lifetime;
    common_write_16_bit(conf->lifetime_unit, opt_out + 14);
    return opt_out + 16;
}

static uint_fast8_t rpl_control_conf_length(void)
{
    return 16;
}

/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Type = 0x08 |Opt Length = 30| Prefix Length |L|A|R|Reserved1|
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Valid Lifetime                        |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                       Preferred Lifetime                      |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                           Reserved2                           |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                            Prefix                             +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *       Figure 29: Format of the Prefix Information Option
 */
static void rpl_control_process_prefix_options(protocol_interface_info_entry_t *cur, rpl_instance_t *instance, rpl_dodag_t *dodag, rpl_neighbour_t *neighbour, const uint8_t *start, const uint8_t *end)
{
    (void)cur;

    bool router_addr_set = false;

    rpl_neighbour_t *pref_parent = rpl_instance_preferred_parent(instance);
    if (neighbour == pref_parent) {
        rpl_dodag_update_unpublished_dio_prefix_start(dodag);
    }

    for (;;) {
        const uint8_t *ptr = rpl_control_find_option(start, end - start, RPL_PREFIX_INFO_OPTION, 30);
        if (!ptr) {
            break;
        }
        uint8_t prefix_len = ptr[2];
        uint8_t flags = ptr[3];
        uint32_t valid = common_read_32_bit(ptr + 4);
        uint32_t preferred = common_read_32_bit(ptr + 8);
        const uint8_t *prefix = ptr + 16;

        if (rpl_upward_accept_prefix_update(dodag, neighbour, pref_parent)) {

            /* Store prefixes for possible forwarding */
            /* XXX if leaf - don't bother? Or do we want to remember them for
             * when we switch DODAG, as mentioned above?
             */
            prefix_entry_t *prefix_entry = rpl_dodag_update_dio_prefix(dodag, prefix, prefix_len, flags, valid, preferred, false, true);
            if (prefix_entry && pref_parent) {
                rpl_control_process_prefix_option(prefix_entry, cur);
                rpl_domain_t *domain = cur->rpl_domain;
                if (domain && domain->prefix_cb) {
                    uint8_t ll_address[16];
                    memcpy(ll_address, rpl_neighbour_ll_address(pref_parent), 16);
                    domain->prefix_cb(prefix_entry, domain->cb_handle, ll_address);
                }
            }
        }

        if ((flags & PIO_R) && !router_addr_set) {
            /* Routers can have multiple global addresses - one for each
             * prefix being advertised. We don't attempt to track this;
             * we just remember their first-listed address.
             * (
             */
            router_addr_set = true;
            rpl_neighbour_update_global_address(neighbour, ptr + 16);
        }

        start = ptr + 32;
    }
    if (neighbour == pref_parent) {
        rpl_dodag_update_unpublished_dio_prefix_finish(dodag);
    }
}

void rpl_control_process_prefix_option(prefix_entry_t *prefix, protocol_interface_info_entry_t *cur)
{
    //Check is L Flag active
    if (prefix->options & PIO_L) {
        //define ONLink Route Information
        //tr_debug("Register On Link Prefix to routing table");
        ipv6_route_add(prefix->prefix, prefix->prefix_len, cur->id, NULL, ROUTE_RADV, prefix->lifetime, 0);
    }

}


/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Type = 0x03 | Option Length | Prefix Length |Resvd|Prf|Resvd|
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                        Route Lifetime                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  .                   Prefix (Variable Length)                    .
 *  .                                                               .
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *        Figure 23: Format of the Route Information Option
 */
static void rpl_control_process_route_options(rpl_instance_t *instance, rpl_dodag_t *dodag, rpl_dodag_version_t *version, rpl_neighbour_t *neighbour, uint16_t rank, const uint8_t *start, const uint8_t *end)
{
    (void)neighbour;

    /* For non-current DODAGs, it's simplest to accept routes from anyone. */
    if (rpl_instance_current_dodag(instance) == dodag) {
        /* It's for the current DODAG - more care required. Check versions */
        rpl_cmp_t v_cmp = rpl_dodag_version_compare(version, rpl_instance_current_dodag_version(instance));
        if (v_cmp & RPL_CMP_EQUAL) {
            /* If coming from a neighbour in the same version, make sure they have lower rank */
            if (!(rpl_rank_compare(dodag, rank, rpl_instance_current_rank(instance)) & RPL_CMP_LESS)) {
                return;
            }
        } else if (v_cmp & RPL_CMP_GREATER) {
            /* If coming from a neighbour in a newer version, we'll accept it */
        } else {
            /* From older or unknown version, we reject it */
            return;
        }
    }

    for (;;) {
        const uint8_t *ptr = rpl_control_find_option(start, end - start, RPL_ROUTE_INFO_OPTION, 0);
        if (!ptr) {
            break;
        }
        uint8_t opt_len = ptr[1];
        start = ptr + 2 + opt_len;
        if (opt_len < 6) {
            tr_warn("Malformed RIO");
            continue;
        }
        uint8_t prefix_len = ptr[2];
        uint8_t flags = ptr[3];
        uint32_t lifetime = common_read_32_bit(ptr + 4);
        const uint8_t *prefix = ptr + 8;
        if (opt_len < 6 + (prefix_len + 7u) / 8) {
            tr_warn("Malformed RIO");
            continue;
        }
        rpl_dodag_update_dio_route(dodag, prefix, prefix_len, flags, lifetime, true);
    }

    /* We do not purge unadvertised routes. Thus if the root wants to purge
     * a route before its lifetime is up, stopping advertising it is not
     * sufficient, it has to advertise it with low or zero lifetime. This fits
     * with ND Router Advertisement behaviour.
     */
}

static void rpl_control_dao_trigger_request(rpl_instance_t *instance, rpl_dodag_t *dodag, rpl_neighbour_t *neighbour)
{
    switch (rpl_dodag_mop(dodag)) {
        case RPL_MODE_NON_STORING:
            rpl_instance_dao_request(instance, NULL);
            rpl_dodag_increment_dtsn(dodag);
            break;
        case RPL_MODE_STORING:
        case RPL_MODE_STORING_MULTICAST:
            rpl_instance_dao_request(instance, neighbour);
            break;
        default:
            /* Nothing */
            break;
    }
}

/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | RPLInstanceID |Version Number |             Rank              |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |G|0| MOP | Prf |     DTSN      |     Flags     |   Reserved    |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                            DODAGID                            +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Option(s)...
 *  +-+-+-+-+-+-+-+-+
 *
 *                 Figure 14: The DIO Base Object
 */
static buffer_t *rpl_control_dio_handler(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, buffer_t *buf)
{
    if (!rpl_control_options_well_formed_in_buffer(buf, 24)) {
malformed:
        protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
        return buffer_free(buf);
    }

    /* Read the base object */
    const uint8_t *ptr = buffer_data_pointer(buf);
    uint8_t instance_id, version_num, g_mop_prf, dtsn;
    uint16_t rank;
    const uint8_t *dodagid;
    bool become_leaf = false;

    instance_id = ptr[0];
    version_num = ptr[1];
    rank = common_read_16_bit(ptr + 2);
    g_mop_prf = ptr[4];
    dtsn = ptr[5];
    dodagid = ptr + 8;
    ptr += 24;
    tr_info("DIO from %s, rank %x", trace_ipv6(buf->src_sa.address), rank);
    if (addr_is_ipv6_link_local(dodagid) || addr_is_ipv6_multicast(dodagid)) {
        tr_error("DIO DODAGID");
        goto malformed;
    }

    /* Check if this is for an existing RPL Instance - create if necessary */
    rpl_instance_t *instance = rpl_lookup_instance(domain, instance_id, dodagid);
    if (!instance) {
        /* Policy can gate at this point, by instance ID and DODAG ID */
        if (!rpl_policy_join_instance(domain, instance_id, dodagid)) {
            tr_info("Policy ignoring instance (%d,%s)", instance_id, trace_ipv6(dodagid));
            return buffer_free(buf);
        }

        instance = rpl_create_instance(domain, instance_id);
        if (!instance) {
            return buffer_free(buf);
        }
    }

    /* Lookup any existing neighbour entry */
    rpl_neighbour_t *neighbour = rpl_lookup_neighbour_by_ll_address(instance, buf->src_sa.address, cur->id);

    /* From this point on, we are potentially updating an existing neighbour */
    /* Any decision to reject the DIO, or failure to handle it, means removing that neighbour */

    /* Any DIO with infinite rank is immediate poison - no further analysis required */
    if (rank == RPL_RANK_INFINITE) {
        goto invalid_parent;
    }

    /* Policy can gate by DODAGID and other bits */
    if (!rpl_policy_join_dodag(domain, g_mop_prf, instance_id, dodagid)) {
        tr_info("Policy ignoring DODAG (%02x,%s)", g_mop_prf, trace_ipv6(dodagid));
        goto invalid_parent;
    }

    /* Look up and create the DODAG structure */
    rpl_dodag_t *dodag = rpl_lookup_dodag(instance, dodagid);
    if (!dodag) {
        /* Policy can gate by DODAGID and other bits */
        if (!rpl_policy_join_dodag(domain, g_mop_prf, instance_id, dodagid)) {
            tr_info("Policy ignoring DODAG (%02x,%s)", g_mop_prf, trace_ipv6(dodagid));
            goto invalid_parent;
        }
        dodag = rpl_create_dodag(instance, dodagid, g_mop_prf);
        if (!dodag) {
            goto invalid_parent;
        }
    }

    /* Never listen to nodes in a DODAG we're rooting */
    if (rpl_dodag_am_root(dodag)) {
        /* TODO - if version is newer or unordered, increment our version to be higher? */
        /* Old code had this trick - actually, would need to go further. Want to listen first, then use a higher
         * than existing. */
        /*
        uint8_t our_version = rpl_dodag_get_version_number_as_root(dodag);
        if (rpl_seq_compare(version_num, our_version) & (RPL_CMP_UNORDERED|RPL_CMP_GREATER))
        */
        goto invalid_parent;
    }

    /* Even if we're not currently rooting - what if it's our address? Ignore stale info on network */
    if (addr_interface_address_compare(cur, dodagid) == 0) {
        tr_info("DIO our DODAGID %s", trace_ipv6(dodagid));
        /* Should we transmit poison? */
        goto invalid_parent;
    }


    /* Update DODAG config information, if option present, and either we don't have it or version is newer */
    const uint8_t *dodag_conf_ptr = rpl_control_find_option_in_buffer(buf, 24, RPL_DODAG_CONF_OPTION, 14);
    if (dodag_conf_ptr) {
        rpl_dodag_conf_t conf_buf;
        rpl_control_read_conf(&conf_buf, dodag_conf_ptr);
        if (!rpl_dodag_update_config(dodag, &conf_buf, buf->src_sa.address, &become_leaf)) {
            goto invalid_parent;
        }
    }

    /* If we don't have any DODAG config information, ask by unicast DIS */
    const rpl_dodag_conf_t *conf = rpl_dodag_get_config(dodag);
    if (!conf) {
        /* TODO - rate limit DIS? */
        if (domain->new_parent_add && !domain->new_parent_add(buf->src_sa.address, domain->cb_handle)) {
            goto invalid_parent;
        }
        rpl_control_transmit_dis(domain, cur, RPL_SOLINFO_PRED_DODAGID | RPL_SOLINFO_PRED_INSTANCEID, instance_id, dodagid, 0, buf->src_sa.address);
        goto invalid_parent;
    }

    /* Check whether config is acceptable  */
    if (!rpl_policy_join_config(domain, conf, &become_leaf)) {
        goto invalid_parent;
    }

    /* Lookup or create DODAG Version state info */
    rpl_dodag_version_t *version = rpl_lookup_dodag_version(dodag, version_num);
    if (!version) {
        version = rpl_create_dodag_version(dodag, version_num);
        if (!version) {
            goto invalid_parent;
        }
    }

    const uint8_t *metric_ptr = rpl_control_find_option_in_buffer(buf, 24, RPL_DAG_METRIC_OPTION, 0);
    /* We currently don't understand anything about metrics, so to be on the safe side, we don't join */
    if (metric_ptr) {
        become_leaf = true;
    }

    /* We mustn't process DIOs from our potential sub-DODAG, unless local repair is ongoing */
    if (!rpl_instance_local_repair(instance) && rpl_dodag_version_rank_indicates_possible_sub_dodag(version, rank)) {
        goto invalid_parent;
    }

    /* Now we create the neighbour, if we don't already have a record */
    if (!neighbour) {
        neighbour = rpl_create_neighbour(version, buf->src_sa.address, cur->id, g_mop_prf, dtsn);
        //Call Here new parent create
        if (!neighbour) {
            goto invalid_parent;
        }

        if (domain->new_parent_add && !domain->new_parent_add(buf->src_sa.address, domain->cb_handle)) {
            goto invalid_parent;
        }

    }

    /* Update neighbour info */
    rpl_neighbour_update_dodag_version(neighbour, version, rank, g_mop_prf);

    if (rpl_neighbour_update_dtsn(neighbour, dtsn)) {
        tr_info("Parent %s incremented DTSN", trace_ipv6(buf->src_sa.address));
        rpl_control_dao_trigger_request(instance, dodag, neighbour);
    }


    rpl_control_process_prefix_options(cur, instance, dodag, neighbour, ptr, buffer_data_end(buf));
    //rpl_dodag_update_implicit_system_routes(dodag, neighbour);
    rpl_control_process_route_options(instance, dodag, version, neighbour, rank, ptr, buffer_data_end(buf));

    //rpl_control_process_metric_containers(neighbour, ptr, buffer_data_end(buf))

    if (become_leaf) {
        rpl_dodag_set_leaf(dodag, true);
    }

    /* RFC 6550 8.3: A DIO from a sender with lesser DAGRank that causes no
     * changes to the recipient's parent set, preferred parent, or Rank SHOULD
     * be considered consistent with respect to the Trickle timer.
     *
     * Now, if we don't run parent selection immediately, how do we know if it's
     * consistent or not? Compromise is to treat all lower ranked DIOs as
     * consistent, and reset (and hold) the consistent counter to 0 if any of
     * the above change. This actually seems better than the RFC 6550 rule, as
     * it guarantees we will transmit if those change. The rule as stated
     * would mean a large number of parent messages would stop us advertising
     * a Rank change.
     */
    if (version == rpl_instance_current_dodag_version(instance) &&
            (rpl_rank_compare(dodag, rank, rpl_instance_current_rank(instance)) & RPL_CMP_LESS)) {
        rpl_instance_consistent_rx(instance);
    }

    rpl_instance_neighbours_changed(instance, dodag);

    return buffer_free(buf);

invalid_parent:
    if (neighbour) {
        rpl_delete_neighbour(instance, neighbour);
    }
    return buffer_free(buf);
}

static void rpl_control_transmit_one_interface(protocol_interface_info_entry_t *cur, buffer_t *buf)
{
    /* If destination is global, this will end up routed, regardless of original interface */
    /* (But we do currently need an interface specified anyway?) */
    if (cur) {
        buf->interface = cur;
    }

    /* RPL requires us to use link-local source for all messages except DAO/DAO-ACK
     * in storing mode, which use global source. Default address selection in
     * icmpv6_down should do the right thing, as long as the interface does have
     * both LL and global, so don't bother to set source here.
     */

    protocol_push(buf);
}

static void rpl_control_transmit_all_interfaces(rpl_domain_t *domain, buffer_t *buf)
{
    protocol_interface_info_entry_t *first_if = NULL, *cur;
    int8_t last_id = -1;

    while ((cur = protocol_stack_interface_info_get_by_rpl_domain(domain, last_id)) != NULL) {
        if (!first_if) {
            /* Note first interface - it will get original buffer after loop */
            first_if = cur;
        } else {
            /* This is a subsequent interface - send a clone */
            buffer_t *clone = buffer_clone(buf);
            if (clone) {
                rpl_control_transmit_one_interface(cur, clone);
            }
        }
        last_id = cur->id;
    }

    if (first_if) {
        rpl_control_transmit_one_interface(first_if, buf);
    } else {
        // RPL domain with no interfaces? Odd...
        buffer_free(buf);
    }
}

/* Complete and send a RPL control message - all interfaces multicast if dst+cur are NULL, else unicast */
void rpl_control_transmit(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, uint8_t code, buffer_t *buf, const uint8_t *dst)
{
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
    buf->options.type = ICMPV6_TYPE_INFO_RPL_CONTROL;
    buf->options.code = code;
    buf->dst_sa.addr_type = ADDR_IPV6;
    memcpy(buf->dst_sa.address, dst ? dst : ADDR_LINK_LOCAL_ALL_RPL_NODES, 16);

    /* Use 255 hop limit for link-local stuff, akin to other ICMP */
    /* Others set "0", which means use interface default */
    buf->options.hop_limit = addr_ipv6_scope(buf->dst_sa.address, cur) <= IPV6_SCOPE_LINK_LOCAL ? 255 : 0;


    if (dst == NULL && cur == NULL) {
        rpl_control_transmit_all_interfaces(domain, buf);
    } else {
        /* Fudge - need a dummy interface for non-storing DAO to root - won't
         * actually be used as it'll get globally routed.
         */
        if (!cur) {
            cur = protocol_stack_interface_info_get_by_id(domain->non_storing_downstream_interface);
        }
        rpl_control_transmit_one_interface(cur, buf);
    }
}


/* Transmit a DIO (unicast or multicast); cur may be NULL if multicast */
void rpl_control_transmit_dio(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, uint8_t instance_id, uint8_t dodag_version, uint16_t rank, uint8_t g_mop_prf, uint8_t dtsn, rpl_dodag_t *dodag, const uint8_t dodagid[16], const rpl_dodag_conf_t *conf, const uint8_t *dst)
{
    uint16_t length;

    const rpl_dio_route_list_t *routes = rpl_dodag_get_route_list(dodag);
    const prefix_list_t *prefixes = rpl_dodag_get_prefix_list(dodag);

    tr_debug("transmit dio, rank: %x", rank);
    protocol_interface_info_entry_t *downstream_if = protocol_stack_interface_info_get_by_id(domain->non_storing_downstream_interface);
    length = 24;
    if (conf) {
        length += rpl_control_conf_length();
    }

    ns_list_foreach(prefix_entry_t, prefix, prefixes) {
        /* We must not forward 'L' prefixes */
        if ((prefix->options & (PIO_L | RPL_PIO_PUBLISHED)) == PIO_L) {
            continue;
        }

        /* In storing mode, do not forward a prefix until we have an
         * address for it. (RFC 6550 6.7.10 - "A non-storing node SHOULD
         * refrain from advertising a prefix until it owns an address of
         * that prefix, and then it SHOULD advertise its full address in
         * this field, with the 'R' flag set.
         */
        /* XXX We must also be advertising the corresponding address as a DAO target */
        const uint8_t *addr = downstream_if ? addr_select_with_prefix(downstream_if, prefix->prefix, prefix->prefix_len, 0) : NULL;
        if (addr) {
            prefix->options |= PIO_R;
            memcpy(prefix->prefix, addr, 16);
        } else {
            prefix->options &= ~ PIO_R;

            if (rpl_dodag_mop(dodag) == RPL_MODE_NON_STORING && (prefix->lifetime != 0 || !(prefix->options & PIO_A))) {
                continue;
            }
        }
        length += 32;
    }

    ns_list_foreach(rpl_dio_route_t, route, routes) {
        length += 8 + (route->prefix_len + 7u) / 8;
    }

    /* Add metric lengths here */

    buffer_t *buf = buffer_get(length);
    if (!buf) {
        return;
    }
    uint8_t *ptr = buffer_data_pointer(buf);
    ptr[0] = instance_id;
    ptr[1] = dodag_version;
    common_write_16_bit(rank, ptr + 2);
    ptr[4] = g_mop_prf;
    ptr[5] = dtsn;
    ptr[6] = 0;
    ptr[7] = 0;
    memcpy(ptr + 8, dodagid, 16);
    ptr += 24;

    if (conf) {
        ptr = rpl_control_write_conf(ptr, conf);
    }

    /* Write prefix/route/metric options here */


    ns_list_foreach_safe(prefix_entry_t, prefix, prefixes) {
        /* See equivalent checks in length calculation above */
        if ((prefix->options & (PIO_L | RPL_PIO_PUBLISHED)) == PIO_L ||
                (!(prefix->options & PIO_R) && rpl_dodag_mop(dodag) == RPL_MODE_NON_STORING && (prefix->lifetime != 0 || !(prefix->options & PIO_A)))) {
            continue;
        }

        ptr[0] = RPL_PREFIX_INFO_OPTION;
        ptr[1] = 30;
        ptr[2] = prefix->prefix_len;
        ptr[3] = prefix->options & (PIO_R | PIO_A | PIO_L);
        common_write_32_bit(prefix->lifetime, ptr + 4);
        common_write_32_bit(prefix->preftime, ptr + 8);
        common_write_32_bit(0, ptr + 12); // reserved
        memcpy(ptr + 16, prefix->prefix, 16);
        ptr += 32;
        /* Transmitting a multicast DIO decrements the hold count for 0 lifetime prefixes */
        if (dst == NULL && (prefix->options & RPL_PIO_AGE)) {
            int hold_count = prefix->options & RPL_PIO_HOLD_MASK;
            if (hold_count) {
                hold_count--;
                prefix->options = (prefix->options & ~RPL_PIO_HOLD_MASK) | hold_count;
            }
        }
    }

    ns_list_foreach_safe(rpl_dio_route_t, route, routes) {
        uint8_t prefix_bytes = (route->prefix_len + 7u) / 8u;
        ptr[0] = RPL_ROUTE_INFO_OPTION;
        ptr[1] = 6 + prefix_bytes;
        ptr[2] = route->prefix_len;
        ptr[3] = route->flags;
        common_write_32_bit(route->lifetime, ptr + 4);
        bitcopy0(ptr + 8, route->prefix, route->prefix_len);
        ptr += 8 + prefix_bytes;
        /* Transmitting a multicast DIO decrements the hold count for 0 lifetime routes */
        if (dst == NULL && route->lifetime == 0) {
            if (route->hold_count) {
                route->hold_count--;
            }
            if (route->hold_count == 0) {
                rpl_dodag_delete_dio_route(dodag, route);
            }
        }
    }

    buffer_data_end_set(buf, ptr);

    rpl_control_transmit(domain, cur, ICMPV6_CODE_RPL_DIO, buf, dst);
}

/*  0                   1                   2
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |     Flags     |   Reserved    |   Option(s)...
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                Figure 13: The DIS Base Object
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Type = 0x07 |Opt Length = 19| RPLInstanceID |V|I|D|  Flags  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                            DODAGID                            +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |Version Number |
 *  +-+-+-+-+-+-+-+-+
 *
 *      Figure 28: Format of the Solicited Information Option
 */
static buffer_t *rpl_control_dis_handler(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, buffer_t *buf, bool multicast)
{
    if (!rpl_control_options_well_formed_in_buffer(buf, 2)) {
        protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
        return buffer_free(buf);
    }

    /* Base object irrelevant - find the Solicited Information option, if any */
    const uint8_t *sol = rpl_control_find_option_in_buffer(buf, 2, RPL_SOL_INFO_OPTION, 19);
    uint8_t preds, sol_instance_id, sol_version;
    const uint8_t *sol_dodagid;
    if (sol) {
        preds = sol[3];
        sol_instance_id = sol[2];
        sol_dodagid = sol + 4;
        sol_version = sol[20];
    } else {
        /* No option present means "match any" */
        preds = 0;
        sol_instance_id = 0;
        sol_dodagid = NULL;
        sol_version = 0;
    }

    /* If it's a multicast DIS, then we reset trickle timer for each matching
     * instances.
     *
     * If it's a unicast DIS, we unicast a DIO back to the sender for each
     * matching instance.
     */
    rpl_loopfn_trigger_unicast_dio_arg_t arg;
    arg.interface = cur;
    arg.dst = buf->src_sa.address;

    rpl_control_predicate_loop(domain,
                               multicast ? rpl_loopfn_reset_dio_timer : rpl_loopfn_trigger_unicast_dio,
                               &arg,
                               preds, sol_instance_id, sol_dodagid, sol_version);

    return buffer_free(buf);
}

void rpl_control_transmit_dis(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, const uint8_t version, const uint8_t *dst)
{
    uint16_t length = 2;
    if (pred) {
        length += 2 + 19;
    }
    buffer_t *buf = buffer_get(length);
    if (!buf) {
        return;
    }
    uint8_t *ptr = buffer_data_pointer(buf);
    *ptr++ = 0; // flags
    *ptr++ = 0; // reserved
    if (pred) {
        ptr[0] = RPL_SOL_INFO_OPTION;
        ptr[1] = 19;
        if (pred & RPL_SOLINFO_PRED_INSTANCEID) {
            ptr[2] = instance_id;
        } else {
            ptr[2] = 0;
        }
        ptr[3] = pred;
        if (pred & RPL_SOLINFO_PRED_DODAGID) {
            memcpy(ptr + 4, dodagid, 16);
        } else {
            memset(ptr + 4, 0, 16);
        }
        if (pred & RPL_SOLINFO_PRED_VERSION) {
            ptr[20] = version;
        } else {
            ptr[20] = 0;
        }
        ptr += 21;
    }

    buffer_data_end_set(buf, ptr);
    rpl_control_transmit(domain, cur, ICMPV6_CODE_RPL_DIS, buf, dst);
    tr_info("Transmit DIS");
}

#ifdef HAVE_RPL_DAO_HANDLING
/*
 *         0                   1                   2                   3
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      | RPLInstanceID |D|  Reserved   |  DAOSequence  |    Status     |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +                                                               +
 *      |                                                               |
 *      +                            DODAGID*                           +
 *      |                                                               |
 *      +                                                               +
 *      |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |   Option(s)...
 *      +-+-+-+-+-+-+-+-+
 *
 *                   Figure 17: The DAO ACK Base Object
 */

static void rpl_control_transmit_dao_ack(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, uint8_t instance_id, uint8_t dao_sequence, uint8_t status, const uint8_t dodagid[16], const uint8_t *dst)
{
    buffer_t *buf = buffer_get(dodagid ? 4 + 20 : 4);
    if (!buf) {
        return;
    }
    uint8_t *ptr = buffer_data_pointer(buf);
    ptr[0] = instance_id;
    ptr[1] = dodagid ? RPL_DAO_ACK_FLAG_DODAGID : 0;
    ptr[2] = dao_sequence;
    ptr[3] = status;
    ptr += 4;
    if (dodagid) {
        memcpy(ptr, dodagid, 16);
        ptr += 16;
    }
    buffer_data_end_set(buf, ptr);
    rpl_control_transmit(domain, cur, ICMPV6_CODE_RPL_DAO_ACK, buf, dst);
    tr_info("Transmit DAO-ACK to: %s", trace_ipv6(dst));
}
#endif // HAVE_RPL_DAO_HANDLING

static buffer_t *rpl_control_dao_ack_handler(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, buffer_t *buf)
{
    (void)cur;

    if (buffer_data_length(buf) < 4) {
format_error:
        protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
        return buffer_free(buf);
    }
    const uint8_t *ptr = buffer_data_pointer(buf);
    uint8_t instance_id = ptr[0];
    const uint8_t *dodagid;

    if (ptr[1] & RPL_DAO_ACK_FLAG_DODAGID) {
        if (buffer_data_length(buf) < 4 + 16) {
            goto format_error;
        }
        dodagid = ptr + 4;
    } else {
        dodagid = NULL;
    }

    rpl_instance_t *instance = rpl_lookup_instance(domain, instance_id, dodagid);
    if (!instance) {
        protocol_stats_update(STATS_RPL_UNKNOWN_INSTANCE, 1);
        return buffer_free(buf);
    }

    uint8_t dao_sequence = ptr[2];
    uint8_t status = ptr[3];
    rpl_instance_dao_acked(instance, buf->src_sa.address, buf->interface->id, dao_sequence, status);
    return buffer_free(buf);
}

/*
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | RPLInstanceID |K|D|   Flags   |   Reserved    | DAOSequence   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                            DODAGID*                           +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   Option(s)...
 *  +-+-+-+-+-+-+-+-+
 *
 *               Figure 16: The DAO Base Object
 */
bool rpl_control_transmit_dao(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, rpl_instance_t *instance, uint8_t instance_id, uint8_t dao_sequence, const uint8_t dodagid[16], const uint8_t *opts, uint16_t opts_size, const uint8_t *dst)
{
    uint8_t dodagid_flag = 0;
    if (rpl_instance_id_is_local(instance_id)) {
        dodagid_flag = RPL_DAO_FLAG_DODAGID;
    }

    uint8_t base_size = dodagid_flag ? 4 + 16 : 4;
    buffer_t *buf = buffer_get(base_size + opts_size);
    if (!buf) {
        return true;
    }
    uint8_t *ptr = buffer_data_pointer(buf);
    ptr[0] = instance_id;
    ptr[1] = dodagid_flag;
    if (rpl_policy_request_dao_acks(domain, rpl_instance_mop(instance))) {
        ptr[1] |= RPL_DAO_FLAG_ACK_REQ;
    }
    ptr[2] = 0;
    ptr[3] = dao_sequence;
    if (dodagid_flag) {
        memcpy(ptr + 4, dodagid, 16);
    }
    memcpy(ptr + base_size, opts, opts_size);
    buffer_data_end_set(buf, ptr + base_size + opts_size);

    rpl_control_transmit(domain, cur, ICMPV6_CODE_RPL_DAO, buf, dst);

    return ptr[1] & RPL_DAO_FLAG_ACK_REQ;
}

#ifdef HAVE_RPL_DAO_HANDLING
static buffer_t *rpl_control_dao_handler(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, buffer_t *buf, bool multicast)
{
    if (buffer_data_length(buf) < 4) {
format_error:
        protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
        return buffer_free(buf);
    }
    const uint8_t *ptr = buffer_data_pointer(buf);
    uint8_t instance_id = ptr[0];
    uint8_t flags = ptr[1];
    uint8_t dao_sequence = ptr[3];
    const uint8_t *dodagid;
    tr_info("DAO from %s", trace_ipv6(buf->src_sa.address));
    ptr += 4;
    if (flags & RPL_DAO_FLAG_DODAGID) {
        if (buffer_data_length(buf) < 4 + 16) {
            goto format_error;
        }
        dodagid = ptr;
        ptr += 16;
    } else {
        dodagid = NULL;
    }

    rpl_instance_t *instance = rpl_lookup_instance(domain, instance_id, dodagid);
    if (!instance) {
        protocol_stats_update(STATS_RPL_UNKNOWN_INSTANCE, 1);
        return buffer_free(buf);
    }

    uint16_t opts_len = buffer_data_end(buf) - ptr;
    if (!rpl_control_options_well_formed(ptr, opts_len)) {
        goto format_error;
    }

#if 0
    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    if (!dodag) {
        return buffer_free(buf);

    }
    uint8_t mode = rpl_dodag_mop(dodag);
    switch (mo)!rpl_instance_am_root(instance))

        /* No current processing - pretend to accept */
        uint8_t status = 0;


    }
#endif
uint8_t status;
bool reply_ok = rpl_instance_dao_received(instance, buf->src_sa.address, buf->interface->id, multicast, ptr, opts_len, &status);

/* Ack if requested or non-zero status */
if (reply_ok && ((flags &RPL_DAO_FLAG_ACK_REQ) || status != 0))
{
    rpl_control_transmit_dao_ack(domain, cur, instance_id, dao_sequence, status, dodagid, buf->src_sa.address);
}
return buffer_free(buf);
}
#endif // HAVE_RPL_DAO_HANDLING

buffer_t *rpl_control_handler(buffer_t *buf)
{
    protocol_interface_info_entry_t *cur = buf->interface;
    rpl_domain_t *domain = cur ? cur->rpl_domain : NULL;
    if (!domain) {
        tr_warning("RPL control on non-RPL interface");
        return buffer_free(buf);
    }
    bool multicast = addr_is_ipv6_multicast(buf->dst_sa.address);

    if (addr_is_ipv6_multicast(buf->src_sa.address)) {
        protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
        return buffer_free(buf);
    }

    switch (buf->options.code) {
        case ICMPV6_CODE_RPL_DIS:
            return rpl_control_dis_handler(cur, domain, buf, multicast);
        case ICMPV6_CODE_RPL_DIO:
            return rpl_control_dio_handler(cur, domain, buf);
#ifdef HAVE_RPL_DAO_HANDLING
        case ICMPV6_CODE_RPL_DAO:
            return rpl_control_dao_handler(cur, domain, buf, multicast);
#endif
        case ICMPV6_CODE_RPL_DAO_ACK:
            return rpl_control_dao_ack_handler(cur, domain, buf);
        default:
            tr_warning("Unknown code 0x%02x", buf->options.code);
            protocol_stats_update(STATS_RPL_MALFORMED_MESSAGE, 1);
            return buffer_free(buf);
    }
}

#ifdef HAVE_RPL_ROOT
/* Buffer contains ICMP payload, so 4 bytes unused, followed by invoking packet */
buffer_t *rpl_control_source_route_error_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    if (buffer_data_length(buf) < 40 || !cur->rpl_domain) {
        return buf;
    }

    const uint8_t *target = buffer_data_pointer(buf) + 4 + 24; // Dest in IP header in ICMP invoking packet payload
    const uint8_t *transit = buf->src_sa.address; // Source in IP header of ICMP packet

    tr_warn("Source route error: %s->%s", trace_ipv6(transit), trace_ipv6(target));
    /* We can't identify the instance - logically though it's instance-independent.
     * If transit can't reach target, that applies to all instances.
     */
    ns_list_foreach(rpl_instance_t, instance, &cur->rpl_domain->instances) {
        rpl_downward_transit_error(instance, target, transit);
    }

    return buf;
}
#endif

void rpl_control_fast_timer(uint16_t ticks)
{
    ns_list_foreach(rpl_domain_t, domain, &rpl_domains) {
        ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
            rpl_upward_dio_timer(instance, ticks);
            rpl_downward_dao_timer(instance, ticks);
        }
    }

}

#if 0
static void trace_info_print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vtracef(TRACE_LEVEL_INFO, TRACE_GROUP, fmt, ap);
    va_end(ap);
}
#endif
void rpl_control_slow_timer(uint16_t seconds)
{
    bool purge = rpl_alloc_total > rpl_purge_threshold;

    ns_list_foreach(rpl_domain_t, domain, &rpl_domains) {
        ns_list_foreach_safe(rpl_instance_t, instance, &domain->instances) {
            rpl_control_publish_own_addresses(domain, instance);
            rpl_instance_slow_timer(instance, seconds);
            rpl_downward_dao_slow_timer(instance, seconds);
            /* We purge one item from each instance, so as not to favour one domain or instance */
            if (purge) {
                rpl_instance_purge(instance);
            }
        }
    }

#if 0 // If including this, make sure to include the above trace_info_print helper function as well.
    static int rpl_print_timer;
    if ((rpl_print_timer += seconds) >= 50) {
        rpl_print_timer = 0;
        void arm_print_routing_table2(void (*print_fn)(const char *fmt, ...));
        protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get(IF_6LoWPAN);
        if (cur) {
            ipv6_neighbour_cache_print(&cur->ipv6_neighbour_cache, trace_info_print);
        }
        arm_print_routing_table2(trace_info_print);
        rpl_control_print(trace_info_print);
    }
#endif
}

rpl_instance_t *rpl_control_enumerate_instances(rpl_domain_t *domain, rpl_instance_t *instance)
{
    if (instance) {
        return ns_list_get_next(&domain->instances, instance);
    } else {
        return ns_list_get_first(&domain->instances);
    }
}

rpl_instance_t *rpl_control_lookup_instance(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid)
{
    return rpl_lookup_instance(domain, instance_id, dodagid);
}

bool rpl_control_get_instance_dao_target_count(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid, const uint8_t *prefix, uint16_t *target_count)
{
    rpl_instance_t *instance = rpl_lookup_instance(domain, instance_id, dodagid);
    if (!instance) {
        return false;
    }

    *target_count = rpl_upward_read_dao_target_list_size(instance, prefix);
    return true;
}

/* Backwards-compatibility implementation of net_rpl.h API designed for old implementation */
bool rpl_control_read_dodag_info(const rpl_instance_t *instance, rpl_dodag_info_t *dodag_info)
{
    return rpl_upward_read_dodag_info(instance, dodag_info);
}

const rpl_dodag_conf_t *rpl_control_get_dodag_config(const rpl_instance_t *instance)
{
    rpl_dodag_t *dodag = rpl_instance_current_dodag(instance);
    if (!dodag) {
        return NULL;
    }
    return rpl_dodag_get_config(dodag);
}

const uint8_t *rpl_control_preferred_parent_addr(const rpl_instance_t *instance, bool global)
{
    const rpl_neighbour_t *parent = rpl_instance_preferred_parent(instance);
    if (!parent) {
        return NULL;
    }
    if (global) {
        return rpl_neighbour_global_address(parent);
    } else {
        return rpl_neighbour_ll_address(parent);
    }
}

uint16_t rpl_control_current_rank(const struct rpl_instance *instance)
{
    return rpl_instance_current_rank(instance);
}


static void rpl_domain_print(const rpl_domain_t *domain, route_print_fn_t *print_fn)
{
    print_fn("RPL Domain %p", (void *) domain);
    ns_list_foreach(rpl_instance_t, instance, &domain->instances) {
        rpl_upward_print_instance(instance, print_fn);
        rpl_downward_print_instance(instance, print_fn);
    }
}

void rpl_control_print(route_print_fn_t *print_fn)
{
    print_fn("RPL memory usage %zu", rpl_alloc_total);
    ns_list_foreach(rpl_domain_t, domain, &rpl_domains) {
        rpl_domain_print(domain, print_fn);
    }
}

#ifdef RPL_STRUCTURES_H_
#error "rpl_structures.h should not be included by rpl_control.c"
#endif

#endif /* HAVE_RPL */
