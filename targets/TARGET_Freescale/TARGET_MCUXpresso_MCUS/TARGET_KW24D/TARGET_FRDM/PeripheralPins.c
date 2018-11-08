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
    {PTD1,  ADC0_SE5b, 0},
    {PTD4,  ADC0_SE21, 0},
    {PTD5,  ADC0_SE6b, 0},
    {PTD6,  ADC0_SE7b, 0},
    {PTD7,  ADC0_SE22, 0},
    {PTE0,  ADC0_SE10, 0},
    {PTE1,  ADC0_SE11, 0},
    {PTE16, ADC0_SE4a, 0},
    {PTE17, ADC0_SE5a, 0},
    {PTE18, ADC0_SE6a, 0},
    {PTE19, ADC0_SE7a, 0},
    {NC, NC, 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTD3,  I2C_0, 4},
    {PTE0,  I2C_1, 6},
    {PTE18, I2C_0, 4},
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTD2,  I2C_0, 4},
    {PTE1,  I2C_1, 6},
    {PTE19, I2C_0, 4},
    {NC, NC, 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTA2,  UART_0, 2},
    {PTD7,  UART_0, 3},
    {PTC4,  UART_1, 3},
    {PTE0,  UART_1, 3},
    {PTD3,  UART_2, 3},
    {PTE16, UART_2, 3},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTA1,  UART_0, 2},
    {PTD6,  UART_0, 3},
    {PTE1,  UART_1, 3},
    {PTD2,  UART_2, 3},
    {PTE17, UART_2, 3},

    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {PTD1,  UART_2, 3},
    {PTD5,  UART_0, 3},
    {PTE2,  UART_1, 3},
    {PTE18, UART_2, 3},
    {PTA0,  UART_0, 2},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    {PTD4,  UART_0, 3},
    {PTE3, UART_1, 3},
    {PTE19, UART_2, 3},
    {PTA3,  UART_0, 2},
    {NC, NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTB11, SPI_1, 2},
    {PTC5,  SPI_0, 2},
    {PTD1,  SPI_0, 2},
    {PTE2,  SPI_1, 2},
    {PTE17, SPI_0, 2},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTB16, SPI_1, 2},
    {PTC6,  SPI_0, 2},
    {PTD2,  SPI_0, 2},
    {PTE1,  SPI_1, 2},
    {PTE3,  SPI_1, 7},
    {PTE18, SPI_0, 2},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTB17, SPI_1, 2},
    {PTC7,  SPI_0, 2},
    {PTD3,  SPI_0, 2},
    {PTE1,  SPI_1, 7},
    {PTE3,  SPI_1, 2},
    {PTE19, SPI_0, 2},
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTB10, SPI_1, 2},
    {PTC4,  SPI_0, 2},
    {PTE4,  SPI_1, 2},
    {PTE16, SPI_0, 2},
    {NC, NC, 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PTC4,  PWM_3, 4},
    {PTD4,  PWM_4, 4},
    {PTD5,  PWM_5, 4},
    {PTD6,  PWM_6, 4},
    {PTD7,  PWM_7, 4},
    {PTA0,  PWM_5, 3},
    {PTA1,  PWM_6, 3},
    {PTA2,  PWM_7, 3},
    {PTA3,  PWM_0, 3},
    {PTA4,  PWM_1, 3},
    {NC, NC, 0}
};
