/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __US_TICKER_DATA_H
#define __US_TICKER_DATA_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx.h"
#include "stm32l4xx_ll_tim.h"
#include "cmsis_nvic.h"
   
#define TIM_MST      TIM5
#define TIM_MST_IRQ  TIM5_IRQn
#define TIM_MST_RCC  __HAL_RCC_TIM5_CLK_ENABLE()
#define TIM_MST_DBGMCU_FREEZE  __HAL_DBGMCU_FREEZE_TIM5()

#define TIM_MST_RESET_ON   __HAL_RCC_TIM5_FORCE_RESET()
#define TIM_MST_RESET_OFF  __HAL_RCC_TIM5_RELEASE_RESET()

#define TIM_MST_BIT_WIDTH  32 // 16 or 32

#define TIM_MST_PCLK  1 // Select the peripheral clock number (1 or 2)

#define HAL_TICK_DELAY (1000) // 1 ms

#ifdef __cplusplus
}
#endif

#endif // __US_TICKER_DATA_H
