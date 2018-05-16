/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
namespace generic {

/** Naive memory implementation for verification. */
class MemorySecurityDb : public SecurityDb {
private:
    struct entry_t {
        entry_t() { };
        SecurityDistributionFlags_t flags;
        SecurityEntryKeys_t local_keys;
        SecurityEntryKeys_t peer_keys;
        SecurityEntryIdentity_t peer_identity;
        SecurityEntrySigning_t peer_signing;
    };

    static const size_t MAX_ENTRIES = 5;

    static entry_t* as_entry(entry_handle_t db_handle)
    {
        return reinterpret_cast<entry_t*>(db_handle);
    }

public:
    MemorySecurityDb() : SecurityDb() { }
    virtual ~MemorySecurityDb() { }

    virtual SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_handle
    ) {
        return reinterpret_cast<SecurityDistributionFlags_t*>(db_handle);
    }

    /* local keys */

    /* set */
    virtual void set_entry_local_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->flags.ltk_sent = true;
            entry->local_keys.ltk = ltk;
        }
    }

    virtual void set_entry_local_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->local_keys.ediv = ediv;
            entry->local_keys.rand = rand;
        }
    }

    /* peer's keys */

    /* set */

    virtual void set_entry_peer_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_keys.ltk = ltk;
            entry->flags.ltk_stored = true;
        }
    }

    virtual void set_entry_peer_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_keys.ediv = ediv;
            entry->peer_keys.rand = rand;
        }
    }

    virtual void set_entry_peer_irk(
        entry_handle_t db_handle,
        const irk_t &irk
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_identity.irk = irk;
            entry->flags.irk_stored = true;
        }
    }

    virtual void set_entry_peer_bdaddr(
        entry_handle_t db_handle,
        bool address_is_public,
        const address_t &peer_address
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_identity.identity_address = peer_address;
            entry->peer_identity.identity_address_is_public = address_is_public;
        }
    }

    virtual void set_entry_peer_csrk(
        entry_handle_t db_handle,
        const csrk_t &csrk
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->flags.csrk_stored = true;
            entry->peer_signing.csrk = csrk;
        }
    }

    virtual void set_entry_peer_sign_counter(
        entry_handle_t db_handle,
        sign_count_t sign_counter
    ) {
        entry_t *entry = as_entry(db_handle);
        if (entry) {
            entry->peer_signing.counter = sign_counter;
        }
    }

private:
    virtual uint8_t get_entry_count() {
        return MAX_ENTRIES;
    }

    virtual SecurityDistributionFlags_t* get_entry_handle_by_index(uint8_t index) {
        if (index < MAX_ENTRIES) {
            return &_entries[index].flags;
        } else {
            return NULL;
        }
    }

    virtual void reset_entry(entry_handle_t db_entry) {
        entry_t *entry = reinterpret_cast<entry_t*>(db_entry);
        *entry = entry_t();
    }

    virtual SecurityEntryIdentity_t* read_in_entry_peer_identity(entry_handle_t db_entry) {
        entry_t *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_identity;
    };

    virtual SecurityEntryKeys_t* read_in_entry_peer_keys(entry_handle_t db_entry) {
        entry_t *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_keys;
    };

    virtual SecurityEntryKeys_t* read_in_entry_local_keys(entry_handle_t db_entry) {
        entry_t *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->local_keys;
    };

    virtual SecurityEntrySigning_t* read_in_entry_peer_signing(entry_handle_t db_entry) {
        entry_t *entry = reinterpret_cast<entry_t*>(db_entry);
        return &entry->peer_signing;
    };

private:
    entry_t _entries[MAX_ENTRIES];
};

} /* namespace pal */
} /* namespace ble */

#endif /*GENERIC_MEMORY_SECURITY_DB_H_*/
