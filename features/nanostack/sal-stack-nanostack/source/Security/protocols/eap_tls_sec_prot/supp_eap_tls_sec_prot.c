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
#include "Security/protocols/eap_tls_sec_prot/supp_eap_tls_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "eaps"

typedef enum {
    EAP_TLS_STATE_INIT = SEC_STATE_INIT,
    EAP_TLS_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    EAP_TLS_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    EAP_TLS_STATE_CREATE_IND = SEC_STATE_CREATE_IND,

    EAP_TLS_STATE_REQUEST_ID = SEC_STATE_FIRST,
    EAP_TLS_STATE_REQUEST_TLS_EAP,
    EAP_TLS_STATE_REQUEST,

    EAP_TLS_STATE_FINISH = SEC_STATE_FINISH,
    EAP_TLS_STATE_FINISHED = SEC_STATE_FINISHED
} eap_tls_sec_prot_state_e;

// Filters EAP re-transmission bursts that arrive with same EAP sequence number
#define BURST_FILTER_TIMER_TIMEOUT                  5 * 10

typedef struct {
    sec_prot_common_t             common;           /**< Common data */
    sec_prot_t                    *tls_prot;        /**< TLS security protocol */
    eapol_pdu_t                   recv_eapol_pdu;   /**< Received EAPOL PDU */
    tls_data_t                    tls_send;         /**< EAP-TLS send buffer */
    tls_data_t                    tls_recv;         /**< EAP-TLS receive buffer */
    uint16_t                      burst_filt_timer; /**< Burst filter timer */
    uint8_t                       eap_id_seq;       /**< EAP sequence */
    uint8_t                       eap_code;         /**< Received EAP code */
    uint8_t                       eap_type;         /**< Received EAP type */
    int8_t                        tls_result;       /**< Result of TLS operation */
    bool                          wait_tls: 1;      /**< Wait TLS (ECC calculation) before sending EAP-TLS message */
    bool                          tls_ongoing: 1;   /**< TLS handshake is ongoing */
    bool                          send_pending: 1;  /**< TLS data is not yet send to network */
} eap_tls_sec_prot_int_t;

#define FWH_RETRY_TIMEOUT_SMALL 330*10 // retry timeout for small network additional 30 seconds for authenticator delay
#define FWH_RETRY_TIMEOUT_LARGE 750*10 // retry timeout for large network additional 30 seconds for authenticator delay

static uint16_t retry_timeout = FWH_RETRY_TIMEOUT_SMALL;

static uint16_t supp_eap_tls_sec_prot_size(void);
static int8_t supp_eap_tls_sec_prot_init(sec_prot_t *prot);

static void supp_eap_tls_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);
static void supp_eap_tls_sec_prot_delete(sec_prot_t *prot);
static int8_t supp_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);

static void supp_eap_tls_sec_prot_state_machine(sec_prot_t *prot);

static int8_t supp_eap_tls_sec_prot_message_handle(sec_prot_t *prot);
static int8_t supp_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type, uint8_t tls_state);

static void supp_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);
static int8_t supp_eap_tls_sec_prot_init_tls(sec_prot_t *prot);
static void supp_eap_tls_sec_prot_delete_tls(sec_prot_t *prot);

static void supp_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot);

#define eap_tls_sec_prot_get(prot) (eap_tls_sec_prot_int_t *) &prot->data

int8_t supp_eap_tls_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_1X_MKA, supp_eap_tls_sec_prot_size, supp_eap_tls_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

int8_t supp_eap_sec_prot_timing_adjust(uint8_t timing)
{
    if (timing < 16) {
        retry_timeout = FWH_RETRY_TIMEOUT_SMALL;
    } else {
        retry_timeout = FWH_RETRY_TIMEOUT_LARGE;
    }
    return 0;
}


static uint16_t supp_eap_tls_sec_prot_size(void)
{
    return sizeof(eap_tls_sec_prot_int_t);
}

static int8_t supp_eap_tls_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = 0;
    prot->create_resp = supp_eap_tls_sec_prot_create_response;
    prot->receive = supp_eap_tls_sec_prot_receive;
    prot->delete = supp_eap_tls_sec_prot_delete;
    prot->state_machine = supp_eap_tls_sec_prot_state_machine;
    prot->timer_timeout = supp_eap_tls_sec_prot_timer_timeout;

    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_INIT);

    data->tls_prot = NULL;
    data->burst_filt_timer = 0;
    data->eap_id_seq = 0;
    data->eap_code = 0;
    data->eap_type = 0;
    eap_tls_sec_prot_lib_message_init(&data->tls_recv);
    eap_tls_sec_prot_lib_message_init(&data->tls_send);
    data->tls_result = EAP_TLS_RESULT_ERROR;
    data->wait_tls = false;
    data->tls_ongoing = false;
    return 0;
}

static void supp_eap_tls_sec_prot_delete(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    eap_tls_sec_prot_lib_message_free(&data->tls_send);
    eap_tls_sec_prot_lib_message_free(&data->tls_recv);
}

static void supp_eap_tls_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    // Call state machine
    sec_prot_result_set(&data->common, result);
    prot->state_machine_call(prot);
}

static int8_t supp_eap_tls_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Handle only EAP messages (should never receive EAPOL-key messages during EAP-TLS)
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

static int8_t supp_eap_tls_sec_prot_message_handle(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint8_t *data_ptr = data->recv_eapol_pdu.msg.eap.data_ptr;
    uint16_t length = data->recv_eapol_pdu.msg.eap.length;

    tr_info("EAP-TLS recv %s type %s id %i flags %x len %i", eap_msg_trace[data->eap_code - 1],
            data->eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->recv_eapol_pdu.msg.eap.id_seq,
            length >= 6 ? data_ptr[0] : 0, length);

    uint8_t new_seq_id = false;
    // New sequence identifier received
    if (data->recv_eapol_pdu.msg.eap.id_seq > data->eap_id_seq) {
        data->burst_filt_timer = BURST_FILTER_TIMER_TIMEOUT;
        new_seq_id = true;
    } else if (data->recv_eapol_pdu.msg.eap.id_seq == data->eap_id_seq) {
        if (data->burst_filt_timer > 0) {
            /* If retransmission arrives when burst filter timer is running, ignores it
               and starts timer again */
            data->burst_filt_timer = BURST_FILTER_TIMER_TIMEOUT;
            return EAP_TLS_MSG_DECODE_ERROR;
        } else {
            // If retransmission arrives after timeout, starts timer again
            data->burst_filt_timer = BURST_FILTER_TIMER_TIMEOUT;
        }
    } else if (data->recv_eapol_pdu.msg.eap.id_seq < data->eap_id_seq) {
        // Already received sequence ID is received again, ignore
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    if (data->eap_type == EAP_IDENTITY) {
        return EAP_TLS_MSG_IDENTITY;
    }

    if (!data_ptr || length < 6) {
        return EAP_TLS_MSG_DECODE_ERROR;
    }

    length -= 5; // EAP fields: code, id, length, type

    return eap_tls_sec_prot_lib_message_handle(data_ptr, length, new_seq_id, &data->tls_send, &data->tls_recv);
}

static int8_t supp_eap_tls_sec_prot_message_send(sec_prot_t *prot, uint8_t eap_code, uint8_t eap_type, uint8_t tls_state)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    uint8_t flags = 0xff;
    // EAP-TLS flags field is always present during TLS exchange
    if (tls_state == EAP_TLS_EXCHANGE_ONGOING) {
        flags = 0x00;
    }

    if (eap_code == EAP_RESPONSE) {
        if (eap_type == EAP_IDENTITY) {
            const uint8_t identity[] = {"Anonymous"};
            const uint8_t identity_size = sizeof(identity) - 1;
            if (data->tls_send.total_len != identity_size) {
                eap_tls_sec_prot_lib_message_allocate(&data->tls_send, TLS_HEAD_LEN, identity_size);
                if (!data->tls_send.data) {
                    return -1;
                }
                memcpy(data->tls_send.data + TLS_HEAD_LEN, identity, identity_size);
            }
            flags = 0xff;
        }
    } else {
        return -1;
    }

    uint16_t eapol_pdu_size;
    uint8_t *eapol_decoded_data = eap_tls_sec_prot_lib_message_build(eap_code, eap_type, &flags, data->eap_id_seq, prot->header_size, &data->tls_send, &eapol_pdu_size);
    if (!eapol_decoded_data) {
        return -1;
    }

    tr_info("EAP-TLS: send %s type %s id %i flags %x len %i", eap_msg_trace[eap_code - 1],
            eap_type == EAP_IDENTITY ? "IDENTITY" : "TLS", data->eap_id_seq, flags, eapol_pdu_size);

    if (prot->send(prot, eapol_decoded_data, eapol_pdu_size + prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static void supp_eap_tls_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

    if (data->burst_filt_timer > ticks) {
        data->burst_filt_timer -= ticks;
    } else {
        data->burst_filt_timer = 0;
    }

    sec_prot_timer_timeout_handle(prot, &data->common, NULL, ticks);
}

static void supp_eap_tls_sec_prot_tls_create_confirm(sec_prot_t *tls_prot, sec_prot_result_e result)
{
    if (result != SEC_RESULT_OK) {
        sec_prot_t *prot = tls_prot->type_get(tls_prot, SEC_PROT_TYPE_EAP_TLS);
        if (!prot) {
            return;
        }

        eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);

        sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
    }
}

static void supp_eap_tls_sec_prot_tls_finished_indication(sec_prot_t *tls_prot, sec_prot_result_e result, sec_prot_keys_t *sec_keys)
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
        // On failure has sent ALERT
        data->tls_result = EAP_TLS_RESULT_HANDSHAKE_FAILED;
        tr_error("EAP-TLS: handshake failed");
    }

    data->tls_ongoing = false;

    if (data->tls_result == EAP_TLS_RESULT_HANDSHAKE_OVER || data->tls_result == EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR) {
        // On fatal error and on success calls state machine to sent empty EAP-TLS message
        prot->state_machine_call(prot);
    }
}

static int8_t supp_eap_tls_sec_prot_tls_send(sec_prot_t *tls_prot, void *pdu, uint16_t size)
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

static int8_t supp_eap_tls_sec_prot_init_tls(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    if (data->tls_prot) {
        return 0;
    }

    data->tls_prot = prot->type_get(prot, SEC_PROT_TYPE_TLS);
    if (!data->tls_prot) {
        return -1;
    }

    data->tls_prot->header_size = TLS_HEAD_LEN;
    data->tls_prot->sec_keys = prot->sec_keys;

    data->tls_prot->create_conf = supp_eap_tls_sec_prot_tls_create_confirm;
    data->tls_prot->create_ind = NULL;
    data->tls_prot->finished_ind = supp_eap_tls_sec_prot_tls_finished_indication;
    data->tls_prot->send = supp_eap_tls_sec_prot_tls_send;

    data->tls_ongoing = true;

    return 0;
}

static void supp_eap_tls_sec_prot_delete_tls(sec_prot_t *prot)
{
    // Triggers TLS to terminate if it is not already terminating by its own
    sec_prot_t *tls_prot = prot->type_get(prot, SEC_PROT_TYPE_TLS);
    if (tls_prot) {
        tls_prot->finished_send(tls_prot);
    }
}

static void supp_eap_tls_sec_prot_state_machine(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    int8_t result;

    // EAP-TLS supplicant state machine
    switch (sec_prot_state_get(&data->common)) {
        case EAP_TLS_STATE_INIT:
            tr_info("EAP-TLS init");
            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_REQUEST_ID);
            prot->timer_start(prot);
            break;

        // Wait EAP request, Identity (starts handshake on supplicant)
        case EAP_TLS_STATE_REQUEST_ID:

            // Handle EAP request, Identity
            if (supp_eap_tls_sec_prot_message_handle(prot) != EAP_TLS_MSG_IDENTITY) {
                return;
            }

            // Set default timeout for the total maximum length of the negotiation
            sec_prot_default_timeout_set(&data->common);

            // Store sequence ID
            supp_eap_tls_sec_prot_seq_id_update(prot);

            tr_info("EAP-TLS start");

            // Send KMP-CREATE.indication
            prot->create_ind(prot);
            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_CREATE_RESP);
            break;

        // Wait KMP-CREATE.response
        case EAP_TLS_STATE_CREATE_RESP:
            if (sec_prot_result_ok_check(&data->common)) {
                // Send EAP response, Identity
                supp_eap_tls_sec_prot_message_send(prot, EAP_RESPONSE, EAP_IDENTITY, EAP_TLS_EXCHANGE_NONE);

                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_REQUEST_TLS_EAP);
            } else {
                // Ready to be deleted
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISHED);
            }
            break;

        case EAP_TLS_STATE_REQUEST_TLS_EAP:

            // Handle EAP request (expected TLS EAP start)
            result = supp_eap_tls_sec_prot_message_handle(prot);

            if (result == EAP_TLS_MSG_IDENTITY) {
                // If received EAP request, Identity: re-send EAP response, Identity
                supp_eap_tls_sec_prot_message_send(prot, EAP_RESPONSE, EAP_IDENTITY, EAP_TLS_EXCHANGE_NONE);
                return;
            }

            // Handle EAP request, TLS EAP start
            if (result != EAP_TLS_MSG_START) {
                return;
            }

            // Store sequence ID
            supp_eap_tls_sec_prot_seq_id_update(prot);

            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_REQUEST);
            data->common.ticks = retry_timeout;

            // Initialize TLS protocol
            if (supp_eap_tls_sec_prot_init_tls(prot) < 0) {
                tr_error("TLS init failed");
                return;
            }
            // Request TLS to start (send client hello)
            data->tls_prot->create_req(data->tls_prot, prot->sec_keys);
            break;

        case EAP_TLS_STATE_REQUEST:

            // EAP success
            if (data->eap_code == EAP_SUCCESS) {
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
                return;
            } else if (data->eap_code == EAP_FAILURE) {
                sec_prot_result_set(&data->common, SEC_RESULT_ERROR);
                sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISH);
                return;
            } else if (data->eap_code == EAP_REQ) {
                // EAP request, handle EAP request, TLS EAP
                result = supp_eap_tls_sec_prot_message_handle(prot);
                if (result == EAP_TLS_MSG_DECODE_ERROR) {
                    return;
                }

                // Store sequence ID
                supp_eap_tls_sec_prot_seq_id_update(prot);

                // All fragments received for a message
                if (result == EAP_TLS_MSG_RECEIVE_DONE && data->tls_ongoing) {
                    // Call TLS
                    data->tls_prot->receive(data->tls_prot, data->tls_recv.data, data->tls_recv.total_len);
                    eap_tls_sec_prot_lib_message_init(&data->tls_recv);
                    sec_prot_timer_trickle_stop(&data->common);
                    if (data->send_pending || data->tls_result != EAP_TLS_RESULT_HANDSHAKE_FAILED) {
                        data->wait_tls = true;
                    }
                }
                // Wait TLS to process the received message
                if (data->wait_tls) {
                    return;
                }
            } else {
                data->wait_tls = false;
                if (!data->tls_send.data || data->tls_result == EAP_TLS_RESULT_HANDSHAKE_FATAL_ERROR) {
                    // If no more data send response, TLS EAP (empty)
                    eap_tls_sec_prot_lib_message_allocate(&data->tls_send, TLS_HEAD_LEN, 0);
                }
            }
            // Send EAP response
            supp_eap_tls_sec_prot_message_send(prot, EAP_RESPONSE, EAP_TLS, EAP_TLS_EXCHANGE_ONGOING);
            data->send_pending = false;

            // Add more time for re-send if no response
            data->common.ticks = retry_timeout;

            break;

        case EAP_TLS_STATE_FINISH:
            tr_info("EAP-TLS finish");

            // KMP-FINISHED.indication,
            prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);
            sec_prot_state_set(prot, &data->common, EAP_TLS_STATE_FINISHED);
            break;

        case EAP_TLS_STATE_FINISHED:
            tr_info("EAP-TLS finished");
            supp_eap_tls_sec_prot_delete_tls(prot);
            prot->timer_stop(prot);
            prot->finished(prot);
            break;

        default:
            break;
    }
}

static void supp_eap_tls_sec_prot_seq_id_update(sec_prot_t *prot)
{
    eap_tls_sec_prot_int_t *data = eap_tls_sec_prot_get(prot);
    data->eap_id_seq = data->recv_eapol_pdu.msg.eap.id_seq;
}

#endif /* HAVE_WS */

