/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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

#ifndef PANA_INTERNAL_API_H_
#define PANA_INTERNAL_API_H_
#ifdef PANA
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "net_interface.h"
#include "net_pana_parameters_api.h"
#include "Security/Common/sec_lib_definitions.h"
struct nwk_pana_params;

void pana_reset_values(uint16_t pan_id);
int8_t pana_client_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id);
void pana_reset_client_session(void);




/* Allocate Pana Parameters */
struct nwk_pana_params *pana_client_parameter_allocate(void);
/* Define Key material */
uint8_t *pana_key_get(const uint8_t *key);

void pana_common_state_machine(sec_suite_t *suite);

#ifdef PANA_SERVER_API

int8_t pana_server_process_ready_cb_set(void (*cb_fptr)(void));

int8_t pana_server_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key);

/** Request Key material */
int8_t pana_server_key_material_load(int8_t interface_id);

/* Get Pana server key material */
int8_t pana_network_key_get(int8_t interface_id, ns_keys_t *key);

/** Trig New Key Material */
int8_t pana_server_trig_new_key(int8_t interface_id);

/** Start Key Update Process */
int8_t pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material);

#else
#define pana_server_process_ready_cb_set(cb_fptr) -2
#define pana_server_key_material_load(interface_id) -2
#define pana_server_interface_init(interface_id,cipher_mode, key_material, time_period_before_activate_key) -2
#define pana_network_key_get(interface_id, key) -2
#define pana_server_trig_new_key(interface_id) -2
#define pana_server_key_update(interface_id, network_key_material) -2
#endif

/** TriG Pull Key Material */
int8_t pana_client_key_pull(int8_t interface_id);

int8_t pana_interface_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info);

const pana_lib_parameters_s *pana_get_params_ptr(void);

uint32_t pana_handshake_timeout(void);

uint8_t pana_retry_req_max_get(void);

int8_t pana_set_params(const pana_lib_parameters_s *params);
int8_t pana_get_params(pana_lib_parameters_s *params);
uint8_t pana_ping_notify_msg_tx(uint16_t pan_id);
#else
#define pana_server_process_ready_cb_set(cb_fptr) -2
#define pana_get_params_ptr() NULL
#define pana_common_state_machine(suite) ((void)0)
#define pana_reset_values(pan_id) ((void)0)
#define pana_key_get(key) NULL
#define pana_server_key_material_load(interface_id) -2
#define pana_server_interface_init(interface_id,cipher_mode, key_material, time_period_before_activate_key) -2
#define pana_client_interface_init(interface_id, cipher_mode, psk_key_id) -2
#define pana_reset_client_session() ((void)0)
#define pana_client_parameter_allocate() NULL
#define pana_server_trig_new_key(interface_id) -2
#define pana_interface_certificate_chain_set(chain_info) -2
#define pana_client_key_pull(interface_id) -2
#define pana_network_key_get(interface_id, key) -2
#define pana_set_params(params) -2
#define pana_get_params(params) -2
#define pana_server_key_update(interface_id, network_key_material) -2
#define pana_ping_notify_msg_tx(pan_id) 0
#endif
#endif /* PANA_INTERNAL_API_H_ */
