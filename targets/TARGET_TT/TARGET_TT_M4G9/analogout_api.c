/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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

#define REG_DAC_DAxCR_REN_DISABLE           ((uint32_t)0x00000000)  // DAC Control Disable.
#define REG_DAC_DAxCR_REN_ENABLE            ((uint32_t)0x00000001)  // DAC Control Enable.
#define DAC_RANGE                           (0xFF)                  // 8 bits
#define DAC_NB_BITS                         (8)
#define DAC0_CLR_IN_CLR_OUT                 (1)          // As per TRM DAC pin inout mode should be neither in nor out
#define DAC1_CLR_IN_CLR_OUT                 (2)
#define MAX_ANALOG_VAL                      (1.0f)
#define MIN_ANALOG_VAL                      (0.0f)
#define MAX_DIGITAL_VAL                     (0xFF)
#define MIN_DIGITAL_VAL                     (0x00)

static const PinMap PinMap_DAC[] = {
    {PT0, DAC_0, PIN_DATA(0, 1)},
    {PT1, DAC_1, PIN_DATA(0, 1)},
    {NC, NC, 0}
};

static inline void dac_write(dac_t *obj,int val)
{
    // Set the DAC output
    obj->DACx->REG = (val &= DAC_RANGE);
}

static inline int dac_read(dac_t *obj)
{
    return ((obj->DACx->REG) & DAC_RANGE);
}

void analogout_init(dac_t *obj, PinName pin)
{
    DACName dac_name = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(dac_name != (DACName)NC);
    obj->dac = dac_name;
    switch (dac_name) {
        case DAC_0:
            obj->DACx = TSB_DA0;
            //Enable clock for DAC0 and Port T
            TSB_CG_FSYSMENA_IPMENA04 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB17 = TXZ_ENABLE;
            break;
        case DAC_1:
            obj->DACx = TSB_DA1;
            //Enable clock for DAC1 and Port T
            TSB_CG_FSYSMENA_IPMENA05 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB17 = TXZ_ENABLE;
            break;
        default:
            break;
    }
    // Pinout the chosen DAC
    pinmap_pinout(pin, PinMap_DAC);
    //DAC pins as neither input and nor output
    if(dac_name == DAC_0) {
        TSB_PT->CR &= ~(DAC0_CLR_IN_CLR_OUT);
        TSB_PT->IE &= ~(DAC0_CLR_IN_CLR_OUT);
    } else if(dac_name == DAC_1) {
        TSB_PT->CR &= ~(DAC1_CLR_IN_CLR_OUT);
        TSB_PT->IE &= ~(DAC1_CLR_IN_CLR_OUT);
    } else {
        return;
    }

    //Enable DAC
    obj->DACx->CR = REG_DAC_DAxCR_REN_ENABLE;
    analogout_write_u16(obj, MIN_DIGITAL_VAL);
}

void analogout_free(dac_t *obj)
{
    obj->DACx->CR = REG_DAC_DAxCR_REN_DISABLE;
    obj->dac = (DACName)NC;
}

void analogout_write(dac_t *obj, float value)
{
    if (value < MIN_ANALOG_VAL) {
        dac_write(obj, MIN_DIGITAL_VAL);
    } else if (value > MAX_ANALOG_VAL) {
        dac_write(obj,DAC_RANGE);
    } else {
        dac_write(obj, value * (float)DAC_RANGE);
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    // writing higher 8-bits to Data Register
    dac_write(obj, (value >> (16 - DAC_NB_BITS)));
}

float analogout_read(dac_t *obj)
{
    uint32_t value = dac_read(obj);
    return (float)value * (MAX_ANALOG_VAL / (float)DAC_RANGE);
}

uint16_t analogout_read_u16(dac_t *obj)
{
    uint32_t value = dac_read(obj);
    //Upper and lower byte stored with read value
    return ((value << (16 - DAC_NB_BITS)) | value);
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}
