/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 Arm Limited
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

#include "mbed_error.h"
#include "mbed_serial_platform.h"
#include "serial_api.h"

#define UART_DEVICES_NUMBER   2

struct uart_irq_t uart_irq[UART_DEVICES_NUMBER];

const PinMap PinMap_UART_TX[] = {
    {UART0_TX, UART_0, ALTERNATE_FUNC_1},
    {UART1_TX, UART_1, PRIMARY_FUNC},
    {NC,       NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {UART0_RX, UART_0, ALTERNATE_FUNC_1},
    {UART1_RX, UART_1, PRIMARY_FUNC},
    {NC,       NC,     0}
};

const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}

#ifdef UART0_PL011_DEV
#ifndef uart0_tx_irq_handler
#error "uart0_tx_irq_handler should be defined, check device_cfg.h!"
#endif
void uart0_tx_irq_handler(void)
{
    uart_pl011_clear_intr(&UART0_PL011_DEV, UART_PL011_TX_INTR_MASK);
    if(uart_irq[UART_0].handler != 0) {
        uart_irq[UART_0].handler(uart_irq[UART_0].id, TxIrq);
    }
}

#ifndef uart0_rx_irq_handler
#error "uart0_rx_irq_handler should be defined, check device_cfg.h!"
#endif
void uart0_rx_irq_handler(void)
{
    uart_pl011_clear_intr(&UART0_PL011_DEV, UART_PL011_RX_INTR_MASK);
    if(uart_irq[UART_0].handler != 0) {
        uart_irq[UART_0].handler(uart_irq[UART_0].id, RxIrq);
    }
}

#ifndef uart0_rx_timeout_irq_handler
#error "uart0_rx_timeout_irq_handler should be defined, check device_cfg.h!"
#endif
void uart0_rx_timeout_irq_handler(void)
{
    if(uart_irq[UART_0].handler != 0) {
        uart_irq[UART_0].handler(uart_irq[UART_0].id, RxIrq);
    }
}
#endif

#ifdef UART1_PL011_DEV
#ifndef uart1_tx_irq_handler
#error "uart1_tx_irq_handler should be defined, check device_cfg.h!"
#endif
void uart1_tx_irq_handler(void)
{
    uart_pl011_clear_intr(&UART1_PL011_DEV, UART_PL011_TX_INTR_MASK);
    if(uart_irq[UART_1].handler != 0) {
        uart_irq[UART_1].handler(uart_irq[UART_1].id, TxIrq);
    }
}

#ifndef uart1_rx_irq_handler
#error "uart1_rx_irq_handler should be defined, check device_cfg.h!"
#endif
void uart1_rx_irq_handler(void)
{
    uart_pl011_clear_intr(&UART1_PL011_DEV, UART_PL011_RX_INTR_MASK);
    if(uart_irq[UART_1].handler != 0) {
        uart_irq[UART_1].handler(uart_irq[UART_1].id, RxIrq);
    }
}

#ifndef uart1_rx_timeout_irq_handler
#error "uart1_rx_timeout_irq_handler should be defined, check device_cfg.h!"
#endif
void uart1_rx_timeout_irq_handler(void)
{
    if(uart_irq[UART_1].handler != 0) {
        uart_irq[UART_1].handler(uart_irq[UART_1].id, RxIrq);
    }
}
#endif

void mbed_uart_platform_init(serial_t *obj, UARTName uart)
{
    switch (uart) {
#ifdef UART0_PL011_DEV
    case UART_0:
        obj->uart_dev = &UART0_PL011_DEV;
        obj->tx_irq = UART0_Tx_IRQn;
        obj->rx_irq = UART0_Rx_IRQn;
        obj->rx_timeout_irq = UART0_RxTimeout_IRQn;
        break;
#endif
#ifdef UART1_PL011_DEV
    case UART_1:
        obj->uart_dev = &UART1_PL011_DEV;
        obj->tx_irq = UART1_Tx_IRQn;
        obj->rx_irq = UART1_Rx_IRQn;
        obj->rx_timeout_irq = UART1_RxTimeout_IRQn;
        break;
#endif
    default:
        error("serial_init: No uart selected");
    }
}
