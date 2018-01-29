/* mbed Microcontroller Library
 * Copyright (c) 2017-2018 ARM Limited
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

#include "SecurityManager.h"
#include "PalSecurityManager.h"
#include "ble/generic/GenericSecurityManager.h"

namespace ble {
namespace generic {

/* Implements SecurityManager */

////////////////////////////////////////////////////////////////////////////
// SM lifecycle management
//
ble_error_t GenericSecurityManager::init(
    bool bondable,
    bool mitm,
    SecurityIOCapabilities_t iocaps,
    const Passkey_t passkey,
    bool signing
) {
    _db.restore();
    _pal.set_io_capability((io_capability_t::type) iocaps);
    _pal.set_display_passkey(PasskeyAsci::to_num(passkey));
    _legacy_pairing_allowed = true;

    bool secure_connections;
    _pal.get_secure_connections_support(secure_connections);

    _default_authentication.set_bondable(bondable);
    _default_authentication.set_mitm(mitm);
    _default_authentication.set_secure_connections(secure_connections);
    _default_authentication.set_keypress_notification(true);

    _default_key_distribution.set_signing(signing);
    if (signing) {
        init_signing();
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::reset(void) {
    _db.sync();
    SecurityManager::reset();

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::preserveBondingStateOnReset(bool enabled) {
    _db.set_restore(enabled);
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// List management
//

ble_error_t GenericSecurityManager::purgeAllBondingState(void) {
    _db.clear_entries();
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Pairing
//

ble_error_t GenericSecurityManager::requestPairing(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    /* TODO: remove */
    _db.get_entry(connection)->master = true;
    /* end temp code */

    if (!_legacy_pairing_allowed && !_default_authentication.get_secure_connections()) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    set_mitm_performed(connection, false);

    AuthenticationMask link_authentication(_default_authentication);
    link_authentication.set_mitm(entry->mitm_requested);

    KeyDistribution link_key_distribution(_default_key_distribution);
    link_key_distribution.set_signing(entry->signing_requested);
    link_key_distribution.set_encryption(_master_sends_keys);

    return _pal.send_pairing_request(
        connection,
        entry->oob,
        link_authentication,
        link_key_distribution,
        link_key_distribution
    );
}

ble_error_t GenericSecurityManager::acceptPairingRequest(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    AuthenticationMask link_authentication(_default_authentication);
    link_authentication.set_mitm(entry->mitm_requested);

    KeyDistribution link_key_distribution(_default_key_distribution);
    link_key_distribution.set_signing(entry->signing_requested);

    return _pal.send_pairing_response(
        connection,
        entry->oob,
        link_authentication,
        link_key_distribution,
        link_key_distribution
    );
}

ble_error_t GenericSecurityManager::canceltPairingRequest(connection_handle_t connection) {
    return _pal.cancel_pairing(connection, pairing_failure_t::UNSPECIFIED_REASON);
}

ble_error_t GenericSecurityManager::setPairingRequestAuthorisation(bool required) {
    _pairing_authorisation_required = required;
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Feature support
//

ble_error_t GenericSecurityManager::allowLegacyPairing(bool allow) {
    _legacy_pairing_allowed = allow;
    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::getSecureConnectionsSupport(bool *enabled) {
    return _pal.get_secure_connections_support(*enabled);
}

////////////////////////////////////////////////////////////////////////////
// Security settings
//

ble_error_t GenericSecurityManager::setIoCapability(SecurityIOCapabilities_t iocaps) {
    return _pal.set_io_capability((io_capability_t::type) iocaps);
}

ble_error_t GenericSecurityManager::setDisplayPasskey(const Passkey_t passkey) {
    return _pal.set_display_passkey(PasskeyAsci::to_num(passkey));
}

ble_error_t GenericSecurityManager::setAuthenticationTimeout(
    connection_handle_t connection,
    uint32_t timeout_in_ms
) {
    return _pal.set_authentication_timeout(connection, timeout_in_ms / 10);
}

ble_error_t GenericSecurityManager::getAuthenticationTimeout(
    connection_handle_t connection,
    uint32_t *timeout_in_ms
) {
    uint16_t timeout_in_10ms;
    ble_error_t status = _pal.get_authentication_timeout(connection, timeout_in_10ms);
    *timeout_in_ms = 10 * timeout_in_10ms;
    return status;
}

ble_error_t GenericSecurityManager::setLinkSecurity(
    connection_handle_t connection,
    SecurityMode_t securityMode
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (entry->encryption_requested) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    switch (securityMode) {
        case SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            return setLinkEncryption(connection, link_encryption_t::NOT_ENCRYPTED);

        case SECURITY_MODE_ENCRYPTION_NO_MITM:
            return setLinkEncryption(connection, link_encryption_t::ENCRYPTED);

        case SECURITY_MODE_ENCRYPTION_WITH_MITM:
            return setLinkEncryption(connection, link_encryption_t::ENCRYPTED_WITH_MITM);

        case SECURITY_MODE_SIGNED_NO_MITM:
            return getSigningKey(connection, false);

        case SECURITY_MODE_SIGNED_WITH_MITM:
            return getSigningKey(connection, true);

        default:
            return BLE_ERROR_INVALID_PARAM;
    }
}

ble_error_t GenericSecurityManager::setKeypressNotification(bool enabled) {
    _default_authentication.set_keypress_notification(enabled);
    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::enableSigning(
    connection_handle_t connection,
    bool enabled
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    entry->signing_requested = enabled;

    if (entry->encrypted) {
        return BLE_ERROR_INVALID_STATE;
    }
    if (!entry->csrk_stored && entry->signing_requested) {
        init_signing();
        if (entry->master) {
            return requestPairing(connection);
        } else {
            return slave_security_request(connection);
        }
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::setHintFutureRoleReversal(bool enable) {
    _master_sends_keys = enable;
    return BLE_ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

/**
 * Get the security status of a connection.
 *
 * @param[in]  connection   Handle to identify the connection.
 * @param[out] securityStatusP    Security status.
 *
 * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
 */
ble_error_t GenericSecurityManager::getLinkEncryption(
    connection_handle_t connection,
    link_encryption_t *encryption
) {

    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (entry->encrypted) {
        if (entry->mitm_ltk) {
            *encryption = link_encryption_t::ENCRYPTED_WITH_MITM;
        } else {
            *encryption = link_encryption_t::ENCRYPTED;
        }
    } else if (entry->encryption_requested) {
        *encryption = link_encryption_t::ENCRYPTION_IN_PROGRESS;
    } else {
        *encryption = link_encryption_t::NOT_ENCRYPTED;
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::setLinkEncryption(
    connection_handle_t connection,
    link_encryption_t encryption
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    link_encryption_t current_encryption(link_encryption_t::NOT_ENCRYPTED);
    getLinkEncryption(connection, &current_encryption);

    if (current_encryption == link_encryption_t::ENCRYPTION_IN_PROGRESS) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    /* ignore if the link is already at required state*/
    if (current_encryption == encryption) {
        return BLE_ERROR_NONE;
    }

    if (encryption == link_encryption_t::NOT_ENCRYPTED) {

        if (entry->encrypted) {
            return _pal.disable_encryption(connection);
        }

    } else if (encryption == link_encryption_t::ENCRYPTED) {

        /* if already better than encrypted don't bother */
        if (current_encryption == link_encryption_t::ENCRYPTED_WITH_MITM) {
            return BLE_ERROR_NONE;
        }
        entry->encryption_requested = true;
        return enable_encryption(connection);

    } else if (encryption == link_encryption_t::ENCRYPTED_WITH_MITM) {

        if (entry->mitm_ltk && !entry->encrypted) {
            entry->encryption_requested = true;
            return enable_encryption(connection);
        } else {
            entry->encryption_requested = true;
            return requestAuthentication(connection);
        }

    } else {
        return BLE_ERROR_INVALID_PARAM;
    }

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::getEncryptionKeySize(
    connection_handle_t connection,
    uint8_t *size
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (entry) {
        *size = entry->encryption_key_size;
        return BLE_ERROR_NONE;
    } else {
        return BLE_ERROR_INVALID_PARAM;
    }
}

ble_error_t GenericSecurityManager::setEncryptionKeyRequirements(
    uint8_t minimumByteSize,
    uint8_t maximumByteSize
) {
    return _pal.set_encryption_key_requirements(minimumByteSize, maximumByteSize);
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

ble_error_t GenericSecurityManager::getSigningKey(connection_handle_t connection, bool authenticated) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (entry->csrk_stored && (entry->mitm_csrk || !authenticated)) {
        /* we have a key that is either authenticated or we don't care if it is
         * so retrieve it from the db now */
        _db.get_entry_peer_csrk(
            mbed::callback(this, &GenericSecurityManager::return_csrk_cb),
            connection
        );
        return BLE_ERROR_NONE;

    } else {
        /* we don't have the right key so we need to get it first
         * keys exchange will create the signingKey event */
        if (authenticated) {
            return requestAuthentication(connection);
        } else if (entry->master) {
            return requestPairing(connection);
        } else {
            return slave_security_request(connection);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
// Privacy
//

ble_error_t GenericSecurityManager::setPrivateAddressTimeout(uint16_t timeout_in_seconds) {
   return _pal.set_private_address_timeout(timeout_in_seconds);
}

////////////////////////////////////////////////////////////////////////////
// Authentication
//

ble_error_t GenericSecurityManager::requestAuthentication(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (entry->mitm_ltk) {
        if (entry->authenticated) {
            return BLE_ERROR_NONE;
        } else {
            entry->encryption_requested = true;
            return enable_encryption(connection);
        }
    } else {
        entry->mitm_requested = true;
        if (entry->master) {
            return requestPairing(connection);
        } else {
            return slave_security_request(connection);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

ble_error_t GenericSecurityManager::setOOBDataUsage(
    connection_handle_t connection,
    bool useOOB,
    bool OOBProvidesMITM
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    entry->oob = useOOB;
    entry->oob_mitm_protection = OOBProvidesMITM;
    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::confirmationEntered(
    connection_handle_t connection,
    bool confirmation
) {
    return _pal.confirmation_entered(connection, confirmation);
}

ble_error_t GenericSecurityManager::passkeyEntered(
    connection_handle_t connection,
    Passkey_t passkey
) {
    return _pal.passkey_request_reply(
        connection,
        PasskeyAsci::to_num(passkey)
    );
}

ble_error_t GenericSecurityManager::sendKeypressNotification(
    connection_handle_t connection,
    Keypress_t keypress
) {
    return _pal.send_keypress_notification(connection, keypress);
}

////////////////////////////////////////////////////////////////////////////
// Event handler
//

void GenericSecurityManager::setSecurityManagerEventHandler(
    ::SecurityManager::SecurityManagerEventHandler* handler
 ) {
    SecurityManager::setSecurityManagerEventHandler(handler);
    if (handler) {
        _app_event_handler = handler;
    }
}

////////////////////////////////////////////////////////////////////////////
// Helper functions
//

ble_error_t GenericSecurityManager::init_signing() {
    /* TODO: store init bit to avoid rerunning needlessly*/
    const csrk_t *pcsrk = _db.get_local_csrk();
    if (!pcsrk) {
        csrk_t csrk;
        /* TODO: generate csrk */
        pcsrk = &csrk;
        _db.set_local_csrk(pcsrk);
    }
    _pal.set_csrk(pcsrk);
    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::slave_security_request(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }
    AuthenticationMask link_authentication(_default_authentication);
    link_authentication.set_mitm(entry->mitm_requested);
    return _pal.slave_security_request(connection, link_authentication);
}

ble_error_t GenericSecurityManager::enable_encryption(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }
    if (entry->master) {
        if (entry->ltk_stored) {
            _db.get_entry_peer_keys(
                mbed::callback(this, &GenericSecurityManager::enable_encryption_cb),
                connection
            );
            return BLE_ERROR_NONE;
        } else {
            return requestPairing(connection);
        }
    } else {
        return slave_security_request(connection);
    }
}

/**
 * Returns the requested LTK to the PAL. Called by the security db.
 *
 * @param entry security entry returned by the database.
 * @param entryKeys security entry containing keys.
 *
 * @return no action instruction to the db since this only reads the keys.
 */
DbCbAction_t GenericSecurityManager::enable_encryption_cb(
    SecurityEntry_t& entry,
    SecurityEntryKeys_t& entryKeys
) {
    _pal.enable_encryption(entry.handle, &entryKeys.ltk, &entryKeys.rand, &entryKeys.ediv);
    return DB_CB_ACTION_NO_UPDATE_REQUIRED;
}

void GenericSecurityManager::check_against_irk_cb(
    const irk_t *irk
) {

}

bool GenericSecurityManager::check_against_identity_address(
    const address_t peer_address,
    const irk_t *irk
) {
    /* we need to verify the identity by encrypting the
     * PRAND part with the IRK key and checking the result
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 2.2.2 */
    octet_type_t<6> prand_hash(peer_address.data(), 6);

    /* remove the hash and leave only prand */
    prand_hash[3] = 0;
    prand_hash[4] = 0;
    prand_hash[5] = 0;

    _pal.encrypt_data(irk, prand_hash.data());

    /* prand_hash now contains the hash result in the first 3 octects
     * compare it with the hash in the peer identity address */

    /* can't use memcmp because of address_t constness */
    if ((prand_hash[0] == peer_address[3])
        || (prand_hash[1] == peer_address[4])
        || (prand_hash[2] == peer_address[5])) {
        return true;
    }
    return false;
}

/**
 * Returns the requested LTK to the PAL. Called by the security db.
 *
 * @param entry security entry returned by the database.
 * @param entryKeys security entry containing keys.
 *
 * @return no action instruction to the db since this only reads the keys.
 */
DbCbAction_t GenericSecurityManager::set_ltk_cb(
    SecurityEntry_t& entry,
    SecurityEntryKeys_t& entryKeys
) {
    _pal.set_ltk(entry.handle, &entryKeys.ltk);
    return DB_CB_ACTION_NO_UPDATE_REQUIRED;
}

void GenericSecurityManager::return_csrk_cb(
    connection_handle_t connection,
    const csrk_t *csrk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    _app_event_handler->signingKey(
        connection,
        csrk,
        entry->mitm_csrk
    );
}

/* Implements ble::pal::SecurityManagerEventHandler */

////////////////////////////////////////////////////////////////////////////
// Pairing
//

void GenericSecurityManager::on_pairing_request(
    connection_handle_t connection,
    bool use_oob,
    AuthenticationMask authentication,
    KeyDistribution initiator_dist,
    KeyDistribution responder_dist
) {
    /* TODO: remove */
    _db.get_entry(connection)->master = false;
    /* end temp code */

    /* cancel pairing if secure connection paring is not possible */
    if (!_legacy_pairing_allowed && !authentication.get_secure_connections()) {
        canceltPairingRequest(connection);
    }

    set_mitm_performed(connection, false);

    if (_pairing_authorisation_required) {
        _app_event_handler->acceptPairingRequest(connection);
    }
}

void GenericSecurityManager::on_pairing_error(
    connection_handle_t connection,
    pairing_failure_t error
) {
    set_mitm_performed(connection, false);

    _app_event_handler->pairingResult(
        connection,
        (SecurityManager::SecurityCompletionStatus_t)(error.value() | 0x80)
    );
}

void GenericSecurityManager::on_pairing_timed_out(connection_handle_t connection) {
    set_mitm_performed(connection, false);

    _app_event_handler->pairingResult(
        connection,
        SecurityManager::SEC_STATUS_TIMEOUT
    );
}

void GenericSecurityManager::on_pairing_completed(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (entry) {
        if (entry->encryption_requested) {
            enable_encryption(connection);
        }

        /* sc doesn't need to exchange ltk */
        if (entry->secure_connections) {
            entry->mitm_ltk = entry->mitm_performed;
        }
    }

    _app_event_handler->pairingResult(
        connection,
        SecurityManager::SEC_STATUS_SUCCESS
    );
}

////////////////////////////////////////////////////////////////////////////
// Security
//

void GenericSecurityManager::on_valid_mic_timeout(connection_handle_t connection) {
    _app_event_handler->validMicTimeout(connection);
}

void GenericSecurityManager::on_slave_security_request(
    connection_handle_t connection,
    AuthenticationMask authentication
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    if (authentication.get_secure_connections()
        && _default_authentication.get_secure_connections()
        && !entry->secure_connections) {
        requestPairing(connection);
    }

    if (authentication.get_mitm()
        && !entry->mitm_ltk) {
        entry->mitm_requested = true;
        requestPairing(connection);
    }
}

////////////////////////////////////////////////////////////////////////////
// Encryption
//

void GenericSecurityManager::on_link_encryption_result(
    connection_handle_t connection,
    link_encryption_t result
) {

    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    if (result == link_encryption_t::ENCRYPTED) {
       entry->encryption_requested = false;
    }

    if (result == link_encryption_t::ENCRYPTED_WITH_MITM) {
        entry->encryption_requested = false;
        entry->authenticated = true;
    }

    _app_event_handler->linkEncryptionResult(connection, result);
}

void GenericSecurityManager::on_link_encryption_request_timed_out(
    connection_handle_t connection
) {
    _app_event_handler->linkEncryptionResult(
        connection,
        link_encryption_t::NOT_ENCRYPTED
    );
}

////////////////////////////////////////////////////////////////////////////
// MITM
//
void GenericSecurityManager::set_mitm_performed(connection_handle_t connection, bool enable) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (entry) {
        entry->mitm_performed = true;
    }
}

void GenericSecurityManager::on_passkey_display(
    connection_handle_t connection,
    passkey_num_t passkey
) {
    set_mitm_performed(connection);
    _app_event_handler->passkeyDisplay(connection, PasskeyAsci(passkey).asci);
}

void GenericSecurityManager::on_keypress_notification(
    connection_handle_t connection,
    SecurityManager::Keypress_t keypress
) {
    set_mitm_performed(connection);
    _app_event_handler->keypressNotification(connection, keypress);
}

void GenericSecurityManager::on_passkey_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    _app_event_handler->passkeyRequest(connection);
}

void GenericSecurityManager::on_confirmation_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    _app_event_handler->confirmationRequest(connection);
}

void GenericSecurityManager::on_legacy_pairing_oob_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    _app_event_handler->legacyPairingOobRequest(connection);
}

void GenericSecurityManager::on_oob_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    _app_event_handler->oobRequest(connection);
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

void GenericSecurityManager::on_keys_distributed(
    connection_handle_t connection,
    advertising_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address,
    const ediv_t *ediv,
    const rand_t *rand,
    const ltk_t *ltk,
    const irk_t *irk,
    const csrk_t *csrk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    entry->mitm_ltk = entry->mitm_performed;
    entry->mitm_csrk = entry->mitm_performed;

    _db.set_entry_peer(
        connection,
        (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
        peer_identity_address,
        ediv,
        rand,
        ltk,
        irk,
        csrk
    );

    _app_event_handler->signingKey(
        connection,
        csrk,
        entry->mitm_csrk
    );
}

void GenericSecurityManager::on_keys_distributed_ltk(
    connection_handle_t connection,
    const ltk_t *ltk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }
    entry->mitm_ltk = entry->mitm_performed;
    _db.set_entry_peer_ltk(connection, ltk);
}

void GenericSecurityManager::on_keys_distributed_ediv_rand(
    connection_handle_t connection,
    const ediv_t *ediv,
    const rand_t *rand
) {
    _db.set_entry_peer_ediv_rand(connection, ediv, rand);
}

void GenericSecurityManager::on_keys_distributed_local_ltk(
    connection_handle_t connection,
    const ltk_t *ltk
) {
    _db.set_entry_local_ltk(connection, ltk);
}

void GenericSecurityManager::on_keys_distributed_local_ediv_rand(
    connection_handle_t connection,
    const ediv_t *ediv,
    const rand_t *rand
) {
    _db.set_entry_local_ediv_rand(connection, ediv, rand);
}

void GenericSecurityManager::on_keys_distributed_irk(
    connection_handle_t connection,
    const irk_t *irk
) {
    _db.set_entry_peer_irk(connection, irk);
}

void GenericSecurityManager::on_keys_distributed_bdaddr(
    connection_handle_t connection,
    advertising_peer_address_type_t peer_address_type,
    const address_t &peer_identity_address
) {
    _db.set_entry_peer_bdaddr(
        connection,
        (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
        peer_identity_address
    );
}

void GenericSecurityManager::on_keys_distributed_csrk(
    connection_handle_t connection,
    const csrk_t *csrk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    entry->mitm_csrk = entry->mitm_performed;

    _db.set_entry_peer_csrk(connection, csrk);

    _app_event_handler->signingKey(
        connection,
        csrk,
        entry->mitm_csrk
    );
}

void GenericSecurityManager::on_ltk_request(
    connection_handle_t connection,
    const ediv_t *ediv,
    const rand_t *rand
) {
    _db.get_entry_local_keys(
        mbed::callback(this, &GenericSecurityManager::set_ltk_cb),
        connection,
        ediv,
        rand
    );
}

void GenericSecurityManager::on_disconnected(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }
    entry->connected = false;
    _db.sync();
}

void GenericSecurityManager::on_connected(connection_handle_t connection, address_t peer_address, bool is_master) {
    SecurityEntry_t *entry = NULL;

    /* if it's a resolvable address, check against IRKs */
    if ((peer_address[0] & 0x3) == 0x2) {
        IdentytList_t *list = NULL;
        if (_db.get_identity_list(list) == BLE_ERROR_NONE && list) {
            for (size_t i; i < list->size; ++i) {
                /* if the address resolves connect to an existing entry based on the identity address */
                if (check_against_identity_address(peer_address, &list->identities[i].irk)) {
                    entry = _db.connect_entry(connection, list->identities[i].peer_identity_address);
                }
            }
        }
    }

    entry = _db.connect_entry(connection, peer_address);
    if (!entry) {
        return;
    }

    entry->reset();
    entry->master = is_master;
}


} /* namespace generic */
} /* namespace ble */
