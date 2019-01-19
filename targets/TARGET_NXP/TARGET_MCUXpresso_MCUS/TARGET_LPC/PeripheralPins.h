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

#ifndef MBED_PERIPHERALPINS_H
#define MBED_PERIPHERALPINS_H

#include "pinmap.h"
#include "PeripheralNames.h"

/************RTC***************/
extern const PinMap PinMap_RTC[];

/************ADC***************/
extern const PinMap PinMap_ADC[];

#if defined(FSL_FEATURE_SOC_LPC_CAN_COUNT) && (FSL_FEATURE_SOC_LPC_CAN_COUNT > 0)
/************CAN***************/
extern const PinMap PinMap_CAN_TD[];
extern const PinMap PinMap_CAN_RD[];
#endif

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

#if DEVICE_QSPI
/************QSPI***************/
extern const PinMap PinMap_QSPI_DATA0[];
extern const PinMap PinMap_QSPI_DATA1[];
extern const PinMap PinMap_QSPI_DATA2[];
extern const PinMap PinMap_QSPI_DATA3[];
extern const PinMap PinMap_QSPI_SCLK[];
extern const PinMap PinMap_QSPI_SSEL[];
#endif

#endif
