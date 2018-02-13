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

#ifndef TESTS_GENERIC_GATTCLIENT_MOCK_MOCKCALLBACKS_H_
#define TESTS_GENERIC_GATTCLIENT_MOCK_MOCKCALLBACKS_H_

#include "gmock/gmock.h"
#include "ble/Gap.h"
#include "ble/DiscoveredService.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/GattCallbackParamTypes.h"
#include "ble/CharacteristicDescriptorDiscovery.h"

////////////////////////////////////////////////////////////////////////////////
// Mocks declaration for callbacks objects used in GattClient
//

/*
 * Mock of the service discovery callback.
 */
struct service_callback_t {
	service_callback_t();
	virtual ~service_callback_t();
	MOCK_METHOD1(call, void(const DiscoveredService*));
};

/*
 * Mock of the characteristic discovery callback.
 */
struct characteristic_callback_t {
	characteristic_callback_t();
	virtual ~characteristic_callback_t();
	MOCK_METHOD1(call, void(const DiscoveredCharacteristic*));
};

/*
 * Mock of the discovery termination callback.
 */
struct termination_callback_t {
	termination_callback_t();
	virtual ~termination_callback_t();
	MOCK_METHOD1(call, void(Gap::Handle_t));
};

/*
 * Mock of the read attribute callback.
 */
struct read_callback_t {
	read_callback_t();
	virtual ~read_callback_t();
	MOCK_METHOD1(call, void(const GattReadCallbackParams*));
};

/*
 * Mock of the write attribute callback.
 */
struct write_callback_t {
	write_callback_t();
	virtual ~write_callback_t();
	MOCK_METHOD1(call, void(const GattWriteCallbackParams*));
};

/*
 * Mock of the server initiated events callback (notification and indication).
 */
struct server_event_callback_t {
	server_event_callback_t();
	virtual ~server_event_callback_t();
	MOCK_METHOD1(call, void(const GattHVXCallbackParams*));
};

/*
 * Mock of the characteristic descriptor discovery callback.
 */
struct descriptor_callback_t {
	descriptor_callback_t();
	virtual ~descriptor_callback_t();
	MOCK_METHOD1(
		call,
		void(const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t*)
	);
};

/*
 * Mock of the characteristic descriptor termination callback.
 */
struct descriptor_termination_callback_t {
	descriptor_termination_callback_t();
	virtual ~descriptor_termination_callback_t();
	MOCK_METHOD1(
		call,
		void(const CharacteristicDescriptorDiscovery::TerminationCallbackParams_t*)
	);
};

#endif /* TESTS_GENERIC_GATTCLIENT_MOCK_MOCKCALLBACKS_H_ */
