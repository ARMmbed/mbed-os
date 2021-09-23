/*
 * Copyright (c) 2016-2020, Pelion and affiliates.
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

#ifndef SEC_PROT_LIB_H_
#define SEC_PROT_LIB_H_

/*
 * Library functions used by security protocols. These include helper functions
 * related to different hash functions, common message handling functions, and
 * common timer and state machine functions.
 *
 */

#define EUI64_LEN                     8
#define SEC_TOTAL_TIMEOUT             30 * 60 * 10 // 30 minutes
#define SEC_FINISHED_TIMEOUT          5 * 10       // 5 seconds

#define FWH_NONCE_LENGTH              32
#define EUI64_LEN                     8
#define SEC_TOTAL_TIMEOUT             30 * 60 * 10 // 30 minutes
#define SEC_INIT_TIMEOUT              60 * 10      // 60 seconds
#define SEC_FINISHED_TIMEOUT          5 * 10       // 5 seconds


// Common data shared between security protocols needing general timers and state machines
typedef struct {
    trickle_t                     trickle_timer;    /**< Trickle timer for re-sending */
    uint16_t                      ticks;            /**< Timer ticks */
    int8_t                        state;            /**< Protocol state machine state */
    sec_prot_result_e             result;           /**< Result for ongoing negotiation */
    bool                          trickle_running;  /**< Trickle running */
} sec_prot_common_t;

/**
 * sec_prot_lib_nonce_init init nonce
 *
 * \param nonce nonce
 * \param eui64 EUI-64
 * \param time current time
 *
 */
void sec_prot_lib_nonce_init(uint8_t *nonce, uint8_t *eui64, uint64_t time);

/**
 * sec_prot_lib_nonce_generate generates nonce
 *
 * \param nonce nonce
 *
 */
void sec_prot_lib_nonce_generate(uint8_t *nonce);

/**
 * sec_prot_lib_pmkid_calc calculates PKMID from PMK
 *
 * \param pmk PMK
 * \param auth_eui64 authenticator EUI-64
 * \param supp_eui64 supplicant EUI-64
 * \param pmkid PMKID
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_pmkid_calc(const uint8_t *pmk, const uint8_t *auth_eui64, const uint8_t *supp_eui64, uint8_t *pmkid);

/**
 * sec_prot_lib_ptkid_calc calculates PTMID from PTK
 *
 * \param pmk PTK
 * \param auth_eui64 authenticator EUI-64
 * \param supp_eui64 supplicant EUI-64
 * \param pmkid PTKID
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_ptkid_calc(const uint8_t *ptk, const uint8_t *auth_eui64, const uint8_t *supp_eui64, uint8_t *ptkid);

/**
 * sec_prot_lib_ptk_calc calculates PTK from KMP, EUI-64s and nonces
 *
 * \param pmk PMK
 * \param eui64_1 first EUI-64
 * \param eui64_2 second EUI-64
 * \param nonce1 first nonce
 * \param nonce2 second nonce
 * \param ptk PTK
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_ptk_calc(const uint8_t *pmk, const uint8_t *eui64_1, const uint8_t *eui64_2, const uint8_t *nonce1, const uint8_t *nonce2, uint8_t *ptk);

/**
 * sec_prot_lib_message_build builds a message
 *
 * \param ptk PTK for MIC calculation and encryption
 * \param kde KDEs
 * \param kde_len length of the KDEs
 * \param eapol_pdu EAPOL PDU
 * \param eapol_pdu_size EAPOL PDU size
 * \param header_size lower level header size
 *
 * \return < 0 failure
 * \return >= 0 success
 */
uint8_t *sec_prot_lib_message_build(uint8_t *ptk, uint8_t *kde, uint16_t kde_len, eapol_pdu_t *eapol_pdu, uint16_t eapol_pdu_size, uint8_t header_size);

/**
 * sec_prot_lib_message_handle handles a message
 *
 * \param ptk PTK for decryption
 * \param kde_len length of the KDEs
 * \param eapol_pdu EAPOL PDU
 *
 * \return pointer to start of the KDEs
 * \return NULL failure
 */
uint8_t *sec_prot_lib_message_handle(uint8_t *ptk, uint16_t *kde_len, eapol_pdu_t *eapol_pdu);

/**
 * sec_prot_lib_gtk_read reads GTK, GTKL and lifetime KDEs
 *
 * \param kde KDEs
 * \param kde_len length of the KDEs
 * \param sec_keys security keys
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_gtk_read(uint8_t *kde, uint16_t kde_len, sec_prot_keys_t *sec_keys);

/**
 * sec_prot_lib_mic_validate validates MIC
 *
 * \param ptk PTK for MIC validation
 * \param kde_len length of the KDEs
 * \param pdu pointer to message
 * \param pdu_size message size
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_mic_validate(uint8_t *ptk, uint8_t *mic, uint8_t *pdu, uint8_t pdu_size);

/**
 *  sec_prot_lib_pmkid_generate generate PMK ID from PMK
 *
 * \param prot security protocol
 * \param pmkid PMK ID
 * \param is_auth set for authenticator
 * \param alt_remote_eui64_use use alternative remote EUI-64 if available
 * \param used_remote_eui64 remote EUI-64 used on PMKID generation
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_pmkid_generate(sec_prot_t *prot, uint8_t *pmkid, bool is_auth, bool alt_remote_eui64_use, uint8_t *used_remote_eui64);

/**
 *  sec_prot_lib_ptkid_generate generate PTK ID from PTK
 *
 * \param prot security protocol
 * \param ptkid PTK ID
 * \param is_auth set for authenticator
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_ptkid_generate(sec_prot_t *prot, uint8_t *ptkid, bool is_auth);

/**
 *  sec_prot_lib_gtkhash_generate generate GTK hash from GTK
 *
 * \param gtk GTK
 * \param gtk_hash GTK hash
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t sec_prot_lib_gtkhash_generate(uint8_t *gtk, uint8_t *gtk_hash);

/**
 * sec_prot_remote_eui_64_addr_get get remote EUI-64 (trusted EUI-64 from security keys)
 *
 * \param prot security protocol
 *
 * \return pointer to EUI-64 or NULL
 */
uint8_t *sec_prot_remote_eui_64_addr_get(sec_prot_t *prot);

/**
 * sec_prot_init initiates common data
 *
 * \param data common data
 *
 */
void sec_prot_init(sec_prot_common_t *data);

/**
 * sec_prot_init timeout handler
 *
 * \param prot protocol
 * \param data common data
 * \param trickle_params trickle parameters
 *
 */
void sec_prot_timer_timeout_handle(sec_prot_t *prot, sec_prot_common_t *data, const trickle_params_t *trickle_params, uint16_t ticks);

/**
 * sec_prot_timer_trickle_start starts trickle timer
 *
 * \param data common data
 * \param trickle_params trickle parameters
 *
 */
void sec_prot_timer_trickle_start(sec_prot_common_t *data, const trickle_params_t *trickle_params);

/**
 * sec_prot_timer_trickle_stop stops trickle timer
 *
 * \param data common data
 *
 */
void sec_prot_timer_trickle_stop(sec_prot_common_t *data);

/**
 * sec_prot_state_set sets state machine state
 *
 * \param prot protocol
 * \param data common data
 * \param state new state
 *
 */
void sec_prot_state_set(sec_prot_t *prot, sec_prot_common_t *data, uint8_t state);

/**
 * sec_prot_state_get gets state machine state
 *
 * \param data common data
 *
 * \return state
 *
 */
uint8_t sec_prot_state_get(sec_prot_common_t *data);

/**
 * sec_prot_result_set sets result for operation
 *
 * \param data common data
 * \param result result
 *
 */
void sec_prot_result_set(sec_prot_common_t *data, sec_prot_result_e result);

/**
 * sec_prot_result_get gets result for operation
 *
 * \param data common data
 *
 * \return result
 *
 */
sec_prot_result_e sec_prot_result_get(sec_prot_common_t *data);

/**
 * sec_prot_result_timeout_check checks if result is timeout
 *
 * \param data common data
 *
 * \return true result is timeout
 * \return false result is not timeout
 *
 */
bool sec_prot_result_timeout_check(sec_prot_common_t *data);

/**
 * sec_prot_result_ok_check checks if result is ok
 *
 * \param data common data
 *
 * \return true result is ok
 * \return false result is not ok
 *
 */
bool sec_prot_result_ok_check(sec_prot_common_t *data);

/**
 * sec_prot_default_timeout_set sets default timeout for protocol
 *
 * \param data common data
 *
 */
void sec_prot_default_timeout_set(sec_prot_common_t *data);

#endif /* SEC_PROT_LIB_H_ */
