/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "UBLOX_onboard_modem_api.h"
#include "gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"

// Note microseconds not milliseconds
static void press_power_button(int time_us)
{
    gpio_t gpio;

#if defined(TARGET_UBLOX_C030_R41XM)
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrain, 0);
#else
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
#endif

    wait_us(time_us);
    gpio_write(&gpio, 1);
}

void onboard_modem_init()
{
    gpio_t gpio;

#if defined(TARGET_UBLOX_C030_R41XM)
    // Set the pin to high so on powerup we can set low
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrain, 1);
#endif
    // Take us out of reset
    gpio_init_out_ex(&gpio, MDMRST,    1);
}

void onboard_modem_deinit()
{
#ifndef TARGET_UBLOX_C030_R41XM
    gpio_t gpio;

    // Back into reset
    gpio_init_out_ex(&gpio, MDMRST, 0);
#endif
}

void onboard_modem_power_up()
{
#if defined(TARGET_UBLOX_C030_R41XM)
    /* keep the power line low for 1 seconds */
    press_power_button(1000000);
#else
    /* keep the power line low for 50 microseconds */
    press_power_button(50);
#endif

    /* give modem a little time to respond */
    thread_sleep_for(100);
}

void onboard_modem_power_down()
{
    /* keep the power line low for 1.5 seconds */
    press_power_button(1500000);
}

#endif //MBED_CONF_NSAPI_PRESENT
