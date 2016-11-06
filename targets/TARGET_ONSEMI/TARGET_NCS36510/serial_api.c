/**
******************************************************************************
* @file Serial.c
* @brief Implementation of a 16C550 UART driver
* @internal
* @author ON Semiconductor
* $Rev: 0.1 $
* $Date: 2015-11-04 05:30:00 +0530 (Wed, 04 Nov 2015) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a �ON Semiconductor�).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (�ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software�) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup uart_16c550
*
*/
#if DEVICE_SERIAL

#include "serial_api.h"

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#include "mbed_assert.h"
#include <string.h>
#include "uart_16c550.h"
#include "cmsis_nvic.h"

static IRQn_Type Irq;

uint32_t stdio_uart_inited = 0;
serial_t stdio_uart;

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;
static inline void uart_irq(uint8_t status, uint32_t index);


/** Opens UART device.
 * @details
 * Sets the necessary registers. Set to default Baud rate 115200, 8 bit, parity None and stop bit 1.
 *  The UART interrupt is enabled.
 *
 * @note The UART transmit interrupt is not enabled, because sending is controlled
 * by the task.
 *
 * @param UartNum A UART device instance.
 * @param options The options parameter containing the baud rate.
 * @return True if opening was successful.
 */

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    uint16_t clockDivisor;

    CrossbReg_t *CbRegOffSet;
    PadReg_t *PadRegOffset;

    //find which peripheral is associated with the rx and tx pins
    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    //check if the peripherals for each pin are the same or not
    //returns the enum associated with the peripheral
    //in the case of this target, the enum is the base address of the peripheral
    obj->UARTREG = (Uart16C550Reg_pt) pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT(obj->UARTREG != (Uart16C550Reg_pt) NC);

    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    /*TODO: Mac Lobdell - we should recommend using the instance method and not using base addresses as index */

    if (obj->UARTREG == (Uart16C550Reg_pt)STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
    /*TODO: determine if pullups are needed/recommended */
    /*    if (tx != NC) {
                pin_mode(tx, PullUp);
      }
        if (rx != NC) {
                pin_mode(rx, PullUp);
        }
    */
    /* Configure IOs to UART using cross bar, pad and GPIO settings */

    if(obj->UARTREG == UART2REG) {
        /* UART 2 */
        CLOCK_ENABLE(CLOCK_UART2);
        Irq = Uart2_IRQn;
    } else if(obj->UARTREG == UART1REG) {
        /* UART 1  */
        CLOCK_ENABLE(CLOCK_UART1);

        Irq = Uart1_IRQn;
    } else {
        MBED_ASSERT(False);
    }

    CLOCK_ENABLE(CLOCK_GPIO);
    CLOCK_ENABLE(CLOCK_CROSSB);
    CLOCK_ENABLE(CLOCK_PAD);

    /*TODO: determine if tx and rx are used correctly in this case - this depends on the pin enum matching the position in the crossbar*/

    /* Configure tx pin as UART */
    CbRegOffSet = (CrossbReg_t*)(CROSSBREG_BASE + (tx * CROSS_REG_ADRS_BYTE_SIZE));
    CbRegOffSet->DIOCTRL0 = CONFIGURE_AS_UART; /* tx pin as UART */

    /* Configure rx pin as UART */
    CbRegOffSet = (CrossbReg_t*)(CROSSBREG_BASE + (rx * CROSS_REG_ADRS_BYTE_SIZE));
    CbRegOffSet->DIOCTRL0 = CONFIGURE_AS_UART; /* rx pin as UART */

    /** - Set pad parameters, output drive strength, pull piece control, output drive type */
    PadRegOffset = (PadReg_t*)(PADREG_BASE + (tx * PAD_REG_ADRS_BYTE_SIZE));
    PadRegOffset->PADIO0.WORD = PAD_UART_TX; /* Pad setting for UART Tx */

    PadRegOffset = (PadReg_t*)(PADREG_BASE + (rx * PAD_REG_ADRS_BYTE_SIZE));
    PadRegOffset->PADIO0.WORD = PAD_UART_RX;  /* Pad settings for UART Rx */

    GPIOREG->W_OUT  = (0x1 << tx); /* tx as OUT direction */
    GPIOREG->W_IN   = (0x1 << rx); /* rx as IN directon */

    CLOCK_DISABLE(CLOCK_PAD);
    CLOCK_DISABLE(CLOCK_CROSSB);
    CLOCK_DISABLE(CLOCK_GPIO);

    /* Set the divisor value.  To do so, LCR[7] needs to be set to 1 in order to access the divisor registers.
     * The right-shift of 4 is a division of 16, representing the oversampling rate. */
    clockDivisor = (fClockGetPeriphClockfrequency() / UART_DEFAULT_BAUD) >> 4;
    obj->UARTREG->LCR.WORD = 0x80;
    obj->UARTREG->DLL = clockDivisor & 0xFF;
    obj->UARTREG->DLM = clockDivisor >> 8;

    /* Set the character width to 8 data bits, no parity, 1 stop bit.  Write the entire line control register,
     *   effectively disabling the divisor latch. */
    obj->UARTREG->LCR.WORD = 0x03;

    /* Enable the FIFOs, reset the Tx and Rx FIFOs, set the Rx FIFO trigger level to 8 bytes, and set DMA Mode
       to 1. */
    obj->UARTREG->FCR.WORD = (FCR_RXFIFOTRIGGERLEVEL_8 | FCR_DMA_MODE_1 |
                              FCR_TXFIFO_RESET | FCR_RXFIFO_RESET | FCR_FIFO_ENABLE);

    /* Make a copy of the current MSR to the SCR register.  This is used from task space to determine the
     *   flow control state. */
    obj->UARTREG->SCR = obj->UARTREG->MSR.WORD;

    if((int)obj->UARTREG == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    NVIC_ClearPendingIRQ(Irq);

    return;
}

/** Closes a UART device.
 * @details
 * Disables the UART interrupt.
 *
 * @param device The UART device to close.
 */
void serial_free(serial_t *obj)
{
    NVIC_DisableIRQ(obj->IRQType);
}

void serial_baud(serial_t *obj, int baudrate)
{
    /* Set the divisor value.  To do so, LCR[7] needs to be set to 1 in order to access the divisor registers.
     * The right-shift of 4 is a division of 16, representing the oversampling rate. */
    uint16_t clockDivisor = (fClockGetPeriphClockfrequency() / baudrate) >> 4;

    obj->UARTREG->LCR.BITS.DLAB = True;
    obj->UARTREG->DLL = clockDivisor & 0xFF;
    obj->UARTREG->DLM = clockDivisor >> 8;
    obj->UARTREG->LCR.BITS.DLAB = False;
}

/*
Parity XX0 – Parity disabled; 001 – Odd Parity; 011 – Even Parity; 101 – Stick Parity, checked as 1; 111 – Stick Parity, checked as 0.
StopBit 0 – 1 stop bit; 1 – 2 stop bits.
DataLen 00 – 5 bits; 01 – 6 bits; 10 – 7 bits; 11 – 8 bits
*/
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    if(data_bits >= 5 && data_bits <= 8 && parity <= 7 && stop_bits >= 1 && stop_bits <= 2) {
        if(parity == (SerialParity)0) {
            parity  = (SerialParity)0;
        } else {
            parity = (SerialParity)(parity + parity - 1) ;
        }

        obj->UARTREG->LCR.WORD |= ((((data_bits - 5) << UART_LCR_DATALEN_BIT_POS) |
                                    (parity << UART_LCR_PARITY_BIT_POS) |
                                    ((stop_bits - 1) << UART_LCR_STPBIT_BIT_POS)) & 0x3F);
    } else {
        MBED_ASSERT(False);
    }
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

/******************************************************
************* Internal IRQ functions ******************
*******************************************************/
void Uart1_Irq()
{
    uint8_t active_irq = (uint8_t)(UART1REG->LSR.WORD) & 0xFF;
    uint8_t irq_mask = 0;

    if(UART1REG->IER.WORD & UART_IER_TX_EMPTY_MASK) { /*check if TX interrupt is enabled*/
        irq_mask |= active_irq & UART_LSR_TX_EMPTY_MASK;
    }

    if(UART1REG->IER.WORD & UART_IER_RX_DATA_READY_MASK) { /*check if RX interrupt is enabled*/
        irq_mask |= active_irq & UART_LSR_RX_DATA_READY_MASK;
    }

    //uart_irq((uint8_t)(UART1REG->LSR.WORD & 0xFF), 0);
    uart_irq(active_irq & irq_mask, 0);
}

void Uart2_Irq()
{
    uint8_t active_irq = (uint8_t)(UART2REG->LSR.WORD) & 0xFF;
    uint8_t irq_mask = 0;

    if(UART2REG->IER.WORD & UART_IER_TX_EMPTY_MASK) { /*check if TX interrupt is enabled*/
        irq_mask |= active_irq & UART_LSR_TX_EMPTY_MASK;
    }

    if(UART2REG->IER.WORD & UART_IER_RX_DATA_READY_MASK) { /*check if RX interrupt is enabled*/
        irq_mask |= active_irq & UART_LSR_RX_DATA_READY_MASK;
    }

    //uart_irq((uint8_t)(UART2REG->LSR.WORD & 0xFF), 1);
    uart_irq(active_irq & irq_mask, 1);

}

static inline void uart_irq(uint8_t status, uint32_t index)
{
    if (serial_irq_ids[index] != 0) {
        if (status & UART_LSR_TX_EMPTY_MASK) {
            irq_handler(serial_irq_ids[index], TxIrq);
        }
        if (status & UART_LSR_RX_DATA_READY_MASK) {
            irq_handler(serial_irq_ids[index], RxIrq);
        }
    }
}
/******************************************************/

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t Vector = 0;

    /* Check UART number & assign irq handler */
    if(obj->UARTREG == UART1REG) {
        /* UART 2 */
        Vector = (uint32_t)&Uart1_Irq;
        irq_n = Uart1_IRQn;
    } else if(obj->UARTREG == UART2REG) {
        /* UART 1 */
        Vector = (uint32_t)&Uart2_Irq;
        irq_n = Uart2_IRQn;
    } else {
        MBED_ASSERT(False);
    }

    /* Check IRQ type & enable/disable accordingly */
    if(enable) {
        /* Enable */
        if(irq == RxIrq) {
            /* Rx IRQ */
            obj->UARTREG->FCR.BITS.RX_FIFO_TRIG = 0x0;
            obj->UARTREG->IER.BITS.RX_DATA_INT = True;
        } else if(irq == TxIrq) {
            /* Tx IRQ */
            obj->UARTREG->IER.BITS.TX_HOLD_INT = True;
        } else {
            MBED_ASSERT(False);
        }
        NVIC_SetVector(irq_n, Vector);
        NVIC_EnableIRQ(irq_n);
    } else {
        /* Disable */
        NVIC_DisableIRQ(irq_n);
        if(irq == RxIrq) {
            /* Rx IRQ */
            obj->UARTREG->IER.BITS.RX_DATA_INT = False;
        } else if(irq == TxIrq) {
            /* Tx IRQ */

            obj->UARTREG->IER.BITS.TX_HOLD_INT = False;
        } else {
            MBED_ASSERT(False);
        }
    }
}

int serial_getc(serial_t *obj)
{
    uint8_t c;

    while(!obj->UARTREG->LSR.BITS.READY);     /* Wait for received data is ready */
    c = obj->UARTREG->RBR & 0xFF;             /* Get received character */
    return c;
}

void serial_putc(serial_t *obj, int c)
{

    while(!obj->UARTREG->LSR.BITS.TX_HOLD_EMPTY);/* Wait till THR is empty */
    obj->UARTREG->THR = c; /* Transmit byte */

}

int serial_readable(serial_t *obj)
{
    return obj->UARTREG->LSR.BITS.READY;
}

int serial_writable(serial_t *obj)
{
    return obj->UARTREG->LSR.BITS.TX_HOLD_EMPTY;
}

void serial_clear(serial_t *obj)
{
    /* Reset TX & RX FIFO */
    obj->UARTREG->FCR.WORD |= ((True << UART_FCS_TX_FIFO_RST_BIT_POS) |
                               (True << UART_FCS_RX_FIFO_RST_BIT_POS));
}

void serial_break_set(serial_t *obj)
{
    obj->UARTREG->LCR.BITS.BREAK = True;
}

void serial_break_clear(serial_t *obj)
{
    obj->UARTREG->LCR.BITS.BREAK = False;
}

void serial_pinout_tx(PinName tx)
{
    /* COnfigure PinNo to drive strength of 1, Push pull and pull none */
    fPadIOCtrl(tx, 1, 0, 1);
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
    /* TODO: This is an empty implementation for now.*/
}

#endif /* DEVICE_SERIAL  */
