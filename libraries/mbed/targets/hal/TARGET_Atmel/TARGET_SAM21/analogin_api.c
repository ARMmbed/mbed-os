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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "adc.h"

void analogin_init(analogin_t *obj, PinName pin)
{
    struct adc_config config_adc;
    uint32_t pos_input;

    switch(pin) {
        case PA04:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN4;
            break;
        case PA05:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN5;
            break;
        case PA06:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN6;
            break;
        case PA07:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN7;
            break;
        case PA08:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN16;
            break;
        case PA09:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN17;
            break;
        case PB02:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN10;
            break;
        case PB03:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN11;
            break;
        default:
            pos_input = ADC_INPUTCTRL_MUXPOS_PIN4;  // MBED_ASSERT
            break;

    }
    adc_get_config_defaults(&(obj->adc_instance));
    config_adc.positive_input = pos_input;
    adc_init(&(obj->adc_instance), ADC, &config_adc);
    adc_enable(&(obj->adc_instance));
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t result;
    adc_start_conversion(&(obj->adc_instance));
    adc_read(&(obj->adc_instance), &result);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}

