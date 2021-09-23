/*
 * Copyright (c) 2021, Pelion and affiliates.
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
#include "Security/protocols/msg_sec_prot/msg_sec_prot.h"

#ifdef HAVE_WS

#define TRACE_GROUP "msep"

typedef enum {
    MSG_STATE_INIT = SEC_STATE_INIT,
    MSG_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    MSG_STATE_FINISH = SEC_STATE_FINISH,
    MSG_STATE_FINISHED = SEC_STATE_FINISHED
} msg_sec_prot_state_e;

typedef struct {
    sec_prot_common_t              common;       /**< Common data */
} msg_sec_prot_int_t;

static uint16_t msg_sec_prot_size(void);
static int8_t msg_sec_prot_init(sec_prot_t *prot);
static void msg_sec_prot_delete(sec_prot_t *prot);

static void msg_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys);
static void msg_sec_prot_state_machine(sec_prot_t *prot);
static int8_t msg_sec_prot_auth_rejected_send(sec_prot_t *prot, sec_prot_keys_t *sec_keys);

#define msg_sec_prot_get(prot) (msg_sec_prot_int_t *) &prot->data

int8_t msg_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, MSG_PROT, msg_sec_prot_size, msg_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t msg_sec_prot_size(void)
{
    return sizeof(msg_sec_prot_int_t);
}

static int8_t msg_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = msg_sec_prot_create_request;
    prot->delete = msg_sec_prot_delete;
    prot->state_machine = msg_sec_prot_state_machine;

    msg_sec_prot_int_t *data = msg_sec_prot_get(prot);
    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, MSG_STATE_INIT);

    return 0;
}

static void msg_sec_prot_delete(sec_prot_t *prot)
{
    (void) prot;
}

static void msg_sec_prot_create_request(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    (void) sec_keys;

    prot->state_machine(prot);
}

static int8_t msg_sec_prot_auth_rejected_send(sec_prot_t *prot, sec_prot_keys_t *sec_keys)
{
    (void) sec_keys;

    uint8_t *eapol_pdu_frame = ns_dyn_mem_temporary_alloc(prot->header_size);

    // Send zero length message to relay which requests LLC to remove EAPOL temporary entry based on EUI-64
    if (prot->send(prot, eapol_pdu_frame, prot->header_size) < 0) {
        return -1;
    }

    return 0;
}

static void msg_sec_prot_state_machine(sec_prot_t *prot)
{
    msg_sec_prot_int_t *data = msg_sec_prot_get(prot);

    switch (sec_prot_state_get(&data->common)) {
        case MSG_STATE_INIT:
            sec_prot_state_set(prot, &data->common, MSG_STATE_CREATE_REQ);
            break;
        case MSG_STATE_CREATE_REQ:
            // KMP-CREATE.confirm
            prot->create_conf(prot, sec_prot_result_get(&data->common));
            // Authentication rejected (will continue only after new EAPOL Initial-Key)
            (void) msg_sec_prot_auth_rejected_send(prot, prot->sec_keys);
            sec_prot_state_set(prot, &data->common, MSG_STATE_FINISH);
            break;
        case MSG_STATE_FINISH:
            sec_prot_state_set(prot, &data->common, MSG_STATE_FINISHED);
        /* fall through */
        case MSG_STATE_FINISHED:
            prot->finished(prot);
            break;
        default:
            break;
    }
}

#endif /* HAVE_WS */

