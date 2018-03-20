/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "ns_types.h"
#include "eventOS_event.h"
#include "ns_trace.h"
#include "string.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ccmLIB.h"
#include "shalib.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "Common_Protocols/udp.h"

#ifdef ECC
#include    "ecc.h"
#endif
#include "common_functions.h"
#include "Security/PANA/pana_nvm.h"
#include "Security/PANA/pana_avp.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/PANA/pana_header.h"
#include "Security/PANA/eap_protocol.h"
#include "net_pana_parameters_api.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "6LoWPAN/NVM/nwk_nvm.h"

#ifdef PANA
#ifdef PANA_SERVER_API

#define TRACE_GROUP "PanS"
static pana_server_base_t *NS_LARGE pana_server_base = NULL;
static pana_server_update_cb *pana_nvm_storage_cb = NULL;
static pana_server_session_get_cb *pana_server_nvm_get = NULL;
static pana_server_session_get_by_id_cb *pana_server_nvm__session_get = NULL;
static void (*pana_key_update_process_ready)(void) = 0;
static uint8_t *pana_nvm_buffer = 0;

static void pana_server_packet_handler(buffer_t *buf);
static int8_t pana_server_init(int8_t nwk_id, const uint8_t *network_key_material, uint8_t supported_chipher_suites, uint32_t key_update_delay);
static pana_server_base_t * pana_server_staructure_allocate(void);
static pana_key_material_t *pana_server_key_get(bool primary);
static void pana_server_set_random_key_value(uint8_t *network_key_material);
static void pana_server_material_read(const uint8_t *ptr);
static void pana_server_material_write(uint8_t *ptr);
static void pana_server_client_save_to_nvm(sec_suite_t *suite, pana_nvm_update_process_t nvm_event);
static uint8_t *pana_avp_zip_key_material(uint8_t *dptr, uint8_t *key_info, uint32_t message_seq, sec_suite_t *suite);
static void pana_client_session_init(sec_suite_t *suite);

static bool pana_auth_msg_validate(uint8_t *ptr, uint16_t length, uint8_t *key);
static void pana_pna_response_build(buffer_t *buf, uint8_t operation, pana_header_t *header, sec_suite_t *suite);
static void pana_client_authentication_fail(sec_suite_t *suite);
static void pana_key_update_delivery_ready(void);
static void pana_server_build_key_push(buffer_t *buf, sec_suite_t *suite);
static void pana_success_server_build(buffer_t *buf, sec_suite_t *suite);
static void pana_server_state_machine_func(sec_suite_t *suite);


static uint8_t *pana_avp_zip_key_material(uint8_t *dptr, uint8_t *key_info, uint32_t message_seq, sec_suite_t *suite)
{
    //SET AVP BASE
    dptr = pana_avp_base_write(AVP_ENCRYPT_ALGORITHM_CODE, 32, dptr, 0, 0);
    uint8_t *ptr = dptr;
    dptr = pana_avp_vendor_id_write_n_bytes(PANA_EAP_KEYWRAP_TYPE, 18, key_info, dptr, ZIGBEE_VENDOR_ID);
    //Crypt Key Info
    pana_ccm_data_crypt(ptr, 32, AES_CCM_ENCRYPT, message_seq, suite);
    return dptr;
}

static pana_server_base_t * pana_server_staructure_allocate(void)
{
    pana_server_base_t *server = ns_dyn_mem_alloc(sizeof(pana_server_base_t));
    if (server) {
        memset(server, 0, sizeof(pana_server_base_t));
    }
    return server;
}

static pana_key_material_t *pana_server_key_get(bool primary)
{
    if (primary) {
        return &pana_server_base->sec_key_material[pana_server_base->primary_material];
    }

    if (pana_server_base->primary_material) {
        return    &pana_server_base->sec_key_material[0];
    }

    return &pana_server_base->sec_key_material[1];
}

static void pana_server_set_random_key_value(uint8_t *network_key_material)
{
    randLIB_get_n_bytes_random(network_key_material, 16);
}

static void pana_server_material_write(uint8_t *ptr)
{
    pana_key_material_t *key_ptr;
    ptr = common_write_32_bit(pana_server_base->pana_key_id, ptr);
    ptr = common_write_32_bit(pana_server_base->session_lifetime, ptr);
    ptr = common_write_32_bit(pana_server_base->key_wrap, ptr);
    ptr = common_write_32_bit(pana_server_base->prf_algorythm, ptr);
    ptr = common_write_32_bit(pana_server_base->integrity_algorythm, ptr);
    ptr = common_write_32_bit(pana_server_base->next_session_id, ptr);
    ptr = common_write_32_bit(pana_server_base->key_update_delay, ptr);
    ptr = common_write_32_bit(pana_server_base->pana_key_update_delay_used, ptr); //32
    *ptr++ = pana_server_base->supported_chipher_suites;
    *ptr++ = pana_server_base->auth_cnt;
    *ptr++ = pana_server_base->primary_material;
    key_ptr = &pana_server_base->sec_key_material[0];
    *ptr++ = key_ptr->key_id;
    memcpy(ptr, key_ptr->key_material, 16);
    ptr += 16;//52
    key_ptr = &pana_server_base->sec_key_material[1];
    *ptr++ = key_ptr->key_id;
    memcpy(ptr, key_ptr->key_material, 16);
    ptr += 16;  //69

    if (pana_server_base->pana_key_update) {
        pana_key_update_t *pku = pana_server_base->pana_key_update;
        *ptr++ = 1;
        *ptr++ = pku->key_id;
        ptr = common_write_16_bit(pku->key_delivery_cnt, ptr); //73
        memcpy(ptr, pku->new_key_material, 16); // 89
    } else {
        *ptr = 0; //70
    }

}

static void pana_server_material_read(const uint8_t *ptr)
{
    pana_key_material_t *key_ptr;
    //Set Stored setup
    pana_server_base->pana_key_id = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->session_lifetime = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->key_wrap = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->prf_algorythm = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->integrity_algorythm = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->next_session_id = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->key_update_delay = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->pana_key_update_delay_used = common_read_32_bit(ptr);
    ptr += 4;
    pana_server_base->supported_chipher_suites = *ptr++;
    pana_server_base->auth_cnt = *ptr++;
    pana_server_base->primary_material = *ptr++;

    key_ptr = &pana_server_base->sec_key_material[0];
    key_ptr->key_id = *ptr++;
    memcpy(key_ptr->key_material, ptr, 16);
    tr_debug("KID0: %02x, %s", key_ptr->key_id, trace_array(ptr, 16));
    ptr += 16;

    key_ptr = &pana_server_base->sec_key_material[1];
    key_ptr->key_id = *ptr++;
    memcpy(key_ptr->key_material, ptr, 16);
    tr_debug("KID1: %02x, %s", key_ptr->key_id, trace_array(ptr, 16));
    ptr += 16;

    uint8_t key_update = *ptr++;
    if (key_update) {
        pana_key_update_t *pku = ns_dyn_mem_temporary_alloc(sizeof(pana_key_update_t));
        if (pku) {
            pku->key_id = *ptr++;
            pku->key_delivery_cnt = common_read_16_bit(ptr);
            ptr += 2;
            memcpy(pku->new_key_material, ptr, 16);
            pana_server_base->pana_key_update = pku;
            tr_debug("Key Update Process active");
        }
    }
}


static int8_t pana_server_init(int8_t nwk_id, const uint8_t *network_key_material, uint8_t supported_chipher_suites, uint32_t key_update_delay)
{
    if (!pana_socket_init(pana_server_packet_handler, pana_server_state_machine_func, tls_server_up)) {
        return -1;
    }

    int8_t ret_val = -1;
    sec_suite_list_clean();

    //Allocate
    if (pana_server_base == 0) {
        pana_server_base = pana_server_staructure_allocate();
    } else {
        if (pana_server_base->network_interface_id != nwk_id) {
            tr_debug("UnKnow id");
            return -2;
        }
    }
    if (pana_server_base) {

        pana_server_base->primary_material = 0;
        pana_key_material_t *key_info = pana_server_key_get(false);
        key_info->key_id = 0;
        key_info = pana_server_key_get(true);


        key_info->key_id = 1;
        if (network_key_material) {
            memcpy(key_info->key_material, network_key_material, 16);
        } else {
            pana_server_set_random_key_value(key_info->key_material);
        }
        pana_server_base->auth_cnt = 0;
        pana_server_base->pana_key_id = 1;
        pana_server_base->session_lifetime = 0xffffffff;
        pana_server_base->key_wrap = 1;
        pana_server_base->prf_algorythm = 5;
        pana_server_base->integrity_algorythm = 0x0c;
        pana_server_base->next_session_id = 1;
        pana_server_base->key_update_delay = 0;
        pana_server_base->supported_chipher_suites = supported_chipher_suites;
        pana_server_base->pana_key_update = 0;
        pana_server_base->open_pana_authentication_cnt = 0;
        pana_server_base->network_interface_id = nwk_id;
        pana_server_base->pana_key_update_delay_used = key_update_delay;
        if (pana_nvm_storage_cb) {
            pana_server_material_write(pana_nvm_buffer);
            pana_nvm_storage_cb(PANA_SERVER_MATERIAL_UPDATE);
        }
        ret_val = 0;
    }
    return ret_val;
}

void pana_key_update_delivery_ready(void)
{
    if (pana_server_base && pana_server_base->pana_key_update) {
        if (pana_server_base->pana_key_update->key_delivery_cnt-- == 1) {
            if (pana_key_update_process_ready) {
                pana_server_base->key_update_delay = 1;
            } else {
                pana_server_base->key_update_delay = pana_server_base->pana_key_update_delay_used;
                if (pana_server_base->key_update_delay == 0) {
                    tr_debug("Delivery Ready");
                    pana_server_base->key_update_delay = 1;
                }
            }
            //NVM UPDAte for timer and key material
        }
        if (pana_nvm_storage_cb) {
            pana_server_material_write(pana_nvm_buffer);
            pana_nvm_storage_cb(PANA_SERVER_MATERIAL_UPDATE);
        }

    }
}

/* Save and load stored settings to Pana server core */
static void pana_server_client_save_to_nvm(sec_suite_t *suite, pana_nvm_update_process_t nvm_event)
{
    uint8_t *ptr = pana_nvm_buffer;
    pana_session_t *p_session = &suite->pana_session;
    /* static Part of every session */
    ptr = common_write_16_bit(p_session->nvm_offset, ptr);
    memcpy(ptr, suite->session_address, 16);
    ptr += 16;
    ptr = common_write_16_bit(suite->session_port, ptr);
    ptr = common_write_32_bit(p_session->session_id, ptr);
    if (nvm_event == PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE) {
        tr_debug("Remove From NVM");
    } else {
        /* First Encrypted sector start */
        *ptr++ = suite->state;
        ptr = common_write_32_bit(suite->timer, ptr);
        uint8_t state = 0;
        if (p_session->key_warp) {
            state |= PANA_ST_KEY_WRAP;
        }

        if (p_session->session_ready) {
            state |= PANA_ST_READY;
        }
        *ptr++ = state; //6
        ptr = common_write_32_bit(p_session->req_seq, ptr);
        ptr = common_write_32_bit(p_session->res_seq, ptr);
        *ptr++ = p_session->address_status;
        memcpy(ptr, p_session->session_relay_address, 16);
        ptr += 16;
        ptr = common_write_16_bit(p_session->relay_port, ptr); //37 bytes sector
        *ptr++ = p_session->auth_cnt;
        *ptr++ = p_session->nwk_key_id;
        memcpy(ptr, p_session->pana_auth_key, 32);
        ptr += 32;
        memcpy(ptr, p_session->pana_PAA_enc_key, 16);
        ptr += 16;
        ptr = common_write_32_bit(p_session->pana_key_id, ptr);
        ptr = common_write_32_bit(p_session->session_lifetime, ptr);
        *ptr++ = p_session->eap_id_seq;

    }
}

static void pana_server_client_restore_from_nvm(sec_suite_t *suite, const uint8_t *ptr)
{

    pana_session_t *p_session = &suite->pana_session;
    p_session->nvm_offset = common_read_16_bit(ptr);
    ptr += 2;
    memcpy(suite->session_address, ptr, 16);
    ptr += 16;
    suite->session_port = common_read_16_bit(ptr);
    ptr += 2;
    p_session->session_id = common_read_32_bit(ptr);
    ptr += 4; //24

    suite->state = (sec_state_machine_t) * ptr++;
    suite->timer = common_read_32_bit(ptr);
    ptr += 4;
    uint8_t state = *ptr++;
    p_session->user_server = true;
    if (state & PANA_ST_KEY_WRAP) {
        p_session->key_warp = true;
    } else {
        p_session->key_warp = false;
    }

    if (state & PANA_ST_READY) {
        p_session->session_ready = true;
    } else {
        p_session->session_ready = false;
    }

    p_session->req_seq = common_read_32_bit(ptr);
    ptr += 4;
    p_session->res_seq = common_read_32_bit(ptr);
    ptr += 4;
    p_session->address_status = *ptr++;
    memcpy(p_session->session_relay_address, ptr, 16);
    ptr += 16;
    p_session->relay_port = common_read_16_bit(ptr);
    ptr += 2;
    p_session->auth_cnt = *ptr++;
    p_session->nwk_key_id = *ptr++;
    memcpy(p_session->pana_auth_key, ptr, 32);
    ptr += 32;
    memcpy(p_session->pana_PAA_enc_key, ptr, 16);
    ptr += 16;
    p_session->pana_key_id = common_read_32_bit(ptr);
    ptr += 4;
    p_session->session_lifetime = common_read_32_bit(ptr);
    ptr += 4;
    p_session->eap_id_seq = *ptr++;
}

void pana_session_nvm_udate(sec_suite_t *suite, pana_nvm_update_process_t update_event)
{
    if (pana_nvm_storage_cb) {
        if (pana_server_base) {
            pana_server_client_save_to_nvm(suite, update_event);
            suite->pana_session.nvm_offset = pana_nvm_storage_cb(update_event);
        }
    }
}

static void pana_client_session_init(sec_suite_t *suite)
{
    suite->pana_session.session_ready = false;
    suite->pana_session.key_warp = true;
    suite->pana_session.user_server = true;
    suite->pana_session.session_id = pana_server_base->next_session_id; // Take Random Number
    pana_server_base->next_session_id++;
    suite->pana_session.req_seq = randLIB_get_32bit();
    suite->pana_session.res_seq = 0;
    suite->pana_session.eap_id_seq = randLIB_get_8bit();
    suite->pana_session.eap_id_seq &= 63;
    suite->pana_session.eap_id_seq++;


    suite->pana_session.prf_algorythm = pana_server_base->prf_algorythm;
    suite->pana_session.integrity_algorythm = pana_server_base->integrity_algorythm;
    suite->pana_session.key_wrap = pana_server_base->key_wrap;

    //init eap fragmentation
    suite->pana_session.pana_key_id = pana_server_base->pana_key_id;
    suite->pana_session.session_lifetime = pana_server_base->session_lifetime;
    suite->supported_chipher_suites = pana_server_base->supported_chipher_suites;

    //suite->setups &= ~TLS_HANSHAKE_HASH;
    suite->setups = TLS_SERVER_MODE;
    suite->timer = 1;
    suite->state = PANA_REQUEST_TX;
    suite->retry_counter = 0;
    eap_fragmentation_init(suite);
    suite->pana_session.pana_heap->handshake_len = 0;
    suite->pana_session.pana_heap->handshake_req_offset = 0;
    randLIB_get_n_bytes_random(suite->pana_session.pana_heap->agent_nonce, 16);

    if (pana_nvm_storage_cb) {
        pana_server_material_write(pana_nvm_buffer);
        pana_nvm_storage_cb(PANA_SERVER_MATERIAL_UPDATE);
    }
}



static bool pana_auth_msg_validate(uint8_t *ptr, uint16_t length, uint8_t *key)
{
    pana_avp_t authency;
    authency.code = AVP_AUTHENCY_CODE;

    if (!pana_avp_discover(ptr, length, &authency)) {
        return false;
    }

    if (!pana_auth_check(ptr, length, authency.avp_ptr, key)) {
        tr_debug("Auth Fail");
        return false;
    }
    return true;
}

static void pana_server_set_key_material(uint8_t *key_material, bool new_key, uint8_t auth_cnt)
{

    if (new_key) {
        memcpy(key_material, pana_server_base->pana_key_update->new_key_material, 16);
        key_material += 16;
        *key_material++ = pana_server_base->pana_key_update->key_id;

    } else {
        pana_key_material_t *primary_key = pana_server_key_get(true);
        memcpy(key_material, primary_key->key_material, 16);
        key_material += 16;
        *key_material++ = primary_key->key_id;
    }
    *key_material = auth_cnt;
}

static void pana_pna_response_build(buffer_t *buf, uint8_t operation, pana_header_t *header, sec_suite_t *suite)
{
    uint8_t *ptr;

    header->type = PANA_MSG_PNA;
    header->flags &= ~PANA_FLAGS_REQUEST;

    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);

    if (operation == 4) {
        //Leave emtpy
    } else {
        uint8_t key_material[18];
        if (operation == 3) {
            pana_server_set_key_material(key_material, true, suite->pana_session.auth_cnt);
        } else {
            pana_server_set_key_material(key_material, false, suite->pana_session.auth_cnt);
        }
        ptr = pana_avp_zip_key_material(ptr, key_material, header->seq, suite);
    }

    ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);

    buffer_data_end_set(buf, ptr);
    buf = build_pana_base(buf, header, suite);
    if (!buf) {
        return;
    }

    pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);

    //Encode Pana Auth
    if (buf->src_sa.address[0] == 0xfe) {
        pana_set_agend_address(buf, false, suite);
    } else {
        //Check IF need Relay
        pana_set_agend_address(buf, true, suite);
    }

    if (suite->pana_session.address_status & 1) {
        buf = pana_relay_avp_build(buf, suite);
        if (buf) {
            header->flags = 0;
            header->type = PANA_MSG_RELAY;
            header->session_id = 0;
            header->seq = 0;
            buf = build_pana_base(buf, header, suite);
        }
    }

    protocol_push(buf);
}


static void pana_complete_msg_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{

    uint16_t length = buffer_data_length(buf);
    uint8_t *ptr = buffer_data_pointer(buf);

    if (sec_check_suite_ptrs(suite) == 0) {
        buffer_free(buf);
        return;
    }

    if (header->flags & PANA_FLAGS_REQUEST) {
        buffer_free(buf);
        return;
    }


    uint32_t key_id = 0xffffffff;

    bool key_id_parsed = false;
    pana_avp_t avp_temp;
    //Read Key id if they are coming


    avp_temp.code = AVP_KEY_ID_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp) &&  avp_temp.len == 4) {
        key_id= common_read_32_bit(avp_temp.avp_ptr);
        key_id_parsed = true;
    }



    if (key_id_parsed) {

        if (!pana_auth_msg_validate(ptr, length, suite->pana_session.pana_auth_key)) {
            goto pana_failure;
        }

        if (suite->pana_session.pana_key_id != key_id) {
            goto pana_failure;
        }
        tr_debug("Client AUTH_OK");
        sec_lib_state_machine_trig(suite, PANA_READY);
        if (pana_server_base) {
            if (pana_server_base->open_pana_authentication_cnt) {
                pana_server_base->open_pana_authentication_cnt--;
            }
        }
    } else {
        sec_lib_state_machine_trig(suite, PANA_ERROR);
    }

    buffer_free(buf);
    return;

    pana_failure:
    tr_debug("Drop Key MSG");
    sec_lib_state_machine_trig(suite, PANA_FAILURE); //shuold be calc
    buffer_free(buf);
    return;
}

static void pana_server_finnish_error_build(buffer_t *buf, sec_suite_t *suite)
{
    buf->buf_ptr = PANA_HEADER_LENGTH;
    uint8_t *ptr = buffer_data_pointer(buf);
    //tr_debug("End Pana and EAP");
    uint8_t eap_status[4];
    eap_header_build(eap_status, 4,EAP_FAILURE, suite->pana_session.eap_id_seq, 0);

    ptr = pana_avp_32_bit_write(AVP_RESULT_CODE, 2, ptr);
    ptr = pana_avp_write_n_bytes(AVP_EAP_PAYLOAD_CODE, 4, eap_status, ptr);
    buffer_data_end_set(buf, ptr);
}

static void sec_auth_ready(sec_suite_t *suite) {
    suite->timer = 0;
    tr_debug("Pana:OK");
    suite->pana_session.session_ready = true;
    if (suite->state == PANA_READY) {
        pana_session_nvm_udate(suite, PANA_SERVER_CLIENT_SESSION_UPDATE);
    }

    //Reset pointer
    suite->pana_session.auth_info = NULL;
    pana_free_dynamic_ram(suite);
    sec_suite_tls_free(suite, true);

}
#ifdef ECC
static uint32_t tls_backoff_random_timer_start(void)
{
    return randLIB_get_random_in_range(2, 16);
}
#endif

static void pana_server_pana_error_handler(sec_suite_t *suite)
{
    sec_lib_state_machine_lock(suite, PANA_ERROR);
    pana_client_authentication_fail(suite);
    seclib_session_clean(suite);
}


static void pana_server_state_machine_func(sec_suite_t *suite)
{
    if (!suite) {
        return;
    }
    uint8_t general_tx = 0;
    switch (suite->state) {
        case PANA_ERROR:
            pana_server_pana_error_handler(suite);
            return;

        case PANA_READY:
        case PANA_RE_VALID:
        case PANA_PULL_DONE:
            sec_auth_ready(suite);
            break;

        case PRF_CALC:
        case PRF_CALC2:
        case TLS_ECC_CERTIFICATE_VERIFY_SIGNATURE:
        case TLS_ECC_MESSAGE_VERIFY:
        case TLS_ECC_CERTIFICATE_SIGNATURE_CHECK:
        case TLS_ECC_GENERATE_PUBLIC_KEY:
        case TLS_ECC_GENERATE_PREMASTER_SECRET:
        case TLS_ECC_SIGNATURE_MESSAGE:
            break;
        case TLS_HELLO_RX:
        case TLS_SERVER_KEY_EXCHANGE_RX:
        case TLS_SERVER_WAIT_CHANGE_CHIPHERSUITE:
        case TLS_CLIENT_KEY_EXCHANGE_RX:
        case TLS_CHANGE_CHIPHER:
            tr_debug("%02x", suite->state);
            tr_debug("Timeout");
            sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
            general_tx = 1;
            break;

#ifdef ECC
        case TLS_ECC_GENERATE_PUBLIC_KEY_START:
            sec_ecc_gen_public_key_start(suite);
            break;

        case TLS_ECC_MESSAGE_SERVER_VERIFY_START:
        case TLS_ECC_MESSAGE_VERIFY_START:
            if (suite->tls_session) {
                if (suite->tls_session->tls_heap) {
                    int start = 0;
                    tls_heap_t *theap = suite->tls_session->tls_heap;

                    if (theap->cert_temp_buf) {
                        if (suite->state == TLS_ECC_MESSAGE_SERVER_VERIFY_START) {
                            if (theap->signature_temp_buf == 0) {
                                start = 1;
                            }
                        }
                    } else {
                        start = 1;
                    }
                    if (start) {
                        tls_server_finnish_handle_start(suite);
                    } else {
                        tr_debug("Start Certi Check");
                        tls_certificate_signature_verify(suite);
                    }
                }
            }
            break;
        case TLS_ECC_MESSAGE_VERIFY_START2:
            tls_ecc_verfify_start(suite);
            break;

        case TLS_SERVER_ECC_PUB_KEY_GEN:
            if (sec_auth_re_check(suite)) {
                if (tls_server_certi_hash_copy(suite) == 0) {
                    suite->timer = tls_backoff_random_timer_start();
                } else {

                    suite->setups &= ~TLS_HANSHAKE_HASH;
                    //PUBLIC key cal
                    sec_lib_state_machine_trig(suite, TLS_ECC_GENERATE_PUBLIC_KEY_START);
                }
            } else {
                tr_debug("TLS_SERVER_ECC_PUB_KEY_GEN timeout");
                sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                general_tx = 1;
                //Alert Shuold send Here
            }
            break;

        case TLS_ECC_CLIENT_SIGNATURE_START:
            sec_ecc_client_signature_start(suite);
            break;
#endif
        case TLS_UPDATE_HAS_WITH_CERTIFICATE:
#ifdef ECC
            if (sec_auth_re_check(suite)) {
                if (tls_certi_hash_copy(suite) == 0) {
                    tr_warn("Server Certficate Alloc fail");
                    suite->timer = 4;
                } else {
                    sec_lib_state_machine_trig(suite, TLS_ECC_CLIENT_SIGNATURE_START);
                }
            } else
#endif
            {
                sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                general_tx = 1;
            }
            break;

        case TLS_TX_SERVER_KEY_EXCHANGE:
#ifdef ECC
            if (sec_auth_re_check(suite)) {
                bool tx_start_OK = false;
                if (suite->pana_session.assy_length || suite->pana_session.frag_length || suite->pana_session.packet_delivered) {
                    //Build next EAP Packet
                    //tr_debug("TX same again fragment piece");
                    tx_start_OK = pana_eap_frag_re_tx(suite);

                } else {
                    if (tls_pana_server_exchange_build(suite)) {
                        suite->pana_session.packet_delivered = false;
                        tx_start_OK = true;
                    }

                }
                if (tx_start_OK) {
                    pana_timeout_timer_set(suite, suite->state);
                } else {
                    suite->timer = tls_backoff_random_timer_start();
                }
            } else
#endif
            {
                if (suite->pana_session.assy_length) {
                    buffer_free(suite->pana_session.eap_assy_buf);
                    suite->pana_session.eap_assy_buf = 0;
                    suite->pana_session.assy_length = 0;
                    suite->pana_session.assy_off_set = 0;
                }
                tr_debug("TLS_TX_SERVER_KEY_EXCHANGE timeout");
                sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                general_tx = 1;
            }
            break;


        default:
            general_tx = 1;
            break;
    }

    if (general_tx) {
        if (sec_auth_re_check(suite)) {
            buffer_t *buf = buffer_get(140);
            if (buf) {
                buf->interface = suite->interface;
                suite->timer = 600;
                switch (suite->state) {
                    case PANA_REQUEST_TX:
                        pana_start_message_build(buf, suite);
                        break;

                    case EAP_IDENTITY_REQ:
                        pana_eap_identity_build(buf, suite);
                        break;

                    case TLS_START:
                        pana_eap_tls_start_build(buf, suite);
                        break;

                    case TLS_SERVER_TX_SERVER_HELLO:
                        tls_server_hello_build(buf, suite);
                        pana_eap_down(buf, suite);
                        break;

                    case TLS_KEY_CHANGE:
                        //Print Handshake message
                        tls_prepare_change_chipher_spec(suite);

                        tls_build_client_change_chipher_suite_finnish(buf, suite);
                        tls_nonce_update(suite->tls_session->tls_nonce_explit);
                        tr_debug("Set Keys");
                        pana_eap_down(buf, suite);

                        break;

                    case TLS_EAP_END_PANA_VERIFY:
                        pana_success_server_build(buf, suite);
                        break;


                    case PANA_KEY_UPDATE:
                        pana_server_build_key_push(buf, suite);
                        break;

                    case PANA_FAILURE:
                        pana_server_finnish_error_build(buf, suite);
                        pana_down(buf, suite);

                        break;

                    case TLS_FINISH:
                    case TLS_ALERT:
                        eap_fragmentation_init(suite);
                        pana_eap_tls_finnish_build(buf, suite);
                        break;


                    case TLS_ALERT_CLOSE_FATAL:
                    case TLS_ALERT_INTERNAL:
                    case TLS_ALERT_CHIPHER_SUITE:
                    case TLS_ALERT_DECRYPT:
                    case TLS_ALERT_BAD_CERTIFICATE:

                        eap_fragmentation_init(suite);

                        suite->setups &= ~(TLS_ECC_CERTIFICATE_REQUESTED | TLS_ECC_CERTIFICATE_RECEIVED | TLS_ECC_CERTIFICATE_VERIFY);
#ifdef ECC
                        {
                            tls_heap_t *tls_heap = suite->tls_session->tls_heap;
                            if (tls_heap) {
                                tls_ecc_heap_free(tls_heap);
                            }
                        }
#endif
                        if (suite->state == TLS_ALERT_DECRYPT) {
                            tls_alert_build(buf, ALERT_BAD_RECORD);
                        } else if (suite->state == TLS_ALERT_CLOSE_FATAL) {
                            tls_alert_build(buf, ALERT_INTERNAL_ERR);
                        } else if (suite->state == TLS_ALERT_BAD_CERTIFICATE) {

                            tls_alert_build(buf, ALERT_BAD_CERTIFICATE);
                        } else {
                            tls_alert_build(buf, ALERT_INTERNAL_ERR);

                        }
                        pana_eap_down(buf, suite);
                        break;


                    default:
                        tr_debug("Unknown Packet. State: %x", suite->state);
                        buf = buffer_free(buf);
                        break;
                }

            } else {
                suite->timer = 2;
            }
        } else {
            tr_debug("Tls Auth Re TX limit Reached. State: %x", suite->state);

            switch (suite->state) {
                case TLS_SERVER_TX_SERVER_HELLO:
                case TLS_KEY_CHANGE:
                    sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                    break;

                default:
                    if (suite->state == PANA_KEY_UPDATE) {
                        tr_warn("Key Delivery Fail");
                        pana_key_update_delivery_ready();
                    }
                    pana_server_pana_error_handler(suite);
                    break;
            }
        }
    }
}

static void pana_server_tls_session_init(sec_suite_t *suite)
{
    //Generate TLS SESSION
    tls_session_id_genrate(suite->tls_session->tls_session_id, 4);
    suite->tls_session->id_length = 4;
    memset(suite->tls_session->tls_nonce_explit, 0, 8);
}

static void pana_server_session_address_set_for_relay(sec_suite_t *suite, buffer_t *buf)
{
    memcpy(suite->session_address, buf->dst_sa.address, 16);
    suite->session_port = buf->dst_sa.port;
    suite->pana_session.address_status = 1;
    //Copy Relay Address
    memcpy(suite->pana_session.session_relay_address, buf->src_sa.address, 16);
    suite->pana_session.relay_port = buf->src_sa.port;
    memcpy(&(buf->src_sa), &(buf->dst_sa), sizeof(sockaddr_t));

}

static sec_suite_t *pana_server_session_restore(uint8_t *nvm_data, protocol_interface_info_entry_t *interface)
{
    sec_suite_t *suite = sec_lib_security_session_allocate(false);
    if (!suite) {
        return NULL;
    }
    //Restore
    pana_server_client_restore_from_nvm(suite, nvm_data);
    //Init Satic Information At ZIP
    suite->setups = TLS_SERVER_MODE;
    suite->interface = interface;
    suite->pana_session.key_wrap = pana_server_base->key_wrap;
    suite->pana_session.prf_algorythm = pana_server_base->prf_algorythm;
    suite->pana_session.integrity_algorythm = pana_server_base->integrity_algorythm;
    return suite;
}


static sec_suite_t * pana_server_get_session_by_session_id(uint32_t session_id, protocol_interface_info_entry_t *interface)
{
    sec_suite_t *suite = sec_suite_selected_pana_session(session_id);
    if (!suite && pana_server_nvm__session_get) {
        if (!pana_server_nvm__session_get(session_id)) {
            return NULL;
        }
        suite = pana_server_session_restore(pana_nvm_buffer, interface);
    }

    return suite;
}

static sec_suite_t *pana_server_get_session_by_address(uint8_t *ll_address, protocol_interface_info_entry_t *interface)
{
    sec_suite_t *suite =  sec_suite_selected_address(ll_address);

    if (!suite && pana_server_nvm_get) {
        if (!pana_server_nvm_get(ll_address)) {
            return NULL;
        }
        suite = pana_server_session_restore(pana_nvm_buffer, interface);
    }

    return suite;
}


static void pana_server_packet_handler(buffer_t *buf)
{
    bool relay_pack = false;
    sec_suite_t *suite =  NULL;
    pana_header_t header;
    if (!pana_header_parse(buffer_data_pointer(buf), buffer_data_length(buf), &header)) {
        buffer_free(buf);
        return;
    }
    buffer_data_strip_header(buf, PANA_HEADER_LENGTH);


    if (header.type == PANA_MSG_RELAY) {
        /** Parse Relay */
        buf = pana_relay_parse(buf);
        if (buf) {
            /** Parse Relayed Packet */
            if (!pana_header_parse(buffer_data_pointer(buf), buffer_data_length(buf), &header)) {
                buffer_free(buf);
                return;
            }
            buffer_data_strip_header(buf, PANA_HEADER_LENGTH);
            relay_pack = true;
        }
        if (buf == 0) {
            tr_debug("Drop Pana Relay Pack");
            return;
        }
    }

    //Server will use address and session when address is LL and GP use only ID
    //SERVER: if type is relay parse relay address and save
    if (header.type == PANA_MSG_PNA) {
        //tr_debug("PNA");
        suite = pana_server_get_session_by_session_id(header.session_id, buf->interface);
        if (!suite || !(header.flags & PANA_FLAGS_PING)) {
            buffer_free(buf);
            return;
        }
        if (!pana_auth_msg_validate(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key)) {
            buffer_free(buf);
            return;
        }

        if ((header.flags & PANA_FLAGS_REQUEST) == PANA_FLAGS_RESPONSE) {
            if (suite->pana_session.req_seq != header.seq) {
                tr_debug("PNA RES:Drop Old seq");
                buffer_free(buf);
                return;
            }

            suite->pana_session.req_seq++;

            if (suite->pana_session.session_ready ) {

                tr_debug("Key push OK");
                if (suite->state == PANA_KEY_UPDATE) {
                    sec_lib_state_machine_lock(suite, PANA_READY);
                    tr_debug("Key Delivery OK");
                    //Call SEC KEY UPDATE
                    //tr_debug("NVM SEQ Update by Key Pull Ready");
                    pana_session_nvm_udate(suite, PANA_SERVER_CLIENT_SESSION_UPDATE);
                    pana_key_update_delivery_ready();
                }
            }
            buffer_free(buf);
            return;
        } else {
            if (suite->pana_session.res_seq == 0) {
                suite->pana_session.res_seq = header.seq;
            } else {
                if ((suite->pana_session.res_seq + 1) == header.seq) {
                    suite->pana_session.res_seq = header.seq;
                } else if (suite->pana_session.res_seq != header.seq) {
                    tr_debug("PNA REQ:Drop unknow Request");
                    buffer_free(buf);
                    return;
                }
            }
        }

        if (relay_pack) {
            pana_server_session_address_set_for_relay(suite, buf);

        } else {
            if ((buf->src_sa.address[0] != 0xfe)) {
                //tr_debug("SET GP & SEQ");
                suite->pana_session.address_status = 0;
                memcpy(suite->pana_session.session_relay_address, buf->src_sa.address, 16);
                suite->pana_session.relay_port = buf->src_sa.port;
                suite->session_port = buf->src_sa.port;

            } else {
                if (memcmp(suite->session_address, buf->src_sa.address, 16) != 0) {
                    tr_debug("PNA REQ:Drop By Address");
                    buffer_free(buf);
                    return;
                }

                //Shuold Update Relay Information
                suite->pana_session.address_status = 2;
                suite->session_port = buf->src_sa.port;
                lowpan_neighbour_data_clean(suite->interface->id, buf->src_sa.address);
            }
        }

        if (!suite->pana_session.session_ready) {
            buffer_free(buf);
            return;
        }

        //KEY PULL
        pana_avp_t avp_temp;
        //Read Resul and Key id if they are coming
        avp_temp.code = PANA_EAP_KEYREQ_TYPE;
        avp_temp.len = 0;
        uint8_t key_delivery;
        if (!pana_avp_discover(buffer_data_pointer(buf), buffer_data_length(buf), &avp_temp)) {
            key_delivery = 2;
            avp_temp.avp_ptr = NULL;
        } else {
            if (avp_temp.len != 2) {
                buffer_free(buf);
                return;
            }
            uint8_t *ptr = avp_temp.avp_ptr;
            pana_key_material_t *primary_key = pana_server_key_get(true);
            tr_debug("Network KEY Request: %s", trace_array(ptr, 2));
            if (*ptr++ & 1) {
                tr_debug("REQ bit, 1");
                //if(*ptr == pana_server_base->network_key_id)
                if (*ptr == primary_key->key_id) {
                    if (pana_server_base->pana_key_update) {
                        key_delivery = 3;
                    } else {
                        key_delivery = 4;
                    }
                } else {
                    key_delivery = 2;
                }

            } else {
            //if(*ptr == pana_server_base->network_key_id)
                if (*ptr == primary_key->key_id) {
                    key_delivery = 2;
                } else {
                    key_delivery = 4;
                }
            }
        }

        tr_debug("NVM  SEQ update BY Client key Pull");
        pana_pna_response_build(buf, key_delivery, &header, suite);
        pana_session_nvm_udate(suite, PANA_SERVER_CLIENT_SESSION_UPDATE);
        return;
    } else if (header.type == PANA_MSG_PCI) {
        if (relay_pack) {
            suite =  pana_server_get_session_by_address(buf->dst_sa.address, buf->interface);
        } else {
            suite =  pana_server_get_session_by_address(buf->src_sa.address, buf->interface);
        }
        if (suite) {
            //Check State
            //if state ready create new session
            if (!suite->pana_session.session_ready ) {
                tr_debug("PCI received; Discard unfinished session and start a new one");
                if (pana_server_base->open_pana_authentication_cnt) {
                    pana_server_base->open_pana_authentication_cnt--;
                }
                sec_suite_remove(suite);
                suite = 0;
            } else {
                tr_debug("Accept new session");
            }
        }

        if (suite) {
            if (suite->pana_session.pana_heap == NULL) {
                suite->pana_session.pana_heap = pana_heap_structure_allocate();
            }
            if (suite->tls_session == NULL) {
                suite->tls_session = amr_tls_session_allocate();
                if (suite->tls_session == NULL) {
                    if (suite->pana_session.pana_heap) {
                        ns_dyn_mem_free(suite->pana_session.pana_heap);
                        suite->pana_session.pana_heap = NULL;
                    }
                }
            }
            if (suite->pana_session.pana_heap) {

                arm_tls_session_clear(suite->tls_session);
                suite->tls_session->tls_heap = tls_heap_allocate();
                if (suite->tls_session->tls_heap) {
                    //Generate TLS SESSION
                    pana_server_tls_session_init(suite);
                    pana_client_session_init(suite);
                    tr_debug("UPdate Auth Counter");
                    if (relay_pack) {
                        pana_server_session_address_set_for_relay(suite, buf);
                    } else {
                        suite->pana_session.address_status = 2;
                        memcpy(suite->session_address, buf->src_sa.address, 16);
                        suite->session_port = buf->src_sa.port;
                    }
                    pana_server_base->open_pana_authentication_cnt++;
                } else {
                    tr_debug("TLS heap allocate");
                }
            }

        } else {
            //Generate session
            suite = sec_suite_create();
            if (suite) {
                tr_debug("Create new Pana Session");
                pana_server_tls_session_init(suite);
                pana_server_base->open_pana_authentication_cnt++;
                pana_client_session_init(suite);
                suite->interface = buf->interface;
                if (relay_pack) {
                    pana_server_session_address_set_for_relay(suite, buf);
                } else {
                    suite->pana_session.address_status = 2;
                    memcpy(suite->session_address, buf->src_sa.address, 16);
                    suite->session_port = buf->src_sa.port;
                }
            }
        }
        buffer_free(buf);
        return;
    } else if (header.type == PANA_MSG_PA) {

        if (buf->src_sa.address[0] == 0xfe || relay_pack) {
            //accept only now session
            suite = sec_suite_selected_pana_session(header.session_id);
            if (!suite) {
                tr_debug("Drop Packet by session ID");
                buffer_free(buf);
                return;
            }


            if ((header.flags & PANA_FLAGS_REQUEST) == PANA_FLAGS_RESPONSE) {
                if (suite->pana_session.req_seq == header.seq) {
                    suite->pana_session.req_seq++;
                    //ADD Check for EAP fragmentation!!!!!!!!!!!!!!!
                    if ((header.flags & PANA_FLAGS_COMPLETE) == PANA_FLAGS_COMPLETE) {
                        if (suite->state != TLS_EAP_END_PANA_VERIFY && suite->state != PANA_READY) {
                            if (pana_server_base->open_pana_authentication_cnt) {
                                pana_server_base->open_pana_authentication_cnt--;
                            }
                            tr_debug("Remove Current session by 'Comp bit'");
                            sec_suite_remove(suite);
                            buffer_free(buf);
                            return;
                        }
                    }
                } else {
                    tr_debug("Pana RES:Drop Old seq");
                    buffer_free(buf);
                    return;
                }
            } else {
                suite->pana_session.res_seq = header.seq;
            }

            if (relay_pack) {
                buf->src_sa = buf->dst_sa;
            }

            if (header.flags & PANA_FLAGS_START) {
                pana_session_startms_parse(buf, &header, suite);
            } else if (header.flags & PANA_FLAGS_COMPLETE) {
                pana_complete_msg_parse(buf, &header, suite);
            } else {

                buf = pana_auth_message_handler(buf, &header, suite);

                if (buf) {
                    pana_eap_tls_up(buf, suite);
                }
            }
            return;
        }
        buffer_free(buf);
    } else {
        buffer_free(buf);
    }
}


int8_t pana_server_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key)
{
    int8_t ret_val;
    uint8_t supported_chipher_suites;
    protocol_interface_info_entry_t *cur;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->if_lowpan_security_params) {
        return -1;
    }

    if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        return -4;
    } else if (cur->if_lowpan_security_params->pana_params == 0) {
        return -3;
    } else if (cur->if_lowpan_security_params->nwk_security_mode != NET_SEC_MODE_PANA_LINK_SECURITY) {
        return -5;
    }

    switch (cipher_mode) {
        case NET_TLS_PSK_CIPHER: /**< Network Authentication support only PSK */
            supported_chipher_suites = SEC_CIPHERSUITE_PSK;
            break;

        case NET_TLS_ECC_CIPHER:        /**< Network Authentication support only ECC */
        case NET_TLS_PSK_AND_ECC_CIPHER: /**< Network Authentication support PSK & ECC */
#ifdef ECC
            //Verify Certficate
            if (sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN) == NULL) {
                return -1;
            }
            if (cipher_mode == NET_TLS_PSK_AND_ECC_CIPHER) {
                supported_chipher_suites = (SEC_CIPHERSUITE_PSK | SEC_CIPHERSUITE_ECC);
            } else {
                supported_chipher_suites = SEC_CIPHERSUITE_ECC;
            }
            break;
#endif // if ECC is not supported, flow into default => nothing is supported.
        default:
            supported_chipher_suites = 0;
            break;
    }


    ret_val = pana_server_init(interface_id, key_material, supported_chipher_suites, time_period_before_activate_key);
    if (ret_val == 0) {
        cur->if_lowpan_security_params->pana_params->nwk_chipher_mode = cipher_mode;
        cur->if_lowpan_security_params->pana_params->psk_key_id = 0;
        cur->if_lowpan_security_params->pana_params->pana_client = 0;
        cur->lowpan_info |= (INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION);
        cur->configure_flags |= INTERFACE_SECURITY_DEFINED;
    }

    return ret_val;
}

int8_t pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material)
{

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!pana_server_base || pana_server_base->network_interface_id != interface_id || pana_server_base->pana_key_update) {
        return -2;
    }

    if (!cur->if_lowpan_security_params || !cur->if_lowpan_security_params->pana_params) {
        return -3;
    }

    if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
        return -4;
    } else if (cur->if_lowpan_security_params->nwk_security_mode != NET_SEC_MODE_PANA_LINK_SECURITY) {
        return -5;
    }



    pana_server_base->pana_key_update = ns_dyn_mem_temporary_alloc(sizeof(pana_key_update_t));
    if (!pana_server_base->pana_key_update) {
        return -3;
    }

    pana_key_material_t *primary_key = pana_server_key_get(true);

    if (network_key_material) {
        memcpy(pana_server_base->pana_key_update->new_key_material, network_key_material, 16);
    } else {
        pana_server_set_random_key_value(pana_server_base->pana_key_update->new_key_material);
        network_key_material = pana_server_base->pana_key_update->new_key_material;
    }

    pana_lib_parameters_s *parameters = pana_parameters_get();

    /*  Update Key ID */
    pana_server_base->pana_key_update->key_id = primary_key->key_id;
    if (pana_server_base->pana_key_update->key_id == parameters->KEY_ID_MAX_VALUE) {
        /*  Start Lollipop Sechema from 1 again */
        pana_server_base->pana_key_update->key_id = 0;
    }
    pana_server_base->pana_key_update->key_id++;


    //Set New Key to secondary key
    primary_key = pana_server_key_get(false);
    primary_key->key_id = pana_server_base->pana_key_update->key_id;
    memcpy(primary_key->key_material, pana_server_base->pana_key_update->new_key_material, 16);
    //TRIG Timers
    pana_server_base->pana_key_update->key_delivery_cnt = sec_pana_key_update_trig(parameters->KEY_UPDATE_THRESHOLD);
    uint8_t *key_ptr = pana_key_get(network_key_material);
    if (key_ptr) {
        //pana_server_base->auth_cnt++;
        tr_debug("SET Secondary Key ready");
        mac_helper_security_next_key_set(cur, (key_ptr + 16), pana_server_base->pana_key_update->key_id, MAC_KEY_ID_MODE_IDX);
        mle_service_security_set_security_key(cur->id, key_ptr, pana_server_base->pana_key_update->key_id, false);
    }
    tr_debug("KEY Delivery CNT: %02x", pana_server_base->pana_key_update->key_delivery_cnt);
    if (pana_server_base->pana_key_update->key_delivery_cnt == 0) {
        tr_debug("TRIG NEW Key");
        pana_server_base->pana_key_update->key_delivery_cnt = 1;
        pana_key_update_delivery_ready();
    }
    if (pana_nvm_storage_cb) {
        pana_server_material_write(pana_nvm_buffer);
        pana_nvm_storage_cb(PANA_SERVER_MATERIAL_UPDATE);
    }
    return 0;
}

void pana_key_update_delay_timer(void)
{
    if (pana_server_base && pana_server_base->key_update_delay) {
        if (pana_server_base->key_update_delay <= 100) {
            protocol_interface_info_entry_t *cur;
            pana_server_base->key_update_delay = 0;
            cur = protocol_stack_interface_info_get_by_id(pana_server_base->network_interface_id);

            //SWAP new KEY
            if (cur) {
                tr_debug("Trig New Key ID");
                uint8_t keyId = mle_service_security_next_key_id_get(cur->id);
                mac_helper_security_key_swap_next_to_default(cur);
                mle_service_security_key_trig(cur->id, keyId);
                if (cur->nwk_wpan_nvm_api) {
                    cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
                }
                if (pana_key_update_process_ready) {
                    pana_key_update_process_ready();
                }
            }
            if (pana_server_base->primary_material) {
                pana_server_base->primary_material = 0;
            } else {
                pana_server_base->primary_material = 1;
            }

            pana_key_material_t *primary_key = pana_server_key_get(true);
            primary_key->key_id = pana_server_base->pana_key_update->key_id;
            memcpy(primary_key->key_material , pana_server_base->pana_key_update->new_key_material, 16);

            if (pana_server_base->pana_key_update) {
                ns_dyn_mem_free(pana_server_base->pana_key_update);
                pana_server_base->pana_key_update = 0;
            }
            if (pana_nvm_storage_cb) {
                tr_debug("PS:NVM Cover");
                pana_server_material_write(pana_nvm_buffer);
                pana_nvm_storage_cb(PANA_SERVER_MATERIAL_UPDATE);
            }

        } else {
            pana_server_base->key_update_delay -= 100;
        }
    }

}

int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    if (!update_cb || !nvm_static_buffer || !nvm_get || !nvm_session_get) {
        return -1;
    }
    pana_nvm_storage_cb = update_cb;
    pana_server_nvm_get = nvm_get;
    pana_server_nvm__session_get = nvm_session_get;
    pana_nvm_buffer = nvm_static_buffer;
    return 0;
}

int8_t pana_server_process_ready_cb_set(void (*cb_fptr)(void))
{
    if (cb_fptr) {
        pana_key_update_process_ready = cb_fptr;
        return 0;
    }
    return -1;
}

/* Pana Client session load from NVM API */
int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer)
{

    if (!pana_server_base) {
        return -1;
    }
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(pana_server_base->network_interface_id);
    if (!cur) {
        return -1;
    }

    if (pana_socket_id_get() == -1) {
        return -2;
    }

    if (pana_server_session_restore(nvm_pointer, cur)) {
        return -3;
    }
    return 0;
}

static void pana_success_server_build(buffer_t *buf, sec_suite_t *suite)
{
    uint8_t *ptr;
    pana_header_t header;
    header.type = PANA_MSG_PA;
    header.flags = PANA_FLAGS_COMPLETE | PANA_FLAGS_REQUEST;
    //REQUEST
    header.seq = suite->pana_session.req_seq;
    header.session_id = suite->pana_session.session_id;

    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);

    uint8_t eap_status[4];
    eap_header_build(eap_status, 4,EAP_SUCCESS, suite->pana_session.eap_id_seq, 0);

    ptr = pana_avp_32_bit_write(AVP_RESULT_CODE, 0, ptr);
    ptr = pana_avp_write_n_bytes(AVP_EAP_PAYLOAD_CODE, 4, eap_status, ptr);
    ptr = pana_avp_32_bit_write(AVP_KEY_ID_CODE, suite->pana_session.pana_key_id, ptr);
    ptr = pana_avp_32_bit_write(AVP_SESSION_LIFETIME_CODE, suite->pana_session.session_lifetime, ptr);
    //ENC
    uint8_t key_material[18];
    pana_server_set_key_material(key_material, false, suite->pana_session.auth_cnt);
    ptr = pana_avp_zip_key_material(ptr, key_material, header.seq, suite);
    ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);
    buffer_data_end_set(buf, ptr);
    buf = build_pana_base(buf, &header, suite);
    if (!buf) {
        return;
    }

    pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);

    //Encode Pana Auth
    pana_set_agend_address(buf, false, suite);
    if (suite->pana_session.address_status & 1) {
        buf = pana_relay_avp_build(buf, suite);
        if (buf) {
            header.flags = 0;
            header.type = PANA_MSG_RELAY;
            header.session_id = 0;
            header.seq = 0;
            buf = build_pana_base(buf, &header, suite);
        }
    }

    protocol_push(buf);
}



static void pana_server_build_key_push(buffer_t *buf, sec_suite_t *suite)
{
    uint8_t *ptr;
    pana_header_t header;
    header.type = PANA_MSG_PNA;
    header.flags = PANA_FLAGS_REQUEST | PANA_FLAGS_PING;
    //REQUEST
    header.seq = suite->pana_session.req_seq;
    header.session_id = suite->pana_session.session_id;


    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);
    //ENC
    uint8_t key_material[18];
    pana_server_set_key_material(key_material, true, suite->pana_session.auth_cnt);
    ptr = pana_avp_zip_key_material(ptr, key_material, header.seq, suite);
    ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);

    buffer_data_end_set(buf, ptr);
    buf = build_pana_base(buf, &header, suite);
    if (!buf) {
        return;
    }

    pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);
    pana_set_agend_address(buf, true, suite);

    protocol_push(buf);
}

int8_t pana_network_key_get(int8_t interface_id, ns_keys_t *key)
{
    (void)interface_id;

    if (pana_server_base && key) {
        pana_key_material_t *primary_key = pana_server_key_get(true);
        pana_key_material_t *prev_key = pana_server_key_get(false);
        if (prev_key->key_id == 0) {
            prev_key = 0;
        }
        key->current_active_key_index = primary_key->key_id;
        memcpy(key->current_active_network_key , primary_key->key_material, 16);
        if (prev_key) {
            key->previous_active_key_index = prev_key->key_id;
            memcpy(key->previous_active_network_key , prev_key->key_material, 16);
        } else {
            memset(key->previous_active_network_key, 0, 16);
        }
        return 0;
    }
    return -1;
}

int8_t pana_server_trig_new_key(int8_t interface_id)
{

    if (!pana_server_base) {
        return -1;
    }
    if (pana_server_base->network_interface_id != interface_id) {
        return -2;
    }
    if (pana_server_base->key_update_delay) {
        tr_debug("TRIG faster");
        pana_server_base->key_update_delay = 1;
        return  0;
    }
    return -1;

}

int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
     return -1;
    }

    if (!pana_socket_init(pana_server_packet_handler, pana_server_state_machine_func, tls_server_up)) {
        return -1;
    }

    sec_suite_list_clean();

    //Allocate
    if (pana_server_base == 0) {
        pana_server_base = pana_server_staructure_allocate();
    }
    if (!pana_server_base || !cur->if_lowpan_security_params || !cur->if_lowpan_security_params->pana_params) {
        return -1;
    }

    net_tls_cipher_e cipher_mode;
    pana_server_base->open_pana_authentication_cnt = 0;
    pana_server_base->network_interface_id = interface_id;
    pana_server_base->pana_key_update = 0;
    pana_server_base->key_update_delay = 0;
    pana_server_material_read(nvm_data);
    if (pana_server_base->supported_chipher_suites == SEC_CIPHERSUITE_PSK) {
        cipher_mode = NET_TLS_PSK_CIPHER;
    } else if (pana_server_base->supported_chipher_suites == SEC_CIPHERSUITE_ECC) {
        cipher_mode = NET_TLS_ECC_CIPHER;
    } else {
        cipher_mode = NET_TLS_PSK_AND_ECC_CIPHER;
    }

    cur->if_lowpan_security_params->pana_params->nwk_chipher_mode = cipher_mode;
    cur->if_lowpan_security_params->pana_params->psk_key_id = 0;//TODO?????
            cur->if_lowpan_security_params->pana_params->pana_client = 0;
    cur->lowpan_info |= (INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION);
    cur->configure_flags |= INTERFACE_SECURITY_DEFINED;

    return 0;
}

int8_t pana_server_key_material_load(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    } else if (!pana_server_base) {
        return -1;
    }
    uint8_t *key_ptr;
    pana_key_material_t *key_mat;
    if (pana_server_base->pana_key_update) {

        pana_server_base->auth_cnt++;
        //Load First Primary to master
        key_mat = pana_server_key_get(true);
        key_ptr = pana_key_get(key_mat->key_material);
        mac_helper_security_default_key_set(cur, (key_ptr + 16), key_mat->key_id, MAC_KEY_ID_MODE_IDX);
        mle_service_security_set_security_key(cur->id, key_ptr, key_mat->key_id, true);
        if (cur->nwk_wpan_nvm_api) {
            cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
        }
        //Set Secondary to coming key
        key_mat = pana_server_key_get(false);
        key_ptr = pana_key_get(key_mat->key_material);
        mac_helper_security_next_key_set(cur, (key_ptr + 16), key_mat->key_id, MAC_KEY_ID_MODE_IDX);
        mle_service_security_set_security_key(cur->id, key_ptr, key_mat->key_id, false);
    } else {
        bool secondary_active = false;
        //Load first Secondary and then Primary
        key_mat = pana_server_key_get(false);
        if (key_mat->key_id) {
            key_ptr = pana_key_get(key_mat->key_material);
            secondary_active = true;
            mac_helper_security_default_key_set(cur, (key_ptr + 16), key_mat->key_id, MAC_KEY_ID_MODE_IDX);
            mle_service_security_set_security_key(cur->id, key_ptr, key_mat->key_id, true);
            if (cur->nwk_wpan_nvm_api) {
                cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
            }
        }
        key_mat = pana_server_key_get(true);
        key_ptr = pana_key_get(key_mat->key_material);
        if (secondary_active) {
            mac_helper_security_next_key_set(cur, (key_ptr + 16), key_mat->key_id, MAC_KEY_ID_MODE_IDX);
            mle_service_security_set_security_key(cur->id, key_ptr, key_mat->key_id, false);
            mac_helper_security_key_swap_next_to_default(cur);
            mle_service_security_key_trig(cur->id, key_mat->key_id);
        } else {
            mac_helper_security_default_key_set(cur, (key_ptr + 16), key_mat->key_id, MAC_KEY_ID_MODE_IDX);
            mle_service_security_set_security_key(cur->id, key_ptr, key_mat->key_id, true);
        }
        if (cur->nwk_wpan_nvm_api) {
            cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
        }
    }
    return 0;
}

static void pana_client_authentication_fail(sec_suite_t *suite)
{
    if (suite->pana_session.nvm_offset) {
        //tr_debug("NVM Session Remove");
        pana_session_nvm_udate(suite, PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE);
    }
    if (pana_server_base && pana_server_base->open_pana_authentication_cnt) {
        pana_server_base->open_pana_authentication_cnt--;
    }
}

#else
int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    (void)update_cb;
    (void)nvm_static_buffer;
    return -1;
}

int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer)
{
    (void) nvm_pointer;
    return -1;
}

int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id)
{
    (void) nvm_data;
    (void)interface_id;
    return -1;
}
#endif
#else
int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    (void)update_cb;
    (void)nvm_static_buffer;
    return -1;
}

int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer)
{
    (void) nvm_pointer;
    return -1;
}

int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id)
{
    (void) nvm_data;
    (void)interface_id;
    return -1;
}
#endif
