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
#include "ns_types.h"
#include "common_functions.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "Service_Libs/Neighbor_cache/neighbor_table_definition.h"

void neighbor_cache_init(neigh_cache_s *neigh_cache)
{
}

int neighbor_cache_flush(neigh_cache_s *neigh_cache)
{
    return 0;
}


neigh_cache_entry_s *neighbor_cache_entry_get(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr)
{
    return NULL;
}

neigh_cache_entry_s *neighbor_cache_entry_create(neigh_cache_s *neigh_cache, const uint8_t address_ptr[8])
{
    return NULL;
}

int8_t neighbor_cache_entry_delete(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr)
{
    return 0;
}

neigh_cache_entry_s *neighbor_cache_entry_delete_by_entry_pointer(neigh_cache_s *neigh_cache, neigh_cache_entry_s *entry)
{
    return NULL;
}

int neighbor_cache_ttl_update(neigh_cache_s *neigh_cache, uint16_t ticks)
{
    return 0;
}

neigh_cache_entry_s *neighbor_cache_entry_get_timed_out(neigh_cache_s *neigh_cache)
{
    return NULL;
}
