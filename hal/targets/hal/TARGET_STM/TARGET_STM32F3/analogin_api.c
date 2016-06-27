/* mbed Microcontroller Library
 * Copyright (c) 2015, STMicroelectronics
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

#include "wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

ADC_HandleTypeDef AdcHandle;

void analogin_init(analogin_t *obj, PinName pin)
{
#if defined(ADC1)
    static int adc1_inited = 0;
#endif
#if defined(ADC2)
    static int adc2_inited = 0;
#endif
#if defined(ADC3)
    static int adc3_inited = 0;
#endif
#if defined(ADC4)
    static int adc4_inited = 0;
#endif

    // Get the peripheral name from the pin and assign it to the object
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    // Get the pin function and assign the used channel to the object
    uint32_t function = pinmap_function(pin, PinMap_ADC);
    MBED_ASSERT(function != (uint32_t)NC);
    obj->channel = STM_PIN_CHANNEL(function);

    // Configure GPIO
    pinmap_pinout(pin, PinMap_ADC);

    // Save pin number for the read function
    obj->pin = pin;

    // Check if ADC is already initialized
    // Enable ADC clock
#if defined(ADC1)
    if ((obj->adc == ADC_1) && adc1_inited) return;
    if (obj->adc == ADC_1) {
        AdcHandle.State = HAL_ADC_STATE_RESET;
        __ADC1_CLK_ENABLE();
        adc1_inited = 1;
    }
#endif
#if defined(ADC2)
    if ((obj->adc == ADC_2) && adc2_inited) return;
    if (obj->adc == ADC_2) {
        AdcHandle.State = HAL_ADC_STATE_RESET;
        __ADC2_CLK_ENABLE();
        adc2_inited = 1;
    }
#endif
#if defined(ADC3)
    if ((obj->adc == ADC_3) && adc3_inited) return;
    if (obj->adc == ADC_3) {
        AdcHandle.State = HAL_ADC_STATE_RESET;
        __ADC34_CLK_ENABLE();
        adc3_inited = 1;
    }
#endif
#if defined(ADC4)
    if ((obj->adc == ADC_4) && adc4_inited) return;
    if (obj->adc == ADC_4) {
        AdcHandle.State = HAL_ADC_STATE_RESET;
        __ADC34_CLK_ENABLE();
        adc4_inited = 1;
    }
#endif

    // Configure ADC
    AdcHandle.Instance = (ADC_TypeDef *)(obj->adc);
    AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;
    AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;
    AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    AdcHandle.Init.ScanConvMode          = DISABLE;
    AdcHandle.Init.EOCSelection          = EOC_SINGLE_CONV;
    AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    AdcHandle.Init.ContinuousConvMode    = DISABLE;
    AdcHandle.Init.NbrOfConversion       = 1;
    AdcHandle.Init.DiscontinuousConvMode = DISABLE;
    AdcHandle.Init.NbrOfDiscConversion   = 0;
    AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    AdcHandle.Init.DMAContinuousRequests = DISABLE;
    AdcHandle.Init.Overrun               = OVR_DATA_OVERWRITTEN;

    if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
        error("Cannot initialize ADC");
    }
}

static inline uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    AdcHandle.Instance = (ADC_TypeDef *)(obj->adc);

    // Configure ADC channel
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;

    switch (obj->channel) {
        case 1:
            sConfig.Channel = ADC_CHANNEL_1;
            break;
        case 2:
            sConfig.Channel = ADC_CHANNEL_2;
            break;
        case 3:
            sConfig.Channel = ADC_CHANNEL_3;
            break;
        case 4:
            sConfig.Channel = ADC_CHANNEL_4;
            break;
        case 5:
            sConfig.Channel = ADC_CHANNEL_5;
            break;
        case 6:
            sConfig.Channel = ADC_CHANNEL_6;
            break;
        case 7:
            sConfig.Channel = ADC_CHANNEL_7;
            break;
        case 8:
            sConfig.Channel = ADC_CHANNEL_8;
            break;
        case 9:
            sConfig.Channel = ADC_CHANNEL_9;
            break;
        case 10:
            sConfig.Channel = ADC_CHANNEL_10;
            break;
        case 11:
            sConfig.Channel = ADC_CHANNEL_11;
            break;
        case 12:
            sConfig.Channel = ADC_CHANNEL_12;
            break;
        case 13:
            sConfig.Channel = ADC_CHANNEL_13;
            break;
        case 14:
            sConfig.Channel = ADC_CHANNEL_14;
            break;
        case 15:
            sConfig.Channel = ADC_CHANNEL_15;
            break;
        case 16:
            sConfig.Channel = ADC_CHANNEL_16;
            break;
        case 17:
            sConfig.Channel = ADC_CHANNEL_17;
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_18;
            break;
        default:
            return 0;
    }

    HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

    HAL_ADC_Start(&AdcHandle); // Start conversion

    // Wait end of conversion and get value
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) == HAL_OK) {
        return (HAL_ADC_GetValue(&AdcHandle));
    } else {
        return 0;
    }
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
