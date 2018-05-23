/**
 * @file xmc_uart.c
 * @date 2016-07-22
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
 *     - xmc_uart_ch_stop API implementation corrected.
 *     - Modified XMC_UART_CH_Stop() API for not setting to IDLE the channel if it is busy <br>
 *
 * 2015-06-20:
 *     - Removed GetDriverVersion API <br>
 *
 * 2015-09-01:
 *     - Modified XMC_UART_CH_EnableEvent() and XMC_UART_CH_DisableEvent() for supporting multiple events configuration <br>
 * 
 * 2016-07-22:
 *     - Modified XMC_UART_CH_Init() to enable transfer status BUSY
 *     - Modified XMC_UART_CH_Stop() to check for transfer status
 *
 * @endcond 
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include <xmc_scu.h>
#include <xmc_uart.h>

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
#define XMC_UART_CH_OVERSAMPLING (16UL)
#define XMC_UART_CH_OVERSAMPLING_MIN_VAL (4UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/

void XMC_UART_CH_Init(XMC_USIC_CH_t *channel, const XMC_UART_CH_CONFIG_t *const config)
{
  uint32_t oversampling = XMC_UART_CH_OVERSAMPLING;

  /* USIC channel switched on*/
  XMC_USIC_CH_Enable(channel);
  
  if(config->oversampling != 0U)
  {
    oversampling = (uint32_t)config->oversampling;
  }
  
  /* Configure baud rate */
  (void)XMC_USIC_CH_SetBaudrate(channel, config->baudrate, oversampling);
  
  /* Configure frame format
   * Configure the number of stop bits
   * Pulse length is set to 0 to have standard UART signaling, 
   * i.e. the 0 level is signaled during the complete bit time
   * Sampling point set equal to the half of the oversampling period
   * Enable Sample Majority Decision 
   * Enable Transfer Status BUSY
   */
  channel->PCR_ASCMode = (uint32_t)(((config->stop_bits - 1UL) << USIC_CH_PCR_ASCMode_STPB_Pos) |
                                    (((oversampling >> 1UL) + 1UL) << USIC_CH_PCR_ASCMode_SP_Pos) |
                                    USIC_CH_PCR_ASCMode_SMD_Msk |
                                    USIC_CH_PCR_ASCMode_RSTEN_Msk | USIC_CH_PCR_ASCMode_TSTEN_Msk);

  /* Set passive data level, high
     Set word length. Data bits - 1
     If frame length is > 0, frame_lemgth-1; else, FLE = WLE (Data bits - 1)
     Transmission Mode: The shift control signal is considered active if it
     is at 1-level. This is the setting to be programmed to allow data transfers */
  channel->SCTR = (uint32_t)((((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_WLE_Pos) |
                             ((0x1UL << USIC_CH_SCTR_TRM_Pos) | USIC_CH_SCTR_PDL_Msk));

  if (config->frame_length != 0U)
  {
    channel->SCTR |= (uint32_t)(((uint32_t)config->frame_length - 1UL) << USIC_CH_SCTR_FLE_Pos);
  }
  else
  {
    channel->SCTR |= (uint32_t)(((uint32_t)config->data_bits - 1UL) << USIC_CH_SCTR_FLE_Pos);
  }

  /* Enable transfer buffer */
  channel->TCSR = (0x1UL << USIC_CH_TCSR_TDEN_Pos) |
                  USIC_CH_TCSR_TDSSM_Msk;

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;

  /* Set parity settings */
  channel->CCR = (uint32_t)config->parity_mode;
}

XMC_UART_CH_STATUS_t XMC_UART_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate, uint32_t oversampling)
{
  XMC_UART_CH_STATUS_t status;
  
  status = XMC_UART_CH_STATUS_ERROR;
  
  if ((rate <= (XMC_SCU_CLOCK_GetPeripheralClockFrequency() >> 2U)) && (oversampling >= XMC_UART_CH_OVERSAMPLING_MIN_VAL))
  {
    if (XMC_USIC_CH_SetBaudrate(channel, rate, oversampling) == XMC_USIC_CH_STATUS_OK)
    {
      status = XMC_UART_CH_STATUS_OK;
    }
  } 
  return status;
}

void XMC_UART_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0UL)
  {
    /* Wait till the Transmit Buffer is free for transmission */
    while(XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
    {
    }
  
    /* Clear the Transmit Buffer indication flag */
    XMC_UART_CH_ClearStatusFlag(channel, (uint32_t)XMC_UART_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);
  
    /*Transmit data */
    channel->TBUF[0U] = data;
  }
  else
  {
    channel->IN[0U] = data;
  }
}

uint16_t XMC_UART_CH_GetReceivedData(XMC_USIC_CH_t *const channel)
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

XMC_UART_CH_STATUS_t XMC_UART_CH_Stop(XMC_USIC_CH_t *const channel)
{
  XMC_UART_CH_STATUS_t status = XMC_UART_CH_STATUS_OK;

  if (((XMC_USIC_CH_GetTransmitBufferStatus(channel) & (uint32_t) XMC_USIC_CH_TBUF_STATUS_BUSY) != 0U) ||
      ((XMC_UART_CH_GetStatusFlag(channel) & XMC_UART_CH_STATUS_FLAG_TRANSFER_STATUS_BUSY) != 0))
  {
    status = XMC_UART_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_IDLE);
  }
  return status;
}

void XMC_UART_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event&0x1fc00U);
  channel->PCR_ASCMode |= (event&0xf8U);
}

void XMC_UART_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event&0x1fc00U);
  channel->PCR_ASCMode &= (uint32_t)~(event&0xf8U);
}
