/*
 *****************************************************************************
 * @file:    adi_spi_data.c
 * @brief:   Data declaration for SPORT Device Driver 
 *****************************************************************************

Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef _ADI_SPI_DATA_C_
#define _ADI_SPI_DATA_C_

 /*! \cond PRIVATE */

#include "adi_spi_def.h"
#include "adi_spi_config.h"
#include <drivers/dma/adi_dma.h>

/* Stores the information about the specific device */
static  ADI_SPI_DEVICE_INFO spi_device_info [ADI_SPI_NUM_INSTANCES]=
{
    {
             DMA0_CH4_DONE_IRQn,
             SPI0_TX_CHANn,
	     DMA0_CH5_DONE_IRQn,
             SPI0_RX_CHANn,
	     (volatile ADI_SPI_TypeDef *)pADI_SPI0,
	     SPI0_EVT_IRQn,
	     NULL
    },
    {
             DMA0_CH6_DONE_IRQn,
             SPI1_TX_CHANn,
	     DMA0_CH7_DONE_IRQn,
             SPI1_RX_CHANn,
	     (volatile ADI_SPI_TypeDef *)pADI_SPI1,
	     SPI1_EVT_IRQn,
	     NULL
    },

    {
             DMA0_CH0_DONE_IRQn,
             SPI2_TX_CHANn,
   	     DMA0_CH1_DONE_IRQn,
             SPI2_RX_CHANn,
   	     (volatile ADI_SPI_TypeDef *)pADI_SPI2,
	     SPI2_EVT_IRQn,
	     NULL
    }
};

/* SPI Application configuration array */
static const ADI_SPI_CFG_TYPE gSPICfg[ADI_SPI_NUM_INSTANCES] =
{
  /* Initialize SPI0 Instance configuration. */
  {
       /**** SPI_CFG register configuration *** */
       (( ADI_SPI0_CFG_ENABLE               << BITP_SPI_CTL_SPIEN         ) |
        ( ADI_SPI0_CFG_CLK_PHASE            << BITP_SPI_CTL_CPHA          ) |
        ( ADI_SPI0_CFG_CLK_POLARITY         << BITP_SPI_CTL_CPOL          ) |
        ( ADI_SPI0_CFG_WIRED_OR             << BITP_SPI_CTL_WOM           ) |
        ( ADI_SPI0_CFG_LSB_MSB              << BITP_SPI_CTL_LSB           ) |
        ( ADI_SPI0_CFG_TRANSFER_INITIATE    << BITP_SPI_CTL_TIM           ) |
        ( ADI_SPI0_CFG_TX_UNDERFLOW         << BITP_SPI_CTL_ZEN           ) |
        ( ADI_SPI0_CFG_RX_OVERFLOW          << BITP_SPI_CTL_RXOF          ) |
        ( ADI_SPI0_CFG_MISO_ENABLE          << BITP_SPI_CTL_OEN           ) |
        ( ADI_SPI0_CFG_LOOPBACK             << BITP_SPI_CTL_LOOPBACK      ) |
        ( ADI_SPI0_CFG_CONTINUOUS           << BITP_SPI_CTL_CON           ) |
        ( ADI_SPI0_CFG_RX_FLUSH             << BITP_SPI_CTL_RFLUSH        ) |
        ( ADI_SPI0_CFG_TX_FLUSH             << BITP_SPI_CTL_TFLUSH        ) |
        ( ADI_SPI0_CFG_CSERR_RESET          << BITP_SPI_CTL_CSRST         )),

       /**** SPI_DIV buad rate selection register *** */
       (((((ADI_CFG_SYSTEM_CLOCK_HZ / (ADI_SPI0_CFG_BIT_RATE)) >>1u)-1u))\
                                        << BITP_SPI_DIV_VALUE           ) 
    },
  /* Initialize SPI1 Instance configuration. */
  {
       /**** SPI_CFG register configuration *** */
       (( ADI_SPI1_CFG_ENABLE               << BITP_SPI_CTL_SPIEN            ) |
        ( ADI_SPI1_CFG_CLK_PHASE            << BITP_SPI_CTL_CPHA          ) |
        ( ADI_SPI1_CFG_CLK_POLARITY         << BITP_SPI_CTL_CPOL          ) |
        ( ADI_SPI1_CFG_WIRED_OR             << BITP_SPI_CTL_WOM           ) |
        ( ADI_SPI1_CFG_LSB_MSB              << BITP_SPI_CTL_LSB           ) |
        ( ADI_SPI1_CFG_TRANSFER_INITIATE    << BITP_SPI_CTL_TIM           ) |
        ( ADI_SPI1_CFG_TX_UNDERFLOW         << BITP_SPI_CTL_ZEN           ) |
        ( ADI_SPI1_CFG_RX_OVERFLOW          << BITP_SPI_CTL_RXOF          ) |
        ( ADI_SPI1_CFG_MISO_ENABLE          << BITP_SPI_CTL_OEN           ) |
        ( ADI_SPI1_CFG_LOOPBACK             << BITP_SPI_CTL_LOOPBACK      ) |
        ( ADI_SPI1_CFG_CONTINUOUS           << BITP_SPI_CTL_CON           ) |
        ( ADI_SPI1_CFG_RX_FLUSH             << BITP_SPI_CTL_RFLUSH        ) |
        ( ADI_SPI1_CFG_TX_FLUSH             << BITP_SPI_CTL_TFLUSH        ) |
        ( ADI_SPI1_CFG_CSERR_RESET          << BITP_SPI_CTL_CSRST         )),

       /**** SPI_DIV buad rate selection register *** */
       (((((ADI_CFG_SYSTEM_CLOCK_HZ / (ADI_SPI1_CFG_BIT_RATE)) >>1u)-1u))\
                                        << BITP_SPI_DIV_VALUE           ) 
    },
  /* Initialize SPI2 Instance configuration. */
  {
       /**** SPI_CFG register configuration *** */
       (( ADI_SPI2_CFG_ENABLE               << BITP_SPI_CTL_SPIEN            ) |
        ( ADI_SPI2_CFG_CLK_PHASE            << BITP_SPI_CTL_CPHA          ) |
        ( ADI_SPI2_CFG_CLK_POLARITY         << BITP_SPI_CTL_CPOL          ) |
        ( ADI_SPI2_CFG_WIRED_OR             << BITP_SPI_CTL_WOM           ) |
        ( ADI_SPI2_CFG_LSB_MSB              << BITP_SPI_CTL_LSB           ) |
        ( ADI_SPI2_CFG_TRANSFER_INITIATE    << BITP_SPI_CTL_TIM           ) |
        ( ADI_SPI2_CFG_TX_UNDERFLOW         << BITP_SPI_CTL_ZEN           ) |
        ( ADI_SPI2_CFG_RX_OVERFLOW          << BITP_SPI_CTL_RXOF          ) |
        ( ADI_SPI2_CFG_MISO_ENABLE          << BITP_SPI_CTL_OEN           ) |
        ( ADI_SPI2_CFG_LOOPBACK             << BITP_SPI_CTL_LOOPBACK      ) |
        ( ADI_SPI2_CFG_CONTINUOUS           << BITP_SPI_CTL_CON           ) |
        ( ADI_SPI2_CFG_RX_FLUSH             << BITP_SPI_CTL_RFLUSH        ) |
        ( ADI_SPI2_CFG_TX_FLUSH             << BITP_SPI_CTL_TFLUSH        ) |
        ( ADI_SPI2_CFG_CSERR_RESET          << BITP_SPI_CTL_CSRST         )),

       /**** SPI_DIV buad rate selection register *** */
       (((((ADI_CFG_SYSTEM_CLOCK_HZ / (ADI_SPI2_CFG_BIT_RATE)) >>1u)-1u))\
                                        << BITP_SPI_DIV_VALUE           ) 
    }
};

/*! \endcond */

#endif /* _ADI_SPI_DATA_C_ */
