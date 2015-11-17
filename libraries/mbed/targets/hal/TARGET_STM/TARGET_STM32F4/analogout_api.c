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
#include "stm32f4xx_hal.h"
#include "PeripheralPins.h"

#define DAC_RANGE    (0xFFF) // 12 bits
#define DAC_NB_BITS  (12)

DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef sConfig;

void analogout_init(dac_t *obj, PinName pin) {
    uint32_t channel ;
    HAL_StatusTypeDef status;

    // Get the peripheral name (DAC_1, ...) from the pin and assign it to the object
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    // Get the functions (dac channel) from the pin and assign it to the object
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);
    // Save the channel for the write and read functions
    obj->channel = STM_PIN_CHANNEL(function);

    if (obj->dac == (DACName)NC) {
        error("DAC pin mapping failed");
    }

    // Configure GPIO
    pinmap_pinout(pin, PinMap_DAC);

    __GPIOA_CLK_ENABLE();

    __DAC_CLK_ENABLE();

    DacHandle.Instance = DAC;

    status = HAL_DAC_Init(&DacHandle);
    if ( status != HAL_OK ) {
        error("HAL_DAC_Init failed");
    }

    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

    if (obj->channel == 1) {
        channel = DAC_CHANNEL_1; 
    } else {
        channel = DAC_CHANNEL_2;
    }

    if (HAL_DAC_ConfigChannel(&DacHandle, &sConfig, channel) != HAL_OK) {
        error("HAL_DAC_ConfigChannel failed");
    }

    if (HAL_DAC_Start(&DacHandle, channel) != HAL_OK) {
        error("HAL_DAC_Start failed");
    }

    if (HAL_DAC_SetValue(&DacHandle, channel, DAC_ALIGN_12B_R, 0x000) != HAL_OK) {
        error("HAL_DAC_SetValue failed");
    }

}

void analogout_free(dac_t *obj) {
}

static inline void dac_write(dac_t *obj, int value) {
    HAL_StatusTypeDef status = HAL_ERROR;

    if (obj->channel == 1) {
        status = HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (value & DAC_RANGE));
    } else if (obj->channel == 2) {
        status = HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (value & DAC_RANGE));
    }

    if ( status != HAL_OK ) {
        error("DAC pin mapping failed");
    }
}

static inline int dac_read(dac_t *obj) {
    if (obj->channel == 1) {
        return (int)HAL_DAC_GetValue(&DacHandle, DAC_CHANNEL_1);
    } else if (obj->channel == 2) {
        return (int)HAL_DAC_GetValue(&DacHandle, DAC_CHANNEL_2);
    }
	return 0;	/* Just silented warning */
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
