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

#include "ble/Gap.h"

ble_error_t Gap::getRandomAddressType(
    BLEProtocol::AddressBytes_t address,
    RandomAddressType_t* type
) {
    // see section Device address in Bluetooth Link Layer specification
    // (Vol 6 - Part B)
    switch (address[5] >> 6) {
        case 0x03:
            *type = RandomAddressType_t::STATIC;
            return BLE_ERROR_NONE;
        case 0x00:
            *type = RandomAddressType_t::NON_RESOLVABLE_PRIVATE;
            return BLE_ERROR_NONE;
        case 0x02:
            *type = RandomAddressType_t::RESOLVABLE_PRIVATE;
            return BLE_ERROR_NONE;
        default:
            return BLE_ERROR_INVALID_PARAM;
    }
}
