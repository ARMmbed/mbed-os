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
 * \param next_gtks next group keys to be used
 * \param cert_chain certificate chain
 * \param timer_settings timer settings
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_init(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks, sec_prot_gtk_keys_t *next_gtks, const sec_prot_certs_t *certs, timer_settings_t *timer_settings);

/**
 * ws_pae_auth_timing_adjust Adjust retries and timings of the security protocols
 *
 * Timing value is a generic number between 0 to 32 that goes from fast and
 * reactive network to low bandwidth and long latency.
 *
 * example value definitions:
 * 0-8 very fast network
 * 9-16 medium network
 * 16-24 slow network
 * 25-32 extremely slow network
 *
 * There is no need to have lots variations in every layer if protocol is not very active in any case.
 *
 * \param timing Timing value.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_timing_adjust(uint8_t timing);

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
 * ws_pae_auth_fast_timer PAE authenticator fast timer call
 *
 * \param ticks elapsed ticks
 *
 */
void ws_pae_auth_fast_timer(uint16_t ticks);

/**
 * ws_pae_auth_slow_timer PAE authenticator slow call
 *
 * \param seconds elapsed seconds
 *
 */
void ws_pae_auth_slow_timer(uint16_t seconds);

/**
 * ws_pae_auth_start start PAE authenticator
 *
 * \param interface_ptr interface
 *
 */
void ws_pae_auth_start(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_auth_gtks_updated indicates that GTKs has been updated
 *
 * \param interface_ptr interface
 *
 */
void ws_pae_auth_gtks_updated(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_auth_gtks_updated indicates that key index has been updated
 *
 * \param interface_ptr interface
 * \param index key index
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 * ws_pae_auth_node_keys_remove removes nodes keys
 *
 * \param interface_ptr interface
 * \param eui64 node's EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_node_keys_remove(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui64);

/**
 * ws_pae_auth_node_access_revoke_start start node's access revoke
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_node_access_revoke_start(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_auth_node_limit_set set node limit
 *
 * \param interface_ptr interface
 * \param limit limit for nodes
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_auth_node_limit_set(protocol_interface_info_entry_t *interface_ptr, uint16_t limit);

/**
 * ws_pae_auth_forced_gc garbage cleanup call
 *
 * \param interface_ptr interface
 *
 */
void ws_pae_auth_forced_gc(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_auth_gtk_hash_set GTK hash set callback
 *
 * \param interface_ptr interface
 * \param gtkhash GTK hash, 32 bytes
 *
 */
typedef void ws_pae_auth_gtk_hash_set(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);

/**
 * ws_pae_auth_nw_key_insert network key insert callback
 *
 * \param interface_ptr interface
 * \param gtks group keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t ws_pae_auth_nw_key_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_auth_nw_key_index_set network send key index set callback
 *
 * \param interface_ptr interface
 * \param index network send key index
 *
 */
typedef void ws_pae_auth_nw_key_index_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 *  ws_pae_auth_cb_register register PAE authenticator callbacks
 *
 * \param interface_ptr interface
 * \param hash_set GTK hash set callback
 * \param nw_key_insert network key index callback
 * \param nw_key_index_set network send key index callback
 *
 */
void ws_pae_auth_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_auth_gtk_hash_set *hash_set, ws_pae_auth_nw_key_insert *nw_key_insert, ws_pae_auth_nw_key_index_set *nw_key_index_set);

#else

#define ws_pae_auth_init(interface_ptr, gtks, next_gtks, certs, timer_settings) 1
#define ws_pae_auth_timing_adjust(timing) 1
#define ws_pae_auth_addresses_set(interface_ptr, local_port, remote_addr, remote_port) 1
#define ws_pae_auth_delete NULL
#define ws_pae_auth_cb_register(interface_ptr, hash_set, nw_key_insert, nw_key_index_set) {(void) hash_set;}
#define ws_pae_auth_start(interface_ptr)
#define ws_pae_auth_gtks_updated NULL
#define ws_pae_auth_nw_key_index_update NULL
#define ws_pae_auth_node_keys_remove(interface_ptr, eui64) -1
#define ws_pae_auth_node_access_revoke_start(interface_ptr)
#define ws_pae_auth_node_limit_set(interface_ptr, limit)
#define ws_pae_auth_forced_gc(interface_ptr)
#define ws_pae_auth_fast_timer NULL
#define ws_pae_auth_slow_timer NULL

#endif

#endif /* WS_PAE_AUTH_H_ */
