/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef RAWCAN_H
#define RAWCAN_H

#include "platform/platform.h"
#include "drivers/CAN.h"

#if DEVICE_CAN || defined(DOXYGEN_ONLY)

#include "interfaces/InterfaceCAN.h"
#include "hal/can_api.h"
#include "platform/Callback.h"
#include "platform/PlatformMutex.h"

namespace mbed {
class RawCAN: public CAN {
    public:
    RawCAN(PinName rd, PinName td);

    /** Initialize CAN interface and set the frequency
      *
      * @param rd the read pin
      * @param td the transmit pin
      * @param hz the bus frequency in hertz
      */
    RawCAN(PinName rd, PinName td, int hz);

    /** Initialize CAN interface
      *
      * @param pinmap reference to structure which holds static pinmap
      * @param td the transmit pin
      * @param hz the bus frequency in hertz
      */

    virtual ~RawCAN() {};

    /** Read a CANMessage from the bus.
     *
     *  @param msg A CANMessage to read to.
     *  @param handle message filter handle (0 for any message)
     *
     *  @returns
     *    0 if no message arrived,
     *    1 if message arrived
     */
    int read(CANMessage &msg, int handle = 0);
    
    };
}    //namespace mbed

#endif

#endif //RAWCAN_H