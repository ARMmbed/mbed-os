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

#include <algorithm>
#include <limits>

#include "CordioHCITransportDriver.h"
#include "CordioHCIDriver.h"

extern "C" void hciTrSerialRxIncoming(uint8_t *pBuf, uint8_t len);

namespace ble {
namespace vendor {
namespace cordio {

CordioHCITransportDriver::data_received_handler_t
    CordioHCITransportDriver::data_received_handler = hciTrSerialRxIncoming;

void CordioHCITransportDriver::on_data_received(uint8_t* data, uint16_t len)
{
    while (len) {
        uint8_t chunk_length = std::min(len, (uint16_t) std::numeric_limits<uint8_t>::max());
        data_received_handler(data, chunk_length);
        len = len - chunk_length;
        data = data + chunk_length;
    }
}

void CordioHCITransportDriver::set_data_received_handler(data_received_handler_t handler)
{
    data_received_handler = handler;
}

} // namespace cordio
} // namespace vendor
} // namespace ble
