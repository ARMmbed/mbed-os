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
typedef struct st_scif SCIF_TypeDef;
#include "cpg_iodefine.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#define UART_NUM    6

static const PinMap PinMap_UART_TX[] = {
    {P6_3 , P_SCIF2, 7},
    {P2_14, P_SCIF0, 6},
    {P5_0 , P_SCIF4, 5},
    {P5_3 , P_SCIF3, 5},
    {P5_6 , P_SCIF6, 5},
    {P2_5 , P_SCIF1, 6},
    {P8_14, P_SCIF4, 7},
    {P8_13, P_SCIF5, 5},
    {P7_5 , P_SCIF7, 4},
    {NC   , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {P6_2 , P_SCIF2, 7},
    {P2_15, P_SCIF0, 6},
    {P5_1 , P_SCIF4, 5},
    {P5_4 , P_SCIF3, 5},
    {P5_7 , P_SCIF6, 5},
    {P2_6 , P_SCIF1, 6},
    {P8_15, P_SCIF4, 7},
    {P8_11, P_SCIF5, 5},
    {P7_4 , P_SCIF7, 4},
    {NC   , NC    , 0}
};

/* [TODO] impliment hardware Flow Control, interrupt
static const PinMap PinMap_UART_RTS[] = {
    {P7_7,  (int)P_SCIF7, 4},
    {P2_7 , (int)P_SCIF1, 6},
    {NC,    NC,     0}
};

static const PinMap PinMap_UART_CTS[] = {
    {P7_6,  (int)P_SCIF7, 4},
    {P2_3,  (int)P_SCIF1, 6},
    {NC,    NC,     0}
};*/

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

struct serial_global_data_s {
    uint32_t serial_irq_id;
    gpio_t sw_rts, sw_cts;
    uint8_t count, rx_irq_set_flow, rx_irq_set_api;
};

static struct serial_global_data_s uart_data[UART_NUM];

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    int is_stdio_uart = 0;

    // determine the UART to use
    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    uint32_t uart = pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)uart != NC);

    obj->uart = (SCIF_TypeDef *)uart;
    // enable power
    switch (uart) {
        case P_SCIF0: CPG.STBCR4 &= ~(1 <<  7); break;
        case P_SCIF1: CPG.STBCR4 &= ~(1 <<  6); break;
        case P_SCIF2: CPG.STBCR4 &= ~(1 <<  5); break;
        case P_SCIF3: CPG.STBCR4 &= ~(1 <<  4); break;
        case P_SCIF4: CPG.STBCR4 &= ~(1 <<  3); break;
        case P_SCIF5: CPG.STBCR4 &= ~(1 <<  2); break;
        case P_SCIF6: CPG.STBCR4 &= ~(1 <<  1); break;
        case P_SCIF7: CPG.STBCR4 &= ~(1 <<  0); break;
    }
    volatile uint8_t dummy ; 
    dummy = CPG.STBCR4;

    /* ==== SCIF initial setting ==== */
    /* ---- Serial control register (SCSCR) setting ---- */
    /* B'00 : Internal CLK */
    obj->uart->SCSCR = 0x0000u;          /* SCIF transmitting and receiving operations stop */

    /* ---- FIFO control register (SCFCR) setting ---- */
    /* Transmit FIFO reset & Receive FIFO data register reset */
    obj->uart->SCFCR = 0x0006;

    /* ---- Serial status register (SCFSR) setting ---- */
    obj->uart->SCFSR &= 0xFF6Cu;         /* ER,BRK,DR bit clear */

    /* ---- Line status register (SCLSR) setting ---- */
    /* ORER bit clear */
    obj->uart->SCLSR = 0;

    /* ---- Serial extension mode register (SCEMR) setting ---- 
    b7 BGDM - Baud rate generator double-speed mode  : Normal mode
    b0 ABCS - Base clock select in asynchronous mode : Base clock is 16 times the bit rate */
    obj->uart->SCEMR = 0x0000u;

    /* ---- Bit rate register (SCBRR) setting ---- */
    serial_baud  (obj, 9600);
    serial_format(obj, 8, ParityNone, 1);

    /* ---- FIFO control register (SCFCR) setting ---- */
    obj->uart->SCFCR = 0x0030u;
    
    /* ---- Serial port register (SCSPTR) setting ---- 
    b1 SPB2IO - Serial port break output : disabled
    b0 SPB2DT - Serial port break data   : High-level */
    //obj->uart->SCSPTR |= 0x0000u;

    obj->uart->SCSCR = 0x00F0;

    // pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    switch (uart) {
        case P_SCIF0: obj->index = 0; break;
        case P_SCIF1: obj->index = 1; break;
        case P_SCIF2: obj->index = 2; break;
        case P_SCIF3: obj->index = 3; break;
        case P_SCIF4: obj->index = 4; break;
        case P_SCIF5: obj->index = 5; break;
        case P_SCIF6: obj->index = 6; break;
        case P_SCIF7: obj->index = 7; break;
    }
    uart_data[obj->index].sw_rts.pin = NC;
    uart_data[obj->index].sw_cts.pin = NC;
//    serial_set_flow_control(obj, FlowControlNone, NC, NC);

    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);

    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
    uart_data[obj->index].serial_irq_id = 0;
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate) {

    uint32_t PCLK = 66666666;

    uint16_t DL = (PCLK / (32 * baudrate)) -1;

    // set LCR[DLAB] to enable writing to divider registers
    obj->uart->SCBRR = DL;
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2)); // 0: 1 stop bits, 1: 2 stop bits
    MBED_ASSERT((data_bits > 6) && (data_bits < 9)); // 0: 5 data bits ... 3: 8 data bits
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven) ||
           (parity == ParityForced1) || (parity == ParityForced0));

    stop_bits = (stop_bits == 1)? 0:
                (stop_bits == 2)? 1:
                0; // must not to be

    data_bits = (data_bits == 8)? 0:
                (data_bits == 7)? 1:
                0; // must not to be

    int parity_enable, parity_select;
    switch (parity) {
        case ParityNone: parity_enable = 0; parity_select = 0; break;
        case ParityOdd : parity_enable = 1; parity_select = 0; break;
        case ParityEven: parity_enable = 1; parity_select = 1; break;
        default:
            parity_enable = 0, parity_select = 0;
            break;
    }

    obj->uart->SCSMR = data_bits          << 6
                   | parity_enable        << 5
                   | parity_select        << 4
                   | stop_bits            << 3;
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_tx_irq(IRQn_Type irq_num, uint32_t index) {
    uint16_t dummy_read;
    /* Clear TDFE */
    switch (index) {
        case 0:
            dummy_read = SCFSR_0;
            SCFSR_0 = (dummy_read & ~0x0060);
            break;
        case 1:
            dummy_read = SCFSR_1;
            SCFSR_1 = (dummy_read & ~0x0060);
            break;
        case 2:
            dummy_read = SCFSR_2;
            SCFSR_2 = (dummy_read & ~0x0060);
            break;
        case 3:
            dummy_read = SCFSR_3;
            SCFSR_3 = (dummy_read & ~0x0060);
            break;
        case 4:
            dummy_read = SCFSR_4;
            SCFSR_4 = (dummy_read & ~0x0060);
            break;
        case 5:
            dummy_read = SCFSR_5;
            SCFSR_5 = (dummy_read & ~0x0060);
            break;
        case 6:
            dummy_read = SCFSR_6;
            SCFSR_6 = (dummy_read & ~0x0060);
            break;
        case 7:
            dummy_read = SCFSR_7;
            SCFSR_7 = (dummy_read & ~0x0060);
            break;
    }
    irq_handler(uart_data[index].serial_irq_id, TxIrq);
    GIC_EndInterrupt(irq_num);
}

static void uart_rx_irq(IRQn_Type irq_num, uint32_t index) {
    uint16_t dummy_read;
    /* Clear RDF */
    switch (index) {
        case 0:
            dummy_read = SCFSR_0;
            SCFSR_0 = (dummy_read & ~0x0003);
            break;
        case 1:
            dummy_read = SCFSR_1;
            SCFSR_1 = (dummy_read & ~0x0003);
            break;
        case 2:
            dummy_read = SCFSR_2;
            SCFSR_2 = (dummy_read & ~0x0003);
            break;
        case 3:
            dummy_read = SCFSR_3;
            SCFSR_3 = (dummy_read & ~0x0003);
            break;
        case 4:
            dummy_read = SCFSR_4;
            SCFSR_4 = (dummy_read & ~0x0003);
            break;
        case 5:
            dummy_read = SCFSR_5;
            SCFSR_5 = (dummy_read & ~0x0003);
            break;
        case 6:
            dummy_read = SCFSR_6;
            SCFSR_6 = (dummy_read & ~0x0003);
            break;
        case 7:
            dummy_read = SCFSR_7;
            SCFSR_7 = (dummy_read & ~0x0003);
            break;
    }
    irq_handler(uart_data[index].serial_irq_id, RxIrq);
    GIC_EndInterrupt(irq_num);
}
/* TX handler */
void uart0_tx_irq()  {uart_tx_irq(SCIFTXI0_IRQn, 0);}
void uart1_tx_irq()  {uart_tx_irq(SCIFTXI1_IRQn, 1);}
void uart2_tx_irq()  {uart_tx_irq(SCIFTXI2_IRQn, 2);}
void uart3_tx_irq()  {uart_tx_irq(SCIFTXI3_IRQn, 3);}
void uart4_tx_irq()  {uart_tx_irq(SCIFTXI4_IRQn, 4);}
void uart5_tx_irq()  {uart_tx_irq(SCIFTXI5_IRQn, 5);}
void uart6_tx_irq()  {uart_tx_irq(SCIFTXI6_IRQn, 6);}
void uart7_tx_irq()  {uart_tx_irq(SCIFTXI7_IRQn, 7);}
/* RX handler */
void uart0_rx_irq()  {uart_rx_irq(SCIFRXI0_IRQn, 0);}
void uart1_rx_irq()  {uart_rx_irq(SCIFRXI1_IRQn, 1);}
void uart2_rx_irq()  {uart_rx_irq(SCIFRXI2_IRQn, 2);}
void uart3_rx_irq()  {uart_rx_irq(SCIFRXI3_IRQn, 3);}
void uart4_rx_irq()  {uart_rx_irq(SCIFRXI4_IRQn, 4);}
void uart5_rx_irq()  {uart_rx_irq(SCIFRXI5_IRQn, 5);}
void uart6_rx_irq()  {uart_rx_irq(SCIFRXI6_IRQn, 6);}
void uart7_rx_irq()  {uart_rx_irq(SCIFRXI7_IRQn, 7);}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    uart_data[obj->index].serial_irq_id = id;
}

static void serial_irq_set_internal(serial_t *obj, SerialIrq irq, uint32_t enable) {
    switch (obj->index){
        case 0:
            InterruptHandlerRegister(SCIFTXI0_IRQn, (void (*)(uint32_t))uart0_tx_irq);
            InterruptHandlerRegister(SCIFRXI0_IRQn, (void (*)(uint32_t))uart0_rx_irq);
            GIC_SetPriority(SCIFTXI0_IRQn, 5);
            GIC_SetPriority(SCIFRXI0_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI0_IRQn);
            GIC_EnableIRQ(SCIFRXI0_IRQn);
            break;
        case 1:
            InterruptHandlerRegister(SCIFTXI1_IRQn, (void (*)(uint32_t))uart1_tx_irq);
            InterruptHandlerRegister(SCIFRXI1_IRQn, (void (*)(uint32_t))uart1_rx_irq);
            GIC_SetPriority(SCIFTXI1_IRQn, 5);
            GIC_SetPriority(SCIFRXI1_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI1_IRQn);
            GIC_EnableIRQ(SCIFRXI1_IRQn);
            break;
        case 2:
            InterruptHandlerRegister(SCIFTXI2_IRQn, (void (*)(uint32_t))uart2_tx_irq);
            InterruptHandlerRegister(SCIFRXI2_IRQn, (void (*)(uint32_t))uart2_rx_irq);
            GIC_SetPriority(SCIFTXI2_IRQn, 5);
            GIC_SetPriority(SCIFRXI2_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI2_IRQn);
            GIC_EnableIRQ(SCIFRXI2_IRQn);
            break;
        case 3:
            InterruptHandlerRegister(SCIFTXI3_IRQn, (void (*)(uint32_t))uart3_tx_irq);
            InterruptHandlerRegister(SCIFRXI3_IRQn, (void (*)(uint32_t))uart3_rx_irq);
            GIC_SetPriority(SCIFTXI3_IRQn, 5);
            GIC_SetPriority(SCIFRXI3_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI3_IRQn);
            GIC_EnableIRQ(SCIFRXI3_IRQn);
            break;
        case 4:
            InterruptHandlerRegister(SCIFTXI4_IRQn, (void (*)(uint32_t))uart4_tx_irq);
            InterruptHandlerRegister(SCIFRXI4_IRQn, (void (*)(uint32_t))uart4_rx_irq);
            GIC_SetPriority(SCIFTXI4_IRQn, 5);
            GIC_SetPriority(SCIFRXI4_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI4_IRQn);
            GIC_EnableIRQ(SCIFRXI4_IRQn);
            break;
        case 5:
            InterruptHandlerRegister(SCIFTXI5_IRQn, (void (*)(uint32_t))uart5_tx_irq);
            InterruptHandlerRegister(SCIFRXI5_IRQn, (void (*)(uint32_t))uart5_rx_irq);
            GIC_SetPriority(SCIFTXI5_IRQn, 5);
            GIC_SetPriority(SCIFRXI5_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI5_IRQn);
            GIC_EnableIRQ(SCIFRXI5_IRQn);
            break;
        case 6:
            InterruptHandlerRegister(SCIFTXI6_IRQn, (void (*)(uint32_t))uart6_tx_irq);
            InterruptHandlerRegister(SCIFRXI6_IRQn, (void (*)(uint32_t))uart6_rx_irq);
            GIC_SetPriority(SCIFTXI6_IRQn, 5);
            GIC_SetPriority(SCIFRXI6_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI6_IRQn);
            GIC_EnableIRQ(SCIFRXI6_IRQn);
            break;
        case 7:
            InterruptHandlerRegister(SCIFTXI7_IRQn, (void (*)(uint32_t))uart7_tx_irq);
            InterruptHandlerRegister(SCIFRXI7_IRQn, (void (*)(uint32_t))uart7_rx_irq);
            GIC_SetPriority(SCIFTXI7_IRQn, 5);
            GIC_SetPriority(SCIFRXI7_IRQn, 5);
            GIC_EnableIRQ(SCIFTXI7_IRQn);
            GIC_EnableIRQ(SCIFRXI7_IRQn);
            break;
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    if (RxIrq == irq)
        uart_data[obj->index].rx_irq_set_api = enable;
    serial_irq_set_internal(obj, irq, enable);
}

static void serial_flow_irq_set(serial_t *obj, uint32_t enable) {
    uart_data[obj->index].rx_irq_set_flow = enable;
    serial_irq_set_internal(obj, RxIrq, enable);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    if (obj->uart->SCFSR & 0x93) { obj->uart->SCFSR = ~0x93;}
    while (!serial_readable(obj));
    int data = obj->uart->SCFRDR & 0xff;
                /* Clear DR,RDF */
    obj->uart->SCFSR &= 0xfffc; 
    return data;
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
    obj->uart->SCFTDR = c;
    obj->uart->SCFSR &= 0xff9f;  // Clear TEND/TDFE
    uart_data[obj->index].count++;
}

int serial_readable(serial_t *obj) {
    return obj->uart->SCFSR & 0x02;  // RDF
}

int serial_writable(serial_t *obj) {
    return obj->uart->SCFSR & 0x20;  // TDFE
}

void serial_clear(serial_t *obj) {
    obj->uart->SCFCR = 0x06;
    obj->uart->SCFCR = 0x06;
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
    //obj->uart->LCR |= (1 << 6);
}

void serial_break_clear(serial_t *obj) {
    //obj->uart->LCR &= ~(1 << 6);
}

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow) {
    serial_flow_irq_set(obj, 0);
    // Only UART1 has hardware flow control on LPC176x
    /*LPC_UART1_TypeDef *uart1 = (uint32_t)obj->uart == (uint32_t)LPC_UART1 ? LPC_UART1 : NULL;
    int index = obj->index;

    // First, disable flow control completely
    if (uart1)
        uart1->MCR = uart1->MCR & ~UART_MCR_FLOWCTRL_MASK;
    uart_data[index].sw_rts.pin = uart_data[index].sw_cts.pin = NC;
    serial_flow_irq_set(obj, 0);
    if (FlowControlNone == type)
        return;
    // Check type(s) of flow control to use
    UARTName uart_rts = (UARTName)pinmap_find_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart_cts = (UARTName)pinmap_find_peripheral(txflow, PinMap_UART_CTS);
    if (((FlowControlCTS == type) || (FlowControlRTSCTS == type)) && (NC != txflow)) {
        // Can this be enabled in hardware?
        if ((UART_1 == uart_cts) && (NULL != uart1)) {
            // Enable auto-CTS mode
            uart1->MCR |= UART_MCR_CTSEN_MASK;
            pinmap_pinout(txflow, PinMap_UART_CTS);
        } else {
            // Can't enable in hardware, use software emulation
            gpio_init_in(&uart_data[index].sw_cts, txflow);
        }
    }
    if (((FlowControlRTS == type) || (FlowControlRTSCTS == type)) && (NC != rxflow)) {
        // Enable FIFOs, trigger level of 1 char on RX FIFO
        obj->uart->FCR = 1 << 0  // FIFO Enable - 0 = Disables, 1 = Enabled
                       | 1 << 1  // Rx Fifo Reset
                       | 1 << 2  // Tx Fifo Reset
                       | 0 << 6; // Rx irq trigger level - 0 = 1 char, 1 = 4 chars, 2 = 8 chars, 3 = 14 chars
         // Can this be enabled in hardware?
        if ((UART_1 == uart_rts) && (NULL != uart1)) {
            // Enable auto-RTS mode
            uart1->MCR |= UART_MCR_RTSEN_MASK;
            pinmap_pinout(rxflow, PinMap_UART_RTS);
        } else { // can't enable in hardware, use software emulation
            gpio_init_out_ex(&uart_data[index].sw_rts, rxflow, 0);
            // Enable RX interrupt
            serial_flow_irq_set(obj, 1);
        }
    }*/
}

