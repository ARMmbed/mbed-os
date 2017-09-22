/**************************************************************************//**
 * @file     system_KM_app.c
 * @brief    CMSIS Cortex-M# Device Peripheral Access Layer Source File for
 *           Device KM
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include <stdint.h>
#include "KM_app.h"


/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies             */
/* TODO: Use the correct frequencies */
#define __HSI             (300000000UL)
#define __XTAL            (300000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (    ___HSI)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (10*__XTAL)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit()    */
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
/* ToDo: add code to calculate the system frequency based upon the current
         register settings.
         This function can be used to retrieve the system core clock frequeny
         after user changed register sittings.                                */
  SystemCoreClock = __SYSTEM_CLOCK;
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
/* ToDo: add code to initialize the system
         do not use global variables because this function is called before
         reaching pre-main. RW section maybe overwritten afterwards.          */
		 	 
    uint32_t x = __get_xPSR();
	
	SystemCoreClock = __SYSTEM_CLOCK;

    /* Check for interrupt context and reboot needed. */
    if (x & 0x3f) {
        /* Processor is in an interrupt context, reset the core by triggering
         * the reset vector using the SYSRESETREQ bit in the AIRCR register */
        x =  cortex_m7_scs->aircr;
        x &= 0xffff;            			/* Mask out the top 16 bits */
        x |= 0x05fa0000;        			/* Must write with this value for the write to take effect */
        x |= 0x04;              			/* Set the SYSRESETREQ bit */
        cortex_m7_scs->aircr  = x;        	/* Reset the core */ /* aircr: Application Interrupt/Reset Control Register */
    }
	
    /* Release claim on any pins */  /* These registers are the both the method of claiming a PIO, The PIO’s are specified by five 32 bit registers */
    PIO_FUNC0_BITCLR = 0xFFFFFFFF;
    PIO_FUNC1_BITCLR = 0xFFFFFFFF;
    PIO_FUNC2_BITCLR = 0xFFFFFFFF;
    PIO_FUNC3_BITCLR = 0xFFFFFFFF;
    PIO_FUNC4_BITCLR = 0xFFFFFFFF;

	cortex_m7_scs->nvic_iser[0] = 0xFFFFFFFF; /* Disable all IRQ interrupts */ /* nvic_iser: Interupt Clear_Enable register */ 
   
    __set_PRIMASK(0);	/* Ensure interrupts are enabled */
	
	SystemAllowSleep(true);	/* Allow sleep */
}


void SystemAllowSleep(bool sleepAllowed)
{
    if (sleepAllowed) {
        /* Set deep sleep, though not on exit */
        cortex_m7_scs->scr |= SCB_SCR_SLEEPDEEP_Msk;
        cortex_m7_scs->scr &= ~SCB_SCR_SLEEPONEXIT_Msk;
    } else {
        /* Unset deep sleep */
        cortex_m7_scs->scr &= ~SCB_SCR_SLEEPDEEP_Msk;
    }
}
