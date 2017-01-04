/*
 *****************************************************************************
 * @file:    adi_sport_data_v1.c
 * @brief:   Data declaration for SPORT Device Driver
 * @version: $Revision: 33290 $
 * @date:    $Date: 2016-01-19 14:11:48 +0000 (Tue, 19 Jan 2016) $
 *****************************************************************************

Copyright (c) 2010-2014 Analog Devices, Inc.

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

#ifndef _ADI_SPORT_DATA_V1_C_
/*! \cond PRIVATE */
#define _ADI_SPORT_DATA_V1_C_

#include "adi_sport_def_v1.h"
#define ADI_SPORT_NUM_INSTANCES         1u

/* Stores the information about the specific device */
#ifdef __ICCARM__
/*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for MMR address casts.
*/
#pragma diag_suppress=Pm140
#endif /* __ICCARM__ */
static  ADI_SPORT_DEVICE_INFO sport_device_info [ADI_SPORT_NUM_INSTANCES][2]=
{
    {
        {ADI_SPORT_STATE_UNINITIALIZED,ADI_DMA_CHANNEL_SPORT0_A,(volatile ADI_SPORT_TypeDef *)REG_SPORT0_CTL_A,SPORT_A_EVT_IRQn, SPORT0A_Int_Handler,NULL},
        {ADI_SPORT_STATE_UNINITIALIZED,ADI_DMA_CHANNEL_SPORT0_B,(volatile ADI_SPORT_TypeDef *)REG_SPORT0_CTL_B,SPORT_B_EVT_IRQn, SPORT0B_Int_Handler,NULL},
    },
};
#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif /* __ICCARM__ */


#if (ADI_SPORT_CFG_ENABLE_STATIC_CONFIG_SUPPORT == 1)
static const ADI_SPORT_CFG_TYPE gSPORTfg[ADI_SPORT_NUM_INSTANCES][2] =
{
  {
     { ADI_SPORT0A_ENABLE_CKMUXSEL     << BITP_SPORT_CTL_A_CKMUXSEL   |
       ADI_SPORT0A_ENABLE_FSMUXSEL     << BITP_SPORT_CTL_A_FSMUXSEL   |
       ADI_SPORT0A_LSB_FIRST           << BITP_SPORT_CTL_A_LSBF       |
       (ADI_SPORT0A_SERIAL_WLEN-1)     << BITP_SPORT_CTL_A_SLEN       |
       ADI_SPORT0A_INTERNAL_CLK        << BITP_SPORT_CTL_A_ICLK       |
       ADI_SPORT0A_OPERATION_MODE      << BITP_SPORT_CTL_A_OPMODE     |
       ADI_SPORT0A_CLOCK_EDGE          << BITP_SPORT_CTL_A_CKRE       |
       ADI_SPORT0A_FS_REQUIRED         << BITP_SPORT_CTL_A_FSR        |
       ADI_SPORT0A_INTERNAL_FS         << BITP_SPORT_CTL_A_IFS        |
       ADI_SPORT0A_DATA_INDEPENDENT_FS << BITP_SPORT_CTL_A_DIFS       |
       ADI_SPORT0A_ACTIVE_LOW_FS       << BITP_SPORT_CTL_A_LFS        |
       ADI_SPORT0A_LATE_FS             << BITP_SPORT_CTL_A_LAFS       |
       ADI_SPORT0A_ENABLE_PACKING      << BITP_SPORT_CTL_A_PACK       |
       ADI_SPORT0A_FS_ERROR_OPERATION  << BITP_SPORT_CTL_A_FSERRMODE  |
       ADI_SPORT0A_GATED_CLOCK         << BITP_SPORT_CTL_A_GCLKEN,


       ADI_SPORT0A_CLOCK_DIVISOR      << BITP_SPORT_DIV_A_CLKDIV |
       ADI_SPORT0A_FS_DIVISOR         << BITP_SPORT_DIV_A_FSDIV,

       ADI_SPORT0A_CONVT_FS_DURATION   << BITP_SPORT_CNVT_A_CNVT2FS |
       ADI_SPORT0A_CONVT_POLARITY      << BITP_SPORT_CNVT_A_POL     |
       ADI_SPORT0A_CONVT_WIDTH         << BITP_SPORT_CNVT_A_WID,
       ADI_SPORTA_CFG_WORD_WIDTH,
       ADI_SPORTA_CFG_WORD_INC
     },

     { 
       ADI_SPORT0B_LSB_FIRST           << BITP_SPORT_CTL_A_LSBF       |
       (ADI_SPORT0B_SERIAL_WLEN-1)     << BITP_SPORT_CTL_A_SLEN       |
       ADI_SPORT0B_INTERNAL_CLK        << BITP_SPORT_CTL_A_ICLK       |
       ADI_SPORT0B_OPERATION_MODE      << BITP_SPORT_CTL_A_OPMODE     |
       ADI_SPORT0B_CLOCK_EDGE          << BITP_SPORT_CTL_A_CKRE       |
       ADI_SPORT0B_FS_REQUIRED         << BITP_SPORT_CTL_A_FSR        |
       ADI_SPORT0B_INTERNAL_FS         << BITP_SPORT_CTL_A_IFS        |
       ADI_SPORT0B_DATA_INDEPENDENT_FS << BITP_SPORT_CTL_A_DIFS       |
       ADI_SPORT0B_ACTIVE_LOW_FS       << BITP_SPORT_CTL_A_LFS        |
       ADI_SPORT0B_LATE_FS             << BITP_SPORT_CTL_A_LAFS       |
       ADI_SPORT0B_ENABLE_PACKING      << BITP_SPORT_CTL_A_PACK       |
       ADI_SPORT0B_FS_ERROR_OPERATION  << BITP_SPORT_CTL_A_FSERRMODE  |
       ADI_SPORT0B_GATED_CLOCK         << BITP_SPORT_CTL_A_GCLKEN,


       ADI_SPORT0B_CLOCK_DIVISOR     << BITP_SPORT_DIV_A_CLKDIV      |
       ADI_SPORT0B_FS_DIVISOR        <<BITP_SPORT_DIV_A_FSDIV,

       ADI_SPORT0B_CONVT_FS_DURATION  << BITP_SPORT_CNVT_A_CNVT2FS |
       ADI_SPORT0B_CONVT_POLARITY     << BITP_SPORT_CNVT_A_POL      |
       ADI_SPORT0B_CONVT_WIDTH        << BITP_SPORT_CNVT_A_WID,
       ADI_SPORTB_CFG_WORD_WIDTH,
       ADI_SPORTB_CFG_WORD_INC
       
     }

  }
};
#endif

/*! \endcond */

#endif /* _ADI_SPORT_DATA_V1_C_ */
