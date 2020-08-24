/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include "source/PalGenericAccessServiceImpl.h"
#include "source/GattServerImpl.h"

namespace ble {
namespace impl {

#if 0 // Disabled until reworked and reintroduced to GattServer API

virtual ble_error_t PalGenericAccessService::get_device_name_length(uint8_t& length)
{
#if BLE_FEATURE_GATT_SERVER
        const uint8_t* name = nullptr;
        uint16_t actual_length = 0;

        gatt_server().getDeviceName(name, actual_length);
        length = actual_length;

        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif
    }

    virtual ble_error_t PalGenericAccessService::get_device_name(Span<uint8_t>& array) {
#if BLE_FEATURE_GATT_SERVER
        const uint8_t* name = nullptr;
        uint16_t length = 0;

        gatt_server().getDeviceName(name, length);

        if (length > array.size()) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }

        memcpy(array.data(), name, length);

        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

    virtual ble_error_t PalGenericAccessService::set_device_name(const uint8_t* device_name) {
#if BLE_FEATURE_GATT_SERVER
        return gatt_server().setDeviceName(device_name);
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

    virtual ble_error_t PalGenericAccessService::get_appearance(
        GapAdvertisingData::Appearance& appearance
    ) {
#if BLE_FEATURE_GATT_SERVER
        appearance = gatt_server().getAppearance();
        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

    virtual ble_error_t PalGenericAccessService::set_appearance(
        GapAdvertisingData::Appearance appearance
    ) {
#if BLE_FEATURE_GATT_SERVER
        gatt_server().setAppearance(appearance);
        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

#endif // Disabled until reworked and reintroduced to GattServer API


ble_error_t PalGenericAccessService::get_peripheral_preferred_connection_parameters(
    ble::Gap::PreferredConnectionParams_t &parameters
)
{
#if BLE_FEATURE_GATT_SERVER
    parameters = gatt_server().getPreferredConnectionParams();
    return BLE_ERROR_NONE;
#else
    return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
}

ble_error_t PalGenericAccessService::set_peripheral_preferred_connection_parameters(
    const ble::Gap::PreferredConnectionParams_t &parameters
)
{
#if BLE_FEATURE_GATT_SERVER
    gatt_server().setPreferredConnectionParams(parameters);
    return BLE_ERROR_NONE;
#else
    return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
}


#if BLE_FEATURE_GATT_SERVER

ble::impl::GattServer &PalGenericAccessService::gatt_server()
{
    return ble::impl::GattServer::getInstance();
}

#endif // BLE_FEATURE_GATT_SERVER

} // namespace impl
} // namespace ble


