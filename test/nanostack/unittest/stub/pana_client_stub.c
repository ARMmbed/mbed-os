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
#define TRACE_GROUP "PaCli"

void (*pana_client_nvm_storage_cb)(pana_client_nvm_update_process_t) = 0;
static uint8_t *pana_client_nvm_buffer = 0;

static nwk_pana_params_s pana_params;

static sec_suite_t tempsuite;

bool pana_check_address(buffer_t *buf)
{
    return false;
}

sec_suite_t *pana_client_init(auth_info_t *auth_ptr, uint8_t *session_address_ptr, pana_tls_setup_s *setup)
{
    return &tempsuite;
}

void pana_reset_client_session(void)
{

}

int8_t pana_client_interface_init(int8_t interface_id, net_tls_cipher_e cipher_mode, uint32_t psk_key_id)
{
    return 0;
}

nwk_pana_params_s * pana_client_parameter_allocate(void)
{
    return &pana_params;
}

int8_t pana_client_key_pull(int8_t interface_id)
{
    return 0;
}

uint8_t pana_ping_notify_msg_tx(uint16_t pan_id)
{
    return 1;

}

int8_t pana_client_nvm_callback_set(pana_client_session_update_cb *nvm_update, pana_client_session_get_cb *nvm_get, uint8_t *nvm_static_buffer)
{
    return 0;
}


#else
int8_t pana_client_nvm_callback_set(pana_client_session_update_cb *nvm_update, pana_client_session_get_cb *nvm_get, uint8_t *nvm_static_buffer)
{
    return -1;
}

#endif

