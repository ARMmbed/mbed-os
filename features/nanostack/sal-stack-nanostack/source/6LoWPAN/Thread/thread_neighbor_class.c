/*
 * Copyright (c) 2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_neighbor_class.h"


static thread_neigh_table_entry_t *thread_neighbor_class_table_entry_get(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    if (!class_ptr->neigh_info_list || attribute_index >= class_ptr->list_size) {
        return NULL;
    }

    thread_neigh_table_entry_t *entry = class_ptr->neigh_info_list + attribute_index;
    return entry;
}

bool thread_neighbor_class_create(thread_neighbor_class_t *class_ptr, uint8_t neigh_table_size)
{
    class_ptr->neigh_info_list = ns_dyn_mem_alloc(sizeof(thread_neigh_table_entry_t) * neigh_table_size);
    if (!class_ptr->neigh_info_list) {
        return false;
    }

    class_ptr->list_size = neigh_table_size;
    thread_neigh_table_entry_t *list_ptr = class_ptr->neigh_info_list;
    for (uint8_t i = 0; i < neigh_table_size; i++) {
        memset(list_ptr, 0, sizeof(thread_neigh_table_entry_t));
        list_ptr++;
    }
    return true;
}

void thread_neighbor_class_delete(thread_neighbor_class_t *class_ptr)
{
    ns_dyn_mem_free(class_ptr->neigh_info_list);
    class_ptr->neigh_info_list = NULL;
    class_ptr->list_size = 0;
}

void thread_neighbor_class_add_mleid(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, const uint8_t *mleid)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return;
    }
    memcpy(entry->mlEid, mleid, 8);
}

uint8_t *thread_neighbor_class_get_mleid(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return NULL;
    }
    return entry->mlEid;
}

void thread_neighbor_last_communication_time_update(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return;
    }
    entry->last_contact_time = protocol_core_monotonic_time;
}

void thread_neighbor_class_update_link(thread_neighbor_class_t *class_ptr, uint8_t attribute_index, uint8_t linkmargin, bool new_link)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return;
    }

    if (new_link) {
        entry->link_margin = entry->link_margin + linkmargin - (entry->link_margin >> THREAD_LINK_MARGIN_SCALING);
    } else {
        entry->link_margin = linkmargin << THREAD_LINK_MARGIN_SCALING;
    }
}

uint16_t thread_neighbor_entry_linkmargin_get(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return 0;
    }
    return entry->link_margin;
}

uint32_t thread_neighbor_last_communication_time_get(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return 0;
    }
    return entry->last_contact_time;
}

bool thread_neighbor_class_mleid_compare(thread_neighbor_class_t *class_ptr, uint8_t attribute_index, const uint8_t *mleid)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry || memcmp(entry->mlEid, mleid, 8)) {
        return false;
    }
    return true;
}

bool thread_neighbor_class_request_full_data_setup(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return false;
    }
    return entry->request_full_data_set;

}

bool thread_neighbor_class_secured_data_request(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (!entry) {
        return false;
    }
    return entry->secured_data_request;
}

void thread_neighbor_class_request_full_data_setup_set(thread_neighbor_class_t *class_ptr, uint8_t attribute_index, bool value)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (entry) {
        entry->request_full_data_set = value;
    }
}

void thread_neighbor_class_secured_data_request_set(thread_neighbor_class_t *class_ptr, uint8_t attribute_index, bool value)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (entry) {
        entry->secured_data_request = value;
    }
}

void thread_neighbor_class_mode_parse_to_entry(thread_neighbor_class_t *class_ptr, uint8_t attribute_index, uint8_t mode)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (entry) {
        entry->request_full_data_set = mode & MLE_THREAD_REQ_FULL_DATA_SET;
        entry->secured_data_request = mode & MLE_THREAD_SECURED_DATA_REQUEST;
    }
}

uint8_t thread_neighbor_class_mode_write_from_entry(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    uint8_t mode = 0;
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (entry) {
        if (entry->request_full_data_set) {
            mode |= MLE_THREAD_REQ_FULL_DATA_SET;
        }

        if (entry->secured_data_request) {
            mode |= MLE_THREAD_SECURED_DATA_REQUEST;
        }
    }
    return mode;
}


void thread_neighbor_class_entry_remove(thread_neighbor_class_t *class_ptr, uint8_t attribute_index)
{
    thread_neigh_table_entry_t *entry = thread_neighbor_class_table_entry_get(class_ptr, attribute_index);
    if (entry) {
        memset(entry, 0, sizeof(thread_neigh_table_entry_t));
    }
}
