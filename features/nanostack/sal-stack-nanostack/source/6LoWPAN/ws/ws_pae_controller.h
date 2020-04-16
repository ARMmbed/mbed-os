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

typedef enum {
    AUTH_RESULT_OK = 0,                    // Successful
    AUTH_RESULT_ERR_NO_MEM = -1,           // No memory
    AUTH_RESULT_ERR_TX_NO_ACK = -2,        // No acknowledge was received
    AUTH_RESULT_ERR_UNSPEC = -3            // Other reason
} auth_result_e;

struct nvm_tlv_entry;
struct ws_sec_timer_cfg_s;
struct ws_sec_prot_cfg_s;

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
 * ws_pae_controller_bootstrap_done indicates to PAE controller that bootstrap is ready
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_bootstrap_done(protocol_interface_info_entry_t *interface_ptr);

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
 * ws_pae_controller_config_set sets PAE controller configuration
 *
 * \param interface_ptr interface
 * \param sec_timer_cfg timer configuration or NULL if not set
 * \param sec_prot_cfg protocol configuration or NULL if not set
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_configure(protocol_interface_info_entry_t *interface_ptr, struct ws_sec_timer_cfg_s *sec_timer_cfg, struct ws_sec_prot_cfg_s *sec_prot_cfg);

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
 * ws_pae_controller_own_certificate_add add own certificate to certificate chain
 *
 * \param cert own certificate
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_own_certificate_add(const arm_certificate_entry_s *cert);

/**
 * ws_pae_controller_own_certificates_remove removes own certificates
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_own_certificates_remove(void);

/**
 * ws_pae_controller_trusted_certificate_add add trusted certificate
 *
 * \param cert trusted certificate
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_trusted_certificate_add(const arm_certificate_entry_s *cert);

/**
 * ws_pae_controller_trusted_certificate_remove remove trusted certificate
 *
 * \param cert trusted certificate
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_trusted_certificate_remove(const arm_certificate_entry_s *cert);

/**
 * ws_pae_controller_trusted_certificates_remove removes trusted certificates
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_trusted_certificates_remove(void);

/**
 * ws_pae_controller_certificate_revocation_list_add add certification revocation list
 *
 * \param crl certification revocation list
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_certificate_revocation_list_add(const arm_cert_revocation_list_entry_s *crl);

/**
 * ws_pae_controller_certificate_revocation_list_remove remove certification revocation list
 *
 * \param crl certification revocation list
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_certificate_revocation_list_remove(const arm_cert_revocation_list_entry_s *crl);

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
 * ws_pae_controller_gtk_update update GTKs (test interface)
 *
 * \param interface_id interface identifier
 * \param gtk GTK array, if GTK is not set, pointer for the index shall be NULL.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_gtk_update(int8_t interface_id, uint8_t *gtk[4]);

/**
 * ws_pae_controller_next_gtk_update update next GTKs used during GTK lifecycle (test interface)
 *
 * \param interface_id interface identifier
 * \param gtk GTK array, if GTK is not set, pointer for the index shall be NULL.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_next_gtk_update(int8_t interface_id, uint8_t *gtk[4]);

/**
 * ws_pae_controller_key_lifetime_update update key lifetime
 *
 * \param interface_id interface identifier
 * \param gtk_lifetime GTK lifetime
 * \param pmk_lifetime PMK lifetime
 * \param ptk_lifetime PTK lifetime
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_key_lifetime_update(int8_t interface_id, uint32_t gtk_lifetime, uint32_t pmk_lifetime, uint32_t ptk_lifetime);

/**
 * ws_pae_controller_gtk_time_settings_update update GTK time settings
 *
 * \param interface_id interface identifier
 * \param revocat_lifetime_reduct revocation lifetime reduction
 * \param new_activation_time new activation time
 * \param new_install_req new install required
 * \param max_mismatch max mismatch time
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_gtk_time_settings_update(int8_t interface_id, uint8_t revocat_lifetime_reduct, uint8_t new_activation_time, uint8_t new_install_req, uint32_t max_mismatch);

/**
 * ws_pae_controller_node_keys_remove remove node's keys
 *
 * \param interface_id interface identifier
 * \param eui-64 EUI-64
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_node_keys_remove(int8_t interface_id, uint8_t *eui_64);

/**
 * ws_pae_controller_node_access_revoke_start start node's access revoke
 *
 * \param interface_id interface identifier
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_node_access_revoke_start(int8_t interface_id);

/**
 * ws_pae_controller_node_limit_set set node limit
 *
 * \param interface_id interface identifier
 * \param limit limit for nodes
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_node_limit_set(int8_t interface_id, uint16_t limit);

/**
 * ws_pae_controller_ext_certificate_validation_set enable or disable extended certificate validation
 *
 * \param interface_ptr interface
 * \param enabled       true to enable extended validation, false to disable
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_ext_certificate_validation_set(int8_t interface_id, bool enabled);

/**
 * ws_pae_controller_active_key_update update active key (test interface)
 *
 * \param interface_id interface identifier
 * \param index GTK index
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_active_key_update(int8_t interface_id, uint8_t index);

/**
 * ws_pae_controller_gtk_hash_ptr_get get pointer to GTK hash storage
 *
 * \param interface_ptr interface
 *
 * \return pointer to GTK has storage or NULL
 *
 */
uint8_t *ws_pae_controller_gtk_hash_ptr_get(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_gtk_hash_update GTK hash has been updated (on PAN configuration)
 *
 * \param interface_ptr interface
 * \param gtkhash new GTK hash
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_gtk_hash_update(protocol_interface_info_entry_t *interface_ptr, uint8_t *gtkhash);

/**
 * ws_pae_controller_nw_key_index_update key index been updated (on PAN configuration)
 *
 * \param interface_ptr interface
 * \param index key index
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_nw_key_index_update(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 * ws_pae_controller_nw_keys_remove remove network keys
 *
 * \param interface_ptr interface
 *
 */
void ws_pae_controller_nw_keys_remove(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_nw_key_insert network key insert callback
 *
 * \param interface_ptr interface
 * \param slot key slot (MAC key descriptor), from 0 to 4
 * \param index index of the new network key
 * \param key new key
 *
 */
typedef void ws_pae_controller_nw_key_set(protocol_interface_info_entry_t *interface_ptr, uint8_t slot, uint8_t index, uint8_t *key);

/**
 * ws_pae_controller_nw_key_clear network key clear callback
 *
 * \param interface_ptr interface
 * \param slot key slot (MAC key descriptor), from 0 to 4
 *
 */
typedef void ws_pae_controller_nw_key_clear(protocol_interface_info_entry_t *interface_ptr, uint8_t slot);

/**
 * ws_pae_controller_nw_send_key_index_set network send key index set callback
 *
 * \param interface_ptr interface
 * \param index index of the key to be used on sending
 *
 */
typedef void ws_pae_controller_nw_send_key_index_set(protocol_interface_info_entry_t *interface_ptr, uint8_t index);

/**
 * ws_pae_controller_nw_frame_counter_set network frame counter set callback
 *
 * \param interface_ptr interface
 * \param counter frame counter
 * \param slot key slot (MAC key descriptor), from 0 to 4
 *
 */
typedef void ws_pae_controller_nw_frame_counter_set(protocol_interface_info_entry_t *interface_ptr, uint32_t counter, uint8_t slot);

/**
 * ws_pae_controller_nw_frame_counter_read network frame counter read callback
 *
 * \param interface_ptr interface
 * \param counter frame counter
 *
 */
typedef void ws_pae_controller_nw_frame_counter_read(protocol_interface_info_entry_t *interface_ptr, uint32_t *counter, uint8_t slot);

/**
 * ws_pae_controller_auth_completed authentication completed callback
 *
 * \param interface_ptr interface
 * \param result result, either ok or failure reason
 * \param target_eui_64 EAPOL target in case of failure or NULL
 *
 */
typedef void ws_pae_controller_auth_completed(protocol_interface_info_entry_t *interface_ptr, auth_result_e result, uint8_t *target_eui_64);

/**
 * ws_pae_controller_pan_ver_increment PAN version increment callback
 *
 * \param interface_ptr interface
 *
 */
typedef void ws_pae_controller_pan_ver_increment(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_cb_register register PEA controller callbacks
 *
 * \param interface_ptr interface
 * \param completed authentication completed callback
 * \param nw_key_set network key set callback
 * \param nw_key_clear network key clear callback
 * \param nw_send_key_index_set network send key index set callback
 * \param nw_frame_counter_set network frame counter set callback
 * \param nw_frame_counter_read network frame counter read callback
 * \param pan_ver_increment PAN version increment callback
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_controller_cb_register(protocol_interface_info_entry_t *interface_ptr, ws_pae_controller_auth_completed *completed, ws_pae_controller_nw_key_set *nw_key_set, ws_pae_controller_nw_key_clear *nw_key_clear, ws_pae_controller_nw_send_key_index_set *nw_send_key_index_set, ws_pae_controller_nw_frame_counter_set *nw_frame_counter_set, ws_pae_controller_nw_frame_counter_read *nw_frame_counter_read, ws_pae_controller_pan_ver_increment *pan_ver_increment);

/**
 * ws_pae_controller_fast_timer PAE controller fast timer call
 *
 * \param ticks elapsed ticks
 *
 */
void ws_pae_controller_fast_timer(uint16_t ticks);

/**
 * ws_pae_controller_slow_timer PAE controller slow timer call
 *
 * \param seconds elapsed seconds
 *
 */
void ws_pae_controller_slow_timer(uint16_t seconds);

struct nvm_tlv_entry *ws_pae_controller_nvm_tlv_get(protocol_interface_info_entry_t *interface_ptr);

/**
 * ws_pae_controller_forced_gc PAE controller garbage cleanup callback
 *
 * \param full_gc Full cleanup (true for critical garbage cleanup)
 *
 */
void ws_pae_controller_forced_gc(bool full_gc);

#else

#define ws_pae_controller_set_target(interface_ptr, target_pan_id, target_dest_eui_64)
#define ws_pae_controller_authenticate(interface_ptr)

#define ws_pae_controller_authenticator_start(interface_ptr, local_port, remote_address, remote_port)

#define ws_pae_controller_border_router_addr_write(interface_ptr, eui_64) -1
#define ws_pae_controller_border_router_addr_read(interface_ptr, eui_64) -1

#define ws_pae_controller_gtk_set(interface_id, gtk) -1
#define ws_pae_controller_next_gtks_update(interface_id, gtk) -1

#define ws_pae_controller_init(interface_ptr) 1
#define ws_pae_controller_supp_init(interface_ptr) 1
#define ws_pae_controller_auth_init(interface_ptr) 1

#define ws_pae_controller_stop(interface_ptr)
#define ws_pae_controller_delete(interface_ptr)
#define ws_pae_controller_cb_register(interface_ptr, completed, nw_key_set, nw_key_clear, nw_send_key_index_set, pan_ver_increment) 1
#define ws_pae_controller_nvm_tlv_get(interface_ptr) NULL

#define ws_pae_controller_forced_gc NULL

#endif

#endif /* WS_PAE_CONTROLLER_H_ */
