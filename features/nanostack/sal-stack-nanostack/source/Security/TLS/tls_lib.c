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
#ifdef PANA
#include "string.h"
#include "Core/include/ns_address_internal.h"
#include "Core/include/ns_buffer.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "randLIB.h"
#include "Core/include/ns_socket.h"
//#include "6LoWPAN/Bootstraps/network_lib.h"
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/TLS/tls_ccm_crypt.h"
#include "Security/Common/sec_lib.h"
#include "nsdynmemLIB.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "net_interface.h"

#define TRACE_GROUP "TLSl"

typedef struct tls_psk_key_ {
    uint16_t key_id;
    uint8_t psk_key[16];
    ns_list_link_t link;
} tls_psk_key_t;

static NS_LIST_DEFINE(tls_psk_list, tls_psk_key_t, link);

NS_LARGE tls_header_t tls_header;
NS_LARGE tls_msg_t tls_msg;

#ifdef ECC

tls_ecc_heap_t *ecc_allocate_ram(void);
static uint8_t tls_parse_certificate(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite);
static uint8_t tls_parse_certificate_verify(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite);
static uint8_t ecc_verify_calculate_hash(sec_suite_t *tls_suite);
void tls_ecc_reverse_hash(uint8_t *ptr);
uint8_t tls_write_signature_parameters(uint8_t *ptr, uint8_t *signature_parameter, uint8_t leadin_zeros);
uint8_t tls_parse_certificate_request(uint8_t *ptr, uint16_t len);
static uint8_t tls_parse_client_key_exchange(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite);
static uint8_t tls_parse_server_key_exchange(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite);

#endif

static uint8_t *tls_set_client_key_excange(uint8_t *ptr, sec_suite_t *tls_suite);
static uint8_t tls_parse_server_hello(uint8_t *ptr, sec_suite_t *tls_suite);
static uint8_t tls_parse_client_hello(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite);
static tls_psk_key_t *tls_get_key(uint16_t key_id);

tls_session_t *amr_tls_session_allocate(void)
{
    tls_session_t *t_session = 0;
    t_session = ns_dyn_mem_alloc(sizeof(tls_session_t));
    if (t_session) {
        memset(t_session, 0, sizeof(tls_session_t));
    }
    return t_session;
}

void arm_tls_session_clear(tls_session_t *t_session)
{
    if (t_session->tls_heap) {
        tls_heap_free(t_session->tls_heap);
        t_session->tls_heap = NULL;
    }
}

static tls_heap_t *tls_heap_structure_allocate(void)
{
    tls_heap_t *heap_ptr = (tls_heap_t *) ns_dyn_mem_temporary_alloc(sizeof(tls_heap_t));
    if (heap_ptr) {
        memset(heap_ptr, 0, sizeof(tls_heap_t));
    }
    return heap_ptr;
}

int8_t arm_tls_add_psk_key(const uint8_t *key_ptr, uint16_t key_id)
{
    tls_psk_key_t *key_entry = tls_get_key(key_id);

    if (key_ptr == NULL) {
        return -1;
    }

    /* If key with given ID already exists, remove old */
    if (key_entry) {
        arm_tls_remove_psk_key(key_id);
        key_entry = NULL;
    }

    /* Make new entry */
    key_entry = ns_dyn_mem_alloc(sizeof(tls_psk_key_t));
    if (key_entry == NULL) {
        return -1;
    }

    memcpy(key_entry->psk_key, key_ptr, 16);
    key_entry->key_id = key_id;

    ns_list_add_to_end(&tls_psk_list, key_entry);

    return 0;
}

int8_t arm_tls_check_key(uint16_t key_id)
{
    if (tls_get_key(key_id) == NULL) {
        return -1;
    }
    return 0;
}

int8_t arm_tls_remove_psk_key(uint16_t key_id)
{
    tls_psk_key_t *key_to_remove = tls_get_key(key_id);

    if (key_to_remove == NULL) {
        return -1;
    }

    ns_list_remove(&tls_psk_list, key_to_remove);

    ns_dyn_mem_free(key_to_remove);

    return 0;
}

/* returns key entry, null if not found */
static tls_psk_key_t *tls_get_key(uint16_t key_id)
{
    ns_list_foreach(tls_psk_key_t, entry, &tls_psk_list) {
        if (entry->key_id == key_id) {
            return entry;
        }
    }
    return NULL;
}



void tls_finnish_copy(uint8_t *ptr, tls_heap_t *heap_ptr)
{
    tls_msg_t *tmp_msg = tls_msg_ptr_get();
    tmp_msg->len = 12;
    tmp_msg->msg_ptr = ptr;
    tls_handshake_copy(tmp_msg, heap_ptr);

}

uint8_t tls_parse_client_hello(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite)
{
    uint8_t ret_val = 0, i = 0;
    uint16_t tls_version;
    tls_version = common_read_16_bit(ptr);
    ptr += 2;
    if (tls_version == TLS_1_2_VERSION) {
        uint8_t id_len;
        tls_heap_t *thep = tls_suite->tls_session->tls_heap;
        memcpy((thep->tls_hello_random + CLIENT_HELLO_PTR), ptr, 32);
        ptr += 32;
        //skip sesion id
        id_len = *ptr++;
        len -= 35;
        if (id_len < 33 && (len > id_len + 3)) {
            if (id_len == 0) {
                tr_debug("Fisrt Time generate ID!!");
                len -= 2;
                tls_session_id_genrate(tls_suite->tls_session->tls_session_id, 4);
                tls_suite->tls_session->id_length = 4;
            } else {
                if (tls_suite->tls_session->id_length == id_len) {
                    if (memcmp(tls_suite->tls_session->tls_session_id, ptr, id_len) == 0) {
                        tr_debug("Generate new Session");
                        tls_session_id_genrate(tls_suite->tls_session->tls_session_id, 4);
                        tls_suite->tls_session->id_length = 4;

                    }
                }

                if (ret_val != 0) {
                    tr_debug("TLS SESSION ID FAIL");
                    return 0;
                }
                ptr += id_len;
                len -= id_len;
                len -= 1;
            }
            ptr++;

            id_len = *ptr++;
            ret_val = 0;

            while (id_len) {
                if (len < 2) {
                    tr_debug("Cor Client hello pack");
                    return 0;
                }
                uint16_t tls_ciphersuite = common_read_16_bit(ptr);
                switch (tls_ciphersuite) {
                    case TLS_PSK_WITH_AES_128_CCM_8:
                        tr_debug("Client Sup PSK");
                        ret_val |= SEC_CIPHERSUITE_PSK;
                        break;
#ifdef ECC
                    case TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8:
                        thep->client_knows_standard_ecc_ciphersuite = true;
                    /* no break */
                    case TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8_COMPAT:
                        tr_debug("Client Sup ECC");
                        ret_val |= SEC_CIPHERSUITE_ECC;
                        break;
#endif
                    default:
                        tr_debug("Un Sup Suite: %04" PRIx16, tls_ciphersuite);
                        break;
                }
                ptr += 2;
                id_len -= 2;
                len -= 2;

            }
            ret_val &= tls_suite->supported_chipher_suites;
            if (ret_val) {
                tr_debug("Client pack TRUE");

                if (ret_val & SEC_CIPHERSUITE_ECC) {
                    thep->tls_chipher_mode = CHIPHER_ECC;
                } else {
                    thep->tls_chipher_mode = CHIPHER_PSK;
                }
            } else {
                tr_debug("CipherSuite Err");
                i = 2;
            }
        } else {
            tr_debug("Session ID length Fail: %02x", *ptr);
            i = 4;
        }
    } else {
        i = 3;
    }
    if (i) {
        tr_debug("%02x", i);
    }
    return ret_val;
}


uint8_t tls_parse_server_hello(uint8_t *ptr, sec_suite_t *tls_suite)
{
    uint8_t ret_val = 0, i = 0;
    uint16_t tls_version;
    tls_version = common_read_16_bit(ptr);
    ptr += 2;
    if (tls_version == TLS_1_2_VERSION) {
        uint8_t id_len;
        tls_heap_t *thep = tls_suite->tls_session->tls_heap;
        if (thep == 0) {
            return 0;
        }
        memcpy((thep->tls_hello_random + SERVER_HELLO_PTR), ptr, 32);
        ptr += 32;
        //skip sesion id
        id_len = *ptr++;
        if (id_len < 33) {
            tls_suite->tls_session->id_length = id_len;
            memcpy(tls_suite->tls_session->tls_session_id, ptr, id_len);
            ptr += id_len;
            uint16_t tls_cipher_suite = common_read_16_bit(ptr);
            switch (tls_cipher_suite) {
                case TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8:
                case TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8_COMPAT:
                    tr_debug("ECC CipherSuite");
                    ret_val = 2;
                    thep->tls_chipher_mode = CHIPHER_ECC;
                    break;
                case TLS_PSK_WITH_AES_128_CCM_8:
                    tr_debug("PSK CipherSuite");
                    ret_val = 1;
                    thep->tls_chipher_mode = CHIPHER_PSK;
                    tls_ecc_heap_free(thep);
                    break;
                default:
                    tr_debug("CipherSuite Err: %04x", tls_cipher_suite);
                    ptr += 2;
                    i = 2;
                    break;
            }
        } else {
            tr_debug("Session ID length Fail: %02x", *ptr);
            i = 4;
        }
    } else {
        i = 3;
    }
    if (i) {
        tr_debug("%02x", i);
    }
    return ret_val;
}


void tls_alert_build(buffer_t *buf, uint8_t alert)
{
    tr_debug("TTLs TX:Alert");
    //Build Client Hello
    buffer_data_clear(buf);
    buffer_push_uint8(buf, TLS_ALERT_TYPE);
    buffer_push_uint16(buf, TLS_1_2_VERSION);
    buffer_push_uint16(buf, 0x0002); //Length

    if (alert == ALERT_CLOSE) {
        buffer_push_uint8(buf, 1); //Mean 2=Fatal, 1=warming
    } else {
        buffer_push_uint8(buf, 2); //Mean 2=Fatal, 1=warming
    }
    buffer_push_uint8(buf, alert);
}

#ifdef PANA_SERVER_API
void tls_server_hello_build(buffer_t *buf,  sec_suite_t *tls_suite)
{
    uint16_t tls_len = 0;
    uint8_t *ptr;
    tls_msg_t *tmp_msg = tls_msg_ptr_get();
    tr_debug("TTLs TX:Server Hello");
    //Build Client Hello
    buffer_data_clear(buf);
    ptr = buffer_data_pointer(buf);
    *ptr++ = TLS_HANDSHAKE;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    ptr = common_write_16_bit(tls_len, ptr);

    //for Hash calculation
    tmp_msg->msg_ptr = ptr + 4;
    ptr = tls_build_server_hello_msg(ptr, tls_suite->tls_session);
    buffer_data_end_set(buf, ptr);

    tls_len = buffer_data_length(buf);
    tls_len -= 9;
    tmp_msg->len = tls_len;
    tls_handshake_copy(tmp_msg, tls_suite->tls_session->tls_heap);

    tls_len += 4;
    ptr = buffer_data_pointer(buf);
    ptr += 3;
    ptr = common_write_16_bit(tls_len, ptr);
}
#endif

void tls_prepare_change_chipher_spec(sec_suite_t *tls_suite)
{
    tls_heap_t *theap = tls_suite->tls_session->tls_heap;
    bool server;
    tls_build_client_verify_payload(theap);
    if ((tls_suite->setups & TLS_SERVER_MODE) == 0) {
        server = false;
        if ((tls_suite->setups & TLS_HANSHAKE_HASH) == 0) {
            tls_finnish_copy(theap->hash_buf + 4, theap);
            tls_suite->setups |= TLS_HANSHAKE_HASH;
        }

    } else {
        server = true;
    }

    tls_ccm_data_encrypt(theap->hash_buf, 16, tls_suite->tls_session->key_expansion, tls_suite->tls_session->tls_nonce_explit, TLS_HANDSHAKE, server);
}


static buffer_t *tls_down(buffer_t *buf)
{
    uint16_t tls_len;
    if ((buf = buffer_headroom(buf, 5)) != 0) {
        uint8_t *ptr;
        buffer_data_reserve_header(buf, 5);
        ptr = buffer_data_pointer(buf);
        tls_len = buffer_data_length(buf);
        tls_len -= 5; //Cut Flags byte off

        *ptr++ = TLS_HANDSHAKE;
        ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
        ptr = common_write_16_bit(tls_len, ptr);
    }
    return (buf);
}

#ifdef ECC

void tls_parse_subject_get_pub_key_from_chain(tls_heap_t *theap, uint8_t rd_ptr)
{
    uint8_t *ptr = 0;
    certificate_info_t *cer_info = &(theap->rx_ceri_chain.certi_chain[rd_ptr]);
    uint8_t i;
    ptr = cer_info->pub_key_ptr;
    theap->ecc_heap->cert_pub_key.finite = 1;
    theap->ecc_heap->cert_pub_key.invalid = 0;

    memset(theap->ecc_heap->cert_pub_key.x.data, 0, sizeof(MPint));
    memset(theap->ecc_heap->cert_pub_key.y.data, 0, sizeof(MPint));
    tr_debug("Certificates PUB Key: %s", tr_array(ptr, 64));
    for (i = 0; i < 32; i++) {

        *((uint8_t *)theap->ecc_heap->cert_pub_key.x.data + i) = *(ptr + 31 - i) ;
        *((uint8_t *)theap->ecc_heap->cert_pub_key.y.data + i) = *(ptr + 63 - i) ;

    }
}



uint8_t tls_parse_certificate(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite)
{

    uint16_t sub_len = 0;
    uint8_t ret_val = 0;
    tls_heap_t *theap = tls_suite->tls_session->tls_heap;
    //Check Lengths
    if (*ptr++) {
        tr_debug("Too Long len");
    } else {
        sub_len = common_read_16_bit(ptr);
        ptr += 2;
        len -= 3;
        tr_debug("Certi(Chain) Len: %04x", sub_len);
        if (sub_len == 0 || sub_len > len) {
            tr_debug("Cert Base len mis match");
        } else {
            if (x509_v3_certi_chain_analyze(ptr, sub_len, &(theap->rx_ceri_chain)) == 0) {
                tr_debug("Certificate Chain not valid");
            } else {
                tls_parse_subject_get_pub_key_from_chain(theap, 0);
                ret_val = 1;
            }
        }
    }
    return ret_val;

}


uint8_t tls_parse_server_key_exchange(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite)
{
    (void)len;
    uint16_t curve_type;
    if (*ptr++ == 3) {
        curve_type = common_read_16_bit(ptr);
        if (curve_type == TLS_NAMED_CURVE_SECP256R1) {
            uint8_t c_len = 0, mode;
            uint16_t sig_algh, signature_len;
            tls_heap_t *theap = tls_suite->tls_session->tls_heap;
            ptr += 2;
            //len -= 5;
            c_len = *ptr++;
            mode = *ptr++;

            if (mode == 4) {
                c_len--;
                memcpy(theap->ecc_heap->server_public_key, ptr, 64);
                ptr += 64;
                sig_algh = common_read_16_bit(ptr);
                if (sig_algh != TLS_SIG_HASH_ALG_SHA256_ECDSA) {
                    tr_debug("tls ser key ex. er");
                    return 0;
                }
                ptr += 2;
                signature_len = common_read_16_bit(ptr);
                if (signature_len < 11) {
                    return 0;
                }
                ptr += 2;
                if (*ptr++ != 0x30) {
                    return 0;
                }
                if (*ptr++ < 9) {
                    return 0;
                }
                if (*ptr++ != 2) {
                    return 0;
                } else {
                    uint8_t rslen;
                    //uint8_t i;
                    theap->key_signature_ptr = ptr;
                    rslen = *ptr++;

                    if (rslen > 33) {
                        return 0;
                    } else if (rslen == 0) {
                        return 0;
                    } else if (rslen == 33) {

                        ptr++;
                        rslen = 32;

                    }
                    if (theap->ecc_heap->sgnt == 0) {
                        theap->ecc_heap->sgnt = ECDSA_get_signature();
                    }
                    if (!theap->ecc_heap->sgnt) {
                        return 0;
                    }

                    ptr += rslen;
                    if (*ptr++ != 2) {
                        return 0;
                    }

                    rslen = *ptr++;

                    if (rslen > 33) {
                        return 0;
                    } else if (rslen == 0) {
                        return 0;
                    }
                    return 1;
                }

            } else {
                tr_debug("Mode!: %02x", mode);
            }
        }
    }
    return 0;
}

static uint8_t tls_parse_client_key_exchange(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite)
{
    (void)len;
    uint8_t d_len = 0;
    uint8_t mode = 0;
    d_len = *ptr++;
    mode = *ptr++;
    if (d_len == 65 && mode == 4) {
        tls_heap_t *theap = tls_suite->tls_session->tls_heap;
        tr_debug("Valid Client ECC curve:");
        memcpy(theap->ecc_heap->client_public_key, ptr, 64);
        tr_debug("%s", tr_array(ptr, 64));
        return 1;
    } else {
        tr_debug("Len: %02x, mode fail: %02x", d_len, mode);
    }

    return 0;
}


void tls_read_certi_signature(tls_heap_t *theap, uint8_t certificate)
{
    uint8_t rslen = 0;
    uint8_t *ptr = 0;
    if (certificate) {
        //
        certificate_info_t *cer_info = &(theap->rx_ceri_chain.certi_chain[theap->rx_ceri_chain.rd_ptr]);
        ptr = cer_info->signature_ptr;
    } else {
        ptr = theap->key_signature_ptr;
    }
    memset(theap->ecc_heap->sgnt->m_R.data, 0, sizeof(MPint));
    memset(theap->ecc_heap->sgnt->m_s.data, 0, sizeof(MPint));
    rslen  = x509_v3_parse_signature_parameter(ptr, ((uint8_t *)theap->ecc_heap->sgnt->m_R.data));
    ptr += rslen;
    ptr++;
    x509_v3_parse_signature_parameter(ptr, ((uint8_t *)theap->ecc_heap->sgnt->m_s.data));

}


uint8_t tls_parse_certificate_verify(uint8_t *ptr, uint16_t len, sec_suite_t *tls_suite)
{
    (void)len;
    uint16_t sig_algh, sig_len;
    tls_heap_t *theap = tls_suite->tls_session->tls_heap;

    sig_algh = common_read_16_bit(ptr);
    if (sig_algh != TLS_SIG_HASH_ALG_SHA256_ECDSA) {
        tr_debug("tls ser key ex. er2");
        return 0;
    }
    ptr += 2;
    sig_len = common_read_16_bit(ptr);
    if (sig_len < 11) {
        return 0;
    }
    ptr += 2;
    if (*ptr++ != 0x30) {
        return 0;
    }
    if (*ptr++ < 9) {
        return 0;
    }
    if (theap->ecc_heap) {
        if (theap->ecc_heap->sgnt == 0) {
            tr_debug("Allocate SIG");
            theap->ecc_heap->sgnt = ECDSA_get_signature();

            if (!theap->ecc_heap->sgnt) {
                tr_debug("Signature Allocate Fail");
                return 0;
            }
        }
    } else {
        return 0;
    }
    memset(theap->ecc_heap->sgnt->m_R.data, 0, sizeof(MPint));
    memset(theap->ecc_heap->sgnt->m_s.data, 0, sizeof(MPint));


    if (*ptr++ != 2) {
        return 0;
    } else {
        uint8_t rslen = 0;
        theap->key_signature_ptr = ptr;
        rslen  = x509_v3_parse_signature_parameter(ptr, ((uint8_t *)theap->ecc_heap->sgnt->m_R.data));
        if (rslen == 0) {
            return 0;
        }
        ptr += rslen;
    }
    if (*ptr++ != 2) {
        return 0;
    } else {
        uint8_t rslen = 0;
        rslen  = x509_v3_parse_signature_parameter(ptr, ((uint8_t *)theap->ecc_heap->sgnt->m_s.data));
        if (rslen == 0) {
            return 0;
        }
        //ptr += rslen;
    }
    return 1;
}


tls_ecc_heap_t *ecc_allocate_ram(void)
{
    tls_ecc_heap_t *heap_ptr = (tls_ecc_heap_t *) ns_dyn_mem_temporary_alloc(sizeof(tls_ecc_heap_t));

    if (heap_ptr == 0) {
        tr_debug("ECC variable malloc fail");
    } else {
        //heap_scan();
        heap_ptr->sgnt = ecc_get_ecdsa_signature();
        if (!heap_ptr->sgnt) {
            tr_debug("Signature Fail");
            ns_dyn_mem_free(heap_ptr);
            heap_ptr = 0;
        }
    }
    return heap_ptr;

}


uint8_t *tls_client_key_exchange_msg_set(uint8_t *ptr, tls_heap_t *heap_ptr)
{
    /* Client Key Exchange */
    *ptr++ = TLS_CLIENT_KEY_EXCHANGE;
    ptr = common_write_24_bit(66, ptr);
    ptr = common_write_16_bit(0x4104, ptr);
    //There shuold be calculated case now only test purpose
    memcpy(ptr, heap_ptr->ecc_heap->client_public_key, 64);
    ptr += 64;
    return ptr;
}
#endif


uint8_t *tls_set_client_key_excange(uint8_t *ptr, sec_suite_t *tls_suite)
{
    uint8_t key_id_len = (tls_suite->psk_key_id > 0xFF) ? 2 : 1;

    /* TLS plaintext header = [type][version][length] */
    *ptr++ = TLS_HANDSHAKE;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    ptr = common_write_16_bit(6 + key_id_len, ptr);

    /* Handshake message [msg_type][length] */
    *ptr++ = TLS_CLIENT_KEY_EXCHANGE;
    ptr = common_write_24_bit(2 + key_id_len, ptr);

    /* ClientKeyExchange [length][psk_id] */
    ptr = common_write_16_bit(key_id_len, ptr); //len
    if (tls_suite->psk_key_id > 0xFF) {
        *ptr++ = tls_suite->psk_key_id >> 8;
    }
    *ptr++ = tls_suite->psk_key_id;

    return ptr;
}

uint8_t *tls_build_change_chipher_suite_finnish_msg(uint8_t *ptr, tls_session_t *tls_session)
{
    *ptr++ = TLS_CHANGE_CIPHER_SPEC;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    ptr = common_write_16_bit(1, ptr); //len
    *ptr++ = 1;
    *ptr++ = TLS_HANDSHAKE;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    ptr = common_write_16_bit(32, ptr); //len
    //Calculate Verify and MAC (MIC)
    //CCMNONCEEXPLICT //SHuold be 0x0000000000000000 at client side

    memcpy(ptr, tls_session->tls_nonce_explit, 8); // Dynamic
    ptr += 8;
    //MSG Finished
    memcpy(ptr, tls_session->tls_heap->hash_buf, 24);
    ptr += 24;
    return ptr;
}

void tls_build_client_change_chipher_suite_finnish(buffer_t *buf, sec_suite_t *tls_suite)
{
    uint8_t *ptr;
    buffer_data_clear(buf);
    ptr = buffer_data_pointer(buf);
    if ((tls_suite->setups & TLS_SERVER_MODE) == 0) {
        ptr = tls_set_client_key_excange(ptr, tls_suite);
    }
    ptr = tls_build_change_chipher_suite_finnish_msg(ptr, tls_suite->tls_session);
    buffer_data_end_set(buf, ptr);
    tr_debug("TLS TX: KEY Exchange");
}
#ifdef ECC
void tls_ecc_verfify_start(sec_suite_t *tls_suite)
{
    if (ecc_state_idle_check() == ECC_STATUS_OK) {
        tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
        uint8_t hash_response = ecc_verify_calculate_hash(tls_suite);
        if (hash_response) {
            tr_debug("HASH calc Fail: %02x", hash_response);
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
        } else {
            int8_t ecc_response = ecc_calculate_verify(tls_heap->ecc_heap->sgnt, &(tls_heap->ecc_heap->cert_pub_key), &ecc_operation_done_callback);
            if (ecc_response == ECC_STATUS_OK) {
                tls_heap->ecc_heap->sgnt = 0;
                sec_lib_state_machine_lock(tls_suite, TLS_ECC_MESSAGE_VERIFY);
                sec_ecc_state_save(tls_suite);
            } else {
                tr_debug("calcVerify Fail: %d", ecc_response);
            }
        }
    } else {
        sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_VERIFY_START2);
    }
}


void tls_certificate_signature_verify(sec_suite_t *tls_suite)
{
    tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
    if (ecc_state_idle_check() == ECC_STATUS_OK) {
        uint8_t alert_tx = 0;
        uint8_t chain_analyze_ok = 0;
        if (tls_heap->rx_ceri_chain.rd_ptr + 1 <= tls_heap->rx_ceri_chain.wr_ptr) {
            uint8_t certi_verify = 0;
            certificate_info_t *cer_info = &(tls_heap->rx_ceri_chain.certi_chain[tls_heap->rx_ceri_chain.rd_ptr]);

            //Verify Cur Certi Signature bye Next Certi PK
            tr_debug("index,Check Signature: rd: %02x, wr: %02x", tls_heap->rx_ceri_chain.rd_ptr, tls_heap->rx_ceri_chain.wr_ptr);

            if ((tls_heap->rx_ceri_chain.rd_ptr + 1) == tls_heap->rx_ceri_chain.wr_ptr) {
                uint8_t self_signed = 0;
                if (cer_info->issue_len == cer_info->subj_len) {
                    if (memcmp(cer_info->issue_ptr, cer_info->subj_ptr, cer_info->issue_len) == 0) {
                        self_signed =  1;
                    }
                }

                if (self_signed) {
                    certificate_chain_internal_t   *temp;

                    temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);

                    tr_debug("SELF Signed. Compare Root");
                    if (temp) {
                        uint16_t root_len = 0;
                        uint8_t root_type = 0;
                        const uint8_t *ptr = temp->certi_chain[0];
                        uint8_t *root_ptr = cer_info->certi_ptr;
                        root_ptr -= 4;
                        asn1_parse(root_ptr, &root_len, &root_type);
                        alert_tx = 1;
                        if ((root_len + 4) == temp->certi_len[0]) {
                            if (memcmp(root_ptr, ptr, temp->certi_len[0]) == 0) {
                                tr_debug("Trusted Root certificate");
                                alert_tx = 0;
                                chain_analyze_ok = 1;
                            } else {
                                tr_debug("Not Trusted Root");
                            }
                        } else {
                            alert_tx = 1;
                        }
                    } else {
                        alert_tx = 1;
                    }
                } else {

                    //Compare all ways to Root Certi
                    certificate_chain_internal_t   *temp;

                    temp = sec_cetificate_chain_get(SEC_NWK_AUTHENTICATION_CERTI_CHAIN);
                    if (temp) {
                        if (temp->sub_len[0] == cer_info->issue_len) {
                            if (memcmp(cer_info->issue_ptr, temp->sub_chain[0], cer_info->issue_len) == 0) {
                                tr_debug("Get Root PK");
                                tls_heap->rx_ceri_chain.wr_ptr--;
                                if (x509_v3_certi_pk_get((uint8_t *)temp->certi_chain[0], &(tls_heap->rx_ceri_chain)) == 0) {
                                    tr_debug("..fail");
                                } else {
                                    tr_debug("..OK");
                                    tls_parse_subject_get_pub_key_from_chain(tls_heap, (tls_heap->rx_ceri_chain.rd_ptr));
                                    certi_verify = 2;
                                }
                            }
                        }
                        if (certi_verify == 0) {
                            alert_tx = 1;
                            tr_debug("Unknow CA");
                        }
                    } else {
                        alert_tx = 1;
                    }
                }
            } else {

                certificate_info_t *cer_sig = &(tls_heap->rx_ceri_chain.certi_chain[tls_heap->rx_ceri_chain.rd_ptr + 1]);

                if (cer_info->issue_len == cer_sig->subj_len) {
                    if (memcmp(cer_info->issue_ptr, cer_sig->subj_ptr, cer_info->issue_len) == 0) {
                        certi_verify =  1;
                    } else {
                        alert_tx = 1;
                        tr_debug("Not Valid chain signature subject entry");
                    }
                } else {
                    alert_tx = 1;
                    tr_debug("Not Valid chain signature subject len");
                }


            }
            if (certi_verify) {
                //Calc HASH
                memset((uint8_t *)tls_heap->ecc_heap->sgnt->m_m.data, 0, sizeof(MPint));
                ns_sha256(cer_info->certi_ptr, cer_info->certi_len, tls_heap->ecc_heap->sgnt->m_m.data);
                tls_ecc_reverse_hash((uint8_t *)tls_heap->ecc_heap->sgnt->m_m.data);

                //GET Signature From Cur Certi
                tls_read_certi_signature(tls_heap, 1);

                //GET PUB KEY from next HOP
                if (certi_verify == 1) {
                    tls_parse_subject_get_pub_key_from_chain(tls_heap, (tls_heap->rx_ceri_chain.rd_ptr + 1));

                }

                if (ecc_calculate_verify(tls_heap->ecc_heap->sgnt, &tls_heap->ecc_heap->cert_pub_key, &ecc_operation_done_callback) == ECC_STATUS_OK) {
                    tls_heap->ecc_heap->sgnt = 0;
                    sec_lib_state_machine_lock(tls_suite, TLS_ECC_CERTIFICATE_SIGNATURE_CHECK);
                    sec_ecc_state_save(tls_suite);
                    tls_heap->rx_ceri_chain.rd_ptr++;
                } else {
                    if (tls_heap->ecc_heap->sgnt) {
                        ns_dyn_mem_free(tls_heap->ecc_heap->sgnt);
                        tls_heap->ecc_heap->sgnt = 0;
                    }
                    alert_tx = 1;

                }
            }
        } else {
            chain_analyze_ok = 1;
        }

        if (chain_analyze_ok) {
            if (tls_heap->signature_temp_buf) {
                //Take Signature
                tls_read_certi_signature(tls_heap, 0);
                if (tls_heap->signature_temp_buf == tls_heap->cert_temp_buf) {
                    tls_heap->signature_temp_buf = 0;
                } else {
                    if (tls_heap->pointer_types & 2) {
                        buffer_free(tls_heap->signature_temp_buf);
                    } else {
                        ns_dyn_mem_free(tls_heap->signature_temp_buf);
                    }
                    tls_heap->signature_temp_buf = 0;
                }
            }

            tr_debug("Certi signature check OK");
            tls_parse_subject_get_pub_key_from_chain(tls_heap, 0);

            if (tls_heap->rx_ceri_chain.wr_ptr == 0) {
                certificate_info_t *c_ptr = &(tls_heap->rx_ceri_chain.certi_chain[0]);
                if (x509_v3_certi_pk_get((uint8_t *)c_ptr->certi_ptr, &tls_heap->rx_ceri_chain) == 0) {
                    tr_debug("Fail read certificate PUB Key");
                } else {
                    tls_parse_subject_get_pub_key_from_chain(tls_heap, (tls_heap->rx_ceri_chain.wr_ptr));
                }
            }

            if (tls_heap->pointer_types & 1) {
                buffer_free(tls_heap->cert_temp_buf);
            } else {
                ns_dyn_mem_free(tls_heap->cert_temp_buf);
            }
            tls_heap->cert_temp_buf = 0;

            if (tls_suite->setups & TLS_SERVER_MODE) {
                sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_SERVER_VERIFY_START);
            } else {
                tr_debug("Start Do Certi Verify calc");
                sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_VERIFY_START2);
            }
        }
        if (alert_tx) {
            tr_debug("PSK cal fail");
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
        }
    } else {
        tls_suite->timer = 1;
    }
}

void tls_server_finnish_handle_start(sec_suite_t *tls_suite)
{
    if (ecc_state_idle_check() == ECC_STATUS_OK) {
        tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
        uint8_t alert_tx = 0;
        if (tls_suite->setups & TLS_ECC_CERTIFICATE_VERIFY) {
            memcpy(tls_heap->ecc_heap->sgnt->m_m.data, tls_heap->hash_buf, 32);
            // initialize the rest of MPint as the hash is covering only part of it
            memset(((uint8_t *)tls_heap->ecc_heap->sgnt->m_m.data) + 32, 0, sizeof(MPint) - 32);
            tls_ecc_reverse_hash((uint8_t *)tls_heap->ecc_heap->sgnt->m_m.data);

            if (ecc_calculate_verify(tls_heap->ecc_heap->sgnt, &tls_heap->ecc_heap->cert_pub_key, &ecc_operation_done_callback) == ECC_STATUS_OK) {
                tls_heap->ecc_heap->sgnt = 0;
                sec_lib_state_machine_lock(tls_suite, TLS_ECC_MESSAGE_VERIFY);
                sec_ecc_state_save(tls_suite);
            } else {
                if (tls_heap->ecc_heap->sgnt) {
                    ecc_library_free_pointer(tls_heap->ecc_heap->sgnt);
                    //ns_dyn_mem_free(tls_heap->ecc_heap->sgnt);
                    tls_heap->ecc_heap->sgnt = 0;
                }
                alert_tx = 1;

            }
        } else {
            if (tls_ecc_start_premaster_secret(0, tls_suite) == 0) {
                alert_tx = 1;
            }
        }
        if (alert_tx) {
            tr_debug("PSK cal fail");
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
        }
    } else {
        sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_SERVER_VERIFY_START);
    }
}
#endif

#ifdef PANA_SERVER_API
static buffer_t *tls_verify_handler(uint8_t certi_rx, tls_header_t *tls_header_ptr, buffer_t *buf, sec_suite_t *tls_suite)
{
    (void) certi_rx;
    tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
    tls_heap->client_verify_buf_len = tls_header_ptr->length;
    if (tls_heap->client_verify_buf) {
        tr_debug("Free Server Client 1r");
        ns_dyn_mem_free(tls_heap->client_verify_buf);
        tls_heap->client_verify_buf = 0;
    }
    tls_heap->client_verify_buf = ns_dyn_mem_alloc(tls_header_ptr->length);
    if (tls_heap->client_verify_buf) {
        memcpy(tls_heap->client_verify_buf, tls_header_ptr->ptr, tls_header_ptr->length);

#ifdef ECC
        if (tls_heap->tls_chipher_mode == CHIPHER_ECC) {

            if (certi_rx) {
                sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_SERVER_VERIFY_START);
                tr_debug("Certi RX > 0");
                if (tls_suite->tls_session->tls_heap) {
                    uint8_t free_buf = 1;
                    if (certi_rx & 1) {
                        if (tls_heap->cert_temp_buf == 0) {
                            tls_heap->pointer_types |= 1;
                            tls_heap->cert_temp_buf = buf;
                            free_buf = 0;
                        }
                    }
                    if (certi_rx & 2) {
                        if (tls_heap->signature_temp_buf == 0) {
                            tls_heap->pointer_types |= 2;
                            tls_heap->signature_temp_buf = buf;
                            free_buf = 0;
                        }
                    }

                    if (free_buf == 0) {
                        buf = NULL;
                    }
                }

            }
            if (buf) {
                buf = buffer_free(buf);
            }

        } else
#endif
        {
            tr_debug("Start Server PRF_CALC state");
            sec_prf_state_set(tls_suite);
        }
    } else {
        tr_debug("Heap_error: client verify chiphersuite!!");
    }

    return (buf);
}
#endif

static int8_t tls_client_verify_handler(tls_header_t *tls_header_ptr, sec_suite_t *tls_suite)
{
    if (tls_ccm_data_decrypt(tls_header_ptr->ptr, tls_header_ptr->length, tls_suite->tls_session->key_expansion, TLS_HANDSHAKE, true) == 0) {
        uint8_t *ptr = tls_header_ptr->ptr + 8;
        tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;

        if (ptr[0] == TLS_FINISHED && common_read_24_bit(ptr + 1) == 12) {
            ptr += 4;
            //
            tr_debug("Finish RX: %s", tr_array(ptr, 12));

            memcpy(tls_heap->verify, ptr, 12);
            return 0;
        } else {
            //RETURN error
            tr_debug("No Chiphertext");
            return -1;
        }
    }

    tr_debug("Encode error");
    return -2;

}

static buffer_t *tls_certificate_buffer_store(buffer_t *buf, uint8_t certi_rx, sec_suite_t *tls_suite)
{
    if (certi_rx) {
        if (tls_suite->tls_session->tls_heap) {
            uint8_t free_buf = 1;
            tls_heap_t *theap = tls_suite->tls_session->tls_heap;
            if (certi_rx & 1) {
                if (theap->cert_temp_buf == 0) {
                    theap->pointer_types |= 1;
                    theap->cert_temp_buf = buf;
                    free_buf = 0;

                }
            }
            if (certi_rx & 2) {
                if (theap->signature_temp_buf == 0) {
                    theap->pointer_types |= 2;
                    theap->signature_temp_buf = buf;
                    free_buf = 0;
                }
            }
            if (free_buf == 0) {
                buf = NULL;
            }
        }
    }

    if (buf) {
        buf = buffer_free(buf);
    }

    return buf;
}

buffer_t *tls_client_up(buffer_t *buf, sec_suite_t *tls_suite)
{
    uint8_t i = 0, certi_rx = 0;
    uint16_t data_len = 0;
    uint8_t alert_case = 0;
    uint8_t *ptr;
    tls_header_t *tls_header_ptr;
    tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
    uint8_t algo_ok = 0, j = 0;
    ptr = buffer_data_pointer(buf);

    i = 1;
    while (buf->buf_ptr < buf->buf_end) {
        data_len = buffer_data_length(buf);
        ptr = buffer_data_pointer(buf);
        tls_header_ptr = NULL;

        if (data_len >= 5) {
            uint16_t tls_version;
            tls_header.type = *ptr++;
            tls_version = common_read_16_bit(ptr);
            ptr += 2;
            if (tls_version != TLS_1_2_VERSION) {
                tr_debug("Len: %04x", data_len);
                tr_debug("%s", tr_array(ptr, 4));
                alert_case = 4;
            } else {
                tls_header.length = common_read_16_bit(ptr);
                ptr += 2;
                data_len -= 5;
                if (tls_header.length > data_len) {
                    alert_case = 5;
                } else {
                    tr_debug("Full TLS Record");
                    tls_header.ptr = ptr;
                    buf->buf_ptr += tls_header.length;
                    buf->buf_ptr += 5;
                    tls_header_ptr = &tls_header;
                }
            }
        } else {
            alert_case = 5;
        }

        if (alert_case) {
            tr_debug("TLS Segmentation or datagram error: %02x", alert_case);
            buf->buf_ptr = buf->buf_end;
            //SET Alert Case
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
            return buffer_free(buf);
        }

        if (tls_header_ptr) {
            if (tls_header_ptr->type == TLS_HANDSHAKE && (tls_heap != 0)) {
                tr_debug("Type:Handshake");
                if (tls_suite->state == TLS_CHANGE_CHIPHER) {
                    if (tls_header_ptr->length < 32) {
                        tr_debug("Too short Chiher Text");
                    } else if ((algo_ok & 0x20) && (tls_suite->state == PRF_CALC)) {
                        tr_debug("Drop Client RE TX");
                        /*tls_text_cnt = */j = 1;
                    } else {
                        int8_t returnCode = tls_client_verify_handler(tls_header_ptr, tls_suite);
                        if (returnCode == 0) {
                            algo_ok |= 0x10;
                            sec_lib_state_machine_trig(tls_suite, TLS_FINISH);
                            j = 1;
                        } else if (returnCode == -1) {
                            sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
                            return buffer_free(buf);
                        } else {
                            i = 5;
                            sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
                            break;
                        }

                        //Set Back Originals
                        tls_header_ptr->ptr -= 8;
                        tls_header_ptr->length += 16;
                    }
                } else {
                    uint8_t tls_msg_cnt = 0;
                    tls_msg_t *tls_msg_ptr = 0;
                    if (algo_ok != 0x18) {
                        tls_msg_cnt = tls_msg_analyzy(tls_header_ptr->ptr, tls_header_ptr->length);
                        j = 0;
                    }
                    ptr = tls_header_ptr->ptr;
                    while (j < tls_msg_cnt) {
                        tls_msg_ptr = tls_msg_get(ptr);

                        switch (tls_msg_ptr->type) {
                            case TLS_SERVER_HELLO:
                                if (tls_msg_ptr->len >= 38) {
                                    if (tls_suite->state == TLS_INIT) {
                                        if (tls_parse_server_hello(tls_msg_ptr->msg_ptr, tls_suite)) {

                                            uint8_t switch_state = 0;
                                            tr_debug("TLS:S_Hello OK");
                                            if (tls_heap->tls_chipher_mode != CHIPHER_ECC) {
                                                algo_ok |= 1;
                                                switch_state = 1;
                                                tls_ecc_heap_free(tls_heap);
                                            } else {

#ifdef ECC
                                                tls_suite->retry_counter = 0;
                                                switch_state = 1;
#else
                                                tr_debug("NO ECC Sup");
                                                sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
                                                return buffer_free(buf);
#endif

                                            }
                                            if (switch_state) {
                                                tls_suite->setups &= ~TLS_HANSHAKE_HASH;
                                                sec_set_auth_timeout(tls_suite, TLS_HELLO_RX);
                                                tls_suite->retry_counter = 0;
                                                tls_handshake_copy(tls_msg_ptr, tls_heap);
                                            }
                                        } else {
                                            tr_debug("TLS:S_Hello Fail: %02x", tls_suite->state);
                                        }
                                    } else {
                                        tr_debug("SERver RE Tx  drop Ser Hello");
                                    }
                                }
                                break;

                            case TLS_SERVER_HELLO_DONE:
                                if (tls_msg_ptr->len == 0) {
                                    if (tls_suite->state >= TLS_CERTIFICATE_RX && tls_suite->state < PANA_ERROR) {
                                        tr_debug("TLS:S_Hello DONE");
                                        algo_ok |= 2;
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);
                                        sec_lib_state_machine_trig(tls_suite, TLS_HELLO_DONE);
                                    } else {
                                        tr_debug("Drop Ser Hello Done: %02x", tls_suite->state);
                                    }
                                }
                                break;

                            case TLS_CERTIFICATE:
#ifdef ECC
                                if (tls_suite->state == TLS_HELLO_RX) {
                                    tr_debug("TLS Certi RX");

                                    if (tls_parse_certificate(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {

                                        tls_suite->setups |= TLS_ECC_CERTIFICATE_RECEIVED;
                                        algo_ok = 0;
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);
                                        tls_suite->timer = pana_retry_req_max_get();

                                        tls_suite->state = TLS_CERTIFICATE_RX;
                                        certi_rx |= 1;

                                    } else {
                                        tr_debug("TLS Malformed Certificate");

                                        sec_lib_state_machine_trig(tls_suite, TLS_ALERT_BAD_CERTIFICATE);
                                        return buffer_free(buf);
                                    }

                                } else {
                                    tr_debug("Drop Cert: %02x", tls_suite->state);
                                    return buffer_free(buf);
                                }
#else
                                sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
#endif
                                break;

                            case TLS_SERVER_KEY_EXCHANGE:
                                tr_debug(" TLS Serv KEY Exchange RX");
#ifdef ECC
                                if (!tls_parse_server_key_exchange(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {
                                    tr_debug("Drop Key");
                                    sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
                                } else {
                                    algo_ok = 0;
                                    tls_handshake_copy(tls_msg_ptr, tls_heap);
                                    certi_rx |= 2;
                                    sec_set_auth_timeout(tls_suite, TLS_SERVER_KEY_EXCHANGE_RX);
                                }
#else
                                tr_debug("Drop Key");
                                sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
#endif

                                break;

                            case TLS_CERTIFICATE_REQUEST:
#ifdef ECC
                                if (tls_parse_certificate_request(tls_msg_ptr->msg_ptr, tls_msg_ptr->len)) {
                                    tr_debug(" TLS Cert request- ");
                                    if ((tls_suite->setups & TLS_ECC_CERTIFICATE_REQUESTED) == 0) {
                                        tls_suite->setups |= TLS_ECC_CERTIFICATE_REQUESTED;
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);

                                    }
                                } else
#endif
                                {
                                    tr_debug("Client drop Cert Request");
                                    sec_lib_state_machine_trig(tls_suite, TLS_ALERT_DECRYPT);
                                    return buffer_free(buf);
                                }
                                break;

                            default:
                                break;
                        }
                        ptr = tls_msg_ptr->msg_ptr;
                        ptr += tls_msg_ptr->len;
                        j++;
                    }
                }

            } else if (tls_header_ptr->type == TLS_CHANGE_CIPHER_SPEC && (tls_heap != 0) /*&& (tls_header[i].length == 1)*/) {
                uint8_t tx_alert = 1;
                tr_debug("TLS:Change ChipherS");
                tr_debug("%02x", tls_suite->state);
                if (tls_suite->state == TLS_KEY_CHANGE) {
                    tx_alert = 0;
                }

                if (tx_alert) {
                    tr_debug("Wrong state TX Alert");
                    sec_lib_state_machine_trig(tls_suite, TLS_ALERT_CLOSE_FATAL);
                    return buffer_free(buf);
                } else {
                    sec_set_auth_timeout(tls_suite, TLS_CHANGE_CHIPHER);
                    algo_ok |= 8;
                }

            } else if (tls_header_ptr->type == TLS_ALERT_TYPE) {
                tr_debug("Alert!!");
                if (tls_header_ptr->length == 2) {
                    uint8_t *dptr = tls_header_ptr->ptr;
                    tr_debug("%s", tr_array(tls_header_ptr->ptr, 2));
                    //Skip Alert Type and descriptions
                    dptr += 2;
                    algo_ok = 0xf0;
                    sec_lib_state_machine_trig(tls_suite, PANA_FAILURE);
                }
                i = 0xfe;
            }
        }
    }

    if (algo_ok) {
        if (tls_suite->state == TLS_HELLO_RX) {
            sec_set_auth_timeout(tls_suite, TLS_HELLO_RX);
            tls_suite->retry_counter = 0;
            i = 0xff;
        } else if (tls_suite->state == TLS_HELLO_DONE) {
            //ADD already Now Client key change handshake part
#ifdef ECC
            if (tls_heap->tls_chipher_mode == CHIPHER_ECC) {
                if (tls_suite->setups & TLS_ECC_CERTIFICATE_REQUESTED) {
                    tr_debug("Message verify start");
                    sec_lib_state_machine_trig(tls_suite, TLS_ECC_MESSAGE_VERIFY_START);
                } else {
                    tr_debug("Cert REQ diabled");
                }
            } else
#endif
            {

                tls_msg_t *tmp_msg = tls_msg_ptr_get();

                tmp_msg->len = 3;
                if (tls_suite->psk_key_id > 0xff) {
                    tmp_msg->len++;
                }
                ptr = buf->buf;
                *ptr++ = TLS_CLIENT_KEY_EXCHANGE;
                ptr = common_write_24_bit(tmp_msg->len, ptr);
                tmp_msg->msg_ptr = ptr;
                ptr = common_write_16_bit(tmp_msg->len - 2, ptr);
                if (tls_suite->psk_key_id > 0xff) {
                    *ptr++ = tls_suite->psk_key_id >> 8;
                }

                *ptr = tls_suite->psk_key_id;;
                tls_handshake_copy(tmp_msg, tls_heap);
                sec_prf_state_set(tls_suite);
            }
            i = 0xff;
        } else if (tls_suite->state == TLS_CHANGE_CHIPHER) {
            tr_debug("No Chipher Set Close state");
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
            i = 0xff;
        } else if (tls_suite->state == TLS_FINISH) {
            tr_debug("Finish RX");
            sec_lib_state_machine_trig(tls_suite, TLS_FINISH);
            i = 0xff;
        } else if (algo_ok == 0x18) {

        } else {
            i = 11;
            tr_debug("%02x", algo_ok);
        }
    }

    if (i) {
        buf = tls_certificate_buffer_store(buf, certi_rx, tls_suite);
    }
    return (buf);
}



buffer_t *tls_server_up(buffer_t *buf, sec_suite_t *tls_suite)
{
#ifdef PANA_SERVER_API
    uint8_t i = 0, certi_rx = 0;
    uint16_t data_len = 0;
    uint8_t alert_case = 0;
    uint8_t *ptr;
    tls_header_t *tls_header_ptr;
    tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
    uint8_t algo_ok = 0, j = 0;
    ptr = buffer_data_pointer(buf);

    i = 1;
    while (buf->buf_ptr < buf->buf_end) {
        data_len = buffer_data_length(buf);
        ptr = buffer_data_pointer(buf);
        tls_header_ptr = NULL;

        if (data_len >= 5) {
            uint16_t tls_version;
            tls_header.type = *ptr++;
            tls_version = common_read_16_bit(ptr);
            ptr += 2;
            if (tls_version != TLS_1_2_VERSION) {
                tr_debug("Len: %04x", data_len);
                tr_debug("%s", tr_array(ptr, 4));
                alert_case = 4;
            } else {
                tls_header.length = common_read_16_bit(ptr);
                ptr += 2;
                data_len -= 5;
                if (tls_header.length > data_len) {
                    alert_case = 5;
                } else {
                    tr_debug("Full TLS Record");
                    tls_header.ptr = ptr;
                    buf->buf_ptr += tls_header.length;
                    buf->buf_ptr += 5;
                    tls_header_ptr = &tls_header;
                }
            }
        } else {
            alert_case = 5;
        }

        if (alert_case) {
            tr_debug("TLS Segmentation or datagram error: %02x", alert_case);
            buf->buf_ptr = buf->buf_end;
            //SET Alert Case
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
            return buffer_free(buf);
        }

        if (tls_header_ptr) {
            if (tls_header_ptr->type == TLS_HANDSHAKE && (tls_heap != 0)) {
                tr_debug("Type:Handshake");
                if (tls_suite->state == TLS_CHANGE_CHIPHER) {
                    if (tls_header_ptr->length < 32) {
                        tr_debug("Too short Chiher Text");
                    } else if ((algo_ok & 0x20) && (tls_suite->state == PRF_CALC)) {
                        tr_debug("Drop Client RE TX");
                        /*tls_text_cnt = */j = 1;
                    } else {
                        buf = tls_verify_handler(certi_rx, tls_header_ptr, buf, tls_suite);
                        if (!buf) {
                            return buf;
                        }
                    }
                } else {
                    uint8_t tls_msg_cnt = 0;
                    tls_msg_t *tls_msg_ptr = 0;
                    if (algo_ok != 0x18) {
                        tls_msg_cnt = tls_msg_analyzy(tls_header_ptr->ptr, tls_header_ptr->length);
                        j = 0;
                    }
                    ptr = tls_header_ptr->ptr;
                    while (j < tls_msg_cnt) {
                        tls_msg_ptr = tls_msg_get(ptr);

                        switch (tls_msg_ptr->type) {
                            case TLS_CLIENT_HELLO:
                                //Parse
                                if (tls_parse_client_hello(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {
                                    tr_debug("TLS:C_Hello OK");
                                    algo_ok = 0;
                                    tls_heap->tls_handshake_h_len = 0;
                                    tls_handshake_copy(tls_msg_ptr, tls_heap);
                                    if (tls_heap->tls_chipher_mode == CHIPHER_ECC) {
#ifdef ECC
                                        sec_lib_state_machine_trig(tls_suite, TLS_SERVER_ECC_PUB_KEY_GEN);
#else
                                        tr_debug("NO ECC Sup");
                                        sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
                                        return buffer_free(buf);
#endif

                                    } else {
                                        tls_ecc_heap_free(tls_heap);
                                        sec_lib_state_machine_trig(tls_suite, TLS_SERVER_TX_SERVER_HELLO);
                                    }

                                } else {
                                    tr_debug("TLS:C_Hello Fail");
                                    sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
                                    return buffer_free(buf);
                                }
                                break;

                            case TLS_CERTIFICATE:
#ifdef ECC

                                if (tls_suite->state == TLS_TX_SERVER_KEY_EXCHANGE || tls_suite->state == TLS_SERVER_WAIT_CHANGE_CHIPHERSUITE) {
                                    tr_debug("TLS Certi RX");

                                    if (tls_parse_certificate(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {

                                        tls_suite->setups |= TLS_ECC_CERTIFICATE_RECEIVED;
                                        algo_ok = 0;
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);
                                        tls_suite->timer = pana_retry_req_max_get();
                                        tls_suite->state = TLS_CERTIFICATE_RX;
                                        certi_rx |= 1;

                                    } else {
                                        tr_debug("TLS Malformed Certificate");

                                        sec_lib_state_machine_trig(tls_suite, TLS_ALERT_BAD_CERTIFICATE);
                                        return buffer_free(buf);
                                    }

                                } else {
                                    tr_debug("Drop Cert: %02x", tls_suite->state);
                                    return buffer_free(buf);
                                }
#else
                                sec_lib_state_machine_trig(tls_suite, PANA_ERROR);
#endif
                                break;

                            case TLS_CLIENT_KEY_EXCHANGE:
                                tr_debug(" TLS Client KEY Exchange RX");
                                if (tls_heap->tls_chipher_mode == CHIPHER_PSK) {
                                    uint8_t *d_ptr = tls_msg_ptr->msg_ptr;
                                    uint16_t key_len;
                                    uint16_t received_key_id = 0;

                                    key_len = common_read_16_bit(d_ptr);
                                    d_ptr += 2;

                                    if (key_len) {
                                        if (key_len == 2) {
                                            received_key_id = (*d_ptr++) << 8;
                                        }
                                        received_key_id += *d_ptr;
                                    }

                                    if (tls_get_key(received_key_id) != NULL) {
                                        tls_suite->psk_key_id = received_key_id;

                                        if (tls_suite->state != PRF_CALC) {
                                            tr_debug("Client KEY Exchange ");
                                            tls_handshake_copy(tls_msg_ptr, tls_heap);
                                            algo_ok = 0;
                                        } else {
                                            tr_debug("Client Re TX");
                                        }
                                    }

                                    /* Not valid Key ID */
                                    else {
                                        tr_debug("Server drop Client Key Exchange");
                                        sec_lib_state_machine_trig(tls_suite, TLS_ALERT_DECRYPT);
                                        return buffer_free(buf);
                                    }
                                } else {
#ifdef ECC
                                    if (tls_parse_client_key_exchange(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {
                                        algo_ok = 0;
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);
                                        sec_set_auth_timeout(tls_suite, TLS_CLIENT_KEY_EXCHANGE_RX);
                                    } else
#endif
                                    {
                                        tr_debug("Server drop Client Key EX");
                                        sec_lib_state_machine_trig(tls_suite, TLS_ALERT_DECRYPT);
                                        return buffer_free(buf);
                                    }
                                }
                                break;

                            case TLS_CERTIFICATE_VERIFY:
                                tr_debug(" TLS Cert Verify ");
#ifdef ECC
                                if (tls_parse_certificate_verify(tls_msg_ptr->msg_ptr, tls_msg_ptr->len, tls_suite)) {

                                    if ((tls_suite->setups & TLS_ECC_CERTIFICATE_VERIFY) == 0) {
                                        tr_debug("Calc Hash for Cert Verify");
                                        tls_suite->setups |= TLS_ECC_CERTIFICATE_VERIFY;
                                        tls_hanshake_hash_cal(tls_heap);
                                        tls_handshake_copy(tls_msg_ptr, tls_heap);
                                        certi_rx |= 2;
                                    }
                                } else
#endif
                                {
                                    tr_debug("Client Cert Verfify fail");
                                    sec_lib_state_machine_trig(tls_suite, TLS_ALERT_DECRYPT);
                                    return buffer_free(buf);
                                }
                                break;
                            default:
                                break;
                        }
                        ptr = tls_msg_ptr->msg_ptr;
                        ptr += tls_msg_ptr->len;
                        j++;
                    }
                }

            } else if (tls_header_ptr->type == TLS_CHANGE_CIPHER_SPEC && (tls_heap != 0) /*&& (tls_header[i].length == 1)*/) {
                uint8_t tx_alert = 1;
                tr_debug("TLS:Change ChipherS");
                tr_debug("%02x", tls_suite->state);
                if (tls_suite->state == TLS_CLIENT_KEY_EXCHANGE_RX || tls_suite->state == TLS_SERVER_WAIT_CHANGE_CHIPHERSUITE) {
                    tx_alert = 0;
                }

                if (tx_alert) {
                    tr_debug("Wrong state TX Alert");
                    sec_lib_state_machine_trig(tls_suite, TLS_ALERT_CLOSE_FATAL);
                    return buffer_free(buf);
                } else {
                    sec_set_auth_timeout(tls_suite, TLS_CHANGE_CHIPHER);
                    algo_ok |= 8;
                }

            } else if (tls_header_ptr->type == TLS_ALERT_TYPE) {
                tr_debug("Alert!!");
                if (tls_header_ptr->length == 2) {
                    uint8_t *dptr = tls_header_ptr->ptr;
                    tr_debug("%s", tr_array(tls_header_ptr->ptr, 2));
                    //Skip Alert Type and descriptions
                    dptr += 2;
                    algo_ok = 0xf0;
                    sec_lib_state_machine_trig(tls_suite, PANA_FAILURE);
                }
                i = 0xfe;
            }
        }
    }
    if (algo_ok) {
        if (tls_suite->state == TLS_CHANGE_CHIPHER) {
            tr_debug("No Chipher RX--TXalert");
            sec_lib_state_machine_trig(tls_suite, TLS_ALERT_INTERNAL);
            i = 0xff;
        } else if (algo_ok == 0x18) {

        } else {
            i = 11;
            tr_debug("%02x", algo_ok);
        }
    }

    if (i) {
        buf = tls_certificate_buffer_store(buf, certi_rx, tls_suite);
    }
#else
    if (buf) {
        buf = buffer_free(buf);
    }
#endif
    return (buf);
}


buffer_t *tls_client_hello_build(buffer_t *buf, sec_suite_t *tls_suite)
{
    uint8_t *ptr;
    tls_msg_t *tmp_msg = tls_msg_ptr_get();
    tls_heap_t *tls_heap = tls_suite->tls_session->tls_heap;
    tr_debug("TTLs TX:Client Hello");
    //Build Client Hello
    ptr = buffer_data_clear(buf);
    //for Hash calculation

    *ptr++ = TLS_CLIENT_HELLO;
    ptr += 3; // skip length, fill in at the end
    tmp_msg->msg_ptr = ptr;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    memcpy(ptr, (tls_heap->tls_hello_random + CLIENT_HELLO_PTR), 32);
    ptr += 32;
    *ptr++ = 0; //Session ID length
    ptr += 2; // Skip cipher suite length; fill in a moment
    uint8_t *ciphers_ptr = ptr;
#ifdef ECC
    if (tls_suite->supported_chipher_suites & SEC_CIPHERSUITE_ECC) {
        ptr = common_write_16_bit(TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8, ptr);
        ptr = common_write_16_bit(TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8_COMPAT, ptr);
    }
#endif
    if (tls_suite->supported_chipher_suites & SEC_CIPHERSUITE_PSK) {
        ptr = common_write_16_bit(TLS_PSK_WITH_AES_128_CCM_8, ptr);
    }
    // Go back and fill in length
    common_write_16_bit(ptr - ciphers_ptr, ciphers_ptr - 2);

    *ptr++ = 1; // 1 Compression method
    *ptr++ = TLS_COMPRESSION_METHOD_NULL;
#ifdef ECC

    //Set ECC Extensions
    if (tls_suite->supported_chipher_suites & SEC_CIPHERSUITE_ECC) {
        ptr = common_write_16_bit(8 + 8 + 6, ptr); //Extensions length

        ptr = common_write_16_bit(TLS_EXTENSION_SIGNATURE_ALGORITHMS, ptr);
        ptr = common_write_16_bit(4, ptr); //extension data len
        ptr = common_write_16_bit(2, ptr); //algorithm list len
        ptr = common_write_16_bit(TLS_SIG_HASH_ALG_SHA256_ECDSA, ptr);

        ptr = common_write_16_bit(TLS_EXTENSION_ELLIPTIC_CURVES, ptr);
        ptr = common_write_16_bit(4, ptr); //extension data len
        ptr = common_write_16_bit(2, ptr); //curve list len
        ptr = common_write_16_bit(TLS_NAMED_CURVE_SECP256R1, ptr);

        ptr = common_write_16_bit(TLS_EXTENSION_EC_POINT_FORMATS, ptr);
        ptr = common_write_16_bit(2, ptr); //extension data len
        *ptr++ = 1; // format list len
        *ptr++ = 0; //Uncompressed mode
    }
#endif
    buffer_data_end_set(buf, ptr);

    // Go back and write the length
    uint16_t tls_len = buffer_data_length(buf) - 4;
    tmp_msg->len = tls_len;
    common_write_24_bit(tls_len, buffer_data_pointer(buf) + 1);

    tls_heap->tls_handshake_h_len = 0;
    tls_handshake_copy(tmp_msg, tls_heap);
    tls_suite->setups |= TLS_HANSHAKE_HASH;
    return tls_down(buf);

}

#ifdef ECC
uint8_t *tls_server_key_excahnge_msg_build(uint8_t *ptr, tls_heap_t *heap_ptr)
{
    uint8_t r_zeros;
    uint8_t s_zeros;

    r_zeros = tls_get_leading_zeros(heap_ptr->ecc_heap->sgnt->m_R.data);
    s_zeros = tls_get_leading_zeros(heap_ptr->ecc_heap->sgnt->m_s.data);
    *ptr++ = TLS_SERVER_KEY_EXCHANGE;
    ptr = common_write_24_bit((145 - r_zeros - s_zeros), ptr);
    *ptr++ = 3; //type
    ptr = common_write_16_bit(TLS_NAMED_CURVE_SECP256R1, ptr);
    //There shuold be calculated case now only test purpose
    *ptr++ = 65; //len
    *ptr++ = 4; //type
    memcpy(ptr, heap_ptr->ecc_heap->server_public_key, 64);
    ptr += 64;
    ptr = common_write_16_bit(TLS_SIG_HASH_ALG_SHA256_ECDSA, ptr);
    ptr = common_write_16_bit((72 - r_zeros - s_zeros), ptr);
    *ptr++ = 0x30;
    *ptr++ = 70 - r_zeros - s_zeros;
    ptr += tls_write_signature_parameters(ptr, ((uint8_t *)heap_ptr->ecc_heap->sgnt->m_R.data), r_zeros);
    ptr += tls_write_signature_parameters(ptr, ((uint8_t *)heap_ptr->ecc_heap->sgnt->m_s.data), s_zeros);
    return ptr;
}


uint8_t *tls_certificate_verify_msg_set(uint8_t *ptr, tls_heap_t *heap_ptr)
{
    uint8_t r_zeros;
    uint8_t s_zeros;
    r_zeros = tls_get_leading_zeros(heap_ptr->ecc_heap->sgnt->m_R.data);
    s_zeros = tls_get_leading_zeros(heap_ptr->ecc_heap->sgnt->m_s.data);

    *ptr++ = TLS_CERTIFICATE_VERIFY;
    ptr = common_write_24_bit((76 - r_zeros - s_zeros), ptr);
    *ptr++ = 4;
    *ptr++ = 3;
    *ptr++ = 0;
    *ptr++ = 72 - r_zeros - s_zeros;
    *ptr++ = 0x30;
    *ptr++ = 70 - r_zeros - s_zeros;
    ptr += tls_write_signature_parameters(ptr, ((uint8_t *)heap_ptr->ecc_heap->sgnt->m_R.data), r_zeros);
    ptr += tls_write_signature_parameters(ptr, ((uint8_t *)heap_ptr->ecc_heap->sgnt->m_s.data), s_zeros);
    return ptr;
}
#define CERTI_CHAIN_3

uint16_t tls_certificate_len(certificate_chain_internal_t   *temp)
{
    uint8_t i;
    uint16_t len = 0;

    if (temp) {
        len += 10;
        if (temp->chain_length > 1) {
            for (i = 0; i < (temp->chain_length - 1); i++) {
                if (i) {
                    len += 3;
                }
                len += temp->certi_len[i + 1];
            }
        } else if (temp->chain_length == 1) {
            len += temp->certi_len[0];
        }
    }
    return len;
}

uint8_t *tls_certificate_msg_set(uint8_t *ptr, certificate_chain_internal_t *temp)
{
    uint8_t i;
    uint16_t len = 0;
    len = tls_certificate_len(temp);
    if (temp) {
        len -= 4; // Cut TLS haeder space off
        /* Set Certificate */
        *ptr++ = TLS_CERTIFICATE;
        ptr = common_write_24_bit(len, ptr);
        len -= 3;
        ptr = common_write_24_bit(len, ptr);

        i = temp->chain_length;
        if (i > 1) {
            while (i) {
                i--;
                if (i) {
                    ptr = common_write_24_bit(temp->certi_len[i], ptr);
                    memcpy(ptr, temp->certi_chain[i], temp->certi_len[i]);
                    ptr += temp->certi_len[i];
                }
            }
        } else if (i == 1) {
            ptr = common_write_24_bit(temp->certi_len[0], ptr);
            memcpy(ptr, temp->certi_chain[0], temp->certi_len[0]);
            ptr += temp->certi_len[0];
        }
    }
    return ptr;
}

uint8_t tls_write_signature_parameters(uint8_t *ptr, uint8_t *signature_parameter, uint8_t leadin_zeros)
{
    uint8_t len = 2;
    uint8_t i;
    *ptr++ = 2;
    *ptr++ = 33 - leadin_zeros;
    if (leadin_zeros == 0) {
        *ptr++ = 0;
        len += 33;
    } else {
        leadin_zeros -= 1;
        len += (32 - leadin_zeros);
    }
    for (i = leadin_zeros; i < 32; i++) {
        *ptr++ = signature_parameter[31 - i];
    }
    return len;
}

void tls_ecc_point_reverse_order(uint8_t *dst, uint8_t *src)
{
    uint8_t i;
    for (i = 0; i < 32; i++) {
        dst[i] = src[31 - i];
    }
}
#endif

uint8_t *tls_build_server_hello_msg(uint8_t *ptr, tls_session_t *tls_session)
{
    tls_heap_t *heap_ptr = tls_session->tls_heap;
    *ptr++ = TLS_SERVER_HELLO;
    ptr += 3; // fill length in at the end
    uint8_t *startptr = ptr;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);

    memcpy(ptr, (heap_ptr->tls_hello_random + SERVER_HELLO_PTR), 32);
    ptr += 32;

    *ptr++ = tls_session->id_length;
    memcpy(ptr, tls_session->tls_session_id, tls_session->id_length);
    ptr += tls_session->id_length;

    uint16_t ciphersuite;
#ifdef ECC
    if (heap_ptr->tls_chipher_mode == CHIPHER_ECC) {
        ciphersuite = heap_ptr->client_knows_standard_ecc_ciphersuite ?
                      TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8 :
                      TLS_ECDHE_ECDSA_WITH_AES_128_CCM_8_COMPAT;
    } else
#endif
    {
        ciphersuite = TLS_PSK_WITH_AES_128_CCM_8;
    }

    ptr = common_write_16_bit(ciphersuite, ptr);
    *ptr++ = TLS_COMPRESSION_METHOD_NULL;

    if (heap_ptr->tls_chipher_mode == CHIPHER_ECC) {
        ptr = common_write_16_bit(6, ptr); // 6 bytes of Extensions
        ptr = common_write_16_bit(TLS_EXTENSION_EC_POINT_FORMATS, ptr);
        ptr = common_write_16_bit(2, ptr); // 2 bytes of extension data
        *ptr++ = 1; // 1 byte of ec_point_format_list
        *ptr++ = 0; // ECPointFormat::uncompressed
    }

    // Go back and fill in length
    common_write_24_bit(ptr - startptr, startptr - 3);

    if (heap_ptr->tls_chipher_mode != CHIPHER_ECC) {
        *ptr++ = TLS_SERVER_HELLO_DONE;
        ptr = common_write_24_bit(0, ptr); // length
    }
    return ptr;
}

void tls_header_set(buffer_t *buf)
{
    uint8_t *ptr;
    uint16_t len;
    ptr = buffer_data_pointer(buf);
    len = buffer_data_length(buf);
    len -= 5; //Cut Flags byte off
    *ptr++ = TLS_HANDSHAKE;
    ptr = common_write_16_bit(TLS_1_2_VERSION, ptr);
    ptr = common_write_16_bit(len, ptr);
}

void tls_session_id_genrate(uint8_t *suite, uint8_t length)
{
    if (length > 32) {
        length = 32;
    }
    randLIB_get_n_bytes_random(suite, length);
}

tls_heap_t *tls_heap_allocate(void)
{
    tls_heap_t *heap_ptr = tls_heap_structure_allocate();
    if (heap_ptr) {
#ifdef ECC
        tls_ecc_heap_t *ecc_ptr = ecc_allocate_ram();
        if (ecc_ptr == 0) {
            tr_debug("ECC Alloc Fail");
            ns_dyn_mem_free(heap_ptr);
            heap_ptr = 0;
            return heap_ptr;
        } else {
            //uint8_t *ptr, i;
            heap_ptr->ecc_heap = ecc_ptr;
        }
        heap_ptr->client_knows_standard_ecc_ciphersuite = false;
#endif
        randLIB_get_n_bytes_random(heap_ptr->tls_hello_random, 64);
        ns_sha256_init(&heap_ptr->sha256_ctx);
        heap_ptr->tls_chipher_mode = CHIPHER_NONE;
        heap_ptr->tls_handshake_h_len = 0;
        heap_ptr->client_verify_buf = 0;
        heap_ptr->client_verify_buf_len = 0;
    }
    return heap_ptr;
}

void tls_heap_free(tls_heap_t *heap_ptr)
{
    if (heap_ptr) {
        if (heap_ptr->cert_temp_buf) {
            if (heap_ptr->signature_temp_buf) {
                if (heap_ptr->signature_temp_buf == heap_ptr->cert_temp_buf) {
                    heap_ptr->signature_temp_buf = 0;
                }


            }

            if (heap_ptr->pointer_types & 1) {
                buffer_free(heap_ptr->cert_temp_buf);
            } else {
                ns_dyn_mem_free(heap_ptr->cert_temp_buf);
            }
            heap_ptr->cert_temp_buf = 0;
        }
        if (heap_ptr->signature_temp_buf) {
            if (heap_ptr->pointer_types & 2) {
                buffer_free(heap_ptr->signature_temp_buf);
            } else {
                ns_dyn_mem_free(heap_ptr->signature_temp_buf);
            }
            heap_ptr->signature_temp_buf = 0;
        }
        if (heap_ptr->client_verify_buf) {
            ns_dyn_mem_free(heap_ptr->client_verify_buf);
            heap_ptr->client_verify_buf = 0;
        }
        tls_ecc_heap_free(heap_ptr);
        ns_sha256_free(&heap_ptr->sha256_ctx);
        ns_dyn_mem_free(heap_ptr);
    }
}
#ifdef ECC
void tls_ecc_heap_free(tls_heap_t *heap_ptr)
{
    if (heap_ptr) {
        if (heap_ptr->ecc_heap) {
            if (heap_ptr->ecc_heap->sgnt) {
                ecc_library_free_pointer(heap_ptr->ecc_heap->sgnt);
                //ns_dyn_mem_free(heap_ptr->ecc_heap->sgnt);
            }
            ns_dyn_mem_free(heap_ptr->ecc_heap);
            heap_ptr->ecc_heap = 0;
        }
    }
}
#endif

#ifdef ECC
uint8_t tls_get_leading_zeros(void *data)
{

    uint8_t *ptr = (uint8_t *)data + 31;
    uint8_t i = 0;

    for (;; ptr--) {
        if (*ptr & 0x80) {
            break;
        }
        i++;
        if (i == 33) {
            break;
        }

        if (*ptr) {
            break;
        }
    }

    return i;

}
#endif


void tls_key_expansion_cal(tls_heap_t *heap_ptr, uint8_t *key_save_ptr, uint8_t *master_secret)
{
    uint8_t *ptr;
    prf_sec_param_t *prf_ptr = shalib_prf_param_get();
    prf_ptr->secret = master_secret;
    prf_ptr->sec_len = 48;
    prf_ptr->label = "key expansion";
    prf_ptr->seed = heap_ptr->temp_buf;
    ptr = heap_ptr->temp_buf;
    memcpy(ptr, (heap_ptr->tls_hello_random + SERVER_HELLO_PTR), 32);
    ptr += 32;
    memcpy(ptr, (heap_ptr->tls_hello_random + CLIENT_HELLO_PTR), 32);
    prf_ptr->seedlen = 64;
    shalib_prf_calc(key_save_ptr, 10); // Why 40 bytes?
}

void tls_master_key_cal(tls_heap_t *heap_ptr, sec_suite_t *tls_suite)
{
    uint8_t *ptr;
    prf_sec_param_t *prf_ptr = shalib_prf_param_get();
    uint8_t secret_buf[2 + 16 + 2 + 16];
    tr_debug("CAL Master secret:");
    //Her have to to be set check is
#ifdef ECC
    if (heap_ptr->tls_chipher_mode == CHIPHER_ECC) {
        prf_ptr->secret = heap_ptr->ecc_heap->pre_secret_mat;
        prf_ptr->sec_len = 32;
    } else
#endif
    {
        tls_psk_key_t *key_temp = tls_get_key(tls_suite->psk_key_id);
        if (key_temp) {
            /* RFC 4279 - premaster secret for N-octet PSK is
             *  +----------+----------+----------+----------+
             *  | 2 octets | N octets | 2 octets | N octets |
             *  +----------+----------+----------+----------+
             *  |    N     |    0     |    N     |   PSK    |
             *  +----------+----------+----------+----------+
             */
            ptr = secret_buf;
            ptr = common_write_16_bit(16, ptr);
            memset(ptr, 0, 16), ptr += 16;
            ptr = common_write_16_bit(16, ptr);
            memcpy(ptr, key_temp->psk_key, 16);
            prf_ptr->secret = secret_buf;
            prf_ptr->sec_len = sizeof secret_buf;
        }
    }
    prf_ptr->label = "master secret";
    prf_ptr->seed = heap_ptr->temp_buf;
    ptr = heap_ptr->temp_buf;
    memcpy(ptr, (heap_ptr->tls_hello_random + CLIENT_HELLO_PTR), 32);
    ptr += 32;
    memcpy(ptr, (heap_ptr->tls_hello_random + SERVER_HELLO_PTR), 32);
    prf_ptr->seedlen = 64;

    shalib_prf_calc(tls_suite->tls_session->master_secret, 12);
}

void tls_verify_calc(uint8_t output[12], uint8_t server, tls_heap_t *heap_ptr, uint8_t *master_secret)
{
    prf_sec_param_t *prf_ptr = shalib_prf_param_get();
    prf_ptr->secret = master_secret;
    prf_ptr->sec_len = 48;
    prf_ptr->label = server ? "server finished" : "client finished";
    prf_ptr->seed = heap_ptr->hash_buf;
    prf_ptr->seedlen = 32;
    shalib_prf_calc(output, 3);
}


void tls_handshake_copy(tls_msg_t *tls_msg_ptr, tls_heap_t *heap_ptr)
{
    uint16_t len = tls_msg_ptr->len;
    uint8_t *ptr = tls_msg_ptr->msg_ptr;
    ptr -= 4;
    len += 4;

    if (heap_ptr->tls_handshake_h_len == 0) {
        ns_sha256_starts(&heap_ptr->sha256_ctx);
    }
    ns_sha256_update(&heap_ptr->sha256_ctx, ptr, len);
    heap_ptr->tls_handshake_h_len += len;
}


void tls_hanshake_hash_cal(tls_heap_t *heap_ptr)
{
    tr_debug("HASH Calc");
    ns_sha256_context ctx;
    ns_sha256_clone(&ctx, &heap_ptr->sha256_ctx);
    ns_sha256_finish(&ctx, heap_ptr->hash_buf);
    ns_sha256_free(&ctx);
}

#if 0
uint8_t tls_txt_analyze(buffer_t *buf)
{
    uint8_t *dptr;
    uint8_t tls_text_cnt = 0;
    uint16_t data_len = buffer_data_length(buf);
    uint16_t readed = 0;
    dptr = buffer_data_pointer(buf);

    while (readed  < data_len) {
        uint16_t tls_version;
        tls_header.type = *dptr++;
        tls_version = common_read_16_bit(dptr);
        dptr += 2;
        if (tls_version == TLS_1_2_VERSION) {
            tls_text_cnt = 0;
            break;
        } else {
            uint16_t tls_temp_var_16;
            tls_temp_var_16 = common_read_16_bit(dptr);
            dptr += 2;
            if (tls_temp_var_16) {
                tls_header.length = tls_temp_var_16;
                tls_header.ptr = dptr;

                readed += 5;
                readed += tls_temp_var_16;

                if (readed > data_len) {
                    tr_debug("Over Read1: %02x", tls_header.type);
                    tr_debug("LEN: %04x", data_len);
                    tr_debug("Read: %04x", readed);
                    tls_text_cnt = 0;
                    break;
                } else {
                    dptr += tls_temp_var_16;
                    tls_text_cnt++;
                }
            } else {
                tls_text_cnt = 0;
                break;;
            }
        }
    }

    if (readed != data_len) {
        tr_debug("TSL txt Parse Fail");
        tls_text_cnt = 0;
    }
    return tls_text_cnt;
}
#endif

#if 0
tls_header_t *tls_message_get(uint8_t *dptr)
{
    uint16_t tls_temp_var_16;
    tls_header.type = *dptr;
    dptr += 3;
    tls_temp_var_16 = common_read_16_bit(dptr);
    dptr += 2;
    tls_header.length = tls_temp_var_16;
    tls_header.ptr = dptr;
    return &tls_header;
}
#endif

uint8_t tls_msg_analyzy(uint8_t *ptr, uint16_t data_len)
{
    uint8_t *dptr;
    uint8_t msg_cnt = 0;
    uint16_t readed = 0, m_len = 0;
    dptr = ptr;
    while (readed  < data_len) {
        dptr += 1; //Skip Type
        if (*dptr++) {
            //Too long skip packet
            tr_debug("Too long: %s", tr_array(ptr, data_len));
            msg_cnt = 0;
            break;
        } else {
            m_len = common_read_16_bit(dptr);
            dptr += 2;
        }
        readed += 4;
        readed += m_len;
        dptr += m_len;

        if (readed > data_len) {
            tr_debug("Over Read1: %02x", tls_msg.type);
            tr_debug("LEN: %04x", data_len);
            tr_debug("Read: %04x", readed);
            msg_cnt = 0;
            break;
        } else {
            msg_cnt++;
        }
    }
    if (readed != data_len) {
        tr_debug("TLS MSG Parse Fail");
        msg_cnt = 0;
    }
    return msg_cnt;
}

tls_msg_t *tls_msg_get(uint8_t *dptr)
{
    tls_msg.type = *dptr;
    dptr += 2;
    tls_msg.len = common_read_16_bit(dptr);
    dptr += 2;
    tls_msg.msg_ptr = dptr;
    return &tls_msg;
}

tls_msg_t *tls_msg_ptr_get(void)
{
    return &tls_msg;
}

void tls_build_client_verify_payload(tls_heap_t *tls_heap)
{
    uint8_t *ptr;
    ptr = tls_heap->hash_buf;
    ptr = common_write_32_bit(0x1400000c, ptr);
    memcpy(ptr, tls_heap->verify, 12);
}

void tls_nonce_update(uint8_t *ptr)
{
    uint8_t i = 8;
    ptr += 7;

    while (i--) {
        if (*ptr == 0xff) {
            *ptr = 0;
            ptr--;
        } else {
            *ptr += 1;
            break;
        }
    }
}
#ifdef ECC
uint8_t tls_parse_certificate_request(uint8_t *ptr, uint16_t len)
{
    (void)len;
    uint16_t hash_alg;
    uint16_t count;
    uint8_t found = 0;
    count = *ptr++;

    if (!count) {
        tr_debug("cert req 0 count 1");
        return 0;
    }

    while (count--) {

        if (*ptr++ == TLS_CERT_TYPE_ECDSA) {

            found = TLS_CERT_TYPE_ECDSA;

        }

    }

    if (!found) {
        tr_debug("cert req no supported type");
        return 0;
    }
    found = 0;

    count = (uint16_t) * ptr++ << 8;
    count |= *ptr++;
    count >>= 1;

    if (!count) {
        tr_debug("cert req 0 count 2");
        return 0;
    }


    while (count--) {

        hash_alg = common_read_16_bit(ptr);
        ptr += 2;
        if (hash_alg == TLS_SIG_HASH_ALG_SHA256_ECDSA) {
            found = 1;
        }

    }

    if (!found) {

        tr_debug("cert req no supported Algo");
        return 0;
    }

    return 1;
    /* There is also names field but we do not have parser for it yet */

}


static uint8_t ecc_verify_calculate_hash(sec_suite_t *tls_suite)
{
    tls_heap_t *theap = tls_suite->tls_session->tls_heap;
    if (!(tls_suite->setups & TLS_ECC_CERTIFICATE_RECEIVED)) {
        return 1;
    } else {
        tls_suite->setups &= ~TLS_ECC_CERTIFICATE_RECEIVED;
    }

    if (tls_suite->setups & TLS_SERVER_MODE) {
        if (!(tls_suite->setups & TLS_ECC_CERTIFICATE_VERIFY)) {
            return 2;
        } else {
            tls_suite->setups &= ~TLS_ECC_CERTIFICATE_VERIFY;
        }
    }
    if (!theap->ecc_heap->sgnt) {
        return 3;
    }

    tls_ecc_server_key_signature_hash(theap);
    return 0;

}


void tls_ecc_reverse_hash(uint8_t *ptr)
{
    uint8_t start = 0;
    uint8_t end = 31;
    uint8_t chr;
    while (start < end) {
        chr = ptr[start];
        ptr[start] = ptr[end];
        ptr[end] = chr;

        start++;
        end--;
    }
}

void tls_ecc_server_key_signature_hash(tls_heap_t *heap_ptr)
{
    memset(heap_ptr->ecc_heap->sgnt->m_m.data, 0, MPINT_DATA_SIZE);
    ns_sha256_context ctx;
    ns_sha256_init(&ctx);
    ns_sha256_starts(&ctx);
    ns_sha256_update(&ctx, heap_ptr->tls_hello_random, 64);
    //ServerECDHParams(ECParameters+ECPoint)
    heap_ptr->hash_buf[0] = TLS_EC_CURVE_TYPE_NAMED_CURVE;
    common_write_16_bit(TLS_NAMED_CURVE_SECP256R1, heap_ptr->hash_buf + 1);
    heap_ptr->hash_buf[3] = 65; // ECPoint public - length indicator for following 512-bit public key?
    heap_ptr->hash_buf[4] = 4;
    ns_sha256_update(&ctx, heap_ptr->hash_buf, 5);
    ns_sha256_update(&ctx, heap_ptr->ecc_heap->server_public_key, 64);
    ns_sha256_finish(&ctx, heap_ptr->ecc_heap->sgnt->m_m.data);
    ns_sha256_free(&ctx);
    tls_ecc_reverse_hash((uint8_t *)heap_ptr->ecc_heap->sgnt->m_m.data);
}

#endif

#else
int8_t arm_tls_add_psk_key(const uint8_t *key_ptr, uint16_t key_id)
{
    (void)key_ptr;
    (void)key_id;
    return -1;
}
#endif



