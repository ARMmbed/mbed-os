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

#ifdef PANA
#ifdef PANA_SERVER_API

#define TRACE_GROUP "PaSrv"

void pana_key_update_delivery_ready(void)
{
}

void pana_session_nvm_udate(sec_suite_t *suite, pana_nvm_update_process_t update_event)
{

}

int8_t pana_server_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, const uint8_t *key_material, uint32_t time_period_before_activate_key)
{
    return -1;
}

int8_t pana_server_key_update(int8_t interface_id, const uint8_t *network_key_material)
{
    return 0;
}

void pana_key_update_delay_timer(void)
{

}

int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    return -1;
}

int8_t pana_server_process_ready_cb_set(void (*cb_fptr)(void))
{
    return -1;
}

/* Pana Client session load from NVM API */
int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer)
{
    return 0;
}

int8_t pana_network_key_get(int8_t interface_id, ns_keys_t *key)
{
    return -1;
}

int8_t pana_server_trig_new_key(int8_t interface_id)
{
    return -1;

}

int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id)
{
    return 0;
}

int8_t pana_server_key_material_load(int8_t interface_id)
{
    return 0;
}

#else
int8_t pana_server_nvm_callback_set(pana_server_update_cb *update_cb, pana_server_session_get_cb *nvm_get, pana_server_session_get_by_id_cb *nvm_session_get, uint8_t *nvm_static_buffer)
{
    return -1;
}

int8_t pana_server_nvm_client_session_load(uint8_t *nvm_pointer)
{
    return -1;
}

int8_t pana_server_restore_from_nvm(uint8_t *nvm_data, int8_t interface_id)
{
    return -1;
}
#endif

#endif
