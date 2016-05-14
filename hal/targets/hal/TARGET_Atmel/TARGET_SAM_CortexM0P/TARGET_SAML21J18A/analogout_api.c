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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "dac.h"

extern uint8_t g_sys_init;

#define MAX_VAL_12BIT 0x0FFF  /*12 Bit DAC for SAML21*/

void analogout_init(dac_t *obj, PinName pin)
{
    MBED_ASSERT(obj);
    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }
    struct dac_config config_dac;
    struct dac_chan_config config_dac_chan;
    uint32_t dacperipheral;
    uint32_t ch_index;

    dacperipheral = pinmap_find_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(dacperipheral != NC);
    obj->pin = pin;
    obj->dac = dacperipheral;
    if (pin == PA02) {
        ch_index = 0;
    } else if (pin == PA05) {
        ch_index = 1;
    } else { /*Only 2 pins for DAC*/
        return 0;
    }
    obj->channel = ch_index;

    dac_get_config_defaults(&config_dac);
    dac_init(&(obj->dac_instance), (Dac *)dacperipheral, &config_dac);
    dac_chan_get_config_defaults(&config_dac_chan);
    dac_chan_set_config(&(obj->dac_instance), ch_index, &config_dac_chan);
    dac_chan_enable(&(obj->dac_instance), ch_index);
    dac_enable(&(obj->dac_instance));
}

void analogout_free(dac_t *obj)
{
    MBED_ASSERT(obj);
    struct system_pinmux_config pin_conf;

    dac_disable(&(obj->dac_instance));
    pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
    pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
    pin_conf.powersave    = false;
    pin_conf.mux_position = SYSTEM_PINMUX_GPIO;
    system_pinmux_pin_set_config(obj->pin, &pin_conf);
}

void analogout_write(dac_t *obj, float value)
{
    MBED_ASSERT(obj);
    uint16_t count_val = 0;
    if (value < 0.0f) {
        count_val = 0;
    } else if (value > 1.0f) {
        count_val = MAX_VAL_12BIT;
    } else {
        count_val = (uint16_t)(value * (float)MAX_VAL_12BIT);
    }
    dac_chan_write(&(obj->dac_instance), obj->channel, count_val);

}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    MBED_ASSERT(obj);
    uint16_t count_val;
    count_val = (uint16_t)((value * (float)MAX_VAL_12BIT) / 0xFFFF);  /*Normalization to the value 0xFFFF*/
    dac_chan_write(&(obj->dac_instance), obj->channel, count_val);

}

static uint32_t data_reg_read(dac_t *obj)
{
    Dac *const dac_module = (Dac *)obj->dac;
    return (uint32_t)dac_module->DATA[obj->channel].reg;
}

float analogout_read(dac_t *obj)
{
    MBED_ASSERT(obj);
    uint32_t data_val = data_reg_read(obj);
    return data_val/(float)MAX_VAL_12BIT;
}

uint16_t analogout_read_u16(dac_t *obj)
{
    MBED_ASSERT(obj);
    uint32_t data_val = data_reg_read(obj);
    return (uint16_t)((data_val / (float)MAX_VAL_12BIT) * 0xFFFF);   /*Normalization to the value 0xFFFF*/
}
