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

#ifdef REVB
/** Watch Dog Timer Control HW Structure Overlay */
typedef struct {
    __IO uint32_t LOAD; /**< Watchdog load value */
    __I uint32_t VALUE; /**< Watchdog current value */
    union {
        struct {
            __IO uint32_t INT_EN :1; /**< interrupt event : 0 = disable counter and interrupt , 1 = enable counter and interrupt */
            __IO uint32_t RESET_EN :1; /**< Watchdog reset output : 0 = disable 1 = enable */
            __IO uint32_t PAD :30; /**< Reserved, read undefined, must read as zeros. */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;
    __IO uint32_t INT_CLEAR; /**< Watchdog interrupt clear  */
    __I uint32_t RAW_INT_STAT; /**< Raw interrupt status from the counter  */
    __I uint32_t MASKED_INT_STAT; /**< Enabled interrupt status from the counter */
    union {
        struct {
            __IO uint32_t WRITE_EN :1; /**< write access to all other registers : 0 = enabled(default) , 1 = disabled */
            __IO uint32_t REG_WRITE_EN :31; /**< Enable write access to all other registers by writing 0x1ACCE551. Disable it by writing any other value.*/
        } BITS;
        __IO uint32_t WORD;
    } LOCK;
    __I uint32_t TEST_CTRL; /**< Integration Test Mode : 0 = disable , 1 = Enable  */
    union {
        struct {
            __IO uint32_t VAL_INT :1; /**< Value output on WDOGINT when in Integration Test Mode */
            __IO uint32_t VAL_RES :1; /**< Value output on WDOGRES when in Integration Test Mode */
            __IO uint32_t PAD:30; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } TEST_OUT;
    union {
        struct {
            __IO uint32_t PART_0 :8; /**< These bits read back as 0x05 */
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PID_REG0;
    union {
        struct {
            __IO uint32_t PART_1 :4; /**< These bits read back as 0x08 */
            __IO uint32_t DESIGNER_0 :4; /**< These bits read back as 0x01 */
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PID_REG1;
    union {
        struct {
            __IO uint32_t DESIGNER_1 :4; /**< These bits read back as 0x4 */
            __IO uint32_t REVISION :4; /**< These bits read back as 0x0*/
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PID_REG2;
    union {
        struct {
            __IO uint32_t CONFIG :8; /**< These bits read back as 0x00 */
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PID_REG3;
    union {
        struct {
            __IO uint32_t ID0 :8; /**< These bits read back as 0x0D */
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PCELL_ID0;
    union {
        struct {
            __IO uint32_t ID :8; /**< These bits read back as 0xF0*/
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PCELL_ID1;
    union {
        struct {
            __IO uint32_t ID :8; /**< These bits read back as 0x05*/
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PCELL_ID2;
    union {
        struct {
            __IO uint32_t ID :8; /**< These bits read back as 0xB1*/
            __IO uint32_t PAD :24; /**< Reserved, read undefined, must read as zeros.*/
        } BITS;
        __IO uint32_t WORD;
    } PCELL_ID3;
} WdtReg_t, *WdtReg_pt;
#endif /* REVB */

#ifdef REVD
typedef struct {
    __IO uint32_t LOAD; 		/**< 0x4000A000 Contains the value from which the counter is decremented.  When this register is written to the count is immediately restarted from the new value.  The minimum valid value is 1. */
    __I uint32_t CURRENT_VALUE; /**< 0x4000A004 Gives the current value of the decrementing counter */
    union {
        struct {
            __IO uint32_t WDT_EN :1; 	/**< Watchdog enable, 0 – Watchdog disabled, 1 – Watchdog enabled */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;			/* 0x4000A008 */
    __O uint32_t KICK; 	/**< 0x4000A00C A write of any value to this register reloads the value register from the load register */
    __O uint32_t LOCK; 	/**< 0x4000A010 Use of this register causes write-access to all other registers to be disabled.  This is to prevent rogue software from disabling the watchdog functionality.  Writing a value of 0x1ACCE551 enables write access to all other registers.  Writing any other value disables write access.  A read from this register only returns the bottom bit…, 0 – Write access is enabled, 1 – Write access is disabled */
    union {
        struct {
            __I uint32_t WRITE_BUSY_ANY :1; 	/**< Busy writing any register */
            __I uint32_t WRITE_BUSY_LOAD :1; 	/**< Busy writing the load register */
            __I uint32_t WRITE_BUSY_CONTROL :1; /**< Busy writing the control enable register */
            __IO uint32_t WRITE_ERROR :1;		/**< Error bit.  Set when write occurs before previous write completes (busy) */
        } BITS;
        __IO uint32_t WORD;
    } STATUS;	/* 0x4000A014 */
} WdtReg_t, *WdtReg_pt;
#endif /* REVD */
#endif /* WDT_MAP_H_ */
