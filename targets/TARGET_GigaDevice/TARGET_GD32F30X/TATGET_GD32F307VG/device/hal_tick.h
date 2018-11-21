/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
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

#include "gd32f30x.h"
#include "cmsis_nvic.h"

#define TICKER_TIMER                        TIMER2
#define TICKER_TIMER_IRQ                    TIMER2_IRQn
#define TICKER_TIMER_RCU_CLOCK_ENABLE       rcu_periph_clock_enable(RCU_TIMER2);
#define TICKER_TIMER_DEBUG_STOP             dbg_periph_enable(DBG_TIMER2_HOLD);

#define TICKER_TIMER_RESET_ENABLE           rcu_periph_reset_enable(RCU_TIMER2RST)
#define TICKER_TIMER_RESET_DISABLE          rcu_periph_reset_disable(RCU_TIMER2RST)

/* 16 for 16-bit timer, 32 for 32-bit timer */
#define TICKER_TIMER_WIDTH_BIT  16

/* 0 for CK_APB1, 1 for CK_APB2 */
#define TICKER_TIMER_CKAPB  1

#ifdef __cplusplus
}
#endif

#endif /* __HAL_TICK_H */
