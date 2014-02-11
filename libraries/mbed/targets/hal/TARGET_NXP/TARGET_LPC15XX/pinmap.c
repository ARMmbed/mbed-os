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

__IO uint32_t* IOCON_REGISTERS[76] = {
        &LPC_IOCON->PIO0_0 , &LPC_IOCON->PIO0_1 , &LPC_IOCON->PIO0_2 ,
        &LPC_IOCON->PIO0_3 , &LPC_IOCON->PIO0_4 , &LPC_IOCON->PIO0_5 ,
        &LPC_IOCON->PIO0_6 , &LPC_IOCON->PIO0_7 , &LPC_IOCON->PIO0_8 ,
        &LPC_IOCON->PIO0_9 , &LPC_IOCON->PIO0_10, &LPC_IOCON->PIO0_11,
        &LPC_IOCON->PIO0_12, &LPC_IOCON->PIO0_13, &LPC_IOCON->PIO0_14,
        &LPC_IOCON->PIO0_15, &LPC_IOCON->PIO0_16, &LPC_IOCON->PIO0_17,
        &LPC_IOCON->PIO0_18, &LPC_IOCON->PIO0_19, &LPC_IOCON->PIO0_20,
        &LPC_IOCON->PIO0_21, &LPC_IOCON->PIO0_22, &LPC_IOCON->PIO0_23,
        &LPC_IOCON->PIO0_24, &LPC_IOCON->PIO0_25, &LPC_IOCON->PIO0_26,
        &LPC_IOCON->PIO0_27, &LPC_IOCON->PIO0_28, &LPC_IOCON->PIO0_29,
        &LPC_IOCON->PIO0_30, &LPC_IOCON->PIO0_31,

        &LPC_IOCON->PIO1_0 , &LPC_IOCON->PIO1_1 , &LPC_IOCON->PIO1_2 ,
        &LPC_IOCON->PIO1_3 , &LPC_IOCON->PIO1_4 , &LPC_IOCON->PIO1_5 ,
        &LPC_IOCON->PIO1_6 , &LPC_IOCON->PIO1_7 , &LPC_IOCON->PIO1_8 ,
        &LPC_IOCON->PIO1_9 , &LPC_IOCON->PIO1_10, &LPC_IOCON->PIO1_11,
        &LPC_IOCON->PIO1_12, &LPC_IOCON->PIO1_13, &LPC_IOCON->PIO1_14,
        &LPC_IOCON->PIO1_15, &LPC_IOCON->PIO1_16, &LPC_IOCON->PIO1_17,
        &LPC_IOCON->PIO1_18, &LPC_IOCON->PIO1_19, &LPC_IOCON->PIO1_20,
        &LPC_IOCON->PIO1_21, &LPC_IOCON->PIO1_22, &LPC_IOCON->PIO1_23,
        &LPC_IOCON->PIO1_24, &LPC_IOCON->PIO1_25, &LPC_IOCON->PIO1_26,
        &LPC_IOCON->PIO1_27, &LPC_IOCON->PIO1_28, &LPC_IOCON->PIO1_29,
        &LPC_IOCON->PIO1_30, &LPC_IOCON->PIO1_31,

        &LPC_IOCON->PIO2_0 , &LPC_IOCON->PIO2_1 , &LPC_IOCON->PIO2_2 ,
        &LPC_IOCON->PIO2_3 , &LPC_IOCON->PIO2_4 , &LPC_IOCON->PIO2_5 ,
        &LPC_IOCON->PIO2_6 , &LPC_IOCON->PIO2_7 , &LPC_IOCON->PIO2_8 ,
        &LPC_IOCON->PIO2_9 , &LPC_IOCON->PIO2_10, &LPC_IOCON->PIO2_11,
};

void pin_function(PinName pin, int function) {
    
}

void pin_mode(PinName pin, PinMode mode) {
    if (pin == (uint32_t)NC) { return; }
    
    if ((pin == 22) || (pin == 23)) {
        // The true open-drain pins PIO0_22 and PIO0_23 can be configured for different I2C-bus speeds.
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
}
