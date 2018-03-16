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

#ifndef FHSS_MAC_INTERFACE_H_
#define FHSS_MAC_INTERFACE_H_

bool fhss_is_broadcast_channel_cb(const fhss_api_t *api);
bool fhss_use_broadcast_queue_cb(const fhss_api_t *api, bool is_broadcast_addr, int frame_type);
int fhss_tx_handle_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t *destination_address, int frame_type, uint8_t *synch_info, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length);
bool fhss_check_tx_conditions_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t handle, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length);
void fhss_receive_frame_cb(const fhss_api_t *api, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info, int frame_type);
void fhss_data_tx_done_cb(const fhss_api_t *api, bool waiting_ack, bool tx_completed, uint8_t handle);
bool fhss_data_tx_fail_cb(const fhss_api_t *api, uint8_t handle, int frame_type);
void fhss_synch_state_set_cb(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id);
uint32_t fhss_read_timestamp_cb(const fhss_api_t *api);
uint16_t fhss_get_retry_period_cb(const fhss_api_t *api, uint8_t *destination_address, uint16_t phy_mtu);
int fhss_init_callbacks_cb(const fhss_api_t *api, fhss_callback_t *callbacks);

#endif /* FHSS_MAC_INTERFACE_H_ */
