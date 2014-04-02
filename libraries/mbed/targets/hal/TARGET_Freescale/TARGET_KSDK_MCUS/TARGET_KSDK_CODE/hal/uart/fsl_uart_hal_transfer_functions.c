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
 * Function Name : uart_hal_putchar
 * Description   : This function allows the user to send an 8-bit character from the UART
 *                 data register.
 *
 *END**************************************************************************/
void uart_hal_putchar(uint32_t uartInstance, uint8_t data)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* put 8-bit data into the uart data register*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        HW_UART0_D_WR(data);
    }
    else
#endif
    {
        /* in addition to sending a char, this function also clears the transmit status flags
         * for this uart instance, there is a two step process to clear the transmit status flags:
         * 1. Read the status register with the status bit set
         * 2. write to the data register
         * first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* second, perform a write to the data register  */
        HW_UART_D_WR(uartInstance, data);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_putchar9
 * Description   : This function allows the user to send a 9-bit character from the UART
 *                 data register.
 *
 *END**************************************************************************/

void uart_hal_putchar9(uint32_t uartInstance, uint16_t data)
{
    uint8_t ninthDataBit;

    assert(uartInstance < UART_INSTANCE_COUNT);

    ninthDataBit = (data >> 8U) & 0x1U;  /* isolate the ninth data bit*/

    /* put 9-bit data to transmit*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* first, write to the ninth data bit (bit position T8, where T[0:7]=8-bits, T8=9th bit)*/
        BW_UART0_C3_R9T8(ninthDataBit);

        /* write to the data register last since this will trigger transmit complete status flag
         * also typecast to uint8_t to match register type
         */
        HW_UART0_D_WR((uint8_t)data);
    }
    else
#endif
    {
        /* first, write to the ninth data bit (bit position T8, where T[0:7]=8-bits, T8=9th bit)*/
        BW_UART_C3_T8(uartInstance, ninthDataBit);

        /* in addition to sending a char, this function also clears the transmit status flags
         * for this uart instance, there is a two step process to clear the transmit status flags:
         * 1. Read the status register with the status bit set
         * 2. write to the data register
         * first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* write to the data register last since this will trigger transmit complete status flags
         * also typecast to uint8_t to match register type
         */
        HW_UART_D_WR(uartInstance, (uint8_t)data);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_putchar10
 * Description   : This function allows the user to send a 10-bit character from the UART
 *                 data register. (Note: Feature available on select UART instances)
 *
 *END**************************************************************************/
uart_status_t uart_hal_putchar10(uint32_t uartInstance, uint16_t data)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* put 10-bit data to transmit*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        uint8_t ninthDataBit;
        uint8_t tenthDataBit;
        ninthDataBit = (data >> 8U) & 0x1U;  /* isolate the ninth data bit*/
        tenthDataBit = (data >> 9U) & 0x1U;  /* isolate the tenth data bit*/

        /* first, write to the tenth data bit (bit position T9, where T[0:7]=8-bits,
         * T9=10th bit, T8=9th bit)
         */
        BW_UART0_C3_R8T9(tenthDataBit);

        /* next, write to the ninth data bit (bit position T8, where T[0:7]=8-bits,
         * T9=10th bit, T8=9th bit)
         */
        BW_UART0_C3_R9T8(ninthDataBit);

        /* write to the data register last since this will trigger transmit complete status flag
         * also typecast to uint8_t to match register type
         */
        HW_UART0_D_WR((uint8_t)data);

        return kStatus_UART_Success;
    }
    else
#endif
    {
        /* unsupported feature of this instance number, return error code*/
        return kStatus_UART_BitCountNotSupported;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_getchar
 * Description   : This function gets a received 8-bit character from the UART data register.
 *
 *END**************************************************************************/
void uart_hal_getchar(uint32_t uartInstance, uint8_t *readData)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* get 8-bit data from the uart data register*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        *readData = HW_UART0_D_RD();  /* read 8-bit data from data register*/
    }
    else
#endif
    {
        /* in addition to getting a char, this function also clears the receive status flag RDRF
         * along with IDLE, OR, NF, FE, and PF (these can also be cleared in separate functions)
         * for this uart instance, there is a two step process to clear the receive status flag:
         * 1. Read the status register with the status bit set
         * 2. read from the data register
         * first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* second, perform a read from the data register */
        *readData = HW_UART_D_RD(uartInstance);  /* read 8-bit data from data register*/
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_getchar9
 * Description   : This function gets a received 9-bit character from the UART data register.
 *
 *END**************************************************************************/
void  uart_hal_getchar9(uint32_t uartInstance, uint16_t *readData)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    uint16_t temp;

     /* get 9-bit data from the uart data register*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* read ninth data bit and left shift to bit position R8 before reading
         * the 8 other data bits R[7:0]
         * *readData = (HW_UART0_C3.B.R8T9) << 8;
         */
        temp = HW_UART0_C3.B.R8T9;  /* need this two step process to work around mishra rule*/
        *readData = temp << 8;

        /* do last: get 8-bit data from the uart data register, will clear certain
         * receive status bits once completed
         * need to OR these 8-bits with the ninth bit value above
         */
        *readData |= HW_UART0_D_RD();  /* read 8-bit data from data register*/
    }
    else
#endif
    {
        /* read ninth data bit and left shift to bit position R8 before reading
         * the 8 other data bits R[7:0]
         * *readData = (HW_UART_C3(uartInstance).B.R8) << 8;
         */
        temp = (HW_UART_C3(uartInstance).B.R8);
        *readData = temp << 8;

        /* in addition to getting a char, this function also clears the receive status flag RDRF
         * along with IDLE, OR, NF, FE, and PF (these can also be cleared in separate functions)
         * for this uart instance, there is a two step process to clear the receive status flag:
         * 1. Read the status register with the status bit set
         * 2. read from the data register
         * first read the status register
         * no need to store the read value, it's assumed the status bit is set
         */
        HW_UART_S1_RD(uartInstance);
        /* do last: get 8-bit data from the uart data register,
         * will clear certain receive status bits once completed
         * need to OR these 8-bits with the ninth bit value above
         */
        *readData |= HW_UART_D_RD(uartInstance);  /* read 8-bit data from data register*/
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_getchar10
 * Description   : This function gets a received 10-bit character from the UART data register.
 *                 (Note: Feature available on select UART instances)
 *
 *END**************************************************************************/
uart_status_t uart_hal_getchar10(uint32_t uartInstance, uint16_t *readData)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

     /* get 10-bit data from the uart data register, available only on supported uarts*/
#if FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT
    if (uartInstance == 0)
    {
        /* read tenth data bit and left shift to bit position R9 before reading the 9 other
         * data bits: R8 and R[7:0]
         */
        *readData = (uint16_t)((uint32_t)(HW_UART0_C3.B.R9T8) << 9U);

        /* read ninth data bit and left shift to bit position R8 before reading the 8 other
         * data bits R[7:0]
         */
        *readData |= (uint16_t)((uint32_t)(HW_UART0_C3.B.R8T9) << 8U);

        /* do last: get 8-bit data from the uart data register, will clear certain receive
         * status bits once completed
         * need to OR these 8-bits with the ninth bit value above
         */
        *readData |= HW_UART0_D_RD();  /* read 8-bit data from data register*/

        return kStatus_UART_Success;
    }
    else
#endif
    {
        /* unsupported feature of this instance number, return error code*/
        return kStatus_UART_BitCountNotSupported;
    }
}

#if FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS
/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_configure_bit10_as_paritybit_operation
 * Description   : Configure the UART bit 10 (if enabled) or bit 9 (if disabled) as the
 *                 parity bit in the serial transmission.
 * This function configures bit 10 or bit 9 to be the parity bit.  To configure bit 10 as the parity
 * bit, the function sets UARTx_C4[M10]; it also sets UARTx_C1[M] and UARTx_C1[PE] as required.
 *
 *END**************************************************************************/
void uart_hal_configure_bit10_as_paritybit_operation(uint32_t uartInstance, bool enable)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* to enable the parity bit as the tenth data bit, along with enabling UARTx_C4[M10]
     * need to also enable parity and set UARTx_C1[M] bit
     * assumed that the user has already set the appropriate bits
     */
    BW_UART_C4_M10(uartInstance, enable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_bit10_set_as_paritybit
 * Description   : Get the configuration of the UART bit 10 (if enabled) or bit 9 (if disabled)
 *                 as the parity bit in the serial transmission
 * This function returns true if bit 10 is configured as the parity bit, otherwise it returns
 * false if bit 9 is configured as the parity bit.
 *
 *END**************************************************************************/
bool uart_hal_is_bit10_set_as_paritybit(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* to see if the parity bit is set as the tenth data bit,
     * return value of UARTx_C4[M10]
     */
    return HW_UART_C4(uartInstance).B.M10;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_current_dataword_received_with_noise
 * Description   : Determine if the UART received data word was received with noise.
 * This function will return true if the received data word was received with noise. Otherwise,
 * it will return false indicating no noise was detected.
 *
 *END**************************************************************************/
bool uart_hal_is_current_dataword_received_with_noise(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* to see if the current dataword was received with noise,
     * return value of UARTx_ED[NOISY]
     */
    return HW_UART_ED(uartInstance).B.NOISY;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uart_hal_is_current_dataword_received_with_parityerror
 * Description   : Determine if the UART received data word was received with a parity error.
 * This function will return true if the received data word was received with a parity error.
 * Otherwise, it will return false indicating no parity error was detected.
 *
 *END**************************************************************************/
bool uart_hal_is_current_dataword_received_with_parityerror(uint32_t uartInstance)
{
    assert(uartInstance < UART_INSTANCE_COUNT);

    /* to see if the current dataword was received with parity error,
     * return value of UARTx_ED[PARITYE]
     */
    return HW_UART_ED(uartInstance).B.PARITYE;
}
#endif  /* FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

