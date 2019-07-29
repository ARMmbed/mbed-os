/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "analogin_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "mbed_wait_api.h"

static uint8_t adc_reset_init = 0;  // Is ADC Reset happened yet?
#define ADC_12BIT_RANGE   0xFFF

static const PinMap PinMap_ADC[] = {
    {PJ0, ADC_A0,  PIN_DATA(2, 0)},
    {PJ1, ADC_A1,  PIN_DATA(2, 0)},
    {PJ2, ADC_A2,  PIN_DATA(2, 0)},
    {PJ3, ADC_A3,  PIN_DATA(2, 0)},
    {PJ4, ADC_A4,  PIN_DATA(2, 0)},
    {PJ5, ADC_A5,  PIN_DATA(2, 0)},
    {PJ6, ADC_A6,  PIN_DATA(2, 0)},
    {PJ7, ADC_A7,  PIN_DATA(2, 0)},
    {NC,  NC,     0}
};

void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Enable clock supply
    CG_SetADCClkSupply(ENABLE);
    // Set pin function as ADC
    pinmap_pinout(pin, PinMap_ADC);
    if (!adc_reset_init) {
        // Software reset ADC
        ADC_SWReset(TSB_AD);
        adc_reset_init = 1;
    }
    // Set sample hold time and pre-scale clock
    ADC_SetClk(TSB_AD, ADC_CONVERSION_CLK_80, ADC_FC_DIVIDE_LEVEL_8);
    // Set input channel
    ADC_SetInputChannel(TSB_AD, (ADC_AINx)obj->adc);
    // Turn VREF on
    ADC_SetVref(TSB_AD, ENABLE);
    // Use fixed-channel single conversion mode
    ADC_SetRepeatMode(TSB_AD, DISABLE);
    ADC_SetScanMode(TSB_AD, DISABLE);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_Result ret = {0x00};

    // Assert that ADC channel is valid
    MBED_ASSERT(obj->adc != (ADCName)NC);
    wait_us(300); // Wait for register to update with convert value

    // Set input channel
    ADC_SetInputChannel(TSB_AD, (ADC_AINx)obj->adc);
    // Enable Vref
    ADC_SetVref(TSB_AD, ENABLE);
    // Wait at least 3us to ensure the voltage is stable
    wait_us(10U);
    // Start ADC conversion
    ADC_Start(TSB_AD);
    // Wait until AD conversion complete
    while (ADC_GetConvertState(TSB_AD).Bit.NormalComplete != 1) {
        // Do nothing
    }
    wait_us(30);
    // Convert result
    ret = ADC_GetConvertResult(TSB_AD, (ADC_REGx)obj->adc);
    // Disable Vref to go into standby mode
    ADC_SetVref(TSB_AD, DISABLE);
    return (uint16_t)ret.Bit.ADResult;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)(value * (1.0f / (float)ADC_12BIT_RANGE));
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
