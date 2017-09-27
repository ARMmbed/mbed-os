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

#include "CordioHCITransportDriver.h"
#include "CordioHCIDriver.h"

extern "C" void hciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len);

namespace ble {
namespace vendor {
namespace cordio {

void CordioHCITransportDriver::on_data_received(uint8_t* data, uint16_t len)
{
    hciTrSerialRxIncoming(data, len);
}

} // namespace cordio
} // namespace vendor
} // namespace ble
