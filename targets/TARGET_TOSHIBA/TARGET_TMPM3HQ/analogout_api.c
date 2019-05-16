/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "analogout_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "mbed_wait_api.h"
#include "gpio_include.h"

static const PinMap PinMap_DAC[] = {
    {DAC0, DAC_A0, PIN_DATA(0, 3)},
    {DAC1, DAC_A1, PIN_DATA(0, 3)},
    {NC,   NC,     0}
};

void analogout_init(dac_t *obj, PinName pin)
{
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);  // Check that pin belong to DAC module
    MBED_ASSERT(obj->dac != (DACName)NC);

    pinmap_pinout(pin, PinMap_DAC);  // Set pin function as DAC
    TSB_CG_FSYSENA_IPENA06 = ENABLE;
    if (obj->dac == DAC_A0) {        // Compute handler
        obj->handler = TSB_DA0;
        TSB_CG_FSYSENB_IPENB17 = ENABLE;
    } else {
        if (obj->dac == DAC_A1) {
            obj->handler = TSB_DA1;
            TSB_CG_FSYSENB_IPENB18 = ENABLE;
        } else {
            obj->handler = NULL;
        }
    }
    obj->handler->CTL = DAC_STOP;
}

void analogout_free(dac_t *obj)
{
    obj->handler->CTL = DAC_STOP;
}

void analogout_write(dac_t *obj, float value)
{
    uint8_t outputcode = 0;

    // Enable DAC
    obj->handler->CTL = DAC_START;

    if (value < 0.0f) {
        value = 0.0f;
    } else {
        if (value >= 1.0f) {
            value = 1.0f;
        }
    }

    outputcode = (uint8_t)(value * 255.0f);
    obj->handler->REG = outputcode;
    wait_ms(3);
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    // Enable DAC
    obj->handler->CTL = DAC_START;
    obj->handler->REG = (uint8_t)(value & 0xFF);
    wait_ms(3);
}

float analogout_read(dac_t *obj)
{
    float    result = 0.0;
    uint32_t value  = 0;

    value = ((obj->handler->REG) & (0xFF));
    result = ((float)value / 255.0f);

    return result;
}

uint16_t analogout_read_u16(dac_t *obj)
{
    uint16_t value = 0;

    value = (uint16_t)((obj->handler->REG) & (0xFF));

    return value;
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}
