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
#include "Common_Protocols/icmpv6_prefix.h"
#include "common_functions.h"

prefix_entry_t *icmpv6_prefix_add(prefix_list_t *list, const uint8_t *prefixPtr, uint8_t prefix_len, uint32_t lifeTime, uint32_t prefTime, uint8_t flags)
{
    return NULL;
}

prefix_entry_t *icmpv6_prefix_compare(prefix_list_t *list, const uint8_t *addr, uint8_t prefix_len)
{
    return NULL;
}


void icmpv6_prefix_list_free(prefix_list_t *list)
{
}
