/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#include "common_functions.h"
#include "ccmLIB.h"
#include "nsdynmemLIB.h"
#include "Core/include/ns_address_internal.h"
#include "Core/include/ns_buffer.h"
#include "MLE/mle.h"
#include "mac_common_defines.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/mle_service/mle_service_frame_counter_table.h"



typedef struct {
    mle_neighbor_security_counter_info_t *security_counter_list;
    uint8_t table_size;
    int8_t interface_id;
    ns_list_link_t link;
} mle_service_framecounter_instance_list_t;

typedef NS_LIST_HEAD(mle_service_framecounter_instance_list_t, link) mle_service_counter_list_t;

static mle_service_counter_list_t srv_framecounter_instance_list;


static mle_service_framecounter_instance_list_t *mle_service_framecounter_params_get(int8_t interface_id)
{
    ns_list_foreach(mle_service_framecounter_instance_list_t, cur_ptr, &srv_framecounter_instance_list) {
        if (cur_ptr->interface_id == interface_id) {
            return cur_ptr;
        }
    }
    return NULL;
}

static void mle_service_framecounter_delete(mle_neighbor_security_counter_info_t *cur_ptr)
{
    cur_ptr->last_key_sequence = 0;
    cur_ptr->mle_frame_counter = 0;
    cur_ptr->new_key_pending = false;
}


mle_neighbor_security_counter_info_t *mle_service_counter_info_get(int8_t interface_id, uint8_t attribute_index)
{
    mle_service_framecounter_instance_list_t *srv_ptr = mle_service_framecounter_params_get(interface_id);

    if (!srv_ptr || !srv_ptr->security_counter_list || attribute_index >= srv_ptr->table_size) {
        return NULL;
    }
    mle_neighbor_security_counter_info_t *entry = srv_ptr->security_counter_list + attribute_index;
    return entry;
}

int mle_service_frame_counter_table_free(int8_t interface_id)
{
    mle_service_framecounter_instance_list_t *srv_ptr = mle_service_framecounter_params_get(interface_id);
    if (!srv_ptr) {
        return -1;
    }
    ns_list_remove(&srv_framecounter_instance_list, srv_ptr);
    ns_dyn_mem_free(srv_ptr->security_counter_list);
    ns_dyn_mem_free(srv_ptr);
    return 0;
}

int mle_service_frame_counter_table_allocate(int8_t interface_id, uint8_t table_size)
{

    mle_service_framecounter_instance_list_t *srv_ptr = mle_service_framecounter_params_get(interface_id);
    if (!srv_ptr) {
        srv_ptr = ns_dyn_mem_alloc(sizeof(mle_service_framecounter_instance_list_t));
        if (!srv_ptr) {
            return -1;
        }
        ns_list_add_to_start(&srv_framecounter_instance_list, srv_ptr);
        srv_ptr->interface_id = interface_id;
        srv_ptr->table_size = 0;
        srv_ptr->security_counter_list = NULL;
    }

    if (srv_ptr->table_size != table_size) {
        ns_dyn_mem_free(srv_ptr->security_counter_list);
        srv_ptr->security_counter_list = ns_dyn_mem_alloc(sizeof(mle_neighbor_security_counter_info_t) * table_size);
    }

    if (!srv_ptr->security_counter_list) {
        mle_service_frame_counter_table_free(interface_id);
        return -1;
    }
    srv_ptr->table_size = table_size;

    mle_neighbor_security_counter_info_t *cur_ptr = srv_ptr->security_counter_list;
    for (uint8_t i = 0; i < table_size; i++) {
        mle_service_framecounter_delete(cur_ptr);
        cur_ptr++;
    }
    return 0;
}

void mle_service_frame_counter_entry_add(int8_t interface_id, uint8_t attribute_index, uint32_t frame_counter)
{
    mle_neighbor_security_counter_info_t *entry = mle_service_counter_info_get(interface_id, attribute_index);
    if (entry) {
        entry->mle_frame_counter = frame_counter;
    }
}

void mle_service_frame_counter_entry_new_key_pending_set(int8_t interface_id, uint8_t attribute_index)
{
    mle_neighbor_security_counter_info_t *entry = mle_service_counter_info_get(interface_id, attribute_index);
    if (entry) {
        entry->new_key_pending = true;
    }
}

void mle_service_frame_counter_entry_delete(int8_t interface_id, uint8_t attribute_index)
{
    mle_neighbor_security_counter_info_t *entry = mle_service_counter_info_get(interface_id, attribute_index);
    if (entry) {
        mle_service_framecounter_delete(entry);
    }

}

uint32_t mle_service_neighbor_frame_counter_get(int8_t interface_id, uint8_t attribute_index)
{
    mle_neighbor_security_counter_info_t *entry = mle_service_counter_info_get(interface_id, attribute_index);
    if (!entry) {
        return 0;
    }
    return entry->mle_frame_counter;

}

