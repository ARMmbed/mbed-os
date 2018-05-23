/**
 * @file xmc4_rtc.c
 * @date 2017-08-04
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
 * 2016-03-09:
 *     - Optimize write only registers
 *      
 * 2017-08-04:
 *     - Changed XMC_RTC_EnableHibernationWakeUp() and XMC_RTC_DisableHibernationWakeUpcheck() 
 *       Check SCU_MIRRSTS to ensure that no transfer over serial interface is pending to the RTC_CTR register
 *
 * @endcond 
 *
 */

/**
 * @brief RTC driver for XMC microcontroller family.
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include <xmc_rtc.h>

#if UC_FAMILY == XMC4
#include <xmc_scu.h>

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/ 

/*
 * Enables RTC peripheral for programming its registers
 */
void XMC_RTC_Enable(void)
{
  XMC_SCU_HIB_EnableHibernateDomain();
}

/*
 * Disables RTC peripheral for programming its registers
 */
void XMC_RTC_Disable(void)
{
  /*
   * Empty because disabling the hibernate
   * domain is not done intentionally.
   */
}

/*
 * Checks RTC peripheral is enabled for programming to its registers
 */
bool XMC_RTC_IsEnabled(void)
{
  return XMC_SCU_HIB_IsHibernateDomainEnabled();
}

/*
 * Initialize the RTC peripheral
 */
XMC_RTC_STATUS_t XMC_RTC_Init(const XMC_RTC_CONFIG_t *const config)
{
  if (XMC_RTC_IsRunning() == false)
  {
    if (XMC_SCU_HIB_IsHibernateDomainEnabled() == false)
    {
      XMC_SCU_HIB_EnableHibernateDomain();
    }
    
    XMC_RTC_SetPrescaler(config->prescaler);
    
    while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->TIM0 = config->time.raw0;
    
    while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->TIM1 = config->time.raw1;
    
    while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->ATIM0 = config->alarm.raw0;
    
    while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->ATIM1 = config->alarm.raw1;    
  }  
  return XMC_RTC_STATUS_OK;
}

/*
 * Enable RTC periodic and alarm event(s)
 */ 
void XMC_RTC_EnableEvent(const uint32_t event)
{
  while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_MSKSR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->MSKSR |= event;
}

/*
 * Disable RTC periodic and alarm event(s)
 */ 
void XMC_RTC_DisableEvent(const uint32_t event)
{
  while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_MSKSR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->MSKSR &= ~event;
}

/*
 * Clear RTC periodic and alarm event(s)
 */ 
void XMC_RTC_ClearEvent(const uint32_t event)
{
  while ((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CLRSR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CLRSR = event;
}

void XMC_RTC_EnableHibernationWakeUp(const uint32_t event)
{
  while((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR |= event;
}

void XMC_RTC_DisableHibernationWakeUp(const uint32_t event)
{
  while((XMC_SCU_GetMirrorStatus() & SCU_GENERAL_MIRRSTS_RTC_CTR_Msk) != 0U)
  {
    /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
  }
  RTC->CTR &= ~event;
}

#endif /* UC_FAMILY == XMC4 */
