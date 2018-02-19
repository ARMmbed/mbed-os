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

#ifndef TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYMANAGER_H_
#define TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYMANAGER_H_

#include "gmock/gmock.h"
#include "ble/pal/PalSecurityManager.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Mock of ble::pal::SecurityManager
 */
class MockPalSecurityManager : public ble::pal::SecurityManager {
public:
	MockPalSecurityManager();

	virtual ~MockPalSecurityManager();

	MOCK_METHOD0(initialize, ble_error_t());

	MOCK_METHOD0(terminate, ble_error_t());

	MOCK_METHOD0(reset, ble_error_t());

	MOCK_METHOD0(read_resolving_list_capacity, uint8_t());

	MOCK_METHOD3(
        add_device_to_resolving_list,
        ble_error_t(
            advertising_peer_address_type_t,
            const address_t &,
            const irk_t &
        )
    );

	MOCK_METHOD2(
        remove_device_from_resolving_list,
        ble_error_t(advertising_peer_address_type_t, const address_t &)
    );

	MOCK_METHOD0(clear_resolving_list, ble_error_t());

	MOCK_METHOD5(
        send_pairing_request,
        ble_error_t(
            connection_handle_t,
            bool,
            AuthenticationMask,
            KeyDistribution,
            KeyDistribution
        )
    );

	MOCK_METHOD5(
        send_pairing_response,
        ble_error_t(
            connection_handle_t,
            bool,
            AuthenticationMask,
            KeyDistribution,
            KeyDistribution
        )
    );

	MOCK_METHOD2(
        cancel_pairing,
        ble_error_t(connection_handle_t, pairing_failure_t)
    );

	MOCK_METHOD1(get_secure_connections_support, ble_error_t(bool &));

	MOCK_METHOD1(set_io_capability, ble_error_t(io_capability_t));

	MOCK_METHOD2(
        set_authentication_timeout,
        ble_error_t(connection_handle_t, uint16_t)
    );

	MOCK_METHOD2(
        get_authentication_timeout,
        ble_error_t(connection_handle_t, uint16_t &)
    );

	MOCK_METHOD2(
        set_encryption_key_requirements,
        ble_error_t(uint8_t, uint8_t)
    );

	MOCK_METHOD2(
        slave_security_request,
        ble_error_t(connection_handle_t, AuthenticationMask)
    );

	MOCK_METHOD4(
        slave_security_request,
        ble_error_t(
            connection_handle_t,
            const ltk_t &,
            const rand_t &,
            const ediv_t &
        )
    );

	MOCK_METHOD2(
        enable_encryption,
        ble_error_t(connection_handle_t, const ltk_t &)
    );

	MOCK_METHOD1(disable_encryption, ble_error_t(connection_handle_t));

	MOCK_METHOD2(
        get_encryption_key_size,
        ble_error_t(connection_handle_t, uint8_t &)
    );

	MOCK_METHOD2(
        encrypt_data,
        ble_error_t(const key_t &, encryption_block_t &)
    );

	MOCK_METHOD1(set_private_address_timeout, ble_error_t(uint16_t));

	MOCK_METHOD2(set_ltk, ble_error_t(connection_handle_t, const ltk_t &));

	MOCK_METHOD1(set_ltk_not_found, ble_error_t(connection_handle_t));

	MOCK_METHOD1(set_irk, ble_error_t(const irk_t &));

    MOCK_METHOD1(set_csrk, ble_error_t(const csrk_t &));

    MOCK_METHOD0(generate_public_key, ble_error_t());

    MOCK_METHOD1(request_authentication, ble_error_t(connection_handle_t));

    MOCK_METHOD1(get_random_data, ble_error_t(random_data_t &));

    MOCK_METHOD1(set_display_passkey, ble_error_t(passkey_num_t));

    MOCK_METHOD2(passkey_request_reply, ble_error_t(connection_handle_t, passkey_num_t));

    MOCK_METHOD2(
        legacy_pairing_oob_data_request_reply,
        ble_error_t(connection_handle_t, const oob_tk_t &)
    );

    MOCK_METHOD2(confirmation_entered, ble_error_t(connection_handle_t, bool));

    MOCK_METHOD2(
        send_keypress_notification,
        ble_error_t(connection_handle_t, Keypress_t)
    );

    MOCK_METHOD3(
        oob_data_verified,
        ble_error_t(connection_handle_t, const oob_rand_t &, const oob_rand_t &)
    );

    // get the event handler present in the mock object.
    using pal::SecurityManager::get_event_handler;
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALSECURITYMANAGER_H_ */
