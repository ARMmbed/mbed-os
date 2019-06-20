/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "pinmap.h"
#include "mbed_error.h"
#include "gpio_include.h"
#include "objects.h"

static const PinMap PinMap_UART_TX[] = {
    {PA1, SERIAL_0, PIN_DATA(1, 1)},
    {PJ1, SERIAL_1, PIN_DATA(1, 1)},
    {PB2, SERIAL_2, PIN_DATA(1, 1)},
    {PA6, SERIAL_3, PIN_DATA(2, 1)},
    {PV6, SERIAL_4, PIN_DATA(1, 1)},
    {PN2, SERIAL_5, PIN_DATA(2, 1)},
    {NC,  NC,       0}
};

static const PinMap PinMap_UART_RX[] = {
    {PA2, SERIAL_0, PIN_DATA(1, 0)},
    {PJ2, SERIAL_1, PIN_DATA(1, 0)},
    {PB3, SERIAL_2, PIN_DATA(1, 0)},
    {PA7, SERIAL_3, PIN_DATA(2, 0)},
    {PV7, SERIAL_4, PIN_DATA(1, 0)},
    {PN3, SERIAL_5, PIN_DATA(2, 0)},
    {NC, NC,        0}
};

#if DEVICE_SERIAL_FC
static const PinMap PinMap_UART_CTS[] = {
    {PM3, SERIAL_0, PIN_DATA(1, 0)},
    {PJ3, SERIAL_1, PIN_DATA(1, 0)},
    {PB4, SERIAL_2, PIN_DATA(1, 0)},
    {PC5, SERIAL_4, PIN_DATA(1, 0)},
    {PN0, SERIAL_5, PIN_DATA(2, 0)},
    {NC,  NC,       0}
};

static const PinMap PinMap_UART_RTS[] = {
    {PM4, SERIAL_0, PIN_DATA(1, 1)},
    {PJ4, SERIAL_1, PIN_DATA(1, 1)},
    {PB5, SERIAL_2, PIN_DATA(1, 1)},
    {PC6, SERIAL_4, PIN_DATA(1, 1)},
    {PN1, SERIAL_5, PIN_DATA(2, 1)},
    {NC,  NC,       0}
};
#endif

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;
int stdio_uart_inited = 0;
serial_t stdio_uart;
static void uart_init(TSB_UART_TypeDef *UARTx, uart_inittypedef_t *InitStruct);
static void uart_get_boudrate_setting(uart_boudrate_t *brddiviser, uint32_t boudrate);
static void uart_swreset(TSB_UART_TypeDef *UARTx);

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    int is_stdio_uart = 0;

    UARTName uart_tx   = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx   = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart_name = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart_name != NC);

    obj->index = uart_name;
    switch (uart_name) {
        case SERIAL_0:
            obj->UARTx = TSB_UART0;
            TSB_CG_FSYSENB_IPENB05 = ENABLE;
            TSB_CG_FSYSENA_IPENA00 = ENABLE;
            TSB_CG_FSYSENA_IPENA11 = ENABLE;
            break;
        case SERIAL_1:
            obj->UARTx = TSB_UART1;
            TSB_CG_FSYSENB_IPENB06 = ENABLE;
            TSB_CG_FSYSENA_IPENA08 = ENABLE;
            break;
        case SERIAL_2:
            obj->UARTx = TSB_UART2;
            TSB_CG_FSYSENB_IPENB07 = ENABLE;
            TSB_CG_FSYSENA_IPENA01 = ENABLE;
            break;
        case SERIAL_3:
            obj->UARTx = TSB_UART3;
            TSB_CG_FSYSENB_IPENB08 = ENABLE;
            TSB_CG_FSYSENA_IPENA00 = ENABLE;
            break;
        case SERIAL_4:
            obj->UARTx = TSB_UART4;
            TSB_CG_FSYSENB_IPENB09 = ENABLE;
            TSB_CG_FSYSENA_IPENA17 = ENABLE;
            TSB_CG_FSYSENA_IPENA02 = ENABLE;
            break;
        case SERIAL_5:
            obj->UARTx = TSB_UART5;
            TSB_CG_FSYSENB_IPENB10 = ENABLE;
            TSB_CG_FSYSENA_IPENA12 = ENABLE;
            break;
        default:
            error("UART is not available");
            break;
    }

    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

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

    obj->uart_config.BaudRate = 9600;
    obj->uart_config.DataBits = 8;
    obj->uart_config.StopBits = 0;
    obj->uart_config.Parity   = ParityNone;
    obj->uart_config.FlowCtrl = FlowControlNone;

    uart_init(obj->UARTx, &obj->uart_config);
    is_stdio_uart = (uart_name == STDIO_UART) ? (1) : (0);

    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    obj->UARTx->TRANS = 0;
    obj->UARTx->CR0   = 0;
    obj->UARTx->CR1   = 0;

    uart_swreset(obj->UARTx);

    obj->uart_config.BaudRate = 0;
    obj->uart_config.DataBits = 0;
    obj->uart_config.StopBits = 0;
    obj->uart_config.Parity   = 0;
    obj->uart_config.Mode     = 0;
    obj->uart_config.FlowCtrl = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    obj->uart_config.BaudRate = baudrate;
    uart_init(obj->UARTx, &obj->uart_config);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));  // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits > 6) && (data_bits < 10));  // 0: 7 data bits ... 2: 9 data bits

    obj->uart_config.DataBits = data_bits;
    obj->uart_config.StopBits = stop_bits;
    obj->uart_config.Parity = parity;
    uart_init(obj->UARTx, &obj->uart_config);
}

void INTUART0TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], TxIrq);
}

void INTUART0RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_0], RxIrq);
}

void INTUART1TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], TxIrq);
}

void INTUART1RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_1], RxIrq);
}

void INTUART2TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_2], TxIrq);
}

void INTUART2RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_2], RxIrq);
}

void INTUART3TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_3], TxIrq);
}

void INTUART3RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_3], RxIrq);
}

void INTUART4TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_4], TxIrq);
}

void INTUART4RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_4], RxIrq);
}

void INTUART5TX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_5], TxIrq);
}

void INTUART5RX_IRQHandler(void)
{
    irq_handler(serial_irq_ids[SERIAL_5], RxIrq);
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

    while (!serial_readable(obj)) {  // Wait until Rx buffer is full
        // Do nothing
    }

    if (obj->uart_config.Mode & UART_ENABLE_TX) {
        obj->UARTx->TRANS &= 0x0D;
    }

    data = data | (obj->UARTx->DR & 0xFFU);

    if (obj->uart_config.Mode & UART_ENABLE_TX) {
        obj->UARTx->TRANS |= UART_ENABLE_TX;
    }

    return data;
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj)) {
        // Do nothing
    }

    if (obj->uart_config.Mode & UART_ENABLE_RX) {
        obj->UARTx->TRANS &= 0x0E;
    }

    obj->UARTx->DR = c & 0xFFU;

    if (obj->uart_config.Mode & UART_ENABLE_RX) {
        obj->UARTx->TRANS |= UART_ENABLE_RX;
    }
}

int serial_readable(serial_t *obj)
{
    int ret = 0;

    if ((obj->UARTx->SR & 0x0000000F) != 0) {
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

void serial_clear(serial_t *obj)
{
    obj->UARTx->FIFOCLR = 0x03;
}

void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    obj->UARTx->TRANS |= 0x08;
}

void serial_break_clear(serial_t *obj)
{
    obj->UARTx->TRANS &= ~(0x08);
}

#if DEVICE_SERIAL_FC
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    UARTName uart_cts  = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);
    UARTName uart_rts  = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_name = (UARTName)pinmap_merge(uart_cts, uart_rts);
    MBED_ASSERT((int)uart_name != NC);

    pinmap_pinout(rxflow, PinMap_UART_RTS);
    pinmap_pinout(txflow, PinMap_UART_CTS);
    pin_mode(txflow, PullUp);
    pin_mode(rxflow, PullUp);

    obj->UARTx->CR0 |= (3U << 9);
}
#endif

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

static void  uart_get_boudrate_setting(uart_boudrate_t *brddiviser, uint32_t boudrate)
{
    uint32_t clock     = 0;
    uint32_t k         = 0;
    uint64_t tx        = 0;
    uint64_t work      = 1;
    uint64_t p_range64 = 0;
    uint64_t boud64    = 0;
    uint64_t tx64      = 0;
    uint64_t work64    = 1;

    SystemCoreClockUpdate();  // Get the peripheral I/O clock frequency
    clock = SystemCoreClock;

    tx   =   (uint64_t)((uint64_t)clock << 6);
    tx   /=  work;
    tx64 =   (uint64_t)((uint64_t)clock << 8);
    tx64 /=  work64;
    work =   ((uint64_t)boudrate);
    tx   /=  work;
    tx   >>= 4;

    boud64    = (64U * boudrate);
    p_range64 = ((boud64 / 100) * 3);

    for (k = UART_RANGE_K_MIN; (k <= UART_RANGE_K_MAX); k++) {
        work = tx + k;
        if (work >= (uint64_t)((uint64_t)1 << 6)) {
            work -= (uint64_t)((uint64_t)1 << 6);
            work >>= 6;
            if ((UART_RANGE_N_MIN <= (uint32_t)work) && ((uint32_t)work <= UART_RANGE_N_MAX)) {
                work64 = work <<6;
                work64 = (uint64_t)(work64 + (64 - (uint64_t)k));
                work64 = (tx64 / work64);
                if (((boud64 - p_range64) <= work64) && (work64 <= (boud64 + p_range64))) {
                    brddiviser->brn = work;
                    brddiviser->brk = k;
                    break;
                }
            }
        }
    }
}

static void uart_init(TSB_UART_TypeDef *UARTx, uart_inittypedef_t *InitStruct)
{
    uart_boudrate_t UTx_brd = {0};
    uint32_t brk            = 0;
    uint32_t tmp            = 0;
    uint32_t parity_check   = 0;
    uint32_t data_length    = 0;

    UARTx->CLK = UART_PLESCALER_1;  //  Register Setting
    uart_get_boudrate_setting(&UTx_brd, InitStruct->BaudRate);
    UTx_brd.ken = UART_DIVISION_ENABLE;
    brk = (UTx_brd.brk << 16);
    UARTx->BRD = (UTx_brd.ken | brk | UTx_brd.brn);
    parity_check = (InitStruct->Parity == ParityOdd) ? 1 : ((InitStruct->Parity == ParityEven) ? 3 : 0);
    data_length = (InitStruct->DataBits) == 8 ? 1 : (((InitStruct->DataBits) == 7) ? 0 : 2);
    tmp = (((InitStruct->FlowCtrl) << 9) | ((InitStruct->StopBits) << 4) | (parity_check << 2) | data_length);
    UARTx->CR0 = tmp;
    UARTx->CR1 = (UART_RX_FIFO_FILL_LEVEL | UART_TX_INT_ENABLE | UART_RX_INT_ENABLE);
    UARTx->FIFOCLR = (UARTxFIFOCLR_TFCLR_CLEAR | UARTxFIFOCLR_RFCLR_CLEAR);
    UARTx->TRANS = InitStruct->Mode;
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
