/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "string.h"
#include "common_functions.h"
#include "Security/PANA/pana_eap_header.h"
#ifdef PANA

bool eap_header_parse(uint8_t *data_ptr, uint16_t length, eap_header_t *header)
{
    if (length < 4) {
        return false;
    }

    header->eap_code = *data_ptr++;
    header->id_seq = *data_ptr++;
    header->length = common_read_16_bit(data_ptr);
    header->type = 0;
    data_ptr += 2;
    if (header->length < length || header->length > length) {
        return false;
    }


    switch (header->eap_code) {
        case EAP_REQ:
        case EAP_RESPONSE:
            if (header->length < 5) {
                return false;
            }
            header->type = *data_ptr++;
            break;

        case EAP_SUCCESS:
        case EAP_FAILURE:
            if (header->length != 4) {
                return false;
            }
            break;

        default:
            return false;
    }
    header->data_ptr = data_ptr;
    return true;

}

uint8_t eap_header_size(uint8_t eap_code)
{
    if (eap_code == EAP_REQ || eap_code ==EAP_RESPONSE)
    {
        return 5;
    }
    return 4;
}

uint8_t *eap_header_build(uint8_t *ptr, uint16_t data_length, uint8_t eap_code, uint8_t id_seq, uint8_t type)
{
    *ptr++ = eap_code;
    *ptr++ = id_seq;
    ptr = common_write_16_bit(data_length, ptr);
    if (eap_code == EAP_REQ || eap_code ==EAP_RESPONSE) {
        *ptr++ = type;
    }
    return ptr;

}

bool eap_tls_header_parse(uint8_t *eap_data_ptr, uint16_t eap_datalength, eap_tls_header_t *header)
{
    header->tls_length = 0;
    if (eap_datalength ==  0) {
        return false;
    }
    header->eap_tls_flags = *eap_data_ptr++;
    eap_datalength--;

    if (header->eap_tls_flags & EAP_TLS_FRAGMENT_LENGTH) {
        if (eap_datalength < 4) {
            return false;
        }
        uint32_t temp32 = common_read_32_bit(eap_data_ptr);
        eap_datalength -= 4;
        if (temp32 > 0x0000ffff) {
            return false;
        }
        eap_data_ptr += 4;
        header->tls_length = (uint16_t)temp32;
    }
    header->tls_frame_length = eap_datalength;
    header->data_ptr = eap_data_ptr;
    return true;
}

uint8_t eap_tls_header_size(uint16_t eap_tls_flags)
{
    if (eap_tls_flags & EAP_TLS_FRAGMENT_LENGTH) {
        return 5;
    }
    return 1;
}

uint8_t *eap_tls_header_build(uint8_t *ptr, uint16_t eap_tls_flags, uint16_t frame_length)
{
    *ptr++ = eap_tls_flags;
    //Test EAP Length Field
    if (eap_tls_flags & EAP_TLS_FRAGMENT_LENGTH) {
        ptr = common_write_32_bit(frame_length, ptr);
    }


    return ptr;
}
#endif
