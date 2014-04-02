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
 * Function Name : uart_hal_get_all_status_flag
 * Description   : Get all of the UART status flag states.
 * This function gets all of the UART status flag states and places into a structure of
 * type uart_status_flag_all_t.  The user must pass in a pointer to this structure.
 *
 *END**************************************************************************/
void uart_hal_get_all_status_flag(uint32_t uartInstance, uart_status_flag_all_t *allStatusFlag)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of all status flags  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        allStatusFlag->transmitDataRegisterEmpty = HW_UART0_S1.B.TDRE;
        allStatusFlag->transmissionComplete =  HW_UART0_S1.B.TC;
        allStatusFlag->receiveDataRegisterFull = HW_UART0_S1.B.RDRF;
        allStatusFlag->idleLineDetect = HW_UART0_S1.B.IDLE;
        allStatusFlag->receiveOverrun = HW_UART0_S1.B.OR;
        allStatusFlag->noiseDetect = HW_UART0_S1.B.NF;
        allStatusFlag->frameError = HW_UART0_S1.B.FE;
        allStatusFlag->parityError = HW_UART0_S1.B.PF;
        allStatusFlag->lineBreakDetect = HW_UART0_S2.B.LBKDIF;
        allStatusFlag->receiveActiveEdgeDetect = HW_UART0_S2.B.RXEDGIF;
        allStatusFlag->receiverActive = HW_UART0_S2.B.RAF;
    }
    else
#endif
    {
        allStatusFlag->transmitDataRegisterEmpty = HW_UART_S1(uartInstance).B.TDRE;
        allStatusFlag->transmissionComplete =  HW_UART_S1(uartInstance).B.TC;
        allStatusFlag->receiveDataRegisterFull = HW_UART_S1(uartInstance).B.RDRF;
        allStatusFlag->idleLineDetect = HW_UART_S1(uartInstance).B.IDLE;
        allStatusFlag->receiveOverrun = HW_UART_S1(uartInstance).B.OR;
        allStatusFlag->noiseDetect = HW_UART_S1(uartInstance).B.NF;
        allStatusFlag->frameError = HW_UART_S1(uartInstance).B.FE;
        allStatusFlag->parityError = HW_UART_S1(uartInstance).B.PF;
        allStatusFlag->lineBreakDetect = HW_UART_S2(uartInstance).B.LBKDIF;
        allStatusFlag->receiveActiveEdgeDetect = HW_UART_S2(uartInstance).B.RXEDGIF;
        allStatusFlag->receiverActive = HW_UART_S2(uartInstance).B.RAF;
#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
        allStatusFlag->noiseInCurrentWord = HW_UART_ED(uartInstance).B.NOISY;
        allStatusFlag->parityErrorInCurrentWord = HW_UART_ED(uartInstance).B.PARITYE;
#endif
#if FSL_FEATURE_UART_HAS_FIFO
        allStatusFlag->txBufferEmpty = HW_UART_SFIFO(uartInstance).B.TXEMPT;
        allStatusFlag->rxBufferEmpty = HW_UART_SFIFO(uartInstance).B.RXEMPT;
        allStatusFlag->txBufferOverflow = HW_UART_SFIFO(uartInstance).B.TXOF;
        allStatusFlag->rxBufferUnderflow = HW_UART_SFIFO(uartInstance).B.RXUF;
#endif
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_transmit_data_register_empty
 * Description   : Get the UART Transmit data register empty flag.
 * This function returns the state of the UART Transmit data register empty flag.
 *
 *END**************************************************************************/
bool uart_hal_is_transmit_data_register_empty(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of TDRE flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.TDRE;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.TDRE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_transmission_complete
 * Description   : Get the UART Transmission complete flag.
 * This function returns the state of the UART Transmission complete flag.
 *
 *END**************************************************************************/
bool uart_hal_is_transmission_complete(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of TC flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.TC;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.TC;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receive_data_register_full
 * Description   : Get the UART Receive data register full flag.
 * This function returns the state of the UART Receive data register full flag.
 *
 *END**************************************************************************/
bool uart_hal_is_receive_data_register_full(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of RDRF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.RDRF;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.RDRF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_idle_line_detected
 * Description   : Get the UART Idle line detect flag.
 * This function returns the state of the UART Idle line detect flag.
 *
 *END**************************************************************************/
bool uart_hal_is_idle_line_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of IDLE flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.IDLE;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.IDLE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receive_overrun_detected
 * Description   : Get the UART Receiver Overrun status flag.
 * This function returns the state of the the UART Receiver Overrun status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_receive_overrun_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of OR flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.OR;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.OR;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_noise_detected
 * Description   : Get the UART noise status flag.
 * This function returns the state of the UART noise status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_noise_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of NF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.NF;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.NF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_frame_error_detected
 * Description   : Get the UART Frame error status flag.
 * This function returns the state of the UART Frame error status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_frame_error_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of FE flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.FE;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.FE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_parity_error_detected
 * Description   : Get the UART parity error status flag.
 * This function returns the state of the UART parity error status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_parity_error_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of PF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S1.B.PF;
    }
    else
#endif
    {
        return HW_UART_S1(uartInstance).B.PF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_line_break_detected
 * Description   : Get the UART LIN break detect interrupt status flag.
 * This function returns the state of the UART LIN break detect interrupt status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_line_break_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of LBKDIF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S2.B.LBKDIF;
    }
    else
#endif
    {
        return HW_UART_S2(uartInstance).B.LBKDIF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receive_active_edge_detected
 * Description   : Get the UART Receive pin active edge interrupt status flag.
 * This function returns the state of the UART Receive pin active edge interrupt status flag.
 *
 *END**************************************************************************/
bool uart_hal_is_receive_active_edge_detected(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of RXEDGIF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S2.B.RXEDGIF;
    }
    else
#endif
    {
        return HW_UART_S2(uartInstance).B.RXEDGIF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receiver_active
 * Description   : Get the UART Receiver Active Flag (RAF) state.
 * This function returns the state of the UART Receiver Active Flag (RAF).
 *
 *END**************************************************************************/
bool uart_hal_is_receiver_active(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return status condition of RAF flag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_S2.B.RAF;
    }
    else
#endif
    {
        return HW_UART_S2(uartInstance).B.RAF;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_clear_status_flag
 * Description   : Clear an individual and specific UART status flag.
 * This function allows the user to clear an individual and specific UART status flag. Refer to
 * structure definition uart_status_flag_t for list of status bits.
 *
 *END**************************************************************************/
uart_status_t uart_hal_clear_status_flag(uint32_t uartInstance, uart_status_flag_t statusFlag)
{
    uart_status_t returnCode;       /* return code variable */

    assert(uartInstance < UART_INSTANCE_COUNT);

    returnCode = kStatus_UART_Success;  /* default return code, unless changed by error condition*/

    /* clear the desired, individual status flag as passed in through statusFlag  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        switch(statusFlag)
        {
            case kUartTransmitDataRegisterEmpty:
                /* This flag is cleared automatically by other uart operations
                 * and cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartTransmissionComplete:
                /* This flag is cleared automatically by other uart operations
                 * and cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartReceiveDataRegisterFull:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartIdleLineDetect:
                /* write one to clear status flag */
                HW_UART0_S1_SET(BM_UART0_S1_IDLE);
                break;

            case kUartReceiveOverrun:
                /* write one to clear status flag */
                HW_UART0_S1_SET(BM_UART0_S1_OR);
                break;

            case kUartNoiseDetect:
                /* write one to clear status flag */
                HW_UART0_S1_SET(BM_UART0_S1_NF);
                break;

            case kUartFrameError:
                /* write one to clear status flag */
                HW_UART0_S1_SET(BM_UART0_S1_FE);
                break;

            case kUartParityError:
                /* write one to clear status flag */
                HW_UART0_S1_SET(BM_UART0_S1_PF);
                break;

            case kUartLineBreakDetect:
                /* write one to clear status flag */
                HW_UART0_S2_SET(BM_UART0_S2_LBKDIF);
                break;

            case kUartReceiveActiveEdgeDetect:
                /* write one to clear status flag */
                HW_UART0_S2_SET(BM_UART0_S2_RXEDGIF);
                break;

            case kUartReceiverActive:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            default:  /* catch inputs that are not recognized*/
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;
        }
    }
    else
#endif
    {
        switch(statusFlag)
        {
            case kUartTransmitDataRegisterEmpty:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartTransmissionComplete:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartReceiveDataRegisterFull:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartIdleLineDetect:
                /* to clear the status is a two-step process:
                 * first, read S1 register with the status flag set
                 */
                HW_UART_S1_RD(uartInstance);
                /* second, read the data register*/
                HW_UART_D_RD(uartInstance);
                break;

            case kUartReceiveOverrun:
                /* to clear the status is a two-step process:
                 * first, read S1 register with the status flag set
                 */
                HW_UART_S1_RD(uartInstance);
                /* second, read the data register*/
                HW_UART_D_RD(uartInstance);
                break;

            case kUartNoiseDetect:
                /* to clear the status is a two-step process:
                 * first, read S1 register with the status flag set
                 */
                HW_UART_S1_RD(uartInstance);
                /* second, read the data register*/
                HW_UART_D_RD(uartInstance);
                break;

            case kUartFrameError:
                /* to clear the status is a two-step process:
                 * first, read S1 register with the status flag set
                 */
                HW_UART_S1_RD(uartInstance);
                /* second, read the data register*/
                HW_UART_D_RD(uartInstance);
                break;

            case kUartParityError:
                /* to clear the status is a two-step process:
                 * first, read S1 register with the status flag set
                 */
                HW_UART_S1_RD(uartInstance);
                /* second, read the data register*/
                HW_UART_D_RD(uartInstance);
                break;

            case kUartLineBreakDetect:
                /* write one to clear status flag */
                HW_UART_S2_SET(uartInstance, BM_UART_S2_LBKDIF);
                break;

            case kUartReceiveActiveEdgeDetect:
                /* write one to clear status flag */
                HW_UART_S2_SET(uartInstance, BM_UART_S2_RXEDGIF);
                break;

            case kUartReceiverActive:
                /* This flag is cleared automatically by other uart operations and
                 * cannot be manually cleared, return error code
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
            case kUartNoiseInCurrentWord:
                /* This flag is not clearable, it simply reflects the status in the
                 * current data word and changes with each new data word
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartParityErrorInCurrentWord:
                /* This flag is not clearable, it simply reflects the status in the
                 * current data word and changes with each new data word
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;
#endif
#if FSL_FEATURE_UART_HAS_FIFO
            case kUartTxBufferEmpty:
                /* This flag is not clearable, it simply reflects the current
                 * status of the buffer/FIFO
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartRxBufferEmpty:
                /* This flag is not clearable, it simply reflects the current
                 * status of the buffer/FIFO
                 */
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;

            case kUartTxBufferOverflow:
                /* write one to clear status flag */
                HW_UART_SFIFO_SET(uartInstance, BM_UART_SFIFO_TXOF);
                break;

            case kUartRxBufferUnderflow:
                /* write one to clear status flag */
                HW_UART_SFIFO_SET(uartInstance, BM_UART_SFIFO_RXUF);
                break;
#endif
            default:  /* catch inputs that are not recognized*/
                returnCode = kStatus_UART_ClearStatusFlagError;
                break;
        }
    }

    return (returnCode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_clear_all_non_autoclear_status_flags
 * Description   : Clear ALL of the UART status flags.
 * This function tries to clear all of the UART status flags.  In some cases, some of the status
 * flags may not get cleared because of the condition that set the flag may still exist.
 *
 *END**************************************************************************/
void uart_hal_clear_all_non_autoclear_status_flags(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* clear the status flags that can be manually cleared
     * note, some flags are automatically cleared and cannot be cleared automatically
     */
    uart_hal_clear_status_flag(uartInstance, kUartIdleLineDetect);
    uart_hal_clear_status_flag(uartInstance, kUartReceiveOverrun);
    uart_hal_clear_status_flag(uartInstance, kUartNoiseDetect);
    uart_hal_clear_status_flag(uartInstance, kUartFrameError);
    uart_hal_clear_status_flag(uartInstance, kUartParityError);
    uart_hal_clear_status_flag(uartInstance, kUartLineBreakDetect);
    uart_hal_clear_status_flag(uartInstance, kUartReceiveActiveEdgeDetect);
#if FSL_FEATURE_UART_HAS_FIFO
    uart_hal_clear_status_flag(uartInstance, kUartTxBufferOverflow);
    uart_hal_clear_status_flag(uartInstance, kUartRxBufferUnderflow);
#endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

