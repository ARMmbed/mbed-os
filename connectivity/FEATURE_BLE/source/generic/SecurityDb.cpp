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

#include "SecurityDb.h"

#include "mbed-trace/mbed_trace.h"
#include "common/ble_trace_helpers.h"

#define TRACE_GROUP "BLDB"

namespace ble {

SecurityDb::SecurityDb() : _local_sign_counter(0) { };

void SecurityDb::set_distribution_flags(
    entry_handle_t db_handle,
    const SecurityDistributionFlags_t& new_flags
)
{
    SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
    if (flags) {
        *flags = new_flags;
    }
}

void SecurityDb::get_entry_local_keys(
    SecurityEntryKeysDbCb_t cb,
    entry_handle_t* db_handle,
    const ediv_t &ediv,
    const rand_t &rand
)
{
    SecurityEntryKeys_t* keys = read_in_entry_local_keys(*db_handle);
    /* validate we have the correct key */
    if (keys && ediv == keys->ediv && rand == keys->rand) {
        cb(*db_handle, keys);
    } else {
        // Maybe this isn't the correct entry, try to find one that matches
        entry_handle_t correct_handle = find_entry_by_peer_ediv_rand(ediv, rand);
        if (!correct_handle) {
            tr_warn("Failed to find ltk matching given ediv&rand");
            cb(*db_handle, NULL);
            return;
        }
        tr_warn("Found ltk matching given ediv&rand but it belonged to a different identity, update entry with new identity");
        // Note: keys should never be null as a matching entry has been retrieved
        SecurityEntryKeys_t* keys = read_in_entry_local_keys(correct_handle);
        MBED_ASSERT(keys);

        /* set flags connected */
        SecurityDistributionFlags_t* flags = get_distribution_flags(correct_handle);
        flags->connected = true;

        /* update peer address */
        SecurityDistributionFlags_t* old_flags = get_distribution_flags(*db_handle);
        flags->peer_address = old_flags->peer_address;
        flags->peer_address_is_public = old_flags->peer_address_is_public;

        close_entry(*db_handle, false);
        *db_handle = correct_handle;
        cb(*db_handle, keys);
    }
}

void SecurityDb::get_entry_local_keys(
    SecurityEntryKeysDbCb_t cb,
    entry_handle_t db_handle
)
{
    SecurityEntryKeys_t* keys = read_in_entry_local_keys(db_handle);
    SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
    /* validate we have the correct key */
    if (flags && keys && flags->secure_connections_paired) {
        cb(db_handle, keys);
    } else {
        cb(db_handle, NULL);
    }
}

void SecurityDb::get_entry_peer_csrk(
    SecurityEntrySigningDbCb_t cb,
    entry_handle_t db_handle
)
{
    SecurityEntrySigning_t* signing = read_in_entry_peer_signing(db_handle);
    cb(db_handle, signing);
}

void SecurityDb::get_entry_peer_keys(
    SecurityEntryKeysDbCb_t cb,
    entry_handle_t db_handle
)
{
    SecurityEntryKeys_t* keys = read_in_entry_peer_keys(db_handle);
    cb(db_handle, keys);
}

void SecurityDb::get_entry_identity(
    SecurityEntryIdentityDbCb_t cb,
    entry_handle_t db_handle
)
{
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

void SecurityDb::get_identity_list(
    IdentitylistDbCb_t cb,
    Span<SecurityEntryIdentity_t>& identity_list
)
{
    size_t count = 0;
    for (size_t i = 0; i < get_entry_count() && count < (size_t) identity_list.size(); ++i) {

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

const csrk_t *SecurityDb::get_local_csrk()
{
    return &_local_csrk;
}

sign_count_t SecurityDb::get_local_sign_counter()
{
    return _local_sign_counter;
}

void SecurityDb::set_local_csrk(const csrk_t &csrk)
{
    _local_csrk = csrk;
}

void SecurityDb::set_local_sign_counter(
    sign_count_t sign_counter
)
{
    _local_sign_counter = sign_counter;
}

void SecurityDb::set_local_identity(
    const irk_t &irk,
    const address_t &identity_address,
    bool public_address
)
{
    _local_identity.irk = irk;
    _local_identity.identity_address = identity_address;
    _local_identity.identity_address_is_public = public_address;
}

irk_t SecurityDb::get_local_irk()
{
    return _local_identity.irk;
}


const address_t &SecurityDb::get_local_identity_address()
{
    return _local_identity.identity_address;
}

bool SecurityDb::is_local_identity_address_public()
{
    return _local_identity.identity_address_is_public;
}

SecurityDb::entry_handle_t SecurityDb::open_entry(
    peer_address_type_t peer_address_type,
    const address_t &peer_address
)
{
    entry_handle_t db_handle = find_entry_by_peer_address(peer_address_type, peer_address);
    if (db_handle) {
        tr_debug("Found old DB entry (connected to peer previously)");
        ((SecurityDistributionFlags_t*)db_handle)->connected = true;
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
        flags->connected = true;
        return flags;
    }

    return nullptr;
}

SecurityDb::entry_handle_t SecurityDb::find_entry_by_peer_address(
    peer_address_type_t peer_address_type,
    const address_t &peer_address
)
{
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

    return nullptr;
}

SecurityDb::entry_handle_t SecurityDb::find_entry_by_peer_ediv_rand(
    const ediv_t &ediv,
    const rand_t &rand
)
{
    for (size_t i = 0; i < get_entry_count(); i++) {
        entry_handle_t db_handle = get_entry_handle_by_index(i);
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);

        if (!flags || flags->connected) {
            continue;
        }

        SecurityEntryKeys_t* keys = read_in_entry_local_keys(db_handle);
        if (!keys) {
            continue;
        }

        if (keys->ediv == ediv && keys->rand == rand) {
            return db_handle;
        }
    }

    return nullptr;
}

void SecurityDb::close_entry(entry_handle_t db_handle, bool require_sync) {
    SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);
    if (flags) {
        flags->connected = false;
    }
    if (require_sync) {
        sync(db_handle);
    }
}

void SecurityDb::remove_entry(
    peer_address_type_t peer_address_type,
    const address_t &peer_address
) {
    tr_info("Clearing entry for address %s", to_string(peer_address));
    entry_handle_t db_handle = find_entry_by_peer_address(peer_address_type, peer_address);
    if (db_handle) {
        reset_entry(db_handle);
    }
}

void SecurityDb::clear_entries()
{
    tr_info("Clearing all entries");
    for (size_t i = 0; i < get_entry_count(); i++) {
        entry_handle_t db_handle = get_entry_handle_by_index(i);
        reset_entry(db_handle);
    }
    _local_identity = SecurityEntryIdentity_t();
    _local_csrk = csrk_t();
}

void SecurityDb::get_whitelist(
    WhitelistDbCb_t cb,
    ::ble::whitelist_t *whitelist
) {
    /*TODO: fill whitelist*/
    cb(whitelist);
}

void SecurityDb::generate_whitelist_from_bond_table(
    WhitelistDbCb_t cb,
    ::ble::whitelist_t *whitelist
) {
    for (size_t i = 0; i < get_entry_count() && whitelist->size < whitelist->capacity; i++) {
        entry_handle_t db_handle = get_entry_handle_by_index(i);
        SecurityDistributionFlags_t* flags = get_distribution_flags(db_handle);

        if (!flags || !flags->irk_stored) {
            continue;
        }

        // Add the connection address
        whitelist->addresses[whitelist->size].address = flags->peer_address.data();

        if (flags->peer_address_is_public) {
            whitelist->addresses[whitelist->size].type = peer_address_type_t::PUBLIC;
        } else {
            whitelist->addresses[whitelist->size].type = peer_address_type_t::RANDOM;
        }

        whitelist->size++;
        if (whitelist->size == whitelist->capacity) {
            break;
        }

        // Add the identity address
        SecurityEntryIdentity_t* identity = read_in_entry_peer_identity(db_handle);
        if (!identity) {
            continue;
        }

        whitelist->addresses[whitelist->size].address = identity->identity_address;

        if (identity->identity_address_is_public) {
            whitelist->addresses[whitelist->size].type = peer_address_type_t::PUBLIC_IDENTITY;
        } else {
            whitelist->addresses[whitelist->size].type = peer_address_type_t::RANDOM_STATIC_IDENTITY;
        }

        whitelist->size++;
    }

    cb(whitelist);
}

void SecurityDb::set_whitelist(const ::ble::whitelist_t &whitelist) { };

void SecurityDb::add_whitelist_entry(const address_t &address) { };

void SecurityDb::remove_whitelist_entry(const address_t &address) { };

void SecurityDb::clear_whitelist() { };

void SecurityDb::restore() { };

void SecurityDb::sync(entry_handle_t db_handle) { };

void SecurityDb::set_restore(bool reload) { };

SecurityDistributionFlags_t* SecurityDb::get_free_entry_flags() {
    /* get a free one if available */
    tr_debug("Retrieve a disconnected entry to use as a new entry in DB.");
    SecurityDistributionFlags_t* match = nullptr;
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
                tr_debug("Using a previously unused entry as a new entry in DB.");
                break;
            }
        }
    }

    if (match) {
        reset_entry(match);
    }

    return match;
}

} /* namespace ble */