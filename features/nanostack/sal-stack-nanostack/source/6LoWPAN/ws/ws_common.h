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

#ifndef WS_COMMON_H_
#define WS_COMMON_H_


#include "ns_types.h"
#include "ns_list.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"


extern uint16_t test_max_child_count_override;

struct ws_pan_information_s;
struct ws_neighbor_class_s;

typedef struct parent_info_s {
    uint16_t pan_id;            /**< PAN ID */
    uint8_t addr[8];            /**< address */
    uint8_t link_quality;       /**< LQI value measured during reception of the MPDU */
    int8_t signal_dbm;          /**< This extension for normal IEEE 802.15.4 Data indication */
    ws_pan_information_t pan_information;
    ws_utt_ie_t ws_utt;
    ws_us_ie_t ws_us;
    uint32_t timestamp;            /**< Timestamp when packet was received */
} parent_info_t;

typedef struct ws_nud_table_entry {
    void                            *neighbor_info;
    uint16_t                        timer;                    /*!< Timer which resolution is 100ms*/
    unsigned                        retry_count: 2;
    bool                            wait_response: 1;           /*!< True when NS is sended and wait NA, False when random timer is active*/
    bool                            nud_process;
    ns_list_link_t  link;
} ws_nud_table_entry_t;

typedef NS_LIST_HEAD(ws_nud_table_entry_t, link) ws_nud_table_list_t;

typedef struct ws_info_s {
    char network_name[33]; // Network name max 32 octets + terminating 0.
    uint16_t network_pan_id;

    trickle_t trickle_pan_config_solicit;
    trickle_t trickle_pan_config;
    trickle_t trickle_pan_advertisement_solicit;
    trickle_t trickle_pan_advertisement;
    trickle_params_t trickle_params_pan_discovery;
    uint8_t network_size_config; // configuration for network size selection of application.
    uint8_t rpl_state; // state from rpl_event_t
    uint8_t pas_requests; // Amount of PAN solicits sent
    parent_info_t parent_info;
    uint32_t pan_version_timer;            /**< border router version udate timeout */
    uint32_t pan_version_timeout_timer;    /**< routers will fallback to previous state after this */
    uint8_t gtkhash[32];
    bool configuration_learned: 1;
    bool trickle_pas_running: 1;
    bool trickle_pa_running: 1;
    bool trickle_pcs_running: 1;
    bool trickle_pc_running: 1;
    // default fhss parameters for this device
    uint8_t fhss_uc_dwell_interval;
    uint8_t fhss_bc_dwell_interval;
    uint32_t fhss_bc_interval;
    uint8_t fhss_uc_channel_function;
    uint8_t fhss_bc_channel_function;
    uint16_t fhss_uc_fixed_channel;
    uint16_t fhss_bc_fixed_channel;
    uint32_t fhss_channel_mask[8];
    ws_nud_table_entry_t nud_table_entrys[ACTIVE_NUD_PROCESS_MAX];
    ws_nud_table_list_t active_nud_process;
    ws_nud_table_list_t free_nud_entries;
    struct ws_pan_information_s pan_information;
    ws_hopping_schedule_t hopping_schdule;
    struct ws_statistics *stored_stats_ptr;
    struct ws_neighbor_class_s neighbor_storage;
    struct fhss_timer *fhss_timer_ptr; // Platform adaptation for FHSS timers.
    struct fhss_api *fhss_api;
} ws_info_t;

#ifdef HAVE_WS

int8_t ws_generate_channel_list(uint32_t *channel_mask, uint16_t number_of_channels, uint8_t regulatory_domain);

int8_t ws_common_regulatory_domain_config(protocol_interface_info_entry_t *cur);

uint16_t ws_common_channel_number_calc(uint8_t regulatory_domain, uint8_t operating_class);

int8_t ws_common_allocate_and_init(protocol_interface_info_entry_t *cur);

void ws_common_network_size_configure(protocol_interface_info_entry_t *cur, uint16_t network_size);

void ws_common_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_common_fast_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_common_neighbor_update(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

void ws_common_aro_failure(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

void ws_common_neighbor_remove(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

bool ws_common_allow_child_registration(protocol_interface_info_entry_t *cur, const uint8_t *eui64);

void ws_common_etx_validate(protocol_interface_info_entry_t *interface, mac_neighbor_table_entry_t *neigh);

bool ws_common_negative_aro_mark(protocol_interface_info_entry_t *interface, const uint8_t *eui64);


uint32_t ws_common_version_lifetime_get(uint8_t config);

uint32_t ws_common_version_timeout_get(uint8_t config);

#define ws_info(cur) ((cur)->ws_info)
#else
#define ws_info(cur) ((ws_info_t *) NULL)
#define ws_common_seconds_timer(cur, seconds)
#define ws_common_neighbor_update(cur, ll_address) ((void) 0)
#define ws_common_aro_failure(cur, ll_address)
#define ws_common_neighbor_remove(cur, ll_address)
#define ws_common_fast_timer(cur, ticks) ((void) 0)
#define ws_common_allow_child_registration(cur, eui64) (false)
#define ws_common_etx_validate(interface, neigh) ((void) 0)
#define ws_common_negative_aro_mark(interface, eui64)(false)

#endif //HAVE_WS
#endif //WS_COMMON_H_
