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

#ifndef WS_EAPOL_RELAY_LIB_H_
#define WS_EAPOL_RELAY_LIB_H_

int8_t ws_eapol_relay_lib_send_to_relay(const uint8_t socket_id, const uint8_t *eui_64, const ns_address_t *dest_addr, const void *data, uint16_t data_len);

#endif /* WS_EAPOL_RELAY_LIB_H_ */
