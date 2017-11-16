/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include <string.h>
#include <ns_types.h>
#include <ns_list.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "Core/include/socket.h"
#include "6LoWPAN/Mesh/mesh.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mesh_stub.h"

mesh_stub_def mesh_stub;

buffer_t *mesh_up(buffer_t *buf)
{
    return NULL;
}
buffer_t *mesh_down(buffer_t *buf)
{

    return NULL;
}

bool mesh_address_map(protocol_interface_info_entry_t *cur, addrtype_t *addr_type, uint8_t *addr)
{
    return mesh_stub.bool_value;
}

bool mesh_forwardable_address(const protocol_interface_info_entry_t *cur, addrtype_t addr_type, const uint8_t *addr)
{
    return mesh_stub.bool_value;
}

bool mesh_header_needed(const buffer_t *buf)
{
    return mesh_stub.bool_value;
}
bool mesh_supports_multicast(void)
{
    return mesh_stub.bool_value;
}
void mesh_all_addresses_unicast(bool flag)
{

}
bool mesh_short_address_is_multicast(uint16_t addr)
{
    return mesh_stub.bool_value;
}
bool mesh_short_address_is_broadcast_or_multicast(uint16_t addr)
{
    return mesh_stub.bool_value;
}

void mesh_rewrite_bc0_header(uint8_t *header)
{
}

uint_fast8_t mesh_header_size(const buffer_t *buf)
{
    return mesh_stub.uint8_value;
}

uint_fast8_t mesh_header_len_from_type_byte(uint8_t type)
{
    return mesh_stub.uint8_value;
}
uint_fast8_t mesh_header_len_from_buffer_type_byte(const buffer_t *buf)
{
    return mesh_stub.uint8_value;
}
