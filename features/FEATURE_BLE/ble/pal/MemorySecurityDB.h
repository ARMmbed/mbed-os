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

#ifndef PAL_MEMORY_SECURITY_MANAGER_DB_H__
#define PAL_MEMORY_SECURITY_MANAGER_DB_H__

#include "SecurityDB.h"
#include "Gap.h"

namespace ble {
namespace pal {

/* naive memory implementation for verification
 * TODO: make thread safe */
class MemorySecurityDb : public SecurityDb {
private:
    struct db_store_t {
        db_store_t() { };
        SecurityEntry_t entry;
        SecurityEntryKeys_t peer_keys;
        SecurityEntryKeys_t local_keys;
        csrk_t csrk;
    };
    static const size_t MAX_ENTRIES = 5;

public:
    MemorySecurityDb() { };
    virtual ~MemorySecurityDb() { };

    virtual SecurityEntry_t* get_entry(
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }
        return entry;
    }

    virtual SecurityEntry_t* get_entry(
        const address_t &peer_address
    ) {
        SecurityEntry_t *entry = NULL;
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                continue;
            } else if (peer_address == _db[i].entry.peer_address) {
                entry = &_db[i].entry;
            }
        }
        return entry;
    }

    /* local keys */

    /* get */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }

        /* validate we have the correct key */
        if (ediv == store->local_keys.ediv
            && rand == store->local_keys.rand) {
            cb(entry, &store->local_keys);
        } else {
            cb(entry, NULL);
        }
    }

    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }

        /* validate we have the correct key */
        if (entry->secure_connections_paired) {
            cb(entry, &store->local_keys);
        } else {
            cb(entry, NULL);
        }
    }


    /* set */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->local_keys.ltk = ltk;
        }
    }

    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->local_keys.ediv = ediv;
            store->local_keys.rand = rand;
        }
    }

    /* peer's keys */

    /* get */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        csrk_t csrk;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
            csrk = store->csrk;
        }
        cb(entry->handle, &csrk);
    }

    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) {
        SecurityEntry_t *entry = NULL;
        SecurityEntryKeys_t *key = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
            key = &store->peer_keys;
        }
        cb(entry, key);
    }

    /* set */

    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->peer_keys.ltk = ltk;
        }
    }

    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->peer_keys.ediv = ediv;
            store->peer_keys.rand = rand;
        }
    }

    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t &irk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            size_t index = store - _db;
            _identities[index].irk = irk;
        }
    }

    virtual void set_entry_peer_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            size_t index = store - _db;
            _identities[index].identity_address = peer_address;
        }
    }

    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t &csrk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->csrk = csrk;
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

    virtual const public_key_t& get_public_key_x() {
        return _public_key_x;
    }

    virtual const public_key_t& get_public_key_y() {
        return _public_key_y;
    }

    virtual void set_public_key(
        const public_key_t &public_key_x,
        const public_key_t &public_key_y
    ) {
        _public_key_x = public_key_x;
        _public_key_y = public_key_y;
    }

    /* oob data */

    /** There is always only one OOB data set stored at a time */

    virtual const address_t& get_peer_sc_oob_address() {
        return _peer_sc_oob_address;
    }

    virtual const oob_rand_t& get_peer_sc_oob_random() {
        return _peer_sc_oob_random;
    }

    virtual const oob_confirm_t& get_peer_sc_oob_confirm() {
        return _peer_sc_oob_confirm;
    }

    virtual void get_sc_oob_data(
        address_t &peer_address,
        oob_rand_t &peer_random,
        oob_confirm_t &peer_confirm,
        oob_rand_t &local_random
    ) {
        peer_address = _peer_sc_oob_address;
        peer_random = _peer_sc_oob_random;
        peer_confirm = _peer_sc_oob_confirm;
        local_random = _local_sc_oob_random;
    }

    virtual const oob_rand_t& get_local_sc_oob_random() {
        return _local_sc_oob_random;
    }

    virtual void set_peer_sc_oob_data(
        const address_t &address,
        const oob_rand_t &random,
        const oob_confirm_t &confirm
    ) {
        _peer_sc_oob_address = address;
        _peer_sc_oob_random = random;
        _peer_sc_oob_confirm = confirm;
    }

    virtual void set_local_sc_oob_random(
        const oob_rand_t &random
    ) {
        _local_sc_oob_random = random;
    }

    /* list management */

    virtual SecurityEntry_t* connect_entry(
        connection_handle_t connection,
        BLEProtocol::AddressType_t peer_address_type,
        const address_t &peer_address,
        const address_t &local_address
    ) {
        const bool peer_address_public =
            (peer_address_type == BLEProtocol::AddressType::PUBLIC);

        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_db[i].entry.connected) {
                continue;
            } else if (peer_address == _identities[i].identity_address
                && _db[i].entry.peer_address_is_public == peer_address_public) {
                return &_db[i].entry;
            }
        }

        /* if we din't find one grab the first disconnected slot*/
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                _db[i] = db_store_t();
                _identities[i] = SecurityEntryIdentity_t();
                _db[i].entry.peer_address = peer_address;
                _db[i].entry.local_address = local_address;
                _db[i].entry.peer_address_is_public = peer_address_public;
                return &_db[i].entry;
            }
        }

        return NULL;
    }

    virtual void disconnect_entry(connection_handle_t connection) { }

    virtual void remove_entry(address_t peer_identity_address) { }

    virtual void clear_entries() {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            _db[i] = db_store_t();
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
            if (_db[i].entry.peer_address_is_public) {
                whitelist->addresses[i].type = BLEProtocol::AddressType::PUBLIC;
            } else {
                whitelist->addresses[i].type = BLEProtocol::AddressType::RANDOM_STATIC;
            }

            memcpy(
                whitelist->addresses[i].address,
                _identities[i].identity_address.data(),
                sizeof(BLEProtocol::AddressBytes_t)
            );
        }

        cb(whitelist);
    }

    virtual void update_whitelist(::Gap::Whitelist_t &whitelist) { }

    virtual void set_whitelist(const ::Gap::Whitelist_t &whitelist) { };

    virtual void add_whitelist_entry(const address_t &address) { }

    virtual void remove_whitelist_entry(const address_t &address) { }

    virtual void clear_whitelist() { }

    /* saving and loading from nvm */

    virtual void restore() { }

    virtual void sync() { }

    virtual void set_restore(bool reload) { }

private:

    virtual db_store_t* get_store(connection_handle_t connection) {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                continue;
            } else if (connection == _db[i].entry.handle) {
                return &_db[i];
            }
        }
        return NULL;
    }

    db_store_t _db[MAX_ENTRIES];
    SecurityEntryIdentity_t _identities[MAX_ENTRIES];
    SecurityEntryIdentity_t _local_identity;
    csrk_t _local_csrk;
    public_key_t _public_key_x;
    public_key_t _public_key_y;

    address_t _peer_sc_oob_address;
    oob_rand_t _peer_sc_oob_random;
    oob_confirm_t _peer_sc_oob_confirm;
    oob_rand_t _local_sc_oob_random;
};

} /* namespace pal */
} /* namespace ble */

#endif /*PAL_MEMORY_SECURITY_MANAGER_DB_H__*/
