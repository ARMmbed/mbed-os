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
#ifndef FHSS_H_
#define FHSS_H_
#define MAX_SCRAMBLE_TABLE_INDEXES  20
// Lifetime is given as seconds
#define BEACON_INFO_LIFETIME        600
// Limits the number of synchronization info messages sent on broadcast channels
#define MAX_SYNCH_INFOS_PER_CHANNEL_LIST    2
// FHSS randomly selects the starting superframe for broadcast channel. This defines how many superframes are used for randomization.
#define NUMBER_OF_BC_START_SUPERFRAMES  3
#define FHSS_MAX_ALLOWED_HOPS   254
#define MAX_FHSS_TIMER_DIVIDER  100
#define SYNCH_MONITOR_AVG_SAMPLES   5
#define FHSS_DATA_START_DELIMETER   0x00
#define FHSS_SYNCHRONIZATION_LOST   10
#define BEACON_INTERVAL_INIT_DIVIDER 100
#define FHSS_SYNCH_DRIFT_TOO_HIGH_LIMIT 20000
#define CLOSE_TO_SUPERFRAME_LENGTH  2000
#define BEACON_FIELD_SIZE(field) (sizeof((fhss_synchronization_beacon_payload_s*)0)->field)

typedef struct fhss_structure fhss_structure_t;
typedef struct fhss_beacon_info fhss_beacon_info_t;
typedef struct fhss_synch_monitor fhss_synch_monitor_s;
typedef struct fhss_failed_tx fhss_failed_tx_t;
typedef struct fhss_bs fhss_bs_t;
typedef struct fhss_synchronization_beacon_payload fhss_synchronization_beacon_payload_s;

struct fhss_synchronization_beacon_payload {
    /** Start delimeter */
    uint8_t data_start_delimeter;
    /** Channel index */
    uint8_t channel_index;
    /** Sender unicast channel index */
    uint8_t sender_unicast_channel;
    /** Current superframe */
    uint16_t current_superframe;
    /** Remaining time (us) to next superframe */
    uint16_t remaining_slots;
    /** Channel list counter */
    uint16_t channel_list_counter;
    /** Hop count */
    uint8_t hop_count;
    /** Number of broadcast channels */
    uint8_t number_of_broadcast_channels;
    /** Number of TX slots per channel */
    uint8_t number_of_tx_slots;
    /** Time since last beacon (us) */
    uint32_t time_since_last_beacon;
    /** Processing delay (us) */
    uint16_t processing_delay;
    /** Superframe length */
    uint16_t superframe_length;
    /** Number of superframes per channel */
    uint8_t number_of_superframes_per_channel;
};

struct fhss_beacon_info {
    uint16_t pan_id;
    uint8_t source_address[8];
    uint32_t timestamp;
    uint8_t synch_info[FHSS_SYNCH_INFO_LENGTH];
    struct fhss_beacon_info *next;
};

struct fhss_synch_monitor {
    int32_t avg_synch_fix;
    int avg_synch_fix_counter;
    int drift_compensation;
    int channel_counter;
};

struct fhss_bs {
    uint8_t uc_channel_index;
    uint8_t current_superframe;
    uint8_t current_channel_index;
    uint8_t beacons_received_timer;
    uint8_t broadcast_start_superframe;
    uint8_t synch_infos_sent_counter;
    bool tx_allowed: 1;
    bool send_synch_info_on_next_broadcast_channel: 1;
    bool beacon_received_on_this_bc_channel: 1;
    uint16_t channel_list_counter;
    uint16_t synch_panid;
    uint32_t synch_interval;
    struct fhss_beacon_info *fhss_beacon_info_store;
    struct fhss_configuration fhss_configuration;
    struct fhss_synch_configuration synch_configuration;
    struct fhss_synch_monitor synch_monitor;
    uint8_t fhss_scramble_table[MAX_SCRAMBLE_TABLE_INDEXES];
};

fhss_structure_t *fhss_enable(fhss_api_t *fhss_api, const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics);
bool fhss_is_synch_root(fhss_structure_t *fhss_structure);
/**
 * Calculate time in microseconds to start of next superframe.
 *
 * @param fhss_struct FHSS structure
 * @return microseconds left to start of next superframe
 */
uint32_t fhss_get_remaining_time_to_next_superframe(const fhss_structure_t *fhss_struct);
int8_t fhss_set_synch_configuration(fhss_structure_t *fhss_structure, const fhss_synch_configuration_t *fhss_synch_configuration);
int fhss_set_callbacks(fhss_structure_t *fhss_structure);
uint8_t fhss_calculate_uc_index(uint8_t channel_index, uint16_t number_of_channels, uint8_t number_of_broadcast_channels);

#endif /* FHSS_H_ */
