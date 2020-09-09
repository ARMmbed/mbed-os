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
// SPDX-License-Identifier: Apache-2.0
#include "PeripheralPins.h"
#include "PeripheralPinConfigs.h"

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, 0, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {NC, NC, 0}
};

/************DAC***************/
const PinMap PinMap_DAC[] = {
    {NC, NC, 0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {AP3_PER_IOM0_SDA, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SDA},
    {AP3_PER_IOM1_SDA, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SDA},
    {AP3_PER_IOM2_SDA, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SDA},
    {AP3_PER_IOM4_SDA, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SDA},
    {AP3_PER_IOM5_SDA, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SDA},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SDA, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SDA},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_I2C_SCL[] = {
    {AP3_PER_IOM0_SCL, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SCL},
    {AP3_PER_IOM1_SCL, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SCL},
    {AP3_PER_IOM2_SCL, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SCL},
    {AP3_PER_IOM4_SCL, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SCL},
    {AP3_PER_IOM5_SCL, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SCL},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SCL, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SCL},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {AP3_PER_UART0_TX_1, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_1},
    {AP3_PER_UART0_TX_7, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_7},
    {AP3_PER_UART0_TX_16, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_16},
    {AP3_PER_UART0_TX_20, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_20},
    {AP3_PER_UART0_TX_22, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_22},
    {AP3_PER_UART0_TX_26, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_26},
    {AP3_PER_UART0_TX_28, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_28},
    {AP3_PER_UART0_TX_39, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_39},
    {AP3_PER_UART0_TX_41, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_41},
    {AP3_PER_UART0_TX_44, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_44},
    {AP3_PER_UART0_TX_48, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_48},
    {AP3_PER_UART1_TX_8, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_8},
    {AP3_PER_UART1_TX_10, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_10},
    {AP3_PER_UART1_TX_12, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_12},
    {AP3_PER_UART1_TX_14, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_14},
    {AP3_PER_UART1_TX_18, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_18},
    {AP3_PER_UART1_TX_20, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_20},
    {AP3_PER_UART1_TX_24, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_24},
    {AP3_PER_UART1_TX_39, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_39},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_UART0_TX_30, UART_0, (uint32_t) &g_AP3_PER_UART0_TX_30},
    {AP3_PER_UART1_TX_35, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_35},
    {AP3_PER_UART1_TX_37, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_37},
    {AP3_PER_UART1_TX_42, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_42},
    {AP3_PER_UART1_TX_46, UART_1, (uint32_t) &g_AP3_PER_UART1_TX_46},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {AP3_PER_UART0_RX_2, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_2},
    {AP3_PER_UART0_RX_11, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_11},
    {AP3_PER_UART0_RX_17, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_17},
    {AP3_PER_UART0_RX_21, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_21},
    {AP3_PER_UART0_RX_23, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_23},
    {AP3_PER_UART0_RX_27, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_27},
    {AP3_PER_UART0_RX_29, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_29},
    {AP3_PER_UART0_RX_40, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_40},
    {AP3_PER_UART0_RX_49, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_49},
    {AP3_PER_UART1_RX_2, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_2},
    {AP3_PER_UART1_RX_4, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_4},
    {AP3_PER_UART1_RX_9, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_9},
    {AP3_PER_UART1_RX_13, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_13},
    {AP3_PER_UART1_RX_15, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_15},
    {AP3_PER_UART1_RX_19, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_19},
    {AP3_PER_UART1_RX_21, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_21},
    {AP3_PER_UART1_RX_25, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_25},
    {AP3_PER_UART1_RX_40, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_40},
    {AP3_PER_UART1_RX_47, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_47},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_UART0_RX_31, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_31},
    {AP3_PER_UART0_RX_34, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_34},
    {AP3_PER_UART0_RX_45, UART_0, (uint32_t) &g_AP3_PER_UART0_RX_45},
    {AP3_PER_UART1_RX_36, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_36},
    {AP3_PER_UART1_RX_38, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_38},
    {AP3_PER_UART1_RX_43, UART_1, (uint32_t) &g_AP3_PER_UART1_RX_43},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    {3, UART_0, AM_HAL_PIN_3_UART0RTS},
    {5, UART_0, AM_HAL_PIN_5_UART0RTS},
    {13, UART_0, AM_HAL_PIN_13_UART0RTS},
    {18, UART_0, AM_HAL_PIN_18_UART0RTS},
    {41, UART_0, AM_HAL_PIN_41_UART0RTS},
    {10, UART_1, AM_HAL_PIN_10_UART1RTS},
    {16, UART_1, AM_HAL_PIN_16_UART1RTS},
    {20, UART_1, AM_HAL_PIN_20_UART1RTS},
    {41, UART_1, AM_HAL_PIN_41_UART1RTS},
    {44, UART_1, AM_HAL_PIN_44_UART1RTS},
#if defined (AM_PACKAGE_BGA)
    {34, UART_0, AM_HAL_PIN_34_UART0RTS},
    {35, UART_0, AM_HAL_PIN_35_UART0RTS},
    {37, UART_0, AM_HAL_PIN_37_UART0RTS},
    {30, UART_1, AM_HAL_PIN_30_UART1RTS},
    {31, UART_1, AM_HAL_PIN_31_UART1RTS},
    {34, UART_1, AM_HAL_PIN_34_UART1RTS},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {4, UART_0, AM_HAL_PIN_4_UART0CTS},
    {6, UART_0, AM_HAL_PIN_6_UART0CTS},
    {12, UART_0, AM_HAL_PIN_12_UART0CTS},
    {24, UART_0, AM_HAL_PIN_24_UART0CTS},
    {29, UART_0, AM_HAL_PIN_29_UART0CTS},
    {11, UART_1, AM_HAL_PIN_11_UART1CTS},
    {17, UART_1, AM_HAL_PIN_17_UART1CTS},
    {21, UART_1, AM_HAL_PIN_21_UART1CTS},
    {26, UART_1, AM_HAL_PIN_26_UART1CTS},
    {29, UART_1, AM_HAL_PIN_29_UART1CTS},
#if defined (AM_PACKAGE_BGA)
    {33, UART_0, AM_HAL_PIN_33_UART0CTS},
    {36, UART_0, AM_HAL_PIN_36_UART0CTS},
    {38, UART_0, AM_HAL_PIN_38_UART0CTS},
    {32, UART_1, AM_HAL_PIN_32_UART1CTS},
    {36, UART_1, AM_HAL_PIN_36_UART1CTS},
    {45, UART_1, AM_HAL_PIN_45_UART1CTS},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {AP3_PER_IOM0_SCK, IOM_0, (uint32_t) &g_AP3_PER_IOM0_SCK},
    {AP3_PER_IOM1_SCK, IOM_1, (uint32_t) &g_AP3_PER_IOM1_SCK},
    {AP3_PER_IOM2_SCK, IOM_2, (uint32_t) &g_AP3_PER_IOM2_SCK},
    {AP3_PER_IOM4_SCK, IOM_4, (uint32_t) &g_AP3_PER_IOM4_SCK},
    {AP3_PER_IOM5_SCK, IOM_5, (uint32_t) &g_AP3_PER_IOM5_SCK},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_SCK, IOM_3, (uint32_t) &g_AP3_PER_IOM3_SCK},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
    {AP3_PER_IOM0_MOSI, IOM_0, (uint32_t) &g_AP3_PER_IOM0_MOSI},
    {AP3_PER_IOM1_MOSI, IOM_1, (uint32_t) &g_AP3_PER_IOM1_MOSI},
    {AP3_PER_IOM2_MOSI, IOM_2, (uint32_t) &g_AP3_PER_IOM2_MOSI},
    {AP3_PER_IOM4_MOSI, IOM_4, (uint32_t) &g_AP3_PER_IOM4_MOSI},
    {AP3_PER_IOM5_MOSI, IOM_5, (uint32_t) &g_AP3_PER_IOM5_MOSI},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_MOSI, IOM_3, (uint32_t) &g_AP3_PER_IOM3_MOSI},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_MISO[] = {
    {AP3_PER_IOM0_MISO, IOM_0, (uint32_t) &g_AP3_PER_IOM0_MISO},
    {AP3_PER_IOM1_MISO, IOM_1, (uint32_t) &g_AP3_PER_IOM1_MISO},
    {AP3_PER_IOM2_MISO, IOM_2, (uint32_t) &g_AP3_PER_IOM2_MISO},
    {AP3_PER_IOM4_MISO, IOM_4, (uint32_t) &g_AP3_PER_IOM4_MISO},
    {AP3_PER_IOM5_MISO, IOM_5, (uint32_t) &g_AP3_PER_IOM5_MISO},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_IOM3_MISO, IOM_3, (uint32_t) &g_AP3_PER_IOM3_MISO},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {AP3_PER_NCE_0, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_0},
    {AP3_PER_NCE_1, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_1},
    {AP3_PER_NCE_2, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_2},
    {AP3_PER_NCE_3, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_3},
    {AP3_PER_NCE_4, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_4},
    {AP3_PER_NCE_7, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_7},
    {AP3_PER_NCE_8, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_8},
    {AP3_PER_NCE_9, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_9},
    {AP3_PER_NCE_10, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_10},
    {AP3_PER_NCE_11, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_11},
    {AP3_PER_NCE_12, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_12},
    {AP3_PER_NCE_13, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_13},
    {AP3_PER_NCE_14, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_14},
    {AP3_PER_NCE_15, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_15},
    {AP3_PER_NCE_16, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_16},
    {AP3_PER_NCE_17, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_17},
    {AP3_PER_NCE_18, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_18},
    {AP3_PER_NCE_19, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_19},
    {AP3_PER_NCE_20, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_20},
    {AP3_PER_NCE_21, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_21},
    {AP3_PER_NCE_22, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_22},
    {AP3_PER_NCE_23, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_23},
    {AP3_PER_NCE_24, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_24},
    {AP3_PER_NCE_25, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_25},
    {AP3_PER_NCE_26, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_26},
    {AP3_PER_NCE_27, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_27},
    {AP3_PER_NCE_28, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_28},
    {AP3_PER_NCE_29, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_29},
    {AP3_PER_NCE_41, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_41},
    {AP3_PER_NCE_44, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_44},
    {AP3_PER_NCE_47, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_47},
    {AP3_PER_NCE_48, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_48},
    {AP3_PER_NCE_49, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_49},
#if defined (AM_PACKAGE_BGA)
    {AP3_PER_NCE_30, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_30},
    {AP3_PER_NCE_31, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_31},
    {AP3_PER_NCE_32, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_32},
    {AP3_PER_NCE_33, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_33},
    {AP3_PER_NCE_34, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_34},
    {AP3_PER_NCE_35, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_35},
    {AP3_PER_NCE_36, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_36},
    {AP3_PER_NCE_37, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_37},
    {AP3_PER_NCE_38, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_38},
    {AP3_PER_NCE_42, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_42},
    {AP3_PER_NCE_43, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_43},
    {AP3_PER_NCE_45, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_45},
    {AP3_PER_NCE_46, IOM_ANY, (uint32_t) &g_AP3_PER_NCE_46},
#endif // defined (AM_PACKAGE_BGA)
    {NC, NC, 0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {NC, NC, 0}
};
