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

#ifndef TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALCONENCTIONEVENTMONITOR_H_
#define TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALCONENCTIONEVENTMONITOR_H_

#include "gmock/gmock.h"
#include "ble/pal/ConnectionEventMonitor.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Mock of ble::pal::ConnectionEventMonitor
 */
class MockPalConnectionEventMonitor : public ble::pal::ConnectionEventMonitor {
public:
	MockPalConnectionEventMonitor();

	virtual ~MockPalConnectionEventMonitor();

	MOCK_METHOD1(set_connection_event_handler, void(ConnectionEventHandler *));
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_MOCKPALCONENCTIONEVENTMONITOR_H_ */
