/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "pan_blacklist_api.h"

#define TRACE_GROUP "pbl"
static pan_blaclist_entry_t *pan_blacklist_entry_find(pan_blaclist_cache_s *list_ptr, uint16_t pan_id);
static pan_blaclist_entry_t *pan_blacklist_entry_allocate(pan_blaclist_cache_s *list_ptr);
static pan_coordinator_blacklist_entry_t *pan_cordinator_blacklist_entry_find(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *compare_data);
static pan_coordinator_blacklist_entry_t *pan_coordinator_blacklist_entry_allocate(pan_coordinator_blaclist_cache_s *list_ptr);

void pan_blacklist_cache_init(pan_blaclist_cache_s *blacklist_cache)
{
    ns_list_init(&blacklist_cache->head);
}

void pan_coordinator_blacklist_cache_init(pan_coordinator_blaclist_cache_s *blacklist_cache)
{
    ns_list_init(&blacklist_cache->head);
}

void pan_blacklist_pan_set(pan_blaclist_cache_s *list_ptr, uint16_t panid, uint16_t timeout)
{
    pan_blaclist_entry_t *entry = pan_blacklist_entry_find(list_ptr, panid);
    if (!entry) {
        //Allocate entry
        entry = pan_blacklist_entry_allocate(list_ptr);
    }

    if(entry) {
        entry->pan_id = panid;
        entry->timeout_in_seconds = timeout;
    }
}

void pan_cordinator_blacklist_pan_set(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *cordinator_data, uint16_t timeout)
{
    pan_coordinator_blacklist_entry_t *entry = pan_cordinator_blacklist_entry_find(list_ptr, cordinator_data);
    if (!entry) {
        //Allocate entry
        entry = pan_coordinator_blacklist_entry_allocate(list_ptr);
    }

    if(entry) {
        memcpy(entry->coordinator_pan_address, cordinator_data, 10);
        entry->timeout_in_seconds = timeout;
    }
}

void pan_blacklist_time_update(pan_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds)
{
    ns_list_foreach_safe(pan_blaclist_entry_t, cur_ptr, &list_ptr->head) {
        if (cur_ptr->timeout_in_seconds > time_update_in_seconds) {
            cur_ptr->timeout_in_seconds -= time_update_in_seconds;
        } else {
            ns_list_remove(&list_ptr->head, cur_ptr);
            ns_dyn_mem_free(cur_ptr);
        }
    }
}

void pan_coordinator_blacklist_time_update(pan_coordinator_blaclist_cache_s *list_ptr, uint16_t time_update_in_seconds)
{
    ns_list_foreach_safe(pan_coordinator_blacklist_entry_t, cur_ptr, &list_ptr->head) {
        if (cur_ptr->timeout_in_seconds > time_update_in_seconds) {
            cur_ptr->timeout_in_seconds -= time_update_in_seconds;
        } else {
            ns_list_remove(&list_ptr->head, cur_ptr);
            ns_dyn_mem_free(cur_ptr);
        }
    }
}

void pan_coordinator_blacklist_free(pan_coordinator_blaclist_cache_s *list_ptr)
{
    ns_list_foreach_safe(pan_coordinator_blacklist_entry_t, cur_ptr, &list_ptr->head) {
        ns_list_remove(&list_ptr->head, cur_ptr);
        ns_dyn_mem_free(cur_ptr);
    }
}

bool pan_blacklist_filter(pan_blaclist_cache_s *list_ptr, uint16_t panid)
{
    if (pan_blacklist_entry_find(list_ptr, panid) ) {
        return true;
    }
    return false;
}

bool pan_cordinator_blacklist_filter(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *compare_data)
{
    if (pan_cordinator_blacklist_entry_find(list_ptr, compare_data) ) {
        return true;
    }
    return false;
}


static pan_blaclist_entry_t *pan_blacklist_entry_find(pan_blaclist_cache_s *list_ptr, uint16_t pan_id)
{
    ns_list_foreach(pan_blaclist_entry_t, cur_ptr, &list_ptr->head) {
        if (cur_ptr->pan_id == pan_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

static pan_blaclist_entry_t *pan_blacklist_entry_allocate(pan_blaclist_cache_s *list_ptr)
{
    pan_blaclist_entry_t *entry = ns_dyn_mem_alloc(sizeof(pan_blaclist_entry_t));
    if (entry) {
        //Add to list
        ns_list_add_to_end(&list_ptr->head, entry);
    }
    return entry;
}

static pan_coordinator_blacklist_entry_t *pan_cordinator_blacklist_entry_find(pan_coordinator_blaclist_cache_s *list_ptr, uint8_t *compare_data)
{
    ns_list_foreach(pan_coordinator_blacklist_entry_t, cur_ptr, &list_ptr->head) {
        if (memcmp(cur_ptr->coordinator_pan_address, compare_data, 10) == 0) {
            return cur_ptr;
        }
    }
    return NULL;
}

static pan_coordinator_blacklist_entry_t *pan_coordinator_blacklist_entry_allocate(pan_coordinator_blaclist_cache_s *list_ptr)
{
    pan_coordinator_blacklist_entry_t *entry = ns_dyn_mem_alloc(sizeof(pan_coordinator_blacklist_entry_t));
    if (entry) {
        //Add to list
        ns_list_add_to_end(&list_ptr->head, entry);
    }
    return entry;
}



