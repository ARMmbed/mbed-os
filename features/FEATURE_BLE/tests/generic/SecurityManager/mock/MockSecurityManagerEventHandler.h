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

#ifndef TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKSECURITYMANAGEREVENTHANDLER_H_
#define TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKSECURITYMANAGEREVENTHANDLER_H_

#include "gmock/gmock.h"
#include "ble/SecurityManager.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Mock of ble::pal::SecurityManager
 */
class MockSecurityManagerEventHandler :
    public ::SecurityManager::SecurityManagerEventHandler {
public:
	MockSecurityManagerEventHandler();

	virtual ~MockSecurityManagerEventHandler();

	MOCK_METHOD1(pairingRequest, void(connection_handle_t));

    MOCK_METHOD2(pairingResult, void(connection_handle_t, ::SecurityManager::SecurityCompletionStatus_t));

    MOCK_METHOD1(validMicTimeout, void(connection_handle_t));

    MOCK_METHOD1(whitelistFromBondTable, void(::Gap::Whitelist_t*));

    MOCK_METHOD2(whitelistFromBondTable, void(connection_handle_t, link_encryption_t));

    MOCK_METHOD2(passkeyDisplay, void(connection_handle_t, const ::SecurityManager::Passkey_t));

    MOCK_METHOD1(confirmationRequest, void(connection_handle_t));

    MOCK_METHOD1(passkeyRequest, void(connection_handle_t));

    MOCK_METHOD2(keypressNotification, void(connection_handle_t, ::SecurityManager::Keypress_t));

    MOCK_METHOD1(legacyPairingOobRequest, void(connection_handle_t));

    MOCK_METHOD1(oobRequest, void(connection_handle_t));

    MOCK_METHOD2(legacyPairingOobGenerated, void(const address_t *, const oob_tk_t *));

    MOCK_METHOD3(oobGenerated, void(const address_t *, const oob_rand_t *, const oob_confirm_t *));

    MOCK_METHOD3(signingKey, void(connection_handle_t, const csrk_t *, bool));
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKSECURITYMANAGEREVENTHANDLER_H_ */
