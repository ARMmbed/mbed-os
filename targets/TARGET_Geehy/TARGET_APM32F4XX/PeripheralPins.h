/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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

extern const int APM_GPIO_MODE[];
extern const int APM_GPIO_PULL_UP_DOWN[];
extern const int APM_GPIO_OUTPUT_MODE[];
extern const int APM_GPIO_SPEED[];
extern const int APM_GPIO_AF[];

/* ADC */
#ifdef DEVICE_ANALOGIN
extern const PinMap PinMap_ADC[];
#endif

/* DAC */
#ifdef DEVICE_ANALOGOUT
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
#ifdef DEVICE_SERIAL
extern const PinMap PinMap_UART_TX[];
extern const PinMap PinMap_UART_RX[];
#ifdef DEVICE_SERIAL_FC
extern const PinMap PinMap_UART_RTS[];
extern const PinMap PinMap_UART_CTS[];
#endif
#endif

/* SPI */
#ifdef DEVICE_SPI
extern const PinMap PinMap_SPI_MOSI[];
extern const PinMap PinMap_SPI_MISO[];
extern const PinMap PinMap_SPI_SCLK[];
extern const PinMap PinMap_SPI_SSEL[];
#endif

/* CAN */
#ifdef DEVICE_CAN
extern const PinMap PinMap_CAN_RD[];
extern const PinMap PinMap_CAN_TD[];
#endif

#define USE_USB_NO_OTG          0
#define USE_USB_OTG_FS          1
#define USE_USB_OTG_HS          2
#define USE_USB_HS_IN_FS        3

/* USB */
#ifdef DEVICE_USBDEVICE
extern const PinMap PinMap_USB_FS[];
extern const PinMap PinMap_USB_HS[];
#endif

#endif
