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
#include "net_pana_parameters_api.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#ifdef PANA

#define TRACE_GROUP "pana"

typedef enum {
    ARM_PANA_INIT = 0,
    ARM_PANA_TLS_CB = 1,
} arm_pana_event_id_e;

#define PANA_HEADER_START 0x0000
#define ZIGBEE_VENDOR_ID 0x0000917c
#define PANA_EAP_VENDOR_FLAG 0x8000

#define PANA_EAP_KEYWARP_TYPE 0x0001
#define PANA_EAP_KEYREQ_TYPE 0x0002
//Pana Relay Constant
const uint8_t PANA_AUTH_STRING[9] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A'};
const uint8_t PANA_PAA_ENC_KEY_STRING[18] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A', ' ', 'P', 'A', 'A', ' ', 'E', 'n', 'c', 'r'};
const uint8_t PANA_PAC_ENC_KEY_STRING[18] = {'I', 'E', 'T', 'F', ' ', 'P', 'A', 'N', 'A', ' ', 'P', 'a', 'C', ' ', 'E', 'n', 'c', 'r'};
const uint8_t EAP_ANYMOUS[9] = {'a', 'n', 'o', 'n', 'y', 'm', 'o', 'u', 's'};

buffer_t *parse_pana_base(buffer_t *buf);
uint8_t pana_parse_key_data(uint8_t *ptr, uint16_t length);
uint8_t pana_analyze_zigbee_key_data(uint8_t *ptr, uint16_t len);
/** Pana Core functions */

buffer_t *pana_relay_parse(buffer_t *buf);


pana_session_t * pana_session_allocate(void)
{
    return NULL;
}

void pana_session_state_init(pana_session_t *p_session)
{
}

pana_heap_t * pana_heap_structure_allocate(void)
{
    return NULL;
}

int8_t pana_get_params(pana_lib_parameters_s *params)
{
    return 0;
}

int8_t pana_set_params(const pana_lib_parameters_s *params)
{
    return 0;
}

const pana_lib_parameters_s *pana_get_params_ptr(void)
{
    return NULL;
}

void pana_timeout_timer_set(sec_suite_t *suite, sec_state_machine_t cur_state)
{
}

uint8_t pana_retry_req_max_get(void)
{
    return 0;
}

uint32_t pana_handshake_timeout(void)
{
    return 0;
}

int pana_retry_check(uint8_t counter, sec_state_machine_t cur_state)
{
    return 0;
}

void pana_avp_temp_set(uint32_t avp_value)
{
}

uint8_t *pana_key_get(const uint8_t *key)
{
    return NULL;
}

void pana_session_init_by_session_ptr(sec_suite_t *suite, auth_info_t *auth_ptr)
{
}


void pana_authentication_ready(uint8_t status, protocol_interface_info_entry_t *cur_interface)
{
}

void pana_reset_values(uint16_t pan_id)
{
}

void pana_event_handler(arm_event_s *event)
{
}

bool pana_socket_init(pana_socket_packet_handler_cb * socket_handler, pana_state_machine_step *state_machine_stepper, pana_eap_tls_up_cb *tls_handler_cb)
{
    return true;
}

buffer_t *parse_pana_base(buffer_t *buf)
{
    return NULL;
}

buffer_t *pana_relay_avp_build(buffer_t *buf, sec_suite_t *suite)
{
    return NULL;
}

uint8_t pana_analyze_zigbee_key_data(uint8_t *ptr, uint16_t len)
{
    return 0;
}

buffer_t *pana_relay_parse(buffer_t *buf)
{
    return NULL;
}

void pana_session_startms_parse(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
}

void pana_complete_msg_parse(buffer_t *buf)
{
}

uint8_t *pana_avp_set(uint16_t avp, uint8_t *dptr)
{
    return NULL;
}

void pana_start_response_build(buffer_t *buf)
{
}


uint8_t pana_parse_key_data(uint8_t *ptr, uint16_t length)
{
    return 0;
}


void pana_free_dynamic_ram(sec_suite_t *suite)
{
}

void pana_key_calculation(sec_suite_t *suite)
{

}

void pana_start_message_build(buffer_t *buf, sec_suite_t *suite)
{

}

int8_t pana_interface_certificate_chain_set(const arm_certificate_chain_entry_s *chain_info)
{
    return -1;
}

bool pana_auth_check(uint8_t *ptr, uint16_t length, uint8_t *authency, uint8_t *key)
{
    return true;
}

buffer_t *pana_auth_message_handler(buffer_t *buf, pana_header_t *header, sec_suite_t *suite)
{
    return buf;
}

void pana_eap_tls_up(buffer_t *buf, sec_suite_t *suite)
{

}

void pana_eap_down(buffer_t *buf, sec_suite_t *suite)
{

}

void pana_eap_payload_down(buffer_t *buf, const uint8_t *nonce, sec_suite_t *suite)
{

}

void pana_down(buffer_t *buf, sec_suite_t *suite)
{

}

void pana_common_state_machine(sec_suite_t *suite)
{

}

int8_t pana_ccm_data_crypt(uint8_t *ptr, uint16_t len, uint8_t operation_type, uint32_t message_seq, sec_suite_t *suite)
{
    return 0;
}

void pana_set_agend_address(buffer_t *buf, bool relay, sec_suite_t *suite)
{

}

void pana_session_base_init(pana_session_t * p_session)
{

}

#else

int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    return -1;
}


#endif /*PANA*/

/* end of file */
