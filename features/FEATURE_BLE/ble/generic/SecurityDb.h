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

#ifndef GENERIC_SECURITY_MANAGER_DB_H__
#define GENERIC_SECURITY_MANAGER_DB_H__

#include "platform/Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/Gap.h"
#include <stdlib.h>

namespace ble {
namespace generic {


/**
 * Security flags associated with a bond.
 */
struct SecurityDistributionFlags_t {
    SecurityDistributionFlags_t() :
        peer_address(),
        encryption_key_size(0),
        peer_address_is_public(false),
        csrk_stored(false),
        ltk_stored(false),
        ltk_sent(false),
        irk_stored(false),
        csrk_mitm_protected(false),
        ltk_mitm_protected(false),
        secure_connections_paired(false),
        connected(false) {
    }

    /** peer address */
    address_t peer_address;

    /** encryption key size */
    uint8_t encryption_key_size;
    /** true if peer address is public, false if it's static random */
    uint8_t peer_address_is_public:1;

    /** CSRK (Connection Signature Resolving Key) has been distributed and stored */
    uint8_t csrk_stored:1;
    /** LTK (Long Term Key) has been distributed and stored */
    uint8_t ltk_stored:1;
    uint8_t ltk_sent:1;
    /** the security entry has been distributed and stored */
    uint8_t irk_stored:1;

    /** CSRK that is stored has MITM protection */
    uint8_t csrk_mitm_protected:1;
    /** LTK that is stored has MITM protection */
    uint8_t ltk_mitm_protected:1;
    /** the current pairing was done using Secure Connections */
    uint8_t secure_connections_paired:1;
    uint8_t connected:1;
};

/** Long Term Key and data used to identify it */
struct SecurityEntryKeys_t {
    /** Long Term Key */
    ltk_t ltk;
    /** EDIV (Encryption diversifier) used to identify LTK during legacy pairing */
    ediv_t ediv;
    /** Rand (random number) used to identify LTK during legacy pairing */
    rand_t rand;
};

/** CSRK and sign counter used to verify messages */
struct SecurityEntrySigning_t {
    SecurityEntrySigning_t() : counter(0) { };
    /** Signing key */
    csrk_t csrk;
    /** counter used to verify message to guard from replay attacks */
    sign_count_t counter;
};

/** Data for resolving random resolvable addresses */
struct SecurityEntryIdentity_t {
    /** identity address */
    address_t identity_address;
    /** Identity Resolving Key */
    irk_t irk;
    /** true if peer identity address is public, false if it's static random */
    uint8_t identity_address_is_public:1;
};

/**
 * SecurityDb holds the state for active connections and bonded devices.
 * Keys can be stored in NVM and are returned via callbacks.
 * SecurityDb is responsible for serialising any requests and keeping
 * the store in a consistent state.
 * Active connections state must be returned immediately.
 */
class SecurityDb {
public:
    /**
     * Opaque type representing a handle to a database entry.
     */
    typedef void* entry_handle_t;

    /* callbacks for asynchronous data retrieval from the security db */

    typedef mbed::Callback<void(entry_handle_t, const SecurityEntryKeys_t*)>
        SecurityEntryKeysDbCb_t;
    typedef mbed::Callback<void(entry_handle_t, const SecurityEntrySigning_t*)>
        SecurityEntrySigningDbCb_t;
    typedef mbed::Callback<void(entry_handle_t, const SecurityEntryIdentity_t*)>
        SecurityEntryIdentityDbCb_t;
    typedef mbed::Callback<void(ArrayView<SecurityEntryIdentity_t>&, size_t count)>
        IdentitylistDbCb_t;
    typedef mbed::Callback<void(::Gap::Whitelist_t*)>
        WhitelistDbCb_t;

    SecurityDb() : _local_sign_counter(0) { };
    virtual ~SecurityDb() { };

    /**
     * Return immediately security flags associated to a db entry.
     *
     * @param[in] db_handle Entry of the database queried.
     * @return pointer to the flags or NULL if the entry do not have any
     * associated flags.
     */
    virtual SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_handle
    ) = 0;

    /**
     * Set the distribution flags of a DB entry.
     *
     * @param[in] db_handle Entry of the database that will store the flags.
     * @param[in] flags Distribution flags to store in @p db_handle.
     */
    virtual void set_distribution_flags(
        entry_handle_t db_handle,
        const SecurityDistributionFlags_t& new_flags
    ) {
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
        if (flags) {
            *flags = new_flags;
        }
    }

    /* local keys */

    /**
     * Retrieve stored LTK based on passed in EDIV and RAND values.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] db_handle handle of the entry being queried.
     * @param[in] ediv one of the values used to identify the LTK
     * @param[in] rand one of the values used to identify the LTK
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        SecurityEntryKeys_t* keys = read_in_entry_local_keys(db_handle);
        /* validate we have the correct key */
        if (keys && ediv == keys->ediv && rand == keys->rand) {
            cb(db_handle, keys);
        } else {
            cb(db_handle, NULL);
        }
    }

    /**
     * Retrieve stored LTK generated during secure connections pairing.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] db_handle handle of the entry being queried.
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_handle
    ) {
        SecurityEntryKeys_t* keys = read_in_entry_local_keys(db_handle);
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
        /* validate we have the correct key */
        if (flags && keys && flags->secure_connections_paired) {
            cb(db_handle, keys);
        } else {
            cb(db_handle, NULL);
        }
    }

    /**
     * Save new local LTK for a connection.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] ltk the new LTK, if the device is slave, this is the LTK that
     * will be used when link is encrypted
     */
    virtual void set_entry_local_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_local_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /* peer's keys */

    /**
     * Return asynchronously the peer signing key through a callback
     * so that signed packets can be verified.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] db_handle handle of the entry being queried.
     */
    virtual void get_entry_peer_csrk(
        SecurityEntrySigningDbCb_t cb,
        entry_handle_t db_handle
    ) {
        SecurityEntrySigning_t* signing = read_in_entry_peer_signing(db_handle);
        cb(db_handle, signing);
    }

    /**
     * Return asynchronously the peer encryption key through a callback
     * so that encryption can be enabled.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] db_handle handle of the entry being queried.
     */
    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_handle
    ) {
        SecurityEntryKeys_t* keys = read_in_entry_peer_keys(db_handle);
        cb(db_handle, keys);
    }

    /**
     * Save new LTK received from the peer.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] ltk the new LTK, if the peer device is slave, this is the LTK
     * that will be used when link is encrypted
     */
    virtual void set_entry_peer_ltk(
        entry_handle_t db_handle,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK sent by the peer.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_peer_ediv_rand(
        entry_handle_t db_handle,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /**
     * Update IRK for this connection.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] irk new IRK value
     */
    virtual void set_entry_peer_irk(
        entry_handle_t db_handle,
        const irk_t &irk
    ) = 0;

    /**
     * Update the identity address of the peer.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] address_is_public is the identity address public or private
     * @param[in] peer_address the new address
     */
    virtual void set_entry_peer_bdaddr(
        entry_handle_t db_handle,
        bool address_is_public,
        const address_t &peer_address
    ) = 0;

    /**
     * Retrieve stored identity address and IRK.
     *
     * @param[in] cb callback that will receive the SecurityEntryIdentity_t struct
     * @param[in] db_handle handle of the entry being queried.
     */
    virtual void get_entry_identity(
        SecurityEntryIdentityDbCb_t cb,
        entry_handle_t db_handle
    ) {
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
        if (flags && flags->irk_stored) {
            SecurityEntryIdentity_t* peer_identity = read_in_entry_peer_identity(db_handle);
            if (peer_identity) {
                cb(db_handle, peer_identity);
                return;
            }
        }
        /* avoid duplicate else */
        cb(db_handle, NULL);
    }

    /**
     * Asynchronously return the identity list stored in NVM through a callback.
     * Function takes ownership of the memory. The identity list and the
     * ownership will be returned in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] identity_list preallocated identity_list that will be filled
     * in.
     */
    virtual void get_identity_list(
        IdentitylistDbCb_t cb,
        ArrayView<SecurityEntryIdentity_t>& identity_list
    ) {
        size_t count = 0;
        for (size_t i = 0; i < get_entry_count() && count < identity_list.size(); ++i) {

            entry_handle_t db_handle = get_entry_handle_by_index(i);
            SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);


            if (flags && flags->irk_stored) {
                SecurityEntryIdentity_t* peer_identity = read_in_entry_peer_identity(db_handle);
                if (peer_identity) {
                    identity_list[count] = *peer_identity;
                    count++;
                }
            }
        }

        cb(identity_list, count);
    }

    /**
     * Update peer signing key.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] csrk new CSRK value
     */
    virtual void set_entry_peer_csrk(
        entry_handle_t db_handle,
        const csrk_t &csrk
    ) = 0;

    /**
     * Update peer signing counter.
     *
     * @param[in] db_handle handle of the entry being updated.
     * @param[in] sign_counter new signing counter value
     */
    virtual void set_entry_peer_sign_counter(
        entry_handle_t db_handle,
        sign_count_t sign_counter
    ) = 0;

    /* local csrk */

    /**
     * Return local signing key used for signing packets.
     *
     * @return pointer to local CSRK
     */
    virtual const csrk_t* get_local_csrk() {
        return &_local_csrk;
    }

    /**
     * Return local signing counter.
     *
     * @return signing counter
     */
    virtual sign_count_t get_local_sign_counter() {
        return _local_sign_counter;
    }

    /**
     * Update local signing key.
     *
     * @param[in] csrk new CSRK value
     */
    virtual void set_local_csrk(
        const csrk_t &csrk
    )  {
        _local_csrk = csrk;
    }

    /**
     * Update local signing counter.
     *
     * @param[in] sign_counter new signing counter value
     */
    virtual void set_local_sign_counter(
        sign_count_t sign_counter
    ) {
        _local_sign_counter = sign_counter;
    }

    /* list management */

    /**
     * Open a database entry.
     *
     * While this entry is opened; it can be queried and updated with the help
     * of the database setter and getter functions.
     *
     * @param[in] peer_address_type type of address
     * @param[in] peer_address this address will be used to locate an existing
     * entry.
     *
     * @return A handle to the entry.
     */
    virtual entry_handle_t open_entry(
        peer_address_type_t peer_address_type,
        const address_t &peer_address
    ) {
        entry_handle_t db_handle = find_entry_by_peer_address(peer_address_type, peer_address);
        if (db_handle) {
            return db_handle;
        }

        SecurityDistributionFlags_t* flags = get_free_entry_flags();
        if (flags) {
            const bool peer_address_public =
                (peer_address_type == peer_address_type_t::PUBLIC) ||
                (peer_address_type == peer_address_type_t::PUBLIC_IDENTITY);
            /* we need some address to store, so we store even random ones
             * this address will be used as an id, possibly replaced later
             * by identity address */
            flags->peer_address = peer_address;
            flags->peer_address_is_public = peer_address_public;
            return flags;
        }

        return NULL;
    }

    /**
     * Find a database entry based on peer address.
     *
     * @param[in] peer_address_type type of address
     * @param[in] peer_address this address will be used to locate an existing entry.
     *
     * @return A handle to the entry.
     */
    virtual entry_handle_t find_entry_by_peer_address(
        peer_address_type_t peer_address_type,
        const address_t &peer_address
    ) {
        const bool peer_address_public =
            (peer_address_type == peer_address_type_t::PUBLIC) ||
            (peer_address_type == peer_address_type_t::PUBLIC_IDENTITY);

        for (size_t i = 0; i < get_entry_count(); i++) {
            entry_handle_t db_handle = get_entry_handle_by_index(i);
            SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);

            /* only look among disconnected entries */
            if (flags && !flags->connected) {
                if (peer_address_type == peer_address_type_t::PUBLIC_IDENTITY &&
                    flags->irk_stored == false) {
                    continue;
                }

                /* lookup for connection address used during bonding */
                if (flags->peer_address == peer_address &&
                    flags->peer_address_is_public == peer_address_public) {
                    return flags;
                }

                /* look for the identity address if stored */
                if (flags->irk_stored) {
                    SecurityEntryIdentity_t* identity = read_in_entry_peer_identity(db_handle);

                    if (identity &&
                        identity->identity_address == peer_address &&
                        identity->identity_address_is_public == peer_address_public) {
                        return flags;
                    }
                }
            }
        }

        return NULL;
    }

    /**
     * Close a connection entry.
     *
     * @param[in] db_handle this handle will be freed up from the security db.
     */
    virtual void close_entry(entry_handle_t db_handle) {
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
        if (flags) {
            flags->connected = false;
        }
        sync(db_handle);
    }

    /**
     * Remove entry for this peer from NVM.
     *
     * @param[in] peer_address_type type of address
     * @param[in] peer_address this address will be used to locate an existing
     * entry.
     *
     * @return A handle to the entry.
     */
    virtual void remove_entry(
        peer_address_type_t peer_address_type,
        const address_t &peer_address
    ) {
        entry_handle_t db_handle = find_entry_by_peer_address(peer_address_type, peer_address);
        if (db_handle) {
            reset_entry(db_handle);
        }
    }

    /**
     * Remove all entries from the security DB.
     */
    virtual void clear_entries() {
        for (size_t i = 0; i < get_entry_count(); i++) {
            entry_handle_t db_handle = get_entry_handle_by_index(i);
            reset_entry(db_handle);
        }
        _local_identity = SecurityEntryIdentity_t();
        _local_csrk = csrk_t();
    }

    /**
     * Asynchronously return the whitelist stored in NVM through a callback.
     * Function takes ownership of the memory. The whitelist and the ownership
     * will be returned in the callback.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void get_whitelist(
        WhitelistDbCb_t cb,
        ::Gap::Whitelist_t *whitelist
    ) {
        /*TODO: fill whitelist*/
        cb(whitelist);
    }

    /**
     * Asynchronously return a whitelist through a callback, generated from the
     * bond table.
     *
     * @param[in] cb callback that will receive the whitelist
     * @param[in] whitelist preallocated whitelist that will be filled in
     */
    virtual void generate_whitelist_from_bond_table(
        WhitelistDbCb_t cb,
        ::Gap::Whitelist_t *whitelist
    ) {
        for (size_t i = 0; i < get_entry_count() && whitelist->size < whitelist->capacity; i++) {
            entry_handle_t db_handle = get_entry_handle_by_index(i);
            SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);

            if (!flags || !flags->irk_stored) {
                continue;
            }

            SecurityEntryIdentity_t* identity = read_in_entry_peer_identity(db_handle);
            if (!identity) {
                continue;
            }

            memcpy(
                whitelist->addresses[whitelist->size].address,
                identity->identity_address.data(),
                sizeof(BLEProtocol::AddressBytes_t)
            );

            if (flags->peer_address_is_public) {
                whitelist->addresses[whitelist->size].type = BLEProtocol::AddressType::PUBLIC;
            } else {
                whitelist->addresses[whitelist->size].type = BLEProtocol::AddressType::RANDOM_STATIC;
            }

            whitelist->size++;
        }

        cb(whitelist);
    }

    /**
     * Update the whitelist stored in NVM by replacing it with new one.
     *
     * @param[in] whitelist
     */
    virtual void set_whitelist(const ::Gap::Whitelist_t &whitelist) { };

    /**
     * Add a new entry to the whitelist in the NVM.
     *
     * @param[in] address new whitelist entry
     */
    virtual void add_whitelist_entry(const address_t &address) { };

    /**
     * Remove whitelist entry from NVM.
     *
     * @param[in] address entry to be removed
     */
    virtual void remove_whitelist_entry(const address_t &address) { };

    /**
     *Remove all whitelist entries stored in the NVM.
     */
    virtual void clear_whitelist() { };

    /* saving and loading from nvm */

    /**
     * Read values from storage.
     */
    virtual void restore() { };

    /**
     * Flush all values which might be stored in memory into NVM.
     */
    virtual void sync(entry_handle_t db_handle) { };

    /**
     * Toggle whether values should be preserved across resets.
     *
     * @param[in] reload if true values will be preserved across resets.
     */
    virtual void set_restore(bool reload) { };

private:
    /**
     * Get an entry for a new connection not present in the db yet. This will find a free entry
     * or use a disconnected entry by reseting all the stored information.
     * @return empty entry for the new connection
     */
    virtual SecurityDistributionFlags_t* get_free_entry_flags() {
        /* get a free one if available */
        SecurityDistributionFlags_t* match = NULL;
        for (size_t i = 0; i < get_entry_count(); i++) {
            entry_handle_t db_handle = get_entry_handle_by_index(i);
            SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);

            if (flags && !flags->connected) {
                /* we settle for any disconnected if we don't find an empty one */
                match = flags;
                if (!flags->csrk_stored
                    && !flags->ltk_stored
                    && !flags->ltk_sent
                    && !flags->irk_stored) {
                    /* empty one found, stop looking*/
                    break;
                }
            }
        }

        if (match) {
            reset_entry(match);
        }

        return match;
    }

    /**
     * How many entries can be stored in the databes.
     * @return max number of entries
     */
    virtual uint8_t get_entry_count() = 0;

    /**
     * Return database entry based on its index.
     * @param index index from 0 to get_entry_count()
     * @return databse entry stored at index
     */
    virtual SecurityDistributionFlags_t* get_entry_handle_by_index(uint8_t index) = 0;

    /**
     * Delete all the information.
     * @param db_handle handle for the entry to be reset
     */
    virtual void reset_entry(entry_handle_t db_handle) = 0;

    /**
     * This will read in the requested information into a buffer that will remain valid
     * until another read_in call is made.
     * @param db_handle handle of the entry to be read
     * @return pointer to buffer holding the query result, NULL when not found
     */
    virtual SecurityEntryIdentity_t* read_in_entry_peer_identity(entry_handle_t db_handle) = 0;

    /**
     * This will read in the requested information into a buffer that will remain valid
     * until another read_in call is made.
     * @param db_handle handle of the entry to be read
     * @return pointer to buffer holding the query result, NULL when not found
     */
    virtual SecurityEntryKeys_t* read_in_entry_peer_keys(entry_handle_t db_handle) = 0;

    /**
     * This will read in the requested information into a buffer that will remain valid
     * until another read_in call is made.
     * @param db_handle handle of the entry to be read
     * @return pointer to buffer holding the query result, NULL when not found
     */
    virtual SecurityEntryKeys_t* read_in_entry_local_keys(entry_handle_t db_handle) = 0;

    /**
     * This will read in the requested information into a buffer that will remain valid
     * until another read_in call is made.
     * @param db_handle handle of the entry to be read
     * @return pointer to buffer holding the query result, NULL when not found
     */
    virtual SecurityEntrySigning_t* read_in_entry_peer_signing(entry_handle_t db_handle) = 0;

protected:
    SecurityEntryIdentity_t _local_identity;
    csrk_t _local_csrk;
    sign_count_t _local_sign_counter;
};

} /* namespace pal */
} /* namespace ble */

#endif /*GENERIC_SECURITY_MANAGER_DB_H__*/
