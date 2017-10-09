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
    PinName rx_pin; /* PinName will be used to find if UART1 or UART2 is addressed, PinNames, PortNames, PeripheralNames, Pinmap is yet to be updated */
    PinName tx_pin; 
    SerialConfig config; /* This enum is used to initialize the UART being addressed */	
    volatile struct uart_s *reg_base;
    uint8_t index; /* IRQ index number */
    uint32_t baud_rate;
    uint16_t baud_rate_divider; /* calculated from baudrate */
    uint8_t nco_phase; /* calculated from baudrate */
    uint8_t flow_control; 
    bool baudrate_gen_enable;
    bool format_set; /* If true then the struct that follows is populated */
    struct {
        uint8_t rx_stop_bits; /* ref. uart_stop_bits */
        uint8_t tx_stop_bits;
        uint8_t data_bits; /* ref. uart_driver_mode */
        uint8_t parity; /* ref. uart_parity (UART_PARITY_EVEN or UART_PARITY_ODD) */
    } format;
    irq_setting_t irq_rx_setting; /* used in serial_irq_set if IRQ has been set or not */
    irq_setting_t irq_tx_setting;		
};

#include "gpio_object.h"

#ifdef __cplusplus
}
#endif

#endif
