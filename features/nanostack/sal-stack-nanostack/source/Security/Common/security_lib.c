/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include "string.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "shalib.h"
#include "randLIB.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/TLS/tls_ccm_crypt.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana_nvm.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/eap_protocol.h"
#include "Security/PANA/pana_internal_api.h"
#include "common_functions.h"

#ifdef PANA

#ifndef SEC_TLS_TIMEOUT
#define SEC_TLS_TIMEOUT 65000
#endif

#define TRACE_GROUP "secl"

NS_LARGE NS_LIST_DEFINE(sec_suite_list, sec_suite_t, link);
sec_suite_t *NS_LARGE active_ecc_sec_suite = 0;

static void tls_MSK_calc(sec_suite_t *suite);

#ifdef PANA_SERVER_API
#ifdef ECC
static void tls_server_hash_copy(uint8_t *ptr, tls_msg_t *tmp_msg, sec_suite_t *suite);
#endif
#endif

#ifdef ECC

static MPint temp_key_entry;

static int8_t ecc_signature_calculate_hash(tls_heap_t *theap);
#endif

static void sec_tx_done(sec_suite_t *suite);
static bool sec_suite_tls_allocate(sec_suite_t *suite);


sec_suite_t * sec_lib_security_session_allocate(bool tls_allocate)
{
    sec_suite_t *cur = ns_dyn_mem_alloc(sizeof(sec_suite_t));
    if (cur) {

        tls_session_t *tls_session;
        if (tls_allocate) {
            tls_session = amr_tls_session_allocate();
            if (!tls_session) {
                ns_dyn_mem_free(cur);
                return NULL;
            }
        } else {
            tls_session = NULL;
        }
        memset(cur, 0, sizeof(sec_suite_t));
        cur->tls_session = tls_session;
        cur->psk_key_id = -1;
        cur->supported_chipher_suites = SEC_DEFAULT_SUPPORTED_CHIPHER_SUITES;
        pana_session_base_init(&cur->pana_session);
        ns_list_add_to_start(&sec_suite_list, cur);
    }
    return cur;
}


void sec_lib_state_machine_lock(sec_suite_t *suite, sec_state_machine_t state)
{
    suite->state = state;
    suite->timer = 0;
    suite->retry_counter = 0;
}

void sec_lib_state_machine_trig(sec_suite_t *suite, sec_state_machine_t state)
{
    suite->state = state;
    suite->timer = 1;
    suite->retry_counter = 0;

}

#ifdef ECC
void tls_server_hash_copy(uint8_t *ptr, tls_msg_t *tmp_msg, sec_suite_t *suite)
{
#ifdef PANA_SERVER_API
    tls_heap_t *t_heap = suite->tls_session->tls_heap;
    uint16_t t_length = 39; //Base and random and seq len
    t_length += suite->tls_session->id_length;
    if (t_heap->tls_chipher_mode == CHIPHER_ECC) {
        t_length += 11;
    } else {
        t_length += 7;
    }

    tmp_msg->len = t_length;
    tmp_msg->len -= 4;
    tmp_msg->msg_ptr = ptr + 4;
    tls_build_server_hello_msg(ptr, suite->tls_session);
    tls_handshake_copy(tmp_msg, t_heap);
    tr_debug("Pana server S-Hello,Cert hash");
#endif
}
#endif

#ifdef PANA_SERVER_API
uint8_t tls_server_certi_hash_copy(sec_suite_t *suite)
{
    if (suite->setups & TLS_HANSHAKE_HASH) {
        return 1;
    } else {
#ifdef ECC
        certificate_chain_internal_t *temp;
        uint16_t len = 0;
        buffer_t *buf;
        tls_msg_t *tmp_msg = tls_msg_ptr_get();
        tls_heap_t *t_heap = suite->tls_session->tls_heap;


        temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

        if (temp) {
            len = tls_certificate_len(temp);

            buf = buffer_get(len);
            if (buf) {
                uint8_t *ptr;
                buf->interface = suite->interface;

                ptr = buffer_data_pointer(buf);
                tls_server_hash_copy(ptr, tmp_msg, suite);
                ptr = buffer_data_pointer(buf);
                tmp_msg->len = len - 4;
                tmp_msg->msg_ptr = ptr + 4;
                ptr = tls_certificate_msg_set(ptr, temp);
                tls_handshake_copy(tmp_msg, t_heap);
                suite->setups |= TLS_HANSHAKE_HASH;
                buffer_free(buf);
                return 1;

            }
        }
#endif
    }
    return 0;
}
#endif

#ifdef ECC
uint8_t tls_certi_hash_copy(sec_suite_t *suite)
{
    if (suite->setups & TLS_HANSHAKE_HASH) {
        return 1;
    } else {
#ifdef ECC
        certificate_chain_internal_t *temp;
        uint16_t len = 0;
        buffer_t *buf;
        tls_msg_t *tmp_msg = tls_msg_ptr_get();
        tls_heap_t *t_heap = suite->tls_session->tls_heap;


        temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

        if (temp) {
            len = tls_certificate_len(temp);

            buf = buffer_get(len);
            if (buf) {
                uint8_t *ptr;

                buf->interface = suite->interface;
                ptr = buffer_data_pointer(buf);
                ptr = buffer_data_pointer(buf);
                tmp_msg->len = len - 4;
                tmp_msg->msg_ptr = ptr + 4;
                ptr = tls_certificate_msg_set(ptr, temp);
                tls_handshake_copy(tmp_msg, t_heap);

                ptr = buffer_data_pointer(buf);
                tmp_msg->len = 66;
                tmp_msg->msg_ptr = ptr + 4;

                ptr = tls_client_key_exchange_msg_set(ptr, t_heap);
                tls_handshake_copy(tmp_msg, t_heap);

                suite->setups |= TLS_HANSHAKE_HASH;
                buffer_free(buf);
                return 1;

            }
        }
#endif
    }
    return 0;
}
#endif

static void tls_MSK_calc(sec_suite_t *suite)
{
    uint8_t *ptr;
    tls_heap_t *theap = 0;
    pana_heap_t *pheap = 0;
    prf_sec_param_t *prf_ptr = shalib_prf_param_get();
    theap = suite->tls_session->tls_heap;
    pheap = suite->pana_session.pana_heap;
    //tr_debug("CAL MSK:");
    prf_ptr->secret = suite->tls_session->master_secret;
    prf_ptr->sec_len = 48;
    prf_ptr->label = "client EAP encryption";
    prf_ptr->seed = theap->temp_buf;
    ptr = theap->temp_buf;
    memcpy(ptr, (theap->tls_hello_random + CLIENT_HELLO_PTR) , 32);
    ptr += 32;
    memcpy(ptr, (theap->tls_hello_random + SERVER_HELLO_PTR) , 32);
    prf_ptr->seedlen = 64;
    shalib_prf_calc(pheap->MSK, 16);
}

#ifdef PANA_SERVER_API
static int tls_check_client_change_chiphersuite(uint8_t *verfify, sec_suite_t *suite)
{
    tls_heap_t *tls_heap = suite->tls_session->tls_heap;
    int ret_val = -1;
    if (tls_heap == NULL) {
        tr_warn("TLS Heap fail 1");
        return -1;
    }

    if (tls_heap->client_verify_buf == NULL) {
        tr_warn("TLS Heap verfify ptr fail 1");
        return -1;
    }

    if (tls_ccm_data_decrypt(tls_heap->client_verify_buf, tls_heap->client_verify_buf_len, suite->tls_session->key_expansion, TLS_HANDSHAKE, false) != 0) {
        tr_warn("AUTH Mic Fail");
        goto end_process;
    }

    uint8_t *ptr = tls_heap->client_verify_buf + 8;

    if (ptr[0] == TLS_FINISHED && common_read_24_bit(ptr + 1) == 12) {
        ptr += 4;
        if (memcmp(verfify, ptr, 12) == 0) {
            //tr_debug("Client verify OK");
            ret_val = 0;
            tls_finnish_copy(ptr, tls_heap);
            tls_hanshake_hash_cal(tls_heap);
            sec_lib_state_machine_lock(suite, PRF_CALC2);
            tls_verify_calc(tls_heap->verify, 1, tls_heap, suite->tls_session->master_secret);
            sec_lib_state_machine_trig(suite,TLS_KEY_CHANGE);

        } else {
            tr_warn("Verify Mismatch: %s != %s", trace_array(ptr, 12), trace_array(verfify, 12));
        }
    } else {
        tr_debug("No Chiphertext");
    }

    end_process:
    //tr_debug("Free");
    if (tls_heap->client_verify_buf) {
        ns_dyn_mem_free(tls_heap->client_verify_buf);
        tls_heap->client_verify_buf = NULL;
    }

    return ret_val;
}
#endif
#ifdef ECC

void sec_ecc_state_free(sec_suite_t *suite)
{
    if (active_ecc_sec_suite) {
        tr_debug("Active ECC");
        if (suite == active_ecc_sec_suite) {
            tr_debug("Stop ECC");
            ecc_free_memory();
            active_ecc_sec_suite = NULL;
        }
    }
}


void sec_ecc_state_save(sec_suite_t *suite)
{
    if (active_ecc_sec_suite == 0 && suite) {
        active_ecc_sec_suite = suite;
        //Send ECC event callback

        arm_event_s event = {
            .receiver = protocol_read_tasklet_id(),
            .sender = 0,
            .event_type = ARM_IN_SECURITY_ECC_CALLER,
            .data_ptr = NULL,
            .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        };
        if (eventOS_event_send(&event) != 0) {
            tr_error("sec_ecc_state_save(): event send failed");
        }
    }
}


uint8_t tls_certificate_build(sec_suite_t *suite)
{
    buffer_t *buf;
    certificate_chain_internal_t *temp;
    uint16_t len = 0;

    temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

    if (temp) {

        len = tls_certificate_len(temp);
        len += 50; //TLS/EAP/PANA/TCP /IP

        //Client Key Exchange 70, Certiverify max 80, Finnish 43
        len += (70 + 80 + 43);
        buf = buffer_get(len); //(690);

        if (buf) {
            uint8_t *ptr;
            tls_msg_t *tmp_msg = tls_msg_ptr_get();
            buf->interface = suite->interface;
            ptr = buffer_data_pointer(buf);
            //Leave Space for Tls Handshake Header
            ptr += 5;
            /* Set Certificate */
            ptr = tls_certificate_msg_set(ptr, temp);
            /* Client Key Exchange */
            ptr = tls_client_key_exchange_msg_set(ptr, suite->tls_session->tls_heap);

            /* Certificate Verify */
            if ((suite->setups & TLS_HANSHAKE_HASH) == 0) {
                uint8_t *ptr_2;
                ptr_2 = ptr;
                ptr_2 += 4;

                tmp_msg->msg_ptr = ptr_2;
                ptr = tls_certificate_verify_msg_set(ptr, suite->tls_session->tls_heap);
                tr_debug("Set Cert Ver");
                tmp_msg->len = ptr - ptr_2;
                tls_handshake_copy(tmp_msg, suite->tls_session->tls_heap);
                sec_prf_state_set(suite);

            } else {
                ptr = tls_certificate_verify_msg_set(ptr, suite->tls_session->tls_heap);
            }

            buffer_data_end_set(buf, ptr);

            tls_header_set(buf);
            ptr = buffer_data_end(buf);
            if ((suite->setups & TLS_HANSHAKE_HASH) == 0) {
                uint8_t *ptr_2;
                ptr_2 = ptr;
                ptr_2 += 9;
                tmp_msg->msg_ptr = ptr;
                ptr = tls_build_change_chipher_suite_finnish_msg(ptr, suite->tls_session);
                tmp_msg->len = ptr - ptr_2;
                tls_handshake_copy(tmp_msg, suite->tls_session->tls_heap);
                suite->setups |= TLS_HANSHAKE_HASH;
            } else {
                ptr = tls_build_change_chipher_suite_finnish_msg(ptr, suite->tls_session);
            }
            buf->buf_end = ptr - buf->buf;

            tr_debug("TX Cer");
            pana_eap_down(buf, suite);
            return 1;
        }
    }
    return 0;
}
#endif

#ifdef ECC
void sec_ecc_sceduler(void)
{
    if (ecc_run() != ECC_STATUS_IDLE) {
        arm_event_s event = {
            .receiver = protocol_read_tasklet_id(),
            .sender = 0,
            .event_type = ARM_IN_SECURITY_ECC_CALLER,
            .data_ptr = NULL,
            .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        };
        if (eventOS_event_send(&event) != 0) {
            tr_error("sec_ecc_sceduler(): event send failed");
        }
    }
}
#ifdef PANA_SERVER_API
uint8_t tls_pana_server_exchange_build(sec_suite_t *suite)
{
    certificate_chain_internal_t *temp;
    buffer_t *buf;
    tls_heap_t *t_heap = suite->tls_session->tls_heap;
    uint16_t len = 39;

    temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

    if (temp) {
        len += suite->tls_session->id_length;
        if (t_heap->tls_chipher_mode == CHIPHER_ECC) {
            len += 11;
        } else {
            len += 7;
        }

        len += tls_certificate_len(temp); //Certi Len
        len += 240; //server key exchange
        len += 16; //Cert REQ & Hello Done

        buf = buffer_get(len);
        if (buf) {
            uint8_t *ptr, *t_ptr;
            tls_msg_t *tmp_msg = tls_msg_ptr_get();
            buf->interface = suite->interface;
            ptr = buffer_data_pointer(buf);
            ptr += 5;

            //Server Hello build
            ptr = tls_build_server_hello_msg(ptr, suite->tls_session);
            // Certificate(s)
            ptr = tls_certificate_msg_set(ptr, temp);
            //Server Key Exchange
            tmp_msg->msg_ptr = ptr + 4;
            t_ptr = ptr;
            ptr = tls_server_key_excahnge_msg_build(ptr, t_heap);
            tmp_msg->len = ptr - t_ptr;
            tmp_msg->len -= 4;
            if ((suite->setups & TLS_HANSHAKE_HASH) == 0) {
                tls_handshake_copy(tmp_msg, t_heap);
            }
            //Cert Req
            *ptr++ = TLS_CERTIFICATE_REQUEST;
            ptr = common_write_24_bit(8, ptr);

            tmp_msg->msg_ptr = ptr;
            *ptr++ = 1; //Type Count
            *ptr++ = TLS_CERT_TYPE_ECDSA;
            ptr = common_write_16_bit(2, ptr); //length
            ptr = common_write_16_bit(TLS_SIG_HASH_ALG_SHA256_ECDSA, ptr); //SET ALGORYTH
            ptr = common_write_16_bit(0, ptr);


            tmp_msg->len = 8;
            if ((suite->setups & TLS_HANSHAKE_HASH) == 0) {
                tls_handshake_copy(tmp_msg, t_heap);
            }


            //Server Hello Done
            *ptr++ = TLS_SERVER_HELLO_DONE;
            ptr = common_write_24_bit(0, ptr);
            tmp_msg->msg_ptr = ptr;
            tmp_msg->len = 0;
            if ((suite->setups & TLS_HANSHAKE_HASH) == 0) {
                tls_handshake_copy(tmp_msg, t_heap);
                suite->setups |= TLS_HANSHAKE_HASH;
            }
            buffer_data_end_set(buf, ptr);
            tls_header_set(buf);
            pana_eap_down(buf, suite);
            return 1;
        }
    }

    return 0;
}
#endif

static int8_t ecc_signature_calculate_hash(tls_heap_t *theap)
{
    tr_debug("sign hash:");
    if (theap->ecc_heap) {
        //tr_debug("Allocate SIG");
        if (theap->ecc_heap->sgnt == 0) {
            theap->ecc_heap->sgnt = ecc_get_ecdsa_signature();
        }

        if (!theap->ecc_heap->sgnt) {
            tr_warn("Signature Allocate Fail");
            return -1;
        }
    } else {
        tr_warn("Signature Allocate Fail");
        return -1;
    }
    tls_ecc_server_key_signature_hash(theap);
    tr_debug(" done");
    return 0;

}
#endif
void sec_libray_init(void)
{
#ifdef ECC
    ecc_init();
#endif
}


static bool sec_suite_tls_allocate(sec_suite_t *suite)
{
    tls_heap_t *tls_heap;
    tls_heap = tls_heap_allocate();
    if (!tls_heap) {
        return false;
    }

    suite->tls_session->tls_heap = tls_heap;
    if (suite->setups & TLS_SERVER_MODE) {
        if (suite->tls_session->id_length == 0) {
            tls_session_id_genrate(suite->tls_session->tls_session_id, 4);
            suite->tls_session->id_length = 4;
        }
        memset(suite->tls_session->tls_nonce_explit, 0, 8);
    }
    return true;
}

static bool sec_suite_pana_allocate_dynamic_ram(sec_suite_t *suite)
{
    ns_dyn_mem_free(suite->pana_session.pana_heap);
    suite->pana_session.pana_heap = pana_heap_structure_allocate();
    if (!suite->pana_session.pana_heap) {
        return false;
    }
    return true;
}


int sec_pana_protocol_init(sec_suite_t *suite)
{
    if (suite->tls_session == NULL) {
        suite->tls_session = amr_tls_session_allocate();
    }

    if (!suite->tls_session) {
        sec_lib_state_machine_trig(suite, PANA_ERROR);
        return 0;
    }
    //Initialize current TLS session data
    sec_suite_tls_free(suite, false);

    pana_session_state_init(&suite->pana_session);
    suite->pana_session.prf_algorythm = 0;
    suite->pana_session.integrity_algorythm = 0;
    suite->pana_session.session_id = 0;
    suite->pana_session.req_seq = randLIB_get_32bit();
    suite->pana_session.res_seq = 0;

    //Free pana heap and init eap fragmentation
    pana_free_dynamic_ram(suite);

    suite->setups &= ~TLS_HANSHAKE_HASH;
    suite->timer = 1;
    if (!sec_suite_tls_allocate(suite) || !sec_suite_pana_allocate_dynamic_ram(suite)) {
        sec_suite_tls_free(suite, true);
        sec_lib_state_machine_trig(suite, PANA_ERROR);
         return 0;
    }
    suite->state = PANA_PCI_TX;

    return 1;
}


void sec_suite_tls_free(sec_suite_t *suite, bool free_session)
{
    if (suite->tls_session) {
        arm_tls_session_clear(suite->tls_session);
        if (free_session) {
            ns_dyn_mem_free(suite->tls_session);
            suite->tls_session = NULL;
        }
    }
    suite->setups &= ~(TLS_ECC_CERTIFICATE_REQUESTED | TLS_ECC_CERTIFICATE_RECEIVED | TLS_ECC_CERTIFICATE_VERIFY);
}


uint8_t sec_auth_re_check(sec_suite_t *suite) {
    if (pana_retry_check(suite->retry_counter, suite->state) == 0) {
        suite->retry_counter++;
        return 1;
    } else {
        //Init back to zero after fail
        suite->retry_counter = 0;
    }
    return 0;
}

uint8_t sec_check_suite_ptrs(sec_suite_t *suite)
{
    uint8_t ret_val = 1;
    if (suite->pana_session.auth_info == 0) {
        if (!suite->pana_session.user_server) {
            tr_debug("AUTH Info!");
            ret_val = 0;
        }
    }
    if (suite->tls_session) {
        if (suite->tls_session->tls_heap == 0) {
            tr_debug("TLS Heap!");
            ret_val = 0;
        }
    } else {
        if (suite->state != PANA_PING_REQ) {
            ret_val = 0;
            tr_debug("No TLS session allocated");
        }
    }
    return ret_val;

}



void sec_timer_handle(void)
{
    ns_list_foreach_safe(sec_suite_t, cur, &sec_suite_list) {
        uint8_t remove_cur = 0;
        if (cur->timer && --cur->timer == 0) {
            pana_common_state_machine(cur);
            if ((cur->state == PANA_ERROR) && (cur->timer == 0)) {
                if (cur->setups & TLS_SERVER_MODE) {
                    remove_cur = 1;
                }
            }
        }
        if (remove_cur) {
            tr_debug("Remove Session");
            sec_suite_remove(cur);
        }
    }
    //Here Possible to set 1 Second delay
    pana_key_update_delay_timer();
}

void sec_suite_list_clean(void)
{
    ns_list_foreach_safe(sec_suite_t, cur, &sec_suite_list) {
        sec_suite_remove(cur);
    }

}

sec_suite_t *sec_suite_verify(sec_suite_t *session)
{
    ns_list_foreach(sec_suite_t, cur, &sec_suite_list) {
        if (cur == session) {
            return cur;
        }
    }
    return NULL;
}

sec_suite_t *sec_suite_selected_py_pan_id(uint16_t pan_id)
{
    ns_list_foreach(sec_suite_t, cur, &sec_suite_list) {
        if (cur->pan_id == pan_id) {
            return cur;
        }
    }
    return NULL;
}

sec_suite_t *sec_suite_selected_pana_session(uint32_t session_id)
{
    ns_list_foreach(sec_suite_t, cur, &sec_suite_list) {
        if (cur->pana_session.session_id == session_id) {
            return cur;
        }
    }
    return NULL;
}

sec_suite_t *sec_suite_selected_address(const uint8_t address[static 16])
{
    ns_list_foreach(sec_suite_t, cur, &sec_suite_list) {
        if (memcmp(cur->session_address, address, 16) == 0) {
            return cur;
        }
    }
    return NULL;
}

uint16_t sec_pana_key_update_trig(uint16_t th_time)
{
    uint32_t trig_interval = 1;
    uint16_t counter = 0;
    uint32_t threshold_change;
    //Convert seconds to 100ms ticks
    threshold_change = (th_time * 10);
    ns_list_foreach(sec_suite_t, cur, &sec_suite_list) {
        if (cur->pana_session.session_ready) {
            if ((cur->pana_session.address_status & 3) == 0) { //
                tr_debug("Trig Key Update");
                cur->timer = trig_interval;
                trig_interval += threshold_change;
                cur->state = PANA_KEY_UPDATE;
                cur->retry_counter = 0;
                tr_debug("NVM SEQ Update by Key Push");
                pana_session_nvm_udate(cur, PANA_SERVER_CLIENT_SESSION_UPDATE);
                //Clear Current value
                cur->retry_counter = 0;
                counter++;
            }
        }
    }
    return counter;
}

int8_t sec_suite_remove(sec_suite_t *cur) {
    if (!cur) {
        return -1;
    }

    pana_free_dynamic_ram(cur);

    sec_suite_tls_free(cur, true);
#ifdef ECC
    sec_ecc_state_free(cur);
#endif
    ns_list_remove(&sec_suite_list, cur);
    ns_dyn_mem_free(cur);
    return 0;
}

sec_suite_t *sec_suite_create(void)
{
    sec_suite_t *cur = sec_lib_security_session_allocate(true);
    if (!cur) {
        return NULL;
    }
    tls_heap_t *t_heap = tls_heap_allocate();
    pana_heap_t *p_heap = pana_heap_structure_allocate();

    if (!t_heap || !p_heap ) {
        ns_list_remove(&sec_suite_list, cur);
        ns_dyn_mem_free(t_heap);
        ns_dyn_mem_free(t_heap);
        ns_dyn_mem_free(p_heap);
        ns_dyn_mem_free(cur);
        return NULL;
    }

    cur->tls_session->tls_heap = t_heap;
    cur->pana_session.pana_heap = p_heap;
    return cur;
}



void sec_prf_state_set(sec_suite_t *suite)
{
    if (suite->tls_session) {
        tls_heap_t *tls_heap = suite->tls_session->tls_heap;
        sec_lib_state_machine_lock(suite, PRF_CALC);
        tls_master_key_cal(tls_heap, suite);
        tls_key_expansion_cal(tls_heap, suite->tls_session->key_expansion, suite->tls_session->master_secret);
        tls_MSK_calc(suite);
        tls_hanshake_hash_cal(tls_heap);

        if (suite->setups & TLS_SERVER_MODE) {
#ifdef PANA_SERVER_API
            tls_verify_calc(tls_heap->verify, 0, tls_heap, suite->tls_session->master_secret);
            if (tls_check_client_change_chiphersuite(tls_heap->verify, suite) != 0) {
                sec_lib_state_machine_trig(suite,TLS_ALERT_DECRYPT);
            }
#endif
        } else {
            tls_verify_calc(tls_heap->verify, 0, tls_heap, suite->tls_session->master_secret);
            if (tls_heap->tls_chipher_mode != CHIPHER_ECC) {
                sec_lib_state_machine_trig(suite,TLS_KEY_CHANGE);
            } else {
                tls_prepare_change_chipher_spec(suite);
            }
        }
    }
}
#ifdef ECC
static void tls_key_set_elliptic_point(EllipticPoint *ellicpt_ptr, uint8_t *keyPtr)
{
    tls_ecc_point_reverse_order((uint8_t *)ellicpt_ptr->x.data, keyPtr);
    tls_ecc_point_reverse_order((uint8_t *)ellicpt_ptr->y.data, (keyPtr + 32));
}

uint8_t tls_ecc_start_premaster_secret(EllipticPoint *ellicpt_ptr, sec_suite_t *suite)
{
    uint8_t auth_setup = suite->setups;
    if (ellicpt_ptr == 0) {
        ellicpt_ptr = ecc_get_elliptic_point();
    } else {
        memset(ellicpt_ptr, 0, sizeof(EllipticPoint));
        ellicpt_ptr->finite = 1;
    }

    if (ellicpt_ptr) {
        tls_heap_t *tls_heap = suite->tls_session->tls_heap;
        tr_debug("Cal ECC PSK");
        if ((auth_setup & TLS_SERVER_MODE) == 0) {
            tls_key_set_elliptic_point(ellicpt_ptr, tls_heap->ecc_heap->server_public_key);
        } else {
            tls_key_set_elliptic_point(ellicpt_ptr, tls_heap->ecc_heap->client_public_key);
        }
        if (ecc_calculate_pre_master_secret(ellicpt_ptr, &(tls_heap->ecc_heap->private_key), &ecc_operation_done_callback) == ECC_STATUS_OK) {
            //tr_debug("PRE started");
            sec_lib_state_machine_lock(suite,TLS_ECC_GENERATE_PREMASTER_SECRET);
            sec_ecc_state_save(suite);
            return 1;
        } else {
            tr_debug("Failed pre start");
            ecc_library_free_pointer(ellicpt_ptr);
        }
    }

    return 0;
}

void ecc_operation_done_callback(int8_t status, void *result_ptr)
{
    uint8_t alert = 0;
    sec_suite_t *suite = active_ecc_sec_suite;
    active_ecc_sec_suite = NULL;
    //tr_debug("ECC CB");
    if (status != ECC_STATUS_OK) {
        tr_warn("ECC proces Fail: %i", status);
        if (result_ptr) {
            tr_debug("Free ECC Result");
            ecc_library_free_pointer(result_ptr);
        }
        ecc_free_memory();
        alert = 1;
        if (suite == 0) {
            tr_warn("NOt active ECC2!!");
            return ;
        }
    } else {
        tls_heap_t *tls_heap = 0;
        if (suite == 0) {
            tr_warn("NOt active ECC!!");
            return ;
        }

        tls_heap = suite->tls_session->tls_heap;

        switch (suite->state) {


            case TLS_ECC_CERTIFICATE_SIGNATURE_CHECK:
                if (tls_heap->ecc_heap->sgnt == 0) {
                    tls_heap->ecc_heap->sgnt = ecc_get_ecdsa_signature();
                }
                if (!tls_heap->ecc_heap->sgnt) {
                    tr_warn("Signature Fail");
                    alert = 1;
                } else {

                    tls_certificate_signature_verify(suite);
                }

                break;

            case TLS_ECC_MESSAGE_VERIFY:
                if (suite->setups & TLS_SERVER_MODE) {
                    tr_debug("Certi verify valid");
                    if (tls_ecc_start_premaster_secret(0, suite) == 0) {
                        tr_debug("Pre sec start fail");
                        alert = 1;
                    }
                } else {
                    tr_debug("Server Key valid");
                    if (ecc_calculate_public_key(&(tls_heap->ecc_heap->private_key), &ecc_operation_done_callback) == ECC_STATUS_OK) {
                        sec_lib_state_machine_lock(suite, TLS_ECC_GENERATE_PUBLIC_KEY);
                        sec_ecc_state_save(suite);
                    } else {
                        alert = 2;
                    }

                }

                break;
            case TLS_ECC_CERTIFICATE_VERIFY_SIGNATURE:
                if (!result_ptr) {
                    alert = 1;
                } else {
                    tls_heap->ecc_heap->sgnt = result_ptr;
                    suite->setups &= ~TLS_HANSHAKE_HASH;
                    sec_lib_state_machine_trig(suite, TLS_CLIENT_TX_CERTIFICATE_VERIFY);
                }
                active_ecc_sec_suite = 0;
                break;
            case TLS_ECC_GENERATE_PUBLIC_KEY: {
                uint8_t *temp_ptr;
                EllipticPoint *result;
                result = result_ptr;

                //Save Result
                if (suite->setups & TLS_SERVER_MODE) {
                    temp_ptr = tls_heap->ecc_heap->server_public_key;
                } else {
                    temp_ptr = tls_heap->ecc_heap->client_public_key;

                }
                if (result) {
                    tls_ecc_point_reverse_order(temp_ptr, (uint8_t *)result->x.data);
                    tls_ecc_point_reverse_order((temp_ptr + 32), (uint8_t *)result->y.data);
                }

                if (suite->setups & TLS_SERVER_MODE) {
                    if (result) {
                        ecc_library_free_pointer(result);
                        result = 0;
                    }
                    //Cal Signature
                    if (ecc_signature_calculate_hash(tls_heap) == 0) {
                        //SET Array to MPINT
                        uint8_t *ptr = NULL;
                        certificate_chain_internal_t   *temp;


                        temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

                        if (temp) {
                            ptr = x509_get_key_from_pki((uint8_t *) temp->key_chain[(temp->chain_length - 1)], 0);
                        }

                        if (ptr) {
                            uint8_t i;
                            memset(&temp_key_entry.data, 0, MPINT_DATA_SIZE);

                            //Revert bytes
                            for (i = 0; i < 32; i++) {
                                *(((uint8_t *)&temp_key_entry.data) + i) = ptr[31 - i];
                            }
                            status = ecc_calculate_signature(tls_heap->ecc_heap->sgnt, (MPint *) &temp_key_entry, &ecc_operation_done_callback);
                            if (status == ECC_STATUS_OK) {
                                tls_heap->ecc_heap->sgnt = 0;
                                sec_lib_state_machine_lock(suite, TLS_ECC_SIGNATURE_MESSAGE);
                                sec_ecc_state_save(suite);
                            } else {
                                tr_warn("SigNature start Fail");
                                alert = 3;
                            }
                        } else {
                            alert = 4;
                        }
                    } else {
                        //Alert
                        tr_debug("ECC Error");
                        sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                        alert = 4;
                    }


                } else {
                    //Calculate PreMaster Secret
                    if (result == NULL) {
                        alert = 5;
                    } else {
                        if (tls_ecc_start_premaster_secret(result, suite) == 0) {
                            alert = 5;
                            result = 0;

                        }
                    }
                }
            }

            break;

            case TLS_ECC_GENERATE_PREMASTER_SECRET: {
                EllipticPoint *result;
                result = result_ptr;

                if (result) {
                    tls_ecc_point_reverse_order(tls_heap->ecc_heap->pre_secret_mat, (uint8_t *)result->x.data);
                    ecc_library_free_pointer(result);
                }
                active_ecc_sec_suite = 0;
                if ((suite->setups & TLS_SERVER_MODE) == 0) {
                    suite->setups &= ~TLS_HANSHAKE_HASH;
                        sec_lib_state_machine_trig(suite, TLS_UPDATE_HAS_WITH_CERTIFICATE);
                } else {
                    sec_prf_state_set(suite);
                }
            }
            break;

            case TLS_ECC_SIGNATURE_MESSAGE:
                active_ecc_sec_suite = 0;
                if (!result_ptr) {
                    alert = 1;
                } else {
                    tls_heap->ecc_heap->sgnt = result_ptr;

#ifdef PANA_SERVER_API
                    if (suite->setups & TLS_SERVER_MODE) {
                        suite->setups &= ~TLS_HANSHAKE_HASH;
                        sec_lib_state_machine_trig(suite, TLS_TX_SERVER_KEY_EXCHANGE);
                    }
#endif
                }

                break;
            default:

                break;

        }
    }
    if (alert) {
        tr_debug("State: %x, Alert: %x", suite->state, alert);
        if (suite->state == 0x15) {
            sec_lib_state_machine_trig(suite, TLS_ALERT_BAD_CERTIFICATE);
        } else {
            sec_lib_state_machine_trig(suite, TLS_ALERT_DECRYPT);
        }
        active_ecc_sec_suite = 0;
    }
}
void sec_ecc_client_signature_start(sec_suite_t *suite)
{
    uint8_t trig_alert = 0;
    if (ecc_state_idle_check() == ECC_STATUS_OK) {
        //Signature
        tls_heap_t *tls_heap = suite->tls_session->tls_heap;
        ECDSASignature *sgnt;
        tr_debug("HASH Calc for Cert Verify");
        tls_hanshake_hash_cal(tls_heap);

        if (tls_heap->ecc_heap->sgnt == 0) {
            sgnt = ecc_get_ecdsa_signature();
        } else {
            sgnt = tls_heap->ecc_heap->sgnt;
            tls_heap->ecc_heap->sgnt = 0;
        }

        if (sgnt) {
            certificate_chain_internal_t   *temp;
            uint8_t *ptr = 0;
            memset((uint8_t *) sgnt->m_m.data, 0, sizeof(MPint));
            tls_ecc_point_reverse_order((uint8_t *) sgnt->m_m.data, (uint8_t *)tls_heap->hash_buf);

            temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);


            ptr = x509_get_key_from_pki((uint8_t *)temp->key_chain[(temp->chain_length - 1)], 0);
            if (ptr) {
                uint8_t i;
                memset(&temp_key_entry.data, 0, MPINT_DATA_SIZE);

                //Revert bytes
                for (i = 0; i < 32; i++) {
                    *(((uint8_t *)&temp_key_entry.data) + i) = ptr[31 - i];
                }

                if (ecc_calculate_signature(sgnt, (MPint *) &temp_key_entry, &ecc_operation_done_callback) == ECC_STATUS_OK) {
                    tr_debug("Certi verify");
                    sec_lib_state_machine_lock(suite, TLS_ECC_CERTIFICATE_VERIFY_SIGNATURE);
                    sec_ecc_state_save(suite);
                } else {
                    tr_warn("Signature start Fail");
                    ns_dyn_mem_free(sgnt);
                    trig_alert = 1;
                }
            } else {
                tr_warn("Key get Fail");
                ns_dyn_mem_free(sgnt);
                trig_alert = 1;
            }
        } else {
            tr_warn("Signature Allocate Fail");
            trig_alert = 1;
        }
        if (trig_alert) {
            sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
        }
    } else {
        //tr_debug("ECC Busy Wait1");
        sec_lib_state_machine_trig(suite, TLS_ECC_CLIENT_SIGNATURE_START);
    }
}
void sec_ecc_gen_public_key_start(sec_suite_t *suite)
{
    tls_heap_t *tls_heap = suite->tls_session->tls_heap;

    if (ecc_calculate_public_key(&(tls_heap->ecc_heap->private_key), &ecc_operation_done_callback) == ECC_STATUS_OK) {
        sec_lib_state_machine_lock(suite, TLS_ECC_GENERATE_PUBLIC_KEY);
        sec_ecc_state_save(suite);
    } else {
        sec_lib_state_machine_trig(suite, TLS_ECC_GENERATE_PUBLIC_KEY_START);
    }
}
#endif

void eap_fragmentation_init(sec_suite_t *suite)
{
        if (suite->pana_session.eap_frag_buf) {
            buffer_free(suite->pana_session.eap_frag_buf);
            suite->pana_session.eap_frag_buf = NULL;
        }
        suite->pana_session.frag_length = 0;
        if (suite->pana_session.eap_assy_buf) {
            buffer_free(suite->pana_session.eap_assy_buf);
            suite->pana_session.eap_assy_buf = NULL;
        }
        suite->pana_session.assy_length = 0;
}

#ifdef ECC
static void seclib_stop_ecc(sec_suite_t *suite)
{
    if (active_ecc_sec_suite) {
        if (suite == active_ecc_sec_suite) {
            tr_debug("Stop ECC");
            ecc_free_memory();
            active_ecc_sec_suite = 0;
        }
    }
}
#endif

void seclib_session_clean(sec_suite_t *suite)
{
    suite->timer = 0;
    suite->pana_session.auth_info = NULL;
    pana_free_dynamic_ram(suite);
    sec_suite_tls_free(suite, true);
#ifdef ECC
    seclib_stop_ecc(suite);
#endif
}


uint8_t sec_suite_socket_event(uint8_t event_type, sec_suite_t *suite)
{
    suite = sec_suite_verify(suite);
    if (!suite) {
        return 0;
    }

    if (event_type == SOCKET_TX_DONE) {
        sec_tx_done(suite);
    } else {
        tr_debug("SEC Suite CB event. Event type: %x", event_type);
        suite->timer = 15;
    }
    return 1;
}

void sec_set_auth_timeout(sec_suite_t *suite, sec_state_machine_t cur_state)
{
    suite->retry_counter = pana_retry_req_max_get();
    suite->timer = pana_handshake_timeout();
    suite->state = cur_state;
}

static void sec_tx_done(sec_suite_t *suite)
{
    sec_state_machine_t cur_state = suite->state;
    if (suite->timer) {
        if (cur_state != TLS_INIT) {
            switch (cur_state) {
#ifdef ECC
#ifdef PANA_SERVER_API
                case TLS_TX_SERVER_KEY_EXCHANGE:
                    pana_timeout_timer_set(suite, suite->state);
                    break;
#endif

                case TLS_CLIENT_TX_CERTIFICATE_VERIFY:
                    if ((suite->setups & TLS_SERVER_MODE) == 0) {
                        sec_set_auth_timeout(suite, TLS_KEY_CHANGE);
                    } else {
                        sec_lib_state_machine_trig(suite, PANA_READY);
                    }
                    break;
#endif
                case PANA_FAILURE:
                    if (suite->setups & TLS_SERVER_MODE) {
                        pana_timeout_timer_set(suite, suite->state);
                    } else {
                        sec_lib_state_machine_trig(suite, PANA_ERROR);
                    }
                    break;

                case TLS_SERVER_TX_SERVER_HELLO:
                    sec_set_auth_timeout(suite, TLS_SERVER_WAIT_CHANGE_CHIPHERSUITE);
                    break;

                case TLS_KEY_CHANGE:
                    if ((suite->setups & TLS_SERVER_MODE) == 0) {
                        sec_set_auth_timeout(suite, TLS_KEY_CHANGE);
                    } else {

                        tr_debug("Wait EAP RESPONSE");
                        pana_timeout_timer_set(suite, suite->state);
                    }
                    break;
                case TLS_FINISH:
                    if (suite->setups & TLS_SERVER_MODE) {
                        pana_timeout_timer_set(suite, suite->state);
                    } else {
                        sec_set_auth_timeout(suite, TLS_FINISH);
                    }
                    break;


                case TLS_ALERT_INTERNAL:
                case TLS_ALERT_CHIPHER_SUITE:
                case TLS_ALERT_DECRYPT:
                case TLS_ALERT_BAD_CERTIFICATE:

                    tr_debug("Alert TX Done. cur_state: %x", cur_state);
                    suite->state = PANA_FAILURE;
                    suite->timer = 15;
                    suite->retry_counter = 0;
                    break;

                case PANA_PCI_TX:
                case PANA_KEY_UPDATE:
                case PANA_PING_REQ:
                case PANA_KEY_PULL:
                case EAP_IDENTITY_REQ:
                case PANA_REQUEST_TX:
                case TLS_START:
                case TLS_EAP_END_PANA_VERIFY:
                    pana_timeout_timer_set(suite, suite->state);
                    break;
                case EAP_IDENTITY_RES:
                case PANA_START_RESPONSE:
                case PANA_READY:
                    break;

                default:
                    tr_debug("unknown cur_state: %x", cur_state);
                    suite->timer = 100;
                    break;
            }
        }
    }
}

#endif


//************************ECC Certificates end
