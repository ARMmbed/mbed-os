/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#if DEVICE_ANALOGIN
#include "mbed_assert.h"
#include "analogin_api.h"

#include "cmsis.h"
#include "PeripheralPins.h"

#include "iodefine.h"

#define ANALOGIN_MEDIAN_FILTER      0

static volatile uint16_t *ADDR[] = {
    &ADC.ADDR0.WORD,
    &ADC.ADDR1.WORD,
    &ADC.ADDR2.WORD,
    &ADC.ADDR3.WORD,
    &ADC.ADDR4.WORD,
    &ADC.ADDR5.WORD,
    &ADC.ADDR6.WORD,
    &ADC.ADDR7.WORD,
};

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    CPG.STBCR5.BIT.MSTP57 = 0;

    ADC.ADCSR.BIT.ADCS = 0x0;  /* single scan mode */

    pinmap_pinout(pin, PinMap_ADC);
}

static inline uint32_t adc_read(analogin_t *obj)
{

    ADC.ADANSA0.WORD = (1 << obj->adc);
    ADC.ADCSR.BIT.ADST = 0x1;

    // Wait end of conversion
    while (ADC.ADCSR.BIT.ADST != 0) {
        ;

    }
    return *(ADDR[obj->adc]) & 0x0FFF;   // 12 bits range
}

#if ANALOGIN_MEDIAN_FILTER
static inline void order(uint32_t *a, uint32_t *b)
{
    if (*a > *b) {
        uint32_t t = *a;
        *a = *b;
        *b = t;
    }
}
#endif

static inline uint32_t adc_read_u32(analogin_t *obj)
{
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

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t value = adc_read_u32(obj);

    return (value << 4) | ((value >> 8) & 0x000F); // 12-bit to 16-bit conversion
}

float analogin_read(analogin_t *obj)
{
    uint32_t value = adc_read_u32(obj);

    return (float)value * (1.0f / (float)0x0FFF);  // 12 bits range
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
#endif
