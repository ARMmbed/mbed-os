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

#ifndef MBED_PERIPHERALPINMAPS_H
#define MBED_PERIPHERALPINMAPS_H

#include <mstd_cstddef>

/************RTC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_ADC[] = {
    {PTA17, ADC1_SE17, 0},
    {PTB0, ADC0_SE8, 0},
    {PTB1, ADC0_SE9, 0},
    {PTB2, ADC0_SE12, 0},
    {PTB3, ADC0_SE13, 0},
    {PTB6, ADC1_SE12, 0},
    {PTB7, ADC1_SE13, 0},
    {PTB10, ADC1_SE14, 0},
    {PTB11, ADC1_SE15, 0},
    {PTC0, ADC0_SE14, 0},
    {PTC1, ADC0_SE15, 0},
    {PTC2,  ADC0_SE4b, 0},
    {PTC8,  ADC1_SE4b, 0},
    {PTC9,  ADC1_SE5b, 0},
    {PTC10, ADC1_SE6b, 0},
    {PTC11, ADC1_SE7b, 0},
    {PTD1,  ADC0_SE5b, 0},
    {PTD5,  ADC0_SE6b, 0},
    {PTD6,  ADC0_SE7b, 0},
    {PTE0,  ADC1_SE4a, 0},
    {PTE1,  ADC1_SE5a, 0},
    {PTE2,  ADC1_SE6a, 0},
    {PTE3,  ADC1_SE7a, 0},
    //{PTE24, ADC0_SE17, 0}, //I2C pull up
    //{PTE25, ADC0_SE18, 0}, //I2C pull up
    {NC, NC, 0}
};

/************DAC***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_DAC[] = {
    {DAC0_OUT, DAC_0, 0},
    {NC, NC, 0}
};

/************I2C***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SDA[] = {
    {PTE25, I2C_0, 5},
    {PTB1, I2C_0, 2},
    {PTB3, I2C_0, 2},
    {PTC11, I2C_1, 2},
    {PTA13, I2C_2, 5},
//    {PTD3, I2C_0, 7}, // Fails FPGA testing
    {PTE0, I2C_1, 6},
    {NC, NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_I2C_SCL[] = {
    {PTE24, I2C_0, 5},
    {PTB0, I2C_0, 2},
    {PTB2, I2C_0, 2},
    {PTC10, I2C_1, 2},
    {PTA12, I2C_2, 5},
    {PTA14, I2C_2, 5},
    {PTD2, I2C_0, 7},
    {PTE1, I2C_1, 6},
    {NC, NC, 0}
};

/************UART***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_TX[] = {
    {PTB17, UART_0, 3},
    {PTC17, UART_3, 3},
    {PTD7, UART_0, 3},
    {PTD3, UART_2, 3},
    {PTC4, UART_1, 3},
    {PTC15, UART_4, 3},
    {PTB11, UART_3, 3},
    {PTA14, UART_0, 3},
    {PTE24, UART_4, 3},
    {PTE4, UART_3, 3},
    {PTE0,  UART_1, 3},
    {NC,  NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RX[] = {
    {PTB16, UART_0, 3},
    {PTE1, UART_1, 3},
    {PTE5, UART_3, 3},
    {PTE25, UART_4, 3},
    {PTA15, UART_0, 3},
    {PTC16, UART_3, 3},
    {PTB10, UART_3, 3},
    {PTC3, UART_1, 3},
    {PTC14, UART_4, 3},
    {PTD2, UART_2, 3},
    {PTD6, UART_0, 3},
    {NC,  NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_CTS[] = {
    {PTB13, UART_3, 2},
    {PTE2, UART_1, 3},
    {PTE6, UART_3, 3},
    {PTE26, UART_4, 3},
    {PTA0, UART_0, 2},
    {PTA16, UART_0, 3},
    {PTB3, UART_0, 3},
    {PTB9, UART_3, 3},
    {PTC2, UART_1, 3},
    {PTC13, UART_4, 3},
    {PTC19, UART_3, 3},
    {PTD1, UART_2, 3},
    {PTD5, UART_0, 3},
    {NC, NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_UART_RTS[] = {
    {PTB12, UART_3, 2},
    {PTE3, UART_1, 3},
    {PTE7, UART_3, 3},
    {PTE27, UART_4, 3},
    {PTA17, UART_0, 3},
    {PTB8, UART_3, 3},
    {PTC1, UART_1, 3},
    {PTC12, UART_4, 3},
    {PTC18, UART_3, 3},
    {PTD0, UART_2, 3},
    {PTD4, UART_0, 3},
    {PTA3, UART_0, 2},
    {PTB2, UART_0, 3},
    {NC, NC, 0}
};

/************SPI***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SCLK[] = {
    {PTD1, SPI_0, 2},
    {PTE2, SPI_1, 2},
    {PTA15, SPI_0, 2},
    {PTB11, SPI_1, 2},
    {PTB21, SPI_2, 2},
    {PTC5, SPI_0, 2},
    {PTD5, SPI_1, 7},
    {NC, NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MOSI[] = {
    {PTD2, SPI_0, 2},
    {PTE1, SPI_1, 2},
    {PTE3, SPI_1, 7},
    {PTA16, SPI_0, 2},
    {PTB16, SPI_1, 2},
    {PTB22, SPI_2, 2},
    {PTC6, SPI_0, 2},
    {PTD6, SPI_1, 7},
    {NC, NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_MISO[] = {
    {PTD3, SPI_0, 2},
    {PTE1, SPI_1, 7},
    {PTE3, SPI_1, 2},
    {PTA17, SPI_0, 2},
    {PTB17, SPI_1, 2},
    {PTB23, SPI_2, 2},
    {PTC7, SPI_0, 2},
    {PTD7, SPI_1, 7},
    {NC, NC, 0}
};

MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_SPI_SSEL[] = {
    {PTD0, SPI_0, 2},
    {PTE4, SPI_1, 2},
    {PTA14, SPI_0, 2},
    {PTB10, SPI_1, 2},
    {PTB20, SPI_2, 2},
    {PTC4, SPI_0, 2},
    {PTD4, SPI_1, 7},
    {NC, NC, 0}
};

/************PWM***************/
MSTD_CONSTEXPR_OBJ_11 PinMap PinMap_PWM[] = {
    {PTA0, PWM_6, 3},
    {PTA1, PWM_7, 3},
    {PTA2, PWM_8, 3},
    {PTA3, PWM_1, 3},
    {PTA4, PWM_2, 3},
    {PTA5, PWM_3, 3},
    {PTA6, PWM_4, 3},
    {PTA7, PWM_5, 3},
    {PTA8, PWM_9, 3},
    {PTA9, PWM_10, 3},
    {PTA10, PWM_17, 3},
    {PTA11, PWM_18, 3},
    {PTA12, PWM_9, 3},
    {PTA13, PWM_10, 3},

    {PTB0, PWM_9, 3},
    {PTB1, PWM_10, 3},
    {PTB18, PWM_17, 3},
    {PTB19, PWM_18, 3},

    {PTC1, PWM_1, 4},
    {PTC2, PWM_2, 4},
    {PTC3, PWM_3, 4},
    {PTC4, PWM_4, 4},
    {PTC5, PWM_3, 7},
    {PTC8, PWM_29, 3},
    {PTC9, PWM_30, 3},
    {PTC10, PWM_31, 3},
    {PTC11, PWM_32, 3},

    {PTD0, PWM_25, 4},
    {PTD1, PWM_26, 4},
    {PTD2, PWM_27, 4},
    {PTD3, PWM_28, 4},
    {PTD4, PWM_5, 4},
    {PTD5, PWM_6, 4},
    {PTD6, PWM_7, 4},
    {PTD4, PWM_5, 4},
    {PTD7, PWM_8, 4},

    {PTE5, PWM_25, 6},
    {PTE6, PWM_26, 6},

    {NC, NC, 0}
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


#endif
