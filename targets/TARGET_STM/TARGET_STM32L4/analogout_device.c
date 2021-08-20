/* mbed Microcontroller Library
 * Copyright (c) 2016-2020 STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed_assert.h"
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "mbed_power_mgmt.h"

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
    __HAL_RCC_DAC1_CLK_ENABLE();

    // Configure DAC
    obj->handle.Instance = DAC;
    obj->handle.State = HAL_DAC_STATE_RESET;

    if (HAL_DAC_Init(&obj->handle) != HAL_OK) {
        error("HAL_DAC_Init failed");
    }

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

    if (obj->channel == DAC_CHANNEL_1) {
        channel1_used = 1;
    }
#if defined(DAC_CHANNEL_2)
    if (obj->channel == DAC_CHANNEL_2) {
        channel2_used = 1;
    }
#endif

    if (HAL_DAC_ConfigChannel(&obj->handle, &sConfig, obj->channel) != HAL_OK) {
        error("Cannot configure DAC channel\n");
    }

    analogout_write_u16(obj, 0);
    HAL_DAC_Start(&obj->handle, obj->channel);

    /* DAC cannot be used in deepsleep/STOP mode */
    sleep_manager_lock_deep_sleep();
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

    // Configure GPIO back to reset value
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_ANALOG, GPIO_NOPULL, 0));

    sleep_manager_unlock_deep_sleep();
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif // DEVICE_ANALOGOUT
