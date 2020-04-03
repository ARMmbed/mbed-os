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

#ifndef WS_BBR_API_PRIVATE_H_
#define WS_BBR_API_PRIVATE_H_


#ifdef HAVE_WS_BORDER_ROUTER

extern uint16_t test_pan_size_override;

void ws_bbr_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_bbr_pan_version_increase(protocol_interface_info_entry_t *cur);

uint16_t ws_bbr_pan_size(protocol_interface_info_entry_t *cur);

void ws_bbr_rpl_config(protocol_interface_info_entry_t *cur, uint8_t imin, uint8_t doubling, uint8_t redundancy, uint16_t dag_max_rank_increase, uint16_t min_hop_rank_increase);

bool ws_bbr_ready_to_start(protocol_interface_info_entry_t *cur);


#else

#define ws_bbr_seconds_timer( cur, seconds)
#define ws_bbr_pan_version_increase(cur)
#define ws_bbr_pan_size(cur) 0
#define ws_bbr_rpl_config( cur, imin, doubling, redundancy, dag_max_rank_increase, min_hop_rank_increase)
#define ws_bbr_ready_to_start(cur) true

#endif //HAVE_WS_BORDER_ROUTER

#endif /* WS_BBR_API_PRIVATE_H_ */
