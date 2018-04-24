/**
 * @file xmc_i2s.c
 * @date 2015-06-30
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
 * 2015-08-21:
 *     - Initial <br>
 *
 * 2015-09-01:
 *     - Modified XMC_I2S_CH_EnableEvent() and XMC_I2S_CH_DisableEvent() for supporting multiple events configuration<br>
 *
 * 2015-09-14:
 *     - Modified XMC_I2S_CH_SetSystemWordLength for supporting up to 63 system word length.
 *     - Removed parity configuration<br>
 *
 * 2015-09-28:
 *     - Fixed bugs in the XMC_I2S_CH_Init() and in the ASSERTs <br>
 *
 * 2015-11-04: 
 *     - Modified the check of XMC_USIC_CH_GetTransmitBufferStatus() in the XMC_I2S_CH_Transmit() API <br>
 *
 * 2016-06-30:
 *     - Modified XMC_I2S_CH_Init:
 *       + change default passive level to 0
 *       + Call XMC_I2S_CH_SetSystemWordLength() to set the system frame length equal to the frame length.
 *     - Modified XMC_I2S_CH_SetBaudrate:
 *       + Optional Master clock output signal generated with a fixed phase relation to SCLK.
 *
 * @endcond
 *
 */
/**
 *
 * @brief I2S driver for XMC microcontroller family
 *
 */
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/

#include <xmc_scu.h>
#include <xmc_i2s.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* To take into account the SCLK divider by 2 and the PPPEN divider (see Divider Mode Counter figure in RM) */
#define XMC_I2S_CH_OVERSAMPLING (4UL)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* Initializes the selected I2S channel with the config structure. */
void XMC_I2S_CH_Init(XMC_USIC_CH_t *const channel, const XMC_I2S_CH_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_I2S_CH_Init: data_delayed_sclk_periods value not valid",(config->data_delayed_sclk_periods  > 0U) &&
		                                                   (config->data_delayed_sclk_periods  < config->frame_length));
  XMC_USIC_CH_Enable(channel);

  if(config->bus_mode == XMC_I2S_CH_BUS_MODE_MASTER)
  {
    /* Configure baud rate */
    (void)XMC_I2S_CH_SetBaudrate(channel, config->baudrate);
  }
  /* Configuration of USIC Shift Control */
  /* Transmission Mode (TRM) = 1  */
  channel->SCTR = (uint32_t)(0x3UL << USIC_CH_SCTR_TRM_Pos) |
                  (uint32_t)((config->frame_length -1U) << USIC_CH_SCTR_FLE_Pos) |
                  (uint32_t)((config->data_bits - 1U) << USIC_CH_SCTR_WLE_Pos) |
                  USIC_CH_SCTR_SDIR_Msk;

  /* Configuration of USIC Transmit Control/Status Register */
  /* TBUF Data Enable (TDEN) = 1 */
  /* TBUF Data Single Shot Mode (TDSSM) = 1 */
  /* WA mode enabled(WAMD) = 1 */
  channel->TCSR = (uint32_t)((channel->TCSR & (~(USIC_CH_TCSR_WLEMD_Msk |
                                                 USIC_CH_TCSR_SELMD_Msk |
                                                 USIC_CH_TCSR_FLEMD_Msk |
                                                 USIC_CH_TCSR_HPCMD_Msk))) |
				  USIC_CH_TCSR_WAMD_Msk |
                  (0x01UL  << USIC_CH_TCSR_TDEN_Pos) |
                  USIC_CH_TCSR_TDSSM_Msk);

  if(config->bus_mode == XMC_I2S_CH_BUS_MODE_MASTER)
  {
    /* Configuration of Protocol Control Register */
    channel->PCR_IISMode = (uint32_t)USIC_CH_PCR_IISMode_WAGEN_Msk;
  }

  /* Configuration of Protocol Control Register */
  channel->PCR_IISMode |= (uint32_t)(USIC_CH_PCR_IISMode_DTEN_Msk |
                          (uint32_t)config->wa_inversion) |
                          ((uint32_t)((uint32_t)config->data_delayed_sclk_periods - 1U) << USIC_CH_PCR_IISMode_TDEL_Pos);

  XMC_I2S_CH_SetSystemWordLength(channel, config->frame_length);

  /* Clear protocol status */
  channel->PSCR = 0xFFFFFFFFUL;
}


XMC_I2S_CH_STATUS_t XMC_I2S_CH_SetBaudrate(XMC_USIC_CH_t *const channel, const uint32_t rate)
{
  XMC_I2S_CH_STATUS_t status;

  status = XMC_I2S_CH_STATUS_ERROR;

  if (rate <= (XMC_SCU_CLOCK_GetPeripheralClockFrequency() >> 1U))
  {
    if (XMC_USIC_CH_SetBaudrate(channel, rate, XMC_I2S_CH_OVERSAMPLING) == XMC_USIC_CH_STATUS_OK)
    {
      channel->BRG = (uint32_t)((channel->BRG & ~(USIC_CH_BRG_CTQSEL_Msk)) |
                     (0x2UL << USIC_CH_BRG_CTQSEL_Pos)) |
                     USIC_CH_BRG_PPPEN_Msk;

      status = XMC_I2S_CH_STATUS_OK;
    }

  }
  return status;
}

void XMC_I2S_CH_SetSystemWordLength(XMC_USIC_CH_t *const channel,uint32_t sclk_cycles_system_word_length)
{
  uint32_t sclk_cycles_system_word_length_temp;
  uint8_t dctq_temp;
  uint8_t pctq_temp;
  uint8_t dctq = 1U;
  uint8_t pctq = 1U;
  uint8_t best_error = 64U;
  uint8_t error;
  XMC_ASSERT("XMC_I2S_CH_Init: data_delayed_sclk_periods value not valid",(sclk_cycles_system_word_length  > 0U) && (sclk_cycles_system_word_length  < 65U));


  for (dctq_temp =1U; dctq_temp < 33U ; dctq_temp++)
  {
    for (pctq_temp =1U; pctq_temp < 5U ; pctq_temp++)
    {
      sclk_cycles_system_word_length_temp = ((uint32_t)dctq_temp) * ((uint32_t)pctq_temp);
      if(sclk_cycles_system_word_length_temp == sclk_cycles_system_word_length)
      {
        dctq = dctq_temp;
        pctq = pctq_temp;
        break;
      }
      if (sclk_cycles_system_word_length_temp > sclk_cycles_system_word_length)
      {
        error = (uint8_t)(sclk_cycles_system_word_length_temp  - sclk_cycles_system_word_length);
      }
      else
      {
        error = (uint8_t)(sclk_cycles_system_word_length - sclk_cycles_system_word_length_temp);
      }

      if(error < best_error)
      {
         best_error = error;
         dctq =  dctq_temp;
         pctq = pctq_temp;
      }
    }
  }
  channel->BRG = (uint32_t)((channel->BRG & ~((uint32_t)(USIC_CH_BRG_DCTQ_Msk | 
                                                         USIC_CH_BRG_PCTQ_Msk))) |
                 (uint32_t)((uint32_t)((uint32_t)((uint32_t)dctq- 1U) << USIC_CH_BRG_DCTQ_Pos) |
                            (uint32_t)((uint32_t)((uint32_t)pctq- 1U) << USIC_CH_BRG_PCTQ_Pos)));
}

/* Puts the data into FIFO if FIFO mode is enabled or else into standard buffers, by setting the proper mode. */
void XMC_I2S_CH_Transmit(XMC_USIC_CH_t *const channel, const uint16_t data, const XMC_I2S_CH_CHANNEL_t channel_number)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while((uint32_t)XMC_USIC_CH_GetTransmitBufferStatus(channel) == (uint32_t)XMC_USIC_CH_TBUF_STATUS_BUSY)
    {
    }

    XMC_I2S_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2S_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[(uint32_t)channel_number << 4] = data;
  }
  else
  {
    channel->IN[(uint32_t)channel_number << 4] = data;
  }
}

/* Reads the data from the buffers based on the FIFO mode selection. */
uint16_t XMC_I2S_CH_GetReceivedData(XMC_USIC_CH_t *const channel)
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

XMC_I2S_CH_STATUS_t XMC_I2S_CH_Stop(XMC_USIC_CH_t *const channel)
{
  XMC_I2S_CH_STATUS_t status = XMC_I2S_CH_STATUS_OK;

  if (((uint32_t)XMC_USIC_CH_GetTransmitBufferStatus(channel) & (uint32_t)XMC_USIC_CH_TBUF_STATUS_BUSY) != 0U)
  {
    status = XMC_I2S_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_IDLE);
  }

  return status;
}

void XMC_I2S_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event&0x1fc00U);
  channel->PCR_IISMode |= ((event >> 2U) & 0x8070U);
}

void XMC_I2S_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event&0x1fc00U);
  channel->PCR_IISMode &= (uint32_t)~((event >> 2U) & 0x8070U);
}
