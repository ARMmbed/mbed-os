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

void ublox_board_init(void) {
    gpio_t gpio;

    // Enable power to 3V3
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull, 1);
    gpio_init_inout(&gpio, VUSB_EN, PIN_OUTPUT, OpenDrainNoPull, 0);
    
    // start with modem disabled 
    gpio_init_out_ex(&gpio, MDMRST, 1);
    gpio_init_out_ex(&gpio, MDMPWRON, 0);
}

// End Of File
