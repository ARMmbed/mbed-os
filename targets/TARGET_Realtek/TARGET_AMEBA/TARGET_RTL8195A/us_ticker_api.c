/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

//#if DEVICE_USTICKER

#define TICK_READ_FROM_CPU     0   // 1: read tick from CPU, 0: read tick from G-Timer
#define US_SYS_TIM_ID          1   // the G-Timer ID for System
#define US_APP_TIM_ID          3   // the G-Timer ID for Application

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

static int if_us_ticker_init_inited = 0;
static TIMER_ADAPTER TimerAdapter;
extern HAL_TIMER_OP HalTimerOp;
extern HAL_TIMER_OP_EXT HalTimerOpExt;

static int if_us_ticker_fire_interrupt = 0;
static int if_us_dis_irq = 0;
static int if_us_ticker_timer_inited = 0;
static int if_us_ticker_disable_read = 0;

void rtw_us_ticker_disable_read(void)
{
    if_us_ticker_disable_read = 1;
    us_ticker_free();
}

void _us_ticker_irq_handler(void *Data)
{
    if (if_us_dis_irq == 0) {
        us_ticker_irq_handler();
        if_us_dis_irq = 1;
    }
}

void us_ticker_init(void)
{
    // Initial a G-Timer
    TimerAdapter.IrqDis = 1;    // Disable Irq
    if_us_dis_irq = 1;

    if (if_us_ticker_init_inited == 1) {
        return;
    }

    TimerAdapter.IrqHandle.IrqFun = (IRQ_FUN) _us_ticker_irq_handler;
    TimerAdapter.IrqHandle.IrqNum = TIMER2_7_IRQ;
    TimerAdapter.IrqHandle.Priority = 10;
    TimerAdapter.IrqHandle.Data = (u32)NULL;
    TimerAdapter.TimerId = US_APP_TIM_ID;
    TimerAdapter.TimerIrqPriority = 0;
    TimerAdapter.TimerLoadValueUs = 1;
    TimerAdapter.TimerMode = FREE_RUN_MODE; // Countdown Free Run

    if (if_us_ticker_timer_inited == 0) {
        HalTimerOp.HalTimerInit((void *) &TimerAdapter);
        if_us_ticker_timer_inited = 1;
        HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
    }

    if_us_ticker_init_inited = 1;

    //DBG_TIMER_INFO("%s: Timer_Id=%d\n", __FUNCTION__, US_APP_TIM_ID);
}

void us_ticker_free(void)
{
    HalTimerDeInit((void*) &TimerAdapter);
    if_us_ticker_init_inited = 0;
    if_us_ticker_timer_inited = 0;
}

static uint32_t tick_cnt;
static uint32_t before_tick_cnt = 0;

uint32_t us_ticker_read(void)
{
    tick_cnt = HalTimerOp.HalTimerReadCount(US_SYS_TIM_ID);

    if (if_us_ticker_disable_read == 0) {
        tick_cnt = 0xffffffff - tick_cnt;
        before_tick_cnt = tick_cnt;
        return tick_cnt;
    } else {
        if_us_ticker_disable_read = 0;
        return before_tick_cnt;
    }
}

uint32_t us_TICK_TO_US(uint32_t cur_tick)
{
    static uint32_t ticks_125ms;
    static uint32_t ticks_remain;
    static uint64_t us_tick;

    ticks_125ms = cur_tick/(GTIMER_CLK_HZ/8);
    ticks_remain = cur_tick - (ticks_125ms*(GTIMER_CLK_HZ/8));
    us_tick = ticks_125ms * 125000;
    us_tick += (ticks_remain * 1000000)/GTIMER_CLK_HZ;

    return ((uint32_t)us_tick);
}


static uint32_t cur_time_us;
static uint32_t time_def;

void us_ticker_set_interrupt(timestamp_t timestamp) 
{
    core_util_critical_section_enter();

    if (timestamp == 0) {
        time_def = TIMER_TICK_US;
    } else {
        cur_time_us = us_ticker_read();

        if ((uint32_t)timestamp >= cur_time_us) {
            time_def = (uint32_t)timestamp - cur_time_us;
        } else {
            time_def = 0xffffffff - cur_time_us + (uint32_t)timestamp;
        }

        time_def = us_TICK_TO_US(time_def);

        if (time_def < TIMER_TICK_US) {
            time_def = TIMER_TICK_US;
        }
    }

    if (if_us_ticker_timer_inited == 1) {
        HalTimerDeInit((void*) &TimerAdapter);
        if_us_ticker_timer_inited = 0;
    }

    TimerAdapter.IrqDis = 0;
    if_us_dis_irq = 0;

    TimerAdapter.TimerLoadValueUs = time_def;
    TimerAdapter.TimerMode = USER_DEFINED;
    HalTimerOp.HalTimerInit((void*) &TimerAdapter);
    if_us_ticker_timer_inited = 1;
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);

    core_util_critical_section_exit();
}

void us_ticker_disable_interrupt(void)
{
    if_us_dis_irq = 1;
}

void us_ticker_clear_interrupt(void)
{
    HalTimerOp.HalTimerIrqClear((u32)TimerAdapter.TimerId);
}

void us_ticker_fire_interrupt(void)
{
    us_ticker_set_interrupt(0);
}

const ticker_info_t* us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        32768,      // 32KHz
        32          // 32 bit counter
    };
    return &info;
}
//#endif //us_ticker_api.c

