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

/***************************************************************** Includes **/
#include "PeripheralPins.h"
#include "PinNames.h"

/********************************************* Peripheral Pins declarations **/

/**
 * UART Pin Map TX
 */
const PinMap PinMap_UART_TX[] = {
    {P1_5,  UART_0, 2},
    {P1_7,  UART_0, 2},
    {P5_1,  UART_0, 1},
    {P2_5,  UART_1, 2},
    {P3_5,  UART_1, 4},
    {P0_5,  UART_2, 2},
    {P2_14, UART_2, 2},
    {P0_1,  UART_3, 2},
    {P5_0,  UART_4, 1},
    {P3_5,  UART_5, 1},

    /* Not connected */
	{NC,    NC    , 0}
};

/**
 * UART Pin Map RX
 */
const PinMap PinMap_UART_RX[] = {
    {P1_5,  UART_0, 0},
    {P1_4,  UART_0, 0},
    {P5_0,  UART_0, 0},
    {P2_2,  UART_1, 0},
    {P2_5,  UART_1, 0},
    {P4_0,  UART_1, 0},
    {P0_4,  UART_2, 0},
    {P0_5,  UART_2, 0},
    {P0_0,  UART_3, 0},
    {P5_1,  UART_4, 0},
    {P5_0,  UART_4, 0},
    {P3_5,  UART_5, 0},
    {P3_4,  UART_5, 0},
    {P4_0,  UART_5, 0},

    /* Not connected */
    {NC,    NC    , 0}
};

/**
 * ERU_0 Pin Map
 */
const PinMap PinMap_ERU_0[] = {
    {P0_0,  ERU0_0, 0},
    {P0_1,  ERU0_0, 0},
    {P0_2,  ERU0_3, 0},
    {P0_4,  ERU0_2, 0},
    {P0_6,  ERU0_3, 0},
    {P0_7,  ERU0_2, 0},
    {P0_8,  ERU0_2, 0},
    {P0_9,  ERU0_1, 0},
    {P0_10, ERU0_1, 0},
    {P0_11, ERU0_3, 0},
    {P0_12, ERU0_2, 0},
    {P0_13, ERU0_2, 0},
    {P1_0,  ERU0_3, 0},
    {P1_1,  ERU0_3, 0},
    {P1_4,  ERU0_2, 0},
    {P1_5,  ERU0_2, 0},
    {P2_0,  ERU0_0, 0},
    {P2_2,  ERU0_1, 0},
    {P2_3,  ERU0_1, 0},
    {P2_4,  ERU0_0, 0},
    {P2_5,  ERU0_0, 0},
    {P2_6,  ERU0_1, 0},
    {P3_1,  ERU0_0, 0},
    {P3_2,  ERU0_0, 0},
    {P3_5,  ERU0_3, 0},
    {P3_6,  ERU0_3, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

/**
 * ERU_1 Pin Map
 */
const PinMap PinMap_ERU_1[] = {
    {P0_3,  ERU1_3, 0},
    {P0_5,  ERU1_3, 0},
    {P1_2,  ERU1_2, 0},
    {P1_3,  ERU1_2, 0},
    {P1_5,  ERU1_0, 0},
    {P1_15, ERU1_1, 0},
    {P2_1,  ERU1_0, 0},
    {P2_7,  ERU1_1, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

/**
 * DAC Pin Map
 */
const PinMap PinMap_DAC[] = {
    {P14_8,  DAC0, 0},
    {P14_9,  DAC1, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

/**
 * ADC Pin Map
 */
const PinMap PinMap_ADC[] = {
    {P14_0,  ADC0_0, 0},
    {P14_1,  ADC0_1 , 0},
    {P14_2,  ADC0_2, 0},
    {P14_3,  ADC0_3, 0},
    {P14_4,  ADC0_4, 0},
    {P14_5,  ADC0_5, 0},
    {P14_6,  ADC0_6, 0},
    {P14_7,  ADC0_7, 0},
    {P14_8,  ADC1_0, 0},
    {P14_9,  ADC1_1, 0},
    {P14_12, ADC1_4, 0},
    {P14_13, ADC1_5, 0},
    {P14_14, ADC1_6, 0},
    {P14_15, ADC1_7, 0},
    {P15_2,  ADC2_2, 0},
    {P15_3,  ADC2_3, 0},
    {P15_8,  ADC3_0, 0},
    {P15_9,  ADC3_1, 0},

    /* Not connected */
    {NC  , NC   , NC}
};

/**
 * PWM Pin Map
 */
const PinMap PinMap_PWM[] = {
    {P0_0,  PWM_2, 3},
    {P0_1,  PWM_1, 3},
    {P0_2,  PWM_0, 3},
    {P0_3,  PWM_2, 3},
    {P0_4,  PWM_1, 3},
    {P0_5,  PWM_0, 3},
    {P0_6,  PWM_3, 3},
    {P0_9,  PWM_1, 3},
    {P0_10, PWM_0, 3},
    {P0_11, PWM_3, 3},
    {P0_12, PWM_11, 3},
    {P1_0,  PWM_11, 3},
    {P1_1,  PWM_10, 3},
    {P1_2,  PWM_9, 3},
    {P1_3,  PWM_8, 3},
    {P1_4,  PWM_3, 3},
    {P1_5,  PWM_2, 3},
    {P1_10, PWM_6, 3},
    {P1_11, PWM_5, 3},
    {P1_12, PWM_4, 3},
    {P1_13, PWM_6, 3},
    {P1_14, PWM_5, 3},
    {P1_15, PWM_4, 3},
    {P2_1,  PWM_5, 2},
    {P2_6,  PWM_1, 3},
    {P2_14, PWM_2, 3},
    {P2_15, PWM_1, 3},
    {P3_0,  PWM_16, 3},
    {P3_3,  PWM_19, 3},
    {P3_4,  PWM_18, 3},
    {P5_0,  PWM_7, 3},
    {P5_1,  PWM_7, 3},
    {P5_2,  PWM_6, 3},
    {P5_7,  PWM_4, 3},

    /* Not connected */
    {NC  , NC   , NC}
};

/*
 * I2C_SDA Pin Map
 */
const PinMap PinMap_I2C_SDA[] = {
    {P5_0,  I2C_4, 1},
    {P1_5,  I2C_0, 2},
    {P0_5,  I2C_2, 2},

    /* Not connected */
    {NC  ,  NC   , 0}
};

/*
 * I2C_SCL Pin Map
 */
const PinMap PinMap_I2C_SCL[] = {
    {P5_2,  I2C_4, 1},
    {P1_10, I2C_0, 2},
    {P0_11, I2C_2, 2},

    /* Not connected */
    {NC  ,  NC,    0}
};

/*
 * SPI_MOSI Pin Map
 */
const PinMap PinMap_SPI_MOSI[] = {
    {P0_1,  SPI_3, 2},
    {P5_0,  SPI_4, 1},
    {P1_5,  SPI_0, 2},
    {P0_5,  SPI_2, 2},

    /* Not connected */
    {NC  ,  NC,    0}
};

/*
 * SPI_MISO Pin Map
 */
const PinMap PinMap_SPI_MISO[] = {
    {P0_0,  SPI_3, 0},
    {P5_1,  SPI_4, 0},
    {P1_4,  SPI_0, 0},
    {P0_4,  SPI_2, 0},

    /* Not connected */
    {NC  ,  NC,    0}
};

/*
 * SPI_SCLK Pin Map
 */
const PinMap PinMap_SPI_SCLK[] = {
    {P0_10, SPI_3, 2},
    {P5_2,  SPI_4, 1},
    {P1_10, SPI_0, 2},
    {P0_11, SPI_2, 2},

    /* Not connected */
    {NC  ,  NC,    0}
};

/*
 * SPI_SSEL Pin Map
 */
const PinMap PinMap_SPI_SSEL[] = {
    {P0_9,  SPI_3, 2},
    {P3_3,  SPI_3, 2},
    {P3_4,  SPI_3, 2},
    {P3_5,  SPI_3, 2},
    {P3_6,  SPI_3, 2},
    {P2_6,  SPI_4, 1},
    {P1_11, SPI_0, 2},
    {P1_8,  SPI_0, 2},
    {P0_6,  SPI_2, 2},

    /* Not connected */
    {NC  ,  NC,    0}
};

/*EOF*/
