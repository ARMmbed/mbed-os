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

#ifndef MBED_PERIPHERALPINMAPS_H
#define MBED_PERIPHERALPINMAPS_H

#include <mstd_cstddef>

/************RTC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_ADC[] = {
    {P0_23, ADC0_SE0,    0},
    {P0_10, ADC0_SE1,    0},
    {P0_31, ADC0_SE3,    0},
    {P1_8,  ADC0_SE4,    0},
    {P2_0,  ADC0_SE5,    0},
    {P2_13, ADC0_SE6,    0},
    {P2_11, ADC0_SE7,    0},
    {NC   , NC      ,    0}
};

/************CAN***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_CAN_TD[] = {
    {NC   , NC   ,  0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_CAN_RD[] = {
    {NC   , NC   ,  0}
};


/************DAC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_DAC[] = {
    {NC      , NC   , 0}
};

/************I2C***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SDA[] = {
    {P0_13, I2C_0, 1},
    {P1_21, I2C_1, 5},
    {NC   , NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SCL[] = {
    {P0_14, I2C_0, 1},
    {P1_20, I2C_1, 5},
    {NC   , NC   , 0}
};

/************UART***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_TX[] = {
    {P0_30, UART_0, 1},
    {P1_6,  UART_0, 1},
    {P0_27, UART_1, 1},
    {NC   ,  NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RX[] = {
    {P0_29, UART_0, 1},
    {P1_5,  UART_0, 1},
    {P1_24, UART_1, 1},
    {NC   ,  NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_CTS[] = {
    {P1_8,  UART_0, 1},
    {P1_26, UART_1, 1},
    {NC   , NC    , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RTS[] = {
    {P1_7,  UART_0, 1},
    {P1_27, UART_1, 1},
    {NC   , NC    , 0}
};

/************SPI***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SCLK[] = {
    {P0_6,  SPI_0, 1},
    {P0_21, SPI_1, 7},
    {P1_2,  SPI_2, 6},
    {NC   , NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MOSI[] = {
    {P0_3,  SPI_0, 1},
    {P0_20, SPI_1, 7},
    {P0_26, SPI_2, 9},
    {NC   , NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MISO[] = {
    {P0_2,  SPI_0, 1},
    {P0_19, SPI_1, 7},
    {P1_3,  SPI_2, 6},
    {NC   , NC   , 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SSEL[] = {
    {P0_4,  SPI_0, 8},
    {P1_20, SPI_1, ((1 << SSELNUM_SHIFT) | 1)},
    {P1_1,  SPI_2, ((1 << SSELNUM_SHIFT) | 5)},
    {NC  ,  NC   , 0}
};

/************PWM***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_PWM[] = {
    {NC   , NC    , 0}
};


#define PINMAP_ANALOGIN PinMap_ADC
#define PINMAP_ANALOGOUT PinMap_DAC
#define PINMAP_I2C_SDA PinMap_I2C_SDA
#define PINMAP_I2C_SCL PinMap_I2C_SCL
#define PINMAP_UART_TX PinMap_UART_TX
#define PINMAP_UART_RX PinMap_UART_RX
#define PINMAP_UART_CTS PinMap_UART_CTS
#define PINMAP_UART_RTS PinMap_UART_RTS
#define PINMAP_SPI_SCLK PinMap_SPI_SCLK
#define PINMAP_SPI_MOSI PinMap_SPI_MOSI
#define PINMAP_SPI_MISO PinMap_SPI_MISO
#define PINMAP_SPI_SSEL PinMap_SPI_SSEL
#define PINMAP_PWM PinMap_PWM
#define PINMAP_CAN_TD PinMap_CAN_TD
#define PINMAP_CAN_RD PinMap_CAN_RD

#endif
