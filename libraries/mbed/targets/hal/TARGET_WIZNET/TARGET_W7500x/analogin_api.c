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

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"


ADC_TypeDef * AdcHandle;

int adc_inited = 0;

void analogin_init(analogin_t *obj, PinName pin)
{
    // Get the peripheral name from the pin and assign it to the object
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);

    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Configure GPIO
    pinmap_pinout(pin, PinMap_ADC);

    // Save pin number for the read function
    obj->pin = pin;

    // The ADC initialization is done once
    if (adc_inited == 0) {
        adc_inited = 1;

        ADC_Init();
    }
}

static inline uint16_t adc_read(analogin_t *obj)
{
    ADC_CH adc_ch;

    switch (obj->pin) {
        case PC_15:
            adc_ch = ADC_CH0;
            break;
        case PC_14:
            adc_ch = ADC_CH1;
            break;
        case PC_13:
            adc_ch = ADC_CH2;
            break;
        case PC_12:
            adc_ch = ADC_CH3;
            break;
        case PC_11:
            adc_ch = ADC_CH4;
            break;
        case PC_10:
            adc_ch = ADC_CH5;
            break;
        default:
            return 0;
    }

    ADC_ChannelSelect(adc_ch);
    ADC_Start();

    // Wait end of conversion and get value
    while(ADC_IsInterrupt());
    return ADC_ReadData();
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    // 12-bit to 16-bit conversion
    return value;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = adc_read(obj);

    return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}

#endif
