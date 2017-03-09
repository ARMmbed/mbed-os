/**
 ******************************************************************************
 * @file rtc_map.h
 * @brief Real Time Clock HW register map
 * @internal
 * @author  ON Semiconductor.
 * $Rev: 3008 $
 * $Date: 2014-10-16 18:42:48 +0530 (Thu, 16 Oct 2014) $
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
 * @ingroup rtc
 *
 * @details
 * <p>
 * Teal Time Clock HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a HOURef="../pdf/IPC7206_RTC_APB_DS_v1P0.pdf" target="_blank">
 * IPC7206 APB RTC Design Specification v1.0 </a>
 * </p>
 */

#ifndef RTC_MAP_H_
#define RTC_MAP_H_

#include "architecture.h"

/** Real Time Clock Control HW Structure Overlay */
typedef struct {
    __IO uint32_t SUB_SECOND_COUNTER;    /**<SUB SECOND Counter */   /* 0x4000F000 */
    __IO uint32_t SECOND_COUNTER;        /**<SECOND Counter */       /* 0x4000F004 */
    __IO uint32_t SUB_SECOND_ALARM;      /**< SUB SECOND alarm */    /* 0x4000F008 */
    __IO uint32_t SECOND_ALARM;          /**< SECOND alarm */        /* 0x4000F00c */
    union {
        struct {
            __IO uint32_t SUB_SEC_COUNTER_EN :1;    /**<Sub-second counter enable.  (1=count is enabled, 0=retain count value) */
            __IO uint32_t SEC_COUNTER_EN :1;        /**<Second counter enable. (1=count is enabled, 0=retain count value) */
            __IO uint32_t SUB_SECOND_INT_EN :1;     /**<Sub-second interrupt enable (1=interrupt enabled, 0=interrupt disabled) */
            __IO uint32_t SECOND_INT_EN :1;         /**<Second interrupt enable (1=interrupt enabled, 0=interrupt disabled) */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;            /* 0x4000F010 */
    union {
        struct {
            /**<Any write to the status register will clear the error bit. */
            __IO uint32_t SUB_SECOND_INT:1;             /**<Sub-second interrupt status.  (1=interrupt active, 0=no interrupt)*/
            __IO uint32_t SECOND_INT :1;                /**<Second interrupt status.  (1=interrupt active, 0=no interrupt)*/
            __IO uint32_t WRITE_ERROR :1;               /**<Reads error bit which is set when a write occurs before a previous write to the same register has completed. */
            __IO uint32_t BSY_ANY_WRT :1;               /**<Busy with any write.*/
            __IO uint32_t BSY_SUB_SEC_CNTR_REG_WRT :1;  /**<Busy with a sub-second counter register write.*/
            __IO uint32_t BSY_SEC_CNTR_REG_WRT :1;      /**<Busy with a second counter register write.*/
            __IO uint32_t BSY_SUB_SEC_ALRM_REG_WRT :1;  /**<Busy with a sub-second alarm register write.*/
            __IO uint32_t BSY_SEC_ALRM_REG_WRT:1;       /**<Busy with a second alarm register write.*/
            __IO uint32_t BSY_CTRL_REG_WRT :1;          /**<Busy with a control register write.*/
            __IO uint32_t BSY_SUB_SEC_INT_CLR_WRT :1;   /**<Busy with a sub-second interrupt clear write.*/
            __IO uint32_t BSY_SEC_INT_CLR_WRT :1;       /**<Busy with a second interrupt clear write.*/
        } BITS;
        __IO uint32_t WORD;
    } STATUS;            /* 0x4000F014 */
    union {
        struct {
            __O uint32_t SUB_SECOND :1;        /**<Write 1 to this register to clear the sub-second interrupt.*/
            __O uint32_t SECOND :1;            /**<Write 1 to this register to clear the second interrupt.*/
        } BITS;
        __O uint32_t WORD;
    } INT_CLEAR;            /* 0x4000F018 */
} RtcReg_t, *RtcReg_pt;

#endif /* RTC_MAP_H_ */
