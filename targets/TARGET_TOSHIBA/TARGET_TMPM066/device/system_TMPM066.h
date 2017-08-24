/**
 *****************************************************************************
 * @file     system_TMPM066.h
 * @brief    CMSIS Cortex-M0 Device Peripheral Access Layer Header File for the
 *           TOSHIBA 'TMPM066' Device Series 
 * @version  V2.0.2.1 (Tentative)
 * @date     2015/8/13
 *
 * (C)Copyright TOSHIBA CORPORATION 2015 All rights reserved
 *****************************************************************************
 */

#include <stdint.h>

#ifndef __SYSTEM_TMPM066_H
#define __SYSTEM_TMPM066_H

#ifdef __cplusplus
extern "C" {
#endif

    extern uint32_t SystemCoreClock;    /*!< System Clock Frequency (Core Clock)  */

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
    extern void SystemInit(void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock 
 *         retrieved from cpu registers.
 */
    extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif
#endif
