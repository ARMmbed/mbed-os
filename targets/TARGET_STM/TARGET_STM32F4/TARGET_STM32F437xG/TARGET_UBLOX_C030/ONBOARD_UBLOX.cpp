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
#include "ONBOARD_UBLOX_N2XX.h"
#include "drivers/BufferedSerial.h"

using namespace mbed;

CellularDevice *CellularDevice::get_target_default_instance()
{
#if defined(TARGET_UBLOX_C030_R41XM)
#if (NSAPI_PPP_AVAILABLE)
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_PPP device(&serial);
#else
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_AT device(&serial);
#endif
#elif defined(TARGET_UBLOX_C030_N211)
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_N2XX device(&serial);
#elif defined(TARGET_UBLOX_C030_U201)
#if (NSAPI_PPP_AVAILABLE)
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_PPP device(&serial);
#else
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_AT device(&serial);
#endif
#else //UBLOX_C027
    static BufferedSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
    static ONBOARD_UBLOX_PPP device(&serial);
#endif
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
