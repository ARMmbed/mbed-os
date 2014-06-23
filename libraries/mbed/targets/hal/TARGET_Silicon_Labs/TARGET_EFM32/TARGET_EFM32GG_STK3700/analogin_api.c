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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "em_adc.h"
#include "em_cmu.h"


void analogin_init(analogin_t *obj, PinName pin) {
    obj->adc_input = (ADCName) pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc_input != (ADCName)NC);
    
    /* Enable required clocks */
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_ADC0, true);

    /* Init with default settings */
    ADC_Init_TypeDef       init       = ADC_INIT_DEFAULT;
    ADC_Init(ADC0, &init);

    ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

    /* Init for single conversion use, measure input channel with Vdd reference. */
    singleInit.reference  = adcRefVDD;
    singleInit.input      = (ADC_SingleInput_TypeDef) obj->adc_input;
    singleInit.resolution = adcRes12Bit;

    singleInit.acqTime = adcAcqTime32;

    ADC_InitSingle(ADC0, &singleInit);
}

uint16_t analogin_read_u16(analogin_t *obj) {
    uint16_t sample = 0;
    //Make sure a single conversion is not in progress
    ADC0->CMD = ADC_CMD_SINGLESTOP;

    // Make sure we are checking the correct channel 
    ADC0->SINGLECTRL |= (obj->adc_input << _ADC_SINGLECTRL_INPUTSEL_SHIFT);
    ADC0->SINGLECTRL &= ~(~obj->adc_input << _ADC_SINGLECTRL_INPUTSEL_SHIFT);

    ADC_Start(ADC0, adcStartSingle);

    /* Wait while conversion is active */
    while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;

    /* Get ADC result */
    sample = ADC_DataSingleGet(ADC0);

    /* The ADC has 12 bit resolution. We shift in 4 0s */
    /* from the right to make it a 16 bit number as expected */
    return sample << 4;
}

float analogin_read(analogin_t *obj) {
    /* Convert from a uint16 to a float between 0 and 1 by division by 0xFFFF */
    return analogin_read_u16(obj)/(float)0xFFFF;
}

