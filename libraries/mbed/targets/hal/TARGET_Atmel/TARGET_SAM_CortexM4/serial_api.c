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
//#include "PeripheralPins.h"
//#include "pinmap_function.h"

static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

extern uint8_t g_sys_init;


void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    if (g_sys_init == 0) {
        sysclk_init();
        g_sys_init = 1;
    }
    const usart_serial_options_t uart_serial_options = {
        .baudrate = (9600UL),
        .charlength = US_MR_CHRL_8_BIT,
        .paritytype = US_MR_PAR_NO,
        .stopbits = US_MR_NBSTOP_1_BIT,
    };

    /* Configure UART pins */
//    ioport_set_port_peripheral_mode(IOPORT_PIOA, (PIO_PA27B_RXD7| PIO_PA28B_TXD7),
//                                    (IOPORT_MODE_MUX_B));
    ioport_set_port_mode(IOPORT_PIOA, (PIO_PA27B_RXD7| PIO_PA28B_TXD7), (IOPORT_MODE_MUX_B));
    ioport_disable_port(IOPORT_PIOA, (PIO_PA27B_RXD7| PIO_PA28B_TXD7));

    sysclk_enable_peripheral_clock(ID_FLEXCOM7); // stdio uart being 7 . Have to implement pin to peripheral mapping
    usart_serial_init(USART7, &uart_serial_options);

}

void serial_free(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

void serial_baud(serial_t *obj, int baudrate)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT((baudrate == 110) || (baudrate == 150) || (baudrate == 300) || (baudrate == 1200) ||
                (baudrate == 2400) || (baudrate == 4800) || (baudrate == 9600) || (baudrate == 19200) || (baudrate == 38400) ||
                (baudrate == 57600) || (baudrate == 115200) || (baudrate == 230400) || (baudrate == 460800) || (baudrate == 921600) );
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits == 5) || (data_bits == 6) || (data_bits == 7) || (data_bits == 8) /*|| (data_bits == 9)*/);

}

#ifdef DEVICE_SERIAL_FC

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

}

#endif  //DEVICE_SERIAL_FC

void serial_break_set(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

}

void serial_break_clear(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

}

void serial_pinout_tx(PinName tx)
{

}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}


void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

}

void serial_putc(serial_t *obj, int c)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

int serial_readable(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

int serial_writable(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

/************************************************************************************
 * 			ASYNCHRONOUS HAL														*
 ************************************************************************************/

#if DEVICE_SERIAL_ASYNCH

/************************************
 * HELPER FUNCTIONS					*
 ***********************************/

/************************************
 * TRANSFER FUNCTIONS				*
 ***********************************/
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(tx != (void*)0);
    if(tx_length == 0) return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
    MBED_ASSERT(rx != (void*)0);
}

uint8_t serial_tx_active(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

uint8_t serial_rx_active(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
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
}

void serial_rx_abort_asynch(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);
}

#endif