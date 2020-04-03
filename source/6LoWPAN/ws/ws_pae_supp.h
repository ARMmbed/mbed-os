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

#ifndef WS_PAE_SUPP_H_
#define WS_PAE_SUPP_H_

#define PAE_SUPP_NOT_ENABLED 5

#ifdef HAVE_PAE_SUPP

/*
 * Supplicant port access entity controls key security protocols using KMP API.
 *
 * Configures KMP service network access and provides timing and callback services
 * for it. Registers needed security protocols to KMP service.
 *
 * PAE maintains security keys that are internal to port access entity. After
 * (re-)authentication provides network access keys to application.
 *
 */

/**
 * ws_pae_supp_init initializes PAE supplicant
 *
 * \param interface_ptr interface
 * \param cert_chain certificate chain
 * \param sec_timer_cfg timer configuration
 * \param sec_prot_cfg protocol configuration
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_init(protocol_interface_info_entry_t *interface_ptr, const sec_prot_certs_t *certs, sec_timer_cfg_t *sec_timer_cfg, sec_prot_cfg_t *sec_prot_cfg);

/**
 * ws_pae_supp_delete deletes PAE supplicant
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_delete(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_supp_fast_timer PAE supplicant fast timer call
 *
 * \param ticks elapsed ticks
 *
 */
void ws_pae_supp_fast_timer(uint16_t ticks);

/**
 * ws_pae_supp_slow_timer PAE supplicant slow timer call
 *
 * \param ticks elapsed seconds
 *
 */
void ws_pae_supp_slow_timer(uint16_t seconds);

/**
 * ws_pae_supp_authenticate start EAPOL authentication
 *
 * \param interface_ptr interface
 * \param dest_pan_id EAPOL target PAN ID
 * \param dest_eui_64 EAPOL target
 *
 * \return < 0 failure
 * \return 0 authentication done, continue
 * \return > 0 authentication started
 *
 */
int8_t ws_pae_supp_authenticate(protocol_interface_info_entry_t *interface_ptr, uint16_t dest_pan_id, uint8_t *dest_eui_64);

/**
 * ws_pae_supp_nw_info_set set network information
 *
 * \param interface_ptr interface
 * \param pan_id PAD ID
 * \param network_name network name
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_nw_info_set(protocol_interface_info_entry_t *interface_ptr, uint16_t pan_id, char *network_name);

/**
 * ws_pae_supp_border_router_addr_write write border router address
 *
 * \param interface_ptr interface
 * \param eui_64 pointer to EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_border_router_addr_write(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);

/**
 * ws_pae_supp_border_router_addr_read read border router address
 *
 * \param interface_ptr interface
 * \param eui_64 pointer to EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_border_router_addr_read(protocol_interface_info_entry_t *interface_ptr, uint8_t *eui_64);

/**
 * ws_pae_supp_nw_key_valid network key is valid i.e. used successfully on bootstrap
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_nw_key_valid(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_supp_gtk_hash_update GTK hash has been updated (on PAN configuration)
 *
 * \param interface_ptr interface
 * \param gtkhash GTK hash, 32 bytes
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);

/**
 * ws_pae_supp_nw_key_index_update key index been updated (on PAN configuration)
 *
 * \param interface_ptr interface
 * \param index key index
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 *  ws_pae_supp_gtks_set set supplicant GTKs
 *
 * \param interface_ptr interface
 * \param gtks GTKs
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_gtks_set(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_supp_eapol_target_remove remove EAPOL target set using authentication start
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_supp_eapol_target_remove(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_supp_nw_key_index_set network send key index set callback
 *
 * \param interface_ptr interface
 * \param index network send key index
 *
 */
typedef void ws_pae_supp_nw_key_index_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 * ws_pae_supp_auth_completed authentication completed callback
 *
 * \param interface_ptr interface
 * \param result result, either ok or failure reason
 * \param target_eui_64 EAPOL target in case of failure or NULL
 *
 */
typedef void ws_pae_supp_auth_completed(protocol_interface_info_entry_t *interface_ptr, auth_result_e result, uint8_t *target_eui_64);

/**
 * ws_pae_supp_nw_key_insert network key insert callback
 *
 * \param interface_ptr interface
 * \param gtks group keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t ws_pae_supp_nw_key_insert(protocol_interface_info_entry_t *interface_ptr, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_supp_gtk_hash_ptr_get get pointer to GTK hash storage callback
 *
 * \param interface_ptr interface
 *
 * \return pointer to GTK has storage or NULL
 *
 */
typedef uint8_t *ws_pae_supp_gtk_hash_ptr_get(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_supp_cb_register register PEA supplicant callbacks
 *
 * \param interface_ptr interface
 * \param completed authentication completed callback
 * \param nw_key_insert network key index callback
 * \param nw_key_index_set network send key index callback
 *
 */
void ws_pae_supp_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_supp_auth_completed *completed, ws_pae_supp_nw_key_insert *nw_key_insert, ws_pae_supp_nw_key_index_set *nw_key_index_set, ws_pae_supp_gtk_hash_ptr_get *gtk_hash_ptr_get);

#else

#define ws_pae_supp_init(interface_ptr, certs, sec_timer_cfg, sec_prot_cfg) 1
#define ws_pae_supp_delete NULL
#define ws_pae_supp_timing_adjust(timing) 1
#define ws_pae_supp_cb_register(interface_ptr, completed, nw_key_insert, nw_key_index_set)
#define ws_pae_supp_nw_info_set(interface_ptr, pan_id, network_name) -1
#define ws_pae_supp_nw_key_valid(interface_ptr) -1
#define ws_pae_supp_fast_timer NULL
#define ws_pae_supp_slow_timer NULL
#define ws_pae_supp_authenticate(interface_ptr, dest_pan_id, dest_eui_64) PAE_SUPP_NOT_ENABLED
#define ws_pae_supp_border_router_addr_write NULL
#define ws_pae_supp_border_router_addr_read NULL
#define ws_pae_supp_gtk_hash_update NULL
#define ws_pae_supp_nw_key_index_update NULL
#define ws_pae_supp_gtks_set(interface_ptr, gtks)
#define ws_pae_supp_eapol_target_remove(interface_ptr)

#endif

#endif /* WS_PAE_SUPP_H_ */
