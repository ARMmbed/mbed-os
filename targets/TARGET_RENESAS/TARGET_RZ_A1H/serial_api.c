/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
// math.h required for floating point operations for baud rate calculation
#include "mbed_assert.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "serial_api.h"
#include "cmsis.h"
#include "pinmap.h"
#include "gpio_api.h"

#include "scif_iodefine.h"
#include "cpg_iodefine.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#define PCLK (66666666)     // Define the peripheral clock P1 frequency.

#define UART_NUM    8
#define IRQ_NUM     4

static void uart0_tx_irq(void);
static void uart1_tx_irq(void);
static void uart2_tx_irq(void);
static void uart3_tx_irq(void);
static void uart4_tx_irq(void);
static void uart5_tx_irq(void);
static void uart6_tx_irq(void);
static void uart7_tx_irq(void);
static void uart0_rx_irq(void);
static void uart1_rx_irq(void);
static void uart2_rx_irq(void);
static void uart3_rx_irq(void);
static void uart4_rx_irq(void);
static void uart5_rx_irq(void);
static void uart6_rx_irq(void);
static void uart7_rx_irq(void);
static void uart0_er_irq(void);
static void uart1_er_irq(void);
static void uart2_er_irq(void);
static void uart3_er_irq(void);
static void uart4_er_irq(void);
static void uart5_er_irq(void);
static void uart6_er_irq(void);
static void uart7_er_irq(void);

static void serial_put_done(serial_t *obj);
static uint8_t serial_available_buffer(serial_t *obj);
static void serial_irq_err_set(serial_t *obj, uint32_t enable);

static const PinMap PinMap_UART_TX[] = {
    {P2_14 , UART0, 6},
    {P2_5  , UART1, 6},
    {P4_12 , UART1, 7},
    {P6_3  , UART2, 7},
    {P4_14 , UART2, 7},
    {P5_3  , UART3, 5},
    {P8_8  , UART3, 7},
    {P5_0  , UART4, 5},
    {P8_14 , UART4, 7},
    {P8_13 , UART5, 5},
    {P11_10, UART5, 3},
    {P6_6  , UART5, 5},
    {P5_6  , UART6, 5},
    {P11_1 , UART6, 4},
    {P7_4  , UART7, 4},
    {NC    , NC   , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {P2_15 , UART0, 6},
    {P2_6  , UART1, 6},
    {P4_13 , UART1, 7},
    {P6_2  , UART2, 7},
    {P4_15 , UART2, 7},
    {P5_4  , UART3, 5},
    {P8_9  , UART3, 7},
    {P5_1  , UART4, 5},
    {P8_15 , UART4, 7},
    {P8_11 , UART5, 5},
    {P11_11, UART5, 3},
    {P6_7  , UART5, 5},
    {P5_7  , UART6, 5},
    {P11_2 , UART6, 4},
    {P7_5  , UART7, 4},
    {NC    , NC   , 0}
};

static const PinMap PinMap_UART_CTS[] = {
    {P2_3  , UART1, 6},
    {P11_7 , UART5, 3},
    {P7_6  , UART7, 4},
    {NC    , NC   , 0}
};
static const PinMap PinMap_UART_RTS[] = {
    {P2_7  , UART1, 6},
    {P11_8 , UART5, 3},
    {P7_7  , UART7, 4},
    {NC    , NC   , 0}
};



static const struct st_scif *SCIF[] = SCIF_ADDRESS_LIST;
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    gpio_t sw_rts, sw_cts;
    uint8_t rx_irq_set_flow, rx_irq_set_api;
    serial_t *tranferring_obj, *receiving_obj;
    uint32_t async_tx_callback, async_rx_callback;
    int event, wanted_rx_events;
};

static struct serial_global_data_s uart_data[UART_NUM];

static const IRQn_Type irq_set_tbl[UART_NUM][IRQ_NUM] = {
    {SCIFRXI0_IRQn, SCIFTXI0_IRQn, SCIFBRI0_IRQn, SCIFERI0_IRQn},
    {SCIFRXI1_IRQn, SCIFTXI1_IRQn, SCIFBRI1_IRQn, SCIFERI1_IRQn},
    {SCIFRXI2_IRQn, SCIFTXI2_IRQn, SCIFBRI2_IRQn, SCIFERI2_IRQn},
    {SCIFRXI3_IRQn, SCIFTXI3_IRQn, SCIFBRI3_IRQn, SCIFERI3_IRQn},
    {SCIFRXI4_IRQn, SCIFTXI4_IRQn, SCIFBRI4_IRQn, SCIFERI4_IRQn},
    {SCIFRXI5_IRQn, SCIFTXI5_IRQn, SCIFBRI5_IRQn, SCIFERI5_IRQn},
    {SCIFRXI6_IRQn, SCIFTXI6_IRQn, SCIFBRI6_IRQn, SCIFERI6_IRQn},
    {SCIFRXI7_IRQn, SCIFTXI7_IRQn, SCIFBRI7_IRQn, SCIFERI7_IRQn}
};

static const IRQHandler hander_set_tbl[UART_NUM][IRQ_NUM] = {
    {uart0_rx_irq, uart0_tx_irq, uart0_er_irq, uart0_er_irq},
    {uart1_rx_irq, uart1_tx_irq, uart1_er_irq, uart1_er_irq},
    {uart2_rx_irq, uart2_tx_irq, uart2_er_irq, uart2_er_irq},
    {uart3_rx_irq, uart3_tx_irq, uart3_er_irq, uart3_er_irq},
    {uart4_rx_irq, uart4_tx_irq, uart4_er_irq, uart4_er_irq},
    {uart5_rx_irq, uart5_tx_irq, uart5_er_irq, uart5_er_irq},
    {uart6_rx_irq, uart6_tx_irq, uart6_er_irq, uart6_er_irq},
    {uart7_rx_irq, uart7_tx_irq, uart7_er_irq, uart7_er_irq}
};

static __IO uint16_t *SCSCR_MATCH[] = {
    &SCSCR_0,
    &SCSCR_1,
    &SCSCR_2,
    &SCSCR_3,
    &SCSCR_4,
    &SCSCR_5,
    &SCSCR_6,
    &SCSCR_7,
};

static __IO uint16_t *SCFSR_MATCH[] = {
    &SCFSR_0,
    &SCFSR_1,
    &SCFSR_2,
    &SCFSR_3,
    &SCFSR_4,
    &SCFSR_5,
    &SCFSR_6,
    &SCFSR_7,
};


void serial_init(serial_t *obj, PinName tx, PinName rx) {
    volatile uint8_t dummy ;
    int is_stdio_uart = 0;
    // determine the UART to use
    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    uint32_t uart = pinmap_merge(uart_tx, uart_rx);

    MBED_ASSERT((int)uart != NC);

    obj->serial.uart = (struct st_scif *)SCIF[uart];
    // enable power
    switch (uart) {
    case UART0:
        CPG.STBCR4 &= ~(1 <<  7);
        break;
    case UART1:
        CPG.STBCR4 &= ~(1 <<  6);
        break;
    case UART2:
        CPG.STBCR4 &= ~(1 <<  5);
        break;
    case UART3:
        CPG.STBCR4 &= ~(1 <<  4);
        break;
    case UART4:
        CPG.STBCR4 &= ~(1 <<  3);
        break;
    case UART5:
        CPG.STBCR4 &= ~(1 <<  2);
        break;
    case UART6:
        CPG.STBCR4 &= ~(1 <<  1);
        break;
    case UART7:
        CPG.STBCR4 &= ~(1 <<  0);
        break;
    }
    dummy = CPG.STBCR4;

    /* ==== SCIF initial setting ==== */
    /* ---- Serial control register (SCSCR) setting ---- */
    /* B'00 : Internal CLK */
    obj->serial.uart->SCSCR = 0x0000u;          /* SCIF transmitting and receiving operations stop */

    /* ---- FIFO control register (SCFCR) setting ---- */
    /* Transmit FIFO reset & Receive FIFO data register reset */
    obj->serial.uart->SCFCR = 0x0006;

    /* ---- Serial status register (SCFSR) setting ---- */
    dummy = obj->serial.uart->SCFSR;
    obj->serial.uart->SCFSR = (dummy & 0xFF6Cu);         /* ER,BRK,DR bit clear */

    /* ---- Line status register (SCLSR) setting ---- */
    /* ORER bit clear */
    obj->serial.uart->SCLSR = 0;

    /* ---- Serial extension mode register (SCEMR) setting ----
    b7 BGDM - Baud rate generator double-speed mode  : Normal mode
    b0 ABCS - Base clock select in asynchronous mode : Base clock is 16 times the bit rate */
    obj->serial.uart->SCEMR = 0x0000u;

    /* ---- Bit rate register (SCBRR) setting ---- */
    serial_baud  (obj, 9600);
    serial_format(obj, 8, ParityNone, 1);

    /* ---- FIFO control register (SCFCR) setting ---- */
    obj->serial.uart->SCFCR = 0x0030u;

    /* ---- Serial port register (SCSPTR) setting ----
    b1 SPB2IO - Serial port break output : disabled
    b0 SPB2DT - Serial port break data   : High-level */
    obj->serial.uart->SCSPTR = 0x0003u;    // SPB2IO = 1, SPB2DT = 1

    /* ---- Line status register (SCLSR) setting ----
    b0 ORER - Overrun error detect : clear */

    if (obj->serial.uart->SCLSR & 0x0001) {
        obj->serial.uart->SCLSR = 0u;      // ORER clear
    }

    // pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    switch (uart) {
    case UART0:
        obj->serial.index = 0;
        break;
    case UART1:
        obj->serial.index = 1;
        break;
    case UART2:
        obj->serial.index = 2;
        break;
    case UART3:
        obj->serial.index = 3;
        break;
    case UART4:
        obj->serial.index = 4;
        break;
    case UART5:
        obj->serial.index = 5;
        break;
    case UART6:
        obj->serial.index = 6;
        break;
    case UART7:
        obj->serial.index = 7;
        break;
    }
    uart_data[obj->serial.index].sw_rts.pin = NC;
    uart_data[obj->serial.index].sw_cts.pin = NC;

    /* ---- Serial control register (SCSCR) setting ---- */
    /* Setting the TE and RE bits enables the TxD and RxD pins to be used. */
    obj->serial.uart->SCSCR = 0x0070;

    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);

    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
    uart_data[obj->serial.index].serial_irq_id = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate) {
    uint16_t DL;

    obj->serial.uart->SCSMR &= ~0x0003;

    if (baudrate > 32552) {
        obj->serial.uart->SCEMR = 0x0081;  // BGDM = 1, ABCS = 1
        DL = PCLK / (8 * baudrate);
        if (DL > 0) {
            DL--;
        }
        obj->serial.uart->SCBRR = (uint8_t)DL;
    } else if (baudrate > 16276) {
        obj->serial.uart->SCEMR = 0x0080;  // BGDM = 1
        obj->serial.uart->SCBRR = PCLK / (16 * baudrate) - 1;
    } else if (baudrate > 8138) {
        obj->serial.uart->SCEMR = 0x0000;
        obj->serial.uart->SCBRR = PCLK / (32 * baudrate) - 1;
    } else if (baudrate > 4169) {
        obj->serial.uart->SCSMR |= 0x0001;
        obj->serial.uart->SCEMR = 0x0080;  // BGDM = 1
        obj->serial.uart->SCBRR = PCLK / (64 * baudrate) - 1;
    } else if (baudrate > 2034) {
        obj->serial.uart->SCSMR |= 0x0001;
        obj->serial.uart->SCEMR = 0x0000;
        obj->serial.uart->SCBRR = PCLK / (128 * baudrate) - 1;
    } else if (baudrate > 1017) {
        obj->serial.uart->SCSMR |= 0x0002;
        obj->serial.uart->SCEMR = 0x0080;  // BGDM = 1
        obj->serial.uart->SCBRR = PCLK / (256 * baudrate) - 1;
    } else if (baudrate > 508) {
        obj->serial.uart->SCSMR |= 0x0002;
        obj->serial.uart->SCEMR = 0x0000;
        obj->serial.uart->SCBRR = PCLK / (512 * baudrate) - 1;
    } else if (baudrate > 254) {
        obj->serial.uart->SCSMR |= 0x0003;
        obj->serial.uart->SCEMR = 0x0080;  // BGDM = 1
        obj->serial.uart->SCBRR = PCLK / (1024 * baudrate) - 1;
    } else if (baudrate > 127) {
        obj->serial.uart->SCSMR |= 0x0003;
        obj->serial.uart->SCEMR = 0x0000;
        obj->serial.uart->SCBRR = PCLK / (2048 * baudrate) - 1;
    } else {
        obj->serial.uart->SCSMR |= 0x0003;
        obj->serial.uart->SCEMR = 0x0000;
        obj->serial.uart->SCBRR = 0xFFu;
    }
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    int parity_enable;
    int parity_select;

    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2)); // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((data_bits > 4) && (data_bits < 9)); // 5: 5 data bits ... 3: 8 data bits
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven) ||
                (parity == ParityForced1) || (parity == ParityForced0));

    stop_bits = (stop_bits == 1)? 0:
                (stop_bits == 2)? 1:
                0; // must not to be

    data_bits = (data_bits == 8)? 0:
                (data_bits == 7)? 1:
                0; // must not to be

    switch (parity) {
    case ParityNone:
        parity_enable = 0;
        parity_select = 0;
        break;
    case ParityOdd:
        parity_enable = 1;
        parity_select = 1;
        break;
    case ParityEven:
        parity_enable = 1;
        parity_select = 0;
        break;
    case ParityForced1:
    case ParityForced0:
    default:
        parity_enable = 0;
        parity_select = 0;
        break;
    }

    obj->serial.uart->SCSMR = (obj->serial.uart->SCSMR & ~0x0078)
                       | (data_bits       << 6)
                       | (parity_enable   << 5)
                       | (parity_select   << 4)
                       | (stop_bits       << 3);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_tx_irq(IRQn_Type irq_num, uint32_t index) {
    __IO uint16_t *dmy_rd_scscr;
    __IO uint16_t *dmy_rd_scfsr;
    serial_t *obj;
    int i;
    
    dmy_rd_scscr = SCSCR_MATCH[index];
    *dmy_rd_scscr &= 0x007B;                    // Clear TIE and Write to bit15~8,2 is always 0
    dmy_rd_scfsr = SCFSR_MATCH[index];
    *dmy_rd_scfsr = (*dmy_rd_scfsr & ~0x0020);  // Set TEND
    
    obj = uart_data[index].tranferring_obj;
    if (obj) {
        i = obj->tx_buff.length - obj->tx_buff.pos;
        if (0 < i) {
            if (serial_available_buffer(obj) < i) {
                i = serial_available_buffer(obj);
            }
            do {
                uint8_t c = *(uint8_t *)obj->tx_buff.buffer;
                obj->tx_buff.buffer = (uint8_t *)obj->tx_buff.buffer + 1;
                ++obj->tx_buff.pos;
                obj->serial.uart->SCFTDR = c;
            } while (--i);
            serial_put_done(obj);
        } else {
            uart_data[index].tranferring_obj = NULL;
            uart_data[index].event = SERIAL_EVENT_TX_COMPLETE;
            ((void (*)())uart_data[index].async_tx_callback)();
        }
    }
    
    irq_handler(uart_data[index].serial_irq_id, TxIrq);
}

static void uart_rx_irq(IRQn_Type irq_num, uint32_t index) {
    __IO uint16_t *dmy_rd_scscr;
    __IO uint16_t *dmy_rd_scfsr;
    serial_t *obj;
    int c;
    
    dmy_rd_scscr = SCSCR_MATCH[index];
    *dmy_rd_scscr &= 0x00B3;                    // Clear RIE,REIE and Write to bit15~8,2 is always 0
    dmy_rd_scfsr = SCFSR_MATCH[index];
    *dmy_rd_scfsr = (*dmy_rd_scfsr & ~0x0003);  // Clear RDF,DR
    
    obj = uart_data[index].receiving_obj;
    if (obj) {
        if (obj->serial.uart->SCLSR & 1) {
            if (uart_data[index].wanted_rx_events & SERIAL_EVENT_RX_OVERRUN_ERROR) {
                serial_rx_abort_asynch(obj);
                uart_data[index].event = SERIAL_EVENT_RX_OVERRUN_ERROR;
                ((void (*)())uart_data[index].async_rx_callback)();
            }
            return;
        }
        c = serial_getc(obj);
        if (c != -1) {
            ((uint8_t *)obj->rx_buff.buffer)[obj->rx_buff.pos] = c;
            ++obj->rx_buff.pos;
            if (c == obj->char_match && ! obj->char_found) {
                obj->char_found = 1;
                if (obj->rx_buff.pos == obj->rx_buff.length) {
                    if (uart_data[index].wanted_rx_events & SERIAL_EVENT_RX_COMPLETE) {
                        uart_data[index].event = SERIAL_EVENT_RX_COMPLETE;
                    }
                }
                if (uart_data[index].wanted_rx_events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
                    uart_data[index].event |= SERIAL_EVENT_RX_CHARACTER_MATCH;
                }
                if (uart_data[index].event) {
                    uart_data[index].receiving_obj = NULL;
                    ((void (*)())uart_data[index].async_rx_callback)();
                }
            } else if (obj->rx_buff.pos == obj->rx_buff.length) {
                uart_data[index].receiving_obj = NULL;
                if (uart_data[index].wanted_rx_events & SERIAL_EVENT_RX_COMPLETE) {
                    uart_data[index].event = SERIAL_EVENT_RX_COMPLETE;
                    ((void (*)())uart_data[index].async_rx_callback)();
                }
            }
        } else {
            serial_rx_abort_asynch(obj);
            if (uart_data[index].wanted_rx_events & (SERIAL_EVENT_RX_PARITY_ERROR | SERIAL_EVENT_RX_FRAMING_ERROR)) {
                uart_data[index].event = SERIAL_EVENT_RX_PARITY_ERROR | SERIAL_EVENT_RX_FRAMING_ERROR;
                if (obj->serial.uart->SCFSR & 1 << 2) {
                    uart_data[index].event = SERIAL_EVENT_RX_PARITY_ERROR;
                } else if (obj->serial.uart->SCFSR & 1 << 3) {
                    uart_data[index].event = SERIAL_EVENT_RX_FRAMING_ERROR;
                }
                ((void (*)())uart_data[index].async_rx_callback)();
            }
            return;
        }
    }
    
    irq_handler(uart_data[index].serial_irq_id, RxIrq);
}

static void uart_err_irq(IRQn_Type irq_num, uint32_t index) {
    serial_t *obj = uart_data[index].receiving_obj;
    int was_masked, err_read;
    
    if (obj) {
        serial_irq_err_set(obj, 0);
        if (uart_data[index].wanted_rx_events & (SERIAL_EVENT_RX_PARITY_ERROR | SERIAL_EVENT_RX_FRAMING_ERROR)) {
            uart_data[index].event = SERIAL_EVENT_RX_PARITY_ERROR | SERIAL_EVENT_RX_FRAMING_ERROR;
            if (obj->serial.uart->SCFSR & 1 << 2) {
                uart_data[index].event = SERIAL_EVENT_RX_PARITY_ERROR;
            } else if (obj->serial.uart->SCFSR & 1 << 3) {
                uart_data[index].event = SERIAL_EVENT_RX_FRAMING_ERROR;
            }
            ((void (*)())uart_data[index].async_rx_callback)();
        }
        serial_rx_abort_asynch(obj);
        
#if defined ( __ICCARM__ )
        was_masked = __disable_irq_iar();
#else
        was_masked = __disable_irq();
#endif /* __ICCARM__ */
        if (obj->serial.uart->SCFSR & 0x93) {
            err_read = obj->serial.uart->SCFSR;
            obj->serial.uart->SCFSR = (err_read & ~0x93);
        }
        if (obj->serial.uart->SCLSR & 1) {
            obj->serial.uart->SCLSR = 0;
        }
        if (!was_masked) {
            __enable_irq();
        }
    }
}

/* TX handler */
static void uart0_tx_irq(void)  {
    uart_tx_irq(SCIFTXI0_IRQn, 0);
}
static void uart1_tx_irq(void)  {
    uart_tx_irq(SCIFTXI1_IRQn, 1);
}
static void uart2_tx_irq(void)  {
    uart_tx_irq(SCIFTXI2_IRQn, 2);
}
static void uart3_tx_irq(void)  {
    uart_tx_irq(SCIFTXI3_IRQn, 3);
}
static void uart4_tx_irq(void)  {
    uart_tx_irq(SCIFTXI4_IRQn, 4);
}
static void uart5_tx_irq(void)  {
    uart_tx_irq(SCIFTXI5_IRQn, 5);
}
static void uart6_tx_irq(void)  {
    uart_tx_irq(SCIFTXI6_IRQn, 6);
}
static void uart7_tx_irq(void)  {
    uart_tx_irq(SCIFTXI7_IRQn, 7);
}
/* RX handler */
static void uart0_rx_irq(void)  {
    uart_rx_irq(SCIFRXI0_IRQn, 0);
}
static void uart1_rx_irq(void)  {
    uart_rx_irq(SCIFRXI1_IRQn, 1);
}
static void uart2_rx_irq(void)  {
    uart_rx_irq(SCIFRXI2_IRQn, 2);
}
static void uart3_rx_irq(void)  {
    uart_rx_irq(SCIFRXI3_IRQn, 3);
}
static void uart4_rx_irq(void)  {
    uart_rx_irq(SCIFRXI4_IRQn, 4);
}
static void uart5_rx_irq(void)  {
    uart_rx_irq(SCIFRXI5_IRQn, 5);
}
static void uart6_rx_irq(void)  {
    uart_rx_irq(SCIFRXI6_IRQn, 6);
}
static void uart7_rx_irq(void)  {
    uart_rx_irq(SCIFRXI7_IRQn, 7);
}
/* Error handler */
static void uart0_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 0);
}
static void uart1_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 1);
}
static void uart2_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 2);
}
static void uart3_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 3);
}
static void uart4_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 4);
}
static void uart5_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 5);
}
static void uart6_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 6);
}
static void uart7_er_irq(void)
{
    uart_err_irq(SCIFERI0_IRQn, 7);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    uart_data[obj->serial.index].serial_irq_id = id;
}

static void serial_irq_set_irq(IRQn_Type IRQn, IRQHandler handler, uint32_t enable)
{
    if (enable) {
        InterruptHandlerRegister(IRQn, (void (*)(uint32_t))handler);
        GIC_SetPriority(IRQn, 5);
        GIC_EnableIRQ(IRQn);
    } else {
        GIC_DisableIRQ(IRQn);
    }
}

static void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable) {
    IRQn_Type IRQn;
    IRQHandler handler;

    IRQn = irq_set_tbl[obj->serial.index][irq];
    handler = hander_set_tbl[obj->serial.index][irq];

    if ((obj->serial.index >= 0) && (obj->serial.index <= 7)) {
        serial_irq_set_irq(IRQn, handler, enable);
    }
}

static void serial_irq_err_set(serial_t *obj, uint32_t enable)
{
    serial_irq_set_irq(irq_set_tbl[obj->serial.index][2], hander_set_tbl[obj->serial.index][2], enable);
    serial_irq_set_irq(irq_set_tbl[obj->serial.index][3], hander_set_tbl[obj->serial.index][3], enable);
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    if (RxIrq == irq) {
        uart_data[obj->serial.index].rx_irq_set_api = enable;
    }
    serial_irq_set_internal(obj, irq, enable);
}

static void serial_flow_irq_set(serial_t *obj, uint32_t enable) {
    uart_data[obj->serial.index].rx_irq_set_flow = enable;
    serial_irq_set_internal(obj, RxIrq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    uint16_t err_read;
    int data;
    int was_masked;

#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */
    if (obj->serial.uart->SCFSR & 0x93) {
        err_read = obj->serial.uart->SCFSR;
        obj->serial.uart->SCFSR = (err_read & ~0x93);
    }
    obj->serial.uart->SCSCR |= 0x0040;     // Set RIE
    if (!was_masked) {
        __enable_irq();
    }

    if (obj->serial.uart->SCLSR & 0x0001) {
        obj->serial.uart->SCLSR = 0u;      // ORER clear
    }

    while (!serial_readable(obj));
    data = obj->serial.uart->SCFRDR & 0xff;

#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */
    err_read = obj->serial.uart->SCFSR;
    obj->serial.uart->SCFSR = (err_read & 0xfffD);     // Clear RDF
    if (!was_masked) {
        __enable_irq();
    }

    if (err_read & 0x80) {
        data = -1;  //err
    }
    return data;
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
    obj->serial.uart->SCFTDR = c;
    serial_put_done(obj);
}

static void serial_put_done(serial_t *obj)
{
    int was_masked;
    volatile uint16_t dummy_read;
    
#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */
    dummy_read = obj->serial.uart->SCFSR;
    obj->serial.uart->SCFSR = (dummy_read & 0xff9f);  // Clear TEND/TDFE
    obj->serial.uart->SCSCR |= 0x0080;     // Set TIE
    if (!was_masked) {
        __enable_irq();
    }
}

int serial_readable(serial_t *obj) {
    return ((obj->serial.uart->SCFSR & 0x02) != 0);  // RDF
}

int serial_writable(serial_t *obj) {
    return ((obj->serial.uart->SCFSR & 0x20) != 0);  // TDFE
}

void serial_clear(serial_t *obj) {
    int was_masked;
#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */

    obj->serial.uart->SCFCR |=  0x06;          // TFRST = 1, RFRST = 1
    obj->serial.uart->SCFCR &= ~0x06;          // TFRST = 0, RFRST = 0
    obj->serial.uart->SCFSR &= ~0x0093u;       // ER, BRK, RDF, DR = 0

    if (!was_masked) {
        __enable_irq();
    }
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
    int was_masked;
#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */
    // TxD Output(L)
    obj->serial.uart->SCSPTR &= ~0x0001u;  // SPB2DT = 0
    obj->serial.uart->SCSCR &= ~0x0020u;   // TE = 0 (Output disable)
    if (!was_masked) {
        __enable_irq();
    }
}

void serial_break_clear(serial_t *obj) {
    int was_masked;
#if defined ( __ICCARM__ )
    was_masked = __disable_irq_iar();
#else
    was_masked = __disable_irq();
#endif /* __ICCARM__ */
    obj->serial.uart->SCSCR |= 0x0020u; // TE = 1 (Output enable)
    obj->serial.uart->SCSPTR |= 0x0001u; // SPB2DT = 1
    if (!was_masked) {
        __enable_irq();
    }
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
    // determine the UART to use
    int was_masked;

    serial_flow_irq_set(obj, 0);

    if (type == FlowControlRTSCTS) {
#if defined ( __ICCARM__ )
        was_masked = __disable_irq_iar();
#else
        was_masked = __disable_irq();
#endif /* __ICCARM__ */
        obj->serial.uart->SCFCR = 0x0008u;   // CTS/RTS enable
        if (!was_masked) {
            __enable_irq();
        }
        pinmap_pinout(rxflow, PinMap_UART_RTS);
        pinmap_pinout(txflow, PinMap_UART_CTS);
    } else {
#if defined ( __ICCARM__ )
        was_masked = __disable_irq_iar();
#else
        was_masked = __disable_irq();
#endif /* __ICCARM__ */
        obj->serial.uart->SCFCR = 0x0000u; // CTS/RTS diable
        if (!was_masked) {
            __enable_irq();
        }
    }
}

static uint8_t serial_available_buffer(serial_t *obj)
{
    return 1;
    /* Faster but unstable way */
    /*
     uint16_t ret = 16 - ((obj->serial.uart->SCFDR >> 8) & 0x1F);
     while (ret == 0) {
     ret = 16 - ((obj->serial.uart->SCFDR >> 8) & 0x1F);
     }
     MBED_ASSERT(0 < ret && ret <= 16);
     return ret;
     */
}

#if DEVICE_SERIAL_ASYNCH

/******************************************************************************
 * ASYNCHRONOUS HAL
 ******************************************************************************/

int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    int i;
    buffer_t *buf = &obj->tx_buff;
    struct serial_global_data_s *data = uart_data + obj->serial.index;
    
    if (tx_length == 0) {
        return 0;
    }
    
    buf->buffer = (void *)tx;
    buf->length = tx_length * tx_width / 8;
    buf->pos = 0;
    buf->width = tx_width;
    data->tranferring_obj = obj;
    data->async_tx_callback = handler;
    serial_irq_set(obj, TxIrq, 1);
    
    while (!serial_writable(obj));
    i = buf->length;
    if (serial_available_buffer(obj) < i) {
        i = serial_available_buffer(obj);
    }
    do {
        uint8_t c = *(uint8_t *)buf->buffer;
        obj->tx_buff.buffer = (uint8_t *)obj->tx_buff.buffer + 1;
        ++buf->pos;
        obj->serial.uart->SCFTDR = c;
    } while (--i);
    serial_put_done(obj);
    
    return buf->length;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    buffer_t *buf = &obj->rx_buff;
    struct serial_global_data_s *data = uart_data + obj->serial.index;
    
    if (rx_length == 0) {
        return;
    }
    
    buf->buffer = rx;
    buf->length = rx_length * rx_width / 8;
    buf->pos = 0;
    buf->width = rx_width;
    obj->char_match = char_match;
    obj->char_found = 0;
    data->receiving_obj = obj;
    data->async_rx_callback = handler;
    data->event = 0;
    data->wanted_rx_events = event;
    
    serial_irq_set(obj, RxIrq, 1);
    serial_irq_err_set(obj, 1);
}

uint8_t serial_tx_active(serial_t *obj)
{
    return uart_data[obj->serial.index].tranferring_obj != NULL;
}

uint8_t serial_rx_active(serial_t *obj)
{
    return uart_data[obj->serial.index].receiving_obj != NULL;
}

int serial_irq_handler_asynch(serial_t *obj)
{
    return uart_data[obj->serial.index].event;
}

void serial_tx_abort_asynch(serial_t *obj)
{
    uart_data[obj->serial.index].tranferring_obj = NULL;
    obj->serial.uart->SCFCR |= 1 << 2;
    obj->serial.uart->SCFCR &= ~(1 << 2);
}

void serial_rx_abort_asynch(serial_t *obj)
{
    uart_data[obj->serial.index].receiving_obj = NULL;
    obj->serial.uart->SCFCR |= 1 << 1;
    obj->serial.uart->SCFCR &= ~(1 << 1);
}

#endif
