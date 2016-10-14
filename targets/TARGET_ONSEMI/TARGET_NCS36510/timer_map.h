/**
 ******************************************************************************
 * @file timer_map.h
 * @brief Timer HW register map
 * @internal
 * @author ON Semiconductor
 * $Rev: 3423 $
 * $Date: 2015-06-09 11:16:49 +0530 (Tue, 09 Jun 2015) $
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
 * @ingroup timer
 *
 * @details
 * <p>
 * Timer HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../pdf/IPC7200_Timer_APB_DS_v1P2.pdf" target="_blank">
 * IPC7200 APB Timer Design Specification v1.2</a>
 * </p>
 */

#ifndef TIMER_MAP_H_
#define TIMER_MAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "architecture.h"

/** Timer HW Structure Overlay */
typedef struct {
    __IO uint32_t LOAD; /**< 16bit counter (re-)load value */
    __I uint32_t VALUE; /**< 16bit current counter value */
    union {
        struct {
            __IO uint32_t PAD0 :2;         /**< Always reads 0 */
            __IO uint32_t PRESCALE :3;     /**< 0:no division, 1..7: divide by 16, 256, 2, 8, 32, 128, 1024*/
            __IO uint32_t PAD1 :1;        /**< Always reads 0 */
            __IO uint32_t MODE :1;         /**< 0:free-run, 1:periodic */
            __IO uint32_t ENABLE :1;     /**< 0: disable, 1:enable */
            __I  uint32_t INT :1;         /**< interrupt status */
        } BITS;
        __IO uint32_t WORD;
    } CONTROL;
    __O uint32_t CLEAR;     /**< Write any value to clear the interrupt */
} TimerReg_t, *TimerReg_pt;

#ifdef __cplusplus
}
#endif

#endif /* TIMER_MAP_H_ */
