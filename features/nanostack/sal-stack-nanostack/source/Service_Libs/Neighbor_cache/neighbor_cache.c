/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "Service_Libs/Neighbor_cache/neighbor_table_definition.h"

#ifdef HAVE_NEIGHBOR_CACHE

static void entry_delete(neigh_cache_s *neigh_cache, neigh_cache_entry_s *entry);

/**
 * \brief A function to initialize a neighbor cache before use
 *
 * \param neigh_cache pointer to neighbor cache.
 */
void neighbor_cache_init(neigh_cache_s *neigh_cache)
{
    ns_list_init(&neigh_cache->head);
}

/**
 * \brief A function to flush an entire neighbour cache
 *
 * \param neigh_cache pointer to neighbor cache.
 *
 * \return <n> The number of entries removed
 */
int neighbor_cache_flush(neigh_cache_s *neigh_cache)
{
    int count = 0;

    ns_list_foreach_safe(neigh_cache_entry_s, entry, &neigh_cache->head) {
        entry_delete(neigh_cache, entry);
        count++;
    }

    return count;
}

/**
 * \brief Extended internal implementation of neighbor_cache_entry_get
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_type type for 16-bit or 64-bit address
 * \param address_ptr pointer to address of specified type
 * \param reorder true if permitted to reorder the list
 *
 * \return pointer to found entry
 * \return NULL if not found
 */
static neigh_cache_entry_s *entry_get(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr, bool reorder)
{
    uint16_t mac16 = 0;
    neigh_cache_entry_s *entry = NULL;

    if (address_type == NEIGH_16_BIT_ADDRESS) {
        mac16 = common_read_16_bit(address_ptr);
    }

    ns_list_foreach(neigh_cache_entry_s, e, &neigh_cache->head) {
        bool match = false;
        switch (address_type) {
            case NEIGH_64_BIT_ADDRESS:
                match = memcmp(address_ptr, e->mac64, sizeof(entry->mac64)) == 0;
                break;
            case NEIGH_16_BIT_ADDRESS:
                match = e->mac16 == mac16;
                break;
            case NEIGH__TIMED_OUT:
                match = e->ttl == 0;
                break;
        }
        if (match) {
            entry = e;
            break;
        }
    }

    if (!entry) {
        return NULL;
    }

    /*
     * If permitted, move this entry to head of list, so frequently-used entries
     * stay near the top.
     */
    if (entry != ns_list_get_first(&neigh_cache->head) && reorder) {
        ns_list_remove(&neigh_cache->head, entry);
        ns_list_add_to_start(&neigh_cache->head, entry);
    }

    return entry;
}

/**
 * \brief A function to locate a specific entry by address
 *
 * Note that this can re-order the cache, so could upset iteration using macros.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_type type for 16-bit or 64-bit address
 * \param address_ptr pointer to address of specified type
 *
 * \return pointer to cache entry
 * \return NULL if not found
 */
neigh_cache_entry_s *neighbor_cache_entry_get(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr)
{
    return entry_get(neigh_cache, address_type, address_ptr, true);
}

/**
 * \brief A function to create or return an existing neighbor cache entry.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_ptr pointer to EUI-64 address (64-bit)
 *
 * \return pointer to cache entry, possibly newly created.
 * \return NULL if entry not found an unable to allocate memory for new entry
 */
neigh_cache_entry_s *neighbor_cache_entry_create(neigh_cache_s *neigh_cache, const uint8_t address_ptr[8])
{
    neigh_cache_entry_s *entry;

    entry = neighbor_cache_entry_get(neigh_cache, NEIGH_64_BIT_ADDRESS, address_ptr);
    if (entry) {
        return entry;
    }

    entry = ns_dyn_mem_alloc(sizeof * entry);
    if (!entry) {
        return NULL;
    }

    memset(entry, 0, sizeof * entry);
    memcpy(entry->mac64, address_ptr, sizeof entry->mac64);
    entry->mac16 = 0xFFFF;
    entry->neighbor_keypair_info = NULL;
    ns_list_add_to_start(&neigh_cache->head, entry);

    return entry;
}

/**
 * \brief Delete an entry from the list, including housekeeping
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param entry pointer to entry
 */
static void entry_delete(neigh_cache_s *neigh_cache, neigh_cache_entry_s *entry)
{
    ns_list_remove(&neigh_cache->head, entry);
    if (entry->neighbor_keypair_info) {
        ns_dyn_mem_free(entry->neighbor_keypair_info);
    }
    ns_dyn_mem_free(entry);
}

/**
 * \brief A function to delete an entry by address.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_type type for 16-bit or 64-bit address
 * \param address_ptr pointer to address of specified type
 *
 * \return 0 Removed OK
 * \return -1 Entry not found
 */
int8_t neighbor_cache_entry_delete(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr)
{
    neigh_cache_entry_s *entry;

    entry = entry_get(neigh_cache, address_type, address_ptr, false);
    if (!entry) {
        return -1;
    }

    entry_delete(neigh_cache, entry);

    return 0;
}

/**
 * \brief A function to delete an entry by entry pointer.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param entry pointer to entry
 *
 * \return pointer to the next entry, to allow deletion during iteration
 * \return NULL if no more entries
 * \return NEIGH_ENTRY_PTR_ERR if entry pointer not found (no longer checked)
 */
neigh_cache_entry_s *neighbor_cache_entry_delete_by_entry_pointer(neigh_cache_s *neigh_cache, neigh_cache_entry_s *entry)
{
    neigh_cache_entry_s *next = ns_list_get_next(&neigh_cache->head, entry);

    entry_delete(neigh_cache, entry);

    return next;
}

/**
 * \brief A function to update Neighbor cache Time-To-Live values.
 *
 * This decrements the TTL for all entries in the cache. TTL values are
 * are clamped to not wrap past zero. When an entry's TTL value becomes zero,
 * link_req_counter is set to NEIGH_LINK_REQUEST_COUNTER. (Note that
 * newly-created entries have ttl and link_req_counter both zero - they will
 * need initialising before use).
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param tick amount to decrement TTL
 *
 * \return total number of entries in the cache whose TTL is 0 after the update
 */
int neighbor_cache_ttl_update(neigh_cache_s *neigh_cache, uint16_t ticks)
{
    int count = 0;

    ns_list_foreach(neigh_cache_entry_s, entry, &neigh_cache->head) {
        if (entry->ttl > ticks) {
            entry->ttl -= ticks;
        } else {
            if (entry->ttl > 0) {
                entry->link_req_counter = NEIGH_LINK_REQUEST_COUNTER;
                entry->ttl = 0;
            }
            count++;
        }
    }

    return count;
}

/**
 * \brief A function to get a timed-out neighbor entry.
 *
 * Returns an entry whose TTL field is set to zero.
 *
 * \param neigh_cache pointer to neighbor cache.
 *
 * \return pointer to a timed-out entry
 * \return NULL if no timed-out entries
 */
neigh_cache_entry_s *neighbor_cache_entry_get_timed_out(neigh_cache_s *neigh_cache)
{
    return neighbor_cache_entry_get(neigh_cache, NEIGH__TIMED_OUT, NULL);
}

#else // HAVE_NEIGHBOR_CACHE

void neighbor_cache_init(neigh_cache_s *neigh_cache)
{
    (void)neigh_cache;
}
int neighbor_cache_flush(neigh_cache_s *neigh_cache)
{
    (void)neigh_cache;
    return 0;
}

#endif // HAVE_NEIGHBOR_CACHE
