/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "ns_list.h"
#define MAX_SCRAMBLE_TABLE_INDEXES  20

// Lifetime is given as seconds
#define BEACON_INFO_LIFETIME        600
// Limits the number of synchronization info messages sent on broadcast channels
#define MAX_SYNCH_INFOS_PER_CHANNEL_LIST	2
// FHSS randomly selects the starting superframe for broadcast channel. This defines how many superframes are used for randomization.
#define NUMBER_OF_BC_START_SUPERFRAMES  3

#define FHSS_TASKLET_INIT_EVENT             0
#define FHSS_TIMER_EVENT                    1
#define FHSS_COMPARE_SYNCH_PARENT           2
#define FHSS_BROADCAST_CHANNEL              3
#define FHSS_UPDATE_SYNCH_INFO_STORAGE      4

struct fhss_callback;

typedef struct fhss_beacon_info
{
    uint16_t pan_id;
    uint8_t source_address[8];
    uint32_t timestamp;
    uint8_t synch_info[FHSS_SYNCH_INFO_LENGTH];
    struct fhss_beacon_info *next;
}fhss_beacon_info_t;

typedef struct fhss_failed_tx
{
    uint8_t handle;
    uint8_t bad_channel;
    uint8_t retries_done;
    ns_list_link_t link;
}fhss_failed_tx_t;

typedef NS_LIST_HEAD(fhss_failed_tx_t, link) fhss_failed_tx_list_t;

typedef struct
{
    int32_t avg_synch_fix;
    int avg_synch_fix_counter;
    int drift_compensation;
    int channel_counter;
} fhss_synch_monitor_s;

typedef struct
{
    fhss_api_t *fhss_api;
    uint32_t datarate;
    fhss_states fhss_state;
    fhss_timer_t platform_functions;
    fhss_configuration_t fhss_configuration;
    fhss_synch_configuration_t synch_configuration;

    uint8_t fhss_resolution_divider;
    /** Unicast channel index, [0..(number_of_channels-number_of_beacon_channels-1)] */
    uint8_t uc_channel_index;
    /** Current superframe number, [0..(fhss_number_of_superframes-1)] */
    uint8_t current_superframe;
    /** Current channel index, [0..(number_of channels-1)] */
    uint8_t current_channel_index;
    /** Current broadcast index, [0..(number_of_bc_channels-1)] */
    uint8_t broadcast_index;
    /** Number of channels (unicast and broadcast) */
    uint16_t number_of_channels;
    /** Channel list counter is increased every time channel list is gone through*/
    uint16_t channel_list_counter;
    /** This is used to store current RX channel*/
    uint8_t rx_channel;
    /** Own hop count*/
    uint8_t own_hop;
    /** Holds the information: transmission is allowed or not on this superframe*/
    bool tx_allowed;
    /** The tasklet_id of periodic beacon sender, -1 if none is started. */
    int8_t beacon_tasklet_id;
    /** When synchronization is lost, this counter is used to detect the situation and stop network*/
    uint8_t beacons_received_timer;
    /** Holds the current synchronization parent MAC address*/
    uint8_t synch_parent[8];
    // Synch info needs to be sent on next broadcast channel
    bool send_synch_info_on_next_broadcast_channel;
    // Used for randomizing broadcast sending. Device is not allowed to start broadcasting before the given superframe.
    uint8_t broadcast_start_superframe;
    /*Indexes in this table will be used to extend the repeated channel sequence*/
    uint8_t fhss_scramble_table[MAX_SCRAMBLE_TABLE_INDEXES];
    /** Used to monitor and fix synchronization drift*/
    fhss_synch_monitor_s synch_monitor;
    /** Used to drop multiple synch info messages on same broadcast channel*/
    bool beacon_received_on_this_bc_channel;
    /** Timer used for events other than synchronization.*/
    int8_t fhss_event_timer;
    /* Counts the number of sent synch Beacons (Beacon on broadcast channel). Used to limit Beacon traffic when several nodes are scanning network*/
    uint8_t synch_infos_sent_counter;
    uint32_t synch_interval;
    /* This timer/timeout is used to trigger periodically Beacon requests. Resolution is superframe length. Note that FHSS must be synchronized to use this*/
    uint32_t fhss_timeout;
    uint32_t fhss_timer;
    uint16_t synch_panid;
    /* Bit mask for FHSS events pushed to event queue. Prevents pushing same event to queue multiple times*/
    uint8_t active_fhss_events;

    struct fhss_callback callbacks;
    fhss_beacon_info_t *fhss_beacon_info_store;
    fhss_failed_tx_list_t fhss_failed_tx_list;
    fhss_statistics_t *fhss_stats_ptr;
} fhss_structure_t;

int8_t fhss_enable(fhss_api_t *fhss_api, const fhss_configuration_t *fhss_configuration, const fhss_timer_t *fhss_timer, fhss_statistics_t *fhss_statistics);
int8_t fhss_disable(fhss_structure_t *fhss_structure);
int8_t fhss_set_datarate(fhss_structure_t *fhss_structure, uint32_t datarate);
bool fhss_is_synch_root(fhss_structure_t *fhss_structure);
int8_t fhss_set_synch_configuration(fhss_structure_t *fhss_structure, const fhss_synch_configuration_t *fhss_synch_configuration);
bool fhss_check_bad_channel(fhss_structure_t *fhss_structure, uint8_t handle);
bool fhss_check_channel_type(fhss_structure_t *fhss_structure, bool is_bc, int frame_type);
bool fhss_check_tx_allowed(fhss_structure_t *fhss_structure, bool is_bc, uint16_t frame_length, int frame_type, uint8_t phy_header_length, uint8_t phy_tail_length);
fhss_structure_t *fhss_get_object_with_api(const fhss_api_t *fhss_api);
fhss_beacon_info_t *fhss_get_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id);
int fhss_flush_beacon_info_storage(fhss_structure_t *fhss_structure);
int fhss_add_beacon_info(fhss_structure_t *fhss_structure, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info);
void fhss_update_beacon_info_lifetimes(fhss_structure_t *fhss_structure, uint32_t timestamp);
void fhss_trig_event(fhss_structure_t *fhss_structure, uint8_t event_type);
int fhss_timeout_start(fhss_structure_t *fhss_structure, uint32_t time);
int fhss_timeout_stop(fhss_structure_t *fhss_structure);
int fhss_compare_with_synch_parent_address(fhss_structure_t *fhss_structure, const uint8_t *source_addr);
uint32_t fhss_get_tx_time(fhss_structure_t *fhss_structure, uint16_t bytes_to_send, uint8_t phy_header_length, uint8_t phy_tail_length);
int fhss_get_parent_address(fhss_structure_t *fhss_structure, uint8_t *p_addr);
int fhss_down(fhss_structure_t *fhss_structure);
int fhss_update_synch_parent_address(fhss_structure_t *fhss_structure);
void fhss_superframe_handler(const fhss_api_t *fhss_api, uint16_t delay);
fhss_failed_tx_t *fhss_failed_handle_find(fhss_structure_t *fhss_structure, uint8_t handle);
int fhss_failed_handle_add(fhss_structure_t *fhss_structure, uint8_t handle);
int fhss_failed_handle_remove(fhss_structure_t *fhss_structure, uint8_t handle);
void fhss_set_active_event(fhss_structure_t *fhss_structure, uint8_t event_type);
void fhss_clear_active_event(fhss_structure_t *fhss_structure, uint8_t event_type);
bool fhss_read_active_event(fhss_structure_t *fhss_structure, uint8_t event_type);
#define MAX_FHSS_TIMER_DIVIDER  100
#define SYNCH_MONITOR_AVG_SAMPLES   5

// TX/RX slot management interface
/**
 * Update slots
 *
 * Every superframe handler interrupt calls this function to update the TX/RX slot status.
 *
 * @param fhss_structure Pointer to FHSS structure
 * @return 0 for success, -1 for fail
 */
int fhss_update_txrx_slots(fhss_structure_t *fhss_structure);

void fhss_start_timer(fhss_structure_t *fhss_structure, uint32_t time, void (*callback)(const fhss_api_t *fhss_api, uint16_t));


/**
 * Calculate time in microseconds to start of next superframe.
 *
 * @param fhss_struct FHSS state
 * @return microseconds left to start of next superframe
 */
uint32_t fhss_get_remaining_time_to_next_superframe(const fhss_structure_t *fhss_struct);

#endif /* FHSS_H_ */
