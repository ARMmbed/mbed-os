/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/


#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "gpio_object.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_SERIAL
struct serial_s {
    UARTName uart;
    int index; // Used by irq
    uint32_t baudrate;
    uint32_t databits;
    uint32_t stopbits;
    uint32_t parity;
    PinName pin_tx;
    PinName pin_rx;
#if DEVICE_SERIAL_ASYNCH
    uint32_t events;
#endif
#if DEVICE_SERIAL_FC
    uint32_t hw_flow_ctl;
    PinName pin_rts;
    PinName pin_cts;
#endif
};
#endif

struct gpio_irq_s {
    PinName  pin;
};
#if DEVICE_SPI
struct spi_s {
    BP_SPI_TypeDef *spi;
    uint32_t freq;
    uint32_t freq_r;
    gpio_t   cs;
};
#endif

#if DEVICE_I2C
struct i2c_s {
    BP_I2C_TypeDef *i2c;
    uint32_t index;
    unsigned int addrlen;
    uint32_t xfer_freq;
};
#endif

#if DEVICE_ANALOGIN
struct analogin_s {
    uint8_t channel;
};
#endif

#if DEVICE_FLASH
struct flash_s {
    uint32_t dummy;
};
#endif
#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
