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
using ble::pal::AuthenticationMask;
using ble::pal::KeyDistribution;
using ble::pal::connection_peer_address_type_t;

using ble::irk_t;
using ble::csrk_t;
using ble::ltk_t;
using ble::ediv_t;
using ble::rand_t;
using ble::pairing_failure_t;
using ble::PasskeyAscii;
using ble::passkey_num_t;

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

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    virtual ble_error_t purgeAllBondingState(void);

    virtual ble_error_t generateWhitelistFromBondTable(Gap::Whitelist_t *whitelist) const;

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

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

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

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t setPrivateAddressTimeout(uint16_t timeout_in_seconds);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual ble_error_t getSigningKey(
        connection_handle_t connection,
        bool authenticated
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

    virtual ble_error_t set_peer_oob(
        const address_t& peer_address,
        const oob_rand_t& random,
        const oob_confirm_t& confirm
    );

    virtual ble_error_t get_local_oob(
        connection_handle_t connection,
        address_t& peer_address,
        oob_rand_t& random,
        oob_confirm_t& confirm
    ) const;

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

    ////////////////////////////////////////////////////////////////////////////
    // Helper functions
    //

private:
    /**
     * Generate the CSRK if needed.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t init_signing();

    /**
     * Send slave security request based on current link settings.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t slave_security_request(connection_handle_t connection);

    /**
     * Enable encryption on the link, depending on whether device is master or slave.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t enable_encryption(connection_handle_t connection);

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param[in] entry security entry returned by the database.
     * @param[in] entryKeys security entry containing keys.
     */
    void enable_encryption_cb(
        const SecurityEntry_t* entry,
        const SecurityEntryKeys_t* entryKeys
    );

    /**
     * Check if identity resolving key resolves the peer address.
     *
     * @param[in] peer_address Resolvable random address.
     * @param[in] irk Identity resolving key.
     * @return True if identity resolving key resolves the peer address.
     */
    bool check_against_identity_address(
        const address_t peer_address,
        const irk_t *irk
    );

    /**
     * Checks device address against an identity resolving key. Called by the security db.
     *
     * @param[in] identity identity resolving key and identity address
     */
    void check_against_identity_cb(
        const SecurityEntryIdentity_t *identity
    );

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param[in] entry security entry returned by the database.
     * @param[in] entryKeys security entry containing keys.
     */
    void set_ltk_cb(
        const SecurityEntry_t* entry,
        const SecurityEntryKeys_t* entryKeys
    );

    /**
     * Returns the CSRK for the connection. Called by the security db.
     *
     * @param[in] entry security entry returned by the database.
     * @param[in] entryKeys security entry containing keys.
     */
    void return_csrk_cb(
        connection_handle_t connection,
        const csrk_t *csrk
    );

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

    /** @copydoc SecurityManagerEventHandler::on_pairing_request
     */
    virtual void on_pairing_request(
        connection_handle_t connection,
        bool use_oob,
        AuthenticationMask authentication,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /** @copydoc SecurityManagerEventHandler::on_pairing_error
     */
    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    );

    /** @copydoc SecurityManagerEventHandler::on_pairing_timed_out
     */
    virtual void on_pairing_timed_out(connection_handle_t connection);

    /** @copydoc SecurityManagerEventHandler::on_pairing_completed
     */
    virtual void on_pairing_completed(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    /** @copydoc SecurityManagerEventHandler::on_valid_mic_timeout
     */
    virtual void on_valid_mic_timeout(connection_handle_t connection);

    /** @copydoc SecurityManagerEventHandler::on_slave_security_request
     */
    virtual void on_slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /** @copydoc SecurityManagerEventHandler::on_link_encryption_result
     */
    virtual void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    );

    /** @copydoc SecurityManagerEventHandler::on_link_encryption_request_timed_out
     */
    virtual void on_link_encryption_request_timed_out(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /** @copydoc SecurityManagerEventHandler::set_mitm_performed
     */
    virtual void set_mitm_performed(connection_handle_t connection, bool enable = true);

    /** @copydoc SecurityManagerEventHandler::on_passkey_display
     */
    virtual void on_passkey_display(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /** @copydoc SecurityManagerEventHandler::on_keypress_notification
     */
    virtual void on_keypress_notification(
        connection_handle_t connection,
        SecurityManager::Keypress_t keypress
    );

    /** @copydoc SecurityManagerEventHandler::on_passkey_request
     */
    virtual void on_passkey_request(connection_handle_t connection);

    /** @copydoc SecurityManagerEventHandler::on_confirmation_request
     */
    virtual void on_confirmation_request(connection_handle_t connection);

    /** @copydoc SecurityManagerEventHandler::on_legacy_pairing_oob_request
     */
    virtual void on_legacy_pairing_oob_request(connection_handle_t connection);

    /** @copydoc SecurityManagerEventHandler::on_oob_request
     */
    virtual void on_oob_request(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /** @copydoc SecurityManagerEventHandler::on_secure_connections_ltk_generated
     */
    virtual void on_secure_connections_ltk_generated(
        connection_handle_t connection,
        const ltk_t *ltk
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed
     */
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

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_ltk
     */
    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_ediv_rand
     */
    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_local_ltk
     */
    virtual void on_keys_distributed_local_ltk(
        connection_handle_t connection,
        const ltk_t *ltk
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_local_ediv_rand
     */
    virtual void on_keys_distributed_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_irk
     */
    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t *irk
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_bdaddr
     */
    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    );

    /** @copydoc SecurityManagerEventHandler::on_keys_distributed_csrk
     */
    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t *csrk
    );

    /** @copydoc SecurityManagerEventHandler::on_ltk_requeston_ltk_request
     */
    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t *ediv,
        const rand_t *rand
    );

    /* end implements ble::pal::SecurityManagerEventHandler */

    /* GAP integration */
public:

    /**
     * Inform the security manager that a device has been disconnected and its
     * entry can be put in NVM storage. Called by GAP.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual void on_disconnected(
        connection_handle_t connection
    );

    /**
     * Inform the Security manager of a new connection. This will create
     * or retrieve an existing security manager entry for the connected device.
     * Called by GAP.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] is_master True if device is the master.
     * @param[in] peer_address_type type of address
     * @param[in] peer_address Address of the connected device.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual void on_connected(
        connection_handle_t connection,
        bool is_master,
        connection_peer_address_type_t::type peer_address_type,
        const address_t &peer_address
    );

private:
    /* handler is always a valid pointer */
    ::SecurityManager::SecurityManagerEventHandler *_app_event_handler;
};


} /* namespace generic */
} /* namespace ble */


#endif /*__GENERIC_SECURITY_MANAGER_H__*/
