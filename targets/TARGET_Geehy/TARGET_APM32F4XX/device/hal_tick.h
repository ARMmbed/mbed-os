/* mbed Microcontroller Library
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef __HAL_TICK_H
#define __HAL_TICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f4xx.h"
#include "cmsis_nvic.h"
#include "apm32f4xx_tmr.h"

#define TICKER_TIMER                        TMR8
#define TICKER_TIMER_IRQ                    TMR8_CC_IRQn
#define TICKER_TIMER_RCM_CLOCK_ENABLE       RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR8);
#define TICKER_TIMER_DEBUG_STOP             DBGMCU_EnableAPB2Periph(DBGMCU_TMR8_STOP);

#define TICKER_TIMER_RESET_ENABLE           RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_TMR8)
#define TICKER_TIMER_RESET_DISABLE          RCM_EnableAPB2PeriphReset(RCM_APB2_PERIPH_TMR8)

/* 16 for 16-bit timer, 32 for 32-bit timer */
#define TICKER_TIMER_WIDTH_BIT  16

/* 0 for CK_APB1, 1 for CK_APB2 */
#define TICKER_TIMER_CKAPB  1

#ifdef __cplusplus
}
#endif

#endif /* __HAL_TICK_H */
