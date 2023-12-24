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

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_error.h"

#define ADC_ACCURACY_12BIT             (0xFFF)
#define ADC_PRECISION_12TO16(value)    ((value << 4)| ((value >> 8) & (uint16_t)0x000F))

int temp_flag = 0;

static void delay(int ms)
{
    for (int i = 0; i < ms; i++){};
}

 /*!
 * @brief     Initialize the analogin peripheral
 *
 * @param     obj : The analogin object to initialize
 *
 * @param     pin : The analogin pin name
 *
 * @retval    None
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    uint32_t adc;
    uint32_t function;

    MBED_ASSERT(obj);

    obj->adc = pinmap_peripheral(pin,PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    function = pinmap_function(pin, PinMap_ADC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = APM_PIN_CHANNEL_GET(function);
    MBED_ASSERT(obj->channel <= ADC_CHANNEL_18);

    adc = obj->adc;
    obj->pin = pin;
    ADC_T *_adc= (ADC_T *)obj->adc;

    if (adc == ADC_1)
    {
        RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    }
    else if (adc == ADC_2)
    {
        RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC2);
        pin = (PinName)(pin & 0xff);
    }
    else if (adc == ADC_3)
    {
        RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC3);
        if ((pin > MUL1) | (pin == MUL1))
        {
            pin = (PinName)(pin & 0xff);
        }
    }

    ADC_CommonConfig_T  adcCommonConfig;
    ADC_CommonConfigStructInit(&adcCommonConfig);
    /*Set ADC Clock Prescaler. ADC_Clock = APB2_Clock/4, 84/4=21MHz*/
    adcCommonConfig.prescaler = ADC_PRESCALER_DIV4;
    ADC_CommonConfig(&adcCommonConfig);

    if (obj->channel == ADC_CHANNEL_16)
    {
        ADC_EnableTempSensorVrefint();
        temp_flag = 1;
    }
    if (obj->channel == ADC_CHANNEL_17)
    {
        ADC_EnableTempSensorVrefint();
        temp_flag = 0;
    }
    if (obj->channel == ADC_CHANNEL_18)
    {
        ADC_EnableVbat();
    }

    if (pin < ADC_TEMP)
    {
        pinmap_pinout(pin, PinMap_ADC);
    }

    ADC_Config_T adcConfig;
    ADC_ConfigStructInit(&adcConfig);
    adcConfig.resolution          = ADC_RESOLUTION_12BIT;
    adcConfig.continuousConvMode  = DISABLE;
    adcConfig.extTrigConv         = ADC_EXT_TRIG_CONV_TMR8_CC1;
    adcConfig.dataAlign           = ADC_DATA_ALIGN_RIGHT;
    adcConfig.extTrigEdge         = ADC_EXT_TRIG_EDGE_NONE;
    adcConfig.scanConvMode        = DISABLE;
    adcConfig.nbrOfChannel        = 1;
    ADC_Config(_adc, &adcConfig);

    if (temp_flag == 1)
    {
        ADC_ConfigRegularChannel(_adc, obj->channel, 1, ADC_SAMPLETIME_480CYCLES);
    }
    else
    {
        ADC_ConfigRegularChannel(_adc, obj->channel, 1, ADC_SAMPLETIME_15CYCLES);
    }

    ADC_Enable(_adc);
}

 /*!
 * @brief     Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param     obj : The analogin object
 *
 * @retval    An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t ret = 0;
    ADC_T *adc= (ADC_T *)obj->adc;

    ADC_ClearStatusFlag(adc,ADC_FLAG_EOC);
    ADC_SoftwareStartConv(adc);
    while (ADC_ReadStatusFlag(adc,ADC_FLAG_EOC) == RESET)
    {
    }
    delay(100);

    ret = ADC_ReadConversionValue(adc);

    return ret;
}

 /*!
 * @brief     Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param     obj : The analogin object
 *
 * @retval    A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    uint16_t ret;
    float voltage = 0;
    ADC_T *adc= (ADC_T *)obj->adc;

    ADC_ClearStatusFlag(adc,ADC_FLAG_EOC);

    ADC_SoftwareStartConv(adc);
    while (ADC_ReadStatusFlag(adc,ADC_FLAG_EOC) == RESET)
    {
    }
    delay(100);
    ret = ADC_ReadConversionValue(adc);
    voltage = (float)ret * (1.0f / (float)0xFFF);

    return voltage;
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif /* DEVICE_ANALOGIN */
