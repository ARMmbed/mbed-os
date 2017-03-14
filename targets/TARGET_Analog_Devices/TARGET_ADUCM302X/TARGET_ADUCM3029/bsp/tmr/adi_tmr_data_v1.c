/*!
 *****************************************************************************
 * @file:    adi_tmr_data_v1.c
 * @brief:   Power Management service implementation
 * @version: $Revision$
 * @date:    $Date$
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

#ifndef _ADI_TMR_DATA_V1_C_
#define _ADI_TMR_DATA_V1_C_

#include <stdlib.h>
#include "adi_tmr_def_v1.h"

/*! \cond PRIVATE */
#ifdef __ICCARM__
/*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for MMR address casts.
*/
#pragma diag_suppress=Pm140
#endif /* __ICCARM__ */
static ADI_TIMER_DEVICE_INFO aTimerDeviceInfo[ADI_TMR_NUM_INSTANCE] =
{
     {
	(ADI_TMR_TypeDef *)REG_TMR0_LOAD,TMR0_EVT_IRQn,GP_Tmr0_Int_Handler,NULL
     },
     {
	(ADI_TMR_TypeDef *)REG_TMR1_LOAD,TMR1_EVT_IRQn,GP_Tmr1_Int_Handler,NULL
     },
     {
	(ADI_TMR_TypeDef *)REG_TMR2_LOAD,TMR2_EVT_IRQn,GP_Tmr2_Int_Handler,NULL
     }
    
};
#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif /* __ICCARM__ */

static ADI_TMR_CONFIG aTimerConfig[ADI_TMR_NUM_INSTANCE] =
{
  {
    TMR0_CFG_LOAD_VALUE,
    
    TMR0_CFG_PRESCALE_FACTOR       << BITP_TMR_CTL_PRE            |
    TMR0_CFG_COUNT_UP              << BITP_TMR_CTL_UP             |    
    TMR0_CFG_MODE                  << BITP_TMR_CTL_MODE           |    
    TMR0_CFG_CLOCK_SOURCE          << BITP_TMR_CTL_CLK            |    
    TMR0_CFG_RELOAD_CONTROL        << BITP_TMR_CTL_RLD            |    
    TMR0_CFG_EVENT_CAPTURE         <<  BITP_TMR_CTL_EVTRANGE      |    
    TMR0_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_CTL_SYNCBYP        |
    TMR0_CFG_ENABLE_PRESCALE_RESET  << BITP_TMR_CTL_RSTEN         |
    TMR0_CFG_ENABLE_EVENT_CAPTURE  <<  BITP_TMR_CTL_EVTEN,
    
    TMR0_CFG_ASYNC_LOAD_VALUE,
    
    TMR0_CFG_ENABLE_PWM_MATCH_MODE << BITP_TMR_PWMCTL_MATCH       |
    TMR0_CFG_PWM_IDLE_STATE        << BITP_TMR_PWMCTL_IDLESTATE,
    
    TMR0_CFG_PWM_MATCH_VALUE
  },
  
  {
    TMR1_CFG_LOAD_VALUE,
    
    TMR1_CFG_PRESCALE_FACTOR       << BITP_TMR_CTL_PRE             |
    TMR1_CFG_COUNT_UP              << BITP_TMR_CTL_UP              |    
    TMR1_CFG_MODE                  << BITP_TMR_CTL_MODE            |    
    TMR1_CFG_CLOCK_SOURCE          << BITP_TMR_CTL_CLK             |    
    TMR1_CFG_RELOAD_CONTROL        << BITP_TMR_CTL_RLD             |    
    TMR1_CFG_EVENT_CAPTURE         <<  BITP_TMR_CTL_EVTRANGE       |  
    TMR1_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_CTL_SYNCBYP         |    
    TMR1_CFG_ENABLE_PRESCALE_RESET  << BITP_TMR_CTL_RSTEN         |    
    TMR1_CFG_ENABLE_EVENT_CAPTURE  <<  BITP_TMR_CTL_EVTEN,
    
    TMR1_CFG_ASYNC_LOAD_VALUE,
    
    TMR1_CFG_ENABLE_PWM_MATCH_MODE << BITP_TMR_PWMCTL_MATCH        |
    TMR1_CFG_PWM_IDLE_STATE        << BITP_TMR_PWMCTL_IDLESTATE,
    
    TMR1_CFG_PWM_MATCH_VALUE
  },

  {
    TMR2_CFG_LOAD_VALUE,
    
    TMR2_CFG_PRESCALE_FACTOR       << BITP_TMR_CTL_PRE            |
    TMR2_CFG_COUNT_UP              << BITP_TMR_CTL_UP             |    
    TMR2_CFG_MODE                  << BITP_TMR_CTL_MODE           |    
    TMR2_CFG_CLOCK_SOURCE          << BITP_TMR_CTL_CLK            |    
    TMR2_CFG_RELOAD_CONTROL        << BITP_TMR_CTL_RLD            |    
    TMR2_CFG_EVENT_CAPTURE         <<  BITP_TMR_CTL_EVTRANGE      |    
    TMR2_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_CTL_SYNCBYP        |    
    TMR2_CFG_ENABLE_PRESCALE_RESET  << BITP_TMR_CTL_RSTEN         |        
    TMR2_CFG_ENABLE_EVENT_CAPTURE  <<  BITP_TMR_CTL_EVTEN,
    
    TMR2_CFG_ASYNC_LOAD_VALUE,
    
    TMR2_CFG_ENABLE_PWM_MATCH_MODE << BITP_TMR_PWMCTL_MATCH       |
    TMR2_CFG_PWM_IDLE_STATE        << BITP_TMR_PWMCTL_IDLESTATE,
    
    TMR2_CFG_PWM_MATCH_VALUE
  },

};
/*! \endcond */

#endif /* _ADI_TMR_DATA_V1_C_ */
