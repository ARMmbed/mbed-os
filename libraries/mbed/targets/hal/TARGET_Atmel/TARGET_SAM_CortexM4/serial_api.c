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
#include <string.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "serial_api.h"
#include "sysclk.h"
#include "serial_platform.h"
#include "ioport.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#if DEVICE_SERIAL_ASYNCH
#define pUSART_S(obj)			obj->serial.uart
#define pSERIAL_S(obj)			((struct serial_s*)&(obj->serial))
#else
#define pUSART_S(obj)			obj->uart
#define pSERIAL_S(obj)			((struct serial_s*)obj)
#endif
#define _USART(obj)			((Usart*)pUSART_S(obj))
#define USART_NUM 8

static uint8_t serial_get_index(serial_t *obj);
static IRQn_Type get_serial_irq_num (serial_t *obj);
static uint32_t get_serial_vector (serial_t *obj);

static uint32_t serial_irq_ids[USART_NUM] = {0};
static uart_irq_handler irq_handler;
static void uart0_irq(void);
static void uart1_irq(void);
static void uart2_irq(void);
static void uart3_irq(void);
static void uart4_irq(void);
static void uart5_irq(void);
static void uart6_irq(void);
static void uart7_irq(void);


int stdio_uart_inited = 0;
serial_t stdio_uart;

extern uint8_t g_sys_init;

static int get_usart_clock_id(UARTName peripheral)
{
    int cid;
    switch (peripheral) {
        case UART_0:
            cid = ID_FLEXCOM0;
            break;
        case UART_1:
            cid = ID_FLEXCOM1;
            break;
        case UART_2:
            cid = ID_FLEXCOM2;
            break;
        case UART_3:
            cid = ID_FLEXCOM3;
            break;
        case UART_4:
            cid = ID_FLEXCOM4;
            break;
        case UART_5:
            cid = ID_FLEXCOM5;
            break;
        case UART_6:
            cid = ID_FLEXCOM6;
            break;
        case UART_7:
            cid = ID_FLEXCOM7;
            break;
        default :
            cid = NC;
            break;
    }
    return cid;
}

static int get_flexcom_id(UARTName peripheral)
{
    int fid;
    switch (peripheral) {
        case UART_0:
            fid = (int)FLEXCOM0;
            break;
        case UART_1:
            fid = (int)FLEXCOM1;
            break;
        case UART_2:
            fid = (int)FLEXCOM2;
            break;
        case UART_3:
            fid = (int)FLEXCOM3;
            break;
        case UART_4:
            fid = (int)FLEXCOM4;
            break;
        case UART_5:
            fid = (int)FLEXCOM5;
            break;
        case UART_6:
            fid = (int)FLEXCOM6;
            break;
        case UART_7:
            fid = (int)FLEXCOM7;
            break;
        default :
            fid = NC;
            break;
    }
    return fid;
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    int clockid = NC, flexcom = NC;

    /*To determine the uart peripheral associated with pins*/
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    MBED_ASSERT(uart != (UARTName)NC);

    if (g_sys_init == 0) {
        sysclk_init();
        g_sys_init = 1;
    }
    pUSART_S(obj) = uart;
    pSERIAL_S(obj)->uart_serial_options.baudrate = (9600UL);
    pSERIAL_S(obj)->uart_serial_options.charlength = US_MR_CHRL_8_BIT;
    pSERIAL_S(obj)->uart_serial_options.paritytype = US_MR_PAR_NO;
    pSERIAL_S(obj)->uart_serial_options.stopbits = US_MR_NBSTOP_1_BIT;

    /* Configure UART pins */
    if(tx != NC) {
        pin_function(tx, pinmap_find_function(tx, PinMap_UART_TX));
        ioport_disable_pin(tx);
    }
    if(rx != NC) {
        pin_function(rx, pinmap_find_function(rx, PinMap_UART_RX));
        ioport_disable_pin(rx);
    }
    clockid = get_usart_clock_id(uart);
    if (clockid != NC) {
        sysclk_enable_peripheral_clock(clockid);
    }

    flexcom = (int)get_flexcom_id(uart);
#if (!SAM4L)
#if (SAMG55)
    /* Configure flexcom for usart */
    flexcom_enable((Flexcom* )flexcom);
    flexcom_set_opmode((Flexcom* )flexcom, FLEXCOM_USART);
#else
    sysclk_enable_peripheral_clock(clockid);
#endif
    /* Configure USART */
    usart_init_rs232((Usart*)uart, &(pSERIAL_S(obj)->uart_serial_options),
                     sysclk_get_peripheral_hz());
#endif
#if (SAM4L)
    sysclk_enable_peripheral_clock(clockid);
    /* Configure USART */
    usart_init_rs232((Usart*)uart,  &(pSERIAL_S(obj)->uart_serial_options, sysclk_get_peripheral_bus_hz((Usart*)uart));
#endif
                     /* Disable rx and tx in case 1 line only required to be configured for usart */
                     usart_disable_tx((Usart*)uart);
                     usart_disable_rx((Usart*)uart);
                     /* Enable the receiver and transmitter. */
    if(tx != NC) {
    usart_enable_tx((Usart*)uart);
    }
    if(rx != NC) {
    usart_enable_rx((Usart*)uart);
    }

    if(uart == STDIO_UART) {
    stdio_uart_inited = 1;
    memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    usart_reset(_USART(obj));
}

void serial_baud(serial_t *obj, int baudrate)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT((baudrate == 110) || (baudrate == 150) || (baudrate == 300) || (baudrate == 1200) ||
                (baudrate == 2400) || (baudrate == 4800) || (baudrate == 9600) || (baudrate == 19200) || (baudrate == 38400) ||
                (baudrate == 57600) || (baudrate == 115200) || (baudrate == 230400) || (baudrate == 460800) || (baudrate == 921600) );
    uint32_t clockid = 0;
    clockid = get_usart_clock_id(pUSART_S(obj));
    if (clockid != NC) {
        sysclk_disable_peripheral_clock(clockid);
    }
    pSERIAL_S(obj)->uart_serial_options.baudrate = baudrate;
    usart_serial_init(_USART(obj), &(pSERIAL_S(obj)->uart_serial_options));
    sysclk_enable_peripheral_clock(clockid);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits == 5) || (data_bits == 6) || (data_bits == 7) || (data_bits == 8));

    uint32_t clockid = 0;
    clockid = get_usart_clock_id(pUSART_S(obj));
    if (clockid != (uint32_t)NC) {
        sysclk_disable_peripheral_clock(clockid);
    }

    switch(stop_bits) { /*selecting the stop bits*/
        case 1:
            pSERIAL_S(obj)->uart_serial_options.stopbits = US_MR_NBSTOP_1_BIT;
            break;
        case 2:
            pSERIAL_S(obj)->uart_serial_options.stopbits = US_MR_NBSTOP_2_BIT;
            break;
    }

    switch(parity) { /*selecting the parity bits*/
        case ParityNone:
            pSERIAL_S(obj)->uart_serial_options.paritytype = US_MR_PAR_NO;
            break;
        case ParityOdd:
            pSERIAL_S(obj)->uart_serial_options.paritytype = US_MR_PAR_ODD;
            break;
        case ParityEven:
            pSERIAL_S(obj)->uart_serial_options.paritytype = US_MR_PAR_EVEN;
            break;
    }

    switch(data_bits) { /*selecting the data bits*/
        case 5:
            pSERIAL_S(obj)->uart_serial_options.charlength = US_MR_CHRL_5_BIT;
            break;
        case 6:
            pSERIAL_S(obj)->uart_serial_options.charlength = US_MR_CHRL_6_BIT;
            break;
        case 7:
            pSERIAL_S(obj)->uart_serial_options.charlength = US_MR_CHRL_7_BIT;
            break;
        case 8:
            pSERIAL_S(obj)->uart_serial_options.charlength = US_MR_CHRL_8_BIT;
            break;
    }

    usart_serial_init(_USART(obj), &(pSERIAL_S(obj)->uart_serial_options));
    sysclk_enable_peripheral_clock(clockid);
}

#ifdef DEVICE_SERIAL_FC

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    if(FlowControlNone == type) {
        /* Disable Hardware Handshaking. */
        _USART(obj)->US_MR = (_USART(obj)->US_MR & ~US_MR_USART_MODE_Msk) | US_MR_USART_MODE_NORMAL;
        return;
    }

    /*To determine the uart peripheral associated with pins*/
    UARTName uart_cts = (UARTName)pinmap_peripheral(txflow, PinMap_UART_CTS);
    UARTName uart_rts = (UARTName)pinmap_peripheral(rxflow, PinMap_UART_RTS);
    UARTName uart = (UARTName)pinmap_merge(uart_cts, uart_rts);
    MBED_ASSERT(uart != (UARTName)NC);

    if((FlowControlCTS == type) || (FlowControlRTSCTS== type)) {
        /* Configure CTS pin. */
        pin_function(txflow, pinmap_find_function(txflow, PinMap_UART_CTS));
        ioport_disable_pin(txflow);
    }

    if((FlowControlRTS == type) || (FlowControlRTSCTS== type))  {
        /* Configure CTS pin. */
        pin_function(rxflow, pinmap_find_function(rxflow, PinMap_UART_RTS));
        ioport_disable_pin(rxflow);
    }

    /* Set hardware handshaking mode. */
    _USART(obj)->US_MR = (_USART(obj)->US_MR & ~US_MR_USART_MODE_Msk) | US_MR_USART_MODE_HW_HANDSHAKING;
}

#endif  //DEVICE_SERIAL_FC

void serial_break_set(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    _USART(obj)->US_CR = US_CR_STTBRK;

}

void serial_break_clear(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    _USART(obj)->US_CR = US_CR_STPBRK;

}

void serial_pinout_tx(PinName tx)
{
    pin_function(tx, pinmap_find_function(tx, PinMap_UART_TX));
    ioport_disable_pin(tx);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    irq_handler = handler;
    serial_irq_ids[serial_get_index(obj)] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    vector = get_serial_vector(obj);
    irq_n = get_serial_irq_num(obj);

    if (enable) {
        switch (irq) {
            case RxIrq:
                usart_enable_interrupt(_USART(obj), US_IER_RXRDY);
                break;
            case TxIrq:
                usart_enable_interrupt(_USART(obj), US_IER_TXEMPTY);
                break;
        }

        NVIC_ClearPendingIRQ(irq_n);
        NVIC_DisableIRQ(irq_n);
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);
    } else {
        switch (irq) {
            case RxIrq:
                usart_disable_interrupt(_USART(obj), US_IER_RXRDY);
                break;
            case TxIrq:
                usart_disable_interrupt(_USART(obj), US_IDR_TXEMPTY);
                break;
        }
        NVIC_DisableIRQ(irq_n);
    }
}

static inline void uart_irq(Usart *const usart, uint32_t index)
{
    MBED_ASSERT(usart != (void*)0);
    uint32_t mask, status;
    int a= 1;
    /* Read and clear mask. */
    status = usart_get_status(usart);
    mask = usart_get_interrupt_mask(usart);
    status &= mask;

    if (serial_irq_ids[index] != 0) {
        if (status & US_IER_RXRDY) { /*For Receive Complete*/
            if (irq_handler) {
                irq_handler(serial_irq_ids[index], RxIrq);
            }
        }
        if (status & US_IER_TXEMPTY) { /*For Transmit Complete*/
            if (irq_handler) {
                irq_handler(serial_irq_ids[index], TxIrq);
            }
        }
    }
}

void uart0_irq(void)
{
    uart_irq(USART0, 0);
}

void uart1_irq(void)
{
    uart_irq(USART1, 1);
}

void uart2_irq(void)
{
    uart_irq(USART2, 2);
}

void uart3_irq(void)
{
    uart_irq(USART3, 3);
}

void uart4_irq(void)
{
    uart_irq(USART4, 4);
}

void uart5_irq(void)
{
    uart_irq(USART5, 5);
}

void uart6_irq(void)
{
    uart_irq(USART6, 6);
}

void uart7_irq(void)
{
    uart_irq(USART7, 7);
}

static uint8_t serial_get_index(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    switch ((int)pUSART_S(obj)) {
        case UART_0:
            return 0;
        case UART_1:
            return 1;
        case UART_2:
            return 2;
        case UART_3:
            return 3;
        case UART_4:
            return 4;
        case UART_5:
            return 5;
        case UART_6:
            return 6;
        case UART_7:
            return 7;
    }
    return 0;
}

static uint32_t get_serial_vector (serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    uint32_t vector = 0;
    switch ((int)pUSART_S(obj)) {
        case UART_0:
            vector = (uint32_t)uart0_irq;
            break;
        case UART_1:
            vector = (uint32_t)uart1_irq;
            break;
        case UART_2:
            vector = (uint32_t)uart2_irq;
            break;
        case UART_3:
            vector = (uint32_t)uart3_irq;
            break;
        case UART_4:
            vector = (uint32_t)uart4_irq;
            break;
        case UART_5:
            vector = (uint32_t)uart5_irq;
            break;
        case UART_6:
            vector = (uint32_t)uart6_irq;
            break;
        case UART_7:
            vector = (uint32_t)uart7_irq;
            break;
    }
    return vector;
}

IRQn_Type get_serial_irq_num (serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    switch ((int)pUSART_S(obj)) {
        case UART_0:
            return FLEXCOM0_IRQn;
        case UART_1:
            return FLEXCOM1_IRQn;
        case UART_2:
            return FLEXCOM2_IRQn;
        case UART_3:
            return FLEXCOM3_IRQn;
        case UART_4:
            return FLEXCOM4_IRQn;
        case UART_5:
            return FLEXCOM5_IRQn;
        case UART_6:
            return FLEXCOM6_IRQn;
        case UART_7:
            return FLEXCOM7_IRQn;
        default:
            MBED_ASSERT(0);
    }
    return 0; /*Warning Suppression*/
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    while (!serial_readable(obj));
    return (int)((_USART(obj)->US_RHR & US_RHR_RXCHR_Msk) & 0xFF);
}

void serial_putc(serial_t *obj, int c)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    while (!serial_writable(obj));
    _USART(obj)->US_THR = US_THR_TXCHR(c);
}

int serial_readable(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    uint32_t status = 1;
    if (!(_USART(obj)->US_CSR & US_CSR_RXRDY)) {
        status = 0;
    } else {
        status = 1;
    }
    return status;
}

int serial_writable(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    uint32_t status = 1;
    if (!(_USART(obj)->US_CSR & US_CSR_TXRDY)) {
        status = 0;
    } else {
        status = 1;
    }
    return status;
}

/************************************************************************************
 * 			ASYNCHRONOUS HAL														*
 ************************************************************************************/


#if DEVICE_SERIAL_ASYNCH
/************************************
 * HELPER FUNCTIONS					*
 ***********************************/
void serial_tx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    if(enable) {
        pSERIAL_S(obj)->events |= event;
    } else {
        pSERIAL_S(obj)->events &= ~ event;
    }
}

void serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    if(enable) {
        pSERIAL_S(obj)->events |= event;
    } else {
        pSERIAL_S(obj)->events &= ~ event;
    }
}

void serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(tx != (void*)0);
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);

    if(serial_tx_active(obj)) return;

    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;

    return;
}

void serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);

    if(serial_rx_active(obj)) return;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;

    return;
}

void serial_set_char_match(serial_t *obj, uint8_t char_match)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    if (char_match != SERIAL_RESERVED_CHAR_MATCH) {
        obj->char_match = char_match;
    }
}

/************************************
 * TRANSFER FUNCTIONS				*
 ***********************************/
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(tx != (void*)0);
    if(tx_length == 0) return 0;
    IRQn_Type irq_n = (IRQn_Type)0;
    irq_n = get_serial_irq_num(obj);

    serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);
    serial_tx_enable_event(obj, event, true);

    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    usart_enable_interrupt(_USART(obj), US_IER_TXBUFE);

    return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
    IRQn_Type irq_n = (IRQn_Type)0;
    irq_n = get_serial_irq_num(obj);

    serial_rx_enable_event(obj, SERIAL_EVENT_RX_ALL, false);
    serial_rx_enable_event(obj, event, true);
    serial_set_char_match(obj, char_match);
    serial_rx_buffer_set(obj, rx, rx_length, rx_width);

    NVIC_ClearPendingIRQ(irq_n);
    NVIC_DisableIRQ(irq_n);
    NVIC_SetVector(irq_n, (uint32_t)handler);
    NVIC_EnableIRQ(irq_n);

    usart_enable_interrupt(_USART(obj), US_IER_RXBUFF);

    return;
}

uint8_t serial_tx_active(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    return ((obj->tx_buff.length > 0) ? true : false);
}

uint8_t serial_rx_active(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    return ((obj->rx_buff.length > 0) ? true : false);
}

int serial_tx_irq_handler_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    serial_tx_abort_asynch(obj);
    return SERIAL_EVENT_TX_COMPLETE & obj->serial.events;
}
int serial_rx_irq_handler_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    int event = 0;
    /* This interrupt handler is called from USART irq */
    uint8_t *buf = (uint8_t*)obj->rx_buff.buffer;
    uint8_t error_code = 0;
    uint8_t received_data = 0;

    uint32_t ul_status;

    /* Read USART Status. */
    ul_status = usart_get_status(_USART(obj));

    if (ul_status & US_CSR_OVRE) { /* Overrun Error */
        serial_rx_abort_asynch(obj);
        return SERIAL_EVENT_RX_OVERFLOW;
    }
    if (ul_status & US_CSR_FRAME) { /* Framing Error */
        serial_rx_abort_asynch(obj);
        return SERIAL_EVENT_RX_FRAMING_ERROR;
    }
    if (ul_status & US_CSR_PARE) { /* Parity Error */
        serial_rx_abort_asynch(obj);
        return SERIAL_EVENT_RX_PARITY_ERROR;
    }

    /* Read current packet from DATA register,
    * increment buffer pointer and decrement buffer length */
    received_data = ((_USART(obj)->US_RHR & US_RHR_RXCHR_Msk) & 0xFF);

    /* Read value will be at least 8-bits long */
    buf[obj->rx_buff.pos] = received_data;
    /* Increment 8-bit pointer */
    obj->rx_buff.pos++;

    /* Check if the last character have been received */
    if(--(obj->rx_buff.length) == 0) {
        event |= SERIAL_EVENT_RX_COMPLETE;
        if((buf[obj->rx_buff.pos - 1] == obj->char_match) && (obj->serial.events & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
            event |= SERIAL_EVENT_RX_CHARACTER_MATCH;
        }
        serial_rx_abort_asynch(obj);
        return event & obj->serial.events;
    }

    /* Check for character match event */
    if((buf[obj->rx_buff.pos - 1] == obj->char_match) && (obj->serial.events & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
        event |= SERIAL_EVENT_RX_CHARACTER_MATCH;
    }

    /* Return to the call back if character match occured */
    if(event != 0) {
        serial_rx_abort_asynch(obj);
        return event & obj->serial.events;
    }
    return 0;
}


int serial_irq_handler_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

void serial_tx_abort_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    usart_disable_interrupt(_USART(obj), US_IER_TXBUFE);
    obj->tx_buff.length = 0;
    obj->rx_buff.pos = 0;
}

void serial_rx_abort_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    usart_disable_interrupt(_USART(obj), US_IER_RXBUFF);
    obj->tx_buff.length = 0;
    obj->rx_buff.pos = 0;
}

#endif