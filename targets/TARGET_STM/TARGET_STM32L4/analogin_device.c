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
    obj->handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV2;          // Asynchronous clock mode, input ADC clock
    obj->handle.Init.Resolution            = ADC_RESOLUTION_12B;
    obj->handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ScanConvMode          = DISABLE;                       // Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
    obj->handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           // On STM32L1xx ADC, overrun detection is enabled only if EOC selection is set to each conversion (or transfer by DMA enabled, this is not the case in this example).
    obj->handle.Init.LowPowerAutoWait      = DISABLE;
    obj->handle.Init.ContinuousConvMode    = DISABLE;                       // Continuous mode disabled to have only 1 conversion at each conversion trig
    obj->handle.Init.NbrOfConversion       = 1;                             // Parameter discarded because sequencer is disabled
    obj->handle.Init.DiscontinuousConvMode = DISABLE;                       // Parameter discarded because sequencer is disabled
    obj->handle.Init.NbrOfDiscConversion   = 1;                             // Parameter discarded because sequencer is disabled
    obj->handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            // Software start to trig the 1st conversion manually, without external event
    obj->handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    obj->handle.Init.DMAContinuousRequests = DISABLE;
    obj->handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      // DR register is overwritten with the last conversion result in case of overrun
    obj->handle.Init.OversamplingMode      = DISABLE;                       // No oversampling
#if defined(ADC_CFGR_DFSDMCFG) &&defined(DFSDM1_Channel0)
    obj->handle.Init.DFSDMConfig           = 0;
#endif

#if defined(TARGET_DISCO_L496AG)
    /* VREF+ is not connected to VDDA by default */
    /* Use 2.5V as reference (instead of 3.3V) for internal channels calculation */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE1); /* VREF_OUT2 = 2.5 V */
    HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);
    if (HAL_SYSCFG_EnableVREFBUF() != HAL_OK) {
        error("HAL_SYSCFG_EnableVREFBUF issue\n");
    }
#endif /* TARGET_DISCO_L496AG */

    // Enable ADC clock
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_SYSCLK);

    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC\n");
    }

    // ADC calibration is done only once
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
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5; //  default value (1.5 us for 80MHz clock)
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;

    switch (obj->channel) {
        case 0:
            sConfig.Channel = ADC_CHANNEL_VREFINT;
            sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5; // Minimum ADC sampling time when reading the internal reference voltage is 4us
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
            sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
            sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5; // Minimum ADC sampling time when reading the temperature is 5us
            break;
        case 18:
            sConfig.Channel = ADC_CHANNEL_VBAT;
            sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5; // Minimum ADC sampling time when reading the VBAT is 12us
            break;
        default:
            return 0;
    }

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
