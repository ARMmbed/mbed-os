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
#include "ns_trace.h"
#include "string.h"
#include "eventOS_event.h"
#include "ccmLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "shalib.h"
#include "randLIB.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana_nvm.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "common_functions.h"

sec_suite_t *NS_LARGE active_ecc_sec_suite = 0;

sec_suite_t * sec_lib_security_session_allocate(bool tls_allocate)
{
    return NULL;
}


void sec_lib_state_machine_lock(sec_suite_t *suite, sec_state_machine_t state)
{
}

void sec_lib_state_machine_trig(sec_suite_t *suite, sec_state_machine_t state)
{
}

uint8_t tls_server_certi_hash_copy(sec_suite_t *suite)
{
}

#ifdef ECC
uint8_t tls_certi_hash_copy(sec_suite_t *suite)
{
}
#endif


#ifdef ECC
void sec_ecc_state_free(sec_suite_t *suite)
{
}

void sec_ecc_state_save(sec_suite_t *suite)
{
}

#ifdef PANA
void sec_ecc_sceduler()
{

}
#endif

#endif

void sec_libray_init(void)
{
}

int sec_pana_protocol_init(sec_suite_t *suite)
{
    return 0;
}

void sec_suite_tls_free(sec_suite_t *suite, bool free_session)
{
}



uint8_t sec_auth_re_check(sec_suite_t *suite)
{
    return 0;
}

uint8_t sec_check_suite_ptrs(sec_suite_t *suite)
{
    return 0;
}

#ifdef PANA
void sec_timer_handle()
{
}
#endif

sec_suite_t *sec_suite_verify(sec_suite_t *session)
{
    return NULL;
}

sec_suite_t *sec_suite_selected_pana_session(uint32_t session_id)
{
    return NULL;
}

sec_suite_t *sec_suite_selected_address(const uint8_t address[static 16])
{
    return NULL;
}

uint16_t sec_pana_key_update_trig(uint16_t th_time)
{
    return 0;
}


int8_t sec_suite_remove(sec_suite_t *cur)
{
    return 0;
}

sec_suite_t *sec_suite_selected_py_pan_id(uint16_t pan_id)
{
    return NULL;
}


sec_suite_t *sec_suite_create(void)
{
    return NULL;
}

void sec_prf_state_set(sec_suite_t *suite)
{
}

#ifdef ECC
uint8_t tls_ecc_start_premaster_secret(EllipticPoint *ellicpt_ptr, sec_suite_t *suite)
{
    return 0;
}

void ecc_operation_done_callback(int8_t status, void *result_ptr)
{
}

void sec_ecc_client_signature_start(sec_suite_t *suite)
{
}

void sec_ecc_gen_public_key_start(sec_suite_t *suite)
{
}
#endif

void eap_fragmentation_init(sec_suite_t *suite)
{
}

void sec_auth_ready(void)
{
}

uint8_t sec_suite_socket_event(uint8_t event_type, sec_suite_t *suite)
{
    return 0;
}

void sec_set_auth_timeout(sec_suite_t *suite, sec_state_machine_t cur_state)
{
}
void sec_suite_list_clean(void)
{
}
