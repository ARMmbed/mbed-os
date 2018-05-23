/**
 * @file xmc_i2c.c
 * @date 2015-10-02
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
       - Modified XMC_I2C_CH_Stop() API for not setting to IDLE the channel if it is busy <br>
 *
 * 2015-06-20:
 *     - Removed GetDriverVersion API <br>
 *
 * 2015-08-14:
 *     - updated the XMC_I2C_CH_SetBaudrate API to support dynamic change from 400K to low frequencies <br>
 *
 * 2015-09-01:
 *     - Modified XMC_I2C_CH_EnableEvent() and XMC_I2C_CH_DisableEvent() for supporting multiple events configuration <br>
 *
 * 2015-10-02:
 *     - Fixed 10bit addressing
 *
 * @endcond 
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include <xmc_i2c.h>

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define XMC_I2C_7BIT_ADDR_Pos   (8U)		/**< 7-bit address position */
#define TRANSMISSION_MODE       (3U)		/**< The shift control signal is considered active
                                                 without referring to the actual signal level. Data
                                                 frame transfer is possible after each edge of the signal.*/
#define WORDLENGTH              (7U)        /**< Word length */
#define SET_TDV                 (1U)		/**< Transmission data valid */
#define XMC_I2C_10BIT_ADDR_MASK (0x7C00U)   /**< Address mask for 10-bit mode */

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

typedef enum XMC_I2C_CH_TDF
{
  XMC_I2C_CH_TDF_MASTER_SEND =         0U,
  XMC_I2C_CH_TDF_SLAVE_SEND =          (uint32_t)1U << 8U,
  XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK =  (uint32_t)2U << 8U,
  XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK = (uint32_t)3U << 8U,
  XMC_I2C_CH_TDF_MASTER_START =        (uint32_t)4U << 8U,
  XMC_I2C_CH_TDF_MASTER_RESTART =      (uint32_t)5U << 8U,
  XMC_I2C_CH_TDF_MASTER_STOP =         (uint32_t)6U << 8U
} XMC_I2C_CH_TDF_t;

typedef enum XMC_I2C_CH_MAX_SPEED
{
  XMC_I2C_CH_MAX_SPEED_STANDARD = 100000U,
  XMC_I2C_CH_MAX_SPEED_FAST = 400000U
} XMC_I2C_CH_MAX_SPEED_t;

typedef enum XMC_I2C_CH_CLOCK_OVERSAMPLING
{
  XMC_I2C_CH_CLOCK_OVERSAMPLING_STANDARD = 10U,
  XMC_I2C_CH_CLOCK_OVERSAMPLING_FAST     = 25U
} XMC_I2C_CH_CLOCK_OVERSAMPLINGS_t;

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/* Initializes the USIC channel by setting the data format, slave address, baudrate, transfer buffer */
void XMC_I2C_CH_Init(XMC_USIC_CH_t *const channel, const XMC_I2C_CH_CONFIG_t *const config)
{
  XMC_USIC_CH_Enable(channel);
  
  /* Data format configuration */
  channel->SCTR = ((uint32_t)TRANSMISSION_MODE << (uint32_t)USIC_CH_SCTR_TRM_Pos) | /* Transmision mode */
                  ((uint32_t)WORDLENGTH << (uint32_t)USIC_CH_SCTR_WLE_Pos) | /* 8 data bits */
                  USIC_CH_SCTR_FLE_Msk |           /* unlimited data flow */
                  USIC_CH_SCTR_SDIR_Msk |          /* MSB shifted first */
                  USIC_CH_SCTR_PDL_Msk;            /* Passive Data Level */

  XMC_I2C_CH_SetSlaveAddress(channel, config->address);
  (void)XMC_I2C_CH_SetBaudrate(channel, config->baudrate);
    
  /* Enable transfer buffer */
  channel->TCSR = ((uint32_t)SET_TDV << (uint32_t)USIC_CH_TCSR_TDEN_Pos) | USIC_CH_TCSR_TDSSM_Msk;

  /* Clear status flags */
  channel->PSCR = 0xFFFFFFFFU;

  /* Disable parity generation */
  channel->CCR = 0x0U;
}
/* Sets the slave address */
void XMC_I2C_CH_SetSlaveAddress(XMC_USIC_CH_t *const channel, const uint16_t address)
{
  if ((address & XMC_I2C_10BIT_ADDR_MASK) == XMC_I2C_10BIT_ADDR_GROUP)
  {
    channel->PCR_IICMode = (address & 0xffU) | ((address << 1) & 0xfe00U);
  }
  else
  {
    channel->PCR_IICMode = ((uint32_t)address) << XMC_I2C_7BIT_ADDR_Pos;
  }
}
/* Read the slave address */
uint16_t XMC_I2C_CH_GetSlaveAddress(const XMC_USIC_CH_t *const channel)
{
  uint32_t address = channel->PCR_IICMode & (uint32_t)USIC_CH_PCR_IICMode_SLAD_Msk;
  
  if ((address & 0xffU) == 0U)
  {
    address = address >> XMC_I2C_7BIT_ADDR_Pos;
  }
  else
  {
    address = (address & 0xffU) | ((address >> 1) & 0x0300U);
  }
  
  return (uint16_t)address;
}
/* Sets the baudrate and oversampling based on standard speed or fast speed */
XMC_I2C_CH_STATUS_t XMC_I2C_CH_SetBaudrate(XMC_USIC_CH_t *const channel, uint32_t rate)
{
  XMC_I2C_CH_STATUS_t status;
  
  status = XMC_I2C_CH_STATUS_ERROR;
  
  if (rate <= (uint32_t)XMC_I2C_CH_MAX_SPEED_STANDARD)
  {
		channel->PCR_IICMode &= (uint32_t)~USIC_CH_PCR_IICMode_STIM_Msk;
    if (XMC_USIC_CH_SetBaudrate(channel, rate, (uint32_t)XMC_I2C_CH_CLOCK_OVERSAMPLING_STANDARD) == XMC_USIC_CH_STATUS_OK)
    {
      status = XMC_I2C_CH_STATUS_OK;
    }
  }
  else if (rate <= (uint32_t)XMC_I2C_CH_MAX_SPEED_FAST)
  {
    channel->PCR_IICMode |= (uint32_t)USIC_CH_PCR_IICMode_STIM_Msk;
    if (XMC_USIC_CH_SetBaudrate(channel, rate, (uint32_t)XMC_I2C_CH_CLOCK_OVERSAMPLING_FAST) == XMC_USIC_CH_STATUS_OK)
    {
      status = XMC_I2C_CH_STATUS_OK;
    }
  }
  else 
  {
    status = XMC_I2C_CH_STATUS_ERROR;
  }
  
  return status;
}
/* Sends master start condition along with read/write command to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterStart(XMC_USIC_CH_t *const channel, const uint16_t addr, const XMC_I2C_CH_CMD_t command)
{
  uint32_t temp;
  
  temp = addr | (uint32_t)XMC_I2C_CH_TDF_MASTER_START;
  if (command == XMC_I2C_CH_CMD_READ)
  {
    temp |= 0x1U;
  }

  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = temp;
  }
  else
  {
    channel->IN[0U] = temp;
  }
}
/* Sends master repeated start condition along with read/write command to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterRepeatedStart(XMC_USIC_CH_t *const channel, const uint16_t addr, const XMC_I2C_CH_CMD_t command)
{
  uint32_t tmp;
  tmp = addr | (uint32_t)XMC_I2C_CH_TDF_MASTER_RESTART;
  if (command == XMC_I2C_CH_CMD_READ)
  {
    tmp |= 0x1U;
  }

  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {  
    while (XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = tmp;
  }
  else
  {
    channel->IN[0U] = tmp;
  }
}

/* Sends master stop command to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterStop(XMC_USIC_CH_t *const channel)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {
    while (XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
    {
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_STOP;
  }
  else
  {
    channel->IN[0U] = (uint32_t)XMC_I2C_CH_TDF_MASTER_STOP;
  }
}

/* Sends master send command along with data to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterTransmit(XMC_USIC_CH_t *const channel, const uint8_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {    
    while (XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_SEND | data;
  }
  else
  {
    channel->IN[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_SEND | data;
  }
}

/* Sends slave send command along with data to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_SlaveTransmit(XMC_USIC_CH_t *const channel, const uint8_t data)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {    
    while(XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }    

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)XMC_I2C_CH_TDF_SLAVE_SEND | data;
  }
  else
  {
    channel->IN[0] = (uint32_t)XMC_I2C_CH_TDF_SLAVE_SEND | data;
  }
}

/* Sends master receive ack command to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterReceiveAck(XMC_USIC_CH_t *const channel)
{
/* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {    
    while(XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK;
  }
  else
  {
    channel->IN[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_RECEIVE_ACK;
  }
}

/* Sends master receive nack command to IN/TBUF register based on FIFO/non-FIFO modes. */
void XMC_I2C_CH_MasterReceiveNack(XMC_USIC_CH_t *const channel)
{
  /* Check FIFO size */
  if ((channel->TBCTR & USIC_CH_TBCTR_SIZE_Msk) == 0U)
  {    
    while(XMC_USIC_CH_GetTransmitBufferStatus(channel) == XMC_USIC_CH_TBUF_STATUS_BUSY)
	{
      /* check TDV, wait until TBUF is ready */
    }

    /* clear PSR_TBIF */
    XMC_I2C_CH_ClearStatusFlag(channel, (uint32_t)XMC_I2C_CH_STATUS_FLAG_TRANSMIT_BUFFER_INDICATION);

    channel->TBUF[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK;
  }
  else 
  {
    channel->IN[0] = (uint32_t)XMC_I2C_CH_TDF_MASTER_RECEIVE_NACK;
  }
}

/* Reads the data from RBUF if FIFO size is 0 otherwise from OUTR. */
uint8_t XMC_I2C_CH_GetReceivedData(const XMC_USIC_CH_t *const channel)
{
  uint8_t retval;

  /* Check FIFO size */
  if ((channel->RBCTR & USIC_CH_RBCTR_SIZE_Msk) == 0U)
  {
    retval = (uint8_t)channel->RBUF;
  }
  else
  {
    retval = (uint8_t)channel->OUTR;
  }

  return retval;
}

/* Sets the operating mode of USIC to IDLE */
XMC_I2C_CH_STATUS_t XMC_I2C_CH_Stop(XMC_USIC_CH_t *const channel)
{
  XMC_I2C_CH_STATUS_t status = XMC_I2C_CH_STATUS_OK;

  if (((uint32_t)XMC_USIC_CH_GetTransmitBufferStatus(channel) & (uint32_t)XMC_USIC_CH_TBUF_STATUS_BUSY) != 0U)
  {
    status = XMC_I2C_CH_STATUS_BUSY;
  }
  else
  {
    /* USIC channel in IDLE mode */
    XMC_USIC_CH_SetMode(channel, XMC_USIC_CH_OPERATING_MODE_IDLE);
  }
  return status;
}

void XMC_I2C_CH_EnableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR |= (event&0x1fc00U);
  channel->PCR_IICMode |= ((event) & 0x41fc0000U);
}

void XMC_I2C_CH_DisableEvent(XMC_USIC_CH_t *const channel, const uint32_t event)
{
  channel->CCR &= (uint32_t)~(event&0x1fc00U);
  channel->PCR_IICMode &= (uint32_t)~((event) & 0x41fc0000U);
}
