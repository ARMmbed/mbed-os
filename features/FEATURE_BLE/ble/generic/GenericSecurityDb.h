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

#ifndef __GENERIC_SECURITY_MANAGER_DB_H__
#define __GENERIC_SECURITY_MANAGER_DB_H__

#include "Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include <stdlib.h>

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::irk_t;
using ble::csrk_t;
using ble::ltk_t;
using ble::ediv_t;
using ble::rand_t;
using ble::pal::connection_peer_address_type_t;

/* separate structs for keys to allow db implementation
 * to minimise memory usage, only holding live connection
 * state in memory */

struct SecurityEntry_t {
    SecurityEntry_t()
        : handle(0),
        encryption_key_size(0),
        peer_address_public(false),
        local_address_public(false),
        csrk_stored(false),
        mitm_csrk(false),
        ltk_stored(false),
        mitm_ltk(false),
        secure_connections(false),
        connected(false),
        authenticated(false),
        master(false),
        encryption_requested(false),
        encryption_failed(false),
        encrypted(false),
        signing_requested(false),
        mitm_requested(false),
        mitm_performed(false),
        oob(false),
        oob_mitm_protection(false) { }

    /**
     * Reset state of the connection when disconnected.
     */
    void reset() {
        mitm_requested = false;
        mitm_performed = false;
        connected = true;
        authenticated = false;
        encryption_requested = false;
        encryption_failed = false;
        encrypted = false;
        signing_requested = false;
        oob = false;
        oob_mitm_protection = false;
    }

    connection_handle_t handle;
    uint8_t encryption_key_size;
    uint8_t peer_address_public:1;
    uint8_t local_address_public:1;

    uint8_t csrk_stored:1;
    uint8_t mitm_csrk:1;
    uint8_t ltk_stored:1;
    uint8_t mitm_ltk:1;
    uint8_t secure_connections:1;

    /* do not store in NVM */

    uint8_t connected:1;
    uint8_t authenticated:1; /**< have we turned encryption on during this connection */
    uint8_t master:1;

    uint8_t encryption_requested:1;
    uint8_t encryption_failed:1;
    uint8_t encrypted:1;
    uint8_t signing_requested:1;

    uint8_t mitm_requested:1;
    uint8_t mitm_performed:1; /**< keys exchange will have MITM protection */

    uint8_t oob:1;
    uint8_t oob_mitm_protection:1;
};

struct SecurityEntryKeys_t {
    ltk_t ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    address_t peer_identity_address;
    irk_t peer_irk;
    address_t local_identity_address;
    irk_t local_irk;
};

/* callbacks for asynchronous data retrieval from the security db */

typedef mbed::Callback<void(const SecurityEntry_t*, const SecurityEntryKeys_t*)> SecurityEntryKeysDbCb_t;
typedef mbed::Callback<void(connection_handle_t, const csrk_t*)> SecurityEntryCsrkDbCb_t;
typedef mbed::Callback<void(Gap::Whitelist_t*)> WhitelistDbCb_t;

/**
 * SecurityDB holds the state for active connections and bonded devices.
 * Keys can be stored in NVM and are returned via callbacks.
 * SecurityDB is responsible for serialising any requests and keeping
 * the store in a consistent state.
 * Active connections state must be returned immediately.
 */
class GenericSecurityDb {
public:
    GenericSecurityDb() { };
    virtual ~GenericSecurityDb() { };

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     *
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    virtual SecurityEntry_t* get_entry(
        connection_handle_t connection
    ) = 0;

    /* local keys */

    /**
     * Retrieve stored LTK based on passed in EDIV and RAND values.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] connection handle for the connection requesting the key
     * @param[in] ediv one of the values used to identify the LTK
     * @param[in] rand one of the values used to identify the LTK
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) = 0;

    /**
     * Save new local LTK for a connection.
     *
     * @param[in] connection handle for which the LTK is being updated
     * @param[in] ltk the new LTK, if the device is slave, this is the LTK that will
     *            be used when link is encrypted
     */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK.
     *
     * @param[in] connection handle for the connection which values are being updated
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) = 0;

    /* peer's keys */

    /**
     * Return asynchronously the peer signing key through a callback
     * so that signed packets can be verified.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] connection handle of the connection queried
     */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /**
     * Return asynchronously the peer encryption key through a callback
     * so that encryption can be enabled.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] connection handle of the connection queried
     */
    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection
    ) = 0;

    /**
     * Update all values in one call.
     *
     * @param[in] connection for which the values are being updated
     * @param[in] address_is_public is the address public or private
     * @param[in] peer_address identity address of the peer
     * @param[in] ediv EDIV value
     * @param[in] rand RAND value
     * @param[in] ltk LTK value
     * @param[in] irk IRK value
     * @param[in] csrk CSRK value
     */
    virtual void set_entry_peer(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address,
        const ediv_t *ediv,
        const rand_t *rand,
        const ltk_t *ltk,
        const irk_t *irk,
        const csrk_t *csrk
    ) = 0;

    /**
     * Save new LTK received from the peer.
     *
     * @param[in] connection handle for which the LTK is being updated
     * @param[in] ltk the new LTK, if the peer device is slave, this is the LTK that will
     *            be used when link is encrypted
     */
    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK sent by the peer.
     *
     * @param[in] connection handle for the connection which values are being updated
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) = 0;

    /**
     * Update IRK for this connection.
     *
     * @param[in] connection handle of the connection being updated
     * @param[in] irk new IRK value
     */
    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t *irk
    ) = 0;

    /**
     * Update the identity address of the peer.
     *
     * @param[in] connection connection for the peer address being updated
     * @param[in] address_is_public is the identity address public or private
     * @param[in] peer_address the new address
     */
    virtual void set_entry_peer_bdaddr(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address
    ) = 0;

    /**
     * Update peer signing key.
     *
     * @param[in] connection handle of the connection being updated
     * @param[in] csrk new CSRK value
     */
    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t *csrk
    ) = 0;

    /* local csrk */

    /**
     * Return local signing key used for signing packets.
     *
     * @return pointer to local CSRK
     */
    virtual const csrk_t* get_local_csrk() = 0;

    /**
     * Update local signing key.
     *
     * @param[in] csrk new CSRK value
     */
    virtual void set_local_csrk(
        const csrk_t *csrk
    ) = 0;

    /* list management */

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param[in] connection this will be the index for live entries.
     * @param[in] peer_address_type type of address
     * @param[in] peer_address this address will be used to locate existing entry.
     *
     * @return pointer to entry newly created or located existing entry.
     */
    virtual SecurityEntry_t* connect_entry(
        connection_handle_t connection,
        connection_peer_address_type_t::type peer_address_type,
        const address_t& peer_address
    ) = 0;

    /**
     * Create a new entry or retrieve existing stored entry
     * and put it in the live connections store to be retrieved
     * synchronously through connection handle.
     *
     * @param[in] connection this handle will be freed up from the security db
     */
    virtual void disconnect_entry(
        connection_handle_t connection
    ) = 0;

    /**
     * Remove entry for this peer from NVM.
     *
     * @param[in] peer_identity_address peer address that no longer needs NVM storage.
     */
    virtual void remove_entry(
        const address_t peer_identity_address
    ) = 0;

    /**
     * Remove all entries from the security DB.
     */
    virtual void clear_entries() = 0;

    /**
     * Asynchronously return the whitelist stored in NVM through a callback. Function
     * takes ownership of the memory. The whitelist and the ownership will be returned
     * in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void get_whitelist(
        WhitelistDbCb_t cb,
        Gap::Whitelist_t *whitelist
    ) = 0;

    /**
     * Asynchronously return a whitelist through a callback, generated from the bond table.
     * Function takes ownership of the memory. The whitelist and the ownership will be
     * returned in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void generate_whitelist_from_bond_table(
        WhitelistDbCb_t cb,
        Gap::Whitelist_t *whitelist
    ) = 0;

    /**
     * Update the whitelist stored in NVM by replacing it with new one.
     *
     * @param[in] whitelist
     */
    virtual void set_whitelist(
        const Gap::Whitelist_t& whitelist
    ) = 0;

    /**
     * Add a new entry to the whitelist in the NVM.
     *
     * @param[in] address new whitelist entry
     */
    virtual void add_whitelist_entry(
        const address_t &address
    ) = 0;

    /**
     * Remove whitelist entry from NVM.
     *
     * @param[in] address entry to be removed
     */
    virtual void remove_whitelist_entry(
        const address_t &address
    ) = 0;

    /**
     *Remove all whitelist entries stored in the NVM.
     */
    virtual void clear_whitelist() = 0;

    /* saving and loading from nvm */

    /**
     * Read values from storage.
     */
    virtual void restore() = 0;

    /**
     * Flush all values which might be stored in memory into NVM.
     */
    virtual void sync() = 0;

    /**
     * Toggle whether values should be preserved across resets.
     *
     * @param[in] reload if true values will be preserved across resets.
     */
    virtual void set_restore(bool reload) = 0;
};

/* naive memory implementation for verification */
class MemoryGenericSecurityDb : public GenericSecurityDb {
private:
    struct db_store_t {
        db_store_t() { };
        SecurityEntry_t entry;
        SecurityEntryKeys_t key;
        csrk_t csrk;
    };
    static const size_t MAX_ENTRIES = 5;

public:
    MemoryGenericSecurityDb() { };
    virtual ~MemoryGenericSecurityDb() { };

    virtual SecurityEntry_t* get_entry(connection_handle_t connection) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }
        return entry;
    }

    /* local keys */

    /* get */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) {
        SecurityEntry_t *entry = NULL;
        db_store_t *store = get_store(connection);
        if (store) {
            entry = &store->entry;
        }

        /* validate we have the correct key */
        if (ediv && rand
            && *ediv == _local_keys.ediv
            && *rand == _local_keys.rand) {
            cb(entry, &_local_keys);
        } else {
            cb(entry, NULL);
        }
    }

    /* set */
    virtual void set_entry_local_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    ) {
        _local_keys.ltk = *ltk;
    }

    virtual void set_entry_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) {
        _local_keys.ediv = *ediv;
        _local_keys.rand = *rand;
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
            key = &store->key;
        }
        cb(entry, key);
    }

    /* set */
    virtual void set_entry_peer(
        connection_handle_t connection,
        bool address_is_public,
        const address_t &peer_address,
        const ediv_t *ediv,
        const rand_t *rand,
        const ltk_t *ltk,
        const irk_t *irk,
        const csrk_t *csrk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->key.ltk = *ltk;
            store->key.ediv = *ediv;
            store->key.rand = *rand;
            store->csrk = *csrk;
            size_t index = store - _db;
            _identities[index].peer_irk = *irk;
            _identities[index].peer_identity_address = peer_address;
        }
    }

    virtual void set_entry_peer_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->key.ltk = *ltk;
        }
    }

    virtual void set_entry_peer_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->key.ediv = *ediv;
            store->key.rand = *rand;
        }
    }

    virtual void set_entry_peer_irk(
        connection_handle_t connection,
        const irk_t *irk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            size_t index = store - _db;
            _identities[index].peer_irk = *irk;
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
            _identities[index].peer_identity_address = peer_address;
        }
    }

    virtual void set_entry_peer_csrk(
        connection_handle_t connection,
        const csrk_t *csrk
    ) {
        db_store_t *store = get_store(connection);
        if (store) {
            store->csrk = *csrk;
        }
    }

    /* local csrk */

    virtual const csrk_t* get_local_csrk() {
        return &_local_csrk;
    }

    virtual void set_local_csrk(const csrk_t *csrk) {
        _local_csrk = *csrk;
    }

    /* list management */

    virtual SecurityEntry_t* connect_entry(
        connection_handle_t connection,
        connection_peer_address_type_t::type peer_address_type,
        const address_t& peer_address
    ) {
        const bool peer_address_public =
            (peer_address_type == connection_peer_address_type_t::PUBLIC_ADDRESS);

        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (_db[i].entry.connected) {
                continue;
            } else if (peer_address == _identities[i].peer_identity_address
                && _db[i].entry.peer_address_public == peer_address_public) {
                return &_db[i].entry;
            }
        }

        /* if we din't find one grab the first disconnected slot*/
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            if (!_db[i].entry.connected) {
                _db[i] = db_store_t();
                _identities[i] = SecurityEntryIdentity_t();
                _identities[i].peer_identity_address = peer_address;
                _db[i].entry.peer_address_public = peer_address_public;
                return &_db[i].entry;
            }
        }

        return NULL;
    }

    virtual void disconnect_entry(connection_handle_t connection) { }

    virtual void remove_entry(address_t peer_identity_address);

    virtual void clear_entries() {
        for (size_t i = 0; i < MAX_ENTRIES; i++) {
            _db[i] = db_store_t();
        }
        _local_keys = SecurityEntryKeys_t();
        _local_identity = SecurityEntryIdentity_t();
        _local_csrk = csrk_t();
    }

    virtual void get_whitelist(WhitelistDbCb_t cb, Gap::Whitelist_t *whitelist) {
        /*TODO: fill whitelist*/
        cb(whitelist);
    }

    virtual void generate_whitelist_from_bond_table(WhitelistDbCb_t cb, Gap::Whitelist_t *whitelist) {
        for (size_t i = 0; i < MAX_ENTRIES && i < whitelist->capacity; i++) {
            if (_db[i].entry.peer_address_public) {
                whitelist->addresses[i].type = BLEProtocol::AddressType::PUBLIC;
            } else {
                whitelist->addresses[i].type = BLEProtocol::AddressType::RANDOM_STATIC;
            }

            memcpy(
                whitelist->addresses[i].address,
                _identities[i].peer_identity_address.data(),
                sizeof(BLEProtocol::AddressBytes_t)
            );
        }

        cb(whitelist);
    }

    virtual void update_whitelist(Gap::Whitelist_t& whitelist) { }

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
    SecurityEntryKeys_t _local_keys;
    SecurityEntryIdentity_t _local_identity;
    csrk_t _local_csrk;
};

} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_DB_H__*/
