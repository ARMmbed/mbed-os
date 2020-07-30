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

#ifndef IMPL_PAL_GENERIC_ACCESS_SERVICE_
#define IMPL_PAL_GENERIC_ACCESS_SERVICE_

#include "ble/internal/PalGenericAccessService.h"
#include "ble/GattServer.h"
#include "ble/Gap.h"

namespace ble {

/**
 * Implementation of ble::PalGenericAccessService for the Cordio stack.
 */
class PalGenericAccessService : public interface::PalGenericAccessService {
public:
    PalGenericAccessService() { }

    virtual ~PalGenericAccessService() { }

#if 0 // Disabled until reworked and reintroduced to GattServer API
    virtual ble_error_t get_device_name_length(uint8_t& length);
    virtual ble_error_t get_device_name(Span<uint8_t>& array);
    virtual ble_error_t set_device_name(const uint8_t* device_name);
    virtual ble_error_t get_appearance(GapAdvertisingData::Appearance& appearance);
    virtual ble_error_t set_appearance(GapAdvertisingData::Appearance appearance);
#endif // Disabled until reworked and reintroduced to GattServer API

    virtual ble_error_t get_peripheral_preferred_connection_parameters(
        ble::Gap::PreferredConnectionParams_t& parameters
    );

    virtual ble_error_t set_peripheral_preferred_connection_parameters(
        const ble::Gap::PreferredConnectionParams_t& parameters
    );

private:
#if BLE_FEATURE_GATT_SERVER
    ble::GattServer& gatt_server();
#endif // BLE_FEATURE_GATT_SERVER
};

} // ble

#endif /* IMPL_PAL_GENERIC_ACCESS_SERVICE_ */
