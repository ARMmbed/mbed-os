/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#ifndef MBED_PWMOUT_DEVICE_H
#define MBED_PWMOUT_DEVICE_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEVICE_PWMOUT

typedef enum {
    PWMOUT_ON_APB1 = 0,
    PWMOUT_UNKNOWN = 1
} PwmoutApb;

/*  Structure to describe Timers to APB */
typedef struct pwm_apb_map {
    PWMName pwm;   // an index entry for each EXIT line
    PwmoutApb pwmoutApb;
} pwm_apb_map_t;

/*  there is only 1 APB clock in F0 family */
#define PWMOUT_APB2_NOT_SUPPORTED

extern const pwm_apb_map_t pwm_apb_map_table[];

#endif // DEVICE_PWMOUT

#ifdef __cplusplus
}
#endif

#endif
