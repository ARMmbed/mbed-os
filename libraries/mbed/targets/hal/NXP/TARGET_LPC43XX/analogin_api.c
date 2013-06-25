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
#include "analogin_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

#define ANALOGIN_MEDIAN_FILTER      1

static inline int div_round_up(int x, int y) {
  return (x + (y - 1)) / y;
}

// ToDo: Add support for ADC1
static const PinMap PinMap_ADC[] = {
    {P_ADC0, ADC0_0, 0x08},
    {P_ADC1, ADC0_1, 0x07},
    {P_ADC2, ADC0_2, 0x01},
    {P_ADC3, ADC0_3, 0x08},
    {P_ADC4, ADC0_4, 0x08},
    {P_ADC5, ADC0_5, 0x08},
    {NC   , NC    , 0   }
};

void analogin_init(analogin_t *obj, PinName pin) {
    uint8_t num, chan;

    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    if (obj->adc == (uint32_t)NC) {
        error("ADC pin mapping failed");
    }


    // Configure the pin as GPIO input
    if (pin < SFP_AIO0) {
        pin_function(pin, (SCU_PINIO_PULLNONE | 0x0));
        pin_mode(pin, PullNone);
        num = (uint8_t)(obj->adc) / 8; // Heuristic?
        chan = (uint8_t)(obj->adc) % 7;
    } else {
        num = MBED_ADC_NUM(pin);
        chan = MBED_ADC_CHAN(pin);
    }

    // Calculate minimum clock divider
    //  clkdiv = divider - 1
		uint32_t PCLK = SystemCoreClock;
    uint32_t adcRate = 400000;
    uint32_t clkdiv = div_round_up(PCLK, adcRate) - 1;
    
    // Set the generic software-controlled ADC settings
    LPC_ADC0->CR = (0 << 0)      // SEL: 0 = no channels selected
                  | (clkdiv << 8) // CLKDIV:
                  | (0 << 16)     // BURST: 0 = software control
                  | (1 << 21)     // PDN: 1 = operational
                  | (0 << 24)     // START: 0 = no start
                  | (0 << 27);    // EDGE: not applicable

    // Select ADC on analog function select register in SCU
    LPC_SCU->ENAIO[num] |= 1UL << chan;
}

static inline uint32_t adc_read(analogin_t *obj) {
    // Select the appropriate channel and start conversion
    LPC_ADC0->CR &= ~0xFF;
    LPC_ADC0->CR |= 1 << (int)obj->adc;
    LPC_ADC0->CR |= 1 << 24;

    // Repeatedly get the sample data until DONE bit
    unsigned int data;
    do {
        data = LPC_ADC0->GDR;
    } while ((data & ((unsigned int)1 << 31)) == 0);

    // Stop conversion
    LPC_ADC0->CR &= ~(1 << 24);

    return (data >> 6) & ADC_RANGE; // 10 bit
}

static inline void order(uint32_t *a, uint32_t *b) {
    if (*a > *b) {
        uint32_t t = *a;
        *a = *b;
        *b = t;
    }
}

static inline uint32_t adc_read_u32(analogin_t *obj) {
    uint32_t value;
#if ANALOGIN_MEDIAN_FILTER
    uint32_t v1 = adc_read(obj);
    uint32_t v2 = adc_read(obj);
    uint32_t v3 = adc_read(obj);
    order(&v1, &v2);
    order(&v2, &v3);
    order(&v1, &v2);
    value = v2;
#else
    value = adc_read(obj);
#endif
    return value;
}

uint16_t analogin_read_u16(analogin_t *obj) {
    uint32_t value = adc_read_u32(obj);

    return (value << 6) | ((value >> 4) & 0x003F); // 10 bit
}

float analogin_read(analogin_t *obj) {
    uint32_t value = adc_read_u32(obj);
    return (float)value * (1.0f / (float)ADC_RANGE);
}
