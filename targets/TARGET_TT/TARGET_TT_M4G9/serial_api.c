/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "mbed_error.h"
#include "serial_api.h"
#include "pinmap.h"

#define UART_NUM                                            8
#define UART_ENABLE_RX                                      ((uint32_t)0x00000001)
#define UART_ENABLE_TX                                      ((uint32_t)0x00000002)
#define UARTxFIFOCLR_TFCLR_CLEAR                            ((uint32_t)0x00000002)
#define UARTxFIFOCLR_RFCLR_CLEAR                            ((uint32_t)0x00000001)
#define UARTxSWRST_SWRSTF_MASK                              ((uint32_t)0x00000080)
#define UARTxSWRST_SWRSTF_RUN                               ((uint32_t)0x00000080)
#define UARTxSWRST_SWRST_10                                 ((uint32_t)0x00000002)
#define UARTxSWRST_SWRST_01                                 ((uint32_t)0x00000001)
#define UART_RX_FIFO_FILL_LEVEL                             ((uint32_t)0x00000100)
#define FUART_ENABLE_RX                                     ((uint32_t)0x00000200)
#define FUART_ENABLE_TX                                     ((uint32_t)0x00000100)
#define BAUDRATE_DEFAULT                                    (9600)
#define CLR_REGISTER                                        (0x00)

static const PinMap PinMap_UART_TX[] = {
    {PE3, SERIAL_0, PIN_DATA(7, 1)},
    {PH1, SERIAL_1, PIN_DATA(3, 1)},
    {PG1, SERIAL_2, PIN_DATA(3, 1)},
    {PU7, SERIAL_3, PIN_DATA(7, 1)},
    {PU0, SERIAL_4, PIN_DATA(7, 1)},
    {PJ1, SERIAL_5, PIN_DATA(3, 1)},
    {NC,  NC,       0}
};

static const PinMap PinMap_UART_RX[] = {
    {PE2, SERIAL_0, PIN_DATA(7, 0)},
    {PH0, SERIAL_1, PIN_DATA(3, 0)},
    {PG0, SERIAL_2, PIN_DATA(3, 0)},
    {PU6, SERIAL_3, PIN_DATA(7, 0)},
    {PU1, SERIAL_4, PIN_DATA(7, 0)},
    {PJ0, SERIAL_5, PIN_DATA(3, 0)},
    {NC,  NC,       0}
};

static int serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

static void uart_swreset(TSB_UART_TypeDef *UARTx);
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    int is_stdio_uart = 0;
    obj->mode = 0;
    cg_t paramCG;
    paramCG.p_instance = TSB_CG;
    uart_clock_t prescal = {0};

    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart_name = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart_name != NC);

    obj->index = uart_name;
    // Initialize UART instance
    switch (uart_name) {
        case SERIAL_0:
            obj->UARTx = TSB_UART0;
            //Enable clock for UART0 and Port E
            TSB_CG_FSYSMENA_IPMENA23 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB06 = TXZ_ENABLE;
            break;
        case SERIAL_1:
            obj->UARTx = TSB_UART1;
            //Enable clock for UART1 and Port H
            TSB_CG_FSYSMENA_IPMENA24 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB09 = TXZ_ENABLE;
            break;
        case SERIAL_2:
            obj->UARTx = TSB_UART2;
            //Enable clock for UART2 and Port G
            TSB_CG_FSYSMENA_IPMENA25 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB08 = TXZ_ENABLE;
            break;
        case SERIAL_3:
            obj->UARTx = TSB_UART3;
            //Enable clock for UART3 and Port U
            TSB_CG_FSYSMENA_IPMENA26 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB18 = TXZ_ENABLE;
            break;
        case SERIAL_4:
            obj->UARTx = TSB_UART4;
            //Enable clock for UART4 and Port U
            TSB_CG_FSYSMENA_IPMENA27 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB18 = TXZ_ENABLE;
            break;
        case SERIAL_5:
            obj->UARTx = TSB_UART5;
            //Enable clock for UART5 and Port J
            TSB_CG_FSYSMENA_IPMENA28 = TXZ_ENABLE;
            TSB_CG_FSYSMENB_IPMENB10 = TXZ_ENABLE;
            break;
        default:
            break;
    }
    //Set alternate function
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    if (tx != NC && rx != NC) {
        obj->mode = UART_ENABLE_RX | UART_ENABLE_TX;
    } else {
        if (tx != NC) {
            obj->mode = UART_ENABLE_TX;
        } else {
            if (rx != NC) {
                obj->mode = UART_ENABLE_RX;
            }
        }
    }
    //software reset
    uart_swreset(obj->UARTx);
    //mbed default configurations
    obj->UARTx->CR0 |= (1U);                   // data lengh 8 bit No parity one stop bit
    prescal.prsel = UART_PLESCALER_1;
    uart_get_boudrate_setting(cg_get_mphyt0(&paramCG), &prescal, BAUDRATE_DEFAULT, &obj->boud_obj);
    obj->UARTx->BRD |=((obj->boud_obj.ken) | (obj->boud_obj.brk << 16) | (obj->boud_obj.brn));
    obj->UARTx->FIFOCLR = (UARTxFIFOCLR_TFCLR_CLEAR | UARTxFIFOCLR_RFCLR_CLEAR);             // Clear FIFO
    obj->UARTx->TRANS |= obj->mode;                                                          // Enable TX RX block.
    obj->UARTx->CR1 = (UART_RX_FIFO_FILL_LEVEL | UART_TX_INT_ENABLE | UART_RX_INT_ENABLE);
    is_stdio_uart = (uart_name == STDIO_UART) ? (1) : (0);
    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    obj->UARTx->TRANS = CLR_REGISTER;
    obj->UARTx->CR0 = CLR_REGISTER;
    obj->UARTx->CR1 = CLR_REGISTER;
    obj->UARTx = CLR_REGISTER;
    uart_swreset(obj->UARTx);
    obj->index = (uint32_t)NC;
}

void serial_baud(serial_t *obj, int baudrate)
{
    cg_t paramCG;
    paramCG.p_instance = TSB_CG;
    uart_clock_t prescal;
    prescal.prsel = UART_PLESCALER_1;
    uart_get_boudrate_setting(cg_get_mphyt0(&paramCG), &prescal, baudrate, &obj->boud_obj);
    obj->UARTx->BRD = CLR_REGISTER;               //clear BRD register
    obj->UARTx->BRD |=((obj->boud_obj.ken) | (obj->boud_obj.brk << 16) | (obj->boud_obj.brn));
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    uint32_t parity_check = 0;
    uint32_t data_length = 0;
    uint32_t tmp = 0;
    uint32_t sblen = 0;

    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));

    MBED_ASSERT((data_bits > 6) && (data_bits < 10));                       // 0: 7 data bits ... 2: 9 data bits
    parity_check = ((parity == ParityOdd) ? 1 :((parity == ParityEven) ? 3 : 0));
    data_length = (data_bits == 8 ? 1 :((data_bits == 7) ? 0 : 2));
    sblen = (stop_bits == 1) ? 0 : 1;                                       // 0: 1 stop bits, 1: 2 stop bits
    tmp = ((sblen << 4) |(parity_check << 2) | data_length);
    obj->UARTx->CR0 = tmp;
}

// INTERRUPT HANDLING
void INTUART0RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], RxIrq);
}

void INTUART0TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], TxIrq);
}

void INTUART1RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], RxIrq);
}

void INTUART1TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], TxIrq);
}

void INTUART2RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_2], RxIrq);
}

void INTUART2TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_2], TxIrq);
}

void INTUART3RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_3], RxIrq);
}

void INTUART3TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_3], TxIrq);
}

void INTUART4RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_4], RxIrq);
}

void INTUART4TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_4], TxIrq);
}

void INTUART5RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_5], RxIrq);
}

void INTUART5TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_5], TxIrq);
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
                irq_n = INTUART0RX_IRQn;
            } else {
                irq_n = INTUART0TX_IRQn;
            }
            break;
        case SERIAL_1:
            if (irq == RxIrq) {
                irq_n = INTUART1RX_IRQn;
            } else {
                irq_n = INTUART1TX_IRQn;
            }
            break;
        case SERIAL_2:
            if (irq == RxIrq) {
                irq_n = INTUART2RX_IRQn;
            } else {
                irq_n = INTUART2TX_IRQn;
            }
            break;
        case SERIAL_3:
            if (irq == RxIrq) {
                irq_n = INTUART3RX_IRQn;
            } else {
                irq_n = INTUART3TX_IRQn;
            }
            break;
        case SERIAL_4:
            if (irq == RxIrq) {
                irq_n = INTUART4RX_IRQn;
            } else {
                irq_n = INTUART4TX_IRQn;
            }
            break;
        case SERIAL_5:
            if (irq == RxIrq) {
                irq_n = INTUART5RX_IRQn;
            } else {
                irq_n = INTUART5TX_IRQn;
            }
            break;
        default:
            break;
    }

    NVIC_ClearPendingIRQ(irq_n);

    if (enable) {
        NVIC_EnableIRQ(irq_n);
    } else {
        NVIC_DisableIRQ(irq_n);
    }
}

int serial_getc(serial_t *obj)
{
    int data = 0;

    while (!serial_readable(obj)) {         // Wait until Rx buffer is full
        // Do nothing
    }
    //Read Data Register
    data = (obj->UARTx->DR & 0xFFU);
    obj->UARTx->SR |= (1U << 6);        // clear RXEND flag

    return data;
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj)) {
        // Do nothing
    }
    //Write Data Register
    obj->UARTx->DR = (c & 0xFF);
    while((obj->UARTx->SR & (1U << 14)) == 0) {
    }
    obj->UARTx->SR |= (1U << 14);       // clear TXEND flag
}

int serial_readable(serial_t *obj)
{
    int ret = 0;

    if ((obj->UARTx->SR & 0x000F) != 0) {
        ret = 1;
    }

    return ret;
}

int serial_writable(serial_t *obj)
{
    int ret = 0;

    if ((obj->UARTx->SR & 0x8000) == 0) {
        ret = 1;
    }

    return ret;
}

// Pause transmission
void serial_break_set(serial_t *obj)
{
    obj->UARTx->TRANS |= 0x08;
}

// Switch to normal transmission
void serial_break_clear(serial_t *obj)
{
    obj->UARTx->TRANS &= ~(0x08);
}

static void uart_swreset(TSB_UART_TypeDef *UARTx)
{
    while (((UARTx->SWRST) & UARTxSWRST_SWRSTF_MASK) == UARTxSWRST_SWRSTF_RUN) {
        // No process
    }

    UARTx->SWRST = UARTxSWRST_SWRST_10;
    UARTx->SWRST = UARTxSWRST_SWRST_01;

    while (((UARTx->SWRST) & UARTxSWRST_SWRSTF_MASK) == UARTxSWRST_SWRSTF_RUN) {
        // No process
    }
}

const PinMap *serial_tx_pinmap()
{
    return PinMap_UART_TX;
}

const PinMap *serial_rx_pinmap()
{
    return PinMap_UART_RX;
}

const PinMap *serial_cts_pinmap()
{
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_CTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_CTS;
}

const PinMap *serial_rts_pinmap()
{
#if !DEVICE_SERIAL_FC
    static const PinMap PinMap_UART_RTS[] = {
        {NC, NC, 0}
    };
#endif

    return PinMap_UART_RTS;
}
