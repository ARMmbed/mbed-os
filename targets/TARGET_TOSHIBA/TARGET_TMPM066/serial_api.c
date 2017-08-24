/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include <string.h>
#include "serial_api.h"
#include "PeripheralNames.h"
#include "pinmap.h"

#define UART_NUM              2

static const PinMap PinMap_UART_TX[] = {
    {PC2, SERIAL_0, PIN_DATA(1, 1)},
    {PE2, SERIAL_1, PIN_DATA(1, 1)},
    {NC,  NC,       0}
};

static const PinMap PinMap_UART_RX[] = {
    {PC3, SERIAL_0, PIN_DATA(1, 0)},
    {PE1, SERIAL_1, PIN_DATA(1, 0)},
    {NC,  NC,       0}
};

static int serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    int is_stdio_uart = 0;

    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart_name = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart_name != NC);

    obj->index = uart_name;
    // Initialize UART instance
    switch (uart_name) {
        case SERIAL_0:
            CG_SetFcPeriphA(CG_FC_PERIPH_SIO0, ENABLE);
            obj->UARTx = UART0;
            break;
        case SERIAL_1:
            CG_SetFcPeriphB(CG_FC_PERIPH_SIO1, ENABLE);
            obj->UARTx = UART1;
            break;
        default:
            break;
    }

    obj->uart_config.BaudRate = 9600;
    obj->uart_config.DataBits = UART_DATA_BITS_8;
    obj->uart_config.StopBits = UART_STOP_BITS_1;
    obj->uart_config.Parity   = UART_NO_PARITY;
    obj->uart_config.FlowCtrl = UART_NONE_FLOW_CTRL;

    if (tx != NC && rx != NC) {
        obj->uart_config.Mode = UART_ENABLE_RX | UART_ENABLE_TX;
    } else {
        if (tx != NC) {
            obj->uart_config.Mode = UART_ENABLE_TX;
        } else {
            if (rx != NC) {
                obj->uart_config.Mode = UART_ENABLE_RX;
            }
        }
    }

    // Pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);
    // Initialize UART configure
    UART_Enable(obj->UARTx);
    UART_SetIdleMode(obj->UARTx, ENABLE);
    UART_Init(obj->UARTx, &obj->uart_config);
    is_stdio_uart = (uart_name == STDIO_UART) ? (1) : (0);
    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    // Disable UART
    UART_Disable(obj->UARTx);
    UART_SWReset(obj->UARTx);
    // Set information of object to invalid
    obj->uart_config.BaudRate = 0;
    obj->uart_config.DataBits = 0;
    obj->uart_config.StopBits = 0;
    obj->uart_config.Parity = 0;
    obj->uart_config.Mode = 0;
    obj->uart_config.FlowCtrl = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    obj->uart_config.BaudRate = baudrate;
    UART_Init(obj->UARTx, &obj->uart_config);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven) ||
                (parity == ParityForced1) || (parity == ParityForced0));
    // 0: 7 data bits ... 2: 9 data bits
    MBED_ASSERT((data_bits > 6) && (data_bits < 10));

    obj->uart_config.DataBits = data_bits;
    obj->uart_config.StopBits = stop_bits;
    obj->uart_config.Parity = parity;
    UART_Init(obj->UARTx, &obj->uart_config);
}

// INTERRUPTS HANDLING
void INTTX0_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], TxIrq);
}

void INTRX0_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], RxIrq);
}

void INTTX1_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], TxIrq);
}

void INTRX1_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], RxIrq);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    switch (obj->index) {
        case SERIAL_0:
            if (irq == RxIrq) {
                irq_n = INTRX0_IRQn;
            } else {
                irq_n = INTTX0_IRQn;
            }
            break;
        case SERIAL_1:
            if (irq == RxIrq) {
                irq_n = INTRX1_IRQn;
            } else {
                irq_n = INTTX1_IRQn;
            }
            break;
        default:
            break;
    }

    if (enable) {
        NVIC_EnableIRQ(irq_n);
    } else {
        NVIC_DisableIRQ(irq_n);
    }
}

int serial_getc(serial_t *obj)
{
    // Wait until Rx buffer is full
    while (!serial_readable(obj)) {
        // Do nothing
    }

    return UART_GetRxData(obj->UARTx);
}

void serial_putc(serial_t *obj, int c)
{
    // Wait until Tx buffer is empty
    while (!serial_writable(obj)) {
        // Do nothing
    }

    UART_SetTxData(obj->UARTx, (uint32_t)c);
}

int serial_readable(serial_t *obj)
{
    int ret = 0;

    if (UART_GetBufState(obj->UARTx, UART_RX) == DONE) {
        ret = 1;
    }

    return ret;
}

int serial_writable(serial_t *obj)
{
    int ret = 0;

    if (UART_GetBufState(obj->UARTx, UART_TX) == DONE) {
        ret = 1;
    }

    return ret;
}

void serial_clear(serial_t *obj)
{
    UART_TxFIFOClear(obj->UARTx);
    UART_RxFIFOClear(obj->UARTx);
    UART_TxBufferClear(obj->UARTx);
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
}

void serial_break_clear(serial_t *obj)
{
}
