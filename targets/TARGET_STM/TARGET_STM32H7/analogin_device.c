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
    obj->differential = STM_PIN_INVERTED(function);

    // Save pin number for the read function
    obj->pin = pin;

    // Configure ADC object structures
    obj->handle.State = HAL_ADC_STATE_RESET;
    obj->handle.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;
    obj->handle.Init.Resolution               = ADC_RESOLUTION_16B;
    obj->handle.Init.ScanConvMode             = DISABLE;
    obj->handle.Init.ContinuousConvMode       = DISABLE;
    obj->handle.Init.DiscontinuousConvMode    = DISABLE;
    obj->handle.Init.NbrOfDiscConversion      = 0;
    obj->handle.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
    obj->handle.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T1_CC1;
    obj->handle.Init.LeftBitShift             = 0;
    obj->handle.Init.NbrOfConversion          = 1;
    obj->handle.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    obj->handle.Init.EOCSelection             = DISABLE;
    obj->handle.Init.LowPowerAutoWait         = DISABLE;
    obj->handle.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;
    obj->handle.Init.OversamplingMode         = DISABLE;

    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_CLKP;
    PeriphClkInitStruct.PLL2.PLL2P           = 4;
#if defined(DUAL_CORE)
    while (LL_HSEM_1StepLock(HSEM, CFG_HW_RCC_SEMID)) {
    }
#endif /* DUAL_CORE */
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
#if defined(DUAL_CORE)
    LL_HSEM_ReleaseLock(HSEM, CFG_HW_RCC_SEMID, HSEM_CR_COREID_CURRENT);
#endif /* DUAL_CORE */

#if defined(ADC1)
    if ((ADCName)obj->handle.Instance == ADC_1) {
        __HAL_RCC_ADC12_CLK_ENABLE();
    }
#endif
#if defined(ADC2)
    if ((ADCName)obj->handle.Instance == ADC_2) {
        __HAL_RCC_ADC12_CLK_ENABLE();
    }
#endif
#if defined(ADC3)
    if ((ADCName)obj->handle.Instance == ADC_3) {
        __HAL_RCC_ADC3_CLK_ENABLE();
    }
#endif

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC");
    }

    // Calibration
    if (obj->differential) {
        HAL_ADCEx_Calibration_Start(&obj->handle, ADC_CALIB_OFFSET, ADC_DIFFERENTIAL_ENDED);
    } else {
        HAL_ADCEx_Calibration_Start(&obj->handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    }
}

uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC channel
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    sConfig.Offset       = 0;
    if (obj->differential) {
        sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
    } else {
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
    }
    sConfig.OffsetNumber           = ADC_OFFSET_NONE;
    sConfig.OffsetRightShift       = DISABLE;
    sConfig.OffsetSignedSaturation = DISABLE;

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
            sConfig.Channel = ADC_CHANNEL_16;
            break;
        case 17:
            sConfig.Channel = ADC_CHANNEL_17;

            if ((ADCName)obj->handle.Instance == ADC_3) {
                sConfig.Channel = ADC_CHANNEL_VBAT;
                sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
            }
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_18;

            if ((ADCName)obj->handle.Instance == ADC_3) {
                sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
                sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
            }
            break;
        case 19:
            sConfig.Channel = ADC_CHANNEL_19;

            if ((ADCName)obj->handle.Instance == ADC_3) {
                sConfig.Channel = ADC_CHANNEL_VREFINT;
                sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
            }
            break;
        default:
            return 0;
    }

    LL_ADC_Disable((&obj->handle)->Instance);

    HAL_ADC_ConfigChannel(&obj->handle, &sConfig);

    HAL_ADC_Start(&obj->handle); // Start conversion

    // Wait end of conversion and get value
    uint16_t adcValue = 0;
    if (HAL_ADC_PollForConversion(&obj->handle, 10) == HAL_OK) {
        adcValue = (uint16_t)HAL_ADC_GetValue(&obj->handle);
    }
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE((&obj->handle)->Instance), LL_ADC_PATH_INTERNAL_NONE);
    return adcValue;
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
