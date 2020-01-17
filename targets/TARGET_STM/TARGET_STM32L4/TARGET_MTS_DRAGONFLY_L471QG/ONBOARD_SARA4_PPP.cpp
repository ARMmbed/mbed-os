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

#include "ONBOARD_SARA4_PPP.h"
#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"
#include "drivers/BufferedSerial.h"
#include "CellularLog.h"
#include "mbed_wait_api.h"

using namespace mbed;

ONBOARD_SARA4_PPP::ONBOARD_SARA4_PPP(FileHandle *fh) : SARA4_PPP(fh)
{
}

nsapi_error_t ONBOARD_SARA4_PPP::hard_power_on()
{
    onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_SARA4_PPP::hard_power_off()
{
    onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_SARA4_PPP::soft_power_on()
{
    onboard_modem_power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_SARA4_PPP::soft_power_off()
{
    onboard_modem_power_down();
    return NSAPI_ERROR_OK;
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
    static ONBOARD_SARA4_PPP device(&serial);
    return &device;
}

void ONBOARD_SARA4_PPP::press_power_button(int time_us)
{
    gpio_t gpio;
    gpio_init_out_ex(&gpio, MDMPWRON, 1);
    wait_us(time_us);
    gpio_write(&gpio, 0);
}

void ONBOARD_SARA4_PPP::onboard_modem_init()
{
    gpio_t gpio;

    // Take us out of reset
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull,   1);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull,   0);
    gpio_init_out_ex(&gpio, MDMRST, 0);
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_OUTPUT, OpenDrainNoPull, 0);
}

void ONBOARD_SARA4_PPP::onboard_modem_deinit()
{
    onboard_modem_power_down();
    gpio_t gpio;

    // Back into reset
    gpio_init_out_ex(&gpio, MDMRST, 1);
    gpio_init_out_ex(&gpio, MDMPWRON, 1);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull,   1);
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull,   0);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_OUTPUT, OpenDrainNoPull, 1);
}

void ONBOARD_SARA4_PPP::onboard_modem_power_up()
{
    onboard_modem_init();

    gpio_t gpio;
    gpio_init_in(&gpio, MON_1V8);

    if(gpio_is_connected(&gpio) && !gpio_read(&gpio)) {
        unsigned int i = 0;
        while(i < 3)
        {
            press_power_button(150000);
            thread_sleep_for(250);

            if(gpio_read(&gpio))
            {
                break;
            }
            i++;
        }
    }
}

void ONBOARD_SARA4_PPP::onboard_modem_power_down()
{
    /* Activate PWR_ON for 1.5s to switch off */
    press_power_button(1500000);
    // check for 1.8v low if not, take reset low for 10s
}

#endif // MBED_CONF_NSAPI_PRESENT
