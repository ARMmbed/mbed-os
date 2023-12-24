/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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

#include "analogout_api.h"
#include "mbed_assert.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

#define DAC_BITS    12

 /*!
 * @brief     Initialize the analogout peripheral
 *
 * @param     obj : obj The analogout object to initialize
 *
 * @param     pin : The analogout pin name
 *
 * @retval    None
 */
void analogout_init(dac_t *obj, PinName pin)
{
    uint32_t function;

    obj->dac = (DACName)pinmap_peripheral(pin,PinMap_DAC);
    MBED_ASSERT(obj->dac != (DACName)NC);
    
    function = pinmap_function(pin,PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = APM_PIN_CHANNEL_GET(function);
    MBED_ASSERT(obj->channel <= DAC_1);

    pinmap_pinout(pin, PinMap_DAC);

    obj->pin = pin;

    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);

    DAC_Config_T dacConfig;
    dacConfig.waveGeneration = DAC_WAVE_GENERATION_NONE;
    dacConfig.trigger = DAC_TRIGGER_NONE;
    dacConfig.maskAmplitudeSelect = DAC_LFSR_MASK_NONE;
    dacConfig.outputBuffer = DAC_OUTPUT_BUFFER_ENABLE;

    DAC_Config(obj->channel,&dacConfig);

}

 /*!
 * @brief     Release the analogout object
 *
 * @param     obj : The analogout object to initialize
 *
 * @retval    None
 */
void analogout_free(dac_t *obj)
{
    uint32_t function;

    DAC_Reset();
    RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_DAC);

    function = pinmap_function(obj->pin,PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

    pin_function(obj->pin,function);
}

 /*!
 * @brief     Release the analogout object
 *
 * @param     obj : The analogout object to initialize
 *
 * @param     val : The integer-point output voltage to be set
 *
 * @retval    None
 */
static inline void dac_data_config(dac_t *obj,uint16_t val)
{
    if (obj->channel == DAC_CHANNEL_1)
    {
        DAC_ConfigChannel1Data(DAC_ALIGN_12BIT_R,(val & 0xfff));
    }
    else if (obj->channel == DAC_CHANNEL_2)
    {
        DAC_ConfigChannel2Data(DAC_ALIGN_12BIT_R,(val & 0xfff));
    }

    DAC_Enable(obj->channel);
}

 /*!
 * @brief     Set the output voltage, specified as a percentage (float)
 *
 * @param     obj : The analogout object to initialize
 *
 * @param     value : The floating-point output voltage to be set
 *
 * @retval    None
 */
void analogout_write(dac_t *obj, float value)
{
    MBED_ASSERT(obj->channel != (uint32_t)NC);

    /* if value < 0,set DAC output data to 0 */
    if (value < 0.0f)
    {
        dac_data_config(obj,0);
    }
    /* if value > 1,set DAC output data to 0xfff */
    else if (value > 1.0f)
    {
        dac_data_config(obj,(uint16_t)0xfff);
    }
    else
    {
        dac_data_config(obj,(uint16_t)(value * (float)0xfff));
    }
}

 /*!
 * @brief     Set the output voltage, specified as unsigned 16-bit
 *
 * @param     obj : The analogout object
 *
 * @param     value : The unsigned 16-bit output voltage to be set
 *
 * @retval    None
 */
void analogout_write_u16(dac_t *obj, uint16_t value)
{
    dac_data_config(obj,value >> (16 - DAC_BITS));
}

 /*!
 * @brief     Set the output voltage, specified as unsigned 16-bit
 *
 * @param     obj : The analogout object
 *
 * @retval    A floating-point value representing the current voltage on the pin,measured as a percentage
 */
float analogout_read(dac_t *obj)
{
    uint16_t ret = DAC_ReadDataOutputValue(obj->channel);

    return (float)ret * ((1.0f) / (float)0xfff);
}

 /*!
 * @brief     Set the output voltage, specified as unsigned 16-bit
 *
 * @param     obj : The analogout object
 *
 * @retval    An unsigned 16-bit value representing the current voltage on the pin
 */
uint16_t analogout_read_u16(dac_t *obj)
{
    uint16_t ret = DAC_ReadDataOutputValue(obj->channel);
    return (uint16_t)((ret << 4) | ((ret >> 8) & 0x000F));
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif /* DEVICE_ANALOGOUT */
