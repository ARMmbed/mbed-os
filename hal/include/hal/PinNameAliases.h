
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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
#ifndef MBED_PIN_NAME_ALIASES_H
#define MBED_PIN_NAME_ALIASES_H

/* Aliases for legacy reasons. To be removed in the next Mbed OS version */
#if defined (CONSOLE_TX) && (CONSOLE_RX)
#define USBTX CONSOLE_TX
#define USBRX CONSOLE_RX
#else
#define CONSOLE_TX USBTX
#define CONSOLE_RX USBRX
#endif

#if defined (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO)

#ifdef TARGET_FF_ARDUINO_UNO

#define A0  ARDUINO_UNO_A0
#define A1  ARDUINO_UNO_A1
#define A2  ARDUINO_UNO_A2
#define A3  ARDUINO_UNO_A3
#define A4  ARDUINO_UNO_A4
#define A5  ARDUINO_UNO_A5
#define D0  ARDUINO_UNO_D0
#define D1  ARDUINO_UNO_D1
#define D2  ARDUINO_UNO_D2
#define D3  ARDUINO_UNO_D3
#define D4  ARDUINO_UNO_D4
#define D5  ARDUINO_UNO_D5
#define D6  ARDUINO_UNO_D6
#define D7  ARDUINO_UNO_D7
#define D8  ARDUINO_UNO_D8
#define D9  ARDUINO_UNO_D9
#define D10 ARDUINO_UNO_D10
#define D11 ARDUINO_UNO_D11
#define D12 ARDUINO_UNO_D12
#define D13 ARDUINO_UNO_D13
#define D14 ARDUINO_UNO_D14
#define D15 ARDUINO_UNO_D15

#endif // TARGET_FF_ARDUINO_UNO

#ifdef TARGET_FF_ARDUINO

#warning ARDUINO form factor should not be used any more => use ARDUINO_UNO

#define ARDUINO_UNO_A0  A0
#define ARDUINO_UNO_A1  A1
#define ARDUINO_UNO_A2  A2
#define ARDUINO_UNO_A3  A3
#define ARDUINO_UNO_A4  A4
#define ARDUINO_UNO_A5  A5
#define ARDUINO_UNO_D0  D0
#define ARDUINO_UNO_D1  D1
#define ARDUINO_UNO_D2  D2
#define ARDUINO_UNO_D3  D3
#define ARDUINO_UNO_D4  D4
#define ARDUINO_UNO_D5  D5
#define ARDUINO_UNO_D6  D6
#define ARDUINO_UNO_D7  D7
#define ARDUINO_UNO_D8  D8
#define ARDUINO_UNO_D9  D9
#define ARDUINO_UNO_D10 D10
#define ARDUINO_UNO_D11 D11
#define ARDUINO_UNO_D12 D12
#define ARDUINO_UNO_D13 D13
#define ARDUINO_UNO_D14 D14
#define ARDUINO_UNO_D15 D15

#endif // TARGET_FF_ARDUINO

// Arduino Uno I2C signals aliases
#define ARDUINO_UNO_I2C_SDA ARDUINO_UNO_D14
#define ARDUINO_UNO_I2C_SCL ARDUINO_UNO_D15

// Arduino Uno SPI signals aliases
#define ARDUINO_UNO_SPI_CS   ARDUINO_UNO_D10
#define ARDUINO_UNO_SPI_MOSI ARDUINO_UNO_D11
#define ARDUINO_UNO_SPI_MISO ARDUINO_UNO_D12
#define ARDUINO_UNO_SPI_SCK  ARDUINO_UNO_D13

// Arduino Uno UART signals aliases
#define ARDUINO_UNO_UART_TX ARDUINO_UNO_D1
#define ARDUINO_UNO_UART_RX ARDUINO_UNO_D0

#endif // (TARGET_FF_ARDUINO) || (TARGET_FF_ARDUINO_UNO)

#endif // MBED_PIN_NAME_ALIASES_H

/** @}*/
