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

#ifndef WS_BOOTSTRAP_FFN_H_
#define WS_BOOTSTRAP_FFN_H_

#ifdef HAVE_WS

int8_t ws_bootstrap_ffn_up(protocol_interface_info_entry_t *cur);
int8_t ws_bootstrap_ffn_down(protocol_interface_info_entry_t *cur);

void ws_dhcp_client_address_request(protocol_interface_info_entry_t *cur, uint8_t *prefix, uint8_t *parent_link_local);

void ws_bootstrap_ffn_rpl_configure(protocol_interface_info_entry_t *cur);

#endif

#endif /* WS_BOOTSTRAP_FFN_H_ */
