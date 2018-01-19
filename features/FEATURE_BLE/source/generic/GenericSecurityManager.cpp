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

#ifndef __GENERIC_SECURITY_MANAGER_H__
#define __GENERIC_SECURITY_MANAGER_H__

#include "SecurityManager.h"
#include "PalSecurityManager.h"
#include "Callback.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"

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

/* separate structs to allow db implementation to minimise memory usage */

struct SecurityEntry_t {
    connection_handle_t handle;
    address_t peer_identity_address;
    uint8_t encryption_key_size;
    uint8_t peer_address_public:1;
    uint8_t mitm:1; /**< does the key provide mitm */
    uint8_t connected:1;
    uint8_t authenticated:1; /**< have we authenticated during this connection */
    uint8_t sign_data:1;
    uint8_t encryption_requested:1;
    uint8_t encrypted:1;
    uint8_t oob:1;
    uint8_t oob_mitm_protection:1;
    uint8_t secure_connections:1;
};

struct SecurityEntryKeys_t {
    ltk_t  ltk;
    ediv_t ediv;
    rand_t rand;
};

struct SecurityEntryIdentity_t {
    irk_t  irk;
    csrk_t csrk;
};

enum DbCbAction_t {
    DB_CB_ACTION_UPDATE,
    DB_CB_ACTION_NO_UPDATE_REQUIRED, /* does not guarantee discarding changes if you made any */
    DB_CB_ACTION_REMOVE
};

typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&)> SecurityEntryDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryKeys_t&)> SecurityEntryKeysDbCb_t;
typedef mbed::Callback<DbCbAction_t(SecurityEntry_t&, SecurityEntryIdentity_t&)> SecurityEntryIdentityDbCb_t;
typedef mbed::Callback<DbCbAction_t(Gap::Whitelist_t&)> WhitelistDbCb_t;

class GenericSecurityManagerEventHandler;

/**
 * SecurityDB holds the state for active connections and bonded devices.
 * Keys can be stored in NVM and are returned via callbacks.
 * SecurityDB is responsible for serialising any requests and keeping
 * the store in a consistent state.
 * Active connections state must be returned immediately.
 */
class SecurityDb {
public:
    SecurityDb() {};
    ~SecurityDb() {};

    /**
     * Return immediately security entry containing the state
     * information for active connection.
     * @param[in] handle valid connection handle
     * @return pointer to security entry, NULL if handle was invalid
     */
    SecurityEntry_t* get_entry(connection_handle_t connection);

    void get_entry_keys(SecurityEntryKeysDbCb_t cb, const ediv_t ediv, const rand_t rand);
    void get_entry_identityt(SecurityEntryIdentityDbCb_t cb, address_t &identity_address);

    void update_entry(connection_handle_t connection,
                      bool address_is_public,
                      const address_t &peer_address,
                      const ediv_t ediv,
                      const rand_t rand,
                      const ltk_t ltk,
                      const irk_t irk,
                      const csrk_t csrk);
    void update_entry_ltk(connection_handle_t connection,
                          const ltk_t ltk);
    void update_entry_ediv_rand(connection_handle_t connection,
                                const ediv_t ediv,
                                const rand_t rand);
    void update_entry_irk(connection_handle_t connection,
                          const irk_t irk);
    void update_entry_bdaddr(connection_handle_t connection,
                             bool address_is_public,
                             const address_t &peer_address);
    void update_entry_csrk(connection_handle_t connection,
                           const csrk_t csrk);

    void remove_entry(SecurityEntry_t&);
    void clear_entries();

    void get_whitelist(WhitelistDbCb_t cb);

    void update_whitelist(Gap::Whitelist_t& whitelist);
    void add_whitelist_entry(const address_t &address);

    void remove_whitelist_entry(const address_t &address);
    void clear_whitelist();

    void restore();
    void sync();
    void set_restore(bool reload);
private:

};

class GenericSecurityManager : public SecurityManager,
                               public ble::pal::SecurityManagerEventHandler {
public:
    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //
    ble_error_t init(bool                     bondable = true,
                     bool                     mitm     = true,
                     SecurityIOCapabilities_t iocaps   = IO_CAPS_NONE,
                     const Passkey_t          passkey  = NULL) {
        db.restore();
        pal.set_io_capability((io_capability_t::type) iocaps);
        pal.set_display_passkey(PasskeyAsci::to_num(passkey));
        legacy_pairing_allowed = true;

        bool secure_connections;
        pal.get_secure_connections_support(secure_connections);

        authentication.set_bondable(bondable);
        authentication.set_mitm(mitm);
        authentication.set_secure_connections(secure_connections);
        authentication.set_keypress_notification(true);

        return BLE_ERROR_NONE;
    }

    ble_error_t reset(void) {
        db.sync();
        SecurityManager::reset();

        return BLE_ERROR_NONE;
    }

    ble_error_t preserveBondingStateOnReset(bool enabled) {
        db.set_restore(enabled);
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    ble_error_t purgeAllBondingState(void) {
        db.clear_entries();
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    ble_error_t requestPairing(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            return pal.send_pairing_request(
                connection,
                entry->oob,
                authentication,
                initiator_dist,
                responder_dist
            );
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    ble_error_t acceptPairingRequest(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            return pal.send_pairing_response(
                connection,
                entry->oob,
                authentication,
                initiator_dist,
                responder_dist
            );
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    ble_error_t canceltPairingRequest(connection_handle_t connection) {
        return pal.cancel_pairing(connection, pairing_failure_t::UNSPECIFIED_REASON);
    }

    ble_error_t setPairingRequestAuthorisation(bool required = true) {
        pairing_authorisation_required = required;
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    ble_error_t allowLegacyPairing(bool allow = true) {
        legacy_pairing_allowed = allow;
        return BLE_ERROR_NONE;
    }

    ble_error_t getSecureConnectionsSupport(bool *enabled) {
        return pal.get_secure_connections_support(*enabled);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    ble_error_t setIoCapability(SecurityIOCapabilities_t iocaps) {
        return pal.set_io_capability((io_capability_t::type) iocaps);
    }

    ble_error_t setDisplayPasskey(const Passkey_t passkey) {
        return pal.set_display_passkey(PasskeyAsci::to_num(passkey));
    }

    ble_error_t setAuthenticationTimeout(connection_handle_t connection,
                                         uint32_t timeout_in_ms) {
        return pal.set_authentication_timeout(connection, timeout_in_ms / 10);
    }

    ble_error_t getAuthenticationTimeout(connection_handle_t connection,
                                         uint32_t *timeout_in_ms) {
        uint16_t timeout_in_10ms;
        ble_error_t status = pal.get_authentication_timeout(connection, timeout_in_10ms);
        *timeout_in_ms = 10 * timeout_in_10ms;
        return status;
    }

    ble_error_t setLinkSecurity(connection_handle_t connection,
                                SecurityMode_t securityMode) {
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    ble_error_t getLinkSecurity(connection_handle_t connection,
                                SecurityMode_t *securityMode) {

        *securityMode = SECURITY_MODE_ENCRYPTION_OPEN_LINK;
        return BLE_ERROR_NONE;
    }

    ble_error_t setKeypressNotification(bool enabled = true) {
        authentication.set_keypress_notification(enabled);
        return BLE_ERROR_NONE;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @deprecated
     *
     * Get the security status of a connection.
     *
     * @param[in]  connection   Handle to identify the connection.
     * @param[out] securityStatusP    Security status.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t getLinkSecurity(connection_handle_t connection, LinkSecurityStatus_t *securityStatus) {
        return getLinkEncryption(connection, securityStatus);
    }

    ble_error_t getLinkEncryption(connection_handle_t connection, LinkSecurityStatus_t *securityStatus) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            if (entry->encrypted) {
                *securityStatus = ENCRYPTED;
            } else if (entry->encryption_requested) {
                *securityStatus = ENCRYPTION_IN_PROGRESS;
            } else {
                *securityStatus = NOT_ENCRYPTED;
            }
            return BLE_ERROR_NONE;
         } else {
             return BLE_ERROR_INVALID_PARAM;
         }
    }

    ble_error_t getEncryptionKeySize(connection_handle_t connection, uint8_t *size) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            *size = entry->encryption_key_size;
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t setEncryptionKeyRequirements(uint8_t minimumByteSize, uint8_t maximumByteSize) {
        return pal.set_encryption_key_requirements(minimumByteSize, maximumByteSize);
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

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param entry security entry returned by the database.
     * @param entryKeys security entry containing keys.
     *
     * @return no action instruction to the db since this only reads the keys.
     */
    DbCbAction_t set_ltk_cb(SecurityEntry_t& entry, SecurityEntryKeys_t& entryKeys) {
        pal.set_ltk(entry.handle, entryKeys.ltk);
        return DB_CB_ACTION_NO_UPDATE_REQUIRED;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    ble_error_t requestAuthentication(connection_handle_t connection) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            if (entry->mitm) {
                if (entry->authenticated) {
                    return BLE_ERROR_NONE;
                } else {
                    return pal.enable_encryption(connection);
                }
            } else {
                /* don't change the default value of authentication */
                AuthenticationMask connection_authentication = authentication;
                connection_authentication.set_mitm(true);
                return pal.send_pairing_request(
                    connection,
                    entry->oob,
                    authentication,
                    initiator_dist,
                    responder_dist
                );
            }
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    ble_error_t setOOBDataUsage(connection_handle_t connection,
                                bool useOOB, bool OOBProvidesMITM = true) {
        SecurityEntry_t *entry = db.get_entry(connection);
        if (entry) {
            entry->oob = useOOB;
            entry->oob_mitm_protection = OOBProvidesMITM;
            return BLE_ERROR_NONE;
        } else {
            return BLE_ERROR_INVALID_PARAM;
        }
    }

    virtual ble_error_t confirmationEntered(connection_handle_t connection,
                                            bool confirmation) {
        return pal.confirmation_entered(connection, confirmation);
    }

    virtual ble_error_t passkeyEntered(connection_handle_t connection,
                                       Passkey_t passkey) {
        return pal.passkey_request_reply(
            connection,
            PasskeyAsci::to_num(passkey)
        );
    }

    virtual ble_error_t sendKeypressNotification(connection_handle_t connection,
                                                 Keypress_t keypress) {
        return pal.send_keypress_notification(connection, keypress);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Event handler
    //

    void setSecurityManagerEventHandler(::SecurityManager::SecurityManagerEventHandler* handler) {
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
          authentication(0),
          initiator_dist(0),
          responder_dist(0) {
        _app_event_handler = &defaultEventHandler;
        pal.set_event_handler(this);
    }

private:
    ble::pal::SecurityManager& pal;
    SecurityDb db;

    bool pairing_authorisation_required;
    bool legacy_pairing_allowed;

    AuthenticationMask  authentication;
    KeyDistribution     initiator_dist;
    KeyDistribution     responder_dist;

    /*  implements ble::pal::SecurityManagerEventHandler */
public:

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    void on_pairing_request(connection_handle_t connection,
                            bool use_oob,
                            AuthenticationMask authentication,
                            KeyDistribution initiator_dist,
                            KeyDistribution responder_dist) {
        if (_app_event_handler && pairing_authorisation_required) {
            _app_event_handler->acceptPairingRequest(connection);
        }
    }

    void on_pairing_error(connection_handle_t connection, pairing_failure_t error) {
        if (_app_event_handler) {
            _app_event_handler->pairingResult(
                connection,
                (SecurityManager::SecurityCompletionStatus_t)(error.value() | 0x80)
            );
        }
    }

    void on_pairing_timed_out(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->pairingResult(
                connection,
                SecurityManager::SEC_STATUS_TIMEOUT
            );
        }
    }

    void on_pairing_completed(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->pairingResult(
                connection,
                SecurityManager::SEC_STATUS_SUCCESS
            );
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    void on_valid_mic_timeout(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->validMicTimeout(connection);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    void on_link_encryption_result(connection_handle_t connection,
                                   bool encrypted) {
        if (_app_event_handler) {
            _app_event_handler->linkEncryptionResult(connection, encrypted);
        }
    }

    void on_link_encryption_request_timed_out(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->linkEncryptionResult(connection, false);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    void on_passkey_display(connection_handle_t connection,
                            const passkey_num_t passkey) {
        if (_app_event_handler) {
            _app_event_handler->passkeyDisplay(connection, PasskeyAsci(passkey).asci);
        }
    }

    void on_keypress_notification(connection_handle_t connection,
                                  SecurityManager::Keypress_t keypress) {
        if (_app_event_handler) {
            _app_event_handler->keypressNotification(connection, keypress);
        }
    }

    void on_passkey_request(connection_handle_t connection) {

        if (_app_event_handler) {
            _app_event_handler->passkeyRequest(connection);
        }
    }

    void on_confirmation_request(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->confirmationRequest(connection);
        }
    }

    void on_legacy_pairing_oob_request(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->legacyPairingOobRequest(connection);
        }
    }

    void on_oob_request(connection_handle_t connection) {
        if (_app_event_handler) {
            _app_event_handler->oobRequest(connection);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    void on_keys_distributed(connection_handle_t connection,
                             advertising_peer_address_type_t peer_address_type,
                             const address_t &peer_identity_address,
                             const ediv_t ediv,
                             const rand_t rand,
                             const ltk_t ltk,
                             const irk_t irk,
                             const csrk_t csrk) {
        db.update_entry(
            connection,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_identity_address,
            ediv,
            rand,
            ltk,
            irk,
            csrk
        );
    }

    void on_keys_distributed_ltk(connection_handle_t connection,
                                 const ltk_t ltk) {
        db.update_entry_ltk(connection, ltk);
    }

    void on_keys_distributed_ediv_rand(connection_handle_t connection,
                                       const ediv_t ediv,
                                       const rand_t rand) {
        db.update_entry_ediv_rand(connection, ediv, rand);
    }

    void on_keys_distributed_irk(connection_handle_t connection,
                                 const irk_t irk) {
        db.update_entry_irk(connection, irk);
    }

    void on_keys_distributed_bdaddr(connection_handle_t connection,
                                    advertising_peer_address_type_t peer_address_type,
                                    const address_t &peer_identity_address) {
        db.update_entry_bdaddr(
            connection,
            (peer_address_type == advertising_peer_address_type_t::PUBLIC_ADDRESS),
            peer_identity_address
        );
    }

    void on_keys_distributed_csrk(connection_handle_t connection,
                                  const csrk_t csrk) {
        db.update_entry_csrk(connection, csrk);
    }

    void on_ltk_request(connection_handle_t connection,
                        const ediv_t ediv,
                        const rand_t rand) {
        db.get_entry_keys(mbed::callback(this, &GenericSecurityManager::set_ltk_cb), ediv, rand);
    }

private:
    /* handler is always a valid pointer */
    ::SecurityManager::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */

#endif /*__GENERIC_SECURITY_MANAGER_H__*/
