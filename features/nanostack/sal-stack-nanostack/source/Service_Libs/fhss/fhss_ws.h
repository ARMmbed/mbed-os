/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

/* WS requires at least 19 MAC retransmissions (total 1+19=20 attempts). 802.15.4 macMaxFrameRetries is 3 (total 1+3=4 attempts).
 * At least 4 channel retries must be used: (Initial channel + WS_NUMBER_OF_CHANNEL_RETRIES) * MAC attempts = (1+4)*4=20 attempts
 */
#define WS_NUMBER_OF_CHANNEL_RETRIES    4
// TX slot length is optimised to this packet length
#define OPTIMAL_PACKET_LENGTH   500
// Default TX/RX slot length in milliseconds. Is used when datarate is not given by PHY.
#define WS_TXRX_SLOT_LEN_MS             100
// Default minimum broadcast synchronization interval in seconds
#define DEFAULT_MIN_SYNCH_INTERVAL      60
// Drift compensation allowed if at least SYNCH_COMPENSATION_MIN_INTERVAL (seconds) since last synchronization
#define SYNCH_COMPENSATION_MIN_INTERVAL 60
// MAX compensation per received synchronization info in ns
#define MAX_DRIFT_COMPENSATION_STEP     10
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
    bool unicast_timer_running;
    bool is_on_bc_channel;
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
void fhss_set_txrx_slot_length(fhss_structure_t *fhss_structure);

#endif /*FHSS_WS_H_*/
