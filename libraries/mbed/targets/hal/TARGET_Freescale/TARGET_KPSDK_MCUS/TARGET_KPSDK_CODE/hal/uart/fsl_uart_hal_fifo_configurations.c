/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_uart_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if FSL_FEATURE_UART_HAS_FIFO
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_tx_fifo
 * Description   : Enable the UART transmit FIFO.
 * This function allows the user to enable the UART transmit FIFO.
 * It is required that the transmitter/receiver should be disabled before calling this function
 * and when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_enable_tx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* before enabling the tx fifo, UARTx_C2[TE] (transmitter) and
     * UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    uint8_t txEnable = HW_UART_C2(uartInstance).B.TE;
    uint8_t rxEnable = HW_UART_C2(uartInstance).B.RE;

    if (txEnable || rxEnable)
    {
        return kStatus_UART_TxOrRxNotDisabled;
    }
    else
    {
        /* Set TXFE*/
        HW_UART_PFIFO_SET(uartInstance, BM_UART_PFIFO_TXFE);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_tx_fifo
 * Description   : Disable the UART transmit FIFO.
 * This function allows the user to disable the UART transmit FIFO.
 * It is required that the transmitter/receiver should be disabled before calling this function
 * and when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_disable_tx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* before enabling the tx fifo, UARTx_C2[TE] (transmitter) and
     * UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    uint8_t txEnable = HW_UART_C2(uartInstance).B.TE;
    uint8_t rxEnable = HW_UART_C2(uartInstance).B.RE;

    if (txEnable || rxEnable)
    {
        return kStatus_UART_TxOrRxNotDisabled;
    }
    else
    {
        /* Clear TXFE*/
        HW_UART_PFIFO_CLR(uartInstance, BM_UART_PFIFO_TXFE);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_rx_fifo
 * Description   : Enable the UART receive FIFO.
 * This function allows the user to enable the UART receive FIFO.
 * It is required that the transmitter/receiver should be disabled before calling this function
 * and when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_enable_rx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* before enabling the rx fifo, UARTx_C2[TE] (transmitter) and
     * UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    uint8_t txEnable = HW_UART_C2(uartInstance).B.TE;
    uint8_t rxEnable = HW_UART_C2(uartInstance).B.RE;

    if (txEnable || rxEnable)
    {
        return kStatus_UART_TxOrRxNotDisabled;
    }
    else
    {
        /* Set RXFE*/
        HW_UART_PFIFO_SET(uartInstance, BM_UART_PFIFO_RXFE);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_rx_fifo
 * Description   : Disable the UART receive FIFO.
 * This function allows the user to disable the UART receive FIFO.
 * It is required that the transmitter/receiver should be disabled before calling this function
 * and when the FIFO is empty.
 * Additionally, TXFLUSH and RXFLUSH commands should be issued after calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_disable_rx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* before disabling the rx fifo, UARTx_C2[TE] (transmitter) and
     * UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    uint8_t txEnable = HW_UART_C2(uartInstance).B.TE;
    uint8_t rxEnable = HW_UART_C2(uartInstance).B.RE;

    if (txEnable || rxEnable)
    {
        return kStatus_UART_TxOrRxNotDisabled;
    }
    else
    {
        /* Clear RXFE*/
        HW_UART_PFIFO_CLR(uartInstance, BM_UART_PFIFO_RXFE);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_tx_fifo_size
 * Description   : Get the size of the UART transmit FIFO.
 * This function returns the size (number of entries) supported in the UART transmit FIFO for
 * a particular module instance.
 *
 *END**************************************************************************/
uint8_t uart_hal_get_tx_fifo_size(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_PFIFO(uartInstance).B.TXFIFOSIZE;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_rx_fifo_size
 * Description   : Get the size of the UART receive FIFO.
 * This function returns the size (number of entries) supported in the UART receive FIFO for
 * a particular module instance.
 *
 *END**************************************************************************/
uint8_t uart_hal_get_rx_fifo_size(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_PFIFO(uartInstance).B.RXFIFOSIZE;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_flush_tx_fifo
 * Description   : Flush the UART transmit FIFO.
 * This function allows you to flush the UART transmit FIFO for a particular module instance.
 * Flushing the FIFO may result in data loss.
 * It is recommended that the transmitter should be disabled before calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_flush_tx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* in order to flush the tx fifo, UARTx_C2[TE] (transmitter) must be disabled
     * if not, return an error code
     */
    if (HW_UART_C2(uartInstance).B.TE != 0)
    {
        return kStatus_UART_TxNotDisabled;
    }
    else
    {
        /* Set the bit to flush fifo*/
        HW_UART_CFIFO_SET(uartInstance, BM_UART_CFIFO_TXFLUSH);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_flush_rx_fifo
 * Description   : Flush the UART receive FIFO.
 * This function allows you to flush the UART receive FIFO for a particular module instance.
 * Flushing the FIFO may result in data loss.
 * It is recommended that the receiver should be disabled before calling this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_flush_rx_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* in order to flush the rx fifo, UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    if (HW_UART_C2(uartInstance).B.RE != 0)
    {
        return kStatus_UART_RxNotDisabled;
    }
    else
    {
        /* Set the bit to flush fifo*/
        HW_UART_CFIFO_SET(uartInstance, BM_UART_CFIFO_RXFLUSH);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_tx_fifo_overflow_interrupt
 * Description   : Enable the UART transmit FIFO overflow interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_tx_fifo_overflow_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Set to enable*/
    HW_UART_CFIFO_SET(uartInstance, BM_UART_CFIFO_TXOFE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_tx_fifo_overflow_interrupt
 * Description   : Disable the UART transmit FIFO overflow interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_tx_fifo_overflow_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Clear to disable*/
    HW_UART_CFIFO_CLR(uartInstance, BM_UART_CFIFO_TXOFE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_tx_fifo_overflow_interrupt_enabled
 * Description   : Get the configuration of the UART transmit FIFO overflow interrupt enable.
 *
 *END**************************************************************************/
bool uart_hal_is_tx_fifo_overflow_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_CFIFO(uartInstance).B.TXOFE;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_rx_fifo_underflow_interrupt
 * Description   : Enable the UART receive FIFO underflow interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_rx_fifo_underflow_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Set to enable*/
    HW_UART_CFIFO_SET(uartInstance, BM_UART_CFIFO_RXUFE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_rx_fifo_underflow_interrupt
 * Description   : Disable the UART receive FIFO underflow interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_rx_fifo_underflow_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Clear to disable*/
    HW_UART_CFIFO_CLR(uartInstance, BM_UART_CFIFO_RXUFE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rx_fifo_underflow_interrupt_enabled
 * Description   : Get the configuration of the UART receive FIFO underflow interrupt enable.
 *
 *END**************************************************************************/
bool uart_hal_is_rx_fifo_underflow_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_CFIFO(uartInstance).B.RXUFE;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_tx_fifo_empty
 * Description   : Get the UART transmit FIFO empty status state.
 * The function returns the state of the transmit FIFO empty status state but does not take into
 * account data in the shift register.
 *
 *END**************************************************************************/
bool uart_hal_is_tx_fifo_empty(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_SFIFO(uartInstance).B.TXEMPT;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rx_fifo_empty
 * Description   : Get the UART transmit FIFO empty status state.
 * The function returns the state of the receive FIFO empty status state but does not take into
 * account data in the shift register.
 *
 *END**************************************************************************/
bool uart_hal_is_rx_fifo_empty(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_SFIFO(uartInstance).B.RXEMPT;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_tx_fifo_overflow
 * Description   : Get the UART transmit FIFO overflow status state.
 *
 *END**************************************************************************/
bool uart_hal_is_tx_fifo_overflow(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_SFIFO(uartInstance).B.TXOF;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_clear_tx_fifo_overflow
 * Description   : Clear the UART transmit FIFO overflow status.
 *
 *END**************************************************************************/
void  uart_hal_clear_tx_fifo_overflow(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* write one to clear status flag */
    HW_UART_SFIFO_SET(uartInstance, BM_UART_SFIFO_TXOF);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rx_fifo_underflow
 * Description   : Get the UART receive FIFO underflow status state.
 *
 *END**************************************************************************/
bool uart_hal_is_rx_fifo_underflow(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    return HW_UART_SFIFO(uartInstance).B.RXUF;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_clear_rx_fifo_underflow
 * Description   : Clear the UART receive FIFO underflow status.
 *
 *END**************************************************************************/
void  uart_hal_clear_rx_fifo_underflow(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* write one to clear status flag */
    HW_UART_SFIFO_SET(uartInstance, BM_UART_SFIFO_RXUF);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_tx_fifo_watermark
 * Description   : Set the UART transmit FIFO watermark value.
 * Programming the transmit watermark should be done when UART the transmitter is disabled
 * and the value must be set less than the size obtained from uart_hal_get_tx_fifo_size.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_tx_fifo_watermark(uint32_t uartInstance, uint8_t watermark)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* in order to set the tx watermark, UARTx_C2[TE] (transmitter) must be disabled
     * if not, return an error code
     */
    if (HW_UART_C2(uartInstance).B.TE != 0)
    {
        return kStatus_UART_TxNotDisabled;
    }
    else
    {
        /* set watermark
         * Programming the transmit watermark should be done when the transmitter is disabled
         * and the value must be set less than the size given in PFIFO[TXFIFOSIZE]
         */
        HW_UART_TWFIFO_WR(uartInstance, watermark);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_tx_fifo_watermark
 * Description   : Get the UART transmit FIFO watermark value.
 *
 *END**************************************************************************/
uint8_t uart_hal_get_tx_fifo_watermark(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get watermark*/
    return HW_UART_TWFIFO_RD(uartInstance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_tx_dataword_count_in_fifo
 * Description   : Get the UART transmit FIFO data word count (number of words in the
 *                 transmit FIFO).
 * The function uart_hal_get_tx_dataword_count_in_fifo excludes any data that may
 * be in the UART transmit shift register
 *
 *END**************************************************************************/
uint8_t uart_hal_get_tx_dataword_count_in_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get the current nmber of datawords in the FIFO*/
    return HW_UART_TCFIFO_RD(uartInstance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_rx_fifo_watermark
 * Description   : Set the UART receive FIFO watermark value.
 * Programming the receive watermark should be done when the receiver is disabled
 * and the value must be set less than the size obtained from uart_hal_get_rx_fifo_size and
 * greater than zero.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_rx_fifo_watermark(uint32_t uartInstance, uint8_t watermark)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* in order to set the rx watermark, UARTx_C2[RE] (receiver) must be disabled
     * if not, return an error code
     */
    if (HW_UART_C2(uartInstance).B.RE != 0)
    {
        return kStatus_UART_RxNotDisabled;
    }
    else
    {
        /* set watermark
         * Programming the receive watermark should be done when the receiver is disabled
         * and the value must be set less than the size given in PFIFO[RXFIFOSIZE]
         * and greater than zero.
         */
        HW_UART_RWFIFO_WR(uartInstance, watermark);
        return kStatus_UART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_rx_fifo_watermark
 * Description   : Get the UART receive FIFO data word count (number of words in the receive FIFO).
 * The function uart_hal_get_rx_dataword_count_in_fifo excludes any data that may be
 * in the receive shift register.
 *
 *END**************************************************************************/
uint8_t uart_hal_get_rx_fifo_watermark(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get watermark*/
    return HW_UART_RWFIFO_RD(uartInstance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_rx_dataword_count_in_fifo
 * Description   : Get the UART receive FIFO watermark value.
 *
 *END**************************************************************************/
uint8_t uart_hal_get_rx_dataword_count_in_fifo(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get the current nmber of datawords in the FIFO*/
    return HW_UART_RCFIFO_RD(uartInstance);
}
#endif  /* FSL_FEATURE_UART_HAS_FIFO*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

