/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "eventOS_scheduler.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Service_Libs/blacklist/blacklist.h"

#define TRACE_GROUP "bl"

static blacklist_entry_t *blacklist_entry_find(const uint8_t *eui64);
static void blacklist_entry_add(const uint8_t *eui64);
static int8_t blacklist_entry_free(blacklist_entry_t *blacklist_entry);
static uint16_t blacklist_entries_count(void);

static blacklist_data_t *blacklist_data = NULL;

uint8_t blacklist_init(void)
{
    if (blacklist_data) {
        return 0;
    }

    blacklist_data = ns_dyn_mem_alloc(sizeof(blacklist_data_t));

    if (!blacklist_data) {
        return 1;
    }

    ns_list_init(&blacklist_data->blacklist);

    blacklist_data->blacklist_purge_ttl = BLACKLIST_DEFAULT_PURGE_TIMER_TIMEOUT;
    blacklist_data->blacklist_entry_lifetime = BLACKLIST_DEFAULT_ENTRY_LIFETIME;
    blacklist_data->blacklist_timer_max_timeout = BLACKLIST_DEFAULT_TIMER_MAX_TIMEOUT;
    blacklist_data->blacklist_timer_timeout = BLACKLIST_DEFAULT_TIMER_TIMEOUT;
    blacklist_data->blacklist_entry_max_nbr = BLACKLIST_DEFAULT_ENTRY_MAX_NBR;
    blacklist_data->blacklist_purge_nbr = BLACKLIST_DEFAULT_PURGE_NBR;
    blacklist_data->blacklist_purge_timer_timeout = BLACKLIST_DEFAULT_PURGE_TIMER_TIMEOUT;

    return 0;
}

void blacklist_params_set(uint16_t entry_lifetime, uint16_t timer_max_timeout, uint16_t timer_timeout, uint16_t entry_max_nbr, uint16_t purge_nbr, uint16_t purge_timer_timeout)
{
    if (!blacklist_data) {
        return;
    }

    blacklist_data->blacklist_purge_ttl = purge_timer_timeout;
    blacklist_data->blacklist_entry_lifetime = entry_lifetime;
    blacklist_data->blacklist_timer_max_timeout = timer_max_timeout;
    blacklist_data->blacklist_timer_timeout = timer_timeout;
    blacklist_data->blacklist_entry_max_nbr = entry_max_nbr;
    blacklist_data->blacklist_purge_nbr = purge_nbr;
    blacklist_data->blacklist_purge_timer_timeout = purge_timer_timeout;
}

bool blacklist_reject(const uint8_t *ll64_address)
{
    if (!blacklist_data) {
        return false;
    }

    blacklist_entry_t *blacklist_entry;

    blacklist_entry = blacklist_entry_find(ll64_address + 8);

    // If blacklist entry exists
    if (blacklist_entry) {
        // If address is blacklisted rejects
        if (blacklist_entry->ttl > blacklist_data->blacklist_entry_lifetime) {
            tr_debug("blacklist reject: %s", trace_array(ll64_address + 8, 8));
            return true;
        // Neighbor heard; updates blacklist entry TTL to full lifetime
        } else {
            blacklist_entry->ttl = blacklist_data->blacklist_entry_lifetime;
            return false;
        }
    } else {
        // If blacklist is full rejects
        if (blacklist_entries_count() >= blacklist_data->blacklist_entry_max_nbr) {
            tr_debug("blacklist full reject");
            return true;
        } else {
            tr_debug("blacklist not found %s", trace_array(ll64_address + 8, 8));
            return false;
        }
    }
}

void blacklist_update(const uint8_t *ll64_address, bool success)
{
    if (!blacklist_data) {
        return;
    }

    blacklist_entry_t *blacklist_entry;

    if (!ll64_address) {
        return;
    }

    // Check that address is LL64 (not LL16)
    if (memcmp(ll64_address, ADDR_LINK_LOCAL_PREFIX, 8) != 0) {
        return;
    }

    if (memcmp((ll64_address + 8), ADDR_SHORT_ADR_SUFFIC, 6) == 0) {
        return;
    }

    blacklist_entry = blacklist_entry_find(ll64_address + 8);

    // On successful link establishment remove address from blacklist
    if (success) {
        if (blacklist_entry) {
            tr_debug("Blacklist removed");
            blacklist_entry_free(blacklist_entry);
        }
    // On failure add address to blacklist or update timeout
    } else {
        if (blacklist_entry) {
            blacklist_entry->interval = blacklist_entry->interval * 2;
            if (blacklist_entry->interval > blacklist_data->blacklist_timer_max_timeout) {
                blacklist_entry->interval = blacklist_data->blacklist_timer_max_timeout;
            }
            /* TTL is blacklist entry lifetime + from 1.0 to 1.5 * interval */
            blacklist_entry->ttl = blacklist_data->blacklist_entry_lifetime + randLIB_randomise_base(blacklist_entry->interval, 0x8000, 0xC000);
            tr_debug("Blacklist updated, ttl=%"PRIu16, blacklist_entry->ttl);
        } else {
            tr_debug("Blacklist add");
            blacklist_entry_add(ll64_address + 8);
        }
    }
}

void blacklist_clear(void)
{
    if (!blacklist_data) {
        return;
    }

    ns_list_foreach_safe(blacklist_entry_t, blacklist_entry, &blacklist_data->blacklist) {
        blacklist_entry_free(blacklist_entry);
    }
}

void blacklist_free(void)
{
    if (!blacklist_data) {
        return;
    }

    ns_dyn_mem_free(blacklist_data);
    blacklist_data = NULL;
}

void blacklist_ttl_update(uint16_t ticks)
{
    if (!blacklist_data) {
        return;
    }

    if (blacklist_data->blacklist_purge_ttl > ticks) {
        blacklist_data->blacklist_purge_ttl -= ticks;
    } else {
        /* 0.5 to 1.5 times timeout */
        blacklist_data->blacklist_purge_ttl = randLIB_randomise_base(blacklist_data->blacklist_purge_timer_timeout, 0x4000, 0xC000);

        if (blacklist_entries_count() >= blacklist_data->blacklist_entry_max_nbr - blacklist_data->blacklist_purge_nbr) {
            uint8_t count = 0;
            blacklist_entry_t *blacklist_entry_min_ttl;

            tr_debug("Blacklist entries purge");

            while (count++ < blacklist_data->blacklist_purge_nbr) {
                blacklist_entry_min_ttl = NULL;
                ns_list_foreach(blacklist_entry_t, blacklist_entry, &blacklist_data->blacklist) {
                    if (!blacklist_entry_min_ttl) {
                        blacklist_entry_min_ttl = blacklist_entry;
                    } else if (blacklist_entry->ttl < blacklist_entry_min_ttl->ttl) {
                        blacklist_entry_min_ttl = blacklist_entry;
                    }
                }
                if (blacklist_entry_min_ttl) {
                    blacklist_entry_free(blacklist_entry_min_ttl);
                }
            }
        }
    }

    ns_list_foreach_safe(blacklist_entry_t, blacklist_entry, &blacklist_data->blacklist) {
        if (blacklist_entry->ttl > ticks) {
            blacklist_entry->ttl -= ticks;
        } else {
            tr_debug("Blacklist remove entry: %s", trace_array(blacklist_entry->eui64, 8));
            blacklist_entry_free(blacklist_entry);
        }
    }
}

static blacklist_entry_t *blacklist_entry_find(const uint8_t *eui64)
{
    if (!eui64) {
        return NULL;
    }

    if (!blacklist_data) {
        return NULL;
    }

    ns_list_foreach(blacklist_entry_t, blacklist_entry, &blacklist_data->blacklist) {
        if (memcmp(blacklist_entry->eui64, eui64, 8) == 0) {
            return blacklist_entry;
        }
    }
    return NULL;
}

static void blacklist_entry_add(const uint8_t *eui64)
{
    blacklist_entry_t *blacklist_entry;

    if (!eui64) {
        return;
    }

    if (!blacklist_data) {
        return;
    }

    if (blacklist_entries_count() >= blacklist_data->blacklist_entry_max_nbr) {
        return;
    }

    blacklist_entry = ns_dyn_mem_alloc(sizeof(blacklist_entry_t));

    if (!blacklist_entry) {
       return;
    }

    blacklist_entry->interval = blacklist_data->blacklist_timer_timeout;

    /* TTL is blacklist entry lifetime + from 1.0 to 1.5 * interval */
    blacklist_entry->ttl = blacklist_data->blacklist_entry_lifetime + randLIB_randomise_base(blacklist_entry->interval, 0x8000, 0xC000);

    memcpy(blacklist_entry->eui64, eui64, 8);
    tr_debug("Blacklist add, ttl=%"PRIu16, blacklist_entry->ttl);
    ns_list_add_to_start(&blacklist_data->blacklist, blacklist_entry);
}

static int8_t blacklist_entry_free(blacklist_entry_t *blacklist_entry)
{
    if (!blacklist_entry) {
        return -1;
    }

    if (!blacklist_data) {
        return -2;
    }

    ns_list_remove(&blacklist_data->blacklist, blacklist_entry); //Remove from the list
    ns_dyn_mem_free(blacklist_entry); // Free entry
    return 0;
}

static uint16_t blacklist_entries_count(void)
{
    uint16_t entry_count = 0;

    if (!blacklist_data) {
        return 0;
    }

    entry_count = ns_list_count(&blacklist_data->blacklist);

    return entry_count;
}
