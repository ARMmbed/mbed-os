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

#include "ble/pal/PalSecurityManager.h"

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
        address_t peer_identity_address,
        irk_t peer_irk,
        irk_t local_irk
    );

    /**
     * @see ::ble::pal::SecurityManager::remove_device_from_resolving_list
     */
    virtual ble_error_t remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        address_t peer_identity_address
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
        bool &enabled, bool &secure_connections_only
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

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @see ::ble::pal::SecurityManager::enable_encryption
     */
    virtual ble_error_t enable_encryption(connection_handle_t connection);

    /**
     * @see ::ble::pal::SecurityManager::disable_encryption
     */
    virtual ble_error_t disable_encryption(connection_handle_t connection);

    /**
     * @see ::ble::pal::SecurityManager::get_encryption_status
     */
    virtual ble_error_t get_encryption_status(
        connection_handle_t connection, LinkSecurityStatus_t &status
    );

    /**
     * @see ::ble::pal::SecurityManager::get_encryption_key_size
     */
    virtual ble_error_t get_encryption_key_size(
        connection_handle_t, uint8_t &bitsize
    );

    /**
     * @see ::ble::pal::SecurityManager::refresh_encryption_key
     */
    virtual ble_error_t refresh_encryption_key(connection_handle_t connection);

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
    virtual ble_error_t set_ltk(connection_handle_t connection, ltk_t ltk);

    /**
     * @see ::ble::pal::SecurityManager::set_irk
     */
    virtual ble_error_t set_irk(const irk_t& irk);

    /**
     * @see ::ble::pal::SecurityManager::set_csrk
     */
    virtual ble_error_t set_csrk(const csrk_t& csrk);

    /**
     * @see ::ble::pal::SecurityManager::generate_irk
     */
    virtual ble_error_t generate_irk();

    /**
     * @see ::ble::pal::SecurityManager::generate_csrk
     */
    virtual ble_error_t generate_csrk();

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_request
     */
    virtual ble_error_t send_pairing_request(
        connection_handle_t connection,
        io_capability_t io_capability,
        bool oob_data_flag,
        authentication_t authentication_requirements,
        uint8_t maximum_encryption_key_size,
        key_distribution_t initiator_dist,
        key_distribution_t responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_response
     */
    virtual ble_error_t send_pairing_response(
        connection_handle_t connection,
        io_capability_t io_capability,
        bool oob_data_flag,
        authentication_t authentication_requirements,
        uint8_t maximum_encryption_key_size,
        key_distribution_t initiator_dist,
        key_distribution_t responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::cancel_pairing
     */
    virtual ble_error_t cancel_pairing(
        connection_handle_t connection, pairing_failure_t reason
    );

    /**
     * @see ::ble::pal::SecurityManager::request_authentication
     */
    virtual ble_error_t request_authentication(connection_handle_t connection);

    /**
     * @see ::ble::pal::SecurityManager::get_random_data
     */
    virtual ble_error_t get_random_data(random_data_t &random_data);

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * @see ::ble::pal::SecurityManager::passkey_request_reply
     */
    virtual ble_error_t passkey_request_reply(
        connection_handle_t connection, const passkey_num_t passkey
    );

    /**
     * @see ::ble::pal::SecurityManager::oob_data_request_reply
     */
    virtual ble_error_t oob_data_request_reply(
        connection_handle_t connection, const oob_data_t& oob_data
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

    // singleton of nordic Security Manager
    static nRF5xSecurityManager& get_security_manager();

    // Event handler
    // FIXME: set proper event handling type
    static bool sm_handler();
};

} // nordic
} // vendor
} // pal
} // ble

#endif /* NRF5X_PAL_SECURITY_MANAGER_ */
