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

#define TICK_READ_FROM_CPU  0   // 1: read tick from CPU, 0: read tick from G-Timer
#define SYS_TIM_ID          1   // the G-Timer ID for System
#define APP_TIM_ID          6   // the G-Timer ID for Application

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
#define MS_TO_TICK(x)       (uint64_t)(((x)*327) / 10)
#define US_TO_TICK(x)       (uint64_t)(((x)*32) / 1000)
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

    DBG_TIMER_INFO("%s: Timer_Id=%d\n", __FUNCTION__, APP_TIM_ID);
}

uint32_t us_ticker_read(void)
{
    uint32_t tick_cnt;
    uint64_t tick_us;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    tick_cnt = HalTimerOp.HalTimerReadCount(SYS_TIM_ID);
    tick_us = TICK_TO_US(0xFFFFFFFFUL - tick_cnt);

    return ((uint32_t)tick_us);  //return ticker value in micro-seconds (us)
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t time_cur;
    uint32_t time_cnt;

    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
    time_cur = us_ticker_read();
    if (timestamp > time_cur + TIMER_TICK_US) {
        time_cnt = timestamp - time_cur;
    } else {
        HalTimerOpExt.HalTimerReLoad((u32)TimerAdapter.TimerId, 0xffffffff);
        HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
        us_ticker_fire_interrupt();
        return;
    }

    TimerAdapter.TimerLoadValueUs = MAX(MS_TO_TICK(time_cnt/1000) + US_TO_TICK(time_cnt%1000), 1);
    HalTimerOpExt.HalTimerReLoad((u32)TimerAdapter.TimerId, TimerAdapter.TimerLoadValueUs);
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(TIMER2_7_IRQ);
}

void us_ticker_disable_interrupt(void)
{
    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
}

void us_ticker_clear_interrupt(void)
{
    HalTimerOp.HalTimerIrqClear((u32)TimerAdapter.TimerId);
}
