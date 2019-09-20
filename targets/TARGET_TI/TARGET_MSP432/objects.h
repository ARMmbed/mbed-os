/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include <stdbool.h>
#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName         pin;
    uint8_t         mask;
    uint8_t         port_index;
    __IO uint8_t   *pin_base;
} gpio_t;

struct gpio_irq_s {
    uint8_t         pin;
    uint8_t         port;
    bool            rise;
    bool            fall;
    __IO uint8_t   *pin_base;
};

struct port_s {
    PortName        port;
    uint8_t         mask;
    __IO uint8_t   *port_in;
    __IO uint8_t   *port_out;
    __IO uint8_t   *port_dir;
};

struct serial_s {
    UARTName        uart;
    PinName         pin_tx;
    PinName         pin_rx;
};

struct analogin_s {
    uint8_t         mem_index;
};

struct i2c_s {
    I2CName         i2c;
#if DEVICE_I2C_ASYNCH
    bool            active;
    bool            send_stop;
    void (*handler)();
    uint8_t         event;
    uint8_t         available_events;
#endif
};

struct spi_s {
    SPIName spi;
    uint8_t module;
    PinName pin_miso;
    PinName pin_mosi;
    PinName pin_sclk;
    PinName pin_ssel;
    // Addresses of HW-registers
    __IO uint16_t *EUSCI_CTLW0;
    __IO uint16_t *EUSCI_BRW;
    __IO uint16_t *EUSCI_STATW;
    __I  uint16_t *EUSCI_RXBUF;
    __IO uint16_t *EUSCI_TXBUF;
    __IO uint16_t *EUSCI_IE;
    __IO uint16_t *EUSCI_IFG;
    __I  uint16_t *EUSCI_IV;
#if DEVICE_SPI_ASYNCH
    bool            active;
    void (*handler)();
    uint8_t         event;
    uint8_t         available_events;
#endif
};

struct pwmout_s {
    PWMName         pwm;
    PinName         pin;
    uint8_t         ccr_index;
    uint32_t        divider;
    uint32_t        pulse;
    uint32_t        ccr0;
    float           dutyCycle;
};

struct flash_s {
    /*  nothing to be stored for now */
    uint32_t dummy;
};

#ifdef __cplusplus
}
#endif

#endif
