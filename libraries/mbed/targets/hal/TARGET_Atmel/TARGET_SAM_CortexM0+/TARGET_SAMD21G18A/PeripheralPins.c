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

#include "PeripheralPins.h"

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {PA02,  ADC_0, 1},
    {PA03,  ADC_1, 1},
    {PB08,  ADC_2, 1},
    {PB09,  ADC_3, 1},
    {PA04,  ADC_4, 1},
    {PA05,  ADC_5, 1},
    {PA06,  ADC_6, 1},
    {PA07,  ADC_7, 1},
    {PB02,  ADC_10, 1},
    {PB03,  ADC_11, 1},
    {PA08,  ADC_16, 1},
    {PA09,  ADC_17, 1},
    {PA10,  ADC_18, 1},
    {PA11,  ADC_19, 1},

    /* Not connected */
    {NC  , NC   , NC}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {PA02,  DAC_0, 1},

    /* Not connected */
    {NC  , NC   , NC}
};

/************SERCOM Pins***********/
const PinMap PinMap_SERCOM_PAD[] = {

    /* Not connected */
    {NC  , NC   , NC}
};

/*******SERCOM Pins extended*******/
const PinMap PinMap_SERCOM_PADEx[] = {

    /* Not connected */
    {NC  , NC   , NC}
};


/************PWM***************/
const PinMap PinMap_PWM[] = {

    /* Not connected */
    {NC  , NC   , NC}
};

/**********EXTINT*************/
const PinMap PinMap_EXTINT[] = {
    {PA16,  EXTINT_0, 0},
    {PA00,  EXTINT_0, 0},

    {PA17,  EXTINT_1, 0},
    {PA01,  EXTINT_1, 0},

    {PA18,  EXTINT_2, 0},
    {PA02,  EXTINT_2, 0},
    {PB02,  EXTINT_2, 0},

    {PA03,  EXTINT_3, 0},
    {PA19,  EXTINT_3, 0},
    {PB03,  EXTINT_3, 0},

    {PA04,  EXTINT_4, 0},
    {PA20,  EXTINT_4, 0},

    {PA05,  EXTINT_5, 0},
    {PA21,  EXTINT_5, 0},

    {PA06,  EXTINT_6, 0},
    {PA22,  EXTINT_6, 0},
    {PB22,  EXTINT_6, 0},

    {PA07,  EXTINT_7, 0},
    {PA23,  EXTINT_7, 0},
    {PB23,  EXTINT_7, 0},

    {PA28,  EXTINT_8, 0},
    {PB08,  EXTINT_8, 0},

    {PA09,  EXTINT_9, 0},
    {PB09,  EXTINT_9, 0},

    {PA10,  EXTINT_10, 0},
    {PA30,  EXTINT_10, 0},
    {PB10,  EXTINT_10, 0},

    {PA11,  EXTINT_11, 0},
    {PA31,  EXTINT_11, 0},
    {PB11,  EXTINT_11, 0},

    {PA12,  EXTINT_12, 0},
    {PA24,  EXTINT_12, 0},

    {PA13,  EXTINT_13, 0},
    {PA25,  EXTINT_13, 0},

    {PA14,  EXTINT_14, 0},

    {PA15,  EXTINT_15, 0},
    {PA27,  EXTINT_15, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

const struct pwm_pin_channel pwn_pins[] = {

    /* Not connected */
    {NC  , NC   , NC}
};



