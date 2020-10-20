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

#ifndef __US_TICKER_DATA_H
#define __US_TICKER_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx.h"
#include "stm32h7xx_ll_tim.h"
#include "cmsis_nvic.h"

#if defined(DUAL_CORE)
#if defined(CORE_CM7)
#define TIM_MST      TIM5
#define TIM_MST_IRQ  TIM5_IRQn
#define TIM_MST_RCC  __TIM5_CLK_ENABLE()
#define TIM_MST_DBGMCU_FREEZE  __HAL_DBGMCU_FREEZE_TIM5()

#define TIM_MST_RESET_ON   __TIM5_FORCE_RESET()
#define TIM_MST_RESET_OFF  __TIM5_RELEASE_RESET()

#define TIM_MST_BIT_WIDTH  32 // 16 or 32

#define TIM_MST_PCLK  1 // Select the peripheral clock number (1 or 2)

#elif defined(CORE_CM4)
#define TIM_MST      TIM2
#define TIM_MST_IRQ  TIM2_IRQn
#define TIM_MST_RCC  __TIM2_CLK_ENABLE()
#define TIM_MST_DBGMCU_FREEZE  __HAL_DBGMCU_FREEZE_TIM2()

#define TIM_MST_RESET_ON   __TIM2_FORCE_RESET()
#define TIM_MST_RESET_OFF  __TIM2_RELEASE_RESET()

#define TIM_MST_BIT_WIDTH  32 // 16 or 32

#define TIM_MST_PCLK  1 // Select the peripheral clock number (1 or 2)
#else
#error "Core not supported"
#endif
#else
#define TIM_MST      TIM5
#define TIM_MST_IRQ  TIM5_IRQn
#define TIM_MST_RCC  __TIM5_CLK_ENABLE()
#define TIM_MST_DBGMCU_FREEZE  __HAL_DBGMCU_FREEZE_TIM5()

#define TIM_MST_RESET_ON   __TIM5_FORCE_RESET()
#define TIM_MST_RESET_OFF  __TIM5_RELEASE_RESET()

#define TIM_MST_BIT_WIDTH  32 // 16 or 32

#define TIM_MST_PCLK  1 // Select the peripheral clock number (1 or 2)
#endif


#ifdef __cplusplus
}
#endif

#endif // __US_TICKER_DATA_H

