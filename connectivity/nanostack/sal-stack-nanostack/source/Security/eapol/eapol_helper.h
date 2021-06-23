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

#ifndef EAPOL_HELPER_H_
#define EAPOL_HELPER_H_

#define EAPOL_PROTOCOL_VERSION      3
#define EAPOL_EAP_TYPE              0
#define EAPOL_KEY_TYPE              3
#define EAPOL_KEY_NONCE_LEN         32
#define EAPOL_KEY_MIC_LEN           16
#define EAPOL_KEY_LEN               16

#define EAPOL_BASE_LENGTH           4 //Protocol version 1 byte, Packet type 1 byte, packet length 2 byte

#define EAPOL_KEY_FRAME_BASE_SIZE   95

struct eap_header_t;

typedef struct eapol_key_information {
    unsigned description_version: 3;
    bool pairwise_key: 1;
    bool install: 1;
    bool key_ack: 1;
    bool key_mic: 1;
    bool secured_key_frame: 1;
    bool error: 1;
    bool request: 1;
    bool encrypted_key_data: 1;
    bool smk_handshake: 1;
} eapol_key_information_t;

typedef struct eapol_key_frame {
    uint8_t key_description;
    eapol_key_information_t key_information;
    uint16_t key_length;
    uint64_t replay_counter;
    uint8_t *key_nonce;         /*< Write operation: NULL memset are by 0, otherwise write data  */
    uint8_t *key_iv;            /*< Write operation: NULL memset are by 0, otherwise write data  */
    uint8_t *key_rsc;           /*< Write operation: NULL memset are by 0, otherwise write data  */
    uint8_t *key_mic;           /*< Write operation: NULL memset are by 0, otherwise write data  */
    uint16_t key_data_length;
    uint8_t *key_data;
} eapol_key_frame_t;

typedef struct eapol_pdu {
    uint8_t packet_type;      /*< EAPOL_EAP_TYPE or  EAPOL_KEY_TYPE */
    uint16_t packet_length;   /*< EAPOL Total length includin full packet body and data */
    uint8_t *packet_body;     /*< Data pointer to packet body*/
    union {
        eapol_key_frame_t key;
        struct eap_header eap;
    } msg;
} eapol_pdu_t;

#define EAPOL_RSN_KEY_DESCRIPTION 2
#define KEY_DESCRIPTION_HMAC_MD5_MIC_ARC4_ENC 1
#define KEY_DESCRIPTION_HMAC_SHA1_MIC_AES_ENC 2
#define KEY_DESCRIPTION_AES_128_CMAC_MIC_AES_ENC 3

/**
 *  Helper macro to get full message length
 */
#define eapol_pdu_total_length(x) (x->packet_length + EAPOL_BASE_LENGTH)

/**
 *  Helper macro to message start
 */
#define eapol_pdu_msg_start(x) (x->packet_body - EAPOL_BASE_LENGTH)

/**
 *  Parse EAPOL message to EAPOL-pdu frame
 *
 *  \return true when message is valid and supported otherwise return false
 */
bool eapol_parse_pdu_header(uint8_t *ptr, uint16_t data_length, eapol_pdu_t *eapol_pdu);

uint8_t *eapol_write_pdu_frame(uint8_t *ptr, eapol_pdu_t *eapol_pdu);

uint16_t eapol_pdu_eap_frame_init(eapol_pdu_t *eapol_pdu, uint8_t eap_code, uint8_t id_seq, uint8_t type, uint16_t data_length, uint8_t *data_ptr);

uint16_t eapol_pdu_key_frame_init(eapol_pdu_t *eapol_pdu, uint16_t data_length, uint8_t *data_ptr);

void eapol_write_key_packet_mic(uint8_t *eapol_pdu, uint8_t *mic);

#define KEY_INFO_INSTALL              0x01
#define KEY_INFO_KEY_ACK              0x02
#define KEY_INFO_KEY_MIC              0x04
#define KEY_INFO_SECURED_KEY_FRAME    0x08

/**
 * eapol_pdu_key_mask_get gets masked EAPOL-Key message bits
 *
 * \param eapol_pdu EAPOL PDU
 *
 * \return mask
 */
uint8_t eapol_pdu_key_mask_get(eapol_pdu_t *eapol_pdu);

#endif /* EAPOL_HELPER_H_ */
