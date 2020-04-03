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
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Security/eapol/kde_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"
#include "Security/protocols/gkh_sec_prot/auth_gkh_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "agkh"

typedef enum {
    GKH_STATE_INIT = SEC_STATE_INIT,
    GKH_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    GKH_STATE_MESSAGE_2 = SEC_STATE_FIRST,
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

static uint16_t auth_gkh_sec_prot_size(void);
static int8_t auth_gkh_sec_prot_init(sec_prot_t *prot);

static void auth_gkh_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static void auth_gkh_sec_prot_delete(sec_prot_t *prot);
static int8_t auth_gkh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static gkh_sec_prot_msg_e auth_gkh_sec_prot_message_get(eapol_pdu_t *eapol_pdu, sec_prot_keys_t *sec_keys);
static void auth_gkh_sec_prot_state_machine(sec_prot_t *prot);

static int8_t auth_gkh_sec_prot_message_send(sec_prot_t *prot, gkh_sec_prot_msg_e msg);
static void auth_gkh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);
static int8_t auth_gkh_sec_prot_mic_validate(sec_prot_t *prot);

#define gkh_sec_prot_get(prot) (gkh_sec_prot_int_t *) &prot->data

int8_t auth_gkh_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_11_GKH, auth_gkh_sec_prot_size, auth_gkh_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t auth_gkh_sec_prot_size(void)
{
    return sizeof(gkh_sec_prot_int_t);
}

static int8_t auth_gkh_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = auth_gkh_sec_prot_create_request;
    prot->create_resp = 0;
    prot->receive = auth_gkh_sec_prot_receive;
    prot->delete = auth_gkh_sec_prot_delete;
    prot->state_machine = auth_gkh_sec_prot_state_machine;
    prot->timer_timeout = auth_gkh_sec_prot_timer_timeout;

    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, GKH_STATE_INIT);

    return 0;
}

static void auth_gkh_sec_prot_delete(sec_prot_t *prot)
{
    // No op at the moment
    (void) prot;
}

static void auth_gkh_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    prot->sec_keys = sec_keys;

    // Call state machine
    prot->state_machine_call(prot);
}

static int8_t auth_gkh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Get message
        if (auth_gkh_sec_prot_message_get(&data->recv_eapol_pdu, prot->sec_keys) != GKH_MESSAGE_UNKNOWN) {
            tr_info("GKH: recv Message 2, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Call state machine
            data->recv_pdu = pdu;
            data->recv_size = size;
            prot->state_machine(prot);
        } else {
            tr_error("GKH: recv error, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
        }
        ret_val = 0;
    } else {
        tr_error("GKH: recv error, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
    }

    memset(&data->recv_eapol_pdu, 0, sizeof(eapol_pdu_t));
    data->recv_pdu = 0;
    data->recv_size = 0;

    return ret_val;
}

static gkh_sec_prot_msg_e auth_gkh_sec_prot_message_get(eapol_pdu_t *eapol_pdu, sec_prot_keys_t *sec_keys)
{
    gkh_sec_prot_msg_e msg = GKH_MESSAGE_UNKNOWN;

    if (eapol_pdu->msg.key.key_information.pairwise_key) {
        // This is mismatch between KMP ID indicating 802.11/GKH and key type
        return GKH_MESSAGE_UNKNOWN;
    }

    uint8_t key_mask = eapol_pdu_key_mask_get(eapol_pdu);

    switch (key_mask) {
        case KEY_INFO_KEY_MIC | KEY_INFO_SECURED_KEY_FRAME:
            // Only accept message from supplicant with expected replay counter
            if (eapol_pdu->msg.key.replay_counter == sec_prot_keys_pmk_replay_cnt_get(sec_keys)) {
                msg = GKH_MESSAGE_2;
            }
            break;
        default:
            break;
    }

    return msg;
}

static int8_t auth_gkh_sec_prot_message_send(sec_prot_t *prot, gkh_sec_prot_msg_e msg)
{
    uint16_t kde_len = 0;

    switch (msg) {
        case GKH_MESSAGE_1:
            kde_len = KDE_GTK_LEN + KDE_LIFETIME_LEN + KDE_GTKL_LEN;
            kde_len = kde_len + 8; // One 64 bit block for AES Key Wrap
            kde_len = kde_padded_length_calc(kde_len);
            break;
        default:
            break;
    }

    uint8_t *kde_start = ns_dyn_mem_temporary_alloc(kde_len);

    if (!kde_start) {
        return -1;
    }

    uint8_t *kde_end = kde_start;

    switch (msg) {
        case GKH_MESSAGE_1: {
            uint8_t gtk_index;
            uint8_t *gtk = sec_prot_keys_get_gtk_to_insert(prot->sec_keys, &gtk_index);
            if (gtk) {
                kde_end = kde_gtk_write(kde_end, gtk_index, gtk);

                uint32_t gtk_lifetime = sec_prot_keys_gtk_lifetime_get(prot->sec_keys->gtks, gtk_index);
                kde_end = kde_lifetime_write(kde_end, gtk_lifetime);
            }
            uint8_t gtkl = sec_prot_keys_fresh_gtkl_get(prot->sec_keys->gtks);
            kde_end = kde_gtkl_write(kde_end, gtkl);
            kde_padding_write(kde_end, kde_start + kde_len);
        }
        break;
        default:
            break;
    }

    eapol_pdu_t eapol_pdu;
    uint16_t eapol_pdu_size = eapol_pdu_key_frame_init(&eapol_pdu, kde_len, NULL);

    switch (msg) {
        case GKH_MESSAGE_1:
            sec_prot_keys_pmk_replay_cnt_increment(prot->sec_keys);
            eapol_pdu.msg.key.replay_counter = sec_prot_keys_pmk_replay_cnt_get(prot->sec_keys);
            eapol_pdu.msg.key.key_information.key_ack = true;
            eapol_pdu.msg.key.key_information.key_mic = true;
            eapol_pdu.msg.key.key_information.secured_key_frame = true;
            eapol_pdu.msg.key.key_information.encrypted_key_data = true;
            eapol_pdu.msg.key.key_length = 0;
            break;
        default:
            break;
    }

    uint8_t *eapol_pdu_frame = sec_prot_lib_message_build(prot->sec_keys->ptk, kde_start, kde_len, &eapol_pdu, eapol_pdu_size, prot->header_size);

    ns_dyn_mem_free(kde_start);

    if (eapol_pdu_frame == NULL) {
        return -1;
    }

    tr_info("GKH: send Message 1, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    if (prot->send(prot, eapol_pdu_frame, eapol_pdu_size + prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static void auth_gkh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    sec_prot_timer_timeout_handle(prot, &data->common, &prot->cfg->sec_prot_trickle_params, ticks);
}

static void auth_gkh_sec_prot_state_machine(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);

    // GKH authenticator state machine
    switch (sec_prot_state_get(&data->common)) {
        case GKH_STATE_INIT:
            tr_info("GKH init");
            sec_prot_state_set(prot, &data->common, GKH_STATE_CREATE_REQ);
            prot->timer_start(prot);
            break;

        // Wait KMP-CREATE.request
        case GKH_STATE_CREATE_REQ:
            tr_info("GKH start, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Set default timeout for the total maximum length of the negotiation
            sec_prot_default_timeout_set(&data->common);

            // KMP-CREATE.confirm
            prot->create_conf(prot, SEC_RESULT_OK);

            // Sends 4WH Message 1
            auth_gkh_sec_prot_message_send(prot, GKH_MESSAGE_1);

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &prot->cfg->sec_prot_trickle_params);

            sec_prot_state_set(prot, &data->common, GKH_STATE_MESSAGE_2);

            // Store the hash for to-be installed GTK as used for the PTK
            sec_prot_keys_ptk_installed_gtk_hash_set(prot->sec_keys);
            break;

        // Wait GKH message 2
        case GKH_STATE_MESSAGE_2:

            if (sec_prot_result_timeout_check(&data->common)) {
                // Re-sends GKH Message 1
                auth_gkh_sec_prot_message_send(prot, GKH_MESSAGE_1);
            } else {
                if (auth_gkh_sec_prot_mic_validate(prot) < 0) {
                    return;
                }
                // Set inserted GTK valid
                sec_prot_keys_gtkl_from_gtk_insert_index_set(prot->sec_keys);

                sec_prot_state_set(prot, &data->common, GKH_STATE_FINISH);
            }
            break;

        case GKH_STATE_FINISH:
            tr_info("GKH finish, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // KMP-FINISHED.indication,
            prot->finished_ind(prot, sec_prot_result_get(&data->common), 0);

            sec_prot_state_set(prot, &data->common, GKH_STATE_FINISHED);
            break;

        case GKH_STATE_FINISHED: {
            uint8_t *remote_eui_64 = sec_prot_remote_eui_64_addr_get(prot);
            tr_info("GKH finished, eui-64: %s", remote_eui_64 ? trace_array(sec_prot_remote_eui_64_addr_get(prot), 8) : "not set");
            prot->timer_stop(prot);
            prot->finished(prot);
            break;
        }

        default:
            break;
    }
}

static int8_t auth_gkh_sec_prot_mic_validate(sec_prot_t *prot)
{
    gkh_sec_prot_int_t *data = gkh_sec_prot_get(prot);
    return sec_prot_lib_mic_validate(prot->sec_keys->ptk, data->recv_eapol_pdu.msg.key.key_mic, data->recv_pdu, data->recv_size);
}

#endif /* HAVE_WS */

