/*
 * Copyright (c) 2019-2020, Pelion and affiliates.
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

#ifndef EAP_TLS_SEC_PROT_LIB_H_
#define EAP_TLS_SEC_PROT_LIB_H_

/*
 * EAP-TLS security protocol library
 *
 */

typedef enum {
    EAP_TLS_EXCHANGE_NONE = 0,
    EAP_TLS_EXCHANGE_START,
    EAP_TLS_EXCHANGE_ONGOING
} eap_tls_sec_prot_tls_exchange_e;

typedef enum {
    EAP_TLS_MSG_IDENTITY = 0,
    EAP_TLS_MSG_START,
    EAP_TLS_MSG_CONTINUE,
    EAP_TLS_MSG_SEND_DONE,
    EAP_TLS_MSG_MORE_FRAG,
    EAP_TLS_MSG_RECEIVE_DONE,
    EAP_TLS_MSG_DECODE_ERROR,
    EAP_TLS_MSG_FAIL,
} eap_tls_sec_prot_msg_e;

typedef enum {
    EAP_TLS_RESULT_NONE = 0,
    EAP_TLS_RESULT_ERROR,
    EAP_TLS_RESULT_HANDSHAKE_OVER,
    EAP_TLS_RESULT_HANDSHAKE_FAILED,
    EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR,
} eap_tls_sec_prot_result_e;

typedef struct {
    uint8_t                       *data;          /**< Data buffer */
    uint16_t                      total_len;      /**< Total length of the data buffer */
    uint16_t                      handled_len;    /**< Handled length of the data buffer (e.g. acked by other end) */
} tls_data_t;

#define TLS_HEAD_LEN              5      //EAP-TLS flags and EAP-TLS length

extern const uint8_t eap_msg_trace[4][10];

/**
 * eap_tls_sec_prot_lib_message_allocate allocate message buffer
 *
 * \param data data buffer (length of the allocated data is header length + data length)
 * \param head_len header length
 * \param len data len
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t eap_tls_sec_prot_lib_message_allocate(tls_data_t *data, uint8_t head_len, uint16_t len);

/**
 * eap_tls_sec_prot_lib_message_realloc allocates larger message buffer and copies existing data to it
 *
 * \param data data buffer which length is increased
 * \param head_len header length
 * \param new_len new length for the buffer
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t eap_tls_sec_prot_lib_message_realloc(tls_data_t *data, uint8_t head_len, uint16_t new_len);

/**
 * eap_tls_sec_prot_lib_message_free free message buffer
 *
 * \param data data buffer
 *
 */
void eap_tls_sec_prot_lib_message_free(tls_data_t *data);

/**
 * eap_tls_sec_prot_lib_message_init init message buffer
 *
 * \param data data buffer
 *
 */
void eap_tls_sec_prot_lib_message_init(tls_data_t *data);

/**
 * eap_tls_sec_prot_lib_message_handle decode incoming message EAP-TLS part (from EAP-TLS flags field onward)
 *
 * \param data message data (points to EAP-TLS flags)
 * \param length of the message remaining
 * \param new_seq_id EAP sequence identifier is new (message is not re-send by other end)
 * \param tls_send EAP-TLS send buffer, when sending data, updates the data (fragments) that has been acknowledged by other end
 * \param tls_recv EAP_TLS receive buffer, if receiving data, updates the received data (fragments)
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t eap_tls_sec_prot_lib_message_handle(uint8_t *data, uint16_t length, bool new_seq_id, tls_data_t *tls_send, tls_data_t *tls_recv);

/**
 * eap_tls_sec_prot_lib_message_build builds EAP-TLS message
 *
 * \param eap_code EAP code
 * \param eap_type EAP type
 * \param flags EAP-TLS flags
 * \param eap_id_seq EAP sequence identifier
 * \param header_size header size
 * \param tls_send EAP-TLS send buffer, sends either a fragment or full message
 * \param length of the message to be send
 *
 * \return pointer to message to be sent or NULL in case of failure
 *
 */
uint8_t *eap_tls_sec_prot_lib_message_build(uint8_t eap_code, uint8_t eap_type, uint8_t *flags, uint8_t eap_id_seq, uint8_t header_size, tls_data_t *tls_send, uint16_t *length);

#endif /* EAP_TLS_SEC_PROT_H_ */
