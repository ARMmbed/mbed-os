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
#include "app_util_platform.h"
#include "nrf_drv_saadc.h"

#ifdef DEVICE_ANALOGIN

#define ADC_12BIT_RANGE 0xFFF
#define ADC_RANGE       ADC_12BIT_RANGE

void analog_in_event_handler(nrf_drv_saadc_evt_t const *p_event)// type of nrf_drv_saadc_event_handler_t 
{
    (void) p_event;
}

const nrf_drv_saadc_config_t saadc_config = 
{
    .resolution         = NRF_SAADC_RESOLUTION_12BIT,
    .oversample         = NRF_SAADC_OVERSAMPLE_DISABLED,
    .interrupt_priority = APP_IRQ_PRIORITY_LOW
};

void analogin_init(analogin_t *obj, PinName pin)
{
    /// @todo check if initialized
    ret_code_t ret_code;
    
    ret_code = nrf_drv_saadc_init(&saadc_config, analog_in_event_handler);
    MBED_ASSERT(((ret_code == NRF_SUCCESS) || (ret_code == NRF_ERROR_INVALID_STATE))); //NRF_ERROR_INVALID_STATE expected for multiple channels used.
    
    uint8_t saadcIn = nrf_drv_saadc_gpio_to_ain(pin);
    MBED_ASSERT(saadcIn != NRF_SAADC_INPUT_DISABLED);
    
    obj->adc     = ADC0_0; // only one instance of ADC in nRF52 SoC
    obj->adc_pin = saadcIn  - 1;
    
    nrf_saadc_channel_config_t channel_config =
            NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(saadcIn);
    
    ret_code = nrf_drv_saadc_channel_init(obj->adc_pin, &channel_config);
    MBED_ASSERT(ret_code == NRF_SUCCESS);
}

int16_t analogin_read_i16(analogin_t *obj)
{
    nrf_saadc_value_t adc_value;
    ret_code_t ret_code;
    
    ret_code = nrf_drv_saadc_sample_convert(obj->adc_pin, &adc_value);
    MBED_ASSERT(ret_code == NRF_SUCCESS);
    
    return adc_value;
}

float analogin_read(analogin_t *obj)
{
    int16_t value = analogin_read_i16(obj);
    return (float)value * (1.0f / (float)ADC_RANGE);
}

#endif // DEVICE_ANALOGIN
