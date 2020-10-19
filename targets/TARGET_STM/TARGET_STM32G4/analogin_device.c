/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "mbed_wait_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_debug.h"
#include "PeripheralPins.h"
#include "stm32g4xx_ll_bus.h"
#include "string.h"

#if defined(ADC1)
static uint8_t adc1_en_counter = 0;
#define ADC1_EN_CTR adc1_en_counter
#else
#define ADC1_EN_CTR 0
#endif

#if defined(ADC2)
static uint8_t adc2_en_counter = 0;
#define ADC2_EN_CTR adc2_en_counter
#else
#define ADC2_EN_CTR 0
#endif

#if defined(ADC3)
static uint8_t adc3_en_counter = 0;
#define ADC3_EN_CTR adc3_en_counter
#else
#define ADC3_EN_CTR 0
#endif

#if defined(ADC4)
static uint8_t adc4_en_counter = 0;
#define ADC4_EN_CTR adc4_en_counter
#else
#define ADC4_EN_CTR 0
#endif

#if defined(ADC5)
static uint8_t adc5_en_counter = 0;
#define ADC5_EN_CTR adc5_en_counter
#else
#define ADC5_EN_CTR 0
#endif

#if STATIC_PINMAP_READY
#define ANALOGIN_INIT_DIRECT analogin_init_direct
void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#else
#define ANALOGIN_INIT_DIRECT _analogin_init_direct
static void _analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#endif
{
    uint32_t function = (uint32_t)pinmap->function;

    // Get the peripheral name from the pin and assign it to the object
    obj->handle.Instance = (ADC_TypeDef *)pinmap->peripheral;

    // ADC Internal Channels "pins"  (Temperature, Vref, Vbat, ...)
    //   are described in PinNames.h and PeripheralPins.c
    //   Pin value must be between 0xF0 and 0xFF
    if ((pinmap->pin < 0xF0) || (pinmap->pin >= 0x100)) {
        // Normal channels
        // Configure GPIO
        pin_function(pinmap->pin, pinmap->function);
        pin_mode(pinmap->pin, PullNone);
    } else {
        // Internal channels
        // No GPIO configuration for internal channels
    }
    MBED_ASSERT(obj->handle.Instance != (ADC_TypeDef *)NC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = STM_PIN_CHANNEL(function);

    // Save pin number for the read function
    obj->pin = pinmap->pin;

    // Configure ADC object structures
    obj->handle.State = HAL_ADC_STATE_RESET;
    memset(&obj->handle.Init, 0, sizeof(obj->handle.Init));
    obj->handle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;
    obj->handle.Init.Resolution            = ADC_RESOLUTION_12B;
    obj->handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ScanConvMode          = DISABLE;
    obj->handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    obj->handle.Init.LowPowerAutoWait      = DISABLE;
    obj->handle.Init.ContinuousConvMode    = DISABLE;
    obj->handle.Init.NbrOfConversion       = 1;
    obj->handle.Init.DiscontinuousConvMode = DISABLE;
    obj->handle.Init.NbrOfDiscConversion   = 0;
    obj->handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    obj->handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    obj->handle.Init.DMAContinuousRequests = DISABLE;
    obj->handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
    obj->handle.Init.GainCompensation      = 0;
    obj->handle.Init.OversamplingMode      = DISABLE;
    obj->handle.Init.SamplingMode          = ADC_SAMPLING_MODE_NORMAL;

#if defined(ADC1)
    if ((ADCName)obj->handle.Instance == ADC_1) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
        __HAL_RCC_ADC12_CONFIG(RCC_ADC12CLKSOURCE_SYSCLK);
        adc1_en_counter++;
    }
#endif
#if defined(ADC2)
    if ((ADCName)obj->handle.Instance == ADC_2) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
        __HAL_RCC_ADC12_CONFIG(RCC_ADC12CLKSOURCE_SYSCLK);
        adc2_en_counter++;
    }
#endif
#if defined(ADC3)
    if ((ADCName)obj->handle.Instance == ADC_3) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC345);
        __HAL_RCC_ADC345_CONFIG(RCC_ADC345CLKSOURCE_SYSCLK);
        adc3_en_counter++;
    }
#endif
#if defined(ADC4)
    if ((ADCName)obj->handle.Instance == ADC_4) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC345);
        __HAL_RCC_ADC345_CONFIG(RCC_ADC345CLKSOURCE_SYSCLK);
        adc4_en_counter++;
    }
#endif

#if defined(ADC5)
    if ((ADCName)obj->handle.Instance == ADC_5) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC345);
        __HAL_RCC_ADC345_CONFIG(RCC_ADC345CLKSOURCE_SYSCLK);
        adc5_en_counter++;
    }

#endif

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC\n");
    }

    if (!HAL_ADCEx_Calibration_GetValue(&obj->handle, ADC_SINGLE_ENDED)) {
        HAL_ADCEx_Calibration_Start(&obj->handle, ADC_SINGLE_ENDED);
    }
}

void analogin_init(analogin_t *obj, PinName pin)
{
    int peripheral;
    int function;

    if ((pin < 0xF0) || (pin >= 0x100)) {
        peripheral = (int)pinmap_peripheral(pin, PinMap_ADC);
        function = (int)pinmap_find_function(pin, PinMap_ADC);
    } else {
        peripheral = (int)pinmap_peripheral(pin, PinMap_ADC_Internal);
        function = (int)pinmap_find_function(pin, PinMap_ADC_Internal);
    }

    const PinMap static_pinmap = {pin, peripheral, function};

    ANALOGIN_INIT_DIRECT(obj, &static_pinmap);
}

uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC channel
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
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

            if ((ADCName)obj->handle.Instance == ADC_1) {
                sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
                sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
            }
            break;
        case 17:
            sConfig.Channel = ADC_CHANNEL_17;

            if ((ADCName)obj->handle.Instance == ADC_1) {
                sConfig.Channel = ADC_CHANNEL_VBAT;
                sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
            }
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_18;

            if ((ADCName)obj->handle.Instance == ADC_1) {
                sConfig.Channel = ADC_CHANNEL_VREFINT;
                sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
            }
            break;
        default:
            return 0;
    }

    if (HAL_ADC_ConfigChannel(&obj->handle, &sConfig) != HAL_OK) {
        debug("HAL_ADC_ConfigChannel issue\n");;
    }

    if (HAL_ADC_Start(&obj->handle) != HAL_OK) {
        debug("HAL_ADC_Start issue\n");;
    }

    uint16_t MeasuredValue = 0;

    if (HAL_ADC_PollForConversion(&obj->handle, 10) == HAL_OK) {
        MeasuredValue = (uint16_t)HAL_ADC_GetValue(&obj->handle);
    } else {
        debug("HAL_ADC_PollForConversion issue\n");
    }

    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE((&obj->handle)->Instance), LL_ADC_PATH_INTERNAL_NONE);

    if (HAL_ADC_Stop(&obj->handle) != HAL_OK) {
        debug("HAL_ADC_Stop issue\n");;
    }

    return MeasuredValue;
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

void analogin_free(analogin_t *obj)
{
#if defined(ADC1)
    if ((ADCName)obj->handle.Instance == ADC_1) {
        adc1_en_counter--;
        if (ADC1_EN_CTR == 0) {
            HAL_ADC_DeInit(&obj->handle);

            // Disable clock if ADC2 is also unused
            if (ADC2_EN_CTR == 0) {
                LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC12);
            }
        }
    }
#endif
#if defined(ADC2)
    if ((ADCName)obj->handle.Instance == ADC_2) {
        adc2_en_counter--;
        if (ADC2_EN_CTR == 0) {
            HAL_ADC_DeInit(&obj->handle);

            // Disable clock if ADC1 is also unused
            if (ADC1_EN_CTR == 0) {
                LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC12);
            }
        }
    }
#endif
#if defined(ADC3)
    if ((ADCName)obj->handle.Instance == ADC_3) {
        adc3_en_counter--;
        if (ADC3_EN_CTR == 0) {
            HAL_ADC_DeInit(&obj->handle);

            // Disable clock if ADC4 and ADC5 are also unused
            if ((ADC4_EN_CTR + ADC5_EN_CTR) == 0) {
                LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC345);
            }
        }
    }
#endif
#if defined(ADC4)
    if ((ADCName)obj->handle.Instance == ADC_4) {
        adc4_en_counter--;
        if (ADC4_EN_CTR == 0) {
            HAL_ADC_DeInit(&obj->handle);

            // Disable clock if ADC3 and ADC5 are also unused
            if ((ADC3_EN_CTR + ADC5_EN_CTR) == 0) {
                LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC345);
            }
        }
    }
#endif

#if defined(ADC5)
    if ((ADCName)obj->handle.Instance == ADC_5) {
        adc5_en_counter--;
        if (ADC5_EN_CTR == 0) {
            HAL_ADC_DeInit(&obj->handle);

            // Disable clock if ADC3 and ADC4 are also unused
            if ((ADC3_EN_CTR + ADC4_EN_CTR) == 0) {
                LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_ADC345);
            }
        }
    }
#endif
}

#endif
