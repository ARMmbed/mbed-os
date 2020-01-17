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

#include "ONBOARD_UBLOX_PPP.h"

#include "ublox_low_level_api.h"
#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "CellularLog.h"

using namespace mbed;

ONBOARD_UBLOX_PPP::ONBOARD_UBLOX_PPP(FileHandle *fh) : UBLOX_PPP(fh)
{
}

nsapi_error_t ONBOARD_UBLOX_PPP::hard_power_on()
{
    //currently USB is not supported, so pass 0 to disable USB
    //This call does everything except actually pressing the power button
    ublox_mdm_powerOn(0);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_PPP::hard_power_off()
{
    ublox_mdm_powerOff();

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_PPP::soft_power_on()
{
    /* keep the power line low for 150 milisecond */
    press_power_button(150);
    /* give modem a little time to respond */
    thread_sleep_for(100);

    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_PPP::soft_power_off()
{
    /* keep the power line low for 1 second */
    press_power_button(1000);

    return NSAPI_ERROR_OK;
}

void ONBOARD_UBLOX_PPP::press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, MDMPWRON, 0);
    thread_sleep_for(time_ms);
    gpio_write(&gpio, 1);
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static BufferedSerial serial(MDMTXD, MDMRXD, 115200);
#if DEVICE_SERIAL_FC
    if (MDMRTS != NC && MDMCTS != NC) {
        tr_debug("Modem flow control: RTS %d CTS %d", MDMRTS, MDMCTS);
        serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
    }
#endif
    static ONBOARD_UBLOX_PPP device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
