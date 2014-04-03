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
 * Function Name : uart_hal_init
 * Description   : Initialize the UART controller.
 * This function will initialize the module to user defined settings and default settings.
 * Here is an example demonstrating how to define the uart_config_t structure and call
 * the uart_hal_init function:
 *   uart_config_t uartConfig;
 *   uartConfig.uartSourceClockInHz = uartSourceClock;
 *   uartConfig.baudRate = baudRate;
 *   uartConfig.bitCountPerChar = kUart8BitsPerChar;
 *   uartConfig.parityMode = kUartParityDisabled;
 *   uartConfig.stopBitCount = kUartOneStopBit;
 *   uartConfig.txDataInvert = 0;    uartConfig.rxDataInvert = 0;
 *   uart_hal_init(uartInstance, &uartConfig);
 *
 *END**************************************************************************/
uart_status_t uart_hal_init(uint32_t uartInstance, const uart_config_t *config)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* first, disable the UART transmitter and receiver*/
    uart_hal_disable_transmitter(uartInstance);
    uart_hal_disable_receiver(uartInstance);

    /*********************************************************/
    /* calculate baud rate settings                          */
    /*********************************************************/
    if (uart_hal_set_baud_rate(uartInstance, config->uartSourceClockInHz, config->baudRate)
        != kStatus_UART_Success)
    {
        return kStatus_UART_BaudRateCalculationError;
    }

    /*********************************************************/
    /* configure number of bits in a char                    */
    /*********************************************************/
    if (uart_hal_configure_bit_count_per_char(uartInstance, config->bitCountPerChar)
        != kStatus_UART_Success)
    {
        return kStatus_UART_BitCountNotSupported;
    }

    /*********************************************************/
    /* configure the parity enable/type                      */
    /*********************************************************/
    uart_hal_configure_parity_mode(uartInstance, config->parityMode);

    /*********************************************************/
    /* configure the number of stop bits                     */
    /*********************************************************/
    if (uart_hal_configure_stop_bit_count(uartInstance, config->stopBitCount) !=
        kStatus_UART_Success)
    {
        return kStatus_UART_StopBitCountNotSupported;
    }

    /*********************************************************/
    /* configure tx and rx inversions                        */
    /*********************************************************/
    uart_hal_configure_tx_rx_inversion(uartInstance, config->rxDataInvert, config->txDataInvert);

    /* finally, enable the UART transmitter and receiver*/
    uart_hal_enable_transmitter(uartInstance);
    uart_hal_enable_receiver(uartInstance);

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_baud_rate
 * Description   : Configure the UART baud rate.
 * This function programs the UART baud rate to the desired value passed in by the user. The user
 * must also pass in the module source clock so that the function can calculate the baud
 * rate divisors to their appropriate values.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this is be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_baud_rate(uint32_t uartInstance, uint32_t sourceClockInHz,
                                uint32_t desiredBaudRate)
{
    uint16_t sbr;

    assert(uartInstance < UART_INSTANCE_COUNT);

/* SoCs with low power uart support differ in how they calculate the baud rate
 * for various instances of the uart from SoCs without low power uart support
 */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    uint16_t sbrTemp;
    uint32_t osr;
    uint8_t i;
    uint32_t tempDiff;
    uint32_t calculatedBaud, baudDiff;

    /* calculate baud rate settings*/
    if (uartInstance == 0)
    {
        /* This uart instantiation uses a slightly different baud rate calculation
         * The idea is to use the best OSR (over-sampling rate) possible
         * Note, osr is typically hard-set to 16 in other uart instantiations
         * First calculate the baud rate using the minimum OSR possible (4)
         */
        osr = 4;
        sbr = (sourceClockInHz/(desiredBaudRate * osr));
        calculatedBaud = (sourceClockInHz / (osr * sbr));

        if (calculatedBaud > desiredBaudRate)
        {
            baudDiff = calculatedBaud - desiredBaudRate;
        }
        else
        {
            baudDiff = desiredBaudRate - calculatedBaud;
        }

        /* loop to find the best osr value possible, one that generates minimum baudDiff
         * iterate through the rest of the supported values of osr
         */
        for (i = 5; i <= 32; i++)
        {
            /* calculate the temporary sbr value   */
            sbrTemp = (sourceClockInHz/(desiredBaudRate * i));
            /* calculate the baud rate based on the temporary osr and sbr values*/
            calculatedBaud = (sourceClockInHz / (i * sbrTemp));

            if (calculatedBaud > desiredBaudRate)
            {
                tempDiff = calculatedBaud - desiredBaudRate;
            }
            else
            {
                tempDiff = desiredBaudRate - calculatedBaud;
            }

            if (tempDiff <= baudDiff)
            {
                baudDiff = tempDiff;
                osr = i;  /* update and store the best osr value calculated*/
                sbr = sbrTemp;  /* update store the best sbr value calculated*/
            }
        }

        /* next, check to see if actual baud rate is within 3% of desired baud rate
         * based on the best calculate osr value
         */
        if (baudDiff < ((desiredBaudRate / 100) * 3))
        {
            /* Acceptable baud rate */
            /* Check if osr is between 4x and 7x oversampling*/
            /* If so, then "BOTHEDGE" sampling must be turned on*/
            if ((osr > 3) && (osr < 8))
            {
                HW_UART0_C5_SET(BM_UART0_C5_BOTHEDGE);
            }

            /* program the osr value (bit value is one less than actual value)*/
            BW_UART0_C4_OSR(osr-1);

            /* program the sbr (divider) value obtained above*/
            BW_UART0_BDH_SBR((uint8_t)(sbr >> 8));
            BW_UART0_BDL_SBR((uint8_t)sbr);
        }
        else
        {
            /* Unacceptable baud rate difference of more than 3%*/
            return kStatus_UART_BaudRatePercentDiffExceeded;
        }
    }
    else
    {
        /* calculate the baud rate modulo divisor, sbr*/
        sbr = sourceClockInHz / (desiredBaudRate * 16);

        /* check to see if sbr is out of range of register bits */
        if ( (sbr > 0x1FFF) || (sbr < 1) )
        {
            /* unsupported baud rate for given source clock input*/
            return kStatus_UART_BaudRateCalculationError;
        }

        /* next, check to see if actual baud rate is within 3% of desired baud rate
         * calculate baud rate based on previosuly calculated sbr
         */
        calculatedBaud = (sourceClockInHz  / (16 * sbr));

        if (calculatedBaud > desiredBaudRate)
        {
            baudDiff = calculatedBaud - desiredBaudRate;
        }
        else
        {
            baudDiff = desiredBaudRate - calculatedBaud;
        }

        /* check for baud rate to be within 3%*/
        if (baudDiff > ((desiredBaudRate / 100) * 3))
        {
            /* Unacceptable baud rate difference of more than 3%*/
            return kStatus_UART_BaudRatePercentDiffExceeded;
        }

        /* Else, baud rate is ok and write to the SBR bits in the BDH and BDL registers*/
        BW_UART_BDH_SBR(uartInstance, (uint8_t)(sbr >> 8));
        BW_UART_BDL_SBR(uartInstance, (uint8_t)sbr);
    }
#else
    /* BaudRate = (SourceClkInHz)/[16 * (SBR +  BRFA)]
     * First, calculate SBR (integer part) then calculate the BRFA (fine adjust fractional field)
     */
    uint16_t brfa;

    /* calculate the baud rate modulo divisor, sbr*/
    sbr = sourceClockInHz / (desiredBaudRate * 16);

    /* check to see if sbr is out of range of register bits */
    if ( (sbr > 0x1FFF) || (sbr < 1) )
    {
        /* unsupported baud rate for given source clock input*/
        return kStatus_UART_BaudRateCalculationError;
    }

    /* write the sbr value to the BDH and BDL registers*/
    BW_UART_BDH_SBR(uartInstance, (uint8_t)(sbr >> 8));
    BW_UART_BDL_SBR(uartInstance, (uint8_t)sbr);

    /* determine if a fractional divider is needed to fine tune closer to the desired baud
     * each value of brfa is in 1/32 increments, hence the multiply-by-32
     */
    brfa = (32*sourceClockInHz/(desiredBaudRate*16)) - 32*sbr;

    /* write the brfa value to the register*/
    BW_UART_C4_BRFA(uartInstance, brfa);
#endif

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_baud_rate_divisor
 * Description   : Set the UART baud rate modulo divisor value.
 * This function allows the user to program the baud rate divisor directly in situations
 * where the divisor value is known. In this case, the user may not want to call the
 * uart_hal_set_baud_rate() function as the divisor is already known to them.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_baud_rate_divisor(uint32_t uartInstance, uint32_t baudRateDivisor)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* check to see if baudRateDivisor is out of range of register bits */
    if ( (baudRateDivisor > 0x1FFF) || (baudRateDivisor < 1) )
    {
        /* unsupported bit setting*/
        return kStatus_UART_InvalidBitSetting;
    }

#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* program the sbr (baudRateDivisor) value to the BDH and BDL registers*/
        BW_UART0_BDH_SBR((uint8_t)(baudRateDivisor >> 8));
        BW_UART0_BDL_SBR((uint8_t)baudRateDivisor);
    }
    else
#endif
    {
        /* program the sbr (baudRateDivisor) value to the BDH and BDL registers*/
        BW_UART_BDH_SBR(uartInstance, (uint8_t)(baudRateDivisor >> 8));
        BW_UART_BDL_SBR(uartInstance, (uint8_t)baudRateDivisor);
    }

    return kStatus_UART_Success;
}

#if FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_baud_rate_fine_adjust
 * Description   : Set the UART baud rate fine adjust. (Note: Feature available on select
 *                 UART instances used in conjunction with baud rate programming)
 * This function, which programs the baud rate fine adjust, is used in conjuction with
 * programming the baud rate modulo divisor in situations where these divisors value are known
 * In this case, the user may not want to call the uart_hal_set_baud_rate() function as the
 * divisors are already known to them.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_baud_rate_fine_adjust(uint32_t uartInstance, uint8_t baudFineAdjust)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    if (baudFineAdjust > 0x1F)
    {
        /* unsupported bit setting*/
        return kStatus_UART_InvalidBitSetting;
    }

    BW_UART_C4_BRFA(uartInstance, baudFineAdjust);

    return kStatus_UART_Success;
}
#endif

#if FSL_FEATURE_UART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_set_oversampling_ratio
 * Description   : Set the UART baud rate oversampling ratio. (Note: Feature available on select
 *                 UART instances used in conjunction with baud rate programming)
 * This function allows the user to directly configure the oversampling ratio (OSR). Normally this
 * ratio is automatically configured when calling the uart_hal_set_baud_rate(). However, if the user
 * knows the necessary dividers and wishes to directly program them, they also have the option to
 * directly program the OSR.
 * The oversampling ratio should be set between 4x (00011) and 32x (11111), writing
 * an invalid oversampling ratio with result in an error and will be set to a default
 * 16x (01111) oversampling ratio.
 * It is required that the transmitter/receiver be disabled before calling
 * this function.
 *
 *END**************************************************************************/
uart_status_t uart_hal_set_oversampling_ratio(uint32_t uartInstance, uint32_t overSamplingRatio)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    if (overSamplingRatio > 0x1F)
    {
        /* unsupported bit setting*/
        return kStatus_UART_InvalidBitSetting;
    }

    if (uartInstance == 0)
    {
        BW_UART0_C4_OSR(overSamplingRatio);
        return kStatus_UART_Success;
    }
    else
    {
        return kStatus_UART_OverSamplingNotSupported;
    }
}
#endif

#if FSL_FEATURE_UART_HAS_BOTH_EDGE_SAMPLING_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_both_edge_sampling
 * Description   : Configure the UART baud rate both edge sampling option.
 *                 (Note: Feature available on select UART instances used in conjunction with
 *                 baud rate programming)
 * This function allows the user to directly configure the both edge sampling option.
 * When enabled, the received data is sampled on both edges of the baud rate clock.
 * This must be set when the oversampling ratio is between 4x and 7x.
 * This function should only be called when the receiver is disabled.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_both_edge_sampling(uint32_t uartInstance,
                                                    bool enableBothEdgeSampling)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    if (uartInstance == 0)
    {
        BW_UART0_C5_BOTHEDGE(enableBothEdgeSampling);
        return kStatus_UART_Success;
    }
    else
    {
        return kStatus_UART_BothEdgeNotSupported;
    }
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_both_edge_sampling
 * Description   : Configure number of bits per character in the UART controller.
 * This function allows the user to configure the number of bits per character according to the
 * typedef uart_bit_count_per_char_t.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_bit_count_per_char(uint32_t uartInstance,
                                               uart_bit_count_per_char_t bitCountPerChar)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure number of bits in a char*/
#if FSL_FEATURE_UART_HAS_10BIT_DATA_SUPPORT
    if (uartInstance == 0)
    {
        if(bitCountPerChar == kUart10BitsPerChar)
        {
            HW_UART0_C4_SET(BM_UART0_C4_M10); /*set M10 for 10-bit mode, M bit in C1 is don't care*/
        }
        else
        {
            BW_UART0_C1_M(bitCountPerChar);  /* config 8- (M=0) or 9-bits (M=1) */
            HW_UART0_C4_CLR(BM_UART0_C4_M10); /* clear M10 to make sure not 10-bit mode*/
        }
    }
    else
#endif
    {
        if(bitCountPerChar == kUart10BitsPerChar)
        {
            /* re-enable the UART transmitter and receiver*/
            uart_hal_enable_transmitter(uartInstance);
            uart_hal_enable_receiver(uartInstance);

            /* illegal value, this uart instance does not support 10-bit*/
            return kStatus_UART_BitCountNotSupported;
        }
        else
        {
            BW_UART_C1_M(uartInstance, bitCountPerChar);  /* config 8- (M=0) or 9-bits (M=1) */
        }
    }

    return kStatus_UART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_parity_mode
 * Description   : Configure the parity mode in the UART controller.
 * This function allows the user to configure the parity mode of the UART controller to disable
 * it or enable it for even parity or for odd parity.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
void uart_hal_configure_parity_mode(uint32_t uartInstance, uart_parity_mode_t parityModeType)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure the parity enable/type    */
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        if ((parityModeType) == 0)
        {
            /* parity disabled, hence parity type is don't care*/
            HW_UART0_C1_CLR(BM_UART0_C1_PE);
        }
        else
        {
            /* parity enabled*/
            HW_UART0_C1_SET(BM_UART0_C1_PE);
            /* parity odd/even depending on parity mode setting*/
            BW_UART0_C1_PT((parityModeType) & 0x1);
        }
    }
    else
#endif
    {
        if ((parityModeType) == 0)
        {
            /* parity disabled, hence parity type is don't care*/
            HW_UART_C1_CLR(uartInstance, BM_UART_C1_PE);
        }
        else
        {
            /* parity enabled*/
            HW_UART_C1_SET(uartInstance, BM_UART_C1_PE);
            /* parity odd/even depending on parity mode setting*/
            BW_UART_C1_PT(uartInstance, (parityModeType) & 0x1);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_stop_bit_count
 * Description   : Configure the number of stop bits in the UART controller.
 * This function allows the user to configure the number of stop bits in the UART controller
 * to be one or two stop bits.
 * In some UART instances it is required that the transmitter/receiver should be disabled
 * before calling this function.
 * Generally this may be applied to all UARTs to ensure safe operation.
 *
 *END**************************************************************************/
uart_status_t uart_hal_configure_stop_bit_count(uint32_t uartInstance,
                                           uart_stop_bit_count_t stopBitCount)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    /* configure the number of stop bits*/
    if (uartInstance == 0)
    {
        BW_UART0_BDH_SBNS(stopBitCount);
    }
    else
    {
        BW_UART_BDH_SBNS(uartInstance, stopBitCount);
    }

    return kStatus_UART_Success;
#else
    /* stop bit configuration not supported, only one stop bit is supported*/
    if(stopBitCount != kUartOneStopBit)
    {
        return kStatus_UART_StopBitCountNotSupported;
    }
    else
    {
        return kStatus_UART_Success;
    }
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_tx_rx_inversion
 * Description   : Configure the transmit and receive inversion control in UART controller.
 * This function allows the user to invert the transmit and receive signals, independently.
 * This function should only be called when the UART is between transmit and receive packets.
 *
 *END**************************************************************************/
void uart_hal_configure_tx_rx_inversion(uint32_t uartInstance, uint32_t rxInvert, uint32_t txInvert)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* configure tx and rx inversions*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* 0 - receive data not inverted, 1 - receive data inverted */
        BW_UART0_S2_RXINV(rxInvert);
        /* 0 - transmit data not inverted, 1 - transmit data inverted*/
        BW_UART0_C3_TXINV(txInvert);
    }
    else
#endif
    {
        /* 0 - receive data not inverted, 1 - receive data inverted */
        BW_UART_S2_RXINV(uartInstance, rxInvert);
        /* 0 - transmit data not inverted, 1 - transmit data inverted*/
        BW_UART_C3_TXINV(uartInstance, txInvert);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_transmitter
 * Description   : Enable the UART transmitter.
 * This function allows the user to enable the UART transmitter.
 *
 *END**************************************************************************/
void uart_hal_enable_transmitter(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* enable the transmitter based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_TE);
    }
    else
#endif
    {
        /* for this uart instance, there is a two step process to clear the transmit complete
         * status flag:
         * 1. Read the status register with the status bit set
         * 2. enable the transmitter (change TE from 0 to 1)
         */

        /*  first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* second, enable the transmitter */
        HW_UART_C2_SET(uartInstance, BM_UART_C2_TE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_transmitter
 * Description   : Disable the UART transmitter.
 * This function allows the user to disable the UART transmitter.
 *
 *END**************************************************************************/
void uart_hal_disable_transmitter(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* disable the transmitter based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_TE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_TE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_transmitter_enabled
 * Description   : Get the UART transmitter enabled/disabled configuration setting.
 * This function allows the user to get the setting of the UART transmitter.
 *
 *END**************************************************************************/
bool uart_hal_is_transmitter_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get the transmitter config based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.TE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.TE;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_enable_receiver
 * Description   : Enable the UART receiver.
 * This function allows the user to enable the UART receiver.
 *
 *END**************************************************************************/
void uart_hal_enable_receiver(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* enable the receiver based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_SET(BM_UART0_C2_RE);
    }
    else
#endif
    {
        HW_UART_C2_SET(uartInstance, BM_UART_C2_RE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_disable_receiver
 * Description   : Disable the UART receiver.
 * This function allows the user to disable the UART receiver.
 *
 *END**************************************************************************/
void uart_hal_disable_receiver(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* disable the receiver based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_C2_CLR(BM_UART0_C2_RE);
    }
    else
#endif
    {
        HW_UART_C2_CLR(uartInstance, BM_UART_C2_RE);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_receiver_enabled
 * Description   : Get the UART receiver enabled/disabled configuration setting.
 * This function allows the user to get the setting of the UART receiver.
 *
 *END**************************************************************************/
bool uart_hal_is_receiver_enabled(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get the receiver config based on the uart instance*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        return HW_UART0_C2.B.RE;
    }
    else
#endif
    {
        return HW_UART_C2(uartInstance).B.RE;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

