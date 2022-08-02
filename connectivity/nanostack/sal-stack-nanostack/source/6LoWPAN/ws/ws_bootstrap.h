/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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

#ifndef WS_BOOTSTRAP_H_
#define WS_BOOTSTRAP_H_

typedef enum {
    WS_INIT_EVENT = 0,       /**< tasklet initializion event*/
    WS_DISCOVERY_START,      /**< discovery start*/
    WS_CONFIGURATION_START,  /**< configuration learn start*/
    WS_OPERATION_START,      /**< active operation start*/
    WS_ROUTING_READY,        /**< RPL routing connected to BR*/
    WS_FAST_DISCONNECT,      /**< Do fast timeout after Border router timeout*/
    WS_NORMAL_DISCONNECT,    /**< Border have been rebooted so Slow poison Process*/
    WS_TEST_PROC_TRIGGER     /**< Trigger test procedure */
} ws_bootsrap_event_type_e;

/* Bootstrap internal test procedures, these must match to ws_test_proc_t
   on net_ws_test_ext.h */
typedef enum {
    PROCEDURE_DIS,
    PROCEDURE_DIO,
    PROCEDURE_DAO,

    PROCEDURE_PAS,
    PROCEDURE_PA,
    PROCEDURE_PCS,
    PROCEDURE_PC,

    PROCEDURE_EAPOL,
    PROCEDURE_RPL,
    PROCEDURE_AUTO_ON,
    PROCEDURE_AUTO_OFF,

    /* Above must match to ws_test_proc_t */

    PROCEDURE_PAS_TRICKLE_INCON,
    PROCEDURE_PCS_TRICKLE_INCON

} ws_bootsrap_procedure_t;

typedef enum {
    WS_PARENT_SOFT_SYNCH = 0,  /**< let FHSS make decision if synchronization is needed*/
    WS_PARENT_HARD_SYNCH,      /**< Synch FHSS with latest synch information*/
    WS_EAPOL_PARENT_SYNCH,  /**< Broadcast synch with EAPOL parent*/
} ws_parent_synch_e;

#ifdef HAVE_WS

//#include "6LoWPAN/ws/ws_llc.h"
#include "6LoWPAN/ws/ws_common_defines.h"

struct rpl_instance;
struct llc_neighbour_req;
struct ws_stack_info;
struct ws_neighbour_info;
struct mcps_data_ie_list;
struct mcps_data_ind_s;

extern uint16_t test_pan_version;

int ws_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode);

int ws_bootstrap_restart(int8_t interface_id);

int ws_bootstrap_restart_delayed(int8_t interface_id);

int ws_bootstrap_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

int ws_bootstrap_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

void ws_bootstrap_configuration_trickle_reset(protocol_interface_info_entry_t *cur);

void ws_bootstrap_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_bootstrap_trickle_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_nud_entry_remove_active(protocol_interface_info_entry_t *cur, void *neighbor);

void ws_nud_active_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_bootstrap_eapol_parent_synch(struct protocol_interface_info_entry *cur, struct llc_neighbour_req *neighbor_info);

void ws_bootstrap_ll_address_validate(struct protocol_interface_info_entry *cur);

uint16_t ws_local_etx_read(protocol_interface_info_entry_t *interface, addrtype_t addr_type, const uint8_t *mac_adddress);

bool ws_bootstrap_nd_ns_transmit(protocol_interface_info_entry_t *cur, ipv6_neighbour_t *entry,  bool unicast, uint8_t seq);

void ws_bootstrap_memory_configuration();

bool ws_bootstrap_validate_channel_plan(struct ws_us_ie *ws_us, struct protocol_interface_info_entry *cur);

bool ws_bootstrap_validate_channel_function(struct ws_us_ie *ws_us, struct ws_bs_ie *ws_bs);

void ws_bootstrap_neighbor_set_stable(struct protocol_interface_info_entry *interface, const uint8_t *src64);

int ws_bootstrap_stack_info_get(protocol_interface_info_entry_t *cur, struct ws_stack_info *info_ptr);

int ws_bootstrap_neighbor_info_get(protocol_interface_info_entry_t *cur, struct ws_neighbour_info *neighbor_ptr, uint16_t table_max);

void ws_bootstrap_mac_neighbor_short_time_set(struct protocol_interface_info_entry *interface, const uint8_t *src64, uint32_t valid_time);

int ws_bootstrap_test_procedure_trigger(protocol_interface_info_entry_t *cur, ws_bootsrap_procedure_t procedure);

/*
 * Functions shared with different bootstrap modes
 */

bool ws_bootstrap_network_name_matches(const struct mcps_data_ie_list *ie_ext, const char *network_name_ptr);

/*State machine transactions*/
void ws_bootstrap_event_discovery_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_configuration_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_authentication_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_operation_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_routing_ready(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_disconnect(protocol_interface_info_entry_t *cur, ws_bootsrap_event_type_e event_type);

void ws_bootstrap_test_procedure_trigger_exec(protocol_interface_info_entry_t *cur, ws_bootsrap_procedure_t procedure);

void ws_bootstrap_network_down(protocol_interface_info_entry_t *cur);

// Bootstrap functions
void ws_bootstrap_start_authentication(protocol_interface_info_entry_t *cur);


// Bootstrap state machine state Functions
bool ws_bootstrap_state_discovery(struct protocol_interface_info_entry *cur);
bool ws_bootstrap_state_authenticate(struct protocol_interface_info_entry *cur);
bool ws_bootstrap_state_configure(struct protocol_interface_info_entry *cur);
bool ws_bootstrap_state_wait_rpl(struct protocol_interface_info_entry *cur);
bool ws_bootstrap_state_active(struct protocol_interface_info_entry *cur);
void ws_bootstrap_state_disconnect(protocol_interface_info_entry_t *cur, ws_bootsrap_event_type_e event_type);
void ws_bootstrap_state_change(protocol_interface_info_entry_t *cur, icmp_state_t nwk_bootstrap_state);

void ws_bootstrap_candidate_list_clean(struct protocol_interface_info_entry *cur, uint8_t pan_max, uint32_t current_time, uint16_t pan_id);
void ws_bootstrap_candidate_parent_store(parent_info_t *parent, const struct mcps_data_ind_s *data, ws_utt_ie_t *ws_utt, ws_us_ie_t *ws_us, ws_pan_information_t *pan_information);
void ws_bootstrap_candidate_table_reset(protocol_interface_info_entry_t *cur);
parent_info_t *ws_bootstrap_candidate_parent_get(struct protocol_interface_info_entry *cur, const uint8_t *addr, bool create);
void ws_bootstrap_candidate_parent_sort(struct protocol_interface_info_entry *cur, parent_info_t *new_entry);
parent_info_t *ws_bootstrap_candidate_parent_get_best(protocol_interface_info_entry_t *cur);

void ws_bootstrap_primary_parent_set(struct protocol_interface_info_entry *cur, struct llc_neighbour_req *neighbor_info, ws_parent_synch_e synch_req);
bool ws_bootstrap_neighbor_info_request(struct protocol_interface_info_entry *interface, const uint8_t *mac_64, struct llc_neighbour_req *neighbor_buffer, bool request_new);
void ws_bootstrap_neighbor_list_clean(struct protocol_interface_info_entry *interface);
int8_t ws_bootstrap_neighbor_set(protocol_interface_info_entry_t *cur, parent_info_t *parent_ptr, bool clear_list);
void ws_address_reregister_trig(struct protocol_interface_info_entry *interface);
void ws_nud_table_reset(protocol_interface_info_entry_t *cur);

void ws_bootstrap_configure_csma_ca_backoffs(protocol_interface_info_entry_t *cur, uint8_t max_backoffs, uint8_t min_be, uint8_t max_be);
void ws_bootstrap_fhss_configure_channel_masks(protocol_interface_info_entry_t *cur, fhss_ws_configuration_t *fhss_configuration);
int8_t ws_bootstrap_fhss_initialize(protocol_interface_info_entry_t *cur);
int8_t ws_bootstrap_fhss_set_defaults(protocol_interface_info_entry_t *cur, fhss_ws_configuration_t *fhss_configuration);
void ws_bootstrap_fhss_activate(protocol_interface_info_entry_t *cur);
uint16_t ws_bootstrap_randomize_fixed_channel(uint16_t configured_fixed_channel, uint8_t number_of_channels, uint32_t *channel_mask);
int ws_bootstrap_set_domain_rf_config(protocol_interface_info_entry_t *cur);
void ws_bootstrap_configure_max_retries(protocol_interface_info_entry_t *cur, uint8_t max_mac_retries);
void ws_bootstrap_configure_data_request_restart(protocol_interface_info_entry_t *cur, uint8_t cca_failure_restart_max, uint8_t tx_failure_restart_max, uint16_t blacklist_min_ms, uint16_t blacklist_max_ms);


void ws_bootstrap_llc_hopping_update(struct protocol_interface_info_entry *cur, const fhss_ws_configuration_t *fhss_configuration);

void ws_bootstrap_ip_stack_reset(protocol_interface_info_entry_t *cur);
void ws_bootstrap_ip_stack_activate(protocol_interface_info_entry_t *cur);

void ws_bootstrap_packet_congestion_init(protocol_interface_info_entry_t *cur);

void ws_bootstrap_asynch_trickle_stop(protocol_interface_info_entry_t *cur);
void ws_bootstrap_advertise_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_network_start(protocol_interface_info_entry_t *cur);
#else

#define ws_bootstrap_init(interface_id, bootstrap_mode) (-1)
#define ws_bootstrap_restart(cur)
#define ws_bootstrap_neighbor_remove(cur, ll_address)
#define ws_bootstrap_aro_failure(cur, ll_address)
#define ws_bootstrap_neighbor_set_stable(interface, src64)
#define ws_bootstrap_stack_info_get(cur, info_ptr)
#define ws_bootstrap_neighbor_info_get(cur, neighbor_ptr, count)
#define ws_bootstrap_test_procedure_trigger(cur, procedure);


#endif //HAVE_WS

#endif /* WS_BOOTSTRAP_H_ */
