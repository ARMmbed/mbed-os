/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include "drivers/RawCan.h"

#if DEVICE_CAN

#include "platform/mbed_power_mgmt.h"

namespace mbed {
    RawCAN::RawCAN(PinName rd, PinName td): CAN(rd, td) {}

    RawCAN::RawCAN(PinName rd, PinName td, int hz): CAN(rd, td, hz) {}

    /* There are situations where the RX interrupt of CAN are cleared only by 
     * CAN read operations and locks are not allowed in ISR context in mbed
     * hence this provides an unlocked read to resolve such problem without 
     * any effect on the performance. This will work only in case of a single 
     * thread accessing a CAN instance, multiple threads will lead to race conditions 
    */
    int RawCAN::read(CANMessage &msg, int handle){
        int ret = can_read(&_can, &msg, handle);
        return ret;
    }

} // namespace
#endif //DEVICE_CAN