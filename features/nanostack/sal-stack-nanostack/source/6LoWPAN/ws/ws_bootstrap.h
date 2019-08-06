/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
    WS_ROUTING_READY        /**< RPL routing connected to BR*/
} ws_bootsrap_event_type_e;

#ifdef HAVE_WS

struct llc_neighbour_req;

int ws_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode);

void ws_bootstrap_state_machine(protocol_interface_info_entry_t *cur);

int ws_bootstrap_restart(int8_t interface_id);

int ws_bootstrap_set_rf_config(protocol_interface_info_entry_t *cur, phy_rf_channel_configuration_s rf_configs);

int ws_bootstrap_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

int ws_bootstrap_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

/*State machine transactions*/
void ws_bootstrap_event_discovery_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_configuration_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_authentication_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_operation_start(protocol_interface_info_entry_t *cur);

void ws_bootstrap_event_routing_ready(protocol_interface_info_entry_t *cur);

void ws_bootstrap_configuration_trickle_reset(protocol_interface_info_entry_t *cur);

void ws_bootstrap_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_bootstrap_trickle_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_primary_parent_update(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neighbor);

void ws_secondary_parent_update(protocol_interface_info_entry_t *interface);

void ws_nud_entry_remove_active(protocol_interface_info_entry_t *cur, void *neighbor);

void ws_nud_active_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local);

void ws_dhcp_client_address_delete(protocol_interface_info_entry_t *cur, uint8_t *prefix);

bool ws_eapol_relay_state_active(protocol_interface_info_entry_t *cur);

void ws_bootstrap_eapol_parent_synch(struct protocol_interface_info_entry *cur, struct llc_neighbour_req *neighbor_info);

void ws_bootstrap_etx_accelerate(struct protocol_interface_info_entry *cur, mac_neighbor_table_entry_t *neigh);

#else

#define ws_bootstrap_init(interface_id, bootstrap_mode) (-1)
#define ws_bootstrap_state_machine(cur)
#define ws_bootstrap_restart(cur)
#define ws_bootstrap_neighbor_remove(cur, ll_address)
#define ws_bootstrap_aro_failure(cur, ll_address)
#define ws_primary_parent_update(interface, neighbor)
#define ws_secondary_parent_update(interface)
#define ws_bootstrap_etx_accelerate(cur, neigh) ((void) 0)

#endif //HAVE_WS

#endif /* WS_BOOTSTRAP_H_ */
