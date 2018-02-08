/*!
 *****************************************************************************
 * @file:    adi_rtc_data.c
 * @brief:   rtc device data file
 * @version: $Revision: 34933 $
 * @date:    $Date: 2016-06-28 07:11:25 -0400 (Tue, 28 Jun 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*! \cond PRIVATE */
#ifndef ADI_RTC_DATA_C_
#define ADI_RTC_DATA_C_

#include <stdlib.h>
#include <adi_processor.h>
#include "adi_rtc_def.h"

static ADI_RTC_DEVICE_INFO aRTCDeviceInfo[ADI_RTC_NUM_INSTANCE] =
{
     {
	(ADI_RTC_TypeDef *)pADI_RTC0,RTC0_EVT_IRQn, NULL
     },
     {
	(ADI_RTC_TypeDef *)pADI_RTC1,RTC1_EVT_IRQn,NULL,
     }
};


static ADI_RTC_CONFIG aRTCConfig[ADI_RTC_NUM_INSTANCE] =
{
  {
    /* CR0 */
   RTC0_CFG_ENABLE_ALARM                    <<  BITP_RTC_CR0_ALMEN         |
   RTC0_CFG_ENABLE_ALARM_INTERRUPT          <<  BITP_RTC_CR0_ALMINTEN      |
   RTC0_CFG_ENABLE_TRIM                      <<  BITP_RTC_CR0_TRMEN         |
   RTC0_CFG_ENABLE_PENDERROR_INTERRUPT       <<  BITP_RTC_CR0_WPNDERRINTEN  |
   RTC0_CFG_ENABLE_WSYNC_INTERRUPT           <<  BITP_RTC_CR0_WSYNCINTEN    |
   RTC0_CFG_ENABLE_WRITEPEND_INTERRUPT       <<  BITP_RTC_CR0_WPNDINTEN ,
   /* CR1 */
   0,
   /* CNT0 */
   RTC0_CFG_COUNT_VALUE_0,
   /* CNT1 */
   RTC0_CFG_COUNT_VALUE_1,
   /* ALM0 */
   RTC0_CFG_ALARM_VALUE_0,
   /* ALM1 */
   RTC0_CFG_ALARM_VALUE_1,
   /* ALM2 */
   0,
   /* TRIM */
   RTC0_CFG_POW2_TRIM_INTERVAL               << BITP_RTC_TRM_IVL2EXPMIN   |
   RTC0_CFG_TRIM_INTERVAL                    << BITP_RTC_TRM_IVL          |
   RTC0_CFG_TRIM_OPERATION                   << BITP_RTC_TRM_ADD          |
   RTC0_CFG_TRIM_VALUE                       << BITP_RTC_TRM_VALUE,
   0,     /* CR2IC */
   0,     /* CR3SS */
   0,     /* CR4SS */
   0,     /* SSMSK */
   0,     /* SS1 */
   0,     /* CR5SSS */
   0,     /* CR6SSS */
   0,     /* CR7SSS */
   0,     /* GPMUX0 */
   0     /* GPMUX1 */

  },
  /* RTC-1 */
  {
    /* CR0 */
   RTC1_CFG_ENABLE_ALARM                    <<  BITP_RTC_CR0_ALMEN         |
   RTC1_CFG_ENABLE_ALARM_INTERRUPT          <<  BITP_RTC_CR0_ALMINTEN      |
   RTC1_CFG_ENABLE_TRIM                      <<  BITP_RTC_CR0_TRMEN         |
   RTC1_CFG_ENABLE_MOD60_ALARM              <<  BITP_RTC_CR0_MOD60ALMEN    |
   RTC1_CFG_ENABLE_MOD60_ALARM_PERIOD       <<  BITP_RTC_CR0_MOD60ALM      |
   RTC1_CFG_ENABLE_MOD60_ALARM_INTERRUPT    <<  BITP_RTC_CR0_MOD60ALMINTEN |
   RTC1_CFG_ENABLE_ISO_INTERRUPT             <<  BITP_RTC_CR0_ISOINTEN      |
   RTC1_CFG_ENABLE_PENDERROR_INTERRUPT       <<  BITP_RTC_CR0_WPNDERRINTEN  |
   RTC1_CFG_ENABLE_WSYNC_INTERRUPT           <<  BITP_RTC_CR0_WSYNCINTEN    |
   RTC1_CFG_ENABLE_WRITEPEND_INTERRUPT       <<  BITP_RTC_CR0_WPNDINTEN ,
    /* CR1 */
   RTC1_CFG_ENABLE_COUNT_INTERRUPT           <<  BITP_RTC_CR1_CNTINTEN       |
   RTC1_CFG_ENABLE_MOD1_COUNT_INTERRUPT      <<  BITP_RTC_CR1_PSINTEN        |
   RTC1_CFG_ENABLE_TRIM_INTERRUPT            <<  BITP_RTC_CR1_TRMINTEN    |
   RTC1_CFG_CNT_MOD60_ROLLLOVER_INTERRUPT    <<  BITP_RTC_CR1_CNTROLLINTEN   |
   RTC1_CFG_PRESCALE                         <<  BITP_RTC_CR1_PRESCALE2EXP   |
   RTC1_CFG_CNT_ROLLLOVER_INTERRUPT          <<  BITP_RTC_CR1_CNTMOD60ROLLINTEN ,
   /* CNT0 */
   RTC1_CFG_COUNT_VALUE_0,
   /* CNT1 */
   RTC1_CFG_COUNT_VALUE_1,

   /* ALM[123] */
   RTC1_CFG_ALARM_VALUE_0,
   RTC1_CFG_ALARM_VALUE_1,
   RTC1_CFG_ALARM_VALUE_2,

   /* TRIM */
   RTC1_CFG_POW2_TRIM_INTERVAL               << BITP_RTC_TRM_IVL2EXPMIN  |
   RTC1_CFG_TRIM_INTERVAL                    << BITP_RTC_TRM_IVL         |
   RTC1_CFG_TRIM_OPERATION                   << BITP_RTC_TRM_ADD         |
   RTC1_CFG_TRIM_VALUE                       << BITP_RTC_TRM_VALUE,

   /* CR2IC */
   RTC1_CFG_IC0_ENABLE                       <<  BITP_RTC_CR2IC_IC0EN    |
   RTC1_CFG_IC2_ENABLE                       <<  BITP_RTC_CR2IC_IC2EN    |
   RTC1_CFG_IC3_ENABLE                       <<  BITP_RTC_CR2IC_IC3EN    |
   RTC1_CFG_IC4_ENABLE                       <<  BITP_RTC_CR2IC_IC4EN    |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_IC0IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_IC2IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_IC3IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_IC4IRQEN |
   RTC1_CFG_IC0_EDGE_POLARITY                <<  BITP_RTC_CR2IC_IC0LH    |
   RTC1_CFG_IC2_EDGE_POLARITY                <<  BITP_RTC_CR2IC_IC2LH    |
   RTC1_CFG_IC3_EDGE_POLARITY                <<  BITP_RTC_CR2IC_IC3LH    |
   RTC1_CFG_IC4_EDGE_POLARITY                <<  BITP_RTC_CR2IC_IC4LH    |
   RTC1_CFG_IC_OVER_WRITE_ENABLE             <<  BITP_RTC_CR2IC_ICOWUSEN,

#if defined(__ADUCM4x50__)
   /* CR3SS */
   RTC1_CFG_SS1_ENABLE                       <<  BITP_RTC_CR3SS_SS1EN    |
   RTC1_CFG_SS2_ENABLE                       <<  BITP_RTC_CR3SS_SS2EN    |
   RTC1_CFG_SS3_ENABLE                       <<  BITP_RTC_CR3SS_SS3EN    |
   RTC1_CFG_SS4_ENABLE                       <<  BITP_RTC_CR3SS_SS4EN    |
   RTC1_CFG_SS1_INT_ENABLE                   <<  BITP_RTC_CR3SS_SS1IRQEN |
   RTC1_CFG_SS2_INT_ENABLE                   <<  BITP_RTC_CR3SS_SS2IRQEN |
   RTC1_CFG_SS3_INT_ENABLE                   <<  BITP_RTC_CR3SS_SS3IRQEN |
   RTC1_CFG_SS4_INT_ENABLE                   <<  BITP_RTC_CR3SS_SS4IRQEN,

   /* CR4SS */
   RTC1_CFG_SS1_MASK_ENABLE                  <<  BITP_RTC_CR4SS_SS1MSKEN |
   RTC1_CFG_SS2_MASK_ENABLE                  <<  BITP_RTC_CR4SS_SS2MSKEN |
   RTC1_CFG_SS3_MASK_ENABLE                  <<  BITP_RTC_CR4SS_SS3MSKEN |
   RTC1_CFG_SS4_MASK_ENABLE                  <<  BITP_RTC_CR4SS_SS4MSKEN |
   RTC1_CFG_SS1_AUTO_RELOADING_ENABLE        <<  BITP_RTC_CR4SS_SS1ARLEN,
#elif defined(__ADUCM302x__)
   /* CR3SS */
   RTC1_CFG_SS1_ENABLE                       <<  BITP_RTC_CR3SS_SS1EN    |
   RTC1_CFG_SS1_INT_ENABLE                   <<  BITP_RTC_CR3SS_SS1IRQEN |

   /* CR4SS */
   RTC1_CFG_SS1_MASK_ENABLE                  <<  BITP_RTC_CR4SS_SS1MSKEN |
   RTC1_CFG_SS1_AUTO_RELOADING_ENABLE        <<  BITP_RTC_CR4SS_SS1ARLEN,
#else
#error RTC driver not ported to this processor
#endif
   /* SSMSK */
   RTC1_CFG_SS1_MASK_VALUE,

   /* SS1 */
   RTC1_CFG_SS1_AUTO_RELOAD_VALUE,

   0,      /* CR5SSS */                      /* TODO: Add the following to the static configuration macros */
   0,      /* CR6SSS */
   0,      /* CR7SSS */
   0x4688, /* GPMUX0 */
   0x01F5, /* GPMUX1 */

  }

};

#endif
/*! \endcond */
