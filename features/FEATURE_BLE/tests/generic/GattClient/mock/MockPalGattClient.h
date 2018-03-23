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

#ifndef TESTS_GENERIC_GATTCLIENT_MOCK_MOCKPALGATTCLIENT_H_
#define TESTS_GENERIC_GATTCLIENT_MOCK_MOCKPALGATTCLIENT_H_

#include "gmock/gmock.h"
#include "ble/pal/PalGattClient.h"

namespace ble {
namespace pal {
namespace vendor {
namespace mock {

/*
 * Mock of ble::pal::GattClient
 */
class MockPalGattClient : public ble::pal::GattClient {
public:
	MockPalGattClient();

	virtual ~MockPalGattClient();

	MOCK_METHOD0(initialize, ble_error_t());

    MOCK_METHOD0(terminate, ble_error_t());

    MOCK_METHOD1(exchange_mtu, ble_error_t(connection_handle_t));

    MOCK_METHOD2(get_mtu_size, ble_error_t(connection_handle_t, uint16_t&));

    MOCK_METHOD2(
    	discover_primary_service,
		ble_error_t(connection_handle_t, attribute_handle_t)
	);

    MOCK_METHOD3(
    	discover_primary_service_by_service_uuid,
		ble_error_t(connection_handle_t, attribute_handle_t, const UUID&)
	);

    MOCK_METHOD2(
    	find_included_service,
		ble_error_t(connection_handle_t, attribute_handle_range_t)
	);

    MOCK_METHOD2(
    	discover_characteristics_of_a_service,
		ble_error_t(connection_handle_t, attribute_handle_range_t)
    );

    MOCK_METHOD2(
    	discover_characteristics_descriptors,
		ble_error_t(connection_handle_t, attribute_handle_range_t)
    );

    MOCK_METHOD2(
    	read_attribute_value,
		ble_error_t(connection_handle_t, attribute_handle_t)
    );

    MOCK_METHOD3(
    	read_using_characteristic_uuid,
		ble_error_t(connection_handle_t, attribute_handle_range_t, const UUID&)
	);

    MOCK_METHOD3(
    	read_attribute_blob,
		ble_error_t(connection_handle_t, attribute_handle_t, uint16_t)
	);

    MOCK_METHOD2(
    	read_multiple_characteristic_values,
		ble_error_t(
			connection_handle_t, const ArrayView<const attribute_handle_t>&
		)
    );

    MOCK_METHOD3(
    	write_without_response,
		ble_error_t(
			connection_handle_t,
			attribute_handle_t,
			const ArrayView<const uint8_t>&
		)
    );

    MOCK_METHOD3(
    	signed_write_without_response,
		ble_error_t(
			connection_handle_t,
			attribute_handle_t,
			const ArrayView<const uint8_t>&
		)
    );

    MOCK_METHOD3(
    	write_attribute,
		ble_error_t(
			connection_handle_t,
			attribute_handle_t,
			const ArrayView<const uint8_t>&
		)
    );

    MOCK_METHOD4(
    	queue_prepare_write,
		ble_error_t(
			connection_handle_t,
			attribute_handle_t,
			const ArrayView<const uint8_t>&,
			uint16_t
		)
    );

    MOCK_METHOD2(execute_write_queue, ble_error_t(connection_handle_t, bool));

    // let event injection functions be public so it can be used inside the tests.
    using pal::GattClient::on_server_event;
    using pal::GattClient::on_transaction_timeout;
};

} // namespace ble
} // namespace pal
} // namespace vendor
} // namespace mock

#endif /* TESTS_GENERIC_GATTCLIENT_MOCK_MOCKPALGATTCLIENT_H_ */
