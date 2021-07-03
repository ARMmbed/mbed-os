/*
 *  Copyright (C) 2021, STMicroelectronics, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
    using CAN::CAN;


    // override lock apis to create unlocked CAN
    void lock() override {};
    void unlock() override {};

};
}

#endif
#endif //RAWCAN_H
