/***************************************************************************//**
 * @file PinNames.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "CommonPinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EFM32_STANDARD_PIN_DEFINITIONS,

    /* Starter Kit says LED0 and LED1, but mbed expects 1 and 2. This way using 1 and 2 or 0 and 1 will work.
     * EFM32GG11-STK3701 has two RGB LEDs. For the purpose of standard LED functionality, we set up the
     * standard LED definitions to point to the green channel of the respective LEDs. */
    LED0 = PH11,
    LED1 = PH14,
    LED2 = LED0,
    LED3 = LED1,
    LED4 = LED0,

    LED0R = PH10,
    LED0G = PH11,
    LED0B = PH12,
    LED1R = PH13,
    LED1G = PH14,
    LED1B = PH15,

    /* Push Buttons */
    SW0 = PC8,
    SW1 = PC9,
    BTN0 = SW0,
    BTN1 = SW1,
    // Standardized button names
    BUTTON1 = BTN0,
    BUTTON2 = BTN1,

    /* Expansion headers */
    EXP3  = PA12,
    EXP4  = PE10,
    EXP5  = PA13,
    EXP6  = PE11,
    EXP7  = PC4,
    EXP8  = PE12,
    EXP9  = PC5,
    EXP10 = PE13,
    EXP11 = PB11,
    EXP12 = PE8,
    EXP13 = PB9,
    EXP14 = PE9,
    EXP15 = PC1,
    EXP16 = PC0,

    /* Serial (just some usable pins) */
    SERIAL_TX   = PE10,
    SERIAL_RX   = PE11,

    /* Board Controller UART (USB)*/
    USBTX       = PH4,
    USBRX       = PH5,

    /* Board Controller */
    STDIO_UART_TX = USBTX,
    STDIO_UART_RX = USBRX,

    /* On-board MX25R3235F */
    QSPI_FLASH1_IO0 = PG1,
    QSPI_FLASH1_IO1 = PG2,
    QSPI_FLASH1_IO2 = PG3,
    QSPI_FLASH1_IO3 = PG4,
    QSPI_FLASH1_SCK = PG0,
    QSPI_FLASH1_CSN = PG9,

} PinName;

#ifdef __cplusplus
}
#endif

#endif
