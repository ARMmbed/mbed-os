/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/*
 * \file icmpv6_prefix.c
 * \brief Add short description about this file!!!
 *
 */
prefix_entry_t *icmpv6_prefix_add(prefix_list_t *list, const uint8_t *prefixPtr, uint8_t prefix_len, uint32_t lifeTime, uint32_t prefTime, uint8_t flags)
{
    prefix_entry_t *entry;

    entry = icmpv6_prefix_compare(list, prefixPtr, prefix_len);
    if (entry) {
        if (flags != 0xff) {
            entry->options = flags;
            entry->lifetime = lifeTime;
            entry->preftime = prefTime;
        }
        return entry;
    }

    entry = ns_dyn_mem_alloc(sizeof(prefix_entry_t));
    if (entry) {
        entry->prefix_len = prefix_len;
        entry->options = 0xff;
        entry->lifetime = lifeTime;
        entry->preftime = prefTime;
        memset(entry->prefix, 0, 16);
        bitcopy0(entry->prefix, prefixPtr, prefix_len);
        ns_list_add_to_end(list, entry);
    }
    return entry;
}

prefix_entry_t *icmpv6_prefix_compare(prefix_list_t *list, const uint8_t *addr, uint8_t prefix_len)
{
    ns_list_foreach(prefix_entry_t, cur, list) {
        if (cur->prefix_len == prefix_len && bitsequal(cur->prefix, addr, prefix_len)) {
            return cur;
        }
    }
    return NULL;
}


void icmpv6_prefix_list_free(prefix_list_t *list)
{
    ns_list_foreach_safe(prefix_entry_t, cur, list) {
        ns_list_remove(list, cur);
        ns_dyn_mem_free(cur);
    }
}
