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

#ifndef BLE_PAL_GENERIC_ACCESS_SERVICE_H_
#define BLE_PAL_GENERIC_ACCESS_SERVICE_H_

#include "GapTypes.h"
#include "ble/common/BLETypes.h"
#include "ble/common/blecommon.h"
#include "ble/Gap.h"

namespace ble {

/**
 * Manage state of the GAP service exposed by the GATT server.
 *
 * @see Bluetooth 4.2 Vol 3 PartC: 12 - GAP service and characteristics for GATT
 * server.
 */
struct PalGenericAccessService {
public:
    /**
     * Acquire the peripheral preferred connection parameters stored in the GAP
     * GATT service.
     *
     * @param parameters: If the call succeed will contain the value of
     * the peripheral preferred connection parameters characteristic.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.3 - Peripheral Preferred Connection
     * Parameters Characteristic
     */
     virtual ble_error_t get_peripheral_preferred_connection_parameters(
         ble::Gap::PreferredConnectionParams_t& parameters
     ) = 0;

    /**
     * set the value of the peripheral preferred connection parameters stored in
     * the GAP GATT service.
     *
     * @param parameters: If the peripheral preferred connection parameters
     * to set.
     *
     * @return BLE_ERROR_NONE in case of success or the appropriate error code
     * otherwise.
     *
     * @see Bluetooth 4.2 Vol 3 PartC: 12.3 - Peripheral Preferred Connection
     * Parameters Characteristic
     */
    virtual ble_error_t set_peripheral_preferred_connection_parameters(
        const ble::Gap::PreferredConnectionParams_t& parameters
    ) = 0;
};

} // namespace ble

#endif /* BLE_PAL_GENERIC_ACCESS_SERVICE_H_ */
