/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include <string.h>

#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/ipv6_resolution.h"
#include "Common_Protocols/icmpv6.h"

#include "RPL/rpl_protocol.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_structures.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_data.h"
#include "rpl_data_stub.h"

rpl_data_stub_def rpl_data_stub;

void rpl_data_sr_invalidate(void)
{

}

/* Public API */
void rpl_data_init(void)
{

}
void rpl_data_init_root(void)
{

}

ipv6_route_predicate_fn_t *rpl_data_get_route_predicate(struct rpl_domain *domain, const buffer_t *buf)
{
return NULL;
}
bool rpl_data_process_hbh(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *opt, const struct ns_sockaddr *ll_src)
{
    return rpl_data_stub.bool_value;
}
bool rpl_data_remember_outer(buffer_t *buf)
{
    return rpl_data_stub.bool_value;
}
bool rpl_data_is_rpl_route(ipv6_route_src_t source)
{
    return rpl_data_stub.bool_value;
}
bool rpl_data_is_rpl_parent_route(ipv6_route_src_t source)
{
    return rpl_data_stub.bool_value;
}
bool rpl_data_forwarding_error(buffer_t *buf)
{
    return rpl_data_stub.bool_value;
}
buffer_t *rpl_data_process_routing_header(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t *hdrlen_out, bool *forward_out)
{
    return NULL;
}
buffer_t *rpl_data_source_route_error_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}
bool rpl_data_get_srh_last_address(const uint8_t *srh, uint8_t *addr_out)
{
    return rpl_data_stub.bool_value;
}
