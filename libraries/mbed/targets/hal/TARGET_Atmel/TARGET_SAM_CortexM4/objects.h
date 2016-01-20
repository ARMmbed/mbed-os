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
#include "serial_platform.h"
#include "adc2.h"

#ifdef __cplusplus
extern "C" {
#endif

struct waveconfig_t {
    /** Internal clock signals selection. */
    uint32_t ul_intclock;
    /** Waveform frequency (in Hz). */
    uint16_t us_frequency;
    /** Duty cycle in percent (positive).*/
    uint16_t us_dutycycle;
};

struct gpio_irq_s {
    uint32_t pin;
    uint32_t irqmask;
};

struct port_s {
    PortName port;
    uint32_t mask;
    uint8_t mode;
    uint8_t direction;
};

struct serial_s {
    UARTName uart;
    usart_serial_options_t uart_serial_options;
#if DEVICE_SERIAL_ASYNCH
    uint8_t actrec;
    uint8_t acttra;
    uint32_t events;
#endif
};

struct analogin_s {
    enum adc_channel_num channel;
};

struct pwmout_s {
    uint32_t channel;
    uint32_t ioline;
    uint32_t pin;
    uint32_t prescalarindex;
    struct waveconfig_t waveconfig;
};

struct i2c_s {
    Twi * i2c_base;
    Flexcom * flexcom;
    uint8_t is_slave;
    uint8_t module_number;
    uint32_t speed;
    uint32_t master_clk;
    uint32_t address;
#if DEVICE_I2C_ASYNCH
    Pdc * pdc;
    uint8_t dma_usage;
    IRQn_Type irq_type;
    uint32_t event;
    uint32_t stop;
#endif
};
struct spi_s {
    Spi * spi_base;
    Flexcom * flexcom;
    uint8_t cs;
    uint8_t polarity;
    uint8_t phase;
    uint32_t transferrate;
    uint8_t is_slave;
    uint8_t module_number;
#if DEVICE_SPI_ASYNCH
    Pdc * pdc;
    uint8_t dma_usage;
    IRQn_Type irq_type;
    uint32_t event;
#endif
};

#ifdef __cplusplus
}
#endif

#endif
