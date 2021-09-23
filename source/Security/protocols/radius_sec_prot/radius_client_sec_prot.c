/*
 * Copyright (c) 2020-2021, Pelion and affiliates.
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
#include "randLIB.h"
#include "mbedtls/sha256.h"
#include "mbedtls/md5.h"
#include "fhss_config.h"
#include "Service_Libs/Trickle/trickle.h"
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
#include "Security/protocols/radius_sec_prot/radius_client_sec_prot.h"
#include "Security/protocols/radius_sec_prot/avp_helper.h"
#include "Security/protocols/tls_sec_prot/tls_sec_prot_lib.h"
#include "Service_Libs/hmac/hmac_md.h"

#ifdef HAVE_WS

#define TRACE_GROUP "radp"

typedef enum {
    RADIUS_STATE_INIT = SEC_STATE_INIT,
    RADIUS_STATE_CREATE_REQ = SEC_STATE_CREATE_REQ,
    RADIUS_STATE_CREATE_RESP = SEC_STATE_CREATE_RESP,
    RADIUS_STATE_CREATE_IND = SEC_STATE_CREATE_IND,

    RADIUS_STATE_STATE_RESPONSE_ID = SEC_STATE_FIRST,
    RADIUS_STATE_SEND_INITIAL_ACCESS_REQUEST,
    RADIUS_STATE_SEND_ACCESS_REQUEST,
    RADIUS_STATE_ACCESS_ACCEPT_REJECT_CHALLENGE,
    RADIUS_STATE_ACCESS_REQUEST,
    RADIUS_STATE_CONFIGURE,
    RADIUS_STATE_PROCESS,

    RADIUS_STATE_FINISH = SEC_STATE_FINISH,
    RADIUS_STATE_FINISHED = SEC_STATE_FINISHED
} radius_client_sec_prot_state_e;

#define RADIUS_MSG_FIXED_LENGTH       20
#define RADIUS_ACCESS_REQUEST         1
#define RADIUS_ACCESS_ACCEPT          2
#define RADIUS_ACCESS_REJECT          3
#define RADIUS_ACCESS_CHALLENGE       11
#define RADIUS_MESSAGE_NONE           0

#define MS_MPPE_RECV_KEY_SALT_LEN     2
#define MS_MPPE_RECV_KEY_BLOCK_LEN    16

#define RADIUS_CONN_NUMBER            3
#define RADIUS_ID_RANGE_SIZE          10
#define RADIUS_ID_RANGE_NUM           (255 / RADIUS_ID_RANGE_SIZE) - 1

#define RADIUS_ID_TIMEOUT             60

typedef struct radius_client_sec_prot_lib_int_s radius_client_sec_prot_lib_int_t;

typedef struct {
    sec_prot_common_t             common;                       /**< Common data */
    sec_prot_t                    *radius_eap_tls_prot;         /**< Radius EAP-TLS security protocol */
    sec_prot_receive              *radius_eap_tls_send;         /**< Radius EAP-TLS security protocol send (receive from peer) */
    sec_prot_delete               *radius_eap_tls_deleted;      /**< Radius EAP-TLS security protocol peer deleted (notify to peer that radius client deleted) */
    uint8_t                       radius_eap_tls_header_size;   /**< Radius EAP-TLS header size */
    uint8_t                       new_pmk[PMK_LEN];             /**< New Pair Wise Master Key */
    uint16_t                      recv_eap_msg_len;             /**< Received EAP message length */
    uint8_t                       *recv_eap_msg;                /**< Received EAP message */
    uint16_t                      send_radius_msg_len;          /**< Send radius message length */
    uint8_t                       *send_radius_msg;             /**< Send radius message */
    uint8_t                       identity_len;                 /**< Supplicant EAP identity length */
    uint8_t                       *identity;                    /**< Supplicant EAP identity */
    uint8_t                       radius_code;                  /**< Radius code that was received */
    uint8_t                       radius_identifier;            /**< Radius identifier that was last sent */
    uint8_t                       radius_id_conn_num;           /**< Radius identifier connection number (socket instance) */
    uint8_t                       radius_id_range;              /**< Radius identifier range */
    uint8_t                       request_authenticator[16];    /**< Radius request authenticator that was last sent */
    uint8_t                       state_len;                    /**< Radius state length that was last received */
    uint8_t                       *state;                       /**< Radius state that was last received */
    uint8_t                       remote_eui_64_hash[8];        /**< Remote EUI-64 hash used for calling station id */
    bool                          remote_eui_64_hash_set : 1;   /**< Remote EUI-64 hash used for calling station id set */
    bool                          new_pmk_set : 1;              /**< New Pair Wise Master Key set */
    bool                          radius_id_range_set : 1;      /**< Radius identifier start value set */
} radius_client_sec_prot_int_t;

typedef struct {
    uint8_t radius_identifier_timer[RADIUS_CONN_NUMBER][RADIUS_ID_RANGE_NUM];
    shared_comp_data_t comp_data;                               /**< Shared component data (timer, delete) */
    uint8_t local_eui64_hash[8];                                /**< Local EUI-64 hash used for called stations id */
    uint8_t hash_random[16];                                    /**< Random used to generate local and remote EUI-64 hashes */
    bool local_eui64_hash_set : 1;                              /**< Local EUI-64 hash used for called stations id set */
    bool hash_random_set : 1;                                   /**< Random used to generate local and remote EUI-64 hashes set */
    bool radius_id_timer_running : 1;                           /**> Radius identifier timer running */
} radius_client_sec_prot_shared_t;

static uint16_t radius_client_sec_prot_size(void);
static int8_t radius_client_sec_prot_init(sec_prot_t *prot);
static int8_t radius_client_sec_prot_shared_data_timeout(uint16_t ticks);
static int8_t radius_client_sec_prot_shared_data_delete(void);
static void radius_identifier_timer_value_set(uint8_t conn_num, uint8_t id_range, uint8_t value);
static void radius_client_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result);
static void radius_client_sec_prot_delete(sec_prot_t *prot);
static int8_t radius_client_sec_prot_receive_check(sec_prot_t *prot, const void *pdu, uint16_t size);
static int8_t radius_client_sec_prot_init_radius_eap_tls(sec_prot_t *prot);
static void radius_client_sec_prot_radius_eap_tls_deleted(sec_prot_t *prot);
static uint16_t radius_client_sec_prot_eap_avps_handle(uint16_t avp_length, uint8_t *avp_ptr, uint8_t *copy_to_ptr);
static int8_t radius_client_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size, uint8_t conn_number);
static int8_t radius_client_sec_prot_radius_eap_receive(sec_prot_t *prot, void *pdu, uint16_t size);
static void radius_client_sec_prot_allocate_and_create_radius_message(sec_prot_t *prot);
static int8_t radius_client_sec_prot_radius_msg_send(sec_prot_t *prot);
static void radius_client_sec_prot_radius_msg_free(sec_prot_t *prot);
static uint8_t radius_client_sec_prot_identifier_allocate(sec_prot_t *prot, uint8_t value);
static void radius_client_sec_prot_identifier_free(sec_prot_t *prot);
static uint8_t radius_client_sec_prot_hex_to_ascii(uint8_t value);
static int8_t radius_client_sec_prot_eui_64_hash_generate(uint8_t *eui_64, uint8_t *hashed_eui_64);
static void radius_client_sec_prot_station_id_generate(uint8_t *eui_64, uint8_t *station_id_ptr);
static int8_t radius_client_sec_prot_message_authenticator_calc(sec_prot_t *prot, uint16_t msg_len, uint8_t *msg_ptr, uint8_t *auth_ptr);
static int8_t radius_client_sec_prot_response_authenticator_calc(sec_prot_t *prot, uint16_t msg_len, uint8_t *msg_ptr, uint8_t *auth_ptr);
static int8_t radius_client_sec_prot_ms_mppe_recv_key_pmk_decrypt(sec_prot_t *prot, uint8_t *recv_key, uint8_t recv_key_len, uint8_t *request_authenticator, uint8_t *pmk_ptr);
static void radius_client_sec_prot_finished_send(sec_prot_t *prot);
static void radius_client_sec_prot_state_machine(sec_prot_t *prot);
static void radius_client_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks);

#define radius_client_sec_prot_get(prot) (radius_client_sec_prot_int_t *) &prot->data

// Data shared between radius client instances
static radius_client_sec_prot_shared_t *shared_data = NULL;

int8_t radius_client_sec_prot_register(kmp_service_t *service)
{
    if (!service) {
        return -1;
    }

    if (kmp_service_sec_protocol_register(service, RADIUS_CLIENT_PROT, radius_client_sec_prot_size, radius_client_sec_prot_init) < 0) {
        return -1;
    }

    return 0;
}

static uint16_t radius_client_sec_prot_size(void)
{
    return sizeof(radius_client_sec_prot_int_t);
}

static int8_t radius_client_sec_prot_shared_data_timeout(uint16_t ticks)
{
    if (shared_data == NULL || !shared_data->radius_id_timer_running) {
        return -1;
    }

    bool timer_running = false;

    for (uint8_t conn_num = 0; conn_num < RADIUS_CONN_NUMBER; conn_num++) {
        for (uint8_t id_range = 0; id_range < RADIUS_ID_RANGE_NUM; id_range++) {
            if (shared_data->radius_identifier_timer[conn_num][id_range] > ticks) {
                shared_data->radius_identifier_timer[conn_num][id_range] -= ticks;
                timer_running = true;
            } else {
                shared_data->radius_identifier_timer[conn_num][id_range] = 0;
            }
        }
    }

    if (!timer_running) {
        shared_data->radius_id_timer_running = false;
    }

    return 0;
}

static void radius_identifier_timer_value_set(uint8_t conn_num, uint8_t id_range, uint8_t value)
{
    shared_data->radius_identifier_timer[conn_num][id_range] = value;
    shared_data->radius_id_timer_running = true;
}

static int8_t radius_client_sec_prot_shared_data_delete(void)
{
    if (shared_data == NULL) {
        return -1;
    }
    ns_dyn_mem_free(shared_data);
    shared_data = NULL;
    return 0;
}

static int8_t radius_client_sec_prot_init(sec_prot_t *prot)
{
    prot->create_req = NULL;
    prot->create_resp = radius_client_sec_prot_create_response;
    prot->conn_receive = radius_client_sec_prot_receive;
    prot->receive_peer = radius_client_sec_prot_radius_eap_receive;
    prot->peer_deleted = radius_client_sec_prot_radius_eap_tls_deleted;
    prot->delete = radius_client_sec_prot_delete;
    prot->state_machine = radius_client_sec_prot_state_machine;
    prot->timer_timeout = radius_client_sec_prot_timer_timeout;
    prot->finished_send = radius_client_sec_prot_finished_send;
    prot->receive_check = radius_client_sec_prot_receive_check;

    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    sec_prot_init(&data->common);
    sec_prot_state_set(prot, &data->common, RADIUS_STATE_INIT);
    data->radius_eap_tls_prot = NULL;
    data->radius_eap_tls_send = NULL;
    data->radius_eap_tls_header_size = 0;
    memset(data->new_pmk, 0, PMK_LEN);
    data->recv_eap_msg_len = 0;
    data->recv_eap_msg = NULL;
    data->send_radius_msg_len = 0;
    data->send_radius_msg = NULL;
    data->identity_len = 0;
    data->identity = NULL;
    data->radius_code = RADIUS_MESSAGE_NONE;
    data->radius_identifier = 0;
    memset(data->request_authenticator, 0, 16);
    data->state_len = 0;
    data->state = NULL;
    memset(data->remote_eui_64_hash, 0, 8);
    data->remote_eui_64_hash_set = false;
    data->new_pmk_set = false;
    data->radius_id_range_set = false;

    if (!shared_data) {
        shared_data = ns_dyn_mem_alloc(sizeof(radius_client_sec_prot_shared_t));
        if (!shared_data) {
            return -1;
        }
        memset(shared_data, 0, sizeof(radius_client_sec_prot_shared_t));
        shared_data->local_eui64_hash_set = false;
        shared_data->hash_random_set = false;
        shared_data->radius_id_timer_running = false;
        // Add as shared component to enable timers and delete
        shared_data->comp_data.timeout = radius_client_sec_prot_shared_data_timeout;
        shared_data->comp_data.delete = radius_client_sec_prot_shared_data_delete;
        prot->shared_comp_add(prot, &shared_data->comp_data);
    }

    return 0;
}

static void radius_client_sec_prot_delete(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (data->recv_eap_msg != NULL) {
        ns_dyn_mem_free(data->recv_eap_msg);
    }
    if (data->send_radius_msg != NULL) {
        ns_dyn_mem_free(data->send_radius_msg);
    }
    if (data->identity != NULL) {
        ns_dyn_mem_free(data->identity);
    }
    if (data->state != NULL) {
        ns_dyn_mem_free(data->state);
    }
}

static void radius_client_sec_prot_create_response(sec_prot_t *prot, sec_prot_result_e result)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    // Call state machine
    sec_prot_result_set(&data->common, result);
    prot->state_machine_call(prot);
}

static int8_t radius_client_sec_prot_receive_check(sec_prot_t *prot, const void *pdu, uint16_t size)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (size >= 2) {
        const uint8_t *radius_msg = pdu;
        if (radius_msg[1] == data->radius_identifier) {
            return 0;
        }
    }

    return -1;
}

static int8_t radius_client_sec_prot_init_radius_eap_tls(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);
    if (data->radius_eap_tls_prot) {
        return 0;
    }

    data->radius_eap_tls_prot = prot->type_get(prot, SEC_PROT_TYPE_RADIUS_EAP_TLS);
    if (!data->radius_eap_tls_prot) {
        return -1;
    }
    data->radius_eap_tls_header_size = data->radius_eap_tls_prot->receive_peer_hdr_size;
    data->radius_eap_tls_send = data->radius_eap_tls_prot->receive_peer;
    data->radius_eap_tls_deleted = data->radius_eap_tls_prot->peer_deleted;

    return 0;
}

static void radius_client_sec_prot_radius_eap_tls_deleted(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    tr_info("Radius: EAP-TLS deleted");

    data->radius_eap_tls_prot = NULL;
    data->radius_eap_tls_send = NULL;
    data->radius_eap_tls_deleted = NULL;
}

static uint16_t radius_client_sec_prot_eap_avps_handle(uint16_t avp_length, uint8_t *avp_ptr, uint8_t *copy_to_ptr)
{
    // Calculate EAP AVPs length and copy EAP AVPs to continuous buffer if buffer is give
    uint16_t eap_len = 0;
    while (true) {
        uint8_t avp_eap_len;
        uint8_t *eap_message = avp_eap_message_read(avp_ptr, avp_length, &avp_eap_len);
        if (eap_message) {
            avp_eap_len -= AVP_FIXED_LEN;

            // Calculate EAP AVPs length
            eap_len += avp_eap_len;

            // Copy EAP AVPs to continuous buffer
            if (copy_to_ptr) {
                memcpy(copy_to_ptr, eap_message, avp_eap_len);
                copy_to_ptr += avp_eap_len;
            }

            uint16_t offset = eap_message - avp_ptr;
            avp_length = avp_length - (offset + avp_eap_len);
            avp_ptr = eap_message + avp_eap_len;
        } else {
            break;
        }
    }

    return eap_len;
}

static int8_t radius_client_sec_prot_receive(sec_prot_t *prot, void *pdu, uint16_t size, uint8_t conn_number)
{
    (void) conn_number;

    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (size < RADIUS_MSG_FIXED_LENGTH) {
        return -1;
    }

    uint8_t *radius_msg_ptr = pdu;

    uint8_t code = *radius_msg_ptr++;
    if (code != RADIUS_ACCESS_ACCEPT && code != RADIUS_ACCESS_REJECT && code != RADIUS_ACCESS_CHALLENGE) {
        return -1;
    }

    uint8_t identifier = *radius_msg_ptr++;
    /* If identifier does not match to sent identifier, silently ignore message,
       already checked on socket if before routing the request to receive, so
       this is double check to ensure correct routing */
    if (identifier != data->radius_identifier) {
        return -1;
    }

    uint16_t length = common_read_16_bit(radius_msg_ptr);
    radius_msg_ptr += 2;

    if (length < RADIUS_MSG_FIXED_LENGTH) {
        return -1;
    }

    // Store response authenticator
    uint8_t recv_response_authenticator[16];
    memcpy(recv_response_authenticator, radius_msg_ptr, 16);

    // Replace response authenticator with request authenticator
    memcpy(radius_msg_ptr, data->request_authenticator, 16);
    radius_msg_ptr += 16;

    // Calculate expected response authenticator
    uint8_t calc_response_authenticator[16];
    if (radius_client_sec_prot_response_authenticator_calc(prot, length, pdu, calc_response_authenticator) < 0) {
        tr_error("Could not calculate response authenticator MD5 hash");
        return -1;
    }

    // Verify that received and calculated response authenticator matches
    if (memcmp(recv_response_authenticator, calc_response_authenticator, 16) != 0) {
        tr_error("Invalid response authenticator recv: %s, calc: %s", tr_array(recv_response_authenticator, 16), tr_array(calc_response_authenticator, 16));
        return -1;
    }

    // Response authenticator matches, start validating radius EAP-TLS specific fields
    data->recv_eap_msg = NULL;
    data->recv_eap_msg_len = 0;

    uint16_t avp_length = 0;
    if (length >= RADIUS_MSG_FIXED_LENGTH) {
        avp_length = length - RADIUS_MSG_FIXED_LENGTH;
    }

    uint8_t *message_authenticator = avp_message_authenticator_read(radius_msg_ptr, avp_length);
    if (message_authenticator == NULL || avp_length == 0) {
        tr_error("No message authenticator");
        // Message does not have radius EAP-TLS specific fields
        data->radius_code = code;
        prot->state_machine(prot);
        data->radius_code = RADIUS_MESSAGE_NONE;

        return 0;
    }

    // Store message authenticator
    uint8_t recv_message_authenticator[16];
    memcpy(recv_message_authenticator, message_authenticator, 16);

    // Replace message authenticator with zero
    memset(message_authenticator, 0, 16);

    // Calculate expected message authenticator
    uint8_t calc_message_authenticator[16];
    if (radius_client_sec_prot_message_authenticator_calc(prot, length, pdu, calc_message_authenticator) < 0) {
        tr_error("Could not calculate message authenticator HMAC-MD5 hash");
        return -1;
    }

    // Verify that received and calculated message authenticator matches
    if (memcmp(recv_message_authenticator, calc_message_authenticator, 16) != 0) {
        tr_error("Invalid message authenticator recv: %s, calc: %s", tr_array(recv_message_authenticator, 16), tr_array(calc_message_authenticator, 16));
        return -1;
    }

    // Calculate EAP AVPs length
    data->recv_eap_msg_len = radius_client_sec_prot_eap_avps_handle(avp_length, radius_msg_ptr, NULL);
    if (data->recv_eap_msg_len == 0) {
        return -1;
    }

    // Allocate memory for continuous EAP buffer
    data->recv_eap_msg = ns_dyn_mem_temporary_alloc(data->recv_eap_msg_len + data->radius_eap_tls_header_size);
    if (data->recv_eap_msg == NULL) {
        tr_error("Cannot allocate eap msg");
        return -1;
    }

    // Copy EAP AVPs to continuous buffer
    uint16_t copy_eap_len = radius_client_sec_prot_eap_avps_handle(avp_length, radius_msg_ptr, data->recv_eap_msg + data->radius_eap_tls_header_size);
    if (copy_eap_len != data->recv_eap_msg_len) {
        ns_dyn_mem_free(data->recv_eap_msg);
        return -1;
    }

    // Store state
    uint8_t state_len;
    uint8_t *state = avp_state_read(radius_msg_ptr, avp_length, &state_len);
    if (state) {
        state_len -= AVP_FIXED_LEN;
        if (data->state && data->state_len != state_len) {
            ns_dyn_mem_free(data->state);
            data->state = NULL;
        }
        if (!data->state) {
            data->state = ns_dyn_mem_temporary_alloc(state_len);
        }
        if (!data->state) {
            tr_error("Cannot allocate state");
            ns_dyn_mem_free(data->recv_eap_msg);
            data->recv_eap_msg = NULL;
            return -1;
        }
        memcpy(data->state, state, state_len);
        data->state_len = state_len;
    } else {
        if (data->state) {
            ns_dyn_mem_free(data->state);
            data->state = NULL;
            data->state_len = 0;
        }
    }

    if (code == RADIUS_ACCESS_ACCEPT) {
        uint8_t recv_key_len;
        uint8_t *recv_key = avp_vsa_ms_mppe_recv_key_read(radius_msg_ptr, avp_length, &recv_key_len);
        if (recv_key && recv_key_len > AVP_FIXED_LEN) {
            if (radius_client_sec_prot_ms_mppe_recv_key_pmk_decrypt(prot, recv_key,
                                                                    recv_key_len - AVP_FIXED_LEN, data->request_authenticator, data->new_pmk) >= 0) {
                data->new_pmk_set = true;
#ifdef EXTRA_DEBUG_INFO
                tr_info("RADIUS PMK: %s %s", tr_array(data->new_pmk, 16), tr_array(data->new_pmk + 16, 16));
#endif
            }
        }
    }

    data->radius_code = code;
    data->recv_eap_msg_len += data->radius_eap_tls_header_size;
    prot->state_machine(prot);
    data->radius_code = RADIUS_MESSAGE_NONE;

    return 0;
}

static int8_t radius_client_sec_prot_radius_eap_receive(sec_prot_t *prot, void *pdu, uint16_t size)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    data->recv_eap_msg_len = size;
    data->recv_eap_msg = pdu;

    prot->state_machine(prot);

    data->recv_eap_msg_len = 0;
    data->recv_eap_msg = NULL;

    return 0;
}

static uint8_t radius_client_sec_prot_identifier_allocate(sec_prot_t *prot, uint8_t value)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (!data->radius_id_range_set || value >= (data->radius_id_range * RADIUS_ID_RANGE_SIZE) + RADIUS_ID_RANGE_SIZE - 1) {
        for (uint8_t conn_num = 0; conn_num < RADIUS_CONN_NUMBER; conn_num++) {
            for (uint8_t id_range = 0; id_range < RADIUS_ID_RANGE_NUM; id_range++) {
                if (shared_data->radius_identifier_timer[conn_num][id_range] == 0) {
                    // If range has been already reserved
                    if (data->radius_id_range_set) {
                        // Set previous range to timeout at 1/5 of identifier timeout
                        radius_identifier_timer_value_set(data->radius_id_conn_num, data->radius_id_range, RADIUS_ID_TIMEOUT / 5);
                    }
                    // Set timeout for new range to 60 seconds
                    radius_identifier_timer_value_set(conn_num, id_range, RADIUS_ID_TIMEOUT);
                    data->radius_id_conn_num = conn_num;
                    data->radius_id_range = id_range;
                    data->radius_id_range_set = true;
                    return id_range * RADIUS_ID_RANGE_SIZE;
                }
            }
        }
    } else {
        radius_identifier_timer_value_set(data->radius_id_conn_num, data->radius_id_range, RADIUS_ID_TIMEOUT);
        return value + 1;
    }

    return 0;
}

static void radius_client_sec_prot_identifier_free(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (data->radius_id_range_set) {
        // Timeout at 1/5 of identifier timeout
        radius_identifier_timer_value_set(data->radius_id_conn_num, data->radius_id_range, RADIUS_ID_TIMEOUT / 5);
    }
}

static uint8_t radius_client_sec_prot_eui_64_hash_get(sec_prot_t *prot, uint8_t *local_eui_64_hash, uint8_t *remote_eui_64_hash, bool remote_eui_64_hash_set)
{
    if (!shared_data->local_eui64_hash_set || !remote_eui_64_hash_set) {
        uint8_t local_eui64[8];
        uint8_t remote_eui64[8];
        prot->addr_get(prot, local_eui64, remote_eui64);
        if (!shared_data->local_eui64_hash_set) {
            radius_client_sec_prot_eui_64_hash_generate(local_eui64, shared_data->local_eui64_hash);
            shared_data->local_eui64_hash_set = true;
        }
        if (!remote_eui_64_hash_set) {
            radius_client_sec_prot_eui_64_hash_generate(remote_eui64, remote_eui_64_hash);
        }
    }
    memcpy(local_eui_64_hash, shared_data->local_eui64_hash, 8);

    return 0;
}

static void radius_client_sec_prot_allocate_and_create_radius_message(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    // code(1), packet-identifier(1), length(2), authenticator(16)
    uint16_t radius_msg_length = RADIUS_MSG_FIXED_LENGTH;

    eapol_pdu_t *eap_hdr = (eapol_pdu_t *) data->recv_eap_msg;

    if (eap_hdr->msg.eap.type == EAP_IDENTITY) {
        uint16_t id_len = eap_hdr->msg.eap.length;
        // Calculate identity length
        if (id_len > 5) {
            id_len -= 5;
        }
        // Maximum length is 253 bytes
        if (id_len > AVP_VALUE_MAX_LEN) {
            id_len = AVP_VALUE_MAX_LEN;
        }
        data->identity = ns_dyn_mem_temporary_alloc(id_len);
        if (!data->identity) {
            return;
        }
        data->identity_len = id_len;
        memcpy(data->identity, eap_hdr->msg.eap.data_ptr, id_len);
    }

    // Calculate eap fragments
    uint16_t eap_len = eap_hdr->msg.eap.length;
    while (true) {
        if (eap_len > AVP_VALUE_MAX_LEN) {
            eap_len -= AVP_VALUE_MAX_LEN;
            radius_msg_length += AVP_TYPE_EAP_MESSAGE_LEN(AVP_VALUE_MAX_LEN);
        } else {
            radius_msg_length += AVP_TYPE_EAP_MESSAGE_LEN(eap_len);
            break;
        }
    }

    radius_msg_length += AVP_TYPE_USER_NAME_LEN(data->identity_len);
    radius_msg_length += AVP_TYPE_NAS_PORT_LEN;
    radius_msg_length += AVP_TYPE_FRAMED_MTU_LEN;

    if (data->state) {
        radius_msg_length += AVP_FIXED_LEN + data->state_len;
    }

    radius_msg_length += AVP_TYPE_CALLED_STATION_ID_LEN(STATION_ID_LEN);
    radius_msg_length += AVP_TYPE_CALLING_STATION_ID_LEN(STATION_ID_LEN);
    radius_msg_length += AVP_TYPE_NAS_IDENTIFIER_LEN(STATION_ID_LEN);
    radius_msg_length += AVP_TYPE_NAS_PORT_TYPE_LEN;
    radius_msg_length += AVP_TYPE_MESSAGE_AUTHENTICATOR_LEN;
    radius_msg_length += AVP_TYPE_NAS_IPV6_ADDRESS_LEN;

    uint8_t *radius_msg_ptr = ns_dyn_mem_temporary_alloc(radius_msg_length);
    if (radius_msg_ptr == NULL) {
        if (data->send_radius_msg != NULL) {
            ns_dyn_mem_free(data->send_radius_msg);
        }
        data->send_radius_msg = NULL;
        data->send_radius_msg_len = 0;
        return;
    }
    uint8_t *radius_msg_start_ptr = radius_msg_ptr;

    *radius_msg_ptr++ = RADIUS_ACCESS_REQUEST;                                // code
    data->radius_identifier = radius_client_sec_prot_identifier_allocate(prot, data->radius_identifier);
    *radius_msg_ptr++ = data->radius_identifier;                              // identifier
    radius_msg_ptr = common_write_16_bit(radius_msg_length, radius_msg_ptr);  // length

    randLIB_get_n_bytes_random(data->request_authenticator, 16);
    memcpy(radius_msg_ptr, data->request_authenticator, 16);                  // request authenticator
    radius_msg_ptr += 16;

    radius_msg_ptr = avp_user_name_write(radius_msg_ptr, data->identity_len, data->identity);

    uint8_t local_eui_64_hash[8];
    radius_client_sec_prot_eui_64_hash_get(prot, local_eui_64_hash, data->remote_eui_64_hash, data->remote_eui_64_hash_set);
    data->remote_eui_64_hash_set = true;

    uint8_t station_id[STATION_ID_LEN];

    radius_client_sec_prot_station_id_generate(data->remote_eui_64_hash, station_id);
    radius_msg_ptr = avp_calling_station_id_write(radius_msg_ptr, STATION_ID_LEN, station_id);

    radius_client_sec_prot_station_id_generate(local_eui_64_hash, station_id);
    radius_msg_ptr = avp_called_station_id_write(radius_msg_ptr, STATION_ID_LEN, station_id);

    radius_msg_ptr = avp_nas_identifier_write(radius_msg_ptr, STATION_ID_LEN, station_id);

    radius_msg_ptr = avp_nas_port_write(radius_msg_ptr, NAS_PORT);

    radius_msg_ptr = avp_framed_mtu_write(radius_msg_ptr, FRAMED_MTU);

    if (data->state) {
        radius_msg_ptr = avp_state_write(radius_msg_ptr, data->state_len, data->state);
    }

    radius_msg_ptr = avp_nas_port_type_write(radius_msg_ptr, NAS_PORT_TYPE_WIRELESS_IEEE802_11);

    uint8_t address[16];
    prot->ip_addr_get(prot, address);
    radius_msg_ptr = avp_nas_ipv6_address_write(radius_msg_ptr, address);

    // Write eap fragments
    eap_len = eap_hdr->msg.eap.length;
    uint8_t *eap_ptr = eap_hdr->packet_body;
    while (true) {
        if (eap_len > AVP_VALUE_MAX_LEN) {
            eap_len -= AVP_VALUE_MAX_LEN;
            radius_msg_ptr = avp_eap_message_write(radius_msg_ptr, AVP_VALUE_MAX_LEN, eap_ptr);
            eap_ptr += AVP_VALUE_MAX_LEN;
        } else {
            radius_msg_ptr = avp_eap_message_write(radius_msg_ptr, eap_len, eap_ptr);
            break;
        }
    }

    uint8_t *message_auth_ptr = radius_msg_ptr;
    uint8_t message_auth[16];
    memset(message_auth, 0, 16); // zero for value calculation
    radius_msg_ptr = avp_message_authenticator_write(radius_msg_ptr, message_auth);
    // Calculate message authenticator
    if (radius_client_sec_prot_message_authenticator_calc(prot, radius_msg_length, radius_msg_start_ptr, message_auth) < 0) {
        ns_dyn_mem_free(radius_msg_start_ptr);
        return;
    }
    // Write message authenticator
    radius_msg_ptr = avp_message_authenticator_write(message_auth_ptr, message_auth);

    // Store message for sending
    if (data->send_radius_msg != NULL) {
        ns_dyn_mem_free(data->send_radius_msg);
    }
    data->send_radius_msg = radius_msg_start_ptr;
    data->send_radius_msg_len = radius_msg_length;
}

static int8_t radius_client_sec_prot_radius_msg_send(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (!data->send_radius_msg || data->send_radius_msg_len == 0) {
        return -1;
    }

    if (prot->conn_send(prot, data->send_radius_msg, data->send_radius_msg_len, data->radius_id_conn_num, SEC_PROT_SEND_FLAG_NO_DEALLOC) < 0) {
        return -1;
    }

    return 0;
}

static void radius_client_sec_prot_radius_msg_free(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    if (data->send_radius_msg != NULL) {
        ns_dyn_mem_free(data->send_radius_msg);
    }

    data->send_radius_msg = NULL;
    data->send_radius_msg_len = 0;
}

static int8_t radius_client_sec_prot_eui_64_hash_generate(uint8_t *eui_64, uint8_t *hashed_eui_64)
{
    int8_t ret_val = 0;

    if (!shared_data->hash_random_set) {
        randLIB_get_n_bytes_random(shared_data->hash_random, 16);
    }

    uint8_t hashed_string[24];
    memcpy(&hashed_string[0], eui_64, 8);
    memcpy(&hashed_string[8], shared_data->hash_random, 16);

    mbedtls_sha256_context ctx;

    mbedtls_sha256_init(&ctx);

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_starts(&ctx, 0) != 0) {
#else
    if (mbedtls_sha256_starts_ret(&ctx, 0) != 0) {
#endif
        ret_val = -1;
        goto error;
    }

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_update(&ctx, hashed_string, 24) != 0) {
#else
    if (mbedtls_sha256_update_ret(&ctx, hashed_string, 24) != 0) {
#endif
        ret_val = -1;
        goto error;
    }

    uint8_t output[32];

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_sha256_finish(&ctx, output) != 0) {
#else
    if (mbedtls_sha256_finish_ret(&ctx, output) != 0) {
#endif
        ret_val = -1;
        goto error;
    }

    memcpy(hashed_eui_64, output, 8);

error:
    mbedtls_sha256_free(&ctx);

    return ret_val;
}

static uint8_t radius_client_sec_prot_hex_to_ascii(uint8_t value)
{
    char character = '0';
    if (value <= 9) {
        character = '0' + value;
    } else if (value >= 0x0A && value <= 0x0F) {
        character = 'A' + value - 0x0A;
    }
    return character;
}

static void radius_client_sec_prot_station_id_generate(uint8_t *eui_64, uint8_t *station_id_ptr)
{
    for (uint8_t i = 0; i < 8; i++) {
        *station_id_ptr++ = radius_client_sec_prot_hex_to_ascii(eui_64[i] / 16);
        *station_id_ptr++ = radius_client_sec_prot_hex_to_ascii(eui_64[i] % 16);
        if (i != 7) {
            *station_id_ptr++ = '-';
        }
    }
}

static int8_t radius_client_sec_prot_message_authenticator_calc(sec_prot_t *prot, uint16_t msg_len, uint8_t *msg_ptr, uint8_t *auth_ptr)
{
    if (prot->sec_cfg->radius_cfg->radius_shared_secret == NULL || prot->sec_cfg->radius_cfg->radius_shared_secret_len == 0) {
        return -1;
    }

    const uint8_t *key = prot->sec_cfg->radius_cfg->radius_shared_secret;
    uint16_t key_len = prot->sec_cfg->radius_cfg->radius_shared_secret_len;

#ifndef MBEDTLS_MD5_C
    tr_error("FATAL: MD5 MBEDTLS_MD5_C not enabled");
#endif

    if (hmac_md_calc(ALG_HMAC_MD5, key, key_len, msg_ptr, msg_len, auth_ptr, 16) < 0) {
        return -1;
    }

    return 0;
}

static int8_t radius_client_sec_prot_response_authenticator_calc(sec_prot_t *prot, uint16_t msg_len, uint8_t *msg_ptr, uint8_t *auth_ptr)
{
#ifdef MBEDTLS_MD5_C
    if (prot->sec_cfg->radius_cfg->radius_shared_secret == NULL || prot->sec_cfg->radius_cfg->radius_shared_secret_len == 0) {
        return -1;
    }

    const uint8_t *key = prot->sec_cfg->radius_cfg->radius_shared_secret;
    uint16_t key_len = prot->sec_cfg->radius_cfg->radius_shared_secret_len;
    if (prot->sec_cfg->radius_cfg->radius_shared_secret_len == 0) {
        return -1;
    }

    int8_t ret_value = -1;

    mbedtls_md5_context ctx;

    mbedtls_md5_init(&ctx);

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_md5_starts(&ctx) != 0) {
#else
    if (mbedtls_md5_starts_ret(&ctx) != 0) {
#endif
        goto end;
    }

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_md5_update(&ctx, msg_ptr, msg_len) != 0) {
#else
    if (mbedtls_md5_update_ret(&ctx, msg_ptr, msg_len) != 0) {
#endif
        goto end;
    }

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_md5_update(&ctx, key, key_len) != 0) {
#else
    if (mbedtls_md5_update_ret(&ctx, key, key_len) != 0) {
#endif
        goto end;
    }

#if (MBEDTLS_VERSION_MAJOR >= 3)
    if (mbedtls_md5_finish(&ctx, auth_ptr) != 0) {
#else
    if (mbedtls_md5_finish_ret(&ctx, auth_ptr) != 0) {
#endif
        goto end;
    }

    ret_value = 0;

end:
    mbedtls_md5_free(&ctx);

    return ret_value;
#else
    (void) prot;
    (void) msg_len;
    (void) msg_ptr;
    (void) auth_ptr;

    tr_error("FATAL: MD5 MBEDTLS_MD5_C not enabled");

    return -1;
#endif
}

static int8_t radius_client_sec_prot_ms_mppe_recv_key_pmk_decrypt(sec_prot_t *prot, uint8_t *recv_key, uint8_t recv_key_len, uint8_t *request_authenticator, uint8_t *pmk_ptr)
{
#ifdef MBEDTLS_MD5_C
    if (prot->sec_cfg->radius_cfg->radius_shared_secret == NULL || prot->sec_cfg->radius_cfg->radius_shared_secret_len == 0) {
        return -1;
    }

    const uint8_t *key = prot->sec_cfg->radius_cfg->radius_shared_secret;
    uint16_t key_len = prot->sec_cfg->radius_cfg->radius_shared_secret_len;
    if (prot->sec_cfg->radius_cfg->radius_shared_secret_len == 0) {
        return -1;
    }

    if (recv_key_len < MS_MPPE_RECV_KEY_SALT_LEN + MS_MPPE_RECV_KEY_BLOCK_LEN) {
        return -1;
    }

    uint8_t *salt_ptr = recv_key;
    uint8_t *cipher_text_ptr = recv_key + MS_MPPE_RECV_KEY_SALT_LEN;
    int16_t cipher_text_len = recv_key_len - MS_MPPE_RECV_KEY_SALT_LEN;

    uint8_t plain_text[cipher_text_len];
    uint8_t *plain_text_ptr = plain_text;

    bool first_interm_b_value = true;
    uint8_t interm_b_val[MS_MPPE_RECV_KEY_BLOCK_LEN];

    bool md5_failed = false;

    mbedtls_md5_context ctx;

    while (cipher_text_len >= MS_MPPE_RECV_KEY_BLOCK_LEN) {
        mbedtls_md5_init(&ctx);

#if (MBEDTLS_VERSION_MAJOR >= 3)
        if (mbedtls_md5_starts(&ctx) != 0) {
#else
        if (mbedtls_md5_starts_ret(&ctx) != 0) {
#endif
            md5_failed = true;
            break;
        }

#if (MBEDTLS_VERSION_MAJOR >= 3)
        if (mbedtls_md5_update(&ctx, key, key_len) != 0) {
#else
        if (mbedtls_md5_update_ret(&ctx, key, key_len) != 0) {
#endif
            md5_failed = true;
            break;
        }

        if (first_interm_b_value) {
            // b(1) = MD5(secret + request-authenticator + salt)
#if (MBEDTLS_VERSION_MAJOR >= 3)
            if (mbedtls_md5_update(&ctx, request_authenticator, MS_MPPE_RECV_KEY_BLOCK_LEN) != 0) {
#else
            if (mbedtls_md5_update_ret(&ctx, request_authenticator, MS_MPPE_RECV_KEY_BLOCK_LEN) != 0) {
#endif
                md5_failed = true;
                break;
            }
#if (MBEDTLS_VERSION_MAJOR >= 3)
            if (mbedtls_md5_update(&ctx, salt_ptr, MS_MPPE_RECV_KEY_SALT_LEN) != 0) {
#else
            if (mbedtls_md5_update_ret(&ctx, salt_ptr, MS_MPPE_RECV_KEY_SALT_LEN) != 0) {
#endif
                md5_failed = true;
                break;
            }
        } else {
            // b(i) = MD5(secret + cipher_text(i - 1))
#if (MBEDTLS_VERSION_MAJOR >= 3)
            if (mbedtls_md5_update(&ctx, cipher_text_ptr - MS_MPPE_RECV_KEY_BLOCK_LEN, MS_MPPE_RECV_KEY_BLOCK_LEN) != 0) {
#else
            if (mbedtls_md5_update_ret(&ctx, cipher_text_ptr - MS_MPPE_RECV_KEY_BLOCK_LEN, MS_MPPE_RECV_KEY_BLOCK_LEN) != 0) {
#endif
                md5_failed = true;
                break;
            }
        }

#if (MBEDTLS_VERSION_MAJOR >= 3)
        if (mbedtls_md5_finish(&ctx, interm_b_val) != 0) {
#else
        if (mbedtls_md5_finish_ret(&ctx, interm_b_val) != 0) {
#endif
            md5_failed = true;
            break;
        }
        mbedtls_md5_free(&ctx);

        first_interm_b_value = false;

        for (uint8_t index = 0; index < MS_MPPE_RECV_KEY_BLOCK_LEN; index++) {
            *plain_text_ptr++ = *cipher_text_ptr++ ^ interm_b_val[index];
        }

        cipher_text_len -= MS_MPPE_RECV_KEY_BLOCK_LEN;
    }

    if (md5_failed) {
        mbedtls_md5_free(&ctx);
    }

    if (md5_failed || cipher_text_len != 0) {
        return -1;
    }

    memcpy(pmk_ptr, plain_text + 1, PMK_LEN);

    return 0;
#else
    (void) prot;
    (void) recv_key;
    (void) recv_key_len;
    (void) request_authenticator;
    (void) pmk_ptr;

    tr_error("FATAL: MD5 MBEDTLS_MD5_C not enabled");

    return -1;
#endif
}

static void radius_client_sec_prot_finished_send(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);
    prot->timer_start(prot);
    sec_prot_state_set(prot, &data->common, RADIUS_STATE_FINISHED);
}

static void radius_client_sec_prot_timer_timeout(sec_prot_t *prot, uint16_t ticks)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    sec_prot_timer_timeout_handle(prot, &data->common, &prot->sec_cfg->radius_cfg->radius_retry_trickle_params, ticks);
}

static void radius_client_sec_prot_state_machine(sec_prot_t *prot)
{
    radius_client_sec_prot_int_t *data = radius_client_sec_prot_get(prot);

    switch (sec_prot_state_get(&data->common)) {
        case RADIUS_STATE_INIT:
            tr_info("Radius: init");
            sec_prot_state_set(prot, &data->common, RADIUS_STATE_STATE_RESPONSE_ID);
            prot->timer_start(prot);
            break;

        // Wait EAP response, Identity (starts RADIUS Client protocol)
        case RADIUS_STATE_STATE_RESPONSE_ID:
            tr_info("Radius: start, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Set default timeout for the total maximum length of the negotiation
            sec_prot_default_timeout_set(&data->common);

            sec_prot_state_set(prot, &data->common, RADIUS_STATE_CREATE_RESP);

            if (radius_client_sec_prot_init_radius_eap_tls(prot) < 0) {
                tr_error("Radius: EAP-TLS init failed");
                return;
            }

            radius_client_sec_prot_allocate_and_create_radius_message(prot);

            // Send KMP-CREATE.indication
            prot->create_ind(prot);
            break;

        // Wait KMP-CREATE.response
        case RADIUS_STATE_CREATE_RESP:
            if (sec_prot_result_ok_check(&data->common)) {
                sec_prot_state_set(prot, &data->common, RADIUS_STATE_SEND_INITIAL_ACCESS_REQUEST);
                prot->state_machine_call(prot);
            } else {
                // Ready to be deleted
                sec_prot_state_set(prot, &data->common, RADIUS_STATE_FINISHED);
            }
            break;

        case RADIUS_STATE_SEND_INITIAL_ACCESS_REQUEST:
            tr_info("Radius: send initial access request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            if (radius_client_sec_prot_radius_msg_send(prot) < 0) {
                tr_error("Radius: msg send error");
            }

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &prot->sec_cfg->radius_cfg->radius_retry_trickle_params);

            sec_prot_state_set(prot, &data->common, RADIUS_STATE_ACCESS_ACCEPT_REJECT_CHALLENGE);
            break;

        case RADIUS_STATE_ACCESS_ACCEPT_REJECT_CHALLENGE:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                tr_info("Radius: retry access request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
                if (radius_client_sec_prot_radius_msg_send(prot) < 0) {
                    tr_error("Radius: retry msg send error");
                }
                return;
            }

            tr_info("Radius: received access accept/reject/challenge, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            // Free radius access-request buffer since answer received and retries not needed
            radius_client_sec_prot_radius_msg_free(prot);

            // Stop trickle timer, EAP-TLS will continue and on reject/accept negotiation will end
            sec_prot_timer_trickle_stop(&data->common);

            // Set timeout to wait for EAP-TLS to continue
            data->common.ticks = prot->sec_cfg->prot_cfg.sec_prot_retry_timeout;

            // Send to radius EAP-TLS
            if (data->radius_eap_tls_send && data->radius_eap_tls_prot && data->recv_eap_msg && data->recv_eap_msg_len > 0) {
                data->radius_eap_tls_send(data->radius_eap_tls_prot, (void *) data->recv_eap_msg, data->recv_eap_msg_len);
            } else {
                if (data->recv_eap_msg) {
                    ns_dyn_mem_free(data->recv_eap_msg);
                }
            }
            data->recv_eap_msg = NULL;
            data->recv_eap_msg_len = 0;

            if (data->radius_code == RADIUS_ACCESS_REJECT) {
                sec_prot_result_set(&data->common, SEC_RESULT_ERROR);
                sec_prot_state_set(prot, &data->common, RADIUS_STATE_FINISH);
            } else if (data->radius_code == RADIUS_ACCESS_ACCEPT) {
                if (data->new_pmk_set) {
                    sec_prot_result_set(&data->common, SEC_RESULT_OK);
                } else {
                    sec_prot_result_set(&data->common, SEC_RESULT_ERROR);
                }
                sec_prot_state_set(prot, &data->common, RADIUS_STATE_FINISH);
            } else if (data->radius_code == RADIUS_ACCESS_CHALLENGE) {
                sec_prot_state_set(prot, &data->common, RADIUS_STATE_SEND_ACCESS_REQUEST);
            }
            break;

        case RADIUS_STATE_SEND_ACCESS_REQUEST:

            // On timeout
            if (sec_prot_result_timeout_check(&data->common)) {
                tr_info("Radius: retry access request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));
                if (radius_client_sec_prot_radius_msg_send(prot) < 0) {
                    tr_error("Radius: retry msg send error");
                }
                return;
            }

            if (data->radius_code != RADIUS_MESSAGE_NONE) {
                // Received retry for already handled message from RADIUS server, ignore
                if (data->recv_eap_msg) {
                    ns_dyn_mem_free(data->recv_eap_msg);
                }
                data->recv_eap_msg = NULL;
                data->recv_eap_msg_len = 0;
                return;
            }

            tr_info("Radius: send access request, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            radius_client_sec_prot_allocate_and_create_radius_message(prot);

            if (radius_client_sec_prot_radius_msg_send(prot) < 0) {
                tr_error("Radius: msg send error");
            }

            // Start trickle timer to re-send if no response
            sec_prot_timer_trickle_start(&data->common, &prot->sec_cfg->radius_cfg->radius_retry_trickle_params);

            sec_prot_state_set(prot, &data->common, RADIUS_STATE_ACCESS_ACCEPT_REJECT_CHALLENGE);
            break;

        case RADIUS_STATE_FINISH:
            tr_info("Radius: finish, eui-64: %s", trace_array(sec_prot_remote_eui_64_addr_get(prot), 8));

            if (sec_prot_result_ok_check(&data->common)) {
                sec_prot_keys_pmk_write(prot->sec_keys, data->new_pmk, prot->sec_cfg->timer_cfg.pmk_lifetime);
                // Supplicant PMK is now valid
                sec_prot_keys_pmk_mismatch_reset(prot->sec_keys);
                /* Calls KMP-FINISHED.indication with ignore results because next
                   protocol is triggered from radius EAP-TLS */
                sec_prot_result_set(&data->common, SEC_RESULT_IGNORE);
            }

            // KMP-FINISHED.indication
            prot->finished_ind(prot, sec_prot_result_get(&data->common), prot->sec_keys);
            sec_prot_state_set(prot, &data->common, RADIUS_STATE_FINISHED);

            break;

        case RADIUS_STATE_FINISHED: {
            uint8_t *remote_eui_64 = sec_prot_remote_eui_64_addr_get(prot);
            tr_info("Radius: finished, eui-64: %s", remote_eui_64 ? trace_array(remote_eui_64, 8) : "not set");

            radius_client_sec_prot_identifier_free(prot);

            // Indicate to radius EAP-TLS peer protocol that radius client has been deleted
            if (data->radius_eap_tls_deleted) {
                data->radius_eap_tls_deleted(data->radius_eap_tls_prot);
            }

            prot->timer_stop(prot);
            prot->finished(prot);
            break;
        }

        default:
            break;
    }
}

#endif /* HAVE_WS */
