/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "fhss_config.h"
#include "ws_management_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_lib.h"
#include "6LoWPAN/ws/ws_pae_key_storage.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wspl"

void ws_pae_lib_kmp_list_init(kmp_list_t *kmp_list)
{
    ns_list_init(kmp_list);
}

kmp_entry_t *ws_pae_lib_kmp_list_add(kmp_list_t *kmp_list, kmp_api_t *kmp)
{
    // Already in list
    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(kmp_list, kmp);
    if (entry) {
        return entry;
    }

    entry = ns_dyn_mem_alloc(sizeof(kmp_entry_t));
    if (!entry) {
        return NULL;
    }
    entry->kmp = kmp;
    entry->timer_running = false;

    ns_list_add_to_end(kmp_list, entry);

    return entry;
}

int8_t ws_pae_lib_kmp_list_delete(kmp_list_t *kmp_list, kmp_api_t *kmp)
{
    kmp_entry_t *entry = ws_pae_lib_kmp_list_entry_get(kmp_list, kmp);

    if (entry) {
        ns_list_remove(kmp_list, entry);
        kmp_api_delete(entry->kmp);
        ns_dyn_mem_free(entry);
        return 0;
    }

    return -1;
}

kmp_api_t *ws_pae_lib_kmp_list_type_get(kmp_list_t *kmp_list, kmp_type_e type)
{
    kmp_api_t *kmp = NULL;

    ns_list_foreach(kmp_entry_t, cur, kmp_list) {
        // If kmp type matches
        if (kmp_api_type_get(cur->kmp) == type) {
            /* If receiving of messages has not been disabled for the kmp (kmp is not
               in terminating phase) prioritizes that kmp */
            if (!kmp_api_receive_disable(cur->kmp)) {
                return cur->kmp;
            }
            // Otherwise returns any kmp that matches
            kmp = cur->kmp;
        }
    }
    return kmp;
}

kmp_api_t *ws_pae_lib_kmp_list_instance_id_get(kmp_list_t *kmp_list, uint8_t instance_id)
{
    ns_list_foreach(kmp_entry_t, cur, kmp_list) {
        if (kmp_api_instance_id_get(cur->kmp) == instance_id) {
            return cur->kmp;
        }
    }

    return NULL;
}

void ws_pae_lib_kmp_list_free(kmp_list_t *kmp_list)
{
    ns_list_foreach_safe(kmp_entry_t, cur, kmp_list) {
        kmp_api_delete(cur->kmp);
        ns_dyn_mem_free(cur);
    }
}

kmp_entry_t *ws_pae_lib_kmp_list_entry_get(kmp_list_t *kmp_list, kmp_api_t *kmp)
{
    ns_list_foreach(kmp_entry_t, cur, kmp_list) {
        if (cur->kmp == kmp) {
            return cur;
        }
    }

    return 0;
}

bool ws_pae_lib_kmp_list_empty(kmp_list_t *kmp_list)
{
    return ns_list_is_empty(kmp_list);
}

uint8_t ws_pae_lib_kmp_list_count(kmp_list_t *kmp_list)
{
    return ns_list_count(kmp_list);
}

void ws_pae_lib_kmp_timer_start(kmp_list_t *kmp_list, kmp_entry_t *entry)
{
    if (ns_list_get_first(kmp_list) != entry) {
        ns_list_remove(kmp_list, entry);
        ns_list_add_to_start(kmp_list, entry);
    }
    entry->timer_running = true;
}

void ws_pae_lib_kmp_timer_stop(kmp_list_t *kmp_list, kmp_entry_t *entry)
{
    if (ns_list_get_last(kmp_list) != entry) {
        ns_list_remove(kmp_list, entry);
        ns_list_add_to_end(kmp_list, entry);
    }
    entry->timer_running = false;
}

bool ws_pae_lib_kmp_timer_update(kmp_list_t *kmp_list, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout)
{
    if (ns_list_is_empty(kmp_list)) {
        return false;
    }

    bool timer_running = false;

    ns_list_foreach_safe(kmp_entry_t, entry, kmp_list) {
        if (entry->timer_running) {
            timeout(entry->kmp, ticks);
            timer_running = true;
        } else {
            break;
        }
    }

    return timer_running;
}

void ws_pae_lib_supp_list_init(supp_list_t *supp_list)
{
    ns_list_init(supp_list);
}

supp_entry_t *ws_pae_lib_supp_list_add(supp_list_t *supp_list, const kmp_addr_t *addr)
{
    supp_entry_t *entry = ns_dyn_mem_temporary_alloc(sizeof(supp_entry_t));

    if (!entry) {
        return NULL;
    }

    ws_pae_lib_supp_init(entry);
    memset(&entry->addr, 0, sizeof(kmp_addr_t));
    entry->addr.type = KMP_ADDR_EUI_64_AND_IP;
    kmp_address_copy(&entry->addr, addr);

    ns_list_add_to_start(supp_list, entry);

    return entry;
}

int8_t ws_pae_lib_supp_list_remove(void *instance, supp_list_t *supp_list, supp_entry_t *supp, ws_pae_lib_supp_deleted supp_deleted)
{
    ns_list_remove(supp_list, supp);

    ws_pae_lib_supp_delete(supp);

    ns_dyn_mem_free(supp);

    if (supp_deleted != NULL) {
        supp_deleted(instance);
    }

    return 0;
}

supp_entry_t *ws_pae_lib_supp_list_entry_eui_64_get(const supp_list_t *supp_list, const uint8_t *eui_64)
{
    ns_list_foreach(supp_entry_t, cur, supp_list) {
        if (memcmp(cur->addr.eui_64, eui_64, 8) == 0) {
            return cur;
        }
    }

    return 0;
}

void ws_pae_lib_supp_list_delete(supp_list_t *supp_list)
{
    ns_list_foreach_safe(supp_entry_t, entry, supp_list) {
        ws_pae_lib_supp_list_remove(NULL, supp_list, entry, NULL);
    }
}

bool ws_pae_lib_supp_list_timer_update(void *instance, supp_list_t *active_supp_list, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout, ws_pae_lib_supp_deleted supp_deleted)
{
    bool timer_running = false;

    ns_list_foreach_safe(supp_entry_t, entry, active_supp_list) {
        bool running = ws_pae_lib_supp_timer_update(instance, entry, ticks, timeout);
        if (running) {
            timer_running = true;
        } else {
            ws_pae_lib_supp_list_to_inactive(instance, active_supp_list, entry, supp_deleted);
        }
    }

    return timer_running;
}

void ws_pae_lib_supp_list_slow_timer_update(supp_list_t *supp_list, uint16_t seconds)
{
    ns_list_foreach(supp_entry_t, entry, supp_list) {
        if (sec_prot_keys_pmk_lifetime_decrement(&entry->sec_keys, seconds)) {
            tr_info("PMK and PTK expired, eui-64: %s, system time: %"PRIu32"", trace_array(entry->addr.eui_64, 8), protocol_core_monotonic_time / 10);
        }
        if (sec_prot_keys_ptk_lifetime_decrement(&entry->sec_keys, seconds)) {
            tr_info("PTK expired, eui-64: %s, system time: %"PRIu32"", trace_array(entry->addr.eui_64, 8), protocol_core_monotonic_time / 10);
        }
    }
}

void ws_pae_lib_supp_init(supp_entry_t *entry)
{
    ws_pae_lib_kmp_list_init(&entry->kmp_list);
    memset(&entry->addr, 0, sizeof(kmp_addr_t));
    memset(&entry->sec_keys, 0, sizeof(sec_prot_keys_t));
    entry->ticks = 0;
    entry->waiting_ticks = 0;
    entry->store_ticks = ws_pae_key_storage_storing_interval_get() * 1000;
    entry->active = true;
    entry->access_revoked = false;
}

void ws_pae_lib_supp_delete(supp_entry_t *entry)
{
    ws_pae_lib_kmp_list_free(&entry->kmp_list);
}

bool ws_pae_lib_supp_timer_update(void *instance, supp_entry_t *entry, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout)
{
    // Updates KMP timers and calls timeout callback
    bool keep_timer_running = ws_pae_lib_kmp_timer_update(&entry->kmp_list, ticks, timeout);

    // If KMPs are not active updates supplicant timer
    if (!keep_timer_running) {
        if (entry->ticks > ticks) {
            keep_timer_running = true;
            entry->ticks -= ticks;
        } else {
            entry->ticks = 0;
        }
    }

    // Updates retry timer
    if (entry->waiting_ticks > ticks) {
        keep_timer_running = true;
        entry->waiting_ticks -= ticks;
    } else {
        if (entry->waiting_ticks > 0) {
            tr_info("Waiting supplicant timeout eui-64: %s", trace_array(entry->addr.eui_64, 8));
        }
        entry->waiting_ticks = 0;
    }

    if (!instance) {
        return keep_timer_running;
    }

    // Updates retry timer
    if (entry->store_ticks > ticks) {
        entry->store_ticks -= ticks;
    } else {
        tr_info("PAE active entry key storage update timeout");
        ws_pae_key_storage_supp_write(instance, entry);
        entry->store_ticks = ws_pae_key_storage_storing_interval_get() * 1000;
    }

    return keep_timer_running;
}

void ws_pae_lib_supp_timer_ticks_set(supp_entry_t *entry, uint32_t ticks)
{
    entry->ticks = ticks;
}

void ws_pae_lib_supp_timer_ticks_add(supp_entry_t *entry, uint32_t ticks)
{
    entry->ticks += ticks;
}

bool ws_pae_lib_supp_timer_is_running(supp_entry_t *entry)
{
    if (entry->ticks == 0) {
        return false;
    }

    return true;
}

void ws_pae_lib_supp_list_to_active(supp_list_t *active_supp_list, supp_list_t *inactive_supp_list, supp_entry_t *entry)
{
    if (entry->active) {
        return;
    }

    tr_debug("PAE: to active, eui-64: %s", trace_array(entry->addr.eui_64, 8));

    ns_list_remove(inactive_supp_list, entry);
    ns_list_add_to_start(active_supp_list, entry);

    entry->active = true;
    entry->ticks = 0;

    // Adds relay address data
    entry->addr.type = KMP_ADDR_EUI_64_AND_IP;
}

void ws_pae_lib_supp_list_to_inactive(void *instance, supp_list_t *active_supp_list, supp_entry_t *entry, ws_pae_lib_supp_deleted supp_deleted)
{
    if (!entry->active) {
        return;
    }

    tr_debug("PAE: to inactive, eui-64: %s", trace_array(entry->addr.eui_64, 8));

    if (entry->access_revoked) {
        tr_info("Access revoked; deleted, eui-64: %s", trace_array(entry->addr.eui_64, 8));
        ws_pae_lib_supp_list_remove(instance, active_supp_list, entry, supp_deleted);
        return;
    }

    // Store to key storage
    ws_pae_key_storage_supp_write(instance, entry);

    // Remove supplicant entry
    ws_pae_lib_supp_list_remove(instance, active_supp_list, entry, supp_deleted);
    if (supp_deleted) {
        supp_deleted(instance);
    }
}

void ws_pae_lib_supp_list_purge(void *instance, supp_list_t *active_supp_list, uint16_t max_number, uint8_t max_purge, ws_pae_lib_supp_deleted supp_deleted)
{
    uint16_t active_supp = ns_list_count(active_supp_list);

    if (active_supp > max_number) {
        uint16_t remove_count = active_supp - max_number;
        if (max_purge > 0 && remove_count > max_purge) {
            remove_count = max_purge;
        }

        // Remove entries from active list if there are no active KMPs ongoing for the entry
        ns_list_foreach_safe(supp_entry_t, entry, active_supp_list) {
            if (remove_count > 0 && ws_pae_lib_kmp_list_empty(&entry->kmp_list)) {
                tr_info("Active supplicant removed, eui-64: %s", trace_array(kmp_address_eui_64_get(&entry->addr), 8));
                ws_pae_lib_supp_list_remove(instance, active_supp_list, entry, supp_deleted);
                remove_count--;
            } else {
                break;
            }
        }
    }
}

uint16_t ws_pae_lib_supp_list_kmp_count(supp_list_t *supp_list, kmp_type_e type)
{
    uint16_t kmp_count = 0;

    ns_list_foreach(supp_entry_t, entry, supp_list) {
        ns_list_foreach(kmp_entry_t, kmp_entry, &entry->kmp_list) {
            if (kmp_api_type_get(kmp_entry->kmp) == type) {
                kmp_count++;
            }
        }
    }

    return kmp_count;
}

bool ws_pae_lib_supp_list_entry_is_in_list(supp_list_t *supp_list, supp_entry_t *searched_entry)
{
    ns_list_foreach(supp_entry_t, entry, supp_list) {
        if (entry == searched_entry) {
            return true;
        }
    }

    return false;
}

kmp_api_t *ws_pae_lib_supp_list_kmp_receive_check(supp_list_t *supp_list, const void *pdu, uint16_t size)
{
    ns_list_foreach(supp_entry_t, entry, supp_list) {
        ns_list_foreach(kmp_entry_t, kmp_entry, &entry->kmp_list) {
            if (kmp_api_receive_check(kmp_entry->kmp, pdu, size)) {
                return kmp_entry->kmp;
            }
        }
    }

    return NULL;
}

int8_t ws_pae_lib_shared_comp_list_init(shared_comp_list_t *comp_list)
{
    ns_list_init(comp_list);
    return 0;
}

int8_t ws_pae_lib_shared_comp_list_free(shared_comp_list_t *comp_list)
{
    ns_list_foreach_safe(shared_comp_entry_t, entry, comp_list) {
        if (entry->data->delete) {
            entry->data->delete ();
        }
        ns_list_remove(comp_list, entry);
        ns_dyn_mem_free(entry);
    }
    return 0;
}

int8_t ws_pae_lib_shared_comp_list_add(shared_comp_list_t *comp_list, kmp_shared_comp_t *data)
{
    ns_list_foreach(shared_comp_entry_t, entry, comp_list) {
        if (entry->data == data) {
            return -1;
        }
    }

    shared_comp_entry_t *entry = ns_dyn_mem_alloc(sizeof(shared_comp_entry_t));
    if (!entry) {
        return -1;
    }
    entry->data = data;
    ns_list_add_to_end(comp_list, entry);

    return 0;
}

int8_t ws_pae_lib_shared_comp_list_remove(shared_comp_list_t *comp_list, kmp_shared_comp_t *data)
{
    ns_list_foreach(shared_comp_entry_t, entry, comp_list) {
        if (entry->data == data) {
            ns_list_remove(comp_list, entry);
            ns_dyn_mem_free(entry);
            return 0;
        }
    }

    return 0;
}

int8_t ws_pae_lib_shared_comp_list_timeout(shared_comp_list_t *comp_list, uint16_t ticks)
{
    ns_list_foreach(shared_comp_entry_t, entry, comp_list) {
        if (entry->data->timeout) {
            entry->data->timeout(ticks);
        }
    }

    return 0;
}

#endif /* HAVE_WS */
