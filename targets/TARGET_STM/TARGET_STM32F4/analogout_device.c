/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "PeripheralPins.h"

// These variables are used for the "free" function
static int dac_channel1_used = 0;
#if defined(DAC_CHANNEL_2)
static int dac_channel2_used = 0;
#endif

#if STATIC_PINMAP_READY
#define ANALOGOUT_INIT_DIRECT analogout_init_direct
void analogout_init_direct(dac_t *obj, const PinMap *pinmap)
#else
#define ANALOGOUT_INIT_DIRECT _analogout_init_direct
static void _analogout_init_direct(dac_t *obj, const PinMap *pinmap)
#endif
{
    DAC_ChannelConfTypeDef sConfig = {0};

    // Get the peripheral name (DAC_1, ...) from the pin and assign it to the object
    obj->dac = (DACName)pinmap->peripheral;
    // Get the functions (dac channel) from the pin and assign it to the object
    uint32_t function = (uint32_t)pinmap->function;
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
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);

    // Save the pin for future use
    obj->pin = pinmap->pin;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_DAC_CLK_ENABLE();

    obj->handle.Instance = DAC;
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
        dac_channel1_used = 1;
    }
#if defined(DAC_CHANNEL_2)
    if (obj->channel == DAC_CHANNEL_2) {
        dac_channel2_used = 1;
    }
#endif
    analogout_write_u16(obj, 0);
    HAL_DAC_Start(&obj->handle, obj->channel);
}

void analogout_init(dac_t *obj, PinName pin)
{
    int peripheral = (int)pinmap_peripheral(pin, PinMap_DAC);
    int function = (int)pinmap_find_function(pin, PinMap_DAC);

    const PinMap static_pinmap = {pin, peripheral, function};

    ANALOGOUT_INIT_DIRECT(obj, &static_pinmap);
}

void analogout_free(dac_t *obj)
{
    if (obj->channel == DAC_CHANNEL_1) {
        dac_channel1_used = 0;
    }
#if defined(DAC_CHANNEL_2)
    if (obj->channel == DAC_CHANNEL_2) {
        dac_channel2_used = 0;
    }
#endif

    if ((dac_channel1_used == 0)
#if defined(DAC_CHANNEL_2)
        && (dac_channel2_used == 0)
#endif
        ) {
        // Reset DAC and disable clock
        __HAL_RCC_DAC_FORCE_RESET();
        __HAL_RCC_DAC_RELEASE_RESET();
        __HAL_RCC_DAC_CLK_DISABLE();

        // Configure GPIO back to reset value
        pin_function(obj->pin, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));
    }
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif // DEVICE_ANALOGOUT
