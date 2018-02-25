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
#define TRACE_GROUP "PanC"

static pana_client_session_update_cb *pana_client_nvm_storage_cb = NULL;
static pana_client_session_get_cb *pana_client_session_get = NULL;
static uint8_t *pana_client_nvm_buffer = 0;

static void pana_complete_msg_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
static void pana_client_packet_handler(buffer_t *buf);
static buffer_t *pana_auth_msg_build(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
static void pana_client_pna_handler(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
static bool pana_check_address(buffer_t *buf);
static uint8_t *pana_avp_zip_key_req(uint8_t *dptr, protocol_interface_info_entry_t *cur);
static void pana_client_session_nvm_udate(sec_suite_t *suite);
static void pana_session_data_load_to_session(uint8_t *data_buf, sec_suite_t *suite);
static uint8_t pana_ping_notify_msg_generate(uint8_t key_req, sec_suite_t *suite);

static uint8_t *pana_avp_zip_key_req(uint8_t *dptr, protocol_interface_info_entry_t *cur)
{
    uint8_t key_req[2];
    key_req[0] = 1;
    key_req[1] = mac_helper_default_key_index_get(cur);
    return pana_avp_vendor_id_write_n_bytes(PANA_EAP_KEYREQ_TYPE, 2,key_req, dptr, ZIGBEE_VENDOR_ID);
}

bool pana_check_address(buffer_t *buf)
{
    if (buf->src_sa.address[0] == 0xfe && buf->dst_sa.address[0] == 0xfe) {
        return true;
    }
    return false;
}

static buffer_t *pana_auth_msg_build(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    uint8_t *ptr;
    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);
    ptr = pana_avp_32_bit_write(AVP_KEY_ID_CODE, suite->pana_session.pana_key_id, ptr);
    ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);
    buffer_data_end_set(buf, ptr);
    header->flags = PANA_FLAGS_COMPLETE;
    return build_pana_base(buf, header, suite);

}

static uint8_t *pana_client_keywrap_parse(uint8_t *ptr, uint16_t len, sec_suite_t *suite)
{
    suite->pana_session.key_warp = true;
    pana_avp_t avp_data;
    avp_data.code = PANA_EAP_KEYWRAP_TYPE;
    if (!pana_avp_discover(ptr, len, &avp_data) || avp_data.len != 18 ) {
        return NULL;
    }

    if (avp_data.flags & PANA_EAP_VENDOR_FLAG) {
        if (avp_data.vendor_id != ZIGBEE_VENDOR_ID) {
            tr_debug("Vendor not ZIP: %02"PRIu32, avp_data.vendor_id);
            return NULL;
        }
    }

    tr_debug("Network Key id: %02x, Ctr: %02x", avp_data.avp_ptr[16], avp_data.avp_ptr[17]);
    return avp_data.avp_ptr;
}

static bool pana_message_authency_validate(uint8_t *ptr, uint16_t length, uint8_t *auth_key)
{
    pana_avp_t avp_temp;
    avp_temp.code = AVP_AUTHENCY_CODE;
    if (!pana_avp_discover(ptr, length, &avp_temp)) {
        return false;
    }

    return pana_auth_check(ptr, length, avp_temp.avp_ptr, auth_key);
}

static void pana_complete_msg_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    uint16_t length = buffer_data_length(buf);
    uint8_t *ptr = buffer_data_pointer(buf);

    if (sec_check_suite_ptrs(suite) == 0) {
        buffer_free(buf);
        return;
    }

    if (!(header->flags & PANA_FLAGS_REQUEST)) {
        buffer_free(buf);
        return;
    }

    uint32_t key_id = 0xffffffff;

    bool key_id_parsed = false;
    uint32_t result = 0; //Default if not sended
    pana_avp_t avp_temp;
    //Read Resul and Key id if they are coming
    avp_temp.code = AVP_RESULT_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp) &&  avp_temp.len == 4) {
        result = common_read_32_bit(avp_temp.avp_ptr);
    }

    avp_temp.code = AVP_KEY_ID_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp) &&  avp_temp.len == 4) {
        key_id= common_read_32_bit(avp_temp.avp_ptr);
        key_id_parsed = true;
    }


    uint32_t lifetime = 0xffffffff;
    tr_debug("Handle Compelete request");

    if ((header->agent_retry) && suite->state == PANA_READY) {

        if (!pana_message_authency_validate(ptr, length, suite->pana_session.pana_auth_key) ) {
            buffer_free(buf);
            return;
        }

        tr_debug("RE TX response for Pana Complete");
        goto build_response;
    }

    bool eap_status = true;
    avp_temp.code = AVP_EAP_PAYLOAD_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp)) {
        eap_header_t eap_header;
        if (eap_header_parse(avp_temp.avp_ptr, avp_temp.len, &eap_header) ) {
            if (eap_header.eap_code == EAP_SUCCESS ) {
                tr_debug("EAP success");
            } else if(eap_header.eap_code == EAP_FAILURE) {
                tr_debug("EAP Failure");
                eap_status = false;
            }
        }
    }


    //Validate Result
    if (result || !eap_status) {
        tr_debug("PANA / EAP Failure Result");
        //Check State
        if (suite->state != PANA_PING_REQ) {
            goto pana_failure;
        }

        tr_debug("Reset Pana for new session");
        suite->pana_session.key_warp = false;
        suite->pana_session.session_ready = false;
        pana_session_init_by_session_ptr(suite, suite->pana_session.auth_info);
        buffer_free(buf);
        return;

    }

    avp_temp.code = AVP_SESSION_LIFETIME_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp) &&  avp_temp.len == 4) {
        lifetime = common_read_32_bit(avp_temp.avp_ptr);
    }

    if (!key_id_parsed) {
        goto pana_failure;
    }

    suite->pana_session.pana_key_id = key_id;
    suite->pana_session.session_lifetime = lifetime;
    if (suite->state != PANA_READY) {
        pana_key_calculation(suite);
    }

    if (!pana_message_authency_validate(ptr, length, suite->pana_session.pana_auth_key)) {
        goto pana_failure;
    }


    avp_temp.code = AVP_ENCRYPT_ALGORITHM_CODE;
    if (pana_avp_discover(ptr, length, &avp_temp)) {
        tr_debug("Key Delivery");
        if (pana_ccm_data_crypt(avp_temp.avp_ptr, avp_temp.len, AES_CCM_DECRYPT, header->seq, suite) != 0) {
            goto pana_failure;
        }
        uint8_t *key_material = pana_client_keywrap_parse(avp_temp.avp_ptr, avp_temp.len, suite);
        if (!key_material) {
            goto pana_failure;
        }
        if (!suite->pana_session.auth_info) {
            goto pana_failure;
        }

        tr_debug("Valid");
        suite->pana_session.nwk_key_id = key_material[16];
        suite->pana_session.auth_cnt = key_material[17];
        auth_info_t *pana_auth_info_temp = suite->pana_session.auth_info;
        memcpy(pana_auth_info_temp->network_key, key_material, 16);
        pana_auth_info_temp->key_id = key_material[16];
    }
    tr_debug("Server AUTH_OK");
    suite->state = PANA_READY;
    suite->timer = 95;

    build_response:
    buf = pana_auth_msg_build(buf, header, suite);
    if (!buf) {
        return;
    }

    pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);
    pana_set_agend_address(buf, false, suite);
    protocol_push(buf);
    return;


    pana_failure:
    tr_debug("Drop Key MSG");
    sec_lib_state_machine_trig(suite, PANA_FAILURE); //shuold be calc
    buffer_free(buf);
    return;
}

static void pana_client_pna_handler(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{

    protocol_interface_info_entry_t *cur = buf->interface;
    if (!cur) {
        goto end_of_function;
    }

    if (!suite->pana_session.session_ready) {
        goto end_of_function;
    }

    uint16_t length = buffer_data_length(buf);
    uint8_t *ptr = buffer_data_pointer(buf);
    tr_debug("Parse Ping Auth Notify");

    if (!pana_message_authency_validate(ptr, length, suite->pana_session.pana_auth_key)) {
        tr_warn("Auth Fail");
        goto end_of_function;
    }

    pana_avp_t avp_temp;
    avp_temp.code = AVP_ENCRYPT_ALGORITHM_CODE;

    if (pana_avp_discover(ptr, length, &avp_temp) ) {
        tr_debug("ZIP Key");
        //Calc key
        if (pana_ccm_data_crypt(avp_temp.avp_ptr, avp_temp.len, AES_CCM_DECRYPT, header->seq, suite) != 0) {
            tr_debug("Drop Key MSG");
            goto end_of_function;
        }

        uint8_t *key_delivery = pana_client_keywrap_parse(avp_temp.avp_ptr, avp_temp.len, suite);
        if (!key_delivery) {
            tr_debug("Drop Key MSG");
            goto end_of_function;
        }
        //tr_debug("Valid");
        suite->pana_session.nwk_key_id = key_delivery[16];
        suite->pana_session.auth_cnt = key_delivery[17];
        if (suite->state == PANA_PING_REQ) {
            tr_debug("Save Key to auth Info");

            if (suite->pana_session.auth_info) {
                tr_debug("Save Key to auth Info");
                auth_info_t *pana_auth_info_temp = suite->pana_session.auth_info;
                //Save keys to Auth Info structure
                memcpy(pana_auth_info_temp->network_key, key_delivery, 16);
                pana_auth_info_temp->key_id = key_delivery[16];
            }
        } else {
            //Calc keys
            tr_debug("Calc keys");
            uint8_t *key_ptr = pana_key_get(key_delivery);
            mac_helper_security_next_key_set(cur, (key_ptr + 16), key_delivery[16], MAC_KEY_ID_MODE_IDX);
            mle_service_security_set_security_key(cur->id, key_ptr, key_delivery[16], false);
        }
    }


    if (header->flags & PANA_FLAGS_REQUEST) {
        //tr_debug("Build Response");
        //SET Original packet First
        if ((header->flags & (PANA_FLAGS_PING | PANA_FLAGS_REQUEST)) == (PANA_FLAGS_PING | PANA_FLAGS_REQUEST)) {
            pana_client_session_nvm_udate(suite);
        }
        header->flags &= ~PANA_FLAGS_REQUEST;
        buf->buf_ptr = PANA_HEADER_LENGTH;
        ptr = buffer_data_pointer(buf);
        ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);
        buffer_data_end_set(buf, ptr);
        buf = build_pana_base(buf, header, suite);
        if (buf) {
            pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);
            //memcpy(buf->dst_sa.address, buf->src_sa.address, 16);
            //buf->src_sa.addr_type = ADDR_NONE;
            if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
                pana_set_agend_address(buf, true, suite);
            } else {
                pana_set_agend_address(buf, false, suite);
            }
            protocol_push(buf);
        }
        return;
    }
    //tr_debug("GET KEY");
    if (suite->state == PANA_PING_REQ) {
        //tr_debug("RE Valid Ready");
        tr_debug("pana nvm ok");
        sec_lib_state_machine_trig(suite, PANA_RE_VALID);
    } else if (suite->state == PANA_KEY_PULL) {
        mac_data_poll_protocol_poll_mode_disable(cur);
        sec_lib_state_machine_trig(suite, PANA_PULL_DONE);
    }
    pana_client_session_nvm_udate(suite);


end_of_function:
    buffer_free(buf);
}

static void sec_auth_ready(sec_suite_t *suite) {
    suite->timer = 0;
    tr_debug("Pana:OK");
    suite->pana_session.session_ready = true;
    if (suite->state == PANA_READY) {
        pana_client_session_nvm_udate(suite);
    }
    //Kick ICMP-State Machine to ND / RPL
    if (suite->state != PANA_PULL_DONE) {
        pana_authentication_ready(1, suite->interface);
    }

    //Reset pointer
    suite->pana_session.auth_info = NULL;
    pana_free_dynamic_ram(suite);
    sec_suite_tls_free(suite, true);

}

static void pana_pci_build(sec_suite_t *suite)
{

    buffer_t *buf = buffer_get(127);

    if (!buf) {
        tr_debug("Pana Init Fail");
        return;
    }
    tr_debug("BUILD PCI");
    pana_header_t header;
    header.type = PANA_MSG_PCI;
    header.flags = 0;
    header.seq = 0;
    header.session_id = 0;
    buf->interface = suite->interface;
    pana_set_agend_address(buf, false, suite);
    suite->session_port = UDP_PORT_PANA;
    buf = build_pana_base(buf, &header, suite);
    protocol_push(buf);
}

static void pana_client_pana_error_handler(sec_suite_t *suite)
{
    sec_lib_state_machine_lock(suite, PANA_ERROR);
    pana_authentication_ready(0, suite->interface);
    seclib_session_clean(suite);
}

static void pana_client_state_machine_func(sec_suite_t *suite)
{
    if (!suite) {
        return;
    }
    uint8_t general_tx = 0;


    switch (suite->state) {
        case PANA_ERROR:
            pana_client_pana_error_handler(suite);
            return;
        case PANA_PCI_TX:
            if (sec_auth_re_check(suite)) {
                pana_pci_build(suite);
                pana_timeout_timer_set(suite, suite->state);
            } else {
                pana_client_pana_error_handler(suite);
            }
            return;
        case PANA_KEY_PULL:
        case PANA_PING_REQ:
            if (sec_auth_re_check(suite)) {
                if (suite->state == PANA_PING_REQ) {
                    pana_ping_notify_msg_generate(0, suite);
                } else {
                    pana_ping_notify_msg_generate(1, suite);
                }

            } else {
                //Do Pana
                if (suite->state == PANA_PING_REQ) {
                    tr_error("pana nvm failed");
                }
                tr_debug("Reset Pana");
                suite->pana_session.session_ready = false;
                pana_session_init_by_session_ptr(suite, suite->pana_session.auth_info);
            }
            break;

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

        case TLS_CLIENT_TX_CERTIFICATE_VERIFY:
#ifdef ECC
            if (sec_auth_re_check(suite)) {
                if (tls_certificate_build(suite) == 0) {
                    tr_warn("Client Certi Verify Alloc fail");
                    suite->timer = 4;
                } else {
                    sec_set_auth_timeout(suite, TLS_CLIENT_TX_CERTIFICATE_VERIFY);
                }
            } else
#endif
            {
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
                    case PANA_START_RESPONSE:
                        pana_start_message_build(buf, suite);
                        break;

                    case EAP_IDENTITY_RES:
                        pana_eap_identity_build(buf, suite);
                        break;

                    case TLS_INIT:
                        buf = tls_client_hello_build(buf, suite);
                        if (buf) {
                            pana_eap_down(buf, suite);
                        }
                        sec_set_auth_timeout(suite, TLS_INIT);
                        break;

                    case TLS_KEY_CHANGE:
                        //Print Handshake message
                        tls_prepare_change_chipher_spec(suite);
                        tls_build_client_change_chipher_suite_finnish(buf, suite);
                        pana_eap_down(buf, suite);
                        break;

                    case PANA_FAILURE:
                        buf->buf_ptr = PANA_HEADER_LENGTH;
                        buf->buf_end = PANA_HEADER_LENGTH;
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
                case TLS_KEY_CHANGE:
                    sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                    break;
                case TLS_INIT: //Trig pana failure if not get any response from server
                    sec_lib_state_machine_trig(suite, PANA_FAILURE);
                    break;

                default:
                    pana_client_pana_error_handler(suite);
                    break;
            }
        }
    }
}

static void pana_client_packet_handler(buffer_t *buf)
{
    pana_header_t header;
    if (!pana_header_parse(buffer_data_pointer(buf), buffer_data_length(buf), &header)) {
        buffer_free(buf);
        return;
    }
    buffer_data_strip_header(buf, PANA_HEADER_LENGTH);

    sec_suite_t *suite =  sec_suite_selected_py_pan_id(buf->link_specific.ieee802_15_4.srcPanId);
    if (!suite) {
        buffer_free(buf);
        return;
    }

    header.agent_retry = false;
    //Handle Relay
    if (header.type == PANA_MSG_RELAY) {
        if (suite->pana_session.session_ready ) {
            buf = pana_relay_parse(buf);
        }
        if (buf) {
            protocol_interface_info_entry_t *cur_interface = buf->interface;
            addr_interface_get_ll_address(cur_interface, buf->src_sa.address, 0);
            buf->src_sa.addr_type = ADDR_IPV6;
            buf->src_sa.port = UDP_PORT_PANA;
            buf = buffer_turnaround(buf);
            buf->info = (buffer_info_t)(B_DIR_DOWN + B_FROM_APP + B_TO_UDP);
            protocol_push(buf);
            buf = NULL;
        }
        return;
    }

    bool my_session = false;
    if (header.type != PANA_MSG_PCI) {
        if ((header.flags & PANA_FLAG_START_REQ_MASK)  == (PANA_FLAG_START_REQ_MASK)) {
            if (memcmp(suite->session_address, buf->src_sa.address, 16) == 0) {
                my_session = true;
            }
        } else {
            //Check Session
            if (suite->pana_session.session_id == header.session_id) {
                //Validate Session
                if ((memcmp(suite->session_address, buf->src_sa.address, 16) == 0)) {
                    my_session = true;
                } else if (memcmp(suite->pana_session.session_relay_address, buf->src_sa.address, 16) == 0) {
                    my_session = true;
                }

            }

            if (!my_session) {
                if (buf->src_sa.address[0] != 0xfe) {
                    tr_debug("Not relay src");
                    buffer_free(buf);
                    return;
                }
            }
        }
    }

    if (my_session) {
        if ((header.flags & PANA_FLAG_START_REQ_MASK)  == (PANA_FLAG_START_REQ_MASK)) {
            tr_debug("Take session & Sequency");
            suite->pana_session.session_id = header.session_id;
            suite->pana_session.res_seq = header.seq;
            pana_session_startms_parse(buf, &header, suite);
            return;
        }

        if (suite->pana_session.pana_heap) {
            pana_heap_t *pheap = suite->pana_session.pana_heap;
            if (pheap->handshake_len == 0 || pheap->handshake_req_offset == pheap->handshake_len) {
                tr_debug("Pana REQ dropped because Handshake is not full started");
                buffer_free(buf);
                return;
            }
        }

        if ((header.flags & PANA_FLAGS_REQUEST) == PANA_FLAGS_RESPONSE) {
            if (suite->pana_session.req_seq == header.seq) {
                suite->pana_session.req_seq++;
            } else {
                tr_debug("Pana RES:Drop Packet by seq num. Res: %02"PRIu32", RX: %02"PRIu32, suite->pana_session.req_seq >> 8, header.seq);
                buffer_free(buf);
                return;
            }
        } else {
            if ((suite->pana_session.res_seq + 1) == header.seq) {
                suite->pana_session.res_seq = header.seq;

            } else {
                if (suite->pana_session.res_seq != header.seq) {
                    tr_debug("PANA REQ:Drop Packet by seq num. Res: %02"PRIu32" , RX: %02"PRIu32, suite->pana_session.res_seq >> 8, header.seq);
                    buffer_free(buf);
                    return;
                } else {
                    header.agent_retry = true;
                }
            }
        }
        if (header.flags & PANA_FLAGS_COMPLETE) {
            pana_complete_msg_parse(buf, &header, suite);

        } else {

            if (header.type == PANA_MSG_PNA) {
                pana_client_pna_handler(buf, &header, suite);
                return;
            }

            if (header.type != PANA_MSG_PA) {
                return;
            }

            buf = pana_auth_message_handler(buf, &header, suite);

            if (buf) {
                pana_eap_tls_up(buf, suite);
            }
        }
        return;

    }

    //Relay

    if ((suite->pana_session.session_ready) && pana_check_address(buf)) {

        if (header.type == PANA_MSG_PNA || header.type == PANA_MSG_PCI) {
            //Remove old data
            if (lowpan_neighbour_data_clean(suite->interface->id, buf->src_sa.address) ) {
                uint8_t ll_adr[16];
                memcpy(ll_adr, buf->src_sa.address, 16);
                buffer_free(buf);
                tr_debug("Parent rejoin --> send unsecured Link Reject");
                mle_service_reject_message_build(suite->interface->id, ll_adr, true);
                return;
            }
        }

        buffer_data_reserve_header(buf, 16);
        buf = pana_relay_avp_build(buf, suite);
        if (buf) {
            //Set Pana Headers Like Draft say
            tr_debug("Pana Relay");
            header.flags = 0;
            header.type = PANA_MSG_RELAY;
            header.session_id = 0;
            header.seq = 0;
            pana_set_agend_address(buf, true, suite);
            buf = build_pana_base(buf, &header, suite);
            protocol_push(buf);
            buf = NULL;
        } else {
            tr_debug("Relay AVP Build Fail");
        }
    }


    if (buf) {
        buffer_free(buf);
    }
    return;
}

sec_suite_t *pana_client_init(auth_info_t *auth_ptr, uint8_t *session_address_ptr, pana_tls_setup_s *setup)
{
    sec_suite_t *suite = sec_suite_selected_py_pan_id(setup->pan_id);

    if (!suite) {

       bool loaded_setup = false;
        if (pana_client_nvm_storage_cb) {
            loaded_setup = pana_client_session_get(setup->pan_id);
            if (loaded_setup) {
                tr_debug("load pana nvm for PAN ID %2.2x", setup->pan_id);
                suite = sec_lib_security_session_allocate(false);
            } else {
                tr_debug("nvm pana load fail for PAN ID %2.2x", setup->pan_id);
                suite = sec_lib_security_session_allocate(true);
            }
        } else {
            suite = sec_lib_security_session_allocate(true);
        }
        if (!suite) {
            return NULL;
        }
        if (loaded_setup) {
            pana_session_data_load_to_session(pana_client_nvm_buffer + 16, suite);
        }
        suite->pan_id = setup->pan_id;

        tr_debug("Create Entry");
        memcpy(suite->session_address, session_address_ptr, 16);
        tr_debug("Session adr: %s", tr_ipv6(suite->session_address));
    }

    if (suite) {
        pana_session_init_by_session_ptr(suite, auth_ptr);
        suite->supported_chipher_suites =  setup->security_support;
        suite->psk_key_id = setup->psk_key_id;
    }
    return suite;
}

void pana_reset_client_session(void)
{
    sec_suite_list_clean();
}

int8_t pana_client_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id)
{
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    if (!cur->if_lowpan_security_params) {
        return -1;
    } else if (cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        return -4;
    } else if (cur->if_lowpan_security_params->pana_params == 0) {
        return -3;
    } else if (cur->if_lowpan_security_params->nwk_security_mode != NET_SEC_MODE_PANA_LINK_SECURITY) {
        return -5;
    }

    if (!pana_socket_init(pana_client_packet_handler, pana_client_state_machine_func, tls_client_up)) {
        return -1;
    }

    switch (cipher_mode) {

        case NET_TLS_PSK_CIPHER: /**< Network Authentication support only PSK */
            //Verify PSK KEY ID
            if (arm_tls_check_key(psk_key_id) != 0) {
                return -7;
            }
            break;

        case NET_TLS_PSK_AND_ECC_CIPHER: /**< Network Authentication support PSK & ECC */
            //Verify PSK KEY ID
            if (arm_tls_check_key(psk_key_id) != 0) {
                return -7;
            }
            /* fall through */
        case NET_TLS_ECC_CIPHER:        /**< Network Authentication support only ECC */

#ifdef ECC
            //Verify Certficate
            if(sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN) == NULL) {
                return -6;
            }
#endif
            break;
    }

    cur->if_lowpan_security_params->pana_params->nwk_chipher_mode = cipher_mode;
    cur->if_lowpan_security_params->pana_params->psk_key_id = psk_key_id;
    cur->if_lowpan_security_params->pana_params->pana_client = 1;
    cur->lowpan_info |= (INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION);
    cur->configure_flags |= INTERFACE_SECURITY_DEFINED;

    return 0;
}

nwk_pana_params_s * pana_client_parameter_allocate(void)
{
    nwk_pana_params_s *pana_params = ns_dyn_mem_alloc((sizeof(nwk_pana_params_s)));
    if (pana_params) {

#ifdef ECC
        pana_params->nwk_chipher_mode = NET_TLS_ECC_CIPHER;
#else
        pana_params->nwk_chipher_mode = NET_TLS_PSK_CIPHER;
#endif
        pana_params->client_session_mode = NET_PANA_SINGLE_SESSION;
        pana_params->psk_key_id = 0;
        pana_params->pana_client = 1;
    }
    return pana_params;
}

int8_t pana_client_key_pull(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur || !(cur->lowpan_info & INTERFACE_NWK_ACTIVE)) {
        return -1;
    }

    sec_suite_t *suite =  sec_suite_selected_py_pan_id(cur->mac_parameters->pan_id);
    if (!suite || suite->pana_session.user_server || !suite->pana_session.session_ready || suite->state == PANA_KEY_PULL) {
        return -1;
    }

    mac_data_poll_enable_check(cur);
    sec_lib_state_machine_trig(suite, PANA_KEY_PULL);
    return 0;
}

uint8_t pana_ping_notify_msg_tx(uint16_t pan_id)
{
    sec_suite_t *suite =  sec_suite_selected_py_pan_id(pan_id);
    if (!suite) {
        return 0;
    }

    sec_lib_state_machine_trig(suite, PANA_KEY_PULL);
    return 1;
}

static uint8_t pana_ping_notify_msg_generate(uint8_t key_req, sec_suite_t *suite)
{

    if (suite->pana_session.user_server) {
        return 0;
    }
    protocol_interface_info_entry_t *cur = suite->interface;
    if (!cur) {
        return 0;
    }
    buffer_t *buf = buffer_get(127);

    if (!buf) {
        return 0;
    }
    uint8_t *ptr;
    pana_header_t header;
    header.flags = PANA_FLAGS_REQUEST | PANA_FLAGS_PING;
    header.type = PANA_MSG_PNA;
    header.seq = suite->pana_session.req_seq;
    header.session_id = suite->pana_session.session_id;
    buf->buf_ptr = PANA_HEADER_LENGTH;
    ptr = buffer_data_pointer(buf);
    if (key_req) {
        //Add Key Request
        tr_debug("Key Request");
        ptr = pana_avp_zip_key_req(ptr, cur);
    }

    ptr = pana_avp_write_n_bytes(AVP_AUTHENCY_CODE, 16, NULL, ptr);

    buffer_data_end_set(buf, ptr);
    buf = build_pana_base(buf, &header, suite);
    if (!buf) {
        return 0;
    }

    //Calc
    //tr_debug("Calc");
    pana_auth_hash_calc(buffer_data_pointer(buf), buffer_data_length(buf), suite->pana_session.pana_auth_key);

    if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        if (suite->state == PANA_PING_REQ) {
            pana_set_agend_address(buf, false, suite);
        } else {
            if ((cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY)) {
                pana_set_agend_address(buf, true, suite);
            } else {
                pana_set_agend_address(buf, false, suite);
            }
        }
    } else {
        pana_set_agend_address(buf, false, suite);
    }


    //                  tr_debug("Hash Cal: %s", trace_array(ptr,16));
    if (key_req) {
        pana_timeout_timer_set(suite, PANA_KEY_PULL);
        tr_debug("Pull Key by Req");
    } else {
        tr_debug("TX Ping notify");
        pana_timeout_timer_set(suite, PANA_PING_REQ);
    }
    protocol_push(buf);
    return 1;

}

int8_t pana_client_nvm_callback_set(pana_client_session_update_cb *nvm_update, pana_client_session_get_cb *nvm_get, uint8_t *nvm_static_buffer)
{
    if (!nvm_update || !nvm_static_buffer || !nvm_get) {
        return -1;
    }
    pana_client_nvm_storage_cb = nvm_update;
    pana_client_session_get = nvm_get;
    pana_client_nvm_buffer = nvm_static_buffer;
    return 0;

}

static void pana_client_session_nvm_udate(sec_suite_t *suite)
{
    if (pana_client_nvm_storage_cb) {
        uint8_t *data_buf = pana_client_nvm_buffer;
        memcpy(data_buf, suite->session_address, 16);
        data_buf += 16;
        *data_buf++ = suite->pana_session.auth_cnt;
        *data_buf++ = suite->pana_session.nwk_key_id;
        memcpy(data_buf, suite->pana_session.pana_auth_key, 32);
        data_buf += 32;
        memcpy(data_buf, suite->pana_session.pana_PAA_enc_key, 16);
        data_buf += 16;
        data_buf = common_write_32_bit(suite->pana_session.pana_key_id, data_buf);
        data_buf = common_write_32_bit(suite->pana_session.session_id, data_buf);
        data_buf = common_write_32_bit(suite->pana_session.req_seq, data_buf);
        data_buf = common_write_32_bit(suite->pana_session.res_seq, data_buf);
        data_buf = common_write_32_bit(suite->pana_session.session_lifetime, data_buf);
        pana_client_nvm_storage_cb(suite->pan_id, PANA_CLIENT_SESSION_UPDATE);
    }
}

static void pana_session_data_load_to_session(uint8_t *data_buf, sec_suite_t *suite)
{
    suite->timer = 0;
    suite->supported_chipher_suites = SEC_DEFAULT_SUPPORTED_CHIPHER_SUITES;
    suite->setups = 0;

    pana_session_state_init(&suite->pana_session);

    //Check Is pana Raedy
    suite->pana_session.session_ready = true;
    //Start Copy
    //tr_debug("Set Status");
    suite->pana_session.auth_cnt = *data_buf++;
    suite->pana_session.nwk_key_id = *data_buf++;
    memcpy(suite->pana_session.pana_auth_key, data_buf, 32);
    data_buf += 32;
    memcpy(suite->pana_session.pana_PAA_enc_key, data_buf, 16);
    data_buf += 16;

    suite->pana_session.pana_key_id = common_read_32_bit(data_buf);
    data_buf += 4;

    suite->pana_session.session_id = common_read_32_bit(data_buf);
    data_buf += 4;

    suite->pana_session.req_seq = common_read_32_bit(data_buf);
    data_buf += 4;
    suite->pana_session.res_seq = common_read_32_bit(data_buf);
    data_buf += 4;
    suite->pana_session.session_lifetime = common_read_32_bit(data_buf);
}


#else
int8_t pana_client_nvm_callback_set(pana_client_session_update_cb *nvm_update, pana_client_session_get_cb *nvm_get, uint8_t *nvm_static_buffer)
{
    (void)nvm_update;
    (void)nvm_get;
    (void)nvm_static_buffer;
    return -1;
}

#endif

