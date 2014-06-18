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
#include "em_dac.h"
#include "em_cmu.h"
#include "analogout_api.h"

#include "cmsis.h"
#include "pinmap.h"


void analogout_init(dac_t *obj, PinName pin) {

    /* Use default settings */
    DAC_Init_TypeDef        init        = DAC_INIT_DEFAULT;
    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;

    /* Enable the DAC clock */
    CMU_ClockEnable(cmuClock_DAC0, true);

    /* Calculate the DAC clock prescaler value that will result in a DAC clock
     * close to 500kHz. Second parameter is zero, if the HFPERCLK value is 0, the
     * function will check what the current value actually is. */
    init.prescale = DAC_PrescaleCalc(500000, 0);

    /* Set reference voltage to VDD */
    init.reference = dacRefVDD;

    /* Initialize the DAC and DAC channel. */
    //TODO: Change from DAC0
    DAC_Init(DAC0, &init);
    DAC_InitChannel(DAC0, &initChannel, 0);


    DAC_Enable(DAC0, 0, true);
}

void analogout_free(dac_t *obj) {}

static inline void dac_write(dac_t *obj, int value) {
    DAC0->CH0DATA = value;
}

static inline int dac_read(dac_t *obj) {
    return DAC0->CH0DATA;
}

void analogout_write(dac_t *obj, float value) {
    dac_write(obj, value*0xFFF);
}

void analogout_write_u16(dac_t *obj, uint16_t value) {
    dac_write(obj, value >> 4);
}

float analogout_read(dac_t *obj) {
    return dac_read(obj)/4095.0;
}

uint16_t analogout_read_u16(dac_t *obj) {
    return analogout_read(obj)*0xFFFF;
}

