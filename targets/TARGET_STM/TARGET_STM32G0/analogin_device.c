/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015 STMicroelectronics.
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
#include "PeripheralPins.h"

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
    obj->handle.Init.OversamplingMode           = DISABLE;
    obj->handle.Init.ClockPrescaler             = ADC_CLOCK_SYNC_PCLK_DIV4;
    obj->handle.Init.Resolution                 = ADC_RESOLUTION_12B;
    obj->handle.Init.SamplingTimeCommon1        = ADC_SAMPLETIME_160CYCLES_5;
    obj->handle.Init.SamplingTimeCommon2        = ADC_SAMPLETIME_160CYCLES_5;
    obj->handle.Init.ScanConvMode               = ADC_SCAN_SEQ_FIXED;
    obj->handle.Init.DataAlign                  = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ContinuousConvMode         = DISABLE;
    obj->handle.Init.DiscontinuousConvMode      = DISABLE;
    obj->handle.Init.ExternalTrigConvEdge       = ADC_EXTERNALTRIGCONVEDGE_NONE;
    obj->handle.Init.ExternalTrigConv           = ADC_SOFTWARE_START; // Not used here
    obj->handle.Init.DMAContinuousRequests      = DISABLE;
    obj->handle.Init.EOCSelection               = ADC_EOC_SINGLE_CONV;
    obj->handle.Init.Overrun                    = ADC_OVR_DATA_OVERWRITTEN;
    obj->handle.Init.LowPowerAutoWait           = DISABLE;
    obj->handle.Init.LowPowerAutoPowerOff       = DISABLE;
    obj->handle.Init.NbrOfConversion            = 1;
    obj->handle.Init.OversamplingMode           = DISABLE; // No oversampling
    obj->handle.Init.Oversampling.Ratio         = ADC_OVERSAMPLING_RATIO_2;
    obj->handle.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_NONE;
    obj->handle.Init.Oversampling.TriggeredMode = LL_ADC_OVS_REG_CONT;
    obj->handle.Init.TriggerFrequencyMode       = ADC_TRIGGER_FREQ_HIGH;


    __HAL_RCC_ADC_CLK_ENABLE();

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC");
    }

    if (!HAL_ADCEx_Calibration_GetValue(&obj->handle)) {
        HAL_ADCEx_Calibration_Start(&obj->handle);
    }

    LL_ADC_Enable(obj->handle.Instance);
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
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
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
            sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
            break;
        case 13:
            sConfig.Channel = ADC_CHANNEL_VREFINT;
            break;
        case 14:
            sConfig.Channel = ADC_CHANNEL_VBAT;
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

    /* Reset ADC channel selection register */
    ADC1->CHSELR = 0;

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
