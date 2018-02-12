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
#if defined(MBEDTLS_CMAC_C)
#include "mbedtls/cmac.h"
#endif

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
    _pal.set_display_passkey(PasskeyAscii::to_num(passkey));
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

    _gap.onConnection(this, &GenericSecurityManager::connection_callback);
    _gap.onDisconnection(this, &GenericSecurityManager::disconnection_callback);

    _pal.generate_public_key();

    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::reset(void) {
    _db.sync();
    _public_keys_generated = false;
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

ble_error_t GenericSecurityManager::generateWhitelistFromBondTable(Gap::Whitelist_t *whitelist) const {
    if (eventHandler) {
        _db.generate_whitelist_from_bond_table(
            mbed::callback(eventHandler, &::SecurityManager::SecurityManagerEventHandler::whitelistFromBondTable),
            whitelist
        );
    }
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

    if (!_legacy_pairing_allowed && !_default_authentication.get_secure_connections()) {
        return BLE_ERROR_OPERATION_NOT_PERMITTED;
    }

    set_mitm_performed(connection, false);
    update_oob_presence(connection);

    AuthenticationMask link_authentication(_default_authentication);
    link_authentication.set_mitm(entry->mitm_requested);

    KeyDistribution link_key_distribution(_default_key_distribution);
    link_key_distribution.set_signing(entry->signing_requested);
    link_key_distribution.set_encryption(_master_sends_keys);

    return _pal.send_pairing_request(
        connection,
        entry->oob_present,
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

    update_oob_presence(connection);

    AuthenticationMask link_authentication(_default_authentication);
    link_authentication.set_mitm(entry->mitm_requested);

    KeyDistribution link_key_distribution(_default_key_distribution);
    link_key_distribution.set_signing(entry->signing_requested);

    return _pal.send_pairing_response(
        connection,
        entry->oob_present,
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
    return _pal.set_display_passkey(PasskeyAscii::to_num(passkey));
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
        if (entry->is_master) {
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

ble_error_t GenericSecurityManager::getLinkEncryption(
    connection_handle_t connection,
    link_encryption_t *encryption
) {

    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return BLE_ERROR_INVALID_PARAM;
    }

    if (entry->encrypted) {
        if (entry->ltk_mitm_protected) {
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

        if (entry->ltk_mitm_protected && !entry->encrypted) {
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

    if (entry->csrk_stored && (entry->csrk_mitm_protected || !authenticated)) {
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
        } else if (entry->is_master) {
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

    if (entry->ltk_mitm_protected) {
        if (entry->authenticated) {
            return BLE_ERROR_NONE;
        } else {
            entry->encryption_requested = true;
            return enable_encryption(connection);
        }
    } else {
        entry->mitm_requested = true;
        if (entry->is_master) {
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

    entry->attempt_oob = useOOB;
    entry->oob_mitm_protection = OOBProvidesMITM;

#if defined(MBEDTLS_CMAC_C)
    if (_public_keys_generated) {
        generate_secure_connections_oob(connection);
    }
#endif

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
        PasskeyAscii::to_num(passkey)
    );
}

ble_error_t GenericSecurityManager::sendKeypressNotification(
    connection_handle_t connection,
    Keypress_t keypress
) {
    return _pal.send_keypress_notification(connection, keypress);
}

ble_error_t GenericSecurityManager::legacyPairingOobReceived(
    const address_t *address,
    const oob_tk_t *tk
) {
    if (address && tk) {
        SecurityEntry_t *entry = _db.get_entry(*address);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        return _pal.legacy_pairing_oob_data_request_reply(entry->handle, *tk);
    }
    return BLE_ERROR_NONE;
}

ble_error_t GenericSecurityManager::oobReceived(
    const address_t *address,
    const oob_rand_t *random,
    const oob_confirm_t *confirm
) {
    if (address && random && confirm) {
        _db.set_peer_sc_oob_data(
            *address,
            *random,
            *confirm
        );

        return BLE_ERROR_NONE;
    }

    return BLE_ERROR_INVALID_PARAM;
}

////////////////////////////////////////////////////////////////////////////
// Helper functions
//

ble_error_t GenericSecurityManager::init_signing() {
    const csrk_t *pcsrk = _db.get_local_csrk();
    if (!pcsrk) {
        csrk_t csrk;

        ble_error_t ret = get_random_data(csrk.buffer(), csrk.size());
        if (ret != BLE_ERROR_NONE) {
            return ret;
        }

        pcsrk = &csrk;
        _db.set_local_csrk(csrk);
    }
    return _pal.set_csrk(*pcsrk);
}

ble_error_t GenericSecurityManager::get_random_data(uint8_t *buffer, size_t size) {
    random_data_t random_data;

    while (size) {
        /* fill out the buffer by reading the random data in chunks
         * and copying it until reaching the set size */
        size_t copy_size = std::max(size, random_data.size());
        ble_error_t ret = _pal.get_random_data(random_data);
        if (ret != BLE_ERROR_NONE) {
            return ret;
        }
        memcpy(buffer, random_data.buffer(), copy_size);
        size -= copy_size;
        buffer += copy_size;
    }

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
    if (entry->is_master) {
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

void GenericSecurityManager::enable_encryption_cb(
    const SecurityEntry_t* entry,
    const SecurityEntryKeys_t* entryKeys
) {
    if (entry && entryKeys) {
        if (entry->secure_connections_paired) {
            _pal.enable_encryption(entry->handle, entryKeys->ltk);
        } else {
            _pal.enable_encryption(entry->handle, entryKeys->ltk, entryKeys->rand, entryKeys->ediv);
        }
    }
}

void GenericSecurityManager::set_ltk_cb(
    const SecurityEntry_t* entry,
    const SecurityEntryKeys_t* entryKeys
) {
    if (entry) {
        if (entryKeys) {
            _pal.set_ltk(entry->handle, entryKeys->ltk);
        } else {
            _pal.set_ltk_not_found(entry->handle);
        }
    }
}

void GenericSecurityManager::return_csrk_cb(
    connection_handle_t connection,
    const csrk_t *csrk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    eventHandler->signingKey(
        connection,
        csrk,
        entry->csrk_mitm_protected
    );
}

#if defined(MBEDTLS_CMAC_C)
void GenericSecurityManager::generate_secure_connections_oob(
    connection_handle_t connection
) {
     oob_confirm_t confirm;
     oob_rand_t random;

     SecurityEntry_t *entry = _db.get_entry(connection);
     if (!entry) {
         return;
     }

     ble_error_t ret = get_random_data(random.buffer(), random.size());
     if (ret != BLE_ERROR_NONE) {
         return;
     }

     crypto_toolbox_f4(
         _db.get_public_key_x(),
         _db.get_public_key_y(),
         random,
         confirm
     );

    eventHandler->oobGenerated(
        &entry->local_address,
        &random,
        &confirm
    );

    _db.set_local_sc_oob_random(random);
}
#endif

void GenericSecurityManager::update_oob_presence(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    /* only update the oob state if we support secure connections,
     * otherwise follow the user set preference for providing legacy
     * pairing oob data */
    entry->oob_present = entry->attempt_oob;

    if (_default_authentication.get_secure_connections()) {
        entry->oob_present = false;
#if defined(MBEDTLS_CMAC_C)
        if (entry->peer_address == _db.get_peer_sc_oob_address()) {
            entry->oob_present = true;
        }
#endif
    }
}

#if defined(MBEDTLS_CMAC_C)
bool GenericSecurityManager::crypto_toolbox_f4(
    const public_key_t& U,
    const public_key_t& V,
    const oob_rand_t& X,
    oob_confirm_t& confirm
) {

    mbedtls_cipher_context_t context;
    const mbedtls_cipher_info_t *info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    const unsigned char Z = 0;
    bool success = false;

    mbedtls_cipher_init(&context);

    /* it's either this chaining or a goto */
    if (mbedtls_cipher_setup(&context, info) == 0
        && mbedtls_cipher_cmac_starts(&context, X.data(), 128) == 0
        && mbedtls_cipher_cmac_update(&context, U.data(), 16) == 0
        && mbedtls_cipher_cmac_update(&context, V.data(), 16) == 0
        && mbedtls_cipher_cmac_update(&context, &Z, 1) == 0
        && mbedtls_cipher_cmac_finish(&context, &confirm[0]) == 0) {
        success = true;
    }

    mbedtls_cipher_free(&context);
    return success;
}
#endif

void GenericSecurityManager::set_mitm_performed(connection_handle_t connection, bool enable) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (entry) {
        entry->mitm_performed = true;
    }
}

void GenericSecurityManager::on_disconnected(connection_handle_t connection) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }
    entry->connected = false;
    _db.sync();
}

void GenericSecurityManager::on_connected(
    connection_handle_t connection,
    bool is_master,
    BLEProtocol::AddressType_t peer_address_type,
    const address_t &peer_address,
    const address_t &local_address
) {
    SecurityEntry_t *entry = _db.connect_entry(
        connection,
        peer_address_type,
        peer_address,
        local_address
    );

    if (!entry) {
        return;
    }

    entry->reset();
    entry->is_master = is_master;
}

void GenericSecurityManager::connection_callback(
    const Gap::ConnectionCallbackParams_t* params
) {
    on_connected(
        params->handle,
        (params->role == Gap::CENTRAL),
        params->peerAddrType,
        address_t(params->peerAddr),
        address_t(params->ownAddr)
    );
}
void GenericSecurityManager::disconnection_callback(
    const Gap::DisconnectionCallbackParams_t* params
) {
    on_disconnected(params->handle);
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
    /* cancel pairing if secure connection paring is not possible */
    if (!_legacy_pairing_allowed && !authentication.get_secure_connections()) {
        canceltPairingRequest(connection);
    }

    set_mitm_performed(connection, false);

    if (_pairing_authorisation_required) {
        eventHandler->pairingRequest(connection);
    }
}

void GenericSecurityManager::on_pairing_error(
    connection_handle_t connection,
    pairing_failure_t error
) {
    set_mitm_performed(connection, false);

    eventHandler->pairingResult(
        connection,
        (SecurityManager::SecurityCompletionStatus_t)(error.value() | 0x80)
    );

    /* if this pairing was triggered by a failed encryption attempt
     * inform the application of the encryption failure */
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (entry && entry->encryption_requested && entry->encryption_failed) {
        eventHandler->linkEncryptionResult(
            connection,
            link_encryption_t::NOT_ENCRYPTED
        );
    }
}

void GenericSecurityManager::on_pairing_timed_out(connection_handle_t connection) {
    set_mitm_performed(connection, false);

    eventHandler->pairingResult(
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
        if (entry->secure_connections_paired) {
            entry->ltk_mitm_protected = entry->mitm_performed;
        }
    }

    eventHandler->pairingResult(
        connection,
        SecurityManager::SEC_STATUS_SUCCESS
    );
}

////////////////////////////////////////////////////////////////////////////
// Security
//

void GenericSecurityManager::on_valid_mic_timeout(connection_handle_t connection) {
    eventHandler->validMicTimeout(connection);
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
        && !entry->secure_connections_paired) {
        requestPairing(connection);
    }

    if (authentication.get_mitm()
        && !entry->ltk_mitm_protected) {
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
        entry->encryption_failed = false;

    } else if (result == link_encryption_t::ENCRYPTED_WITH_MITM) {

        entry->encryption_requested = false;
        entry->encryption_failed = false;
        entry->authenticated = true;

    } else if (result == link_encryption_t::NOT_ENCRYPTED
               && entry->encryption_requested
               && !entry->encryption_failed) {

        /* if we failed encryption for the first time
         * retry repairing in case slave lost LTK */
        requestPairing(entry->handle);
        entry->encryption_failed = true;
        /* don't return an event yet since we are retrying */
        return;
    }

    eventHandler->linkEncryptionResult(connection, result);
}

void GenericSecurityManager::on_link_encryption_request_timed_out(
    connection_handle_t connection
) {
    eventHandler->linkEncryptionResult(
        connection,
        link_encryption_t::NOT_ENCRYPTED
    );
}

////////////////////////////////////////////////////////////////////////////
// MITM
//

void GenericSecurityManager::on_passkey_display(
    connection_handle_t connection,
    passkey_num_t passkey
) {
    set_mitm_performed(connection);
    eventHandler->passkeyDisplay(connection, PasskeyAscii(passkey).value());
}

void GenericSecurityManager::on_keypress_notification(
    connection_handle_t connection,
    SecurityManager::Keypress_t keypress
) {
    set_mitm_performed(connection);
    eventHandler->keypressNotification(connection, keypress);
}

void GenericSecurityManager::on_passkey_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    eventHandler->passkeyRequest(connection);
}

void GenericSecurityManager::on_confirmation_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    eventHandler->confirmationRequest(connection);
}

void GenericSecurityManager::on_legacy_pairing_oob_request(connection_handle_t connection) {
    set_mitm_performed(connection);
    eventHandler->legacyPairingOobRequest(connection);
}

void GenericSecurityManager::on_oob_data_verification_request(
    connection_handle_t connection,
    const public_key_t &peer_public_key_x,
    const public_key_t &peer_public_key_y
) {
#if defined(MBEDTLS_CMAC_C)
    SecurityEntry_t *entry = _db.get_entry(connection);

    oob_confirm_t confirm_verify;

    address_t peer_oob_address;
    oob_rand_t peer_oob_random;
    oob_confirm_t peer_oob_confirm;
    oob_rand_t local_oob_random;
    _db.get_sc_oob_data(
        peer_oob_address,
        peer_oob_random,
        peer_oob_confirm,
        local_oob_random
    );

    crypto_toolbox_f4(
        peer_public_key_x,
        peer_public_key_y,
        peer_oob_random,
        confirm_verify
    );

    if (entry && (entry->peer_address == peer_oob_address)
        && (confirm_verify == peer_oob_confirm)) {
        _pal.oob_data_verified(connection, local_oob_random, peer_oob_random);
    } else {
        _pal.cancel_pairing(connection, pairing_failure_t::CONFIRM_VALUE_FAILED);
    }
#endif
}

////////////////////////////////////////////////////////////////////////////
// Keys
//

void GenericSecurityManager::on_public_key_generated(
    const public_key_t &public_key_x,
    const public_key_t &public_key_y
) {
    _db.set_public_key(public_key_x, public_key_y);
    _public_keys_generated = true;
}

void GenericSecurityManager::on_secure_connections_ltk_generated(
    connection_handle_t connection,
    const ltk_t &ltk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    entry->ltk_mitm_protected = entry->mitm_performed;
    entry->secure_connections_paired = true;

    _db.set_entry_peer_ltk(connection, ltk);
}

void GenericSecurityManager::on_keys_distributed_ltk(
    connection_handle_t connection,
    const ltk_t &ltk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }
    entry->ltk_mitm_protected = entry->mitm_performed;
    _db.set_entry_peer_ltk(connection, ltk);
}

void GenericSecurityManager::on_keys_distributed_ediv_rand(
    connection_handle_t connection,
    const ediv_t &ediv,
    const rand_t &rand
) {
    _db.set_entry_peer_ediv_rand(connection, ediv, rand);
}

void GenericSecurityManager::on_keys_distributed_local_ltk(
    connection_handle_t connection,
    const ltk_t &ltk
) {
    _db.set_entry_local_ltk(connection, ltk);
}

void GenericSecurityManager::on_keys_distributed_local_ediv_rand(
    connection_handle_t connection,
    const ediv_t &ediv,
    const rand_t &rand
) {
    _db.set_entry_local_ediv_rand(connection, ediv, rand);
}

void GenericSecurityManager::on_keys_distributed_irk(
    connection_handle_t connection,
    const irk_t &irk
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
    const csrk_t &csrk
) {
    SecurityEntry_t *entry = _db.get_entry(connection);
    if (!entry) {
        return;
    }

    entry->csrk_mitm_protected = entry->mitm_performed;

    _db.set_entry_peer_csrk(connection, csrk);

    eventHandler->signingKey(
        connection,
        &csrk,
        entry->csrk_mitm_protected
    );
}

void GenericSecurityManager::on_ltk_request(
    connection_handle_t connection,
    const ediv_t &ediv,
    const rand_t &rand
) {
    _db.get_entry_local_keys(
        mbed::callback(this, &GenericSecurityManager::set_ltk_cb),
        connection,
        ediv,
        rand
    );
}

void GenericSecurityManager::on_ltk_request(
    connection_handle_t connection
) {
    _db.get_entry_local_keys(
        mbed::callback(this, &GenericSecurityManager::set_ltk_cb),
        connection
    );
}

} /* namespace generic */
} /* namespace ble */
