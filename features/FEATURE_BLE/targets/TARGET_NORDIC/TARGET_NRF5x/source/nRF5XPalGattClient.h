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

#ifndef BLE_NORDIC_PAL_GATT_CLIENT_H_
#define BLE_NORDIC_PAL_GATT_CLIENT_H_

#include "ble/pal/PalGattClient.h"
#include "ble/blecommon.h"
#include "ble/UUID.h"

#include "headers/nrf_ble.h"
#include "headers/ble_gatt.h"
#include "headers/ble_types.h"
#include "btle.h"

namespace ble {
namespace pal {
namespace vendor {
namespace nordic {

/**
 * Implementation of pal::GattClient for the Nordic stack.
 */
class nRF5xGattClient : public ble::pal::GattClient {

public:
    nRF5xGattClient();

    virtual ~nRF5xGattClient();

    /**
     * see pal::GattClient::initialize .
     */
    virtual ble_error_t initialize();

    /**
     * see pal::GattClient::terminate .
     */
    virtual ble_error_t terminate();

    /**
     * see pal::GattClient::exchange_mtu .
     */
    virtual ble_error_t exchange_mtu(connection_handle_t connection);

    /**
     * see pal::GattClient::get_mtu_size .
     */
    virtual ble_error_t get_mtu_size(
        connection_handle_t connection_handle, uint16_t& mtu_size
    );

    /**
     * see pal::GattClient::discover_primary_service .
     */
    virtual ble_error_t discover_primary_service(
        connection_handle_t connection,
        attribute_handle_t discovery_range_begining
    );

    /**
     * see pal::GattClient::discover_primary_service_by_service_uuid .
     */
    virtual ble_error_t discover_primary_service_by_service_uuid(
        connection_handle_t connection_handle,
        attribute_handle_t discovery_range_beginning,
        const UUID& uuid
    );

    /**
     * see pal::GattClient::find_included_service .
     */
    virtual ble_error_t find_included_service(
        connection_handle_t connection_handle,
        attribute_handle_range_t service_range
    );

    /**
     * see pal::GattClient::discover_characteristics_of_a_service .
     */
    virtual ble_error_t discover_characteristics_of_a_service(
        connection_handle_t connection_handle,
        attribute_handle_range_t discovery_range
    );

    /**
     * see pal::GattClient::discover_characteristics_descriptors .
     */
    virtual ble_error_t discover_characteristics_descriptors(
        connection_handle_t connection_handle,
        attribute_handle_range_t descriptors_discovery_range
    );

    /**
     * see pal::GattClient::read_attribute_value .
     */
    virtual ble_error_t read_attribute_value(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle
    );

    /**
     * see pal::GattClient::read_using_characteristic_uuid .
     */
    virtual ble_error_t read_using_characteristic_uuid(
        connection_handle_t connection_handle,
        attribute_handle_range_t read_range,
        const UUID& uuid
    );

    /**
     * see pal::GattClient::read_attribute_blob .
     */
    virtual ble_error_t read_attribute_blob(
        connection_handle_t connection,
        attribute_handle_t attribute_handle,
        uint16_t offset
    );

    /**
     * see pal::GattClient::read_multiple_characteristic_values .
     */
    virtual ble_error_t read_multiple_characteristic_values(
        connection_handle_t connection,
        const ArrayView<const attribute_handle_t>& characteristic_handles
    );

    /**
     * see pal::GattClient::write_without_response .
     */
    virtual ble_error_t write_without_response(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const ArrayView<const uint8_t>& value
    );

    /**
     * see pal::GattClient::signed_write_without_response .
     */
    virtual ble_error_t signed_write_without_response(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const ArrayView<const uint8_t>& value
    );

    /**
     * see pal::GattClient::write_attribute .
     */
    virtual ble_error_t write_attribute(
        connection_handle_t connection_handle,
        attribute_handle_t attribute_handle,
        const ArrayView<const uint8_t>& value
    );

    /**
     * see pal::GattClient::queue_prepare_write .
     */
    virtual ble_error_t queue_prepare_write(
        connection_handle_t connection_handle,
        attribute_handle_t characteristic_value_handle,
        const ArrayView<const uint8_t>& value,
        uint16_t offset
    );

    /**
     * see pal::GattClient::execute_write_queue .
     */
    virtual ble_error_t execute_write_queue(
        connection_handle_t connection_handle,
        bool execute
    );

    // singleton of the ARM Cordio client
    static nRF5xGattClient& get_client();

    /**
     * Function call from btle.cpp
     *
     * Do not call directly.
     */
    static void handle_events(const ble_evt_t *p_ble_evt);

    /**
     * Called by btle.cpp when a disconnection happens.
     */
    static void handle_connection_termination(connection_handle_t connection);

private:
    struct GattProcedure;
    struct RegularGattProcedure;
    struct DiscoverPrimaryServiceProcedure;
    struct DiscoverPrimaryServiceByUUIDProcedure;
    struct FindIncludedServicesProcedure;
    struct DiscoverCharacteristicsProcedure;
    struct DiscoverDescriptorsProcedure;
    struct ReadAttributeProcedure;
    struct ReadUsingCharacteristicUUIDProcedure;
    struct ReadAttributeBlobProcedure;
    struct ReadMultipleCharacteristicsProcedure;
    struct WriteAttributeProcedure;
    struct QueuePrepareWriteProcedure;
    struct ExecuteWriteQueueProcedure;

    template<typename ProcType, typename A0>
    ble_error_t launch_procedure(connection_handle_t connection, const A0& a0);

    template<typename ProcType, typename A0, typename A1>
    ble_error_t launch_procedure(
        connection_handle_t connection, const A0& a0, const A1& a1
    );

    template<typename ProcType, typename A0, typename A1, typename A2>
    ble_error_t launch_procedure(
        connection_handle_t connection,
        const A0& a0, const A1& a1, const A2& a2
    );

    template<typename ProcType, typename A0, typename A1, typename A2, typename A3>
    ble_error_t launch_procedure(
        connection_handle_t connection,
        const A0& a0, const A1& a1, const A2& a2, const A3& a3
    );

    GattProcedure* get_procedure(connection_handle_t) const;
    bool register_procedure(GattProcedure*);
    bool remove_procedure(GattProcedure*);

    void handle_procedure_event(const ble_evt_t &evt);
    void handle_hvx_event(const ble_evt_t &evt);
    void handle_timeout_event(const ble_evt_t &evt);

    static const size_t max_procedures_count =
        CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT;

    // Note: Ideally we would have used an array of variant here
    GattProcedure* _procedures[max_procedures_count];
};

} // nordic
} // vendor
} // pal
} // ble

#endif /* BLE_NORDIC_PAL_GATT_CLIENT_H_ */
