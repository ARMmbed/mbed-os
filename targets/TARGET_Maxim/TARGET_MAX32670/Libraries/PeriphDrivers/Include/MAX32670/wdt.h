/**
 * @file    wdt.h
 * @brief   Watchdog timer (WDT) function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _WDT_H_
#define _WDT_H_

#ifdef __CC_ARM 
#pragma diag_suppress 66  // enumeration value is out of "int" range
#endif

/* **** Includes **** */
#include <stdint.h>
#include "mxc_device.h"
#include "wdt_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup wdt WDT
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

/** @brief Watchdog upper limit period enumeration.
    Used to configure the period of the watchdog interrupt */
typedef enum {
    MXC_WDT_PERIOD_2_31 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW31, ///< Period 2^31 
    MXC_WDT_PERIOD_2_30 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW30, ///< Period 2^30 
    MXC_WDT_PERIOD_2_29 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW29, ///< Period 2^29 
    MXC_WDT_PERIOD_2_28 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW28, ///< Period 2^28 
    MXC_WDT_PERIOD_2_27 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW27, ///< Period 2^27 
    MXC_WDT_PERIOD_2_26 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW26, ///< Period 2^26 
    MXC_WDT_PERIOD_2_25 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW25, ///< Period 2^25 
    MXC_WDT_PERIOD_2_24 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW24, ///< Period 2^24 
    MXC_WDT_PERIOD_2_23 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW23, ///< Period 2^23 
    MXC_WDT_PERIOD_2_22 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW22, ///< Period 2^22 
    MXC_WDT_PERIOD_2_21 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW21, ///< Period 2^21 
    MXC_WDT_PERIOD_2_20 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW20, ///< Period 2^20 
    MXC_WDT_PERIOD_2_19 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW19, ///< Period 2^19 
    MXC_WDT_PERIOD_2_18 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW18, ///< Period 2^18 
    MXC_WDT_PERIOD_2_17 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW17, ///< Period 2^17 
    MXC_WDT_PERIOD_2_16 = MXC_S_WDT_CTRL_INT_LATE_VAL_WDT2POW16, ///< Period 2^16 
} mxc_wdt_period_t;

/**
 * @brief Watchdog interrupt flag enumeration
 */
typedef enum {
    MXC_WDT_INT_TOO_LATE = MXC_F_WDT_CTRL_INT_LATE,
    MXC_WDT_INT_TOO_SOON = MXC_F_WDT_CTRL_INT_EARLY,
} mxc_wdt_int_t;

/**
 * @brief Watchdog reset flag enumeration
 */
typedef enum {
    MXC_WDT_RST_TOO_LATE = MXC_F_WDT_CTRL_RST_LATE,
    MXC_WDT_RST_TOO_SOON = MXC_F_WDT_CTRL_RST_EARLY,
} mxc_wdt_rst_t;

/**
 * @brief Watchdog mode enumeration
 */
typedef enum {
    MXC_WDT_COMPATIBILITY = 0,
    MXC_WDT_WINDOWED = 1,
} mxc_wdt_mode_t;

/**
 * @brief Timer Configuration
 */
typedef struct {
    mxc_wdt_mode_t   mode;               ///< WDT mode
    mxc_wdt_period_t upperResetPeriod;   ///< Reset upper limit
    mxc_wdt_period_t lowerResetPeriod;   ///< Reset lower limit
    mxc_wdt_period_t upperIntPeriod;     ///< Interrupt upper limit
    mxc_wdt_period_t lowerIntPeriod;     ///< Interrupt lower limit
} mxc_wdt_cfg_t;
/* **** Function Prototypes **** */

/**
 * @brief Initialize the Watchdog Timer
 * @param       wdt     Pointer to the watchdog registers
 * @param       cfg     watchdog configuration
 * @return      See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_WDT_Init (mxc_wdt_regs_t* wdt, mxc_wdt_cfg_t *cfg);

/**
 * @brief Shutdown the Watchdog Timer
 * @param       wdt     Pointer to the watchdog registers
 * @return      See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_WDT_Shutdown (mxc_wdt_regs_t* wdt);

/**
 * @brief       Set the period of the watchdog interrupt.
 * @param       wdt     Pointer to watchdog registers.
 * @param       cfg     watchdog configuration.
 */
void MXC_WDT_SetIntPeriod (mxc_wdt_regs_t* wdt, mxc_wdt_cfg_t *cfg);

/**
 * @brief       Set the period of the watchdog reset.
 * @param       wdt     Pointer to watchdog registers.
 * @param       cfg     watchdog configuration.
 */
void MXC_WDT_SetResetPeriod (mxc_wdt_regs_t* wdt, mxc_wdt_cfg_t *cfg);

/**
 * @brief       Enable the watchdog timer.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_Enable (mxc_wdt_regs_t* wdt);

/**
 * @brief       Disable the watchdog timer.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_Disable (mxc_wdt_regs_t* wdt);

/**
 * @brief       Enable the watchdog interrupt.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_EnableInt (mxc_wdt_regs_t* wdt);

/**
 * @brief       Disable the watchdog interrupt.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_DisableInt (mxc_wdt_regs_t* wdt);

/**
 * @brief       Enable the watchdog reset.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_EnableReset (mxc_wdt_regs_t* wdt);

/**
 * @brief       Disable the watchdog reset.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_DisableReset (mxc_wdt_regs_t* wdt);

/**
 * @brief       Reset the watchdog timer.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_ResetTimer (mxc_wdt_regs_t* wdt);

/**
 * @brief       Get the status of the reset flag.
 * @param       wdt     Pointer to watchdog registers.
 * @returns     1 if the previous reset was caused by the watchdog, 0 otherwise.
 */
int MXC_WDT_GetResetFlag (mxc_wdt_regs_t* wdt);

/**
 * @brief       Clears the reset flag.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_ClearResetFlag (mxc_wdt_regs_t* wdt);

/**
 * @brief       Get the status of the interrupt flag.
 * @param       wdt     Pointer to watchdog registers.
 * @returns     1 if the interrupt is pending, 0 otherwise.
 */
int MXC_WDT_GetIntFlag (mxc_wdt_regs_t* wdt);

/**
 * @brief       Clears the interrupt flag.
 * @param       wdt     Pointer to watchdog registers.
 */
void MXC_WDT_ClearIntFlag (mxc_wdt_regs_t* wdt);

/**@} end of group wdt */

#ifdef __cplusplus
}
#endif

#endif /* _WDT_H_ */
