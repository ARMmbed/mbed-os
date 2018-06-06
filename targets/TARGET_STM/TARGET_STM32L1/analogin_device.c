/* mbed Microcontroller Library
 * Copyright (c) 2016, STMicroelectronics
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
    obj->handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV4;
    obj->handle.Init.Resolution            = ADC_RESOLUTION_12B;
    obj->handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ScanConvMode          = DISABLE;                       // Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
    obj->handle.Init.EOCSelection          = EOC_SINGLE_CONV;               // On STM32L1xx ADC, overrun detection is enabled only if EOC selection is set to each conversion (or transfer by DMA enabled, this is not the case in this example).
    obj->handle.Init.LowPowerAutoWait      = ADC_AUTOWAIT_UNTIL_DATA_READ;  // Enable the dynamic low power Auto Delay: new conversion start only when the previous conversion (for regular group) or previous sequence (for injected group) has been treated by user software.
    obj->handle.Init.LowPowerAutoPowerOff  = ADC_AUTOPOWEROFF_IDLE_PHASE;   // Enable the auto-off mode: the ADC automatically powers-off after a conversion and automatically wakes-up when a new conversion is triggered (with startup time between trigger and start of sampling).
    obj->handle.Init.ChannelsBank          = ADC_CHANNELS_BANK_A;
    obj->handle.Init.ContinuousConvMode    = DISABLE;                       // Continuous mode disabled to have only 1 conversion at each conversion trig
    obj->handle.Init.NbrOfConversion       = 1;                             // Parameter discarded because sequencer is disabled
    obj->handle.Init.DiscontinuousConvMode = DISABLE;                       // Parameter discarded because sequencer is disabled
    obj->handle.Init.NbrOfDiscConversion   = 1;                             // Parameter discarded because sequencer is disabled
    obj->handle.Init.ExternalTrigConv      = 0;                             // Not used
    obj->handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    obj->handle.Init.DMAContinuousRequests = DISABLE;

    __HAL_RCC_ADC1_CLK_ENABLE();

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC");
    }

    if (!__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY)) {
        // Enable the HSI (to clock the ADC)
        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
        RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
        RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);
    }
}

uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC channel
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
        case 16:
            sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
            break;
        case 17:
            sConfig.Channel = ADC_CHANNEL_VREFINT;
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_18;
            break;
        case 19:
            sConfig.Channel = ADC_CHANNEL_19;
            break;
        case 20:
            sConfig.Channel = ADC_CHANNEL_20;
            break;
        case 21:
            sConfig.Channel = ADC_CHANNEL_21;
            break;
        case 22:
            sConfig.Channel = ADC_CHANNEL_22;
            break;
        case 23:
            sConfig.Channel = ADC_CHANNEL_23;
            break;
        case 24:
            sConfig.Channel = ADC_CHANNEL_24;
            break;
        case 25:
            sConfig.Channel = ADC_CHANNEL_25;
            break;
        case 26:
            sConfig.Channel = ADC_CHANNEL_26;
            break;
#ifdef ADC_CHANNEL_27
        case 27:
            sConfig.Channel = ADC_CHANNEL_27;
            break;
#endif
#ifdef ADC_CHANNEL_28
        case 28:
            sConfig.Channel = ADC_CHANNEL_28;
            break;
#endif
#ifdef ADC_CHANNEL_29
        case 29:
            sConfig.Channel = ADC_CHANNEL_29;
            break;
#endif
#ifdef ADC_CHANNEL_30
        case 30:
            sConfig.Channel = ADC_CHANNEL_30;
            break;
#endif
#ifdef ADC_CHANNEL_31
        case 31:
            sConfig.Channel = ADC_CHANNEL_31;
            break;
#endif
        default:
            return 0;
    }

    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES;

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
