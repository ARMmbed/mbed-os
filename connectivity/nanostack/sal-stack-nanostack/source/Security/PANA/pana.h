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

#ifndef PANA_H_
#define PANA_H_
#include "Security/Common/sec_lib_definitions.h"
#include "Security/PANA/pana_header.h"
#include "net_pana_parameters_api.h"
#ifdef PANA

#define ZIGBEE_VENDOR_ID 0x0000917c
#define PANA_EAP_VENDOR_FLAG 0x8000

#define PANA_EAP_KEYWRAP_TYPE 0x0001
#define PANA_EAP_KEYREQ_TYPE 0x0002

typedef void pana_socket_packet_handler_cb(buffer_t *buf);
typedef void pana_state_machine_step(sec_suite_t *suite);
typedef buffer_t *pana_eap_tls_up_cb(buffer_t *buf, sec_suite_t *tls_suite);

extern pana_lib_parameters_s *pana_parameters_get(void);
extern pana_session_t *pana_session_allocate(void);
void pana_session_base_init(pana_session_t *p_session);
extern pana_heap_t *pana_heap_structure_allocate(void);
extern void pana_session_state_init(pana_session_t *p_session);
extern sec_suite_t *pana_client_init(auth_info_t *auth_ptr, uint8_t *session_address_ptr, pana_tls_setup_s *setup);

/** Pana Core functions */
void pana_down(buffer_t *buf, sec_suite_t *suite);

extern bool pana_socket_init(pana_socket_packet_handler_cb *socket_handler, pana_state_machine_step *state_machine_stepper, pana_eap_tls_up_cb *tls_handler_cb);
extern void pana_session_init_by_session_ptr(sec_suite_t *suite, auth_info_t *auth_ptr);

extern void pana_eap_down(buffer_t *buf, sec_suite_t *suite);
extern void eap_tls_payload_push(buffer_t *buf);
extern void pana_eap_payload_down(buffer_t *buf, const uint8_t *nonce, sec_suite_t *suite);

extern void pana_key_calculation(sec_suite_t *suite);

extern void pana_free_dynamic_ram(sec_suite_t *suite);
extern void pana_start_message_build(buffer_t *buf, sec_suite_t *suite);
extern void pana_authentication_ready(uint8_t status, protocol_interface_info_entry_t *cur_interface);
#ifdef PANA_SERVER_API
extern void pana_key_update_delay_timer(void);
#else
#define pana_key_update_delay_timer() ((void)0)
#endif

int8_t pana_ccm_data_crypt(uint8_t *ptr, uint16_t len, uint8_t operation_type, uint32_t message_seq, sec_suite_t *suite);

extern void pana_timeout_timer_set(sec_suite_t *suite, sec_state_machine_t cur_state);
extern int pana_retry_check(uint8_t counter, sec_state_machine_t cur_state);

buffer_t *pana_relay_parse(buffer_t *buf);
bool pana_auth_check(uint8_t *ptr, uint16_t length, uint8_t *authency, uint8_t *key);
void pana_session_startms_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
buffer_t *pana_auth_message_handler(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
buffer_t *pana_relay_avp_build(buffer_t *buf, sec_suite_t *suite);
buffer_t *build_pana_base(buffer_t *buf, pana_header_t *header, sec_suite_t *suite);
void pana_set_agend_address(buffer_t *buf, bool relay, sec_suite_t *suite);
void pana_eap_tls_up(buffer_t *buf, sec_suite_t *suite);
int8_t pana_socket_id_get(void);
void pana_auth_hash_calc(uint8_t *data_ptr, uint16_t data_length, uint8_t *key);
#endif

#endif /* PANA_H_ */
