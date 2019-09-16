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
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"
#include "Security/protocols/eap_tls_sec_prot/eap_tls_sec_prot_lib.h"
#include "Security/protocols/eap_tls_sec_prot/auth_eap_tls_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "eapa"

typedef enum {
    EAP_TLS_STATE_INIT = SEC_STATE_INIT,
    EAP_TLS_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    EAP_TLS_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    EAP_TLS_STATE_CREATE_IND = SEC_STATE_CREATE_IND,

    EAP_TLS_STATE_RESPONSE_ID = SEC_STATE_FIRST,
    EAP_TLS_STATE_RESPONSE_START,
    EAP_TLS_STATE_RESPONSE,

    EAP_TLS_STATE_FINISH = SEC_STATE_FINISH,
    EAP_TLS_STATE_FINISHED = SEC_STATE_FINISHED
} eap_tls_sec_prot_state_e;

typedef struct {
    sec_prot_common_t             common;           /**< Common data */
    sec_prot_t                    *tls_prot;        /**< TLS security protocol */
    eapol_pdu_t                   recv_eapol_pdu;   /**< Received EAPOL PDU */
    tls_data_t                    tls_send;         /**< EAP-TLS send buffer */
    tls_data_t                    tls_recv;         /**< EAP-TLS receive buffer */
    uint8_t                       eap_id_seq;       /**< EAP sequence */
    uint8_t                       recv_eap_id_seq;  /**< Last received EAP sequence */
    uint8_t                       eap_code;         /**< Received EAP code */
    uint8_t                       eap_type;         /**< Received EAP type */
    int8_t                        tls_result;       /**< Result of TLS operation */
    bool                          wait_tls: 1;      /**< Wait TLS (ECC calculation) before sending EAP-TLS message */
    bool                          tls_ongoing: 1;   /**< TLS handshake is ongoing */
    bool                          send_pending: 1;  /**< TLS data is not yet send to network */
} eap_tls_sec_prot_int_t;

/*Small network setup*/
#define EAP_TLS_SMALL_IMIN 300 // retries done in 30 seconds
#define EAP_TLS_SMALL_IMAX 900 // Largest value 90 seconds

/* Large network setup*/
#define EAP_TLS_LARGE_IMIN 600 // retries done in 60 seconds
#define EAP_TLS_LARGE_IMAX 2400 // Largest value 240 seconds


static trickle_params_t eap_tls_trickle_params = {
    .Imin = EAP_TLS_SMALL_IMIN,           /* ticks are 100ms */
    .Imax = EAP_TLS_SMALL_IMAX,           /* ticks are 100ms */
    .k = 0,                /* infinity - no consistency checking */
    .TimerExpirations = 2
};

static uint16_t auth_eap_tls_sec_prot_size(void);
static int8_t auth_eap_tls_sec_prot_init(sec_prot_t *prot);

static void auth_eap_tls_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static void auth_eap_tls_sec_prot_delete(sec_prot_t *prot);
static int8_t auth_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);

static void auth_eap_tls_sec_prot_state_machine(sec_prot_t *prot);

static int8_t auth_eap_tls_sec_prot_message_handle(sec_prot_t *prot);
static int8_t auth_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type, uint8_t tls_state);

static void auth_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);
static void auth_eap_tls_sec_prot_init_tls(sec_prot_t *prot);
static void auth_eap_tls_sec_prot_delete_tls(sec_prot_t *prot);

static void auth_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot);

#define eap_tls_sec_prot_get(prot) (eap_tls_sec_prot_int_t *) &prot->data

int8_t auth_eap_tls_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_1X_MKA, auth_eap_tls_sec_prot_size, auth_eap_tls_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

int8_t auth_eap_tls_sec_prot_timing_adjust(uint8_t timing)
{

    if (timing < 16) {
        eap_tls_trickle_params.Imin = EAP_TLS_SMALL_IMIN;
        eap_tls_trickle_params.Imax = EAP_TLS_SMALL_IMAX;
    } else {
        eap_tls_trickle_params.Imin = EAP_TLS_LARGE_IMIN;
        eap_tls_trickle_params.Imax = EAP_TLS_LARGE_IMAX;
    }
    return 0;
}

static uint16_t auth_eap_tls_sec_prot_size(void)
{
    return sizeof(eap_tls_sec_prot_int_t);
}

static int8_t auth_eap_tls_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = auth_eap_tls_sec_prot_create_request;
    prot->create_resp = 0;
    prot->receive = auth_eap_tls_sec_prot_receive;
    prot->delete = auth_eap_tls_sec_prot_delete;
    prot->state_machine = auth_eap_tls_sec_prot_state_machine;
    prot->timer_timeout = auth_eap_tls_sec_prot_timer_timeout;

    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_INIT);

    data->tls_prot = NULL;
    data->eap_id_seq = 0;
    data->recv_eap_id_seq = 0;
    data->eap_code = 0;
    data->eap_type = 0;
    eap_tls_sec_prot_lib_message_init(&data->tls_recv);
    eap_tls_sec_prot_lib_message_init(&data->tls_send);
    data->tls_result =  EAP_TLS_RESULT_ERROR;
    data->wait_tls = false;
    data->tls_ongoing = false;
    data->send_pending = false;
    return 0;
}

static void auth_eap_tls_sec_prot_delete(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    eap_tls_sec_prot_lib_message_free(&data->tls_send);
    eap_tls_sec_prot_lib_message_free(&data->tls_recv);
}

static void auth_eap_tls_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    prot->sec_keys = sec_keys;

    // Call state machine
    prot->state_machine_call(prot);
}

static int8_t auth_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Handle only EAP messages (ignore initial EAPOL-key retransmissions)
        if (data->recv_eapol_pdu.packet_type == EAPOL_EAP_TYPE) {
            data->eap_code = data->recv_eapol_pdu.msg.eap.eap_code;
            data->eap_type = data->recv_eapol_pdu.msg.eap.type;

            // Call state machine
            prot->state_machine(prot);
        }
        ret_val = 0;
    }

    memset(&data->recv_eapol_pdu, 0, sizeof(eapol_pdu_t));
    data->eap_code = 0;
    data->eap_type = 0;

    return ret_val;
}

static int8_t auth_eap_tls_sec_prot_message_handle(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint8_t *data_ptr = data->recv_eapol_pdu.msg.eap.data_ptr;
    uint16_t length = data->recv_eapol_pdu.msg.eap.length;

    bool new_seq_id = false;
    bool old_seq_id = false;

    // Already received sequence ID is received again, ignore
    if (data->recv_eapol_pdu.msg.eap.id_seq < data->eap_id_seq) {
        old_seq_id = true;
    } else if (data->recv_eapol_pdu.msg.eap.id_seq == data->eap_id_seq) {
        // Confirmation that supplicant has received the message, proceed with protocol
        data->recv_eap_id_seq = data->recv_eapol_pdu.msg.eap.id_seq;
        data->eap_id_seq++;
        new_seq_id = true;
    }

    tr_info("EAP-TLS: recv %s type %s id %i flags %x len %i, eui-64 %s", eap_msg_trace[data->eap_code - 1],
            data->eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->recv_eapol_pdu.msg.eap.id_seq,
            length >= 6 ? data_ptr[0] : 0, length, trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    if (old_seq_id) {
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    if (data->eap_type == EAP_IDENTITY) {
        return EAP_TLS_MSG_IDENTITY;
    }

    if (!data_ptr || length < 6) {
        tr_error("EAP-TLS: decode error");
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    length -= 5; // EAP fields: code, id, length, type

    return eap_tls_sec_prot_lib_message_handle(data_ptr, length, new_seq_id, &data->tls_send, &data->tls_recv);
}

static int8_t auth_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type, uint8_t tls_state)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint8_t flags = 0xff;
    // EAP-TLS flags field is always present during TLS exchange
    if (tls_state == EAP_TLS_EXCHANGE_ONGOING) {
        flags = 0x00;
    }

    if (eap_code == EAP_REQ) {
        if (eap_type == EAP_TLS && tls_state == EAP_TLS_EXCHANGE_START) {
            eap_tls_sec_prot_lib_message_allocate(&data->tls_send, TLS_HEAD_LEN, 0);
            flags = EAP_TLS_START;
        }
    } else if (eap_code == EAP_SUCCESS || eap_code == EAP_FAILURE) {
        // Send Success and Failure with same identifier as received in EAP Response
        data->eap_id_seq = data->recv_eap_id_seq;
    } else {
        return -1;
    }

    uint16_t eapol_pdu_size;
    uint8_t *eapol_decoded_data = eap_tls_sec_prot_lib_message_build(eap_code, eap_type, &flags, data->eap_id_seq, prot->header_size, &data->tls_send, &eapol_pdu_size);
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

static void auth_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    sec_prot_timer_timeout_handle(prot, &data->common, &eap_tls_trickle_params, ticks);
}

static void auth_eap_tls_sec_prot_tls_create_indication(sec_prot_t *tls_prot)
{
    tls_prot->create_resp(tls_prot, SEC_RESULT_OK);
}

static void auth_eap_tls_sec_prot_tls_finished_indication(sec_prot_t *tls_prot, sec_prot_result_e result, sec_prot_keys_t *sec_keys)
{
    (void) sec_keys;

    sec_prot_t *prot = tls_prot->type_get(tls_prot, SEC_PROT_TYPE_EAP_TLS);
    if (!prot) {
        return;
    }

    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (result == SEC_RESULT_OK) {
        data->tls_result = EAP_TLS_RESULT_HANDSHAKE_OVER;
        tr_info("EAP-TLS: handshake success");
    } else if (result == SEC_RESULT_CONF_ERROR) {
        data->tls_result = EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR;
        tr_error("EAP-TLS: handshake fatal error");
    } else {
        data->tls_result = EAP_TLS_RESULT_HANDSHAKE_FAILED;
        tr_error("EAP-TLS: handshake failed");
    }

    data->tls_ongoing = false;

    if (data->tls_result == EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR) {
        // On fatal error terminate right away
        prot->state_machine_call(prot);
    }
}

static int8_t auth_eap_tls_sec_prot_tls_send(sec_prot_t *tls_prot, void *pdu, uint16_t size)
{
    sec_prot_t *prot = tls_prot->type_get(tls_prot, SEC_PROT_TYPE_EAP_TLS);
    if (!prot) {
        return -1;
    }

    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    eap_tls_sec_prot_lib_message_free(&data->tls_send);

    data->tls_send.data = pdu;
    data->tls_send.total_len = size;
    data->tls_send.handled_len = 0;

    data->send_pending = true;

    prot->state_machine_call(prot);

    return 0;
}

static void auth_eap_tls_sec_prot_init_tls(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    if (data->tls_prot) {
        return;
    }

    data->tls_prot = prot->type_get(prot, SEC_PROT_TYPE_TLS);
    if (!data->tls_prot) {
        return;
    }

    data->tls_prot->header_size = TLS_HEAD_LEN;
    data->tls_prot->sec_keys = prot->sec_keys;

    data->tls_prot->create_conf = NULL;
    data->tls_prot->create_ind = auth_eap_tls_sec_prot_tls_create_indication;
    data->tls_prot->finished_ind = auth_eap_tls_sec_prot_tls_finished_indication;
    data->tls_prot->send = auth_eap_tls_sec_prot_tls_send;

    data->tls_ongoing = true;
}

static void auth_eap_tls_sec_prot_delete_tls(sec_prot_t *prot)
{
    // Triggers TLS to terminate if it is not already terminating by its own
    sec_prot_t *tls_prot = prot->type_get(prot, SEC_PROT_TYPE_TLS);
    if (tls_prot) {
        tls_prot->finished_send(tls_prot);
    }
}

static void auth_eap_tls_sec_prot_state_machine(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    int8_t result = EAP_TLS_MSG_CONTINUE;

    // EAP-TLS authenticator state machine
    switch (sec_prot_state_get(&data->common)) {
        case EAP_TLS_STATE_INIT:
            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_CREATE_REQ);
            break;

        // Wait KMP-CREATE.request
        case EAP_TLS_STATE_CREATE_REQ:
            tr_info("EAP-TLS start, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            prot->timer_start(prot);

            // KMP-CREATE.confirm
            prot->create_conf(prot, SEC_RESULT_OK);

            // Increment sequence ID
            auth_eap_tls_sec_prot_seq_id_update(prot);

            // Sends EAP request, Identity
            auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_IDENTITY, EAP_TLS_EXCHANGE_NONE);

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &eap_tls_trickle_params);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_RESPONSE_ID);
            break;

        // Wait EAP response, Identity
        case EAP_TLS_STATE_RESPONSE_ID:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                // Re-sends EAP request, Identity
                auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_IDENTITY, EAP_TLS_EXCHANGE_NONE);
                return;
            }

            // Handle EAP response (expected Identity)
            if (auth_eap_tls_sec_prot_message_handle(prot) != EAP_TLS_MSG_IDENTITY) {
                return;
            }

            // Sends EAP request, TLS EAP start
            auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_TLS, EAP_TLS_EXCHANGE_START);

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &eap_tls_trickle_params);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_RESPONSE_START);
            break;

        // Wait EAP response, TLS handshake
        case EAP_TLS_STATE_RESPONSE_START:
        case EAP_TLS_STATE_RESPONSE:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                if (sec_prot_state_get(&data->common) == EAP_TLS_STATE_RESPONSE_START) {
                    // Re-sends EAP request, TLS EAP start
                    auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_TLS, EAP_TLS_EXCHANGE_START);
                } else {
                    // Re-sends EAP request, TLS EAP
                    auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_TLS, EAP_TLS_EXCHANGE_ONGOING);
                }
                return;
            }

            // EAP response
            if (data->eap_code == EAP_RESPONSE) {
                // Handle EAP response, TLS EAP
                result = auth_eap_tls_sec_prot_message_handle(prot);
                if (result == EAP_TLS_MSG_DECODE_ERROR) {
                    return;
                }
                if (result == EAP_TLS_MSG_IDENTITY) {
                    // If received EAP response, Identity: re-sends EAP request, TLS EAP start
                    auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_TLS, EAP_TLS_EXCHANGE_START);
                    return;
                }

                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_RESPONSE);

                // All fragments received for a message
                if (result == EAP_TLS_MSG_RECEIVE_DONE) {
                    auth_eap_tls_sec_prot_init_tls(prot);

                    if (data->tls_ongoing) {
                        // Call TLS
                        data->tls_prot->receive(data->tls_prot, data->tls_recv.data, data->tls_recv.total_len);
                        eap_tls_sec_prot_lib_message_init(&data->tls_recv);
                        sec_prot_timer_trickle_stop(&data->common);
                        if (data->tls_result == EAP_TLS_RESULT_HANDSHAKE_FAILED && !data->send_pending) {
                            // In case has received alert and aborted can fail already here
                            eap_tls_sec_prot_lib_message_free(&data->tls_send);
                        } else {
                            data->wait_tls = true;
                        }
                    }
                } else if (result == EAP_TLS_MSG_SEND_DONE) {
                    // All fragments send for a message, no new fragment received
                    eap_tls_sec_prot_lib_message_free(&data->tls_send);
                }
                // Wait TLS to process the received message
                if (data->wait_tls) {
                    return;
                }
            } else {
                // Call from TLS
                if (data->tls_result == EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR) {
                    // Send failure
                    eap_tls_sec_prot_lib_message_free(&data->tls_send);
                }

                // Call from TLS
                data->wait_tls = false;
            }

            // TLS EAP message to be send
            if (data->tls_send.total_len > 0 || result == EAP_TLS_MSG_MORE_FRAG) {
                data->send_pending = false;

                // Sends EAP request, TLS EAP, TLS exchange
                auth_eap_tls_sec_prot_message_send(prot, EAP_REQ, EAP_TLS, EAP_TLS_EXCHANGE_ONGOING);

                // Start trickle timer to re-send if no response
                sec_prot_timer_trickle_start(&data->common, &eap_tls_trickle_params);
            } else {
                // TLS done, indicate success to peer
                if (data->tls_result == EAP_TLS_RESULT_HANDSHAKE_OVER) {
                    // Supplicant PMK is now valid
                    sec_prot_keys_pmk_mismatch_reset(prot->sec_keys);
                    // Sends EAP success
                    auth_eap_tls_sec_prot_message_send(prot, EAP_SUCCESS, 0, EAP_TLS_EXCHANGE_NONE);
                } else {
                    // Sends EAP failure
                    auth_eap_tls_sec_prot_message_send(prot, EAP_FAILURE, 0, EAP_TLS_EXCHANGE_NONE);
                    sec_prot_result_set(&data->common, SEC_RESULT_ERROR);
                }

                // Done
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
            }
            break;

        case EAP_TLS_STATE_FINISH:
            tr_info("EAP-TLS finish, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // KMP-FINISHED.indication,
            prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISHED);
            break;

        case EAP_TLS_STATE_FINISHED:
            auth_eap_tls_sec_prot_delete_tls(prot);
            prot->timer_stop(prot);
            prot->finished(prot);
            break;

        default:
            break;
    }
}

static void auth_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    data->eap_id_seq++;
}

#endif /* HAVE_WS */

