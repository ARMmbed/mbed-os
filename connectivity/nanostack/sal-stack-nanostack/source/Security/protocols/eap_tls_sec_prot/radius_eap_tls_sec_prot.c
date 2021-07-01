/*
 * Copyright (c) 2020, Pelion and affiliates.
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
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "Security/PANA/pana_eap_header.h"
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
#include "Security/protocols/eap_tls_sec_prot/radius_eap_tls_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "eapr"

typedef enum {
    EAP_TLS_STATE_INIT = SEC_STATE_INIT,
    EAP_TLS_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    EAP_TLS_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    EAP_TLS_STATE_CREATE_IND = SEC_STATE_CREATE_IND,

    EAP_TLS_STATE_RESPONSE_ID = SEC_STATE_FIRST,
    EAP_TLS_STATE_EAP_REQUEST,
    EAP_TLS_STATE_EAP_RESPONSE,
    EAP_TLS_STATE_RESPONSE_START,
    EAP_TLS_STATE_RESPONSE,

    EAP_TLS_STATE_FINISH = SEC_STATE_FINISH,
    EAP_TLS_STATE_FINISHED = SEC_STATE_FINISHED
} eap_tls_sec_prot_state_e;

// Filters initial EAPOL-key re-transmission bursts
#define BURST_FILTER_TIMER_TIMEOUT                  5 * 10

// How many times initial EAPOL-key is accepted on wait for identity response state
#define INITIAL_EAPOL_KEY_MAX_COUNT                 2

// How long to wait RADIUS client to proceed with handshake (RADIUS server to answer)
#define RADIUS_EAP_TLS_CLIENT_TIMEOUT               60 * 10    // 60 seconds

typedef struct {
    sec_prot_common_t             common;                  /**< Common data */
    sec_prot_t                    *radius_client_prot;     /**< RADIUS client security protocol */
    sec_prot_receive              *radius_client_send;     /**< RADIUS client security protocol send (receive from peer) */
    sec_prot_delete               *radius_client_deleted;  /**< RADIUS client security protocol peer deleted (notify to peer that radius EAP-TLS deleted) */
    eapol_pdu_t                   recv_eapol_pdu;          /**< Received EAPOL PDU */
    uint16_t                      recv_eap_msg_len;        /**< Received EAP message length */
    uint8_t                       *recv_eap_msg;           /**< Received EAP message */
    uint16_t                      burst_filt_timer;        /**< Burst filter timer */
    uint8_t                       eap_id_seq;              /**< EAP sequence */
    uint8_t                       recv_eap_id_seq;         /**< Last received EAP sequence */
    uint8_t                       eap_code;                /**< Received EAP code */
    uint8_t                       eap_type;                /**< Received EAP type */
    uint8_t                       init_key_cnt;            /**< How many time initial EAPOL-key has been received */
} radius_eap_tls_sec_prot_int_t;

static uint16_t radius_eap_tls_sec_prot_size(void);
static int8_t radius_eap_tls_sec_prot_init(sec_prot_t *prot);

static void radius_eap_tls_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static void radius_eap_tls_sec_prot_delete(sec_prot_t *prot);
static int8_t radius_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static int8_t radius_eap_tls_sec_prot_radius_client_receive(sec_prot_t *radius_client, void *pdu, uint16_t size);
static void radius_eap_tls_sec_prot_eap_tls_msg_free(sec_prot_t *prot);
static int8_t radius_eap_tls_sec_prot_radius_eap_message_forward(sec_prot_t *prot, uint8_t *eap_code);
static int8_t radius_eap_tls_sec_prot_radius_eap_message_retry(sec_prot_t *prot);

static void radius_eap_tls_sec_prot_state_machine(sec_prot_t *prot);

static int8_t radius_eap_tls_sec_prot_message_handle(sec_prot_t *prot, uint8_t *data_ptr, uint16_t *length);
static int8_t radius_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type);

static void radius_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);
static int8_t radius_eap_tls_sec_prot_init_radius_client(sec_prot_t *prot);
static void radius_eap_tls_sec_prot_radius_client_deleted(sec_prot_t *prot);

static void radius_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot);

#define eap_tls_sec_prot_get(prot) (radius_eap_tls_sec_prot_int_t *) &prot->data

int8_t radius_eap_tls_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, RADIUS_IEEE_802_1X_MKA, radius_eap_tls_sec_prot_size, radius_eap_tls_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t radius_eap_tls_sec_prot_size(void)
{
    return sizeof(radius_eap_tls_sec_prot_int_t);
}

static int8_t radius_eap_tls_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = radius_eap_tls_sec_prot_create_request;
    prot->create_resp = 0;
    prot->receive = radius_eap_tls_sec_prot_receive;
    prot->receive_peer = radius_eap_tls_sec_prot_radius_client_receive;
    prot->peer_deleted = radius_eap_tls_sec_prot_radius_client_deleted;
    prot->delete = radius_eap_tls_sec_prot_delete;
    prot->state_machine = radius_eap_tls_sec_prot_state_machine;
    prot->timer_timeout = radius_eap_tls_sec_prot_timer_timeout;
    prot->receive_peer_hdr_size += EAPOL_BASE_LENGTH; // 4 bytes of EAPOL data

    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_INIT);

    data->radius_client_prot = NULL;
    data->burst_filt_timer = BURST_FILTER_TIMER_TIMEOUT;
    data->eap_id_seq = 0;
    data->recv_eap_id_seq = 0;
    data->eap_code = 0;
    data->eap_type = 0;
    data->init_key_cnt = 0;
    return 0;
}

static void radius_eap_tls_sec_prot_delete(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->recv_eap_msg != NULL) {
        ns_dyn_mem_free(data->recv_eap_msg);
    }
}

static void radius_eap_tls_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    prot->sec_keys = sec_keys;

    // Call state machine
    prot->state_machine_call(prot);
}

static int8_t radius_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Handle EAP messages
        if (data->recv_eapol_pdu.packet_type == EAPOL_EAP_TYPE) {
            data->eap_code = data->recv_eapol_pdu.msg.eap.eap_code;
            data->eap_type = data->recv_eapol_pdu.msg.eap.type;

            // Call state machine
            prot->state_machine(prot);
        } else if (data->recv_eapol_pdu.packet_type == EAPOL_KEY_TYPE &&
                   sec_prot_state_get(&data->common) == EAP_TLS_STATE_RESPONSE_ID) {
            /* If initial EAPOL-key transmission arrives to first EAP-TLS wait state i.e.
             * when waiting for identity response, triggers re-transmission of identity
             * request. This allows the supplicant to start EAP-TLS right away, if it has
             * missed the original identity request.
             */
            if (data->burst_filt_timer == 0 && data->init_key_cnt < INITIAL_EAPOL_KEY_MAX_COUNT) {
                tr_info("EAP-TLS: initial EAPOL-key recv, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
                sec_prot_result_set(&data->common, SEC_RESULT_TIMEOUT);
                // Call state machine
                prot->state_machine(prot);
                // Resets trickle timer to give time for supplicant to answer
                sec_prot_timer_trickle_start(&data->common, &prot->sec_cfg->prot_cfg.sec_prot_trickle_params);
                data->init_key_cnt++;
            }
            // Filters repeated initial EAPOL-key messages
            data->burst_filt_timer = BURST_FILTER_TIMER_TIMEOUT;
        }
        ret_val = 0;
    }

    memset(&data->recv_eapol_pdu, 0, sizeof(eapol_pdu_t));
    data->eap_code = 0;
    data->eap_type = 0;

    return ret_val;
}

static int8_t radius_eap_tls_sec_prot_message_handle(sec_prot_t *prot, uint8_t *data_ptr, uint16_t *length)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    data_ptr = data->recv_eapol_pdu.msg.eap.data_ptr;
    *length = data->recv_eapol_pdu.msg.eap.length;

    bool old_seq_id = false;

    // Already received sequence ID is received again, ignore
    if (data->recv_eapol_pdu.msg.eap.id_seq < data->eap_id_seq) {
        old_seq_id = true;
    } else if (data->recv_eapol_pdu.msg.eap.id_seq == data->eap_id_seq) {
        // Confirmation that supplicant has received the message, proceed with protocol
        data->recv_eap_id_seq = data->recv_eapol_pdu.msg.eap.id_seq;
        data->eap_id_seq++;
    }

    tr_info("EAP-TLS: recv %s type %s id %i flags %x len %i, eui-64 %s", eap_msg_trace[data->eap_code - 1],
            data->eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->recv_eapol_pdu.msg.eap.id_seq,
            *length >= 6 ? data_ptr[0] : 0, *length, trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    if (old_seq_id) {
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    if (data->eap_type == EAP_IDENTITY) {
        return EAP_TLS_MSG_IDENTITY;
    }

    if (!data_ptr || *length < 6) {
        tr_error("EAP-TLS: decode error");
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    return EAP_TLS_MSG_CONTINUE;
}

static int8_t radius_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint8_t flags = 0xff;
    uint16_t eapol_pdu_size;
    uint8_t *eapol_decoded_data = eap_tls_sec_prot_lib_message_build(eap_code, eap_type, &flags, data->eap_id_seq, prot->header_size, NULL, &eapol_pdu_size);
    if (!eapol_decoded_data) {
        return -1;
    }

    tr_info("EAP-TLS: send %s type %s id %i flags %x len %i, eui-64: %s", eap_msg_trace[eap_code - 1],
            eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->eap_id_seq, flags, eapol_pdu_size,
            trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    if (prot->send(prot, eapol_decoded_data, eapol_pdu_size + prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static int8_t radius_eap_tls_sec_prot_radius_eap_message_forward(sec_prot_t *prot, uint8_t *eap_code)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint16_t eap_pdu_len = data->recv_eap_msg_len - prot->receive_peer_hdr_size;
    uint8_t *eap_pdu = data->recv_eap_msg + prot->receive_peer_hdr_size;

    if (eap_pdu_len < 4) {
        return -1;
    }

    *eap_code = *eap_pdu++;
    uint8_t eap_id_seq = *eap_pdu++;
    uint16_t eap_len = common_read_16_bit(eap_pdu);
    eap_pdu += 2;

    if (eap_pdu_len != eap_len) {
        return -1;
    }

    uint16_t eap_body_len = eap_len;
    uint8_t eap_type = 0;
    uint8_t flags = 0;
    uint8_t *tls_ptr = NULL;

    if (*eap_code == EAP_REQ || *eap_code == EAP_RESPONSE) {
        eap_type = *eap_pdu++;
        eap_body_len--;
        if (eap_type == EAP_TLS && eap_len >= 5) {
            tls_ptr = eap_pdu;
            flags = *tls_ptr;
        }
    }

    eapol_pdu_t eapol_pdu;
    uint16_t eapol_pdu_size = eapol_pdu_eap_frame_init(&eapol_pdu, *eap_code, eap_id_seq, eap_type, eap_body_len, tls_ptr);
    if (eapol_pdu_size - EAPOL_BASE_LENGTH != eap_len) {
        return -1;
    }

    eapol_write_pdu_frame(data->recv_eap_msg + prot->header_size, &eapol_pdu);

    tr_info("EAP-TLS: send %s type %s id %i flags %x len %i, eui-64: %s", eap_msg_trace[*eap_code - 1],
            eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->eap_id_seq, flags, eapol_pdu_size,
            trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    if (prot->conn_send(prot, data->recv_eap_msg, eapol_pdu_size + prot->header_size, 0, SEC_PROT_SEND_FLAG_NO_DEALLOC) < 0) {
        return -1;
    }
    data->recv_eap_msg_len = eapol_pdu_size + prot->header_size;

    return 0;
}

static int8_t radius_eap_tls_sec_prot_radius_eap_message_retry(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->recv_eap_msg == NULL || data->recv_eap_msg_len == 0) {
        return -1;
    }

    if (prot->conn_send(prot, data->recv_eap_msg, data->recv_eap_msg_len, 0, SEC_PROT_SEND_FLAG_NO_DEALLOC) < 0) {
        return -1;
    }

    return 0;
}

static void radius_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->burst_filt_timer > ticks) {
        data->burst_filt_timer -= ticks;
    } else {
        data->burst_filt_timer = 0;
    }

    sec_prot_timer_timeout_handle(prot, &data->common,
                                  &prot->sec_cfg->prot_cfg.sec_prot_trickle_params, ticks);
}

static int8_t radius_eap_tls_sec_prot_radius_client_receive(sec_prot_t *radius_client, void *pdu, uint16_t size)
{
    sec_prot_t *prot = radius_client->type_get(radius_client, SEC_PROT_TYPE_RADIUS_EAP_TLS);
    if (!prot) {
        return -1;
    }

    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->recv_eap_msg != NULL) {
        ns_dyn_mem_free(data->recv_eap_msg);
    }

    data->recv_eap_msg_len = size;
    data->recv_eap_msg = pdu;

    prot->state_machine_call(prot);

    return 0;
}

static void radius_eap_tls_sec_prot_eap_tls_msg_free(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->recv_eap_msg != NULL) {
        ns_dyn_mem_free(data->recv_eap_msg);
    }

    data->recv_eap_msg = NULL;
    data->recv_eap_msg_len = 0;
}

static int8_t radius_eap_tls_sec_prot_init_radius_client(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    if (data->radius_client_prot) {
        return 0;
    }

    data->radius_client_prot = prot->type_get(prot, SEC_PROT_TYPE_RADIUS_CLIENT);
    if (!data->radius_client_prot) {
        return -1;
    }
    data->radius_client_send = data->radius_client_prot->receive_peer;
    data->radius_client_deleted = data->radius_client_prot->peer_deleted;

    return 0;
}

static void radius_eap_tls_sec_prot_radius_client_deleted(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    tr_info("EAP-TLS: client deleted");

    data->radius_client_prot = NULL;
    data->radius_client_send = NULL;
    data->radius_client_deleted = NULL;
}

static void radius_eap_tls_sec_prot_state_machine(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    uint8_t *data_ptr = NULL;
    uint16_t length = 0;

    // EAP-TLS authenticator state machine
    switch (sec_prot_state_get(&data->common)) {
        case EAP_TLS_STATE_INIT:
            tr_info("EAP-TLS: init");
            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_CREATE_REQ);
            prot->timer_start(prot);
            break;

        // Wait KMP-CREATE.request
        case EAP_TLS_STATE_CREATE_REQ:
            tr_info("EAP-TLS: start, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Set default timeout for the total maximum length of the negotiation
            sec_prot_default_timeout_set(&data->common);

            // KMP-CREATE.confirm
            prot->create_conf(prot, SEC_RESULT_OK);

            // Increment sequence ID
            radius_eap_tls_sec_prot_seq_id_update(prot);

            // Sends EAP request, Identity
            radius_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_IDENTITY);

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &prot->sec_cfg->prot_cfg.sec_prot_trickle_params);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_RESPONSE_ID);
            break;

        // Wait EAP response, Identity
        case EAP_TLS_STATE_RESPONSE_ID:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                // Re-sends EAP request, Identity
                radius_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_IDENTITY);
                return;
            }

            // Handle EAP response (expected Identity)
            if (radius_eap_tls_sec_prot_message_handle(prot, data_ptr, &length) != EAP_TLS_MSG_IDENTITY) {
                return;
            }

            tr_info("EAP-TLS: EAP response id, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            if (radius_eap_tls_sec_prot_init_radius_client(prot) < 0) {
                tr_error("EAP-TLS: radius client init failed");
                return;
            }

            // Free EAP request buffer since answer received and retries not needed
            radius_eap_tls_sec_prot_eap_tls_msg_free(prot);

            // Stop trickle timer, radius client will continue
            sec_prot_timer_trickle_stop(&data->common);

            // Set timeout to wait for RADIUS client to continue
            data->common.ticks = RADIUS_EAP_TLS_CLIENT_TIMEOUT;

            // Send to radius client
            data->radius_client_send(data->radius_client_prot, (void *) &data->recv_eapol_pdu, length);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_EAP_REQUEST);
            break;

        // Wait EAP request
        case EAP_TLS_STATE_EAP_REQUEST:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                // Do nothing (trickle timer not running, so should not happen)
                return;
            }

            tr_info("EAP-TLS: EAP request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            uint8_t eap_code;
            if (radius_eap_tls_sec_prot_radius_eap_message_forward(prot, &eap_code) < 0) {
                tr_error("EAP-TLS: EAP message forward failed");
                return;
            }

            if (eap_code == EAP_SUCCESS) {
                sec_prot_result_set(&data->common, SEC_RESULT_OK);
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
            } else if (eap_code == EAP_FAILURE) {
                sec_prot_result_set(&data->common, SEC_RESULT_ERROR);
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
            }

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &prot->sec_cfg->prot_cfg.sec_prot_trickle_params);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_EAP_RESPONSE);
            break;

        // Wait EAP response
        case EAP_TLS_STATE_EAP_RESPONSE:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                tr_info("EAP-TLS: retry EAP request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
                if (radius_eap_tls_sec_prot_radius_eap_message_retry(prot) < 0) {
                    tr_error("EAP-TLS: retry msg send error");
                }
                return;
            }

            tr_info("EAP-TLS: EAP response, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Handle EAP response
            if (radius_eap_tls_sec_prot_message_handle(prot, data_ptr, &length) != EAP_TLS_MSG_CONTINUE) {
                return;
            }

            // Free EAP request buffer since answer received and retries not needed
            radius_eap_tls_sec_prot_eap_tls_msg_free(prot);

            // Stop trickle timer, radius client will continue
            sec_prot_timer_trickle_stop(&data->common);

            // Set timeout to wait for RADIUS client to continue
            data->common.ticks = RADIUS_EAP_TLS_CLIENT_TIMEOUT;

            // Send to radius client
            data->radius_client_send(data->radius_client_prot, (void *) &data->recv_eapol_pdu, length);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_EAP_REQUEST);
            break;

        case EAP_TLS_STATE_FINISH:
            tr_info("EAP-TLS finish, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // KMP-FINISHED.indication,
            prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISHED);
            break;

        case EAP_TLS_STATE_FINISHED: {
            uint8_t *remote_eui_64 = sec_prot_remote_eui_64_addr_get(prot);
            tr_info("EAP-TLS finished, eui-64: %s", remote_eui_64 ? trace_array(sec_prot_remote_eui_64_addr_get(prot), 8) : "not set");

            // Indicate to radius client peer protocol that radius EAP-TLS has been deleted
            if (data->radius_client_deleted) {
                data->radius_client_deleted(data->radius_client_prot);
            }

            prot->timer_stop(prot);
            prot->finished(prot);
            break;
        }
        default:
            break;
    }
}

static void radius_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot)
{
    radius_eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    data->eap_id_seq++;
}

#endif /* HAVE_WS */

