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

#if DEVICE_SERIAL

// math.h required for floating point operations for baud rate calculation
#include <math.h>

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

/******************************************************************************
 * INITIALIZATION
 ******************************************************************************/
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
static const PinMap PinMap_UART_TX[] = {
    {P0_0,  UART_3, 2},
    {P0_2,  UART_0, 1},
    {P0_10, UART_2, 1},
    {P0_15, UART_1, 1},
    {P0_25, UART_3, 3},
    {P2_0 , UART_1, 2},
    {P2_8 , UART_2, 2},
    {P4_28, UART_3, 3},
    {NC   , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {P0_1 , UART_3, 2},
    {P0_3 , UART_0, 1},
    {P0_11, UART_2, 1},
    {P0_16, UART_1, 1},
    {P0_26, UART_3, 3},
    {P2_1 , UART_1, 2},
    {P2_9 , UART_2, 2},
    {P4_29, UART_3, 3},
    {NC   , NC    , 0}
};

#define UART_NUM    4

#elif defined(TARGET_LPC11U24)
static const PinMap PinMap_UART_TX[] = {
    {P0_19, UART_0, 1},
    {P1_13, UART_0, 3},
    {P1_27, UART_0, 2},
    { NC  , NC    , 0}
};

static const PinMap PinMap_UART_RX[] = {
    {P0_18, UART_0, 1},
    {P1_14, UART_0, 3},
    {P1_26, UART_0, 2},
    {NC   , NC    , 0}
};

#define UART_NUM    1

#elif defined(TARGET_LPC812)

static const SWM_Map SWM_UART_TX[] = {
    {0, 0},
    {1, 8},
    {2, 16},
};

static const SWM_Map SWM_UART_RX[] = {
    {0, 8},
    {1, 16},
    {2, 24},
};

// bit flags for used UARTs
static unsigned char uart_used = 0;
static int get_available_uart(void) {
    for (int i=0; i<3; i++) {
        if ((uart_used & (1 << i)) == 0)
            return i;
    }
    return -1;
}

#define UART_EN       (0x01<<0)

#define CTS_DELTA     (0x01<<5)
#define RXBRK         (0x01<<10)
#define DELTA_RXBRK   (0x01<<11)

#define RXRDY         (0x01<<0)
#define TXRDY         (0x01<<2)

static uint32_t UARTSysClk;
#endif

#ifndef TARGET_LPC812
static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;
#endif

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    int is_stdio_uart = 0;
    
#ifdef TARGET_LPC812
    int uart_n = get_available_uart();
    if (uart_n == -1) {
        error("No available UART");
    }
    obj->uart_n = uart_n;
    obj->uart = (LPC_USART_TypeDef *)(LPC_USART0_BASE + (0x4000 * uart_n));
    
    const SWM_Map *swm;
    uint32_t regVal;
    
    swm = &SWM_UART_TX[uart_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (tx   << swm->offset);
    
    swm = &SWM_UART_RX[uart_n];
    regVal = LPC_SWM->PINASSIGN[swm->n] & ~(0xFF << swm->offset);
    LPC_SWM->PINASSIGN[swm->n] = regVal |  (rx   << swm->offset);
    
    /* uart clock divided by 1 */
    LPC_SYSCON->UARTCLKDIV = 1;
    
    /* disable uart interrupts */
    NVIC_DisableIRQ((IRQn_Type)(UART0_IRQn + uart_n));
    
    /* Enable UART clock */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << (14 + uart_n));
    
    /* Peripheral reset control to UART, a "1" bring it out of reset. */
    LPC_SYSCON->PRESETCTRL &= ~(0x1 << (3 + uart_n));
    LPC_SYSCON->PRESETCTRL |=  (0x1 << (3 + uart_n));
    
    UARTSysClk = SystemCoreClock / LPC_SYSCON->UARTCLKDIV;
    
    // set default baud rate and format
    serial_baud  (obj, 9600);
    serial_format(obj, 8, ParityNone, 1);
    
    /* Clear all status bits. */
    obj->uart->STAT = CTS_DELTA | DELTA_RXBRK;
    
    /* enable uart interrupts */
    NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + uart_n));
    
    /* Enable UART interrupt */
    // obj->uart->INTENSET = RXRDY | TXRDY | DELTA_RXBRK;
    
    /* Enable UART */
    obj->uart->CFG |= UART_EN;
    
    is_stdio_uart = ((tx == USBTX) && (rx == USBRX));
#else
    // determine the UART to use
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
    if ((int)uart == NC) {
        error("Serial pinout mapping failed");
    }

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
    obj->uart = (LPC_UART_TypeDef *)uart;
    // enable power
    switch (uart) {
        case UART_0: LPC_SC->PCONP |= 1 <<  3; break;
        case UART_1: LPC_SC->PCONP |= 1 <<  4; break;
        case UART_2: LPC_SC->PCONP |= 1 << 24; break;
        case UART_3: LPC_SC->PCONP |= 1 << 25; break;
    }

#elif defined(TARGET_LPC11U24)
    obj->uart = (LPC_USART_Type *)uart;
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);

    // [TODO] Consider more elegant approach
    // disconnect USBTX/RX mapping mux, for case when switching ports
    pin_function(USBTX, 0);
    pin_function(USBRX, 0);
#endif

    // enable fifos and default rx trigger level
    obj->uart->FCR = 1 << 0  // FIFO Enable - 0 = Disables, 1 = Enabled
                   | 0 << 1  // Rx Fifo Reset
                   | 0 << 2  // Tx Fifo Reset
                   | 0 << 6; // Rx irq trigger level - 0 = 1 char, 1 = 4 chars, 2 = 8 chars, 3 = 14 chars

    // disable irqs
    obj->uart->IER = 0 << 0  // Rx Data available irq enable
                   | 0 << 1  // Tx Fifo empty irq enable
                   | 0 << 2; // Rx Line Status irq enable

    // set default baud rate and format
    serial_baud  (obj, 9600);
    serial_format(obj, 8, ParityNone, 1);

    // pinout the chosen uart
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    // set rx/tx pins in PullUp mode
    pin_mode(tx, PullUp);
    pin_mode(rx, PullUp);

    switch (uart) {
        case UART_0: obj->index = 0; break;
#if (UART_NUM > 1)
        case UART_1: obj->index = 1; break;
#endif
#if (UART_NUM > 2)
        case UART_2: obj->index = 2; break;
#endif
#if (UART_NUM > 3)
        case UART_3: obj->index = 3; break;
#endif
    }

    is_stdio_uart = (uart == STDIO_UART) ? (1) : (0);
#endif
    
    if (is_stdio_uart) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj) {
#ifdef TARGET_LPC812
    uart_used &= ~(1 << obj->uart_n);
#else
    serial_irq_ids[obj->index] = 0;
#endif
}

// serial_baud
// set the baud rate, taking in to account the current SystemFrequency
void serial_baud(serial_t *obj, int baudrate) {
#ifdef TARGET_LPC812
    /* Integer divider:
         BRG = UARTSysClk/(Baudrate * 16) - 1
       
       Frational divider:
         FRG = ((UARTSysClk / (Baudrate * 16 * (BRG + 1))) - 1)
       
       where
         FRG = (LPC_SYSCON->UARTFRDADD + 1) / (LPC_SYSCON->UARTFRDSUB + 1)
       
       (1) The easiest way is set SUB value to 256, -1 encoded, thus SUB
           register is 0xFF.
       (2) In ADD register value, depending on the value of UartSysClk,
           baudrate, BRG register value, and SUB register value, be careful
           about the order of multiplier and divider and make sure any
           multiplier doesn't exceed 32-bit boundary and any divider doesn't get
           down below one(integer 0).
       (3) ADD should be always less than SUB.
    */
    obj->uart->BRG = UARTSysClk / 16 / baudrate - 1;
    
    LPC_SYSCON->UARTFRGDIV = 0xFF;
    LPC_SYSCON->UARTFRGMULT = ( ((UARTSysClk / 16) * (LPC_SYSCON->UARTFRGDIV + 1)) /
                                (baudrate * (obj->uart->BRG + 1))
                              ) - (LPC_SYSCON->UARTFRGDIV + 1);

#else
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
// The LPC2300 and LPC1700 have a divider and a fractional divider to control the
// baud rate. The formula is:
//
// Baudrate = (1 / PCLK) * 16 * DL * (1 + DivAddVal / MulVal)
//   where:
//     1 < MulVal <= 15
//     0 <= DivAddVal < 14
//     DivAddVal < MulVal
//
    // set pclk to /1
    switch ((int)obj->uart) {
        case UART_0: LPC_SC->PCLKSEL0 &= ~(0x3 <<  6); LPC_SC->PCLKSEL0 |= (0x1 <<  6); break;
        case UART_1: LPC_SC->PCLKSEL0 &= ~(0x3 <<  8); LPC_SC->PCLKSEL0 |= (0x1 <<  8); break;
        case UART_2: LPC_SC->PCLKSEL1 &= ~(0x3 << 16); LPC_SC->PCLKSEL1 |= (0x1 << 16); break;
        case UART_3: LPC_SC->PCLKSEL1 &= ~(0x3 << 18); LPC_SC->PCLKSEL1 |= (0x1 << 18); break;
        default: error("serial_baud"); break;
    }

    uint32_t PCLK = SystemCoreClock;

#elif defined(TARGET_LPC11U24)
    LPC_SYSCON->UARTCLKDIV = 0x1;
    uint32_t PCLK = SystemCoreClock;
#endif

    // First we check to see if the basic divide with no DivAddVal/MulVal
    // ratio gives us an integer result. If it does, we set DivAddVal = 0,
    // MulVal = 1. Otherwise, we search the valid ratio value range to find
    // the closest match. This could be more elegant, using search methods
    // and/or lookup tables, but the brute force method is not that much
    // slower, and is more maintainable.
    uint16_t DL = PCLK / (16 * baudrate);

    uint8_t DivAddVal = 0;
    uint8_t MulVal = 1;
    int hit = 0;
    uint16_t dlv;
    uint8_t mv, dav;
    if ((PCLK % (16 * baudrate)) != 0) {     // Checking for zero remainder
        float err_best = (float) baudrate;
        uint16_t dlmax = DL;
        for ( dlv = (dlmax/2); (dlv <= dlmax) && !hit; dlv++) {
            for ( mv = 1; mv <= 15; mv++) {
                for ( dav = 1; dav < mv; dav++) {
                    float ratio = 1.0 + ((float) dav / (float) mv);
                    float calcbaud = (float)PCLK / (16.0 * (float) dlv * ratio);
                    float err = fabs(((float) baudrate - calcbaud) / (float) baudrate);
                    if (err < err_best) {
                        DL = dlv;
                        DivAddVal = dav;
                        MulVal = mv;
                        err_best = err;
                        if (err < 0.001) {
                            hit = 1;
                        }
                    }
                }
            }
        }
    }

    // set LCR[DLAB] to enable writing to divider registers
    obj->uart->LCR |= (1 << 7);

    // set divider values
    obj->uart->DLM = (DL >> 8) & 0xFF;
    obj->uart->DLL = (DL >> 0) & 0xFF;
    obj->uart->FDR = (uint32_t) DivAddVal << 0
                   | (uint32_t) MulVal    << 4;

    // clear LCR[DLAB]
    obj->uart->LCR &= ~(1 << 7);
#endif
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    // 0: 1 stop bits, 1: 2 stop bits
    if (stop_bits != 1 && stop_bits != 2) {
        error("Invalid stop bits specified");
    }
    stop_bits -= 1;
    
#ifdef TARGET_LPC812
    // 0: 7 data bits ... 2: 9 data bits
    if (data_bits < 7 || data_bits > 9) {
        error("Invalid number of bits (%d) in serial format, should be 7..9", data_bits);
    }
    data_bits -= 7;
    
    int paritysel;
    switch (parity) {
        case ParityNone: paritysel = 0; break;
        case ParityEven: paritysel = 2; break;
        case ParityOdd : paritysel = 3; break;
        default:
            error("Invalid serial parity setting");
            return;
    }
    
    obj->uart->CFG = (data_bits << 2)
                   | (paritysel << 4)
                   | (stop_bits << 6);
#else
    // 0: 5 data bits ... 3: 8 data bits
    if (data_bits < 5 || data_bits > 8) {
        error("Invalid number of bits (%d) in serial format, should be 5..8", data_bits);
    }

    data_bits -= 5;

    int parity_enable, parity_select;
    switch (parity) {
        case ParityNone: parity_enable = 0; parity_select = 0; break;
        case ParityOdd : parity_enable = 1; parity_select = 0; break;
        case ParityEven: parity_enable = 1; parity_select = 1; break;
        case ParityForced1: parity_enable = 1; parity_select = 2; break;
        case ParityForced0: parity_enable = 1; parity_select = 3; break;
        default:
            error("Invalid serial parity setting");
            return;
    }

    obj->uart->LCR = data_bits << 0
                   | stop_bits << 2
                   | parity_enable << 3
                   | parity_select        << 4;
#endif
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
#ifdef TARGET_LPC812

#else
static inline void uart_irq(uint32_t iir, uint32_t index) {
    // [Chapter 14] LPC17xx UART0/2/3: UARTn Interrupt Handling
    SerialIrq irq_type;
    switch (((iir >> 1) & 0x7)) {
        case 1: irq_type = TxIrq; break;
        case 2: irq_type = RxIrq; break;
        default: return;
    }

    if (serial_irq_ids[index] != 0)
        irq_handler(serial_irq_ids[index], irq_type);
}

#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
void uart0_irq() {uart_irq(LPC_UART0->IIR, 0);}
void uart1_irq() {uart_irq(LPC_UART1->IIR, 1);}
void uart2_irq() {uart_irq(LPC_UART2->IIR, 2);}
void uart3_irq() {uart_irq(LPC_UART3->IIR, 3);}

#elif defined(TARGET_LPC11U24)
void uart0_irq() {uart_irq(LPC_USART->IIR, 0);}
#endif
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
#ifdef TARGET_LPC812
    
#else
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
#endif
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
#ifdef TARGET_LPC812
    
#else
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    switch ((int)obj->uart) {
#if defined(TARGET_LPC1768) || defined(TARGET_LPC2368)
        case UART_0: irq_n=UART0_IRQn; vector = (uint32_t)&uart0_irq; break;
        case UART_1: irq_n=UART1_IRQn; vector = (uint32_t)&uart1_irq; break;
        case UART_2: irq_n=UART2_IRQn; vector = (uint32_t)&uart2_irq; break;
        case UART_3: irq_n=UART3_IRQn; vector = (uint32_t)&uart3_irq; break;
#elif defined(TARGET_LPC11U24)
        case UART_0: irq_n=UART_IRQn ; vector = (uint32_t)&uart0_irq; break;
#endif
    }

    if (enable) {
        obj->uart->IER |= 1 << irq;
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else { // disable
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        obj->uart->IER &= ~(1 << irq);
        all_disabled = (obj->uart->IER & (1 << other_irq)) == 0;
        if (all_disabled)
            NVIC_DisableIRQ(irq_n);
    }
#endif
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj) {
    while (!serial_readable(obj));
#ifdef TARGET_LPC812
    return obj->uart->RXDATA;
#else
    return obj->uart->RBR;
#endif
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
#ifdef TARGET_LPC812
    obj->uart->TXDATA = c;
#else
    obj->uart->THR = c;
#endif
}

int serial_readable(serial_t *obj) {
#ifdef TARGET_LPC812
    return obj->uart->STAT & RXRDY;
#else
    return obj->uart->LSR & 0x01;
#endif
}

int serial_writable(serial_t *obj) {
#ifdef TARGET_LPC812
    return obj->uart->STAT & TXRDY;
#else
    return obj->uart->LSR & 0x20;
#endif
}

void serial_clear(serial_t *obj) {
#ifdef TARGET_LPC812
    // [TODO]
#else
    obj->uart->FCR = 1 << 1  // rx FIFO reset
                   | 1 << 2  // tx FIFO reset
                   | 0 << 6; // interrupt depth
#endif
}

void serial_pinout_tx(PinName tx) {
#ifdef TARGET_LPC812
    
#else
    pinmap_pinout(tx, PinMap_UART_TX);
#endif
}

#endif
