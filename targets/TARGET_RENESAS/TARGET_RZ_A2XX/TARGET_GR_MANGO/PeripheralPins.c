
/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

//#define ISEL_BIT    (0x40)  // use interrupt

/************IRQ***************/
enum {
    IRQ0, IRQ1,
    IRQ2, IRQ3,
    IRQ4, IRQ5,
    IRQ6, IRQ7,
} IRQNo;

const PinMap PinMap_IRQ[] = {
    {P6_2,  IRQ0,   6}, {PL_4,  IRQ0,   5}, {PD_0,  IRQ0,   2},
    {PJ_6,  IRQ0,   5}, {PJ_1,  IRQ0,   6}, {P5_4,  IRQ0,   2},
    {P1_0,  IRQ0,   3}, {P4_0,  IRQ0,   6}, {PC_5,  IRQ0,   6},
    {PF_4,  IRQ1,   6}, {PE_1,  IRQ1,   6}, {PD_1,  IRQ1,   2},
    {PF_7,  IRQ1,   5}, {P1_1,  IRQ1,   3}, {PC_4,  IRQ1,   6},
    {P5_5,  IRQ1,   2}, {P4_1,  IRQ1,   6}, {P8_2,  IRQ2,   5},
    {PH_1,  IRQ2,   6}, {PD_2,  IRQ2,   2}, {PH_4,  IRQ2,   6},
    {P1_2,  IRQ2,   3}, {P5_6,  IRQ2,   2}, {PC_0,  IRQ2,   5},
    {P4_2,  IRQ2,   6},
    {PH_0,  IRQ3,   6}, {PD_3,  IRQ3,   2}, {P8_1,  IRQ3,   5},
    {PH_3,  IRQ3,   6}, {P4_3,  IRQ3,   6}, {P5_7,  IRQ3,   2},
    {P3_0,  IRQ3,   5}, {P1_3,  IRQ3,   3},
    {PL_0,  IRQ4,   5}, {PF_1,  IRQ4,   6}, {PD_4,  IRQ4,   2},
    {PG_2,  IRQ4,   6}, {PH_6,  IRQ4,   5}, {PJ_5,  IRQ4,   6},
    {P1_4,  IRQ4,   3}, {P5_0,  IRQ4,   2},
    {PL_1,  IRQ5,   5}, {PA_5,  IRQ5,   6}, {PK_2,  IRQ5,   6},
    {PD_5,  IRQ5,   2}, {PH_5,  IRQ5,   5}, {PG_6,  IRQ5,   6},
    {P2_0,  IRQ5,   3}, {P5_1,  IRQ5,   2},
    {PL_2,  IRQ6,   5}, {PA_1,  IRQ6,   6}, {PD_6,  IRQ6,   2},
    {PK_4,  IRQ6,   6}, {P3_1,  IRQ6,   6}, {P5_2,  IRQ6,   2},
    {PC_7,  IRQ6,   6}, {P2_1,  IRQ6,   3},
    {PL_3,  IRQ7,   5}, {PD_7,  IRQ7,   2}, {P3_3,  IRQ7,   6},
    {P2_2,  IRQ7,   3}, {PC_6,  IRQ7,   6}, {P5_3,  IRQ7,   2},
    {NC,    NC,     0}
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {P5_0,  AN0,    1},
    {P5_1,  AN1,    1},
    {P5_2,  AN2,    1},
    {P5_3,  AN3,    1},
    {P5_4,  AN4,    1},
    {P5_5,  AN5,    1},
    {P5_6,  AN6,    1},
    {P5_7,  AN7,    1},
    {NC,    NC,     0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PD_1,  I2C_0,  1},
    {PD_3,  I2C_1,  1},
    {PD_5,  I2C_2,  1},
    {PD_7,  I2C_3,  1},
    {NC,    NC,     0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PD_0,  I2C_0,  1},
    {PD_2,  I2C_1,  1},
    {PD_4,  I2C_2,  1},
    {PD_6,  I2C_3,  1},
    {NC,    NC,     0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {P8_5,  UART0,  5},
    {P4_2,  UART0,  1},
    {P7_3,  UART1,  4},
    {PJ_2,  UART1,  4},
    {PF_5,  UART2,  1},
    {PE_2,  UART2,  3},
    {P6_3,  UART3,  3},
    {PF_2,  UART3,  1},
    {P9_0,  UART4,  4},
    {P4_6,  UART4,  4},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {P7_7,  UART0,  5},
    {P4_1,  UART0,  1},
    {P7_1,  UART1,  4},
    {PJ_1,  UART1,  4},
    {PF_4,  UART2,  1},
    {PE_1,  UART2,  3},
    {P6_2,  UART3,  3},
    {PF_1,  UART3,  1},
    {P9_1,  UART4,  4},
    {P4_5,  UART4,  4},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_CTS[] = {
    {PB_3,  UART0,  5},
    {P4_4,  UART0,  1},
    {P7_5,  UART1,  4},
    {PJ_4,  UART1,  4},
    {PH_2,  UART2,  1},
    {PH_4,  UART2,  2},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {PB_4,  UART0,  5},
    {P4_3,  UART0,  1},
    {P7_4,  UART1,  4},
    {PJ_3,  UART1,  4},
    {PF_6,  UART2,  1},
    {PH_3,  UART2,  2},
    {NC,    NC,     0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {P8_7,  SPI_0,  4},
    {PG_0,  SPI_0,  3},
    {PK_2,  SPI_0,  5},
    {PF_0,  SPI_1,  5},
    {PG_4,  SPI_1,  3},
    {P3_1,  SPI_2,  5},
    {PC_0,  SPI_2,  4},
    {NC,    NC,     0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {P8_6,  SPI_0,  4},
    {PK_3,  SPI_0,  5},
    {PG_1,  SPI_0,  3},
    {PF_1,  SPI_1,  5},
    {PG_5,  SPI_1,  3},
    {P3_2,  SPI_2,  5},
    {PC_1,  SPI_2,  4},
    {NC,    NC,     0}
};

const PinMap PinMap_SPI_MISO[] = {
    {P8_5,  SPI_0,  4},
    {PG_2,  SPI_0,  3},
    {PK_4,  SPI_0,  5},
    {PF_2,  SPI_1,  5},
    {PG_6,  SPI_1,  3},
    {PC_2,  SPI_2,  4},
    {P3_3,  SPI_2,  5},
    {NC,    NC,     0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {P8_4,  SPI_0,  4},
    {PG_3,  SPI_0,  3},
    {P3_5,  SPI_0,  5},
    {PF_3,  SPI_1,  5},
    {PG_7,  SPI_1,  3},
    {P3_4,  SPI_2,  5},
    {PC_3,  SPI_2,  4},
    {NC,    NC,     0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PG_2,  PWM_GTIOC0A, 5},
    {PG_3,  PWM_GTIOC0B, 5},
    {PG_4,  PWM_GTIOC1A, 5},
    {PG_5,  PWM_GTIOC1B, 5},
    {PG_6,  PWM_GTIOC2A, 5},
    {PG_7,  PWM_GTIOC2B, 5},
    {P8_3,  PWM_GTIOC3A, 5},
    {P7_6,  PWM_GTIOC3A, 4},
    {P7_7,  PWM_GTIOC3B, 4},
    {P0_0,  PWM_GTIOC3B, 5},
    {PH_0,  PWM_GTIOC4A, 3},
    {P0_1,  PWM_GTIOC4A, 5},
    {PH_1,  PWM_GTIOC4B, 3},
    {P0_2,  PWM_GTIOC4B, 5},
    {P8_2,  PWM_GTIOC5A, 4},
    {P8_1,  PWM_GTIOC5B, 4},
    {PH_3,  PWM_GTIOC6A, 3},
    {P0_3,  PWM_GTIOC6A, 5},
    {P2_0,  PWM_GTIOC6A, 2},
    {PH_4,  PWM_GTIOC6B, 3},
    {P0_4,  PWM_GTIOC6B, 5},
    {P2_1,  PWM_GTIOC6B, 2},
    {P0_5,  PWM_GTIOC7A, 5},
    {P2_2,  PWM_GTIOC7A, 2},
    {P0_6,  PWM_GTIOC7B, 5},
    {P2_3,  PWM_GTIOC7B, 2},
    {NC,    NC,     0}
};

/************CAN***************/
const PinMap PinMap_CAN_RD[] = {
    {P1_1, CAN_0, 4},
    {P2_0, CAN_1, 4},
    {NC, NC, 0}
};

const PinMap PinMap_CAN_TD[] = {
    {P1_3, CAN_0, 4},
    {P2_2, CAN_1, 4},
    {NC, NC, 0}
};

