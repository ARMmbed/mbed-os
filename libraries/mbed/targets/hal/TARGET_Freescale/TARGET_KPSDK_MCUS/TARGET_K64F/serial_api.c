/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

// math.h required for floating point operations for baud rate calculation
#include <math.h>

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "fsl_uart_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_uart_features.h"

/* TODO:
    putchar/getchar 9 and 10 bits support
*/

static const PinMap PinMap_UART_TX[] = {
    {PTB17, UART_0, 3},
    {PTC17, UART_3, 3},
    {PTD7 , UART_0, 3},
    {PTD3 , UART_2, 3},
    {PTC4 , UART_1, 3},
    {PTC15, UART_4, 3},
    {PTB11, UART_3, 3},
    {PTA14, UART_0, 3},
    {PTE24, UART_4, 3},
    {PTE4 , UART_3, 3},
    {PTE0,  UART_1, 3},
    {NC  ,  NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {PTB16, UART_0, 3},
    {PTE1 , UART_1, 3},
    {PTE5 , UART_3, 3},
    {PTE25, UART_4, 3},
    {PTA15, UART_0, 3},
    {PTC16, UART_3, 3},
    {PTB10, UART_3, 3},
    {PTC3 , UART_1, 3},
    {PTC14, UART_4, 3},
    {PTD2 , UART_2, 3},
    {PTC6 , UART_0, 3},
    {NC  ,  NC    , 0}
};

#define UART_NUM    4

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

static uint32_t serial_get_clock(uint32_t uart_instance)
{
    uint32_t uartSourceClock;

    if ((uart_instance == 0) || (uart_instance == 1)) {
        clock_manager_get_frequency(kSystemClock, &uartSourceClock);
    } else {
        clock_manager_get_frequency(kBusClock, &uartSourceClock);
    }
    return uartSourceClock;
}

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    obj->index = (UARTName)pinmap_merge(uart_tx, uart_rx);
    if ((int)obj->index == NC) {
        error("Serial pinout mapping failed");
    }

    uart_config_t uart_config;
    uart_config.baudRate = 9600;
    uart_config.bitCountPerChar = kUart8BitsPerChar;
    uart_config.parityMode = kUartParityDisabled;
    uart_config.rxDataInvert = 0;
    uart_config.stopBitCount = kUartOneStopBit;
    uart_config.txDataInvert = 0;

    uart_config.uartSourceClockInHz = serial_get_clock(obj->index);

    clock_manager_set_gate(kClockModuleUART, obj->index, true);
    uart_hal_init(obj->index, &uart_config);

    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    pin_mode(tx, PullUp);
    pin_mode(rx, PullUp);

    if (obj->index == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
    serial_irq_ids[obj->index] = 0;
}

void serial_baud(serial_t *obj, int baudrate) {
    uart_hal_set_baud_rate(obj->index, serial_get_clock(obj->index), (uint32_t)baudrate);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    uart_hal_configure_bit_count_per_char(obj->index, (uart_bit_count_per_char_t)data_bits);
    uart_hal_configure_parity_mode(obj->index, (uart_parity_mode_t)parity);
    uart_hal_configure_stop_bit_count(obj->index, (uart_stop_bit_count_t)stop_bits);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t transmit_empty, uint32_t receive_full, uint32_t index) {
    if (serial_irq_ids[index] != 0) {
        if (transmit_empty)
            irq_handler(serial_irq_ids[index], TxIrq);

    if (receive_full)
        irq_handler(serial_irq_ids[index], RxIrq);
    }
}

void uart0_irq() {
    uart_irq(uart_hal_is_transmit_data_register_empty(0), uart_hal_is_receive_data_register_full(0), 0);
    if (uart_hal_is_receive_overrun_detected(0))
        uart_hal_clear_status_flag(0, kUartReceiveOverrun);
}
void uart1_irq() {
    uart_irq(uart_hal_is_transmit_data_register_empty(1), uart_hal_is_receive_data_register_full(1), 1);
}

void uart2_irq() {
    uart_irq(uart_hal_is_transmit_data_register_empty(2), uart_hal_is_receive_data_register_full(2), 2);
}

void uart3_irq() {
    uart_irq(uart_hal_is_transmit_data_register_empty(3), uart_hal_is_receive_data_register_full(3), 3);
}

void uart4_irq() {
    uart_irq(uart_hal_is_transmit_data_register_empty(4), uart_hal_is_receive_data_register_full(4), 4);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    switch (obj->index) {
        case 0: irq_n=UART0_RX_TX_IRQn; vector = (uint32_t)&uart0_irq; break;
        case 1: irq_n=UART1_RX_TX_IRQn; vector = (uint32_t)&uart1_irq; break;
        case 2: irq_n=UART2_RX_TX_IRQn; vector = (uint32_t)&uart2_irq; break;
        case 3: irq_n=UART3_RX_TX_IRQn; vector = (uint32_t)&uart3_irq; break;
        case 4: irq_n=UART4_RX_TX_IRQn; vector = (uint32_t)&uart4_irq; break;
    }

    if (enable) {
        switch (irq) {
            case RxIrq: uart_hal_enable_rx_data_register_full_interrupt(obj->index); break;
            case TxIrq: uart_hal_enable_tx_data_register_empty_interrupt(obj->index); break;
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { // disable
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        switch (irq) {
            case RxIrq: uart_hal_disable_rx_data_register_full_interrupt(obj->index); break;
            case TxIrq: uart_hal_disable_tx_data_register_empty_interrupt(obj->index); break;
        }
        switch (other_irq) {
            case RxIrq: all_disabled = uart_hal_is_receive_data_full_interrupt_enabled(obj->index) == 0; break;
            case TxIrq: all_disabled = uart_hal_is_tx_data_register_empty_interrupt_enabled(obj->index) == 0; break;
        }
        if (all_disabled)
            NVIC_DisableIRQ(irq_n);
    }
}

int serial_getc(serial_t *obj) {
    while (!serial_readable(obj));
    uint8_t data;
    uart_hal_getchar(obj->index, &data);

    return data;
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
    uart_hal_putchar(obj->index, (uint8_t)c);
}

int serial_readable(serial_t *obj) {
    if (uart_hal_is_receive_overrun_detected(obj->index))
        uart_hal_clear_status_flag(obj->index, kUartReceiveOverrun);
    return uart_hal_is_receive_data_register_full(obj->index);
}

int serial_writable(serial_t *obj) {
    if (uart_hal_is_receive_overrun_detected(obj->index))
        uart_hal_clear_status_flag(obj->index, kUartReceiveOverrun);

    return uart_hal_is_transmit_data_register_empty(obj->index);
}

void serial_clear(serial_t *obj) {
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
    uart_hal_queue_break_char_to_send(obj->index, true);
}

void serial_break_clear(serial_t *obj) {
    uart_hal_queue_break_char_to_send(obj->index, false);
}

