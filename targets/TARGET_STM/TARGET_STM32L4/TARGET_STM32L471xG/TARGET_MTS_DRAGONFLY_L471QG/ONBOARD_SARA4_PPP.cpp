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
    initialized = 0;
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
    // See base function description. This function is for power on but reset too.
    onboard_modem_power_down();
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

    // Enable radio power regulator and buffer, configure RESET_N and PWR_ON.
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull,   1);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull,   0);
    gpio_init_out_ex(&gpio, MDMRST, 0);
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_OUTPUT, OpenDrainNoPull, 0);
    initialized = 1;
}

void ONBOARD_SARA4_PPP::onboard_modem_deinit()
{
    // Make sure to power down before removing power!
    onboard_modem_power_down();
    gpio_t gpio;

    // Set all to input no pull. Let pull resistors do their thing. Allows for lowest power draw.
    // Disable radio power regulator and buffer.
    gpio_init_inout(&gpio, MDMRST, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, MDMPWRON, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, BUF_EN, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, RADIO_PWR, PIN_INPUT, PullNone, 0);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_INPUT, PullNone, 1);
    initialized = 0;
}

void ONBOARD_SARA4_PPP::onboard_modem_power_up()
{
    // Make sure the radio is initialized so it can be powered on.
    if (!initialized){
        onboard_modem_init();
    }

    gpio_t radioOn;
    gpio_init_in(&radioOn, MON_1V8);

    // Need volatile so 1.8v check is not optimized out.
    volatile int v1_8 = gpio_read(&radioOn);

    // If radio is on, do nothing.
    if (v1_8) {
        return;
    }

    uint8_t retries = 5;
    do {
        // If it hasn't turned on after multiple tries, exit and return.
        if(0 == retries--) {
            return;
        }
        // Activate PWR_ON for 150ms-3.2s to switch on. Reference ublox SARA-R4 data sheet.
        press_power_button(160000);
        v1_8 = gpio_read(&radioOn);
    } while (!v1_8);
    // Takes ~4.5s to power on. Reference SARA-R4 system integration manual... module power-on.
    // Pad a few more seconds to be sure.
    thread_sleep_for(8000);
}

void ONBOARD_SARA4_PPP::onboard_modem_power_down()
{
    gpio_t radioOn;
    gpio_init_in(&radioOn, MON_1V8);
    // Need volatile so 1.8v check is not optimized out.
    volatile int v1_8 = gpio_read(&radioOn);
    // Do nothing if it's already off.
    if (!v1_8) {
        return;
    }

    // Make sure the I/O are properly initialized.
    if (!initialized){
        onboard_modem_init();
    }
    // Activate PWR_ON for at least 1.5s to switch off. Reference ublox SARA-R4 data sheet.
    press_power_button(1600000);

    // wait a max of 40s for 1.8v to go low. Reference AT command guide +CPWROFF estimated response.
    uint8_t timeout = 40;
    do {
        thread_sleep_for(1000);
        v1_8 = gpio_read(&radioOn);
    } while (v1_8 && timeout--);

    // hold RESET_N low (inverted) at least 10s to power down the radio.
    if (v1_8) {
        gpio_t gpio;
        gpio_init_out_ex(&gpio, RADIO_RESET, 1);
        thread_sleep_for(11000);
        gpio_write(&gpio, 0);
    }

    v1_8 = gpio_read(&radioOn);
    // Cut power to the radio if it's still not powered off.
    if (v1_8) {
        gpio_t gpio;
        gpio_init_inout(&gpio, RADIO_PWR, PIN_INPUT, PullNone, 0);
        thread_sleep_for(1000);
        initialized = 0;
    }
}

#endif // MBED_CONF_NSAPI_PRESENT
