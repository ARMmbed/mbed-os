/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "gpio_include.h"

static const PinMap PinMap_ADC[] = {
    {PD0, ADC_AINA0,  PIN_DATA(0, 0)},
    {PD1, ADC_AINA1,  PIN_DATA(0, 0)},
    {PD2, ADC_AINA2,  PIN_DATA(0, 0)},
    {PD3, ADC_AINA3,  PIN_DATA(0, 0)},
    {PE0, ADC_AINA4,  PIN_DATA(0, 0)},
    {PE1, ADC_AINA5,  PIN_DATA(0, 0)},
    {PE2, ADC_AINA6,  PIN_DATA(0, 0)},
    {PE3, ADC_AINA7,  PIN_DATA(0, 0)},
    {PE4, ADC_AINA8,  PIN_DATA(0, 0)},
    {PE5, ADC_AINA9,  PIN_DATA(0, 0)},
    {PE6, ADC_AINA10, PIN_DATA(0, 0)},
    {PF0, ADC_AINA11, PIN_DATA(0, 0)},
    {PF1, ADC_AINA12, PIN_DATA(0, 0)},
    {PF2, ADC_AINA13, PIN_DATA(0, 0)},
    {PF3, ADC_AINA14, PIN_DATA(0, 0)},
    {PF4, ADC_AINA15, PIN_DATA(0, 0)},
    {NC,  NC,         0}
};

void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    obj->obj = TSB_ADA;
    TSB_CG_FSYSENB_IPENB00 = ENABLE;  // ADC CG Fsys Enable
    pinmap_pinout(pin, PinMap_ADC);   // Set pin function as ADC
    obj->obj->CLK = ADC_SCLK_1;       // Set sample hold time and prescale clock
    obj->obj->MOD0 = (ADxMOD0_RCUT_NORMAL | ADxMOD0_DACON_ON);
    TSB_CG_SPCLKEN_ADCKEN = ENABLE;   // ADC Clock Enable
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t ADCResultValue  = 0;
    uint32_t ADCResultStored = 0;

    wait_us(3U);  // Wait at least 3us to ensure the voltage is stable
    obj->obj->CR0   = (ADxCR0_ADEN_DISABLE | ADxCR0_CNT_DISABLE);               // Disable Conversion
    obj->obj->TSET0 = (ADxTSETn_ENINT_DISABLE | ADxTSETn_TRGS_SGL | obj->adc);  // Enable Conversion
    obj->obj->CR1   = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE |
                       ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);
    obj->obj->CR0   = (ADxCR0_ADEN_ENABLE | ADxCR0_SGL_ENABLE | ADxCR0_CNT_DISABLE);

    while ((obj->obj->ST & ADxST_SNGF_RUN) != ADxST_SNGF_IDLE) {
        // Wait until AD conversion complete
    }

    wait_us(1U); // Wait for register to update with convert value.
    ADCResultStored = obj->obj->REG0;  // Convert result

    if ((ADCResultStored & ADxREGn_ADRFn_MASK) == ADxREGn_ADRFn_ON) {
        ADCResultValue = (uint16_t)((ADCResultStored & ADxREGn_ADRn_MASK) >> 4);
    }

    return ADCResultValue;
}

float analogin_read(analogin_t *obj)
{
    float    result = 0.0;
    uint16_t value  = 0;

    value = analogin_read_u16(obj);
    result = ((float)value * (1.0f / (float)ADC_12BIT_RANGE));

    return result;
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
