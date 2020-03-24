/*
 * Copyright (c) 2015-2016, Nuvoton Technology Corporation
 *
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

#ifdef __cplusplus
extern "C" {
#endif

//*** GPIO ***

extern const PinMap PinMap_GPIO[];

//*** ADC ***

extern const PinMap PinMap_ADC[];

//*** DAC ***

extern const PinMap PinMap_DAC[];

//*** I2C ***

extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];

//*** PWM ***

extern const PinMap PinMap_PWM[];

//*** SERIAL ***

extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
extern const PinMap PinMap_UART_RTS[];
extern const PinMap PinMap_UART_CTS[];

//*** SPI ***

extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];

//*** SD ***

extern const PinMap PinMap_SD_CD[];
extern const PinMap PinMap_SD_CMD[];
extern const PinMap PinMap_SD_CLK[];
extern const PinMap PinMap_SD_DAT0[];
extern const PinMap PinMap_SD_DAT1[];
extern const PinMap PinMap_SD_DAT2[];
extern const PinMap PinMap_SD_DAT3[];

//*** CAN ***

extern PinMap const PinMap_CAN_TD[];
extern PinMap const PinMap_CAN_RD[];

#ifdef __cplusplus
}
#endif

#endif
