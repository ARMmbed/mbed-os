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
#include "string.h"
#include "ns_types.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "Core/include/ns_address_internal.h"
#include "platform/topo_trace.h"

mac_neighbor_table_t *mac_neighbor_table_create(uint8_t table_size, neighbor_entry_remove_notify *remove_cb, neighbor_entry_nud_notify *nud_cb, void *user_indentifier)
{
    mac_neighbor_table_t *table_class = ns_dyn_mem_alloc(sizeof(mac_neighbor_table_t) + sizeof(mac_neighbor_table_entry_t) * table_size);
    if (!table_class) {
        return NULL;
    }
    memset(table_class, 0, sizeof(mac_neighbor_table_t));

    mac_neighbor_table_entry_t *cur_ptr = &table_class->neighbor_entry_buffer[0];
    table_class->list_total_size = table_size;
    table_class->table_user_identifier = user_indentifier;
    table_class->user_nud_notify_cb = nud_cb;
    table_class->user_remove_notify_cb = remove_cb;
    ns_list_init(&table_class->neighbour_list);
    ns_list_init(&table_class->free_list);
    for (uint8_t i = 0; i < table_size; i++) {
        memset(cur_ptr, 0, sizeof(mac_neighbor_table_entry_t));
        cur_ptr->index = i;
        //Add to list
        ns_list_add_to_end(&table_class->free_list, cur_ptr);
        cur_ptr++;
    }

    return table_class;

}

void mac_neighbor_table_delete(mac_neighbor_table_t *table_class)
{
    mac_neighbor_table_neighbor_list_clean(table_class);
    ns_dyn_mem_free(table_class);
}

static void neighbor_table_class_remove_entry(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *entry)
{
    ns_list_remove(&table_class->neighbour_list, entry);
    table_class->neighbour_list_size--;
    if (entry->nud_active) {
        entry->nud_active = false;
        table_class->active_nud_process--;
    }

    if (table_class->user_remove_notify_cb) {
        table_class->user_remove_notify_cb(entry, table_class->table_user_identifier);
    }
    topo_trace(TOPOLOGY_MLE, entry->mac64, TOPO_REMOVE);


    uint8_t index = entry->index;
    memset(entry, 0, sizeof(mac_neighbor_table_entry_t));
    entry->index = index;
    ns_list_add_to_end(&table_class->free_list, entry);
}

void mac_neighbor_table_neighbor_list_clean(mac_neighbor_table_t *table_class)
{
    if (!table_class) {
        return;
    }
    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur, &table_class->neighbour_list) {
        neighbor_table_class_remove_entry(table_class, cur);
    }
    topo_trace(TOPOLOGY_MLE, NULL, TOPO_CLEAR);
}


void mac_neighbor_table_neighbor_timeout_update(mac_neighbor_table_t *table_class, uint32_t time_update)
{
    if (!table_class) {
        return;
    }

    ns_list_foreach_safe(mac_neighbor_table_entry_t, cur, &table_class->neighbour_list) {

        if (cur->lifetime > time_update) {
            cur->lifetime -= time_update;
            if (!table_class->user_nud_notify_cb ||  table_class->active_nud_process > ACTIVE_NUD_PROCESS_MAX || cur->nud_active || !cur->rx_on_idle) {
                continue;
            }

            if (table_class->user_nud_notify_cb(cur, table_class->table_user_identifier)) {
                table_class->active_nud_process++;
                cur->nud_active = true;
            }

        } else {
            neighbor_table_class_remove_entry(table_class, cur);
        }
    }
}


mac_neighbor_table_entry_t *mac_neighbor_table_entry_allocate(mac_neighbor_table_t *table_class, const uint8_t *mac64)
{
    if (!table_class) {
        return NULL;
    }
    mac_neighbor_table_entry_t *entry = ns_list_get_first(&table_class->free_list);
    if (!entry) {
        return NULL;
    }
    //Remove from the list
    ns_list_remove(&table_class->free_list, entry);
    //Add to list
    ns_list_add_to_end(&table_class->neighbour_list, entry);
    table_class->neighbour_list_size++;
    memcpy(entry->mac64, mac64, 8);
    entry->mac16 = 0xffff;
    entry->rx_on_idle = true;
    entry->ffd_device = true;
    entry->nud_active = false;
    entry->advertisment = false;
    entry->connected_device = false;
    entry->trusted_device = false;
    entry->lifetime = NEIGHBOR_CLASS_LINK_DEFAULT_LIFETIME;
    entry->link_lifetime = NEIGHBOR_CLASS_LINK_DEFAULT_LIFETIME;
    entry->link_role = NORMAL_NEIGHBOUR;
    topo_trace(TOPOLOGY_MLE, mac64, TOPO_ADD);
    return entry;
}

static mac_neighbor_table_entry_t *neighbor_table_class_entry_validate(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *neighbor_entry)
{
    ns_list_foreach(mac_neighbor_table_entry_t, cur, &table_class->neighbour_list) {
        if (cur == neighbor_entry) {
            return cur;
        }
    }
    return NULL;

}

void mac_neighbor_table_neighbor_remove(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *neighbor_entry)
{
    mac_neighbor_table_entry_t *entry = neighbor_table_class_entry_validate(table_class, neighbor_entry);
    if (entry) {
        neighbor_table_class_remove_entry(table_class, entry);
    }
}


void mac_neighbor_table_neighbor_refresh(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *neighbor_entry, uint32_t life_time)
{
    neighbor_entry->lifetime = life_time;
    neighbor_entry->link_lifetime = life_time;
    if (neighbor_entry->nud_active) {
        neighbor_entry->nud_active = false;
        table_class->active_nud_process--;
    }

}

void mac_neighbor_table_neighbor_connected(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *neighbor_entry)
{
    (void)table_class;
    neighbor_entry->connected_device = true;
}

void mac_neighbor_table_trusted_neighbor(mac_neighbor_table_t *table_class, mac_neighbor_table_entry_t *neighbor_entry, bool trusted_device)
{
    (void)table_class;
    if (!neighbor_entry->trusted_device && trusted_device) {
        neighbor_entry->lifetime = neighbor_entry->link_lifetime;
    }
    neighbor_entry->trusted_device = trusted_device;
}

mac_neighbor_table_entry_t *mac_neighbor_table_address_discover(mac_neighbor_table_t *table_class, const uint8_t *address, uint8_t address_type)
{
    if (!table_class) {
        return NULL;
    }
    uint16_t short_address;
    if (address_type == ADDR_802_15_4_SHORT) {
        short_address = common_read_16_bit(address);
    } else if (address_type == ADDR_802_15_4_LONG) {

    } else {
        return NULL;
    }

    ns_list_foreach(mac_neighbor_table_entry_t, cur, &table_class->neighbour_list) {
        if (address_type == ADDR_802_15_4_SHORT) {
            if (cur->mac16 != 0xffff && cur->mac16 == short_address) {
                return cur;
            }
        } else {
            if (memcmp(cur->mac64, address, 8) == 0) {
                return cur;
            }
        }
    }

    return NULL;
}

mac_neighbor_table_entry_t *mac_neighbor_table_attribute_discover(mac_neighbor_table_t *table_class, uint8_t index)
{
    ns_list_foreach(mac_neighbor_table_entry_t, cur, &table_class->neighbour_list) {

        if (cur->index == index) {
            return cur;
        }
    }
    return NULL;
}

mac_neighbor_table_entry_t *mac_neighbor_entry_get_by_ll64(mac_neighbor_table_t *table_class, const uint8_t *ipv6Address, bool allocateNew, bool *new_entry_allocated)
{
    // Check it really is LL64 (not LL16)
    if (memcmp(ipv6Address, ADDR_LINK_LOCAL_PREFIX, 8) != 0) {
        return NULL;    //Mot Link Local Address
    }

    if (memcmp((ipv6Address + 8), ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
        return NULL;
    }
    // map
    uint8_t temporary_mac64[8];
    memcpy(temporary_mac64, (ipv6Address + 8), 8);
    temporary_mac64[0] ^= 2;

    return mac_neighbor_entry_get_by_mac64(table_class, temporary_mac64, allocateNew, new_entry_allocated);

}

mac_neighbor_table_entry_t *mac_neighbor_entry_get_by_mac64(mac_neighbor_table_t *table_class, const uint8_t *mac64, bool allocateNew, bool *new_entry_allocated)
{
    mac_neighbor_table_entry_t *entry = mac_neighbor_table_address_discover(table_class, mac64, ADDR_802_15_4_LONG);
    if (entry || !allocateNew) {
        if (new_entry_allocated) {
            *new_entry_allocated = false;
        }
        return entry;
    }

    if (new_entry_allocated) {
        *new_entry_allocated = true;
    }

    return mac_neighbor_table_entry_allocate(table_class, mac64);
}

mac_neighbor_table_entry_t *mac_neighbor_entry_get_priority(mac_neighbor_table_t *table_class)
{

    ns_list_foreach(mac_neighbor_table_entry_t, entry, &table_class->neighbour_list) {
        if (entry->link_role == PRIORITY_PARENT_NEIGHBOUR) {
            return entry;
        }
    }
    return NULL;
}
