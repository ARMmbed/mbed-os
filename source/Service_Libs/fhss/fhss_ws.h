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
#ifndef FHSS_WS_H_
#define FHSS_WS_H_

// TX slot length is optimised to this packet length
#define OPTIMAL_PACKET_LENGTH               500
// Max TX/RX slot length in milliseconds. Is used when datarate is not given by PHY or calculated slot length exceeds maximum allowed.
#define WS_TXRX_SLOT_LEN_MS_MAX             100
// Min TX/RX slot length in milliseconds. Is used when calculated slot length is under minimum allowed.
#define WS_TXRX_SLOT_LEN_MS_MIN             13
// Default minimum broadcast synchronization interval in seconds
#define DEFAULT_MIN_SYNCH_INTERVAL          60
// Drift compensation allowed if at least SYNCH_COMPENSATION_MIN_INTERVAL (seconds) since last synchronization
#define SYNCH_COMPENSATION_MIN_INTERVAL     60
// MAX compensation per received synchronization info in ns
#define MAX_DRIFT_COMPENSATION_STEP         10
// Length of expedited forwarding period in seconds
#define EXPEDITED_FORWARDING_PERIOD         5
// TX poll interval for expedited forwarding in 50us slots
#define EXPEDITED_FORWARDING_POLL_PERIOD    (5000 / 50)
// TX poll interval used when channel schedules are not yet started (50us slots)
#define DEFAULT_POLL_PERIOD    (10000 / 50)
typedef struct fhss_ws fhss_ws_t;

struct fhss_ws {
    uint8_t bc_channel;
    uint16_t uc_slot;
    uint16_t bc_slot;
    uint16_t min_synch_interval;
    uint32_t txrx_slot_length_ms;
    uint32_t synchronization_time;
    uint32_t unicast_start_time_us;
    int32_t drift_per_millisecond_ns;
    int16_t *tr51_channel_table;
    uint8_t *tr51_output_table;
    uint32_t next_uc_timeout;
    uint32_t next_bc_timeout;
    uint32_t expedited_forwarding_enabled_us;
    bool unicast_timer_running;
    bool broadcast_timer_running;
    bool is_on_bc_channel;
    fhss_ws_tx_allow_level tx_level;
    fhss_ws_tx_allow_level ef_tx_level;
    struct fhss_ws_configuration fhss_configuration;
    const struct broadcast_timing_info *parent_bc_info;
    fhss_get_neighbor_info *get_neighbor_info;
};

fhss_structure_t *fhss_ws_enable(fhss_api_t *fhss_api, const fhss_ws_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer);
int fhss_ws_set_callbacks(fhss_structure_t *fhss_structure);
int fhss_ws_set_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8], const broadcast_timing_info_t *bc_timing_info, const bool force_synch);
int fhss_ws_remove_parent(fhss_structure_t *fhss_structure, const uint8_t eui64[8]);
int fhss_ws_configuration_set(fhss_structure_t *fhss_structure, const fhss_ws_configuration_t *fhss_configuration);
int fhss_ws_set_hop_count(fhss_structure_t *fhss_structure, const uint8_t hop_count);
int fhss_ws_set_tx_allowance_level(fhss_structure_t *fhss_structure, const fhss_ws_tx_allow_level global_level, const fhss_ws_tx_allow_level ef_level);
void fhss_set_txrx_slot_length(fhss_structure_t *fhss_structure);

#endif /*FHSS_WS_H_*/
