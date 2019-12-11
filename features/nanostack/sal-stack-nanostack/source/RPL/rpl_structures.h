/*
 * Copyright (c) 2015-2018, Arm Limited and affiliates.
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

#ifndef RPL_STRUCTURES_H_
#define RPL_STRUCTURES_H_

/* RPLstructures are public only for the core processing (rpl_upward,
 * rpl_downward, and objective function implementations). This file should
 * not be included by rpl_control, rpl_objective or rpl_policy, or any
 * external user.
 */

#include <stdint.h>
#include <stdbool.h>

#include "Service_Libs/Trickle/trickle.h"

struct rpl_objective;

/* Descriptor for a RPL neighbour within a DODAG
 *
 * The neighbour is normally associated with a DODAG Version, but may not be,
 * if the version has been retired, and we haven't since heard from that
 * neighbour. In that case dodag_version is NULL.
 *
 * Note that global address is only needed with downward routes, but I don't
 * think it's worth optimising for an "upward-only" build. (Unless to be a RPL
 * leaf?)
 *
 * DODAG parents are identified by the dodag_parent flag, and they are sorted
 * first in the instance candidate_neighbour list, in order of preference.
 */
struct rpl_neighbour {
    rpl_dodag_version_t *dodag_version;     // Back pointer to DODAG Version (may be NULL, if not dodag_parent)
    uint8_t ll_address[16];                 // Link-local address (source of DIO)
    uint8_t global_address[16];             // Global address (from DIO RIO)
    bool dodag_parent: 1;                   // This is a DODAG parent (if true, dodag_version may not be NULL)
    bool was_dodag_parent: 1;               // Was a DODAG parent (used only during parent selection)
    bool have_global_address: 1;            // Global address known
    bool considered: 1;                     // Have considered at least once for parent selection
    bool confirmed: 1;                      // Confirmed
    unsigned dodag_pref: 4;                 // Preference indication for DODAG parents (0=best)
    uint8_t dao_path_control;               // Path control bit assignments for DAO parent
    uint8_t old_dao_path_control;
    int8_t interface_id;
    uint8_t g_mop_prf;
    uint8_t dtsn;
    uint16_t rank;
    uint32_t dio_timestamp;                 // Timestamp of last received DIO message
    /* Need link quality indication */
    ns_list_link_t candidate_neighbour_link;
};

typedef NS_LIST_HEAD(rpl_neighbour_t, candidate_neighbour_link) rpl_neighbour_list_t;

/* Descriptor for a DODAG Version, identified by (RPLInstanceID, DODAGID, DODAGVersionNumber) */
struct rpl_dodag_version {
    rpl_dodag_t *dodag;                             /* Back pointer to DODAG */
    uint8_t number;                                 /* Version Number */
    uint16_t lowest_advertised_rank;
    uint16_t last_advertised_rank;
    uint16_t hard_rank_limit;                       /* Hard rank limit by DAGMaxRankIncrease */
    uint16_t greediness_rank_limit;                 /* Rank limit for greediness */
    ns_list_link_t link;
};

/* Descriptor for a DODAG. A DODAG is identified by a (RPLInstanceID, DODAGID) tuple */
struct rpl_dodag {
    rpl_instance_t *instance;                       /* Back pointer to instance */
    uint32_t timestamp;                             /* How long since we heard a DIO */
    uint8_t id[16];                                 /* Root identifier */
    uint8_t g_mop_prf;                              /* Grounded, Mode, Preference */
    rpl_dodag_conf_t config;                        /* Configuration from DIO */
    uint8_t info_version;                           /* Version for g_mop_prf and config */
    bool root: 1;                                   /* We are the root of this DODAG */
    bool leaf: 1;                                   /* We are a leaf in this DODAG (by policy) */
    bool have_config: 1;                            /* We have the config */
    bool used: 1;                                   /* We have ever been a member of this DODAG? */
    uint8_t new_config_advertisment_count;      /* We have advertiment new config at multicasti DIO */
    NS_LIST_HEAD(rpl_dodag_version_t, link) versions; /* List of DODAG versions (newest first) */
    prefix_list_t prefixes;                         /* Prefixes advertised in DIO PIOs */
    rpl_dio_route_list_t routes;                    /* Routes advertised in DIO RIOs*/
    trickle_params_t dio_timer_params;              /* Trickle parameters */
    ns_list_link_t link;
};

typedef struct rpl_dao_target rpl_dao_target_t;

/* List of transits for a DAO target in a non-storing root */
typedef struct rpl_dao_root_transit {
    uint8_t transit[16];
    rpl_dao_target_t *parent;           /* Current parent matched by transit. NULL if DODAG root, the original target if no match */
    rpl_dao_target_t *target;
    uint8_t path_control;
    uint16_t cost;
    ns_list_link_t parent_link;
    ns_list_link_t target_link;
} rpl_dao_root_transit_t;

typedef NS_LIST_HEAD(rpl_dao_root_transit_t, target_link) rpl_dao_root_transit_list_t;
typedef NS_LIST_HEAD(rpl_dao_root_transit_t, parent_link) rpl_dao_root_transit_children_list_t;

/* Information held for a DAO target in a non-storing root */
typedef struct rpl_dao_root {
    uint32_t cost;                      /* Routing cost - (used as number of incoming graph edges during topo sort) */
    rpl_dao_root_transit_children_list_t children;   /* Child list - only valid after routing cost computation */
    rpl_dao_root_transit_list_t transits;
} rpl_dao_root_t;

/* Information held for a DAO target in other nodes */
typedef struct rpl_dao_non_root {
    uint8_t path_lifetime;              /* Does this count down? 0 means "not set yet" on published targets - we don't hold "No-Path" entries */
    uint8_t pc_assigning;               /* Bitfield of Path Control being assigned by in-flight DAO */
    uint8_t pc_assigned;                /* Bitfield of Path Control successfully assigned */
    uint8_t pc_to_retry;                /* Bitfield of Path Control assignment to be retried */
    uint32_t refresh_timer;             /* Refresh timer (seconds) - 0xFFFFFFFF = infinite, 0 = not yet set */
} rpl_dao_non_root_t;


/* Descriptor for a RPL DAO target */
struct rpl_dao_target {
    rpl_instance_t *instance;
    uint8_t prefix[16];
    uint8_t prefix_len;
    uint8_t path_sequence;
    uint8_t path_control;
    uint8_t response_wait_time;
    int8_t interface_id;
    uint32_t lifetime;                  /* Seconds */
    uint32_t descriptor;                /* Target descriptor */
    bool external: 1;                   /* RPL 'E' flag */
    bool root: 1;                       /* This is a "root" structure - root member of info active */
    bool published: 1;                  /* Are we publishing, or did we get it from a DAO? (non-root) */
    bool own: 1;                        /* Is this our own address, or are we publishing for an attached host? (non-root) */
    bool descriptor_present: 1;         /* Target descriptor specified */
    bool need_seq_inc: 1;
    bool connected: 1;                  /* We know this target has a path to the root */
    bool trig_confirmation_state: 1;         /* Enable confirmation to parent's */
    bool active_confirmation_state: 1;
    union {
#ifdef HAVE_RPL_ROOT
        rpl_dao_root_t root;            /* Info specific to a non-storing root */
#endif
        rpl_dao_non_root_t non_root;    /* Info for other nodes (any in storing, non-root in non-storing) */
    } info;
    ns_list_link_t link;
};

typedef NS_LIST_HEAD(rpl_dao_target_t, link) rpl_dao_target_list_t;

/* Descriptor for a RPL Instance. An instance can have multiple DODAGs.
 *
 * If top bit of instance_id is set then it's a local DODAG, and the dodags
 * list must have exactly one member, whose dodag_id disambiguates the
 * instance_id.
 */
struct rpl_instance {
    ns_list_link_t link;
    rpl_domain_t *domain;                           /* Back pointer to domain */
    uint8_t id;                                     /* RPLInstanceID */
    uint8_t dtsn;                                   /* Our DTSN for this instance */
    bool neighbours_changed: 1;
    bool local_repair: 1;
    bool root_topo_sort_valid: 1;
    bool root_paths_valid: 1;
    bool dio_not_consistent: 1;                     /* Something changed - not consistent this period */
    bool dao_in_transit: 1;                         /* If we have a DAO in transit */
    bool requested_dao_ack: 1;                      /* If we requested an ACK (so we retry if no ACK, rather than assuming success) */
    bool pending_neighbour_confirmation: 1;         /* if we have not finished address registration state to parent */
    uint8_t poison_count;
    uint8_t repair_dis_count;
    uint16_t repair_dis_timer;
    uint32_t last_dao_trigger_time;
    uint16_t srh_error_count;                       /* SRH errors since last DAO trigger */
    NS_LIST_HEAD(rpl_dodag_t, link) dodags;         /* List of DODAGs */
    rpl_neighbour_t *wait_response;
    rpl_neighbour_list_t candidate_neighbours;      /* Candidate neighbour set */
    // rpl_neighbour_list_t old_neighbours;            /* Old neighbours (without a live DODAG version) */
    rpl_dodag_version_t *current_dodag_version;     /* Pointer to DODAG version we are a member of (if any) */
    uint16_t current_rank;                          /* Current rank in current DODAG Version */
    uint16_t parent_selection_timer;
    rpl_dodag_version_t *last_advertised_dodag_version; /* Pointer to last DODAG version we advertised */
    trickle_t dio_timer;                            /* Trickle timer for DIO transmission */
    rpl_dao_root_transit_children_list_t root_children;
    rpl_dao_target_list_t dao_targets;              /* List of DAO targets */
    uint8_t dao_sequence;                           /* Next DAO sequence to use */
    uint8_t dao_sequence_in_transit;                /* DAO sequence in transit (if dao_in_transit) */
    uint16_t delay_dao_timer;
    uint16_t dao_retry_timer;
    uint8_t dao_attempt;
    struct rpl_objective *of;                       /* Objective function pointer */
};

/* rpl_control.h uses NS_LIST_HEAD_INCOMPLETE */
NS_STATIC_ASSERT(offsetof(struct rpl_instance, link) == 0, "Link must be first in struct rpl_instance")



#endif /* RPL_STRUCTURES_H_ */
