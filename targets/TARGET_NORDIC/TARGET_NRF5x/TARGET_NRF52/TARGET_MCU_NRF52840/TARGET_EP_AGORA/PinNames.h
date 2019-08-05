/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"
#include "nrf_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  3

///> define macro producing for example Px_y = NRF_GPIO_PIN_MAP(x, y)
#define PinDef(port_num, pin_num) P##port_num##_##pin_num = NRF_GPIO_PIN_MAP(port_num, pin_num)


typedef enum {
    PinDef(0, 0),  // P0_0 = 0...
    PinDef(0, 1),
    PinDef(0, 2),
    PinDef(0, 3),
    PinDef(0, 4),
    PinDef(0, 5),
    PinDef(0, 6),
    PinDef(0, 7),
    PinDef(0, 8),
    PinDef(0, 9),
    PinDef(0, 10),
    PinDef(0, 11),
    PinDef(0, 12),
    PinDef(0, 13),
    PinDef(0, 14),
    PinDef(0, 15),
    PinDef(0, 16),
    PinDef(0, 17),
    PinDef(0, 18),
    PinDef(0, 19),
    PinDef(0, 20),
    PinDef(0, 21),
    PinDef(0, 22),
    PinDef(0, 23),
    PinDef(0, 24),
    PinDef(0, 25),
    PinDef(0, 26),
    PinDef(0, 27),
    PinDef(0, 28),
    PinDef(0, 29),
    PinDef(0, 30),
    PinDef(0, 31),

    PinDef(1, 0),  //P1_1 = 32...
    PinDef(1, 1),
    PinDef(1, 2),
    PinDef(1, 3),
    PinDef(1, 4),
    PinDef(1, 5),
    PinDef(1, 6),
    PinDef(1, 7),
    PinDef(1, 8),
    PinDef(1, 9),
    PinDef(1, 10),
    PinDef(1, 11),
    PinDef(1, 12),
    PinDef(1, 13),
    PinDef(1, 14),
    PinDef(1, 15),

    // Port0
    p0  = P0_0,
    p1  = P0_1,
    p2  = P0_2,
    p3  = P0_3,
    p4  = P0_4,
    p5  = P0_5,
    p6  = P0_6,
    p7  = P0_7,
    p8  = P0_8,
    p9  = P0_9,
    p10 = P0_10,
    p11 = P0_11,
    p12 = P0_12,
    p13 = P0_13,
    p14 = P0_14,
    p15 = P0_15,
    p16 = P0_16,
    p17 = P0_17,
    p18 = P0_18,
    p19 = P0_19,
    p20 = P0_20,
    p21 = P0_21,
    p22 = P0_22,
    p23 = P0_23,
    p24 = P0_24,
    p25 = P0_25,
    p26 = P0_26,
    p27 = P0_27,
    p28 = P0_28,
    p29 = P0_29,
    p30 = P0_30,
    p31 = P0_31,

    // Port1
    p32 = P1_0,
    p33 = P1_1,
    p34 = P1_2,
    p35 = P1_3,
    p36 = P1_4,
    p37 = P1_5,
    p38 = P1_6,
    p39 = P1_7,
    p40 = P1_8,
    p41 = P1_9,
    p42 = P1_10,
    p43 = P1_11,
    p44 = P1_12,
    p45 = P1_13,
    p46 = P1_14,
    p47 = P1_15,

    SPI_PSELMOSI0 = P1_13,
    SPI_PSELMISO0 = P1_14,
    SPI_PSELSS0   = P1_12,
    SPI_PSELSCK0  = P1_15,

    SPI_PSELMOSI1 = P1_2,
    SPI_PSELMISO1 = P1_3,
    SPI_PSELSS1   = P1_1,
    SPI_PSELSCK1  = P1_4,

    SPIS_PSELMOSI = P1_2,
    SPIS_PSELMISO = P1_3,
    SPIS_PSELSS   = P1_1,
    SPIS_PSELSCK  = P1_4,

    I2C_SDA0 = p26,
    I2C_SCL0 = p27,

    D0 = P1_1,
    D1 = P1_2,
    D2 = P1_3,
    D3 = P1_4,
    D4 = P1_5,
    D5 = P1_6,
    D6 = P1_7,
    D7 = P1_8,

    D8  = P1_10,
    D9  = P1_11,
    D10 = P1_12,
    D11 = P1_13,
    D12 = P1_14,
    D13 = P1_15,

    D14 = p26,
    D15 = p27,

    A0 = p3,
    A1 = p4,
    A2 = p28,
    A3 = p29,
    A4 = p30,
    A5 = p31,

    /**** QSPI pins ****/
    QSPI1_IO0 = P0_20,
    QSPI1_IO1 = P0_21,
    QSPI1_IO2 = P0_22,
    QSPI1_IO3 = P0_23,
    QSPI1_SCK = P0_19,
    QSPI1_CSN = P0_17,

    /* Agora-specific pins */
#ifdef EP_AGORA_HW_REV_1_0
    // Battery
    PIN_NAME_BATTERY                = P0_2,
    PIN_NAME_BATTERY_MONITOR_ENABLE = P1_11,

    // Board ID
    PIN_NAME_BOARD_ID               = P0_3,
    PIN_NAME_BOARD_ID_DISABLE       = P1_6,

    // NFC
    PIN_NAME_NFC1                   = P0_9,
    PIN_NAME_NFC2                   = P0_10,

    // DEBUG UART
    PIN_NAME_DEBUG_RX               = P0_11,
    PIN_NAME_DEBUG_TX               = P0_13,

    // LoRa
    PIN_NAME_LORA_SSN               = P1_12,
    PIN_NAME_LORA_MOSI              = P1_13,
    PIN_NAME_LORA_MISO              = P1_14,
    PIN_NAME_LORA_SCLK              = P1_15,
    PIN_NAME_LORA_RESETN            = P1_10,
    PIN_NAME_LORA_DIO0              = P1_7,
    PIN_NAME_LORA_DIO1              = P1_9,
    PIN_NAME_LORA_DIO2              = P0_12,
    PIN_NAME_LORA_DIO3              = P0_16,
    PIN_NAME_LORA_DIO4              = P0_14,

    // Cell
    PIN_NAME_CELL_POWER_ENABLE      = P0_28,
    PIN_NAME_CELL_ON_OFF            = P0_30,
    PIN_NAME_CELL_HW_SHUTDOWN       = P0_24,
    PIN_NAME_CELL_RX                = P1_1,
    PIN_NAME_CELL_TX                = P1_2,
    PIN_NAME_CELL_DTR               = P0_7,
    PIN_NAME_CELL_DSR               = P1_3,
    PIN_NAME_CELL_DCD               = P0_15,
    PIN_NAME_CELL_RTS               = P1_4,
    PIN_NAME_CELL_CTS               = P1_5,

    // Sensors (I2C)
    PIN_NAME_SENSOR_POWER_ENABLE    = P0_31,
    PIN_NAME_SDA                    = P0_26,
    PIN_NAME_SCL                    = P0_27,

    // Sensor interrupts
    PIN_NAME_INT_ACCEL              = P1_8,
    PIN_NAME_INT_LIGHT_TOF          = P0_4,

    // Microphone (I2S)
    PIN_NAME_I2S_SD                 = P0_25,
    PIN_NAME_I2S_SCK                = P0_6,
    PIN_NAME_I2S_WS                 = P0_8,

    // QSPI
    PIN_NAME_QSPI_IO0               = P0_20,
    PIN_NAME_QSPI_IO1               = P0_21,
    PIN_NAME_QSPI_IO2               = P0_22,
    PIN_NAME_QSPI_IO3               = P0_23,
    PIN_NAME_QSPI_CSN               = P0_17,
    PIN_NAME_QSPI_CLK               = P0_19,

    // Miscellaneous I/O
    PIN_NAME_PUSH_BUTTON            = P0_29,
    PIN_NAME_LED_RED                = P0_5,
    PIN_NAME_BT840_RESETN           = P0_18,
    PIN_NAME_BT840_SWO              = P1_0,

    /* mbed pins */

    // used by mbed for default serial out on printf statements
    RX_PIN_NUMBER = PIN_NAME_DEBUG_RX,
    TX_PIN_NUMBER = PIN_NAME_DEBUG_TX,
    USBRX = PIN_NAME_DEBUG_RX,
    USBTX = PIN_NAME_DEBUG_TX,
    STDIO_UART_RX = PIN_NAME_DEBUG_RX,
    STDIO_UART_TX = PIN_NAME_DEBUG_TX,

    MDMTXD = PIN_NAME_CELL_TX,
    MDMRXD = PIN_NAME_CELL_RX,
    MDMCTS = PIN_NAME_CELL_CTS,
    MDMDCD = PIN_NAME_CELL_DCD,
    MDMDSR = PIN_NAME_CELL_DSR,
    MDMDTR = PIN_NAME_CELL_DTR,
    MDMRTS = PIN_NAME_CELL_RTS,

    /**** QSPI FLASH pins ****/
    QSPI_FLASH1_IO0 = PIN_NAME_QSPI_IO0,
    QSPI_FLASH1_IO1 = PIN_NAME_QSPI_IO1,
    QSPI_FLASH1_IO2 = PIN_NAME_QSPI_IO2,
    QSPI_FLASH1_IO3 = PIN_NAME_QSPI_IO3,
    QSPI_FLASH1_SCK = PIN_NAME_QSPI_CLK,
    QSPI_FLASH1_CSN = PIN_NAME_QSPI_CSN,

    SPI_MOSI    = PIN_NAME_QSPI_IO0,
    SPI_MISO    = PIN_NAME_QSPI_IO1,
    SPI_SCK     = PIN_NAME_QSPI_CLK,
    SPI_CS      = PIN_NAME_QSPI_CSN,

    // LED
    LED1 = PIN_NAME_LED_RED,
    LED2 = LED1,

    // Not connected
    NC = (int)0xFFFFFFFF,

    // Compiler complains unless these are specified
    STDIO_UART_CTS = NC,
    STDIO_UART_RTS = NC,
    MDMRI = NC
#else
    // Battery
    PIN_NAME_BATTERY                = P0_2,
    PIN_NAME_BATTERY_MONITOR_ENABLE = P1_11,

    // Board ID
    PIN_NAME_BOARD_ID               = P0_3,
    PIN_NAME_BOARD_ID_DISABLE       = P1_6,

    // NFC
    PIN_NAME_NFC1                   = P0_9,
    PIN_NAME_NFC2                   = P0_10,

    // DEBUG UART
    PIN_NAME_DEBUG_RX               = P0_16,
    PIN_NAME_DEBUG_TX               = P0_13,

    // LoRa
    PIN_NAME_LORA_SSN               = P1_9,
    PIN_NAME_LORA_MOSI              = P0_11,
    PIN_NAME_LORA_MISO              = P0_12,
    PIN_NAME_LORA_SCLK              = P0_7,
    PIN_NAME_LORA_RESETN            = P1_10,
    PIN_NAME_LORA_DIO0              = P1_7,
    PIN_NAME_LORA_DIO1              = P1_12,
    PIN_NAME_LORA_DIO2              = P1_13,
    PIN_NAME_LORA_DIO3              = P1_14,
    PIN_NAME_LORA_DIO4              = P1_15,

    // Cell
    PIN_NAME_CELL_POWER_ENABLE      = P0_28,
    PIN_NAME_CELL_ON_OFF            = P0_30,
    PIN_NAME_CELL_HW_SHUTDOWN       = P0_24,
    PIN_NAME_CELL_RX                = P1_1,
    PIN_NAME_CELL_TX                = P1_2,
    PIN_NAME_CELL_DTR               = P1_4,
    PIN_NAME_CELL_DSR               = P1_3,
    PIN_NAME_CELL_DCD               = P0_15,
    PIN_NAME_CELL_RTS               = P0_14,
    PIN_NAME_CELL_CTS               = P1_8,
    PIN_NAME_CELL_PWRMON            = P1_15,

    // Sensors (I2C)
    PIN_NAME_SENSOR_POWER_ENABLE    = P0_31,
    PIN_NAME_SDA                    = P0_26,
    PIN_NAME_SCL                    = P0_27,

    // Sensor interrupts
    PIN_NAME_INT_ACCEL              = P1_5,
    PIN_NAME_INT_LIGHT_TOF          = P0_4,

    // Microphone (I2S)
    PIN_NAME_I2S_SD                 = P0_25,
    PIN_NAME_I2S_SCK                = P0_6,
    PIN_NAME_I2S_WS                 = P0_8,

    // QSPI
    PIN_NAME_QSPI_IO0               = P0_20,
    PIN_NAME_QSPI_IO1               = P0_21,
    PIN_NAME_QSPI_IO2               = P0_22,
    PIN_NAME_QSPI_IO3               = P0_23,
    PIN_NAME_QSPI_CSN               = P0_17,
    PIN_NAME_QSPI_CLK               = P0_19,

    // Miscellaneous I/O
    PIN_NAME_PUSH_BUTTON            = P0_29,
    PIN_NAME_LED_RED                = P0_5,
    PIN_NAME_BT840_RESETN           = P0_18,
    PIN_NAME_BT840_SWO              = P1_0,

    /* mbed pins */

    // used by mbed for default serial out on printf statements
    RX_PIN_NUMBER = PIN_NAME_DEBUG_RX,
    TX_PIN_NUMBER = PIN_NAME_DEBUG_TX,
    USBRX = PIN_NAME_DEBUG_RX,
    USBTX = PIN_NAME_DEBUG_TX,
    STDIO_UART_RX = PIN_NAME_DEBUG_RX,
    STDIO_UART_TX = PIN_NAME_DEBUG_TX,

    MDMTXD = PIN_NAME_CELL_TX,
    MDMRXD = PIN_NAME_CELL_RX,
    MDMCTS = PIN_NAME_CELL_CTS,
    MDMDCD = PIN_NAME_CELL_DCD,
    MDMDSR = PIN_NAME_CELL_DSR,
    MDMDTR = PIN_NAME_CELL_DTR,
    MDMRTS = PIN_NAME_CELL_RTS,

    /**** QSPI FLASH pins ****/
    QSPI_FLASH1_IO0 = PIN_NAME_QSPI_IO0,
    QSPI_FLASH1_IO1 = PIN_NAME_QSPI_IO1,
    QSPI_FLASH1_IO2 = PIN_NAME_QSPI_IO2,
    QSPI_FLASH1_IO3 = PIN_NAME_QSPI_IO3,
    QSPI_FLASH1_SCK = PIN_NAME_QSPI_CLK,
    QSPI_FLASH1_CSN = PIN_NAME_QSPI_CSN,

    SPI_MOSI    = PIN_NAME_QSPI_IO0,
    SPI_MISO    = PIN_NAME_QSPI_IO1,
    SPI_SCK     = PIN_NAME_QSPI_CLK,
    SPI_CS      = PIN_NAME_QSPI_CSN,

    // LED
    LED1 = PIN_NAME_LED_RED,
    LED2 = LED1,

    // Not connected
    NC = (int)0xFFFFFFFF,

    // Compiler complains unless these are specified
    STDIO_UART_CTS = NC,
    STDIO_UART_RTS = NC,
    MDMRI = NC
#endif
} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#define ACTIVE_HIGH_POLARITY    1
#define ACTIVE_LOW_POLARITY     0

#define MDM_PIN_POLARITY        ACTIVE_HIGH_POLARITY

#ifdef __cplusplus
}
#endif

#endif