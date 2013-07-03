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
 *
 * Ported to NXP LPC43XX by Micromint USA <support@micromint.com>
 */
#include "analogout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

static const PinMap PinMap_DAC[] = {
    {P_DAC0 , DAC_0, 0x0},
    {NC     , NC   , 0}
};

void analogout_init(dac_t *obj, PinName pin) {
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    if (obj->dac == (DACName)NC) {
        error("DAC pin mapping failed");
    }

    // Configure the pin as GPIO input
    pin_function(pin, (SCU_PINIO_PULLNONE | 0x0));
    pin_mode(pin, PullNone);
    // Select DAC on analog function select register in SCU
    LPC_SCU->ENAIO[2] |= 1; // Sets pin P4_4 as DAC

    // Set Maximum update rate for DAC */
    LPC_DAC->CR &= ~DAC_BIAS_EN;
	
    analogout_write_u16(obj, 0);
}

void analogout_free(dac_t *obj) {}

static inline void dac_write(int value) {
    uint32_t tmp;
    
    // Set the DAC output
    tmp = LPC_DAC->CR & DAC_BIAS_EN;
    tmp |= DAC_VALUE(value);
    LPC_DAC->CR = tmp;
}

static inline int dac_read() {
    return (DAC_VALUE(LPC_DAC->CR));
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(0);
    } else if (value > 1.0f) {
        dac_write(DAC_RANGE);
    } else {
        dac_write(value * (float)DAC_RANGE);
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    dac_write(value >> 6); // 10-bit
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read();
    return (float)value * (1.0f / (float)DAC_RANGE);
}

uint16_t analogout_read_u16(dac_t *obj) {
    uint32_t value = dac_read(); // 10-bit
    return (value << 6) | ((value >> 4) & 0x003F);
}
