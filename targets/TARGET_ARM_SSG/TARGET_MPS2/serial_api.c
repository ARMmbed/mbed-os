/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#include "cmsis.h"
#include "PinNames.h"
#include "SMM_MPS2.h"
#include "serial_api.h"
#include "gpio_api.h"

#include <stdio.h>

int stdio_uart_inited = 0;
serial_t stdio_uart;

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    (void)obj;
    (void)tx;
    (void)rx;

    /* Do almost nothing, because we are always UART0 */
    stdio_uart_inited = 1;
    /* Not initializing stdio_uart, because don't have to. All UARTs are stdio
     * UARTs. */

    /* Disable RX and TX interrupts, disable overrun interrupts, enable TX,
     * disable RX */
    CMSDK_UART0->CTRL = CMSDK_UART_CTRL_TXEN_Msk;
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj)
{
    (void)obj;
    /* Do nothing, because we don't have to put anything back */
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate)
{
    (void)obj;
    (void)baudrate;

    /* We don't care about baud rates (fast model), so we don't have to
     * configure them.
     *
     * However, the UART resets to a BAUDDIV of 0, which is invalid. Set it to
     * the minimum valid number. */
    CMSDK_UART0->BAUDDIV = 16;
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param obj       The serial object
 * @param data_bits The number of data bits
 * @param parity    The parity
 * @param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    (void)obj;
    (void)data_bits;
    (void)parity;
    (void)stop_bits;
    /* Do nothing, because we don't care about serial format (fast model) */
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    (void)obj;
    (void)handler;
    (void)id;
    /* Do nothing, because this should never be called */

    //printf("Don't call me.\n");
    //for (;;);
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    (void)obj;
    (void)irq;
    (void)enable;
    /* I don't know what word-approach is. */

    /* Do nothing, because we won't use interrupts. */
}

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int  serial_getc(serial_t *obj)
{
    printf("Anybody calling this?\n");
    for(;;);

    /* Wait for the RX buffer to be non-empty */
    while (!serial_readable(obj));

    return CMSDK_UART0->DATA & 0xFF;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c)
{
    /* XXX Is this the right place to do this? Is there no higher level like
     * libc that should do this? */
    if (c == '\n')
    {
        serial_putc(obj, '\r');
    }

    /* Wait for the TX buffer to be no longer full */
    while (!serial_writable(obj));

    /* Write the data (upper 24-bits are reserved) */
    CMSDK_UART0->DATA = c & 0xFF;
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int  serial_readable(serial_t *obj)
{
    (void)obj;

    /* Datasheet says we have RX buffer full bit in STATE, but not RX buffer
     * empty. Such crap. All we have to go on instead is the RX interrupt
     * status, which I assume we get when there is a byte available... */

    /* XXX As currently implemented, this just says if the RX buffer is full.
     * It might never become full if only a few bytes are received (not ever
     * enough to fill a buffer). */
    return !(CMSDK_UART0->STATE & CMSDK_UART_STATE_RXBF_Msk);
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int  serial_writable(serial_t *obj)
{
    (void)obj;

    /* Serial is writable if the TX buffer is not full. */
    return !(CMSDK_UART0->STATE & CMSDK_UART_STATE_TXBF_Msk);
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj)
{
    (void)obj;

    /* Not implemented */
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
    (void)obj;

    /* Not implemented */
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
    (void)obj;

    /* Not implemented */
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
    (void)tx;

    /* We don't care about this for now */
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param rxflow The TX pin name
 * @param txflow The RX pin name
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    (void)obj;
    (void)type;
    (void)rxflow;
    (void)txflow;

    /* Not implemented */
}
