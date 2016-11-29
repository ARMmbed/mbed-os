/**
 ******************************************************************************
 * @file pwm_map.h
 * @brief PWM HW register map
 * @internal
 * @author ON Semiconductor
 * $Rev: 3378 $
 * $Date: 2015-04-28 13:38:36 +0530 (Tue, 28 Apr 2015) $
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
 * @ingroup pwm
 *
 * @details
 * <p>
 * PWM HW register map description
 * </p>
 *
 * <h1> Reference document(s) </h1>
 * <p>
 * <a href="../../../../build/doc/pdf/IPC7205_PWM_APB_DS_v1P1.pdf" target="_blank">
 * IPC7205 APB PWM Design Specification v1.1 </a>
 * </p>
 */

#ifndef PWM_MAP_H_
#define PWM_MAP_H_

#include "architecture.h"

/** Power management Control HW Structure Overlay */
typedef struct {
    __O uint32_t DUTYCYCLE;
    __O uint32_t PWM_ENABLE;
    __O uint32_t PWM_DISABLE;
    __O uint32_t PRESCALE_ENABLE;
    __O uint32_t PRESCALE_DISABLE;
    union {
        struct {
            __I uint32_t CUR_DUTY_CYCLE_VALUE : 8; /** Curent value of duty Cycle */
            __I uint32_t CUR_PRE_SCALER_STATE : 1; /** Current state of the prescaler.  ‘1’ the prescaler is enabled.  ‘0’ the prescaler is disabled. */
            __I uint32_t CUR_PWM_ENABLE_STATE : 1; /** Current state of the PWM.  ‘1’ the PWM is enabled.  ‘0’ the PWM is disabled. */
            __I uint32_t CUR_PWM_OUTPUT_STATE : 1; /** Current state of PWM output */
        } BITS;
        __I uint32_t WORD;
    } READ_CONFIG_STATUS;
} PwmReg_t, *PwmReg_pt;
#endif /* PWM_MAP_H_ */
