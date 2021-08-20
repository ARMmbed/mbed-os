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
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "stm32f7xx_hal.h"
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

    __DAC_CLK_ENABLE();

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
