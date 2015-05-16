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

#include "device.h"
#if DEVICE_ANALOGIN

#include "mbed_assert.h"
#include "analogin_api.h"

#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"

#include "em_adc.h"
#include "em_cmu.h"

uint8_t analogin_get_index(analogin_t *obj)
{
    return 0; //only one module availalbe
}

void analogin_preinit(analogin_t *obj, PinName pin)
{
    obj->adc = (ADC_TypeDef *) pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT((int) obj->adc != NC);

    obj->channel = pin_location(pin, PinMap_ADC);
    MBED_ASSERT((int) obj->channel != NC);
}

void analogin_init(analogin_t *obj, PinName pin)
{
    // TODO_LP only once - module in C++ ?
    /* Init with default settings */
    ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
    ADC_Init(obj->adc, &init);

    ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;

    /* Init for single conversion use, measure input channel with Vdd reference. */
    singleInit.reference = adcRefVDD;
    singleInit.resolution = adcRes12Bit;
    singleInit.acqTime = adcAcqTime32;

    ADC_InitSingle(obj->adc, &singleInit);

    /* Init pins */
    analogin_preinit(obj, pin);
}

void analogin_enable(analogin_t *obj, uint8_t enable)
{
    //not avail for EFM32
}

void analogin_enable_pins(analogin_t *obj, uint8_t enable)
{
    //not avail for EFM32
}

void analogin_enable_interrupt(analogin_t *obj, uint32_t address, uint8_t enable)
{
    NVIC_SetVector(ADC0_IRQn, address);
    if (enable) {
        // enable end of conversion interrupt
        ADC_IntEnable(obj->adc, ADC_IEN_SCAN);
        ADC_IntSet(obj->adc, ADC_IF_SCAN);
        NVIC_EnableIRQ(ADC0_IRQn);
    } else {
        ADC_IntDisable(obj->adc, ADC_IEN_SCAN);
        ADC_IntClear(obj->adc, ADC_IF_SCAN);
        NVIC_DisableIRQ(ADC0_IRQn);
    }
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    ADC_TypeDef *adc = obj->adc;
    uint16_t sample = 0;

    //Make sure a single conversion is not in progress
    adc->CMD = ADC_CMD_SINGLESTOP;

    // Make sure we are checking the correct channel
    adc->SINGLECTRL = (adc->SINGLECTRL & ~_ADC_SINGLECTRL_INPUTSEL_MASK) | obj->channel;

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
    /* Convert from a uint16 to a float between 0 and 1 by division by 0xFFFF */
    return analogin_read_u16(obj) / (float) 0xFFFF;
}

#endif
