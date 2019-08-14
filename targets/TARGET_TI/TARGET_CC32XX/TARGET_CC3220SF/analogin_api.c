/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#include "pinmap.h"
#include "PeripheralPins.h"
#include "PinNames.h"
#include "cmsis.h"

#include "ti/devices/cc32xx/inc/hw_types.h"
#include "ti/devices/cc32xx/driverlib/adc.h"
#include "ti/devices/cc32xx/driverlib/prcm.h"
#include "ti/devices/cc32xx/driverlib/pin.h"
#include "ti/devices/cc32xx/inc/hw_memmap.h"

#define ADC_DATA_MASK 0x3FFC //the data is from bit [13:2]
#define ADC_RESOLUTION 0xFFF

void analogin_init(analogin_t *obj, PinName pin)
{
    ADCEnable(CC3220SF_ADC_BASE);
    obj->pin = pin;
    pin_mode(pin, Analog);
    switch (pin) {
        case PIN_57:
            obj->adc_ch = ADC_CH_0;
            break;
        case PIN_58:
            obj->adc_ch = ADC_CH_1;
            break;
        case PIN_59:
            obj->adc_ch = ADC_CH_2;
            break;
        case PIN_60:
            obj->adc_ch = ADC_CH_3;
            break;
        default:
            MBED_ASSERT(NC != (PinName)NC);
    }

    ADCChannelEnable(CC3220SF_ADC_BASE, obj->adc_ch);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    unsigned long adc_raw =  ADCFIFORead(CC3220SF_ADC_BASE, obj->adc_ch);

    return (uint16_t)((adc_raw & ADC_DATA_MASK) >> 2);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)ADC_RESOLUTION);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
