/*
 * Copyright (c) 2013-2016, Arm Limited and affiliates.
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
/**
 *
 * \file cipv6_fragmenter.c
 * \brief Packet Fragmentation and Reasembly.
 *
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "Core/include/socket.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "nsdynmemLIB.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/IPHC_Decode/iphc_decompress.h"
#include "nwk_stats_api.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "6frg"

#include "cipv6_fragmenter_stub.h"

cipv6_fragmenter_stub_def cipv6_fragmenter_stub;


buffer_t *cipv6_frag_reassembly(int8_t interface_id, buffer_t *buf)
{
    return cipv6_fragmenter_stub.buffer_ptr;
}

void cipv6_frag_timer(uint16_t seconds)
{
}

int8_t reassembly_interface_reset(int8_t interface_id)
{
    return cipv6_fragmenter_stub.int8Value;
}
int8_t reassembly_interface_init(int8_t interface_id, uint8_t reassembly_session_limit, uint16_t reassembly_timeout)
{
    return cipv6_fragmenter_stub.int8Value;
}
int8_t reassembly_interface_free(int8_t interface_id)
{
    return cipv6_fragmenter_stub.int8Value;
}

