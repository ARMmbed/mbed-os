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

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    /* A0-A5 pins */
    {PTA0,  ADC0_SE12, 0},
    {PTA8,  ADC0_SE3,  0},
    {PTA9,  ADC0_SE2,  0},
    {PTB8,  ADC0_SE11, 0},
    {PTB9,  ADC0_SE10, 0},
    {PTB13, ADC0_SE13, 0},
    /* Rest of pins ADC Mux */
    {PTB2, ADC0_SE4,  0},
    {PTB1, ADC0_SE5,  0},
    {PTB5, ADC0_SE1,  0},
    {PTA12, ADC0_SE0, 0},
    {PTB10, ADC0_SE9, 0},
    {PTB11, ADC0_SE8, 0},
    {PTB7, ADC0_SE7, 0},
    {PTB0, ADC0_SE6,  0},
    {NC,    NC,       0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {PTB1, DAC_0, 0},
    {NC  , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTB4, I2C_0, 2},
    {NC  , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTB3, I2C_0, 2},
    {NC  , NC   , 0}
};
/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTB1, UART_0, 2},
    {NC  , NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTB2, UART_0, 2},
    {NC  , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTB0, SPI_0, 3},
    {NC  , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTA7, SPI_0, 3},
    {NC  , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTA6, SPI_0, 3},
    {NC  , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTA5, SPI_0, 3},
    {NC  , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    // LEDs
    {LED_RED  , PWM_4 , 2}, // PTB8 , TPM0 CH3
    {LED_GREEN, PWM_3,  2}, // PTB9 , TPM0 CH2
    {LED_BLUE , PWM_2 , 2}, // PTB10, TPM0 CH1

    // Arduino digital pinout
    {D3,  PWM_8 , 2}, // PTB5 , TPM1 CH1
    {D5,  PWM_7 , 2}, // PTA12, TPM1 CH0
    {D6,  PWM_4 , 2}, // PTB6 , TPM0 CH3
    {D7,  PWM_3 , 2}, // PTB7 , TPM0 CH2
    {D8,  PWM_2 , 2}, // PTB10, TPM0 CH1
    {D9,  PWM_1 , 2}, // PTB11, TPM0 CH0
    {D10, PWM_6 , 2}, // PTA5 , TPM0 CH5
    {D12, PWM_5 , 2}, // PTA6 , TPM0 CH4
    {NC , NC    , 0}
};

