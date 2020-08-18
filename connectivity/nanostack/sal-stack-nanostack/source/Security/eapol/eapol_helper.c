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



#include "nsconfig.h"

#include "ns_types.h"
#include "eventOS_event.h"
#include "ns_trace.h"
#include "string.h"
#include "common_functions.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"

#ifdef HAVE_WS

#define KEY_INFO_VERSION_BIT_MASK   0x0007
#define KEY_INFO_VERSION_BIT_SHIFT  0
#define KEY_INFO_KEY_TYPE_BIT_MASK  0x0008
#define KEY_INFO_KEY_TYPE_BIT_SHIFT 3
#define KEY_INFO_INSTALL_BIT_MASK   0x0040
#define KEY_INFO_INSTALL_BIT_SHIFT  6
#define KEY_INFO_ACK_BIT_MASK       0x0080
#define KEY_INFO_ACK_BIT_SHIFT      7
#define KEY_INFO_MIC_MASK           0x0100
#define KEY_INFO_MIC_SHIFT          8
#define KEY_INFO_SECURE_MASK        0x0200
#define KEY_INFO_SECURE_SHIFT       9
#define KEY_INFO_ERROR_MASK         0x0400
#define KEY_INFO_ERROR_SHIFT        10
#define KEY_INFO_REQUEST_MASK       0x0800
#define KEY_INFO_REQUEST_SHIFT      11
#define KEY_INFO_ENC_KEY_DATA_MASK  0x1000
#define KEY_INFO_ENC_KEY_DATA_SHIFT 12
#define KEY_INFO_SMK_MASK           0x2000
#define KEY_INFO_SMK_SHIFT          13

static uint8_t *eapol_key_information_write(eapol_key_information_t *key_information, uint8_t *ptr)
{
    uint16_t key_info = 0;
    key_info |= (key_information->description_version << KEY_INFO_VERSION_BIT_SHIFT);
    key_info |= (key_information->pairwise_key << KEY_INFO_KEY_TYPE_BIT_SHIFT);
    key_info |= (key_information->install << KEY_INFO_INSTALL_BIT_SHIFT);
    key_info |= (key_information->key_ack << KEY_INFO_ACK_BIT_SHIFT);
    key_info |= (key_information->key_mic << KEY_INFO_MIC_SHIFT);
    key_info |= (key_information->secured_key_frame << KEY_INFO_SECURE_SHIFT);
    key_info |= (key_information->error << KEY_INFO_ERROR_SHIFT);
    key_info |= (key_information->request << KEY_INFO_REQUEST_SHIFT);
    key_info |= (key_information->encrypted_key_data << KEY_INFO_ENC_KEY_DATA_SHIFT);
    key_info |= (key_information->smk_handshake << KEY_INFO_SMK_SHIFT);
    return common_write_16_bit(key_info, ptr);
}

static uint8_t *eapol_key_information_read(eapol_key_information_t *key_information, uint8_t *ptr)
{
    uint16_t key_info = common_read_16_bit(ptr);
    key_information->description_version = ((key_info & KEY_INFO_VERSION_BIT_MASK) >> KEY_INFO_VERSION_BIT_SHIFT);
    key_information->pairwise_key = ((key_info & KEY_INFO_KEY_TYPE_BIT_MASK) >> KEY_INFO_KEY_TYPE_BIT_SHIFT);
    key_information->install = ((key_info & KEY_INFO_INSTALL_BIT_MASK) >> KEY_INFO_INSTALL_BIT_SHIFT);
    key_information->key_ack = ((key_info & KEY_INFO_ACK_BIT_MASK) >> KEY_INFO_ACK_BIT_SHIFT);
    key_information->key_mic = ((key_info & KEY_INFO_MIC_MASK) >> KEY_INFO_MIC_SHIFT);
    key_information->secured_key_frame = ((key_info & KEY_INFO_SECURE_MASK) >> KEY_INFO_SECURE_SHIFT);
    key_information->error = ((key_info & KEY_INFO_ERROR_MASK) >> KEY_INFO_ERROR_SHIFT);
    key_information->request = ((key_info & KEY_INFO_REQUEST_MASK) >> KEY_INFO_REQUEST_SHIFT);
    key_information->encrypted_key_data = ((key_info & KEY_INFO_ENC_KEY_DATA_MASK) >> KEY_INFO_ENC_KEY_DATA_SHIFT);
    key_information->smk_handshake = ((key_info & KEY_INFO_SMK_MASK) >> KEY_INFO_SMK_SHIFT);
    return ptr + 2;
}


static bool eapol_parse_eap_packet(eapol_pdu_t *eapol_pdu)
{
    return eap_header_parse(eapol_pdu->packet_body, eapol_pdu->packet_length, &eapol_pdu->msg.eap);
}


static bool eapol_parse_key_packet(eapol_pdu_t *eapol_pdu)
{
    if (eapol_pdu->packet_length < EAPOL_KEY_FRAME_BASE_SIZE) {
        return false;
    }
    uint8_t *ptr = eapol_pdu->packet_body;
    eapol_key_frame_t *key_frame = &eapol_pdu->msg.key;
    key_frame->key_description = *ptr++;
    if (key_frame->key_description != EAPOL_RSN_KEY_DESCRIPTION) {
        return false;
    }
    ptr = eapol_key_information_read(&key_frame->key_information, ptr);
    if (key_frame->key_information.description_version != KEY_DESCRIPTION_HMAC_SHA1_MIC_AES_ENC) {
        return false;
    }
    key_frame->key_length = common_read_16_bit(ptr);
    ptr += 2;

    key_frame->replay_counter = common_read_64_bit(ptr);
    ptr += 8;

    key_frame->key_nonce = ptr;
    ptr += 32;

    key_frame->key_iv = ptr;
    ptr += 16;

    key_frame->key_rsc = ptr;
    ptr += 16; //Skip 8 byte RSC + RESERVED 8

    key_frame->key_mic = ptr;
    ptr += 16;

    key_frame->key_data_length = common_read_16_bit(ptr);
    ptr += 2;
    key_frame->key_data = ptr;
    if (key_frame->key_data_length > (eapol_pdu->packet_length - EAPOL_KEY_FRAME_BASE_SIZE)) {
        return false;
    }

    return true;

}

void eapol_write_key_packet_mic(uint8_t *eapol_pdu, uint8_t *mic)
{
    if (mic) {
        memcpy(&eapol_pdu[81], mic, 16);
    } else {
        memset(&eapol_pdu[81], 0, 16);
    }
}

bool eapol_parse_pdu_header(uint8_t *ptr, uint16_t data_length, eapol_pdu_t *eapol_pdu)
{
    //Validate MIN length
    if (data_length < EAPOL_BASE_LENGTH) {
        return false;
    }
    //Validate Protocol version
    uint8_t protocol = *ptr++;
    if (protocol != EAPOL_PROTOCOL_VERSION) {
        return false;
    }
    eapol_pdu->packet_type = *ptr++;
    eapol_pdu->packet_length = common_read_16_bit(ptr);
    ptr += 2;
    //Validate Body Length
    if (eapol_pdu->packet_length > data_length - EAPOL_BASE_LENGTH) {
        return false;
    }
    eapol_pdu->packet_body = ptr;

    if (eapol_pdu->packet_type == EAPOL_EAP_TYPE) {
        return eapol_parse_eap_packet(eapol_pdu);
    } else if (eapol_pdu->packet_type == EAPOL_KEY_TYPE) {
        return eapol_parse_key_packet(eapol_pdu);
    } else {
        return false;
    }

}

uint8_t *eapol_write_pdu_frame(uint8_t *ptr, eapol_pdu_t *eapol_pdu)
{
    *ptr++ = EAPOL_PROTOCOL_VERSION;
    *ptr++ = eapol_pdu->packet_type;
    ptr = common_write_16_bit(eapol_pdu->packet_length, ptr);
    eapol_pdu->packet_body = ptr;

    if (eapol_pdu->packet_type == EAPOL_EAP_TYPE) {
        eap_header_t *eap_header = &eapol_pdu->msg.eap;
        ptr = eap_header_build(ptr, eap_header->length, eap_header->eap_code, eap_header->id_seq, eap_header->type);
        memcpy(ptr, eap_header->data_ptr, eap_header->length - (ptr - eapol_pdu->packet_body));
        ptr += eap_header->length - (ptr - eapol_pdu->packet_body);

    } else if (eapol_pdu->packet_type == EAPOL_KEY_TYPE) {
        eapol_key_frame_t *key_frame = &eapol_pdu->msg.key;
        *ptr++ = key_frame->key_description;
        ptr = eapol_key_information_write(&key_frame->key_information, ptr);
        ptr = common_write_16_bit(key_frame->key_length, ptr);
        ptr = common_write_64_bit(key_frame->replay_counter, ptr);

        if (key_frame->key_nonce) {
            memcpy(ptr, key_frame->key_nonce, 32);
        } else {
            memset(ptr, 0, 32);
        }
        ptr += 32;

        if (key_frame->key_iv) {
            memcpy(ptr, key_frame->key_iv, 16);
        } else {
            memset(ptr, 0, 16);
        }
        ptr += 16;

        if (key_frame->key_rsc) {
            memcpy(ptr, key_frame->key_rsc, 8);
        } else {
            memset(ptr, 0, 8);
        }
        ptr += 8;

        //Reserved 8bytes
        memset(ptr, 0, 8);
        ptr += 8;

        if (key_frame->key_mic && key_frame->key_information.key_mic) {
            memcpy(ptr, key_frame->key_mic, 16);
        } else {
            memset(ptr, 0, 16);
        }
        ptr += 16;
        ptr = common_write_16_bit(key_frame->key_data_length, ptr);
        if (key_frame->key_data_length && key_frame->key_data) {
            memcpy(ptr, key_frame->key_data, key_frame->key_data_length);
            ptr += key_frame->key_data_length;
        }
    }

    return ptr;
}



uint16_t eapol_pdu_eap_frame_init(eapol_pdu_t *eapol_pdu, uint8_t eap_code, uint8_t id_seq, uint8_t type, uint16_t data_length, uint8_t *data_ptr)
{
    memset(eapol_pdu, 0, sizeof(eapol_pdu_t));

    eapol_pdu->packet_type = EAPOL_EAP_TYPE;
    eapol_pdu->packet_length = data_length;
    eapol_pdu->msg.eap.eap_code = eap_code;
    eapol_pdu->msg.eap.data_ptr = data_ptr;
    eapol_pdu->msg.eap.length = data_length;
    eapol_pdu->msg.eap.id_seq = id_seq;
    eapol_pdu->msg.eap.type = type;

    if (eap_code == EAP_REQ || eap_code == EAP_RESPONSE) {
        eapol_pdu->packet_body += 5;
        eapol_pdu->msg.eap.length++; // Add space for type
        eapol_pdu->packet_length++;
    } else {
        eapol_pdu->packet_body += 4;
    }

    return eapol_pdu_total_length(eapol_pdu);

}

uint16_t eapol_pdu_key_frame_init(eapol_pdu_t *eapol_pdu, uint16_t data_length, uint8_t *data_ptr)
{
    memset(eapol_pdu, 0, sizeof(eapol_pdu_t));

    eapol_pdu->packet_type = EAPOL_KEY_TYPE;
    eapol_pdu->packet_length = data_length + EAPOL_KEY_FRAME_BASE_SIZE;
    eapol_pdu->msg.key.key_data = data_ptr;
    eapol_pdu->msg.key.key_description = EAPOL_RSN_KEY_DESCRIPTION;
    eapol_pdu->msg.key.key_data_length = data_length;
    eapol_pdu->msg.key.key_information.description_version = KEY_DESCRIPTION_HMAC_SHA1_MIC_AES_ENC;

    return eapol_pdu_total_length(eapol_pdu);

}

uint8_t eapol_pdu_key_mask_get(eapol_pdu_t *eapol_pdu)
{
    uint8_t key_mask = 0;

    if (eapol_pdu->msg.key.key_information.install) {
        key_mask |= KEY_INFO_INSTALL;
    }
    if (eapol_pdu->msg.key.key_information.key_ack) {
        key_mask |= KEY_INFO_KEY_ACK;
    }
    if (eapol_pdu->msg.key.key_information.key_mic) {
        key_mask |= KEY_INFO_KEY_MIC;
    }
    if (eapol_pdu->msg.key.key_information.secured_key_frame) {
        key_mask |= KEY_INFO_SECURED_KEY_FRAME;
    }

    return key_mask;
}

#endif

