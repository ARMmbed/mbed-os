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
#include <tuple>
#include <cstdlib>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ble/generic/GenericGattClient.h"
#include "ble/pal/AttServerMessage.h"
#include "ble/pal/SimpleAttServerMessage.h"
#include "ble/DiscoveredService.h"

#include "mock/MockPalGattClient.h"
#include "mock/MockCallbacks.h"

#include "util/PrettyPrinter.h"
#include "util/Equality.h"
#include "util/Log.h"

// using declarations
using ble::pal::AttErrorResponse;
using ble::pal::AttributeOpcode;
using ble::pal::AttFindInformationResponse;
using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ble::connection_handle_t;
using ble::attribute_handle_range;
using ble::attribute_handle_range_t;

using ::testing::_;
using ::testing::Invoke;
using ::testing::AllOf;
using ::testing::ResultOf;
using ::testing::InSequence;
using ::testing::Pointee;

using std::vector;
using std::tuple;
using std::pair;

struct ConstructibleDiscoveredCharacteristic : public DiscoveredCharacteristic {
	ConstructibleDiscoveredCharacteristic(
		GattClient* client,
		Gap::Handle_t connection_handle,
		const UUID& uuid,
		uint16_t declaration_handle,
		uint16_t value_handle,
		uint16_t last_handle
	) : DiscoveredCharacteristic() {
		this->gattc = client;
		this->uuid = uuid;
		declHandle = declaration_handle;
		valueHandle = value_handle;
		lastHandle = last_handle;
		connHandle = connection_handle;
	}

	void set_last_handle(uint16_t last_handle) {
		lastHandle = last_handle;
	}

	void set_value_handle(uint16_t value_handle) {
		valueHandle = value_handle;
	}
};

/**
 * Test fixture used for client descriptor discovery testing.
 */
class TestGattClientDescriptorDiscovery : public ::testing::Test {
protected:
	TestGattClientDescriptorDiscovery() :
		_mock_client(),
		_gatt_client(&_mock_client),
		_connection_handle(0xDEAD),
		_attribute_handle(0x5645),
		_descriptor_cb(),
		_termination_cb(),
		_mtu_size(23),
		_characteristic(
			&_gatt_client,
			_connection_handle,
			UUID(0xDEAF),
			_attribute_handle,
			_attribute_handle + 1,
			_attribute_handle + 1
		) {
	}

	virtual void SetUp() {
		ON_CALL(
			_mock_client, get_mtu_size(_connection_handle, _)
		).WillByDefault(Invoke([&](auto, auto& size){
			size = this->_mtu_size;
			return BLE_ERROR_NONE;
		}));
	}

	void set_mtu_size_stub() {
		EXPECT_CALL(
			_mock_client, get_mtu_size(_connection_handle, _)
		).WillRepeatedly(::testing::DoDefault());
	}

	MockPalGattClient _mock_client;
	GenericGattClient _gatt_client;
	const Gap::Handle_t _connection_handle;
	uint16_t _attribute_handle;
	descriptor_callback_t _descriptor_cb;
	descriptor_termination_callback_t _termination_cb;
	uint16_t _mtu_size;
	ConstructibleDiscoveredCharacteristic _characteristic;
};

// errors: Invalid handle if:
// 		starting handle > ending handle
//      stating handle == 00
//      ending handle > last handle on the server

// if no handle will be return => ATTRIBUTE NOT FOUND

// Complete when ATTRIBUTE NOT FOUND is returned or an attribute handle
// in the response is equal to the ending handle in the request.


// Find information response:
// format & [(handle, UUID)]
// format == 1 => 16 bit UUID
// format == 2 => 128 bit UUID

/*
 * Given a discovered characteristic with the value handle equal to the last
 * handle of the characteristic.
 * When the client discover the descriptor of the characteristic
 * Then:
 *   - no request is issued to the PAL.
 *   - the function doesn't return any error.
 *   - the termination callback is called immediately
 */
TEST_F(TestGattClientDescriptorDiscovery, descriptor_discovery_on_characteristics_without_descriptor_shall_return_immediatelly) {
	_characteristic.set_last_handle(_characteristic.getValueHandle());

	EXPECT_CALL(
		_mock_client, discover_characteristics_descriptors(_, _)
	).Times(0);

	EXPECT_CALL(
		_termination_cb, call(Pointee(CharacteristicDescriptorDiscovery::TerminationCallbackParams_t {
				_characteristic,
				BLE_ERROR_NONE,
				0x00
			})
		)
	);

	ble_error_t err = _gatt_client.discoverCharacteristicDescriptors(
		_characteristic,
		makeFunctionPointer(&_descriptor_cb, &descriptor_callback_t::call),
		makeFunctionPointer(&_termination_cb, &descriptor_termination_callback_t::call)
	);

	EXPECT_EQ(err, BLE_ERROR_NONE);
}

/**
 * Test parameter pass into tests using TestGattClientDescriptorDiscoveryP fixture.
 * - first element: value handle of the characteristic.
 * - second element: last handle of the characteristic
 * - third element: expected transactions; each transaction can contain multiple
 * pair containing the handle of the descriptor and its UUID.
 */
typedef tuple<uint16_t, uint16_t, vector<vector<pair<uint16_t, UUID>>>> test_param_t;

/**
 * Parametric fixture used for descriptor discovery testing.
 */
class TestGattClientDescriptorDiscoveryP :
	public TestGattClientDescriptorDiscovery,
	public ::testing::WithParamInterface<test_param_t> {

protected:
	TestGattClientDescriptorDiscoveryP() :
		TestGattClientDescriptorDiscovery(),
		_value_handle(0),
		_last_handle(0),
		_descriptors() {
	}

	virtual void SetUp() {
		TestGattClientDescriptorDiscovery::SetUp();
		std::tie(_value_handle, _last_handle, _descriptors) = GetParam();
		_characteristic.set_value_handle(_value_handle);
		_characteristic.set_last_handle(_last_handle);
	}

	uint16_t _value_handle;
	uint16_t _last_handle;
	vector<vector<pair<uint16_t, UUID>>> _descriptors;
};

struct MockFindInformationResponse : public AttFindInformationResponse {
	MockFindInformationResponse(vector<pair<uint16_t, UUID>> response) :
		_response(response) { }

	virtual size_t size() const {
		return _response.size();
	}

	virtual information_data_t operator[](size_t index) const {
		auto& element = _response[index];
		return {
			element.first,
			element.second
		};
	}

	vector<pair<uint16_t, UUID>> _response;
};

/**
 * Helper returning a DiscoveredCharacteristic from a DiscoveryCallbackParams_t*
 */
static const DiscoveredCharacteristic& get_characteristic(const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t* p) {
	return p->characteristic;
}

/**
 * Helper returning a DiscoveredCharacteristicDescriptor from a DiscoveryCallbackParams_t*
 */
static const DiscoveredCharacteristicDescriptor& get_descriptor(const CharacteristicDescriptorDiscovery::DiscoveryCallbackParams_t* p) {
	return p->descriptor;
}

/*
 * Given a discovered characteristic with the value handle not equal to the
 * last handle of the characteristic.
 * when the client launch the discovery of the descriptor of the characteristic.
 * Then:
 *   - the client invoke the pal function discover_characteristics_descriptors
 *   with an handle range starting at characteristic value handle + 1 and ending
 *   at the last characteristic handle.
 *   - The pal will reply with a FindInformationResponse containing the
 *   descriptors discovered.
 * The operation is repeated until the response contains an attribute with an
 * handle equal to the last handle of the range or the pal reply with an error
 * (attribute not found). The termination callback is then call with a status
 * containing BLE_ERROR_NONE.
 */
TEST_P(TestGattClientDescriptorDiscoveryP, descriptor_discovery) {
	uint16_t current_handle = _characteristic.getValueHandle() + 1;
	auto descriptors = _descriptors;

	InSequence seq;

	while (true) {
		//Log::info() << "expect discover_characteristics_descriptors(" << current_handle << ", " << _last_handle << ")" << std::endl;
		EXPECT_CALL(
			_mock_client, discover_characteristics_descriptors(
				_connection_handle,
				attribute_handle_range(current_handle, _last_handle)
			)
		).WillOnce(Invoke([&, descriptors, current_handle](auto connection, auto range) {
			if (descriptors.empty()) {
				// send an attribute not found response
				_mock_client.on_server_event(
					connection,
					AttErrorResponse(
						AttributeOpcode::FIND_INFORMATION_REQUEST,
						current_handle,
						AttErrorResponse::ATTRIBUTE_NOT_FOUND
					)
				);
			} else {
				_mock_client.on_server_event(
					connection,
					MockFindInformationResponse(descriptors.front())
				);
			}
			return BLE_ERROR_NONE;
		}));

		if (descriptors.empty() == false) {
			for (const auto& descriptor : descriptors.front()) {
				DiscoveredCharacteristicDescriptor discovered_descriptor(
					&_gatt_client,
					_connection_handle,
					descriptor.first,
					descriptor.second
				);

				//Log::info() << "expect _descriptor_cb(" << _connection_handle << ", " << descriptor.first << ", " << descriptor.second << ")" << std::endl;

				EXPECT_CALL(
					_descriptor_cb, call(AllOf(
						ResultOf(get_characteristic, _characteristic),
						ResultOf(get_descriptor, discovered_descriptor)
					))
				);
			}
		}

		if (descriptors.empty() ||
			std::any_of(
				begin(descriptors.front()),
				end(descriptors.front()),
				[&](auto& val) { return val.first == _last_handle; }
			)
		) {
			//Log::info() << "expect termination" << std::endl;
			EXPECT_CALL(
				_termination_cb, call(Pointee(CharacteristicDescriptorDiscovery::TerminationCallbackParams_t {
					_characteristic,
					BLE_ERROR_NONE,
					0x00
				}))
			);
			break;
		} else {
			current_handle = descriptors.front().back().first + 1;
			descriptors.erase(begin(descriptors));
		}
	}

	ble_error_t err = _gatt_client.discoverCharacteristicDescriptors(
		_characteristic,
		makeFunctionPointer(&_descriptor_cb, &descriptor_callback_t::call),
		makeFunctionPointer(&_termination_cb, &descriptor_termination_callback_t::call)
	);

	EXPECT_EQ(err, BLE_ERROR_NONE);
}

// Instantiation of the tests cases relying on the parametric fixture
INSTANTIATE_TEST_CASE_P(
	TestGattClientDescriptorDiscoveryP_combination,
	TestGattClientDescriptorDiscoveryP,
	::testing::Values(
		// single transaction, single 16 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				}
			}
		},
		// single transaction, single 16 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1001,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				}
			}
		},
		// single transaction, single 128 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID("1881e01d-02af-41e9-abe9-bc940c09ca65") }
				}
			}
		},
		// single transaction, single 128 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1001,
			{
				{
					{ 0x1001, UUID("8a239fd3-61a3-44a4-8b7c-2db9a9baa3c8") }
				}
			}
		},

		// single transaction, multiple 16 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) },
					{ 0x1030, UUID(0xFFAC) },
					{ 0x1040, UUID(0xFFAD) }
				}
			}
		},
		// single transaction, multiple 16 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1040,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) },
					{ 0x1030, UUID(0xFFAC) },
					{ 0x1040, UUID(0xFFAD) }
				}
			}
		},
		// single transaction, multiple 128 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") },
					{ 0x1020, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") },
					{ 0x1040, UUID("86c7c947-c079-46e4-8bc4-7bfc011b7ffe") }
				}
			}
		},
		// single transaction, multiple 128 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1040,
			{
				{
					{ 0x1001, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") },
					{ 0x1020, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") },
					{ 0x1040, UUID("86c7c947-c079-46e4-8bc4-7bfc011b7ffe") }
				}
			}
		},

		// multiple transaction, single 16 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				},
				{
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1030, UUID(0xFFAC) }
				}
			}
		},
		// single transaction, single 16 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1030,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				},
				{
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1030, UUID(0xFFAC) }
				}
			}
		},
		// multiple transaction, single 128 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID("1881e01d-02af-41e9-abe9-bc940c09ca65") }
				},
				{
					{ 0x1015, UUID("0eb7f338-cd2a-4220-9f41-b61d95485b8d") }
				},
				{
					{ 0x1050, UUID("ac77d105-73eb-4cfd-959e-c191feb592b8") }
				}
			}
		},
		// multiple transaction, single 128 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1050,
			{
				{
					{ 0x1001, UUID("1881e01d-02af-41e9-abe9-bc940c09ca65") }
				},
				{
					{ 0x1015, UUID("0eb7f338-cd2a-4220-9f41-b61d95485b8d") }
				},
				{
					{ 0x1050, UUID("ac77d105-73eb-4cfd-959e-c191feb592b8") }
				}
			}
		},

		// multiple transaction, multiple 16 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1030, UUID(0xFFAC) },
					{ 0x1040, UUID(0xFFAD) }
				},
				{
					{ 0x1055, UUID(0xFFAE) },
					{ 0x1075, UUID(0xFFAF) }
				}
			}
		},
		// multiple transaction, multiple 16 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1075,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1030, UUID(0xFFAC) },
					{ 0x1040, UUID(0xFFAD) }
				},
				{
					{ 0x1055, UUID(0xFFAE) },
					{ 0x1075, UUID(0xFFAF) }
				}
			}
		},
		// multiple transaction, multiple 128 bit value in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") }
				},
				{
					{ 0x1020, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") },
				},
				{
					{ 0x1040, UUID("86c7c947-c079-46e4-8bc4-7bfc011b7ffe") }
				}
			}
		},
		// multiple transaction, multiple 128 bit value in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1040,
			{
				{
					{ 0x1001, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") }
				},
				{
					{ 0x1020, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") },
				},
				{
					{ 0x1040, UUID("86c7c947-c079-46e4-8bc4-7bfc011b7ffe") }
				}
			}
		},

		// multiple transaction, mixed UUID in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				},
				{
					{ 0x1020, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") }
				},
				{
					{ 0x1025, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") }
				},
				{
					{ 0x1030, UUID(0xFFAC) }
				}
			}
		},
		// multiple transaction, mixed UUID in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1030,
			{
				{
					{ 0x1001, UUID(0xFFAA) }
				},
				{
					{ 0x1020, UUID("0b20ef0c-578b-4b8a-8c59-065d4aff5b2e") }
				},
				{
					{ 0x1025, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") }
				},
				{
					{ 0x1030, UUID(0xFFAC) }
				}
			}
		},

		// multiple transaction, mixed multiple UUIDs in the transaction, ends with error
		test_param_t {
			0x1000,
			0x1100,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1025, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") }
				},
				{
					{ 0x1055, UUID(0xFFAE) },
					{ 0x1075, UUID(0xFFAF) }
				}
			}
		},
		// multiple transaction, mixed multiple UUIDs in the transaction, ends without error
		test_param_t {
			0x1000,
			0x1075,
			{
				{
					{ 0x1001, UUID(0xFFAA) },
					{ 0x1020, UUID(0xFFAB) }
				},
				{
					{ 0x1025, UUID("66ef3556-8889-4741-adfc-c89d7ff22710") },
					{ 0x1030, UUID("96a16d34-46d6-4081-82c3-25420ba01e5b") }
				},
				{
					{ 0x1055, UUID(0xFFAE) },
					{ 0x1075, UUID(0xFFAF) }
				}
			}
		}
	)
);

