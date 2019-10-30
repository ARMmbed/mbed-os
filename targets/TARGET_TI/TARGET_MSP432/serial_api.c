/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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

/* Low-level implementation of uart functionality for MSP432.
 * This implementation does not (yet) support SERIAL_ASYNCH and SERIAL_FC.
 */

#if DEVICE_SERIAL

#include "serial_api.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include <string.h>

/* With SERIAL_ASYNCH, our type serial_s is embedded
 * into a bigger structure (see serial_api.h). So we
 * need a macro to extract the serial_s object.
 */
#if DEVICE_SERIAL_ASYNCH
#define SERIAL_S(obj) (&((obj)->serial))
#else
#define SERIAL_S(obj) (obj)
#endif

int stdio_uart_inited = 0; // used in mbed_retarget.cpp
serial_t stdio_uart;

uart_irq_handler uart_handler = 0;
uint32_t serial_irq_ids[4] = {0};

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    struct serial_s *objs = SERIAL_S(obj);
    uint8_t stdio_config = 0;

    /* Determine the UART to use (UART_A0...UART_A3) */
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);
    UARTName uart    = (UARTName)pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT(objs->uart != (UARTName)NC);
    /* Fill the obj structure */
    objs->uart   = uart;
    objs->pin_tx = tx;
    objs->pin_rx = rx;

    /* Check if we selected the STDIO UART */
    if ((tx == STDIO_UART_TX) || (rx == STDIO_UART_RX)) {
        stdio_config = 1;
    }
    /* Configure the TX and RX pins */
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* put EUSCI module in reset state */
    EUSCI->CTLW0 = EUSCI_A_CTLW0_SWRST;
    /* Configure SMCLK as clock source */
    EUSCI->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;
    /* Disable modulation stages */
    EUSCI->MCTLW &= ~(EUSCI_A_MCTLW_BRS_MASK |
                      EUSCI_A_MCTLW_BRF_MASK |
                      EUSCI_A_MCTLW_OS16);
    /* Disable interrupts */
    EUSCI->IE = 0;
    /* enable the UART module again */
    EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
    /* Set default baud rate */
    serial_baud(obj, 9600);
    /* Copy config to stdio structure if needed */
    if (stdio_config) {
        memcpy(&stdio_uart, obj, sizeof(serial_t));
        stdio_uart_inited = 1;
    }
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* Wait for pending operations */
    while (EUSCI->STATW & EUSCI_A_STATW_BUSY);
    /* Put UART to reset state */
    EUSCI->CTLW0 = EUSCI_A_CTLW0_SWRST;
    /* De-configure the RX/TX lines */
    pin_function(objs->pin_tx, MSP432_PIN_DATA(SEL0, PIN_INPUT, PullNone, 0));
    pin_function(objs->pin_rx, MSP432_PIN_DATA(SEL0, PIN_INPUT, PullNone, 0));
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    EUSCI->BRW = (uint16_t)(SubsystemMasterClock / baudrate);
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
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* put module in reset state */
    EUSCI->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    /* Configure data bits */
    switch (data_bits) {
        case 7:
            EUSCI->CTLW0 |= EUSCI_A_CTLW0_SEVENBIT;
            break;
        case 8:
            EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT;
            break;
        default:
            error("Unsupported UART data-bit size");
    }
    /* Configure parity */
    switch (parity) {
        case ParityNone:
            EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_PEN;
            break;
        case ParityOdd:
            EUSCI->CTLW0 |=  EUSCI_A_CTLW0_PEN;
            EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_PAR;
            break;
        case ParityEven:
            EUSCI->CTLW0 |= EUSCI_A_CTLW0_PEN;
            EUSCI->CTLW0 |= EUSCI_A_CTLW0_PAR;
            break;
        case ParityForced1:
        case ParityForced0:
        default:
            error("Unsupported UART parity selection");
    }
    /* Configure stop bits */
    switch (data_bits) {
        case 1:
            EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_SPB;
            break;
        case 2:
            EUSCI->CTLW0 |= EUSCI_A_CTLW0_SPB;
            break;
        default:
            error("Unsupported UART stop-bit size");
    }
    /* re-enable the UART module */
    EUSCI->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    struct serial_s *objs = SERIAL_S(obj);
    uart_handler = handler;
    /* UART base addresses are
     * EUSCI_A0 0x40001000
     * EUSCI_A1 0x40001400
     * EUSCI_A2 0x40001800
     * EUSCI_A3 0x40001c00 */
    uint8_t index = (((uint32_t)(objs->uart)) >> 10) & 0x3;
    serial_irq_ids[index] = id;
    /* Enable the NVIC irq for this UART */
    NVIC_EnableIRQ((IRQn_Type)(EUSCIA0_IRQn + index));
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    switch (irq) {
        case RxIrq:
            BITBAND_PERI(EUSCI->IE, EUSCI_A_IE_RXIE_OFS) = enable;
            break;
        case TxIrq:
            BITBAND_PERI(EUSCI->IE, EUSCI_A_IE_TXIE_OFS) = enable;
            break;
    }
}

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int serial_getc(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* Wait until the RX Buffer is filled.... */
    while ((EUSCI->IFG & EUSCI_A_IFG_RXIFG) == 0);
    /*Transfer single char from RX buffer */
    return EUSCI->RXBUF;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* Wait until the TX Buffer is empty.... */
    while ((EUSCI->IFG & EUSCI_A_IFG_TXIFG) == 0);
    /* Transfer single char to TX buffer */
    EUSCI->TXBUF = (uint16_t)c;
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int serial_readable(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    return EUSCI->IFG & EUSCI_A_IFG_RXIFG;
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int serial_writable(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    return EUSCI->IFG & EUSCI_A_IFG_TXIFG;
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* Clear all flags */
    EUSCI->IFG = 0;
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
    struct serial_s *objs = SERIAL_S(obj);
    /* Get the UART base */
    EUSCI_A_Type *EUSCI = (EUSCI_A_Type *)objs->uart;
    /* Wait until the TX Buffer is empty.... */
    while ((EUSCI->IFG & EUSCI_A_IFG_TXIFG) == 0);
    /* Send break (automatically cleared) */
    EUSCI->CTLW0 |= EUSCI_A_CTLW0_TXBRK;
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
    /* Not needed because break flag is automatically cleared */
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
    pinmap_pinout(tx, PinMap_UART_TX);
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param rxflow The TX pin name
 * @param txflow The RX pin name
 */
//void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
//{
//    /* not used so far */
//}

/** Get the pins that support Serial TX
 *
 * Return a PinMap array of pins that support Serial TX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_tx_pinmap(void)
{
    return PinMap_UART_TX;
}

/** Get the pins that support Serial RX
 *
 * Return a PinMap array of pins that support Serial RX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_rx_pinmap(void)
{
    return PinMap_UART_RX;
}

/**************************/
/* UART interrupt handler */
/**************************/
void handle_UART_Interrupt(uint8_t index, uint8_t vector)
{
    if (uart_handler) {
        switch (vector) {
            case 2:
                uart_handler(serial_irq_ids[index], RxIrq);
                break;
            case 4:
                uart_handler(serial_irq_ids[index], TxIrq);
                break;
        }
    }
}

void EUSCIA0_UART_IRQHandler(void)
{
    handle_UART_Interrupt(0, EUSCI_A0->IV);
}
void EUSCIA1_UART_IRQHandler(void)
{
    handle_UART_Interrupt(1, EUSCI_A1->IV);
}
void EUSCIA2_UART_IRQHandler(void)
{
    handle_UART_Interrupt(2, EUSCI_A2->IV);
}
void EUSCIA3_UART_IRQHandler(void)
{
    handle_UART_Interrupt(3, EUSCI_A3->IV);
}

#endif /* DEVICE_SERIAL */

