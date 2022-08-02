/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#include "analogin_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "mbed_wait_api.h"
#include "mbed_error.h"
#include "txzp_adc_include.h"

#define ADC_12BIT_RANGE                 0xFFF
#define CONVERSION_FLAG                 0x4
#define SINGLE_CONVERSION               0xF

static const PinMap PinMap_ADC[] = {
    {PM2, ADC_A5,  PIN_DATA(0, 0)},
    {PM1, ADC_A6,  PIN_DATA(0, 0)},
    {PM0, ADC_A7,  PIN_DATA(0, 0)},
    {PL7, ADC_A8,  PIN_DATA(0, 0)},
    {PL6, ADC_A9,  PIN_DATA(0, 0)},
    {PL5, ADC_A13,  PIN_DATA(0, 0)},
    {PL3, ADC_A14,  PIN_DATA(0, 0)},
    {PL1, ADC_A15,  PIN_DATA(0, 0)},
    {PL0, ADC_A16,  PIN_DATA(0, 0)},
    {PL2, ADC_A17,  PIN_DATA(0, 0)},
    {PL4, ADC_A18, PIN_DATA(0, 0)},
    {NC,  NC,      0}
};

void analogin_init(analogin_t *obj, PinName pin)
{
    // Check that pin belong to ADC module
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);

    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Enable ADC clock supply
    TSB_CG_FSYSMENB_IPMENB02 = TXZ_ENABLE;
    TSB_CG_SPCLKEN_ADCKEN0  = TXZ_ENABLE;
    TSB_CG_SPCLKEN_TRCKEN  = TXZ_ENABLE;

    // Enable clock for GPIO
    if (obj->adc <= ADC_A7) {
        // Enable clock for GPIO port M
        TSB_CG_FSYSMENA_IPMENA11 = TXZ_ENABLE;
    } else if (obj->adc <= ADC_A18) {
        // Enable clock for GPIO port L
        TSB_CG_FSYSMENA_IPMENA10 = TXZ_ENABLE;
    } else {
        // Nothing when its NC
    }

    // Set pin function as ADC
    pinmap_pinout(pin, PinMap_ADC);

    // Initialize
    obj->p_adc.p_instance = TSB_ADA;
    obj->p_adc.init.id = SINGLE_CONVERSION;
    obj->p_adc.init.convert_time  = ADC_CONVERT_TIME_0_96_AVDD_4_5;
    obj->p_adc.init.rcut = ADC_RCUT_NORMAL;
    obj->p_adc.handler = NULL;
    obj->pin = pin;

    if (adc_init(&obj->p_adc) != TXZ_SUCCESS) {
        error("Failed : ADC Initialization");
    }

    // ADC channel setting
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

    // Wait until AD conversion complete
    while ((obj->p_adc.p_instance->ST & CONVERSION_FLAG)) {
        // Do nothing
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
    uint16_t value = 0;

    value = analogin_read_u16(obj);

    return (float)(value * (1.0f / (float)ADC_12BIT_RANGE));
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}
