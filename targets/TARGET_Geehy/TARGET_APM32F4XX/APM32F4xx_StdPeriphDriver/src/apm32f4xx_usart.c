/*!
 * @file        apm32f4xx_usart.c
 *
 * @brief       This file provides all the USART firmware functions
 *
 * @version     V1.0.3
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2021-2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

#include "apm32f4xx_usart.h"
#include "apm32f4xx_rcm.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @defgroup USART_Driver
  * @brief USART driver modules
  @{
*/

/** @defgroup USART_Functions
  @{
*/

/*!
 * @brief     Reset usart peripheral registers to their default reset values
 *
 * @param     USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or
 *                    UART peripheral.
 *
 * @retval    None
 */
void USART_Reset(USART_T* usart)
{
    if (USART1 == usart)
    {
        RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_USART1);
        RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_USART1);
    }
    else if (USART2 == usart)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_USART2);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_USART2);
    }
    else if (USART3 == usart)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_USART3);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_USART3);
    }
    else if (UART4 == usart)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_UART4);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_UART4);
    }
    else if (UART5 == usart)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_UART5);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_UART5);
    }
    else if (USART6 == usart)
    {
        RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_USART6);
        RCM_DisableAPB2PeriphReset(RCM_APB2_PERIPH_USART6);
    }
}

/*!
 * @brief     Config the USART peripheral according to the specified parameters in the usartConfig
 *
 * @param     usart:         Select the USART or the UART peripheral
 *
 * @param     usartConfig:  pointer to a USART_Config_T structure
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_Config(USART_T* usart, USART_Config_T* usartConfig)
{
    uint32_t temp, fCLK, intDiv, fractionalDiv;

    usart->CTRL1_B.RXEN = usartConfig->mode & 0x01;
    usart->CTRL1_B.TXEN = usartConfig->mode >> 1;
    usart->CTRL1_B.PCFG = usartConfig->parity >> 1;
    usart->CTRL1_B.PCEN = usartConfig->parity & 0x01;
    usart->CTRL1_B.DBLCFG = usartConfig->wordLength;

    usart->CTRL2_B.STOPCFG = usartConfig->stopBits;

    usart->CTRL3_B.RTSEN = usartConfig->hardwareFlow & 0x01;
    usart->CTRL3_B.CTSEN = usartConfig->hardwareFlow >> 1;

    if ((usart == USART1) || (usart == USART6))
    {
        RCM_ReadPCLKFreq(NULL, &fCLK);
    }
    else
    {
        RCM_ReadPCLKFreq(&fCLK, NULL);
    }

    /* Compute the integer part */
    if (usart->CTRL1_B.OSMCFG != RESET) /*!< Oversampling by 8 */
    {
        intDiv = ((25 * fCLK) / (2 * (usartConfig->baudRate)));
    }
    else /*!< Oversampling by 16 */
    {
        intDiv = ((25 * fCLK) / (4 * (usartConfig->baudRate)));
    }

    temp = (intDiv / 100) << 4;
    fractionalDiv = intDiv - (100 * (temp >> 4));

    /* Implement the fractional part in the register */
    if (usart->CTRL1_B.OSMCFG != RESET) /*!< Oversampling by 8 */
    {
        temp |= ((((fractionalDiv * 8) + 50) / 100)) & ((uint8_t)0x07);
    }
    else /*!< Oversampling by 16 */
    {
        temp |= ((((fractionalDiv * 16) + 50) / 100)) & ((uint8_t)0x0F);
    }

    usart->BR = temp;
}

/*!
 * @brief     Fills each USART_InitStruct member with its default value
 *
 * @param     usartConfig:   pointer to a USART_Config_T structure which will be initialized
 *
 * @retval    None
 */
void USART_ConfigStructInit(USART_Config_T* usartConfig)
{
    usartConfig->baudRate = 9600;
    usartConfig->wordLength = USART_WORD_LEN_8B;
    usartConfig->stopBits = USART_STOP_BIT_1;
    usartConfig->parity = USART_PARITY_NONE ;
    usartConfig->mode = USART_MODE_TX_RX;
    usartConfig->hardwareFlow = USART_HARDWARE_FLOW_NONE;
}

/*!
 * @brief     Configuration communication clock
 *
 * @param     usart:  Select the USART or the UART peripheral
 *
 * @param     clockConfig:   Pointer to a USART_clockConfig_T structure
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3 and USART6
 */
void USART_ConfigClock(USART_T* usart, USART_ClockConfig_T* clockConfig)
{
    usart->CTRL2_B.CLKEN   = clockConfig->clock;
    usart->CTRL2_B.CPHA    = clockConfig->phase;
    usart->CTRL2_B.CPOL    = clockConfig->polarity;
    usart->CTRL2_B.LBCPOEN = clockConfig->lastBit;
}

/*!
 * @brief     Fills each clockConfig member with its default value
 *
 * @param     clockConfig:   Pointer to a USART_clockConfig_T structure
 *
 * @retval    None
 *
 * @note
 */
void USART_ConfigClockStructInit(USART_ClockConfig_T* clockConfig)
{
    clockConfig->clock     = USART_CLKEN_DISABLE;
    clockConfig->phase     = USART_CLKPHA_1EDGE;
    clockConfig->polarity  = USART_CLKPOL_LOW;
    clockConfig->lastBit   = USART_LBCP_DISABLE;
}

/*!
 * @brief     Enables the specified USART peripheral
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_Enable(USART_T* usart)
{
    usart->CTRL1_B.UEN = BIT_SET;
}

/*!
 * @brief     Disable the specified USART peripheral
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_Disable(USART_T* usart)
{
    usart->CTRL1_B.UEN = BIT_RESET;
}

/*!
 * @brief     Sets the system clock divider number
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     div:   specifies the divider number
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ConfigPrescaler(USART_T* usart, uint8_t div)
{
    usart->GTPSC_B.PSC = div;
}

/*!
 * @brief     Enables the USART's 8x oversampling mode
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableOverSampling8(USART_T* usart)
{
    usart->CTRL1_B.OSMCFG = BIT_SET;
}

/*!
 * @brief     Disable the USART's 8x oversampling mode
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableOverSampling8(USART_T* usart)
{
    usart->CTRL1_B.OSMCFG = BIT_RESET;
}

/*!
 * @brief     Enables the USART's sample method
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableOverSampling(USART_T* usart)
{
    usart->CTRL3_B.SAMCFG = BIT_SET;
}

/*!
 * @brief     Disable the USART's sample method
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableOverSampling(USART_T* usart)
{
    usart->CTRL3_B.SAMCFG = BIT_RESET;
}

/*!
 * @brief     Transmits single data
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     data:  the data to transmit
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_TxData(USART_T* usart, uint16_t data)
{
    usart->DATA_B.DATA = data;
}

/*!
 * @brief     Returns the most recent received data
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
uint16_t USART_RxData(USART_T* usart)
{
    return (uint16_t)(usart->DATA_B.DATA);
}

/*!
 * @brief     Configures the address of the USART node
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @param     address: Indicates the address of the USART node
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_Address(USART_T* usart, uint8_t address)
{
    usart->CTRL2_B.ADDR = address;
}

/*!
 * @brief     Enable USART Receiver in mute mode
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableMuteMode(USART_T* usart)
{
    usart->CTRL1_B.RXMUTEEN = BIT_SET;
}

/*!
 * @brief     Disable USART Receiver in active mode
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableMuteMode(USART_T* usart)
{
    usart->CTRL1_B.RXMUTEEN = BIT_RESET;
}

/*!
 * @brief     Selects the USART WakeUp method.
 *
 * @param     usart:  Select the USART or the UART peripheral
 *
 * @param     wakeup: Specifies the selected USART auto baud rate method
 *                    This parameter can be one of the following values:
 *                    @arg USART_WAKEUP_IDLE_LINE    : WakeUp by an idle line detection
 *                    @arg USART_WAKEUP_ADDRESS_MARK : WakeUp by an address mark
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ConfigWakeUp(USART_T* usart, USART_WAKEUP_T wakeup)
{
    usart->CTRL1_B.WUPMCFG = wakeup;
}

/*!
 * @brief     Sets the USART LIN Break detection length
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @param     length:  Specifies the LIN break detection length
 *                     This parameter can be one of the following values:
 *                     @arg USART_LBDL_10B : 10-bit break detection
 *                     @arg USART_LBDL_11B : 11-bit break detection
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ConfigLINBreakDetectLength(USART_T* usart, USART_LBDL_T length)
{
    usart->CTRL2_B.LBDLCFG = length;
}

/*!
 * @brief     Enables the USART LIN MODE
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableLIN(USART_T* usart)
{
    usart->CTRL2_B.LINMEN = BIT_SET;
}

/*!
 * @brief     Disable the USART LIN MODE
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableLIN(USART_T* usart)
{
    usart->CTRL2_B.LINMEN = BIT_RESET;
}

/*!
 * @brief     Transmits break characters
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_TxBreak(USART_T* usart)
{
    usart->CTRL1_B.TXBF = BIT_SET;
}

/*!
 * @brief     Enables USART Half Duplex communication
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableHalfDuplex(USART_T* usart)
{
    usart->CTRL3_B.HDEN = BIT_SET;
}

/*!
 * @brief     Disable USART Half Duplex communication
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableHalfDuplex(USART_T* usart)
{
    usart->CTRL3_B.HDEN = BIT_RESET;
}

/*!
 * @brief     Sets the specified USART guard time
 *
 * @param     usart:     Select the USART or the UART peripheral
 *
 * @param     guardTime: Specifies the guard time
 *
 * @retval    None
 *
 * @note      The specified USART guard time is not available for UART4 and UART5
 */
void USART_ConfigGuardTime(USART_T* usart, uint8_t guardTime)
{
    usart->GTPSC_B.GRDT = guardTime;
}

/*!
 * @brief     Enables the USART Smart Card mode
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The Smart Card mode is not available for UART4 and UART5
 */
void USART_EnableSmartCard(USART_T* usart)
{
    usart->CTRL3_B.SCEN = BIT_SET;
}

/*!
 * @brief     Disable the USART Smart Card mode
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The Smart Card mode is not available for UART4 and UART5
 */
void USART_DisableSmartCard(USART_T* usart)
{
    usart->CTRL3_B.SCEN = BIT_RESET;
}

/*!
 * @brief     Enables NACK transmission
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The Smart Card mode is not available for UART4 and UART5
 */
void USART_EnableSmartCardNACK(USART_T* usart)
{
    usart->CTRL3_B.SCNACKEN = BIT_SET;
}

/*!
 * @brief     Disable NACK transmission
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The Smart Card mode is not available for UART4 and UART5
 */
void USART_DisableSmartCardNACK(USART_T* usart)
{
    usart->CTRL3_B.SCNACKEN = BIT_RESET;
}

/*!
 * @brief     Configures the USART's IrDA interface
 *
 * @param     usart:    Select the USART or the UART peripheral
 *
 * @param     IrDAMode: Specifies the IrDA mode
 *                      This parameter can be one of the following values:
 *                      @arg USART_IRDALP_NORMAL    : Normal
 *                      @arg USART_IRDALP_LOWPOWER  : Low-Power
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ConfigIrDA(USART_T* usart, USART_IRDALP_T IrDAMode)
{
    usart->CTRL3_B.IRLPEN = IrDAMode;
}

/*!
 * @brief     Enables the USART's IrDA interface
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableIrDA(USART_T* usart)
{
    usart->CTRL3_B.IREN = BIT_SET;
}

/*!
 * @brief     Disable the USART's IrDA interface
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableIrDA(USART_T* usart)
{
    usart->CTRL3_B.IREN = BIT_RESET;
}

/*!
 * @brief     Enables the USART DMA interface
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @param     dmaReq:  Specifies the DMA request
 *                     This parameter can be one of the following values:
 *                     @arg USART_DMA_TX    : USART DMA receive request
 *                     @arg USART_DMA_RX    : USART DMA transmit request
 *                     @arg USART_DMA_TX_RX : USART DMA transmit/receive request
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableDMA(USART_T* usart, USART_DMA_T dmaReq)
{
    usart->CTRL3_B.DMARXEN = dmaReq & 0x01;
    usart->CTRL3_B.DMATXEN = dmaReq >> 1;
}

/*!
 * @brief     Disable the USART DMA interface
 *
 * @param     usart:   Select the USART or the UART peripheral
 *
 * @param     dmaReq:  Specifies the DMA request
 *                     This parameter can be one of the following values:
 *                     @arg USART_DMA_TX    : USART DMA receive request
 *                     @arg USART_DMA_RX    : USART DMA transmit request
 *                     @arg USART_DMA_TX_RX : USART DMA transmit/receive request
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableDMA(USART_T* usart, USART_DMA_T dmaReq)
{
    usart->CTRL3_B.DMARXEN = !(dmaReq & 0x01);
    usart->CTRL3_B.DMATXEN = !(dmaReq >> 1);
}

/*!
 * @brief     Enable the specified USART interrupts
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     interrupt: Specifies the USART interrupts sources
 *                       The parameter can be one of following values:
 *                       @arg USART_INT_PE      : Parity error interrupt
 *                       @arg USART_INT_TXBE    : Tansmit data buffer empty interrupt
 *                       @arg USART_INT_TXC     : Transmission complete interrupt
 *                       @arg USART_INT_RXBNE   : Receive data buffer not empty interrupt
 *                       @arg USART_INT_IDLE    : Idle line detection interrupt
 *                       @arg USART_INT_LBD     : LIN break detection interrupt
 *                       @arg USART_INT_CTS     : CTS change interrupt
 *                       @arg USART_INT_ERR     : Error interrupt(Frame error, noise error, overrun error)
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_EnableInterrupt(USART_T* usart, USART_INT_T interrupt)
{
    uint32_t temp;

    temp = (uint32_t)(interrupt & 0xffff);

    if (interrupt & 0x10000)
    {
        usart->CTRL1 |= temp;
    }

    if (interrupt & 0x20000)
    {
        usart->CTRL2 |= temp;
    }

    if (interrupt & 0x40000)
    {
        usart->CTRL3 |= temp;
    }
}

/*!
 * @brief     Disables the specified USART interrupts
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     interrupt: Specifies the USART interrupts sources
 *                       The parameter can be one of following values:
 *                       @arg USART_INT_PE      : Parity error interrupt
 *                       @arg USART_INT_TXBE    : Tansmit data buffer empty interrupt
 *                       @arg USART_INT_TXC     : Transmission complete interrupt
 *                       @arg USART_INT_RXBNE   : Receive data buffer not empty interrupt
 *                       @arg USART_INT_IDLE    : Idle line detection interrupt
 *                       @arg USART_INT_LBD     : LIN break detection interrupt
 *                       @arg USART_INT_CTS     : CTS change interrupt
 *                       @arg USART_INT_ERR     : Error interrupt(Frame error, noise error, overrun error)
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_DisableInterrupt(USART_T* usart, USART_INT_T interrupt)
{
    uint32_t temp;

    temp = (uint32_t)~(interrupt & 0xffff);

    if (interrupt & 0x10000)
    {
        usart->CTRL1 &= temp;
    }

    if (interrupt & 0x20000)
    {
        usart->CTRL2 &= temp;
    }

    if (interrupt & 0x40000)
    {
        usart->CTRL3 &= temp;
    }
}

/*!
 * @brief     Read the specified USART flag
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     flag: Specifies the flag to check
 *                  The parameter can be one of following values:
 *                  @arg USART_FLAG_CTS     : CTS Change flag (not available for UART4 and UART5)
 *                  @arg USART_FLAG_LBD     : LIN Break detection flag
 *                  @arg USART_FLAG_TXBE    : Transmit data buffer empty flag
 *                  @arg USART_FLAG_TXC     : Transmission Complete flag
 *                  @arg USART_FLAG_RXBNE   : Receive data buffer not empty flag
 *                  @arg USART_FLAG_IDLE    : Idle Line detection flag
 *                  @arg USART_FLAG_OVRE    : OverRun Error flag
 *                  @arg USART_FLAG_NE      : Noise Error flag
 *                  @arg USART_FLAG_FE      : Framing Error flag
 *                  @arg USART_FLAG_PE      : Parity Error flag
 *
 * @retval    The new state of flag (SET or RESET)
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
uint8_t USART_ReadStatusFlag(USART_T* usart, USART_FLAG_T flag)
{
    return (usart->STS & flag) ? SET : RESET;
}

/*!
 * @brief     Clears the USARTx's pending flags
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     flag: Specifies the flag to clear
 *                  The parameter can be one of following values:
 *                  @arg USART_FLAG_CTS     : CTS Change flag (not available for UART4 and UART5)
 *                  @arg USART_FLAG_LBD     : LIN Break detection flag
 *                  @arg USART_FLAG_TXC     : Transmission Complete flag
 *                  @arg USART_FLAG_RXBNE   : Receive data buffer not empty flag
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ClearStatusFlag(USART_T* usart, USART_FLAG_T flag)
{
    usart->STS &= (uint32_t)~flag;
}

/*!
 * @brief     Read the specified USART interrupt flag
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     flag:  Specifies the USART interrupt source to check
 *                        The parameter can be one of following values:
 *                        @arg USART_INT_TXBE    : Tansmit data buffer empty interrupt
 *                        @arg USART_INT_TXC     : Transmission complete interrupt
 *                        @arg USART_INT_RXBNE   : Receive data buffer not empty interrupt
 *                        @arg USART_INT_IDLE    : Idle line detection interrupt
 *                        @arg USART_INT_LBD     : LIN break detection interrupt
 *                        @arg USART_INT_CTS     : CTS change interrupt
 *                        @arg USART_INT_OVRE_RX : OverRun Error interruptpt if the RXBNFLG bit is set
 *                        @arg USART_INT_OVRE_ER : OverRun Error interruptpt if the EIE bit is set
 *                        @arg USART_INT_NE      : Noise Error interrupt
 *                        @arg USART_INT_FE      : Framing Error interrupt
 *                        @arg USART_INT_PE      : Parity error interrupt
 *
 * @retval    The new state of flag (SET or RESET)
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
uint8_t USART_ReadIntFlag(USART_T* usart, USART_INT_T flag)
{
    uint32_t itFlag, srFlag;

    if (flag & 0x10000)
    {
        itFlag = usart->CTRL1 & flag & 0xffff;
    }
    else if (flag & 0x20000)
    {
        itFlag = usart->CTRL2 & flag & 0xffff;
    }
    else
    {
        itFlag = usart->CTRL3 & flag & 0xffff;
    }

    srFlag = flag >> 24;
    srFlag = (uint32_t)(1 << srFlag);
    srFlag = usart->STS & srFlag;

    if (srFlag && itFlag)
    {
        return SET;
    }

    return RESET;
}

/*!
 * @brief     Clears the USART interrupt pending bits
 *
 * @param     usart: Select the USART or the UART peripheral
 *
 * @param     flag: Specifies the interrupt pending bit to clear
 *                  The parameter can be one of following values:
 *                  @arg USART_INT_RXBNE : Receive data buffer not empty interrupt
 *                  @arg USART_INT_TXC   : Transmission complete interrupt
 *                  @arg USART_INT_LBD   : LIN break detection interrupt
 *                  @arg USART_INT_CTS   : CTS change interrupt
 *
 * @retval    None
 *
 * @note      The usart can be USART1, USART2, USART3, UART4, UART5, USART6, UART7 and UART8
 */
void USART_ClearIntFlag(USART_T* usart, USART_INT_T flag)
{
    uint32_t srFlag;

    srFlag = flag >> 24;
    srFlag = (uint32_t)(1 << srFlag);

    usart->STS &= (uint32_t)~srFlag;
}

/**@} end of group USART_Functions */
/**@} end of group USART_Driver */
/**@} end of group APM32F4xx_StdPeriphDriver */
