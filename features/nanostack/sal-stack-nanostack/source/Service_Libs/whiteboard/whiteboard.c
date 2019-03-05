/*
 * Copyright (c) 2013-2018, Arm Limited and affiliates.
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
#include <stdint.h>
#include "string.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "Common_Protocols/icmpv6.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "platform/os_whiteboard.h"

#ifdef WHITEBOARD
static uint16_t whiteboard_device_limit = 0;
static uint16_t whiteboard_size = 0;

/* Separate internal version to avoid exporting ns_list.h unnecessarily */
typedef struct whiteboard_entry_int_t {
    whiteboard_entry_t entry;
    ns_list_link_t link;
} whiteboard_entry_int_t;

static NS_LIST_DEFINE(whiteboard_info, whiteboard_entry_int_t, link);
static NS_LIST_DEFINE(whiteboard_interface_list, whiteboard_entry_int_t, link);

static void whiteboard_remove_entry(whiteboard_entry_int_t *entry);

static void whiteboard_remove_entry(whiteboard_entry_int_t *entry)
{
    ns_list_remove(&whiteboard_info, entry);
    ns_dyn_mem_free(entry);
    whiteboard_size--;
}

void whiteboard_init(int8_t id)
{
    ns_list_foreach_safe(whiteboard_entry_int_t, cur, &whiteboard_info) {
        if (cur->entry.interface_index == id) {
            whiteboard_remove_entry(cur);
        }
    }
}


void whiteboard_init_by_prefix(int8_t id, const uint8_t address[static 8])
{
    ns_list_foreach_safe(whiteboard_entry_int_t, cur, &whiteboard_info) {
        if (cur->entry.interface_index == id) {
            if (memcmp(cur->entry.address, address, 8) == 0) {
                whiteboard_remove_entry(cur);
            }
        }
    }
}

void whiteboard_rm_entry(int8_t id, const uint8_t address[static 16])
{
    ns_list_foreach_safe(whiteboard_entry_int_t, cur, &whiteboard_info) {
        if (cur->entry.interface_index == id) {
            if (memcmp(cur->entry.address, address, 16) == 0) {

                whiteboard_remove_entry(cur);
                return;
            }
        }
    }
}

whiteboard_entry_t *whiteboard_table_check_address(const uint8_t address[static 16])
{
    ns_list_foreach(whiteboard_entry_int_t, cur, &whiteboard_info) {
        if (memcmp(cur->entry.address, address, 16) == 0) {
            return &cur->entry;
        }
    }
    return 0;
}

whiteboard_entry_t *whiteboard_get(whiteboard_entry_t *cur)
{
    whiteboard_entry_int_t *entry = (whiteboard_entry_int_t *) cur;
    if (entry) {
        entry = ns_list_get_next(&whiteboard_info, entry);
    } else {
        entry = ns_list_get_first(&whiteboard_info);
    }

    if (entry) {
        return &entry->entry;
    } else {
        return NULL;
    }
}

void whiteboard_set_device_hard_limit(uint16_t limit)
{
    whiteboard_device_limit = limit;
}

uint16_t whiteboard_size_get(void)
{
    return whiteboard_size;
}

// XXX what is this doing? I believe it only returns entries that do NOT
// appear to be based on the EUI-64 (with a sloppy check). So given entries:
//
// 2002:db8::2345:2345:2345:2345    03:45:23:45:23:45:23:45
// 2002:db8::1                      03:45:23:45:23:45:23:45
//
// it returns the 2002:db8::1 entry, not the EUI-64-based one.
//
// But why?
static whiteboard_entry_t *whiteboard_table_check_eui64(const uint8_t eui64[static 8])
{
    ns_list_foreach(whiteboard_entry_int_t, cur, &whiteboard_info) {
        /*Compare latter 7 bytes and eui64*/
        if (memcmp(cur->entry.address + 9, eui64 + 1, 7) != 0) {
            if (memcmp(cur->entry.eui64, eui64, 8) == 0) {
                return &cur->entry;
            }
        }
    }
    return 0;
}


int8_t whiteboard_interface_register(const uint8_t address[static 16], int8_t nwk_id)
{
    ns_list_foreach(whiteboard_entry_int_t, cur, &whiteboard_interface_list) {
        if (memcmp(cur->entry.address, address, 16) == 0) {
            if (cur->entry.interface_index != nwk_id) {
                return -2;
            } else {
                return 0;
            }
        }
    }

    whiteboard_entry_int_t *new = ns_dyn_mem_alloc(sizeof(whiteboard_entry_int_t));
    if (!new) {
        return -1;
    }
    memcpy(new->entry.address, address, 16);
    new->entry.interface_index = nwk_id;
    whiteboard_os_modify(address, ADD);
    ns_list_add_to_start(&whiteboard_interface_list, new);

    return 0;
}

int8_t whiteboard_interface_unregister_all_address(int8_t nwk_id)
{
    ns_list_foreach_safe(whiteboard_entry_int_t, cur, &whiteboard_interface_list) {
        if (cur->entry.interface_index == nwk_id) {
            whiteboard_os_modify(cur->entry.address, REMOVE);
            whiteboard_remove_entry(cur);
        }
    }
    return 0;
}

bool whiteboard_interface_address_cmp(const uint8_t address[static 16])
{
    ns_list_foreach(whiteboard_entry_int_t, cur, &whiteboard_interface_list) {
        if (memcmp(cur->entry.address, address, 16) == 0) {
            return true;
        }
    }
    return false;
}

whiteboard_entry_t *whiteboard_table_update(const uint8_t address[static 16], const uint8_t eui64[static 8], uint8_t *status)
{
    whiteboard_entry_t *ret_val = 0;
    if (ns_list_is_empty(&whiteboard_interface_list)) {
        return 0;
    }

    if (whiteboard_interface_address_cmp(address)) {
        *status = ARO_DUPLICATE;
        return 0;
    }

    ret_val = whiteboard_table_check_address(address);
    if (ret_val) {
        if (memcmp(ret_val->eui64, eui64, 8) == 0) {
            *status = ARO_SUCCESS;
        } else {
            *status = ARO_DUPLICATE;
            return 0;
        }
    } else {
        /*Compare latter 7 bytes and eui64 (XXX why?) */
        if (memcmp((address + 9), (eui64 + 1), 7) != 0) {
            ret_val = whiteboard_table_check_eui64(eui64);
            if (ret_val) {
                memcpy(ret_val->address, address, 16);
                *status = ARO_SUCCESS;
            }
        }
    }

    //allocate new
    if (ret_val == 0) {
        /* Check Limiter */
        if (whiteboard_device_limit && whiteboard_size == whiteboard_device_limit) {
            *status = ARO_FULL;
            return 0;
        }

        whiteboard_entry_int_t *new = ns_dyn_mem_alloc(sizeof(whiteboard_entry_int_t));
        if (new) {
            ns_list_add_to_start(&whiteboard_info, new);
            ret_val = &new->entry;
            whiteboard_os_modify(address, ADD);
            memcpy(ret_val->address, address, 16);
            memcpy(ret_val->eui64, eui64, 8);
            *status = ARO_SUCCESS;
            whiteboard_size++;
        } else {
            *status = ARO_FULL;
        }
    }

    return ret_val;
}

void whiteboard_ttl_update(uint16_t ttl_time)
{
    ns_list_foreach_safe(whiteboard_entry_int_t, cur, &whiteboard_info) {
        if (cur->entry.ttl > ttl_time) {
            cur->entry.ttl -= ttl_time;
        } else {
            whiteboard_os_modify(cur->entry.address, REMOVE);
            whiteboard_remove_entry(cur);
        }
    }
}
#else

void whiteboard_set_device_hard_limit(uint16_t limit)
{
    (void)limit;
}

whiteboard_entry_t *whiteboard_get(whiteboard_entry_t *cur)
{
    (void)cur;
    return NULL;
}

#endif
