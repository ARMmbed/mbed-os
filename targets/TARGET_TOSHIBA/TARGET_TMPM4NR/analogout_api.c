/* mbed Microcontroller Library
 * Copyright(C) TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2022
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
#include "cmsis.h"
#include "analogout_api.h"
#include "pinmap.h"

#define REG_DAC_DAxCR_REN_DISABLE       ((uint32_t)0x00000000)  // DAC Control Disable.
#define REG_DAC_DAxCR_REN_ENABLE        ((uint32_t)0x00000001)  // DAC Control Enable.
#define DAC0_CLR_IN_CLR_OUT             (1)                     // As per TRM DAC pin inout mode should be neither in nor out
#define DAC1_CLR_IN_CLR_OUT             (2)
#define MAX_ANALOG_VAL                  (1.0f)
#define MIN_ANALOG_VAL                  (0.0f)


static const PinMap PinMap_DAC[] = {
    {PT0, DAC_0, PIN_DATA(0, 1)},
    {PT1, DAC_1, PIN_DATA(0, 1)},
    {NC,  NC,    0}
};

void analogout_init(dac_t *obj, PinName pin)
{
    DACName dac_name = (DACName)pinmap_peripheral(pin, PinMap_DAC);

    MBED_ASSERT(dac_name != (DACName)NC);

    obj->dac = dac_name;

    switch (dac_name) {
        case DAC_0:
            obj->DACx = TSB_DA0;
            // Enable clock for DAC0 and Port T
            TSB_CG_FSYSMENA_IPMENA04 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB17 = TXZ_ENABLE;
            break;
        case DAC_1:
            obj->DACx = TSB_DA1;
            // Enable clock for DAC1 and Port T
            TSB_CG_FSYSMENA_IPMENA05 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB17 = TXZ_ENABLE;
            break;
        default:
            break;
    }


    pinmap_pinout(pin, PinMap_DAC);

    // DAC pins as neither input and nor output
    if (dac_name == DAC_0) {
        TSB_PT->CR &= ~(DAC0_CLR_IN_CLR_OUT);
        TSB_PT->IE &= ~(DAC0_CLR_IN_CLR_OUT);
    } else if (dac_name == DAC_1) {
        TSB_PT->CR &= ~(DAC1_CLR_IN_CLR_OUT);
        TSB_PT->IE &= ~(DAC1_CLR_IN_CLR_OUT);
    } else {
        return;
    }

    obj->DACx->CR = REG_DAC_DAxCR_REN_DISABLE;
}

void analogout_free(dac_t *obj)
{
    obj->DACx->CR = REG_DAC_DAxCR_REN_DISABLE;
    obj->dac      = (DACName)NC;
}

void analogout_write(dac_t *obj, float value)
{
    uint8_t outputcode = 0;
    obj->DACx->CR = REG_DAC_DAxCR_REN_ENABLE;
    if (value < 0.0f) {
        value = 0.0f;
    } else {
        if (value >= 1.0f) {
            value = 1.0f;
        }
    }
    outputcode = (uint8_t)(value * 255.0f);
    obj->DACx->REG = outputcode;
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    // Writing higher 8-bits to Data Register
    obj->DACx->CR = REG_DAC_DAxCR_REN_ENABLE;
    obj->DACx->REG = (uint8_t)(value & 0xFF);
}

float analogout_read(dac_t *obj)
{
    float    result = 0.0;
    uint32_t value = 0;

    value = ((obj->DACx->REG) & (0xFF));
    result = ((float)value / 255.0f);

    return result;
}

uint16_t analogout_read_u16(dac_t *obj)
{
    uint16_t value = 0;

    value = (uint16_t)((obj->DACx->REG) & (0xFF));
    return value;
    // Upper and lower byte stored with read value
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}
