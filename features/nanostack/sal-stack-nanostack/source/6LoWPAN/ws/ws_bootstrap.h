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
    WS_NORMAL_DISCONNECT     /**< Border have been rebooted so Slow poison Process*/
} ws_bootsrap_event_type_e;

#ifdef HAVE_WS

struct llc_neighbour_req;
struct ws_us_ie;
struct ws_bs_ie;
struct ws_neighbor_class_entry;
struct ws_stack_info;
struct ws_neighbour_info;

int ws_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode);

void ws_bootstrap_state_machine(protocol_interface_info_entry_t *cur);

int ws_bootstrap_restart(int8_t interface_id);

int ws_bootstrap_restart_delayed(int8_t interface_id);

int ws_bootstrap_set_rf_config(protocol_interface_info_entry_t *cur, phy_rf_channel_configuration_s rf_configs);

int ws_bootstrap_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

int ws_bootstrap_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

/*State machine transactions*/
void ws_bootstrap_event_discovery_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_configuration_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_authentication_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_operation_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_routing_ready(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_disconnect(protocol_interface_info_entry_t *cur, ws_bootsrap_event_type_e event_type);

void ws_bootstrap_configuration_trickle_reset(protocol_interface_info_entry_t *cur);

void ws_bootstrap_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_bootstrap_trickle_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_bootstrap_primary_parent_update(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neighbor);

void ws_bootstrap_secondary_parent_update(protocol_interface_info_entry_t *interface);

void ws_nud_entry_remove_active(protocol_interface_info_entry_t *cur, void *neighbor);

void ws_nud_active_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local);

void ws_dhcp_client_address_delete(protocol_interface_info_entry_t *cur, uint8_t *prefix);

bool ws_eapol_relay_state_active(protocol_interface_info_entry_t *cur);

void ws_bootstrap_eapol_parent_synch(struct protocol_interface_info_entry *cur, struct llc_neighbour_req *neighbor_info);

bool ws_bootstrap_validate_channel_plan(struct ws_us_ie *ws_us, struct protocol_interface_info_entry *cur);

bool ws_bootstrap_validate_channel_function(struct ws_us_ie *ws_us, struct ws_bs_ie *ws_bs);

void ws_bootstrap_neighbor_set_stable(struct protocol_interface_info_entry *interface, const uint8_t *src64);

int ws_bootstrap_stack_info_get(protocol_interface_info_entry_t *cur, struct ws_stack_info *info_ptr);

int ws_bootstrap_neighbor_info_get(protocol_interface_info_entry_t *cur, struct ws_neighbour_info *neighbor_ptr, uint16_t table_max);

void ws_bootstrap_mac_neighbor_short_time_set(struct protocol_interface_info_entry *interface, const uint8_t *src64, uint32_t valid_time);

#else

#define ws_bootstrap_init(interface_id, bootstrap_mode) (-1)
#define ws_bootstrap_state_machine(cur)
#define ws_bootstrap_restart(cur)
#define ws_bootstrap_neighbor_remove(cur, ll_address)
#define ws_bootstrap_aro_failure(cur, ll_address)
#define ws_bootstrap_neighbor_set_stable(interface, src64)
#define ws_bootstrap_primary_parent_update(interface, neighbor)
#define ws_bootstrap_secondary_parent_update(interface)
#define ws_bootstrap_stack_info_get(cur, info_ptr)
#define ws_bootstrap_neighbor_info_get(cur, neighbor_ptr, count)


#endif //HAVE_WS

#endif /* WS_BOOTSTRAP_H_ */
