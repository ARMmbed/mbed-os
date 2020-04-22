/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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

/*
 * \file mac_mlme.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MAC_MLME_H_
#define MAC_MLME_H_

//TODO: Check if these are in correct place
#include "net_interface.h"

struct nwk_scan_params;
struct nwk_filter_params;
struct protocol_interface_rf_mac_setup;
struct arm_event_s;
struct arm_device_driver_list;
struct mlme_poll_s;
struct mlme_beacon_ind_s;
struct mlme_reset_s;
struct mlme_scan_s;
struct mlme_start_s;
struct mlme_get_conf_s;
struct mlme_set_s;
struct channel_list_s;

void mac_mlme_scan_confirmation_handle(struct protocol_interface_rf_mac_setup *rf_ptr);

/**
 * MLME Scan Request
 *
 */
void mac_mlme_scan_request(const struct mlme_scan_s *msg, struct protocol_interface_rf_mac_setup *rf_mac_setup);
/**
 * MLME Start Request
 *
 */
int8_t mac_mlme_start_req(const struct mlme_start_s *s, struct protocol_interface_rf_mac_setup *rf_mac_setup);
/**
 * MLME Reset Request
 *
 */
int8_t mac_mlme_reset(struct protocol_interface_rf_mac_setup *rf_mac_setup, const struct mlme_reset_s *reset);

int8_t mac_mlme_set_req(struct protocol_interface_rf_mac_setup *rf_mac_setup, const struct mlme_set_s *set_req);

int8_t mac_mlme_get_req(struct protocol_interface_rf_mac_setup *rf_mac_setup, struct mlme_get_conf_s *get_req);

void mac_extended_mac_set(struct protocol_interface_rf_mac_setup *rf_mac_setup, const uint8_t *mac64);

/**
 * MLME Poll Request
 *
 */
void mac_mlme_poll_req(struct protocol_interface_rf_mac_setup *cur, const struct mlme_poll_s *poll_req);

void mac_mlme_poll_process_confirm(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint8_t status);

/**
 * Radio Disable and enable functions
 */
void mac_mlme_mac_radio_disabled(struct protocol_interface_rf_mac_setup *rf_mac_setup);
void mac_mlme_mac_radio_enable(struct protocol_interface_rf_mac_setup *rf_mac_set);

/**
 * Initialize MAC channel selection sequence
 *
 * TODO: initialize channel select sequence
 *       in coordinator mode
 *
 * \param new_channel channel to set
 *
 * \return 0 success
 * \return -1 HW error
 */
int8_t mac_mlme_rf_channel_change(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint8_t new_channel);

void mac_mlme_active_scan_response_timer_start(void *interface);

/**
 * @brief mac_mlme_beacon_notify Function which is called every time a beacon is received
 * @param rf_mac_setup Current MAC setup
 * @param data Reference to beacon data
 * @return
 */
int mac_mlme_beacon_notify(struct protocol_interface_rf_mac_setup *rf_mac_setup, struct mlme_beacon_ind_s *data);

void mac_mlme_event_cb(void *mac_ptr);

void mac_mlme_set_active_state(struct protocol_interface_rf_mac_setup *entry, bool new_state);

struct protocol_interface_rf_mac_setup *mac_mlme_data_base_allocate(uint8_t *mac64, struct arm_device_driver_list *dev_driver, struct mac_description_storage_size_s *storage_sizes, uint16_t mtu_size);
void mac_mlme_data_base_deallocate(struct protocol_interface_rf_mac_setup *rf_mac);

uint8_t mac_mlme_set_new_sqn(struct protocol_interface_rf_mac_setup *rf_setup);
uint8_t mac_mlme_set_new_beacon_sqn(struct protocol_interface_rf_mac_setup *rf_setup);

uint16_t mac_mlme_get_panid(struct protocol_interface_rf_mac_setup *rf_setup);

void mac_frame_src_address_set_from_interface(uint8_t SrcAddrMode, struct protocol_interface_rf_mac_setup *rf_ptr, uint8_t *addressPtr);

int8_t mac_mlme_beacon_tx(struct protocol_interface_rf_mac_setup *rf_ptr);
uint8_t mac_mlme_beacon_req_tx(struct protocol_interface_rf_mac_setup *rf_ptr);
int8_t mac_mlme_virtual_confirmation_handle(int8_t driver_id, const uint8_t *data_ptr, uint16_t length);

uint16_t mlme_scan_analyze_next_channel(struct channel_list_s *mac_channel_list, bool clear_channel);

#endif /* MAC_MLME_H_ */
