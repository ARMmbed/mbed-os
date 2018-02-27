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

#include <vector>
#include <array>
#include <initializer_list>
#include <tuple>
#include <cstddef>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ble/generic/GenericGattClient.h"
#include "ble/pal/AttServerMessage.h"
#include "ble/pal/SimpleAttServerMessage.h"
#include "ble/DiscoveredService.h"

#include "mock/MockCallbacks.h"
#include "mock/MockPalGattClient.h"

using std::nullptr_t;

using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ::testing::_;

/*
 * Parametric fixture used to test service discovery when no callback is registered.
 * Parameters are:
 * - [0]: service UUID filter
 * - [1]: characteristic UUID filter
 */
class LaunchDiscoveryNoCb : public ::testing::TestWithParam<std::tuple<UUID, UUID>> {
protected:
	typedef std::tuple<UUID, UUID> parameters_t;

	LaunchDiscoveryNoCb() :
		_mock_client(),
		_gatt_client(&_mock_client),
		_connection_handle(0xDEAD),
		_termination_callback(),
		_service_cb(),
		_characteristic_cb(),
		_service_filter(),
		_characteristic_filter() {
	}

	virtual void SetUp() {
		_gatt_client.onServiceDiscoveryTermination(
			makeFunctionPointer(
				&_termination_callback,
				&termination_callback_t::call
			)
		);

		std::tie(_service_filter, _characteristic_filter) = GetParam();
	}

	MockPalGattClient _mock_client;
	GenericGattClient _gatt_client;

	const Gap::Handle_t _connection_handle;
	termination_callback_t _termination_callback;
	nullptr_t _service_cb;
	nullptr_t _characteristic_cb;
	UUID _service_filter;
	UUID _characteristic_filter;
};


/* Given initialised GattClient
 * When the service discovery is launched with service and characteristic
 * callbacks set to NULL.
 * Then
 *   - The function shall return BLE_ERROR_NONE
 *   - No call to any discovery function shall have been made
 *   - The termination callback shall have been called once with the connection
 *     handle used to launch the discovery.
 */

// no pending transaction
TEST_P(LaunchDiscoveryNoCb, shall_never_launch_any_discovery) {
	EXPECT_FALSE(_gatt_client.isServiceDiscoveryActive());

	// no call on _mock_client expected
	EXPECT_CALL(_mock_client, discover_primary_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_primary_service_by_service_uuid(_, _, _)).Times(0);
	EXPECT_CALL(_mock_client, find_included_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_characteristics_of_a_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_characteristics_descriptors(_, _)).Times(0);

	// termination callback shall be call once with connection handle as parameter
	EXPECT_CALL(_termination_callback, call(_)).Times(0);
	EXPECT_CALL(_termination_callback, call(_connection_handle)).Times(1);

	ble_error_t err = _gatt_client.launchServiceDiscovery(
		_connection_handle,
		_service_cb,
		_characteristic_cb,
		_service_filter,
		_characteristic_filter
	);

	EXPECT_EQ(BLE_ERROR_NONE, err);
	EXPECT_FALSE(_gatt_client.isServiceDiscoveryActive());
}

// no CB
// pending transaction
TEST_P(LaunchDiscoveryNoCb, shall_not_change_discovery_status) {
	EXPECT_FALSE(_gatt_client.isServiceDiscoveryActive());

	// launch a discovery on another connection
	const Gap::Handle_t dummy_connection_handle = _connection_handle + 1;
	void (*dummy_service_cb)(const DiscoveredService*) = [](const DiscoveredService *) { };

	EXPECT_CALL(_mock_client, discover_primary_service(dummy_connection_handle, _))
	.WillOnce(::testing::Return(BLE_ERROR_NONE));

	_gatt_client.launchServiceDiscovery(
		dummy_connection_handle,
		dummy_service_cb,
		NULL,
		UUID((uint16_t) 0),
		UUID((uint16_t) 0)
	);

	EXPECT_TRUE(_gatt_client.isServiceDiscoveryActive());

	// no call on _mock_client expected
	EXPECT_CALL(_mock_client, discover_primary_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_primary_service_by_service_uuid(_, _, _)).Times(0);
	EXPECT_CALL(_mock_client, find_included_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_characteristics_of_a_service(_, _)).Times(0);
	EXPECT_CALL(_mock_client, discover_characteristics_descriptors(_, _)).Times(0);

	// termination callback shall be call once with connection handle as parameter
	EXPECT_CALL(_termination_callback, call(_)).Times(0);
	EXPECT_CALL(_termination_callback, call(_connection_handle)).Times(1);

	ble_error_t err = _gatt_client.launchServiceDiscovery(
		_connection_handle,
		_service_cb,
		_characteristic_cb,
		_service_filter,
		_characteristic_filter
	);

	EXPECT_EQ(BLE_ERROR_NONE, err);
	EXPECT_TRUE(_gatt_client.isServiceDiscoveryActive());
}

// Instantiate tests cases with the given parameters
INSTANTIATE_TEST_CASE_P(
	GattClient_launch_discovery_no_cb,
	LaunchDiscoveryNoCb,
	// Yield combination of each generator value
	::testing::Combine(
		// service UUID filter
		::testing::Values(UUID(), UUID(0x1452), UUID("a3d1495f-dba7-4441-99f2-d0a20f663422")),
		// characteristic UUID filter
		::testing::Values(UUID(), UUID(0xBEEF), UUID("1f551ee3-aef4-4719-8c52-8b419fc4ac01"))
	)
);
