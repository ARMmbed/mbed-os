/*
 * Copyright (c) 2021, Pelion and affiliates.
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

#ifndef WS_BOOTSTRAP_LFN_H_
#define WS_BOOTSTRAP_LFN_H_

#if defined(HAVE_WS) && defined(HAVE_WS_HOST)

void ws_bootstrap_lfn_asynch_ind(struct protocol_interface_info_entry *cur, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type);
void ws_bootstrap_lfn_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message);
void ws_bootstrap_lfn_event_handler(protocol_interface_info_entry_t *cur, arm_event_s *event);
void ws_bootstrap_lfn_state_machine(protocol_interface_info_entry_t *cur);
void ws_bootstrap_lfn_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

#define wisun_mode_host(cur) (cur->bootsrap_mode == ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST)

#else

#define ws_bootstrap_ffn_asynch_ind(cur, data, ie_ext, message_type) ((void) 0)
#define ws_bootstrap_ffn_asynch_confirm(interface, asynch_message) ((void) 0)
#define ws_bootstrap_ffn_event_handler(cur, event) ((void) 0)
#define ws_bootstrap_lfn_state_machine(cur) ((void) 0)
#define ws_bootstrap_lfn_seconds_timer(cur, seconds) ((void) 0)

#define wisun_mode_host(cur) (false)

#endif //HAVE_WS

#endif /* WS_BOOTSTRAP_H_ */
