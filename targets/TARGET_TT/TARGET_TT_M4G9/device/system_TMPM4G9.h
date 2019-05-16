/**
 *****************************************************************************
 * @file     system_TMPM4G9.h
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Header File for the
 *           TOSHIBA 'TMPM4G9' Device Series 
 * @version V0.0.0.0
 * $Date:: 2018-04-02 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *****************************************************************************
 */

#include <stdint.h>

#ifndef __SYSTEM_TMPM4G9_H
#define __SYSTEM_TMPM4G9_H

#ifdef __cplusplus
extern "C" {
#endif 

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */
extern uint32_t CoreClockInput;		 /*!< High speed Clock Frequency */


/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif
