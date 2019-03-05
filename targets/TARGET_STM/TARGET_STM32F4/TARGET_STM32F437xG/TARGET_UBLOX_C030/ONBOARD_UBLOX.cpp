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

#if MBED_CONF_NSAPI_PRESENT

#include "ONBOARD_UBLOX_AT.h"
#include "ONBOARD_UBLOX_PPP.h"
#include "UARTSerial.h"

using namespace mbed;

CellularDevice *CellularDevice::get_target_default_instance()
{
#if defined(TARGET_UBLOX_C030_N211) || defined(TARGET_UBLOX_C030_R41XM)
    static UARTSerial serial(MDMTXD, MDMRXD, 115200);
    static ONBOARD_UBLOX_AT device(&serial);
#else
    static UARTSerial serial(MDMTXD, MDMRXD, 115200);
    static ONBOARD_UBLOX_PPP device(&serial);
#endif
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
