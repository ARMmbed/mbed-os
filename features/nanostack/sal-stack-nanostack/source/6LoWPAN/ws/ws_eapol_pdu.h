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

#ifndef WS_EAPOL_PDU_H_
#define WS_EAPOL_PDU_H_

/*
 * EAPOL PDU module transfers EAPOL PDUs to/from MPX. Several users
 * (e.g. supplicant PAE and EAPOL relay) can register to incoming
 * EAPOL PDUs. When registering, users need to define priority that
 * defines in which order incoming EAPOL PDUs are offered to them.
 *
 * Incoming EAPOL PDU user callbacks form a pair on EAPOL PDU module:
 * address check callback is called first, and if it returns match
 * then incoming EAPOL PDU callback is called.
 *
 */

/**
 *  ws_eapol_pdu_init initialize EAPOL PDU module
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_init(protocol_interface_info_entry_t *interface_ptr);

/**
 *  ws_eapol_pdu_mpx_register register EAPOL PDU module to MPX
 *
 * \param interface_ptr interface
 * \param mpx_api MPX API
 * \param mpx_user_id MPX user id
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_mpx_register(protocol_interface_info_entry_t *interface_ptr, struct mpx_api_s *mpx_api, uint16_t mpx_user_id);

/**
 *  ws_eapol_pdu_delete delete EAPOL PDU module
 *
 * \param interface_ptr interface
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_delete(protocol_interface_info_entry_t *interface_ptr);

/**
 *  ws_eapol_pdu_address_check check incoming EAPOL PDU address
 *
 * \param interface_ptr interface
 * \param eui_64 source EUI-64
 *
 * \return < 0 address does not match
 * \return >= 0 address matches, call the PDU receive callback
 *
 */
typedef int8_t ws_eapol_pdu_address_check(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64);

/**
 *  ws_eapol_pdu_receive receive EAPOL PDU
 *
 * \param interface_ptr interface
 * \param eui_64 source EUI-64
 * \param data EAPOL PDU
 * \param size PDU size
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
typedef int8_t ws_eapol_pdu_receive(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *data, uint16_t size);

typedef enum {
    EAPOL_PDU_RECV_HIGH_PRIORITY = 0,
    EAPOL_PDU_RECV_MEDIUM_PRIORITY = 100,
    EAPOL_PDU_RECV_LOW_PRIORITY = 200
} eapol_pdu_recv_prior_t;

typedef struct {
    eapol_pdu_recv_prior_t priority;           /**< Priority: high, medium or low */
    ws_eapol_pdu_address_check *addr_check;    /**< Address check callback */
    ws_eapol_pdu_receive *receive;             /**< PDU receive callback */
} eapol_pdu_recv_cb_data_t;

/**
 *  ws_eapol_pdu_cb_register register an incoming EAPOL PDU callback
 *
 * \param interface_ptr interface
 * \param cb_data callback data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_cb_register(protocol_interface_info_entry_t *interface_ptr, const eapol_pdu_recv_cb_data_t *cb_data);

/**
 *  ws_eapol_pdu_cb_unregister unregister an incoming EAPOL PDU callback
 *
 * \param interface_ptr interface
 * \param cb_data callback data
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_cb_unregister(protocol_interface_info_entry_t *interface_ptr, const eapol_pdu_recv_cb_data_t *cb_data);

/**
 *  ws_eapol_pdu_send_to_mpx send EAPOL PDU to MPX
 *
 * \param interface_ptr interface
 * \param eui_64 destination EUI-64
 * \param data EAPOL PDU
 * \param size PDU size
 * \param buffer pointer to allocated buffer
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_eapol_pdu_send_to_mpx(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *data, uint16_t size, void *buffer);

#endif /* WS_EAPOL_PDU_H_ */
