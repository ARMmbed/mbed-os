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
#include "nRF5xCrypto.h"


namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

template <class EventHandler>
class nRF5xSecurityManager : public ::ble::pal::SecurityManager<nRF5xSecurityManager<EventHandler>, EventHandler> {
public:
    typedef ::ble::pal::SecurityManager<
        nRF5xSecurityManager<EventHandler>,
        EventHandler
    > Base;

    nRF5xSecurityManager();

    ~nRF5xSecurityManager();

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    /**
     * @see ::ble::pal::SecurityManager::initialize
     */
    ble_error_t initialize_();

    /**
     * @see ::ble::pal::SecurityManager::terminate
     */
    ble_error_t terminate_();

    /**
     * @see ::ble::pal::SecurityManager::reset
     */
    ble_error_t reset_() ;

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //

    /**
     * @see ::ble::pal::SecurityManager::read_resolving_list_capacity
     */
    uint8_t read_resolving_list_capacity_();

    /**
     * @see ::ble::pal::SecurityManager::add_device_to_resolving_list
     */
    ble_error_t add_device_to_resolving_list_(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    );

    /**
     * @see ::ble::pal::SecurityManager::remove_device_from_resolving_list
     */
    ble_error_t remove_device_from_resolving_list_(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    );

    /**
     * @see ::ble::pal::SecurityManager::clear_resolving_list
     */
    ble_error_t clear_resolving_list_();

    /**
     * Return the IRKs present in the resolving list
     * @param count The number of entries present in the resolving list.
     * @param pointer to the first entry of the resolving list.
     */
    Span<ble_gap_id_key_t> get_resolving_list();

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_request
     */
    ble_error_t send_pairing_request_(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::send_pairing_response
     */
    ble_error_t send_pairing_response_(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::pal::SecurityManager::cancel_pairing
     */
    ble_error_t cancel_pairing_(
        connection_handle_t connection, pairing_failure_t reason
    );


    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * @see ::ble::pal::SecurityManager::get_secure_connections_support
     */
    ble_error_t get_secure_connections_support_(
        bool &enabled
    );

    /**
     * @see ::ble::pal::SecurityManager::set_io_capability
     */
    ble_error_t set_io_capability_(io_capability_t io_capability);

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * @see ::ble::pal::SecurityManager::set_authentication_timeout
     */
    ble_error_t set_authentication_timeout_(
        connection_handle_t, uint16_t timeout_in_10ms
    );

    /**
     * @see ::ble::pal::SecurityManager::get_authentication_timeout
     */
    ble_error_t get_authentication_timeout_(
        connection_handle_t, uint16_t &timeout_in_10ms
    );

    /**
     * @see ::ble::pal::SecurityManager::set_encryption_key_requirements
     */
    ble_error_t set_encryption_key_requirements_(
        uint8_t min_encryption_key_size,
        uint8_t max_encryption_key_size
    );

    /**
     * @see ::ble::pal::SecurityManager::slave_security_request
     */
    ble_error_t slave_security_request_(
        connection_handle_t connection,
        AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @see ::ble::pal::SecurityManager::enable_encryption
     */
    ble_error_t enable_encryption_(
        connection_handle_t connection,
        const ltk_t &ltk,
        const rand_t &rand,
        const ediv_t &ediv,
        bool mitm
    );

    /**
     * @see ::ble::pal::SecurityManager::enable_encryption
     */
    ble_error_t enable_encryption_(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm
    ) ;

    /**
     * @see ::ble::pal::SecurityManager::encrypt_data
     */
    ble_error_t encrypt_data_(
        const byte_array_t<16> &key,
        encryption_block_t &data
    );

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    /**
     * @see ::ble::pal::SecurityManager::set_private_address_timeout
     */
    ble_error_t set_private_address_timeout_(uint16_t timeout_in_seconds);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * @see ::ble::pal::SecurityManager::set_ltk
     */
    ble_error_t set_ltk_(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm,
        bool secure_connections
    );

    /**
     * @see ::ble::pal::SecurityManager::set_ltk_not_found
     */
    ble_error_t set_ltk_not_found_(
        connection_handle_t connection
    );

    /**
     * @see ::ble::pal::SecurityManager::set_irk
     */
    ble_error_t set_irk_(const irk_t &irk);

    /**
     * @see ::ble::pal::SecurityManager::set_csrk
     */
    ble_error_t set_csrk_(const csrk_t &csrk, sign_count_t sign_counter);

    /**
     * @see ::ble::pal::SecurityManager::set_peer_csrk
     */
    ble_error_t set_peer_csrk_(
        connection_handle_t connection,
        const csrk_t &csrk,
        bool authenticated,
        sign_count_t sign_counter
    );

    /**
     * @see ::ble::pal::SecurityManager::remove_peer_csrk
     */
    ble_error_t remove_peer_csrk_(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * @see ::ble::pal::SecurityManager::get_random_data
     */
    ble_error_t get_random_data_(byte_array_t<8> &random_data);

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * @see ::ble::pal::SecurityManager::set_display_passkey
     */
    ble_error_t set_display_passkey_(passkey_num_t passkey);

    /**
     * @see ::ble::pal::SecurityManager::passkey_request_reply
     */
    ble_error_t passkey_request_reply_(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /**
     * @see ::ble::pal::SecurityManager::secure_connections_oob_request_reply
     */
    ble_error_t secure_connections_oob_request_reply_(
        connection_handle_t connection,
        const oob_lesc_value_t &local_random,
        const oob_lesc_value_t &peer_random,
        const oob_confirm_t &peer_confirm
    );

    /**
     * @see ::ble::pal::SecurityManager::legacy_pairing_oob_request_reply
     */
    ble_error_t legacy_pairing_oob_request_reply_(
        connection_handle_t connection,
        const oob_tk_t &oob_data
    );

    /**
     * @see ::ble::pal::SecurityManager::confirmation_entered
     */
    ble_error_t confirmation_entered_(
        connection_handle_t connection, bool confirmation
    );

    /**
     * @see ::ble::pal::SecurityManager::send_keypress_notification
     */
    ble_error_t send_keypress_notification_(
        connection_handle_t connection, Keypress_t keypress
    );

    /**
     * @see ::ble::pal::SecurityManager::generate_secure_connections_oob
     */
    ble_error_t generate_secure_connections_oob_();

    // singleton of nordic Security Manager
    static nRF5xSecurityManager& get_security_manager();

    // Event handler
    bool sm_handler(const ble_evt_t *evt);

private:
    csrk_t _csrk;
    sign_count_t _sign_counter;
    io_capability_t _io_capability;
    uint8_t _min_encryption_key_size;
    uint8_t _max_encryption_key_size;

    struct pairing_control_block_t;

    ble_gap_sec_params_t make_security_params(
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    ble_gap_sec_keyset_t make_keyset(
        pairing_control_block_t& pairing_cb,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    pairing_control_block_t* allocate_pairing_cb(connection_handle_t connection);
    void release_pairing_cb(pairing_control_block_t* pairing_cb);
    pairing_control_block_t* get_pairing_cb(connection_handle_t connection);
    void release_all_pairing_cb();

    pairing_control_block_t* _control_blocks;
#if defined(MBEDTLS_ECDH_C)
    ble::public_key_coord_t X;
    ble::public_key_coord_t Y;
    ble::public_key_coord_t secret;
#endif

    static const size_t MAX_RESOLVING_LIST_ENTRIES = BLE_GAP_DEVICE_IDENTITIES_MAX_COUNT;

    size_t resolving_list_entry_count;
    ble_gap_id_key_t resolving_list[MAX_RESOLVING_LIST_ENTRIES];
};

} // nordic
} // vendor
} // pal
} // ble

#endif /* NRF5X_PAL_SECURITY_MANAGER_ */
