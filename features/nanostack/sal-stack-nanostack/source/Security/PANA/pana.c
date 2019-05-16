/*
 * Copyright (c) 2013-2019, Arm Limited and affiliates.
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
#include "Core/include/ns_socket.h"
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
#include "socket_api.h"

#ifdef PANA

#define TRACE_GROUP "pana"

typedef enum {
    ARM_PANA_INIT = 0,
    ARM_PANA_TLS_CB = 1,
} arm_pana_event_id_e;

//Pana Relay Constant
const uint8_t PANA_AUTH_STRING[9] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A'};
const uint8_t PANA_PAA_ENC_KEY_STRING[18] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A', ' ', 'P', 'A', 'A', ' ', 'E', 'n', 'c', 'r'};
const uint8_t PANA_PAC_ENC_KEY_STRING[18] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A', ' ', 'P', 'a', 'C', ' ', 'E', 'n', 'c', 'r'};

static void pana_key_calc(bool enc_key, sec_suite_t *suite);
static void pana_handshake_copy(uint8_t *ptr, uint16_t len, bool request, sec_suite_t *suite);

//TLS message parse support

NS_LARGE int8_t pana_socket = -1;           /*socket variable*/
static int8_t pana_tasklet_id = -1;
static pana_socket_packet_handler_cb *pana_socket_packet_handler = NULL;
static pana_state_machine_step *pana_state_machine_step_cb = NULL;
static pana_eap_tls_up_cb *pana_tls_handler_cb = NULL;

static NS_LARGE uint8_t pana_key_material[32];

/** Pana Protocol Dynamic Parameters */
static pana_lib_parameters_s pana_library_params = {
    .PCI_IRT = 10,
    .PCI_MRT = 60,
    .PCI_MRC = 5,
    .REQ_IRT = 20,
    .REQ_MRT = 60,
    .REQ_MRC = 4,
    .AUTHENTICATION_TIMEOUT = 120,
    .KEY_UPDATE_THRESHOLD = 10,
    .KEY_ID_MAX_VALUE = 0xff,
    .EAP_FRAGMENT_SIZE = EAP_MTU_FRAG_SIZE,
    .AUTH_COUNTER_MAX = 0xff
};

pana_lib_parameters_s *pana_parameters_get(void)
{
    return &pana_library_params;
}

int8_t pana_socket_id_get(void)
{
    return pana_socket;
}

void pana_common_state_machine(sec_suite_t *suite)
{
    if (pana_state_machine_step_cb) {
        pana_state_machine_step_cb(suite);
    }
}

pana_session_t *pana_session_allocate(void)
{
    pana_session_t *p_session =   ns_dyn_mem_alloc(sizeof(pana_session_t));
    if (p_session) {
        memset(p_session, 0, sizeof(pana_session_t));
        p_session->session_ready = false;
        p_session->key_warp = false;
        p_session->user_server = false;
        p_session->eap_id_seq = randLIB_get_8bit(); //Take Random EAP ID
    }
    return p_session;
}

void pana_session_base_init(pana_session_t *p_session)
{

    memset(p_session, 0, sizeof(pana_session_t));
    p_session->session_ready = false;
    p_session->key_warp = false;
    p_session->user_server = false;
    p_session->eap_id_seq = randLIB_get_8bit(); //Take Random EAP ID

}


void pana_session_state_init(pana_session_t *p_session)
{
    p_session->key_warp = false;
    p_session->address_status = 0;
}

pana_heap_t *pana_heap_structure_allocate(void)
{
    pana_heap_t *heap = ns_dyn_mem_temporary_alloc(sizeof(pana_heap_t));
    if (heap) {
        heap->handshake_len = 0;
        heap->handshake_req_offset = 0;
        randLIB_get_n_bytes_random(heap->client_nonce, 16);
    }
    return heap;
}

static buffer_t *pana_eap_payload_to_avp(buffer_t *buf)
{
    uint8_t *ptr;

    uint16_t eap_len, padding;
    eap_len = buffer_data_length(buf);
    padding = eap_len;
    if ((buf = buffer_headroom(buf, 8)) == 0) {
        return NULL;
    }

    buffer_data_reserve_header(buf, 8);
    //tr_debug("EAP AVP LEN: %02x", eap_len);
    ptr = buffer_data_pointer(buf);
    ptr = pana_avp_base_write(AVP_EAP_PAYLOAD_CODE, eap_len, ptr, 0, 0);

    //Check Padding
    padding %= 4;
    if (padding) {
        padding = 4 - padding;
        //tr_debug("Add Pad: %02x", padding);
        if ((buf = buffer_headroom(buf, padding)) != 0) {
            uint8_t *ptr2;
            buffer_data_reserve_header(buf, padding);
            ptr = buffer_data_pointer(buf);
            ptr2 = ptr;
            ptr += padding;
            memmove(ptr2, ptr, eap_len + 8);
        } else {
            return NULL;
        }
    }

    return buf;
}

int8_t pana_get_params(pana_lib_parameters_s *params)
{
    if (params) {
        *params = pana_library_params;
        return 0;
    }
    return -1;
}

int8_t pana_set_params(const pana_lib_parameters_s *params)
{
    int8_t ret_val = -1;
    if (!params) {

    } else if (params->PCI_IRT == 0 || params->PCI_MRT == 0 || params->PCI_MRC == 0) {

    } else if (params->REQ_IRT == 0 || params->REQ_MRT == 0 || params->REQ_MRC == 0) {

    } else if (params->KEY_UPDATE_THRESHOLD == 0 || params->KEY_ID_MAX_VALUE <  3) {

    } else if (params->EAP_FRAGMENT_SIZE < 64 || params->EAP_FRAGMENT_SIZE >= 920) {

    } else if ((params->PCI_IRT > params->PCI_MRT) || (params->REQ_IRT > params->REQ_MRT)) {

    } else {
        ret_val = 0;
        pana_library_params = *params;
    }
    return ret_val;

}

const pana_lib_parameters_s *pana_get_params_ptr(void)
{
    return &pana_library_params;
}

void pana_timeout_timer_set(sec_suite_t *suite, sec_state_machine_t cur_state)
{
    uint32_t timer;
    uint32_t timer_inc;
    uint32_t timer_max;
    uint8_t retry;
    retry = suite->retry_counter;
    if (cur_state == PANA_PCI_TX) {
        timer_inc = (pana_library_params.PCI_IRT * 10);
        timer_max = (pana_library_params.PCI_MRT * 10);
    } else {
        timer_inc = (pana_library_params.REQ_IRT * 10);
        timer_max = (pana_library_params.REQ_MRT * 10);
    }
    timer = timer_inc;
    while (retry > 1) {
        timer += timer_inc;
        if (timer > timer_max) {
            timer = timer_max;
            retry = 0;
        } else {
            retry--;
        }
    }
    //Set State
    suite->state = cur_state;
    suite->timer = timer;

}
uint8_t pana_retry_req_max_get(void)
{
    return pana_library_params.REQ_MRC;
}

uint32_t pana_handshake_timeout(void)
{
    uint32_t ret_val = (pana_library_params.AUTHENTICATION_TIMEOUT * 10);
    return ret_val;
}

int pana_retry_check(uint8_t counter, sec_state_machine_t cur_state)
{
    int ret_val = -1;
    if (cur_state == PANA_PCI_TX) {
        if (counter < pana_library_params.PCI_MRC) {
            ret_val = 0;
        }
    } else {
        if (counter < pana_library_params.REQ_MRC) {
            ret_val = 0;
        }
    }
    return ret_val;
}

uint8_t *pana_key_get(const uint8_t *key)
{
    static uint8_t key_seed[8] = {'Z', 'i', 'g', 'B', 'e', 'e', 'I', 'P'};
    SHALIB_init_HMAC(key, 16);
    SHALIB_push_data_HMAC(key_seed, 8);
    SHALIB_finish_HMAC(pana_key_material, 8);
    return pana_key_material;
}

//static void pana_key_pair_data_generate(sec_suite_t *suite)
//{
//    uint8_t *pac, *paa;
//    if (suite->setups & TLS_SERVER_MODE) {
//        //Server
//        paa = suite->interface->iid_eui64;
//        pac = &suite->session_address[8];
//    } else {
//        pac = suite->interface->iid_eui64;
//        paa = &suite->session_address[8];
//
//    }
//    SHALIB_push_data_HMAC(paa, 8);
//    SHALIB_push_data_HMAC(pac, 8);
//}
///* Define Pair wise Key*/
//static uint8_t pana_hmac_temp_key[64];
//static uint8_t *pana_generate_key_pair_key(sec_suite_t *suite, uint8_t *common_key, uint8_t *key_ptr)
//{
//    static uint8_t key_seed[8] = {'K', 'e', 'y', 'P', 'a', 'i', 'r', '?'};
//    memcpy(pana_hmac_temp_key, suite->pana_session.pana_auth_key, 32);
//    memcpy(&pana_hmac_temp_key[32], common_key, 16);
//
//    SHALIB_init_HMAC(pana_hmac_temp_key, 48);
//
//    SHALIB_push_data_HMAC(key_seed, 8);
//    pana_key_pair_data_generate(suite);
//    SHALIB_push_data_HMAC(&suite->pana_session.auth_cnt, 1);
//    SHALIB_finish_HMAC(common_key, 4);
//    return common_key;
//}

void pana_session_init_by_session_ptr(sec_suite_t *suite, auth_info_t *auth_ptr)
{
    if (suite) {
        suite->setups = 0;
        pana_session_state_init(&suite->pana_session);
        suite->pana_session.auth_info = auth_ptr;

        eap_fragmentation_init(suite);
        if (suite->pana_session.session_ready) {
            tr_debug("Ping Notify");
            suite->state = PANA_PING_REQ;
            suite->timer = 1;
            suite->retry_counter = 0;
        } else {
            if (sec_pana_protocol_init(suite) == 0) {
                sec_lib_state_machine_trig(suite, PANA_ERROR);
            }
        }
    }
}

void pana_authentication_ready(uint8_t status, protocol_interface_info_entry_t *cur_interface)
{
    if (status) {
        nwk_6lowpan_bootsrap_pana_authentication_cb(true, cur_interface);
    } else {
        nwk_6lowpan_bootsrap_pana_authentication_cb(false, cur_interface);
    }
}

#ifdef ECC
extern void sec_ecc_state_free(sec_suite_t *suite);
#endif

void pana_reset_values(uint16_t pana_id)
{
    sec_suite_t *suite = 0;

    suite = sec_suite_selected_py_pan_id(pana_id);
    if (suite) {
        suite->timer = 0;
        sec_suite_tls_free(suite, true);
        pana_free_dynamic_ram(suite);
#ifdef ECC
        sec_ecc_state_free(suite);
#endif
    }

}

static void pana_socket_callback(void *cb)
{
    socket_buffer_callback_t *cb_buf = cb;
    if (cb_buf->event_type == SOCKET_DATA) {
        buffer_t *buf = cb_buf->buf;
        if (buf) {

            //Find or create session //Do CB HERE
            buf->socket = socket_dereference(buf->socket);
            buf->session_ptr = NULL;
            pana_socket_packet_handler(buf);
        }
    } else {

        sec_suite_socket_event(cb_buf->event_type, cb_buf->session_ptr);
    }
}

void pana_event_handler(arm_event_s *event)
{
    switch (event->event_type) {
        case ARM_PANA_INIT:
            tr_debug("Pana Tasklet Init");
            break;

        case ARM_PANA_TLS_CB:
            if (event->data_ptr) {
                buffer_t *buf = event->data_ptr;
                sec_suite_t *tls_suite = buf->session_ptr;
                buf->session_ptr = NULL;

                tls_suite = sec_suite_verify(tls_suite);

                if (tls_suite && pana_tls_handler_cb) {
                    buf = pana_tls_handler_cb(buf, tls_suite);
                } else {
                    tr_warn("Pana/TLS er");
                }

                if (buf) {
                    buffer_free(buf);
                }
            }
            break;
        default:
            break;
    }
}

bool pana_socket_init(pana_socket_packet_handler_cb *socket_handler, pana_state_machine_step *state_machine_stepper, pana_eap_tls_up_cb *tls_handler_cb)
{
    tr_debug("Pana sub-layer init");
    if (pana_socket == -1) {
        if (socket_create(SOCKET_FAMILY_IPV6, SOCKET_TYPE_DGRAM, 0, &pana_socket, UDP_PORT_PANA, pana_socket_callback, true) != eOK) {
            return false;
        }
        //GENERATE TASKLET if not created before
        if (pana_tasklet_id == -1) {
            pana_tasklet_id = eventOS_event_handler_create(&pana_event_handler, ARM_PANA_INIT);
            if (pana_tasklet_id < 0) {
                socket_close(pana_socket);
                pana_socket = -1;
                return false;
            }
        }
    }
    tr_debug("Pana socket Id %i", pana_socket);

    pana_socket_packet_handler = socket_handler;
    pana_state_machine_step_cb = state_machine_stepper;
    pana_tls_handler_cb = tls_handler_cb;
    return true;
}

void pana_set_agend_address(buffer_t *buf, bool relay, sec_suite_t *suite)
{
    uint8_t *ptr = 0;
    buf->interface = suite->interface;
    protocol_interface_info_entry_t *cur = buf->interface;
    if (cur) {

        if (suite->pana_session.user_server) {
            if (relay) {
                ptr = protocol_6lowpan_nd_border_router_address_get(buf->interface->nwk_id);
                if (ptr) {
                    memcpy(buf->src_sa.address, ptr, 16);
                    ptr = suite->pana_session.session_relay_address;
                    memcpy(buf->dst_sa.address, ptr, 16);
                }
                //
                buf->options.ll_security_bypass_tx = false;
            } else {
                //Default
                addr_interface_get_ll_address(cur, buf->src_sa.address, 2);
                ptr = suite->session_address;
                memcpy(buf->dst_sa.address, ptr, 16);
                buf->options.ll_security_bypass_tx = true;
            }
        } else {
            if (relay) {
                ptr = protocol_6lowpan_nd_border_router_address_get(buf->interface->nwk_id);
                if (ptr) {
                    memcpy(buf->dst_sa.address, ptr, 16);
                    memcpy(suite->pana_session.session_relay_address, ptr, 16);
                    //Select source by Dst address
                    (void) addr_interface_select_source(cur, buf->src_sa.address, buf->dst_sa.address, 0);
                }
                buf->options.ll_security_bypass_tx = false;
            } else {


                //tr_debug("LL Agent");
                nd_router_t   *object = nd_get_pana_address();
                if (object) {
                    icmp_nd_set_nd_def_router_address(buf->dst_sa.address, object);
                    tr_debug("MD Router adr: %s", trace_ipv6(buf->dst_sa.address));
                } else {
                    //tr_debug("Use Mac Coordinator");
                    protocol_6lowpan_interface_get_link_local_cordinator_address(cur, buf->dst_sa.address);
                }
                ptr = suite->session_address;
                memcpy(ptr, buf->dst_sa.address, 16);

                addr_interface_get_ll_address(cur, buf->src_sa.address, 1);
                buf->options.ll_security_bypass_tx = true;
            }
        }
    }
    tr_debug("DST %s src %s", trace_ipv6(buf->dst_sa.address), trace_ipv6(buf->src_sa.address));
    buf->src_sa.addr_type = ADDR_IPV6;
    buf->dst_sa.addr_type = ADDR_IPV6;

}


buffer_t *build_pana_base(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    uint8_t *ptr;
    buf->session_ptr = NULL;
    buf = buffer_headroom(buf, PANA_HEADER_LENGTH);
    if (!buf) {
        return NULL;
    }

    buf = buffer_turnaround(buf); // In case we're reusing a buffer
    ptr = buffer_data_reserve_header(buf, PANA_HEADER_LENGTH);
    header->payload_len = buffer_data_length(buf);
    pana_header_write(ptr, header);

    buf->src_sa.port = UDP_PORT_PANA;
    buf->dst_sa.port = suite->session_port;
    buf->info = (buffer_info_t)(B_DIR_DOWN + B_FROM_APP + B_TO_UDP);
    if (header->type != PANA_MSG_RELAY || suite->pana_session.user_server) {
        buffer_socket_set(buf, socket_pointer_get(pana_socket));
        buf->session_ptr = suite;
    } else {
        buf->socket = socket_dereference(buf->socket);
        buf->session_ptr = NULL;
    }
    //tr_debug("From Pana-> Core");
    buf->interface = suite->interface;
    tr_debug("PANA len: %d", header->payload_len);
    return buf;
}

void pana_eap_tls_up(buffer_t *buf, sec_suite_t *suite)
{
    buf = eap_up(buf, suite);
    if (!buf) {
        return;
    }
    pana_eap_down(buf, suite);
}

void pana_eap_down(buffer_t *buf, sec_suite_t *suite)
{
    buf = eap_down(buf, suite);
    if (!buf) {
        return;
    }
    //Set EAP Payload
    pana_eap_payload_down(buf, NULL, suite);
}

void pana_eap_payload_down(buffer_t *buf, const uint8_t *nonce, sec_suite_t *suite)
{
    buf = pana_eap_payload_to_avp(buf);

    if (!buf) {
        return;
    }

    if (nonce) {
        if ((buf = buffer_headroom(buf, 24)) == 0) {
            return;
        }
        buffer_data_reserve_header(buf, 24);
        uint8_t *ptr = buffer_data_pointer(buf);
        ptr = pana_avp_write_n_bytes(AVP_NONCE_CODE, 16, nonce, ptr);
    }

    pana_down(buf, suite);
}


void pana_handshake_copy(uint8_t *ptr, uint16_t len, bool request, sec_suite_t *suite)
{
    if (suite->pana_session.pana_heap) {
        uint8_t *dptr = 0;
        pana_heap_t *pheap = suite->pana_session.pana_heap;

        dptr = pheap->pana_handshake;
        if (!request) {
            dptr += pheap->handshake_req_offset;
            pheap->handshake_len = len + pheap->handshake_req_offset;
        } else {
            pheap->handshake_req_offset = len;
            pheap->handshake_len = len;
        }
        memcpy(dptr, ptr, len);
    }
}


void pana_down(buffer_t *buf, sec_suite_t *suite)
{
    //Check Request Or Response
    pana_header_t header;
    header.type = PANA_MSG_PA;
    if (suite->pana_session.user_server) {
        if (suite->state == PANA_REQUEST_TX) {
            header.flags = PANA_FLAGS_START | PANA_FLAGS_REQUEST;
        } else if (suite->state == EAP_PANA_FINISH || suite->state == PANA_FAILURE) {
            header.flags = (PANA_FLAGS_REQUEST | PANA_FLAGS_COMPLETE);
        } else if (suite->state == PANA_FAILURE_RESPONSE) {
            header.flags = (PANA_FLAGS_RESPONSE | PANA_FLAGS_COMPLETE);
        } else {
            header.flags = PANA_FLAGS_REQUEST;
        }
    } else {

        if (suite->state == PANA_FAILURE) {
            header.flags = PANA_FLAGS_COMPLETE;
        } else if (suite->state == PANA_START_RESPONSE) {
            header.flags = PANA_FLAGS_START;
        } else {
            header.flags = 0;
        }
    }

    if (header.flags & PANA_FLAGS_REQUEST) {
        header.seq = suite->pana_session.req_seq;
    } else {
        header.seq = suite->pana_session.res_seq;
    }
    header.session_id = suite->pana_session.session_id;
    pana_set_agend_address(buf, false, suite);
    buf = build_pana_base(buf, &header, suite);
    if (buf) {
        /**
         * Copy Authentication start message
         */
        if (header.flags & PANA_FLAGS_START) {
            uint16_t len = buffer_data_length(buf);
            uint8_t *ptr = buffer_data_pointer(buf);

            if (header.flags & PANA_FLAGS_REQUEST) {
                pana_handshake_copy(ptr, len, true, suite);
            } else {
                pana_handshake_copy(ptr, len, false, suite);
            }
        }

        if (suite->pana_session.address_status & 1) {
            tr_debug("Build Relay");
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


}


buffer_t *pana_relay_avp_build(buffer_t *buf, sec_suite_t *suite)
{
    uint8_t *ptr, *adr_ptr;
    uint16_t relay_len, padding;
    relay_len = buffer_data_length(buf);
    padding = relay_len;
    buf->socket = socket_dereference(buf->socket);
    buf->session_ptr = NULL;
    if ((buf = buffer_headroom(buf, 36)) == 0) {
        return buf;
    } else {
        buffer_data_reserve_header(buf, 36);
        ptr = buffer_data_pointer(buf);
        ptr = pana_avp_base_write(AVP_PAC_INFO_CODE, 18, ptr, 0, 0);
        //SET Relay IPV6 address
        if (suite->pana_session.user_server) {
            memcpy(ptr, suite->session_address, 16);
            ptr += 16;
            ptr = common_write_16_bit(suite->session_port, ptr);

            adr_ptr = protocol_6lowpan_nd_border_router_address_get(buf->interface->nwk_id);
            if (adr_ptr) {
                memcpy(buf->src_sa.address, adr_ptr, 16);
                memcpy(buf->dst_sa.address, suite->pana_session.session_relay_address, 16);
                buf->dst_sa.port = suite->pana_session.relay_port;
            }
        } else {
            memcpy(ptr, buf->src_sa.address, 16);
            ptr += 16;
            ptr = common_write_16_bit(buf->src_sa.port, ptr);
        }
        //PADDING for PAC
        ptr = common_write_16_bit(0, ptr);
        //PANA Relay AVP header Write data is already there
        ptr = pana_avp_base_write(AVP_RELAY_MSG_CODE, relay_len, ptr, 0, 0);
    }
    //Enable security for relay allways by Default
    buf->options.ll_security_bypass_tx = false;
    padding %= 4;
    if (padding) {
        padding = 4 - padding;
        //tr_debug("Add Pad: %02x", padding);
        if ((buf = buffer_headroom(buf, padding)) != 0) {
            uint8_t *ptr2;
            buffer_data_reserve_header(buf, padding);
            ptr = buffer_data_pointer(buf);
            ptr2 = ptr;
            ptr += padding;
            memmove(ptr2, ptr, relay_len + 36);
        }
    }
    return buf;
}

bool pana_auth_check(uint8_t *ptr, uint16_t length, uint8_t *authency, uint8_t *key)
{
    if (!authency) {
        return false;
    }
    uint8_t hasn_buf_temp[16];
    uint8_t compare_hash_temp[16];

    //tr_debug("AV SUCCESS. Hash RX: %s", trace_array(ptr, 16));
    memcpy(compare_hash_temp, authency, 16);
    memset(authency, 0, 16);
    length += 16;
    ptr -= 16;//Shift Pana Headers back
    //tr_debug("Calc: %s", trace_array(key, 32) );
    SHALIB_init_HMAC(key, 32);
    SHALIB_push_data_HMAC(ptr, length);
    //tr_debug("%s", trace_array(ptr,length));
    SHALIB_finish_HMAC(hasn_buf_temp, 4);

    if (memcmp(hasn_buf_temp, compare_hash_temp, 16) != 0) {
        tr_debug("HASH AUTH Fail. RX: %s", trace_array(compare_hash_temp, 16));
        tr_debug("Cal: %s", trace_array(hasn_buf_temp, 16));
        return false;
    }
    return true;
}

int8_t pana_ccm_data_crypt(uint8_t *ptr, uint16_t len, uint8_t operation_type, uint32_t message_seq, sec_suite_t *suite)
{
    uint8_t *explict_ptr;
    uint8_t *key_ptr = 0;
    ccm_globals_t ccm_ptr;
    key_ptr = suite->pana_session.pana_PAA_enc_key;

    //Here Comes AES Decrypt
    if (!ccm_sec_init(&ccm_ptr, AES_SECURITY_LEVEL_ENC, key_ptr, operation_type, 3)) {
        return -1;
    }

    explict_ptr = ccm_ptr.exp_nonce;
    //Set IV
    explict_ptr = common_write_32_bit(suite->pana_session.pana_key_id, explict_ptr);
    //SET EXP 4 octest Session ID, 4 Octet Pana SQN number
    explict_ptr = common_write_32_bit(suite->pana_session.session_id, explict_ptr);
    explict_ptr = common_write_32_bit(message_seq, explict_ptr);
    ccm_ptr.data_len = len;
    ccm_ptr.data_ptr  = ptr;
    return ccm_process_run(&ccm_ptr);
}

buffer_t *pana_relay_parse(buffer_t *buf)
{
    uint8_t *ptr;
    buf->options.ll_security_bypass_tx = true;
    //tr_debug("Relay RX");
    ptr = buffer_data_pointer(buf);
    uint16_t length = buffer_data_length(buf);

    pana_avp_t pac_info;
    pac_info.code = AVP_PAC_INFO_CODE;

    if (!pana_avp_discover(ptr, length, &pac_info)  || pac_info.len != 18) {
        tr_debug("No Pac info");
        return buffer_free(buf);
    }

    pana_avp_t relay_msg;
    relay_msg.code = AVP_RELAY_MSG_CODE;

    if (!pana_avp_discover(ptr, length, &relay_msg)) {
        tr_debug("No Relay MSG");
        return buffer_free(buf);
    }
    //Set Message data to relay msg
    buffer_data_pointer_set(buf, relay_msg.avp_ptr);
    buffer_data_length_set(buf, relay_msg.len);
    //Set Destination to Pac Info
    ptr = pac_info.avp_ptr;
    memcpy(buf->dst_sa.address, ptr, 16);
    //buf->dst_sa.addr_type = ADDR_IPV6;
    ptr += 16;
    buf->dst_sa.port = common_read_16_bit(ptr);
    ptr += 2;
    //tr_debug("%s", trace_array(buf->dst_sa.address, 16) );
    return buf;
}

void pana_session_startms_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    uint32_t prf_algorythm = 0;
    uint32_t integrity_algorythm = 12;
    uint32_t key_wrap = 0;
    bool key_wrap_parsed = false;
    uint16_t len = buffer_data_length(buf);
    uint8_t *ptr = buffer_data_pointer(buf);

    pana_avp_t avp_temp;
    //Read Resul and Key id if they are coming
    avp_temp.code = AVP_PRF_ALGORYTHM_CODE;
    avp_temp.len = 0;
    if (pana_avp_discover(ptr, len, &avp_temp) &&  avp_temp.len == 4) {
        prf_algorythm = common_read_32_bit(avp_temp.avp_ptr);
    }

    avp_temp.code = AVP_INTEGRIRTY_ALGORYTHM_CODE;
    avp_temp.len = 0;
    if (pana_avp_discover(ptr, len, &avp_temp) &&  avp_temp.len == 4) {
        integrity_algorythm = common_read_32_bit(avp_temp.avp_ptr);
    }

    avp_temp.code = AVP_KEY_WRAP_ALG_CODE;
    avp_temp.len = 0;
    if (pana_avp_discover(ptr, len, &avp_temp) &&  avp_temp.len == 4) {
        key_wrap = common_read_32_bit(avp_temp.avp_ptr);
        key_wrap_parsed = true;
    }

    bool drop_message = false;
    if ((header->flags & PANA_FLAGS_REQUEST)  == PANA_FLAGS_RESPONSE) {
        if (prf_algorythm != suite->pana_session.prf_algorythm) {
            tr_debug("PRF!!");
            drop_message = true;
        } else if (integrity_algorythm != suite->pana_session.integrity_algorythm) {
            tr_debug("int!!");
            drop_message = true;
        }
        if (key_wrap_parsed && key_wrap != suite->pana_session.key_wrap) {
            tr_debug("key!!");
            drop_message = true;
        }

    } else {
        if (prf_algorythm != 5) {
            drop_message = true;
        } else if (integrity_algorythm != 12) {
            drop_message = true;
        }
    }

    if (!drop_message) {
        if (key_wrap_parsed) {
            suite->pana_session.key_warp = true;
            suite->pana_session.key_wrap = key_wrap;
        }
        len += 16;
        ptr -= 16; //Shift Pana Headers back
        if ((header->flags & PANA_FLAGS_REQUEST)  == PANA_FLAGS_RESPONSE) {
            sec_lib_state_machine_trig(suite, EAP_IDENTITY_REQ);
            pana_handshake_copy(ptr, len, false, suite);
        } else {
            suite->pana_session.integrity_algorythm = integrity_algorythm;
            suite->pana_session.prf_algorythm = prf_algorythm;
            sec_lib_state_machine_trig(suite, PANA_START_RESPONSE);
            pana_handshake_copy(ptr, len, true, suite);
        }
        suite->retry_counter = 0;

    }

    buffer_free(buf);
}

buffer_t *pana_auth_message_handler(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    if (!buf) {
        return NULL;
    }
    protocol_interface_info_entry_t *cur = buf->interface;
    if (!cur) {
        return buffer_free(buf);
    }

    uint16_t length = buffer_data_length(buf);
    uint8_t *ptr = buffer_data_pointer(buf);
    pana_avp_t avp_temp;

    if (suite->pana_session.session_ready) {
        return buffer_free(buf);
    }

    if (sec_check_suite_ptrs(suite) == 0) {
        tr_warn("SEC Lib Fail");
        return buffer_free(buf);
    }

    avp_temp.code = AVP_NONCE_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp)) {
        if (avp_temp.len == 16) {
            if ((header->flags & PANA_FLAGS_REQUEST)  == PANA_FLAGS_RESPONSE) {
                memcpy(suite->pana_session.pana_heap->client_nonce, avp_temp.avp_ptr, 16);
            } else {
                memcpy(suite->pana_session.pana_heap->agent_nonce, avp_temp.avp_ptr, 16);
            }
            //tr_debug("Pana:A_NONCE OK");

        } else {
            tr_debug("A_NONCE length fail, Len: %x", avp_temp.len);
        }
    }

    avp_temp.code = AVP_EAP_PAYLOAD_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp)) {
        ptr = avp_temp.avp_ptr;
        if (avp_temp.len > 4) {
            buf->buf_ptr = 0;
            buf->buf_end = avp_temp.len;
            memmove(buf->buf, ptr, avp_temp.len);
            return buf;
        }
    }

    return buffer_free(buf);
}

void pana_start_message_build(buffer_t *buf, sec_suite_t *suite)
{
    uint8_t *ptr;
    //tr_debug("TX Pana Start Response");
    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);
    ptr = pana_avp_32_bit_write(AVP_PRF_ALGORYTHM_CODE, suite->pana_session.prf_algorythm, ptr);
    ptr = pana_avp_32_bit_write(AVP_INTEGRIRTY_ALGORYTHM_CODE, suite->pana_session.integrity_algorythm, ptr);
    if (suite->pana_session.key_warp) {
        ptr = pana_avp_32_bit_write(AVP_KEY_WRAP_ALG_CODE, suite->pana_session.key_wrap, ptr);
    }
    buffer_data_end_set(buf, ptr);
    pana_down(buf, suite);
}



void eap_tls_payload_push(buffer_t *buf)
{
    arm_event_s event = {
        .receiver = pana_tasklet_id,
        .sender = 0,
        .data_ptr = buf,
        .event_type = ARM_PANA_TLS_CB,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };
    if (eventOS_event_send(&event) != 0) {
        tr_warn("Free Buffer if fail");
        buf->session_ptr = NULL;
        buffer_free(buf);
    }
}

void pana_free_dynamic_ram(sec_suite_t *suite)
{
    if (suite->pana_session.pana_heap) {
        tr_debug("Free Pana Heap");
        ns_dyn_mem_free(suite->pana_session.pana_heap);
        suite->pana_session.pana_heap = NULL;
    }
    eap_fragmentation_init(suite);
}

static void pana_key_calc(bool enc_key, sec_suite_t *suite)
{
    pana_heap_t *pheap = suite->pana_session.pana_heap;

    //tr_debug("Pana Auth verify. MSK: %s", trace_array(suite->pana_session.MSK, 64) );
    SHALIB_init_HMAC(pheap->MSK, 64);
    if (enc_key) {
        //tr_debug("Cal Pana En Key start");
        SHALIB_push_data_HMAC(PANA_PAA_ENC_KEY_STRING, 18);
    } else {
        SHALIB_push_data_HMAC(PANA_AUTH_STRING, 9);
    }

    //tr_debug("Handshake data: %s", trace_array(pheap->pana_handshake, pheap->handshake_len));
    SHALIB_push_data_HMAC(pheap->pana_handshake, pheap->handshake_len);
//  tr_debug("Handshake data");
//  tr_debug("C Nonce: %s", trace_array(pheap->client_nonce, 16) );
    SHALIB_push_data_HMAC(pheap->client_nonce, 16);
//  tr_debug("A Nonce: %s", trace_array(pheap->agent_nonce, 16) );
    SHALIB_push_data_HMAC(pheap->agent_nonce, 16);
    uint8_t temp32_buf[4];
//  tr_debug("Key ID: %s", trace_array(temp32_buf, 4) );
    common_write_32_bit(suite->pana_session.pana_key_id, temp32_buf);
    SHALIB_push_data_HMAC(temp32_buf, 4);
    SHALIB_push_data_HMAC(&(const uint8_t) {
        1
    }, 1);
    if (enc_key) {
        uint8_t *key_ptr = suite->pana_session.pana_PAA_enc_key;

        SHALIB_finish_HMAC(key_ptr, 4);
    } else {
        SHALIB_finish_HMAC(suite->pana_session.pana_auth_key, 8);
    }
}

void pana_key_calculation(sec_suite_t *suite)
{
    pana_key_calc(false, suite);
    pana_key_calc(true, suite);
}

void pana_auth_hash_calc(uint8_t *data_ptr, uint16_t data_length, uint8_t *key)
{
    SHALIB_init_HMAC(key, 32);
    SHALIB_push_data_HMAC(data_ptr, data_length);
    data_ptr += (data_length - 16);
    SHALIB_finish_HMAC(data_ptr, 4);
}

#ifdef ECC
static void certificate_copy_block(const arm_certificate_chain_entry_s *rx_chain_info, certificate_chain_internal_t *cert)
{
    uint8_t i;
    cert->chain_length = rx_chain_info->chain_length;
    for (i = 0; i < cert->chain_length; i++) {
        //Copy Certi
        cert->certi_chain[i] = rx_chain_info->cert_chain[i];
        cert->certi_len[i] = rx_chain_info->cert_len[i];
        //Copy Cur Key
        cert->key_chain[i] = rx_chain_info->key_chain[i];
    }
}
#endif

int8_t pana_interface_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info)
{
#ifdef ECC
    if (!chain_info) {
        return -1;
    }
    certificate_chain_internal_t temp_certi;
    certificate_copy_block(chain_info, &temp_certi);
    return x509_cetificate_chain_push(SEC_NWK_AUTHENTICATION_CERTI_CHAIN, &temp_certi);
#else
    (void) chain_info;
    return -1;
#endif
}

#endif /*PANA*/
//************************ECC Certificates end

/* end of file */
