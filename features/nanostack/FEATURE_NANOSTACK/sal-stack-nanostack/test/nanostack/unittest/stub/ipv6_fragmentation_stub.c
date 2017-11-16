/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "ns_trace.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ip.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/ipv6_fragmentation.h"

#include "NWK_INTERFACE/Include/protocol_stats.h"

#define TRACE_GROUP "Ifrg"

#ifndef NO_IP_FRAGMENT_RX

/* We would be in trouble if last fragment is < 8 bytes, and we didn't have
 * room for the hole descriptor. Avoid a problem by ensuring that we always
 * allocate a multiple-of-8 reassembly buffer.
 */
uint16_t ipv6_frag_set_mru(uint16_t frag_mru)
{
    return 0;
}

void ipv6_frag_timer(uint8_t secs)
{
}

buffer_t *ipv6_frag_up(buffer_t *frag_buf, const uint8_t *frag_hdr, uint8_t *nh_ptr, uint16_t payload_length)
{
    return NULL;
}
#endif /* NO_IP_FRAGMENT_RX */

#ifndef NO_IP_FRAGMENT_TX
buffer_t *ipv6_frag_down(buffer_t *dgram_buf)
{
    return NULL;
}
#endif /* NO_IP_FRAGMENT_TX */
