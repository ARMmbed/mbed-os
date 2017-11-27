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
#include "ns_list.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6.h"
#include "6LoWPAN/IPHC_Decode/cipv6.h"

typedef struct iphc_compress_state {
    protocol_interface_info_entry_t *const interface;
    const uint8_t *in;
    uint8_t *out;
    uint16_t len;
    uint16_t out_space;
    uint16_t consumed;
    uint16_t produced;
    const uint8_t *outer_src_iid;
    const uint8_t *outer_dst_iid;
} iphc_compress_state_t;

buffer_t *iphc_compress(protocol_interface_info_entry_t *cur, buffer_t *buf, uint16_t hc_space)
{
    return NULL;
}
