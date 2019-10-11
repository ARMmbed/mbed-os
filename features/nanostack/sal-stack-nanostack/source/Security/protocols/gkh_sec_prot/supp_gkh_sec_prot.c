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
#include "Security/protocols/gkh_sec_prot/supp_gkh_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "sgkh"

typedef enum {
    GKH_STATE_INIT = SEC_STATE_INIT,
    GKH_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    GKH_STATE_CREATE_IND = SEC_STATE_CREATE_IND,
    GKH_STATE_MESSAGE_1 = SEC_STATE_FIRST,
    GKH_STATE_FINISH = SEC_STATE_FINISH,
    GKH_STATE_FINISHED = SEC_STATE_FINISHED
} gkh_sec_prot_state_e;

typedef enum {
    GKH_MESSAGE_UNKNOWN = 0,
    GKH_MESSAGE_1,
    GKH_MESSAGE_2
} gkh_sec_prot_msg_e;

typedef struct {
    sec_prot_common_t             common;           /**< Common data */
    eapol_pdu_t                   recv_eapol_pdu;   /**< Received EAPOL PDU */
    void                          *recv_pdu;        /**< Received pdu */
    uint16_t                      recv_size;        /**< Received pdu size */
} gkh_sec_prot_int_t;

static uint16_t supp_gkh_sec_prot_size(void);
static int8_t supp_gkh_sec_prot_init(sec_prot_t *prot);

static void supp_gkh_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);
static void supp_gkh_sec_prot_delete(sec_prot_t *prot);
static int8_t supp_gkh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static gkh_sec_prot_msg_e supp_gkh_sec_prot_message_get(eapol_pdu_t *eapol_pdu, sec_prot_keys_t *sec_keys);
static void supp_gkh_sec_prot_state_machine(sec_prot_t *prot);

static int8_t supp_gkh_sec_prot_message_send(sec_prot_t *prot, gkh_sec_prot_msg_e msg);
static void supp_gkh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);
static int8_t supp_gkh_sec_prot_mic_validate(sec_prot_t *prot);
static void supp_gkh_sec_prot_security_replay_counter_update(sec_prot_t *prot);
static int8_t supp_gkh_kde_handle(sec_prot_t *prot);

#define gkh_sec_prot_get(prot) (gkh_sec_prot_int_t *) &prot->data

int8_t supp_gkh_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_11_GKH, supp_gkh_sec_prot_size, supp_gkh_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t supp_gkh_sec_prot_size(void)
{
    return sizeof(gkh_sec_prot_int_t);
}

static int8_t supp_gkh_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = 0;
    prot->create_resp = supp_gkh_sec_prot_create_response;
    prot->receive = supp_gkh_sec_prot_receive;
    prot->delete = supp_gkh_sec_prot_delete;
    prot->state_machine = supp_gkh_sec_prot_state_machine;
    prot->timer_timeout = supp_gkh_sec_prot_timer_timeout;

    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, GKH_STATE_INIT);

    return 0;
}

static void supp_gkh_sec_prot_delete(sec_prot_t *prot)
{
    // No op at the moment
    (void) prot;
}

static void supp_gkh_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);

    // Call state machine
    sec_prot_result_set(&data->common, result);
    prot->state_machine_call(prot);
}

static int8_t supp_gkh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Get message
        if (supp_gkh_sec_prot_message_get(&data->recv_eapol_pdu, prot->sec_keys) != GKH_MESSAGE_UNKNOWN) {
            tr_info("GKH: recv Message 1");

            // Call state machine
            data->recv_pdu = pdu;
            data->recv_size = size;
            prot->state_machine(prot);
        } else {
            tr_error("GKH: recv error");
        }
        ret_val = 0;
    } else {
        tr_error("GKH: recv error");
    }

    memset(&data->recv_eapol_pdu, 0, sizeof(eapol_pdu_t));
    data->recv_pdu = 0;
    data->recv_size = 0;

    return ret_val;
}

static gkh_sec_prot_msg_e supp_gkh_sec_prot_message_get(eapol_pdu_t *eapol_pdu, sec_prot_keys_t *sec_keys)
{
    gkh_sec_prot_msg_e msg = GKH_MESSAGE_UNKNOWN;

    if (eapol_pdu->msg.key.key_information.pairwise_key) {
        // This is mismatch between KMP ID indicating 802.11/GKH and key type
        return GKH_MESSAGE_UNKNOWN;
    }

    uint8_t key_mask = eapol_pdu_key_mask_get(eapol_pdu);

    switch (key_mask) {
        case KEY_INFO_KEY_ACK | KEY_INFO_KEY_MIC | KEY_INFO_SECURED_KEY_FRAME:
            // Must have valid replay counter
            if (sec_prot_keys_pmk_replay_cnt_compare(eapol_pdu->msg.key.replay_counter, sec_keys)) {
                if (eapol_pdu->msg.key.key_information.encrypted_key_data) {
                    // This should include the GTK KDE, Lifetime KDE and GTKL KDE.
                    msg = GKH_MESSAGE_1;
                }
            } else {
                tr_error("GKH: invalid replay counter %"PRId64, eapol_pdu->msg.key.replay_counter);
            }
            break;
        default:
            break;
    }

    return msg;
}

static int8_t supp_gkh_sec_prot_message_send(sec_prot_t *prot, gkh_sec_prot_msg_e msg)
{
    eapol_pdu_t eapol_pdu;
    uint16_t eapol_pdu_size = eapol_pdu_key_frame_init(&eapol_pdu, 0, NULL);

    switch (msg) {
        case GKH_MESSAGE_2:
            eapol_pdu.msg.key.replay_counter = sec_prot_keys_pmk_replay_cnt_get(prot->sec_keys);
            eapol_pdu.msg.key.key_information.key_mic = true;
            eapol_pdu.msg.key.key_information.secured_key_frame = true;
            eapol_pdu.msg.key.key_length = 0;
            break;
        default:
            break;
    }

    uint8_t *eapol_pdu_frame = sec_prot_lib_message_build(prot->sec_keys->ptk, NULL, 0, &eapol_pdu, eapol_pdu_size, prot->header_size);

    if (eapol_pdu_frame == NULL) {
        return -1;
    }

    tr_info("GKH: send Message 2");

    if (prot->send(prot, eapol_pdu_frame, eapol_pdu_size + prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static void supp_gkh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    sec_prot_timer_timeout_handle(prot, &data->common, NULL, ticks);
}

static void supp_gkh_sec_prot_state_machine(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);

    // GKH supplicant state machine
    switch (sec_prot_state_get(&data->common)) {
        case GKH_STATE_INIT:
            tr_info("GKH init");
            sec_prot_state_set(prot, &data->common, GKH_STATE_MESSAGE_1);
            prot->timer_start(prot);
            break;

        // Wait GKH message 1 (starts handshake on supplicant)
        case GKH_STATE_MESSAGE_1:
            if (supp_gkh_sec_prot_mic_validate(prot) < 0) {
                return;
            }

            if (supp_gkh_kde_handle(prot) < 0) {
                return;
            }

            // Set default timeout for the total maximum length of the negotiation
            sec_prot_default_timeout_set(&data->common);

            supp_gkh_sec_prot_security_replay_counter_update(prot);

            tr_info("GKH start");

            // Send KMP-CREATE.indication
            prot->create_ind(prot);
            sec_prot_state_set(prot, &data->common, GKH_STATE_CREATE_RESP);
            break;

        // Wait KMP-CREATE.response
        case GKH_STATE_CREATE_RESP:
            if (sec_prot_result_ok_check(&data->common)) {
                // Send GKH message 2
                supp_gkh_sec_prot_message_send(prot, GKH_MESSAGE_2);
                sec_prot_state_set(prot, &data->common, GKH_STATE_FINISH);
            } else {
                // Ready to be deleted
                sec_prot_state_set(prot, &data->common, GKH_STATE_FINISHED);
            }
            break;

        case GKH_STATE_FINISH:
            tr_info("GKH finish");

            // KMP-FINISHED.indication,
            prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);
            sec_prot_state_set(prot, &data->common, GKH_STATE_FINISHED);
            break;

        case GKH_STATE_FINISHED:
            tr_info("GKH finished");
            prot->timer_stop(prot);
            prot->finished(prot);
            break;

        default:
            break;
    }
}

static int8_t supp_gkh_sec_prot_mic_validate(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    return sec_prot_lib_mic_validate(prot->sec_keys->ptk, data->recv_eapol_pdu.msg.key.key_mic, data->recv_pdu, data->recv_size);
}

static void supp_gkh_sec_prot_security_replay_counter_update(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    sec_prot_keys_pmk_replay_cnt_set(prot->sec_keys, data->recv_eapol_pdu.msg.key.replay_counter);
}

static int8_t supp_gkh_kde_handle(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);

    uint16_t kde_len;
    uint8_t *kde = sec_prot_lib_message_handle(prot->sec_keys->ptk, &kde_len, &data->recv_eapol_pdu);
    if (!kde) {
        return -1;
    }

    // If a valid new GTK value present, insert it
    int8_t ret = sec_prot_lib_gtk_read(kde, kde_len, prot->sec_keys);

    ns_dyn_mem_free(kde);

    return ret;
}

#endif /* HAVE_WS */

