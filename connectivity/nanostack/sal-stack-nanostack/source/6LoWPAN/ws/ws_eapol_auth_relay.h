/*
 * Copyright (c) 2018-2019, Pelion and affiliates.
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

#ifndef WS_EAPOL_AUTH_RELAY_H_
#define WS_EAPOL_AUTH_RELAY_H_

#ifdef HAVE_PAE_AUTH

/*
 * EAPOL authenticator relay acts as a proxy between EAPOL UDP relay and
 * authenticator PAE (KMP service). Relay is bound by default to EAPOL UDP
 * relay port 10253 (set by local port parameter) and transfers messages
 * to/from authenticator PAE. As default PAE is bound to UDP port 10254
 * (set by remote address and port parameters).
 *
 */

/**
 * ws_eapol_auth_relay_start start authenticator relay
 *
 * \param interface_ptr interface
 * \param local_port local port
 * \param remote_addr remote address
 * \param remote_port remote port
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_auth_relay_start(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port);

/**
 *  ws_eapol_auth_relay_delete delete authenticator relay
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_auth_relay_delete(protocol_interface_info_entry_t *interface_ptr);

#else

#define ws_eapol_auth_relay_start(interface_ptr, local_port, remote_addr, remote_port)
#define ws_eapol_auth_relay_delete(interface_ptr)

#endif

#endif /* WS_EAPOL_AUTH_RELAY_H_ */
