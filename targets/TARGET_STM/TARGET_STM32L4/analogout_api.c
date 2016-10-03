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
#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

#define DAC_RANGE (0xFFF) // 12 bits
#define DAC_NB_BITS  (12)

static DAC_HandleTypeDef DacHandle;

// These variables are used for the "free" function
static int channel1_used = 0;
static int channel2_used = 0;

void analogout_init(dac_t *obj, PinName pin) {
    DAC_ChannelConfTypeDef sConfig = {0};

    // Get the peripheral name from the pin and assign it to the object
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(obj->dac != (DACName)NC);

    // Get the pin function and assign the used channel to the object
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);
    obj->channel = STM_PIN_CHANNEL(function);

    // Configure GPIO
    pinmap_pinout(pin, PinMap_DAC);

    // Save the pin for future use
    obj->pin = pin;

    // Enable DAC clock
    __HAL_RCC_DAC1_CLK_ENABLE();

    // Configure DAC
    DacHandle.Instance = DAC;

    if (HAL_DAC_Init(&DacHandle) != HAL_OK) {
        error("Cannot initialize DAC\n");
    }

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

    if (obj->channel == 2) {
        if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_2) != HAL_OK) {
            error("Cannot configure DAC channel 2\n");
        }
        channel2_used = 1;
    } else { // channel 1 per default
        if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
            error("Cannot configure DAC channel 1\n");
        }
        obj->channel = 1;
        channel1_used = 1;
    }

    analogout_write_u16(obj, 0);
}

void analogout_free(dac_t *obj) {
    // Reset DAC and disable clock
    if (obj->channel == 1) channel1_used = 0;
    if (obj->channel == 2) channel2_used = 0;

    if ((channel1_used == 0) && (channel2_used == 0)) {
        __HAL_RCC_DAC1_FORCE_RESET();
        __HAL_RCC_DAC1_RELEASE_RESET();
        __HAL_RCC_DAC1_CLK_DISABLE();
    }

    // Configure GPIO
    pin_function(obj->pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
}

static inline void dac_write(dac_t *obj, int value) {
    if (obj->channel == 1) {
        HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (value & DAC_RANGE));
        HAL_DAC_Start(&DacHandle, DAC_CHANNEL_1);
    }
    if (obj->channel == 2) {
        HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (value & DAC_RANGE));
        HAL_DAC_Start(&DacHandle, DAC_CHANNEL_2);
    }
}

static inline int dac_read(dac_t *obj) {
    if (obj->channel == 1) {
        return (int)HAL_DAC_GetValue(&DacHandle, DAC_CHANNEL_1);
    }
    if (obj->channel == 2) {
        return (int)HAL_DAC_GetValue(&DacHandle, DAC_CHANNEL_2);
    }
    return 0;
}

void analogout_write(dac_t *obj, float value) {
    if (value < 0.0f) {
        dac_write(obj, 0); // Min value
    } else if (value > 1.0f) {
        dac_write(obj, (int)DAC_RANGE); // Max value
    } else {
        dac_write(obj, (int)(value * (float)DAC_RANGE));
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    dac_write(obj, value >> (16 - DAC_NB_BITS));
}

float analogout_read(dac_t *obj) {
    uint32_t value = dac_read(obj);
    return (float)value * (1.0f / (float)DAC_RANGE);
}

uint16_t analogout_read_u16(dac_t *obj) {
    uint32_t value = dac_read(obj);
    return (value << 4) | ((value >> 8) & 0x000F); // Conversion from 12 to 16 bits
}

#endif // DEVICE_ANALOGOUT
