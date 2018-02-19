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

#ifndef TESTS_GENERIC_SECURITYMANAGER_MOCK_STUBCONNECTIONEVENTMONITOR_H_
#define TESTS_GENERIC_SECURITYMANAGER_MOCK_STUBCONNECTIONEVENTMONITOR_H_

#include "gmock/gmock.h"
#include "ble/pal/ConnectionEventMonitor.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Stub of ble::pal::ConnectionEventMonitor
 */
struct StubPalConnectionEventMonitor : public ble::pal::ConnectionEventMonitor {
	StubPalConnectionEventMonitor() : event_handler(nullptr) { }

	virtual ~StubPalConnectionEventMonitor() { }

	void set_connection_event_handler(pal::ConnectionEventHandler *connection_event_handler)
	{
		event_handler = connection_event_handler;
	}

	pal::ConnectionEventHandler *event_handler;
};

} // namespace mock
} // namespace vendor
} // namespace pal
} // namespace ble

#endif /* TESTS_GENERIC_SECURITYMANAGER_MOCK_STUBCONNECTIONEVENTMONITOR_H_ */
