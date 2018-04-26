/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "mbed_assert.h"
#include "analogout_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "xmc_dac.h"

/****************************************************************** Defines **/
#define RANGE_12BIT     0xFFF

/****************************************************************** Inlines **/

static inline void dac_write(dac_t *obj, int value) {

    XMC_DAC_CH_Write(XMC_DAC0, obj->dac, value);
}

static inline int dac_read(dac_t *obj) {

    return XMC_DAC0->DACDATA[obj->dac];
}

/**************************************************************** Functions **/

void analogout_init(dac_t *obj, PinName pin)
{

    XMC_DAC_CH_CONFIG_t dac_cfg;

    /* Check Pin */
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    if(obj->dac == (uint32_t)NC){
        MBED_ASSERT(obj->dac != (DACName)NC);
        return;
    }

    /* Fill DAC structure */
    dac_cfg.output_offset    = 0U;
    dac_cfg.data_type        = XMC_DAC_CH_DATA_TYPE_SIGNED;
    dac_cfg.output_scale     = XMC_DAC_CH_OUTPUT_SCALE_NONE;
    dac_cfg.output_negation  = XMC_DAC_CH_OUTPUT_NEGATION_DISABLED;

    /* Init DAC */
    XMC_DAC_CH_Init(XMC_DAC0, obj->dac, &dac_cfg);
    XMC_DAC_CH_StartSingleValueMode(XMC_DAC0, obj->dac);
    dac_write(obj, 0);
}

void analogout_free(dac_t *obj)
{

}

void analogout_write(dac_t *obj, float value)
{

    if (value < 0.0) {
        dac_write(obj, 0);
    } else if (value > 1.0) {
        dac_write(obj, RANGE_12BIT);
    } else {
        dac_write(obj, value * (float)RANGE_12BIT);
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{

    dac_write(obj, value >> 4); // 12-bit
}

float analogout_read(dac_t *obj)
{

    uint32_t value = dac_read(obj);
    return (float)value * (1.0f / (float)RANGE_12BIT);
}

uint16_t analogout_read_u16(dac_t *obj)
{

    uint32_t value = dac_read(obj); // 12-bit
    return (value << 4) | ((value >> 8) & 0x003F);
}

/*EOF*/
