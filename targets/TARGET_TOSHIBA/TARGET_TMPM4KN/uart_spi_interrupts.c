/* mbed Microcontroller Library
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
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
#include "serial_api.h"
#include "uart_spi_interrupts.h"

//The UART and TSPI in M4KNA target have shared irq lines. For this reason we have to maintain shared irq handlers.
// Also enable/disable of these shared IRQ lines from NVIC need to be controlled carefully to avoid loss of interrupt when
// any one peripheral disables/enables interrupt. For this reason IRQ related code is seperated from peripheral driver apis and
// maintained separately in this source file.

#define CHANNEL_COUNT (4)

extern void invoke_serial_irq_handler(UARTName uart_name, SerialIrq event);
typedef struct {
    int ch0_tx_uart: 1;
    int ch0_tx_spi: 1;
    int ch0_rx_uart: 1;
    int ch0_rx_spi: 1;

    int ch1_tx_uart: 1;
    int ch1_tx_spi: 1;
    int ch1_rx_uart: 1;
    int ch1_rx_spi: 1;

    int ch2_tx_uart: 1;
    int ch2_tx_spi: 1;
    int ch2_rx_uart: 1;
    int ch2_rx_spi: 1;

    int ch3_tx_uart: 1;
    int ch3_tx_spi: 1;
    int ch3_rx_uart: 1;
    int ch3_rx_spi: 1;
} shared_nvic_irq_status;

shared_nvic_irq_status shared_irqs;

// IRQ Handlers shared by both SPI and UART

void INTSC0RX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_0, RxIrq);
}

void INTSC0TX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_0, TxIrq);
}

void INTSC1RX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_1, RxIrq);
}

void INTSC1TX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_1, TxIrq);
}

void INTSC2RX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_2, RxIrq);
}

void INTSC2TX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_2, TxIrq);
}

void INTSC3RX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_3, RxIrq);
}

void INTSC3TX_IRQHandler(void)
{
    invoke_serial_irq_handler(SERIAL_3, TxIrq);
}



void uart_spi_clear_pending_irq(IRQn_Type irq)
{
    if (NVIC_GetEnableIRQ(irq) == 0) {
        NVIC_ClearPendingIRQ(irq);
    }
}



void uart_spi_enable_irq(IRQn_Type irq, int uart_spi_device)
{
    if (NVIC_GetEnableIRQ(irq) == 0) {
        NVIC_EnableIRQ(irq);
    }

    switch (irq) {
        case INTSC0RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch0_rx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch0_rx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC0TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch0_tx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch0_tx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC1RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch1_rx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch1_rx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC1TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch1_tx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch1_tx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC2RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch2_rx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch2_rx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC2TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch2_tx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch2_tx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC3RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch3_rx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch3_rx_spi = TXZ_ENABLE;
            }
            break;
        case INTSC3TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch3_tx_uart = TXZ_ENABLE;
            } else {
                shared_irqs.ch3_tx_spi = TXZ_ENABLE;
            }
            break;

        default:
            break;

    }
}




void uart_spi_disable_irq(IRQn_Type irq, int uart_spi_device)
{
    int disable_flag = 0;

    switch (irq) {
        case INTSC0RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch0_rx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch0_rx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch0_rx_uart || shared_irqs.ch0_rx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC0TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch0_tx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch0_tx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch0_tx_uart || shared_irqs.ch0_tx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC1RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch1_rx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch1_rx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch1_rx_uart || shared_irqs.ch1_rx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC1TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch1_tx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch1_tx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch1_tx_uart || shared_irqs.ch1_tx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC2RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch2_rx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch2_rx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch2_rx_uart || shared_irqs.ch2_rx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC2TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch2_tx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch2_tx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch2_tx_uart || shared_irqs.ch2_tx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC3RX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch3_rx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch3_rx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch3_rx_uart || shared_irqs.ch3_rx_spi)) {
                disable_flag = 1;
            }
            break;
        case INTSC3TX_IRQn:
            if (uart_spi_device == UART_PERIPH) {
                shared_irqs.ch3_tx_uart = TXZ_DISABLE;
            } else {
                shared_irqs.ch3_tx_spi = TXZ_DISABLE;
            }

            if (!(shared_irqs.ch3_tx_uart || shared_irqs.ch3_tx_spi)) {
                disable_flag = 1;
            }
            break;

        default:
            break;
    }

    if (disable_flag) {
        NVIC_DisableIRQ(irq);
    }
}


