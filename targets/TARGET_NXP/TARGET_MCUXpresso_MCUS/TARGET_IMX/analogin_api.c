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

#include "cmsis.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "PeripheralNames.h"
#include "fsl_adc.h"
#include "PeripheralPins.h"

/* Array of ADC peripheral base address. */
static ADC_Type *const adc_addrs[] = ADC_BASE_PTRS;

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    adc_config_t adc_config;
    gpio_t gpio;

    ADC_GetDefaultConfig(&adc_config);
    ADC_Init(adc_addrs[instance], &adc_config);
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(adc_addrs[instance], false);
#endif
    ADC_DoAutoCalibration(adc_addrs[instance]);

    /* Need to ensure the pin is in input mode */
    gpio_init(&gpio, pin);
    gpio_dir(&gpio, PIN_INPUT);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    adc_channel_config_t adc_channel_config;

    adc_channel_config.channelNumber = obj->adc & 0xF;
    adc_channel_config.enableInterruptOnConversionCompleted = false;

    /*
      When in software trigger mode, each conversion would be launched once calling the "ADC_ChannelConfigure()"
      function, which works like writing a conversion command and executing it. For another channel's conversion,
      just to change the "channelNumber" field in channel configuration structure, and call the function
      "ADC_ChannelConfigure()"" again.
      Also, the "enableInterruptOnConversionCompleted" inside the channel configuration structure is a parameter
      for the conversion command. It takes affect just for the current conversion. If the interrupt is still required
      for the following conversion, it is necessary to assert the "enableInterruptOnConversionCompleted" every
      time for each command.
    */
    ADC_SetChannelConfig(adc_addrs[instance], 0, &adc_channel_config);
    while (0U == ADC_GetChannelStatusFlags(adc_addrs[instance], 0))
    {
    }
    return ADC_GetChannelConversionValue(adc_addrs[instance], 0);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFFF);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
