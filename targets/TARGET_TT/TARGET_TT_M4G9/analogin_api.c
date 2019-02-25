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
#include "mbed_error.h"
#include "adc_include.h"

#define ADC_12BIT_RANGE   0xFFF
#define CONVERSION_FLAG   0x4

static const PinMap PinMap_ADC[] = {
    {PN0, ADC_A0, PIN_DATA(0, 0)},
    {PN1, ADC_A1, PIN_DATA(0, 0)},
    {PN2, ADC_A2, PIN_DATA(0, 0)},
    {PN3, ADC_A3, PIN_DATA(0, 0)},
    {PN4, ADC_A4, PIN_DATA(0, 0)},
    {PN5, ADC_A5, PIN_DATA(0, 0)},
    {PN6, ADC_A6, PIN_DATA(0, 0)},
    {PN7, ADC_A7, PIN_DATA(0, 0)},
    {PP0, ADC_A8, PIN_DATA(0, 0)},
    {PP1, ADC_A9, PIN_DATA(0, 0)},
    {PP2, ADC_A10, PIN_DATA(0, 0)},
    {PP3, ADC_A11, PIN_DATA(0, 0)},
    {PP4, ADC_A12, PIN_DATA(0, 0)},
    {PP5, ADC_A13, PIN_DATA(0, 0)},
    {PP6, ADC_A14, PIN_DATA(0, 0)},
    {PP7, ADC_A15, PIN_DATA(0, 0)},
    {PR0, ADC_A16, PIN_DATA(0, 0)},
    {PR1, ADC_A17, PIN_DATA(0, 0)},
    {PR2, ADC_A18, PIN_DATA(0, 0)},
    {PR3, ADC_A19, PIN_DATA(0, 0)},
    {PR4, ADC_A20, PIN_DATA(0, 0)},
    {PR5, ADC_A21, PIN_DATA(0, 0)},
    {PR6, ADC_A22, PIN_DATA(0, 0)},
    {PR7, ADC_A23, PIN_DATA(0, 0)},
    {NC,  NC, 0}
};

void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Enable ADC clock supply
    TSB_CG_FSYSMENA_IPMENA03 = TXZ_ENABLE;
    TSB_CG_SPCLKEN_ADCKEN = TXZ_ENABLE;
    TSB_CG_SPCLKEN_TRCKEN = TXZ_ENABLE;

    // Enable clock for GPIO
    if(obj->adc <= ADC_A7) {
        TSB_CG_FSYSMENB_IPMENB14 = TXZ_ENABLE;
    } else if(obj->adc <= ADC_A15) {
        TSB_CG_FSYSMENB_IPMENB15 = TXZ_ENABLE;
    } else {
        TSB_CG_FSYSMENB_IPMENB16 = TXZ_ENABLE;
    }

    // Set pin function as ADC
    pinmap_pinout(pin, PinMap_ADC);

    // Initialize
    obj->p_adc.p_instance       = TSB_ADA;
    obj->p_adc.init.clk.exaz0   = ADC_SAMPLING_PERIOD0_XN;
    obj->p_adc.init.clk.exaz1   = ADC_SAMPLING_PERIOD1_XN;
    obj->p_adc.init.clk.vadcld  = ADC_SCLK_1;
    obj->p_adc.init.mod1        = ADC_MOD1_SCLK_3;
    obj->p_adc.init.mod2        = ADC_MOD2_TMPM4G9;
    obj->p_adc.handler.single       = NULL;
    obj->p_adc.handler.continuity   = NULL;
    obj->p_adc.handler.trigger      = NULL;
    obj->p_adc.handler.highpriority = NULL;

    if (adc_init(&obj->p_adc) != TXZ_SUCCESS) {
        error("Failed : ADC Initialization");
    }

    // ADC channel setting
    obj->param.interrupt = ADC_INT_DISABLE;
    obj->param.type = ADC_CONVERSION_SGL;
    obj->param.ain = obj->adc;

    if (adc_channel_setting(&obj->p_adc, obj->param.ain, &obj->param) != TXZ_SUCCESS) {
        error("Failed : ADC channel setting");
    }
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t adc_result = 0;

    // Assert that ADC channel is valid
    MBED_ASSERT(obj->adc != (ADCName)NC);

    if (adc_start(&obj->p_adc) == TXZ_SUCCESS) {
        // adc started
    }

    // wait for Continuous conversion program flag clear.
    while((obj->p_adc.p_instance->ST & CONVERSION_FLAG)) {
        // Do nothing...
    }

    if (adc_channel_get_value(&obj->p_adc, obj->param.ain, &adc_result) != TXZ_SUCCESS) {
        error("Failed : To read ADC converted result");
    }

    if (adc_stop(&obj->p_adc) != TXZ_SUCCESS) {
        error("Failed : To Stop ADC Conversion");
    }
    return (uint16_t)adc_result;
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
