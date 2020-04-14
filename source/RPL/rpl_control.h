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

#ifndef RPL_CONTROL_H_
#define RPL_CONTROL_H_

#ifdef HAVE_RPL

#include "ns_list.h"
#include "ipv6_stack/ipv6_routing_table.h"

struct rpl_instance;
struct rpl_dodag_conf;
struct rpl_dodag;
struct buffer;
struct protocol_interface_info_entry;
struct rpl_dodag_info_t;
struct prefix_entry_t;

typedef enum rpl_event {
    RPL_EVENT_DAO_DONE,         /* Simplistic trigger for bootstrap advance - a DAO registration completed */
    RPL_EVENT_LOCAL_REPAIR_START, /* RPL start scanning new parent by multicast DIS user can disable beacon request responser here*/
    RPL_EVENT_LOCAL_REPAIR_NO_MORE_DIS, /* RPL not sending DIS anymore user can report bootstrap error */
    RPL_EVENT_DAO_PARENT_ADD, /* RPL indicate that DAO downward Parent has been added */
} rpl_event_t;

typedef void rpl_domain_callback_t(rpl_event_t event, void *handle);

typedef void rpl_prefix_callback_t(struct prefix_entry_t *prefix, void *handle, uint8_t *parent_link_local);

typedef bool rpl_new_parent_callback_t(uint8_t *ll_parent_address, void *handle, struct rpl_instance *instance, uint16_t candidate_rank);


typedef struct rpl_domain {
    NS_LIST_HEAD_INCOMPLETE(struct rpl_instance) instances;
    ns_list_link_t link;
    /* For non-storing mode, we have to assume a "downstream" interface -
     * routing headers contain global addresses that we won't know, and we have
     * to assume they're on link on an assumed interface.
     */
    int8_t non_storing_downstream_interface;
    /* As part of shutdown, we can force entering leaf mode */
    bool force_leaf;
    /* if false routes are not set to routing table, instead default route is added for DODAGID */
    bool process_routes;
    rpl_domain_callback_t *callback;
    rpl_prefix_callback_t *prefix_cb;
    rpl_new_parent_callback_t *new_parent_add;
    void *cb_handle;
} rpl_domain_t;

/* Configuration parameters for a DODAG, obtained through DIO DODAG Configuration options */
typedef struct rpl_dodag_conf {
    bool authentication: 1;
    unsigned path_control_size: 3;
    uint8_t dio_interval_min;               /* log2 milliseconds */
    uint8_t dio_interval_doublings;
    uint8_t dio_redundancy_constant;
    uint8_t default_lifetime;               /* lifetime units */
    uint16_t dag_max_rank_increase;
    uint16_t min_hop_rank_increase;
    uint16_t objective_code_point;
    uint16_t lifetime_unit;                 /* seconds */
} rpl_dodag_conf_t;

/* Descriptor for a route from a DIO Route Information option.
 * Used to hold the "master copy" in the DODAG structure - the table for the
 * current DODAG is used to populate routes in our system routing table, and to
 * form our own DIOs.
 * Not used directly for next hop determination - this is done via the system
 * routing table, which has per-parent information including parent preferences,
 * reachability/probing info, etc.
 */
typedef struct rpl_dio_route {
    uint32_t lifetime;                              /* Lifetime in seconds */
    ns_list_link_t link;
    bool age;                                       /* Do we age the lifetime? */
    uint8_t hold_count;                             /* Hold count for relaying 0 lifetime */
    uint8_t flags;                                  /* Flags byte (inc. pref) */
    uint8_t prefix_len_bytes;                       /* Prefix length (bytes) - padding preserved when relaying */
    uint8_t prefix_len;                             /* Prefix length (bits) */
    uint8_t prefix[];                               /* Variable-length prefix */
} rpl_dio_route_t;

typedef NS_LIST_HEAD(rpl_dio_route_t, link) rpl_dio_route_list_t;

/******************************* RPL internal API ****************************/
void *rpl_alloc(uint16_t size);
void *rpl_realloc(void *p, uint16_t old_size, uint16_t new_size);
void rpl_free(void *p, uint16_t size);
void rpl_control_transmit(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, uint8_t code, struct buffer *buf, const uint8_t *dst);
void rpl_control_transmit_multicast_dio(struct rpl_domain *domain, struct rpl_instance *instance, uint8_t instance_id, uint8_t dodag_version, uint16_t rank, uint8_t g_mop_prf, uint8_t dtsn, const uint8_t dodagid[16], const struct rpl_dodag_conf *conf);
void rpl_control_transmit_dio(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, uint8_t instance_id, uint8_t dodag_version, uint16_t rank, uint8_t g_mop_prf, uint8_t dtsn, struct rpl_dodag *dodag, const uint8_t dodagid[16], const struct rpl_dodag_conf *conf, const uint8_t *dst);
bool rpl_control_transmit_dao(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, struct rpl_instance *instance, uint8_t instance_id, uint8_t dao_sequence, const uint8_t dodagid[16], const uint8_t *opts, uint16_t opts_size, const uint8_t *dst);
void rpl_control_disable_ra_routes(struct rpl_domain *domain);
void rpl_control_event(struct rpl_domain *domain, rpl_event_t event);
void rpl_control_process_prefix_option(struct prefix_entry_t *prefix, struct protocol_interface_info_entry *cur);

/*********************** RPL control API to rest of system *******************/

/* Memory configuration */
void rpl_control_set_memory_limits(size_t soft_limit, size_t hard_limit);

/* Timer routines */
void rpl_control_fast_timer(uint16_t ticks);
void rpl_control_slow_timer(uint16_t seconds);

/* Packet handlers, and other data flow callback indications */
struct buffer *rpl_control_handler(struct buffer *buf);
struct buffer *rpl_control_source_route_error_handler(struct buffer *buf, struct protocol_interface_info_entry *cur);

/* Manually send DIS packets for bootstrap */
void rpl_control_transmit_dis(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, const uint8_t version, const uint8_t *dst);
bool rpl_control_have_dodag(struct rpl_domain *domain);

/* APIs used to manipulate configuration at the root */
struct rpl_dodag *rpl_control_create_dodag_root(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid, const rpl_dodag_conf_t *conf, uint16_t rank, uint8_t g_mop_prf);
void rpl_control_delete_dodag_root(rpl_domain_t *domain, struct rpl_dodag *dodag);
void rpl_control_update_dodag_route(struct rpl_dodag *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age);
void rpl_control_update_dodag_prefix(struct rpl_dodag *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool age);
uint8_t rpl_control_increment_dodag_version(struct rpl_dodag *dodag);
void rpl_control_update_dodag_config(struct rpl_dodag *dodag, const rpl_dodag_conf_t *conf);
void rpl_control_set_dodag_pref(struct rpl_dodag *dodag, uint8_t pref);
void rpl_control_increment_dtsn(struct rpl_dodag *dodag);

/* Force leaf behaviour on a domain - useful before shutdown, and in conjunction with poison */
void rpl_control_force_leaf(rpl_domain_t *domain, bool leaf);
/*Process routes from DIOs and add those as real routes. if routes are not processed assume DODAGID as default route*/
void rpl_control_process_routes(rpl_domain_t *domain, bool process_routes);

/* Manually send poison on all existing instances a few times */
void rpl_control_poison(rpl_domain_t *domain, uint8_t poison_count);

/* force DAO to verify connections before given time*/
void rpl_control_dao_timeout(rpl_domain_t *domain, uint16_t seconds);

/* APIs to create domains and map them to interfaces */
rpl_domain_t *rpl_control_create_domain(void);
void rpl_control_delete_domain(rpl_domain_t *domain);
void rpl_control_set_domain_on_interface(struct protocol_interface_info_entry *cur, rpl_domain_t *domain, bool downstream);
void rpl_control_remove_domain_from_interface(struct protocol_interface_info_entry *cur);
void rpl_control_free_domain_instances_from_interface(struct protocol_interface_info_entry *cur);
void rpl_control_set_callback(rpl_domain_t *domain, rpl_domain_callback_t callback, rpl_prefix_callback_t prefix_learn_cb, rpl_new_parent_callback_t new_parent_add, void *cb_handle);

/* Target publishing */
void rpl_control_publish_host_address(rpl_domain_t *domain, const uint8_t addr[16], uint32_t lifetime);
void rpl_control_unpublish_address(rpl_domain_t *domain, const uint8_t addr[16]);
bool rpl_control_is_dodag_parent(struct protocol_interface_info_entry *interface, const uint8_t ll_addr[16]);
bool rpl_control_is_dodag_parent_candidate(struct protocol_interface_info_entry *interface, const uint8_t ll_addr[16], uint16_t candidate_cmp_limiter);
bool rpl_control_probe_parent_candidate(struct protocol_interface_info_entry *interface, const uint8_t ll_addr[16]);
bool rpl_possible_better_candidate(struct protocol_interface_info_entry *interface, struct rpl_instance *rpl_instance, const uint8_t ll_addr[16], uint16_t candidate_rank, uint16_t etx);
uint16_t rpl_control_parent_candidate_list_size(struct protocol_interface_info_entry *interface, bool parent_list);
uint16_t rpl_control_candidate_list_size(struct protocol_interface_info_entry *interface, struct rpl_instance *rpl_instance);
uint16_t rpl_control_selected_parent_count(struct protocol_interface_info_entry *interface, struct rpl_instance *rpl_instance);
void rpl_control_neighbor_delete(struct protocol_interface_info_entry *interface, const uint8_t ll_addr[16]);
void rpl_control_neighbor_delete_from_instance(struct protocol_interface_info_entry *interface, struct rpl_instance *rpl_instance, const uint8_t ll_addr[16]);
bool rpl_control_find_worst_neighbor(struct protocol_interface_info_entry *interface, struct rpl_instance *rpl_instance, uint8_t ll_addr[16]);

/* Parent link confirmation API extension */
void rpl_control_request_parent_link_confirmation(bool requested);
void rpl_control_set_dio_multicast_min_config_advertisment_count(uint8_t min_count);
void rpl_control_set_address_registration_timeout(uint16_t timeout_in_minutes);
void rpl_control_set_dao_retry_count(uint8_t count);
void rpl_control_set_minimum_dao_target_refresh(uint16_t seconds);
void rpl_control_set_initial_dao_ack_wait(uint16_t timeout_in_ms);
void rpl_control_set_mrhof_parent_set_size(uint16_t parent_set_size);
void rpl_control_register_address(struct protocol_interface_info_entry *interface, const uint8_t addr[16]);
bool rpl_control_address_register_done(struct protocol_interface_info_entry *interface, const uint8_t ll_addr[16], uint8_t status);

/* Configure and return the routing lookup predicate for a specified RPL instance ID */
ipv6_route_predicate_fn_t *rpl_control_get_route_predicate(rpl_domain_t *domain, uint8_t instance_id, const uint8_t src[16], const uint8_t dst[16]);

/* Diagnostic APIs */
void rpl_control_print(route_print_fn_t *print_fn);

struct rpl_instance *rpl_control_enumerate_instances(rpl_domain_t *domain, struct rpl_instance *instance);
struct rpl_instance *rpl_control_lookup_instance(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid);
bool rpl_control_get_instance_dao_target_count(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid, const uint8_t *prefix, uint16_t *target_count);
bool rpl_control_read_dodag_info(const struct rpl_instance *instance, struct rpl_dodag_info_t *dodag_info);
const rpl_dodag_conf_t *rpl_control_get_dodag_config(const struct rpl_instance *instance);
const uint8_t *rpl_control_preferred_parent_addr(const struct rpl_instance *instance, bool global);
uint16_t rpl_control_current_rank(const struct rpl_instance *instance);
uint8_t rpl_policy_mrhof_parent_set_size_get(const rpl_domain_t *domain);

#else /* HAVE_RPL */

#define rpl_control_fast_timer(ticks) ((void) 0)
#define rpl_control_slow_timer(seconds) ((void) 0)
#define rpl_control_remove_domain_from_interface(cur) ((void) 0)
#define rpl_control_free_domain_instances_from_interface(cur) ((void) 0)
#define rpl_control_register_address(interface, addr) ((void) 0)
#define rpl_control_address_register_done(interface, ll_addr, status) (false)
#define rpl_policy_mrhof_parent_set_size_get(domain) (0)
#define rpl_control_set_mrhof_parent_set_size(parent_set_size)
#endif /* HAVE_RPL */

#endif /* RPL_CONTROL_H_ */
