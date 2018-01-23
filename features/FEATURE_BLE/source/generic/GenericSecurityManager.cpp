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
#include "Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/generic/GenericSecurityDb.h"

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::advertising_peer_address_type_t;
using ble::pal::irk_t;
using ble::pal::csrk_t;
using ble::pal::ltk_t;
using ble::pal::ediv_t;
using ble::pal::rand_t;
using ble::pal::AuthenticationMask;
using ble::pal::KeyDistribution;
using ble::pairing_failure_t;
using ble::pal::PasskeyAsci;
using ble::pal::passkey_num_t;
typedef SecurityManager::SecurityIOCapabilities_t SecurityIOCapabilities_t;

class GenericSecurityManagerEventHandler;

class GenericSecurityManager : public SecurityManager,
                               public ble::pal::SecurityManagerEventHandler {
public:
    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //
    virtual ble_error_t init(
        bool bondable = true,
        bool mitm = true,
        SecurityIOCapabilities_t iocaps = IO_CAPS_NONE,
        const Passkey_t passkey = NULL,
        bool signing = true
    ) {
        db.restore();
        pal.set_io_capability((io_capability_t::type) iocaps);
        pal.set_display_passkey(PasskeyAsci::to_num(passkey));
        legacy_pairing_allowed = true;

        bool secure_connections;
        pal.get_secure_connections_support(secure_connections);

        default_authentication.set_bondable(bondable);
        default_authentication.set_mitm(mitm);
        default_authentication.set_secure_connections(secure_connections);
        default_authentication.set_keypress_notification(true);

        default_key_distribution.set_signing(signing);
        if (signing) {
            init_signing();
        }

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t reset(void) {
        db.sync();
        SecurityManager::reset();

        return BLE_ERROR_NONE;
    }

    virtual ble_error_t preserveBondingStateOnReset(bool enabled) {
        db.set_restore(enabled);
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t init_signing() {
        /* TODO: store init bit to avoid rerunning needlessly*/
        const csrk_t *pcsrk = db.get_local_csrk();
        if (!pcsrk) {
            csrk_t csrk;
            /* TODO: generate csrk */
            pcsrk = &csrk;
            db.set_local_csrk(*pcsrk);
        }
        pal.set_csrk(*pcsrk);
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    virtual ble_error_t purgeAllBondingState(void) {
        db.clear_entries();
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual ble_error_t requestPairing(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        /* TODO: remove */
        db.get_entry(connection)->master = true;
        /* end temp code */

        if (!legacy_pairing_allowed && !default_authentication.get_secure_connections()) {
            return BLE_ERROR_OPERATION_NOT_PERMITTED;
        }

        AuthenticationMask link_authentication(default_authentication);
        link_authentication.set_mitm(entry->mitm_requested);

        KeyDistribution link_key_distribution(default_key_distribution);
        link_key_distribution.set_signing(entry->signing_requested);
        link_key_distribution.set_encryption(master_sends_keys);

        return pal.send_pairing_request(
            connection,
            entry->oob,
            link_authentication,
            link_key_distribution,
            link_key_distribution
        );
    }

    virtual ble_error_t acceptPairingRequest(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        AuthenticationMask link_authentication(default_authentication);
        link_authentication.set_mitm(entry->mitm_requested);

        KeyDistribution link_key_distribution(default_key_distribution);
        link_key_distribution.set_signing(entry->signing_requested);

        return pal.send_pairing_response(
            connection,
            entry->oob,
            link_authentication,
            link_key_distribution,
            link_key_distribution
        );
    }

    virtual ble_error_t canceltPairingRequest(connection_handle_t connection) {
        return pal.cancel_pairing(connection, pairing_failure_t::UNSPECIFIED_REASON);
    }

    virtual ble_error_t setPairingRequestAuthorisation(bool required = true) {
        pairing_authorisation_required = required;
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t allowLegacyPairing(bool allow = true) {
        legacy_pairing_allowed = allow;
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t getSecureConnectionsSupport(bool *enabled) {
        return pal.get_secure_connections_support(*enabled);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t setIoCapability(SecurityIOCapabilities_t iocaps) {
        return pal.set_io_capability((io_capability_t::type) iocaps);
    }

    virtual ble_error_t setDisplayPasskey(const Passkey_t passkey) {
        return pal.set_display_passkey(PasskeyAsci::to_num(passkey));
    }

    virtual ble_error_t setAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t timeout_in_ms
    ) {
        return pal.set_authentication_timeout(connection, timeout_in_ms / 10);
    }

    virtual ble_error_t getAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t *timeout_in_ms
    ) {
        uint16_t timeout_in_10ms;
        ble_error_t status = pal.get_authentication_timeout(connection, timeout_in_10ms);
        *timeout_in_ms = 10 * timeout_in_10ms;
        return status;
    }

    virtual ble_error_t setLinkSecurity(
        connection_handle_t connection,
        SecurityMode_t securityMode
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
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

    virtual ble_error_t setKeypressNotification(bool enabled = true) {
        default_authentication.set_keypress_notification(enabled);
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t enableSigning(
        connection_handle_t connection,
        bool enabled = true
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
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

    virtual ble_error_t setHintFutureRoleReversal(bool enable = true) {
        master_sends_keys = enable;
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t slave_security_request(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }
        AuthenticationMask link_authentication(default_authentication);
        link_authentication.set_mitm(entry->mitm_requested);
        return pal.slave_security_request(connection, link_authentication);
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
    virtual ble_error_t getLinkEncryption(
        connection_handle_t connection,
        link_encryption_t *encryption
    ) {

        SecurityEntry_t *entry = db.get_entry(connection);
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

    virtual ble_error_t setLinkEncryption(
        connection_handle_t connection,
        link_encryption_t encryption
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
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
                return pal.disable_encryption(connection);
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

    virtual ble_error_t getEncryptionKeySize(
        connection_handle_t connection,
        uint8_t *size
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            *size = entry->encryption_key_size;
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t setEncryptionKeyRequirements(
        uint8_t minimumByteSize,
        uint8_t maximumByteSize
    ) {
        return pal.set_encryption_key_requirements(minimumByteSize, maximumByteSize);
    }

    virtual ble_error_t enable_encryption(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }
        if (entry->master) {
            if (entry->ltk_stored) {
                db.get_entry_peer_keys(
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
    DbCbAction_t enable_encryption_cb(
        SecurityEntry_t& entry,
        SecurityEntryKeys_t& entryKeys
    ) {
        pal.enable_encryption(entry.handle, entryKeys.ltk, entryKeys.ediv, entryKeys.rand);
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t setPrivateAddressTimeout(uint16_t timeout_in_seconds) {
       return pal.set_private_address_timeout(timeout_in_seconds);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual ble_error_t getSigningKey(connection_handle_t connection, bool authenticated) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        if (entry->csrk_stored && (entry->mitm_csrk || !authenticated)) {
            /* we have a key that is either authenticated or we don't care if it is
             * so retrieve it from the db now */
            db.get_entry_csrk(
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

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param entry security entry returned by the database.
     * @param entryKeys security entry containing keys.
     *
     * @return no action instruction to the db since this only reads the keys.
     */
    DbCbAction_t set_ltk_cb(
        SecurityEntry_t& entry,
        SecurityEntryKeys_t& entryKeys
    ) {
        pal.set_ltk(entry.handle, entryKeys.ltk);
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    DbCbAction_t return_csrk_cb(
        connection_handle_t connection,
        csrk_t csrk
    ) {
        _app_event_handler->signingKey(
            connection,
            csrk,
            db.get_entry(connection)->mitm_pairing
        );
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    virtual ble_error_t requestAuthentication(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        if (entry->mitm_pairing) {
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

    virtual ble_error_t setOOBDataUsage(
        connection_handle_t connection,
        bool useOOB,
        bool OOBProvidesMITM = true
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return BLE_ERROR_INVALID_PARAM;
        }

        entry->oob = useOOB;
        entry->oob_mitm_protection = OOBProvidesMITM;
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t confirmationEntered(
        connection_handle_t connection,
        bool confirmation
    ) {
        return pal.confirmation_entered(connection, confirmation);
    }

    virtual ble_error_t passkeyEntered(
        connection_handle_t connection,
        Passkey_t passkey
    ) {
        return pal.passkey_request_reply(
            connection,
            PasskeyAsci::to_num(passkey)
        );
    }

    virtual ble_error_t sendKeypressNotification(
        connection_handle_t connection,
        Keypress_t keypress
    ) {
        return pal.send_keypress_notification(connection, keypress);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Event handler
    //

    virtual void setSecurityManagerEventHandler(
        ::SecurityManager::SecurityManagerEventHandler* handler
     ) {
        SecurityManager::setSecurityManagerEventHandler(handler);
        if (handler) {
            _app_event_handler = handler;
        }
    }

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl)
        : pal(palImpl),
          pairing_authorisation_required(false),
          legacy_pairing_allowed(true),
          master_sends_keys(false),
          default_authentication(0),
          default_key_distribution(KeyDistribution::KEY_DISTRIBUTION_ALL) {
        _app_event_handler = &defaultEventHandler;
        pal.set_event_handler(this);
    }

private:
    ble::pal::SecurityManager& pal;
    GenericSecurityDb db;

    bool pairing_authorisation_required;
    bool legacy_pairing_allowed;
    bool master_sends_keys;

    AuthenticationMask default_authentication;
    KeyDistribution default_key_distribution;

    /*  implements ble::pal::SecurityManagerEventHandler */
public:

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual void on_pairing_request(
        connection_handle_t connection,
        bool use_oob,
        AuthenticationMask authentication,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) {
        /* TODO: remove */
        db.get_entry(connection)->master = false;
        /* end temp code */

        /* cancel pairing if secure connection paring is not possible */
        if (!legacy_pairing_allowed && !authentication.get_secure_connections()) {
            canceltPairingRequest(connection);
        }

        set_mitm_performed(connection, false);

        if (pairing_authorisation_required) {
            _app_event_handler->acceptPairingRequest(connection);
        }
    }

    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    ) {
        set_mitm_performed(connection, false);

        _app_event_handler->pairingResult(
            connection,
            (SecurityManager::SecurityCompletionStatus_t)(error.value() | 0x80)
        );
    }

    virtual void on_pairing_timed_out(connection_handle_t connection) {
        set_mitm_performed(connection, false);

        _app_event_handler->pairingResult(
            connection,
            SecurityManager::SEC_STATUS_TIMEOUT
        );
    }

    virtual void on_pairing_completed(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            if (entry->encryption_requested) {
                enable_encryption(connection);
            }

            /* keys exchanged from now on will share this mitm status */
            entry->mitm_pairing = entry->mitm_performed;
            entry->mitm_performed = false;

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

    virtual void on_valid_mic_timeout(connection_handle_t connection) {
        _app_event_handler->validMicTimeout(connection);
    }

    virtual void on_slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return;
        }

        if (authentication.get_secure_connections()
            && default_authentication.get_secure_connections()
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

    virtual void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    ) {

        SecurityEntry_t *entry = db.get_entry(connection);
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

    virtual void on_link_encryption_request_timed_out(
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
    virtual void set_mitm_performed(connection_handle_t connection, bool enable = true) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            entry->mitm_performed = true;
        }
    }

    virtual void on_passkey_display(
        connection_handle_t connection,
        const passkey_num_t passkey
    ) {
        set_mitm_performed(connection);
        _app_event_handler->passkeyDisplay(connection, PasskeyAsci(passkey).asci);
    }

    virtual void on_keypress_notification(
        connection_handle_t connection,
        SecurityManager::Keypress_t keypress
    ) {
        set_mitm_performed(connection);
        _app_event_handler->keypressNotification(connection, keypress);
    }

    virtual void on_passkey_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->passkeyRequest(connection);
    }

    virtual void on_confirmation_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->confirmationRequest(connection);
    }

    virtual void on_legacy_pairing_oob_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->legacyPairingOobRequest(connection);
    }

    virtual void on_oob_request(connection_handle_t connection) {
        set_mitm_performed(connection);
        _app_event_handler->oobRequest(connection);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual void on_keys_distributed(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return;
        }

        entry->mitm_ltk = entry->mitm_performed;
        entry->mitm_csrk = entry->mitm_performed;

        db.set_entry_peer(
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
            db.get_entry(connection)->mitm_pairing
        );
    }

    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return;
        }
        entry->mitm_ltk = entry->mitm_performed;
        db.set_entry_peer_ltk(connection, ltk);
    }

    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db.set_entry_peer_ediv_rand(connection, ediv, rand);
    }

    virtual void on_keys_distributed_local_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) {
        db.set_entry_local_ltk(connection, ltk);
    }

    virtual void on_keys_distributed_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db.set_entry_local_ediv_rand(connection, ediv, rand);
    }

    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t irk
    ) {
        db.set_entry_peer_irk(connection, irk);
    }

    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    ) {
        db.set_entry_peer_bdaddr(
            connection,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_identity_address
        );
    }

    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    ) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return;
        }

        entry->mitm_csrk = entry->mitm_performed;

        db.set_entry_peer_csrk(connection, csrk);

        _app_event_handler->signingKey(
            connection,
            csrk,
            entry->mitm_csrk
        );
    }

    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) {
        db.get_entry_local_keys(
            mbed::callback(this, &GenericSecurityManager::set_ltk_cb),
            connection,
            ediv,
            rand
        );
    }

    virtual void on_disconnected(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (!entry) {
            return;
        }
        entry->connected = false;
        db.sync();
    }

    virtual void on_connected(connection_handle_t connection, address_t peer_address) {
        SecurityEntry_t *entry = db.connect_entry(connection, peer_address);
        if (!entry) {
            return;
        }

        entry->mitm_requested = false;
        entry->mitm_performed = false;
        entry->mitm_pairing = false;

        entry->connected = true;
        entry->authenticated = false;

        entry->encryption_requested = false;
        entry->encrypted = false;
        entry->signing_requested = false;
    }

private:
    /* handler is always a valid pointer */
    ::SecurityManager::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */
