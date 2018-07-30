/**************************************************************************//**
 * @file     spi5.c
 * @version  V3.00
 * @brief    M2351 series SPI5 driver source file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "NuMicro.h"

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup SPI5_Driver SPI5 Driver
  @{
*/


/** @addtogroup SPI5_EXPORTED_FUNCTIONS SPI5 Exported Functions
  @{
*/

/**
  * @brief  This function make SPI5 module be ready to transfer.
  *         By default, the SPI5 transfer sequence is MSB first and
  *         the automatic slave select function is disabled. In
  *         Slave mode, the u32BusClock must be NULL and the SPI5 clock
  *         divider setting will be 0.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32MasterSlave decides the SPI5 module is operating in master mode or in slave mode. Valid values are:
  *              - \ref SPI5_MASTER
  *              - \ref SPI5_SLAVE
  * @param[in]  u32SPI5Mode decides the transfer timing. Valid values are:
  *              - \ref SPI5_MODE_0
  *              - \ref SPI5_MODE_1
  *              - \ref SPI5_MODE_2
  *              - \ref SPI5_MODE_3
  * @param[in]  u32DataWidth decides the data width of a SPI5 transaction.
  * @param[in]  u32BusClock is the expected frequency of SPI5 bus clock in Hz.
  * @return Actual frequency of SPI5 peripheral clock.
  */
uint32_t SPI5_Open(SPI5_T *spi,
                   uint32_t u32MasterSlave,
                   uint32_t u32SPI5Mode,
                   uint32_t u32DataWidth,
                   uint32_t u32BusClock)
{
    if(u32DataWidth == 32UL)
    {
        u32DataWidth = 0UL;
    }

    spi->CTL = u32MasterSlave | (u32DataWidth << SPI5_CTL_DWIDTH_Pos) | (u32SPI5Mode);

    return (SPI5_SetBusClock(spi, u32BusClock));
}

/**
  * @brief Reset SPI5 module.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_Close(SPI5_T *spi)
{
    if(!(__PC() & (1UL << 28UL)))
    {
        if(spi == SPI5)
        {
            /* Reset SPI5 */
            SYS->IPRST2 |= SYS_IPRST2_SPI5RST_Msk;
            SYS->IPRST2 &= ~SYS_IPRST2_SPI5RST_Msk;
        }
    }
}

/**
  * @brief Clear RX FIFO buffer.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_ClearRxFIFO(SPI5_T *spi)
{
    spi->FIFOCTL |= SPI5_FIFOCTL_RXFBCLR_Msk;
}

/**
  * @brief Clear TX FIFO buffer.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_ClearTxFIFO(SPI5_T *spi)
{
    spi->FIFOCTL |= SPI5_FIFOCTL_TXFBCLR_Msk;
}

/**
  * @brief Disable the automatic slave select function.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_DisableAutoSS(SPI5_T *spi)
{
    spi->SSCTL &= ~SPI5_SSCTL_AUTOSS_Msk;
}

/**
  * @brief Enable the automatic slave select function. Only available in Master mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32SSPinMask specifies slave select pins. (SPI5_SS)
  * @param[in]  u32ActiveLevel specifies the active level of slave select signal. Valid values are:
  *              - \ref SPI5_SS0_ACTIVE_HIGH
  *              - \ref SPI5_SS0_ACTIVE_LOW
  * @return none
  */
void SPI5_EnableAutoSS(SPI5_T *spi, uint32_t u32SSPinMask, uint32_t u32ActiveLevel)
{
    spi->SSCTL = (spi->SSCTL & ~(SPI5_SSCTL_SSACTPOL_Msk | SPI5_SSCTL_SS_Msk)) | (u32SSPinMask | u32ActiveLevel) | SPI5_SSCTL_AUTOSS_Msk;
}

/**
  * @brief Set the SPI5 bus clock. Only available in Master mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32BusClock is the expected frequency of SPI5 bus clock.
  * @return Actual frequency of SPI5 peripheral clock.
  * @note   If u32BusClock >= system clock frequency for Secure, SPI peripheral clock source will be set to APB clock and DIVIDER will be set to 0.
  * @note   If u32BusClock >= system clock frequency for Non-Secure, this function does not do anything to avoid the situation that the frequency of
  *         SPI bus clock cannot be faster than the system clock rate. User should set up carefully.
  */
uint32_t SPI5_SetBusClock(SPI5_T *spi, uint32_t u32BusClock)
{
    uint32_t u32ClkSrc, u32HCLKFreq;
    uint32_t u32Div, u32RetValue;

    /* Get system clock frequency */
    u32HCLKFreq = CLK_GetHCLKFreq();

    if(u32BusClock >= u32HCLKFreq)
    {
        if(!(__PC() & (1UL << 28UL)))
        {
            /* Select PCLK as the clock source of SPI5 */
            if(spi == SPI5)
            {
                CLK->CLKSEL2 = (CLK->CLKSEL2 & (~CLK_CLKSEL2_SPI5SEL_Msk)) | CLK_CLKSEL2_SPI5SEL_PCLK1;
            }
        }
    }

    /* Check clock source of SPI5 */
    if((spi == SPI5) || (spi == SPI5_NS))
    {
        if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_HXT)
        {
            u32ClkSrc = __HXT; /* Clock source is HXT */
        }
        else if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_PLL)
        {
            u32ClkSrc = CLK_GetPLLClockFreq(); /* Clock source is PLL */
        }
        else if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_PCLK1)
        {
            u32ClkSrc = CLK_GetPCLK1Freq(); /* Clock source is PCLK1 */
        }
        else
        {
            u32ClkSrc = __HIRC; /* Clock source is HIRC */
        }
    }

    if(u32BusClock >= u32HCLKFreq)
    {
        /* Set DIVIDER = 0 */
        spi->CLKDIV = 0UL;
        /* Return master peripheral clock rate */
        u32RetValue = u32ClkSrc;
    }
    else if(u32BusClock >= u32ClkSrc)
    {
        /* Set DIVIDER = 0 */
        spi->CLKDIV = 0UL;
        /* Return master peripheral clock rate */
        u32RetValue = u32ClkSrc;
    }
    else if(u32BusClock == 0UL)
    {
        /* Set DIVIDER to the maximum value 0xFF. f_spi = f_spi_clk_src / (DIVIDER + 1) */
        spi->CLKDIV |= SPI5_CLKDIV_DIVIDER_Msk;
        /* Return master peripheral clock rate */
        u32RetValue = (u32ClkSrc / (0xFFUL + 1UL));
    }
    else
    {
        u32Div = (((u32ClkSrc * 10UL) / u32BusClock + 5UL) / 10UL) - 1UL; /* Round to the nearest integer */
        if(u32Div > 0xFFUL)
        {
            u32Div = 0xFFUL;
            spi->CLKDIV |= SPI5_CLKDIV_DIVIDER_Msk;
            /* Return master peripheral clock rate */
            u32RetValue = (u32ClkSrc / (0xFFUL + 1UL));
        }
        else
        {
            spi->CLKDIV = (spi->CLKDIV & (~SPI5_CLKDIV_DIVIDER_Msk)) | (u32Div << SPI5_CLKDIV_DIVIDER_Pos);
            /* Return master peripheral clock rate */
            u32RetValue = (u32ClkSrc / (u32Div + 1UL));
        }
    }

    return u32RetValue;
}

/**
  * @brief Enable FIFO mode with user-specified TX FIFO threshold and RX FIFO threshold configurations.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32TxThreshold decides the TX FIFO threshold.
  * @param[in]  u32RxThreshold decides the RX FIFO threshold.
  * @return none
  */
void SPI5_EnableFIFO(SPI5_T *spi, uint32_t u32TxThreshold, uint32_t u32RxThreshold)
{
    spi->FIFOCTL = ((spi->FIFOCTL & ~(SPI5_FIFOCTL_TXTH_Msk | SPI5_FIFOCTL_RXTH_Msk)) |
                    (u32TxThreshold << SPI5_FIFOCTL_TXTH_Pos) |
                    (u32RxThreshold << SPI5_FIFOCTL_RXTH_Pos));

    spi->CTL |= SPI5_CTL_FIFOM_Msk;
}

/**
  * @brief Disable FIFO mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_DisableFIFO(SPI5_T *spi)
{
    spi->CTL &= ~SPI5_CTL_FIFOM_Msk;
}

/**
  * @brief Get the actual frequency of SPI5 bus clock. Only available in Master mode.
  * @param[in]  spi is the base address of SPI5 module.
  * @return Actual SPI5 bus clock frequency.
  */
uint32_t SPI5_GetBusClock(SPI5_T *spi)
{
    uint32_t u32Div;
    uint32_t u32ClkSrc;

    /* Get DIVIDER setting */
    u32Div = (spi->CLKDIV & SPI5_CLKDIV_DIVIDER_Msk) >> SPI5_CLKDIV_DIVIDER_Pos;

    /* Check clock source of SPI5 */
    if((spi == SPI5) || (spi == SPI5_NS))
    {
        if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_HXT)
        {
            u32ClkSrc = __HXT; /* Clock source is HXT */
        }
        else if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_PLL)
        {
            u32ClkSrc = CLK_GetPLLClockFreq(); /* Clock source is PLL */
        }
        else if((CLK_GetModuleClockSource(SPI5_MODULE) << CLK_CLKSEL2_SPI5SEL_Pos) == CLK_CLKSEL2_SPI5SEL_PCLK1)
        {
            u32ClkSrc = CLK_GetPCLK1Freq(); /* Clock source is PCLK1 */
        }
        else
        {
            u32ClkSrc = __HIRC; /* Clock source is HIRC */
        }
    }

    /* Return SPI bus clock rate */
    return (u32ClkSrc / (u32Div + 1UL));
}

/**
  * @brief Enable FIFO related interrupts specified by u32Mask parameter.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32Mask is the combination of all related interrupt enable bits.
  *         Each bit corresponds to a interrupt bit.
  *         This parameter decides which interrupts will be enabled. Valid values are:
  *           - \ref SPI5_IE_MASK
  *           - \ref SPI5_SSTAIEN_MASK
  *           - \ref SPI5_FIFO_TXTHIEN_MASK
  *           - \ref SPI5_FIFO_RXTHIEN_MASK
  *           - \ref SPI5_FIFO_RXOVIEN_MASK
  *           - \ref SPI5_FIFO_TIMEOUIEN_MASK
  * @return none
  */
void SPI5_EnableInt(SPI5_T *spi, uint32_t u32Mask)
{
    if((u32Mask & SPI5_IE_MASK) == SPI5_IE_MASK)
    {
        spi->CTL |= SPI5_CTL_UNITIEN_Msk;
    }

    if((u32Mask & SPI5_SSTAIEN_MASK) == SPI5_SSTAIEN_MASK)
    {
        spi->SSCTL |= SPI5_SSCTL_SSTAIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_TXTHIEN_MASK) == SPI5_FIFO_TXTHIEN_MASK)
    {
        spi->FIFOCTL |= SPI5_FIFOCTL_TXTHIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_RXTHIEN_MASK) == SPI5_FIFO_RXTHIEN_MASK)
    {
        spi->FIFOCTL |= SPI5_FIFOCTL_RXTHIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_RXOVIEN_MASK) == SPI5_FIFO_RXOVIEN_MASK)
    {
        spi->FIFOCTL |= SPI5_FIFOCTL_RXOVIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_TIMEOUIEN_MASK) == SPI5_FIFO_TIMEOUIEN_MASK)
    {
        spi->FIFOCTL |= SPI5_FIFOCTL_RXTOIEN_Msk;
    }
}

/**
  * @brief Disable FIFO related interrupts specified by u32Mask parameter.
  * @param[in]  spi is the base address of SPI5 module.
  * @param[in]  u32Mask is the combination of all related interrupt enable bits.
  *         Each bit corresponds to a interrupt bit.
  *         This parameter decides which interrupts will be enabled. Valid values are:
  *           - \ref SPI5_IE_MASK
  *           - \ref SPI5_SSTAIEN_MASK
  *           - \ref SPI5_FIFO_TXTHIEN_MASK
  *           - \ref SPI5_FIFO_RXTHIEN_MASK
  *           - \ref SPI5_FIFO_RXOVIEN_MASK
  *           - \ref SPI5_FIFO_TIMEOUIEN_MASK
  * @return none
  */
void SPI5_DisableInt(SPI5_T *spi, uint32_t u32Mask)
{
    if((u32Mask & SPI5_IE_MASK) == SPI5_IE_MASK)
    {
        spi->CTL &= ~SPI5_CTL_UNITIEN_Msk;
    }

    if((u32Mask & SPI5_SSTAIEN_MASK) == SPI5_SSTAIEN_MASK)
    {
        spi->SSCTL &= ~SPI5_SSCTL_SSTAIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_TXTHIEN_MASK) == SPI5_FIFO_TXTHIEN_MASK)
    {
        spi->FIFOCTL &= ~SPI5_FIFOCTL_TXTHIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_RXTHIEN_MASK) == SPI5_FIFO_RXTHIEN_MASK)
    {
        spi->FIFOCTL &= ~SPI5_FIFOCTL_RXTHIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_RXOVIEN_MASK) == SPI5_FIFO_RXOVIEN_MASK)
    {
        spi->FIFOCTL &= ~SPI5_FIFOCTL_RXOVIEN_Msk;
    }

    if((u32Mask & SPI5_FIFO_TIMEOUIEN_MASK) == SPI5_FIFO_TIMEOUIEN_MASK)
    {
        spi->FIFOCTL &= ~SPI5_FIFOCTL_RXTOIEN_Msk;
    }
}

/**
  * @brief Enable wake-up function.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_EnableWakeup(SPI5_T *spi)
{
    spi->CTL |= SPI5_CTL_WKCLKEN_Msk;
}

/**
  * @brief Disable wake-up function.
  * @param[in]  spi is the base address of SPI5 module.
  * @return none
  */
void SPI5_DisableWakeup(SPI5_T *spi)
{
    spi->CTL &= ~SPI5_CTL_WKCLKEN_Msk;
}

/*@}*/ /* end of group SPI5_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group SPI5_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
