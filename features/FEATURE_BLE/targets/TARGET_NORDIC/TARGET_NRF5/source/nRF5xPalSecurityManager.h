/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef NRF5X_PAL_SECURITY_MANAGER_
#define NRF5X_PAL_SECURITY_MANAGER_

#include "ble/BLETypes.h"
#include "ble/pal/PalSecurityManager.h"
#include "nrf_ble.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

class nRF5xSecurityManager : public ::ble::pal::SecurityManager {
public:
    nRF5xSecurityManager();

    virtual ~nRF5xSecurityManager();

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    /**
     * @see ::ble::pal::SecurityManager::initialize
     */
    virtual ble_error_t initialize();

    /**
     * @see ::ble::pal::SecurityManager::terminate
     */
    virtual ble_error_t terminate();

    /**
     * @see ::ble::pal::SecurityManager::reset
     */
    virtual ble_error_t reset() ;

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //

    /**
     * @see ::ble::pal::SecurityManager::read_resolving_list_capacity
     */
    virtual uint8_t read_resolving_list_capacity();

    /**
     * @see ::ble::pal::SecurityManager::add_device_to_resolving_list
     */
    virtual ble_error_t add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    );

    /**
     * @see ::ble::pal::SecurityManager::remove_device_from_resolving_list
     */
    virtual ble_error_t remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    );

    /**
     * @see ::ble::pal::SecurityManager::clear_resolving_list
     */
    virtual ble_error_t clear_resolving_list();

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * @see ::ble::pal::SecurityManager::set_secure_connections_support
     */
    virtual ble_error_t set_secure_connections_support(
        bool enabled, bool secure_connections_only = false
    );

    /**
     * @see ::ble::pal::SecurityManager::get_secure_connections_support
     */
    virtual ble_error_t get_secure_connections_support(
        bool &enabled
    );

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * @see ::ble::pal::SecurityManager::set_authentication_timeout
     */
    virtual ble_error_t set_authentication_timeout(
        connection_handle_t, uint16_t timeout_in_10ms
    );

    /**
     * @see ::ble::pal::SecurityManager::get_authentication_timeout
     */
    virtual ble_error_t get_authentication_timeout(
        connection_handle_t, uint16_t &timeout_in_10ms
    );

    virtual ble_error_t slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @see ::ble::pal::SecurityManager::enable_encryption
     */
    virtual ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        const rand_t &rand,
        const ediv_t &ediv,
        bool mitm
    );

    /**
     * @see ::ble::pal::SecurityManager::enable_encryption
     */
    virtual ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm
    );

    /**
     * @see ::ble::pal::SecurityManager::encrypt_data
     */
    virtual ble_error_t encrypt_data(
        const byte_array_t<16> &key,
        encryption_block_t &data
    );

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    /**
     * @see ::ble::pal::SecurityManager::set_private_address_timeout
     */
    virtual ble_error_t set_private_address_timeout(uint16_t timeout_in_seconds);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * @see ::ble::pal::SecurityManager::set_ltk
     */
    virtual ble_error_t set_ltk(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm,
        bool secure_connections
    );

    /**
     * @see ::ble::pal::SecurityManager::set_ltk_not_found
     */
    virtual ble_error_t set_ltk_not_found(
        connection_handle_t connection
    );

    /**
     * @see ::ble::pal::SecurityManager::set_irk
     */
    virtual ble_error_t set_irk(const irk_t &irk);

    /**
     * @see ::ble::pal::SecurityManager::set_csrk
     */
    virtual ble_error_t set_csrk(const csrk_t &csrk);

    /**
     * @see ::ble::pal::SecurityManager::generate_public_key
     */
    virtual ble_error_t generate_public_key();

    ////////////////////////////////////////////////////////////////////////////
    // Global parameters
    //

    /**
     * @see ::ble::pal::SecurityManager::set_display_passkey
     */
    virtual ble_error_t set_display_passkey(passkey_num_t passkey);

    /**
     * @see ::ble::pal::SecurityManager::set_io_capability
     */
    virtual ble_error_t set_io_capability(io_capability_t io_capability);

    /**
     * @see ::ble::pal::SecurityManager::set_encryption_key_requirements
     */
    virtual ble_error_t set_encryption_key_requirements(
        uint8_t min_encryption_key_size,
        uint8_t max_encryption_key_size
    );

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_request
     */
    virtual ble_error_t send_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_response
     */
    virtual ble_error_t send_pairing_response(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::cancel_pairing
     */
    virtual ble_error_t cancel_pairing(
        connection_handle_t connection, pairing_failure_t reason
    );

    /**
     * @see ::ble::pal::SecurityManager::get_random_data
     */
    virtual ble_error_t get_random_data(byte_array_t<8> &random_data);

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * @see ::ble::pal::SecurityManager::passkey_request_reply
     */
    virtual ble_error_t passkey_request_reply(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /**
     * @see ::ble::pal::SecurityManager::legacy_pairing_oob_data_request_reply
     */
    virtual ble_error_t legacy_pairing_oob_data_request_reply(
        connection_handle_t connection,
        const oob_tk_t &oob_data
    );

    /**
     * @see ::ble::pal::SecurityManager::confirmation_entered
     */
    virtual ble_error_t confirmation_entered(
        connection_handle_t connection, bool confirmation
    );

    /**
     * @see ::ble::pal::SecurityManager::send_keypress_notification
     */
    virtual ble_error_t send_keypress_notification(
        connection_handle_t connection, Keypress_t keypress
    );

    /**
     * @see ::ble::pal::SecurityManager::oob_data_verified
     */
    virtual ble_error_t oob_data_verified(
        connection_handle_t connection,
        const oob_lesc_value_t &local_random,
        const oob_lesc_value_t &peer_random
    );

    // singleton of nordic Security Manager
    static nRF5xSecurityManager& get_security_manager();

    // Event handler
    bool sm_handler(const ble_evt_t *evt);

private:
    bool _use_legacy_pairing;
    bool _use_secure_connections;
    bool _use_default_passkey;
    passkey_num_t _default_passkey;
    
    irk_t _irk;
    csrk_t _csrk;

    io_capability_t _io_capability;
    uint8_t _min_encryption_key_size;
    uint8_t _max_encryption_key_size;

    // The softdevice requires us to manage memory for these keys
    ble_gap_enc_key_t _sp_own_enc_key;
    ble_gap_id_key_t _sp_own_id_key;
    ble_gap_sign_info_t _sp_own_sign_key;
    ble_gap_lesc_p256_pk_t _sp_own_pk;

    ble_gap_enc_key_t _sp_peer_enc_key;
    ble_gap_id_key_t _sp_peer_id_key;
    ble_gap_sign_info_t _sp_peer_sign_key;
    ble_gap_lesc_p256_pk_t _sp_peer_pk;
};

} // nordic
} // vendor
} // pal
} // ble

#endif /* NRF5X_PAL_SECURITY_MANAGER_ */
