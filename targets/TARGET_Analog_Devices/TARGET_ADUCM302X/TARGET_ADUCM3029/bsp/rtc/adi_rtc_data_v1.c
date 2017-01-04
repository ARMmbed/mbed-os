/*!
 *****************************************************************************
 * @file:    adi_rtc_data_v1.c
 * @brief:   rtc device data file
 * @version: $Revision: 34933 $
 * @date:    $Date: 2016-06-28 12:11:25 +0100 (Tue, 28 Jun 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2015 Analog Devices, Inc.

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
#ifndef _ADI_RTC_DATA_V1_C_
#define _ADI_RTC_DATA_V1_C_

#include <stdlib.h>
#include "adi_rtc_def_v1.h"

static ADI_RTC_DEVICE_INFO aRTCDeviceInfo[ADI_RTC_NUM_INSTANCE] =
{
     {
	(ADI_RTC_TypeDef *)pADI_RTC0,RTC0_EVT_IRQn, NULL
     },
     {
	(ADI_RTC_TypeDef *)pADI_RTC1,RTC1_EVT_IRQn,NULL,
     }
};

#if (ADI_RTC_STATIC_CONFIG_SUPPORT_ENABLE == 1)

static ADI_RTC_CONFIG aRTCConfig[ADI_RTC_NUM_INSTANCE] =
{
  {
   RTC0_CFG_ENABLE_ALARM                    <<  BITP_RTC_CR0_ALMEN         |
   RTC0_CFG_ENABLE_ALARM_INTERRUPT          <<  BITP_RTC_CR0_ALMINTEN      |
   RTC0_CFG_ENABLE_TRIM                      <<  BITP_RTC_CR0_TRMEN         |
   RTC0_CFG_ENABLE_PENDERROR_INTERRUPT       <<  BITP_RTC_CR0_WPNDERRINTEN  |
   RTC0_CFG_ENABLE_WSYNC_INTERRUPT           <<  BITP_RTC_CR0_WSYNCINTEN    |
   RTC0_CFG_ENABLE_WRITEPEND_INTERRUPT       <<  BITP_RTC_CR0_WPNDINTEN ,
   0,
   RTC0_CFG_COUNT_VALUE_0,
   RTC0_CFG_COUNT_VALUE_1,
   
   RTC0_CFG_ALARM_VALUE_0,
   RTC0_CFG_ALARM_VALUE_1,
   0,

   RTC0_CFG_POW2_TRIM_INTERVAL               << BITP_RTC_TRM_IVL2EXPMIN   |
   RTC0_CFG_TRIM_INTERVAL                    << BITP_RTC_TRM_IVL          |
   RTC0_CFG_TRIM_OPERATION                   << BITP_RTC_TRM_ADD          |
   RTC0_CFG_TRIM_VALUE                       << BITP_RTC_TRM_VALUE,
   
   0,
   0,
   0,
   0,
   0,
   
  },
  /* RTC-1 */
  {
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

   RTC1_CFG_ENABLE_COUNT_INTERRUPT           <<  BITP_RTC_CR1_CNTINTEN       |
   RTC1_CFG_ENABLE_MOD1_COUNT_INTERRUPT      <<  BITP_RTC_CR1_PSINTEN        |
   RTC1_CFG_ENABLE_TRIM_INTERRUPT            <<  BITP_RTC_CR1_RTCTRMINTEN    |
   RTC1_CFG_CNT_MOD60_ROLLLOVER_INTERRUPT    <<  BITP_RTC_CR1_CNTROLLINTEN   |
   RTC1_CFG_PRESCALE                         <<  BITP_RTC_CR1_PRESCALE2EXP   |
   RTC1_CFG_CNT_ROLLLOVER_INTERRUPT          <<  BITP_RTC_CR1_CNTMOD60ROLLINTEN ,
  
   RTC1_CFG_COUNT_VALUE_0,
   RTC1_CFG_COUNT_VALUE_1,

   RTC1_CFG_ALARM_VALUE_0,
   RTC1_CFG_ALARM_VALUE_1,
   RTC1_CFG_ALARM_VALUE_2,


   RTC1_CFG_POW2_TRIM_INTERVAL               << BITP_RTC_TRM_IVL2EXPMIN   |
   RTC1_CFG_TRIM_INTERVAL                    << BITP_RTC_TRM_IVL          |
   RTC1_CFG_TRIM_OPERATION                   << BITP_RTC_TRM_ADD          |
   RTC1_CFG_TRIM_VALUE                       << BITP_RTC_TRM_VALUE,
   

   RTC1_CFG_IC0_ENABLE                       <<  BITP_RTC_CR2IC_RTCIC0EN     |
   RTC1_CFG_IC2_ENABLE                       <<  BITP_RTC_CR2IC_RTCIC2EN     |
   RTC1_CFG_IC3_ENABLE                       <<  BITP_RTC_CR2IC_RTCIC3EN     |
   RTC1_CFG_IC4_ENABLE                       <<  BITP_RTC_CR2IC_RTCIC4EN   |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_RTCIC0IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_RTCIC2IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_RTCIC3IRQEN |
   RTC1_CFG_IC0_INT_ENABLE                   <<  BITP_RTC_CR2IC_RTCIC4IRQEN  |
   RTC1_CFG_IC0_EDGE_POLARITY                <<  BITP_RTC_CR2IC_RTCIC0LH |
   RTC1_CFG_IC2_EDGE_POLARITY                <<  BITP_RTC_CR2IC_RTCIC2LH |
   RTC1_CFG_IC3_EDGE_POLARITY                <<  BITP_RTC_CR2IC_RTCIC3LH |
   RTC1_CFG_IC4_EDGE_POLARITY                <<  BITP_RTC_CR2IC_RTCIC4LH  |
   RTC1_CFG_IC_OVER_WRITE_ENABLE             <<  BITP_RTC_CR2IC_RTCICOWUSEN,
  
   RTC1_CFG_OC1_ENABLE                       <<  BITP_RTC_CR3OC_RTCOC1EN     |
   RTC1_CFG_OC1_INT_ENABLE                   <<  BITP_RTC_CR3OC_RTCOC1IRQEN,

   RTC1_CFG_OC1_MASK_ENABLE                  <<  BITP_RTC_CR4OC_RTCOC1MSKEN     |
   RTC1_CFG_OC1_AUTO_RELOADING_ENABLE        <<  BITP_RTC_CR4OC_RTCOC1ARLEN,

   RTC1_CFG_OC1_MASK_VALUE,
   
   RTC1_CFG_OC1_AUTO_RELOAD_VALUE   
   
  }

};
#endif
#endif
/*! \endcond */
