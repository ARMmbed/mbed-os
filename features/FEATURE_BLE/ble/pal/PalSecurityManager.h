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

#ifndef MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_
#define MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_

#include "ble/common/StaticInterface.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "ble/BLETypes.h"
#include "ble/BLEProtocol.h"
#include "ble/SecurityManager.h"
#include "ble/pal/GapTypes.h"

namespace ble {
namespace pal {

typedef ::SecurityManager::SecurityCompletionStatus_t SecurityCompletionStatus_t;
typedef ::SecurityManager::SecurityMode_t SecurityMode_t;
typedef ::SecurityManager::Keypress_t Keypress_t;

/**
 * Key distribution as required by the SMP with convenient setters and getters,
 * use value() to get the octet you can use directly in the PDU.
 */
class KeyDistribution {
public:
    enum KeyDistributionFlags_t {
        KEY_DISTRIBUTION_NONE       = 0x00,
        KEY_DISTRIBUTION_ENCRYPTION = 0x01,
        KEY_DISTRIBUTION_IDENTITY   = 0x02,
        KEY_DISTRIBUTION_SIGNING    = 0x04,
        KEY_DISTRIBUTION_LINK       = 0x08,
        KEY_DISTRIBUTION_ALL        = 0x0F
    };

    KeyDistribution() : _value(0) { }
    KeyDistribution(uint8_t value) : _value(value) { }
    KeyDistribution(bool encryption,
                    bool identity,
                    bool signing,
                    bool link) : _value(0) {
        set_encryption(encryption);
        set_identity(identity);
        set_signing(signing);
        set_link(link);
    }

    bool get_encryption() const {
        return _value & KEY_DISTRIBUTION_ENCRYPTION;
    }
    bool get_identity() const {
        return _value & KEY_DISTRIBUTION_IDENTITY;
    }
    bool get_signing() const {
        return _value & KEY_DISTRIBUTION_SIGNING;
    }
    bool get_link() const {
        return _value & KEY_DISTRIBUTION_LINK;
    }

    void set_encryption(bool enabled = true) {
        if (enabled) {
            _value |= KEY_DISTRIBUTION_ENCRYPTION;
        } else {
            _value &= ~KEY_DISTRIBUTION_ENCRYPTION;
        }
    }
    void set_identity(bool enabled = true) {
        if (enabled) {
            _value |= KEY_DISTRIBUTION_IDENTITY;
        } else {
            _value &= ~KEY_DISTRIBUTION_IDENTITY;
        }
    }
    void set_signing(bool enabled = true) {
        if (enabled) {
            _value |= KEY_DISTRIBUTION_SIGNING;
        } else {
            _value &= ~KEY_DISTRIBUTION_SIGNING;
        }
    }
    void set_link(bool enabled = true) {
        if (enabled) {
            _value |= KEY_DISTRIBUTION_LINK;
        } else {
            _value &= ~KEY_DISTRIBUTION_LINK;
        }
    }

    operator uint8_t() {
        return _value;
    }

    KeyDistribution operator&(const KeyDistribution& other) const {
        KeyDistribution result(this->value() & other.value());
        return result;
    }

    KeyDistribution& operator&=(const KeyDistribution& other) {
        this->_value = this->_value & other.value();
        return *this;
    }

    uint8_t value() const {
        return _value;
    }

private:
    uint8_t _value;
};

/**
 * Authentication mask as required by the SMP with convenient setters and getters,
 * use value() to get the octet you can use directly in the PDU.
 */
class AuthenticationMask {
public:
    enum AuthenticationFlags_t {
        AUTHENTICATION_BONDABLE               = 0x01,
        AUTHENTICATION_MITM                   = 0x04, /* 0x02 missing because bonding uses two bits */
        AUTHENTICATION_SECURE_CONNECTIONS     = 0x08,
        AUTHENTICATION_KEYPRESS_NOTIFICATION  = 0x10
    };

    AuthenticationMask() : _value(0) { }
    AuthenticationMask(uint8_t value) : _value(value) { }
    AuthenticationMask(bool bondable,
                       bool mitm,
                       bool secure_connections,
                       bool keypress) : _value(0) {
        set_bondable(bondable);
        set_mitm(mitm);
        set_secure_connections(secure_connections);
        set_keypress_notification(keypress);
    }

    bool get_bondable() const {
        return _value & AUTHENTICATION_BONDABLE;
    }
    bool get_mitm() const {
        return _value & AUTHENTICATION_MITM;
    }
    bool get_secure_connections() const {
        return _value & AUTHENTICATION_SECURE_CONNECTIONS;
    }
    bool get_keypress_notification() const {
        return _value & AUTHENTICATION_KEYPRESS_NOTIFICATION;
    }

    void set_bondable(bool enabled = true) {
        if (enabled) {
            _value |= AUTHENTICATION_BONDABLE;
        } else {
            _value &= ~AUTHENTICATION_BONDABLE;
        }
    }
    void set_mitm(bool enabled = true) {
        if (enabled) {
            _value |= AUTHENTICATION_MITM;
        } else {
            _value &= ~AUTHENTICATION_MITM;
        }
    }
    void set_secure_connections(bool enabled = true) {
        if (enabled) {
            _value |= AUTHENTICATION_SECURE_CONNECTIONS;
        } else {
            _value &= ~AUTHENTICATION_SECURE_CONNECTIONS;
        }
    }
    void set_keypress_notification(bool enabled = true) {
        if (enabled) {
            _value |= AUTHENTICATION_KEYPRESS_NOTIFICATION;
        } else {
            _value &= ~AUTHENTICATION_KEYPRESS_NOTIFICATION;
        }
    }

    operator uint8_t() {
        return _value;
    }
    uint8_t value() const {
        return _value;
    }

private:
    uint8_t _value;
};

/**
 * Handle events generated by ble::pal::SecurityManager
 */
template<class Impl>
class SecurityManagerEventHandler :
    public StaticInterface<Impl, SecurityManagerEventHandler>
{
    using StaticInterface<Impl, ble::pal::SecurityManagerEventHandler>::impl;

public:
    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * Request pairing. This is called on the slave in response to a request from the master.
     * Upper layer shall either send a pairing response (send_pairing_response)
     * or cancel the pairing procedure (cancel_pairing).
     *
     * @param[in] connection connection handle
     * @param[in] oob_data_flag is out of band data present
     * @param[in] authentication_requirements authentication requirements
     * @param[in] initiator_dist key distribution
     * @param[in] responder_dist key distribution
     */
    void on_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) {
        impl()->on_pairing_request_(
            connection,
            oob_data_flag,
            authentication_requirements,
            initiator_dist,
            responder_dist
        );
    }

    /**
     * Indicate that the pairing has failed.
     *
     * @note Any subsequent pairing procedure shall restart from the Pairing
     * Feature Exchange phase.
     * @param[in] connection connection handle
     * @param[in] error reason for the failed pairing
     */
    void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    ) {
        impl()->on_pairing_error_(connection, error);
    }

    /**
     * Indicate that the pairing has timed out.
     *
     * @param[in] connection connection handle
     */
    void on_pairing_timed_out(
        connection_handle_t connection
    ) {
        impl()->on_pairing_timed_out_(connection);
    }

    /**
     * Indicate that the pairing for the link has completed.
     *
     * @param[in] connection connection handle
     */
    void on_pairing_completed(
        connection_handle_t connection
    ) {
        impl()->on_pairing_completed_(connection);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    /**
     * Indicate that the authentication timeout time has elapsed
     * and we received no packets with a valid MIC in that time.
     *
     * @param[in] connection connection handle
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 6, Part B, 5.4
     */
    void on_valid_mic_timeout(
        connection_handle_t connection
    ) {
        impl()->on_valid_mic_timeout_(connection);
    }

    /**
     * Ask the stack to evaluate the security request received from the slave.
     * This might result in the stack enabling encryption, or pairing/re-pairing.
     *
     * @param[in] connection connection handle
     * @param[in] authentication authentication requirements from the slave
     */
    void on_slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    ) {
        impl()->on_slave_security_request_(connection, authentication);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * Inform the application of the result of an encryption request.
     * @note Do no call if request timed out, call on_link_encryption_request_timed_out
     * instead.
     *
     * @param[in] connection connection handle
     * @param[in] result encryption state of the link
     */
    void on_link_encryption_result(
        connection_handle_t connection,
        link_encryption_t result
    ) {
        impl()->on_link_encryption_result_(connection, result);
    }

    /**
     * Indicate that the encryption request failed due to timeout.
     *
     * @param[in] connection connection handle
     */
    void on_link_encryption_request_timed_out(
        connection_handle_t connection
    ) {
        impl()->on_link_encryption_request_timed_out_(connection);
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * Inform the application that should display a passkey.
     *
     * @param[in] connection connection handle
     * @param[in] passkey passkey to be displayed
     */
    void on_passkey_display(
        connection_handle_t connection,
        passkey_num_t passkey
    ) {
        impl()->on_passkey_display_(
            connection,
            passkey
        );
    }

    /**
     * Indicate that user confirmation is required to confirm matching
     * passkeys displayed on devices.
     *
     * @param[in] connection connection handle
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E, 7.7.42
     */
    void on_confirmation_request(
        connection_handle_t connection
    ) {
        impl()->on_confirmation_request_(connection);
    }

    /**
     * Request the passkey entered during pairing.
     *
     * @note shall be followed by: pal::SecurityManager::passkey_request_reply
     * @param[in] connection connection handle
     * or a cancellation of the procedure.
     */
    void on_passkey_request(
        connection_handle_t connection
    ) {
        impl()->on_passkey_request_(connection);
    }

    /**
     * Indicate that a key has been pressed by the peer.
     *
     * @param[in] connection connection handle
     * @param[in] keypress type of keypress event
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H, 3.5.8
     */
    void on_keypress_notification(
        connection_handle_t connection,
        Keypress_t keypress
    ) {
        impl()->on_keypress_notification_(connection, keypress);
    }

    /**
     * Request OOB data from the user application.
     *
     * @param[in] connection connection handle
     * @note shall be followed by: pal::SecurityManager::secure_connections_oob_request_reply
     * or a cancellation of the procedure.
     */
    void on_secure_connections_oob_request(
        connection_handle_t connection
    ) {
        impl()->on_secure_connections_oob_request_(connection);
    }

    /**
     * Request OOB data from the user application.
     *
     * @param[in] connection connection handle
     * @note shall be followed by: pal::SecurityManager::legacy_pairing_oob_request_reply
     * or a cancellation of the procedure.
     */
    void on_legacy_pairing_oob_request(
        connection_handle_t connection
    ) {
        impl()->on_legacy_pairing_oob_request_(connection);
    }

    /**
     * Send OOB data to the application for transport to the peer.
     *
     * @param[in] connection connection handle
     * @param[in] random random number used to generate the confirmation
     * @param[in] confirm confirmation value to be use for authentication
     *                    in secure connections pairing
     * @return BLE_ERROR_NONE or appropriate error code indicating the failure reason.
     */
    void on_secure_connections_oob_generated(
        const oob_lesc_value_t &random,
        const oob_confirm_t &confirm
    ) {
        impl()->on_secure_connections_oob_generated_(random, confirm);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * Store the results of key generation of the stage 2 of secure connections pairing
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 2.3.5.6.5
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key from the peer
     */
    void on_secure_connections_ltk_generated(
        connection_handle_t connection,
        const ltk_t &ltk
    ) {
        impl()->on_secure_connections_ltk_generated_(connection, ltk);
    }

    /**
     * Store the results of key distribution after LTK has been received.
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key from the peer
     */
    void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) {
        impl()->on_keys_distributed_ltk_(connection, ltk);
    }

    /**
     * Store the results of key distribution after EDIV and RAND has been received.
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key from the peer
     */
    void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        impl()->on_keys_distributed_ediv_rand_(connection, ediv, rand);
    }

    /**
     * Store the local key, if we are slave now or in the future
     * this will be used to encrypt.
     *
     * @param[in] connection connection handle
     * @param[in] ltk key sent to the peer
     */
    void on_keys_distributed_local_ltk(
        connection_handle_t connection,
        const ltk_t &ltk
    ) {
        impl()->on_keys_distributed_local_ltk_(connection, ltk);
    }

    /**
     * Store the EDIV and RAND that will be used to identify
     * the stored local LTK. if we are slave that LTK will be
     * used to encrypt, otherwise this will be stored to
     * be used in case of role reversal.
     *
     * @param[in] connection connection handle
     * @param[in] ediv identifies LTK
     * @param[in] rand identifies LTK
     */
    void on_keys_distributed_local_ediv_rand(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        impl()->on_keys_distributed_local_ediv_rand_(connection, ediv, rand);
    }

    /**
     * Store the results of key distribution after IRK has been received.
     *
     * @param[in] connection connection handle
     * @param[in] irk identity resolution key
     */
    void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t &irk
    ) {
        impl()->on_keys_distributed_irk_(connection, irk);
    }

    /**
     * Store the identity address of the peer after it has been distributed.
     *
     * @param[in] connection connection handle
     * @param[in] peer_identity_address_type public or private address indication
     * @param[in] peer_identity_address peer address
     */
    void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    ) {
        impl()->on_keys_distributed_bdaddr_(connection, peer_identity_address_type, peer_identity_address);
    }

    /**
     * Store the peer's CSRK after it has been distributed.
     *
     * @param[in] connection connection handle
     * @param[in] csrk signing key
     */
    void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t &csrk
    ) {
        impl()->on_keys_distributed_csrk_(connection, csrk);
    }

    /**
     * Request the LTK since the peer is asking us to encrypt the link. We need to
     * provide the LTK based on the EDIV and RAND provided by the other side. This
     * is called on the slave.
     *
     * @param[in] connection connection handle
     * @param[in] ediv identifies LTK
     * @param[in] rand identifies LTK
     */
    void on_ltk_request(
        connection_handle_t connection,
        const ediv_t &ediv,
        const rand_t &rand
    ) {
        impl()->on_ltk_request_(connection, ediv, rand);
    }

    /**
     * Request the LTK since the peer is asking us to encrypt the link.
     * @note No EDIV or RAND is provided as this requests a secure
     * connections LTK where their values are all zeroes
     *
     * @param[in] connection connection handle
     */
    void on_ltk_request(
        connection_handle_t connection
    ) {
        impl()->on_ltk_request_(connection);
    }
};


/**
 * Adaptation layer of the Security Manager.
 */
template<class Impl, class EventHandler>
class SecurityManager : private mbed::NonCopyable<SecurityManager<Impl, EventHandler> > {

    Impl* impl() {
        return static_cast<Impl*>(this);
    }

public:
    SecurityManager() : _pal_event_handler(NULL) { };

    ~SecurityManager() { };

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    /**
     * Initialise stack. Called before first use.
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t initialize() {
        return impl()->initialize_();
    }

    /**
     * Finalise all actions. Called before shutdown.
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t terminate() {
        return impl()->terminate_();
    }

    /**
     * Reset to same state as after initialize.
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t reset() {
        return impl()->reset_();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //
    /**
     * Return the number of address translation entries that can be stored by the
     * subsystem.
     *
     * @warning: The number of entries is considered fixed.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.41
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    uint8_t read_resolving_list_capacity() {
        return impl()->read_resolving_list_capacity_();
    }

    /**
     * Add a device definition into the resolving list of the LE subsystem.
     *
     * @param[in] peer_identity_address_type public/private indicator
     * @param[in] peer_identity_address address of the device whose entry is to be added
     * @param[in] peer_irk peer identity resolving key
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.38
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    ) {
        return impl()->add_device_to_resolving_list_(
            peer_identity_address_type,
            peer_identity_address,
            peer_irk
        );
    }

    /**
     * Add a device definition from the resolving list of the LE subsystem.
     *
     * @param[in] peer_identity_address_type public/private indicator
     * @param[in] peer_identity_address address of the device whose entry is to be removed
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.39
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    ) {
        return impl()->remove_device_from_resolving_list_(
            peer_identity_address_type,
            peer_identity_address
        );
    }

    /**
     * Remove all devices from the resolving list.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.40
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t clear_resolving_list() {
        return impl()->clear_resolving_list_();
    }

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * Send a pairing request to a slave.
     *
     * @param[in] connection connection handle
     * @param[in] oob_data_flag is oob data present
     * @param[in] authentication_requirements authentication requirements
     * @param[in] initiator_dist key distribution
     * @param[in] responder_dist key distribution
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.1
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t send_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) {
        return impl()->send_pairing_request_(
            connection,
            oob_data_flag,
            authentication_requirements,
            initiator_dist,
            responder_dist
        );
    }

    /**
     * Send a pairing response to a master.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.2*
     * @param[in] connection connection handle
     * @param[in] oob_data_flag is oob data present
     * @param[in] authentication_requirements authentication requirements
     * @param[in] initiator_dist key distribution
     * @param[in] responder_dist key distribution
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t send_pairing_response(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) {
        return impl()->send_pairing_response_(
            connection,
            oob_data_flag,
            authentication_requirements,
            initiator_dist,
            responder_dist
        );
    }

    /**
     * Cancel an ongoing pairing.
     *
     * @param[in] connection connection handle
     * @param[in] reason pairing failure error
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.5
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t cancel_pairing(
        connection_handle_t connection,
        pairing_failure_t reason
    ) {
        return impl()->cancel_pairing_(
            connection,
            reason
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * Check if the Secure Connections feature is supported by the stack and controller.
     *
     * @param[out] enabled true if SC are supported
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t get_secure_connections_support(
        bool &enabled
    ) {
        return impl()->get_secure_connections_support_(enabled);
    }

    /**
     * Set the IO capability that will be used during pairing feature exchange.
     *
     * @param[in] io_capability type of IO capabilities available on the local device
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_io_capability(
        io_capability_t io_capability
    ) {
        return impl()->set_io_capability_(io_capability);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * Set the time after which an event will be generated unless we received a packet with
     * a valid MIC.
     *
     * @param[in] connection connection handle
     * @param[in] timeout_in_10ms time measured in units of 10 milliseconds
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_authentication_timeout(
        connection_handle_t connection,
        uint16_t timeout_in_10ms
    ) {
        return impl()->set_authentication_timeout_(
            connection,
            timeout_in_10ms
        );
    }

    /**
     * Get the time after which an event will be generated unless we received a packet with
     * a valid MIC.
     *
     * @param[in] connection connection handle
     * @param[out] timeout_in_10ms time measured in units of 10 milliseconds
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t get_authentication_timeout(
        connection_handle_t connection,
        uint16_t &timeout_in_10ms
    ) {
        return impl()->get_authentication_timeout_(
            connection,
            timeout_in_10ms
        );
    }

    /**
     * Set the key size boundaries that will be used during pairing feature
     * exchange.
     *
     * @param[in] min_encryption_key_size The minimum encryption key size in bytes
     * required for pairing. This value shall be in the range [7 : 16].
     *
     * @param[in] max_encryption_key_size The maximum encryption key size in bytes
     * required for pairing. This value shall be in the range
     * [min_encryption_key_size : 16].
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_encryption_key_requirements(
        uint8_t min_encryption_key_size,
        uint8_t max_encryption_key_size
    ) {
        return impl()->set_encryption_key_requirements_(
            min_encryption_key_size,
            max_encryption_key_size
        );
    }

    /**
     * Request change of security level from the master. This is called by the slave when
     * it needs to elevate the security level as it can't change it itself. This will be
     * received by the master who will take the decision about what action to take
     * (encryption, pairing, re-paring).
     *
     * @param[in] connection connection handle
     * @param[in] authentication authentication requirements
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    ) {
        return impl()->slave_security_request_(
            connection,
            authentication
        );
    }

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * Enabled encryption using the LTK given. The EDIV and RAND will be sent to the peer and
     * used to identify the LTK. This is called by the master. This will refresh the key if
     * enabled on an already encrypted link.
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key from the peer
     * @param[in] ediv encryption diversifier from the peer
     * @param[in] rand random value from the peer
     * @param[in] mitm does the LTK have man in the middle protection
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        const rand_t &rand,
        const ediv_t &ediv,
        bool mitm
    ) {
        return impl()->enable_encryption_(
            connection,
            ltk,
            rand,
            ediv,
            mitm
        );
    }

    /**
     * Enabled encryption using the LTK given on a connection established with secure
     * connections pairing.
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key from the peer
     * @param[in] mitm does the LTK have man in the middle protection
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm
    ) {
        return impl()->enable_encryption_(
            connection,
            ltk,
            mitm
        );
    }

    /**
     * Encrypt data with a given key. This uses the facility on the controller to
     * perform the encryption.
     *
     * @param[in] key encryption key
     * @param[in,out] data data to be encrypted, if successful contains the result
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t encrypt_data(
        const byte_array_t<16> &key,
        encryption_block_t &data
    ) {
        return impl()->encrypt_data_(key, data);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    ble_error_t set_private_address_timeout(
        uint16_t timeout_in_seconds
    ) {
        return impl()->set_private_address_timeout(timeout_in_seconds);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * Set the LTK that is to be used for encryption.
     *
     * @param[in] connection connection handle
     * @param[in] ltk long term key
     * @param[in] mitm does the LTK have man in the middle protection
     * @param[in] secure_connections is this a secure_connections pairing
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_ltk(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm,
        bool secure_connections
    ) {
        return impl()->set_ltk_(connection, ltk, mitm, secure_connections);
    }

    /**
     * Inform the stack we don't have the LTK.
     *
     * @param[in] connection connection handle
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_ltk_not_found(
        connection_handle_t connection
    ) {
        return impl()->set_ltk_not_found_(connection);
    }

    /**
     * Set the local IRK.
     *
     * @param[in] irk identity resolution key
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_irk(
        const irk_t &irk
    ) {
        return impl()->set_irk_(irk);
    }

    /**
     * Set the local CSRK.
     *
     * @param[in] csrk local signing key
     * @param[in] sign_counter local signing counter
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_csrk(
        const csrk_t &csrk,
        sign_count_t sign_counter
    ) {
        return impl()->set_csrk_(csrk, sign_counter);
    }

    /**
     * Set the peer CSRK for particular connection.
     *
     * @param[in] connection connection handle
     * @param[in] csrk signing key
     * @param[in] authenticated is the CSRK authenticated
     * @param[in] sign_counter signing counter
     * @retval BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_peer_csrk(
        connection_handle_t connection,
        const csrk_t &csrk,
        bool authenticated,
        sign_count_t sign_counter
    ) {
        return impl()->set_peer_csrk_(
            connection,
            csrk,
            authenticated,
            sign_counter
        );
    }

    ble_error_t remove_peer_csrk(connection_handle_t connection) {
        return impl()->remove_peer_csrk_(connection);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * Generate and return 8 octets of random data compliant with [FIPS PUB 140-2]
     *
     * @param[out] random_data returns 8 octets of random data
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part H 2
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t get_random_data(
        byte_array_t<8> &random_data
    ) {
        return impl()->get_random_data_(random_data);
    }

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * Set the default passkey that will be used when the SM needs a passkey to
     * be displayed.
     *
     * By default, the pal security manager generates a random passkey when a
     * passkey has to be displayed by the application. A call to this function
     * with a valid passkey alter this behaviour and the SecurityManager shall
     * pass the passkey set into SecurityManagerEvent::on_passkey_display .
     *
     * A call to this function with a zero value will reset the behaviour and
     * indicates to the security manager that passkeys passed to
     * SecurityManagerEvent::on_passkey_display shall be randomly generated.
     *
     * @param[in] passkey Set the passkey that shall be used by the security
     * manager when SecurityManagerEvent::on_passkey_display is called. If
     * passkey is set to 0 then the security manager generates a random
     * passkey every time it calls SecurityManagerEvent::on_passkey_display.
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t set_display_passkey(
        passkey_num_t passkey
    ) {
        return impl()->set_display_passkey_(passkey);
    }

    /**
     * Reply to a passkey request received from the SecurityManagerEventHandler.
     *
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t passkey_request_reply(
        connection_handle_t connection,
        passkey_num_t passkey
    ) {
        return impl()->passkey_request_reply_(connection, passkey);
    }

    /**
     * Reply to a Secure Connections oob data request received from the SecurityManagerEventHandler.
     *
     * @param[in] connection connection handle
     * @param[in] local_random local random number used for the last oob exchange
     * @param[in] peer_random random number used to generate the confirmation on peer
     * @param[in] peer_confirm confirmation value to be use for authentication
     *                         in secure connections pairing
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t secure_connections_oob_request_reply(
        connection_handle_t connection,
        const oob_lesc_value_t &local_random,
        const oob_lesc_value_t &peer_random,
        const oob_confirm_t &peer_confirm
    ) {
        return impl()->secure_connections_oob_request_reply_(
            connection,
            local_random,
            peer_random,
            peer_confirm
        );
    }

    /**
     * Reply to a legacy pairing oob data request received from the SecurityManagerEventHandler.
     *
     * @param[in] connection connection handle
     * @param[in] oob_data pointer to out of band data
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t legacy_pairing_oob_request_reply(
        connection_handle_t connection,
        const oob_tk_t &oob_data
    ) {
        return impl()->legacy_pairing_oob_request_reply_(connection, oob_data);
    }

    /**
     * Notify the stack that the user has confirmed the values during numerical
     * comparison stage of pairing.
     *
     * @param[in] connection connection handle
     * @param[in] confirmation true if the user indicated the numbers match
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t confirmation_entered(
        connection_handle_t connection,
        bool confirmation
    ) {
        return impl()->confirmation_entered_(connection, confirmation);
    }

    /**
     * Notify the stack that the user pressed a key. This will be sent to the peer and create
     * an appropriate event there if the keypress protocol is enabled.
     *
     * @param[in] connection connection handle
     * @param[in] keypress type of keypress event
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t send_keypress_notification(
        connection_handle_t connection,
        Keypress_t keypress
    ) {
        return impl()->send_keypress_notification_(connection, keypress);
    }

    /**
     * Generate local OOB data to be sent to the application which sends it to the peer.
     * @return BLE_ERROR_NONE On success, else an error code indicating reason for failure
     */
    ble_error_t generate_secure_connections_oob() {
        return impl()->generate_secure_connections_oob_();
    }

    /* Entry points for the underlying stack to report events back to the user. */
public:
    /**
     * Sets the event handler that us called by the PAL porters to notify the stack of events
     * which will in turn be passed onto the user application when appropriate.
     *
     * @param[in] event_handler the new event handler interface implementation. Memory
     * owned by caller who is responsible for updating this pointer if interface changes.
     */
    void set_event_handler(
        EventHandler *event_handler
    ) {
        _pal_event_handler = event_handler;
    }

    EventHandler* get_event_handler() {
        return _pal_event_handler;
    }

private:
    EventHandler *_pal_event_handler;

};

} /* namespace pal */
} /* namespace ble */

#endif /* MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_ */
