/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "ublox_low_level_api.h"

#include <stdbool.h>
#include "gpio_api.h"

void ublox_board_init(void)
{
    gpio_t gpio;

    // Enable power to 3V3
    gpio_init_inout(&gpio, PWR3V3, PIN_OUTPUT, OpenDrain, 1);

#if defined(TARGET_UBLOX_C030_R41XM)
    /* In case of SARA-R4, MDMRST needs to be asserted for 10 seconds before modem actually powers down.
     * This means that modem is initially responsive to AT commands but powers down
     * after 10 seconds unless MDMRST is de-asserted (onboard_modem_init()).
     *
     * This will cause confusion for application as CellularDevice::is_ready()
     * will return TRUE initially and later modem will power off without any indication to application.
     */
    gpio_init_out_ex(&gpio, MDMRST,    1);
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrain, 1);
#else
    gpio_init_out_ex(&gpio, MDMRST,    0);
    gpio_init_out_ex(&gpio, MDMPWRON,  0);
#endif
    gpio_init_out_ex(&gpio, MDMRTS,    0);
    gpio_init_in_ex(&gpio,  MDMCURRENTSENSE, PullNone);

#if !defined (TARGET_UBLOX_C030_R41XM)
    // start with GNSS disabled, this is ONLY TEMPORARY and that once the HW issue with the GNSSEN pin on the R41XM board is resolved then this line will become default for all platforms.
    gpio_init_inout(&gpio,  GNSSEN,  PIN_OUTPUT, PushPullNoPull, 0);
#endif

    // power on SD card
    gpio_init_out_ex(&gpio, SDPWRON,   1);

    // led should be off
    gpio_init_out_ex(&gpio, LED1,      1);
    gpio_init_out_ex(&gpio, LED2,      1);
    gpio_init_out_ex(&gpio, LED3,      1);
    gpio_init_out_ex(&gpio, LED4,      1);
}

// End Of File
