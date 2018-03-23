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

using ble::pal::AttErrorResponse;
using ble::pal::AttributeOpcode;
using ble::pal::AttReadResponse;
using ble::pal::AttReadBlobResponse;
using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ble::connection_handle_t;
using ble::attribute_handle_t;
using ble::make_const_ArrayView;

using ::testing::_;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::Mock;
using ::testing::Pointee;

using std::vector;
using std::tuple;

static vector<uint8_t> make_char_value(uint16_t length) {
	vector<uint8_t> characteristic_value(length);
	for (auto& byte : characteristic_value) {
		byte = std::rand();
	}
	return characteristic_value;
}

/*
 * fixture used to test GattClient::read.
 */
class TestGattClientRead : public ::testing::Test {
protected:
	TestGattClientRead() :
		_mock_client(),
		_gatt_client(&_mock_client),
		_connection_handle(0xDEAD),
		_attribute_handle(0x5645),
		_read_cb(),
		_mtu_size(23) {
	}

	virtual void SetUp() {
		_gatt_client.onDataRead(
			makeFunctionPointer(&_read_cb, &read_callback_t::call)
		);

		ON_CALL(_mock_client, get_mtu_size(_connection_handle, _)).
		WillByDefault(Invoke([&](auto, auto& size){
			size = this->_mtu_size;
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
	read_callback_t _read_cb;
	uint16_t _mtu_size;
};

/*
 * Given an attribute on a server with a value with a length less than ATT_MTU - 1.
 * When the client issue tries to read is with GattClient::read with an offset
 * of 0.
 * Then:
 *    - The function read_attribute_value from the pal is called with
 *      connection handle and attribute handle used in GattClient::read.
 *    - The pal::GattClient fire an AttReadResponse containing the value of the
 *      attribute.
 *    - The generic GattClient invoke the read handler with a content equal to
 *      the value received and an error _status set to BLE_ERROR_NONE.
 */
TEST_F(TestGattClientRead, read_short_characteristic_without_offset_shall_succeed) {
	uint16_t offset = 0; // force offset to 0 for this test

	auto char_value = make_char_value(1000);

	// iterate over possible MTU size
	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 5) {
		// iterate over possible characteristic value size
		for (uint16_t char_len = 0; char_len < (_mtu_size - 1); ++char_len) {
			set_mtu_size_stub();

			InSequence seq;

			EXPECT_CALL(
				_mock_client, read_attribute_value(_connection_handle, _attribute_handle)
			).WillOnce(Invoke([&](auto connection, auto attribute) {
				_mock_client.on_server_event(
					connection,
					AttReadResponse(
						make_const_ArrayView(char_value.data(), char_len)
					)
				);
				return BLE_ERROR_NONE;
			}));

			EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
				_connection_handle,
				_attribute_handle,
				offset,
				(uint16_t) char_len,
				char_value.data(),
				BLE_ERROR_NONE
			})));

			ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
			EXPECT_EQ(err, BLE_ERROR_NONE);

			bool can_continue = Mock::VerifyAndClearExpectations(&_read_cb) && Mock::VerifyAndClearExpectations(&_mock_client);
			if (!can_continue) {
				return;
			}
		}
	}
}

/*
 * Given an attribute on a server with a value with a length less than ATT_MTU - 1.
 * When the client issue tries to reads it with GattClient::read with an offset
 * not equal to 0 and in the range of the attribute length.
 * Then:
 *    - The function read_attribute_blob from the pal is called with
 *      connection handle, the attribute handle and the offset used in
 *      GattClient::read.
 *    - The pal::GattClient fire an AttErrorresponse with:
 *        * opcode equal to READ_BLOB_REQUEST
 *        * _error_code equal to ATTRIBUTE_NOT_LONG
 *    - The generic GattClient invoke the read handler with the _status set to
 *      BLE_ERROR_PARAM_OUT_OF_RANGE and an _error_code set to ATTRIBUTE_NOT_LONG.
 */
TEST_F(TestGattClientRead, read_short_characteristic_with_offset_shall_fail) {
	// iterate over possible MTU size
	for (_mtu_size = 23; _mtu_size < 517; _mtu_size+=10) {
		// iterate over possible characteristic value size
		for (uint16_t char_len = 1; char_len < (_mtu_size - 1); char_len += 10) {
			// iterate over possible offsets
			for (uint16_t offset = 1; offset <= char_len; offset += 10) {

				InSequence seq;

				EXPECT_CALL(
					_mock_client, read_attribute_blob(_connection_handle, _attribute_handle, offset)
				).WillOnce(Invoke([&](auto connection, auto attribute, auto off) {
					_mock_client.on_server_event(
						connection,
						AttErrorResponse(
							AttributeOpcode::READ_BLOB_REQUEST,
							attribute,
							AttErrorResponse::ATTRIBUTE_NOT_LONG
						)
					);
					return BLE_ERROR_NONE;
				}));

				EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
					_connection_handle,
					_attribute_handle,
					offset,
					AttErrorResponse::ATTRIBUTE_NOT_LONG,
					NULL,
					BLE_ERROR_PARAM_OUT_OF_RANGE
				})));

				ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
				EXPECT_EQ(err, BLE_ERROR_NONE);

				bool can_continue = Mock::VerifyAndClearExpectations(&_read_cb) && Mock::VerifyAndClearExpectations(&_mock_client);
				if (!can_continue) {
					return;
				}
			}
		}
	}
}

/*
 * Given an attribute on a server with a value with a length less than ATT_MTU - 1.
 * When the client tries to reads it with GattClient::read with an offset
 * out of the range of the attribute length.
 * Then:
 *    - The function read_attribute_blob from the pal is called with
 *      connection handle, the attribute handle and the offset used in
 *      GattClient::read.
 *    - The pal::GattClient fire an AttErrorresponse with:
 *        * opcode equal to READ_BLOB_REQUEST
 *        * _error_code equal to INVALID_OFFSET
 *    - The generic GattClient invoke the read handler with the _status set to
 *      BLE_ERROR_PARAM_OUT_OF_RANGE and an _error_code set to INVALID_OFFSET.
 */
TEST_F(TestGattClientRead, read_with_out_of_range_offset_shall_fail) {
	// iterate over possible MTU size
	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		// iterate over possible characteristic value size
		for (uint16_t char_len = 1; char_len < (_mtu_size - 1); char_len += 10) {
			// iterate over possible offsets
			for (uint16_t offset = char_len + 1; offset < 518; offset += 10) {
				InSequence seq;

				EXPECT_CALL(
					_mock_client, read_attribute_blob(_connection_handle, _attribute_handle, offset)
				).WillOnce(Invoke([&](auto connection, auto attribute, auto off) {
					_mock_client.on_server_event(
						connection,
						AttErrorResponse(
							AttributeOpcode::READ_BLOB_REQUEST,
							attribute,
							AttErrorResponse::INVALID_OFFSET
						)
					);
					return BLE_ERROR_NONE;
				}));

				EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
					_connection_handle,
					_attribute_handle,
					offset,
					AttErrorResponse::INVALID_OFFSET,
					NULL,
					BLE_ERROR_PARAM_OUT_OF_RANGE,
				})));

				ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
				EXPECT_EQ(err, BLE_ERROR_NONE);

				bool can_continue = Mock::VerifyAndClearExpectations(&_read_cb) && Mock::VerifyAndClearExpectations(&_mock_client);
				if (!can_continue) {
					return;
				}
			}
		}
	}
}

/*
 * Parametric fixture used to test error generated during GattClient::read.
 * Parameters are:
 * - [0] The attribute error code
 * - [1] Expected status returned in the read callback.
 */
class TestGattClientReadAttributeError :
	public TestGattClientRead,
	public ::testing::WithParamInterface<tuple<AttErrorResponse::AttributeErrorCode, ble_error_t>> {
protected:
	TestGattClientReadAttributeError() :
		TestGattClientRead(),
		_error_code((AttErrorResponse::AttributeErrorCode) 0x00),
		_status(BLE_ERROR_NONE) {
	}

	virtual void SetUp() {
		TestGattClientRead::SetUp();
		std::tie(_error_code, _status) = GetParam();
	}

	AttErrorResponse::AttributeErrorCode _error_code;
	ble_error_t _status;
};

/*
 * Given an attribute which cannot be read.
 * When the client tries to reads it with GattClient::read with an offset
 * of 0
 * Then:
 *    - The function read_attribute_value from the pal is called with
 *      connection handle, the attribute handle in GattClient::read.
 *    - The pal::GattClient fire an AttErrorresponse with:
 *        * opcode equal to READ_REQUEST
 *        * _error_code equal to the expected error
 *    - The generic GattClient invoke the read handler with the _status set to
 *      the expected _status and an _error_code set to error code in the
 *      AttErrorResponse.
 */
TEST_P(TestGattClientReadAttributeError, read_no_offset) {
	uint16_t offset = 0;

	InSequence seq;
	EXPECT_CALL(
		_mock_client, read_attribute_value(_connection_handle, _attribute_handle)
	).WillOnce(Invoke([&](auto connection, auto attribute) {
		_mock_client.on_server_event(
			connection,
			AttErrorResponse(
				AttributeOpcode::READ_REQUEST,
				attribute,
				_error_code
			)
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
		_connection_handle,
		_attribute_handle,
		offset,
		_error_code,
		/* data */ NULL,
		_status
	})));

	ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
	EXPECT_EQ(err, BLE_ERROR_NONE);
}

/*
 * Given an attribute which cannot be read.
 * When the client tries to reads it with GattClient::read with an offset
 * of 0
 * Then:
 *    - The function read_attribute_blob from the pal is called with
 *      connection handle, the attribute handle and the offset passed to
 *      GattClient::read.
 *    - The pal::GattClient fire an AttErrorresponse with:
 *        * opcode equal to READ_BLOB_REQUEST
 *        * _error_code equal to the expected error
 *    - The generic GattClient invoke the read handler with the _status set to
 *      the expected _status and an _error_code set to error code in the
 *      AttErrorResponse.
 */
TEST_P(TestGattClientReadAttributeError, read_with_offset) {
	uint16_t offset = 10;

	InSequence seq;

	EXPECT_CALL(
		_mock_client, read_attribute_blob(_connection_handle, _attribute_handle, offset)
	).WillOnce(Invoke([&](auto connection, auto attribute, auto off) {
		_mock_client.on_server_event(
			connection,
			AttErrorResponse(
				AttributeOpcode::READ_BLOB_REQUEST,
				attribute,
				_error_code
			)
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
		_connection_handle,
		_attribute_handle,
		offset,
		_error_code,
		/* data */ NULL,
		_status
	})));

	ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
	EXPECT_EQ(err, BLE_ERROR_NONE);
}

// Instantiate test cases using TestGattClientReadAttributeError
INSTANTIATE_TEST_CASE_P(
	TestGattClientReadAttributeError_combination,
	TestGattClientReadAttributeError,
	::testing::Values(
		std::make_tuple(AttErrorResponse::INVALID_HANDLE, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHORIZATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHENTICATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::READ_NOT_PERMITTED, BLE_ERROR_OPERATION_NOT_PERMITTED)
	)
);

/*
 * Given a readable attribute with a value longer than ATT_MTU - 2.
 * When the GattClient::read is called without offset
 * Then:
 *   - The client call the pal function read_attribute_value with the connection
 *     handle and the the attribute handle used in GattClient::read.
 *   When the pal::GattClient fire an AttReadResponse containing the first
 *   (ATT_MTU - 1) bytes of the attribute value.
 *   Then:
 *     - The client call the function read_attribute_blob with the connection
 *       handle and the attribute handle used in GattClient::read. The offset is
 *       equal of the number of bytes read in the characteristic.
 *     When the pal::Client fire an AttReadBlobResponse containing the partial value
 *     of the characteristic starting at the offset requested. If the length of
 *     the remaining bytes is greater than ATT_MTU - 1 then the length of the
 *     partial value in the response shall be ATT_MTU - 1. Otherwise it is the
 *     length of the remaining bytes.
 *     Then:
 *       - If the value received by the client has a len greater than ATT_MTU - 1.
 *         Then the client issue a new read_attribute_blob with an updated index.
 *       - Else the read callback is called with the value of the attribute read.
 */
TEST_F(TestGattClientRead, read_long_characteristic_without_offset_shall_succeed) {
	const uint16_t offset = 0; // force offset to 0 for this test

	auto char_value = make_char_value(517 * 6);

	// iterate over possible MTU size
	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 5) {
		// iterate over possible characteristic value size
		for (uint16_t char_len = (_mtu_size - 1); char_len < (_mtu_size * 6); char_len += 10) {
			set_mtu_size_stub();

			InSequence seq;
			EXPECT_CALL(
				_mock_client, read_attribute_value(_connection_handle, _attribute_handle)
			).WillOnce(Invoke([&](auto connection, auto attribute) {
				_mock_client.on_server_event(
					connection,
					AttReadResponse(
						make_const_ArrayView(char_value.data(), _mtu_size - 1)
					)
				);
				return BLE_ERROR_NONE;
			}));

			for(uint16_t blob_offset = (_mtu_size - 1); blob_offset <= char_len; blob_offset += (_mtu_size - 1)) {
				EXPECT_CALL(
					_mock_client, read_attribute_blob(_connection_handle, _attribute_handle, blob_offset)
				).WillOnce(Invoke([&](auto connection, auto attribute, uint16_t offset) {
					_mock_client.on_server_event(
						connection,
						AttReadBlobResponse(
							make_const_ArrayView(
								char_value.data() + offset,
								(char_len - offset) > (_mtu_size - 1) ? (_mtu_size - 1) : (char_len - offset)
							)
						)
					);

					return BLE_ERROR_NONE;
				}));
			}

			EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
				_connection_handle,
				_attribute_handle,
				offset,
				(uint16_t) char_len,
				char_value.data(),
				BLE_ERROR_NONE
			})));

			ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
			EXPECT_EQ(err, BLE_ERROR_NONE);

			bool can_continue = Mock::VerifyAndClearExpectations(&_mock_client) && Mock::VerifyAndClearExpectations(&_read_cb);
			if (!can_continue) {
				return;
			}
		}
	}
}

/*
 * Given a readable attribute with a value longer than ATT_MTU - 2.
 * When the GattClient::read is called without offset
 * Then:
 *   - The client call the pal function read_attribute_blob with the connection
 *     handle, the attribute handle and the offset used in GattClient::read.
 *   When the pal::GattClient fire an AttReadResponse containing the first bytes
 *   of the attribute value.
 *   Then:
 *     - If the response length is less than (ATT_MTU - 1), the read callback is
 *       called with the value of the attribute read.
 *     - The client call the function read_attribute_blob with the connection
 *       handle and the attribute handle used in GattClient::read. The offset is
 *       equal of the number of bytes read in the characteristic plus the
 *       first offset.
 *     When the pal::Client fire an AttReadBlobResponse containing the partial value
 *     of the characteristic starting at the offset requested. If the length of
 *     the remaining bytes is greater than ATT_MTU - 1 then the length of the
 *     partial value in the response shall be ATT_MTU - 1. Otherwise it is the
 *     length of the remaining bytes.
 *     Then:
 *       - If the value received by the client has a len greater than ATT_MTU - 1.
 *         Then the client issue a new read_attribute_blob with an updated index.
 *       - Else the read callback is called with the value of the attribute read.
 */
TEST_F(TestGattClientRead, read_long_characteristic_with_offset_shall_succeed) {
	auto char_value = make_char_value(517 * 6);

	// iterate over possible MTU size
	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 35) {
		// iterate over possible characteristic value size
		for (uint16_t char_len = (_mtu_size - 1); char_len < (_mtu_size * 4); char_len += 30) {
			for(uint16_t offset = 1; offset < char_len; offset += 20) {
				set_mtu_size_stub();

				InSequence seq;
				for(uint16_t blob_offset = offset; blob_offset <= char_len; blob_offset += (_mtu_size - 1)) {
					EXPECT_CALL(
						_mock_client, read_attribute_blob(_connection_handle, _attribute_handle, blob_offset)
					).WillOnce(Invoke([&](auto connection, auto attribute, auto offset) {
						_mock_client.on_server_event(
							connection,
							AttReadBlobResponse(
								make_const_ArrayView(
									char_value.data() + offset,
									(char_len - offset) > (_mtu_size - 1) ? (_mtu_size - 1) : (char_len - offset)
								)
							)
						);

						return BLE_ERROR_NONE;
					}));
				}

				EXPECT_CALL(_read_cb, call(Pointee(GattReadCallbackParams {
					_connection_handle,
					_attribute_handle,
					offset,
					(uint16_t) (char_len - offset),
					char_value.data() + offset,
					BLE_ERROR_NONE
				})));

				ble_error_t err = _gatt_client.read(_connection_handle, _attribute_handle, offset);
				EXPECT_EQ(err, BLE_ERROR_NONE);

				bool can_continue = Mock::VerifyAndClearExpectations(&_mock_client) && Mock::VerifyAndClearExpectations(&_read_cb);
				if (!can_continue) {
					return;
				}
			}
		}
	}
}


// TODO: Add test for failure in the middle of the transaction
