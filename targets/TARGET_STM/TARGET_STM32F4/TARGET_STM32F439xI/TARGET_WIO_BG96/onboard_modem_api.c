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

#include "cellular/onboard_modem_api.h"
#include "gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "PinNames.h"

#if MODEM_ON_BOARD

static void press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PWRKEY, 1);
    gpio_write(&gpio, 1);
    wait_ms(time_ms);
    gpio_write(&gpio, 0);
}

void onboard_modem_init()
{
    gpio_t gpio;
    // start with modem disabled
    gpio_init_out_ex(&gpio, RESET_MODULE, 0);
    gpio_init_in_ex(&gpio, MDMSTAT, PullUp);
    gpio_init_out_ex(&gpio, MDMDTR,       0);
    gpio_init_out_ex(&gpio, M_POWR, 1);

//    gpio_write(&gpio, M_POWR, 1);
    wait_ms(500);
}

void onboard_modem_deinit()
{
//    wio3g_mdm_powerOff();
}

void onboard_modem_power_up()
{
    /* keep the power line HIGH for 200 milisecond */
    press_power_button(200);
    /* give modem a little time to respond */
    wait_ms(100);
}

void onboard_modem_power_down()
{
    /* keep the power line low for 1 second */
//    press_power_button(1000);

//    gpio_write(&mpowr, M_POWR, 0);
}
#endif //MODEM_ON_BOARD
#endif //MBED_CONF_NSAPI_PRESENT
