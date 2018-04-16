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

#ifndef PAL_MEMORY_SECURITY_DB_H_
#define PAL_MEMORY_SECURITY_DB_H_

#include "SecurityDb.h"

namespace ble {
namespace pal {

/** Naive memory implementation for verification. */
class MemorySecurityDb : public SecurityDb {
private:
    enum state_t {
        ENTRY_FREE,
        ENTRY_RESERVED,
        ENTRY_WRITTEN
    };

    struct entry_t {
        entry_t() : state(ENTRY_FREE) { };
        SecurityDistributionFlags_t flags;
        SecurityEntryKeys_t peer_keys;
        SecurityEntryKeys_t local_keys;
        SecurityEntryIdentity_t peer_identity;
        csrk_t csrk;
        state_t state;
    };
    static const size_t MAX_ENTRIES = 5;

    static entry_t* as_entry(entry_handle_t entry_handle)
    {
        return reinterpret_cast<entry_t*>(entry_handle);
    }

public:
    MemorySecurityDb() { };
    virtual ~MemorySecurityDb() { };

    virtual const SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t entry_handle
    ) {
        entry_t* entry = as_entry(entry_handle);
        if (!entry) {
            return NULL;
        }

        return &entry->flags;
    }

    /**
     * Set the distribution flags of the DB entry
     */
    virtual void set_distribution_flags(
        entry_handle_t entry_handle,
        const SecurityDistributionFlags_t& flags
    ) {
        entry_t* entry = as_entry(entry_handle);
        if (!entry) {
            return;
        }

        entry->state = ENTRY_WRITTEN;
        entry->flags = flags;
    }

    /* local keys */

    /* get */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t entry_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        entry_t* entry = as_entry(entry_handle);
        if (!entry) {
            return;
        }

        /* validate we have the correct key */
        if (ediv == entry->local_keys.ediv && rand == entry->local_keys.rand) {
            cb(entry_handle, &entry->local_keys);
        } else {
            cb(entry_handle, NULL);
        }
    }

    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t entry_handle
    ) {
        entry_t* entry = as_entry(entry_handle);
        if (!entry) {
            return;
        }

        /* validate we have the correct key */
        if (entry->flags.secure_connections_paired) {
            cb(entry_handle, &entry->local_keys);
        } else {
            cb(entry_handle, NULL);
        }
    }


    /* set */
    virtual void set_entry_local_ltk(
        entry_handle_t entry_handle,
        const ltk_t &ltk
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->local_keys.ltk = ltk;
        }
    }

    virtual void set_entry_local_ediv_rand(
        entry_handle_t entry_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->local_keys.ediv = ediv;
            entry->local_keys.rand = rand;
        }
    }

    /* peer's keys */

    /* get */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        entry_handle_t entry_handle
    ) {
        csrk_t csrk;
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            csrk = entry->csrk;
        }
        cb(entry_handle, &csrk);
    }

    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t entry_handle
    ) {
        SecurityEntryKeys_t *key = NULL;
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            key = &entry->peer_keys;
        }
        cb(entry_handle, key);
    }

    /* set */

    virtual void set_entry_peer_ltk(
        entry_handle_t entry_handle,
        const ltk_t &ltk
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->peer_keys.ltk = ltk;
        }
    }

    virtual void set_entry_peer_ediv_rand(
        entry_handle_t entry_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->peer_keys.ediv = ediv;
            entry->peer_keys.rand = rand;
        }
    }

    virtual void set_entry_peer_irk(
        entry_handle_t entry_handle,
        const irk_t &irk
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->peer_identity.irk = irk;
        }
    }

    virtual void set_entry_peer_bdaddr(
        entry_handle_t entry_handle,
        bool address_is_public,
        const address_t &peer_address
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->peer_identity.identity_address = peer_address;
        }
    }

    virtual void set_entry_peer_csrk(
        entry_handle_t entry_handle,
        const csrk_t &csrk
    ) {
        entry_t *entry = as_entry(entry_handle);
        if (entry) {
            entry->state = ENTRY_WRITTEN;
            entry->csrk = csrk;
        }
    }

    /* local csrk */

    virtual const csrk_t* get_local_csrk() {
        return &_local_csrk;
    }

    virtual void set_local_csrk(const csrk_t &csrk) {
        _local_csrk = csrk;
    }

    /* public key */

    virtual const public_key_coord_t& get_public_key_x() {
        return _public_key_x;
    }

    virtual const public_key_coord_t& get_public_key_y() {
        return _public_key_y;
    }

    virtual void set_public_key(
        const public_key_coord_t &public_key_x,
        const public_key_coord_t &public_key_y
    ) {
        _public_key_x = public_key_x;
        _public_key_y = public_key_y;
    }

    /* list management */

    virtual entry_handle_t open_entry(
        BLEProtocol::AddressType_t peer_address_type,
        const address_t &peer_address
    ) {
        const bool peer_address_public =
            (peer_address_type == BLEProtocol::AddressType::PUBLIC);

        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_entries[i].state == ENTRY_FREE) {
                continue;
            } else if (peer_address == _entries[i].peer_identity.identity_address
                && _entries[i].flags.peer_address_is_public == peer_address_public) {
                return &_entries[i];
            }
        }

        /* if we din't find one grab the first disconnected slot*/
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_entries[i].state == ENTRY_FREE) {
                _entries[i] = entry_t();
                _entries[i].flags.peer_address = peer_address;
                _entries[i].flags.peer_address_is_public = peer_address_public;
                _entries[i].state = ENTRY_RESERVED;
                return &_entries[i];
            }
        }

        return NULL;
    }

    virtual void close_entry(entry_handle_t entry_handle)
    {
        entry_t *entry = as_entry(entry_handle);
        if (entry && entry->state == ENTRY_RESERVED) {
            entry->state = ENTRY_FREE;
        }
    }

    virtual void remove_entry(const address_t peer_identity_address)
    {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_entries[i].state == ENTRY_FREE) {
                continue;
            } else if (peer_identity_address == _entries[i].peer_identity.identity_address) {
                _entries[i] = entry_t();
                _entries[i].state = ENTRY_FREE;
                return;
            }
        }
    }

    virtual void clear_entries() {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            _entries[i] = entry_t();
        }
        _local_identity = SecurityEntryIdentity_t();
        _local_csrk = csrk_t();
    }

    virtual void get_whitelist(WhitelistDbCb_t cb, ::Gap::Whitelist_t *whitelist) {
        /*TODO: fill whitelist*/
        cb(whitelist);
    }

    virtual void generate_whitelist_from_bond_table(WhitelistDbCb_t cb, ::Gap::Whitelist_t *whitelist) {
        for (size_t i = 0; i < MAX_ENTRIES && i < whitelist->capacity; i++) {
            if (_entries[i].flags.peer_address_is_public) {
                whitelist->addresses[i].type = BLEProtocol::AddressType::PUBLIC;
            } else {
                whitelist->addresses[i].type = BLEProtocol::AddressType::RANDOM_STATIC;
            }

            memcpy(
                whitelist->addresses[i].address,
                _entries[i].peer_identity.identity_address.data(),
                sizeof(BLEProtocol::AddressBytes_t)
            );
        }

        cb(whitelist);
    }

    virtual void set_whitelist(const ::Gap::Whitelist_t &whitelist) { };

    virtual void add_whitelist_entry(const address_t &address) { }

    virtual void remove_whitelist_entry(const address_t &address) { }

    virtual void clear_whitelist() { }

    /* saving and loading from nvm */

    virtual void restore() { }

    virtual void sync() { }

    virtual void set_restore(bool reload) { }

private:
    entry_t _entries[MAX_ENTRIES];
    SecurityEntryIdentity_t _local_identity;
    csrk_t _local_csrk;
    public_key_coord_t _public_key_x;
    public_key_coord_t _public_key_y;
};

} /* namespace pal */
} /* namespace ble */

#endif /*PAL_MEMORY_SECURITY_DB_H_*/
