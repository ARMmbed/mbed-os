/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#ifndef SW_MAC_INTERNAL_H
#define SW_MAC_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    STAT_MAC_TX_QUEUE,
    STAT_MAC_RX_COUNT,
    STAT_MAC_TX_COUNT,
    STAT_MAC_BC_RX_COUNT,
    STAT_MAC_BC_TX_COUNT,
    STAT_MAC_BEA_RX_COUNT,
    STAT_MAC_BEA_TX_COUNT,
    STAT_MAC_RX_DROP,
    STAT_MAC_TX_FAIL,
    STAT_MAC_TX_RETRY,
    STAT_MAC_TX_CCA_ATT,
    STAT_MAC_TX_CCA_FAIL,
} mac_stats_type_t;

typedef enum arm_nwk_timer_id {
    ARM_NWK_MAC_TIMER = 0,
    ARM_NWK_IFS_TIMER = 1,
    ARM_NWK_MLME_TIMER = 2,
    ARM_NWK_CCA_TIMER = 3,
    ARM_NWK_BC_TIMER = 4,
    ARM_MCPS_TIMER = 5,
} arm_nwk_timer_id_e;

struct mac_api_s;
struct fhss_api;
struct protocol_interface_rf_mac_setup;

struct mac_api_s *get_sw_mac_api(struct protocol_interface_rf_mac_setup *setup);
struct protocol_interface_rf_mac_setup *get_sw_mac_ptr_by_mac_api(struct mac_api_s *api);
struct protocol_interface_rf_mac_setup *get_sw_mac_ptr_by_fhss_api(const struct fhss_api *api);
struct protocol_interface_rf_mac_setup *get_sw_mac_ptr_by_timer(int8_t id, enum arm_nwk_timer_id type);
struct protocol_interface_rf_mac_setup *get_sw_mac_ptr_by_driver_id(int8_t id);
void sw_mac_stats_update(struct protocol_interface_rf_mac_setup *setup, mac_stats_type_t type, uint32_t update_val);

#ifdef __cplusplus
}
#endif

#endif // SW_MAC_INTERNAL_H
