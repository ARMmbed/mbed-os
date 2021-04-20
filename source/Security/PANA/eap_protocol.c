/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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
#include "Core/include/ns_socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"

#include "common_functions.h"
#include "Security/PANA/pana_eap_header.h"
#ifdef PANA

#define TRACE_GROUP "eap"

const uint8_t EAP_ANYMOUS[9] = {'a', 'n', 'o', 'n', 'y', 'm', 'o', 'u', 's'};

static bool force_frag_last_retry = false;

static bool force_frag_start_fail = false;

static bool force_frag_timeout = false;

static void eap_seq_back_to_accept(sec_suite_t *suite)
{
    if (suite->pana_session.eap_id_seq == 0) {
        suite->pana_session.eap_id_seq = 0xff;
    } else {
        suite->pana_session.eap_id_seq--;
    }
}

void pana_eap_fragmetation_start_filter(bool state)
{
    tr_debug("Set start state %u", state);
    force_frag_start_fail = state;
}

void pana_eap_fragmetation_force_timeout(bool state)
{
    force_frag_timeout = state;
}

void pana_eap_fragmetation_force_retry(bool state)
{
    force_frag_last_retry = state;
}

static buffer_t *eap_common_headroom_get_to_buffer(buffer_t *buf, uint16_t header_size)
{
    if ((buf = buffer_headroom(buf, header_size)) == 0) {
        return NULL;
    }
    buffer_data_reserve_header(buf, header_size);
    return buf;
}

void pana_eap_identity_build(buffer_t *buf, sec_suite_t *suite)
{
    uint8_t *ptr;
    uint8_t eap_code;
    buf->buf_ptr = 0;
    const uint8_t *nonce_ptr;
    if (suite->pana_session.user_server) {
        buf->buf_end = 0;
        eap_code = EAP_REQ;
        nonce_ptr = suite->pana_session.pana_heap->agent_nonce;
    } else {

        buf->buf_end = 9;
        ptr = buf->buf;
        memcpy(ptr, EAP_ANYMOUS, 9);
        eap_code = EAP_RESPONSE;
        nonce_ptr = suite->pana_session.pana_heap->client_nonce;
    }

    buf = eap_common_headroom_get_to_buffer(buf, eap_header_size(eap_code));
    if (!buf) {
        return;
    }

    eap_header_build(buffer_data_pointer(buf), buffer_data_length(buf), eap_code, suite->pana_session.eap_id_seq, EAP_IDENTITY);

    pana_eap_payload_down(buf, nonce_ptr, suite);
    tr_warn("TX EAP Identity!");
}

void pana_eap_tls_start_build(buffer_t *buf, sec_suite_t *suite)
{
    buf->buf_ptr = 0;
    buf->buf_end = 0;
    if (suite->tls_session->tls_heap) {
        tls_heap_t *tls_heap = suite->tls_session->tls_heap;
        tls_heap->tls_handshake_h_len = 0;
    }

    uint8_t header_size = eap_header_size(EAP_REQ);
    header_size += eap_tls_header_size(EAP_TLS_START);

    buf = eap_common_headroom_get_to_buffer(buf, header_size);
    if (!buf) {
        return;
    }

    uint8_t *ptr = eap_header_build(buffer_data_pointer(buf), buffer_data_length(buf), EAP_REQ, suite->pana_session.eap_id_seq, EAP_TLS);

    eap_tls_header_build(ptr, EAP_TLS_START, 0);

    pana_eap_payload_down(buf, NULL, suite);

}


void pana_eap_tls_finnish_build(buffer_t *buf, sec_suite_t *suite)
{
    buffer_data_clear(buf);
    uint8_t eap_code;
    if (suite->state == TLS_ALERT) {
        eap_code = EAP_FAILURE;
    } else {
        eap_code = EAP_RESPONSE;
    }


    uint8_t header_size = eap_header_size(eap_code);
    header_size += eap_tls_header_size(0);

    buf = eap_common_headroom_get_to_buffer(buf, header_size);
    if (!buf) {
        return;
    }

    uint8_t *ptr = eap_header_build(buffer_data_pointer(buf), buffer_data_length(buf), eap_code, suite->pana_session.eap_id_seq, EAP_TLS);

    eap_tls_header_build(ptr, 0, 0);

    pana_eap_payload_down(buf, NULL, suite);
}

bool pana_eap_frag_re_tx(sec_suite_t *suite)
{
    buffer_t *f_buf;
    if (suite->pana_session.eap_assy_buf) {
        buffer_t *buf = 0;
        f_buf = buffer_get(suite->pana_session.last_assy_size + 50);
        buf = suite->pana_session.eap_assy_buf;
        if (f_buf) {
            memcpy(buffer_data_pointer(f_buf), buffer_data_pointer(buf) + suite->pana_session.assy_off_set, suite->pana_session.last_assy_size);
            buffer_data_length_set(f_buf, suite->pana_session.last_assy_size);
            goto success_push;
        }
    } else if (suite->pana_session.eap_frag_buf || suite->pana_session.packet_delivered) {
        f_buf = buffer_get(127);
        if (f_buf) {

            //tr_debug("EAP assembly re REQ timer");
            f_buf->buf_ptr = f_buf->buf_end;
            goto success_push;
        }
    }
    return false;
success_push:
    f_buf->interface = suite->interface;
    memcpy(f_buf->dst_sa.address, suite->session_address, 16);
    f_buf->src_sa.addr_type = ADDR_NONE;
    pana_eap_down(f_buf, suite);
    return true;
}

buffer_t *eap_down(buffer_t *buf, sec_suite_t *suite)
{
    //tr_debug("EAP Down");
    uint16_t tls_payload_len = 0;
    uint8_t eap_code;
    uint8_t eap_tls_flags = 0;
    tls_payload_len = buffer_data_length(buf);
    if (tls_payload_len >  EAP_MTU_SIZE) {
        //Check Fragmentation
        if (suite->pana_session.assy_length) {
            tr_debug("Already Fragmentation");
            buffer_free(buf);
            buf = (buffer_t *)0;
            return buf;
        } else {
            pana_lib_parameters_s *parameters = pana_parameters_get();

            buffer_t *f_buf = buffer_get(parameters->EAP_FRAGMENT_SIZE + 50);
            buf->seq = suite->pana_session.eap_id_seq;
            suite->pana_session.eap_assy_buf  = buf;
            suite->pana_session.assy_length = tls_payload_len;
            suite->pana_session.assy_off_set = 0;
            suite->pana_session.last_assy_size = parameters->EAP_FRAGMENT_SIZE;
            if (suite->pana_session.user_server) {
                //tr_debug("Set Re Tx Timer");
                pana_timeout_timer_set(suite, suite->state);
            } else {
                sec_set_auth_timeout(suite, TLS_KEY_CHANGE);
            }
            if (f_buf) {
                f_buf->interface = suite->interface;
                eap_tls_flags = EAP_TLS_FRAGMENT_LENGTH | EAP_TLS_MORE_FRAGMENTS;
                memcpy(buffer_data_pointer(f_buf), buffer_data_pointer(buf), parameters->EAP_FRAGMENT_SIZE);
                memcpy(f_buf->dst_sa.address, buf->src_sa.address, 16);
                buf->src_sa.addr_type = ADDR_NONE;
                f_buf->src_sa.addr_type = ADDR_NONE;
                buffer_data_length_set(f_buf, parameters->EAP_FRAGMENT_SIZE);
                buf = f_buf;

            } else {
                return NULL;
            }
        }
    } else {
        if (suite->pana_session.assy_length) {
            if (suite->pana_session.assy_length > (suite->pana_session.assy_off_set + suite->pana_session.last_assy_size)) {
                eap_tls_flags = EAP_TLS_MORE_FRAGMENTS;
                if (suite->pana_session.assy_off_set == 0) {
                    eap_tls_flags |= EAP_TLS_FRAGMENT_LENGTH;
                    //tr_debug("Retry First");
                }
            }
        }
    }

    if (suite->pana_session.user_server) {
        eap_code = EAP_REQ;
    } else {
        eap_code = EAP_RESPONSE;
    }

    uint8_t header_size = eap_header_size(eap_code);
    header_size += eap_tls_header_size(eap_tls_flags);

    buf = eap_common_headroom_get_to_buffer(buf, header_size);
    if (!buf) {
        return NULL;
    }

    uint8_t *ptr = eap_header_build(buffer_data_pointer(buf), buffer_data_length(buf), eap_code, suite->pana_session.eap_id_seq, EAP_TLS);

    eap_tls_header_build(ptr, eap_tls_flags, suite->pana_session.assy_length);

    return (buf);
}

buffer_t *eap_up(buffer_t *buf, sec_suite_t *suite)
{
    eap_header_t header;
    uint8_t *ptr = buffer_data_pointer(buf);
    uint16_t payload_length = buffer_data_length(buf);
    uint8_t response_counter = suite->retry_counter;
    if (!eap_header_parse(ptr, payload_length, &header)) {
        return buffer_free(buf);
    }

    if (header.eap_code == EAP_RESPONSE) {
        if (header.id_seq != suite->pana_session.eap_id_seq) {
            tr_warn("EAP:Drop Packet by ID");
            suite->timer = 65;
            return buffer_free(buf);
        }
        //tr_debug("EAP RES");
        suite->pana_session.eap_id_seq++;
        suite->retry_counter = 0;
    } else if (header.eap_code == EAP_REQ) {

        if (header.type == EAP_IDENTITY) {
            suite->pana_session.eap_id_seq = header.id_seq;
        } else {

            uint8_t eap_seq = suite->pana_session.eap_id_seq;
            if (eap_seq == 0xff) {
                eap_seq = 0;
            } else {
                eap_seq++;
            }

            if (eap_seq != header.id_seq) {
                if (suite->pana_session.eap_id_seq == header.id_seq) {
                    //tr_debug("EAP:Same SEQ");
                    if (suite->pana_session.assy_length) {
                        tr_debug("SEND ACK. Should send same packet again");
                    } else if (suite->pana_session.frag_length) {
                        tr_debug("SEND ACK. Should send same packet again");
                        buf->buf_ptr = buf->buf_end;
                        buf = buffer_turnaround(buf);
                        memcpy(buf->dst_sa.address, buf->src_sa.address, 16);
                        buf->src_sa.addr_type = ADDR_NONE;
                        return buf;
                    } else {
                        tr_debug("REQ but Frag ready");
                        return buffer_free(buf);
                    }
                } else {
                    tr_debug("EAP:Drop unknown Req ID. MSG: %x, ses: %x", header.id_seq, suite->pana_session.eap_id_seq);
                    return buffer_free(buf);
                }
            } else {
                suite->pana_session.eap_id_seq = header.id_seq;
            }
        }
    }

    switch (header.type) {
        case EAP_IDENTITY:
            if (header.eap_code == EAP_REQ) {
                sec_lib_state_machine_trig(suite, EAP_IDENTITY_RES);
            } else {
#ifdef PANA_SERVER_API
                if (suite->pana_session.user_server) {
                    sec_lib_state_machine_trig(suite, TLS_START);
                }
#endif
            }
            return buffer_free(buf);

        case EAP_TLS:
        case EAP_TTLS: {
            eap_tls_header_t eap_tls_header;
            if (!eap_tls_header_parse(header.data_ptr, header.length - 5, &eap_tls_header)) {
                return buffer_free(buf);
            }

            if (eap_tls_header.eap_tls_flags & EAP_TLS_START) {
                suite->pana_session.eap_id_seq = header.id_seq;
                sec_lib_state_machine_trig(suite, TLS_INIT);
                return buffer_free(buf);
            }
            buffer_data_pointer_set(buf, eap_tls_header.data_ptr);

            if (header.eap_code == EAP_RESPONSE) {
                if (eap_tls_header.eap_tls_flags) {
                    if (suite->pana_session.assy_length) {
                        suite->pana_session.assy_off_set += suite->pana_session.last_assy_size;
                        if (suite->pana_session.assy_off_set < suite->pana_session.assy_length) {
                            tr_warn("Packet TX process fail");
                        } else {
                            tr_debug("EAP Frag TX Done & Start RX frag");

                        }
                        //tr_debug("Free Frag Buf");
                        if (suite->pana_session.eap_assy_buf) {
                            tr_debug("Free Frag Buf");
                            buffer_free(suite->pana_session.eap_assy_buf);
                            suite->pana_session.eap_assy_buf  = NULL;
                        }
                        suite->pana_session.assy_length = 0;
                        suite->pana_session.assy_off_set = 0;
                        suite->pana_session.last_assy_size = 0;
                        suite->pana_session.packet_delivered = true;
                        suite->retry_counter = 0;
                    }
                }
            }

            if ((eap_tls_header.eap_tls_flags & EAP_TLS_MORE_FRAGMENTS) == 0) {
                if (suite->pana_session.frag_length) {
                    if (force_frag_last_retry || force_frag_timeout) {
                        force_frag_last_retry = false;
                        if (header.eap_code == EAP_RESPONSE) {
                            suite->retry_counter = response_counter;
                        }
                        eap_seq_back_to_accept(suite);
                        return buffer_free(buf);
                    }
                    buffer_t *t_buf = suite->pana_session.eap_frag_buf;

                    uint16_t check_len = suite->pana_session.frag_off_set;
                    check_len += eap_tls_header.tls_frame_length;
                    suite->pana_session.eap_frag_buf = NULL;
                    suite->pana_session.frag_length = 0;
                    if (check_len == buffer_data_length(t_buf)) {
                        memcpy(buffer_data_pointer(t_buf) + suite->pana_session.frag_off_set, eap_tls_header.data_ptr, eap_tls_header.tls_frame_length);
                        t_buf->seq = header.id_seq;
                        tr_debug("Full packet RX");
                        buffer_free(buf);
                        buf = t_buf;
                        suite->pana_session.frag_off_set = 0;
                    } else {

                        buffer_free(t_buf);
                        suite->pana_session.frag_off_set = 0;
                        sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                        return buffer_free(buf);
                    }
                } else if (suite->pana_session.assy_length) {
                    buffer_t *t_buf = suite->pana_session.eap_assy_buf;
                    if (header.eap_code == EAP_RESPONSE) {
                        suite->pana_session.assy_off_set += suite->pana_session.last_assy_size;
                        if (eap_tls_header.tls_frame_length == 0) {
                            t_buf->seq = header.id_seq;
                        }
                    } else {
                        uint8_t cmp_seq = t_buf->seq;
                        if (cmp_seq == 0xff) {
                            cmp_seq = 0;
                        } else {
                            cmp_seq++;
                        }
                        if (cmp_seq == header.id_seq) {
                            suite->pana_session.assy_off_set += suite->pana_session.last_assy_size;
                            if (eap_tls_header.tls_frame_length == 0) {
                                t_buf->seq = header.id_seq;
                            }
                        } else {
                            tr_debug("RETX");
                        }
                    }
                    if (suite->pana_session.assy_off_set < suite->pana_session.assy_length) {
                        if (eap_tls_header.tls_frame_length == 0) {
                            uint16_t len = 0;
                            buffer_t *f_buf = 0;
                            buffer_free(buf);
                            suite->retry_counter = 0;
                            pana_lib_parameters_s *parameters = pana_parameters_get();
                            if (suite->pana_session.assy_off_set + parameters->EAP_FRAGMENT_SIZE < suite->pana_session.assy_length) {
                                len = parameters->EAP_FRAGMENT_SIZE;
                            } else {
                                len = suite->pana_session.assy_length - suite->pana_session.assy_off_set;
                            }

                            f_buf = buffer_get(len + 50);
                            if (f_buf) {
                                //memcpy(f_buf->dst_sa.address, t_buf->src_sa.address, 16);
                                f_buf->interface = suite->interface;
                                f_buf->src_sa.addr_type = ADDR_NONE;
                                f_buf->dst_sa.addr_type = ADDR_NONE;
                                memcpy(buffer_data_pointer(f_buf), buffer_data_pointer(t_buf) + suite->pana_session.assy_off_set, len);
                                buffer_data_length_set(f_buf, len);
                                suite->pana_session.last_assy_size = len;
                                tr_debug("Push frag");
                                return (f_buf);
                            } else {
                                tr_warn("No Mem");
                                return NULL;
                            }
                        } else {
                            //IT cuold be alert
                            buf->session_ptr = suite;
                            eap_tls_payload_push(buf);
                            return NULL;
                        }
                    } else {
                        tr_debug("EAP Frag TX Done");
                        suite->pana_session.eap_assy_buf  = buffer_free(t_buf);
                        suite->pana_session.assy_length = 0;
                        suite->pana_session.assy_off_set = 0;
                        suite->pana_session.last_assy_size = 0;
                    }
                } else if (!eap_tls_header.tls_frame_length) {
                    if (header.eap_code == EAP_RESPONSE) {
#ifdef PANA_SERVER_API
                        if (suite->state == TLS_KEY_CHANGE && suite->pana_session.user_server) {
                            //tr_debug("TLS Auth Ready");
                            pana_key_calculation(suite);
                            sec_lib_state_machine_trig(suite, TLS_EAP_END_PANA_VERIFY);
                        }
#endif
                    }

                    return buffer_free(buf);
                }
                buf->session_ptr = suite;
                eap_tls_payload_push(buf);
                return NULL;

            }

            //More flag is active
            if (!eap_tls_header.tls_frame_length) {
                tr_debug("More without Data");
                return buffer_free(buf);
            }
            bool skip_packet = false;

            if (eap_tls_header.tls_length) {
                //Check did we have a already action
                if (suite->pana_session.frag_length == 0) {

                    buffer_t *f_buf = NULL;
                    if (force_frag_start_fail) {
                        tr_debug("Force to drop fragment");
                        force_frag_start_fail = false;
                    } else {
                        tr_debug("First Fragment");
                        f_buf = buffer_get(eap_tls_header.tls_length);
                    }
                    if (f_buf) {
                        buffer_data_length_set(f_buf, eap_tls_header.tls_length);
                        memcpy(buffer_data_pointer(f_buf), eap_tls_header.data_ptr, eap_tls_header.tls_frame_length);
                        suite->pana_session.frag_off_set = eap_tls_header.tls_frame_length;
                        f_buf->seq = header.id_seq;
                        suite->pana_session.eap_frag_buf = f_buf;
                        suite->pana_session.frag_length = eap_tls_header.tls_length;
                        skip_packet = true;
                    } else {
                        tr_debug("No free men for Fragment: %i", eap_tls_header.tls_length);
                        sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                        return buffer_free(buf);
                    }
                } else if (suite->pana_session.frag_length != eap_tls_header.tls_length) {
                    tr_debug("Fragment started already");
                    skip_packet = true;
                } else {

                }
            }

            if (!skip_packet && suite->pana_session.frag_length) {
                uint16_t check_len = 0;
                buffer_t *t_buf = suite->pana_session.eap_frag_buf;
                check_len = suite->pana_session.frag_off_set;
                check_len += eap_tls_header.tls_frame_length;

                //new sequency
                if (check_len < buffer_data_length(t_buf)) {
                    tr_debug("Copy Data to Fragment");
                    memcpy(buffer_data_pointer(t_buf) + suite->pana_session.frag_off_set, eap_tls_header.data_ptr, eap_tls_header.tls_frame_length);
                    suite->pana_session.frag_off_set += eap_tls_header.tls_frame_length;
                    t_buf->seq = suite->pana_session.eap_id_seq;
                } else {
                    tr_debug("Overflow possible Free Current entry and set Alert");
                    buffer_free(t_buf);
                    suite->pana_session.eap_frag_buf = NULL;
                    suite->pana_session.frag_length = 0;
                    suite->pana_session.frag_off_set = 0;
                    sec_lib_state_machine_trig(suite, TLS_ALERT_INTERNAL);
                    return buffer_free(buf);
                }
            }

            if (header.eap_code == EAP_RESPONSE) {
#ifdef PANA_SERVER_API
                if (suite->state == TLS_TX_SERVER_KEY_EXCHANGE) {
                    if (suite->pana_session.assy_length) {
                        suite->retry_counter = 0;
                    }
                }
#endif
                pana_timeout_timer_set(suite, suite->state);
            }
            /* TX ACK or new request to sender*/
            buf = buffer_turnaround(buf);
            buf->buf_ptr = buf->buf_end;
            memcpy(buf->dst_sa.address, buf->src_sa.address, 16);
            buf->src_sa.addr_type = ADDR_NONE;
            return (buf);
        }


        default:
            tr_debug("%x", header.type);
            return buffer_free(buf);
    }
}
#else
void pana_eap_fragmetation_start_filter(bool state)
{
    (void) state;
}

void pana_eap_fragmetation_force_timeout(bool state)
{
    (void) state;
}

void pana_eap_fragmetation_force_retry(bool state)
{
    (void) state;
}

#endif
