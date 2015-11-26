/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "gpio_object.h"
#include "uart_serial.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gpio_irq_s {
    uint32_t pin;
    uint32_t irqmask;
};

struct port_s {
};

struct serial_s {
    UARTName uart;
    usart_serial_options_t uart_serial_options;
#if DEVICE_SERIAL_ASYNCH
#endif
};

struct analogin_s {
};

struct pwmout_s {
};

struct i2c_s {
#if DEVICE_I2C_ASYNCH
#endif
};

struct spi_s {
#if DEVICE_SPI_ASYNCH
#endif
};

#ifdef __cplusplus
}
#endif

#endif
