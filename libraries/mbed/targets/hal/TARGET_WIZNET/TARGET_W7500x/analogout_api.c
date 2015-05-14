/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#define DAC_RANGE (0xFFF) // 12 bits

static DAC_HandleTypeDef DacHandle;

void analogout_init(dac_t *obj, PinName pin)
{

}

void analogout_free(dac_t *obj)
{
}

static inline void dac_write(dac_t *obj, uint16_t value)
{
}

static inline int dac_read(dac_t *obj)
{
}

void analogout_write(dac_t *obj, float value)
{
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
}

float analogout_read(dac_t *obj)
{
}

uint16_t analogout_read_u16(dac_t *obj)
{
}

#endif // DEVICE_ANALOGOUT
