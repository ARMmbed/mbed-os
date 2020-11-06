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

#include "source/pal/PalSecurityManager.h"
#include "ble/common/blecommon.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "sec_api.h"
#include "smp_defs.h"
#include "cfg_stack.h"

namespace ble {
namespace impl {

class BLEInstanceBase;

class PalSecurityManager final : public ble::PalSecurityManager {
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
    ble_error_t initialize() final;

    /**
     * @see ::ble::PalSecurityManager::terminate
     */
    ble_error_t terminate() final;

    /**
     * @see ::ble::PalSecurityManager::reset
     */
    ble_error_t reset() final;

    ////////////////////////////////////////////////////////////////////////////
    // Pairing
    //

#if BLE_ROLE_CENTRAL
    /**
     * @see ::ble::PalSecurityManager::send_pairing_request
     */
    ble_error_t send_pairing_request(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) final;
#endif // BLE_ROLE_CENTRAL

#if BLE_ROLE_PERIPHERAL
    /**
     * @see ::ble::PalSecurityManager::send_pairing_response
     */
    ble_error_t send_pairing_response(
        connection_handle_t connection,
        bool oob_data_flag,
        AuthenticationMask authentication_requirements,
        KeyDistribution initiator_dist,
        KeyDistribution responder_dist
    ) final;
#endif // BLE_ROLE_PERIPHERAL

    /**
     * @see ::ble::PalSecurityManager::cancel_pairing
     */
    ble_error_t cancel_pairing(
        connection_handle_t connection, pairing_failure_t reason
    ) final;

    ////////////////////////////////////////////////////////////////////////////
    // Feature support
    //

    /**
     * @see ::ble::PalSecurityManager::get_secure_connections_support
     */
    ble_error_t get_secure_connections_support(
        bool &enabled
    ) final;

    /**
     * @see ::ble::PalSecurityManager::set_io_capability
     */
    ble_error_t set_io_capability(io_capability_t io_capability) final;

    ////////////////////////////////////////////////////////////////////////////
    // Security settings
    //

    /**
     * @see ::ble::PalSecurityManager::set_authentication_timeout
     */
    ble_error_t set_authentication_timeout(
        connection_handle_t, uint16_t timeout_in_10ms
    ) final;

    /**
     * @see ::ble::PalSecurityManager::get_authentication_timeout
     */
    ble_error_t get_authentication_timeout(
        connection_handle_t, uint16_t &timeout_in_10ms
    ) final;

    /**
     * @see ::ble::PalSecurityManager::set_encryption_key_requirements
     */
    ble_error_t set_encryption_key_requirements(
        uint8_t min_encryption_key_size,
        uint8_t max_encryption_key_size
    ) final;

#if BLE_ROLE_PERIPHERAL
    /**
     * @see ::ble::PalSecurityManager::slave_security_request
     */
    ble_error_t slave_security_request(
        connection_handle_t connection,
        AuthenticationMask authentication
    ) final;
#endif

    ////////////////////////////////////////////////////////////////////////////
    // Encryption
    //

#if BLE_ROLE_CENTRAL
    /**
     * @see ::ble::PalSecurityManager::enable_encryption
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        const rand_t &rand,
        const ediv_t &ediv,
        bool mitm
    ) final;

#if BLE_FEATURE_SECURE_CONNECTIONS
    /**
     * @see ::ble::PalSecurityManager::enable_encryption
     */
    ble_error_t enable_encryption(
        connection_handle_t connection,
        const ltk_t &ltk,
        bool mitm
    ) final;
#endif // BLE_FEATURE_SECURE_CONNECTIONS
#endif // BLE_ROLE_CENTRAL

    /**
     * @see ::ble::PalSecurityManager::encrypt_data
     */
    ble_error_t encrypt_data(
        const byte_array_t<16> &key,
        encryption_block_t &data
    ) final;

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
    ) final;

    /**
     * @see ::ble::PalSecurityManager::set_ltk_not_found
     */
    ble_error_t set_ltk_not_found(
        connection_handle_t connection
    ) final;

    /**
     * @see ::ble::PalSecurityManager::set_irk
     */
    ble_error_t set_irk(const irk_t &irk) final;

    /**
     * @see ::ble::PalSecurityManager::set_identity_address
     */
    ble_error_t set_identity_address(
        const address_t &address, bool public_address
    ) final;

#if BLE_FEATURE_SIGNING
    /**
     * @see ::ble::PalSecurityManager::set_csrk
     */
    ble_error_t set_csrk(
        const csrk_t &csrk,
        sign_count_t sign_counter
    ) final;

    /**
     * @see ::ble::PalSecurityManager::set_peer_csrk
     */
    ble_error_t set_peer_csrk(
        connection_handle_t connection,
        const csrk_t &csrk,
        bool authenticated,
        sign_count_t sign_counter
    ) final;

    ble_error_t remove_peer_csrk(connection_handle_t connection) final;
#endif // BLE_FEATURE_SIGNING

    ////////////////////////////////////////////////////////////////////////////
    // Authentication
    //

    /**
     * @see ::ble::PalSecurityManager::get_random_data
     */
    ble_error_t get_random_data(byte_array_t<8> &random_data) final;

    ////////////////////////////////////////////////////////////////////////////
    // MITM
    //

    /**
     * @see ::ble::PalSecurityManager::set_display_passkey
     */
    ble_error_t set_display_passkey(passkey_num_t passkey) final;

    /**
     * @see ::ble::PalSecurityManager::passkey_request_reply
     */
    ble_error_t passkey_request_reply(
        connection_handle_t connection,
        passkey_num_t passkey
    ) final;

#if BLE_FEATURE_SECURE_CONNECTIONS
    /**
     * @see ::ble::PalSecurityManager::secure_connections_oob_request_reply
     */
    ble_error_t secure_connections_oob_request_reply(
        connection_handle_t connection,
        const oob_lesc_value_t &local_random,
        const oob_lesc_value_t &peer_random,
        const oob_confirm_t &peer_confirm
    ) final;
#endif // /BLE_FEATURE_SECURE_CONNECTIONS

    /**
     * @see ::ble::PalSecurityManager::legacy_pairing_oob_request_reply
     */
    ble_error_t legacy_pairing_oob_request_reply(
        connection_handle_t connection,
        const oob_tk_t &oob_data
    ) final;

#if BLE_FEATURE_SECURE_CONNECTIONS
    /**
     * @see ::ble::PalSecurityManager::confirmation_entered
     */
    ble_error_t confirmation_entered(
        connection_handle_t connection, bool confirmation
    ) final;

    /**
     * @see ::ble::PalSecurityManager::send_keypress_notification
     */
    ble_error_t send_keypress_notification(
        connection_handle_t connection, ble::Keypress_t keypress
    ) final;

    /**
     * @see ::ble::PalSecurityManager::generate_secure_connections_oob
     */
    ble_error_t generate_secure_connections_oob() final;
#endif // BLE_FEATURE_SECURE_CONNECTIONS

    /**
     * @see ::ble::PalSecurityManager::set_event_handler
     */
    void set_event_handler(PalSecurityManagerEventHandler *event_handler) final;

    /**
     * @see ::ble::PalSecurityManager::get_event_handler
     */
    PalSecurityManagerEventHandler *get_event_handler() final;

public:
    /* used by the PAL to get the singleton */
    static PalSecurityManager &get_security_manager();

    /* used by PAL to handle security messages coming from the stack Event handler */
    static bool sm_handler(const wsfMsgHdr_t *msg);

private:

#if BLE_FEATURE_SIGNING
    void cleanup_peer_csrks();
#endif // BLE_FEATURE_SIGNING

    PalSecurityManagerEventHandler *_pal_event_handler;

    bool _use_default_passkey;
    passkey_num_t _default_passkey;
#if BLE_FEATURE_SECURE_CONNECTIONS
    bool _lesc_keys_generated = false;
    uint8_t _public_key_x[SEC_ECC_KEY_LEN] = {0};
#endif // BLE_FEATURE_SECURE_CONNECTIONS
    irk_t _irk;
#if BLE_FEATURE_SIGNING
    csrk_t _csrk;
    csrk_t *_peer_csrks[DM_CONN_MAX] = {0};
#endif // BLE_FEATURE_SIGNING
};

} // namespace impl
} // namespace ble

#endif /* IMPL_PAL_SECURITY_MANAGER_ */
