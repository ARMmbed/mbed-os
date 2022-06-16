/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 
#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"

/************RTC***************/
extern const PinMap PinMap_RTC[];

/************ADC***************/
extern const PinMap PinMap_ADC[];

/************DAC***************/
extern const PinMap PinMap_DAC[];

/************I2C***************/
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];

/************UART***************/
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
extern const PinMap PinMap_UART_CTS[];
extern const PinMap PinMap_UART_RTS[];
/************SPI***************/
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SSEL[];

/************PWM***************/
extern const PinMap PinMap_PWM[];

extern const PinMap PinMap_SEMC_DATA00[];
extern const PinMap PinMap_SEMC_DATA01[];
extern const PinMap PinMap_SEMC_DATA02[];
extern const PinMap PinMap_SEMC_DATA03[];
extern const PinMap PinMap_SEMC_DATA04[];
extern const PinMap PinMap_SEMC_DATA05[];
extern const PinMap PinMap_SEMC_DATA06[];
extern const PinMap PinMap_SEMC_DATA07[];
extern const PinMap PinMap_SEMC_ADD09[];
extern const PinMap PinMap_SEMC_ADD11[];
extern const PinMap PinMap_SEMC_ADD12[];
extern const PinMap PinMap_SEMC_BA1[];
extern const PinMap PinMap_SEMC_RDY[];
extern const PinMap PinMap_SEMC_CSX00[];


#endif
