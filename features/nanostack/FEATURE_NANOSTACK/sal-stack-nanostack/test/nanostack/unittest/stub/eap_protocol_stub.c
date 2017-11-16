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
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"

#include "common_functions.h"
#include "Security/PANA/pana_eap_header.h"
#ifdef PANA

#define TRACE_GROUP "eap"

const uint8_t EAP_ANYMOUS[9] = {'a', 'n', 'o', 'n', 'y', 'm', 'o', 'u', 's'};

void pana_eap_identity_build(buffer_t *buf, sec_suite_t *suite)
{

}

void pana_eap_tls_start_build(buffer_t *buf, sec_suite_t *suite)
{

}


void pana_eap_tls_finnish_build(buffer_t *buf, sec_suite_t *suite)
{

}

bool pana_eap_frag_re_tx(sec_suite_t *suite)
{
    return true;
}

buffer_t *eap_down(buffer_t *buf, sec_suite_t *suite)
{
    return (buf);
}

buffer_t *eap_up(buffer_t *buf, sec_suite_t *suite)
{
    return buf;
}
#endif
