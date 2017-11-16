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
#ifdef PANA
#include "string.h"
#include "ccmLIB.h"
#include "Core/include/address.h"
#include "Core/include/ns_buffer.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "randLIB.h"
#include "Core/include/socket.h"
//#include "6LoWPAN/Bootstraps/network_lib.h"
#include "shalib.h"
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "nsdynmemLIB.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "net_interface.h"

#define TLS_1_2_VERSION 0x0303
#define ECC_CHIPHERSUITE 0xc0c6
#define PSK_CHIPHERSUITE 0xc0a8

const uint8_t SERVER_FIN_STRING[15] = {'s', 'e', 'r', 'v', 'e', 'r', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd'};
const uint8_t CLIENT_FIN_STRING[15] = {'c', 'l', 'i', 'e', 'n', 't', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd'};
const uint8_t KEY_EXT_STRING[13] = {'k', 'e', 'y', ' ', 'e', 'x', 'p', 'a', 'n', 's', 'i', 'o', 'n'};
const uint8_t MASTER_SEC_STRING[13] = {'m', 'a', 's', 't', 'e', 'r', ' ', 's', 'e', 'c', 'r', 'e', 't'};

typedef struct tls_psk_key_ {
    uint16_t key_id;
    uint8_t psk_key[16];
    ns_list_link_t link;
} tls_psk_key_t;

static NS_LIST_DEFINE(tls_psk_list, tls_psk_key_t, link);

NS_LARGE tls_header_t tls_header;
NS_LARGE tls_msg_t tls_msg;
NS_LARGE uint8_t tls_auth_data_general[13];

#ifdef ECC

tls_ecc_heap_t *ecc_allocate_ram(void);
void tls_ecc_reverse_hash(uint8_t *ptr);
uint8_t tls_write_signature_parameters(uint8_t *ptr, uint8_t *signature_parameter, uint8_t leadin_zeros);
uint8_t tls_parse_certificate_request(uint8_t *ptr, uint16_t len);
uint8_t tls_parse_client_key_exchange(uint8_t *ptr, uint16_t len);

#endif

tls_session_t *amr_tls_session_allocate(void)
{
    return NULL;
}

void arm_tls_session_clear(tls_session_t *t_session)
{
}

int8_t arm_tls_add_psk_key(const uint8_t *key_ptr, uint16_t key_id)
{
    return 0;
}

int8_t arm_tls_check_key(uint16_t key_id)
{
    return 0;
}

int8_t arm_tls_remove_psk_key(uint16_t key_id)
{
    return 0;
}

void tls_finnish_copy(uint8_t *ptr, tls_heap_t *heap_ptr)
{
}


void tls_alert_build(buffer_t *buf, uint8_t alert)
{
}

#ifdef PANA_SERVER_API
void tls_server_hello_build(buffer_t *buf,  sec_suite_t *tls_suite)
{
}
#endif

void tls_prepare_change_chipher_spec(sec_suite_t *tls_suite)
{
}

#ifdef ECC

void tls_parse_subject_get_pub_key_from_chain(tls_heap_t *theap, uint8_t rd_ptr)
{

}

uint8_t tls_parse_client_key_exchange(uint8_t *ptr, uint16_t len)
{
    return 0;
}

void tls_read_certi_signature(tls_heap_t *theap, uint8_t certificate)
{
}

tls_ecc_heap_t *ecc_allocate_ram(void)
{
    return NULL;
}

uint8_t *tls_client_key_exchange_msg_set(uint8_t *ptr , tls_heap_t *heap_ptr)
{
    return NULL;
}
#endif

uint8_t *tls_build_change_chipher_suite_finnish_msg(uint8_t *ptr, tls_session_t *tls_session)
{
    return NULL;
}

void tls_build_client_change_chipher_suite_finnish(buffer_t *buf, sec_suite_t *tls_suite)
{
}

#ifdef ECC
void tls_ecc_verfify_start(sec_suite_t *tls_suite)
{
}


void tls_certificate_signature_verify(sec_suite_t *tls_suite)
{
}

void tls_server_finnish_handle_start(sec_suite_t *tls_suite)
{
}
#endif

#ifdef ECC
extern void sec_ecc_state_free(sec_suite_t *suite);
#endif

buffer_t *tls_client_up(buffer_t *buf, sec_suite_t *tls_suite)
{
    return NULL;
}

buffer_t *tls_server_up(buffer_t *buf, sec_suite_t *tls_suite)
{
    return NULL;
}

buffer_t * tls_client_hello_build(buffer_t *buf, sec_suite_t *tls_suite)
{
    return buf;
}
#ifdef ECC

uint8_t *tls_server_key_excahnge_msg_build(uint8_t *ptr , tls_heap_t *heap_ptr)
{
    return NULL;
}


uint8_t *tls_certificate_verify_msg_set(uint8_t *ptr, tls_heap_t *heap_ptr)
{
    return NULL;
}

uint16_t tls_certificate_len(certificate_chain_internal_t   *temp)
{
    return 0;
}

uint8_t *tls_certificate_msg_set(uint8_t *ptr, certificate_chain_internal_t *temp)
{
    return NULL;
}

uint8_t tls_write_signature_parameters(uint8_t *ptr, uint8_t *signature_parameter, uint8_t leadin_zeros)
{
    return 0;
}

void tls_ecc_point_reverse_order(uint8_t *dst, uint8_t *src)
{
}
#endif

uint8_t *tls_build_server_hello_msg(uint8_t *ptr, tls_session_t *tls_session)
{
    return NULL;
}

void tls_header_set(buffer_t *buf)
{
}

void tls_session_id_genrate(uint8_t *suite, uint8_t length)
{
}

tls_heap_t *tls_heap_allocate(void)
{
    return NULL;
}

void tls_heap_free(tls_heap_t *heap_ptr)
{
}
#ifdef ECC
void tls_ecc_heap_free(tls_heap_t *heap_ptr)
{
}
#endif

#ifdef ECC
uint8_t tls_get_leading_zeros(void *data)
{
    return 0;
}
#endif


void tls_key_expansion_cal(tls_heap_t *heap_ptr, uint8_t *key_save_ptr, uint8_t *master_secret)
{
}

void tls_master_key_cal(tls_heap_t *heap_ptr, sec_suite_t *tls_suite)
{
}

void tls_verify_calc(uint8_t output[12], uint8_t server, tls_heap_t *heap_ptr, uint8_t *master_secret)
{
}

void tls_handshake_copy(tls_msg_t *tls_msg_ptr, tls_heap_t *heap_ptr)
{
}

void tls_hanshake_hash_cal(tls_heap_t *heap_ptr)
{
}

uint8_t tls_msg_analyzy(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

tls_msg_t *tls_msg_get(uint8_t *dptr)
{
    return NULL;
}

tls_msg_t *tls_msg_ptr_get(void)
{
    return NULL;
}

void tls_build_client_verify_payload(tls_heap_t *tls_heap)
{
}

void tls_nonce_update(uint8_t *ptr)
{
}
#ifdef ECC
uint8_t tls_parse_certificate_request(uint8_t *ptr, uint16_t len)
{
    return 0;
}


void tls_ecc_reverse_hash(uint8_t *ptr)
{
}

void tls_ecc_server_key_signature_hash(tls_heap_t *heap_ptr)
{
}

#endif


#else
int8_t arm_tls_add_psk_key(const uint8_t *key_ptr, uint16_t key_id) {
    return -1;
}
#endif



