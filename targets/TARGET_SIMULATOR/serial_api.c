/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

// This is incomplete! Just forwards to stdout. Needs to be implemented!

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_api.h"

#if defined(DEVICE_SERIAL)

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    obj->rx_pin = rx;
    obj->tx_pin = tx;

    stdio_uart_inited = 1;
    stdio_uart = *obj;
}

void serial_free(serial_t *obj) {

}


void serial_baud(serial_t *obj, int baudrate) {
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    return 0;
}

void serial_putc(serial_t *obj, int c) {
    putc(c, stdout);
    fflush(stdout);
}

int serial_readable(serial_t *obj) {
    return 0;
}

int serial_writable(serial_t *obj) {
    return 0;
}

void serial_clear(serial_t *obj) {
}

void serial_pinout_tx(PinName tx) {
}

void serial_break_set(serial_t *obj) {
}

void serial_break_clear(serial_t *obj) {
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
}

#endif
