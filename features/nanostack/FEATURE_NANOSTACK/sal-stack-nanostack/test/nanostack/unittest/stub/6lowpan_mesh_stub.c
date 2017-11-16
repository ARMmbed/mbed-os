/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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
#include "ns_types.h"
#define FEA_TRACE_SUPPORT
#define EXTRA_CONSISTENCY_CHECKS
#include "Core/include/ns_buffer.h"
#include "ns_list.h"
#include "mac_api.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "mesh.h"

protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id)
{
    return NULL;
}

extern COMMON_FUNCTIONS_FN uint16_t common_read_16_bit(const uint8_t data_buf[__static 2]);
extern COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit(uint16_t value, uint8_t ptr[__static 2]);
extern COMMON_FUNCTIONS_FN uint8_t *common_write_16_bit_inverse(uint16_t value, uint8_t ptr[__static 2]);

bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t addr[])
{
    return false;
}

uint8_t addr_check_broadcast(const address_t addr, addrtype_t addr_type)
{
    return 0;
}

#ifndef EXTRA_CONSISTENCY_CHECKS
uint8_t *buffer_corrupt_check(buffer_t *buf)
{
    return NULL;
}
#endif

void protocol_stats_update(nwk_stats_type_t type, uint16_t update_val)
{
}

#if 0
buffer_t *buffer_headroom(buffer_t *buf, uint16_t size)
{
    return NULL;
}
#endif
