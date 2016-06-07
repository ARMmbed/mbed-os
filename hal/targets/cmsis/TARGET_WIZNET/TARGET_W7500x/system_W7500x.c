/**************************************************************************//**
 * @file     system_CMSDK_CM0.c
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Source File for
 *           Device CMSDK
 * @version  V3.01
 * @date     06. March 2012
 *
 * @note
 * Copyright (C) 2010-2012 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#include "system_W7500x.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
//#define SYSCLK_EXTERN_OSC



/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemFrequency = 0;    /*!< System Clock Frequency (Core Clock)  */
uint32_t SystemCoreClock = 0;    /*!< Processor Clock Frequency            */


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    uint8_t M,N,OD;

#ifdef SYSCLK_EXTERN_OSC
    CRG->PLL_IFSR = CRG_PLL_IFSR_OCLK;
#else
    CRG->PLL_IFSR = CRG_PLL_IFSR_RCLK;
#endif    
    OD = (1 << (CRG->PLL_FCR & 0x01)) * (1 << ((CRG->PLL_FCR & 0x02) >> 1));
    N = (CRG->PLL_FCR >>  8 ) & 0x3F;
    M = (CRG->PLL_FCR >> 16) & 0x3F;

#ifdef SYSCLK_EXTERN_OSC
    SystemCoreClock = EXTERN_XTAL * M / N * 1 / OD;
#else
    SystemCoreClock = INTERN_XTAL * M / N * 1 / OD;
#endif
}

uint32_t GetSystemClock()
{
    return SystemCoreClock;
}


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
    uint8_t M,N,OD;

    (*((volatile uint32_t *)(W7500x_TRIM_BGT))) = (*((volatile uint32_t *)(W7500x_INFO_BGT)));
    (*((volatile uint32_t *)(W7500x_TRIM_OSC))) = (*((volatile uint32_t *)(W7500x_INFO_OSC)));

    
    // Set PLL input frequency
#ifdef SYSCLK_EXTERN_OSC
    CRG->PLL_IFSR = CRG_PLL_IFSR_OCLK;
#else
    CRG->PLL_IFSR = CRG_PLL_IFSR_RCLK;
#endif    
    OD = (1 << (CRG->PLL_FCR & 0x01)) * (1 << ((CRG->PLL_FCR & 0x02) >> 1));
    N = (CRG->PLL_FCR >>  8 ) & 0x3F;
    M = (CRG->PLL_FCR >> 16) & 0x3F;

#ifdef SYSCLK_EXTERN_OSC
    SystemCoreClock = EXTERN_XTAL * M / N * 1 / OD;
#else
    SystemCoreClock = INTERN_XTAL * M / N * 1 / OD;
#endif
}
