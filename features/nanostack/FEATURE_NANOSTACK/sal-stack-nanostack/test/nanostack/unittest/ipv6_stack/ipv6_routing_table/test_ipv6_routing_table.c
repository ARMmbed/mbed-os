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
#include "test_ipv6_routing_table.h"
#include <string.h>
#include "ipv6_stack/ipv6_routing_table.h"
#include "ipv6_resolution_stub.h"


bool test_ipv6_neighbour_cache_init()
{
    ipv6_neighbour_cache_t entry;
    ns_list_init(&entry.list);
    ipv6_neighbour_cache_init(&entry, 0);
    return true;
}


bool test_ipv6_route_table_set_max_entries()
{
    bool ret_val;

    ipv6_neighbour_cache_t ipv6_neighbour_cache;
    memset(&ipv6_neighbour_cache, 0, sizeof(ipv6_neighbour_cache_t));

    ipv6_resolution_stub.ipv6_neighbour_cache = &ipv6_neighbour_cache;

    // 1st interface
    ipv6_route_table_set_max_entries(1, ROUTE_RADV, 5);

    if (ipv6_neighbour_cache.route_if_info.sources[ROUTE_RADV] != 5 ||
        ipv6_resolution_stub.uint8_input_value != 1) {
        return false;
    }
    memset(&ipv6_neighbour_cache, 0, sizeof(ipv6_neighbour_cache_t));

    // 2nd interface
    ipv6_route_table_set_max_entries(2, ROUTE_REDIRECT, 255);
    if (ipv6_neighbour_cache.route_if_info.sources[ROUTE_REDIRECT] != 255 ||
        ipv6_resolution_stub.uint8_input_value != 2) {
        return false;
    }
    memset(&ipv6_neighbour_cache, 0, sizeof(ipv6_neighbour_cache_t));

    ipv6_resolution_stub.ipv6_neighbour_cache = NULL;
    // No interface
    ipv6_route_table_set_max_entries(3, ROUTE_RADV, 7);

    // 3nd interface
    ipv6_resolution_stub.ipv6_neighbour_cache = &ipv6_neighbour_cache;
    ipv6_route_table_set_max_entries(3, ROUTE_USER, 75);
    if (ipv6_neighbour_cache.route_if_info.sources[ROUTE_USER] != 75 ||
        ipv6_resolution_stub.uint8_input_value != 3) {
        return false;
    }
    memset(&ipv6_neighbour_cache, 0, sizeof(ipv6_neighbour_cache_t));

    return true;
}
