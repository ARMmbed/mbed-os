/* mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2016-2020 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************

 */
#include "cmsis.h"
#include "pwmout_api.h"
#include "pwmout_device.h"

#if DEVICE_PWMOUT

const pwm_apb_map_t pwm_apb_map_table[] = {
#if defined(TIM2_BASE)
    {PWM_2, PWMOUT_ON_APB1},
#endif
#if defined(TIM3_BASE)
    {PWM_3, PWMOUT_ON_APB1},
#endif
#if defined(TIM21_BASE)
    {PWM_21, PWMOUT_ON_APB2},
#endif
#if defined(TIM22_BASE)
    {PWM_22, PWMOUT_ON_APB2},
#endif
    {(PWMName) 0, PWMOUT_UNKNOWN}
};

#endif // DEVICE_PWMOUT
