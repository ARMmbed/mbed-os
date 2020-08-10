/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef IMPL_PAL_SECURITY_MANAGER_
#define IMPL_PAL_SECURITY_MANAGER_

#include "ble/internal/PalSecurityManager.h"
#include "ble/common/ble/blecommon.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "sec_api.h"
#include "smp_defs.h"
#include "cfg_stack.h"

namespace ble {

class BLEInstanceBase;

class PalSecurityManager : public interface::PalSecurityManager {
    friend BLEInstanceBase;
public:

    PalSecurityManager();

    ~PalSecurityManager();

    ////////////////////////////////////////////////////////////////////////////
    // SM lifecycle management
    //

    /**
     * @see ::ble::PalSecurityManager::initialize
     */
    ble_error_t initialize();

    /**
     * @see ::ble::PalSecurityManager::terminate
     */
    ble_error_t terminate();

    /**
     * @see ::ble::PalSecurityManager::reset
     */
    ble_error_t reset();

    ////////////////////////////////////////////////////////////////////////////
    // Resolving list management
    //

    /**
     * @see ::ble::PalSecurityManager::read_resolving_list_capacity
     */
    uint8_t read_resolving_list_capacity();

    /**
     * @see ::ble::PalSecurityManager::add_device_to_resolving_list
     */
    ble_error_t add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    );

    /**
     * @see ::ble::PalSecurityManager::remove_device_from_resolving_list
     */
    ble_error_t remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    );

    /**
     * @see ::ble::PalSecurityManager::clear_resolving_list
     */
    ble_error_t clear_resolving_list();

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

    /**
     * @see ::ble::PalSecurityManager::send_pairing_request
     */
    ble_error_t send_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::PalSecurityManager::send_pairing_response
     */
    ble_error_t send_pairing_response(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    );

    /**
     * @see ::ble::PalSecurityManager::cancel_pairing
     */
    ble_error_t cancel_pairing(
        connection_handle_t connection, pairing_failure_t reason
    );

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * @see ::ble::PalSecurityManager::get_secure_connections_support
     */
    ble_error_t get_secure_connections_support(
        bool &enabled
    );

    /**
     * @see ::ble::PalSecurityManager::set_io_capability
     */
    ble_error_t set_io_capability(io_capability_t io_capability);

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * @see ::ble::PalSecurityManager::set_authentication_timeout
     */
    ble_error_t set_authentication_timeout(
        connection_handle_t, uint16_t timeout_in_10ms
    );

    /**
     * @see ::ble::PalSecurityManager::get_authentication_timeout
     */
    ble_error_t get_authentication_timeout(
        connection_handle_t, uint16_t &timeout_in_10ms
    );

    /**
     * @see ::ble::PalSecurityManager::set_encryption_key_requirements
     */
    ble_error_t set_encryption_key_requirements(
        uint8_t min_encryption_key_size,
        uint8_t max_encryption_key_size
    );

    /**
     * @see ::ble::PalSecurityManager::slave_security_request
     */
    ble_error_t slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    );

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

    /**
     * @see ::ble::PalSecurityManager::enable_encryption
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        const rand_t &rand,
        const ediv_t &ediv,
        bool mitm
    );

    /**
     * @see ::ble::PalSecurityManager::enable_encryption
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm
    );

    /**
     * @see ::ble::PalSecurityManager::encrypt_data
     */
    ble_error_t encrypt_data(
        const byte_array_t<16> &key,
        encryption_block_t &data
    );

    ////////////////////////////////////////////////////////////////////////////
    // Privacy
    //

    /**
     * @see ::ble::PalSecurityManager::set_private_address_timeout
     */
    ble_error_t set_private_address_timeout(uint16_t timeout_in_seconds);

    /**
     * @see ::ble::PalSecurityManager::get_identity_address
     */
    ble_error_t get_identity_address(address_t& address, bool& public_address);

    ////////////////////////////////////////////////////////////////////////////
    // Keys
    //

    /**
     * @see ::ble::PalSecurityManager::set_ltk
     */
    ble_error_t set_ltk(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm,
        bool secure_connections
    );

    /**
     * @see ::ble::PalSecurityManager::set_ltk_not_found
     */
    ble_error_t set_ltk_not_found(
        connection_handle_t connection
    );

    /**
     * @see ::ble::PalSecurityManager::set_irk
     */
    ble_error_t set_irk(const irk_t &irk);

    /**
     * @see ::ble::PalSecurityManager::set_csrk
     */
    ble_error_t set_csrk(
        const csrk_t &csrk,
        sign_count_t sign_counter
    );

    /**
     * @see ::ble::PalSecurityManager::set_peer_csrk
     */
    ble_error_t set_peer_csrk(
        connection_handle_t connection,
        const csrk_t &csrk,
        bool authenticated,
        sign_count_t sign_counter
    );

    ble_error_t remove_peer_csrk(connection_handle_t connection);

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * @see ::ble::PalSecurityManager::get_random_data
     */
    ble_error_t get_random_data(byte_array_t<8> &random_data);

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * @see ::ble::PalSecurityManager::set_display_passkey
     */
    ble_error_t set_display_passkey(passkey_num_t passkey);

    /**
     * @see ::ble::PalSecurityManager::passkey_request_reply
     */
    ble_error_t passkey_request_reply(
        connection_handle_t connection,
        passkey_num_t passkey
    );

    /**
     * @see ::ble::PalSecurityManager::secure_connections_oob_request_reply
     */
    ble_error_t secure_connections_oob_request_reply(
        connection_handle_t connection,
        const oob_lesc_value_t &local_random,
        const oob_lesc_value_t &peer_random,
        const oob_confirm_t &peer_confirm
    );

    /**
     * @see ::ble::PalSecurityManager::legacy_pairing_oob_request_reply
     */
    ble_error_t legacy_pairing_oob_request_reply(
        connection_handle_t connection,
        const oob_tk_t &oob_data
    );

    /**
     * @see ::ble::PalSecurityManager::confirmation_entered
     */
    ble_error_t confirmation_entered(
        connection_handle_t connection, bool confirmation
    );

    /**
     * @see ::ble::PalSecurityManager::send_keypress_notification
     */
    ble_error_t send_keypress_notification(
        connection_handle_t connection, ble::Keypress_t keypress
    );

    /**
     * @see ::ble::PalSecurityManager::generate_secure_connections_oob
     */
    ble_error_t generate_secure_connections_oob();

    /**
     * @see ::ble::PalSecurityManager::set_event_handler
     */
    void set_event_handler(PalSecurityManagerEventHandler *event_handler);

    /**
     * @see ::ble::PalSecurityManager::get_event_handler
     */
    PalSecurityManagerEventHandler* get_event_handler();

private:
    /* used by the PAL to get the singleton */
    static PalSecurityManager &get_security_manager();

    /* used by PAL to handle security messages coming from the stack Event handler */
    static bool sm_handler(const wsfMsgHdr_t* msg);

private:

    struct PrivacyControlBlock;
    struct PrivacyClearResListControlBlock;
    struct PrivacyAddDevToResListControlBlock;
    struct PrivacyRemoveDevFromResListControlBlock;

    // Queue control block to add device to resolving list
    void queue_add_device_to_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address,
        const irk_t &peer_irk
    );

    // Queue control block to remove device from resolving list
    void queue_remove_device_from_resolving_list(
        advertising_peer_address_type_t peer_identity_address_type,
        const address_t &peer_identity_address
    );

    // Queue control block to clear resolving list
    void queue_clear_resolving_list();

    // Clear all control blocks
    void clear_privacy_control_blocks();

    // Queue a control block
    void queue_privacy_control_block(PrivacyControlBlock* block);

    // Try to dequeue and process the next control block
    // cb_completed is set when the previous block has completed
    void process_privacy_control_blocks(bool cb_completed);

    void cleanup_peer_csrks();

    PalSecurityManagerEventHandler* _pal_event_handler;

    bool _use_default_passkey;
    passkey_num_t _default_passkey;
    bool _lesc_keys_generated;
    uint8_t _public_key_x[SEC_ECC_KEY_LEN];

    PrivacyControlBlock* _pending_privacy_control_blocks;
    bool _processing_privacy_control_block;
    irk_t _irk;
    csrk_t _csrk;
    csrk_t* _peer_csrks[DM_CONN_MAX];
};

} // ble

#endif /* IMPL_PAL_SECURITY_MANAGER_ */
