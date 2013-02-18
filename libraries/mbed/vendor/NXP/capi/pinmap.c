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
#include "pinmap.h"
#include "error.h"

/* PORTING STEP 2:
   Implement "pin_function" and "pin_mode" to set:
     * the pin functionality (configuring the multiplexing block)
     * the pin pullup settings
 */

#if defined(TARGET_LPC11U24)
#define LPC_IOCON0_BASE (LPC_IOCON_BASE)
#define LPC_IOCON1_BASE (LPC_IOCON_BASE + 0x60)
#endif

void pin_function(PinName pin, int function) {
    if (pin == (uint32_t)NC) return;

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 4;
    int offset = (pin_number & 0xF) << 1;

    PINCONARRAY->PINSEL[index] &= ~(0x3 << offset);
    PINCONARRAY->PINSEL[index] |= function << offset;

#elif defined(TARGET_LPC11U24)
    uint32_t pin_number = (uint32_t)pin;

    __IO uint32_t *reg = (pin_number < 32) ?
            (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * pin_number) :
            (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));

    // pin function bits: [2:0] -> 111 = (0x7)
    *reg = (*reg & ~0x7) | (function & 0x7);
#endif
}

void pin_mode(PinName pin, PinMode mode) {
    if (pin == (uint32_t)NC) { return; }

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 5;
    int offset = pin_number & 0x1F;
    uint32_t drain = ((uint32_t) mode & (uint32_t) OpenDrain) >> 2;

#if defined(TARGET_LPC2368)
    if (mode == OpenDrain) error("OpenDrain not supported on LPC2368");

#elif defined(TARGET_LPC1768)
    PINCONARRAY->PINMODE_OD[index] &= ~(drain << offset);
    PINCONARRAY->PINMODE_OD[index] |= drain << offset;
#endif

    if (!drain) {
        index = pin_number >> 4;
        offset = (pin_number & 0xF) << 1;

        PINCONARRAY->PINMODE[index] &= ~(0x3 << offset);
        PINCONARRAY->PINMODE[index] |= (uint32_t)mode << offset;
    }

#elif defined(TARGET_LPC11U24)
    uint32_t pin_number = (uint32_t)pin;
    uint32_t drain = ((uint32_t) mode & (uint32_t) OpenDrain) >> 2;

    // Open drain mode is not available on LPC2368
    __IO uint32_t *reg = (pin_number < 32) ?
            (__IO uint32_t*)(LPC_IOCON0_BASE + 4 * pin_number) :
            (__IO uint32_t*)(LPC_IOCON1_BASE + 4 * (pin_number - 32));
    uint32_t tmp = *reg;

    // pin mode bits: [4:3] -> 11000 = (0x3 << 3)
    tmp &= ~(0x3 << 3);
    tmp |= (mode & 0x3) << 3;

    // drain
    tmp &= ~(0x1 << 10);
    tmp |= drain << 10;

    *reg = tmp;
#endif
}
