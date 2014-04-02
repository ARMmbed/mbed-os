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
 * Function Name : uart_hal_configure_interrupts
 * Description   : Configure the UART module interrupts to enable/disable various interrupt sources.
 * This function allows the user to configure all of the UART interrupts with one function call.
 * The user will first need to initialize and pass in a structure of type uart_interrupt_config_t
 * which sets the configuration of each interrupt.
 *
 *END**************************************************************************/
void uart_hal_configure_interrupts(uint32_t uartInstance,
                                   const uart_interrupt_config_t * interruptConfig)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    uint8_t temp;

    /* configure uart interrupt enables*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* 0 - disable interrupt, 1 - enable interrupt*/

        /* UART BDH register  */
        temp = HW_UART0_BDH_RD() & ~(BM_UART0_BDH_LBKDIE|BM_UART0_BDH_RXEDGIE);
        temp |= BF_UART0_BDH_LBKDIE(interruptConfig->linBreakDetect)
               |BF_UART0_BDH_RXEDGIE(interruptConfig->rxActiveEdge);
        HW_UART0_BDH_WR(temp);

        /* UART C2 register  */
        temp = HW_UART0_C2_RD() & ~(BM_UART0_C2_TIE|BM_UART0_C2_TCIE|BM_UART0_C2_RIE|
                                    BM_UART0_C2_ILIE);
        temp |= BF_UART0_C2_TIE(interruptConfig->transmitDataRegisterEmpty)
               |BF_UART0_C2_TCIE(interruptConfig->transmitComplete)
               |BF_UART0_C2_RIE(interruptConfig->receiverDataRegisterFull)
               |BF_UART0_C2_ILIE(interruptConfig->idleLine);
        HW_UART0_C2_WR(temp);

        /* UART C3 register  */
        temp = HW_UART0_C3_RD() & ~(BM_UART0_C3_ORIE|BM_UART0_C3_NEIE|BM_UART0_C3_FEIE|
                                    BM_UART0_C3_PEIE);
        temp |= BF_UART0_C3_ORIE(interruptConfig->receiverOverrun)
               |BF_UART0_C3_NEIE(interruptConfig->noiseErrorFlag)
               |BF_UART0_C3_FEIE(interruptConfig->frameErrorFlag)
               |BF_UART0_C3_PEIE(interruptConfig->parityErrorFlag);
        HW_UART0_C3_WR(temp);

    }
    else
#endif
    {
        /* 0 - disable interrupt, 1 - enable interrupt*/

        /* UART BDH register  */
        temp = HW_UART_BDH_RD(uartInstance) & ~(BM_UART_BDH_LBKDIE|BM_UART_BDH_RXEDGIE);
        temp |= BF_UART_BDH_LBKDIE(interruptConfig->linBreakDetect)
               |BF_UART_BDH_RXEDGIE(interruptConfig->rxActiveEdge);
        HW_UART_BDH_WR(uartInstance, temp);

        /* UART C2 register  */
        temp = HW_UART_C2_RD(uartInstance) & ~(BM_UART_C2_TIE|BM_UART_C2_TCIE|BM_UART_C2_RIE|
                                               BM_UART_C2_ILIE);
        temp |= BF_UART_C2_TIE(interruptConfig->transmitDataRegisterEmpty)
               |BF_UART_C2_TCIE(interruptConfig->transmitComplete)
               |BF_UART_C2_RIE(interruptConfig->receiverDataRegisterFull)
               |BF_UART_C2_ILIE(interruptConfig->idleLine);
        HW_UART_C2_WR(uartInstance, temp);

        /* UART C3 register  */
        temp = HW_UART_C3_RD(uartInstance) & ~(BM_UART_C3_ORIE|BM_UART_C3_NEIE|BM_UART_C3_FEIE|
                                               BM_UART_C3_PEIE);
        temp |= BF_UART_C3_ORIE(interruptConfig->receiverOverrun)
               |BF_UART_C3_NEIE(interruptConfig->noiseErrorFlag)
               |BF_UART_C3_FEIE(interruptConfig->frameErrorFlag)
               |BF_UART_C3_PEIE(interruptConfig->parityErrorFlag);
        HW_UART_C3_WR(uartInstance, temp);
#if FSL_FEATURE_UART_HAS_FIFO
        /* UART CFIFO register  */
        temp = HW_UART_CFIFO_RD(uartInstance) & ~(BM_UART_CFIFO_TXOFE|BM_UART_CFIFO_RXUFE);
        temp |= BF_UART_CFIFO_TXOFE(interruptConfig->txFifoOverflow)
               |BF_UART_CFIFO_RXUFE(interruptConfig->rxFifoUnderflow);
        HW_UART_CFIFO_WR(uartInstance, temp);
#endif
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_break_detect_interrupt
 * Description   : Enable the break_detect_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_break_detect_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Enable break detect interrupt LBKDIE*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_BDH_SET(BM_UART0_BDH_LBKDIE);
    }
    else
#endif
    {
        HW_UART_BDH_SET(uartInstance, BM_UART_BDH_LBKDIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_break_detect_interrupt
 * Description   : Disable the break_detect_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_break_detect_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Disable break detect interrupt LBKDIE*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_BDH_CLR(BM_UART0_BDH_LBKDIE);
    }
    else
#endif
    {
        HW_UART_BDH_CLR(uartInstance, BM_UART_BDH_LBKDIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_break_detect_interrupt_enabled
 * Description   : Get the configuration of the break_detect_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_break_detect_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of LBKDIE*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_BDH.B.LBKDIE;
    }
    else
#endif
    {
        return HW_UART_BDH(uartInstance).B.LBKDIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_rx_active_edge_interrupt
 * Description   : Enable the rx_active_edge_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_rx_active_edge_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Receiver Active Edge interrupt enable*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_BDH_SET(BM_UART0_BDH_RXEDGIE);
    }
    else
#endif
    {
        HW_UART_BDH_SET(uartInstance, BM_UART_BDH_RXEDGIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_rx_active_edge_interrupt
 * Description   : Disable the rx_active_edge_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_rx_active_edge_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* Receiver Active Edge interrupt disable*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_BDH_CLR(BM_UART0_BDH_RXEDGIE);
    }
    else
#endif
    {
        HW_UART_BDH_CLR(uartInstance, BM_UART_BDH_RXEDGIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rx_active_edge_interrupt_enabled
 * Description   : Get the configuration of the rx_active_edge_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_rx_active_edge_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of RXEDGIE*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_BDH.B.RXEDGIE;
    }
    else
#endif
    {
        return HW_UART_BDH(uartInstance).B.RXEDGIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_tx_data_register_empty_interrupt
 * Description   : Enable the tx_data_register_empty_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_tx_data_register_empty_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* transmit interrupt enable for TDRE (transmit data register empty)*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_TIE);
    }
    else
#endif
    {
        HW_UART_C2_SET(uartInstance, BM_UART_C2_TIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_tx_data_register_empty_interrupt
 * Description   : Disable the tx_data_register_empty_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_tx_data_register_empty_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* transmit interrupt disable for TDRE (transmit data register empty)*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_TIE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_TIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_tx_data_register_empty_interrupt_enabled
 * Description   : Get the configuration of the tx_data_register_empty_interrupt enable stting.
 *
 *END**************************************************************************/
bool uart_hal_is_tx_data_register_empty_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of TIE  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.TIE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.TIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_transmission_complete_interrupt
 * Description   : Enable the transmission_complete_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_transmission_complete_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* transmission complete interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_TCIE);
    }
    else
#endif
    {
        HW_UART_C2_SET(uartInstance, BM_UART_C2_TCIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_transmission_complete_interrupt
 * Description   : Disable the transmission_complete_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_transmission_complete_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* transmission complete interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_TCIE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_TCIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_transmission_complete_interrupt_enabled
 * Description   : Get the configuration of the transmission_complete_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_transmission_complete_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of TCIE  */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.TCIE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.TCIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_rx_data_register_full_interrupt
 * Description   : Enable the rx_data_register_full_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_rx_data_register_full_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* receiver interrupt enable for receiver data register full (RDRF)*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_RIE);
    }
    else
#endif
    {
        HW_UART_C2_SET(uartInstance, BM_UART_C2_RIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_rx_data_register_full_interrupt
 * Description   : Disable the rx_data_register_full_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_rx_data_register_full_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* receiver interrupt disable for receiver data register full (RDRF)*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_RIE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_RIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receive_data_full_interrupt_enabled
 * Description   : Get the configuration of the rx_data_register_full_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_receive_data_full_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of RIE   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.RIE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.RIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_idle_line_interrupt
 * Description   : Enable the idle_line_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_idle_line_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /*idle line interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_ILIE);
    }
    else
#endif
    {
        HW_UART_C2_SET(uartInstance, BM_UART_C2_ILIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_idle_line_interrupt
 * Description   : Disable the idle_line_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_idle_line_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* idle line interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_ILIE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_ILIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_idle_line_interrupt_enabled
 * Description   : Get the configuration of the idle_line_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_idle_line_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of ILIE   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.ILIE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.ILIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_rx_overrun_interrupt
 * Description   : Enable the rx_overrun_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_rx_overrun_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /*receiver overrun interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_SET(BM_UART0_C3_ORIE);
    }
    else
#endif
    {
        HW_UART_C3_SET(uartInstance, BM_UART_C3_ORIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_rx_overrun_interrupt
 * Description   : Disable the rx_overrun_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_rx_overrun_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* receiver overrun interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_CLR(BM_UART0_C3_ORIE);
    }
    else
#endif
    {
        HW_UART_C3_CLR(uartInstance, BM_UART_C3_ORIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rx_overrun_interrupt_enabled
 * Description   : Get the configuration of the rx_overrun_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_rx_overrun_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of OR   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C3.B.ORIE;
    }
    else
#endif
    {
        return HW_UART_C3(uartInstance).B.ORIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_noise_error_interrupt
 * Description   : Enable the noise_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_noise_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /*noise error interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_SET(BM_UART0_C3_NEIE);
    }
    else
#endif
    {
        HW_UART_C3_SET(uartInstance, BM_UART_C3_NEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_noise_error_interrupt
 * Description   : Disable the noise_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_noise_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* noise error interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_CLR(BM_UART0_C3_NEIE);
    }
    else
#endif
    {
        HW_UART_C3_CLR(uartInstance, BM_UART_C3_NEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_noise_error_interrupt_enabled
 * Description   : Get the configuration of the noise_error_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_noise_error_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of NF   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C3.B.NEIE;
    }
    else
#endif
    {
        return HW_UART_C3(uartInstance).B.NEIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_framing_error_interrupt
 * Description   : Enable the framing_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_framing_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /*framing error interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_SET(BM_UART0_C3_FEIE);
    }
    else
#endif
    {
        HW_UART_C3_SET(uartInstance, BM_UART_C3_FEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_framing_error_interrupt
 * Description   : Disable the framing_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_framing_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* framing error interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_CLR(BM_UART0_C3_FEIE);
    }
    else
#endif
    {
        HW_UART_C3_CLR(uartInstance, BM_UART_C3_FEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_framing_error_interrupt_enabled
 * Description   : Get the configuration of the framing_error_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_framing_error_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of FE   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C3.B.FEIE;
    }
    else
#endif
    {
        return HW_UART_C3(uartInstance).B.FEIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_parity_error_interrupt
 * Description   : Enable the parity_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_enable_parity_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* parity error interrupt enable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_SET(BM_UART0_C3_PEIE);
    }
    else
#endif
    {
        HW_UART_C3_SET(uartInstance, BM_UART_C3_PEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_parity_error_interrupt
 * Description   : Disable the parity_error_interrupt.
 *
 *END**************************************************************************/
void uart_hal_disable_parity_error_interrupt(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* parity error interrupt disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C3_CLR(BM_UART0_C3_PEIE);
    }
    else
#endif
    {
        HW_UART_C3_CLR(uartInstance, BM_UART_C3_PEIE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_parity_error_interrupt_enabled
 * Description   : Get the configuration of the parity_error_interrupt enable setting.
 *
 *END**************************************************************************/
bool uart_hal_is_parity_error_interrupt_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* return interrupt enable condition of PF   */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C3.B.PEIE;
    }
    else
#endif
    {
        return HW_UART_C3(uartInstance).B.PEIE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_dma
 * Description   : Configure the UART DMA requests for the Transmitter and Receiver.
 * This function allows the user to configure the transmit data register empty flag to
 * generate an interrupt request (default) or a DMA request.  Similarly, this function
 * allows the user to conigure the receive data register full flag to generate an interrupt
 * request (default) or a DMA request.
 *
 *END**************************************************************************/
void uart_hal_configure_dma(uint32_t uartInstance, bool txDmaConfig, bool  rxDmaConfig)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* TDMAE configures the transmit data register empty flag, S1[TDRE],
         * to generate a DMA request.
         */
        BW_UART0_C5_TDMAE(txDmaConfig); /* set TDMAE to enable, clear to disable */
        /* RDMAE configures the receive data register full flag, S1[RDRF],
         * to generate a DMA request.
         */
        BW_UART0_C5_RDMAE(rxDmaConfig); /* set RDMAE to enable, clear to disable*/
    }
    else
#endif
    {
        /* TDMAS configures the transmit data register empty flag, TDRE, to generate interrupt
         * or DMA requests if TIE is set.
         * NOTE: If UART_C2[TIE] is cleared, TDRE DMA and TDRE interrupt request signals are
         * not asserted when the TDRE flag is set, regardless of the state of TDMAS.
         * If UART_C2[TIE] and TDMAS are both set, then UART_C2[TCIE] must be cleared, and UART_D
         * must not be written outside of servicing of a DMA request.
         * 0 If TIE is set and the TDRE flag is set, the TDRE interrupt request signal is asserted
         * to request interrupt service.
         * 1 If TIE is set and the TDRE flag is set, the TDRE DMA request signal is asserted
         * to request a DMA transfer.
         */
        if (txDmaConfig == 1)
        {
            /* enable uart to generate transmit DMA request*/
            HW_UART_C2_SET(uartInstance, BM_UART_C2_TIE); /* set TIE */
            HW_UART_C2_CLR(uartInstance, BM_UART_C2_TCIE); /* clear TCIE */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
            HW_UART_C4_SET(uartInstance, BM_UART_C4_TDMAS); /* set TDMAS */
#else
            HW_UART_C5_SET(uartInstance, BM_UART_C5_TDMAS); /* set TDMAS */
#endif
        }
        else
        {
            /* disable uart transmit DMA request*/
            HW_UART_C2_CLR(uartInstance, BM_UART_C2_TIE); /* clear TIE to disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
            HW_UART_C4_CLR(uartInstance, BM_UART_C4_TDMAS); /* clear TDMAS to disable */
#else
            HW_UART_C5_CLR(uartInstance, BM_UART_C5_TDMAS); /* clear TDMAS to disable */
#endif
        }

        /* RDMAS configures the receiver data register full flag, RDRF, to generate interrupt or
         * DMA requests if RIEis set.
         * NOTE: If RIE is cleared, the RDRF DMA and RDRF interrupt request signals are not
         * asserted when the RDRF flag is set, regardless of the state of RDMAS.
         * 0 If RIE is set and the RDRF flag is set, the RDRF interrupt request signal is
         * asserted to request interrupt service.
         * 1 If RIE is set and the RDRF flag is set, the RDRF DMA request signal is asserted
         * to request a DMA transfer.
         */
        if (rxDmaConfig == 1)
        {
            /* enable uart to generate receive DMA request*/
            HW_UART_C2_SET(uartInstance, BM_UART_C2_RIE); /* set RIE */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
            HW_UART_C4_SET(uartInstance, BM_UART_C4_RDMAS); /* set RDMAS */
#else
            HW_UART_C5_SET(uartInstance, BM_UART_C5_RDMAS); /* set RDMAS */
#endif
        }
        else
        {
            /* disable uart receive DMA request*/
            HW_UART_C2_CLR(uartInstance, BM_UART_C2_RIE); /* clear RIE to disable */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
            HW_UART_C4_CLR(uartInstance, BM_UART_C4_RDMAS); /* clear RDMAS to disable */
#else
            HW_UART_C5_CLR(uartInstance, BM_UART_C5_RDMAS); /* clear RDMAS to disable */
#endif
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_txdma_enabled
 * Description   : Get the UART Transmit DMA request configuration setting.
 * This function returns to the user the configuration setting of the Transmit DMA request.
 *
 *END**************************************************************************/
bool uart_hal_is_txdma_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* TDMAE configures the transmit data register empty flag, S1[TDRE], to
         * generate a DMA request.
         */
        return HW_UART0_C5.B.TDMAE; /* return TDMAE, set: enable, clear: disable */
    }
    else
#endif
    {
        /* create variable for this to work around MISRA rule 12.4 since this is a volatile value*/
        uint32_t tcieBitStatus;
        tcieBitStatus = HW_UART_C2(uartInstance).B.TCIE;

        /* TDMAS configures the transmit data register empty flag, TDRE, to generate interrupt or
         * DMA requests if TIE is set.
         * NOTE: If UART_C2[TIE] is cleared, TDRE DMA and TDRE interrupt request signals are
         * not asserted when the TDRE flag is set, regardless of the state of TDMAS.
         * If UART_C2[TIE] and TDMAS are both set, then UART_C2[TCIE] must be cleared, and UART_D
         * must not be written outside of servicing of a DMA request.
         * 0 If TIE is set and the TDRE flag is set, the TDRE interrupt request signal is asserted
         * to request interrupt service.
         * 1 If TIE is set and the TDRE flag is set, the TDRE DMA request signal is asserted to
         * request a DMA transfer.
         */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
        if (HW_UART_C4(uartInstance).B.TDMAS == 1)
#else
        if (HW_UART_C5(uartInstance).B.TDMAS == 1)
#endif
        {
            /* in order to enable transmit DMA request, TIE must be set and TCIE must be cleared*/
            if ((HW_UART_C2(uartInstance).B.TIE == 1) &&
                (tcieBitStatus == 0))
            {
                /* UART module is configured to generate TxDMA request*/
                return 1;
            }
            else
            {
                /* UART module is NOT configured to generate TxDMA request*/
                return 0;
            }
        }
        else
        {
            /* UART module is NOT configured to generate TxDMA request*/
            return 0;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_rxdma_enabled
 * Description   : Get the UART Receive DMA request configuration setting.
 * This function returns to the user the configuration setting of the Receive DMA request.
 *
 *END**************************************************************************/
bool uart_hal_is_rxdma_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* RDMAE configures the receive data register fell flag, S1[RDRF], to
         * generate a DMA request.
         */
        return HW_UART0_C5.B.RDMAE; /* return RDMAE, set: enable, clear: disable*/
    }
    else
#endif
    {
        /* RDMAS configures the receiver data register full flag, RDRF, to generate interrupt or
         * DMA requests if RIE is set.
         * NOTE: If RIE is cleared, the RDRF DMA and RDRF interrupt request signals are not
         * asserted when the RDRF flag is set, regardless of the state of RDMAS.
         * 0 If RIE is set and the RDRF flag is set, the RDRF interrupt request signal is asserted
         * to requestinterrupt service.
         * 1 If RIE is set and the RDRF flag is set, the RDRF DMA request signal is asserted to
         * request a DMA transfer.
         */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
        if (HW_UART_C4(uartInstance).B.RDMAS == 1)
#else
        if (HW_UART_C5(uartInstance).B.RDMAS == 1)
#endif
         {
            /* enable uart to generate receive DMA request*/
            if (HW_UART_C2(uartInstance).B.RIE == 1)
            {
                /* UART module is configured to generate RxDMA request*/
                return 1;
            }
            else
            {
                /* UART module is NOT configured to generate RxDMA request*/
                return 0;
            }
        }
        else
        {
            /* UART module is NOT configured to generate RxDMA request*/
            return 0;
        }
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

