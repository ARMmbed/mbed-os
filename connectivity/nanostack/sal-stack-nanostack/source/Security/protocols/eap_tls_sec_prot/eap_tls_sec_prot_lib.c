/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"
#include "Security/protocols/eap_tls_sec_prot/eap_tls_sec_prot_lib.h"

#ifdef HAVE_WS

#define TRACE_GROUP "eapl"

static int8_t eap_tls_sec_prot_lib_ack_update(tls_data_t *tls);
static uint8_t *eap_tls_sec_prot_lib_fragment_write(uint8_t *data, uint16_t total_len, uint16_t handled_len, uint16_t *message_len, uint8_t *flags);
static int8_t eap_tls_sec_prot_lib_fragment_read(tls_data_t *tls, uint8_t *data, uint16_t len);

const uint8_t eap_msg_trace[4][10] = {"REQ", "RESPONSE", "SUCCESS", "FAILURE"};

int8_t eap_tls_sec_prot_lib_message_allocate(tls_data_t *data, uint8_t head_len, uint16_t len)
{
    ns_dyn_mem_free(data->data);

    data->data = ns_dyn_mem_temporary_alloc(head_len + len);
    if (!data->data) {
        return -1;
    }
    data->total_len = len;
    data->handled_len = 0;

    return 0;
}

int8_t eap_tls_sec_prot_lib_message_realloc(tls_data_t *data, uint8_t head_len, uint16_t new_len)
{
    tls_data_t new_tls_send;

    eap_tls_sec_prot_lib_message_init(&new_tls_send);
    if (eap_tls_sec_prot_lib_message_allocate(&new_tls_send, head_len, new_len) < 0) {
        return -1;
    }
    memcpy(new_tls_send.data + head_len, data->data + head_len, data->handled_len);
    new_tls_send.handled_len = data->handled_len;
    eap_tls_sec_prot_lib_message_free(data);

    *data = new_tls_send;

    return 0;
}

void eap_tls_sec_prot_lib_message_free(tls_data_t *data)
{
    ns_dyn_mem_free(data->data);
    data->handled_len = 0;
    data->data = 0;
    data->total_len = 0;
}

void eap_tls_sec_prot_lib_message_init(tls_data_t *data)
{
    data->handled_len = 0;
    data->data = 0;
    data->total_len = 0;
}

int8_t eap_tls_sec_prot_lib_message_handle(uint8_t *data, uint16_t length, bool new_seq_id, tls_data_t *tls_send, tls_data_t *tls_recv)
{
    int8_t result = EAP_TLS_MSG_CONTINUE;

    // EAP-TLS start
    if (data[0] & EAP_TLS_START) {
        result = EAP_TLS_MSG_START;
    } else if (data[0] & EAP_TLS_MORE_FRAGMENTS) {
        // More fragments
        eap_tls_sec_prot_lib_message_allocate(tls_send, TLS_HEAD_LEN, 0);

        // Handles the length field
        if (data[0] & EAP_TLS_FRAGMENT_LENGTH) {
            if (length < 5) {
                tr_error("EAP-TLS: decode error");
                return EAP_TLS_MSG_DECODE_ERROR;
            }

            uint32_t len = common_read_32_bit(&data[1]);

            //For first fragment allocates data for incoming TLS packet
            if (!tls_recv->data) {
                eap_tls_sec_prot_lib_message_allocate(tls_recv, 0, len);
            }
            length -= 4;
            data += 4;
        }
        result = EAP_TLS_MSG_MORE_FRAG;
    } else if (data[0] == 0 || data[0] == EAP_TLS_FRAGMENT_LENGTH) {
        // Skip fragment length if present
        if (data[0] & EAP_TLS_FRAGMENT_LENGTH) {
            if (length < 5) {
                tr_error("EAP-TLS: decode error");
                return EAP_TLS_MSG_DECODE_ERROR;
            }
            length -= 4;
            data += 4;
        }
        // Last (or only) fragment or fragment acknowledge. If sending data
        // updates acknowledged fragments.
        if (new_seq_id && eap_tls_sec_prot_lib_ack_update(tls_send)) {
            // All send, free data
            eap_tls_sec_prot_lib_message_allocate(tls_send, TLS_HEAD_LEN, 0);
            result = EAP_TLS_MSG_SEND_DONE;
        }
    }

    length -= 1;  // EAP-TLS flags
    data += 1;

    // No further processing for EAP-TLS start
    if (result == EAP_TLS_MSG_START) {
        return EAP_TLS_MSG_START;
    }

    // TLS data not included
    if (length == 0) {
        if (new_seq_id && result == EAP_TLS_MSG_CONTINUE) {
            // If received only EAP-TLS header fails, and is not
            // fragment acknowledge or last frame
            result = EAP_TLS_MSG_FAIL;
        }

        return result;
    }

    // New (not seen) sequence identifier, update received data
    if (new_seq_id) {
        if (!tls_recv->data) {
            eap_tls_sec_prot_lib_message_allocate(tls_recv, 0, length);
        }
        if (eap_tls_sec_prot_lib_fragment_read(tls_recv, data, length)) {
            result = EAP_TLS_MSG_RECEIVE_DONE;
        }
    }

    return result;
}

uint8_t *eap_tls_sec_prot_lib_message_build(uint8_t eap_code, uint8_t eap_type, uint8_t *flags, uint8_t eap_id_seq, uint8_t header_size, tls_data_t *tls_send, uint16_t *length)
{
    uint16_t eap_len = 4;
    uint8_t *data_ptr = NULL;

    // Write EAP-TLS data (from EAP-TLS flags field onward)
    if (tls_send->data) {
        data_ptr = eap_tls_sec_prot_lib_fragment_write(tls_send->data + TLS_HEAD_LEN, tls_send->total_len, tls_send->handled_len, &eap_len, flags);
    }

    eapol_pdu_t eapol_pdu;

    *length = eapol_pdu_eap_frame_init(&eapol_pdu, eap_code, eap_id_seq, eap_type, eap_len, data_ptr);

    uint8_t *eapol_decoded_data = ns_dyn_mem_temporary_alloc(*length + header_size);
    if (!eapol_decoded_data) {
        return NULL;
    }

    eapol_write_pdu_frame(eapol_decoded_data + header_size, &eapol_pdu);

    return eapol_decoded_data;
}

static int8_t eap_tls_sec_prot_lib_ack_update(tls_data_t *tls)
{
    if (!tls->data || !tls->total_len) {
        return false;
    }

    if (tls->handled_len + TLS_FRAGMENT_LEN < tls->total_len) {
        tls->handled_len += TLS_FRAGMENT_LEN;
        return false;
    }

    tls->handled_len = tls->total_len;
    return true;
}

static int8_t eap_tls_sec_prot_lib_fragment_read(tls_data_t *tls, uint8_t *data, uint16_t len)
{
    if (tls->handled_len + len > tls->total_len) {
        return true;
    }

    memcpy(tls->data + tls->handled_len, data, len);
    tls->handled_len += len;

    if (tls->handled_len == tls->total_len) {
        return true;
    }

    return false;
}

static uint8_t *eap_tls_sec_prot_lib_fragment_write(uint8_t *data, uint16_t total_len, uint16_t handled_len, uint16_t *message_len, uint8_t *flags)
{
    uint8_t *data_begin = data + handled_len;

    if (*flags != 0xff) {
        data_begin -= 1;
        *message_len += 1;
        data_begin[0] = *flags;
    }

    if (total_len - handled_len > TLS_FRAGMENT_LEN) {
        *message_len += TLS_FRAGMENT_LEN;

        if (handled_len == 0) {
            data_begin -= 4; // length
            *message_len += 4;
            *flags |= EAP_TLS_MORE_FRAGMENTS | EAP_TLS_FRAGMENT_LENGTH;
            data_begin[0] = *flags;
            common_write_32_bit(total_len, &data_begin[1]);
        } else {
            *flags |= EAP_TLS_MORE_FRAGMENTS;
            data_begin[0] = *flags;
        }
    } else {
        *message_len += total_len - handled_len;
    }

    return data_begin;
}

#endif /* HAVE_WS */

