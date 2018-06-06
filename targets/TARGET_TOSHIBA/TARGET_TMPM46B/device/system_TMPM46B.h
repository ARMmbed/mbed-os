/**
 *****************************************************************************
 * @file     system_TMPM46B.h
 * @brief    CMSIS Cortex-M4 Device Peripheral Access Layer Header File for the
 *           TOSHIBA 'TMPM46B' Device Series 
 * @version  V2.0.2.1
 * @date     2014/12/03
 * 
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 * 
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *****************************************************************************
 */

#ifndef __SYSTEM_TMPM46B_H
#define __SYSTEM_TMPM46B_H

#ifdef __cplusplus
extern "C" {
#endif 

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

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
