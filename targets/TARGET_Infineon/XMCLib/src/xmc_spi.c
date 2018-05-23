/**
 * @file xmc_spi.c
 * @date 2015-11-04
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *      
 * 2015-05-20:
 *     - Modified XMC_SPI_CH_Stop() API for not setting to IDLE the channel if it is busy
 *     - Modified XMC_SPI_CH_SetInterwordDelay() implementation in order to gain accuracy <br>
 *     
 * 2015-06-20:
 *     - Removed GetDriverVersion API <br>
 *
 * 2015-09-01:
 *     - Modified XMC_SPI_CH_EnableEvent() and XMC_SPI_CH_DisableEvent() for supporting multiple events configuration <br>
 *
 * 2015-11-04: 
 *     - Modified the check of XMC_USIC_CH_GetTransmitBufferStatus() in the XMC_SPI_CH_Transmit() flag <br>
 * @endcond 
 *
 */
/**
 *
 * @brief SPI driver for XMC microcontroller family
 *
 */
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include <xmc_scu.h>
#include <xmc_spi.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_SPI_CH_OVERSAMPLING (2UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/
 
/* Initializes the selected SPI channel with the config structure. */
void XMC_SPI_CH_Init(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_CONFIG_t *const config)
{
  XMC_USIC_CH_Enable(channel);
  
  if(config->bus_mode == XMC_SPI_CH_BUS_MODE_MASTER)
  {
    /* Configure baud rate */
    (void)XMC_USIC_CH_SetBaudrate(channel, config->baudrate, XMC_SPI_CH_OVERSAMPLING); 
  }
  
  /* Configuration of USIC Shift Control */
  /* Transmission Mode (TRM) = 1  */
  /* Passive Data Level (PDL) = 1 */
  channel->SCTR = USIC_CH_SCTR_PDL_Msk |
                  (0x1UL << USIC_CH_SCTR_TRM_Pos) |
                  (0x3fUL << USIC_CH_SCTR_FLE_Pos)|
                  (0x7UL << USIC_CH_SCTR_WLE_Pos);

  /* Configuration of USIC Transmit Control/Status Register */
  /* TBUF Data Enable (TDEN) = 1 */
  /* TBUF Data Single Shot Mode (TDSSM) = 1 */
  channel->TCSR = (uint32_t)(USIC_CH_TCSR_HPCMD_Msk |
                  (0x01UL  << USIC_CH_TCSR_TDEN_Pos) |
                  USIC_CH_TCSR_TDSSM_Msk);

  if(config->bus_mode == XMC_SPI_CH_BUS_MODE_MASTER)
  {
    /* Configuration of Protocol Control Register */
    channel->PCR_SSCMode = (uint32_t)(USIC_CH_PCR_SSCMode_MSLSEN_Msk |
                           USIC_CH_PCR_SSCMode_SELCTR_Msk |
                           (uint32_t)config->selo_inversion |
                           USIC_CH_PCR_SSCMode_FEM_Msk);
  }

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;

  /* Set parity settings */
  channel->CCR = (uint32_t)config->parity_mode;
}

XMC_SPI_CH_STATUS_t XMC_SPI_CH_SetBaudrate(XMC_USIC_CH_t *const channel, const uint32_t rate)
{
  XMC_SPI_CH_STATUS_t status;
  
  status = XMC_SPI_CH_STATUS_ERROR;
  
  if (rate <= (XMC_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (XMC_USIC_CH_SetBaudrate(channel, rate, XMC_SPI_CH_OVERSAMPLING) == XMC_USIC_CH_STATUS_OK)
    {
      status = XMC_SPI_CH_STATUS_OK;
    }
  } 
  return status;
}

/* Enable the selected slave signal by setting (SELO) bits in PCR register. */
void XMC_SPI_CH_EnableSlaveSelect(XMC_USIC_CH_t *const channel, const XMC_SPI_CH_SLAVE_SELECT_t slave)
{
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_SELO_Msk;
  channel->PCR_SSCMode |= (uint32_t)slave;
}

/* Disable the slave signals by clearing (SELO) bits in PCR register. */
void XMC_SPI_CH_DisableSlaveSelect(XMC_USIC_CH_t *const channel)
{
  XMC_SPI_CH_ClearStatusFlag(channel, (uint32_t)XMC_SPI_CH_STATUS_FLAG_MSLS);
  
  /* Configuration of Protocol Control Register */
  channel->PCR_SSCMode &= (uint32_t)~USIC_CH_PCR_SSCMode_SELO_Msk;
}

/* Puts the data into FIFO if FIFO mode is enabled or else into standard buffers, by setting the proper mode. */
void XMC_SPI_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data, const XMC_SPI_CH_MODE_t mode)
{

  channel->CCR = (channel->CCR & (uint32_t)(~USIC_CH_CCR_HPCEN_Msk)) |
                 (((uint32_t) mode << USIC_CH_CCR_HPCEN_Pos) & (uint32_t)USIC_CH_CCR_HPCEN_Msk);


  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while((uint32_t)XMC_USIC_CH_GetTransmitBufferStatus(channel) == (uint32_t)XMC_USIC_CH_TBUF_STATUS_BUSY)
    {
    }
  
    XMC_SPI_CH_ClearStatusFlag(channel, (uint32_t)XMC_SPI_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);
  
    channel->TBUF[mode] = data;
  }
  else
  {
    channel->IN[mode] = data;
  }
}

/* Reads the data from the buffers based on the FIFO mode selection. */
uint16_t XMC_SPI_CH_GetReceivedData(XMC_USIC_CH_t *const channel)
{
  uint16_t retval;

  /* Check FIFO size */
  if ((channel->RBCTR & USIC_CH_RBCTR_SIZE_Msk) == 0U)
  {
    retval = (uint16_t)channel->RBUF;
  }
  else
  {
    retval = (uint16_t)channel->OUTR;
  }

  return retval;
}

/* Configures the inter word delay by setting PCR.PCTQ1, PCR.DCTQ1 bit fields. */
void XMC_SPI_CH_SetInterwordDelay(XMC_USIC_CH_t *const channel,uint32_t tinterword_delay_us)
{
  uint32_t peripheral_clock;
  uint32_t pdiv;
  uint32_t step;
  uint32_t fFD;
  uint32_t fpdiv;
  uint32_t divider_factor1 = 0U;
  uint32_t divider_factor2 = 32U;
  uint32_t divider_factor1_int = 0U;
  uint32_t divider_factor1_int_min = 4U;
  uint32_t divider_factor1_frac_min =100U;
  uint32_t divider_factor1_frac = 0U;
  uint32_t divider_factor2_temp = 0U;
  peripheral_clock = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
  pdiv = (uint32_t)(channel->BRG & USIC_CH_BRG_PDIV_Msk) >> USIC_CH_BRG_PDIV_Pos;
  step = (uint32_t)(channel->FDR & USIC_CH_FDR_STEP_Msk) >> USIC_CH_FDR_STEP_Pos;
  fFD = (uint32_t)((peripheral_clock >> 10U) * step);
  fpdiv= fFD/(1U+pdiv);

  if(tinterword_delay_us < (128000000/fpdiv))
  {
    for(divider_factor2_temp = 32U; divider_factor2_temp > 0U; --divider_factor2_temp)
    {

      divider_factor1 = (tinterword_delay_us*fpdiv)/(divider_factor2_temp*10000);
      divider_factor1_frac = divider_factor1%100U;

      if(divider_factor1_frac > 50)
      {
        divider_factor1_int =  (divider_factor1/100U)+1;
        divider_factor1_frac = (divider_factor1_int*100)-divider_factor1;
      }
      else
      {
        divider_factor1_int =  (divider_factor1/100U);
      }

      if ((divider_factor1_int < 5U) && (divider_factor1_int > 0) && (divider_factor1_frac < divider_factor1_frac_min))
      {
        divider_factor1_frac_min = divider_factor1_frac;
        divider_factor1_int_min = divider_factor1_int;
        divider_factor2= divider_factor2_temp;
      }
    }
  }

  channel->PCR_SSCMode = (uint32_t)((channel->PCR_SSCMode) & (~(USIC_CH_PCR_SSCMode_DCTQ1_Msk |
                                                                USIC_CH_PCR_SSCMode_PCTQ1_Msk |
                                                                USIC_CH_PCR_SSCMode_CTQSEL1_Msk))) |
                         (((divider_factor1_int_min - 1) << USIC_CH_PCR_SSCMode_PCTQ1_Pos) & (uint32_t)USIC_CH_PCR_SSCMode_PCTQ1_Msk) |
                         (((divider_factor2 - 1 ) << USIC_CH_PCR_SSCMode_DCTQ1_Pos) & (uint32_t)USIC_CH_PCR_SSCMode_DCTQ1_Msk);
}

XMC_SPI_CH_STATUS_t XMC_SPI_CH_Stop(XMC_USIC_CH_t *const channel)
{
  XMC_SPI_CH_STATUS_t status = XMC_SPI_CH_STATUS_OK;

  if (((uint32_t)XMC_USIC_CH_GetTransmitBufferStatus(channel) & (uint32_t)XMC_USIC_CH_TBUF_STATUS_BUSY) != 0U)
  {
    status = XMC_SPI_CH_STATUS_BUSY;
  }
  else
  {

    /* USIC channel in IDLE mode */
    XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_IDLE);
  }

  return status;
}

void XMC_SPI_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event&0x1fc00U);
  channel->PCR_SSCMode |= ((event << 13U) & 0xe000U);
}

void XMC_SPI_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event&0x1fc00U);
  channel->PCR_SSCMode &= (uint32_t)~((event << 13U) & 0xe000U);
}
