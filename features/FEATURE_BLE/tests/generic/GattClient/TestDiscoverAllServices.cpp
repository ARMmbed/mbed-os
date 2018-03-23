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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ble/generic/GenericGattClient.h"
#include "ble/pal/AttServerMessage.h"
#include "ble/pal/SimpleAttServerMessage.h"
#include "ble/DiscoveredService.h"

#include "mock/MockPalGattClient.h"
#include "mock/MockCallbacks.h"
#include "util/ServerDescription.h"
#include "util/PrettyPrinter.h"
#include "util/Equality.h"
#include "util/Log.h"

// imports
using ble::pal::SimpleAttFindByTypeValueResponse;
using ble::pal::SimpleAttReadByGroupTypeResponse;
using ble::pal::SimpleAttReadByTypeResponse;
using ble::pal::AttErrorResponse;
using ble::pal::AttributeOpcode;
using ble::attribute_handle_range_t;
using ble::generic::GenericGattClient;
using ble::pal::vendor::mock::MockPalGattClient;

using ble::connection_handle_t;
using ble::attribute_handle_t;
using ble::make_const_ArrayView;

using ::testing::_;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::AllOf;
using ::testing::Property;
using ::testing::InSequence;

/*
 * Parametric fixture used for service discovery.
 *
 * Parameters are modeled by a tuple where:
 * - [0] std::tuple<bool, bool>: pair of boolean indicating the presence of the
 * service callback and the characteristic callback.
 * - [1] UUID: UUID used for service filtering.
 * - [2] UUID: UUID used for characteristic filtering.
 * - [3] server_description_t: Discovered service layout.
 */
class LaunchDiscoveryNoServiceFilter : public ::testing::TestWithParam<
	std::tuple<std::tuple<bool, bool>, UUID, UUID, server_description_t>
> {
protected:
	LaunchDiscoveryNoServiceFilter() :
		mock_client(),
		gatt_client(&mock_client),
		connection_handle(0xDEAD),
		termination_callback(),
		has_service_cb(),
		service_callback(),
		has_characteristic_cb(),
		characteristic_callback(),
		service_filter(),
		characteristic_filter(),
		server_stub() {
	}

	// setup the test environment
	virtual void SetUp() {
		gatt_client.onServiceDiscoveryTermination(
			makeFunctionPointer(
				&termination_callback,
				&termination_callback_t::call
			)
		);

		std::tuple<bool, bool> cb_combination;
		std::tie(
			cb_combination,
			service_filter,
			characteristic_filter,
			server_stub
		) = GetParam();

		std::tie(has_service_cb, has_characteristic_cb) = cb_combination;
	}

	/*
	 * Return a closure that sends a read by group type response to a client.
	 * @param[in] service_transactions The services to include in the response.
	 */
	auto reply_read_by_group_type(const std::vector<service_description_t>& service_transactions) {
		return Invoke([service_transactions, this](connection_handle_t connection, attribute_handle_t handle) -> ble_error_t {
			//Log::info() << "discover primary service (" << connection << "," << handle << ")" << std::endl;
			uint8_t uuid_size = service_transactions.front().uuid.getLen();
			uint8_t element_size = uuid_size + 4;
			size_t data_size = element_size * service_transactions.size();
			std::vector<uint8_t> result(data_size);
			uint8_t* data = result.data();

			for (const auto& service : service_transactions) {
				memcpy(data, &service.start, 2);
				data += 2;
				memcpy(data, &service.end, 2);
				data += 2;
				memcpy(data, service.uuid.getBaseUUID(), uuid_size);
				data += uuid_size;
			}

			mock_client.on_server_event(
				connection,
				SimpleAttReadByGroupTypeResponse(
					element_size,
					make_const_ArrayView(result.data(), result.size())
				)
			);

			return BLE_ERROR_NONE;
		});
	}

	/*
	 * Return a closure that sends a find by type value response to the client.
	 * @param[in] service_transactions Services to include in the response sent
	 * to the client.
	 */
	auto reply_find_by_type_value(const std::vector<service_description_t>& service_transactions) {
		return Invoke([service_transactions, this](connection_handle_t connection, attribute_handle_t handle, const UUID& uuid) -> ble_error_t {
			//Log::info() << "discover primary service by uuid(" << connection << "," << handle << "," << uuid << ")" << std::endl;
			std::vector<uint8_t> result(service_transactions.size() * 4);
			uint8_t* data = result.data();
			for (const auto& service : service_transactions) {
				memcpy(data, &service.start, 2);
				data += 2;
				memcpy(data, &service.end, 2);
				data += 2;
			}

			mock_client.on_server_event(
				connection,
				SimpleAttFindByTypeValueResponse(
					make_const_ArrayView(result.data(), result.size())
				)
			);

			return BLE_ERROR_NONE;
		});
	}

	/**
	 * Convert a DiscoveredCharacteristic::Properties_t into an uint8_t
	 */
	uint8_t properties_to_byte(DiscoveredCharacteristic::Properties_t p) {
		return (
			p.broadcast() << 0 |
			p.read() << 1 |
			p.writeWoResp() << 2 |
			p.write() << 3 |
			p.notify() << 4 |
			p.indicate() << 5 |
			p.authSignedWrite() << 6
			/* extented properties ignored for now */
		);
	}

	/**
	 * Return a closure that sends a read by type to a client.
	 * @param[in] transaction Characteristics to include in the response send to
	 * the client.
	 */
	auto reply_read_by_type(const std::vector<characteristic_description_t>& transaction) {
		return Invoke([transaction, this](connection_handle_t connection, attribute_handle_range_t range) -> ble_error_t {
			//Log::info() << "discover characteristic(" << connection << "," << range.begin << "," << range.end << ")" << std::endl;
			size_t uuid_size = transaction.front().uuid.getLen();
			uint8_t element_size = uuid_size + 5;
			std::vector<uint8_t> result(element_size * transaction.size());
			uint8_t* data = result.data();
			for (const auto& characteristic : transaction) {
				memcpy(data, &characteristic.handle, 2);
				data += 2;
				*data++ = properties_to_byte(characteristic.properties);
				memcpy(data, &characteristic.value_handle, 2);
				data += 2;
				memcpy(data, characteristic.uuid.getBaseUUID(), uuid_size);
				data += uuid_size;
			}

			mock_client.on_server_event(
				connection,
				SimpleAttReadByTypeResponse(
					element_size,
					make_const_ArrayView(result.data(), result.size())
				)
			);

			return BLE_ERROR_NONE;
		});
	}

	/*
	 * Return a closure that send an error response to the client.
	 * @param[in] opcode Opcode that caused the error.
	 * @param[in] error_code Error code.
	 */
	auto reply_error(AttributeOpcode opcode, AttErrorResponse::AttributeErrorCode error_code) {
		return InvokeWithoutArgs([this, opcode, error_code]() -> ble_error_t {
			//Log::info() << "reply error: opcode = " << (uint8_t) opcode << ", error_code = " << error_code << std::endl;
			mock_client.on_server_event(
				connection_handle,
				AttErrorResponse(opcode, error_code)
			);
			return BLE_ERROR_NONE;
		});
	}

	/*
	 * Set service discovery expectation when the procedure discover all services
	 * is used.
	 */
	void set_discover_all_services_expectations() {
		auto services_transactions = get_services_transactions();

		uint16_t next_attribute_handle = 0x0001;
		for (const auto& service_transactions : services_transactions) {
			//Log::info() << "expect discover_primary_service(" << connection_handle << ", " << next_attribute_handle << ")" <<std::endl;
			// set expectation on the discovery function
			EXPECT_CALL(
				mock_client, discover_primary_service(connection_handle, next_attribute_handle)
			).WillOnce(
				reply_read_by_group_type(service_transactions)
			);
			if (!has_characteristic_cb) {
				set_services_callback_expectations(service_transactions);
			}

			next_attribute_handle = service_transactions.back().end + 1;
		}

		// if counter didn't wrap then send an error
		if (next_attribute_handle != 0) {
			// set expectation on the discovery function
			//Log::info() << "expect discover_primary_service(" << connection_handle << ", " << next_attribute_handle << ") "
			//<< "to fail with opcode = " <<  AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST << " and error = " << AttErrorResponse::ATTRIBUTE_NOT_FOUND << std::endl;
			EXPECT_CALL(
				mock_client, discover_primary_service(connection_handle, next_attribute_handle)
			).WillOnce(
				reply_error(AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST, AttErrorResponse::ATTRIBUTE_NOT_FOUND)
			);
		}
	}

	/*
	 * Set client expectations when the discover services by UUID is used.
	 */
	void set_discover_services_by_uuid_expectations() {
		std::vector<std::vector<uint8_t>> services_response;
		auto services_transactions = get_services_transactions();

		uint16_t next_attribute_handle = 0x0001;
		for (const auto& transaction : services_transactions) {

			//Log::info() << "expect discover_primary_service_by_service_uuid(" << connection_handle << ", " << next_attribute_handle << "," << service_filter << ")" <<std::endl;
			// set expectation on the discovery function
			EXPECT_CALL(
				mock_client, discover_primary_service_by_service_uuid(
					connection_handle,
					next_attribute_handle,
					service_filter
				)
			).WillOnce(
				reply_find_by_type_value(transaction)
			);

			if (!has_characteristic_cb) {
				set_services_callback_expectations(transaction);
			}
			next_attribute_handle = transaction.back().end + 1;
		}

		// note if last service end handle was 0xFFFF do nothing
		// otherwise set a discovery expectation and reply with an error
		// if counter didn't wrap then send an error
		if (next_attribute_handle != 0) {
			// set expectation on the discovery function
			//Log::info() << "expect discover_primary_service_by_service_uuid(" << connection_handle << ", " << next_attribute_handle << "," << service_filter << ") "
			//<< "to fail with opcode = " <<  AttributeOpcode::FIND_BY_TYPE_VALUE_REQUEST << " and error = " << AttErrorResponse::ATTRIBUTE_NOT_FOUND << std::endl;

			EXPECT_CALL(
				mock_client, discover_primary_service_by_service_uuid(
					connection_handle, next_attribute_handle, service_filter
				)
			).WillOnce(
				reply_error(AttributeOpcode::FIND_BY_TYPE_VALUE_REQUEST, AttErrorResponse::ATTRIBUTE_NOT_FOUND)
			);
		}
	}

	/*
	 * Set an expectation regarding call of the service discovery callback with
	 * the services @p services.
	 */
	void set_services_callback_expectations(const std::vector<service_description_t>& services) {
		if (!has_service_cb) {
			return;
		}

		for (const auto& service : services) {
			set_service_callback_expectation(service);
		}
	}

	/*
	 * Set an expectation regarding call of the service discovery callback with
	 * the service @p service.
	 */
	void set_service_callback_expectation(const service_description_t& service) {
		if (!has_service_cb) {
			return;
		}

		//Log::info() << "expect service_callback(" << service.start << ", " << service.end << "," << service.uuid << ")" <<std::endl;

		EXPECT_CALL(
			service_callback, call(AllOf(
				Property(
					&DiscoveredService::getStartHandle,
					service.start
				),
				Property(
					&DiscoveredService::getEndHandle,
					service.end
				),
				Property(
					&DiscoveredService::getUUID,
					service.uuid
				)
			))
		).WillOnce(Invoke([](const DiscoveredService* service) -> void {
			//Log::info() << "service_callback(" << service->getStartHandle() << ", " << service->getEndHandle() << "," << service->getUUID() << ")" <<std::endl;
		}));
	}

	/*
	 * Set expectations for characteristic discovery.
	 */
	void set_discover_characteristics_expectations() {
		auto services = get_services_discovered();

		for (const auto& service : services) {
			set_service_callback_expectation(service);
			auto characteristic_transactions = get_characteristics_transactions(service);

			uint16_t next_attribute_handle = service.start;
			characteristic_description_t previous_characteristic { };
			for (const auto& characteristic_transaction : characteristic_transactions) {
				//Log::info() << "expect discover_characteristics_of_a_service(" << connection_handle << ", " << next_attribute_handle << "," << service.end << ")" <<std::endl;
				EXPECT_CALL(
					mock_client, discover_characteristics_of_a_service(
						connection_handle,
						attribute_handle_range_t { next_attribute_handle, service.end }
					)
				).WillOnce(
					reply_read_by_type(characteristic_transaction)
				);

				set_characteristics_callbacks_expectations(
					previous_characteristic,
					characteristic_transaction
				);
				previous_characteristic = characteristic_transaction.back();
				next_attribute_handle = characteristic_transaction.back().value_handle + 1;
			}

			if(next_attribute_handle < service.end) {
				//Log::info() << "expect discover_characteristics_of_a_service(" << connection_handle << ", " << next_attribute_handle << "," << service.end << ") "
//				<< "to fail with opcode = " <<  AttributeOpcode::READ_BY_TYPE_REQUEST << " and error = " << AttErrorResponse::ATTRIBUTE_NOT_FOUND << std::endl;
				EXPECT_CALL(
					mock_client, discover_characteristics_of_a_service(
						connection_handle,
						attribute_handle_range_t { next_attribute_handle, service.end }
					)
				).WillOnce(
					reply_error(AttributeOpcode::READ_BY_TYPE_REQUEST, AttErrorResponse::ATTRIBUTE_NOT_FOUND)
				);

				set_characteristic_callback_expectation(previous_characteristic);
			}
		}

	}

	void set_characteristics_callbacks_expectations(characteristic_description_t previous_characteristic, std::vector<characteristic_description_t> transaction) {
		if (transaction.empty() == false) {
			//Log::info() << "pop last characteristic in transaction" <<std::endl;
			transaction.pop_back();
		}

		if (previous_characteristic != characteristic_description_t()) {
			//Log::info() << "add previous characteristic" <<std::endl;
			transaction.insert(transaction.begin(), previous_characteristic);
		}

		for (const auto& characteristic : transaction) {
			set_characteristic_callback_expectation(characteristic);
		}
	}

	void set_characteristic_callback_expectation(characteristic_description_t characteristic) {
		if (!has_characteristic_cb) {
			return;
		}

		if (characteristic_filter != UUID() && characteristic_filter != characteristic.uuid) {
			return;
		}

		//Log::info() << "expect characteristic_callback(" << characteristic.uuid << ", " << characteristic.handle << "," << characteristic.value_handle << ")" <<std::endl;
		EXPECT_CALL(
			characteristic_callback,
			call(AllOf(
				Property(
					&DiscoveredCharacteristic::getUUID,
					characteristic.uuid
				),
				Property(
					&DiscoveredCharacteristic::getDeclHandle,
					characteristic.handle
				),
				Property(
					&DiscoveredCharacteristic::getValueHandle,
					characteristic.value_handle
				),
				Property(
					&DiscoveredCharacteristic::getConnectionHandle,
					connection_handle
				)
			))
		).WillOnce(Invoke([](const DiscoveredCharacteristic* characteristic) -> void {
			//Log::info() << "characteristic_callback(" << characteristic->getDeclHandle() << ", " << characteristic->getValueHandle() << "," << characteristic->getUUID() << ")" <<std::endl;
		}));
	}

	/*
	 * Compute the transactions involved during the service discovery process.
	 */
	std::vector<std::vector<service_description_t>> get_services_transactions() {
		std::vector<service_description_t> working_set = get_services_discovered();

		std::vector<std::vector<service_description_t>> result;
		for (const auto& service : working_set) {
			if(result.empty()) {
				result.push_back(std::vector<service_description_t>());
			}

			auto& last_group = result.back();
			if (last_group.empty()) {
				last_group.push_back(service);
			} else {
				auto last_group_uuid = last_group.front().uuid.shortOrLong();
				auto last_group_size = last_group.size();
				if (last_group_uuid != service.uuid.shortOrLong()) {
					result.push_back(std::vector<service_description_t> { service });
				} else {
					if (last_group_uuid == UUID::UUID_TYPE_SHORT && last_group_size < 4) {
						last_group.push_back(service);
					} else {
						result.push_back(std::vector<service_description_t> { service });
					}
				}
			}
		}
		return result;
	}

	/*
	 * Compute the list of services discovered during service discovery.
	 */
	std::vector<service_description_t> get_services_discovered() {
		std::vector<service_description_t> working_set;

		if (service_filter != UUID()) {
			for (const auto& service : server_stub.services) {
				if (service.uuid == service_filter) {
					working_set.push_back(service);
				}
			}
		} else {
			working_set = server_stub.services;
		}

		return working_set;
	}

	/*
	 * Compute the transactions present during the characteristic discovery process.
	 */
	std::vector<std::vector<characteristic_description_t>> get_characteristics_transactions(const service_description_t& service) {
		std::vector<std::vector<characteristic_description_t>> transactions;
		for (const auto& characteristic : service.characteristics) {
			if(transactions.empty()) {
				transactions.push_back(
					std::vector<characteristic_description_t> { characteristic }
				);
			} else {
				auto& last_group = transactions.back();
				auto last_group_uuid = last_group.front().uuid.shortOrLong();
				auto last_group_size = last_group.size();
				if (last_group_uuid != service.uuid.shortOrLong()) {
					transactions.push_back(std::vector<characteristic_description_t> { characteristic });
				} else {
					if (last_group_uuid == UUID::UUID_TYPE_SHORT && last_group_size < 3) {
						last_group.push_back(characteristic);
					} else {
						transactions.push_back(std::vector<characteristic_description_t> { characteristic });
					}
				}
			}
		}
		return transactions;
	}

	MockPalGattClient mock_client;
	GenericGattClient gatt_client;
	const Gap::Handle_t connection_handle;
	termination_callback_t termination_callback;
	bool has_service_cb;
	service_callback_t service_callback;
	bool has_characteristic_cb;
	characteristic_callback_t characteristic_callback;
	UUID service_filter;
	UUID characteristic_filter;
	server_description_t server_stub;
};

TEST_P(LaunchDiscoveryNoServiceFilter, regular) {
	/////////////////////////////
	// Install default calls
	ON_CALL(
		mock_client, discover_primary_service(_, _)
	).WillByDefault(Invoke([](connection_handle_t connection, attribute_handle_t handle) -> ble_error_t {
		//Log::info() << "default discover primary service (" << connection << "," << handle << ")" << std::endl;
		return BLE_ERROR_NONE;
	}));

	ON_CALL(
		mock_client, discover_primary_service_by_service_uuid(_, _, _)
	).WillByDefault(Invoke([](connection_handle_t connection, attribute_handle_t handle, const UUID& uuid) -> ble_error_t {
		//Log::info() << "default discover primary service by service uuid(" << connection << "," << handle << ", " << uuid << ")" << std::endl;
		return BLE_ERROR_NONE;
	}));

	ON_CALL(
		mock_client, discover_characteristics_of_a_service(_, _)
	).WillByDefault(Invoke([](connection_handle_t connection, attribute_handle_range_t handle) -> ble_error_t {
		//Log::info() << "default discover characteristic of a service(" << connection << "," << handle << ")" << std::endl;
		return BLE_ERROR_NONE;
	}));

	if (has_service_cb) {
		ON_CALL(
			service_callback, call(_)
		).WillByDefault(Invoke([](const DiscoveredService* service) -> void {
			//Log::info() << "default service_callback(" << service->getEndHandle() << ", " << service->getEndHandle() << ", " << service->getUUID() << ")" << std::endl;
		}));
	}

	if (has_characteristic_cb) {
		ON_CALL(
			characteristic_callback, call(_)
		).WillByDefault(Invoke([](const DiscoveredCharacteristic* characteristic) -> void {
			//Log::info() << "default characteristic_callback(" << characteristic->getDeclHandle() << ", " << characteristic->getValueHandle() << ", " << characteristic->getUUID() << ")" << std::endl;
		}));
	}

	/////////////////////////////
	// Install expectations
	{
		InSequence seq;
		if (service_filter == UUID()) {
			set_discover_all_services_expectations();
		} else {
			set_discover_services_by_uuid_expectations();
		}

		if (has_characteristic_cb) {
			set_discover_characteristics_expectations();
		}

		EXPECT_CALL(termination_callback, call(connection_handle));
	}

	/////////////////////////////
	// Launch the discovery process.
	ble_error_t err = gatt_client.launchServiceDiscovery(
		connection_handle,
		has_service_cb ?
			makeFunctionPointer(
				&service_callback, &decltype(service_callback)::call
			) : NULL,
		has_characteristic_cb ?
			makeFunctionPointer(
				&characteristic_callback, &decltype(characteristic_callback)::call
			) : NULL,
		service_filter,
		characteristic_filter
	);

	EXPECT_EQ(BLE_ERROR_NONE, err);
}

INSTANTIATE_TEST_CASE_P(
	GattClient_launch_discovery_no_service_filter,
	LaunchDiscoveryNoServiceFilter,
	// Yield all combination of each generator
	::testing::Combine(
		// service cb generator
		::testing::Values(
			std::tuple<bool, bool>(true, false),
			std::tuple<bool, bool>(false, true),
			std::tuple<bool, bool>(true, true)
		),
		// service UUID filter generator
		::testing::Values(UUID(), UUID(0x1452), UUID("a3d1495f-dba7-4441-99f2-d0a20f663422")),
		// characteristic UUID filter generator
		::testing::Values(UUID(), UUID(0xBEEF), UUID("1f551ee3-aef4-4719-8c52-8b419fc4ac01")),
		// server layout generator.
		::testing::Values(
			server_description_t { },
			server_description_t {
				{
					0x0001,
					0x0030,
					UUID(0xAAAA),
					{
						{ 0x0002, { 0 }, UUID(0xBBBA) },
						{ 0x0006, { 1, 0 }, UUID(0xBBBB) },
						{ 0x0012, { 1, 0 }, UUID(0xBBBC) },
						{ 0x0015, { 1, 1, 1, 0 }, UUID(0xBBBD) }
					},
				},
				{
					0x0031,
					0x0060,
					UUID(0xAAAB),
					{
						{ 0x0032, { 0 }, UUID(0xCBBA) },
						{ 0x0036, { 1, 0 }, UUID(0xCBBB) },
						{ 0x0042, { 1, 0 }, UUID(0xCBBC) },
						{ 0x0050, { 1, 1, 1, 0 }, UUID(0xCBBD) }
					},
				},
				{
					0x0061,
					0x0090,
					UUID(0xAAAB),
					{
						{ 0x0062, { 0 }, UUID(0xDBBA) },
						{ 0x0066, { 1, 0 }, UUID(0xDBBB) },
						{ 0x0072, { 1, 0 }, UUID(0xDBBC) },
						{ 0x0082, { 1, 1, 1, 0 }, UUID(0xDBBD) }
					}
				}
			},
			server_description_t {
				{
					0x0001,
					0x0030,
					UUID(0xAAAA),
					{
						{ 0x0002, { 0 }, UUID(0xBBBA) },
						{ 0x0006, { 1, 0 }, UUID(0xBBBB) },
						{ 0x0012, { 1, 0 }, UUID(0xBBBC) },
						{ 0x0024, { 1, 1, 1, 0 }, UUID(0xBBBC) }
					},
				},
				{
					0x0031,
					0x0060,
					UUID(0xAAAB),
					{
						{ 0x0032, { 0 }, UUID(0xCBBA) },
						{ 0x0036, { 1, 0 }, UUID(0xCBBB) },
						{ 0x0042, { 1, 0 }, UUID(0xCBBC) },
						{ 0x0045, { 1, 1, 1, 0 }, UUID(0xCBBC) }
					},
				},
				{
					0x0061,
					0xFFFF,
					UUID(0xAAAB),
					{
						{ 0x0062, { 0 }, UUID(0xDBBA) },
						{ 0x0066, { 1, 0 }, UUID(0xDBBB) },
						{ 0x0072, { 1, 0 }, UUID(0xDBBC) },
						{ 0x0082, { 1, 1, 1, 0 }, UUID(0xDBBC) }
					}
				}
			}
		)
	)
);


