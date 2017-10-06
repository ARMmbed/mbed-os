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
    {PTE16, ADC0_SE4a, 0},
    {PTE17, ADC0_SE5a, 0},
    {PTE18, ADC0_SE6a, 0},
    {PTE19, ADC0_SE7a, 0},
    {PTB0,  ADC0_SE8,  0},
    {PTB1,  ADC0_SE9,  0},
    {PTA7,  ADC0_SE10, 0},
    {PTA8,  ADC0_SE11, 0},
    {PTB2,  ADC0_SE12, 0},
    {PTB3,  ADC0_SE13, 0},
    {PTC0,  ADC0_SE14, 0},
    {PTC1,  ADC0_SE15, 0},
    {PTC2,  ADC0_SE4b, 0},
    {PTD1,  ADC0_SE5b, 0},
    {PTD5,  ADC0_SE6b, 0},
    {PTD6,  ADC0_SE7b, 0},
    {NC   , NC       , 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {DAC0_OUT, DAC_0, 0},
    {NC      , NC   , 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTE0,  I2C_1, 6},
    {PTE10, I2C_3, 2},
    {PTE18, I2C_0, 4},
    {PTA21, I2C_0, 2},
    {PTA1,  I2C_3, 4},
    {PTA7,  I2C_2, 2},
    {PTA10, I2C_2, 2},
    {PTB1,  I2C_0, 2},
    {PTB3,  I2C_0, 2},
    {PTB11, I2C_2, 4},
    {PTC11, I2C_1, 2},
    {PTD3,  I2C_0, 7},
    {PTD9,  I2C_0, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTE1,  I2C_1, 6},
    {PTE11, I2C_3, 2},
    {PTE19, I2C_0, 4},
    {PTA20, I2C_0, 2},
    {PTA2,  I2C_3, 4},
    {PTA6,  I2C_2, 2},
    {PTA11, I2C_2, 2},
    {PTB0,  I2C_0, 2},
    {PTB2,  I2C_0, 2},
    {PTB10, I2C_2, 4},
    {PTC10, I2C_1, 2},
    {PTD2,  I2C_0, 7},
    {PTD8,  I2C_0, 2},
    {NC   , NC   , 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {PTE0,  LPUART_1, 3},
    {PTE4,  LPUART_3, 3},
    {PTE12, LPUART_2, 3},
    {PTE16, LPUART_2, 3},
    {PTA20, LPUART_4, 3},
    {PTA2,  LPUART_0, 2},
    {PTA14, LPUART_0, 3},
    {PTB11, LPUART_3, 3},
    {PTB17, LPUART_0, 3},
    {PTC4,  LPUART_1, 3},
    {PTC15, LPUART_4, 3},
    {PTC17, LPUART_3, 3},
    {PTD3,  LPUART_2, 3},
    {PTD7,  LPUART_0, 3},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_RX[] = {
    {PTE1,  LPUART_1, 3},
    {PTE5,  LPUART_3, 3},
    {PTE13, LPUART_2, 3},
    {PTE17, LPUART_2, 3},
    {PTA21, LPUART_4, 3},
    {PTA1,  LPUART_0, 2},
    {PTA15, LPUART_0, 3},
    {PTB10, LPUART_3, 3},
    {PTB16, LPUART_0, 3},
    {PTC3,  LPUART_1, 3},
    {PTC14, LPUART_4, 3},
    {PTC16, LPUART_3, 3},
    {PTD2,  LPUART_2, 3},
    {PTD6,  LPUART_0, 3},
    {NC  ,  NC    , 0}
};

const PinMap PinMap_UART_CTS[] = {
    {PTE2,  LPUART_1, 3},
    {PTE6,  LPUART_3, 3},
    {PTE18, LPUART_2, 3},
    {PTA0,  LPUART_0, 2},
    {PTA16, LPUART_0, 3},
    {PTB3,  LPUART_0, 3},
    {PTB9,  LPUART_3, 3},
    {PTC2,  LPUART_1, 3},
    {PTC13, LPUART_4, 3},
    {PTC19, LPUART_3, 3},
    {PTD1,  LPUART_2, 3},
    {PTD5,  LPUART_0, 3},
    {NC   , NC    , 0}
};

const PinMap PinMap_UART_RTS[] = {
    {PTE3,  LPUART_1, 3},
    {PTE7,  LPUART_3, 3},
    {PTE19, LPUART_2, 3},
    {PTA3,  LPUART_0, 2},
    {PTA17, LPUART_0, 3},
    {PTB2,  LPUART_0, 3},
    {PTB8,  LPUART_3, 3},
    {PTC1,  LPUART_1, 3},
    {PTC12, LPUART_4, 3},
    {PTC18, LPUART_3, 3},
    {PTD0,  LPUART_2, 3},
    {PTD4,  LPUART_0, 3},
    {NC   , NC    , 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTE1,  SPI_1, 2},
    {PTE2,  SPI_1, 7},
    {PTE7,  SPI_2, 2},
    {PTE17, SPI_0, 2},
    {PTA15, SPI_0, 2},
    {PTB11, SPI_1, 2},
    {PTB21, SPI_2, 2},
    {PTC5,  SPI_0, 2},
    {PTD1,  SPI_0, 2},
    {PTD5,  SPI_1, 7},
    {PTD12, SPI_2, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {PTE2,  SPI_1, 2},
    {PTE3,  SPI_1, 7},
    {PTE8,  SPI_2, 3},
    {PTE18, SPI_0, 2},
    {PTA16, SPI_0, 2},
    {PTB16, SPI_1, 2},
    {PTB16, SPI_1, 2},
    {PTB22, SPI_2, 2},
    {PTC6,  SPI_0, 2},
    {PTD2,  SPI_0, 2},
    {PTD6,  SPI_1, 7},
    {PTD13, SPI_2, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PTE1, SPI_1, 7},
    {PTE4, SPI_1, 2},
    {PTE10, SPI_2, 3},
    {PTE19, SPI_0, 2},
    {PTA17, SPI_0, 2},
    {PTB17, SPI_1, 2},
    {PTB23, SPI_2, 2},
    {PTC7,  SPI_0, 2},
    {PTD3,  SPI_0, 2},
    {PTD7,  SPI_1, 7},
    {PTD14, SPI_2, 2},
    {NC   , NC   , 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTE5,  SPI_1, 2},
    {PTE11, SPI_2, 3},
    {PTE16, SPI_0, 2},
    {PTA14, SPI_0, 2},
    {PTB10, SPI_1, 2},
    {PTB20, SPI_2, 2},
    {PTC4,  SPI_0, 2},
    {PTD0,  SPI_0, 2},
    {PTD4,  SPI_1, 7},
    {PTD11, SPI_2, 2},
    {NC   , NC   , 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    /* FTM 0 */
    {PTA0,  PWM_6,  3},
    {PTA1,  PWM_7,  3},
    {PTA2,  PWM_8,  3},
    {PTA3,  PWM_1,  3},
    {PTA4,  PWM_2,  3},
    {PTA5,  PWM_3,  3},
    {PTA6,  PWM_4,  3},
    {PTA7,  PWM_5,  3},
    {PTC1,  PWM_1,  4},
    {PTC2,  PWM_2,  4},
    {PTC3,  PWM_3,  4},
    {PTC4,  PWM_4,  4},
    {PTC5,  PWM_3,  7},
    {PTD4,  PWM_5,  4},
    {PTD5,  PWM_6,  4},
    {PTD6,  PWM_7,  4},
    {PTD7,  PWM_8,  4},
    /* FTM 1 */
    {PTA8,  PWM_9,  3},
    {PTA9,  PWM_10, 3},
    {PTA12, PWM_9,  3},
    {PTA13, PWM_10, 3},
    {PTB0,  PWM_9,  3},
    {PTB1,  PWM_10, 3},
    /* FTM 2 */
    {PTA10, PWM_11, 3},
    {PTA11, PWM_12, 3},
    {PTB18, PWM_11, 3},
    {PTB19, PWM_12, 3},
    /* FTM 3 */
    {PTE5,  PWM_13, 6},
    {PTE6,  PWM_14, 6},
    {PTE7,  PWM_15, 6},
    {PTE8,  PWM_16, 6},
    {PTE9,  PWM_17, 6},
    {PTE10, PWM_18, 6},
    {PTE11, PWM_19, 6},
    {PTE12, PWM_20, 6},
    {PTC8,  PWM_17, 3},
    {PTC9,  PWM_18, 3},
    {PTC10, PWM_19, 3},
    {PTC11, PWM_20, 3},
    {PTD0,  PWM_13, 4},
    {PTD1,  PWM_14, 4},
    {PTD2,  PWM_15, 4},
    {PTD3,  PWM_16, 4},

    {NC   , NC    , 0}
};
