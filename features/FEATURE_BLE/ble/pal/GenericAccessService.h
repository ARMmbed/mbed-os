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

#ifndef BLE_PAL_GENERIC_ACCESS_SERVICE_H_
#define BLE_PAL_GENERIC_ACCESS_SERVICE_H_

#include "GapTypes.h"
#include "ble/BLETypes.h"
#include "ble/blecommon.h"
#include "ble/GapAdvertisingData.h"
#include "ble/Gap.h"

namespace ble {
namespace pal {

/**
 * Manage state of the GAP service exposed by the GATT server.
 *
 * @see Bluetooth 4.2 Vol 3 PartC: 12 - GAP service and characteristics for GATT
 * server.
 */
struct GenericAccessService {

    /**
     * Empty, default, constructor
     */
    GenericAccessService() { }

    /**
     * Virtual destructor
     */
    virtual ~GenericAccessService() { }

    /**
     * Acquire the length of the device name.
     * The length can range from 0 (no device name) to 248 octets
     *
     * @param length The length of the device name currently stored in the GAP
     * service.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.1 - Device Name Characteristic
     */
    virtual ble_error_t get_device_name_length(uint8_t& length) = 0;

    /**
     * Get the current device name.
     * The result is stored in the array pass in input if the operation
     * succeed. Prior to this call the length of the device name can be acquired
     * with a call to get_device_name_length.
     *
     * @param The array which will host the device name
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.1 - Device Name Characteristic
     */
    virtual ble_error_t get_device_name(Span<uint8_t>& array) = 0;

    /**
     * Set the value of the device name characteristic exposed by the GAP GATT
     * service.
     *
     * @param device_name The name of the device. If NULL the device name
     * value has a length of 0.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.1 - Device Name Characteristic
     */
    virtual ble_error_t set_device_name(const uint8_t* device_name) = 0;

    /**
     * Acquire the appearance stored in the appearance characteristic of the GAP
     * GATT service.
     *
     * @param appearance: If the call succeed will contain the value of the
     * appearance characteristic.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.2 - Appearance Characteristic
     */
    virtual ble_error_t get_appearance(
        GapAdvertisingData::Appearance& appearance
    ) = 0;

    /**
     * Set the value of the appearance characteristic of the GAP GATT service.
     *
     * @param appearance: The appearance to set.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.2 - Appearance Characteristic
     */
     virtual ble_error_t set_appearance(
         GapAdvertisingData::Appearance appearance
    ) = 0;

    /**
     * Acquire the peripheral prefered connection parameters stored in the GAP
     * GATT service.
     *
     * @param parameters: If the call succeed will contain the value of
     * the peripheral prefered connection parameters characteristic.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.3 - Peripheral Preferred Connection
     * Parameters Characteristic
     */
     virtual ble_error_t get_peripheral_prefered_connection_parameters(
         ::Gap::ConnectionParams_t& parameters
    ) = 0;

    /**
     * set the value of the peripheral prefered connection parameters stored in
     * the GAP GATT service.
     *
     * @param parameters: If the peripheral prefered connection parameters
     * to set.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.3 - Peripheral Preferred Connection
     * Parameters Characteristic
     */
    virtual ble_error_t set_peripheral_prefered_connection_parameters(
        const ::Gap::ConnectionParams_t& parameters
   ) = 0;

private:
    GenericAccessService(const GenericAccessService&);
    GenericAccessService& operator=(const GenericAccessService&);
};

} // namespace pal
} // namespace ble

#endif /* BLE_PAL_GENERIC_ACCESS_SERVICE_H_ */
