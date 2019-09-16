/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include "Security/eapol/kde_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"
#include "Security/protocols/fwh_sec_prot/supp_fwh_sec_prot.h"
#include "Service_Libs/hmac/hmac_sha1.h"
#include "Service_Libs/nist_aes_kw/nist_aes_kw.h"

#ifdef HAVE_WS

#define TRACE_GROUP "sfwh"

typedef enum {
    FWH_STATE_INIT = SEC_STATE_INIT,
    FWH_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    FWH_STATE_CREATE_IND = SEC_STATE_CREATE_IND,
    FWH_STATE_MESSAGE_1 = SEC_STATE_FIRST,
    FWH_STATE_MESSAGE_3,
    FWH_STATE_MESSAGE_3_RETRY_WAIT,
    FWH_STATE_CREATE_RESP_SUPP_RETRY,
    FWH_STATE_FINISH = SEC_STATE_FINISH,
    FWH_STATE_FINISHED = SEC_STATE_FINISHED
} fwh_sec_prot_state_e;

typedef enum {
    FWH_MESSAGE_UNKNOWN = 0,
    FWH_MESSAGE_1,
    FWH_MESSAGE_2,
    FWH_MESSAGE_3,
    FWH_MESSAGE_4
} fwh_sec_prot_msg_e;

#define KEY_INFO_INSTALL              0x01
#define KEY_INFO_KEY_ACK              0x02
#define KEY_INFO_KEY_MIC              0x04
#define KEY_INFO_SECURED_KEY_FRAME    0x08

typedef struct {
    sec_prot_common_t             common;                      /**< Common data */
    eapol_pdu_t                   recv_eapol_pdu;              /**< Received EAPOL PDU */
    fwh_sec_prot_msg_e            recv_msg;                    /**< Received message */
    uint8_t                       snonce[EAPOL_KEY_NONCE_LEN]; /**< Supplicant nonce */
    uint8_t                       anonce[EAPOL_KEY_NONCE_LEN]; /**< Authenticator nonce */
    uint8_t                       new_ptk[PTK_LEN];            /**< PTK (384 bits) */
    uint8_t                       remote_eui64[8];             /**< Remote EUI-64 used to calculate PTK */
    void                          *recv_pdu;                   /**< received pdu */
    uint16_t                      recv_size;                   /**< received pdu size */
    uint64_t                      recv_replay_cnt;             /**< received replay counter */
    bool                          msg3_received : 1;           /**< Valid Message 3 has been received */
    bool                          msg3_retry_wait : 1;         /**< Waiting for Message 3 retry */
    bool                          recv_replay_cnt_set : 1;     /**< received replay counter set */
} fwh_sec_prot_int_t;

#define FWH_RETRY_TIMEOUT_SMALL 300*10 // retry timeout for small network
#define FWH_RETRY_TIMEOUT_LARGE 720*10 // retry timeout for large network

static uint16_t retry_timeout = FWH_RETRY_TIMEOUT_SMALL;

static uint16_t supp_fwh_sec_prot_size(void);
static int8_t supp_fwh_sec_prot_init(sec_prot_t *prot);

static void supp_fwh_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);
static void supp_fwh_sec_prot_delete(sec_prot_t *prot);
static int8_t supp_fwh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static fwh_sec_prot_msg_e supp_fwh_sec_prot_message_get(sec_prot_t *prot, eapol_pdu_t *eapol_pdu);
static void supp_fwh_sec_prot_state_machine(sec_prot_t *prot);

static int8_t supp_fwh_sec_prot_message_send(sec_prot_t *prot, fwh_sec_prot_msg_e msg);
static void supp_fwh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);

static int8_t supp_fwh_sec_prot_ptk_generate(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static int8_t supp_fwh_sec_prot_mic_validate(sec_prot_t *prot);

static void supp_fwh_sec_prot_recv_replay_counter_store(sec_prot_t *prot);
static bool supp_fwh_sec_prot_recv_replay_cnt_compare(uint64_t received_counter, sec_prot_t *prot);
static void supp_fwh_sec_prot_anonce_store(sec_prot_t *prot);
static int8_t supp_fwh_sec_prot_anonce_validate(sec_prot_t *prot);
static void supp_fwh_sec_prot_security_replay_counter_update(sec_prot_t *prot);

static int8_t supp_fwh_kde_handle(sec_prot_t *prot);

#define fwh_sec_prot_get(prot) (fwh_sec_prot_int_t *) &prot->data

int8_t supp_fwh_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_11_4WH, supp_fwh_sec_prot_size, supp_fwh_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}
int8_t supp_fwh_sec_prot_timing_adjust(uint8_t timing)
{
    if (timing < 16) {
        retry_timeout = FWH_RETRY_TIMEOUT_SMALL;
    } else {
        retry_timeout = FWH_RETRY_TIMEOUT_LARGE;
    }
    return 0;
}

static uint16_t supp_fwh_sec_prot_size(void)
{
    return sizeof(fwh_sec_prot_int_t);
}

static int8_t supp_fwh_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = 0;
    prot->create_resp = supp_fwh_sec_prot_create_response;
    prot->receive = supp_fwh_sec_prot_receive;
    prot->delete = supp_fwh_sec_prot_delete;
    prot->state_machine = supp_fwh_sec_prot_state_machine;
    prot->timer_timeout = supp_fwh_sec_prot_timer_timeout;

    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, FWH_STATE_INIT);

    data->common.ticks = retry_timeout;
    data->msg3_received = false;
    data->msg3_retry_wait = false;
    data->recv_replay_cnt = 0;
    data->recv_replay_cnt_set = false;

    uint8_t eui64[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    sec_prot_lib_nonce_init(data->snonce, eui64, 1000);

    return 0;
}

static void supp_fwh_sec_prot_delete(sec_prot_t *prot)
{
    // No op at the moment
    (void) prot;
}

static void supp_fwh_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);

    // Call state machine
    sec_prot_result_set(&data->common, result);
    prot->state_machine_call(prot);
}

static int8_t supp_fwh_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    int8_t ret_val = -1;

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &data->recv_eapol_pdu)) {
        // Get message
        data->recv_msg = supp_fwh_sec_prot_message_get(prot, &data->recv_eapol_pdu);
        if (data->recv_msg != FWH_MESSAGE_UNKNOWN) {
            tr_info("4WH: recv %s", data->recv_msg == FWH_MESSAGE_1 ? "Message 1" : "Message 3");

            // Call state machine
            data->recv_pdu = pdu;
            data->recv_size = size;
            prot->state_machine(prot);
        } else {
            tr_error("4WH: recv error");
        }
        ret_val = 0;
    } else {
        tr_error("4WH: recv error");
    }

    memset(&data->recv_eapol_pdu, 0, sizeof(eapol_pdu_t));
    data->recv_msg = FWH_MESSAGE_UNKNOWN;
    data->recv_pdu = 0;
    data->recv_size = 0;

    return ret_val;
}

static fwh_sec_prot_msg_e supp_fwh_sec_prot_message_get(sec_prot_t *prot, eapol_pdu_t *eapol_pdu)
{
    fwh_sec_prot_msg_e msg = FWH_MESSAGE_UNKNOWN;

    if (!eapol_pdu->msg.key.key_information.pairwise_key) {
        // This is mismatch between KMP ID indicating 802.11/4WH and key type
        return FWH_MESSAGE_UNKNOWN;
    }

    uint8_t key_mask = eapol_pdu_key_mask_get(eapol_pdu);

    switch (key_mask) {
        // Message 1
        case KEY_INFO_KEY_ACK:
            /* Must have valid replay counter, both larger for PMK and larger that is used on
             * the four way handshake session (note: PMK replay counter is not updated for Message 1
             * but session specific counter is)
             */
            if (sec_prot_keys_pmk_replay_cnt_compare(eapol_pdu->msg.key.replay_counter, prot->sec_keys) &&
                    supp_fwh_sec_prot_recv_replay_cnt_compare(eapol_pdu->msg.key.replay_counter, prot)) {
                msg = FWH_MESSAGE_1;
            } else {
                tr_error("4WH: invalid replay counter %"PRId64, eapol_pdu->msg.key.replay_counter);
            }
            break;
        // Message 3
        case KEY_INFO_INSTALL | KEY_INFO_KEY_ACK | KEY_INFO_KEY_MIC | KEY_INFO_SECURED_KEY_FRAME:
            // Must have valid replay counter
            if (sec_prot_keys_pmk_replay_cnt_compare(eapol_pdu->msg.key.replay_counter, prot->sec_keys)) {
                if (eapol_pdu->msg.key.key_information.encrypted_key_data) {
                    // This should include the GTK KDE, Lifetime KDE and GTKL KDE.
                    // At least some of them should be present
                    msg = FWH_MESSAGE_3;
                }
            } else {
                tr_error("4WH: invalid replay counter %"PRId64, eapol_pdu->msg.key.replay_counter);
            }
            break;
        default:
            break;
    }

    return msg;
}

static int8_t supp_fwh_sec_prot_message_send(sec_prot_t *prot, fwh_sec_prot_msg_e msg)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);

    eapol_pdu_t eapol_pdu;
    uint16_t eapol_pdu_size = eapol_pdu_key_frame_init(&eapol_pdu, 0, NULL);

    eapol_pdu.msg.key.key_information.pairwise_key = true;

    switch (msg) {
        case FWH_MESSAGE_2:
            eapol_pdu.msg.key.replay_counter = data->recv_replay_cnt;
            eapol_pdu.msg.key.key_information.key_mic = true;
            eapol_pdu.msg.key.key_length = 0;
            eapol_pdu.msg.key.key_nonce = data->snonce;
            break;
        case FWH_MESSAGE_4:
            eapol_pdu.msg.key.replay_counter = data->recv_replay_cnt;
            eapol_pdu.msg.key.key_information.key_mic = true;
            eapol_pdu.msg.key.key_information.secured_key_frame = true;
            eapol_pdu.msg.key.key_length = 0;
            break;
        default:
            break;
    }

    uint8_t *eapol_pdu_frame = sec_prot_lib_message_build(data->new_ptk, 0, 0, &eapol_pdu, eapol_pdu_size, prot->header_size);

    if (eapol_pdu_frame == NULL) {
        return -1;
    }

    tr_info("4WH: send %s", msg == FWH_MESSAGE_2 ? "Message 2" : "Message 4");

    if (prot->send(prot, eapol_pdu_frame, eapol_pdu_size + prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static void supp_fwh_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    sec_prot_timer_timeout_handle(prot, &data->common, NULL, ticks);
}

static void supp_fwh_sec_prot_state_machine(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);

    // 4WH supplicant state machine
    switch (sec_prot_state_get(&data->common)) {
        case FWH_STATE_INIT:
            prot->timer_start(prot);
            sec_prot_state_set(prot, &data->common, FWH_STATE_MESSAGE_1);
            break;

        // Wait 4WH message 1 (starts handshake on supplicant)
        case FWH_STATE_MESSAGE_1:
            if (data->recv_msg != FWH_MESSAGE_1) {
                return;
            }

            // PMKID must be valid
            if (supp_fwh_kde_handle(prot) < 0) {
                return;
            }

            tr_info("4WH: start");

            // Store authenticator nonce for check when 4WH Message 3 is received
            supp_fwh_sec_prot_anonce_store(prot);
            sec_prot_lib_nonce_generate(data->snonce);
            if (supp_fwh_sec_prot_ptk_generate(prot, prot->sec_keys) < 0) {
                return;
            }

            supp_fwh_sec_prot_recv_replay_counter_store(prot);

            // Send KMP-CREATE.indication
            prot->create_ind(prot);
            sec_prot_state_set(prot, &data->common, FWH_STATE_CREATE_RESP);
            break;

        // Wait KMP-CREATE.response
        case FWH_STATE_CREATE_RESP:
            if (sec_prot_result_ok_check(&data->common)) {
                // Send 4WH message 2
                supp_fwh_sec_prot_message_send(prot, FWH_MESSAGE_2);
                data->common.ticks = retry_timeout;
                sec_prot_state_set(prot, &data->common, FWH_STATE_MESSAGE_3);
            } else {
                // Ready to be deleted
                sec_prot_state_set(prot, &data->common, FWH_STATE_FINISHED);
            }
            break;

        // Wait 4WH message 3 (message 2 has been sent)
        case FWH_STATE_MESSAGE_3:
            if (data->recv_msg == FWH_MESSAGE_1) {

                // PMKID must be valid
                if (supp_fwh_kde_handle(prot) < 0) {
                    return;
                }

                // Store authenticator nonce for check when 4WH Message 3 is received
                supp_fwh_sec_prot_anonce_store(prot);
                sec_prot_lib_nonce_generate(data->snonce);
                if (supp_fwh_sec_prot_ptk_generate(prot, prot->sec_keys) < 0) {
                    return;
                }

                supp_fwh_sec_prot_recv_replay_counter_store(prot);

                // Send 4WH message 2
                supp_fwh_sec_prot_message_send(prot, FWH_MESSAGE_2);
                data->common.ticks = retry_timeout;
                return;
            } else if (data->recv_msg != FWH_MESSAGE_3) {
                return;
            }

            // MIC must be valid
            if (supp_fwh_sec_prot_mic_validate(prot) < 0) {
                return;
            }

            // Nonce must match to 4WH Message 1
            if (supp_fwh_sec_prot_anonce_validate(prot) < 0) {
                return;
            }

            // Must have at least GTKL
            if (supp_fwh_kde_handle(prot) < 0) {
                return;
            }

            supp_fwh_sec_prot_recv_replay_counter_store(prot);
            supp_fwh_sec_prot_security_replay_counter_update(prot);
            data->msg3_received = true;

            // Sends 4WH Message 4
            supp_fwh_sec_prot_message_send(prot, FWH_MESSAGE_4);
            data->common.ticks = retry_timeout;
            sec_prot_state_set(prot, &data->common, FWH_STATE_FINISH);
            break;

        case FWH_STATE_FINISH:
            if (data->msg3_retry_wait) {
                tr_info("4WH: Message 3 retry timeout");
                sec_prot_state_set(prot, &data->common, FWH_STATE_FINISHED);
                return;
            }

            // If Message 3 has been received updates key data and waits for Message 3 retry
            if (data->msg3_received) {
                data->msg3_retry_wait = true;

                tr_info("4WH: finish, wait Message 3 retry");

                sec_prot_keys_ptk_write(prot->sec_keys, data->new_ptk);
                sec_prot_keys_ptk_eui_64_write(prot->sec_keys, data->remote_eui64);

                data->common.ticks = 60 * 10; // 60 seconds
                // KMP-FINISHED.indication
                prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);
                sec_prot_state_set(prot, &data->common, FWH_STATE_MESSAGE_3_RETRY_WAIT);
            } else {
                tr_info("4WH: finish");
                // KMP-FINISHED.indication
                prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);
                sec_prot_state_set(prot, &data->common, FWH_STATE_FINISHED);
            }
            break;

        case FWH_STATE_MESSAGE_3_RETRY_WAIT:
            if (sec_prot_result_timeout_check(&data->common)) {
                tr_info("4WH: Message 3 retry timeout");
                sec_prot_state_set(prot, &data->common, FWH_STATE_FINISHED);
            } else {
                if (data->recv_msg != FWH_MESSAGE_3) {
                    return;
                }

                // MIC must be valid
                if (supp_fwh_sec_prot_mic_validate(prot) < 0) {
                    return;
                }

                // Nonce must match to 4WH Message 1
                if (supp_fwh_sec_prot_anonce_validate(prot) < 0) {
                    return;
                }

                // Must have at least GTKL
                if (supp_fwh_kde_handle(prot) < 0) {
                    return;
                }

                supp_fwh_sec_prot_recv_replay_counter_store(prot);
                supp_fwh_sec_prot_security_replay_counter_update(prot);

                tr_info("4WH: send Message 4 again");

                supp_fwh_sec_prot_message_send(prot, FWH_MESSAGE_4);
            }
            break;

        case FWH_STATE_FINISHED:
            prot->timer_stop(prot);
            prot->finished(prot);
            break;

        default:
            break;
    }
}

static int8_t supp_fwh_sec_prot_ptk_generate(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);

    uint8_t local_eui64[8];
    prot->addr_get(prot, local_eui64, data->remote_eui64);

    uint8_t *remote_nonce = data->recv_eapol_pdu.msg.key.key_nonce;
    if (!remote_nonce) {
        tr_error("No ANonce");
        return -1;
    }

    uint8_t *pmk = sec_prot_keys_pmk_get(sec_keys);
    sec_prot_lib_ptk_calc(pmk, local_eui64, data->remote_eui64, data->snonce, remote_nonce, data->new_ptk);

    return 0;
}

static int8_t supp_fwh_sec_prot_mic_validate(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    return sec_prot_lib_mic_validate(data->new_ptk, data->recv_eapol_pdu.msg.key.key_mic, data->recv_pdu, data->recv_size);
}

static void supp_fwh_sec_prot_recv_replay_counter_store(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    data->recv_replay_cnt = data->recv_eapol_pdu.msg.key.replay_counter;
    data->recv_replay_cnt_set = true;
}

static bool supp_fwh_sec_prot_recv_replay_cnt_compare(uint64_t received_counter, sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    // If previous value is set must be greater
    if (data->recv_replay_cnt_set && received_counter > data->recv_replay_cnt) {
        return true;
    } else if (!data->recv_replay_cnt_set && received_counter >= data->recv_replay_cnt) {
        // Otherwise allows also same value e.g. zero
        return true;
    }

    return false;
}

static void supp_fwh_sec_prot_anonce_store(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    memcpy(data->anonce, data->recv_eapol_pdu.msg.key.key_nonce, EAPOL_KEY_NONCE_LEN);
}

static int8_t supp_fwh_sec_prot_anonce_validate(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    if (memcmp(data->anonce, data->recv_eapol_pdu.msg.key.key_nonce, EAPOL_KEY_NONCE_LEN) != 0) {
        tr_error("ANonce invalid");
        return -1;
    }
    return 0;
}

static void supp_fwh_sec_prot_security_replay_counter_update(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);
    sec_prot_keys_pmk_replay_cnt_set(prot->sec_keys, data->recv_eapol_pdu.msg.key.replay_counter);
}

static int8_t supp_fwh_kde_handle(sec_prot_t *prot)
{
    fwh_sec_prot_int_t *data = fwh_sec_prot_get(prot);

    uint16_t kde_len;
    uint8_t *kde = sec_prot_lib_message_handle(data->new_ptk, &kde_len, &data->recv_eapol_pdu);
    if (!kde) {
        return -1;
    }

    switch (data->recv_msg) {
        case FWH_MESSAGE_1: {
            uint8_t recv_pmkid[PMKID_LEN];
            uint8_t calc_pmkid[PMKID_LEN];
            if (kde_pmkid_read(kde, kde_len, recv_pmkid) < 0) {
                goto error;
            }
            if (sec_prot_lib_pmkid_generate(prot, calc_pmkid, false) < 0) {
                goto error;
            }
            if (memcmp(recv_pmkid, calc_pmkid, PMKID_LEN) != 0) {
                goto error;
            }
        }
        break;

        case FWH_MESSAGE_3:
            // If a valid new GTK value present, insert it
            if (sec_prot_lib_gtk_read(kde, kde_len, prot->sec_keys) < 0) {
                goto error;
            }
            break;

        default:
            break;
    }

    ns_dyn_mem_free(kde);
    return 0;

error:
    tr_error("Invalid KDEs");
    ns_dyn_mem_free(kde);
    return -1;
}

#endif /* HAVE_WS */
