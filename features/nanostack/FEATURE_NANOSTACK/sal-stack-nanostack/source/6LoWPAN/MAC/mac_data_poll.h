/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 * \file mac_data_poll.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef MAC_DATA_POLL_H_
#define MAC_DATA_POLL_H_

#include "net_polling_api.h"

struct nwk_rfd_poll_setups;
struct protocol_interface_info_entry;
struct mlme_poll_conf_s;

void mac_poll_timer_trig(uint32_t poll_time, struct protocol_interface_info_entry *cur);
void mac_mlme_poll_confirm(struct protocol_interface_info_entry *cur, const struct mlme_poll_conf_s *confirm);
void mac_data_poll_init_protocol_poll(struct protocol_interface_info_entry *cur);
uint32_t mac_data_poll_host_poll_time_max(struct protocol_interface_info_entry *cur);
uint32_t mac_data_poll_host_timeout(struct protocol_interface_info_entry *cur);
void mac_data_poll_protocol_poll_mode_decrement(struct protocol_interface_info_entry *cur);
void mac_data_poll_protocol_poll_mode_disable(struct protocol_interface_info_entry *cur);
void mac_data_poll_enable_check(struct protocol_interface_info_entry *cur);
void mac_data_poll_disable(struct protocol_interface_info_entry *cur);

int8_t mac_data_poll_host_mode_get(struct protocol_interface_info_entry *cur, net_host_mode_t *mode);
int8_t mac_data_poll_host_mode_set(struct protocol_interface_info_entry *cur, net_host_mode_t mode, uint32_t poll_time);

void mac_data_poll_init(struct protocol_interface_info_entry *cur);
uint32_t mac_data_poll_get_max_sleep_period(struct protocol_interface_info_entry *cur);

#endif /* MAC_DATA_POLL_H_ */
