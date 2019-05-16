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

#ifndef WS_PAE_CONTROLLER_H_
#define WS_PAE_CONTROLLER_H_

#ifdef HAVE_WS

/**
 * ws_pae_controller_set_target sets EAPOL target for PAE supplicant
 *
 * \param interface_ptr interface
 * \param target_pan_id EAPOL target PAN ID
 * \param target_eui_64 EAPOL target
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_set_target(protocol_interface_info_entry_t *interface_ptr, uint16_t target_pan_id, uint8_t *target_eui_64);

/**
 * ws_pae_controller_authenticate start PAE supplicant authentication
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_authenticate(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_authenticator_start start PAE authenticator
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
int8_t ws_pae_controller_authenticator_start(protocol_interface_info_entry_t *interface_ptr, uint16_t local_port, const uint8_t *remote_addr, uint16_t remote_port);

/**
 * ws_pae_controller_init initializes PAE controller
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_init(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_init initializes PAE supplicant
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_supp_init(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_init initializes PAE authenticator
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_auth_init(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_stop stop PAE controller (e.g. on interface down)
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_stop(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_delete delete PAE controller (e.g. failure to create interface)
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_delete(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_certificate_chain_set set certificate chain
 *
 * \param chain certificate chain
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_certificate_chain_set(const arm_certificate_chain_entry_s *chain);

/**
 * ws_pae_controller_nw_info_set set network information
 *
 * \param interface_ptr interface
 * \param pan_id PAD ID
 * \param network_name network name
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name);

/**
 * ws_pae_controller_nw_key_valid network key is valid i.e. used successfully on bootstrap
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_nw_key_valid(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_border_router_addr_write write border router address
 *
 * \param interface_ptr interface
 * \param eui_64 pointer to EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_border_router_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);

/**
 * ws_pae_controller_border_router_addr_read read border router address
 *
 * \param interface_ptr interface
 * \param eui_64 pointer to EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_border_router_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);

/**
 * ws_pae_controller_key_insert new GTK key available callback
 *
 * \param interface_ptr interface
 * \param gtk_index index of the new GTK key
 * \param gtk new GTK key
 *
 */
typedef void ws_pae_controller_key_insert(protocol_interface_info_entry_t *interface_ptr, uint8_t gtk_index, uint8_t *gtk);

/**
 * ws_pae_controller_auth_completed authentication completed callback
 *
 * \param interface_ptr interface
 * \param success true if authentication was successful
 *
 */
typedef void ws_pae_controller_auth_completed(protocol_interface_info_entry_t *interface_ptr, bool success);

/**
 * ws_pae_controller_cb_register register PEA controller callbacks
 *
 * \param interface_ptr interface
 * \param completed authentication completed callback
 * \param key_insert GTK key insert callback
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_auth_completed *completed, ws_pae_controller_key_insert *key_insert);

/**
 * ws_pae_controller_timer PAE controller timer call
 *
 * \param ticks elapsed ticks
 *
 */
void ws_pae_controller_timer(uint16_t ticks);

#else

#define ws_pae_controller_set_target(interface_ptr, target_pan_id, target_dest_eui_64)
#define ws_pae_controller_authenticate(interface_ptr)

#define ws_pae_controller_authenticator_start(interface_ptr, local_port, remote_address, remote_port)

#define ws_pae_controller_border_router_addr_write(interface_ptr, eui_64) -1
#define ws_pae_controller_border_router_addr_read(interface_ptr, eui_64) -1

#define ws_pae_controller_init(interface_ptr) 1
#define ws_pae_controller_supp_init(interface_ptr) 1
#define ws_pae_controller_auth_init(interface_ptr) 1

#define ws_pae_controller_stop(interface_ptr)
#define ws_pae_controller_delete(interface_ptr)
#define ws_pae_controller_cb_register(interface_ptr, completed, key_insert) 1
#define ws_pae_controller_timer(ticks)

#endif

#endif /* WS_PAE_CONTROLLER_H_ */
