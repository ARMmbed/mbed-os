/* mbed Microcontroller Library
 * Copyright (c) 2017, STMicroelectronics
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

static inline void dac_write(dac_t *obj, int value)
{
    HAL_DAC_SetValue(&obj->handle, obj->channel, DAC_ALIGN_12B_R, (value & DAC_RANGE));
    HAL_DAC_Start(&obj->handle, obj->channel);
}

static inline int dac_read(dac_t *obj)
{
    return (int)HAL_DAC_GetValue(&obj->handle, obj->channel);
}

void analogout_write(dac_t *obj, float value)
{
    if (value < 0.0f) {
        dac_write(obj, 0); // Min value
    } else if (value > 1.0f) {
        dac_write(obj, (int)DAC_RANGE); // Max value
    } else {
        dac_write(obj, (int)(value * (float)DAC_RANGE));
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    dac_write(obj, value >> (16 - DAC_NB_BITS));
}

float analogout_read(dac_t *obj)
{
    uint32_t value = dac_read(obj);
    return (float)value * (1.0f / (float)DAC_RANGE);
}

uint16_t analogout_read_u16(dac_t *obj)
{
    uint32_t value = dac_read(obj);
    return (value << 4) | ((value >> 8) & 0x000F); // Conversion from 12 to 16 bits
}

#endif // DEVICE_ANALOGOUT
