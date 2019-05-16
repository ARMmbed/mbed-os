/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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

extern const int GD_GPIO_REMAP[];
extern const int GD_GPIO_MODE[];
extern const int GD_GPIO_SPEED[];

/* ADC */
#if DEVICE_ANALOGIN
extern const PinMap PinMap_ADC[];
#endif

/* DAC */
#if DEVICE_ANALOGOUT
extern const PinMap PinMap_DAC[];
#endif

/* I2C */
#if DEVICE_I2C
extern const PinMap PinMap_I2C_SDA[];
extern const PinMap PinMap_I2C_SCL[];
#endif

/* PWM */
#if DEVICE_PWMOUT
extern const PinMap PinMap_PWM[];
#endif

/* SERIAL */
#if DEVICE_SERIAL
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
#if DEVICE_SERIAL_FC
extern const PinMap PinMap_UART_RTS[];
extern const PinMap PinMap_UART_CTS[];
#endif
#endif

/* SPI */
#if DEVICE_SPI
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];
#endif

/* CAN */
#if DEVICE_CAN
extern const PinMap PinMap_CAN_RD[];
extern const PinMap PinMap_CAN_TD[];
#endif

#endif
