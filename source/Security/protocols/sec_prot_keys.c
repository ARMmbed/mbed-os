/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/ipv6_constants.h"
#include "socket_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "Security/protocols/sec_prot_cfg.h"
#include "Security/kmp/kmp_addr.h"
#include "Security/kmp/kmp_api.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "Security/protocols/sec_prot.h"
#include "Security/protocols/sec_prot_lib.h"

#ifdef HAVE_WS

#define TRACE_GROUP "spke"

static const uint8_t empty_hash[GTK_HASH_LEN] = {0};

sec_prot_keys_t *sec_prot_keys_create(sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs)
{
    sec_prot_keys_t *sec_keys = ns_dyn_mem_alloc(sizeof(sec_prot_keys_t));
    if (!sec_keys) {
        return NULL;
    }

    sec_prot_keys_init(sec_keys, gtks, certs);

    return sec_keys;
}

void sec_prot_keys_init(sec_prot_keys_t *sec_keys, sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs)
{
    memset(sec_keys, 0, sizeof(sec_prot_keys_t));
    sec_keys->pmk_lifetime = PMK_LIFETIME_INSTALL;
    sec_keys->ptk_lifetime = PTK_LIFETIME_INSTALL;
    sec_keys->pmk_key_replay_cnt = 0;
    sec_keys->gtks = gtks;
    sec_keys->certs = certs;
    sec_keys->gtkl = 0;
    sec_keys->gtk_set_index = -1;
    sec_keys->pmk_set = false;
    sec_keys->ptk_set = false;
    sec_keys->pmk_key_replay_cnt_set = false;
    sec_keys->updated = false;
    sec_keys->ptk_eui_64_set = false;
    sec_keys->pmk_mismatch = false;
    sec_keys->ptk_mismatch = false;
    sec_prot_keys_ptk_installed_gtk_hash_clear_all(sec_keys);
}

void sec_prot_keys_delete(sec_prot_keys_t *sec_keys)
{
    ns_dyn_mem_free(sec_keys);
}

sec_prot_gtk_keys_t *sec_prot_keys_gtks_create(void)
{
    sec_prot_gtk_keys_t *gtks = ns_dyn_mem_alloc(sizeof(sec_prot_gtk_keys_t));
    if (!gtks) {
        return NULL;
    }

    sec_prot_keys_gtks_init(gtks);

    return gtks;
}

void sec_prot_keys_gtks_init(sec_prot_gtk_keys_t *gtks)
{
    memset(gtks, 0, sizeof(sec_prot_gtk_keys_t));
    gtks->updated = false;
}

void sec_prot_keys_gtks_delete(sec_prot_gtk_keys_t *gtks)
{
    ns_dyn_mem_free(gtks);
}

void sec_prot_keys_pmk_write(sec_prot_keys_t *sec_keys, uint8_t *pmk)
{
    memcpy(sec_keys->pmk, pmk, PMK_LEN);
    sec_keys->pmk_key_replay_cnt = 0;
    sec_keys->pmk_key_replay_cnt_set = false;
    sec_keys->pmk_lifetime = PMK_LIFETIME_INSTALL;
    sec_keys->pmk_set = true;
    sec_keys->updated = true;
}

void sec_prot_keys_pmk_delete(sec_prot_keys_t *sec_keys)
{
    memset(sec_keys->pmk, 0, PMK_LEN);
    sec_keys->pmk_key_replay_cnt = 0;
    sec_keys->pmk_key_replay_cnt_set = false;
    sec_keys->pmk_lifetime = PMK_LIFETIME_INSTALL;
    sec_keys->pmk_set = false;
    sec_keys->updated = true;
}

uint8_t *sec_prot_keys_pmk_get(sec_prot_keys_t *sec_keys)
{
    if (!sec_keys->pmk_set) {
        return NULL;
    }

    return sec_keys->pmk;
}

uint64_t sec_prot_keys_pmk_replay_cnt_get(sec_prot_keys_t *sec_keys)
{
    return sec_keys->pmk_key_replay_cnt;
}

void sec_prot_keys_pmk_replay_cnt_set(sec_prot_keys_t *sec_keys, uint64_t counter)
{
    sec_keys->pmk_key_replay_cnt_set = true;
    sec_keys->pmk_key_replay_cnt = counter;
}

void sec_prot_keys_pmk_replay_cnt_increment(sec_prot_keys_t *sec_keys)
{
    // Start from zero i.e. does not increment on first call
    if (!sec_keys->pmk_key_replay_cnt_set) {
        sec_keys->pmk_key_replay_cnt_set = true;
        return;
    }
    sec_keys->pmk_key_replay_cnt++;
}

bool sec_prot_keys_pmk_replay_cnt_compare(uint64_t received_counter, sec_prot_keys_t *sec_keys)
{
    // If previous value is set must be greater
    if (sec_keys->pmk_key_replay_cnt_set && received_counter > sec_keys->pmk_key_replay_cnt) {
        return true;
    } else if (!sec_keys->pmk_key_replay_cnt_set && received_counter >= sec_keys->pmk_key_replay_cnt) {
        // Otherwise allows also same value e.g. zero
        return true;
    }

    return false;
}

void sec_prot_keys_pmk_mismatch_set(sec_prot_keys_t *sec_keys)
{
    sec_keys->pmk_mismatch = true;
}

void sec_prot_keys_pmk_mismatch_reset(sec_prot_keys_t *sec_keys)
{
    sec_keys->pmk_mismatch = false;
}

bool sec_prot_keys_pmk_mismatch_is_set(sec_prot_keys_t *sec_keys)
{
    return sec_keys->pmk_mismatch;
}

bool sec_prot_keys_pmk_lifetime_decrement(sec_prot_keys_t *sec_keys, uint32_t default_lifetime, uint8_t seconds)
{
    if (!sec_keys->pmk_set) {
        return false;
    }

    if (sec_keys->pmk_lifetime == PMK_LIFETIME_INSTALL) {
        sec_keys->pmk_lifetime = default_lifetime;
    }

    if (sec_keys->pmk_lifetime > seconds) {
        sec_keys->pmk_lifetime -= seconds;
    } else {
        if (sec_keys->pmk_lifetime > 0) {
            sec_keys->pmk_lifetime = 0;
            sec_prot_keys_ptk_delete(sec_keys);
            sec_prot_keys_pmk_delete(sec_keys);
            return true;
        }
    }
    return false;
}

void sec_prot_keys_ptk_write(sec_prot_keys_t *sec_keys, uint8_t *ptk)
{
    memcpy(sec_keys->ptk, ptk, PTK_LEN);
    sec_keys->ptk_lifetime = PTK_LIFETIME_INSTALL;
    sec_keys->ptk_set = true;
    sec_keys->updated = true;
}

void sec_prot_keys_ptk_delete(sec_prot_keys_t *sec_keys)
{
    memset(sec_keys->ptk, 0, PTK_LEN);
    sec_keys->ptk_lifetime = PTK_LIFETIME_INSTALL;
    sec_keys->ptk_set = false;
    sec_keys->updated = true;
}

uint8_t *sec_prot_keys_ptk_get(sec_prot_keys_t *sec_keys)
{
    if (!sec_keys->ptk_set) {
        return NULL;
    }

    return sec_keys->ptk;
}

void sec_prot_keys_ptk_mismatch_set(sec_prot_keys_t *sec_keys)
{
    sec_keys->ptk_mismatch = true;
}

void sec_prot_keys_ptk_mismatch_reset(sec_prot_keys_t *sec_keys)
{
    sec_keys->ptk_mismatch = false;
}

bool sec_prot_keys_ptk_mismatch_is_set(sec_prot_keys_t *sec_keys)
{
    return sec_keys->ptk_mismatch;
}

void sec_prot_keys_ptk_eui_64_write(sec_prot_keys_t *sec_keys, const uint8_t *eui_64)
{
    memcpy(sec_keys->ptk_eui_64, eui_64, 8);
    sec_keys->ptk_eui_64_set = true;
    sec_keys->updated = true;
}

uint8_t *sec_prot_keys_ptk_eui_64_get(sec_prot_keys_t *sec_keys)
{
    if (!sec_keys->ptk_eui_64_set) {
        return NULL;
    }

    return sec_keys->ptk_eui_64;
}

void sec_prot_keys_ptk_eui_64_delete(sec_prot_keys_t *sec_keys)
{
    memset(sec_keys->ptk_eui_64, 0, 8);
    sec_keys->ptk_eui_64_set = false;
    sec_keys->updated = true;
}

bool sec_prot_keys_ptk_lifetime_decrement(sec_prot_keys_t *sec_keys, uint32_t default_lifetime, uint8_t seconds)
{
    if (!sec_keys->ptk_set) {
        return false;
    }

    if (sec_keys->ptk_lifetime == PTK_LIFETIME_INSTALL) {
        sec_keys->ptk_lifetime = default_lifetime;
    }

    if (sec_keys->ptk_lifetime > seconds) {
        sec_keys->ptk_lifetime -= seconds;
    } else {
        if (sec_keys->ptk_lifetime > 0) {
            sec_prot_keys_ptk_delete(sec_keys);
            sec_keys->ptk_lifetime = 0;
            return true;
        }
    }
    return false;
}

bool sec_prot_keys_are_updated(sec_prot_keys_t *sec_keys)
{
    return sec_keys->updated;
}

void sec_prot_keys_updated_reset(sec_prot_keys_t *sec_keys)
{
    sec_keys->updated = false;
}

uint8_t sec_prot_keys_fresh_gtkl_get(sec_prot_gtk_keys_t *gtks)
{
    uint8_t gtkl = 0;

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_status_is_live(gtks, i)) {
            gtkl |= 1 << i;
        }
    }

    return gtkl;
}

void sec_prot_keys_gtkl_set(sec_prot_keys_t *sec_keys, uint8_t gtkl)
{
    sec_keys->gtkl = gtkl;
}

bool sec_prot_keys_gtkl_gtk_is_live(sec_prot_keys_t *sec_keys, uint8_t index)
{
    if (index >= GTK_NUM) {
        return false;
    }

    if (sec_keys->gtkl & (1 << index)) {
        return true;
    }

    return false;
}

int8_t sec_prot_keys_gtkl_gtk_live_set(sec_prot_keys_t *sec_keys, uint8_t index)
{
    if (index >= GTK_NUM) {
        return -1;
    }

    sec_keys->gtkl |= (1 << index);

    return 0;
}

int8_t sec_prot_keys_gtk_insert_index_set(sec_prot_keys_t *sec_keys, uint8_t index)
{
    if (index >= GTK_NUM || !sec_keys->gtks->gtk[index].set) {
        return -1;
    }

    sec_keys->gtk_set_index = index;
    return 0;
}

int8_t sec_prot_keys_gtk_insert_index_get(sec_prot_keys_t *sec_keys)
{
    return sec_keys->gtk_set_index;
}

void sec_prot_keys_gtk_insert_index_clear(sec_prot_keys_t *sec_keys)
{
    sec_keys->gtk_set_index = -1;
}

void sec_prot_keys_gtkl_from_gtk_insert_index_set(sec_prot_keys_t *sec_keys)
{
    if (sec_keys->gtk_set_index >= 0) {
        sec_prot_keys_gtkl_gtk_live_set(sec_keys, sec_keys->gtk_set_index);
        sec_prot_keys_gtk_insert_index_clear(sec_keys);
    }
}

int8_t sec_prot_keys_gtk_insert_index_from_gtkl_get(sec_prot_keys_t *sec_keys)
{
    // Get currently active key index
    int8_t active_index = sec_prot_keys_gtk_status_active_get(sec_keys->gtks);

    if (active_index >= 0 && !sec_prot_keys_gtkl_gtk_is_live(sec_keys, active_index)) {
        // If currently active key is not live on remote, inserts it
        sec_prot_keys_gtk_insert_index_set(sec_keys, active_index);
        return active_index;
    }

    // Checks all keys
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_status_is_live(sec_keys->gtks, i)) {
            // If key is live, but not indicated on GTKL inserts it
            if (!sec_prot_keys_gtkl_gtk_is_live(sec_keys, i)) {
                sec_prot_keys_gtk_insert_index_set(sec_keys, i);
                return i;
            }
        }
    }

    return -1;
}

uint8_t *sec_prot_keys_get_gtk_to_insert(sec_prot_keys_t *sec_keys, uint8_t *index)
{
    if (sec_keys->gtk_set_index >= 0 && sec_keys->gtks->gtk[sec_keys->gtk_set_index].set) {
        *index = sec_keys->gtk_set_index;
        return sec_keys->gtks->gtk[sec_keys->gtk_set_index].key;
    } else {
        return NULL;
    }
}

int8_t sec_prot_keys_gtk_set(sec_prot_gtk_keys_t *gtks, uint8_t index, uint8_t *gtk, uint32_t lifetime)
{
    if (!gtk || index >= GTK_NUM) {
        return -1;
    }

    // If same GTK is given again, do not update
    if (gtks->gtk[index].set && memcmp(gtks->gtk[index].key, gtk, GTK_LEN) == 0) {
        return -1;
    }

    sec_prot_keys_gtk_clear(gtks, index);
    uint8_t install_order = sec_prot_keys_gtk_install_order_last_get(gtks);

    gtks->gtk[index].set = true;
    gtks->gtk[index].lifetime = lifetime;
    gtks->gtk[index].status = GTK_STATUS_NEW;
    gtks->gtk[index].install_order = install_order;
    memcpy(gtks->gtk[index].key, gtk, GTK_LEN);

    gtks->updated = true;

    return 0;
}

int8_t sec_prot_keys_gtk_clear(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (!gtks || index >= GTK_NUM) {
        return -1;
    }

    gtks->gtk[index].set = false;
    gtks->gtk[index].lifetime = 0;   // Should be provided by authenticator
    gtks->gtk[index].status = GTK_STATUS_NEW;
    memset(gtks->gtk[index].key, 0, GTK_LEN);

    sec_prot_keys_gtk_install_order_update(gtks);

    return 0;
}

bool sec_prot_keys_gtk_is_set(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return false;
    }

    return true;
}

uint8_t *sec_prot_keys_gtk_get(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return NULL;
    }

    return gtks->gtk[index].key;
}

uint32_t sec_prot_keys_gtk_lifetime_get(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return 0;
    }

    return gtks->gtk[index].lifetime;
}

uint32_t sec_prot_keys_gtk_lifetime_decrement(sec_prot_gtk_keys_t *gtks, uint8_t index, uint16_t seconds)
{
    if (gtks->gtk[index].lifetime > seconds) {
        gtks->gtk[index].lifetime -= seconds;
    } else {
        gtks->gtk[index].lifetime = 0;
    }

    return gtks->gtk[index].lifetime;
}

bool sec_prot_keys_gtks_are_updated(sec_prot_gtk_keys_t *gtks)
{
    return gtks->updated;
}

void sec_prot_keys_gtks_updated_set(sec_prot_gtk_keys_t *gtks)
{
    gtks->updated = true;
}

void sec_prot_keys_gtks_updated_reset(sec_prot_gtk_keys_t *gtks)
{
    gtks->updated = false;
}

void sec_prot_keys_gtk_status_fresh_set(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return;
    }

    // Active key remains as active, old keys are never reused
    if (gtks->gtk[index].status < GTK_STATUS_FRESH) {
        gtks->gtk[index].status = GTK_STATUS_FRESH;
    }
}

void sec_prot_keys_gtk_status_all_fresh_set(sec_prot_gtk_keys_t *gtks)
{
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        sec_prot_keys_gtk_status_fresh_set(gtks, i);
    }
}

int8_t sec_prot_keys_gtk_status_active_set(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return -1;
    }

    // If key is valid to be taken into use sets it active
    if (gtks->gtk[index].status == GTK_STATUS_FRESH) {
        // Sets previously active key old
        for (uint8_t i = 0; i < GTK_NUM; i++) {
            // Sets previously active key old
            if (gtks->gtk[i].status == GTK_STATUS_ACTIVE) {
                gtks->gtk[i].status = GTK_STATUS_OLD;
            }
        }
        gtks->gtk[index].status = GTK_STATUS_ACTIVE;
        return 0;
    }

    return -1;
}

int8_t sec_prot_keys_gtk_status_active_get(sec_prot_gtk_keys_t *gtks)
{
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (gtks->gtk[i].status == GTK_STATUS_ACTIVE) {
            return i;
        }
    }

    return -1;
}

bool sec_prot_keys_gtk_status_is_live(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return false;
    }

    if (gtks->gtk[index].status == GTK_STATUS_FRESH || gtks->gtk[index].status == GTK_STATUS_ACTIVE) {
        return true;
    }

    return false;
}

void sec_prot_keys_gtk_status_new_set(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return;
    }

    gtks->gtk[index].status = GTK_STATUS_NEW;
}

void sec_prot_keys_gtks_hash_generate(sec_prot_gtk_keys_t *gtks, uint8_t *gtkhash)
{
    memset(gtkhash, 0, GTK_ALL_HASHES_LEN);

    uint8_t *gtk_hash_ptr = gtkhash;

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);
            sec_prot_lib_gtkhash_generate(gtk, gtk_hash_ptr);
        }
        gtk_hash_ptr += GTK_HASH_LEN;
    }
}

int8_t sec_prot_keys_gtk_hash_generate(uint8_t *gtk, uint8_t *gtk_hash)
{
    return sec_prot_lib_gtkhash_generate(gtk, gtk_hash);
}

int8_t sec_prot_keys_gtk_valid_check(uint8_t *gtk)
{
    uint8_t gtk_hash[8];
    sec_prot_lib_gtkhash_generate(gtk, gtk_hash);

    // Checks if GTK hash for the GTK would be all zero
    if (memcmp(gtk_hash, empty_hash, GTK_HASH_LEN) == 0) {
        return -1;
    }

    return 0;
}

gtk_mismatch_e sec_prot_keys_gtks_hash_update(sec_prot_gtk_keys_t *gtks, uint8_t *gtkhash)
{
    uint8_t *gtk_hash_ptr = gtkhash;

    gtk_mismatch_e mismatch = GTK_NO_MISMATCH;

    for (uint8_t i = 0; i < GTK_NUM; i++, gtk_hash_ptr += 8) {
        // If hash is not set, stop using the key
        if (sec_prot_keys_gtk_hash_empty(gtk_hash_ptr)) {
            if (sec_prot_keys_gtk_is_set(gtks, i)) {
                uint32_t lifetime = sec_prot_keys_gtk_lifetime_get(gtks, i);
                if (lifetime > GTK_EXPIRE_MISMATCH_TIME) {
                    tr_info("GTK mismatch %i expired time, lifetime: %"PRIu32"", i, lifetime);
                    if (mismatch < GTK_LIFETIME_MISMATCH) {
                        mismatch = GTK_LIFETIME_MISMATCH;
                    }
                }
                sec_prot_keys_gtk_clear(gtks, i);
            }
        } else {
            // Check is hash matches to existing key
            uint8_t gtk_hash[8];
            uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);
            if (!gtk) {
                // Hash set but GTK is not known, set mismatch
                tr_info("GTK mismatch: %i", i);
                if (mismatch < GTK_HASH_MISMATCH) {
                    mismatch = GTK_HASH_MISMATCH;
                }
                continue;
            }

            sec_prot_lib_gtkhash_generate(gtk, gtk_hash);

            if (memcmp(gtk_hash, gtk_hash_ptr, 8) == 0) {
                // Key is fresh (or active, if old do not change state)
                sec_prot_keys_gtk_status_fresh_set(gtks, i);
            } else {
                // Hash does not match, set mismatch and delete key
                tr_info("GTK mismatch: %i", i);
                if (mismatch < GTK_HASH_MISMATCH) {
                    mismatch = GTK_HASH_MISMATCH;
                }
                sec_prot_keys_gtk_clear(gtks, i);
            }
        }
    }

    return mismatch;
}

bool sec_prot_keys_gtk_hash_empty(uint8_t *gtkhash)
{
    if (memcmp(gtkhash, empty_hash, GTK_HASH_LEN) == 0) {
        return true;
    } else {
        return false;
    }
}

int8_t sec_prot_keys_gtk_install_order_last_get(sec_prot_gtk_keys_t *gtks)
{
    int8_t install_order = -1;

    // Gets the last key index
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            if (gtks->gtk[i].install_order > install_order) {
                install_order = gtks->gtk[i].install_order;
            }
        }
    }

    return install_order + 1;
}

int8_t sec_prot_keys_gtk_install_order_last_index_get(sec_prot_gtk_keys_t *gtks)
{
    int8_t install_order = -1;
    int8_t index = -1;

    // Gets the last key index
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            if (gtks->gtk[i].install_order > install_order) {
                install_order = gtks->gtk[i].install_order;
                index = i;
            }
        }
    }

    return index;
}

uint32_t sec_prot_keys_gtk_install_order_last_lifetime_get(sec_prot_gtk_keys_t *gtks)
{
    uint32_t lifetime = 0;
    int8_t install_order = -1;

    // Gets the last key index
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            if (gtks->gtk[i].install_order > install_order) {
                install_order = gtks->gtk[i].install_order;
                lifetime = gtks->gtk[i].lifetime;
            }
        }
    }

    return lifetime;
}

int8_t sec_prot_keys_gtk_install_order_first_index_get(sec_prot_gtk_keys_t *gtks)
{
    // Gets the first key index
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            if (gtks->gtk[i].install_order == GTK_INSTALL_ORDER_FIRST) {
                return i;
            }
        }
    }

    return -1;
}

int8_t sec_prot_keys_gtk_install_order_second_index_get(sec_prot_gtk_keys_t *gtks)
{
    // Gets the first key index
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            if (gtks->gtk[i].install_order == GTK_INSTALL_ORDER_SECOND) {
                return i;
            }
        }
    }

    return -1;
}

void sec_prot_keys_gtk_install_order_update(sec_prot_gtk_keys_t *gtks)
{
    int8_t ordered_indexes[4] = {-1, -1, -1, -1};

    // Creates table of ordered indexes
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            ordered_indexes[gtks->gtk[i].install_order] = i;
        }
    }

    // Updates indexes of the GTKs
    uint8_t new_install_order = 0;
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (ordered_indexes[i] >= 0) {
            gtks->gtk[ordered_indexes[i]].install_order = new_install_order++;
        }
    }
}

int8_t sec_prot_keys_gtk_install_index_get(sec_prot_gtk_keys_t *gtks)
{
    // Gets the index of the last key to be installed
    int8_t install_index = sec_prot_keys_gtk_install_order_last_index_get(gtks);
    if (install_index < 0) {
        install_index = 0;
    }

    // Checks if there is free index, and available uses that for new GTK
    for (uint8_t ctr = 0, i = install_index; ctr < GTK_NUM; ctr++) {
        if (!sec_prot_keys_gtk_is_set(gtks, i)) {
            install_index = i;
            break;
        }
        i++;
        if (i >= GTK_NUM) {
            i = 0;
        }
    }

    return install_index;
}

uint8_t sec_prot_keys_gtk_count(sec_prot_gtk_keys_t *gtks)
{
    uint8_t count = 0;

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            count++;
        }
    }

    return count;
}

void sec_prot_keys_ptk_installed_gtk_hash_clear_all(sec_prot_keys_t *sec_keys)
{
    for (uint8_t index = 0; index < GTK_NUM; index++) {
        memset(sec_keys->ins_gtk_hash[sec_keys->gtk_set_index].hash, 0, INS_GTK_HASH_LEN);
    }
    sec_keys->ins_gtk_hash_set = 0;
}

void sec_prot_keys_ptk_installed_gtk_hash_set(sec_prot_keys_t *sec_keys)
{
    if (sec_keys->gtk_set_index >= 0) {
        uint8_t *gtk = sec_prot_keys_gtk_get(sec_keys->gtks, sec_keys->gtk_set_index);
        if (!gtk) {
            return;
        }
        uint8_t gtk_hash[GTK_HASH_LEN];
        if (sec_prot_keys_gtk_hash_generate(gtk, gtk_hash) < 0) {
            return;
        }
        /* Store two byte hash. This is long enough for the GTK installed check, since
         * possible conflict between hashes causes only that 4WH is initiated/is not
         * initiated instead of GKH.
         */
        memcpy(sec_keys->ins_gtk_hash[sec_keys->gtk_set_index].hash, gtk_hash, INS_GTK_HASH_LEN);
        sec_keys->ins_gtk_hash_set |= (1 << sec_keys->gtk_set_index);
    }
}

bool sec_prot_keys_ptk_installed_gtk_hash_mismatch_check(sec_prot_keys_t *sec_keys, uint8_t gtk_index)
{
    if ((sec_keys->ins_gtk_hash_set & (1 << sec_keys->gtk_set_index)) == 0) {
        return false;
    }

    uint8_t *gtk = sec_prot_keys_gtk_get(sec_keys->gtks, gtk_index);
    if (!gtk) {
        return false;
    }

    // Calculated GTK hash for the current GTK on the defined index
    uint8_t gtk_hash[GTK_HASH_LEN];
    if (sec_prot_keys_gtk_hash_generate(gtk, gtk_hash) < 0) {
        return false;
    }

    // If PTK has been used to install different GTK to index than the current one, trigger mismatch
    if (memcmp(sec_keys->ins_gtk_hash[sec_keys->gtk_set_index].hash, gtk_hash, INS_GTK_HASH_LEN) != 0) {
        return true;
    }

    return false;
}

#endif /* HAVE_WS */
