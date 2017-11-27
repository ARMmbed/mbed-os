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
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "6LoWPAN/IPHC_Decode/lowpan_context.h"
#include "common_functions.h"

#define TRACE_GROUP_LOWPAN_CONTEXT "lCon"

lowpan_context_t *lowpan_contex_get_by_id(const lowpan_context_list_t *list, uint8_t id)
{
    return NULL;
}

lowpan_context_t *lowpan_context_get_by_address(const lowpan_context_list_t *list, const uint8_t *ipv6Address)
{
    return NULL;
}


int_fast8_t lowpan_context_update(lowpan_context_list_t *list, uint8_t cid_flags, uint16_t lifetime, const uint8_t *prefix, uint_fast8_t len, bool stable)
{
    return 0;
}

void lowpan_context_list_free(lowpan_context_list_t *list)
{
}

void lowpan_context_timer(lowpan_context_list_t *list, uint_fast16_t ticks)
{
}

