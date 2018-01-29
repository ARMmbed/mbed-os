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

#ifndef __GENERIC_SECURITY_MANAGER_H__
#define __GENERIC_SECURITY_MANAGER_H__

#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/generic/GenericSecurityDb.h"
#include "Callback.h"

namespace ble {
namespace generic {

using ble::pal::address_t;
using ble::pal::advertising_peer_address_type_t;
using ble::irk_t;
using ble::csrk_t;
using ble::ltk_t;
using ble::ediv_t;
using ble::rand_t;
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
    );

    virtual ble_error_t reset(void);

    virtual ble_error_t preserveBondingStateOnReset(bool enabled);

    virtual ble_error_t init_signing();

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    virtual ble_error_t purgeAllBondingState(void);

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual ble_error_t requestPairing(connection_handle_t connection);

    virtual ble_error_t acceptPairingRequest(connection_handle_t connection);

    virtual ble_error_t canceltPairingRequest(connection_handle_t connection);

    virtual ble_error_t setPairingRequestAuthorisation(bool required = true);

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t allowLegacyPairing(bool allow = true);

    virtual ble_error_t getSecureConnectionsSupport(bool *enabled);

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t setIoCapability(SecurityIOCapabilities_t iocaps);

    virtual ble_error_t setDisplayPasskey(const Passkey_t passkey);

    virtual ble_error_t setAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t timeout_in_ms
    );

    virtual ble_error_t getAuthenticationTimeout(
        connection_handle_t connection,
        uint32_t *timeout_in_ms
    );

    virtual ble_error_t setLinkSecurity(
        connection_handle_t connection,
        SecurityMode_t securityMode
    );

    virtual ble_error_t setKeypressNotification(bool enabled = true);

    virtual ble_error_t enableSigning(
        connection_handle_t connection,
        bool enabled = true
    );

    virtual ble_error_t setHintFutureRoleReversal(bool enable = true);

    virtual ble_error_t slave_security_request(connection_handle_t connection);

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
    );

    virtual ble_error_t setLinkEncryption(
        connection_handle_t connection,
        link_encryption_t encryption
    );

    virtual ble_error_t getEncryptionKeySize(
        connection_handle_t connection,
        uint8_t *size
    );

    virtual ble_error_t setEncryptionKeyRequirements(
        uint8_t minimumByteSize,
        uint8_t maximumByteSize
    );

    virtual ble_error_t enable_encryption(connection_handle_t connection);

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
    );

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t setPrivateAddressTimeout(uint16_t timeout_in_seconds);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual ble_error_t getSigningKey(connection_handle_t connection, bool authenticated);

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
    );

    DbCbAction_t return_csrk_cb(
        connection_handle_t connection,
        const csrk_t *csrk
    );

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    virtual ble_error_t requestAuthentication(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    virtual ble_error_t setOOBDataUsage(
        connection_handle_t connection,
        bool useOOB,
        bool OOBProvidesMITM = true
    );

    virtual ble_error_t confirmationEntered(
        connection_handle_t connection,
        bool confirmation
    );

    virtual ble_error_t passkeyEntered(
        connection_handle_t connection,
        Passkey_t passkey
    );

    virtual ble_error_t sendKeypressNotification(
        connection_handle_t connection,
        Keypress_t keypress
    );
    ////////////////////////////////////////////////////////////////////////////
    // Event handler
    //

    virtual void setSecurityManagerEventHandler(
        ::SecurityManager::SecurityManagerEventHandler* handler
     );

protected:
    GenericSecurityManager(ble::pal::SecurityManager& palImpl, GenericSecurityDb& dbImpl)
        : _pal(palImpl),
          _db(dbImpl),
          _pairing_authorisation_required(false),
          _legacy_pairing_allowed(true),
          _master_sends_keys(false),
          _default_authentication(0),
          _default_key_distribution(KeyDistribution::KEY_DISTRIBUTION_ALL) {
        _app_event_handler = &defaultEventHandler;
        _pal.set_event_handler(this);
    }

private:
    ble::pal::SecurityManager& _pal;
    GenericSecurityDb& _db;

    bool _pairing_authorisation_required;
    bool _legacy_pairing_allowed;
    bool _master_sends_keys;

    AuthenticationMask _default_authentication;
    KeyDistribution _default_key_distribution;

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
    );

    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    );

    virtual void on_pairing_timed_out(connection_handle_t connection);

    virtual void on_pairing_completed(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    virtual void on_valid_mic_timeout(connection_handle_t connection);

    virtual void on_slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    virtual void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    );

    virtual void on_link_encryption_request_timed_out(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //
    virtual void set_mitm_performed(connection_handle_t connection, bool enable = true);

    virtual void on_passkey_display(
        connection_handle_t connection,
        const passkey_num_t passkey
    );

    virtual void on_keypress_notification(
        connection_handle_t connection,
        SecurityManager::Keypress_t keypress
    );

    virtual void on_passkey_request(connection_handle_t connection);

    virtual void on_confirmation_request(connection_handle_t connection);

    virtual void on_legacy_pairing_oob_request(connection_handle_t connection);

    virtual void on_oob_request(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual void on_keys_distributed(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address,
        const ediv_t *ediv,
        const rand_t *rand,
        const ltk_t *ltk,
        const irk_t *irk,
        const csrk_t *csrk
    );

    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    );

    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    virtual void on_keys_distributed_local_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    );

    virtual void on_keys_distributed_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t *irk
    );

    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    );

    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t *csrk
    );

    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    virtual void on_disconnected(connection_handle_t connection);

    virtual void on_connected(connection_handle_t connection, address_t peer_address, bool is_master);

private:
    /* handler is always a valid pointer */
    ::SecurityManager::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */


#endif /*__GENERIC_SECURITY_MANAGER_H__*/
