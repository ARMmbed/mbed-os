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

#if defined(TARGET_LPC11U24)
#define LPC_IOCON0_BASE (LPC_IOCON_BASE)
#define LPC_IOCON1_BASE (LPC_IOCON_BASE + 0x60)

#elif defined(TARGET_LPC812)
__IO uint32_t* IOCON_REGISTERS[18] = {
        &LPC_IOCON->PIO0_0 , &LPC_IOCON->PIO0_1 , &LPC_IOCON->PIO0_2 ,
        &LPC_IOCON->PIO0_3 , &LPC_IOCON->PIO0_4 , &LPC_IOCON->PIO0_5 ,
        &LPC_IOCON->PIO0_6 , &LPC_IOCON->PIO0_7 , &LPC_IOCON->PIO0_8 ,
        &LPC_IOCON->PIO0_9 , &LPC_IOCON->PIO0_10, &LPC_IOCON->PIO0_11,
        &LPC_IOCON->PIO0_12, &LPC_IOCON->PIO0_13, &LPC_IOCON->PIO0_14,
        &LPC_IOCON->PIO0_15, &LPC_IOCON->PIO0_16, &LPC_IOCON->PIO0_17,
};
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

#elif defined(TARGET_LPC4088)
    __IO uint32_t *reg = (__IO uint32_t*) (LPC_IOCON_BASE + 4 * pin);

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

#elif defined(TARGET_LPC812)
    if ((pin == 10) || (pin == 11)) {
        // True open-drain pins can be configured for different I2C-bus speeds
        return;
    }
    
    __IO uint32_t *reg = IOCON_REGISTERS[pin];
    
    if (mode == OpenDrain) {
        *reg |= (1 << 10);
    } else {
        uint32_t tmp = *reg;
        tmp &= ~(0x3 << 3);
        tmp |= (mode & 0x3) << 3;
        *reg = tmp;
    }

#elif defined(TARGET_LPC4088)
    uint32_t drain = ((uint32_t) mode & (uint32_t) OpenDrain) >> 2;

    __IO uint32_t *reg = (__IO uint32_t*) (LPC_IOCON_BASE + 4 * pin);
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
