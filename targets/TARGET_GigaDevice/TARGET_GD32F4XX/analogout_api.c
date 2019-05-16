/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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
#include "analogout_api.h"
#include "mbed_assert.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"

#define DEV_DAC_ACCURACY_12BIT (0xFFF) // 12 bits
#define DEV_DAC_BITS  (12)

/** Initialize the analogout peripheral
 *
 * Configures the pin used by analogout.
 * @param obj The analogout object to initialize
 * @param pin The analogout pin name
 */
void analogout_init(dac_t *obj, PinName pin)
{
    /* get the peripheral name from the pin and assign it to the object */
    obj->dac = (DACName)pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(obj->dac != (DACName)NC);

    /* get the pin function and assign the used channel to the object */
    uint32_t function = pinmap_function(pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

    obj->channel = GD_PIN_CHANNEL_GET(function);
    MBED_ASSERT(obj->channel <= DAC1);

    /* configure GPIO */
    pinmap_pinout(pin, PinMap_DAC);

    /* save the pin for future use */
    obj->pin = pin;

    /* enable DAC clock */
    rcu_periph_clock_enable(RCU_DAC);

    /* configure DAC */
    dac_wave_mode_config(obj->channel, DAC_WAVE_DISABLE);
    dac_trigger_disable(obj->channel);
    dac_output_buffer_enable(obj->channel);
    analogout_write_u16(obj, 0);
}

/** Release the analogout object
 *
 * Note: This is not currently used in the mbed-drivers
 * @param obj The analogout object
 */
void analogout_free(dac_t *obj)
{
    /* reset DAC and disable clock */
    dac_deinit();
    rcu_periph_clock_disable(RCU_DAC);

    /* configure GPIO */
    /* get the pin function and assign the used channel to the object */
    uint32_t function = pinmap_function(obj->pin, PinMap_DAC);
    MBED_ASSERT(function != (uint32_t)NC);

    pin_function(obj->pin, function);
}

/** set the output voltage with specified as a integer
 *
 * @param obj The analogin object
 * @param value The integer-point output voltage to be set
 */
static inline void dev_dac_data_set(dac_t *obj, uint16_t value)
{
    dac_data_set(obj->channel, DAC_ALIGN_12B_R, (value & DEV_DAC_ACCURACY_12BIT));

    dac_enable(obj->channel);
}

/** get the current DAC data
 *
 * @param obj The analogin object
 * @return DAC data
 */
static inline uint16_t dev_dac_data_get(dac_t *obj)
{
    return (uint16_t)dac_output_value_get(obj->channel);
}

/** Set the output voltage, specified as a percentage (float)
 *
 * @param obj The analogin object
 * @param value The floating-point output voltage to be set
 */
void analogout_write(dac_t *obj, float value)
{
    if (value < 0.0f) {
        /* when the value is less than 0.0, set DAC output data to 0 */
        dev_dac_data_set(obj, 0);
    } else if (value > 1.0f) {
        /* when the value is more than 1.0, set DAC output data to 0xFFF */
        dev_dac_data_set(obj, (uint16_t)DEV_DAC_ACCURACY_12BIT);
    } else {
        dev_dac_data_set(obj, (uint16_t)(value * (float)DEV_DAC_ACCURACY_12BIT));
    }
}

/** Set the output voltage, specified as unsigned 16-bit
 *
 * @param obj The analogin object
 * @param value The unsigned 16-bit output voltage to be set
 */
void analogout_write_u16(dac_t *obj, uint16_t value)
{
    dev_dac_data_set(obj, value >> (16 - DEV_DAC_BITS));
}

/** Read the current voltage value on the pin
 *
 * @param obj The analogin object
 * @return A floating-point value representing the current voltage on the pin,
 *     measured as a percentage
 */
float analogout_read(dac_t *obj)
{
    uint16_t ret_val = dev_dac_data_get(obj);
    return (float)ret_val * (1.0f / (float)DEV_DAC_ACCURACY_12BIT);
}

/** Read the current voltage value on the pin, as a normalized unsigned 16-bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16-bit value representing the current voltage on the pin
 */
uint16_t analogout_read_u16(dac_t *obj)
{
    uint16_t ret_val = dev_dac_data_get(obj);
    return (uint16_t)((ret_val << 4) | ((ret_val >> 8) & 0x000F));
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif /* DEVICE_ANALOGOUT */
