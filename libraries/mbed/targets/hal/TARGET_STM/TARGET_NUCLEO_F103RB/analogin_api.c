/* mbed Microcontroller Library
 * Copyright (c) 2014, STMicroelectronics
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
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
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
 */
#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "wait_api.h"
#include "PeripheralPins.h"

int adc_inited = 0;

void analogin_init(analogin_t *obj, PinName pin)
{
    ADC_TypeDef *adc;
    ADC_InitTypeDef ADC_InitStructure;

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

        // Get ADC registers structure address
        adc = (ADC_TypeDef *)(obj->adc);

        // Enable ADC clock (14 MHz maximum)
        // PCLK2 = 64 MHz --> ADC clock = 64/6 = 10.666 MHz
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        // Configure ADC
        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(adc, &ADC_InitStructure);

        // Enable ADC
        ADC_Cmd(adc, ENABLE);

        // Calibrate ADC
        ADC_ResetCalibration(adc);
        while (ADC_GetResetCalibrationStatus(adc));
        ADC_StartCalibration(adc);
        while (ADC_GetCalibrationStatus(adc));
    }
}

static inline uint16_t adc_read(analogin_t *obj)
{
    // Get ADC registers structure address
    ADC_TypeDef *adc = (ADC_TypeDef *)(obj->adc);
    int channel = 0;

    // Configure ADC channel
    switch (obj->pin) {
        case PA_0:
            channel = 0;
            break;
        case PA_1:
            channel = 1;
            break;
        case PA_2:
            channel = 2;
            break;
        case PA_3:
            channel = 3;
            break;
        case PA_4:
            channel = 4;
            break;
        case PA_5:
            channel = 5;
            break;
        case PA_6:
            channel = 6;
            break;
        case PA_7:
            channel = 7;
            break;
        case PB_0:
            channel = 8;
            break;
        case PB_1:
            channel = 9;
            break;
        case PC_0:
            channel = 10;
            break;
        case PC_1:
            channel = 11;
            break;
        case PC_2:
            channel = 12;
            break;
        case PC_3:
            channel = 13;
            break;
        case PC_4:
            channel = 14;
            break;
        case PC_5:
            channel = 15;
            break;
        default:
            return 0;
    }

    ADC_RegularChannelConfig(adc, channel, 1, ADC_SampleTime_7Cycles5);

    ADC_SoftwareStartConvCmd(adc, ENABLE); // Start conversion

    while (ADC_GetFlagStatus(adc, ADC_FLAG_EOC) == RESET); // Wait end of conversion

    return (ADC_GetConversionValue(adc)); // Get conversion value
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    // 12-bit to 16-bit conversion
    value = ((value << 4) & (uint16_t)0xFFF0) | ((value >> 8) & (uint16_t)0x000F);
    return value;
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}

#endif
