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

#include <algorithm>
#include <vector>
#include <array>
#include <initializer_list>
#include <cstdlib>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ble/generic/GenericGattClient.h"
#include "ble/pal/AttServerMessage.h"
#include "ble/pal/SimpleAttServerMessage.h"
#include "ble/DiscoveredService.h"

#include "mock/MockCallbacks.h"
#include "mock/MockPalGattClient.h"

#include "util/PrettyPrinter.h"
#include "util/Equality.h"

using ble::pal::AttHandleValueIndication;
using ble::pal::AttHandleValueNotification;
using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ble::connection_handle_t;
using ble::make_const_ArrayView;

using ::testing::_;
using ::testing::Invoke;
using ::testing::Pointee;

using std::vector;

static vector<uint8_t> make_char_value(uint16_t length) {
	vector<uint8_t> characteristic_value(length);
	for (auto& byte : characteristic_value) {
		byte = std::rand();
	}
	return characteristic_value;
}

/*
 * Fixture use to test server notification or indication.
 */
class TestGattServerEvent : public ::testing::Test {
protected:
	TestGattServerEvent() :
		_mock_client(),
		_gatt_client(&_mock_client),
		_connection_handle(0xDEAD),
		_attribute_handle(0x5645),
		_mtu_size(23) {
	}

	virtual void SetUp() {
		_gatt_client.onHVX(
			makeFunctionPointer(&_event_cb, &server_event_callback_t::call)
		);

		ON_CALL(_mock_client, get_mtu_size(_connection_handle, _)).
		WillByDefault(Invoke([&](auto, auto& mtu_size ){
			mtu_size = this->_mtu_size;
			return BLE_ERROR_NONE;
		}));
	}

	void set_mtu_size_stub() {
		EXPECT_CALL(_mock_client, get_mtu_size(_connection_handle, _))
		.WillRepeatedly(::testing::DoDefault());
	}

	MockPalGattClient _mock_client;
	GenericGattClient _gatt_client;
	const Gap::Handle_t _connection_handle;
	uint16_t _attribute_handle;
	server_event_callback_t _event_cb;
	uint16_t _mtu_size;
};

/*
 * Ensure the right callback is called with the correct parameters when an
 * indication is received.
 */
TEST_F(TestGattServerEvent, event_callback_shall_be_called_on_indication) {
	auto value = make_char_value(5000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for (uint16_t len = 0; len < (_mtu_size - 3); len += 10) {

			EXPECT_CALL(
				_event_cb, call(Pointee(GattHVXCallbackParams {
					_connection_handle,
					_attribute_handle,
					HVXType_t::BLE_HVX_INDICATION,
					len,
					value.data()
				}))
			);

			_mock_client.on_server_event(
				_connection_handle,
				AttHandleValueIndication(
					_attribute_handle,
					make_const_ArrayView(value.data(), len)
				)
			);
		}
	}
}

/*
 * Ensure the right callback is called with the correct parameters when a
 * notification is received.
 */
TEST_F(TestGattServerEvent, event_callback_shall_be_called_on_notification) {
	auto value = make_char_value(5000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for (uint16_t len = 0; len < (_mtu_size - 3); len += 10) {

			EXPECT_CALL(
				_event_cb, call(Pointee(GattHVXCallbackParams {
					_connection_handle,
					_attribute_handle,
					HVXType_t::BLE_HVX_NOTIFICATION,
					len,
					value.data()
				}))
			);

			_mock_client.on_server_event(
				_connection_handle,
				AttHandleValueNotification(
					_attribute_handle,
					make_const_ArrayView(value.data(), len)
				)
			);
		}
	}
}

