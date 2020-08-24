/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef GENERIC_MEMORY_SECURITY_DB_H_
#define GENERIC_MEMORY_SECURITY_DB_H_

#include "SecurityDb.h"

namespace ble {

/** Naive memory implementation for verification. */
class MemorySecurityDb : public SecurityDb {
private:
    struct entry_t {
        entry_t() = default;
        SecurityDistributionFlags_t flags;
        SecurityEntryKeys_t local_keys;
        SecurityEntryKeys_t peer_keys;
        SecurityEntryIdentity_t peer_identity;
        SecurityEntrySigning_t peer_signing;
    };

    static entry_t* as_entry(entry_handle_t db_handle)
    {
        return reinterpret_cast<entry_t*>(db_handle);
    }

public:
    MemorySecurityDb() : SecurityDb() { }
    ~MemorySecurityDb() override = default;

    SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_handle
    ) override {
        return reinterpret_cast<SecurityDistributionFlags_t*>(db_handle);
    }

    /* local keys */

    /* set */
    void set_entry_local_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->flags.ltk_sent = true;
            entry->local_keys.ltk = ltk;
        }
    }

    void set_entry_local_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->local_keys.ediv = ediv;
            entry->local_keys.rand = rand;
        }
    }

    /* peer's keys */

    /* set */

    void set_entry_peer_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_keys.ltk = ltk;
            entry->flags.ltk_stored = true;
        }
    }

    void set_entry_peer_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_keys.ediv = ediv;
            entry->peer_keys.rand = rand;
        }
    }

    void set_entry_peer_irk(
        entry_handle_t db_handle,
        const irk_t &irk
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_identity.irk = irk;
            entry->flags.irk_stored = true;
        }
    }

    void set_entry_peer_bdaddr(
        entry_handle_t db_handle,
        bool address_is_public,
        const address_t &peer_address
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_identity.identity_address = peer_address;
            entry->peer_identity.identity_address_is_public = address_is_public;
        }
    }

    void set_entry_peer_csrk(
        entry_handle_t db_handle,
        const csrk_t &csrk
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->flags.csrk_stored = true;
            entry->peer_signing.csrk = csrk;
        }
    }

    void set_entry_peer_sign_counter(
        entry_handle_t db_handle,
        sign_count_t sign_counter
    ) override {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_signing.counter = sign_counter;
        }
    }

private:
    uint8_t get_entry_count() override {
        return BLE_SECURITY_DATABASE_MAX_ENTRIES;
    }

    SecurityDistributionFlags_t* get_entry_handle_by_index(uint8_t index) override {
        if (index < BLE_SECURITY_DATABASE_MAX_ENTRIES) {
            return &_entries[index].flags;
        } else {
            return nullptr;
        }
    }

    void reset_entry(entry_handle_t db_entry) override{
        auto *entry = reinterpret_cast<entry_t*>(db_entry);
        *entry = entry_t();
    }

    SecurityEntryIdentity_t* read_in_entry_peer_identity(entry_handle_t db_entry) override {
        auto *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_identity;
    };

    SecurityEntryKeys_t* read_in_entry_peer_keys(entry_handle_t db_entry) override {
        auto *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_keys;
    };

    SecurityEntryKeys_t* read_in_entry_local_keys(entry_handle_t db_entry) override {
        auto *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->local_keys;
    };

    SecurityEntrySigning_t* read_in_entry_peer_signing(entry_handle_t db_entry) override {
        auto *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_signing;
    };

private:
    entry_t _entries[BLE_SECURITY_DATABASE_MAX_ENTRIES];
};

} /* namespace ble */

#endif /*GENERIC_MEMORY_SECURITY_DB_H_*/
