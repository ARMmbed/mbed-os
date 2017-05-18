/*! *****************************************************************************
 * @file    adi_wdt.c
 * @brief   WDT device driver implementation
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
*
* Pm011 (rule 6.3): the basic types of char, int, short, long, float, and double should not be used
*   Necessary for stdbool.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm140 (Rule 11.4): a cast should not be performed between a pointer type and an integral type
*   This violation appears when deferencing the pointer to the register typedef. No way around this.
*/
#pragma diag_suppress=Pm011,Pm073,Pm140,Pm143
#endif /* __ICCARM__ */


/** @addtogroup WDT_Driver WDT Driver
 *  @{
 * @brief Watchdog Timer (WDT) Driver
 * @details The watchdog timer driver allows the user to enable the timer with
 * the static configuration parameters, reset the timer, and read the timer
 * count.  No interface is provided for setting the timer parameters are
 * runtime since the WDT may only be configured once for the program lifetime.
 * The timer is disabled by default by the ADuCM4x50 boot kernel.
 * @note The application must include drivers/wdt/adi_wdt.h to use this driver
 */

#include <stdlib.h>
#include <adi_processor.h>
#include <rtos_map/adi_rtos_map.h>
#include <adi_wdt_config.h>
#include <drivers/wdt/adi_wdt.h>

/*! \cond PRIVATE */

/*! Bus synchronization bits that must go low before writing to the CTL or RESET registers */
#define ADI_WDT_SYNC_BITS ((0x1u << BITP_WDT_STAT_COUNTING) | (0x1u << BITP_WDT_STAT_LOADING) | (0x1u << BITP_WDT_STAT_CLRIRQ))

/*! Value that is written to the reset register to kick the dog */
#define ADI_WDT_CLR_VALUE (0xCCCCu)

/*! Store the callback locally if we are using interrupt mode */
#if (ADI_WDT_CONTROL_TIMEOUT_MODE == 1u)
static ADI_CALLBACK gAppCallback;
#endif

/*! \endcond */

/*********************************************************************************
                                    API IMPLEMENTATIONS 
*********************************************************************************/


/*!
 * @brief      WDT Enable
 *
 * @details    Enables/disables the WDT with the paramters supplied in adi_wdt_config.h
 *
 * @param [in] bEnable    : True to turn WDT on, false to turn it off
 *
 * @param [in] pfCallback : If interrupt mode is enabled, specify application callback function,
 *                          otherwise simply pass NULL for the argument.
 *
 * @return     ADI_WDT_RESULT
 *                - #ADI_WDT_FAILURE_LOCKED           WDT has already been initialized
 *                - #ADI_WDT_SUCCESS                  Function call completed successfully
 */
ADI_WDT_RESULT adi_wdt_Enable(bool const bEnable, ADI_CALLBACK const pfCallback) {
    /* IF(Device is enabled, application can't modify it) */
    if ((pADI_WDT0->STAT & ((uint16_t) BITM_WDT_STAT_LOCKED)) != ((uint16_t) 0x0u)) {
        return ADI_WDT_FAILURE_LOCKED;
    } /* ENDIF */

    /* Setup interrupts if we are in interrupt mode */
#if (ADI_WDT_CONTROL_TIMEOUT_MODE == 1u)
    gAppCallback = pfCallback;
    /* IF(We are enabling the WDT) */
    if (bEnable == true) {
        NVIC_EnableIRQ (WDT_EXP_IRQn);
    /* ELSE (We are disabling the WDT, this might not be necessary, depends on startup config) */
    } else {
        NVIC_DisableIRQ(WDT_EXP_IRQn);
    } /* ENDIF */
#endif

    /* WHILE(Bus sync is underway) */
    while((pADI_WDT0->STAT & ADI_WDT_SYNC_BITS) != 0u) {
        ;
    } /* ENDWHILE */


    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    pADI_WDT0->LOAD = ADI_WDT_LOAD_VALUE;

    /* IF(Turning the WDT on) */
    if (bEnable == true) {
        pADI_WDT0->CTL  = (ADI_WDT_CONTROL_TIMER_MODE      << BITP_WDT_CTL_MODE) |
                          (0x1u                            << BITP_WDT_CTL_EN  ) |
                          (ADI_WDT_CONTROL_CLOCK_PRESCALER << BITP_WDT_CTL_PRE ) |
                          (ADI_WDT_CONTROL_TIMEOUT_MODE    << BITP_WDT_CTL_IRQ ) |
                          (ADI_WDT_CONTROL_POWER_MODE      <<               0u );
    /* ELSE(Turning the WDT off) */
    } else {
        pADI_WDT0->CTL  = (ADI_WDT_CONTROL_TIMER_MODE      << BITP_WDT_CTL_MODE) |
                          (0x0u                            << BITP_WDT_CTL_EN  ) |
                          (ADI_WDT_CONTROL_CLOCK_PRESCALER << BITP_WDT_CTL_PRE ) |
                          (ADI_WDT_CONTROL_TIMEOUT_MODE    << BITP_WDT_CTL_IRQ ) |
                          (ADI_WDT_CONTROL_POWER_MODE      <<               0u );
    } /* ENDIF */

    ADI_EXIT_CRITICAL_REGION();

    return ADI_WDT_SUCCESS;
}

/*!
 * @brief      WDT Reset
 *
 * @details    Resets the WDT
 *
 * @return     None
 */
void adi_wdt_Kick(void) {
    /* WHILE(Bus sync is underway) */
    while((pADI_WDT0->STAT & ADI_WDT_SYNC_BITS) != 0u) {
        ;
    } /* ENDWHILE */

   /* Kick the dog! */
    pADI_WDT0->RESTART = ADI_WDT_CLR_VALUE;
}

/*!
 * @brief       WDT Read Count
 *
 * @details     Read the current WDT count
 *
 * @param [out] pCurCount : Pointer to memory to read the count into
 *
 * @return      None
 */
void adi_wdt_GetCount(uint16_t * const pCurCount) {
    /* Read the count */
    *pCurCount = pADI_WDT0->CCNT;
}

/*! \cond PRIVATE */

/*!
 * @brief     WDT0 Interrupt Handler
 *
 * @details   Kicks the dog and calls the user supplied callback function
 *
 * @return    None
 *
 * @note      Do not need to explicitly clear the interrupt status,
 *            kicking the dog performs this action.
 */
#if (ADI_WDT_CONTROL_TIMEOUT_MODE == 1u)
extern void WDog_Tmr_Int_Handler(void); 
void WDog_Tmr_Int_Handler(void) {
    ISR_PROLOG()
    /* Kick the dog  */
    adi_wdt_Kick();
    /* IF(Application supplied a callback) */
    if(gAppCallback != NULL) {
        /* Call the callback */
        gAppCallback(NULL, 0x0u, NULL);
    } /* ENDIF */
    ISR_EPILOG()
}
#endif /* (ADI_WDT_CONTROL_TIMEOUT_MODE == 1u) */

/*! \endcond */

/*! @} */
