/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
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

#include "MemorySecurityDb.h"

#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLDB"

#define _STR(x) #x
#define STR(x) _STR(x)


namespace ble {

MemorySecurityDb::entry_t *MemorySecurityDb::as_entry(entry_handle_t db_handle)
{
    entry_t* entry = reinterpret_cast<entry_t*>(db_handle);
    if (!entry) {
        tr_error("Invalid security DB handle used");
    }
    return entry;
}


MemorySecurityDb::MemorySecurityDb() : SecurityDb()
{
    tr_info("Using memory security DB (capacity " STR(BLE_SECURITY_DATABASE_MAX_ENTRIES) " entries) - no persistence across reset");
}


SecurityDistributionFlags_t *MemorySecurityDb::get_distribution_flags(
    entry_handle_t db_handle
) {
    return reinterpret_cast<SecurityDistributionFlags_t*>(db_handle);
}

/* local keys */

/* set */
void MemorySecurityDb::set_entry_local_ltk(
    entry_handle_t db_handle,
    const ltk_t &ltk
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: local ltk %s", get_index(db_handle), to_string(ltk));
        entry->flags.ltk_sent = true;
        entry->local_keys.ltk = ltk;
    }
}

void MemorySecurityDb::set_entry_local_ediv_rand(
    entry_handle_t db_handle,
    const ediv_t &ediv,
    const rand_t &rand
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: local ediv %s rand %s", get_index(db_handle), to_string(ediv), to_string(rand));
        entry->local_keys.ediv = ediv;
        entry->local_keys.rand = rand;
    }
}

void MemorySecurityDb::set_entry_peer_ltk(
    entry_handle_t db_handle,
    const ltk_t &ltk
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: peer ltk %s", get_index(db_handle), to_string(ltk));
        entry->peer_keys.ltk = ltk;
        entry->flags.ltk_stored = true;
    }
}

void MemorySecurityDb::set_entry_peer_ediv_rand(
    entry_handle_t db_handle,
    const ediv_t &ediv,
    const rand_t &rand
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: peer ediv %s rand %s", get_index(db_handle), to_string(ediv), to_string(rand));
        entry->peer_keys.ediv = ediv;
        entry->peer_keys.rand = rand;
    }
}

void MemorySecurityDb::set_entry_peer_irk(
    entry_handle_t db_handle,
    const irk_t &irk
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: peer irk %s", get_index(db_handle), to_string(irk));
        entry->peer_identity.irk = irk;
        entry->flags.irk_stored = true;
    }
}

void MemorySecurityDb::set_entry_peer_bdaddr(
    entry_handle_t db_handle,
    bool address_is_public,
    const address_t &peer_address
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: %s peer address %s", get_index(db_handle), address_is_public? "public" : "private", to_string(peer_address));
        entry->peer_identity.identity_address = peer_address;
        entry->peer_identity.identity_address_is_public = address_is_public;
    }
}

void MemorySecurityDb::set_entry_peer_csrk(
    entry_handle_t db_handle,
    const csrk_t &csrk
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: peer csrk %s", get_index(db_handle), to_string(csrk));
        entry->flags.csrk_stored = true;
        entry->peer_signing.csrk = csrk;
    }
}

void MemorySecurityDb::set_entry_peer_sign_counter(
    entry_handle_t db_handle,
    sign_count_t sign_counter
) {
    entry_t *entry = as_entry(db_handle);
    if (entry) {
        tr_info("Write DB entry %d: sign counter %lu", get_index(db_handle), sign_counter);
        entry->peer_signing.counter = sign_counter;
    }
}

uint8_t MemorySecurityDb::get_entry_count() {
    return BLE_SECURITY_DATABASE_MAX_ENTRIES;
}

SecurityDistributionFlags_t *MemorySecurityDb::get_entry_handle_by_index(uint8_t index) {
    if (index < BLE_SECURITY_DATABASE_MAX_ENTRIES) {
        return &_entries[index].flags;
    } else {
        return nullptr;
    }
}

void MemorySecurityDb::reset_entry(entry_handle_t db_entry) {
    auto *entry = reinterpret_cast<entry_t*>(db_entry);
    *entry = entry_t();
}

SecurityEntryIdentity_t *MemorySecurityDb::read_in_entry_peer_identity(entry_handle_t db_entry) {
    auto *entry = reinterpret_cast<entry_t*>(db_entry);
    return &entry->peer_identity;
}

SecurityEntryKeys_t *MemorySecurityDb::read_in_entry_peer_keys(entry_handle_t db_entry) {
    auto *entry = reinterpret_cast<entry_t*>(db_entry);
    return &entry->peer_keys;
}

SecurityEntryKeys_t *MemorySecurityDb::read_in_entry_local_keys(entry_handle_t db_entry) {
    auto *entry = reinterpret_cast<entry_t*>(db_entry);
    return &entry->local_keys;
}

SecurityEntrySigning_t *MemorySecurityDb::read_in_entry_peer_signing(entry_handle_t db_entry) {
    auto *entry = reinterpret_cast<entry_t*>(db_entry);
    return &entry->peer_signing;
}

uint8_t MemorySecurityDb::get_index(const entry_handle_t db_handle) const
{
    return reinterpret_cast<entry_t*>(db_handle) - _entries;
}

} /* namespace ble */