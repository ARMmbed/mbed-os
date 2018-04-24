/**
 * @file xmc_wdt.c
 * @date 2015-06-20
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
 * 2015-06-20:
 *     - Removed definition of GetDriverVersion API <br>
 *
 * @endcond
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include "xmc_wdt.h"
#include "xmc_scu.h"

/*********************************************************************************************************************
 * API IMPLEMENTATION
  ********************************************************************************************************************/

/* Enables watchdog clock and releases watchdog reset. */
void XMC_WDT_Enable(void)
{
#if UC_FAMILY == XMC4
  XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_WDT);
#endif

#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_WDT);
#endif
#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_SCU_RESET_DeassertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_WDT);
#endif  
}

/* Disables watchdog clock and resets watchdog. */
void XMC_WDT_Disable(void)
{
#if defined(PERIPHERAL_RESET_SUPPORTED)
  XMC_SCU_RESET_AssertPeripheralReset(XMC_SCU_PERIPHERAL_RESET_WDT);
#endif  
#if defined(CLOCK_GATING_SUPPORTED)
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_WDT); 
#endif

#if UC_FAMILY == XMC4
  XMC_SCU_CLOCK_DisableClock(XMC_SCU_CLOCK_WDT);
#endif
}
/* Initializes and configures watchdog with configuration data pointed by \a config. */
void XMC_WDT_Init(const XMC_WDT_CONFIG_t *const config)
{
  XMC_WDT_Enable();
  WDT->CTR = config->wdt_ctr;
  WDT->WLB = config->window_lower_bound;
  WDT->WUB = config->window_upper_bound;
}
