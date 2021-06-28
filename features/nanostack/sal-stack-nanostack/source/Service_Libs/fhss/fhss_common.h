/*
 * Copyright (c) 2015-2021, Pelion and affiliates.
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
#ifndef FHSS_COMMON_H_
#define FHSS_COMMON_H_
#include "ns_list.h"

#define FHSS_TASKLET_INIT_EVENT             0
#define FHSS_TIMER_EVENT                    1
#define FHSS_COMPARE_SYNCH_PARENT           2
#define FHSS_BROADCAST_CHANNEL              3
#define FHSS_UPDATE_SYNCH_INFO_STORAGE      4

struct fhss_failed_tx {
    uint8_t handle;
    uint8_t bad_channel;
    uint8_t retries_done;
    ns_list_link_t link;
};
typedef NS_LIST_HEAD(fhss_failed_tx_t, link) fhss_failed_tx_list_t;

struct fhss_structure {
    uint8_t own_hop;
    uint8_t fhss_resolution_divider;
    uint8_t rx_channel;
    int8_t beacon_tasklet_id;
    int8_t fhss_event_timer;
    uint8_t active_fhss_events;
    uint16_t number_of_channels;
    uint16_t number_of_uc_channels;
    uint16_t number_of_bc_channels;
    uint16_t optimal_packet_length;
    fhss_states fhss_state;
    uint32_t fhss_timeout;
    uint32_t fhss_timer;
    struct fhss_api *fhss_api;
    struct fhss_bs *bs;
    struct fhss_ws *ws;
    struct fhss_timer platform_functions;
    struct fhss_callback callbacks;
    struct fhss_statistics *fhss_stats_ptr;
    fhss_failed_tx_list_t fhss_failed_tx_list;
    uint8_t synch_parent[8];
};

fhss_structure_t *fhss_get_object_with_timer_id(const int8_t timer_id);
fhss_structure_t *fhss_allocate_instance(fhss_api_t *fhss_api, const fhss_timer_t *fhss_timer);
int8_t fhss_free_instance(fhss_api_t *fhss_api);
int8_t fhss_set_datarate(fhss_structure_t *fhss_structure, uint32_t datarate);
fhss_structure_t *fhss_get_object_with_api(const fhss_api_t *fhss_api);
void fhss_clear_active_event(fhss_structure_t *fhss_structure, uint8_t event_type);
int8_t fhss_disable(fhss_structure_t *fhss_structure);
void fhss_start_timer(fhss_structure_t *fhss_structure, uint32_t time, void (*callback)(const fhss_api_t *fhss_api, uint16_t));
void fhss_stop_timer(fhss_structure_t *fhss_structure, void (*callback)(const fhss_api_t *fhss_api, uint16_t));
int fhss_timeout_start(fhss_structure_t *fhss_structure, uint32_t time);
int fhss_timeout_stop(fhss_structure_t *fhss_structure);
int fhss_update_synch_parent_address(fhss_structure_t *fhss_structure);
void fhss_trig_event(fhss_structure_t *fhss_structure, uint8_t event_type);
int fhss_get_parent_address(fhss_structure_t *fhss_structure, uint8_t *p_addr);
int fhss_compare_with_synch_parent_address(fhss_structure_t *fhss_structure, const uint8_t *source_addr);
uint32_t fhss_read_timestamp_cb(const fhss_api_t *api);
int fhss_init_callbacks_cb(const fhss_api_t *api, fhss_callback_t *callbacks);
int fhss_failed_handle_remove(fhss_structure_t *fhss_structure, uint8_t handle);
int fhss_failed_handle_add(fhss_structure_t *fhss_structure, uint8_t handle, uint8_t bad_channel);
fhss_failed_tx_t *fhss_failed_handle_find(fhss_structure_t *fhss_structure, uint8_t handle);
void fhss_failed_list_free(fhss_structure_t *fhss_structure);
uint32_t fhss_get_tx_time(fhss_structure_t *fhss_structure, uint16_t bytes_to_send, uint8_t phy_header_length, uint8_t phy_tail_length);
#endif /*FHSS_COMMON_H_*/
