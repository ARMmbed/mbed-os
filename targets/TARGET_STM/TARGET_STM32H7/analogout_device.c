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

#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "stm32h7xx_hal.h"
#include "PeripheralPins.h"

// These variables are used for the "free" function
static int channel1_used = 0;
#if defined(DAC_CHANNEL_2)
static int channel2_used = 0;
#endif

void analogout_init(dac_t *obj, PinName pin)
{
    DAC_ChannelConfTypeDef sConfig = {0};

    // Get the peripheral name (DAC_1, ...) from the pin and assign it to the object
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    // Get the functions (dac channel) from the pin and assign it to the object
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

    // Save the channel for the write and read functions
    switch (STM_PIN_CHANNEL(function)) {
        case 1:
            obj->channel = DAC_CHANNEL_1;
            break;
#if defined(DAC_CHANNEL_2)
        case 2:
            obj->channel = DAC_CHANNEL_2;
            break;
#endif
        default:
            error("Unknown DAC channel");
            break;
    }

    if (obj->dac == (DACName)NC) {
        error("DAC pin mapping failed");
    }

    // Configure GPIO
    pinmap_pinout(pin, PinMap_DAC);

    // Save the pin for future use
    obj->pin = pin;

    __GPIOA_CLK_ENABLE();

    if (obj->dac == DAC_1) {
         __HAL_RCC_DAC12_CLK_ENABLE();
    }
#if defined(DAC2)
    if (obj->dac == DAC_2) {
        __HAL_RCC_DAC2_CLK_ENABLE();
    }
#endif

    obj->handle.Instance = DAC1;
    obj->handle.State = HAL_DAC_STATE_RESET;

    if (HAL_DAC_Init(&obj->handle) != HAL_OK) {
        error("HAL_DAC_Init failed");
    }

    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&obj->handle, &sConfig, obj->channel) != HAL_OK) {
        error("HAL_DAC_ConfigChannel failed");
    }

    if (obj->channel == DAC_CHANNEL_1) {
        channel1_used = 1;
    }
#if defined(DAC_CHANNEL_2)
    if (obj->channel == DAC_CHANNEL_2) {
        channel2_used = 1;
    }
#endif
    analogout_write_u16(obj, 0);
    HAL_DAC_Start(&obj->handle, obj->channel);
}

void analogout_free(dac_t *obj)
{
    if (obj->channel == DAC_CHANNEL_1) {
        channel1_used = 0;
    }
#if defined(DAC_CHANNEL_2)    
    if (obj->channel == DAC_CHANNEL_2) {
        channel2_used = 0;
    }
#endif    

    if ((channel1_used == 0) 
#if defined(DAC_CHANNEL_2)
        && (channel2_used == 0)
#endif
        ) {
        // Reset DAC and disable clock
        __HAL_RCC_DAC12_FORCE_RESET();
        __HAL_RCC_DAC12_RELEASE_RESET();
        __HAL_RCC_DAC12_CLK_DISABLE();
    }

#if defined(DAC2)
    if (obj->dac == DAC_2) {
        __HAL_RCC_DAC2_FORCE_RESET();
        __HAL_RCC_DAC2_RELEASE_RESET();
        __HAL_RCC_DAC2_CLK_DISABLE();
    }
#endif

    // Configure GPIO back to reset value
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}


#endif // DEVICE_ANALOGOUT
