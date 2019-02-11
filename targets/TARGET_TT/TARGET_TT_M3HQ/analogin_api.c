/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
    {PD0, ADC_AINA00, PIN_DATA(0, 0)},
    {PD1, ADC_AINA01, PIN_DATA(0, 0)},
    {PD2, ADC_AINA02, PIN_DATA(0, 0)},
    {PD3, ADC_AINA03, PIN_DATA(0, 0)},
    {PE0, ADC_AINA04, PIN_DATA(0, 0)},
    {PE1, ADC_AINA05, PIN_DATA(0, 0)},
    {PE2, ADC_AINA06, PIN_DATA(0, 0)},
    {PE3, ADC_AINA07, PIN_DATA(0, 0)},
    {PE4, ADC_AINA08, PIN_DATA(0, 0)},
    {PE5, ADC_AINA09, PIN_DATA(0, 0)},
    {PE6, ADC_AINA10, PIN_DATA(0, 0)},
    {PF0, ADC_AINA11, PIN_DATA(0, 0)},
    {PF1, ADC_AINA12, PIN_DATA(0, 0)},
    {PF2, ADC_AINA13, PIN_DATA(0, 0)},
    {PF3, ADC_AINA14, PIN_DATA(0, 0)},
    {PF4, ADC_AINA15, PIN_DATA(0, 0)},
    {PF5, ADC_AINA16, PIN_DATA(0, 0)},
    {PF6, ADC_AINA17, PIN_DATA(0, 0)},
    {PF7, ADC_AINA18, PIN_DATA(0, 0)},
    {PD4, ADC_AINA19, PIN_DATA(0, 0)},
    {PD5, ADC_AINA20, PIN_DATA(0, 0)},
    {NC,  NC,     0}
};

/**
  * [analogin_init]
  * @param obj
  * @param pin
  * @description Initialize analog input
  */
void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    // Assert that ADC channel is valid
    MBED_ASSERT(obj->adc != (ADCName)NC);
    obj->obj = TSB_ADA;
    // ADC CG Fsys Enable
    TSB_CG_FSYSENB_IPENB15 = ENABLE;
    // ADC Clock Enable
    TSB_CG_SPCLKEN_ADCKEN = ENABLE;
    // Set pin function as ADC
    pinmap_pinout(pin, PinMap_ADC);
    // Set sample hold time and pre-scale clock
    obj->obj->CLK = (ADC_SCLK_1 | ADC_SAMPLING_PERIOD_3V);
    obj->obj->MOD0 = (ADxMOD0_RCUT_NORMAL | ADxMOD0_DACON_ON);
    obj->obj->MOD1 = ADC_MOD1_AVDD5_3V;
    obj->obj->MOD2 = ADC_MOD2_TMPM3Hx;
}

/**
  * [analogin_read_u16]
  * @param  obj
  * @return An unsigned short in the range [0x00, 0x0FFF]
  * @description Read the ADC input
  */
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t ADCResultValue = 0;
    uint32_t ADCResultStored = 0;

    // Wait at least 3us to ensure the voltage is stable
    wait_us(300);
    // ADC configuration for data Conversion
    obj->obj->CR0 = (ADxCR0_ADEN_DISABLE | ADxCR0_CNT_DISABLE);
    obj->obj->TSET0 = (ADxTSETn_ENINT_DISABLE | ADxTSETn_TRGS_SGL | obj->adc);
    obj->obj->CR1   = (ADxCR1_CNTDMEN_DISABLE | ADxCR1_SGLDMEN_DISABLE
                       | ADxCR1_TRGDMEN_DISABLE | ADxCR1_TRGEN_DISABLE);
    obj->obj->CR0   = (ADxCR0_ADEN_ENABLE | ADxCR0_SGL_ENABLE | ADxCR0_CNT_DISABLE);
    // Wait until AD conversion complete
    while( (obj->obj->ST & ADxST_SNGF_RUN) != ADxST_SNGF_IDLE);
    // Wait for register to update with convert value
    wait_us(30);
    // Convert result
    ADCResultStored = (obj->obj->REG0 | obj->adc);
    if ((ADCResultStored & ADxREGn_ADRFn_MASK) == ADxREGn_ADRFn_ON) {
        ADCResultValue = (uint16_t)((ADCResultStored & ADxREGn_ADRn_MASK) >> 4);
    }
    return ADCResultValue;
}

/**
  * [analogin_read]
  * @param  obj
  * @return A float in the range [0.0, 1.0]
  * @description Read the ADC input
  */
float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return ((float)(value * (1.0f / (float)ADC_12BIT_RANGE)));
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
