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

#ifndef _GENERIC_SECURITY_MANAGER_H_
#define _GENERIC_SECURITY_MANAGER_H_

#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/generic/SecurityDb.h"
#include "platform/Callback.h"
#include "ble/pal/ConnectionEventMonitor.h"
#include "ble/pal/SigningEventMonitor.h"
#include "ble/generic/GenericGap.h"
#include "ble/pal/PalSecurityManager.h"
#include "ble/ArrayView.h"

namespace ble {
namespace generic {

typedef SecurityManager::SecurityIOCapabilities_t SecurityIOCapabilities_t;

class GenericSecurityManager : public SecurityManager,
                               public pal::SecurityManager::EventHandler,
                               public pal::ConnectionEventMonitor::EventHandler,
                               public pal::SigningEventMonitor::EventHandler {
public:

    /* implements SecurityManager */

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    virtual ble_error_t init(
        bool bondable = true,
        bool mitm = true,
        SecurityIOCapabilities_t iocaps = IO_CAPS_NONE,
        const Passkey_t passkey = NULL,
        bool signing = true,
        const char* db_path = NULL
    );

    virtual ble_error_t setDatabaseFilepath(const char *db_path = NULL);

    virtual ble_error_t reset();

    virtual ble_error_t preserveBondingStateOnReset(
        bool enabled
    );

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    virtual ble_error_t purgeAllBondingState();

    virtual ble_error_t generateWhitelistFromBondTable(
        Gap::Whitelist_t *whitelist
    ) const;

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    virtual ble_error_t requestPairing(
        connection_handle_t connection
    );

    virtual ble_error_t acceptPairingRequest(
        connection_handle_t connection
    );

    virtual ble_error_t cancelPairingRequest(
        connection_handle_t connection
    );

    virtual ble_error_t setPairingRequestAuthorisation(
        bool required = true
    );

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t allowLegacyPairing(
        bool allow = true
    );

    virtual ble_error_t getSecureConnectionsSupport(
        bool *enabled
    );

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t setIoCapability(
        SecurityIOCapabilities_t iocaps
    );

    virtual ble_error_t setDisplayPasskey(
        const Passkey_t passkey
    );

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

    virtual ble_error_t setKeypressNotification(
        bool enabled = true
    );

    virtual ble_error_t enableSigning(
        connection_handle_t connection,
        bool enabled = true
    );

    virtual ble_error_t setHintFutureRoleReversal(
        bool enable = true
    );

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

    virtual ble_error_t setPrivateAddressTimeout(
        uint16_t timeout_in_seconds
    );

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

    virtual ble_error_t requestAuthentication(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    virtual ble_error_t generateOOB(
        const address_t *address
    );

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

    virtual ble_error_t legacyPairingOobReceived(
        const address_t *address,
        const oob_tk_t *tk
    );

    virtual ble_error_t oobReceived(
        const address_t *address,
        const oob_lesc_value_t *random,
        const oob_confirm_t *confirm
    );

    /* ends implements SecurityManager */

public:
    GenericSecurityManager(
        pal::SecurityManager &palImpl,
        pal::ConnectionEventMonitor &connMonitorImpl,
        pal::SigningEventMonitor &signingMonitorImpl
    ) : _pal(palImpl),
        _connection_monitor(connMonitorImpl),
        _signing_monitor(signingMonitorImpl),
        _db(NULL),
        _default_authentication(0),
        _default_key_distribution(pal::KeyDistribution::KEY_DISTRIBUTION_ALL),
        _pairing_authorisation_required(false),
        _legacy_pairing_allowed(true),
        _master_sends_keys(false) {
        _pal.set_event_handler(this);

        /* We create a fake value for oob to allow creation of the next oob which needs
         * the last process to finish first before restarting (this is to simplify checking).
         * This fake value will not be used as the oob address is currently invalid */
        _oob_local_random[0] = 1;
    }

    ~GenericSecurityManager() {
        delete _db;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Helper functions
    //

private:

    /**
     * Initialise the database, if database already exists it will close it and open the new one.
     *
     * @param db_path path to file to store secure db
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t init_database(const char *db_path = NULL);

    /**
     * Generate identity list based on the database of IRK and apply it to the resolving list.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t init_resolving_list();

    /**
     * Generate the CSRK if needed.
     *
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t init_signing();

    /**
     * Fills the buffer with the specified number of bytes of random data
     * produced by the link controller
     *
     * @param[out] buffer buffer to be filled with random data
     * @param[in] size number of bytes to fill with random data
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t get_random_data(
        uint8_t *buffer,
        size_t size
    );

    /**
     * Send slave security request based on current link settings.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t slave_security_request(
        connection_handle_t connection
    );

    /**
     * Enable encryption on the link, depending on whether device is master or slave.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    ble_error_t enable_encryption(
        connection_handle_t connection
    );

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param[in] entry security entry returned by the database.
     * @param[in] entryKeys security entry containing keys.
     */
    void enable_encryption_cb(
        SecurityDb::entry_handle_t entry,
        const SecurityEntryKeys_t* entryKeys
    );

    /**
     * Returns the requested LTK to the PAL. Called by the security db.
     *
     * @param[in] entry security entry returned by the database.
     * @param[in] entryKeys security entry containing keys.
     */
    void set_ltk_cb(
        SecurityDb::entry_handle_t entry,
        const SecurityEntryKeys_t* entryKeys
    );

    /**
     * Returns the CSRK for the connection. Called by the security db.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] signing connection signature resolving key and counter.
     */
    void return_csrk_cb(
        SecurityDb::entry_handle_t connection,
        const SecurityEntrySigning_t *signing
    );

    /**
     * Set the peer CSRK for the connection. Called by the security db.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] signing connection signature resolving key and counter.
     */
    void set_peer_csrk_cb(
        SecurityDb::entry_handle_t connection,
        const SecurityEntrySigning_t *signing
    );

    /**
     * Updates the entry for the connection with OOB data presence.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     */
    void update_oob_presence(
        connection_handle_t connection
    );

    /**
     * Set the MITM protection setting on the database entry
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] enable if true set the MITM protection to on.
     */
    virtual void set_mitm_performed(
        connection_handle_t connection,
        bool enable = true
    );

    /**
     * Inform the Security manager of a new connection. This will create
     * or retrieve an existing security manager entry for the connected device.
     * Called by GAP.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @param[in] is_master True if device is the master.
     * @param[in] peer_address_type type of address.
     * @param[in] peer_address Address of the connected device.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual void on_connected(
        connection_handle_t connection,
        Gap::Role_t role,
        peer_address_type_t peer_address_type,
        const BLEProtocol::AddressBytes_t peer_address,
        BLEProtocol::AddressType_t local_address_type,
        const BLEProtocol::AddressBytes_t local_address,
        const Gap::ConnectionParams_t *connection_params
    );

    /**
     * Inform the security manager that a device has been disconnected and its
     * entry can be put in NVM storage. Called by GAP.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    virtual void on_disconnected(
        connection_handle_t connection,
        Gap::DisconnectionReason_t reason
    );

    /**
     * Callback invoked by the secure DB when an identity entry has been
     * retrieved.
     * @param entry Handle of the entry.
     * @param identity The identity associated with the entry; may be NULL.
     */
    void on_security_entry_retrieved(
        SecurityDb::entry_handle_t entry,
        const SecurityEntryIdentity_t* identity
    );

    /**
     * Callback invoked by the secure DB when the identity list has been
     * retrieved.
     *
     * @param identity View to the array passed to the secure DB. It contains
     * identity entries retrieved.
     *
     * @param count Number of identities entries retrieved.
     */
    void on_identity_list_retrieved(
        ble::ArrayView<SecurityEntryIdentity_t>& identity_list,
        size_t count
    );

private:
    struct ControlBlock_t {
        ControlBlock_t();

        pal::KeyDistribution get_initiator_key_distribution() {
            return pal::KeyDistribution(initiator_key_distribution);
        };
        pal::KeyDistribution get_responder_key_distribution() {
            return pal::KeyDistribution(responder_key_distribution);
        };
        void set_initiator_key_distribution(pal::KeyDistribution mask) {
            initiator_key_distribution = mask.value();
        };
        void set_responder_key_distribution(pal::KeyDistribution mask) {
            responder_key_distribution = mask.value();
        };

        connection_handle_t connection;
        SecurityDb::entry_handle_t db_entry;

        address_t local_address; /**< address used for connection, possibly different from identity */

    private:
        uint8_t initiator_key_distribution:4;
        uint8_t responder_key_distribution:4;
    public:
        uint8_t connected:1;
        uint8_t authenticated:1; /**< have we turned encryption on during this connection */
        uint8_t is_master:1;

        uint8_t encryption_requested:1;
        uint8_t encryption_failed:1;
        uint8_t encrypted:1;
        uint8_t signing_requested:1;
        uint8_t signing_override_default:1;

        uint8_t mitm_requested:1;
        uint8_t mitm_performed:1; /**< keys exchange will have MITM protection */

        uint8_t attempt_oob:1;
        uint8_t oob_mitm_protection:1;
        uint8_t oob_present:1;
        uint8_t legacy_pairing_oob_request_pending:1;

        uint8_t csrk_failures:2;
    };

    pal::SecurityManager &_pal;
    pal::ConnectionEventMonitor &_connection_monitor;
    pal::SigningEventMonitor &_signing_monitor;

    SecurityDb *_db;

    /* OOB data */
    address_t _oob_local_address;
    address_t _oob_peer_address;
    oob_lesc_value_t _oob_peer_random;
    oob_confirm_t _oob_peer_confirm;
    oob_lesc_value_t _oob_local_random;
    address_t _oob_temporary_key_creator_address; /**< device which generated and sent the TK */
    oob_tk_t _oob_temporary_key; /**< used for legacy pairing */

    pal::AuthenticationMask _default_authentication;
    pal::KeyDistribution _default_key_distribution;

    bool _pairing_authorisation_required;
    bool _legacy_pairing_allowed;
    bool _master_sends_keys;

    static const size_t MAX_CONTROL_BLOCKS = 5;
    ControlBlock_t _control_blocks[MAX_CONTROL_BLOCKS];

    /* implements ble::pal::SecurityManager::EventHandler */
public:
    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /** @copydoc ble::pal::SecurityManager::on_pairing_request
     */
    virtual void on_pairing_request(
        connection_handle_t connection,
        bool use_oob,
        pal::AuthenticationMask authentication,
        pal::KeyDistribution initiator_dist,
        pal::KeyDistribution responder_dist
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_error
     */
    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_timed_out
     */
    virtual void on_pairing_timed_out(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_completed
     */
    virtual void on_pairing_completed(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    /** @copydoc ble::pal::SecurityManager::on_valid_mic_timeout
     */
    virtual void on_valid_mic_timeout(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write_received
     */
    virtual void on_signed_write_received(
        connection_handle_t connection,
        uint32_t sign_coutner
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write_verification_failure
     */
    virtual void on_signed_write_verification_failure(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write
     */
    virtual void on_signed_write();

    /** @copydoc ble::pal::SecurityManager::on_slave_security_request
     */
    virtual void on_slave_security_request(
        connection_handle_t connection,
        pal::AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /** @copydoc ble::pal::SecurityManager::on_link_encryption_result
     */
    virtual void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    );

    /** @copydoc ble::pal::SecurityManager::on_link_encryption_request_timed_out
     */
    virtual void on_link_encryption_request_timed_out(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /** @copydoc ble::pal::SecurityManager::on_passkey_display
     */
    virtual void on_passkey_display(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /** @copydoc ble::pal::SecurityManager::on_keypress_notification
     */
    virtual void on_keypress_notification(
        connection_handle_t connection,
        SecurityManager::Keypress_t keypress
    );

    /** @copydoc ble::pal::SecurityManager::on_passkey_request
     */
    virtual void on_passkey_request(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_confirmation_request
     */
    virtual void on_confirmation_request(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_oob_request
     */
    virtual void on_secure_connections_oob_request(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_legacy_pairing_oob_request
     */
    virtual void on_legacy_pairing_oob_request(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_oob_generated
     */
    virtual void on_secure_connections_oob_generated(
        const oob_lesc_value_t &random,
        const oob_confirm_t &confirm
    );

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_ltk_generated
     */
    virtual void on_secure_connections_ltk_generated(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_ltk
     */
    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_ediv_rand
     */
    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_local_ltk
     */
    virtual void on_keys_distributed_local_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_local_ediv_rand
     */
    virtual void on_keys_distributed_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_irk
     */
    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t &irk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_bdaddr
     */
    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        pal::advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_csrk
     */
    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t &csrk
    );

    /** @copydoc ble::pal::SecurityManager::on_ltk_requeston_ltk_request
     */
    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_ltk_requeston_ltk_request
     */
    virtual void on_ltk_request(
        connection_handle_t connection
    );

    /* end implements ble::pal::SecurityManager::EventHandler */

    /* list management */

    ControlBlock_t* acquire_control_block(connection_handle_t connection);

    ControlBlock_t* get_control_block(connection_handle_t connection);

    ControlBlock_t* get_control_block(const address_t &peer_address);

    ControlBlock_t* get_control_block(SecurityDb::entry_handle_t db_entry);

    void release_control_block(ControlBlock_t* entry);
};


} /* namespace generic */
} /* namespace ble */


#endif /*_GENERIC_SECURITY_MANAGER_H_*/
