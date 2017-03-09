/**
 ******************************************************************************
 * @file wdt_map.h
 * @brief WDT HW register map
 * @internal
 * @author  ON Semiconductor
 * $Rev: 3283 $
 * $Date: 2015-02-26 18:52:22 +0530 (Thu, 26 Feb 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup wdt
 *
 * @details
 * <p>
 * Watchdog Timer HW register map description
 * </p>
 *
 */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#ifndef WDT_MAP_H_
#define WDT_MAP_H_

#include "architecture.h"

typedef struct {
    __IO uint32_t LOAD;         /**< 0x4000A000 Contains the value from which the counter is decremented.  When this register is written to the count is immediately restarted from the new value.  The minimum valid value is 1. */
    __I uint32_t CURRENT_VALUE; /**< 0x4000A004 Gives the current value of the decrementing counter */
    union {
        struct {
            __IO uint32_t WDT_EN :1;     /**< Watchdog enable, 0 – Watchdog disabled, 1 – Watchdog enabled */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;             /* 0x4000A008 */
    __O uint32_t KICK;     /**< 0x4000A00C A write of any value to this register reloads the value register from the load register */
    __O uint32_t LOCK;     /**< 0x4000A010 Use of this register causes write-access to all other registers to be disabled.  This is to prevent rogue software from disabling the watchdog functionality.  Writing a value of 0x1ACCE551 enables write access to all other registers.  Writing any other value disables write access.  A read from this register only returns the bottom bit…, 0 – Write access is enabled, 1 – Write access is disabled */
    union {
        struct {
            __I uint32_t WRITE_BUSY_ANY :1;     /**< Busy writing any register */
            __I uint32_t WRITE_BUSY_LOAD :1;    /**< Busy writing the load register */
            __I uint32_t WRITE_BUSY_CONTROL :1; /**< Busy writing the control enable register */
            __IO uint32_t WRITE_ERROR :1;       /**< Error bit.  Set when write occurs before previous write completes (busy) */
        } BITS;
        __IO uint32_t WORD;
    } STATUS;    /* 0x4000A014 */
} WdtReg_t, *WdtReg_pt;
#endif /* WDT_MAP_H_ */
