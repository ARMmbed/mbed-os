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
#define ADC_10BIT_RANGE        0x3FF

static const PinMap PinMap_ADC[] = {
    {PA0, ADC_A0, PIN_DATA(0, 0)},
    {PA1, ADC_A1, PIN_DATA(0, 0)},
    {PA2, ADC_A2, PIN_DATA(0, 0)},
    {PA3, ADC_A3, PIN_DATA(0, 0)},
    {PA4, ADC_A4, PIN_DATA(0, 0)},
    {PA5, ADC_A5, PIN_DATA(0, 0)},
    {PA6, ADC_A6, PIN_DATA(0, 0)},
    {PA7, ADC_A7, PIN_DATA(0, 0)},
    {NC,  NC,     0}
};

void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    // enable clock supply to ADC
    CG_SetFcPeriphA(CG_FC_PERIPH_ADC, ENABLE);
    CG_SetADCClkSupply(ENABLE);
    // Set pin function as ADC
    pinmap_pinout(pin, PinMap_ADC);
    if (!adc_reset_init) {
        // Software reset ADC
        ADC_SWReset();
        adc_reset_init = 1;
    }
    // Set sample hold time and pre-scale clock
    ADC_SetClk(ADC_CONVERSION_81_CLOCK, ADC_FC_DIVIDE_LEVEL_8);
    // Set input channel
    ADC_SetInputChannel(obj->adc);
    // Turn VREF on
    ADC_SetVref(ENABLE);
    // Use fixed-channel single conversion mode
    ADC_SetRepeatMode(DISABLE);
    ADC_SetScanMode(DISABLE);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_ResultTypeDef ret;

    // Assert that ADC channel is valid
    MBED_ASSERT(obj->adc != (ADCName) NC);

    // Set input channel
    ADC_SetInputChannel(obj->adc);
    // Enable Vref
    ADC_SetVref(ENABLE);
    // Wait at least 3us to ensure the voltage is stable
    wait_us(10U);
    // Start ADC conversion
    ADC_Start();
    // Wait until AD conversion complete
    while (ADC_GetConvertState().Bit.NormalComplete != 1) {
        // Do nothing
    }
    // Convert result
    ret = ADC_GetConvertResult(obj->adc);
    // Disable Vref to go into standby mode
    ADC_SetVref(DISABLE);
    return (uint16_t)ret.ADCResultValue;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)(value * (1.0f / (float)ADC_10BIT_RANGE));
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
