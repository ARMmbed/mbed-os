/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#if MBED_CONF_NSAPI_PRESENT

#include "gpio_api.h"
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
    power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::hard_power_off()
{
    power_down();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::soft_power_on()
{
    power_down();
    power_up();
    // From Telit_xE910 Global form factor App note: It is mandatory to avoid sending data to the serial ports during the first 200ms of the module start-up.
    rtos::ThisThread::sleep_for(200);
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_HE910::soft_power_off()
{
    power_down();
    return NSAPI_ERROR_OK;
}

void ONBOARD_TELIT_HE910::press_power_button()
{
    gpio_t gpio;
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
}

void ONBOARD_TELIT_HE910::release_power_button()
{
    gpio_t gpio;
    gpio_init_out_ex(&gpio, MDMPWRON, 1);
}

// 1.8v from the radio should be high before exiting this function. Do nothing if it's already high.
// Releasing MDMPWRON (going high) tells the tiny9 to turn the radio back on, assuming 1.8v was low.
void ONBOARD_TELIT_HE910::power_up()
{
    // Set up to read 1.8v from the radio.
    gpio_t radioOn;
    gpio_init_in(&radioOn, PC_5);

    // Do nothing if it's already powered.
    if (gpio_read(&radioOn)) {
        return;
    }
    else {
        // power it up.
        release_power_button();
    }

    // wait a max of 60s for 1.8v to go high.
    volatile int v1_8 = 0;
    uint8_t timeout = 60;
    do {
        rtos::ThisThread::sleep_for(1000);
        v1_8 = gpio_read(&radioOn);
    } while (!v1_8 && timeout--);

    // The radio should be ready for commands after another 2s minimum.
    rtos::ThisThread::sleep_for(3000);
}

void ONBOARD_TELIT_HE910::power_down()
{
    gpio_t radioOn;
    gpio_init_in(&radioOn, PC_5);

    // Do nothing if it's already off.
    if (!gpio_read(&radioOn)) {
        return;
    }
    else {
        // power down.
        press_power_button();
    }
    
    // wait a max of 60s for 1.8v to go low.
    volatile int v1_8 = 1;
    uint8_t timeout = 60;
    do {
        rtos::ThisThread::sleep_for(1000);
        v1_8 = gpio_read(&radioOn);
    } while (v1_8 && timeout--);
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
