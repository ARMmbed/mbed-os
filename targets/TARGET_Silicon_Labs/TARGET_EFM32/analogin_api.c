/***************************************************************************//**
 * @file analogin_api.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_ANALOGIN

#include "mbed_assert.h"
#include "analogin_api.h"

#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"

#include "em_adc.h"
#include "em_cmu.h"

void analogin_init(analogin_t *obj, PinName pin)
{
    static uint8_t adc_initialized = 0;

    /* Init structure */
    obj->adc = (ADC_TypeDef *) pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT((unsigned int) obj->adc != NC);

    obj->channel = pin_location(pin, PinMap_ADC);
    MBED_ASSERT((unsigned int) obj->channel != NC);

    /* Only initialize the ADC once */
    if (!adc_initialized) {
        /* Turn on the clock */
        CMU_ClockEnable(cmuClock_ADC0, true);

        /* Init with default settings */
        ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
        init.prescale = 4;
        ADC_Init(obj->adc, &init);

        /* Init for single conversion use */
        ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

        /* Measure input channel with Vdd reference. */
        singleInit.reference = adcRefVDD;
        singleInit.resolution = adcRes12Bit;
        singleInit.acqTime = adcAcqTime32;

        ADC_InitSingle(obj->adc, &singleInit);

        adc_initialized = 1;
    }
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_TypeDef *adc = obj->adc;
    uint16_t sample = 0;

    //Make sure a single conversion is not in progress
    adc->CMD = ADC_CMD_SINGLESTOP;

    // Make sure we are checking the correct channel
#if defined _ADC_SINGLECTRL_INPUTSEL_MASK
    adc->SINGLECTRL = (adc->SINGLECTRL & ~_ADC_SINGLECTRL_INPUTSEL_MASK) | obj->channel;
#elif _ADC_SINGLECTRL_POSSEL_MASK
    adc->SINGLECTRL = (adc->SINGLECTRL & ~_ADC_SINGLECTRL_POSSEL_MASK) | obj->channel << _ADC_SINGLECTRL_POSSEL_SHIFT;
#endif

    ADC_Start(adc, adcStartSingle);

    /* Wait while conversion is active */
    while (adc->STATUS & ADC_STATUS_SINGLEACT);

    /* Get ADC result */
    sample = ADC_DataSingleGet(adc);

    /* The ADC has 12 bit resolution. We shift in 4 0s */
    /* from the right to make it a 16 bit number as expected */
    return sample << 4;
}

float analogin_read(analogin_t *obj)
{
    /* Convert from a uint16 to a float between 0 and 1 by division by 0xFFF0 */
    return analogin_read_u16(obj) / (float) 0xFFF0;
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
