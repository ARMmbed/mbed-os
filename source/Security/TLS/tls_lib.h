/*
 * Copyright (c) 2013-2019, Pelion and affiliates.
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

#ifndef TLS_LIB_H_
#define TLS_LIB_H_

#include "Security/Common/sec_lib_definitions.h"

extern tls_session_t *amr_tls_session_allocate(void);
extern void arm_tls_session_clear(tls_session_t *t_session);

extern void tls_finnish_copy(uint8_t *ptr, tls_heap_t *heap_ptr);
extern void tls_alert_build(buffer_t *buf, uint8_t alert);

extern void tls_prepare_change_chipher_spec(sec_suite_t *tls_suite);
extern void tls_build_client_change_chipher_suite_finnish(buffer_t *buf, sec_suite_t *tls_suite);
#ifdef PANA_SERVER_API
extern void tls_server_hello_build(buffer_t *buf, sec_suite_t *tls_suite);
#endif

extern void tls_key_expansion_cal(tls_heap_t *heap_ptr, uint8_t *key_save_ptr, uint8_t *master_secret);
extern void tls_master_key_cal(tls_heap_t *heap_ptr, sec_suite_t *tls_suite);
extern void tls_verify_calc(uint8_t output[12], uint8_t server, tls_heap_t *heap_ptr, uint8_t *master_secret);
extern void tls_hanshake_hash_cal(tls_heap_t *heap_ptr);
extern void tls_handshake_copy(tls_msg_t *tls_msg_ptr, tls_heap_t *heap_ptr);

extern uint8_t tls_txt_analyze(buffer_t *buf);
extern tls_header_t *tls_message_get(uint8_t *dptr);
extern uint8_t tls_msg_analyzy(uint8_t *ptr, uint16_t data_len);
extern tls_msg_t *tls_msg_get(uint8_t *dptr);
extern tls_msg_t *tls_msg_ptr_get(void);

extern buffer_t *tls_client_up(buffer_t *buf, sec_suite_t *tls_suite);
extern buffer_t *tls_server_up(buffer_t *buf, sec_suite_t *tls_suite);

extern buffer_t *tls_client_hello_build(buffer_t *buf, sec_suite_t *tls_suite);
extern void tls_build_client_verify_payload(tls_heap_t *tls_heap);
extern uint8_t *tls_build_change_chipher_suite_finnish_msg(uint8_t *ptr, tls_session_t *tls_session);
extern uint8_t *tls_build_server_hello_msg(uint8_t *ptr, tls_session_t *tls_session);
extern void tls_session_id_genrate(uint8_t *suite, uint8_t length);

#ifdef ECC
extern uint8_t *tls_client_key_exchange_msg_set(uint8_t *ptr, tls_heap_t *heap_ptr);
extern void tls_ecc_point_reverse_order(uint8_t *dst, uint8_t *src);
extern uint8_t *tls_server_key_excahnge_msg_build(uint8_t *ptr, tls_heap_t *heap_ptr);
extern uint8_t *tls_certificate_verify_msg_set(uint8_t *ptr, tls_heap_t *heap_ptr);
extern uint8_t *tls_certificate_msg_set(uint8_t *ptr, certificate_chain_internal_t *temp);
extern void tls_ecc_heap_free(tls_heap_t *heap_ptr);
#else
#define tls_ecc_heap_free(heap_ptr) ((void) 0)
#endif

extern void tls_header_set(buffer_t *buf);
extern tls_heap_t *tls_heap_allocate(void);
extern void tls_heap_free(tls_heap_t *heap_ptr);

extern uint8_t tls_get_leading_zeros(void *data);

#ifdef ECC
extern void tls_read_certi_signature(tls_heap_t *theap, uint8_t certificate);
extern void tls_ecc_server_key_signature_hash(tls_heap_t *heap_ptr);
extern uint16_t tls_certificate_len(certificate_chain_internal_t   *temp);
extern void tls_ecc_verfify_start(sec_suite_t *tls_suite);
extern void tls_server_finnish_handle_start(sec_suite_t *tls_suite);
extern void tls_parse_subject_get_pub_key_from_chain(tls_heap_t *theap, uint8_t rd_ptr);
extern void tls_certificate_signature_verify(sec_suite_t *tls_suite);
#endif
//extern uint16_t tls_server_hello_req_len(tls_heap_t *heap_ptr);

extern void tls_nonce_update(uint8_t *ptr);
#endif /* TLS_LIB_H_ */
