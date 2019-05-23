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
#include "Security/protocols/key_sec_prot/key_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "ksep"

typedef enum {
    KEY_INIT = 0,
    KEY_CREATE_REQ,
    KEY_CREATE_RESP,
    KEY_FINISH,
} key_sec_prot_state_e;

typedef struct {
    key_sec_prot_state_e           state;        /**< Protocol state machine state */
    sec_prot_result_e              result;       /**< Result for ongoing negotiation */
} key_sec_prot_int_t;

static uint16_t key_sec_prot_size(void);
static int8_t key_sec_prot_init(sec_prot_t *prot);

static void key_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static void key_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);
static void key_sec_prot_delete(sec_prot_t *prot);
static int8_t key_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static void key_sec_prot_state_machine(sec_prot_t *prot);

#define key_sec_prot_get(prot) (key_sec_prot_int_t *) &prot->data

int8_t key_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_1X_MKA_KEY, key_sec_prot_size, key_sec_prot_init) < 0) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, IEEE_802_11_GKH_KEY, key_sec_prot_size, key_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t key_sec_prot_size(void)
{
    return sizeof(key_sec_prot_int_t);
}

static int8_t key_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = key_sec_prot_create_request;
    prot->create_resp = key_sec_prot_create_response;

    prot->receive = key_sec_prot_receive;
    prot->delete = key_sec_prot_delete;
    prot->state_machine = key_sec_prot_state_machine;

    key_sec_prot_int_t *data = key_sec_prot_get(prot);
    data->state = KEY_INIT;
    data->result = SEC_RESULT_OK;

    return 0;
}

static void key_sec_prot_delete(sec_prot_t *prot)
{
    // No op at the moment
    (void) prot;
}

static void key_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    key_sec_prot_int_t *data = key_sec_prot_get(prot);

    uint16_t kde_len = KDE_GTKL_LEN;

    uint8_t *pmk = sec_prot_keys_pmk_get(sec_keys);
    uint8_t pmkid[PMKID_LEN];
    if (pmk) {
        if (sec_prot_lib_pmkid_generate(prot, pmkid, false) >= 0) {
            kde_len += KDE_PMKID_LEN;
        } else {
            pmk = NULL;
        }
    }

    uint8_t *ptk = sec_prot_keys_ptk_get(sec_keys);
    uint8_t ptkid[PTKID_LEN];
    if (ptk) {
        if (sec_prot_lib_ptkid_generate(prot, ptkid, false) >= 0) {
            kde_len += KDE_PTKID_LEN;
        } else {
            ptk = NULL;
        }
    }

    uint8_t *kde_start = ns_dyn_mem_temporary_alloc(kde_len);
    if (!kde_start) {
        return;
    }

    uint8_t *kde_end = kde_start;

    if (pmk) {
        kde_end = kde_pmkid_write(kde_end, pmkid);
    }

    if (ptk) {
        kde_end = kde_ptkid_write(kde_end, ptkid);
    }

    uint8_t gtkl = sec_prot_keys_fresh_gtkl_get(sec_keys->gtks);
    kde_end = kde_gtkl_write(kde_end, gtkl);

    kde_len = kde_end - kde_start;

    eapol_pdu_t eapol_pdu;

    uint16_t eapol_pdu_size = eapol_pdu_key_frame_init(&eapol_pdu, kde_len, kde_start);

    uint8_t *eapol_decoded_data = ns_dyn_mem_temporary_alloc(eapol_pdu_size + prot->header_size); // In future fill with data that defines eapol message

    if (!eapol_decoded_data) {
        data->result = SEC_RESULT_ERR_NO_MEM;
    } else {
        eapol_pdu.msg.key.key_information.install = false;
        eapol_pdu.msg.key.key_information.pairwise_key = false;
        eapol_pdu.msg.key.key_information.request = true;
        eapol_pdu.msg.key.replay_counter = 0;
        eapol_pdu.msg.key.key_length = 0;
        eapol_write_pdu_frame(eapol_decoded_data + prot->header_size, &eapol_pdu);

        tr_info("Initial EAPOL-Key send, PMKID %s PTKID %s GTKL %x", pmk ? "set" : "not set", ptk ? "set" : "not set", gtkl);

        if (prot->send(prot, eapol_decoded_data, eapol_pdu_size + prot->header_size) < 0) {
            data->result = SEC_RESULT_ERR_NO_MEM;
        }
    }

    ns_dyn_mem_free(kde_start);

    data->state = KEY_CREATE_REQ;
    prot->state_machine_call(prot);
}

static void key_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result)
{
    key_sec_prot_int_t *data = key_sec_prot_get(prot);
    data->state = KEY_CREATE_RESP;
    data->result = result;
    prot->state_machine_call(prot);
}

static int8_t key_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    eapol_pdu_t eapol_pdu;

    tr_info("Initial EAPOL-Key recv, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

    // Decoding is successful
    if (eapol_parse_pdu_header(pdu, size, &eapol_pdu)) {
        if (eapol_pdu.packet_type != EAPOL_KEY_TYPE) {
            tr_info("not EAPOL-Key packet");
            prot->finished(prot);
            return -1;
        }

        uint16_t kde_len;
        uint8_t *kde = sec_prot_lib_message_handle(prot->sec_keys->ptk, &kde_len, &eapol_pdu);
        if (!kde) {
            tr_error("no KDEs");
            prot->finished(prot);
            return -1;
        }

        // Default assumption is that PMK and PTK are not valid
        prot->sec_keys->pmk_mismatch = true;
        prot->sec_keys->ptk_mismatch = true;

        // Checks if supplicant indicates that it has valid PMK
        uint8_t remote_keyid[KEYID_LEN];
        if (kde_pmkid_read(kde, kde_len, remote_keyid) >= 0) {
            uint8_t pmkid[PMKID_LEN];
            if (sec_prot_lib_pmkid_generate(prot, pmkid, true) >= 0) {
                if (memcmp(remote_keyid, pmkid, PMKID_LEN) == 0) {
                    prot->sec_keys->pmk_mismatch = false;
                }
            }
        }

        // Checks if supplicant indicates that it has valid PTK
        if (kde_ptkid_read(kde, kde_len, remote_keyid) >= 0) {
            uint8_t ptkid[PTKID_LEN];
            if (sec_prot_lib_ptkid_generate(prot, ptkid, true) >= 0) {
                if (memcmp(remote_keyid, ptkid, PTKID_LEN) == 0) {
                    prot->sec_keys->ptk_mismatch = false;
                }
            }
        }

        // Get the GTKL that supplicant indicates
        uint8_t gtkl;
        if (kde_gtkl_read(kde, kde_len, &gtkl) >= 0) {
            prot->sec_keys->gtkl = gtkl;
        } else {
            tr_error("no GTKL");
            return -1;
        }

        tr_info("PMK %s PTK %s GTKL %x", prot->sec_keys->pmk_mismatch ? "not live" : "live", prot->sec_keys->ptk_mismatch ? "not live" : "live", gtkl);

        ns_dyn_mem_free(kde);

        prot->create_ind(prot);
        return 0;
    } else {
        tr_error("Invalid");
        // No error handling yet, indicate just that ready to be deleted
        prot->finished(prot);
        return -1;
    }
}

static void key_sec_prot_state_machine(sec_prot_t *prot)
{
    key_sec_prot_int_t *data = key_sec_prot_get(prot);

    // Mixes currently supplicant and authenticator states
    switch (data->state) {
        case KEY_INIT:
            // empty
            break;
        case KEY_CREATE_REQ:
            // KMP-CREATE.confirm
            prot->create_conf(prot, data->result);

            if (data->result == SEC_RESULT_OK) {
                // KMP-FINISHED.indication, no meaning for eapol-key, just completes transfer
                prot->finished_ind(prot, SEC_RESULT_OK, 0);
            }
            // Ready to be deleted
            prot->finished(prot);
            break;
        case KEY_CREATE_RESP:
            if (data->result == SEC_RESULT_OK) {
                // KMP-FINISHED.indication, no meaning for eapol-key, just completes transfer
                prot->finished_ind(prot, SEC_RESULT_OK, 0);
            }
            // Ready to be deleted
            prot->finished(prot);
            break;
        default:
            break;
    }
}

#endif /* HAVE_WS */

