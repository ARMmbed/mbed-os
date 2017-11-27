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
#include <string.h>
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/udp.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "MLE/mle.h"
#include "shalib.h"

#ifdef PANA
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "ccmLIB.h"
#endif
#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "net_nwk_scan.h"

#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#ifdef HAVE_RPL
#include "RPL/rpl_data.h"
#endif

#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"

#define MAC_MLME_SACN_PERIO_PARAM 5

#define TRACE_GROUP "nw"

uint8_t nwk_ready(nwk_interface_id id)
{
    return 0;
}

void nwk_parent_poll_fail_cb(int8_t id)
{
}

void network_library_init(void)
{
}

buffer_t *nwk_udp_rx_security_check(buffer_t *buf)
{
    return NULL;
}

