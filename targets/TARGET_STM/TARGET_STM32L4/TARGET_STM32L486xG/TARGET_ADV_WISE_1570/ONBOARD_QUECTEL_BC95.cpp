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

#include "ONBOARD_QUECTEL_BC95.h"

#include "drivers/BufferedSerial.h"
#include "CellularLog.h"

using namespace mbed;

ONBOARD_QUECTEL_BC95::ONBOARD_QUECTEL_BC95(FileHandle *fh) : QUECTEL_BC95(fh)
{
}

nsapi_error_t ONBOARD_QUECTEL_BC95::hard_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BC95::hard_power_off()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BC95::soft_power_on()
{
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_BC95::soft_power_off()
{
    return NSAPI_ERROR_OK;
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static BufferedSerial serial(MDMTXD, MDMRXD, 9600);
#if DEVICE_SERIAL_FC
    if (MDMRTS != NC && MDMCTS != NC) {
        tr_debug("Modem flow control: RTS %d CTS %d", MDMRTS, MDMCTS);
        serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
    }
#endif
    static ONBOARD_QUECTEL_BC95 device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
