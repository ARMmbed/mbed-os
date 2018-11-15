/**************************************************************************//**
 * @file     RDA5981_nvic_virtual.h
 * @brief    
 * @version  
 * @date     
 ******************************************************************************/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RDA5981_NVIC
#define __RDA5981_NVIC

//#include "cmsis.h"
//#include <stdint.h>

extern void rda_ccfg_ckrst(void);
extern void rda_wdt_softreset(void);
#ifdef __cplusplus
extern "C" {
#endif

/**
  \brief   System Reset
  \details Initiates a system reset request to reset the MCU.
 */
__NO_RETURN __STATIC_INLINE void __RDA5981_SystemReset(void)
{
	
  rda_ccfg_ckrst () ;
  __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
  SCB->AIRCR  = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos)    |
                           (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                            SCB_AIRCR_VECTRESET_Msk    );         /* Keep priority group unchanged */
  __DSB();                                                          /* Ensure completion of memory access */

  for(;;)                                                           /* wait until reset */
  {
    __NOP();
  }

}


#define NVIC_SetPriorityGrouping    __NVIC_SetPriorityGrouping
#define NVIC_GetPriorityGrouping    __NVIC_GetPriorityGrouping
#define NVIC_EnableIRQ              __NVIC_EnableIRQ
#define NVIC_GetEnableIRQ           __NVIC_GetEnableIRQ
#define NVIC_DisableIRQ             __NVIC_DisableIRQ
#define NVIC_GetPendingIRQ          __NVIC_GetPendingIRQ
#define NVIC_SetPendingIRQ          __NVIC_SetPendingIRQ
#define NVIC_ClearPendingIRQ        __NVIC_ClearPendingIRQ
#define NVIC_GetActive              __NVIC_GetActive
#define NVIC_SetPriority            __NVIC_SetPriority
#define NVIC_GetPriority            __NVIC_GetPriority
#define NVIC_SystemReset            __RDA5981_SystemReset


#ifdef __cplusplus
}
#endif

#endif


