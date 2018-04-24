/**
 * @file xmc1_rtc.c
 * @date 2016-03-09
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
 *     - XMC_RTC_Init() function is modified
 *       by adding RTC running condition check
 *
 * 2016-03-09:
 *     - Optimize write only registers
 *
 * @endcond 
 *
 */


/**
 *
 * @brief RTC driver for XMC microcontroller family.
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
#include <xmc_rtc.h>

#if UC_FAMILY == XMC1
#include <xmc_scu.h>

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
/*
 * Initialize the RTC peripheral
 */
XMC_RTC_STATUS_t XMC_RTC_Init(const XMC_RTC_CONFIG_t *const config)
{
  if (XMC_RTC_IsRunning() == false)
  {
    if (XMC_RTC_IsEnabled() == false)
    {
      XMC_RTC_Enable();
	}
	
    XMC_RTC_SetPrescaler(config->prescaler);

	while ((XMC_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_TIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_TIM1_Msk)) != 0U)
	{
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->TIM0 = config->time.raw0;
    RTC->TIM1 = config->time.raw1;
	
    while ((XMC_SCU_GetMirrorStatus() & (SCU_GENERAL_MIRRSTS_RTC_ATIM0_Msk | SCU_GENERAL_MIRRSTS_RTC_ATIM1_Msk)) != 0U)
    {
      /* check SCU_MIRRSTS to ensure that no transfer over serial interface is pending */
    }
    RTC->ATIM0 = config->alarm.raw0;	
    RTC->ATIM1 = config->alarm.raw1;    
  }
  return XMC_RTC_STATUS_OK;
}

/*
 * Ungates a clock node for RTC
 */ 
void XMC_RTC_Enable(void)
{
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_RTC);
}

/*
 * Gates a clock node for RTC
 */ 
void XMC_RTC_Disable(void)
{
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_RTC);
}

/*
 * Suspends RTC function during CPU HALT mode
 */ 
void XMC_RTC_SetDebugMode(const XMC_RTC_DEBUG_MODE_t debug_mode)
{
  uint32_t regval;
  regval = (RTC->CTR & (uint32_t)~RTC_CTR_SUS_Msk);
  RTC->CTR = (regval | (uint32_t)debug_mode);
}

/*
 * Enable RTC periodic and alarm event(s)
 */ 
void XMC_RTC_EnableEvent(const uint32_t event)
{
  RTC->MSKSR |= event;
}

/*
 * Disable RTC periodic and alarm event(s)
 */
void XMC_RTC_DisableEvent(const uint32_t event)
{
  RTC->MSKSR &= ~event;
}

/*
 * Clear RTC periodic and alarm event(s)
 */
void XMC_RTC_ClearEvent(const uint32_t event)
{
  RTC->CLRSR = event;
}

/*
 * Checks RTC peripheral is enabled for programming to its registers
 */
bool XMC_RTC_IsEnabled(void)
{
  return !XMC_SCU_CLOCK_IsPeripheralClockGated(XMC_SCU_PERIPHERAL_CLOCK_RTC);
}

#endif /* UC_FAMILY == XMC1 */
