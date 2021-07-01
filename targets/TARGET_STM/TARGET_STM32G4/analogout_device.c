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
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

// These variables are used for the "free" function
static int channel1_used = 0;
#if defined(DAC_CHANNEL_2)
static int channel2_used = 0;
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

    // Get the peripheral name from the pin and assign it to the object
    obj->dac = (DACName)pinmap->peripheral;
    MBED_ASSERT(obj->dac != (DACName)NC);

    // Get the pin function and assign the used channel to the object
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

    // Configure GPIO
    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);

    // Save the pin for future use
    obj->pin = pinmap->pin;

    // Enable DAC clock
    if (obj->dac == DAC_1) {
        __HAL_RCC_DAC1_CLK_ENABLE();
    }
#if defined(DAC2)
    if (obj->dac == DAC_2) {
        __HAL_RCC_DAC2_CLK_ENABLE();
    }
#endif

    // Configure DAC
    obj->handle.Instance = (DAC_TypeDef *)(obj->dac);
    obj->handle.State = HAL_DAC_STATE_RESET;

    if (HAL_DAC_Init(&obj->handle) != HAL_OK) {
        error("HAL_DAC_Init failed");
    }

    /* Enable both Buffer and Switch in the configuration,
     * letting HAL layer in charge of selecting either one
     * or the other depending on the actual DAC instance and
     * channel being configured.
     */
    sConfig.DAC_Trigger      = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
#if defined(DAC_OUTPUTSWITCH_ENABLE)
    sConfig.DAC_OutputSwitch = DAC_OUTPUTSWITCH_ENABLE;
#endif

    if (obj->channel == DAC_CHANNEL_1) {
        channel1_used = 1;
    }
#if defined(DAC_CHANNEL_2)
    if (obj->channel == DAC_CHANNEL_2) {
        channel2_used = 1;
    }
#endif

    if (HAL_DAC_ConfigChannel(&obj->handle, &sConfig, obj->channel) != HAL_OK) {
        error("HAL_DAC_ConfigChannel failed");
    }

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
    // Reset DAC and disable clock
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
        __HAL_RCC_DAC1_FORCE_RESET();
        __HAL_RCC_DAC1_RELEASE_RESET();
        __HAL_RCC_DAC1_CLK_DISABLE();
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
