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

#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "ble/BLETypes.h"
#include "ble/BLEProtocol.h"
#include "ble/SecurityManager.h"
#include "ble/pal/GapTypes.h"

namespace ble {
namespace pal {

typedef SecurityManager::SecurityCompletionStatus_t SecurityCompletionStatus_t;
typedef SecurityManager::SecurityMode_t SecurityMode_t;
typedef SecurityManager::LinkSecurityStatus_t LinkSecurityStatus_t;
typedef SecurityManager::Keypress_t Keypress_t;

/* please use typedef for porting not the types directly */

typedef uint8_t oob_data_t[16];

typedef uint8_t irk_t[16];
typedef uint8_t csrk_t[16];
typedef uint8_t ltk_t[16];
typedef uint8_t ediv_t[8];
typedef uint8_t rand_t[2];
typedef uint8_t random_data_t[8];
typedef uint32_t passkey_num_t;

class PasskeyAsci {
public:
    static const uint8_t NUMBER_OFFSET = '0';

    PasskeyAsci() {
        memset(asci, NUMBER_OFFSET, SecurityManager::PASSKEY_LEN);
    }
    PasskeyAsci(const uint8_t* passkey) {
        if (passkey) {
            memcpy(asci, passkey, SecurityManager::PASSKEY_LEN);
        } else {
            memset(asci, NUMBER_OFFSET, SecurityManager::PASSKEY_LEN);
        }
    }
    PasskeyAsci(passkey_num_t passkey) {
        for (int i = 5, m = 100000; i >= 0; --i, m /= 10) {
            uint32_t result = passkey / m;
            asci[i] = NUMBER_OFFSET + result;
            passkey -= result;
        }
    }
    operator passkey_num_t() {
        return to_num(asci);
    }

    static uint32_t to_num(const uint8_t *asci) {
        uint32_t passkey = 0;
        for (size_t i = 0, m = 1; i < SecurityManager::PASSKEY_LEN; ++i, m *= 10) {
            passkey += (asci[i] - NUMBER_OFFSET) * m;
        }
        return passkey;
    }
    uint8_t asci[SecurityManager::PASSKEY_LEN];
};

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

    bool get_encryption() {
        return _value & KEY_DISTRIBUTION_ENCRYPTION;
    }
    bool get_identity() {
        return _value & KEY_DISTRIBUTION_IDENTITY;
    }
    bool get_signing() {
        return _value & KEY_DISTRIBUTION_SIGNING;
    }
    bool get_link() {
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
    uint8_t value() {
        return _value;
    }

private:
    uint8_t _value;
};

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

    bool get_bondable() {
        return _value & AUTHENTICATION_BONDABLE;
    }
    bool get_mitm() {
        return _value & AUTHENTICATION_MITM;
    }
    bool get_secure_connections() {
        return _value & AUTHENTICATION_SECURE_CONNECTIONS;
    }
    bool get_keypress_notification() {
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
    uint8_t value() {
        return _value;
    }

private:
    uint8_t _value;
};

/**
 * Handle events generated by ble::pal::SecurityManager
 */
class SecurityManagerEventHandler {
public:
    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * Called upon reception of a pairing request.
     *
     * Upper layer shall either send a pairing response (send_pairing_response)
     * or cancel the pairing procedure (cancel_pairing).
     */
    virtual void on_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) = 0;

    /**
     * Called upon reception of Pairing failed.
     *
     * @note Any subsequent pairing procedure shall restart from the Pairing
     * Feature Exchange phase.
     */
    virtual void on_pairing_error(
        connection_handle_t connection,
        pairing_failure_t error
    ) = 0;

    /**
     * To indicate that the pairing for the link has completed.
     */
    virtual void on_pairing_completed(
        connection_handle_t connection
    ) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Security
    //

    /**
     * reports change of encryption status or result of encryption request
     */
    virtual void on_link_encryption_result(
        connection_handle_t connection,
        bool encrypted
    ) = 0;

    /**
     * To indicate that the authentication timeout has elapsed
     * and we received no packets with a valid MIC
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 6, Part B, 5.4
     */
    virtual void on_valid_mic_timeout(connection_handle_t connection) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * Called when the application should display a passkey.
     */
    virtual void on_passkey_display(
        connection_handle_t connection,
        const passkey_num_t passkey
    ) = 0;

    /**
     * To indicate user confirmation is require to confirm matching
     * passkeys displayed on devices
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E, 7.7.42
     */
    virtual void on_confirmation_request(connection_handle_t connection) = 0;

    /**
     * Request the passkey entered during pairing.
     *
     * @note shall be followed by: pal::SecurityManager::passkey_request_reply
     * or a cancellation of the procedure.
     */
    virtual void on_passkey_request(connection_handle_t connection) = 0;

    /**
     * To indicate that the peer has pressed a button
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H, 3.5.8
     */
    virtual void on_keypress_notification(connection_handle_t connection, SecurityManager::Keypress_t keypress) = 0;

    /**
     * To indicate that the user application needs to provide OOB data
     */
    virtual void on_legacy_pairing_oob_request(connection_handle_t connection) = 0;

    /**
     * Request oob data entered during pairing
     *
     * @note shall be followed by: pal::SecurityManager::oob_data_request_reply
     * or a cancellation of the procedure.
     */
    virtual void on_oob_request(connection_handle_t connection) = 0;


    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual void on_keys_distributed(
        connection_handle_t handle,
        advertising_peer_address_type_t peer_address_type,
        const address_t &peer_address,
        const ediv_t ediv,
        const rand_t rand,
        const ltk_t ltk,
        const irk_t irk,
        const csrk_t csrk
    ) = 0;

    virtual void on_keys_distributed_ltk(
        connection_handle_t connection,
        const ltk_t ltk
    ) = 0;

    virtual void on_keys_distributed_ediv_rand(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) = 0;

    virtual void on_keys_distributed_irk(
        connection_handle_t connection,
        const irk_t irk
    ) = 0;

    virtual void on_keys_distributed_bdaddr(
        connection_handle_t connection,
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    ) = 0;

    virtual void on_keys_distributed_csrk(
        connection_handle_t connection,
        const csrk_t csrk
    ) = 0;

    virtual void on_ltk_request(
        connection_handle_t connection,
        const ediv_t ediv,
        const rand_t rand
    ) = 0;
};

/**
 * Adaptation layer of the Security Manager.
 *
 *
 * FIXME: ADD API in the pal to set default passkey!
 *
 */
class SecurityManager : private mbed::NonCopyable<SecurityManager> {
public:
    SecurityManager() : _pal_event_handler(NULL) { };

    virtual ~SecurityManager() { };

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    virtual ble_error_t initialize() = 0;

    virtual ble_error_t terminate() = 0;

    virtual ble_error_t reset()  = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //

    /**
     * Return the number of address translation entries that can be stored by the
     * subsystem.
     *
     * @warning: The number of entries is considered fixed.
     *
     * see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.41
     */
    virtual uint8_t read_resolving_list_capacity() = 0;

    /**
     * Add a device definition into the resolving list of the LE subsystem.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.38
     */
    virtual ble_error_t add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        address_t peer_identity_address,
        const irk_t peer_irk,
        const irk_t local_irk
    ) = 0;


    /**
     * Add a device definition from the resolving list of the LE subsystem.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.39
     */
    virtual ble_error_t remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    ) = 0;

    /**
     * Remove all devices from the resolving list.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part E: 7.8.40
     */
    virtual ble_error_t clear_resolving_list() = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    virtual ble_error_t get_secure_connections_support(bool &enabled) = 0;

    virtual ble_error_t set_io_capability(io_capability_t io_capability) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    virtual ble_error_t set_authentication_timeout(
        connection_handle_t,
        uint16_t timeout_in_10ms
    ) = 0;

    virtual ble_error_t get_authentication_timeout(
        connection_handle_t,
        uint16_t &timeout_in_10ms
    ) = 0;

    virtual ble_error_t set_encryption_key_requirements(
        uint16_t min_encryption_key_bitsize,
        uint16_t max_encryption_key_bitsize
    ) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    virtual ble_error_t enable_encryption(connection_handle_t connection) = 0;

    virtual ble_error_t disable_encryption(connection_handle_t connection) = 0;

    virtual ble_error_t get_encryption_key_size(
        connection_handle_t connection,
        uint8_t &bitsize
    ) = 0;

    virtual ble_error_t refresh_encryption_key(connection_handle_t connection) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    virtual ble_error_t set_private_address_timeout(uint16_t timeout_in_seconds) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    virtual ble_error_t set_ltk(connection_handle_t connection, const ltk_t ltk) = 0;

    /**
     * Set the local IRK
     */
    virtual ble_error_t set_irk(const irk_t irk) = 0;

    /**
     * Set the local csrk
     */
    virtual ble_error_t set_csrk(const csrk_t csrk) = 0;

    virtual ble_error_t generate_irk() = 0;

    virtual ble_error_t generate_csrk() = 0;

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * Send a pairing request to a slave.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.1
     */
    virtual ble_error_t send_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * Send a pairing response to a master.
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.2
     */
    virtual ble_error_t send_pairing_response(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) = 0;

    /**
     * Cancel an ongoing pairing
     *
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 3, Part H - 3.5.5
     */
    virtual ble_error_t cancel_pairing(
        connection_handle_t connection, pairing_failure_t reason
    ) = 0;

    virtual ble_error_t request_authentication(connection_handle_t connection) = 0;

    /**
     * Generate and return 8 octets of random data compliant with [FIPS PUB 140-2]
     *
     * @param[out] random_data returns 8 octets of random data
     * @see BLUETOOTH SPECIFICATION Version 5.0 | Vol 2, Part H 2
     */
    virtual ble_error_t get_random_data(random_data_t &random_data) = 0;

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    virtual ble_error_t set_display_passkey(const passkey_num_t passkey) = 0;

    /**
     * Reply to a passkey request received from the SecurityManagerEventHandler.
     */
    virtual ble_error_t passkey_request_reply(
        connection_handle_t connection, const passkey_num_t passkey
    ) = 0;

    /**
     * Reply to an oob data request received from the SecurityManagerEventHandler.
     */
    virtual ble_error_t oob_data_request_reply(
        connection_handle_t connection, const oob_data_t oob_data
    ) = 0;

    virtual ble_error_t confirmation_entered(
        connection_handle_t connection, bool confirmation
    ) = 0;

    virtual ble_error_t send_keypress_notification(
        connection_handle_t connection, Keypress_t keypress
    ) = 0;

    /* Entry points for the underlying stack to report events back to the user. */
public:
    void set_event_handler(SecurityManagerEventHandler *event_handler) {
        _pal_event_handler = event_handler;
    }


protected:
    SecurityManagerEventHandler* get_event_handler() {
        return _pal_event_handler;
    }

private:
    SecurityManagerEventHandler *_pal_event_handler;

};

} /* namespace pal */
} /* namespace ble */

#endif /* MBED_OS_FEATURES_FEATURE_BLE_BLE_PAL_PALSM_H_ */
