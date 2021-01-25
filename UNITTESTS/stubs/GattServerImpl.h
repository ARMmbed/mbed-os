/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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

#ifndef BLE_GATTSERVERSTUB_H
#define BLE_GATTSERVERSTUB_H

#include "gmock/gmock.h"
#include "ble/GattServer.h"

namespace ble {
namespace impl {

class GattServer {
public:
    GattServer() {};
    GattServer(const GattServer&) = delete;
    GattServer& operator=(const GattServer&) = delete;
    virtual ~GattServer() {};

    virtual ble_error_t addService(GattService &service) { return ble_error_t::BLE_ERROR_NONE; };
};

}
}

#endif //BLE_GATTSERVERSTUB_H
