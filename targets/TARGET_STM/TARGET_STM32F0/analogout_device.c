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

void analogout_init(dac_t *obj, PinName pin)
{
    DAC_ChannelConfTypeDef sConfig = {0};

    // Get the peripheral name from the pin and assign it to the object
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(obj->dac != (DACName)NC);

    // Get the pin function and assign the used channel to the object
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

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
    pinmap_pinout(pin, PinMap_DAC);

    // Save the pin for future use
    obj->pin = pin;

    // Enable DAC clock
    __HAL_RCC_DAC1_CLK_ENABLE();

    // Configure DAC
    obj->handle.Instance = (DAC_TypeDef *)(obj->dac);
    obj->handle.State = HAL_DAC_STATE_RESET;

    if (HAL_DAC_Init(&obj->handle) != HAL_OK) {
        error("HAL_DAC_Init failed");
    }

    sConfig.DAC_Trigger      = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (HAL_DAC_ConfigChannel(&obj->handle, &sConfig, obj->channel) != HAL_OK) {
        error("HAL_DAC_ConfigChannel failed");
    }

    analogout_write_u16(obj, 0);
}

void analogout_free(dac_t *obj)
{
    // Reset DAC and disable clock
    __HAL_RCC_DAC1_FORCE_RESET();
    __HAL_RCC_DAC1_RELEASE_RESET();
    __HAL_RCC_DAC1_CLK_DISABLE();

    // Configure GPIO
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif // DEVICE_ANALOGOUT
