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

#include "ONBOARD_RM1000_AT.h"
#include "cellular/onboard_modem_api.h"
#include "UARTSerial.h"

#include "mbed-trace/mbed_trace.h"
#ifndef TRACE_GROUP
#define TRACE_GROUP  "RIOTMICRO"
#endif // TRACE_GROUP

using namespace mbed;

ONBOARD_RM1000_AT::ONBOARD_RM1000_AT(FileHandle *fh) : RM1000_AT(fh)
{
}

nsapi_error_t ONBOARD_RM1000_AT::hard_power_on()
{
    tr_debug("Calling ONBOARD_RM1000_AT::hard_power_on");

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_RM1000_AT::hard_power_off()
{
    tr_debug("Calling ONBOARD_RM1000_AT::hard_power_off");

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_RM1000_AT::soft_power_on()
{
    tr_debug("Calling ONBOARD_RM1000_AT::soft_power_on");

    ::onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_RM1000_AT::soft_power_off()
{
    tr_debug("Calling ONBOARD_RM1000_AT::soft_power_off");

    ::onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    tr_debug("Calling CellularDevice::get_target_default_instance from ONBOARD_RM1000_AT");

    static UARTSerial serial(MDM_UART3_TXD, MDM_UART3_RXD, 230400);
#if DEVICE_SERIAL_FC
    if (MDM_UART3_RTS != NC && MDM_UART3_CTS != NC) {
        tr_debug("Modem flow control: RTS %d CTS %d", MDM_UART3_RTS, MDM_UART3_CTS);
        serial.set_flow_control(SerialBase::RTSCTS, MDM_UART3_RTS, MDM_UART3_CTS);
    }
#endif
    static ONBOARD_RM1000_AT device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
