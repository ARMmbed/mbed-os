/* mbed Microcontroller Library
 * Copyright (c) 2017 Arm Limited
 *
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

#include "cellular/onboard_modem_api.h"
#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"

#define WAIT_AFTER_POWR_CHANGED	(1000)	// [msec.]

#if MODEM_ON_BOARD

static void press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PWRKEY, 1);
    thread_sleep_for(time_ms);
    gpio_write(&gpio, 0);
}

void onboard_modem_init()
{
    gpio_t gpio;

    // Power Supply
    gpio_init_out_ex(&gpio, M_POWR, 0);
    // Turn On/Off
    gpio_init_out_ex(&gpio, PWRKEY, 0);
    gpio_init_out_ex(&gpio, RESET_MODULE, 0);
    // Status Indication
    gpio_init_in_ex(&gpio, MDMSTAT, PullUp);
    // Main UART Interface
    gpio_init_out_ex(&gpio, MDMDTR, 0);

    thread_sleep_for(WAIT_AFTER_POWR_CHANGED);
}

void onboard_modem_deinit()
{
    onboard_modem_power_down();
}

void onboard_modem_power_up()
{
	gpio_t gpio;

    // Power supply ON
	gpio_init_out_ex(&gpio, M_POWR, 1);
	thread_sleep_for(WAIT_AFTER_POWR_CHANGED);

	// Turn on
	thread_sleep_for(100);
	press_power_button(200);
}

void onboard_modem_power_down()
{
	gpio_t gpio;

	// Power supply OFF
	gpio_init_out_ex(&gpio, M_POWR, 0);
	thread_sleep_for(WAIT_AFTER_POWR_CHANGED);
}
#endif //MODEM_ON_BOARD

#endif //MBED_CONF_NSAPI_PRESENT
