/*!
 *****************************************************************************
   @file:    adi_rtc_config.h
   @brief:   Configuration options for Real Time Clock  device  driver.
             This is specific to the RTC driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 33005 $
   @date:    $Date: 2015-12-12 15:43:13 +0000 (Sat, 12 Dec 2015) $
  -----------------------------------------------------------------------------

Copyright (c) 2012-2014 Analog Devices, Inc.

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

*****************************************************************************/

#ifndef __ADI_RTC_CONFIG_H__
#define __ADI_RTC_CONFIG_H__
#include <adi_global_config.h>
/** @defgroup RTC_Driver_Cfg RTC Device Driver Configuration
 *  @ingroup Configuration_macros
 */
 /*! \addtogroup  RTC_Driver_Cfg RTC Device Driver Configuration
 *  @{
 */
/*!
 * The safe write mode insures any pending writes that have not yet synchronized between the faster core clock
 * domain and the internal RTC 32kHz clock domain are reconciled before multiple writes to the same RTC register
 * are allowed
*/

#define ADI_RTC_CFG_ENABLE_SAFE_WRITE 1    

/*!
   Set this macro to 1 for to enable static rtc  initializations
   during the driver initialization routine #adi_rtc_Open().
   To eliminate static driver configuration, set this macro to 0.*/

#define ADI_RTC_STATIC_CONFIG_SUPPORT_ENABLE   0

/*
===================================================================
 ------------------------RTC-0 CONFIGURATION MACRO-----------------
===================================================================
*/
/*! Enabling the Alarm */
#define RTC0_CFG_ENABLE_ALARM                  0     

/*! Enabling the Alarm interrupt*/
#define RTC0_CFG_ENABLE_ALARM_INTERRUPT        0     

/*! Enabling the TRIM */
#define RTC0_CFG_ENABLE_TRIM                    0     

/*! Enabling the PENDERROR interrupt*/
#define RTC0_CFG_ENABLE_PENDERROR_INTERRUPT     0     

/*! Enabling the write sync  interrupt*/
#define RTC0_CFG_ENABLE_WSYNC_INTERRUPT         0     

/*! Enabling the pend write interrupt*/
#define RTC0_CFG_ENABLE_WRITEPEND_INTERRUPT     0     

/*! Initial the count Value*/
#define RTC0_CFG_COUNT_VALUE                    0     

/*! Initial the count Value-0*/
#define RTC0_CFG_COUNT_VALUE_0                  0     

/*! Initial the count Value-1*/
#define RTC0_CFG_COUNT_VALUE_1                  0     

/*! Alarm-0  Value*/
#define RTC0_CFG_ALARM_VALUE_0                  0     

/*! Alarm-1  Value*/
#define RTC0_CFG_ALARM_VALUE_1                  0     

/*! TRIM interval*/
#define RTC0_CFG_TRIM_INTERVAL                  0     

/*! TRIM interval with power of 2*/
#define RTC0_CFG_POW2_TRIM_INTERVAL             0     

/*! Trim operation to be performed for RTC0*/
#define RTC0_CFG_TRIM_OPERATION                 0     

/*! Trim  Value for RTC-0*/
#define RTC0_CFG_TRIM_VALUE                     0     

/*
===================================================================
 ------------------------RTC-1 CONFIGURATION MACRO-----------------
===================================================================
*/

/*!
   Set this macro to 1 for to enable static rtc-1  initializations
   during the driver initialization routine #adi_rtc_Open().
   To eliminate static driver configuration, set this macro to 0.*/
   
/*! Enabling the Alarm */
#define RTC1_CFG_ENABLE_ALARM                       0     

/*! Enabling the Alarm interrupt*/
#define RTC1_CFG_ENABLE_ALARM_INTERRUPT             0     

/*! Enabling the TRIM */
#define RTC1_CFG_ENABLE_TRIM                        0     

/*! Enabling the mod-60 Alarm */
#define RTC1_CFG_ENABLE_MOD60_ALARM                 0     

/*! Enabling the mod-60 Alarm  period*/
#define RTC1_CFG_ENABLE_MOD60_ALARM_PERIOD          0     

/*! Enabling the Alarm interrupt*/
#define RTC1_CFG_ENABLE_MOD60_ALARM_INTERRUPT       0     

/*! Enabling the ISOINT interrupt*/
#define RTC1_CFG_ENABLE_ISO_INTERRUPT               0     

/*! Enabling the PENDERROR interrupt*/
#define RTC1_CFG_ENABLE_PENDERROR_INTERRUPT         0     

/*! Enabling the write sync  interrupt*/
#define RTC1_CFG_ENABLE_WSYNC_INTERRUPT             0     

/*! Enabling the pend write interrupt*/
#define RTC1_CFG_ENABLE_WRITEPEND_INTERRUPT         0     

/*! Enabling the RTC count interrupt*/
#define RTC1_CFG_ENABLE_COUNT_INTERRUPT             0     

/*! Enabling the prescaled modulo-1 interrupt*/
#define RTC1_CFG_ENABLE_MOD1_COUNT_INTERRUPT        0     

/*! Enabling the TRIM interrupt*/
#define RTC1_CFG_ENABLE_TRIM_INTERRUPT              0     

/*! Enabling the Mod60 roll over  interrupt*/
#define RTC1_CFG_CNT_MOD60_ROLLLOVER_INTERRUPT      0     

/*! Prescale value for the RTC1*/
#define RTC1_CFG_PRESCALE                           0     

/*! Enabling the counter roll over  interrupt*/
#define RTC1_CFG_CNT_ROLLLOVER_INTERRUPT            0     

/*! Initial the count Value-0*/
#define RTC1_CFG_COUNT_VALUE_0                      0     

/*! Initial the count Value-1*/
#define RTC1_CFG_COUNT_VALUE_1                      0     

/*! Alarm  Value-0*/
#define RTC1_CFG_ALARM_VALUE_0                      0     

/*! Alarm  Value-1*/
#define RTC1_CFG_ALARM_VALUE_1                      0     

/*! Alarm  Value-2*/
#define RTC1_CFG_ALARM_VALUE_2                      0     

/*! TRIM interval*/
#define RTC1_CFG_TRIM_INTERVAL                      0     

/*! TRIM interval with power of 2*/
#define RTC1_CFG_POW2_TRIM_INTERVAL                 0     

/*! Trim operation to be performed for RTC1*/
#define RTC1_CFG_TRIM_OPERATION                     0     

/*! Trim  Value for RTC-1*/
#define RTC1_CFG_TRIM_VALUE                         0     

/*! Enable the input capture channel-0*/
#define RTC1_CFG_IC0_ENABLE                         0     

/*! Enable the input capture channel-2*/
#define RTC1_CFG_IC2_ENABLE                         0     

/*! Enable the input capture channel-3*/
#define RTC1_CFG_IC3_ENABLE                         0     

/*! Enable the input capture channel-4*/
#define RTC1_CFG_IC4_ENABLE                         0     

/*! Enable the output compare channel-1*/
#define RTC1_CFG_OC1_ENABLE                         1     

/*! Enable the interrupt for input capture channel-0*/
#define RTC1_CFG_IC0_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-2*/
#define RTC1_CFG_IC2_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-3*/
#define RTC1_CFG_IC3_INT_ENABLE                     0     

/*! Enable the interrupt for input capture channel-4*/
#define RTC1_CFG_IC4_INT_ENABLE                     0     

/*! Enable the over write input capture channels*/
#define RTC1_CFG_IC_OVER_WRITE_ENABLE               0     

/*! Polarity for  input capture channel-0*/
#define RTC1_CFG_IC0_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-2*/
#define RTC1_CFG_IC2_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-3*/
#define RTC1_CFG_IC3_EDGE_POLARITY                  0     

/*! Polarity for  input capture channel-4*/
#define RTC1_CFG_IC4_EDGE_POLARITY                  0     

/*! Enable the interrupt for output compare channel-0*/
#define RTC1_CFG_OC1_INT_ENABLE                     1     

/*! Enable the masking  for output compare channel-0*/
#define RTC1_CFG_OC1_MASK_ENABLE                    0     

/*! Enable the auto-reloading   for output compare channel-0*/
#define RTC1_CFG_OC1_AUTO_RELOADING_ENABLE          1     

/*! Mask for output compare channel-0*/
#define RTC1_CFG_OC1_MASK_VALUE                     0     


/*! Auto reload value  for output compare channel-0*/
#define RTC1_CFG_OC1_AUTO_RELOAD_VALUE              32768/2     

/*! @} */
/*! @} */
#endif /* __ADI_RTC_CONFIG_H__ */
