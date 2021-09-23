/*
 * Copyright (c) 2019-2021, Pelion and affiliates.
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
#include "common_functions.h"
#include "6LoWPAN/ws/ws_config.h"
#include "ns_file_system.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/utils/ns_file.h"
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "6LoWPAN/ws/ws_pae_nvm_data.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_pae_time.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsnv"

#define PAE_NVM_FIELD_NOT_SET            0   // Field is not present
#define PAE_NVM_FIELD_SET                1   // Field is present

void ws_pae_nvm_store_nw_info_tlv_create(nw_info_nvm_tlv_t *tlv_entry, uint16_t pan_id, char *nw_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t stored_time, uint8_t time_changed)
{
    int len;
    tlv_entry->tag = PAE_NVM_NW_INFO_TAG;
    tlv_entry->len = PAE_NVM_NW_INFO_LEN;

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    tlv = common_write_16_bit(pan_id, tlv);

    memset(tlv, 0, 33);
    // use strnlen & memset instead of strncpy to avoid gcc warning:
    // call to __builtin___strncpy_chk will always overflow destination buffer [-Werror]
    len = strlen(nw_name);
    if (len > 32) {
        len = 32;
    }
    memcpy((char *)tlv, nw_name, len);
    tlv += 33;

    memcpy((char *)tlv, gtk_eui64, 8);
    tlv += 8;

    *tlv++ = time_changed;
    tlv = common_write_64_bit(stored_time, tlv);

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (gtks && sec_prot_keys_gtk_is_set(gtks, i)) {
            *tlv++ = PAE_NVM_FIELD_SET; // GTK is set

            uint64_t expirytime = sec_prot_keys_gtk_exptime_from_lifetime_get(gtks, i, stored_time);
            // Sets stored expiration time to GTKs; no need to update anymore to NVM if not changed
            sec_prot_keys_gtk_expirytime_set(gtks, i, expirytime);

            tlv = common_write_64_bit(expirytime, tlv);

            uint8_t status = sec_prot_keys_gtk_status_get(gtks, i);
            *tlv++ = status;

            uint8_t install_order = sec_prot_keys_gtk_install_order_get(gtks, i);
            *tlv++ = install_order;

            uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);
            memcpy(tlv, gtk, GTK_LEN);
            tlv += GTK_LEN;
        } else {
            *tlv++ = PAE_NVM_FIELD_NOT_SET; // GTK is not set
            memset(tlv, 0, 8 + 1 + 1 + GTK_LEN);
            tlv += 8 + 1 + 1 + GTK_LEN;
        }
    }

    tr_info("NVM NW_INFO write PAN ID: %i name: %s stored time: %"PRIu64, pan_id, nw_name, stored_time);

}

int8_t ws_pae_nvm_store_nw_info_tlv_read(nw_info_nvm_tlv_t *tlv_entry, uint16_t *pan_id, char *nw_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks, uint64_t current_time, uint8_t *time_changed)
{
    if (!tlv_entry || !pan_id || !nw_name) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_NW_INFO_TAG || tlv_entry->len != PAE_NVM_NW_INFO_LEN) {
        return -1;
    }

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    if (*pan_id == 0xffff) {
        // If application has not set pan_id read it from NVM
        *pan_id = common_read_16_bit(tlv);
    }
    tlv += 2;

    if (strlen(nw_name) == 0) {
        // If application has not set network name read it from NVM
        memset(nw_name, 0, 33);
        strncpy(nw_name, (char *) tlv, 32);
    }
    tlv += 33;

    memcpy(gtk_eui64, (char *)tlv, 8);
    tlv += 8;

    *time_changed = *tlv++;
    uint64_t stored_time = common_read_64_bit(tlv);
    tlv += 8;
    // Use either stored time or current time as reference when calculating lifetimes
    current_time = ws_pae_time_old_or_new_select(stored_time, current_time);
    /* If the selected time has made a large jump then use the stored time to calculate the lifetime
       (this implies e.g. that new time has been received from NTP and old time was not valid) */
    if (*time_changed == SYSTEM_TIME_NOT_CHANGED && !ws_pae_time_old_and_new_validate(stored_time, current_time)) {
        *time_changed = SYSTEM_TIME_CHANGED;
        current_time = stored_time;
        tr_info("NVM NW_INFO system time change");
    }

    tr_info("NVM NW_INFO current time: %"PRIi64" stored time: %"PRIi64, current_time, stored_time);

    if (gtks && sec_prot_keys_gtk_count(gtks) == 0) {
        // If application has not set GTKs read them from NVM
        for (uint8_t i = 0; i < GTK_NUM; i++) {
            if (*tlv++ == PAE_NVM_FIELD_SET) { /* GTK is set */
                uint64_t expirytime = common_read_64_bit(tlv);
                uint32_t lifetime = 0;
                // Calculate lifetime
                if (ws_pae_time_diff_calc(current_time, expirytime, &lifetime, true) < 0) {
                    tlv += 8 + 1 + 1 + GTK_LEN;
                    tr_info("GTK index %i, expired expiry time: %"PRIi64", lifetime: %"PRIi32, i, expirytime, lifetime);
                    continue;
                }
                tlv += 8;

                uint8_t status = *tlv++;

                uint8_t install_order = *tlv++;

                tr_info("GTK index: %i, status: %i, install order %i, expiry time: %"PRIi64", lifetime: %"PRIi32, i, status, install_order, expirytime, lifetime);

                sec_prot_keys_gtk_set(gtks, i, tlv, lifetime);
                sec_prot_keys_gtk_expirytime_set(gtks, i, expirytime);
                tlv += GTK_LEN;
                sec_prot_keys_gtk_status_set(gtks, i, status);
                sec_prot_keys_gtk_install_order_set(gtks, i, install_order);
            } else {
                tlv += 8 + 1 + 1 + GTK_LEN;
            }
        }
        sec_prot_keys_gtks_updated_reset(gtks);
    }

    tr_info("NVM NW_INFO read PAN ID %i name: %s", *pan_id, nw_name);

    return 0;
}

void ws_pae_nvm_store_keys_tlv_create(keys_nvm_tlv_t *tlv_entry, sec_prot_keys_t *sec_keys)
{
    tlv_entry->tag = PAE_NVM_KEYS_TAG;
    tlv_entry->len = PAE_NVM_KEYS_LEN;

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    uint8_t *eui_64 = sec_prot_keys_ptk_eui_64_get(sec_keys);
    if (eui_64) {
        *tlv++ = PAE_NVM_FIELD_SET;
        memcpy(tlv, eui_64, 8);
    } else {
        *tlv++ = PAE_NVM_FIELD_NOT_SET;
        memset(tlv, 0, 8);
    }
    tlv += 8;

    uint8_t *pmk = sec_prot_keys_pmk_get(sec_keys);
    if (pmk) {
        *tlv++ = PAE_NVM_FIELD_SET;
        uint32_t lifetime = sec_prot_keys_pmk_lifetime_get(sec_keys);
        tlv = common_write_32_bit(lifetime, tlv);
        memcpy(tlv, pmk, PMK_LEN);
    } else {
        *tlv++ = PAE_NVM_FIELD_NOT_SET;
        memset(tlv, 0, 4 + PMK_LEN);
    }
    tlv += PMK_LEN;

    uint64_t counter = sec_prot_keys_pmk_replay_cnt_get(sec_keys);
    tlv = common_write_64_bit(counter, tlv);

    uint8_t *ptk = sec_prot_keys_ptk_get(sec_keys);
    if (ptk) {
        *tlv++ = PAE_NVM_FIELD_SET;
        uint32_t lifetime = sec_prot_keys_ptk_lifetime_get(sec_keys);
        tlv = common_write_32_bit(lifetime, tlv);
        memcpy(tlv, ptk, PTK_LEN);
    } else {
        *tlv++ = PAE_NVM_FIELD_NOT_SET;
        memset(tlv, 0, 4 + PTK_LEN);
    }
    tlv += PTK_LEN;

    tr_info("NVM KEYS write");
}

int8_t ws_pae_nvm_store_keys_tlv_read(keys_nvm_tlv_t *tlv_entry, sec_prot_keys_t *sec_keys)
{
    if (!tlv_entry || !sec_keys) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_KEYS_TAG || tlv_entry->len != PAE_NVM_KEYS_LEN) {
        return -1;
    }

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    // EUI-64 set */
    if (*tlv++ == PAE_NVM_FIELD_SET) {
        sec_prot_keys_ptk_eui_64_write(sec_keys, tlv);
    }
    tlv += 8;

    // PMK set
    if (*tlv++ == PAE_NVM_FIELD_SET) {
        uint32_t lifetime = common_read_32_bit(tlv);
        tlv += 4;
        sec_prot_keys_pmk_write(sec_keys, tlv, lifetime);
    } else {
        tlv += 4;
    }
    tlv += PMK_LEN;

    uint64_t counter = common_read_64_bit(tlv);
    tlv += 8;
    sec_prot_keys_pmk_replay_cnt_set(sec_keys, counter);

    // PTK set
    if (*tlv++ == PAE_NVM_FIELD_SET) {
        uint32_t lifetime = common_read_32_bit(tlv);
        tlv += 4;
        sec_prot_keys_ptk_write(sec_keys, tlv, lifetime);
    } else {
        tlv += 4;
    }

    tlv += PTK_LEN;

    sec_prot_keys_updated_reset(sec_keys);

    tr_info("NVM KEYS read");

    return 0;
}

void ws_pae_nvm_store_frame_counter_tlv_create(frame_cnt_nvm_tlv_t *tlv_entry, uint32_t restart_cnt, uint16_t pan_version, frame_counters_t *counters, uint64_t stored_time)
{
    tlv_entry->tag = PAE_NVM_FRAME_COUNTER_TAG;
    tlv_entry->len = PAE_NVM_FRAME_COUNTER_LEN;

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    tlv = common_write_32_bit(restart_cnt, tlv);

    tlv = common_write_64_bit(stored_time, tlv);

    tlv = common_write_16_bit(pan_version, tlv);

    for (uint8_t index = 0; index < GTK_NUM; index++) {
        if (!counters->counter[index].set) {
            *tlv++ = PAE_NVM_FIELD_NOT_SET;
            memset(tlv, 0, GTK_LEN + 4 + 4);
            tlv += GTK_LEN + 4 + 4;
            continue;
        }
        *tlv++ = PAE_NVM_FIELD_SET;
        memcpy(tlv, counters->counter[index].gtk, GTK_LEN);
        tlv += GTK_LEN;
        tlv = common_write_32_bit(counters->counter[index].frame_counter, tlv);
        tlv = common_write_32_bit(counters->counter[index].max_frame_counter_chg, tlv);
    }

    tr_info("NVM FRAME COUNTER write; stored time: %"PRIu64, stored_time);
}

int8_t ws_pae_nvm_store_frame_counter_tlv_read(frame_cnt_nvm_tlv_t *tlv_entry, uint32_t *restart_cnt, uint64_t *stored_time, uint16_t *pan_version, frame_counters_t *counters)
{
    if (!tlv_entry || !counters) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_FRAME_COUNTER_TAG || tlv_entry->len != PAE_NVM_FRAME_COUNTER_LEN) {
        return -1;
    }

    uint8_t *tlv = (uint8_t *) &tlv_entry->data[0];

    *restart_cnt = common_read_32_bit(tlv);
    tlv += 4;

    *stored_time = common_read_64_bit(tlv);
    tlv += 8;

    *pan_version = common_read_16_bit(tlv);
    tlv += 2;

    for (uint8_t index = 0; index < GTK_NUM; index++) {
        // Frame counter not set
        if (*tlv++ == PAE_NVM_FIELD_NOT_SET) {
            counters->counter[index].set = false;
            tlv += GTK_LEN + 4 + 4;
            continue;
        }
        // Frame counter is set, read GTK key and counter values
        counters->counter[index].set = true;
        memcpy(counters->counter[index].gtk, tlv, GTK_LEN);
        tlv += GTK_LEN;
        counters->counter[index].frame_counter = common_read_32_bit(tlv);
        tlv += 4;
        counters->counter[index].max_frame_counter_chg = common_read_32_bit(tlv);
        tlv += 4;
    }

    tr_info("NVM FRAME COUNTER read; stored time: %"PRIu64, *stored_time);

    return 0;
}

void ws_pae_nvm_store_key_storage_index_tlv_create(nvm_tlv_t *tlv_entry, uint64_t bitfield)
{
    tlv_entry->tag = PAE_NVM_KEY_STORAGE_INDEX_TAG;
    tlv_entry->len = PAE_NVM_KEY_STORAGE_INDEX_LEN;

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    tlv = common_write_64_bit(bitfield, tlv);

    tr_info("NVM KEY STORAGE INDEX write");
}

int8_t ws_pae_nvm_store_key_storage_index_tlv_read(nvm_tlv_t *tlv_entry, uint64_t *bitfield)
{
    if (!tlv_entry || !bitfield) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_KEY_STORAGE_INDEX_TAG || tlv_entry->len != PAE_NVM_KEY_STORAGE_INDEX_LEN) {
        return -1;
    }

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;
    *bitfield = common_read_64_bit(tlv);
    tlv += 8;

    tr_info("NVM KEY STORAGE INDEX read");

    return 0;
}

void ws_pae_nvm_store_key_storage_tlv_create(nvm_tlv_t *tlv_entry, uint16_t length)
{
    memset(tlv_entry, 0, sizeof(key_storage_nvm_tlv_entry_t));

    tlv_entry->tag = PAE_NVM_KEY_STORAGE_TAG;
    tlv_entry->len = length - sizeof(nvm_tlv_t);

    tr_debug("NVM KEY STORAGE create");
}

int8_t ws_pae_nvm_store_key_storage_tlv_read(nvm_tlv_t *tlv_entry, uint16_t length)
{
    if (!tlv_entry || !length) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_KEY_STORAGE_TAG || tlv_entry->len != length - sizeof(nvm_tlv_t)) {
        return -1;
    }

    tr_debug("NVM KEY STORAGE read");

    return 0;
}

#endif /* HAVE_WS */

