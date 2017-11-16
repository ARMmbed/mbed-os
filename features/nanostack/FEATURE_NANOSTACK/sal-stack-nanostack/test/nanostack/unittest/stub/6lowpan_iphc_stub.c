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
#define HAVE_CIPV6

#ifdef HAVE_CIPV6
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "Core/include/socket.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "6LoWPAN/IPHC_Decode/iphc_compress.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/Thread/thread_common.h"

#define TRACE_GROUP  "iphc"

#include "6LoWPAN/ND/nd_router_object.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"

buffer_t *lowpan_down(buffer_t *buf)
{
    return NULL;
}

buffer_t *lowpan_up(buffer_t *buf)
{
    return NULL;
}

#endif /* HAVE_CIPV6 */

