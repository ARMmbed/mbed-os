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

#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "ONBOARD_TELIT_HE910.h"
#include "ThisThread.h"
#include "CellularLog.h"

using namespace mbed;

ONBOARD_TELIT_HE910::ONBOARD_TELIT_HE910(FileHandle *fh) : TELIT_HE910(fh)
{
}

nsapi_error_t ONBOARD_TELIT_HE910::hard_power_on()
{
    //does nothing at the moment, TODO: MultiTech to add hardware initialization stuff if needed
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::hard_power_off()
{
    //does nothing at the moment, TODO: MultiTech to add hardware de-initialization stuff if needed
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::soft_power_on()
{
    /* keep the power line low for 200 milisecond */
    press_power_button(200);
    /* give modem a little time to respond */
    thread_sleep_for(100);
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::soft_power_off()
{
    gpio_t gpio;
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
    /* keep the power line low for more than 10 seconds.
     * If 3G_ON_OFF pin is kept low for more than a second, a controlled disconnect and shutdown takes
     * place, Due to the network disconnect, shut-off can take up to 30 seconds. However, we wait for 10
     * seconds only   */
    thread_sleep_for(10 * 1000);
    return NSAPI_ERROR_OK;
}

void ONBOARD_TELIT_HE910::press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, MDMPWRON, 1);
    gpio_write(&gpio, 0);
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
    static ONBOARD_TELIT_HE910 device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
