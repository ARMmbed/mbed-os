/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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

#include "objects.h"
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"

#define SYS_TIM_ID          1   // the G-Timer ID for System
#define APP_TIM_ID          2   // the G-Timer ID for Application

/*
 * For RTL8195AM, clock source is 32k
 *
 *   us per tick: 30.5
 *   tick per ms: 32.7
 *   tick per us: 0.032
 *   tick per sec: 32768
 *
 * Define the following macros to convert between TICK and US.
 */
#define TICK_TO_US(x)       (uint64_t)(((x)/2) * 61 + ((x)%2) * TIMER_TICK_US)

static int us_ticker_inited = 0;
static TIMER_ADAPTER TimerAdapter;

extern HAL_TIMER_OP HalTimerOp;
extern HAL_TIMER_OP_EXT HalTimerOpExt;

VOID _us_ticker_irq_handler(void *Data)
{
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    us_ticker_inited = 1;

    // Reload and restart sys-timer
    HalTimerOp.HalTimerDis(SYS_TIM_ID);
    HalTimerOpExt.HalTimerReLoad(SYS_TIM_ID, 0xFFFFFFFFUL);
    HalTimerOp.HalTimerEn(SYS_TIM_ID);

    // Initial a app-timer
    TimerAdapter.IrqDis = 0;    // Enable Irq @ initial
    TimerAdapter.IrqHandle.IrqFun = (IRQ_FUN) _us_ticker_irq_handler;
    TimerAdapter.IrqHandle.IrqNum = TIMER2_7_IRQ;
    TimerAdapter.IrqHandle.Priority = 10;
    TimerAdapter.IrqHandle.Data = (u32)NULL;
    TimerAdapter.TimerId = APP_TIM_ID;
    TimerAdapter.TimerIrqPriority = 0;
    TimerAdapter.TimerLoadValueUs = 0xFFFFFFFF;
    TimerAdapter.TimerMode = USER_DEFINED;

    HalTimerOp.HalTimerInit((void *) &TimerAdapter);
}

uint32_t us_ticker_read(void)
{
    uint32_t tick_cnt;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    tick_cnt = HalTimerOp.HalTimerReadCount(SYS_TIM_ID);
    return (uint32_t)TICK_TO_US(0xFFFFFFFFUL - tick_cnt);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t time_cur;

    time_cur = us_ticker_read();
    if (timestamp > time_cur + TIMER_TICK_US) {
        TimerAdapter.TimerLoadValueUs = timestamp - time_cur;
    } else {
        TimerAdapter.TimerLoadValueUs = TIMER_TICK_US;
    }

    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
    HalTimerOpExt.HalTimerReLoad((u32)TimerAdapter.TimerId, TimerAdapter.TimerLoadValueUs);
    HalTimerOpExt.HalTimerSync(SYS_TIM_ID);
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
}

void us_ticker_fire_interrupt(void)
{
    TimerAdapter.TimerLoadValueUs = TIMER_TICK_US;

    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
    HalTimerOpExt.HalTimerReLoad((u32)TimerAdapter.TimerId, TimerAdapter.TimerLoadValueUs);
    HalTimerOpExt.HalTimerSync(SYS_TIM_ID);
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
}

void us_ticker_disable_interrupt(void)
{
    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
}

void us_ticker_clear_interrupt(void)
{
}
