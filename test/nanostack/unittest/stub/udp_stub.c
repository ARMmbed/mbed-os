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
#include "string.h"
#include "ns_trace.h"
#include "ip_fsc.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/Bootstraps/network_lib.h" // for nwk_udp_rx_security_check
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/udp.h"
#include "Core/include/socket.h"
#include "common_functions.h"

#define TRACE_GROUP "udp"

void udp_checksum_write(buffer_t *buf)
{
}


buffer_t *udp_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *udp_up(buffer_t *buf)
{
    return NULL;
}
