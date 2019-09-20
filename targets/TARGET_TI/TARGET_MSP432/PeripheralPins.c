/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#include "PeripheralPins.h"
#include "PeripheralNames.h"
#include "cmsis.h"

const PinMap PinMap_empty[] = {
    { NC, NC, 0 }
};

/*** ADC ***/
const PinMap PinMap_ADC[] = {
    { P5_5, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  0) },
    { P5_4, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  1) },
    { P5_3, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  2) },
    { P5_2, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  3) },
    { P5_1, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  4) },
    { P5_0, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  5) },
    { P4_7, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  6) },
    { P4_6, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  7) },
    { P4_5, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  8) },
    { P4_4, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone,  9) },
    { P4_3, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 10) },
    { P4_2, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 11) },
    { P4_1, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 12) },
    { P4_0, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 13) },
    { P6_1, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 14) },
    { P6_0, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 15) },
    { P9_1, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 16) },
    { P9_0, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 17) },
    { P8_7, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 18) },
    { P8_6, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 19) },
    { P8_5, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 20) },
    { P8_4, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 21) },
    { P8_3, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 22) },
    { P8_2, ADC14_BASE, MSP432_PIN_DATA(SEL3, PIN_INPUT, PullNone, 23) },
    { NC, NC, 0 }
};

const PinMap PinMap_ADC_Internal[] = {
    { NC, NC, 0 }
};

//*** I2C ***
const PinMap PinMap_I2C_SDA[] = {
    { P1_6,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P6_4,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P3_6,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P6_6,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL2, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P10_2, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { NC, NC, 0 }
};

const PinMap PinMap_I2C_SCL[] = {
    { P1_7,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P6_5,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P3_7,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P6_7,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL2, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { P10_3, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, OpenDrainPullUp, 0) },
    { NC, NC, 0 }
};

//*** SERIAL ***
const PinMap PinMap_UART_TX[] = {
    { P1_3, EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P2_3, EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P3_3, EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P9_7, EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { NC, NC, 0 }
};

const PinMap PinMap_UART_RX[] = {
    { P1_2, EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 0) },
    { P2_2, EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 0) },
    { P3_2, EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 0) },
    { P9_6, EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 0) },
    { NC, NC, 0 }
};

//*** SPI ***
const PinMap PinMap_SPI_MOSI[] = {
    { P1_3,  EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P2_3,  EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P3_3,  EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P9_7,  EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P1_6,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { P6_4,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 5) },
    { P3_6,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 6) },
    { P6_6,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL2, PIN_OUTPUT, PullNone, 7) },
    { P10_2, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 7) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_MISO[] = {
    { P1_2,  EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 0) },
    { P2_2,  EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 1) },
    { P3_2,  EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 2) },
    { P9_6,  EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 3) },
    { P1_7,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 4) },
    { P6_5,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 5) },
    { P3_7,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 6) },
    { P6_7,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL2, PIN_INPUT, PullNone, 7) },
    { P10_3, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_INPUT, PullNone, 7) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_SCLK[] = {
    { P1_1,  EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P2_1,  EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P3_1,  EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P9_5,  EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P1_5,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { P6_3,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 5) },
    { P3_5,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 6) },
    { P8_1,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 7) },
    { P10_1, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 7) },
    { NC, NC, 0 }
};

const PinMap PinMap_SPI_SSEL[] = {
    { P1_0,  EUSCI_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 0) },
    { P2_0,  EUSCI_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P3_0,  EUSCI_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P9_4,  EUSCI_A3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P1_4,  EUSCI_B0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { P6_2,  EUSCI_B1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 5) },
    { P3_4,  EUSCI_B2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 6) },
    { P8_0,  EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 7) },
    { P10_0, EUSCI_B3_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 7) },
    { NC, NC, 0 }
};

const PinMap PinMap_PWM[] = {
    { P2_4,  TIMER_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P2_5,  TIMER_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P2_6,  TIMER_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P2_7,  TIMER_A0_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { P7_7,  TIMER_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P7_6,  TIMER_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P7_5,  TIMER_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P7_4,  TIMER_A1_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { P5_6,  TIMER_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 1) },
    { P5_7,  TIMER_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 2) },
    { P6_6,  TIMER_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 3) },
    { P6_7,  TIMER_A2_BASE, MSP432_PIN_DATA(SEL1, PIN_OUTPUT, PullNone, 4) },
    { NC, NC, 0 }
};

