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
#define SYS_TIM_ID      1   // the G-Timer ID for System
#define APP_TIM_ID      6   // the G-Timer ID for Application

static int us_ticker_inited = 0;
static TIMER_ADAPTER TimerAdapter;

extern HAL_TIMER_OP HalTimerOp;
extern HAL_TIMER_OP_EXT HalTimerOpExt;

VOID _us_ticker_irq_handler(IN  VOID *Data)
{
    us_ticker_irq_handler();
}

void us_ticker_init(void) 
{
    
    if (us_ticker_inited) return;
    us_ticker_inited = 1;
    

    // Initial a G-Timer
    TimerAdapter.IrqDis = 0;    // Enable Irq @ initial
    TimerAdapter.IrqHandle.IrqFun = (IRQ_FUN) _us_ticker_irq_handler;
    TimerAdapter.IrqHandle.IrqNum = TIMER2_7_IRQ;
    TimerAdapter.IrqHandle.Priority = 10;
    TimerAdapter.IrqHandle.Data = (u32)NULL;
    TimerAdapter.TimerId = APP_TIM_ID;
    TimerAdapter.TimerIrqPriority = 0;
    TimerAdapter.TimerLoadValueUs = 0xFFFFFFFF;
    TimerAdapter.TimerMode = USER_DEFINED;

    HalTimerOp.HalTimerInit((VOID*) &TimerAdapter);

    DBG_TIMER_INFO("%s: Timer_Id=%d\n", __FUNCTION__, APP_TIM_ID);
}

uint32_t us_ticker_read() 
{
    uint32_t tick_cnt;
    uint32_t ticks_125ms;
    uint32_t ticks_remain;
    uint64_t us_tick;

    tick_cnt = HalTimerOp.HalTimerReadCount(SYS_TIM_ID);
    tick_cnt = 0xffffffff - tick_cnt;   // it's a down counter
    ticks_125ms = tick_cnt/(GTIMER_CLK_HZ/8);  //use 125ms as a intermediate unit; 
    ticks_remain = tick_cnt - (ticks_125ms*(GTIMER_CLK_HZ/8));  //calculate the remainder
    us_tick = ticks_125ms * 125000;  //change unit to us, 125ms is 125000 us
    us_tick += (ticks_remain * 1000000)/GTIMER_CLK_HZ;  //also use us as unit

    return ((uint32_t)us_tick);  //return ticker value in micro-seconds (us)
}

void us_ticker_set_interrupt(timestamp_t timestamp) 
{
    uint32_t cur_time_us;
    uint32_t time_def;


    cur_time_us = us_ticker_read();
    if ((uint32_t)timestamp >= cur_time_us) {
        time_def = (uint32_t)timestamp - cur_time_us;
    } else {
        time_def = 0xffffffff - cur_time_us + (uint32_t)timestamp;
    }    

    if (time_def < TIMER_TICK_US) {
        time_def = TIMER_TICK_US;       // at least 1 tick
    }
    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
    HalTimerOpExt.HalTimerReLoad((u32)TimerAdapter.TimerId, time_def);
    HalTimerOpExt.HalTimerIrqEn((u32)TimerAdapter.TimerId);
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);

}

void us_ticker_disable_interrupt(void) 
{
    HalTimerOp.HalTimerDis((u32)TimerAdapter.TimerId);
}

void us_ticker_clear_interrupt(void) 
{
    HalTimerOp.HalTimerIrqClear((u32)TimerAdapter.TimerId);
}
