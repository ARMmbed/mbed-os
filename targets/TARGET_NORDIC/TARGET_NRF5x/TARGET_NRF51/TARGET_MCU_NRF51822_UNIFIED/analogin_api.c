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
#include "nrf_drv_adc.h"

#ifdef DEVICE_ANALOGIN


#define ADC_10BIT_RANGE  0x3FF
#define ADC_RANGE        ADC_10BIT_RANGE

static const PinMap PinMap_ADC[] = {
    {p1, ADC0_0, 4},
    {p2, ADC0_0, 8},
    {p3, ADC0_0, 16},
    {p4, ADC0_0, 32},
    {p5, ADC0_0, 64},
    {p6, ADC0_0, 128},
#ifndef TARGET_NRF51_DONGLE
    {p26, ADC0_0, 1},
    {p27, ADC0_0, 2},
#endif
    {NC, NC, 0}
};

void ADC_IRQHandler(void); // export IRQ handler from nrf_drv_adc.c

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t pinFunc = pinmap_function(pin, PinMap_ADC);
    MBED_ASSERT(pinFunc != (uint32_t)NC);
    
    obj->adc_pin =  pinFunc;
    
    NVIC_SetVector(ADC_IRQn, (uint32_t)ADC_IRQHandler);
    
    ret_code_t ret_code;
                                              // p_config, event_handler
    ret_code = nrf_drv_adc_init(NULL , NULL); // select blocking mode
    MBED_ASSERT((ret_code == NRF_SUCCESS) || (ret_code == NRF_ERROR_INVALID_STATE)); //NRF_ERROR_INVALID_STATE expected for multiple channels used.
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    nrf_adc_value_t adc_value;
       
    nrf_drv_adc_channel_t adc_channel;
    
    // initialization by assigment because IAR dosen't support variable initializer in declaration statement.
    adc_channel.config.config.resolution = NRF_ADC_CONFIG_RES_10BIT;
    adc_channel.config.config.input      = NRF_ADC_CONFIG_SCALING_INPUT_ONE_THIRD;
    adc_channel.config.config.reference  = NRF_ADC_CONFIG_REF_VBG;
    adc_channel.config.config.ain        = (obj->adc_pin);
    adc_channel.p_next = NULL;
    

    ret_code_t ret_code;
    
    ret_code = nrf_drv_adc_sample_convert( &adc_channel, &adc_value);
    MBED_ASSERT(ret_code == NRF_SUCCESS);
    
    return adc_value;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);

    return (float)value * (1.0f / (float)ADC_RANGE);
}

#endif // DEVICE_ANALOGIN
