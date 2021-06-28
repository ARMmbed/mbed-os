/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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
 * \file mac_indirect_data.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MAC_INDIRECT_DATA_H_
#define MAC_INDIRECT_DATA_H_

struct protocol_interface_rf_mac_setup;
struct mac_pre_build_frame;
struct mac_pre_parsed_frame_s;

#define MAC_INDIRECT_TICK_IN_MS 100

void mac_indirect_data_ttl_handle(struct protocol_interface_rf_mac_setup *cur, uint16_t tick_value);
uint8_t mac_indirect_data_req_handle(struct mac_pre_parsed_frame_s *buf, struct protocol_interface_rf_mac_setup *mac_ptr);
void mac_indirect_queue_write(struct protocol_interface_rf_mac_setup *rf_mac_setup, struct mac_pre_build_frame *buffer);

#endif /* MAC_INDIRECT_DATA_H_ */
