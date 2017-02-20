/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef NORDIC_NRFGATTCLIENT_H_
#define NORDIC_NRFGATTCLIENT_H_

#include <stddef.h>
#include "nrf_ble_gattc.h"
#include "nrf_error.h"

namespace ble {
namespace pal {
namespace nordic {

/**
 * Implementation of ble::pal::GattClient for nordic targets.
 */
class NRFGattClient: public GattClient {
public:
    NRFGattClient();
    virtual ~NRFGattClient();

    /**
     * @see ble::pal::GattClient::exchange_mtu
     */
    virtual ble_error_t exchange_mtu(connection_handle_t connection) {
        /* not supported yet */
        return BLE_ERROR_NOT_IMPLEMENTED;
    }

    /**
     * @see ble::pal::GattClient::get_max_rx_mtu_supported
     */
    virtual uint16_t get_max_rx_mtu_supported() {
        // default mtu
        return 23;
    }

    /**
     * @see ble::pal::GattClient::discover_primary_service
     */
    virtual ble_error_t discover_primary_service(
        connection_handle_t connection,
        attribute_handle_t discovery_range_begining
    ) {
        return _error_converter << sd_ble_gattc_primary_services_discover(
            connection,
            discovery_range_begining,
            NULL
        );
    }

    /**
     * @see ble::pal::GattClient::discover_primary_service_by_service_uuid
     */
    virtual ble_error_t discover_primary_service_by_service_uuid(
        connection_handle_t connection_handle,
        attribute_handle_t discovery_range_begining,
        const UUID& uuid
    ) {
        // limitation from the nordic SDK, 128 bits UUID are not supported except if
        // they are registered. This is not applicable to BLE API.
        if (uuid.shortOrLong() == UUID::UUID_TYPE_LONG) {
            return BLE_ERROR_INVALID_PARAM;
        }

        ble_uuid_t nrf_uuid = {
            uuid.getShortUUID(),
            BLE_UUID_TYPE_BLE
        };

        return _error_converter << sd_ble_gattc_primary_services_discover(
            connection_handle,
            discovery_range_begining,
            &nrf_uuid
        );
    }

    /**
     * @see ble::pal::GattClient::find_included_service
     */
    virtual ble_error_t find_included_service(
        connection_handle_t connection_handle,
        attribute_handle_range_t service_range
    ) {
        ble_gattc_handle_range_t range = {
            service_range.begin,
            service_range.end
        };
        return _error_converter << sd_ble_gattc_relationships_discover(
            connection_handle,
            &range
        );
    }

    /**
     * @see ble::pal::GattClient::discover_characteristics_of_a_service
     */
    virtual ble_error_t discover_characteristics_of_a_service(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range
    ) {
        ble_gattc_handle_range_t range = {
            discovery_range.begin,
            discovery_range.end
        };
        return _error_converter << sd_ble_gattc_characteristics_discover(
            connection_handle,
            &range
        );
    }

    /**
     * @see ble::pal::GattClient::discover_characteristics_descriptors
     */
    virtual ble_error_t discover_characteristics_descriptors(
        connection_handle_t connection_handle,
        attribute_handle_range_t descriptors_discovery_range
    ) {
        ble_gattc_handle_range_t range = {
            descriptors_discovery_range.begin,
            descriptors_discovery_range.end
        };
        return _error_converter << sd_ble_gattc_descriptors_discover(
            connection_handle,
            &range
        );
    }

    /**
     * @see ble::pal::GattClient::read_attribute_value
     */
    virtual ble_error_t read_attribute_value(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle
    ) {
        return _error_converter << sd_ble_gattc_read(connection_handle, attribute_handle, 0);
    }

    /**
     * @see ble::pal::GattClient::read_using_characteristic_uuid
     */
    virtual ble_error_t read_using_characteristic_uuid(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& uuid
    ) {
        // limitation from the nordic SDK, 128 bits UUID are not supported except if
        // they are registered. This is not applicable to BLE API.
        if (uuid.shortOrLong() == UUID::UUID_TYPE_LONG) {
            return BLE_ERROR_INVALID_PARAM;
        }

        ble_uuid_t nrf_uuid = {
            uuid.getShortUUID(),
            BLE_UUID_TYPE_BLE
        };

        ble_gattc_handle_range_t range = {
            read_range.begin,
            read_range.end
        };

        return _error_converter << sd_ble_gattc_char_value_by_uuid_read(
            connection_handle,
            &nrf_uuid,
            &range
        );
    }

    /**
     * @see ble::pal::GattClient::read_attribute_blob
     */
    virtual ble_error_t read_attribute_blob(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        uint16_t offset
    ) {
        return _error_converter << sd_ble_gattc_read(
            connection_handle,
            attribute_handle,
            offset
        );
    }

    /**
     * @see ble::pal::GattClient::read_multiple_characteristic_values
     */
    virtual ble_error_t read_multiple_characteristic_values(
        connection_handle_t connection_handle,
        const array_view<const attribute_handle_t>& characteristic_value_handles
    ) {
        return _error_converter << sd_ble_gattc_char_values_read(
            connection_handle,
            characteristic_value_handles.data(),
            characteristic_value_handles.size()
        );
    }

    /**
     * @see ble::pal::GattClient::write_without_response
     */
    virtual ble_error_t write_without_response(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const array_view<const uint8_t>& value
    ) {
        ble_gattc_write_params_t write_params = {
            /* write op */ BLE_GATT_OP_WRITE_CMD,
            /* flags ... NA in this case  */ 0,
            /* handle */ characteristic_value_handle,
            /* offset */ 0,
            /* len */ value.size(),
            /* p_value - Nordic API force the cast ... */ const_cast<uint8_t*>(value.data())
        };

        return _error_converter << sd_ble_gattc_write(
            connection_handle,
            &write_params
        );
    }

    /**
     * @see ble::pal::GattClient::signed_write_without_response
     */
    virtual ble_error_t signed_write_without_response(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const array_view<const uint8_t>& value
    ) {
        // signature is unused ...
        ble_gattc_write_params_t write_params = {
            /* write op */ BLE_GATT_OP_SIGN_WRITE_CMD,
            /* flags ... NA in this case  */ 0,
            /* handle */ characteristic_value_handle,
            /* offset */ 0,
            /* len */ value.size(),
            /* p_value - Nordic API force the cast ... */ const_cast<uint8_t*>(value.data())
        };

        return _error_converter << sd_ble_gattc_write(
            connection_handle,
            &write_params
        );
    }

    /**
     * @see ble::pal::GattClient::write_attribute
     */
    virtual ble_error_t write_attribute(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const array_view<const uint8_t>& value
    )  {
        ble_gattc_write_params_t write_params = {
            /* write op */ BLE_GATT_OP_WRITE_REQ,
            /* flags ... NA in this case  */ 0,
            /* handle */ attribute_handle,
            /* offset */ 0,
            /* len */ value.size(),
            /* p_value - Nordic API force the cast ... */ const_cast<uint8_t*>(value.data())
        };

        return _error_converter << sd_ble_gattc_write(
            connection_handle,
            &write_params
        );
    }

    /**
     * @see ble::pal::GattClient::queue_prepare_write
     */
    virtual ble_error_t queue_prepare_write(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const array_view<const uint8_t>& value,
        uint16_t offset
    ) {
        ble_gattc_write_params_t write_params = {
            /* write op */ BLE_GATT_OP_PREP_WRITE_REQ,
            /* flags ... NA in this case  */ 0,
            /* handle */ characteristic_value_handle,
            /* offset */ offset,
            /* len */ value.size(),
            /* p_value - Nordic API force the cast ... */ const_cast<uint8_t*>(value.data())
        };

        return _error_converter << sd_ble_gattc_write(
            connection_handle,
            &write_params
        );
    }

    /**
     * @see ble::pal::GattClient::execute_write_queue
     */
    virtual ble_error_t execute_write_queue(
        connection_handle_t connection_handle,
        bool execute
    ) {
        ble_gattc_write_params_t write_params = {
            /* write op */ BLE_GATT_OP_EXEC_WRITE_REQ,
            /* flags */ execute ? BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE : BLE_GATT_EXEC_WRITE_FLAG_PREPARED_CANCEL,
            /* handle */ 0x000,
            /* offset */ 0,
            /* len */ 0,
            /* p_value - Nordic API force the cast ... */ NULL
        };

        return _error_converter << sd_ble_gattc_write(
            connection_handle,
            &write_params
        );
    }

    /**
     * @see ble::pal::GattClient::initialize
     */
    virtual ble_error_t initialize() {
        // TODO
        return BLE_ERROR_NONE;
    }

    /**
     * @see ble::pal::GattClient::terminate
     */
    virtual ble_error_t terminate() {
        // TODO
        return BLE_ERROR_NONE;
    }

private:

    /**
     * Placeholder used to convert Nordic error codes into ble_error_t.
     * The operator << of this instance is overloaded and handle the conversion.
     * It become possible to write code like:
     * @code
     * ble_error_t some_function() {
     *   return _error_converter << sd_something();
     * }
     * @endcode
     */
    static struct {
        ble_error_t operator<<(uint32_t nordic_error_code) {
            switch (nordic_error_code) {
                case NRF_SUCCESS: return BLE_ERROR_NONE;
                // TODO convert other error codes
                default:          return BLE_ERROR_UNSPECIFIED;
            }
        }
    } _error_converter;
};

} /* namespace nordic */
} /* namespace pal */
} /* namespace ble */

#endif /* NORDIC_NRFGATTCLIENT_H_ */
