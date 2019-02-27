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
#include "Security/protocols/sec_prot_certs.h"
#include "Security/protocols/sec_prot_keys.h"

#ifdef HAVE_WS

#define TRACE_GROUP "spke"

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
    sec_keys->pmk_key_replay_cnt = 0;
    sec_keys->gtks = gtks;
    sec_keys->certs = certs;
    sec_keys->pmk_set = false;
    sec_keys->ptk_set = false;
    sec_keys->updated = false;
    sec_keys->ptk_eui_64_set = false;
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
    gtks->gtk_set_index = -1;
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
    sec_keys->pmk_set = true;
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
    sec_keys->pmk_key_replay_cnt = counter;
}

void sec_prot_keys_pmk_replay_cnt_increment(sec_prot_keys_t *sec_keys)
{
    sec_keys->pmk_key_replay_cnt++;
}

void sec_prot_keys_ptk_write(sec_prot_keys_t *sec_keys, uint8_t *ptk)
{
    memcpy(sec_keys->ptk, ptk, PTK_LEN);
    sec_keys->ptk_set = true;
    sec_keys->updated = true;
}

uint8_t *sec_prot_keys_ptk_get(sec_prot_keys_t *sec_keys)
{
    if (!sec_keys->ptk_set) {
        return NULL;
    }

    return sec_keys->ptk;
}

void sec_prot_keys_ptk_eui_64_set(sec_prot_keys_t *sec_keys, uint8_t *eui_64)
{
    sec_keys->ptk_eui_64 = eui_64;
}

void sec_prot_keys_ptk_eui_64_write(sec_prot_keys_t *sec_keys, uint8_t *eui_64)
{
    if (sec_keys->ptk_eui_64) {
        memcpy(sec_keys->ptk_eui_64, eui_64, 8);
        sec_keys->ptk_eui_64_set = true;
        sec_keys->updated = true;
    }
}

uint8_t *sec_prot_keys_ptk_eui_64_get(sec_prot_keys_t *sec_keys)
{
    if (!sec_keys->ptk_eui_64 || !sec_keys->ptk_eui_64_set) {
        return NULL;
    }

    return sec_keys->ptk_eui_64;
}

bool sec_prot_keys_are_updated(sec_prot_keys_t *sec_keys)
{
    return sec_keys->updated;
}

void sec_prot_keys_updated_reset(sec_prot_keys_t *sec_keys)
{
    sec_keys->updated = false;
}

uint8_t sec_prot_keys_gtkl_get(sec_prot_gtk_keys_t *gtks)
{
    uint8_t gtkl = (uint8_t) gtks->gtk[0].live |
                   (((uint8_t) gtks->gtk[1].live) << 1) |
                   (((uint8_t) gtks->gtk[2].live) << 2) |
                   (((uint8_t) gtks->gtk[3].live) << 3);

    return gtkl;
}

void sec_prot_keys_gtkl_set(sec_prot_gtk_keys_t *gtks, uint8_t gtkl)
{
    for (uint8_t i = 0; i < GTK_NUM; i++) {
        if (gtks->gtk[i].set) {
            if ((gtkl >> i) & 0x01) {
                gtks->gtk[i].live = true; // Live on authenticator
            } else {
                gtks->gtk[i].live = false;
            }
        }
    }
}

bool sec_prot_keys_gtk_is_live(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].live) {
        return false;
    }

    return true;
}

int8_t sec_prot_keys_gtk_insert_index_set(sec_prot_gtk_keys_t *gtks, uint8_t index)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return -1;
    }

    gtks->gtk_set_index = index;
    return 0;
}

int8_t sec_prot_keys_gtk_insert_index_get(sec_prot_gtk_keys_t *gtks)
{
    return gtks->gtk_set_index;
}

void sec_prot_keys_gtk_insert_index_clear(sec_prot_gtk_keys_t *gtks)
{
    gtks->gtk_set_index = -1;
}

uint8_t *sec_prot_keys_get_gtk_to_insert(sec_prot_gtk_keys_t *gtks, uint8_t *index)
{
    if (gtks->gtk_set_index >= 0 && gtks->gtk[gtks->gtk_set_index].set && gtks->gtk[gtks->gtk_set_index].live) {
        *index = gtks->gtk_set_index;
        return gtks->gtk[gtks->gtk_set_index].key;
    } else {
        return NULL;
    }
}

int8_t sec_prot_keys_gtk_set(sec_prot_gtk_keys_t *gtks, uint8_t index, uint8_t *gtk)
{
    if (!gtk || index >= GTK_NUM) {
        return -1;
    }

    if (gtks->gtk[index].set && memcmp(gtks->gtk[index].key, gtk, GTK_LEN) != 0) {
        return -1;
    }

    gtks->gtk[index].set = true;
    gtks->gtk[index].live = false;   // Set from GTKL
    gtks->gtk[index].hash = false;   // Not verified yet
    gtks->gtk[index].lifetime = 0;   // Should be provided by authenticator
    memcpy(gtks->gtk[index].key, gtk, GTK_LEN);

    gtks->updated = true;

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

void sec_prot_keys_gtk_lifetime_set(sec_prot_gtk_keys_t *gtks, uint8_t index, uint32_t lifetime)
{
    if (index >= GTK_NUM || !gtks->gtk[index].set) {
        return;

    }
    gtks->gtk[index].lifetime = lifetime;

    gtks->updated = true;
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

#endif /* HAVE_WS */
