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

#include "mbed_wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

void analogin_init(analogin_t *obj, PinName pin)
{
    uint32_t function = (uint32_t)NC;

    // ADC Internal Channels "pins"  (Temperature, Vref, Vbat, ...)
    //   are described in PinNames.h and PeripheralPins.c
    //   Pin value must be between 0xF0 and 0xFF
    if ((pin < 0xF0) || (pin >= 0x100)) {
        // Normal channels
        // Get the peripheral name from the pin and assign it to the object
        obj->handle.Instance = (ADC_TypeDef *)pinmap_peripheral(pin, PinMap_ADC);
        // Get the functions (adc channel) from the pin and assign it to the object
        function = pinmap_function(pin, PinMap_ADC);
        // Configure GPIO
        pinmap_pinout(pin, PinMap_ADC);
    } else {
        // Internal channels
        obj->handle.Instance = (ADC_TypeDef *)pinmap_peripheral(pin, PinMap_ADC_Internal);
        function = pinmap_function(pin, PinMap_ADC_Internal);
        // No GPIO configuration for internal channels
    }
    MBED_ASSERT(obj->handle.Instance != (ADC_TypeDef *)NC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = STM_PIN_CHANNEL(function);

    // Save pin number for the read function
    obj->pin = pin;

    // Configure ADC object structures
    obj->handle.State = HAL_ADC_STATE_RESET;
    obj->handle.Init.OversamplingMode      = DISABLE;
    obj->handle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
    obj->handle.Init.Resolution            = ADC_RESOLUTION_12B;
    obj->handle.Init.SamplingTime          = ADC_SAMPLETIME_160CYCLES_5;
    obj->handle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
    obj->handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ContinuousConvMode    = DISABLE;
    obj->handle.Init.DiscontinuousConvMode = DISABLE;
    obj->handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIG_EDGE_NONE;
    obj->handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIG0_T6_TRGO; // Not used here
    obj->handle.Init.DMAContinuousRequests = DISABLE;
    obj->handle.Init.EOCSelection          = EOC_SINGLE_CONV;
    obj->handle.Init.Overrun               = OVR_DATA_OVERWRITTEN;
    obj->handle.Init.LowPowerAutoWait      = ENABLE;
    obj->handle.Init.LowPowerFrequencyMode = DISABLE; // To be enabled only if ADC clock < 2.8 MHz
    obj->handle.Init.LowPowerAutoPowerOff  = DISABLE;

    __HAL_RCC_ADC1_CLK_ENABLE();

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC");
    }

    if (!HAL_ADCEx_Calibration_GetValue(&obj->handle, ADC_SINGLE_ENDED)) {
        HAL_ADCEx_Calibration_Start(&obj->handle, ADC_SINGLE_ENDED);
    }

    __HAL_ADC_ENABLE(&obj->handle);
}

uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC channel
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    switch (obj->channel) {
        case 0:
            sConfig.Channel = ADC_CHANNEL_0;
            break;
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
#ifdef ADC_CHANNEL_VLCD
        case 16:
            sConfig.Channel = ADC_CHANNEL_VLCD;
            break;
#endif
        case 17:
            sConfig.Channel = ADC_CHANNEL_VREFINT;
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
            break;
        default:
            return 0;
    }

    /* Reset ADC channel selection register */
    ADC1->CHSELR = 0;

    HAL_ADC_ConfigChannel(&obj->handle, &sConfig);

    HAL_ADC_Start(&obj->handle); // Start conversion

    // Wait end of conversion and get value
    if (HAL_ADC_PollForConversion(&obj->handle, 10) == HAL_OK) {
        return (uint16_t)HAL_ADC_GetValue(&obj->handle);
    } else {
        return 0;
    }
}

#endif
