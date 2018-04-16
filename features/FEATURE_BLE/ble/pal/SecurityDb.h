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

#ifndef PAL_SECURITY_MANAGER_DB_H__
#define PAL_SECURITY_MANAGER_DB_H__

#include "platform/Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/Gap.h"
#include <stdlib.h>

namespace ble {
namespace pal {


/**
 * Security flags associated with a bond.
 */
struct SecurityDistributionFlags_t {
    SecurityDistributionFlags_t() :
        peer_address(),
        encryption_key_size(0),
        peer_address_is_public(false),
        local_address_is_public(false),
        csrk_stored(false),
        csrk_mitm_protected(false),
        ltk_stored(false),
        ltk_mitm_protected(false),
        secure_connections_paired(false) {
    }

    /** peer address */
    address_t peer_address;

    /** encryption key size */
    uint8_t encryption_key_size;
    /** true if peer address is public, false if it's static random */
    uint8_t peer_address_is_public:1;
    /** true if local address is public, false if it's static random */
    uint8_t local_address_is_public:1;

    /** CSRK (Connection Signature Resolving Key) has been distributed and stored */
    uint8_t csrk_stored:1;
    /** CSRK that is stored has MITM protection */
    uint8_t csrk_mitm_protected:1;
    /** LTK (Long Term Key) has been distributed and stored */
    uint8_t ltk_stored:1;
    /** LTK that is stored has MITM protection */
    uint8_t ltk_mitm_protected:1;
    /** the current pairing was done using Secure Connections */
    uint8_t secure_connections_paired:1;
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

/** Data for resolving random resolvable addresses */
struct SecurityEntryIdentity_t {
    /** identity address */
    address_t identity_address;
    /** Identity Resolving Key */
    irk_t irk;
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
    typedef mbed::Callback<void(entry_handle_t, const csrk_t*)>
        SecurityEntryCsrkDbCb_t;
    typedef mbed::Callback<void(::Gap::Whitelist_t*)>
        WhitelistDbCb_t;

    SecurityDb() { };
    virtual ~SecurityDb() { };

    /**
     * Return immediately security flags associated to a db entry.
     *
     * @param[in] db_entry Entry of the database queried.
     * @return pointer to the flags or NULL if the entry do not have any
     * associated flags.
     */
    virtual const SecurityDistributionFlags_t* get_distribution_flags(
        entry_handle_t db_entry
    ) = 0;

    /**
     * Set the distribution flags of a DB entry.
     *
     * @param[in] db_entry Entry of the database that will store the flags.
     * @param[in] flags Distribution flags to store in @p db_entry.
     */
    virtual void set_distribution_flags(
        entry_handle_t db_entry,
        const SecurityDistributionFlags_t& flags
    ) = 0;

    /* local keys */

    /**
     * Retrieve stored LTK based on passed in EDIV and RAND values.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] db_entry handle of the entry being queried.
     * @param[in] ediv one of the values used to identify the LTK
     * @param[in] rand one of the values used to identify the LTK
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_entry,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /**
     * Retrieve stored LTK generated during secure connections pairing.
     *
     * @param[in] cb callback that will receive the LTK struct
     * @param[in] db_entry handle of the entry being queried.
     */
    virtual void get_entry_local_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_entry
    ) = 0;

    /**
     * Save new local LTK for a connection.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] ltk the new LTK, if the device is slave, this is the LTK that
     * will be used when link is encrypted
     */
    virtual void set_entry_local_ltk(
        entry_handle_t db_entry,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_local_ediv_rand(
        entry_handle_t db_entry,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /* peer's keys */

    /**
     * Return asynchronously the peer signing key through a callback
     * so that signed packets can be verified.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] db_entry handle of the entry being queried.
     */
    virtual void get_entry_peer_csrk(
        SecurityEntryCsrkDbCb_t cb,
        entry_handle_t db_entry
    ) = 0;

    /**
     * Return asynchronously the peer encryption key through a callback
     * so that encryption can be enabled.
     *
     * @param[in] cb callback which will receive the key
     * @param[in] db_entry handle of the entry being queried.
     */
    virtual void get_entry_peer_keys(
        SecurityEntryKeysDbCb_t cb,
        entry_handle_t db_entry
    ) = 0;

    /**
     * Save new LTK received from the peer.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] ltk the new LTK, if the peer device is slave, this is the LTK
     * that will be used when link is encrypted
     */
    virtual void set_entry_peer_ltk(
        entry_handle_t db_entry,
        const ltk_t &ltk
    ) = 0;

    /**
     * Update EDIV and RAND used to identify the LTK sent by the peer.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] ediv new EDIV value
     * @param[in] rand new RAND value
     */
    virtual void set_entry_peer_ediv_rand(
        entry_handle_t db_entry,
        const ediv_t &ediv,
        const rand_t &rand
    ) = 0;

    /**
     * Update IRK for this connection.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] irk new IRK value
     */
    virtual void set_entry_peer_irk(
        entry_handle_t db_entry,
        const irk_t &irk
    ) = 0;

    /**
     * Update the identity address of the peer.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] address_is_public is the identity address public or private
     * @param[in] peer_address the new address
     */
    virtual void set_entry_peer_bdaddr(
        entry_handle_t db_entry,
        bool address_is_public,
        const address_t &peer_address
    ) = 0;

    /**
     * Update peer signing key.
     *
     * @param[in] db_entry handle of the entry being updated.
     * @param[in] csrk new CSRK value
     */
    virtual void set_entry_peer_csrk(
        entry_handle_t db_entry,
        const csrk_t &csrk
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
    virtual void set_local_csrk(const csrk_t &csrk) = 0;

    /* public keys */

    /**
     * Return local public key.
     *
     * @return ref to x component of public key
     */
    virtual const public_key_coord_t& get_public_key_x() = 0;

    /**
     * Return local public key.
     *
     * @return ref to y component of public key
     */
    virtual const public_key_coord_t& get_public_key_y() = 0;

    /**
     * Set local public key.
     *
     * @param[in] public_key_x new public key value of the x coordinate
     * @param[in] public_key_y new public key value of the y coordinate
     */
    virtual void set_public_key(
        const public_key_coord_t &public_key_x,
        const public_key_coord_t &public_key_y
    ) = 0;

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
        BLEProtocol::AddressType_t peer_address_type,
        const address_t &peer_address
    ) = 0;

    /**
     * Close a connection entry.
     *
     * @param[in] db_entry this handle will be freed up from the security db.
     */
    virtual void close_entry(entry_handle_t db_entry) = 0;

    /**
     * Remove entry for this peer from NVM.
     *
     * @param[in] peer_identity_address peer address that no longer needs NVM
     * storage.
     */
    virtual void remove_entry(const address_t peer_identity_address) = 0;

    /**
     * Remove all entries from the security DB.
     */
    virtual void clear_entries() = 0;

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
    ) = 0;

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
    ) = 0;

    /**
     * Update the whitelist stored in NVM by replacing it with new one.
     *
     * @param[in] whitelist
     */
    virtual void set_whitelist(const ::Gap::Whitelist_t &whitelist) = 0;

    /**
     * Add a new entry to the whitelist in the NVM.
     *
     * @param[in] address new whitelist entry
     */
    virtual void add_whitelist_entry(const address_t &address) = 0;

    /**
     * Remove whitelist entry from NVM.
     *
     * @param[in] address entry to be removed
     */
    virtual void remove_whitelist_entry(const address_t &address) = 0;

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

} /* namespace pal */
} /* namespace ble */

#endif /*PAL_SECURITY_MANAGER_DB_H__*/
