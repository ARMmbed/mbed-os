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
    {PB00,  ADC_8, 1},
    {PB01,  ADC_9, 1},
    {PB02,  ADC_10, 1},
    {PB03,  ADC_11, 1},
    {PB04,  ADC_12, 1},
    {PB05,  ADC_13, 1},
    {PB06,  ADC_14, 1},
    {PB07,  ADC_15, 1},
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
    {PA00, SERCOM1_PAD0, 3},
    {PA01, SERCOM1_PAD1, 3},
    {PA04, SERCOM0_PAD0, 3},
    {PA05, SERCOM0_PAD1, 3},
    {PA06, SERCOM0_PAD2, 3},
    {PA07, SERCOM0_PAD3, 3},
    {PA08, SERCOM0_PAD0, 2},
    {PA09, SERCOM0_PAD1, 2},
    {PA10, SERCOM0_PAD2, 2},
    {PA11, SERCOM0_PAD3, 2},
    {PA12, SERCOM2_PAD0, 2},
    {PA13, SERCOM2_PAD1, 2},
    {PA14, SERCOM2_PAD2, 2},
    {PA15, SERCOM2_PAD3, 2},
    {PA16, SERCOM1_PAD0, 2},
    {PA17, SERCOM1_PAD1, 2},
    {PA18, SERCOM1_PAD2, 2},
    {PA19, SERCOM1_PAD3, 2},
    {PA20, SERCOM3_PAD2, 3},
    {PA21, SERCOM3_PAD3, 3},
    {PA22, SERCOM3_PAD0, 2},
    {PA23, SERCOM3_PAD1, 2},
    {PA24, SERCOM3_PAD2, 2},
    {PA25, SERCOM3_PAD3, 2},
    {PA30, SERCOM1_PAD2, 3},
    {PA31, SERCOM1_PAD3, 3},
    {PB00, SERCOM5_PAD2, 3},
    {PB01, SERCOM5_PAD3, 3},
    {PB02, SERCOM5_PAD0, 3},
    {PB03, SERCOM5_PAD1, 3},
    {PB08, SERCOM4_PAD0, 3},
    {PB09, SERCOM4_PAD1, 3},
    {PB10, SERCOM4_PAD2, 3},
    {PB11, SERCOM4_PAD3, 3},
    {PB12, SERCOM4_PAD0, 2},
    {PB13, SERCOM4_PAD1, 2},
    {PB14, SERCOM4_PAD2, 2},
    {PB15, SERCOM4_PAD3, 2},
    {PB16, SERCOM5_PAD0, 2},
    {PB17, SERCOM5_PAD1, 2},
    {PB22, SERCOM5_PAD2, 3},
    {PB23, SERCOM5_PAD3, 3},
    {PB30, SERCOM5_PAD0, 3},
    {PB31, SERCOM5_PAD1, 3},

    /* Not connected */
    {NC  , NC   , NC}
};

/*******SERCOM Pins extended*******/
const PinMap PinMap_SERCOM_PADEx[] = {
    {PA08, SERCOM2_PAD0, 3},
    {PA09, SERCOM2_PAD1, 3},
    {PA10, SERCOM2_PAD2, 3},
    {PA11, SERCOM2_PAD3, 3},
    {PA12, SERCOM4_PAD0, 3},
    {PA13, SERCOM4_PAD1, 3},
    {PA14, SERCOM4_PAD2, 3},
    {PA15, SERCOM4_PAD3, 3},
    {PA16, SERCOM3_PAD0, 3},
    {PA17, SERCOM3_PAD1, 3},
    {PA18, SERCOM3_PAD2, 3},
    {PA19, SERCOM3_PAD3, 3},
    {PA20, SERCOM5_PAD2, 2},
    {PA21, SERCOM5_PAD3, 2},
    {PA22, SERCOM5_PAD0, 3},
    {PA23, SERCOM5_PAD1, 3},
    {PA24, SERCOM5_PAD2, 3},
    {PA25, SERCOM5_PAD3, 3},

    /* Not connected */
    {NC  , NC   , NC}
};


/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PA00, PWM_2, 4},
    {PA01, PWM_2, 4},
    {PA04, PWM_0, 4},
    {PA05, PWM_0, 4},
    {PA06, PWM_1, 4},
    {PA07, PWM_1, 4},
    {PA08, PWM_1, 5},
    {PA09, PWM_1, 5},
    {PA10, PWM_1, 4},
    {PA11, PWM_1, 4},
    {PA12, PWM_2, 4},
    {PA13, PWM_2, 4},
    {PA14, PWM_0, 5},
    {PA15, PWM_0, 5},
    {PA16, PWM_2, 4},
    {PA17, PWM_2, 4},
    {PA18, PWM_0, 5},
    {PA19, PWM_0, 5},
    {PA20, PWM_0, 5},
    {PA21, PWM_0, 5},
    {PA22, PWM_0, 5},
    {PA23, PWM_0, 5},
    {PA24, PWM_1, 5},
    {PA25, PWM_1, 5},
    {PA30, PWM_1, 4},
    {PA31, PWM_1, 4},
    {PB10, PWM_0, 5},
    {PB11, PWM_0, 5},
    {PB12, PWM_0, 5},
    {PB13, PWM_0, 5},
    {PB16, PWM_0, 5},
    {PB17, PWM_0, 5},
    {PB30, PWM_1, 5},
    {PB31, PWM_1, 5},

    /* Not connected */
    {NC  , NC   , NC}
};

/**********EXTINT*************/
const PinMap PinMap_EXTINT[] = {
    {PA16,  EXTINT_0, 0},
    {PB00,  EXTINT_0, 0},
    {PB16,  EXTINT_0, 0},
    {PA00,  EXTINT_0, 0},

    {PA17,  EXTINT_1, 0},
    {PB01,  EXTINT_1, 0},
    {PB17,  EXTINT_1, 0},
    {PA01,  EXTINT_1, 0},

    {PA18,  EXTINT_2, 0},
    {PA02,  EXTINT_2, 0},
    {PB02,  EXTINT_2, 0},

    {PA03,  EXTINT_3, 0},
    {PA19,  EXTINT_3, 0},
    {PB03,  EXTINT_3, 0},

    {PA04,  EXTINT_4, 0},
    {PA20,  EXTINT_4, 0},
    {PB04,  EXTINT_4, 0},

    {PA05,  EXTINT_5, 0},
    {PA21,  EXTINT_5, 0},
    {PB05,  EXTINT_5, 0},

    {PA06,  EXTINT_6, 0},
    {PA22,  EXTINT_6, 0},
    {PB06,  EXTINT_6, 0},
    {PB22,  EXTINT_6, 0},

    {PA07,  EXTINT_7, 0},
    {PA23,  EXTINT_7, 0},
    {PB07,  EXTINT_7, 0},
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
    {PB12,  EXTINT_12, 0},

    {PA13,  EXTINT_13, 0},
    {PA25,  EXTINT_13, 0},
    {PB13,  EXTINT_13, 0},

    {PB14,  EXTINT_14, 0},
    {PB30,  EXTINT_14, 0},
    {PA14,  EXTINT_14, 0},

    {PA15,  EXTINT_15, 0},
    {PA27,  EXTINT_15, 0},
    {PB15,  EXTINT_15, 0},
    {PB31,  EXTINT_15, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

const struct pwm_pin_channel pwn_pins[] = {
    {PA00, PWM_2, 0},
    {PA01, PWM_2, 1},
    {PA04, PWM_0, 0},
    {PA05, PWM_0, 1},
    {PA06, PWM_1, 0},
    {PA07, PWM_1, 1},
    {PA08, PWM_1, 2},
    {PA09, PWM_1, 3},
    {PA10, PWM_1, 0},
    {PA11, PWM_1, 1},
    {PA12, PWM_2, 0},
    {PA13, PWM_2, 1},
    {PA14, PWM_0, 4},
    {PA15, PWM_0, 5},
    {PA16, PWM_2, 0},
    {PA17, PWM_2, 1},
    {PA18, PWM_0, 2},
    {PA19, PWM_0, 3},
    {PA20, PWM_0, 6},
    {PA21, PWM_0, 7},
    {PA22, PWM_0, 4},
    {PA23, PWM_0, 5},
    {PA24, PWM_1, 2},
    {PA25, PWM_1, 3},
    {PA30, PWM_1, 0},
    {PA31, PWM_1, 1},
    {PB10, PWM_0, 4},
    {PB11, PWM_0, 5},
    {PB12, PWM_0, 6},
    {PB13, PWM_0, 7},
    {PB16, PWM_0, 4},
    {PB17, PWM_0, 5},
    {PB30, PWM_1, 2},
    {PB31, PWM_1, 3},

    /* Not connected */
    {NC  , NC   , NC}
};



