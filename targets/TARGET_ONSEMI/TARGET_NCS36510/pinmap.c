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

/* Orion specific header files */

#include "pad.h"

/* MBED header files */
#include "pinmap.h"

void pin_function(PinName pin, int function)
{
    /** - Enable the clock for PAD peripheral device */
    CLOCK_ENABLE(CLOCK_CROSSB);

    /* Note: GPIO 0,1,2,3 are used for UART 1, GPIO 8,9 are used for UART 2 */
    CrossbReg_pt crossBar = (CrossbReg_t *)(CROSSBREG_BASE + (pin * CROSS_REG_ADRS_BYTE_SIZE));
    crossBar->DIOCTRL0 = function;

    /** - Disable the clock for PAD peripheral device */
    CLOCK_DISABLE(CLOCK_CROSSB);
}

void pin_mode(PinName pin, PinMode mode)
{
    /** - Get PAD IO register address for the PAD number */
    PadReg_t *padRegOffset = (PadReg_t*)(PADREG_BASE + (pin * PAD_REG_ADRS_BYTE_SIZE));

    /** - Enable the clock for PAD peripheral device */
    CLOCK_ENABLE(CLOCK_PAD);

    switch (mode) {
        case PushPullPullDown:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OUTCFG_PUSHPULL;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_DOWN;
            break;

        case PushPullNoPull:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OUTCFG_PUSHPULL;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_NONE;
            break;

        case PushPullPullUp:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OUTCFG_PUSHPULL;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_UP;
            break;

        case OpenDrainPullDown:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OOUTCFG_OPENDRAIN;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_DOWN;
            break;

        case OpenDrainNoPull:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OOUTCFG_OPENDRAIN;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_NONE;
            break;

        case OpenDrainPullUp:
            padRegOffset->PADIO0.BITS.TYPE = PAD_OOUTCFG_OPENDRAIN;
            padRegOffset->PADIO0.BITS.PULL = PAD_PULL_UP;
            break;

        default:
            break;
    }

    /** - Disable the clock for PAD peripheral device */
    CLOCK_DISABLE(CLOCK_PAD);

}
