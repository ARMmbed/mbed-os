/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "BlueNRG1_spi.h"
#include "BlueNRG1_uart.h"
#include "BlueNRG1_gpio.h"
#include "cmsis.h"
#include "PeripheralNames.h"
#include "PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

#define gpio_t GPIO_InitType

struct gpio_irq_s{
	GPIO_EXTIConfigType exti;
	uint32_t id;
};

struct serial_s{
    UARTName uart;
    uint32_t index_irq; // Used by irq
    UART_InitType init;  //bluenrg struct
    PinName pin_tx;
    PinName pin_rx;
};

struct spi_s{
	SPI_InitType init;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
    char dummy_char;
};

#ifdef __cplusplus
}
#endif

#endif
