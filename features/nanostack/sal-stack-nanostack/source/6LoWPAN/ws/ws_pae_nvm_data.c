/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"
#include "6LoWPAN/ws/ws_pae_nvm_store.h"
#include "6LoWPAN/ws/ws_pae_nvm_data.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsnv"

#define PAE_NVM_NW_INFO_TAG              1
#define PAE_NVM_KEYS_TAG                 2
#define PAE_NVM_FRAME_COUNTER_TAG        3

// pan_id (2) + network name (33) + (GTK set (1) + GTK lifetime (4) + GTK (16)) * 4
#define PAE_NVM_NW_INFO_LEN              2 + 33 + (1 + 4 + GTK_LEN) * GTK_NUM

// PTK EUI-64 set (1) + PTK EUI-64 (8) + PMK set (1) + PMK (32) + PMK replay counter (8) + PTK set (1) + PTK (48)
#define PAE_NVM_KEYS_LEN                 1 + 8 + 1 + PMK_LEN + 8 + 1 + PTK_LEN

// GTK hash (8), frame counter (4), index (1)
#define PAE_NVM_FRAME_COUNTER_LEN        8 + 4 + 1

nvm_tlv_entry_t *ws_pae_buffer_allocate(void)
{
    //Allocate worts case buffer
    return ns_dyn_mem_temporary_alloc(sizeof(nvm_tlv_entry_t) + PAE_NVM_NW_INFO_LEN);
}

void ws_pae_nvm_store_nw_info_tlv_create(nvm_tlv_entry_t *tlv_entry, uint16_t pan_id, char *nw_name, sec_prot_gtk_keys_t *gtks)
{

    tlv_entry->tag = PAE_NVM_NW_INFO_TAG;
    tlv_entry->len = PAE_NVM_NW_INFO_LEN;

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    tlv = common_write_16_bit(pan_id, tlv);

    memset(tlv, 0, 33);
    strncpy((char *)tlv, nw_name, 32);
    tlv += 33;

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (sec_prot_keys_gtk_is_set(gtks, i)) {
            *tlv++ = 1; // GTK is set
            uint32_t lifetime = sec_prot_keys_gtk_lifetime_get(gtks, i);
            tlv = common_write_32_bit(lifetime, tlv);

            uint8_t *gtk = sec_prot_keys_gtk_get(gtks, i);
            memcpy(tlv, gtk, GTK_LEN);
            tlv += GTK_LEN;
        } else {
            *tlv++ = 0; // GTK is not set
            memset(tlv, 0, 4 + GTK_LEN);
            tlv += 4 + GTK_LEN;
        }
    }

    tr_debug("NVM NW_INFO write PAN ID %i name: %s", pan_id, nw_name);

}

int8_t ws_pae_nvm_store_nw_info_tlv_read(nvm_tlv_entry_t *tlv_entry, uint16_t *pan_id, char *nw_name, sec_prot_gtk_keys_t *gtks)
{
    if (!tlv_entry || !pan_id || !nw_name || !gtks) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_NW_INFO_TAG || tlv_entry->len != PAE_NVM_NW_INFO_LEN) {
        return -1;
    }

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    *pan_id = common_read_16_bit(tlv);
    tlv += 2;

    memset(nw_name, 0, 33);
    strncpy(nw_name, (char *) tlv, 32);
    tlv += 33;

    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (*tlv++ == 1) { /* GTK is set */
            uint32_t lifetime = common_read_32_bit(tlv);
            tlv += 4;
            sec_prot_keys_gtk_set(gtks, i, tlv, lifetime);
            tlv += GTK_LEN;
        } else {
            tlv += 4 + GTK_LEN;
        }
    }
    sec_prot_keys_gtks_updated_reset(gtks);

    tr_debug("NVM NW_INFO read PAN ID %i name: %s", *pan_id, nw_name);

    return 0;
}

void ws_pae_nvm_store_keys_tlv_create(nvm_tlv_entry_t *tlv_entry, sec_prot_keys_t *sec_keys)
{
    tlv_entry->tag = PAE_NVM_KEYS_TAG;
    tlv_entry->len = PAE_NVM_KEYS_LEN;

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    uint8_t *eui_64 = sec_prot_keys_ptk_eui_64_get(sec_keys);
    if (eui_64) {
        *tlv++ = 1;
        memcpy(tlv, eui_64, 8);
    } else {
        *tlv++ = 0;
        memset(tlv, 0, 8);
    }
    tlv += 8;

    uint8_t *pmk = sec_prot_keys_pmk_get(sec_keys);
    if (pmk) {
        *tlv++ = 1;
        memcpy(tlv, pmk, PMK_LEN);
    } else {
        *tlv++ = 0;
        memset(tlv, 0, PMK_LEN);
    }
    tlv += PMK_LEN;

    uint64_t counter = sec_prot_keys_pmk_replay_cnt_get(sec_keys);
    tlv = common_write_64_bit(counter, tlv);

    uint8_t *ptk = sec_prot_keys_ptk_get(sec_keys);
    if (ptk) {
        *tlv++ = 1;
        memcpy(tlv, ptk, PTK_LEN);
    } else {
        *tlv++ = 0;
        memset(tlv, 0, PTK_LEN);
    }
    tlv += PTK_LEN;

    tr_debug("NVM KEYS write");

}

int8_t ws_pae_nvm_store_keys_tlv_read(nvm_tlv_entry_t *tlv_entry, sec_prot_keys_t *sec_keys)
{
    if (!tlv_entry || !sec_keys) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_KEYS_TAG || tlv_entry->len != PAE_NVM_KEYS_LEN) {
        return -1;
    }

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    // EUI-64 set */
    if (*tlv++ == 1) {
        sec_prot_keys_ptk_eui_64_write(sec_keys, tlv);
    }
    tlv += 8;

    // PMK set
    if (*tlv++ == 1) {
        sec_prot_keys_pmk_write(sec_keys, tlv);
    }
    tlv += PMK_LEN;

    uint64_t counter = common_read_64_bit(tlv);
    tlv += 8;
    sec_prot_keys_pmk_replay_cnt_set(sec_keys, counter);

    // PTK set
    if (*tlv++ == 1) {
        sec_prot_keys_ptk_write(sec_keys, tlv);
    }

    tlv += PTK_LEN;

    sec_prot_keys_updated_reset(sec_keys);

    tr_debug("NVM KEYS read");

    return 0;
}

void ws_pae_nvm_store_frame_counter_tlv_create(nvm_tlv_entry_t *tlv_entry, uint8_t index, uint8_t *hash, uint32_t frame_counter)
{

    tlv_entry->tag = PAE_NVM_FRAME_COUNTER_TAG;
    tlv_entry->len = PAE_NVM_FRAME_COUNTER_LEN;

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    memcpy(tlv, hash, GTK_HASH_LEN);
    tlv += GTK_HASH_LEN;
    tlv = common_write_32_bit(frame_counter, tlv);
    *tlv = index;

    tr_debug("NVM FRAME COUNTER write");
}

int8_t ws_pae_nvm_store_frame_counter_tlv_read(nvm_tlv_entry_t *tlv_entry, uint8_t *index, uint8_t *hash, uint32_t *frame_counter)
{
    if (!tlv_entry || !frame_counter) {
        return -1;
    }

    if (tlv_entry->tag != PAE_NVM_FRAME_COUNTER_TAG || tlv_entry->len != PAE_NVM_FRAME_COUNTER_LEN) {
        return -1;
    }

    uint8_t *tlv = ((uint8_t *) &tlv_entry->tag) + NVM_TLV_FIXED_LEN;

    memcpy(hash, tlv, GTK_HASH_LEN);
    tlv += GTK_HASH_LEN;
    *frame_counter = common_read_32_bit(tlv);
    tlv += 4;
    *index = *tlv;

    tr_debug("NVM FRAME COUNTER read");

    return 0;
}

#endif /* HAVE_WS */

