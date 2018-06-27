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

#include "onboard_modem_api.h"
#include "gpio_api.h"
#include "platform/mbed_wait_api.h"
#include "PinNames.h"

#if MODEM_ON_BOARD

// Note microseconds not milliseconds
static void press_power_button(int time_us)
{
    gpio_t gpio;

#if defined(TARGET_UBLOX_C030_R410M)
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrain, 0);
#else
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrainNoPull, 0);
#endif

    wait_us(time_us);
    gpio_write(&gpio, 1);
}

void onboard_modem_init()
{
    gpio_t gpio;

    // Take us out of reset
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull,   1);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull,   0);
    gpio_init_inout(&gpio, MDMRST,  PIN_OUTPUT, OpenDrainNoPull,  1);
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrainNoPull, 1);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_OUTPUT, OpenDrainNoPull, 0);
}

void onboard_modem_deinit()
{
    gpio_t gpio;

    // Back into reset
    gpio_init_inout(&gpio, MDMRST,PIN_OUTPUT, OpenDrainNoPull, 0);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull,   1);
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull,   0);
    gpio_init_inout(&gpio, RADIO_DTR, PIN_OUTPUT, OpenDrainNoPull, 1);
}




void onboard_modem_power_up()
{
#if defined(TARGET_UBLOX_C030_R410M)
	/* keep the power line low for 1 seconds */
    press_power_button(1000000);
#else
	/* keep the power line low for 50 microseconds */
    gpio_t gpio;
    gpio_init_in(&gpio, MON_1V8);

	if(gpio_is_connected(&gpio) != 0 && gpio_read(&gpio) != 0) {
		unsigned int i = 0;
		while(i < 5)
		{
			press_power_button( 2000000);
			wait_ms(1000);
			onboard_modem_deinit();
			wait_ms(1000);
			onboard_modem_init();
			wait_ms(250);

			if(gpio_is_connected(&gpio) != 0 && gpio_read(&gpio) == 0)
			{
				break;
			}
			i++;
		}
	}

    press_power_button( 2000000);

#endif

    /* give modem a little time to respond */
    wait_ms(500);
}

void onboard_modem_power_down()
{
#if defined(TARGET_UBLOX_C030_R410M)
	/* keep the power line low for 1.5 seconds */
    press_power_button(1500000);
#else
	/* keep the power line low for 1 seconds */
    press_power_button(300000);
#endif
}

#endif //MODEM_ON_BOARD
#endif //MBED_CONF_NSAPI_PRESENT
