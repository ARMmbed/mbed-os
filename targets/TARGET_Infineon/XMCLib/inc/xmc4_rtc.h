/**
 * @file xmc4_rtc.h
 * @date 2015-05-20
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
 *     - Initial
 *      
 * 2015-05-20:
 *     - Documentation updates <br>
 *   
 * @endcond 
 *
 */

#ifndef XMC4_RTC_H
#define XMC4_RTC_H

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup RTC
 * @{
 */

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * Wakeup events for RTC from hibernate domain
 */
typedef enum XMC_RTC_WAKEUP_EVENT
{
  XMC_RTC_WAKEUP_EVENT_ON_ALARM   = RTC_CTR_TAE_Msk,  /**< Wakeup from alarm event */
  XMC_RTC_WAKEUP_EVENT_ON_SECONDS = RTC_CTR_ESEC_Msk, /**< Wakeup from seconds event */
  XMC_RTC_WAKEUP_EVENT_ON_MINUTES = RTC_CTR_EMIC_Msk, /**< Wakeup from minutes event */
  XMC_RTC_WAKEUP_EVENT_ON_HOURS   = RTC_CTR_EHOC_Msk, /**< Wakeup from hours event */
  XMC_RTC_WAKEUP_EVENT_ON_DAYS    = RTC_CTR_EDAC_Msk, /**< Wakeup from days event */
  XMC_RTC_WAKEUP_EVENT_ON_MONTHS  = RTC_CTR_EMOC_Msk, /**< Wakeup from months event */
  XMC_RTC_WAKEUP_EVENT_ON_YEARS   = RTC_CTR_EYEC_Msk  /**< Wakeup from years event */
} XMC_RTC_WAKEUP_EVENT_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param event A valid RTC Wakeup event (::XMC_RTC_WAKEUP_EVENT_t) or a valid combination of
 *              logically OR'd events
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable Wakeup from hibernate mode <br>
 *
 * \par
 * The function sets the bitfields of CTR register to enable wakeup from hibernate mode.  
 * Setting the masking value for the RTC wakeup events containing in the ::XMC_RTC_WAKEUP_EVENT_t 
 * leads to a wakeup from hibernate mode.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_DisableHibernationWakeUp()
 */
void XMC_RTC_EnableHibernationWakeUp(const uint32_t event);

/**
 * @param event A valid RTC Wakeup event (::XMC_RTC_WAKEUP_EVENT_t) or a valid combination of
 *              logically OR'd events          
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable Wakeup from hibernate mode <br>
 *
 * \par
 * The function resets the bitfields of CTR register to disable wakeup from hibernate mode.  
 * Resetting the masking value for the RTC wakeup events containing in the ::XMC_RTC_WAKEUP_EVENT_t  
 * disables wakeup from hibernate mode.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_RTC_EnableHibernationWakeUp()
 */
void XMC_RTC_DisableHibernationWakeUp(const uint32_t event);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* XMC4_RTC_H */
