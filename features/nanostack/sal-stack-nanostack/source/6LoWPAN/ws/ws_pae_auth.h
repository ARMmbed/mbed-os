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

#ifndef WS_PAE_AUTH_H_
#define WS_PAE_AUTH_H_

#ifdef HAVE_PAE_AUTH

/*
 * Authenticator port access entity controls key security protocols using KMP API.
 *
 * Configures KMP service network access and provides timing and callback services
 * for it. Registers needed security protocols to KMP service.
 *
 * PAE Maintains security keys that are internal to port access entity for
 * each supplicant and maintains supplicant security registration state.
 *
 * Autenticator PAE controls network access keys and provides new network
 * access keys to supplicants when they are changed. When supplicant
 * network keys are updated, also other keys (master key, pairwise key)
 * are updated as needed.
 *
 */

/**
 * ws_pae_auth_init initializes PAE authenticator
 *
 * \param interface_ptr interface
 * \param local_port local port
 * \param remote_addr remote address
 * \param remote_port remote port
 * \param gtks group keys
 * \param cert_chain certificate chain
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_init(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs);

/**
 * ws_pae_auth_addresses_set set relay addresses
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
int8_t ws_pae_auth_addresses_set(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port);

/**
 * ws_pae_auth_delete deletes PAE authenticator
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_delete(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_auth_timer PAE authenticator timer call
 *
 * \param ticks elapsed ticks
 *
 */
void ws_pae_auth_timer(uint16_t ticks);

#else

#define ws_pae_auth_init(interface_ptr, gtks, certs) 1
#define ws_pae_auth_addresses_set(interface_ptr, local_port, remote_addr, remote_port) 1
#define ws_pae_auth_delete NULL
#define ws_pae_auth_timer NULL

#endif

#endif /* WS_PAE_AUTH_H_ */
