/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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
#include "PinNames.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IRQ_NOT_SET,
    IRQ_ON,
    IRQ_OFF
} irq_setting_t;

struct port_s {
    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_out;
    __IO uint32_t *reg_val;
    __IO uint32_t *reg_drv;
    PortName port;
    uint32_t mask;
};

struct gpio_irq_s {
    /* Don't bother with having a port number here as there's only one */
    uint32_t ch;   /* Corresponds to the interrupt pin */
};

struct serial_s {
    SerialConfig config;
    PinName rx_pin;
    PinName tx_pin;
    volatile uart_ctrl_t *reg_base;
    uint8_t index;
    uint32_t baud_rate;
    bool format_set; /* If true then the struct that follows is populated */
    struct {
        uint8_t stop_bits;
        uint8_t data_bits;
        uint8_t parity;
    } format;
    irq_setting_t irq_rx_setting;
    irq_setting_t irq_tx_setting;
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
