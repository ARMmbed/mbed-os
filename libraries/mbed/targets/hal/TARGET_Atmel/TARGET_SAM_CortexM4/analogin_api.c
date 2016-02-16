/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "adc2.h"
#include "PeripheralPins.h"

extern uint8_t g_sys_init;
static uint8_t adc_inited = 0;

void analogin_init(analogin_t *obj, PinName pin)
{
    MBED_ASSERT(obj);
    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }
    obj->channel = (enum adc_channel_num)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->channel != NC);
    if(!adc_inited) { /*ADC hardare to be initialised only once*/
        adc_enable();
#if SAMG55
        adc_select_clock_source_mck(ADC);
#endif
        struct adc_config adc_cfg;
        adc_get_config_defaults(&adc_cfg);
        adc_cfg.resolution = ADC_16_BITS;
        adc_init(ADC, &adc_cfg);
        adc_average_on_single_trigger(ADC);
        adc_set_trigger(ADC, ADC_TRIG_SW);
        adc_start_calibration(ADC);
        adc_inited = 1;
    }
    adc_channel_enable(ADC, obj->channel);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    MBED_ASSERT(obj);
    adc_channel_get_value(ADC, obj->channel); /*Dummy read of current value*/
    adc_start_software_conversion(ADC);
    while (adc_get_interrupt_status(ADC) & (1 << obj->channel));
    return (uint16_t)adc_channel_get_value(ADC, obj->channel);
}

float analogin_read(analogin_t *obj)
{
    MBED_ASSERT(obj);
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}
