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

#ifndef SEC_LIB_H_
#define SEC_LIB_H_
#include "Security/Common/sec_lib_definitions.h"
#ifdef PANA
extern int sec_pana_protocol_init(sec_suite_t *suite);
extern void sec_suite_tls_free(sec_suite_t *suite, bool free_session);
extern uint8_t sec_check_suite_ptrs(sec_suite_t *suite);

extern sec_suite_t * sec_lib_security_session_allocate(bool tls_allocate);
extern sec_suite_t *sec_suite_create(void);
extern sec_suite_t *sec_suite_selected_py_pan_id(uint16_t pan_id);
extern sec_suite_t *sec_suite_selected_pana_session(uint32_t session_id);
extern sec_suite_t *sec_suite_selected_address(const uint8_t address[static 16]);
extern int8_t sec_suite_remove(sec_suite_t *cur);
void sec_suite_list_clean(void);
extern sec_suite_t *sec_suite_verify(sec_suite_t *session);
extern uint8_t sec_suite_socket_event(uint8_t event_type, sec_suite_t *session_ptr);

extern uint16_t sec_pana_key_update_trig(uint16_t th_time);
/* TLS spesific use case */
extern void eap_fragmentation_init(sec_suite_t *suite);
#ifdef PANA_SERVER_API
uint8_t tls_server_certi_hash_copy(sec_suite_t *suite);
uint8_t tls_pana_server_exchange_build(sec_suite_t *suite);
#endif
extern void sec_prf_state_set(sec_suite_t *suite);
extern void sec_set_auth_timeout(sec_suite_t *suite, sec_state_machine_t cur_state);
extern void sec_lib_state_machine_trig(sec_suite_t *suite, sec_state_machine_t state);
extern void sec_lib_state_machine_lock(sec_suite_t *suite, sec_state_machine_t state);

extern uint8_t sec_auth_re_check(sec_suite_t *suite);

extern void sec_libray_init(void);
extern void sec_timer_handle(void);
extern void seclib_session_clean(sec_suite_t *suite);
#ifdef ECC
extern void ecc_operation_done_callback(int8_t status, void *result_ptr);
extern uint8_t tls_ecc_start_premaster_secret(EllipticPoint *ellicpt_ptr, sec_suite_t *suite);
extern void sec_ecc_state_save(sec_suite_t *suite);
extern void sec_ecc_sceduler(void);
extern void sec_ecc_gen_public_key_start(sec_suite_t *suite);
extern void sec_ecc_client_signature_start(sec_suite_t *suite);
extern uint8_t tls_certi_hash_copy(sec_suite_t *suite);
extern uint8_t tls_certificate_build(sec_suite_t *suite);
#else
#define sec_ecc_sceduler() ((void)0)
#define sec_ecc_gen_public_key_start(suite) ((void)0)
#define sec_ecc_client_signature_start(suite) ((void)0)
#define tls_certi_hash_copy(suite) 0
#define tls_certificate_build(suite) 0
#endif /* ECC */
#else
#define sec_timer_handle() ((void)0)
#define sec_ecc_sceduler() ((void)0)
#endif /* PANA */
#endif /* SEC_LIB_H_ */
