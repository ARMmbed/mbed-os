/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "mbed_wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "W7500x_adc.h"
#include "PeripheralPins.h"


ADC_TypeDef * AdcHandle;

static int adc_inited = 0;

void analogin_init(analogin_t *obj, PinName pin)
{
    // Get the peripheral name from the pin and assign it to the object
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);

    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Configure GPIO
    pinmap_pinout(pin, PinMap_ADC);

    // Save pin number for the read function
    obj->pin = pin;

    // The ADC initialization is done once
    if (adc_inited == 0) {
        adc_inited = 1;

        ADC_Init();
    }
}

static inline uint16_t adc_read(analogin_t *obj)
{
    ADC_CH adc_ch;

    switch (obj->pin) {
        case PC_15:
            adc_ch = ADC_CH0;
            break;
        case PC_14:
            adc_ch = ADC_CH1;
            break;
        case PC_13:
            adc_ch = ADC_CH2;
            break;
        case PC_12:
            adc_ch = ADC_CH3;
            break;
        case PC_11:
            adc_ch = ADC_CH4;
            break;
        case PC_10:
            adc_ch = ADC_CH5;
            break;
        default:
            return 0;
    }

    ADC_ChannelSelect(adc_ch);
    ADC_Start();

    // Wait end of conversion and get value
    while(ADC_IsInterrupt());
    return ADC_ReadData();
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    // 12-bit to 16-bit conversion
    return value;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = adc_read(obj);

    return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}

#endif
