/*
 * Copyright (c) 2012-2017, Arm Limited and affiliates.
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
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "mac_api.h"

#include "RPL/rpl_control.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "shalib.h"

#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "common_functions.h"
#include "net_thread_test.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "BorderRouter/border_router.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/beacon_handler.h"

void arm_nwk_6lowpan_borderrouter_data_free(protocol_interface_info_entry_t *cur)
{
}

void nd_border_router_setup_refresh(nwk_interface_id id, bool fresh_abro)
{
}

int8_t arm_nwk_6lowpan_border_route_nd_default_prefix_timeout_set(int8_t interface_id, uint32_t time)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_context_update(int8_t interface_id, uint8_t c_id_flags, uint8_t context_len, uint16_t ttl, const uint8_t *context_ptr)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_nd_context_load(int8_t interface_id, uint8_t *contex_data)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_configure_push(int8_t interface_id)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_context_remove_by_id(int8_t interface_id, uint8_t c_id)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_context_parameter_update(int8_t interface_id, uint8_t c_id, uint8_t compress_mode, uint16_t ttl)
{
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, const border_router_setup_s *border_router_setup_ptr)
{
    return 0;
}

int arm_border_router_proxy_state_update(int8_t caller_interface_id, int8_t handler_interface_id, bool status)
{
    return 0;
}

void arm_border_router_ready(protocol_interface_info_entry_t *cur)
{
}

int8_t arm_nwk_6lowpan_borderrouter_init(protocol_interface_info_entry_t *cur)
{
    return 0;
}
