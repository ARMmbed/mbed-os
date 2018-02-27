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
using ble::pal::AttWriteResponse;
using ble::pal::AttPrepareWriteResponse;
using ble::pal::AttExecuteWriteResponse;
using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ble::connection_handle_t;
using ble::attribute_handle_t;
using ble::ArrayView;
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
 * Fixture used for GattClient::write tests.
 */
class TestGattClientWrite : public ::testing::Test {
protected:
	TestGattClientWrite() :
		_mock_client(),
		_gatt_client(&_mock_client),
		_connection_handle(0xDEAD),
		_attribute_handle(0x5645),
		_write_cb(),
		_mtu_size(23) {
	}

	virtual void SetUp() {
		_gatt_client.onDataWritten(
			makeFunctionPointer(&_write_cb, &write_callback_t::call)
		);

		ON_CALL(
			_mock_client, get_mtu_size(_connection_handle, _)
		).WillByDefault(Invoke([&](auto, auto& mtu_size){
			mtu_size = _mtu_size;
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
	write_callback_t _write_cb;
	uint16_t _mtu_size;
};

/**
 * Given a payload shorter  than the current MTU size - 2.
 * When the payload is sent in a write command
 * Then the payload is sent to the pal without error.
 */
TEST_F(TestGattClientWrite, write_command_should_succeed_if_it_fit_in_the_payload) {
	auto value = make_char_value(1000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for(uint16_t len = 0; len <= (_mtu_size - 3); len += 10) {
			set_mtu_size_stub();

			EXPECT_CALL(
				_mock_client, write_without_response(
					_connection_handle,
					_attribute_handle,
					make_const_ArrayView(value.data(), len)
				)
			);

			ble_error_t error = _gatt_client.write(
				GattClient::GATT_OP_WRITE_CMD,
				_connection_handle,
				_attribute_handle,
				len,
				value.data()
			);

			EXPECT_EQ(error, BLE_ERROR_NONE);

			bool can_continue =
				Mock::VerifyAndClearExpectations(&_write_cb) &&
				Mock::VerifyAndClearExpectations(&_mock_client);

			if (!can_continue) {
				return;
			}
		}
	}
}

/**
 * Given a payload with a length longer than MTU size -1
 * When the payload is sent in a write command
 * Then the function returns an error.
 */
TEST_F(TestGattClientWrite, write_command_should_fail_if_it_doesnt_fit_in_the_payload) {
	auto value = make_char_value(1000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for(size_t len = (_mtu_size - 3) + 1; len < 517; len += 10) {
			set_mtu_size_stub();

			ble_error_t error = _gatt_client.write(
				GattClient::GATT_OP_WRITE_CMD,
				_connection_handle,
				_attribute_handle,
				len,
				value.data()
			);

			EXPECT_EQ(error, BLE_ERROR_PARAM_OUT_OF_RANGE);

			bool can_continue =
				Mock::VerifyAndClearExpectations(&_write_cb) &&
				Mock::VerifyAndClearExpectations(&_mock_client) &&
				(error == BLE_ERROR_PARAM_OUT_OF_RANGE);

			if (!can_continue) {
				return;
			}
		}
	}
}

// TODO: test unexpected results from write_without_response

/**
 * Given a payload shorter than the current MTU size - 2.
 * When the payload is sent in a write request
 * Then if the operation succeed the write callback shall be invoked.
 */
TEST_F(TestGattClientWrite, write_request_short_characteristics) {
	auto value = make_char_value(1000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for(uint16_t len = 0; len <= (_mtu_size - 3); len += 10) {
			set_mtu_size_stub();

			EXPECT_CALL(
				_mock_client, write_attribute(
					_connection_handle,
					_attribute_handle,
					make_const_ArrayView(value.data(), len)
				)
			).WillOnce(Invoke([&](auto conn_handle, auto, const auto&) {
				_mock_client.on_server_event(
					conn_handle,
					AttWriteResponse()
				);
				return BLE_ERROR_NONE;
			}));

			EXPECT_CALL(_write_cb, call(Pointee(GattWriteCallbackParams {
				_connection_handle,
				_attribute_handle,
				GattWriteCallbackParams::OP_WRITE_REQ,
				BLE_ERROR_NONE,
				/* error code */ 0,
			})));

			ble_error_t error = _gatt_client.write(
				GattClient::GATT_OP_WRITE_REQ,
				_connection_handle,
				_attribute_handle,
				len,
				value.data()
			);

			EXPECT_EQ(error, BLE_ERROR_NONE);

			bool can_continue =
				Mock::VerifyAndClearExpectations(&_write_cb) &&
				Mock::VerifyAndClearExpectations(&_mock_client);

			if (!can_continue) {
				return;
			}
		}
	}
}

/*
 * Fixture used to test error handling during a characteristic write.
 * Parameters are:
 * [0] AttErrorResponse::AttributeErrorCode The attribute error code to push in
 * the AttErrorresponse sent to the client.
 * [1] ble_error_t The expected error status present in the parameters received
 * by the callback handling GattClient::write.
 */
class TestGattClientWriteAttributeError :
	public TestGattClientWrite,
	public ::testing::WithParamInterface<tuple<AttErrorResponse::AttributeErrorCode, ble_error_t>> {
protected:
	TestGattClientWriteAttributeError() :
		TestGattClientWrite(),
		error_code((AttErrorResponse::AttributeErrorCode) 0x00),
		status(BLE_ERROR_NONE) {
	}

	virtual void SetUp() {
		TestGattClientWrite::SetUp();
		std::tie(error_code, status) = GetParam();
	}

	AttErrorResponse::AttributeErrorCode error_code;
	ble_error_t status;
};

/**
 * Given a payload shorter  than the current MTU size - 2.
 * When the payload is sent in a write request
 * Then if the operation fail the write callback shall report the error
 */
TEST_P(TestGattClientWriteAttributeError, write_request_short_characteristics) {
	auto value = make_char_value(1000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for(uint16_t len = 0; len <= (_mtu_size - 3); len += 10) {
			set_mtu_size_stub();

			EXPECT_CALL(
				_mock_client, write_attribute(
					_connection_handle,
					_attribute_handle,
					make_const_ArrayView(value.data(), len)
				)
			).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto&) {
				_mock_client.on_server_event(
					conn_handle,
					AttErrorResponse(
						AttributeOpcode::WRITE_REQUEST,
						att_handle,
						error_code
					)
				);
				return BLE_ERROR_NONE;
			}));

			EXPECT_CALL(
				_write_cb, call(Pointee(GattWriteCallbackParams{
					_connection_handle,
					_attribute_handle,
					GattWriteCallbackParams::OP_PREP_WRITE_REQ,
					status,
					error_code
				}))
			);

			ble_error_t error = _gatt_client.write(
				GattClient::GATT_OP_WRITE_REQ,
				_connection_handle,
				_attribute_handle,
				len,
				value.data()
			);

			EXPECT_EQ(error, BLE_ERROR_NONE);

			bool can_continue =
				Mock::VerifyAndClearExpectations(&_write_cb) &&
				Mock::VerifyAndClearExpectations(&_mock_client);

			if (!can_continue) {
				return;
			}
		}
	}
}

// Instantiate test cases using the TestGattClientWriteAttributeError fixture
INSTANTIATE_TEST_CASE_P(
	TestGattClientWriteAttributeError_combination,
	TestGattClientWriteAttributeError,
	::testing::Values(
		std::make_tuple(AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::INVALID_HANDLE, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHORIZATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHENTICATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::WRITE_NOT_PERMITTED, BLE_ERROR_OPERATION_NOT_PERMITTED),
		std::make_tuple((AttErrorResponse::AttributeErrorCode)/* application error */ 0x80, BLE_ERROR_UNSPECIFIED)
	)
);

/**
 * Given a payload with a length bigger than the current MTU size - 2.
 * When the payload is written as using Gattclient::write
 * Then the payload is split into multiple chunks of a maximum size equal to
 * MTU_size - 5. Those chunks are sent sequentially using a prepare write
 * request which is answered by a prepare write response by the server.
 * Once all the chunks have been received by the server, the client sends an
 * Execute write request. Once the client received an Execute Write request,
 * the server shall invoke the write callback.
 */
TEST_F(TestGattClientWrite, write_request_long_characteristics) {
	auto value = make_char_value(10000);

	for (_mtu_size = 23; _mtu_size < 517; _mtu_size += 10) {
		for(uint16_t len = (_mtu_size - 2); len <= (_mtu_size * 4); len += 10) {
			set_mtu_size_stub();

			InSequence seq;

			for (uint16_t offset = 0; offset < len; offset += (_mtu_size - 5)) {
				EXPECT_CALL(
					_mock_client, queue_prepare_write(
						_connection_handle,
						_attribute_handle,
						make_const_ArrayView(
							value.data() + offset,
							((len - offset) >  (_mtu_size - 5)) ? (_mtu_size - 5) : (len - offset)
						),
						offset
					)
				).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto& data, auto offset) {
					_mock_client.on_server_event(
						conn_handle,
						AttPrepareWriteResponse(
							att_handle,
							offset,
							data
						)
					);
					return BLE_ERROR_NONE;
				}));
			}

			EXPECT_CALL(
				_mock_client, execute_write_queue(
					_connection_handle,
					true
				)
			).WillOnce(Invoke([&](auto conn_handle, bool execute) {
				_mock_client.on_server_event(
					conn_handle,
					AttExecuteWriteResponse()
				);
				return BLE_ERROR_NONE;
			}));

			EXPECT_CALL(
				_write_cb, call(Pointee(GattWriteCallbackParams{
					_connection_handle,
					_attribute_handle,
					GattWriteCallbackParams::OP_WRITE_REQ,
					BLE_ERROR_NONE,
					/* error code */ 0,
				}))
			);

			ble_error_t error = _gatt_client.write(
				GattClient::GATT_OP_WRITE_REQ,
				_connection_handle,
				_attribute_handle,
				len,
				value.data()
			);

			EXPECT_EQ(error, BLE_ERROR_NONE);

			bool can_continue =
				Mock::VerifyAndClearExpectations(&_write_cb) &&
				Mock::VerifyAndClearExpectations(&_mock_client);

			if (!can_continue) {
				return;
			}
		}
	}
}

/*
 * Fixture used to test long write errors.
 * Parameter are similar to the one used in TestGattClientWriteAttributeError
 */
class TestGattClientPrepareWriteAttributeError : public TestGattClientWriteAttributeError { };

/**
 * Test errors received by a client at the beginning of a long write procedure.
 */
TEST_P(TestGattClientPrepareWriteAttributeError, prepare_write_error) {
	auto value = make_char_value(10000);

	_mtu_size = 23;
	set_mtu_size_stub();

	InSequence seq;

	EXPECT_CALL(
		_mock_client, queue_prepare_write(
			_connection_handle,
			_attribute_handle,
			make_const_ArrayView(
				value.data(),
				_mtu_size - 5
			),
			0
		)
	).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto&, auto offset) {
		_mock_client.on_server_event(
			conn_handle,
			AttErrorResponse(
				AttributeOpcode::PREPARE_WRITE_REQUEST,
				att_handle,
				error_code
			)
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(
		_mock_client, execute_write_queue(_connection_handle, false)
	).WillOnce(Invoke([&](auto conn_handle, bool execute) {
		_mock_client.on_server_event(
			conn_handle,
			AttExecuteWriteResponse()
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(
		_write_cb, call(Pointee(GattWriteCallbackParams{
			_connection_handle,
			_attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			status,
			/* error code */ error_code,
		}))
	);

	ble_error_t error = _gatt_client.write(
		GattClient::GATT_OP_WRITE_REQ,
		_connection_handle,
		_attribute_handle,
		_mtu_size * 10,
		value.data()
	);

	EXPECT_EQ(error, BLE_ERROR_NONE);
}

// Instantiate test cases relying on TestGattClientPrepareWriteAttributeError
INSTANTIATE_TEST_CASE_P(
	TestGattClientPrepareWriteAttributeError_combination,
	TestGattClientPrepareWriteAttributeError,
	::testing::Values(
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHORIZATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_AUTHENTICATION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INSUFFICIENT_ENCRYPTION, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::PREPARE_QUEUE_FULL, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INVALID_HANDLE, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::WRITE_NOT_PERMITTED, BLE_ERROR_OPERATION_NOT_PERMITTED),
		std::make_tuple((AttErrorResponse::AttributeErrorCode)/* application error */ 0x80, BLE_ERROR_UNSPECIFIED)
	)
);


/*
 * Fixture used to test errors in the middle of a long write transaction.
 * Parameter are similar to the one used in TestGattClientWriteAttributeError
 */
class TestGattClientPrepareWriteAttributeErrorInTransaction : public TestGattClientWriteAttributeError { };

/**
 * Test that errors are correctly reported to the client if an error occur in the
 * middle of a long write transaction.
 */
TEST_P(TestGattClientPrepareWriteAttributeErrorInTransaction, prepare_write_error_in_transaction) {
	auto value = make_char_value(10000);

	_mtu_size = 23;
	set_mtu_size_stub();

	InSequence seq;

	EXPECT_CALL(
		_mock_client, queue_prepare_write(
			_connection_handle,
			_attribute_handle,
			make_const_ArrayView(
				value.data(),
				_mtu_size - 5
			),
			/* offset */ 0
		)
	).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto&, auto) {
		_mock_client.on_server_event(
			conn_handle,
			AttPrepareWriteResponse(
				att_handle,
				0,
				make_const_ArrayView(value.data(), _mtu_size - 5)
			)
		);
		return BLE_ERROR_NONE;
	}));


	uint16_t offset = _mtu_size - 5;

	EXPECT_CALL(
		_mock_client, queue_prepare_write(
			_connection_handle,
			_attribute_handle,
			make_const_ArrayView(
				value.data() + offset,
				_mtu_size - 5
			),
			offset
		)
	).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto&, auto offset) {
		_mock_client.on_server_event(
			conn_handle,
			AttErrorResponse(
				AttributeOpcode::PREPARE_WRITE_REQUEST,
				att_handle,
				error_code
			)
		);
		return BLE_ERROR_NONE;
	}));


	EXPECT_CALL(
		_mock_client, execute_write_queue(_connection_handle, false)
	).WillOnce(Invoke([&](auto conn_handle, bool execute) {
		_mock_client.on_server_event(
			conn_handle,
			AttExecuteWriteResponse()
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(
		_write_cb, call(Pointee(GattWriteCallbackParams{
			_connection_handle,
			_attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			status,
			/* error code */ error_code,
		}))
	);

	ble_error_t error = _gatt_client.write(
		GattClient::GATT_OP_WRITE_REQ,
		_connection_handle,
		_attribute_handle,
		_mtu_size * 10,
		value.data()
	);

	EXPECT_EQ(error, BLE_ERROR_NONE);
}

// Instantiate test cases relying on TestGattClientPrepareWriteAttributeErrorInTransaction
INSTANTIATE_TEST_CASE_P(
	TestGattClientPrepareWriteAttributeErrorInTransaction_combination,
	TestGattClientPrepareWriteAttributeErrorInTransaction,
	::testing::Values(
		std::make_tuple(AttErrorResponse::PREPARE_QUEUE_FULL, BLE_ERROR_INVALID_STATE),
		std::make_tuple((AttErrorResponse::AttributeErrorCode)/* application error */ 0x80, BLE_ERROR_UNSPECIFIED)
	)
);

/*
 * Fixture used to test errors at the end of a long write transaction.
 * Parameter are similar to the one used in TestGattClientWriteAttributeError
 */
class TestGattClientExecuteWriteRequestError : public TestGattClientWriteAttributeError { };

/**
 * Test that errors are correctly reported to the client if an error occurs when
 * a write queue is executed.
 */
TEST_P(TestGattClientExecuteWriteRequestError, prepare_write_error_in_transaction) {
	auto value = make_char_value(10000);

	_mtu_size = 23;
	set_mtu_size_stub();
	const uint16_t len = _mtu_size * 5;

	InSequence seq;

	for (uint16_t offset = 0; offset < len; offset += (_mtu_size - 5)) {
		EXPECT_CALL(
			_mock_client, queue_prepare_write(
				_connection_handle,
				_attribute_handle,
				make_const_ArrayView(
					value.data() + offset,
					((len - offset) >  (_mtu_size - 5)) ? (_mtu_size - 5) : (len - offset)
				),
				offset
			)
		).WillOnce(Invoke([&](auto conn_handle, auto att_handle, const auto& data, auto offset) {
			_mock_client.on_server_event(
				conn_handle,
				AttPrepareWriteResponse(
					att_handle,
					offset,
					data
				)
			);
			return BLE_ERROR_NONE;
		}));
	}

	EXPECT_CALL(
		_mock_client, execute_write_queue(
			_connection_handle, true
		)
	).WillOnce(Invoke([&](connection_handle_t conn_handle, bool execute)  {
		_mock_client.on_server_event(
			conn_handle,
			AttErrorResponse(
				AttributeOpcode::EXECUTE_WRITE_REQUEST,
				_attribute_handle,
				error_code
			)
		);
		return BLE_ERROR_NONE;
	}));

	EXPECT_CALL(
		_write_cb, call(Pointee(GattWriteCallbackParams{
			_connection_handle,
			_attribute_handle,
			GattWriteCallbackParams::OP_WRITE_REQ,
			status,
			error_code
		}))
	);

	ble_error_t error = _gatt_client.write(
		GattClient::GATT_OP_WRITE_REQ,
		_connection_handle,
		_attribute_handle,
		len,
		value.data()
	);

	EXPECT_EQ(error, BLE_ERROR_NONE);
}

// Instantiate test cases relying on TestGattClientExecuteWriteRequestError
INSTANTIATE_TEST_CASE_P(
	TestGattClientExecuteWriteRequestError_combination,
	TestGattClientExecuteWriteRequestError,
	::testing::Values(
		std::make_tuple(AttErrorResponse::INVALID_HANDLE, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::WRITE_NOT_PERMITTED, BLE_ERROR_OPERATION_NOT_PERMITTED),
		std::make_tuple(AttErrorResponse::PREPARE_QUEUE_FULL, BLE_ERROR_INVALID_STATE),
		std::make_tuple(AttErrorResponse::INVALID_OFFSET, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH, BLE_ERROR_INVALID_PARAM),
		std::make_tuple(AttErrorResponse::WRITE_REQUEST_REJECTED, BLE_ERROR_OPERATION_NOT_PERMITTED),
		std::make_tuple((AttErrorResponse::AttributeErrorCode)/* application error */ 0x80, BLE_ERROR_UNSPECIFIED)
	)
);

