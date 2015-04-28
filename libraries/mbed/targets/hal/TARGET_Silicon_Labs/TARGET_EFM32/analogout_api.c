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

#include "device.h"
#if DEVICE_ANALOGOUT

#include "mbed_assert.h"
#include "error.h"
#include "analogout_api.h"
#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"

#include "em_dac.h"
#include "em_cmu.h"

uint8_t analogout_get_index(dac_t *obj)
{
    return 0;
}

void analogout_preinit(dac_t *obj, PinName pin)
{
    obj->dac = (DAC_TypeDef *) pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT((int) obj->dac != NC);
    
    obj->channel = pin_location(pin, PinMap_DAC);
    MBED_ASSERT((int) obj->channel != NC);
}

void analogout_init(dac_t *obj, PinName pin) {
    static uint8_t initialized = 0;

    if (!initialized) {
        /* Initialize the DAC. Will disable both DAC channels, so should only be done once */
        /* Use default settings */
        DAC_Init_TypeDef init = DAC_INIT_DEFAULT;

        /* Calculate the DAC clock prescaler value that will result in a DAC clock
         * close to 500kHz. Second parameter is zero. This uses the current HFPERCLK
         * frequency instead of setting a new one. */
        init.prescale = DAC_PrescaleCalc(500000, 0);

        /* Set reference voltage to VDD */
        init.reference = dacRefVDD;

        DAC_Init(obj->dac, &init);
        initialized = 1;
    }
    /* Use default channel settings */
    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;
    DAC_InitChannel(obj->dac, &initChannel, obj->channel);

    /* init pins */
    analogout_preinit(obj, pin);
}

void analogout_enable(dac_t *obj, uint8_t enable)
{
    DAC_Enable(obj->dac, obj->channel, enable);
}

void analogout_pins_enable(dac_t *obj, uint8_t enable)
{
    //not avail for EFM32
}

static inline void dac_write(dac_t *obj, int value) {
    switch (obj->channel) {
        case 0:
            obj->dac->CH0DATA = value;
            break;
        case 1:
            obj->dac->CH1DATA = value;
            break;
    }
}

static inline int dac_read(dac_t *obj) {
    switch (obj->channel) {
        case 0:
            return obj->dac->CH0DATA;
            break;
        case 1:
            return obj->dac->CH1DATA;
            break;
        default:
            error("AnalogOut pin error. Invalid channel");
            return -1;
            break;
    }
}

void analogout_write(dac_t *obj, float value) {
    /* We multiply the float value with 0xFFF because the DAC has 12-bit resolution.
     * Ie. accepts values between 0 and 0xFFF (4096). */
    dac_write(obj, value*0xFFF);
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    /* The DAC has 12 bit resolution, so we remove the 4 least significant bits */
    dac_write(obj, value >> 4);
}

float analogout_read(dac_t *obj) {
    /* dac_read returns a number between 0 and 0xFFF. Division gives us a float between 0 and 1 */
    return dac_read(obj)/(float)0xFFF;
}

uint16_t analogout_read_u16(dac_t *obj) {
    /* dac_read returns a number with 12 significant digits,
     * so we shift in 0s from right to make it a 16 bit number */
    return dac_read(obj) << 4;
}

#endif
