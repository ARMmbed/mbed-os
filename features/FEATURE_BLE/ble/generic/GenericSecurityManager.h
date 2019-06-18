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

#include "ble/SecurityManager.h"
#include "ble/pal/GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/generic/SecurityDb.h"
#include "platform/Callback.h"
#include "ble/pal/ConnectionEventMonitor.h"
#include "ble/pal/SigningEventMonitor.h"
#include "ble/generic/GenericGap.h"
#include "ble/pal/PalSecurityManager.h"

namespace ble {
namespace generic {

template <template<class> class TPalSecurityManager, template<class> class SigningMonitor>
class GenericSecurityManager :
    public interface::SecurityManager<GenericSecurityManager<TPalSecurityManager, SigningMonitor > >, // SecurityManager
    public pal::SecurityManagerEventHandler<GenericSecurityManager<TPalSecurityManager, SigningMonitor> >, // PalSmEventHandler
    public pal::ConnectionEventMonitorEventHandler<GenericSecurityManager<TPalSecurityManager, SigningMonitor> >, // ConnectionObserver
    public pal::SigningMonitorEventHandler<GenericSecurityManager<TPalSecurityManager, SigningMonitor> > //SigningObserver
{
    // typedefs
    typedef interface::SecurityManager<GenericSecurityManager> SecurityManager;
    typedef SigningMonitor<GenericSecurityManager> SigningEventMonitor;
    typedef typename TPalSecurityManager<GenericSecurityManager>::Base PalSecurityManager;

    // friends
    friend class pal::ConnectionEventMonitorEventHandler<GenericSecurityManager>;

    // imports from SecurityManager
    typedef typename SecurityManager::SecurityIOCapabilities_t SecurityIOCapabilities_t;
    typedef typename SecurityManager::SecurityMode_t SecurityMode_t;
    typedef typename SecurityManager::SecurityCompletionStatus_t SecurityCompletionStatus_t;
    typedef typename SecurityManager::Passkey_t Passkey_t;
    typedef typename SecurityManager::Keypress_t Keypress_t;
    typedef typename SecurityManager::EventHandler SecurityManagerEventHandler;

    using SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK;
    using SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM;
    using SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM;
    using SecurityManager::SECURITY_MODE_SIGNED_NO_MITM;
    using SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM;
    using SecurityManager::SEC_STATUS_TIMEOUT;
    using SecurityManager::SEC_STATUS_SUCCESS;

    using SecurityManager::eventHandler;
    using SecurityManager::requestPairing;
    using SecurityManager::getLinkEncryption;
    using SecurityManager::requestAuthentication;
    using SecurityManager::generateOOB;
    using SecurityManager::cancelPairingRequest;
    using SecurityManager::acceptPairingRequest;

public:

    /* implements SecurityManager */

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    ble_error_t init_(
        bool bondable,
        bool mitm,
        SecurityIOCapabilities_t iocaps,
        const uint8_t* passkey,
        bool signing,
        const char* db_path
    );

    ble_error_t setDatabaseFilepath_(const char *db_path = NULL);

    ble_error_t reset_();

    ble_error_t preserveBondingStateOnReset_(
        bool enabled
    );

    ////////////////////////////////////////////////////////////////////////////
    // List management
    //

    ble_error_t purgeAllBondingState_();

    ble_error_t generateWhitelistFromBondTable_(
        ::Gap::Whitelist_t *whitelist
    ) const;

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    ble_error_t requestPairing_(
        connection_handle_t connection
    );

    ble_error_t acceptPairingRequest_(
        connection_handle_t connection
    );

    ble_error_t cancelPairingRequest_(
        connection_handle_t connection
    );

    ble_error_t setPairingRequestAuthorisation_(
        bool required = true
    );

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    ble_error_t allowLegacyPairing_(
        bool allow = true
    );

    ble_error_t getSecureConnectionsSupport_(
        bool *enabled
    );

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    ble_error_t setIoCapability_(
        SecurityIOCapabilities_t iocaps
    );

    ble_error_t setDisplayPasskey_(
        const uint8_t* passkey
    );

    ble_error_t setAuthenticationTimeout_(
        connection_handle_t connection,
        uint32_t timeout_in_ms
    );

    ble_error_t getAuthenticationTimeout_(
        connection_handle_t connection,
        uint32_t *timeout_in_ms
    );

    ble_error_t setLinkSecurity_(
        connection_handle_t connection,
        SecurityMode_t securityMode
    );

    ble_error_t setKeypressNotification_(
        bool enabled
    );

    ble_error_t enableSigning_(
        connection_handle_t connection,
        bool enabled
    );

    ble_error_t setHintFutureRoleReversal_(
        bool enable
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    ble_error_t getLinkEncryption_(
        connection_handle_t connection,
        link_encryption_t *encryption
    );

    ble_error_t setLinkEncryption_(
        connection_handle_t connection,
        link_encryption_t encryption
    );

    ble_error_t getEncryptionKeySize_(
        connection_handle_t connection,
        uint8_t *size
    );

    ble_error_t setEncryptionKeyRequirements_(
        uint8_t minimumByteSize,
        uint8_t maximumByteSize
    );

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    ble_error_t setPrivateAddressTimeout_(
        uint16_t timeout_in_seconds
    );

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    ble_error_t getSigningKey_(
        connection_handle_t connection,
        bool authenticated
    );

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    ble_error_t requestAuthentication_(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    ble_error_t generateOOB_(
        const address_t *address
    );

    ble_error_t setOOBDataUsage_(
        connection_handle_t connection,
        bool useOOB,
        bool OOBProvidesMITM
    );

    ble_error_t confirmationEntered_(
        connection_handle_t connection,
        bool confirmation
    );

    ble_error_t passkeyEntered_(
        connection_handle_t connection,
        Passkey_t passkey
    );

    ble_error_t sendKeypressNotification_(
        connection_handle_t connection,
        Keypress_t keypress
    );

    ble_error_t legacyPairingOobReceived_(
        const address_t *address,
        const oob_tk_t *tk
    );

    ble_error_t oobReceived_(
        const address_t *address,
        const oob_lesc_value_t *random,
        const oob_confirm_t *confirm
    );

    /* ends implements SecurityManager */

public:
    GenericSecurityManager(
        PalSecurityManager &palImpl,
        pal::ConnectionEventMonitor<GenericSecurityManager> &connMonitorImpl,
        SigningEventMonitor &signingMonitorImpl
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
    void set_mitm_performed(
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
    void on_connected_(
        connection_handle_t connection,
        ::Gap::Role_t role,
        peer_address_type_t peer_address_type,
        const BLEProtocol::AddressBytes_t peer_address,
        BLEProtocol::AddressType_t local_address_type,
        const BLEProtocol::AddressBytes_t local_address,
        const ::Gap::ConnectionParams_t *connection_params
    );

    /**
     * Inform the security manager that a device has been disconnected and its
     * entry can be put in NVM storage. Called by GAP.
     *
     * @param[in] connectionHandle Handle to identify the connection.
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    void on_disconnected_(
        connection_handle_t connection,
        ::Gap::DisconnectionReason_t reason
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
        Span<SecurityEntryIdentity_t>& identity_list,
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

    PalSecurityManager &_pal;
    pal::ConnectionEventMonitor<GenericSecurityManager> &_connection_monitor;
    SigningEventMonitor &_signing_monitor;

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
    void on_pairing_request_(
        connection_handle_t connection,
        bool use_oob,
        pal::AuthenticationMask authentication,
        pal::KeyDistribution initiator_dist,
        pal::KeyDistribution responder_dist
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_error
     */
    void on_pairing_error_(
        connection_handle_t connection,
        pairing_failure_t error
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_timed_out
     */
    void on_pairing_timed_out_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_pairing_completed
     */
    void on_pairing_completed_(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    /** @copydoc ble::pal::SecurityManager::on_valid_mic_timeout
     */
    void on_valid_mic_timeout_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write_received
     */
    void on_signed_write_received_(
        connection_handle_t connection,
        uint32_t sign_coutner
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write_verification_failure
     */
    void on_signed_write_verification_failure_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_signed_write
     */
    void on_signed_write_();

    /** @copydoc ble::pal::SecurityManager::on_slave_security_request
     */
    void on_slave_security_request_(
        connection_handle_t connection,
        pal::AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /** @copydoc ble::pal::SecurityManager::on_link_encryption_result
     */
    void on_link_encryption_result_(
        connection_handle_t connection,
        link_encryption_t result
    );

    /** @copydoc ble::pal::SecurityManager::on_link_encryption_request_timed_out
     */
    void on_link_encryption_request_timed_out_(
        connection_handle_t connection
    );

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /** @copydoc ble::pal::SecurityManager::on_passkey_display
     */
    void on_passkey_display_(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /** @copydoc ble::pal::SecurityManager::on_keypress_notification
     */
    void on_keypress_notification_(
        connection_handle_t connection,
        Keypress_t keypress
    );

    /** @copydoc ble::pal::SecurityManager::on_passkey_request
     */
    void on_passkey_request_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_confirmation_request
     */
    void on_confirmation_request_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_oob_request
     */
    void on_secure_connections_oob_request_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_legacy_pairing_oob_request
     */
    void on_legacy_pairing_oob_request_(
        connection_handle_t connection
    );

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_oob_generated
     */
    void on_secure_connections_oob_generated_(
        const oob_lesc_value_t &random,
        const oob_confirm_t &confirm
    );

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /** @copydoc ble::pal::SecurityManager::on_secure_connections_ltk_generated
     */
    void on_secure_connections_ltk_generated_(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_ltk
     */
    void on_keys_distributed_ltk_(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_ediv_rand
     */
    void on_keys_distributed_ediv_rand_(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_local_ltk
     */
    void on_keys_distributed_local_ltk_(
        connection_handle_t connection,
        const ltk_t &ltk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_local_ediv_rand
     */
    void on_keys_distributed_local_ediv_rand_(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_irk
     */
    void on_keys_distributed_irk_(
        connection_handle_t connection,
        const irk_t &irk
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_bdaddr
     */
    void on_keys_distributed_bdaddr_(
        connection_handle_t connection,
        pal::advertising_peer_address_type_t peer_address_type,
        const address_t &peer_identity_address
    );

    /** @copydoc ble::pal::SecurityManager::on_keys_distributed_csrk
     */
    void on_keys_distributed_csrk_(
        connection_handle_t connection,
        const csrk_t &csrk
    );

    /** @copydoc ble::pal::SecurityManager::on_ltk_requeston_ltk_request
     */
    void on_ltk_request_(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    );

    /** @copydoc ble::pal::SecurityManager::on_ltk_requeston_ltk_request
     */
    void on_ltk_request_(
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
