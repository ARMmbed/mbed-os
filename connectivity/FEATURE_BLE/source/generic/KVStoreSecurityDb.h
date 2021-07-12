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

#if BLE_SECURITY_DATABASE_KVSTORE

#ifndef GENERIC_KFSTORE_SECURITY_DB_H_
#define GENERIC_KFSTORE_SECURITY_DB_H_

#include "kvstore_global_api.h"
#include "mbed_error.h"

#include "SecurityDb.h"

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

namespace ble {

/** Filesystem implementation */
class KVStoreSecurityDb : public SecurityDb {
private:

    struct entry_t {
        SecurityDistributionFlags_t flags;
        sign_count_t peer_sign_counter;
    };

    static entry_t *as_entry(entry_handle_t db_handle);

    static constexpr uint8_t KVSTORESECURITYDB_VERSION = 1;

    static constexpr size_t DB_PREFIX_SIZE = 7 + sizeof (STR(MBED_CONF_STORAGE_DEFAULT_KV)) - 1;
    static constexpr size_t DB_KEY_SIZE = 3;
    static constexpr size_t DB_ENTRY_KEY_SIZE = 2;
    static constexpr size_t DB_FULL_KEY_SIZE = DB_PREFIX_SIZE + DB_KEY_SIZE + 1;

    static constexpr char DB_PREFIX[DB_PREFIX_SIZE + 1] = { "/" STR(MBED_CONF_STORAGE_DEFAULT_KV) "/_ble_" };

    static constexpr char DB_ENTRIES[DB_KEY_SIZE] = { 'e','n','t' };

    static constexpr char DB_ENTRY_PEER_IDENTITY[DB_ENTRY_KEY_SIZE] = { 'i','d' };
    static constexpr char DB_ENTRY_LOCAL_KEYS[DB_ENTRY_KEY_SIZE] = { 'l','k' };
    static constexpr char DB_ENTRY_PEER_KEYS[DB_ENTRY_KEY_SIZE] = { 'p','k' };
    static constexpr char DB_ENTRY_PEER_SIGNING[DB_ENTRY_KEY_SIZE] = { 'p','s' };

    static constexpr char DB_LOCAL_IDENTITY[DB_KEY_SIZE] = { 'l','i','d' };
    static constexpr char DB_LOCAL_CSRK[DB_KEY_SIZE] = { 'l','c','s' };
    static constexpr char DB_LOCAL_SIGN_COUNT[DB_KEY_SIZE] = { 'l','s','c' };

    static constexpr char DB_VERSION[DB_KEY_SIZE] = { 'v','e','r' };
    static constexpr char DB_RESTORE[DB_KEY_SIZE] = { 'r','e','s' };

    template<class T>
    static void db_read(T *value, const char* key) {
        char db_key[DB_FULL_KEY_SIZE];
        create_key(db_key, key);
        size_t size;
        const int ret = kv_get(db_key, value, sizeof(T), &size);
        (void)ret;//suppress unused var warning
        MBED_ASSERT(ret == MBED_SUCCESS && size == sizeof(T));
    }

    template<class T>
    static void db_write(T *value, const char* key) {
        char db_key[DB_FULL_KEY_SIZE];
        create_key(db_key, key);
        const int ret = kv_set(db_key, value, sizeof(T), 0);
        (void)ret;//suppress unused var warning
        MBED_ASSERT(ret == MBED_SUCCESS);
    }

    template<class T>
    static void db_write_entry(T *value, const char* key, uint8_t index) {
        char db_key[DB_FULL_KEY_SIZE];
        create_entry_key(db_key, key, index);
        const int ret = kv_set(db_key, value, sizeof(T), 0);
        (void)ret;//suppress unused var warning
        MBED_ASSERT(ret == MBED_SUCCESS);
    }

    template<class T>
    static void db_read_entry(T *value, const char* key, uint8_t index) {
        char db_key[DB_FULL_KEY_SIZE];
        create_entry_key(db_key, key, index);
        size_t size;
        const int ret = kv_get(db_key, value, sizeof(T), &size);
        (void)ret;//suppress unused var warning
        MBED_ASSERT(ret == MBED_SUCCESS && size == sizeof(T));
    }

    static void create_key(char* full_key, const char* key) {
        memcpy(full_key, DB_PREFIX, DB_PREFIX_SIZE);
        memcpy(full_key + DB_PREFIX_SIZE, key, DB_KEY_SIZE);
        full_key[DB_PREFIX_SIZE + DB_KEY_SIZE] = '\0';
    }

    static void create_entry_key(char* full_key, const char* key, uint8_t index) {
        memcpy(full_key, DB_PREFIX, DB_PREFIX_SIZE);
        memcpy(full_key + DB_PREFIX_SIZE, key, DB_ENTRY_KEY_SIZE);
        full_key[DB_PREFIX_SIZE + DB_ENTRY_KEY_SIZE] = (char)('0' + index);
        full_key[DB_PREFIX_SIZE + DB_KEY_SIZE] = '\0';
    }

public:
    KVStoreSecurityDb();
    virtual ~KVStoreSecurityDb();

    /**
     * Validates or creates a kvstore entry for the security database.
     * @return true if KVStore works
     */
    static bool open_db();

    virtual SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_handle
    );

    /* local keys */

    /* set */

    virtual void set_entry_local_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    );

    virtual void set_entry_local_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /* peer's keys */

    /* set */

    virtual void set_entry_peer_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    );

    virtual void set_entry_peer_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    );

    virtual void set_entry_peer_irk(
        entry_handle_t db_handle,
        const irk_t &irk
    );

    virtual void set_entry_peer_bdaddr(
        entry_handle_t db_handle,
        bool address_is_public,
        const address_t &peer_address
    );

    virtual void set_entry_peer_csrk(
        entry_handle_t db_handle,
        const csrk_t &csrk
    );

    virtual void set_entry_peer_sign_counter(
        entry_handle_t db_handle,
        sign_count_t sign_counter
    );

    /* local csrk and identity */

    virtual void set_local_csrk(
        const csrk_t &csrk
    );

    virtual void set_local_identity(
        const irk_t &irk,
        const address_t &identity_address,
        bool public_address
    );

    /* I am not overriding set_local_sign_counter to avoid constant filesystem writes,
     * instead this is synced by sync (which is called on disconnection) */

    /* saving and loading from nvm */

    virtual void restore();

    virtual void sync(entry_handle_t db_handle = invalid_entry_handle);

    virtual void set_restore(bool reload);

private:
    virtual uint8_t get_entry_count();

    virtual SecurityDistributionFlags_t* get_entry_handle_by_index(uint8_t index);

    virtual void reset_entry(entry_handle_t db_handle);

    virtual SecurityEntryIdentity_t* read_in_entry_peer_identity(entry_handle_t db_handle);
    virtual SecurityEntryKeys_t* read_in_entry_peer_keys(entry_handle_t db_handle);
    virtual SecurityEntryKeys_t* read_in_entry_local_keys(entry_handle_t db_handle);
    virtual SecurityEntrySigning_t* read_in_entry_peer_signing(entry_handle_t db_handle);

    /**
     * Zero the db file.
     * @return true if KVStore works
     */
    static bool erase_db();

private:
    entry_t _entries[BLE_SECURITY_DATABASE_MAX_ENTRIES];
    uint8_t _buffer[sizeof(SecurityEntryKeys_t)];

    uint8_t get_index(entry_t *entry)
    {
        return  entry - _entries;
    }
};

} /* namespace ble */

#endif /*GENERIC_KFSTORE_SECURITY_DB_H_*/

#endif /*BLE_SECURITY_DATABASE_KVSTORE*/
