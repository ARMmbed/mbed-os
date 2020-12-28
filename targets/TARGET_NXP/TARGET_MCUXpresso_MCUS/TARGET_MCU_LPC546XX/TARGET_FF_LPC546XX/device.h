// The 'features' section in 'target.json' is now used to create the device's hardware preprocessor switches.
// Check the 'features' section of the target description in 'targets.json' for more details.
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#define NUMBER_OF_GPIO_INTS    8

#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                        \
    (SYSCON_PDRUNCFG_PDEN_WDT_OSC_MASK |  SYSCON_PDRUNCFG_PDEN_SRAMX_MASK |                               \
     SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDRUNCFG_PDEN_SRAM1_2_3_MASK)

/* Defines used by the sleep code */
#define LPC_CLOCK_INTERNAL_IRC BOARD_BootClockFRO12M()
#define LPC_CLOCK_RUN          ((SYSCON->DEVICE_ID0 == 0xFFF54628) ? \
                                 BOARD_BootClockPLL220M() : BOARD_BootClockPLL180M())

#define DEVICE_ID_LENGTH       24





#include "objects.h"

#endif
