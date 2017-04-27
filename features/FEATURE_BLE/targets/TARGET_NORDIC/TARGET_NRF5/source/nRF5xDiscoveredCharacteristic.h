/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef __NRF_DISCOVERED_CHARACTERISTIC_H__
#define __NRF_DISCOVERED_CHARACTERISTIC_H__

#include "ble/DiscoveredCharacteristic.h"
#include "ble_gatt.h"

class nRF5xGattClient; /* forward declaration */

class nRF5xDiscoveredCharacteristic : public DiscoveredCharacteristic {
public:
    void setup(nRF5xGattClient         *gattcIn,
               Gap::Handle_t            connectionHandleIn,
               ble_gatt_char_props_t    propsIn,
               GattAttribute::Handle_t  declHandleIn,
               GattAttribute::Handle_t  valueHandleIn);

    void setup(nRF5xGattClient         *gattcIn,
               Gap::Handle_t            connectionHandleIn,
               UUID::ShortUUIDBytes_t   uuidIn,
               ble_gatt_char_props_t    propsIn,
               GattAttribute::Handle_t  declHandleIn,
               GattAttribute::Handle_t  valueHandleIn);

    void setLastHandle(GattAttribute::Handle_t last) {
      lastHandle = last;
    }
};

#endif /* __NRF_DISCOVERED_CHARACTERISTIC_H__ */
