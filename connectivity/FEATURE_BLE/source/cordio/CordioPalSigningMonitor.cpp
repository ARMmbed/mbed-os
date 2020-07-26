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

#include "ble/internal/cordio/CordioPalSigningMonitor.h"
#include "ble/internal/cordio/CordioBLEInstanceBase.h"
#include "ble/internal/cordio/CordioGattClient.h"

namespace ble {

void PalSigningMonitor::set_signing_event_handler(SecurityManager *handler) {
#if BLE_FEATURE_GATT_CLIENT
    CordioBLEInstanceBase::deviceInstance().getGattClient().set_signing_event_handler(handler);
#endif // BLE_FEATURE_GATT_CLIENT
#if BLE_FEATURE_GATT_SERVER
    CordioBLEInstanceBase::deviceInstance().getGattServer().set_signing_event_handler(handler);
#endif // BLE_FEATURE_GATT_SERVER
}

}
