/*
 * Copyright (c) 2020, Pelion and affiliates.
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
#include "randLIB.h"
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/kmp/kmp_socket_if.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_timers.h"
#include "6LoWPAN/ws/ws_pae_auth.h"
#include "6LoWPAN/ws/ws_pae_lib.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "6LoWPAN/ws/ws_pae_nvm_data.h"
#include "6LoWPAN/ws/ws_pae_time.h"
#include "6LoWPAN/ws/ws_pae_key_storage.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsks"

#define KEY_STORAGE_INDEX_FILE                        "key_storage_index"
#define KEY_STORAGE_FILE                              "key_storage_00"
#define KEY_STORAGE_FILE_LEN                          sizeof(KEY_STORAGE_FILE)

// Storage array header size
#define STORAGE_ARRAY_HEADER_LEN                      sizeof(key_storage_nvm_tlv_entry_t)

// Update the key storage reference time on write operations, if entry reference time differs more than a day
#define KEY_STORAGE_REF_TIME_UPDATE_THRESHOLD         86400

/* Force key storage reference time update, if reference time differs more 31 days */
#define KEY_STORAGE_REF_TIME_UPDATE_FORCE_THRESHOLD   GTK_DEFAULT_LIFETIME + 86400

// Base scatter timer value, 3 seconds */
#define KEY_STORAGE_SCATTER_TIMER_BASE_VALUE          30

typedef enum {
    WRITE_SET = 0,
    TIME_SET,
    PMK_CNT_SET,
    PMK_SET,
    PTK_SET,
    EUI64_SET,
    PTKEUI64_SET,
    GTKHASH_SET,
    GTKHASH4WH_SET,
    PMKLTIME_SET,
    PTKLTIME_SET,
} field_set_t;

#define FIELD_SET(field) (field_set | (1 << field))
#define FIELD_IS_SET(field) (field_set & (1 << field))

typedef struct {
    ns_list_link_t link;                                /**< Link */
    const void *instance;                               /**< Instance; for support of multiple authenticators */
    key_storage_nvm_tlv_entry_t *storage_array_handle;  /**< Key storage array handle (NVM header + array) */
    sec_prot_keys_storage_t *storage_array;             /**< Key storage array */
    uint16_t size;                                      /**< Array size in bytes */
    uint16_t entries;                                   /**< Entries in array */
    uint16_t free_entries;                              /**< Free entries in array */
    bool allocated : 1;                                 /**< Allocated */
    bool modified : 1;                                  /**< Array modified */
    bool pending_storing : 1;                           /**< Entry is pending storing to NVM */
} key_storage_array_t;

typedef struct {
    uint8_t settings_set;                               /**< Settings set, do not use defaults */
    uint8_t storages_empty;                             /**< Number of empty i.e. to be allocated storages */
    uint16_t storage_default_size;                      /**< Default size for storages */
    uint16_t replace_index;                             /**< Index to replace when storages are full */
    uint64_t store_bitfield;                            /**< Bitfield of stored files */
    uint16_t store_timer_timeout;                       /**< Storing timing timeout */
    uint16_t store_timer;                               /**< Storing timer */
    uint32_t restart_cnt;                               /**< Re-start counter */
    uint32_t scatter_timer;                             /**< NVM storing scatter timer */
} key_storage_params_t;

static key_storage_params_t key_storage_params;

static NS_LIST_DEFINE(key_storage_array_list, key_storage_array_t, link);

static int8_t ws_pae_key_storage_allocate(const void *instance, uint16_t key_storage_size, void *storage);
static void ws_pae_key_storage_clear(key_storage_array_t *key_storage_array);
static void ws_pae_key_storage_list_all_free(void);
static sec_prot_keys_storage_t *ws_pae_key_storage_get(const void *instance, const uint8_t *eui64, key_storage_array_t **key_storage_array, bool return_free);
static sec_prot_keys_storage_t *ws_pae_key_storage_replace(const void *instance, key_storage_array_t **key_storage_array);
static void ws_pae_key_storage_trace(uint16_t field_set, sec_prot_keys_storage_t *key_storage, key_storage_array_t *key_storage_array);
static void ws_pae_key_storage_scatter_timer_timeout(void);
static void ws_pae_key_storage_fast_timer_start(void);
static void ws_pae_key_storage_timer_expiry_set(void);
static void ws_pae_key_storage_fast_timer_ticks_set(void);
static int8_t ws_pae_key_storage_array_time_update_entry(uint64_t time_difference, sec_prot_keys_storage_t *storage_array_entry);
static int8_t ws_pae_key_storage_array_time_check_and_update_all(key_storage_array_t *key_storage_array, bool modified);
static int8_t ws_pae_key_storage_array_counters_check_and_update_all(key_storage_array_t *key_storage_array);
static int8_t ws_pae_key_storage_array_lifetime_update(uint32_t time_difference, uint16_t *lifetime);
static int8_t ws_pae_key_storage_array_lifetime_get(uint32_t time_difference, uint16_t short_lifetime, uint32_t *lifetime);
static void ws_pae_key_storage_array_pmk_invalid(sec_prot_keys_storage_t *storage_array);
static void ws_pae_key_storage_array_ptk_invalid(sec_prot_keys_storage_t *storage_array);

static void ws_pae_key_storage_filename_set(char *file_name, uint8_t file_number)
{
    // Name is "key_storage_00" where 00 is replaced by file number
    strcpy(file_name, KEY_STORAGE_FILE);
    file_name[12] = (file_number / 10) + 'A'; // 0 is ASCII 'A', 1='B',..., F='P'
    file_name[13] = (file_number % 10) + 'A'; // same as above
}

int8_t ws_pae_key_storage_memory_set(uint8_t key_storages_number, const uint16_t *key_storage_size, void **key_storages)
{
    for (uint8_t index = 0; index < key_storages_number; index++) {
        if (ws_pae_key_storage_allocate(NULL, key_storage_size[index], (sec_prot_keys_storage_t *) key_storages[index]) < 0) {
            return -1;
        }
    }

    key_storage_params.storages_empty = 0;

    return 0;
}

int8_t ws_pae_key_storage_settings_set(uint8_t alloc_max_number, uint16_t alloc_size, uint16_t storing_interval)
{
    key_storage_params.settings_set = true;
    key_storage_params.storages_empty = alloc_max_number;
    key_storage_params.storage_default_size = alloc_size;
    key_storage_params.store_timer = storing_interval;
    key_storage_params.store_timer_timeout = storing_interval;

    return 0;
}

void ws_pae_key_storage_init(void)
{
    if (!key_storage_params.settings_set) {
        key_storage_params.storages_empty = DEFAULT_NUMBER_OF_STORAGES;
        key_storage_params.storage_default_size = STORAGE_ARRAY_HEADER_LEN + (sizeof(sec_prot_keys_storage_t) * DEFAULT_NUMBER_OF_ENTRIES_IN_ONE_STORAGE);
        key_storage_params.store_timer = DEFAULT_STORING_INTERVAL;
        key_storage_params.store_timer_timeout = DEFAULT_STORING_INTERVAL;
    }
    key_storage_params.replace_index = 0;
    key_storage_params.store_bitfield = 0,
    key_storage_params.restart_cnt = 0;
    key_storage_params.scatter_timer = 0;
}

void ws_pae_key_storage_delete(void)
{
    ws_pae_key_storage_list_all_free();
}

static int8_t ws_pae_key_storage_allocate(const void *instance, uint16_t key_storage_size, void *new_storage_array)
{
    key_storage_array_t *key_storage_array = ns_dyn_mem_alloc(sizeof(key_storage_array_t));
    if (!key_storage_array) {
        return -1;
    }

    if (new_storage_array == NULL) {
        key_storage_array->storage_array_handle = ns_dyn_mem_alloc(key_storage_size);
        if (!key_storage_array->storage_array_handle) {
            ns_dyn_mem_free(key_storage_array);
            return -1;
        }
        key_storage_array->allocated = true;
    } else {
        key_storage_array->storage_array_handle = new_storage_array;
        key_storage_array->allocated = false;
    }
    key_storage_array->storage_array = (sec_prot_keys_storage_t *)(((uint8_t *)key_storage_array->storage_array_handle) + STORAGE_ARRAY_HEADER_LEN);
    key_storage_array->size = key_storage_size;
    key_storage_array->entries = (key_storage_size - STORAGE_ARRAY_HEADER_LEN) / sizeof(sec_prot_keys_storage_t);
    key_storage_array->free_entries = key_storage_array->entries;
    key_storage_array->instance = instance;
    key_storage_array->modified = false;
    key_storage_array->pending_storing = false;

    ws_pae_nvm_store_key_storage_tlv_create((nvm_tlv_t *) key_storage_array->storage_array_handle, key_storage_array->size);

    ws_pae_key_storage_clear(key_storage_array);

    ns_list_add_to_end(&key_storage_array_list, key_storage_array);

    tr_info("KeyS new %s, array: %p entries: %i", key_storage_array->allocated ? "allocated" : "static", (void *) key_storage_array->storage_array, key_storage_array->entries);

    return 0;
}

static void ws_pae_key_storage_clear(key_storage_array_t *key_storage_array)
{
    key_storage_array->storage_array_handle->reference_time = ws_pae_current_time_get();
    key_storage_array->storage_array_handle->reference_restart_cnt = key_storage_params.restart_cnt;

    sec_prot_keys_storage_t *storage_array = (sec_prot_keys_storage_t *) key_storage_array->storage_array;
    // Set all entries empty
    for (uint16_t index = 0; index < key_storage_array->entries; index++) {
        if (key_storage_array->allocated) {
            memset(&storage_array[index], 0, sizeof(sec_prot_keys_storage_t));
        }
        storage_array[index].eui_64_set = false;
    }
}

static void ws_pae_key_storage_list_all_free(void)
{
    ns_list_foreach_safe(key_storage_array_t, entry, &key_storage_array_list) {
        if (entry->allocated) {
            ns_dyn_mem_free(entry->storage_array_handle);
        }
        ns_list_remove(&key_storage_array_list, entry);
        ns_dyn_mem_free(entry);
    }
}

static sec_prot_keys_storage_t *ws_pae_key_storage_get(const void *instance, const uint8_t *eui64, key_storage_array_t **key_storage_array, bool return_free)
{
    sec_prot_keys_storage_t *free_key_storage = NULL;
    *key_storage_array = NULL;
    uint16_t free_index = 0;

    ns_list_foreach(key_storage_array_t, entry, &key_storage_array_list) {
        // Checks whether storage is for this authenticator
        if (entry->instance != NULL && entry->instance != instance) {
            tr_info("KeyS get instance other");
            continue;
        }
        // Checks entries in storage array
        sec_prot_keys_storage_t *storage_array = (sec_prot_keys_storage_t *) entry->storage_array;
        for (uint16_t index = 0; index < entry->entries; index++) {
            if (!storage_array[index].eui_64_set) {
                // Stores free array entry and initiates data on it to zero
                if (return_free && free_key_storage == NULL) {
                    *key_storage_array = entry;
                    free_key_storage = &storage_array[index];
                    memset(&storage_array[index], 0, sizeof(sec_prot_keys_storage_t));
                    free_index = index;
                }
                continue;
            }
            // Searches for matching entry
            if (memcmp(&storage_array[index].ptk_eui_64, eui64, 8) == 0) {
                *key_storage_array = entry;
                // If not already reserved for authenticator instance, reserve array for it
                entry->instance = instance;
                tr_info("KeyS get array: %p i: %i eui64: %s", (void *) entry->storage_array, index, tr_array(eui64, 8));
                return &storage_array[index];
            }
        }
    }

    // If not already reserved for authenticator instance, reserve array for it
    if (free_key_storage != NULL && (*key_storage_array)->instance == NULL) {
        (*key_storage_array)->instance = instance;
    }

    tr_info("KeyS get array %s: %p i: %i free: %p eui64: %s", *key_storage_array ? "" : "(not alloc)", *key_storage_array ? (void *)(*key_storage_array)->storage_array : NULL, *key_storage_array ? free_index : 0, (void *)free_key_storage, tr_array(eui64, 8));
    // If matching entry is not found, returns free entry
    return free_key_storage;
}

bool ws_pae_key_storage_supp_delete(const void *instance, const uint8_t *eui64)
{
    (void) instance;

    bool deleted = false;

    ns_list_foreach(key_storage_array_t, entry, &key_storage_array_list) {
        // Checks entries in storage array
        sec_prot_keys_storage_t *storage_array = (sec_prot_keys_storage_t *) entry->storage_array;
        for (uint16_t index = 0; index < entry->entries; index++) {
            if (!storage_array[index].eui_64_set) {
                continue;
            }
            // Searches for matching entry
            if (memcmp(&storage_array[index].ptk_eui_64, eui64, 8) == 0) {
                memset(&storage_array[index], 0, sizeof(sec_prot_keys_storage_t));
                tr_info("KeyS delete array: %p i: %i eui64: %s", (void *) entry->storage_array, index, tr_array(eui64, 8));
                entry->modified = true;
                deleted = true;
            }
        }
    }

    if (deleted) {
        // Trigger storing to NVM right away to keep the stored data in sync
        ws_pae_key_storage_timer_expiry_set();
    }

    return deleted;
}

static sec_prot_keys_storage_t *ws_pae_key_storage_replace(const void *instance, key_storage_array_t **key_storage_array)
{
    uint16_t replace_index = key_storage_params.replace_index;
    sec_prot_keys_storage_t *storage_array = NULL;
    uint16_t storage_array_index = 0;

    ns_list_foreach(key_storage_array_t, entry, &key_storage_array_list) {
        // Checks whether storage is for this authenticator
        if (entry->instance != instance) {
            continue;
        }
        // Finds correct key storage array
        if (replace_index >= entry->entries) {
            replace_index -= entry->entries;
            continue;
        }
        if (key_storage_array) {
            *key_storage_array = entry;
        }
        // Sets array and index and sets replace index to next
        storage_array = (sec_prot_keys_storage_t *) entry->storage_array;
        storage_array_index = replace_index;
        key_storage_params.replace_index++;

        tr_info("KeyS replace array: %p i: %i eui64: %s", (void *) entry->storage_array, replace_index, tr_array(storage_array[replace_index].ptk_eui_64, 8));
        break;
    }

    // If replace index is not found it means that index has gone past the last entry
    if (storage_array == NULL) {
        tr_info("KeyS replace array first index");

        /* Gets first key storage array and sets the array and sets index to zero and sets
           (next) replace index to first */
        key_storage_array_t *key_storage_array_entry = ns_list_get_first(&key_storage_array_list);
        if (key_storage_array_entry == NULL) {
            return NULL;
        }
        if (key_storage_array) {
            *key_storage_array = key_storage_array_entry;
        }
        storage_array = (sec_prot_keys_storage_t *) key_storage_array_entry->storage_array;
        storage_array_index = 0;
        key_storage_params.replace_index = 1;

        tr_info("KeyS replace array: %p i: %i eui64: %s", (void *) key_storage_array_entry->storage_array, storage_array_index, tr_array(key_storage_array_entry->storage_array[storage_array_index].ptk_eui_64, 8));
    }

    // Deletes any previous data
    memset(&storage_array[storage_array_index], 0, sizeof(sec_prot_keys_storage_t));

    return &storage_array[storage_array_index];
}

int8_t ws_pae_key_storage_supp_write(const void *instance, supp_entry_t *pae_supp)
{
    uint8_t *eui_64 = pae_supp->addr.eui_64;

    key_storage_array_t *key_storage_array;
    // Check if entry exists
    sec_prot_keys_storage_t *key_storage = ws_pae_key_storage_get(instance, eui_64, &key_storage_array, false);
    if (key_storage == NULL) {
        // Do not allocate new storage if PMK and PTK are not set
        if (!pae_supp->sec_keys.pmk_set && !pae_supp->sec_keys.ptk_set) {
            tr_info("KeyS PMK and PTK not set, skip storing, eui64: %s", tr_array(eui_64, 8));
            return -1;
        }
        // Allocate new empty entry
        key_storage = ws_pae_key_storage_get(instance, eui_64, &key_storage_array, true);
    }

    // If cannot find existing or empty storage and there is room for storages tries to allocate more storage
    if (key_storage == NULL && key_storage_params.storages_empty > 0) {
        if (ws_pae_key_storage_allocate(instance, key_storage_params.storage_default_size, NULL) >= 0) {
            key_storage_params.storages_empty--;
            key_storage = ws_pae_key_storage_get(instance, eui_64, &key_storage_array, true);
        }
    }

    // If still cannot find empty storage, replaces old one
    if (key_storage == NULL) {
        key_storage = ws_pae_key_storage_replace(instance, &key_storage_array);
    }

    // On failure exit
    if (key_storage == NULL) {
        return -1;
    }

    // Calculate time difference between storage array reference time and current time
    uint32_t time_difference;
    if (ws_pae_time_diff_calc(ws_pae_current_time_get(), key_storage_array->storage_array_handle->reference_time, &time_difference, false) < 0) {
        tr_error("KeyS write time err: %"PRIi64", ref: %"PRIi64", diff: %"PRIi32, ws_pae_current_time_get(), key_storage_array->storage_array_handle->reference_time, time_difference);
        return -1;
    }

    sec_prot_keys_t *sec_keys = &pae_supp->sec_keys;

    // If replay counter is near to exhaust delete PMK and PTK to refresh the counter
    if (sec_keys->pmk_key_replay_cnt_set && (sec_keys->pmk_key_replay_cnt & PMK_KEY_REPLAY_CNT_LIMIT_MASK) >= PMK_KEY_REPLAY_CNT_LIMIT) {
        tr_error("KeyS write PMK cnt limit eui64: %s", tr_array(eui_64, 8));
        sec_prot_keys_ptk_delete(&pae_supp->sec_keys);
        sec_prot_keys_pmk_delete(&pae_supp->sec_keys);
    }

    uint16_t field_set = 1 << WRITE_SET;

    if (key_storage->pmk_key_replay_cnt_set != sec_keys->pmk_key_replay_cnt_set ||
            key_storage->pmk_key_replay_cnt != sec_keys->pmk_key_replay_cnt) {
        /* Does not set modified variable since counter updates are never stored to NVM.
           Instead counter is increased on re-start. */
        key_storage->pmk_key_replay_cnt = sec_keys->pmk_key_replay_cnt & PMK_KEY_REPLAY_CNT_LIMIT_MASK;
        key_storage->pmk_key_replay_cnt_set = sec_keys->pmk_key_replay_cnt_set;
        field_set |= 1 << PMK_CNT_SET;
    }

    if (key_storage->pmk_set != sec_keys->pmk_set ||
            memcmp(key_storage->pmk, sec_keys->pmk, PMK_LEN) != 0) {
        key_storage_array->modified = true;
        key_storage->pmk_set = sec_keys->pmk_set;
        memcpy(key_storage->pmk, sec_keys->pmk, PMK_LEN);
        field_set |= 1 << PMK_SET;
    }

    if (key_storage->ptk_set != sec_keys->ptk_set ||
            memcmp(key_storage->ptk, sec_keys->ptk, PTK_LEN) != 0) {
        key_storage_array->modified = true;
        key_storage->ptk_set = sec_keys->ptk_set;
        memcpy(key_storage->ptk, sec_keys->ptk, PTK_LEN);
        field_set |= 1 << PTK_SET;
    }

    if (key_storage->eui_64_set != true ||
            memcmp(key_storage->ptk_eui_64, eui_64, 8) != 0) {
        key_storage_array->modified = true;
        key_storage->eui_64_set = true;
        memcpy(key_storage->ptk_eui_64, eui_64, 8);
        field_set |= 1 << EUI64_SET;
    }

    if (key_storage->ptk_eui_64_set != sec_keys->ptk_eui_64_set) {
        key_storage_array->modified = true;
        key_storage->ptk_eui_64_set = sec_keys->ptk_eui_64_set;
        field_set |= 1 << PTKEUI64_SET;
    }

    if (key_storage->ins_gtk_hash_set != sec_keys->ins_gtk_hash_set ||
            memcmp(key_storage->ins_gtk_hash, sec_keys->ins_gtk_hash, sizeof(sec_keys->ins_gtk_hash)) != 0) {
        key_storage_array->modified = true;
        key_storage->ins_gtk_hash_set = sec_keys->ins_gtk_hash_set;
        memcpy(key_storage->ins_gtk_hash, sec_keys->ins_gtk_hash, sizeof(sec_keys->ins_gtk_hash));
        field_set |= 1 << GTKHASH_SET;
    }
    if (key_storage->ins_gtk_4wh_hash_set != sec_keys->ins_gtk_hash_set) {
        key_storage->ins_gtk_4wh_hash_set = sec_keys->ins_gtk_hash_set;
        field_set |= 1 << GTKHASH4WH_SET;
    }

    if (sec_keys->pmk_set) {
        // PMK lifetime is: time difference between reference and current time + lifetime
        uint64_t pmk_lifetime = time_difference + sec_keys->pmk_lifetime;
        if (pmk_lifetime > SEC_MAXIMUM_LIFETIME) {
            pmk_lifetime = 0;
        }
        uint16_t short_time = ws_pae_time_to_short_convert(pmk_lifetime);
        // Compares the time from active supplicant entry to stored one, and if time
        if (!key_storage->pmk_lifetime_set || !ws_pae_time_from_short_time_compare(key_storage->pmk_lifetime, short_time)) {
            key_storage_array->modified = true;
            key_storage->pmk_lifetime_set = true;
            key_storage->pmk_lifetime = short_time;
            field_set |= 1 << PMKLTIME_SET;
        }
    } else {
        key_storage->pmk_lifetime_set = false;
        key_storage->pmk_lifetime = 0;
        field_set |= 1 << PMKLTIME_SET;
    }

    if (sec_keys->ptk_set) {
        // PTK lifetime is: time difference between reference and current time + lifetime
        uint64_t ptk_lifetime = time_difference + sec_keys->ptk_lifetime;
        if (ptk_lifetime > SEC_MAXIMUM_LIFETIME) {
            ptk_lifetime = 0;
        }
        uint16_t short_time = ws_pae_time_to_short_convert(ptk_lifetime);
        if (!key_storage->ptk_lifetime_set || !ws_pae_time_from_short_time_compare(key_storage->ptk_lifetime, short_time)) {
            key_storage_array->modified = true;
            key_storage->ptk_lifetime_set = true;
            key_storage->ptk_lifetime = short_time;
            field_set |= 1 << PTKLTIME_SET;
        }
    } else {
        key_storage->ptk_lifetime_set = false;
        key_storage->ptk_lifetime = 0;
        field_set |= 1 << PTKLTIME_SET;
    }

    ws_pae_key_storage_trace(field_set, key_storage, key_storage_array);

    return 0;
}

supp_entry_t *ws_pae_key_storage_supp_read(const void *instance, const uint8_t *eui_64, sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs)
{
    key_storage_array_t *key_storage_array;
    sec_prot_keys_storage_t *key_storage = ws_pae_key_storage_get(instance, eui_64, &key_storage_array, false);
    if (key_storage == NULL) {
        return NULL;
    }

    uint8_t pmk_invalid = false;
    uint8_t ptk_invalid = false;

    uint16_t field_set = 0;

    // Calculate time difference between storage array reference time and current time
    uint32_t time_difference;
    if (ws_pae_time_diff_calc(ws_pae_current_time_get(), key_storage_array->storage_array_handle->reference_time, &time_difference, false) < 0) {
        tr_error("KeyS read time err");
        pmk_invalid = true;
        field_set |= 1 << TIME_SET;
    }

    uint32_t pmk_lifetime = 0;
    uint32_t ptk_lifetime = 0;

    if (!pmk_invalid) {
        // Calculate PMK lifetime
        if (ws_pae_key_storage_array_lifetime_get(time_difference, key_storage->pmk_lifetime, &pmk_lifetime) < 0) {
            // PMK expired, all keys are invalid
            pmk_invalid = true;
            ptk_invalid = true;
            field_set |= 1 << TIME_SET;
        }
        field_set |= 1 << PMKLTIME_SET;
    }

    if (!pmk_invalid) {
        // Calculate PTK lifetime
        if (ws_pae_key_storage_array_lifetime_get(time_difference, key_storage->ptk_lifetime, &ptk_lifetime) < 0) {
            // PMK expired, invalidate PTK related fields, PMK is still valid
            ptk_invalid = true;
            field_set |= 1 << TIME_SET;
        }
        field_set |= 1 << PTKLTIME_SET;
    }

    supp_entry_t *pae_supp = ns_dyn_mem_temporary_alloc(sizeof(supp_entry_t));
    if (!pae_supp) {
        return NULL;
    }
    ws_pae_lib_supp_init(pae_supp);

    // Adds relay address data
    kmp_address_init(KMP_ADDR_EUI_64_AND_IP, &pae_supp->addr, key_storage->ptk_eui_64);

    sec_prot_keys_t *sec_keys = &pae_supp->sec_keys;
    sec_prot_keys_init(sec_keys, gtks, certs);

    // PMK is invalid, do not retrieve any security key data
    if (pmk_invalid) {
        ws_pae_key_storage_trace(field_set, key_storage, key_storage_array);
        return pae_supp;
    }

    // Set PMK security key data
    if (key_storage->pmk_key_replay_cnt_set) {
        // LSB 16 bits is the replay counter
        sec_keys->pmk_key_replay_cnt = key_storage->pmk_key_replay_cnt;

        uint32_t restart_cnt_diff;
        if (key_storage_params.restart_cnt >= key_storage_array->storage_array_handle->reference_restart_cnt) {
            restart_cnt_diff = key_storage_params.restart_cnt - key_storage_array->storage_array_handle->reference_restart_cnt;
        } else {
            restart_cnt_diff = key_storage_params.restart_cnt;
        }
        // MSB 32 bits is the restart count
        sec_keys->pmk_key_replay_cnt |= ((uint64_t) restart_cnt_diff) << 32;
        field_set |= 1 << PMK_CNT_SET;
    } else {
        sec_keys->pmk_key_replay_cnt = 0;
    }
    sec_keys->pmk_key_replay_cnt_set = key_storage->pmk_key_replay_cnt_set;

    memcpy(sec_keys->pmk, key_storage->pmk, PMK_LEN);
    sec_keys->pmk_set = key_storage->pmk_set;

    field_set |= ((uint16_t)sec_keys->pmk_set) << PMK_SET;

    sec_keys->pmk_lifetime = pmk_lifetime;

    field_set |= 1 << PMKLTIME_SET;

    // PTK is invalid, do not retrieve any PTK security key data
    if (ptk_invalid) {
        ws_pae_key_storage_trace(field_set, key_storage, key_storage_array);
        return pae_supp;
    }

    // Set PTK security key data
    memcpy(sec_keys->ptk, key_storage->ptk, PTK_LEN);
    sec_keys->ptk_set = key_storage->ptk_set;

    field_set |= ((uint16_t)sec_keys->ptk_set) << PTK_SET;

    memcpy(sec_keys->ptk_eui_64, key_storage->ptk_eui_64, 8);
    sec_keys->ptk_eui_64_set = key_storage->ptk_eui_64_set;

    field_set |= ((uint16_t) sec_keys->ptk_eui_64_set) << PTKEUI64_SET;

    memcpy(sec_keys->ins_gtk_hash, key_storage->ins_gtk_hash, sizeof(sec_keys->ins_gtk_hash));
    sec_keys->ins_gtk_hash_set = key_storage->ins_gtk_hash_set;
    sec_keys->ins_gtk_4wh_hash_set = sec_keys->ins_gtk_hash_set;

    field_set |= ((uint16_t) sec_keys->ins_gtk_hash_set) << GTKHASH_SET;
    field_set |= ((uint16_t) sec_keys->ins_gtk_4wh_hash_set) << GTKHASH4WH_SET;

    sec_keys->ptk_lifetime = ptk_lifetime;

    field_set |= 1 << PTKLTIME_SET;

    ws_pae_key_storage_trace(field_set, key_storage, key_storage_array);

    return pae_supp;
}

static void ws_pae_key_storage_trace(uint16_t field_set, sec_prot_keys_storage_t *key_storage, key_storage_array_t *key_storage_array)
{
    tr_info("KeyS %s %s%"PRIi64" %"PRIi64" %s%s%i %s%s%s%s%s %s%s%i %i %s%i %i",
            FIELD_IS_SET(WRITE_SET) ? "write" : "read",
            FIELD_IS_SET(TIME_SET) ? "TIME " : "", FIELD_IS_SET(TIME_SET) ? ws_pae_current_time_get() : 0, FIELD_IS_SET(TIME_SET) ? key_storage_array->storage_array_handle->reference_time : 0,
            FIELD_IS_SET(PMK_SET) ? "PMK " : "",
            FIELD_IS_SET(PMK_CNT_SET) ? "PMK CNT " : "", key_storage->pmk_key_replay_cnt,
            FIELD_IS_SET(PTK_SET) ? "PTK " : "",
            FIELD_IS_SET(EUI64_SET) ? "EUI64 " : "",
            FIELD_IS_SET(PTKEUI64_SET) ? "PTKEUI64 " : "",
            FIELD_IS_SET(GTKHASH_SET) ? "GTKHASH " : "", tr_array((uint8_t *)key_storage->ins_gtk_hash, 8),
            FIELD_IS_SET(GTKHASH4WH_SET) ? "GTKHASH4WH " : "",
            FIELD_IS_SET(PMKLTIME_SET) ? "PMKLTIME " : "", STIME_TIME_GET(key_storage->pmk_lifetime), STIME_FORMAT_GET(key_storage->pmk_lifetime),
            FIELD_IS_SET(PTKLTIME_SET) ? "PTKLTIME " : "", STIME_TIME_GET(key_storage->ptk_lifetime), STIME_FORMAT_GET(key_storage->ptk_lifetime)
           );
}

int8_t ws_pae_key_storage_store(void)
{
    uint8_t entry_offset = 0;
    uint64_t store_bitfield = 0;
    bool start_scatter_timer = false;

    ns_list_foreach(key_storage_array_t, entry, &key_storage_array_list) {
        // Bitfield is set for all entries (also that are non-modified in this write)
        store_bitfield |= ((uint64_t) 1) << entry_offset;

        /* Checks whether array reference time needs to be updated */
        int8_t ret_value = ws_pae_key_storage_array_time_check_and_update_all(entry, entry->modified);
        if (ret_value == 1) {
            entry->modified = true;
        } else if (ret_value < 0) {
            // On error clears the whole array
            ws_pae_key_storage_clear(entry);
            entry->modified = true;
        }

        /* On large network there could be different time thresholds for entries full / all updated
           and half empty entries/not all updated where it is likely that data will still be modified */
        if (!entry->modified) {
            entry_offset++;
            // If not pending for storing; skips file write
            continue;
        }

        entry->pending_storing = true;
        start_scatter_timer = true;

        // Item is pending for storing, reset modified flag
        entry->modified = false;
        entry_offset++;
    }

    tr_info("KeyS storage store, bitf: %"PRIx64, store_bitfield);

    if (start_scatter_timer) {
        ws_pae_key_storage_fast_timer_start();
    }

    if (key_storage_params.store_bitfield != store_bitfield) {
        key_storage_params.store_bitfield = store_bitfield;
        nvm_tlv_t *tlv = ws_pae_nvm_store_generic_tlv_allocate_and_create(
                             PAE_NVM_KEY_STORAGE_INDEX_TAG, PAE_NVM_KEY_STORAGE_INDEX_LEN);
        ws_pae_nvm_store_key_storage_index_tlv_create(tlv, key_storage_params.store_bitfield);
        ws_pae_nvm_store_tlv_file_write(KEY_STORAGE_INDEX_FILE, tlv);
        ws_pae_nvm_store_generic_tlv_free(tlv);
    }

    return 0;
}

static void ws_pae_key_storage_scatter_timer_timeout(void)
{
    uint8_t entry_offset = 0;
    bool pending_entry = false;

    ns_list_foreach(key_storage_array_t, entry, &key_storage_array_list) {
        if (!entry->pending_storing) {
            entry_offset++;
            continue;
        }
        pending_entry = true;

        char filename[KEY_STORAGE_FILE_LEN];
        ws_pae_key_storage_filename_set(filename, entry_offset);
        tr_info("KeyS write array: %p file: %s", (void *) entry->storage_array, filename);
        nvm_tlv_t *tlv = (nvm_tlv_t *) entry->storage_array_handle;
        ws_pae_nvm_store_tlv_file_write(filename, tlv);

        // Item has been stored, reset pending storing and modified flag
        entry->pending_storing = false;
        entry->modified = false;
        break;
    }

    if (pending_entry) {
        ws_pae_key_storage_fast_timer_ticks_set();
        return;
    }

    tr_info("KeyS all pending entries stored");
}

void ws_pae_key_storage_read(uint32_t restart_cnt)
{
    key_storage_params.store_bitfield = 0;
    key_storage_params.restart_cnt = restart_cnt;

    nvm_tlv_t *tlv = ws_pae_nvm_store_generic_tlv_allocate_and_create(
                         PAE_NVM_KEY_STORAGE_INDEX_TAG, PAE_NVM_KEY_STORAGE_INDEX_LEN);

    if (ws_pae_nvm_store_tlv_file_read(KEY_STORAGE_INDEX_FILE, tlv) < 0) {
        ws_pae_nvm_store_generic_tlv_free(tlv);
        return;
    }

    uint64_t store_bitfield;
    if (ws_pae_nvm_store_key_storage_index_tlv_read(tlv, &store_bitfield) >= 0) {
        key_storage_params.store_bitfield = store_bitfield;
    }

    ws_pae_nvm_store_generic_tlv_free(tlv);

    if (key_storage_params.store_bitfield == 0) {
        return;
    }

    tr_info("KeyS init store bitf: %"PRIx64, store_bitfield);

    key_storage_array_t *key_storage_array = ns_list_get_first(&key_storage_array_list);
    key_storage_array_t *key_storage_array_prev = NULL;

    for (uint8_t entry_offset = 0; entry_offset < 64; entry_offset++) {
        // There are no more fields
        if (store_bitfield == 0) {
            break;
        }

        if (key_storage_array == NULL && key_storage_params.storages_empty > 0) {
            if (ws_pae_key_storage_allocate(NULL, key_storage_params.storage_default_size, NULL) >= 0) {
                key_storage_params.storages_empty--;
            }
        }

        if (key_storage_array_prev != NULL) {
            key_storage_array = ns_list_get_next(&key_storage_array_list, key_storage_array_prev);
        } else if (key_storage_array == NULL) {
            key_storage_array = ns_list_get_first(&key_storage_array_list);
        }

        if (key_storage_array == NULL) {
            break;
        }
        key_storage_array_prev = key_storage_array;

        // If set on bitfield read
        if ((store_bitfield & (((uint64_t) 1) << entry_offset)) == 0) {
            continue;
        }
        store_bitfield &= ~(((uint64_t) 1) << entry_offset);

        tlv = (nvm_tlv_t *) key_storage_array->storage_array_handle;
        ws_pae_nvm_store_key_storage_tlv_create(tlv, key_storage_array->size);

        char filename[KEY_STORAGE_FILE_LEN];
        ws_pae_key_storage_filename_set(filename, entry_offset);

        tr_info("KeyS init read array: %p file: %s", (void *) key_storage_array->storage_array, filename);
        if (ws_pae_nvm_store_tlv_file_read(filename, tlv) < 0) {
            ws_pae_key_storage_clear(key_storage_array);
            // On error, re-use current one
            key_storage_array_prev = NULL;
            continue;
        }

        bool read_error = false;
        if (ws_pae_nvm_store_key_storage_tlv_read(tlv, key_storage_array->size) < 0) {
            ws_pae_key_storage_clear(key_storage_array);
            read_error = true;
        }

        if (read_error) {
            // On error, re-use current one
            key_storage_array_prev = NULL;
            continue;
        }

        // Calculate time difference between storage array reference time and current time
        uint32_t time_difference;
        if (ws_pae_time_diff_calc(ws_pae_current_time_get(), key_storage_array->storage_array_handle->reference_time, &time_difference, false) < 0) {
            tr_error("KeyS read array time err: %"PRIi64", ref: %"PRIi64", diff: %"PRIi32, ws_pae_current_time_get(), key_storage_array->storage_array_handle->reference_time, time_difference);
            ws_pae_key_storage_clear(key_storage_array);
        }

        // Checks and updates PMK counters
        if (ws_pae_key_storage_array_counters_check_and_update_all(key_storage_array) < 0) {
            tr_error("KeyS read array cnt err");
            // On error clears the whole array
            ws_pae_key_storage_clear(key_storage_array);
        }

        // Entry set, go to next
        key_storage_array = NULL;
    }
}

void ws_pae_key_storage_remove(void)
{
    nvm_tlv_t *tlv = ws_pae_nvm_store_generic_tlv_allocate_and_create(
                         PAE_NVM_KEY_STORAGE_INDEX_TAG, PAE_NVM_KEY_STORAGE_INDEX_LEN);

    uint64_t store_bitfield = 0;
    if (ws_pae_nvm_store_tlv_file_read(KEY_STORAGE_INDEX_FILE, tlv) >= 0) {
        ws_pae_nvm_store_key_storage_index_tlv_read(tlv, &store_bitfield);
    }
    ws_pae_nvm_store_generic_tlv_free(tlv);

    ws_pae_nvm_store_tlv_file_remove(KEY_STORAGE_INDEX_FILE);

    tr_info("KeyS remove store bitf: %"PRIx64, store_bitfield);

    if (store_bitfield == 0) {
        return;
    }

    for (uint8_t entry_offset = 0; entry_offset < 64; entry_offset++) {
        // If set on bitfield delete
        if ((store_bitfield & (((uint64_t) 1) << entry_offset)) == 0) {
            continue;
        }

        char filename[KEY_STORAGE_FILE_LEN];
        ws_pae_key_storage_filename_set(filename, entry_offset);

        tr_info("KeyS remove file: %s", filename);
        ws_pae_nvm_store_tlv_file_remove(filename);
    }
}

void ws_pae_key_storage_timer(uint16_t seconds)
{
    if (key_storage_params.store_timer > seconds) {
        key_storage_params.store_timer -= seconds;
    } else {
        key_storage_params.store_timer = key_storage_params.store_timer_timeout;
        ws_pae_key_storage_store();
    }
}

void ws_pae_key_storage_fast_timer(uint16_t ticks)
{
    if (key_storage_params.scatter_timer == 0) {
        return;
    } else if (key_storage_params.scatter_timer > ticks) {
        key_storage_params.scatter_timer -= ticks;
    } else {
        key_storage_params.scatter_timer = 0;
        ws_pae_key_storage_scatter_timer_timeout();
    }
}

static void ws_pae_key_storage_fast_timer_start(void)
{
    ws_pae_key_storage_fast_timer_ticks_set();
}

static void ws_pae_key_storage_fast_timer_ticks_set(void)
{
    // (0.625 - 1,375) * 3 seconds
    key_storage_params.scatter_timer = randLIB_randomise_base(KEY_STORAGE_SCATTER_TIMER_BASE_VALUE, 0x5000, 0xB000);
    tr_info("KeyS scatter timer %"PRIi32, key_storage_params.scatter_timer);
}

static void ws_pae_key_storage_timer_expiry_set(void)
{
    // Expire in 30 seconds
    key_storage_params.store_timer = 30;
}

uint16_t ws_pae_key_storage_storing_interval_get(void)
{
    return key_storage_params.store_timer_timeout;
}

static int8_t ws_pae_key_storage_array_time_update_entry(uint64_t time_difference, sec_prot_keys_storage_t *storage_array_entry)
{
    if (storage_array_entry->pmk_lifetime_set) {
#ifdef EXTRA_DEBUG_INFO
        tr_debug("KeyS time update diff: %"PRIi64" PMK OLD t: %i %i eui64: %s", time_difference, STIME_TIME_GET(storage_array_entry->pmk_lifetime), STIME_FORMAT_GET(storage_array_entry->pmk_lifetime), tr_array(storage_array_entry->ptk_eui_64, 8));
#endif
        // Calculate new PMK lifetime
        if (ws_pae_key_storage_array_lifetime_update(time_difference, &storage_array_entry->pmk_lifetime) < 0) {
            tr_info("KeyS time update PMK expired diff: %"PRIi64" t: %i %i eui64: %s", time_difference, STIME_TIME_GET(storage_array_entry->pmk_lifetime),  STIME_FORMAT_GET(storage_array_entry->pmk_lifetime), tr_array(storage_array_entry->ptk_eui_64, 8));
            // PMK expired, whole entry is invalid
            ws_pae_key_storage_array_pmk_invalid(storage_array_entry);
            return -1;
        }
#ifdef EXTRA_DEBUG_INFO
        tr_debug("KeyS time update PMK NEW t: %i %i", STIME_TIME_GET(storage_array_entry->pmk_lifetime), STIME_FORMAT_GET(storage_array_entry->pmk_lifetime));
#endif
    }

    if (storage_array_entry->ptk_lifetime_set) {
#ifdef EXTRA_DEBUG_INFO
        tr_debug("KeyS time update diff: %"PRIi64" PTK OLD t: %i %i eui64: %s", time_difference, STIME_TIME_GET(storage_array_entry->ptk_lifetime), STIME_FORMAT_GET(storage_array_entry->ptk_lifetime), tr_array(storage_array_entry->ptk_eui_64, 8));
#endif
        // Calculate new PTK lifetime
        if (ws_pae_key_storage_array_lifetime_update(time_difference, &storage_array_entry->ptk_lifetime) < 0) {
            tr_info("KeyS time update PTK expired diff: %"PRIi64" t: %i %i eui64: %s", time_difference, STIME_TIME_GET(storage_array_entry->ptk_lifetime), STIME_FORMAT_GET(storage_array_entry->ptk_lifetime), tr_array(storage_array_entry->ptk_eui_64, 8));
            // PTK is invalid, invalidate PTK related fields
            ws_pae_key_storage_array_ptk_invalid(storage_array_entry);
            // PMK is still valid
            return 0;
        }
#ifdef EXTRA_DEBUG_INFO
        tr_debug("KeyS time update PTK NEW t: %i %i", STIME_TIME_GET(storage_array_entry->ptk_lifetime), STIME_FORMAT_GET(storage_array_entry->ptk_lifetime));
#endif
    }
    return 0;
}

static int8_t ws_pae_key_storage_array_time_check_and_update_all(key_storage_array_t *key_storage_array, bool modified)
{
    uint64_t reference_time = key_storage_array->storage_array_handle->reference_time;
    uint64_t current_time = ws_pae_current_time_get();

    uint32_t time_difference;
    if (ws_pae_time_diff_calc(current_time, reference_time, &time_difference, false) < 0) {
        tr_error("KeyS array time all err: %"PRIi64", ref: %"PRIi64", diff: %"PRIi32, ws_pae_current_time_get(), reference_time, time_difference);
        return -1;
    }

    if (modified) {
        // Updates once a day on write
        if (time_difference < KEY_STORAGE_REF_TIME_UPDATE_THRESHOLD) {
            return 0;
        }
    } else if (time_difference < KEY_STORAGE_REF_TIME_UPDATE_FORCE_THRESHOLD) {
        // Or every 31 days also when no other writes triggered
        return 0;
    }

    // Checks entries in storage array
    sec_prot_keys_storage_t *storage_array = (sec_prot_keys_storage_t *) key_storage_array->storage_array;
    for (uint16_t index = 0; index < key_storage_array->entries; index++) {
        if (!storage_array[index].eui_64_set) {
            continue;
        }
        // Updates lifetimes on the entry
        ws_pae_key_storage_array_time_update_entry(time_difference, &storage_array[index]);
    }

    // Entries are now on current time; update reference time
    key_storage_array->storage_array_handle->reference_time = current_time;
    return 1;
}

static int8_t ws_pae_key_storage_array_counters_check_and_update_all(key_storage_array_t *key_storage_array)
{
    // Checks entries in storage array
    sec_prot_keys_storage_t *storage_array = (sec_prot_keys_storage_t *) key_storage_array->storage_array;
    for (uint16_t index = 0; index < key_storage_array->entries; index++) {
        if (!storage_array[index].eui_64_set) {
            continue;
        }

        // GTK 4WH hash information is not stored to NVM and might be obsolete (changing it does not trigger NVM write)
        storage_array[index].ins_gtk_4wh_hash_set = 0;

        if (!storage_array[index].pmk_key_replay_cnt_set) {
            continue;
        }

        // Sanity check for replay counter
        if (storage_array[index].pmk_key_replay_cnt >= PMK_KEY_REPLAY_CNT_LIMIT) {
            ws_pae_key_storage_array_pmk_invalid(&storage_array[index]);
            ws_pae_key_storage_array_ptk_invalid(&storage_array[index]);
            continue;
        }

        /* Resets replay counter (lower part). When generating 64bit replay counter used on EAPOL,
           re-start count is set to replay counter MSB 32bits. So the lower part of the counter
           is always fresh after re-start. Thus, each power cycle of device has LSB 32bits of replay
           counter space to use. In practice uses only LSB 16bits since counter is limited to 60000,
           before generating new PMK. */
        storage_array[index].pmk_key_replay_cnt = 0;
    }

    return 0;
}

static int8_t ws_pae_key_storage_array_lifetime_update(uint32_t time_difference, uint16_t *lifetime)
{
    uint32_t entry_lifetime = ws_pae_time_from_short_convert(*lifetime);
    *lifetime = 0;

    // If lifetime has expired, return failure
    if (time_difference >= entry_lifetime) {
        return -1;
    }
    entry_lifetime -= time_difference;
    *lifetime = ws_pae_time_to_short_convert(entry_lifetime);
    // If conversion results zero lifetime return failure
    if (*lifetime == 0) {
        return -1;
    }

    // Lifetime is valid
    return 0;
}

static int8_t ws_pae_key_storage_array_lifetime_get(uint32_t time_difference, uint16_t short_lifetime, uint32_t *lifetime)
{
    *lifetime = ws_pae_time_from_short_convert(short_lifetime);

    // If lifetime has expired, return failure
    if (time_difference >= *lifetime) {
        return -1;
    }
    *lifetime -= time_difference;

    return 0;
}

static void ws_pae_key_storage_array_pmk_invalid(sec_prot_keys_storage_t *storage_array)
{
    memset(storage_array, 0, sizeof(sec_prot_keys_storage_t));
}

static void ws_pae_key_storage_array_ptk_invalid(sec_prot_keys_storage_t *storage_array)
{
    memset(storage_array->ptk, 0, PTK_LEN);
    storage_array->ptk_set = false;
    storage_array->ptk_eui_64_set = false;
    memset(storage_array->ins_gtk_hash, 0, sizeof(storage_array->ins_gtk_hash));
    storage_array->ins_gtk_hash_set = false;
    storage_array->ptk_lifetime = 0;
}

#endif /* HAVE_WS */

