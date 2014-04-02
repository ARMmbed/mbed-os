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

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_wait_mode_operation
 * Description   : Configure the UART to either operate or sieze to operate in WAIT mode.
 * The function configures the UART to either operate or seize to operate when WAIT mode is
 * entered.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void  uart_hal_configure_wait_mode_operation(uint32_t uartInstance, uart_operation_config_t mode)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure uart operation in wait mode*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* In CPU wait mode: 0 - uart clocks continue to run; 1 - uart clocks freeze */
        BW_UART0_C1_DOZEEN(mode);
    }
    else
#endif
    {
        /*In CPU wait mode: 0 - uart is enabled; 1 - uart is disabled */
        BW_UART_C1_UARTSWAI(uartInstance, mode);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_wait_mode_operation_config
 * Description   : Determine if the UART operates or siezes to operate in WAIT mode.
 * This function returns kUartOperates if the UART has been configured to operate in WAIT mode.
 * Else it returns KUartStops if the UART has been configured to seize-to-operate in WAIT mode.
 *
 *END**************************************************************************/
uart_operation_config_t uart_hal_get_wait_mode_operation_config(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get configuration uart operation in wait mode*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* In CPU wait mode: 0 - uart clocks continue to run; 1 - uart clocks freeze */
        if (HW_UART0_C1.B.DOZEEN==0)
        {
             return kUartOperates;
        }
        else
        {
             return kUartStops;
        }
    }
    else
#endif
    {
        /*In CPU wait mode: 0 - uart is enabled; 1 - uart is disabled */
        if (HW_UART_C1(uartInstance).B.UARTSWAI == 0)
        {
             return kUartOperates;
        }
        else
        {
             return kUartStops;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_loopback_mode
 * Description   : Configure the UART loopback operation.
 * This function enables or disables the UART loopback operation.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void uart_hal_configure_loopback_mode(uint32_t uartInstance, bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure uart to enable/disable operation in loopback mode*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* configure LOOPS bit to enable(1)/disable(0) loopback mode, but also need to clear RSRC*/
        BW_UART0_C1_LOOPS(enable);

        /* clear RSRC for loopback mode, and if loopback disabled,
         * this bit has no meaning but clear anyway
         * to set it back to default value
         */
        HW_UART0_C1_CLR(BM_UART0_C1_RSRC);
    }
    else
#endif
    {
        /* configure LOOPS bit to enable(1)/disable(0) loopback mode, but also need to clear RSRC */
        BW_UART_C1_LOOPS(uartInstance, enable);

        /* clear RSRC for loopback mode, and if loopback disabled,
         * this bit has no meaning but clear anyway
         * to set it back to default value
         */
        HW_UART_C1_CLR(uartInstance, BM_UART_C1_RSRC);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_singlewire_mode
 * Description   : Configure the UART single-wire operation.
 * This function enables or disables the UART single-wire operation.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void uart_hal_configure_singlewire_mode(uint32_t uartInstance, bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure uart to enable/disable operation in single mode*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* to enable single-wire mode, need both LOOPS and RSRC set, to disable, clear both*/
        BW_UART0_C1_LOOPS(enable);
        BW_UART0_C1_RSRC(enable);
    }
    else
#endif
    {
        /* to enable single-wire mode, need both LOOPS and RSRC set, to disable, clear both*/
        BW_UART_C1_LOOPS(uartInstance, enable);
        BW_UART_C1_RSRC(uartInstance, enable);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_txdir_in_singlewire_mode
 * Description   : Configure the UART transmit direction while in single-wire mode.
 * This function configures the transmitter direction when the UART is configured for single-wire
 * operation.
 *
 *END**************************************************************************/
void uart_hal_configure_txdir_in_singlewire_mode(uint32_t uartInstance,
                                                 uart_singlewire_txdir_t direction)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure uart transmit direction (input or output) when in single-wire mode
     * it is assumed uart is in single-wire mode
     */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_C3_TXDIR(direction);
    }
    else
#endif
    {
        BW_UART_C3_TXDIR(uartInstance, direction);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_put_receiver_in_standby_mode
 * Description   : Place the UART receiver in standby mode.
 * This function, when called, will place the UART receiver into standby mode.
 * In some UART instances, there is a condition that must be met before placing rx in standby mode.
 * Before placing UART in standby, you need to first determine if receiver is set to
 * wake on idle and if receiver is already in idle state. Per ref manual:
 * NOTE: RWU should only be set with C1[WAKE] = 0 (wakeup on idle) if the channel is currently
 * not idle.
 * This can be determined by the S2[RAF] flag. If set to wake up FROM an IDLE event and the channel
 * is already idle, it is possible that the UART will discard data since data must be received
 * (or a LIN break detect) after an IDLE is detected before IDLE is allowed to reasserted.
 *
 *END**************************************************************************/
uart_status_t uart_hal_put_receiver_in_standby_mode(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* In some uart instances, there is a condition that must be met before placing
     * rx in standby mode.
     * Before placing uart in standby, need to first determine if receiver is set to
     * wake on idle and if receiver is already in idle state. Per ref manual:
     * NOTE: RWU should only be set with C1[WAKE] = 0 (wakeup on idle) if the channel is
     * currently not idle.
     * This can be determined by the S2[RAF] flag. If set to wake up an IDLE event and
     * the channel is already idle, it is possible that the UART will discard data since data
     * must be received (or a LIN break detect) after an IDLE is detected before IDLE is
     * allowed to reasserted.
     */
    uart_wakeup_method_t rxWakeMethod;
    bool uart_current_rx_state;

    /* see if wake is set for idle or */
    rxWakeMethod = uart_hal_get_receiver_wakeup_method(uartInstance);
    uart_current_rx_state = uart_hal_is_receiver_active(uartInstance);

    /* if both rxWakeMethod is set for idle and current rx state is idle, don't put in standy*/
    if ((rxWakeMethod == kUartIdleLineWake) && (uart_current_rx_state == 0))
    {
        return kStatus_UART_RxStandbyModeError;
    }
    else
    {
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
        if (uartInstance == 0)
        {
            /* set the RWU bit to place receiver into standby mode*/
            HW_UART0_C2_SET(BM_UART0_C2_RWU);
            return kStatus_UART_Success;
        }
        else
#endif
        {
            /* set the RWU bit to place receiver into standby mode*/
            HW_UART_C2_SET(uartInstance, BM_UART_C2_RWU);
            return kStatus_UART_Success;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_put_receiver_in_normal_mode
 * Description   : Place the UART receiver in normal mode (disable standby mode operation).
 * This function, when called, will place the UART receiver into normal mode and out of
 * standby mode.
 *
 *END**************************************************************************/
void uart_hal_put_receiver_in_normal_mode(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* clear the RWU bit to place receiver into normal mode (disable standby mode)*/
        HW_UART0_C2_CLR(BM_UART0_C2_RWU);
    }
    else
#endif
    {
        /* clear the RWU bit to place receiver into normal mode (disable standby mode)*/
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_RWU);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receiver_in_standby
 * Description   : Determine if the UART receiver is currently in standby mode.
 * This function determines the state of the UART receiver. If it returns true, this means
 * that the UART receiver is in standby mode, otherwise if it returns false, the UART receiver
 * is in normal mode.
 *
 *END**************************************************************************/
bool uart_hal_is_receiver_in_standby(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* return the RWU bit setting (0 - normal more, 1 - standby)*/
        return HW_UART0_C2.B.RWU;
    }
    else
#endif
    {
        /* return the RWU bit setting (0 - normal more, 1 - standby)*/
        return HW_UART_C2(uartInstance).B.RWU;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_select_receiver_wakeup_method
 * Description   : Select the UART receiver wakeup method (idle line or addr-mark) from standby
 *                 mode.
 * This function configured the wakeup method of the UART receiver from standby mode.  The options
 * are idle-line wake or address-mark wake.
 *
 *END**************************************************************************/
void uart_hal_select_receiver_wakeup_method(uint32_t uartInstance, uart_wakeup_method_t method)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* configure the WAKE bit for idle line wake or address mark wake */
        BW_UART0_C1_WAKE(method);
    }
    else
#endif
    {
        /* configure the WAKE bit for idle line wake or address mark wake */
        BW_UART_C1_WAKE(uartInstance, method);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_get_receiver_wakeup_method
 * Description   : Get the UART receiver wakeup method (idle line or addr-mark) from standby mode.
 * This function returns how the UART recevier is configured to wake from standby mode. The
 * wake method options that can be returned are kUartIdleLineWake or kUartAddrMarkWake.
 *
 *END**************************************************************************/
uart_wakeup_method_t uart_hal_get_receiver_wakeup_method(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

        /* get configuration of the WAKE bit for idle line wake or address mark wake */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        if(HW_UART0_C1.B.WAKE == 1)
        {
            return kUartAddrMarkWake;
        }
        else
        {
            return kUartIdleLineWake;
        }
    }
    else
#endif
    {
        /* get configuration of the WAKE bit for idle line wake or address mark wake */
        if(HW_UART_C1(uartInstance).B.WAKE == 1)
        {
            return kUartAddrMarkWake;
        }
        else
        {
            return kUartIdleLineWake;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_idle_line_detect
 * Description   : Configure the operation options of the UART idle line detect.
 * This function allows the user to configure the UART idle-line detect operation. There are two
 * separate operations for the user to configure, the idle line bit-count start and the receive
 * wake up affect on IDLE status bit. The user will pass in a stucture of type
 * uart_idle_line_config_t.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void uart_hal_configure_idle_line_detect(uint32_t uartInstance,
                                         const uart_idle_line_config_t *config)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Configure the idle line detection configuration as follows:
     * configure the ILT to bit count after start bit or stop bit
     * configure RWUID to set or not set IDLE status bit upon detection of
     * an idle character when recevier in standby
     */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_C1_ILT(config->idleLineType);
        BW_UART0_S2_RWUID(config->rxWakeIdleDetect);
    }
    else
#endif
    {
        BW_UART_C1_ILT(uartInstance, config->idleLineType);
        BW_UART_S2_RWUID(uartInstance, config->rxWakeIdleDetect);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_break_char_transmit_length
 * Description   : Configure the UART break character transmit length.
 * This function allows the user to configure the UART break character transmit length. Refer to
 * the typedef uart_break_char_length_t for setting options.
 * In some UART instances it is required that the transmitter should be disabled before calling
 * this function. Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void uart_hal_set_break_char_transmit_length(uint32_t uartInstance,
                                             uart_break_char_length_t length)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Configure BRK13 - Break Character transmit length configuration
     * UART break character length setting:
     * 0 - minimum 10-bit times (default),
     * 1 - minimum 13-bit times
     */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_S2_BRK13(length);
    }
    else
#endif
    {
        BW_UART_S2_BRK13(uartInstance, length);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_break_char_detect_length
 * Description   : Configure the UART break character detect length.
 * This function allows the user to configure the UART break character detect length. Refer to
 * the typedef uart_break_char_length_t for setting options.
 *
 *END**************************************************************************/
void uart_hal_set_break_char_detect_length(uint32_t uartInstance, uart_break_char_length_t length)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Configure LBKDE - Break Character detect length configuration
     * UART break character length setting:
     * 0 - minimum 10-bit times (default),
     * 1 - minimum 13-bit times
     */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_S2_LBKDE(length);
    }
    else
#endif
    {
        BW_UART_S2_LBKDE(uartInstance, length);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_queue_break_char_to_send
 * Description   : Configure the UART transmit send break character operation.
 * This function allows the user to queue a UART break character to send.  If true is passed into
 * the function, then a break character will be queued for transmission.  A break character will
 * continuously be queued until this function is called again with a false is passed into this
 * function.
 *
 *END**************************************************************************/
void uart_hal_queue_break_char_to_send(uint32_t uartInstance, bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Configure SBK - Send Break
     * UART send break character setting:
     * 0 - normal transmitter operation,
     * 1 - Queue break character(s) to be sent
     */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_C2_SBK(enable);
    }
    else
#endif
    {
        /* in addition to queuing a break char to send, this function also clears the
         * transmit complete status flag
         * for this uart instance, there is a two step process to
         * clear the transmit complete status flag:
         * 1. Read the status register with the status bit set
         * 2. queue a break character
         * first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* second, queue a break character */
        BW_UART_C2_SBK(uartInstance, enable);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_match_address_operation
 * Description   : Configure the UART match address mode control operation. (Note: Feature
 *                 available on select UART instances)
 * The function allows the user to configure the UART match address control operation. The user
 * has the option to enable the match address mode and to program the match address value. There
 * are two match address modes, each with it's own enable and programmable match address value.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_match_address_operation
(
    uint32_t uartInstance,
    bool matchAddrMode1,
    bool matchAddrMode2,
    uint8_t matchAddrValue1,
    uint8_t matchAddrValue2)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        BW_UART0_C4_MAEN1(matchAddrMode1); /* Match Address Mode Enable 1 */
        BW_UART0_C4_MAEN2(matchAddrMode2); /* Match Address Mode Enable 2 */
        HW_UART0_MA1_WR(matchAddrValue1); /* match address register 1 */
        HW_UART0_MA2_WR(matchAddrValue2); /* match address register 2 */
    }
    else
    {
        /* unsupported feature of this instance number, return error code */
        return kStatus_UART_InvalidInstanceNumber;
    }
#else
    BW_UART_C4_MAEN1(uartInstance, matchAddrMode1); /* Match Address Mode Enable 1 */
    BW_UART_C4_MAEN2(uartInstance, matchAddrMode2); /* Match Address Mode Enable 2 */
    HW_UART_MA1_WR(uartInstance, matchAddrValue1); /* match address register 1 */
    HW_UART_MA2_WR(uartInstance, matchAddrValue2); /* match address register 2 */
#endif

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_send_msb_first_operation
 * Description   : Configre the UART to send data MSB first
 *                 (Note: Feature available on select UART instances)
 * The function allows the user to configure the UART to send data MSB first or LSB first.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_send_msb_first_operation(uint32_t uartInstance, bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* Setting this bit reverses the order of the bits that are
         * transmitted and received on the wire.
         */
        BW_UART0_S2_MSBF(enable);
    }
    else
    {
        /* unsupported feature of this instance number, return error code */
        return kStatus_UART_MSBFirstNotSupported;
    }
#else

    BW_UART_S2_MSBF(uartInstance, enable);

#endif

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_receive_resync_disable_operation
 * Description   : Configuration option to disable the UART resynchronization during received data.
 *                 (Note: Feature available on select UART instances)
 * This function allows the user to disable the UART resync of received data. The default setting
 * of this is false meaning that resynchronization during the received data word is supported.
 * If the user passes in true, this disables resynchronization during the received data word.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_receive_resync_disable_operation(uint32_t uartInstance,
                                                                  bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_RX_RESYNC_SUPPORT
    if (uartInstance == 0)
    {
        /* When set, disables the resynchronization of the received data word when a data
         * one followed by data zero transition is detected. This bit should only be changed
         *  when the receiver is disabled.
         */
        BW_UART0_C5_RESYNCDIS(enable);

        return kStatus_UART_Success;
    }
    else
#endif
    {
        /* unsupported feature of this instance number, return error code */
        return kStatus_UART_ResyncNotSupported;
     }
}

#if FSL_FEATURE_UART_HAS_MODEM_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_receiver_rts
 * Description   : Enable the UART receiver request-to-send functionality.
 * This function allows the user to enable the UART receiver request-to-send (RTS) functionality.
 * By enabling, it allows the RTS output to control the CTS input of the transmitting device to
 * prevent receiver overrun. RTS is deasserted if the number of characters in the receiver data
 * register (FIFO) is equal to or greater than RWFIFO[RXWATER]. RTS is asserted when the
 * number of characters in the receiver data register (FIFO) is less than RWFIFO[RXWATER].
 * Do not set both RXRTSE and TXRTSE.
 *
 *END**************************************************************************/
void uart_hal_enable_receiver_rts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Set RXRTSE */
    HW_UART_MODEM_SET(uartInstance, BM_UART_MODEM_RXRTSE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_receiver_rts
 * Description   : Disable the UART receiver request-to-send functionality.
 * This function allows the user to disable the UART receiver request-to-send (RTS) functionality.
 *
 *END**************************************************************************/
void uart_hal_disable_receiver_rts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Clear RXRTSE */
    HW_UART_MODEM_CLR(uartInstance, BM_UART_MODEM_RXRTSE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_transmitter_rts
 * Description   : Enable the UART transmitter request-to-send functionality.
 * This function allows the user to enable the UART transmitter request-to-send (RTS) functionality.
 * When enabled, it allows the UART to control the RTS assertion before and after a transmission
 * such that when a character is placed into an empty transmitter data buffer , RTS
 * asserts one bit time before the start bit is transmitted. RTS deasserts one bit time after all
 * characters in the transmitter data buffer and shift register are completely sent, including
 * the last stop bit.
 *
 *END**************************************************************************/
void uart_hal_enable_transmitter_rts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Set TXRTSE */
    HW_UART_MODEM_SET(uartInstance, BM_UART_MODEM_TXRTSE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_transmitter_rts
 * Description   : Disable the UART transmitter request-to-send functionality.
 * This function allows the user to disable the UART transmitter request-to-send (RTS)
 * functionality.
 *
 *END**************************************************************************/
void uart_hal_disable_transmitter_rts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Clear TXRTSE */
    HW_UART_MODEM_CLR(uartInstance, BM_UART_MODEM_TXRTSE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_transmitter_rts_polarity
 * Description   : Configure the UART transmitter RTS polarity.
 * This function allows the user configure the transmitter RTS polarity to be either active low
 * or active high.
 *
 *END**************************************************************************/
void uart_hal_configure_transmitter_rts_polarity(uint32_t uartInstance, bool polarity)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Configure the transmitter rts polarity: 0=active low, 1=active high */
    BW_UART_MODEM_TXRTSPOL(uartInstance, polarity);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_transmitter_cts
 * Description   : Enable the UART transmitter clear-to-send functionality.
 * This function allows the user to enable the UART transmitter clear-to-send (CTS) functionality.
 * When enabled, he transmitter checks the state of CTS each time it is ready to send a character.
 * If CTS is asserted, the character is sent. If CTS is deasserted, the signal TXD remains in
 * the mark state and transmission is delayed until CTS is asserted. Changes in CTS as a
 * character is being sent do not affect its transmission.
 *
 *END**************************************************************************/
void uart_hal_enable_transmitter_cts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Set TXCTSE */
    HW_UART_MODEM_SET(uartInstance, BM_UART_MODEM_TXCTSE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_transmitter_cts
 * Description   : Disable the UART transmitter clear-to-send functionality.
 * This function allows the user to disable the UART transmitter clear-to-send (CTS)
 * functionality.
 *
 *END**************************************************************************/
void uart_hal_disable_transmitter_cts(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Clear TXCTSE */
    HW_UART_MODEM_CLR(uartInstance, BM_UART_MODEM_TXCTSE);
}
#endif  /* FSL_FEATURE_UART_HAS_MODEM_SUPPORT */

#if FSL_FEATURE_UART_HAS_IR_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_infrared_operation
 * Description   : Configure the UART infrared operation.
 * The function allows the user to enable or disable the UART infrared (IR) operation
 * and to configure the IR pulse width.
 *
 *END**************************************************************************/
void uart_hal_configure_infrared_operation(uint32_t uartInstance, bool enable,
                                           uart_ir_tx_pulsewidth_t pulseWidth)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* enable or disable infrared */
    BW_UART_IR_IREN(uartInstance, enable);

    /* configure the narrow pulse width of the IR pulse */
    BW_UART_IR_TNP(uartInstance, pulseWidth);
}
#endif  /* FSL_FEATURE_UART_HAS_IR_SUPPORT */

/*******************************************************************************
 * EOF
 ******************************************************************************/

