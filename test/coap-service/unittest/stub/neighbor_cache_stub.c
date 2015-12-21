/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 */
#include "config.h"
#include "ns_types.h"
#include "common_functions.h"
#include "platform/ns_debug.h"
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
