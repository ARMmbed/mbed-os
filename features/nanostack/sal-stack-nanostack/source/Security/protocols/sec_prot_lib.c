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
#include <randLIB.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Service_Libs/Trickle/trickle.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Security/eapol/kde_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"
#include "Service_Libs/ieee_802_11/ieee_802_11.h"
#include "Service_Libs/hmac/hmac_sha1.h"
#include "Service_Libs/nist_aes_kw/nist_aes_kw.h"

#ifdef HAVE_WS

#define TRACE_GROUP "secl"

void sec_prot_init(sec_prot_common_t *data)
{
    data->state = SEC_STATE_INIT;
    data->result = SEC_RESULT_OK;
    data->ticks = SEC_TOTAL_TIMEOUT;
    data->trickle_running = false;
}

void sec_prot_timer_timeout_handle(sec_prot_t *prot, sec_prot_common_t *data, const trickle_params_t *trickle_params, uint16_t ticks)
{
    if (data->trickle_running) {
        bool running = trickle_running(&data->trickle_timer, trickle_params);

        // Checks for trickle timer expiration */
        if (trickle_timer(&data->trickle_timer, trickle_params, ticks)) {
            sec_prot_result_set(data, SEC_RESULT_TIMEOUT);
            prot->state_machine(prot);
        }

        // Checks if maximum number of trickle timer expirations has happened
        if (running && !trickle_running(&data->trickle_timer, trickle_params)) {
            sec_prot_result_set(data, SEC_RESULT_TIMEOUT);
            sec_prot_state_set(prot, data, SEC_STATE_FINISH);
        }
    }

    if (data->ticks > ticks) {
        data->ticks -= ticks;
    } else {
        tr_debug("prot timeout");
        data->ticks = 0;
        sec_prot_result_set(data, SEC_RESULT_TIMEOUT);
        sec_prot_state_set(prot, data, SEC_STATE_FINISH);
    }
}

void sec_prot_timer_trickle_start(sec_prot_common_t *data, const trickle_params_t *trickle_params)
{
    trickle_start(&data->trickle_timer, trickle_params);
    data->trickle_running = true;
}

void sec_prot_timer_trickle_stop(sec_prot_common_t *data)
{
    trickle_stop(&data->trickle_timer);
    data->trickle_running = false;
}

void sec_prot_state_set(sec_prot_t *prot, sec_prot_common_t *data, uint8_t state)
{
    switch (state) {
        case SEC_STATE_FINISH:
            if (data->state == SEC_STATE_FINISHED) {
                // Already, do not update state;
            } else {
                data->state = SEC_STATE_FINISH;
            }
            data->trickle_running = false;
            data->ticks = SEC_FINISHED_TIMEOUT;

            // Go to SEC_STATE_FINISH or SEC_STATE_FINISHED
            prot->state_machine(prot);
            return;

        case SEC_STATE_FINISHED:
            // Wait for timeout
            data->trickle_running = false;
            data->ticks = SEC_FINISHED_TIMEOUT;
            // Clear result
            sec_prot_result_set(data, SEC_RESULT_OK);
            break;

        case SEC_STATE_INIT:
            data->state = SEC_STATE_INIT;
            prot->state_machine(prot);
            return;

        default:
            break;
    }

    data->state = state;
}

uint8_t sec_prot_state_get(sec_prot_common_t *data)
{
    return data->state;
}

void sec_prot_result_set(sec_prot_common_t *data, sec_prot_result_e result)
{
    data->result = result;
}

sec_prot_result_e sec_prot_result_get(sec_prot_common_t *data)
{
    return data->result;
}

bool sec_prot_result_timeout_check(sec_prot_common_t *data)
{
    if (data->result == SEC_RESULT_TIMEOUT) {
        data->result = SEC_RESULT_OK;
        return true;
    }
    return false;
}

bool sec_prot_result_ok_check(sec_prot_common_t *data)
{
    if (data->result == SEC_RESULT_OK) {
        return true;
    }
    return false;
}

/*
 * IEEE 802.11 advises using sequential nonces, but should this be
 * randlib?
 */
void sec_prot_lib_nonce_generate(uint8_t *nonce)
{
    // For now, use randlib
    randLIB_get_n_bytes_random(nonce, EAPOL_KEY_NONCE_LEN);

#if 0
    for (int i = 31; i >= 0; i--) {
        if (++nonce[i] != 0) {
            break;
        }
    }
#endif
}

/*
 * From IEEE 802.11 how to init nonce calculation by using non-secure random
 *
 * PRF-256(Random number, “Init Counter”, Local MAC Address || Time)
 */
void sec_prot_lib_nonce_init(uint8_t *nonce, uint8_t *eui64, uint64_t time)
{
    // For now, use randlib
    uint8_t random[EAPOL_KEY_NONCE_LEN];
    randLIB_get_n_bytes_random(random, EAPOL_KEY_NONCE_LEN);

    const uint8_t a_string_val[] = {"Init Counter"};
    const uint8_t a_string_val_len = sizeof(a_string_val) - 1;

    ieee_802_11_prf_t prf;

    uint16_t string_len = ieee_802_11_prf_setup(&prf, EAPOL_KEY_NONCE_LEN * 8, a_string_val_len, EUI64_LEN + EUI64_LEN);
    uint8_t string[string_len];

    uint8_t *a_string = ieee_802_11_prf_get_a_string(&prf, string);
    memcpy(a_string, a_string_val, a_string_val_len);

    uint8_t *b_string = ieee_802_11_prf_get_b_string(&prf, string);
    memcpy(b_string, eui64, EUI64_LEN);
    b_string += EUI64_LEN;
    memcpy(b_string, &time, sizeof(uint64_t));

    uint16_t result_len = ieee_802_11_prf_starts(&prf, random, EAPOL_KEY_NONCE_LEN);

    ieee_802_11_prf_update(&prf, string);

    uint8_t result[result_len];

    ieee_802_11_prf_finish(&prf, result);

    memcpy(nonce, result, EAPOL_KEY_NONCE_LEN);
}

/*
 * PTK = PRF-384(PMK, “Pairwise key expansion”, Min(AUTH EUI-64, SUP EUI-64) ||
 *       Max(AUTH EUI-64, SUP EUI-64) || Min (Anonce, Snonce) || Max(Anonce, Snonce))
 *
 * PMK is 256 bits, PTK is 382 bits
 */
int8_t sec_prot_lib_ptk_calc(const uint8_t *pmk, const uint8_t *eui64_1, const uint8_t *eui64_2, const uint8_t *nonce1, const uint8_t *nonce2, uint8_t *ptk)
{
    const uint8_t a_string_val[] = {"Pairwise key expansion"};
    const uint8_t a_string_val_len = sizeof(a_string_val) - 1;

    const uint8_t *min_eui64 = eui64_1;
    const uint8_t *max_eui64 = eui64_2;
    if (memcmp(eui64_1, eui64_2, EUI64_LEN) > 0) {
        min_eui64 = eui64_2;
        max_eui64 = eui64_1;
    }

    const uint8_t *min_nonce = nonce1;
    const uint8_t *max_nonce = nonce2;
    if (memcmp(nonce1, nonce2, EAPOL_KEY_NONCE_LEN) > 0) {
        min_nonce = nonce2;
        max_nonce = nonce1;
    }

    ieee_802_11_prf_t prf;

    uint16_t string_len = ieee_802_11_prf_setup(&prf, 384, a_string_val_len, EUI64_LEN + EUI64_LEN + EAPOL_KEY_NONCE_LEN + EAPOL_KEY_NONCE_LEN);
    uint8_t string[string_len];

    uint8_t *a_string = ieee_802_11_prf_get_a_string(&prf, string);
    memcpy(a_string, a_string_val, a_string_val_len);

    uint8_t *b_string = ieee_802_11_prf_get_b_string(&prf, string);
    memcpy(b_string, min_eui64, EUI64_LEN);
    b_string += EUI64_LEN;
    memcpy(b_string, max_eui64, EUI64_LEN);
    b_string += EUI64_LEN;

    memcpy(b_string, min_nonce, EAPOL_KEY_NONCE_LEN);
    b_string += EAPOL_KEY_NONCE_LEN;
    memcpy(b_string, max_nonce, EAPOL_KEY_NONCE_LEN);

    uint16_t result_len = ieee_802_11_prf_starts(&prf, pmk, PMK_LEN);

    ieee_802_11_prf_update(&prf, string);

    uint8_t result[result_len];

    ieee_802_11_prf_finish(&prf, result);

    memcpy(ptk, result, PTK_LEN);

    return 0;
}

int8_t sec_prot_lib_pmkid_calc(const uint8_t *pmk, const uint8_t *auth_eui64, const uint8_t *supp_eui64, uint8_t *pmkid)
{
    const uint8_t pmk_string_val[] = {"PMK Name"};
    const uint8_t pmk_string_val_len = sizeof(pmk_string_val) - 1;

    const uint8_t data_len = pmk_string_val_len + EUI64_LEN + EUI64_LEN;
    uint8_t data[data_len];
    uint8_t *ptr = data;
    memcpy(ptr, pmk_string_val, pmk_string_val_len);
    ptr += pmk_string_val_len;
    memcpy(ptr, auth_eui64, EUI64_LEN);
    ptr += EUI64_LEN;
    memcpy(ptr, supp_eui64, EUI64_LEN);

    if (hmac_sha1_calc(pmk, PMK_LEN, data, data_len, pmkid) < 0) {
        return -1;
    }

    return 0;
}

int8_t sec_prot_lib_ptkid_calc(const uint8_t *ptk, const uint8_t *auth_eui64, const uint8_t *supp_eui64, uint8_t *ptkid)
{
    const uint8_t ptk_string_val[] = {"PTK Name"};
    const uint8_t ptk_string_val_len = sizeof(ptk_string_val) - 1;

    const uint8_t data_len = ptk_string_val_len + EUI64_LEN + EUI64_LEN;
    uint8_t data[data_len];
    uint8_t *ptr = data;
    memcpy(ptr, ptk_string_val, ptk_string_val_len);
    ptr += ptk_string_val_len;
    memcpy(ptr, auth_eui64, EUI64_LEN);
    ptr += EUI64_LEN;
    memcpy(ptr, supp_eui64, EUI64_LEN);

    if (hmac_sha1_calc(ptk, PTK_LEN, data, data_len, ptkid) < 0) {
        return -1;
    }

    return 0;
}

uint8_t *sec_prot_lib_message_build(uint8_t *ptk, uint8_t *kde, uint16_t kde_len, eapol_pdu_t *eapol_pdu, uint16_t eapol_pdu_size, uint8_t header_size)
{
    uint8_t *eapol_pdu_frame = ns_dyn_mem_temporary_alloc(header_size + eapol_pdu_size);

    if (!eapol_pdu_frame) {
        return NULL;
    }

    uint8_t *eapol_kde = eapol_write_pdu_frame(eapol_pdu_frame + header_size, eapol_pdu);

    if (kde) {
        if (eapol_pdu->msg.key.key_information.encrypted_key_data) {
            size_t output_len = kde_len;
            if (nist_aes_key_wrap(1, &ptk[KEK_INDEX], 128, kde, kde_len - 8, eapol_kde, &output_len) < 0 || output_len != kde_len) {
                ns_dyn_mem_free(eapol_pdu_frame);
                return NULL;
            }
        } else {
            memcpy(eapol_kde, kde, kde_len);
        }
    }

    if (eapol_pdu->msg.key.key_information.key_mic) {
        uint8_t mic[EAPOL_KEY_MIC_LEN];
        if (hmac_sha1_calc(ptk, KCK_LEN, eapol_pdu_frame + header_size, eapol_pdu_size, mic) < 0) {
            ns_dyn_mem_free(eapol_pdu_frame);
            return NULL;
        }
        eapol_write_key_packet_mic(eapol_pdu_frame + header_size, mic);
    }

    return eapol_pdu_frame;
}

uint8_t *sec_prot_lib_message_handle(uint8_t *ptk, uint16_t *kde_len, eapol_pdu_t *eapol_pdu)
{
    if (eapol_pdu->msg.key.key_data_length == 0 || eapol_pdu->msg.key.key_data == NULL) {
        return NULL;
    }

    uint8_t *key_data = eapol_pdu->msg.key.key_data;
    uint16_t key_data_len = eapol_pdu->msg.key.key_data_length;

    uint8_t *kde = ns_dyn_mem_temporary_alloc(key_data_len);
    *kde_len = key_data_len;

    if (eapol_pdu->msg.key.key_information.encrypted_key_data) {
        size_t output_len = eapol_pdu->msg.key.key_data_length;
        if (nist_aes_key_wrap(0, &ptk[KEK_INDEX], 128, key_data, key_data_len, kde, &output_len) < 0 || output_len != (size_t) key_data_len - 8) {
            ns_dyn_mem_free(kde);
            return NULL;
        }
        *kde_len = output_len;
    } else {
        memcpy(kde, key_data, *kde_len);
    }

    return kde;
}

int8_t sec_prot_lib_gtk_read(uint8_t *kde, uint16_t kde_len, sec_prot_gtk_keys_t *gtks)
{
    // If a valid new GTK value present, insert it
    int8_t gtk_set_index = -1;

    uint8_t key_id;
    uint8_t gtk[GTK_LEN];

    if (kde_gtk_read(kde, kde_len, &key_id, gtk) >= 0) {
        // A new GTK value
        if (sec_prot_keys_gtk_set(gtks, key_id, gtk) >= 0) {
            gtk_set_index = (int8_t) key_id; // Insert
        }

        uint32_t lifetime;
        if (kde_lifetime_read(kde, kde_len, &lifetime) >= 0) {
            sec_prot_keys_gtk_lifetime_set(gtks, key_id, lifetime);
        }
    }
    uint8_t gtkl;
    if (kde_gtkl_read(kde, kde_len, &gtkl) >= 0) {
        sec_prot_keys_gtkl_set(gtks, gtkl);
    } else {
        return -1;
    }

    // Sanity checks
    if (gtk_set_index >= 0) {
        if (!sec_prot_keys_gtk_is_live(gtks, gtk_set_index)) {
            gtk_set_index = -1;
        }
    }

    if (gtk_set_index >= 0) {
        sec_prot_keys_gtk_insert_index_set(gtks, gtk_set_index);
    }

    return 0;
}

int8_t sec_prot_lib_mic_validate(uint8_t *ptk, uint8_t *mic, uint8_t *pdu, uint8_t pdu_size)
{
    uint8_t recv_mic[EAPOL_KEY_MIC_LEN];
    memcpy(recv_mic, mic, EAPOL_KEY_MIC_LEN);

    eapol_write_key_packet_mic(pdu, 0);

    uint8_t calc_mic[EAPOL_KEY_MIC_LEN];
    if (hmac_sha1_calc(ptk, EAPOL_KEY_MIC_LEN, pdu, pdu_size, calc_mic) < 0) {
        return -1;
    }
    if (memcmp(recv_mic, calc_mic, EAPOL_KEY_MIC_LEN) != 0) {
        return -1;
    }
    return 0;
}

uint8_t sec_prot_lib_key_mask_get(eapol_pdu_t *eapol_pdu)
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

int8_t sec_prot_lib_pmkid_generate(sec_prot_t *prot, uint8_t *pmkid, bool is_auth)
{
    uint8_t local_eui64[8];
    uint8_t remote_eui64[8];
    prot->addr_get(prot, local_eui64, remote_eui64);
    uint8_t *pmk = sec_prot_keys_pmk_get(prot->sec_keys);
    if (!pmk) {
        return -1;
    }

    if (is_auth) {
        return sec_prot_lib_pmkid_calc(pmk, local_eui64, remote_eui64, pmkid);
    } else {
        return sec_prot_lib_pmkid_calc(pmk, remote_eui64, local_eui64, pmkid);
    }
}

int8_t sec_prot_lib_ptkid_generate(sec_prot_t *prot, uint8_t *ptkid, bool is_auth)
{
    uint8_t local_eui64[8];
    uint8_t remote_eui64[8];
    prot->addr_get(prot, local_eui64, remote_eui64);
    uint8_t *ptk = sec_prot_keys_pmk_get(prot->sec_keys);
    if (!ptk) {
        return -1;
    }

    if (is_auth) {
        return sec_prot_lib_ptkid_calc(ptk, local_eui64, remote_eui64, ptkid);
    } else {
        return sec_prot_lib_ptkid_calc(ptk, remote_eui64, local_eui64, ptkid);
    }
}


#endif /* HAVE_WS */
