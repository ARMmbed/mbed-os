/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include "mbed_assert.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "rda_ccfg_api.h"

void pin_function(PinName pin, int function)
{
    int index    = pin >> PORT_SHIFT;
    int raw_ofst = pin & 0x00FF;
    int offset;

    MBED_ASSERT(pin != (PinName)NC);

    switch (index) {
        case 1:
            if ((8 == raw_ofst) && (rda_ccfg_hwver() >= 5)) {
                function ^= 0x01;
            }
            break;
        case 4:
            if (1 < raw_ofst) {
                offset = raw_ofst << 1;
                RDA_PINCFG->MODE2 &= ~(0x03UL << offset);
            }
            break;
        case 5:
            if (2 > raw_ofst) {
                offset = (raw_ofst << 1) + 20;
                RDA_PINCFG->MODE2 &= ~(0x03UL << offset);
            } else {
                offset = (raw_ofst << 1) - 4;
                RDA_PINCFG->MODE3 &= ~(0x03UL << offset);
            }
            break;
        default:
            break;
    }

    offset = raw_ofst * 3;
    RDA_PINCFG->IOMUXCTRL[index] &= ~(0x07UL << offset);
    RDA_PINCFG->IOMUXCTRL[index] |= ((function & 0x07UL) << offset);
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
}
