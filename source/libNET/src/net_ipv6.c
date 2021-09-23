/*
 * Copyright (c) 2016-2020, Pelion and affiliates.
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
#include "nsdynmemLIB.h"

#include "Core/include/ns_address_internal.h"
#include "Common_Protocols/ipv6_flow.h"
#include "Common_Protocols/ipv6_fragmentation.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ipv6_stack/protocol_ipv6.h"

#include "net_ipv6_api.h"

#include "ns_trace.h"

#define TRACE_GROUP "nip6"

int8_t arm_nwk_ipv6_frag_mru(uint16_t frag_mru)
{
    uint16_t set_mru = ipv6_frag_set_mru(frag_mru);
    return set_mru >= frag_mru ? 0 : -1;
}

int8_t arm_nwk_ipv6_max_cache_entries(uint16_t max_entries)
{
    return ipv6_neighbour_set_current_max_cache(max_entries);
}

int8_t arm_nwk_ipv6_destination_cache_configure(uint16_t max_entries, uint16_t short_term_threshold, uint16_t long_term_threshold, uint16_t lifetime)
{
    return ipv6_destination_cache_configure(max_entries, short_term_threshold, long_term_threshold, lifetime);
}

int8_t arm_nwk_ipv6_neighbour_cache_configure(uint16_t max_entries, uint16_t short_term_threshold, uint16_t long_term_threshold, uint16_t lifetime)
{
    return ipv6_neighbour_cache_configure(max_entries, short_term_threshold, long_term_threshold, lifetime);
}

void arm_nwk_ipv6_auto_flow_label(bool auto_flow_label)
{
    ipv6_flow_auto_label = auto_flow_label;
}

int8_t arm_nwk_ipv6_opaque_iid_key(const void *secret_key, uint8_t key_len)
{
    return addr_opaque_iid_key_set(secret_key, key_len);
}

int8_t arm_nwk_ipv6_opaque_iid_enable(int8_t interface_id, bool enable)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    cur->opaque_slaac_iids = enable;
    return 0;
}

int8_t arm_nwk_ipv6_default_route_enable(int8_t interface_id, bool enable)
{
#ifdef HAVE_ETHERNET
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }

    ipv6_stack_route_advert_default_route(cur, enable);
    return 0;
#else
    (void)interface_id;
    (void)enable;
    return -1;
#endif
}

int8_t arm_nwk_ipv6_dns_server_add(int8_t interface_id, uint8_t *address, uint32_t lifetime)
{
#ifdef HAVE_ETHERNET
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }
    if (lifetime == 0) {
        ipv6_stack_route_advert_dns_server_delete(address);
    } else {
        return ipv6_stack_route_advert_dns_server_add(address);
    }

    return 0;
#else
    (void)interface_id;
    (void)address;
    (void)lifetime;
    return -1;
#endif
}

int8_t arm_nwk_ipv6_dns_search_list_add(int8_t interface_id, uint8_t *data, uint16_t data_len, uint32_t lifetime)
{
#ifdef HAVE_ETHERNET
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return -1;
    }
    return ipv6_stack_route_advert_dns_search_list_add(data, data_len, lifetime);
#else
    (void)interface_id;
    (void)data;
    (void)data_len;
    (void)lifetime;
    return -1;
#endif
}
