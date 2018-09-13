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
#include "lp_ticker_api.h"
#include "PeripheralNames.h"

#if DEVICE_LPTICKER

#define TICK_READ_FROM_CPU     0   // 1: read tick from CPU, 0: read tick from G-Timer
#define LP_SYS_TIM_ID          1   // the G-Timer ID for System
#define LP_APP_TIM_ID          4   // the G-Timer ID for Application

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

static int if_lp_ticker_init_inited = 0;
static TIMER_ADAPTER TimerAdapter;
extern HAL_TIMER_OP HalTimerOp;
extern HAL_TIMER_OP_EXT HalTimerOpExt;

static int if_lp_dis_irq = 0;
static int lp_ticker_timer_inited = 0;

void _lp_ticker_irq_handler(void)
{
    if (if_lp_dis_irq == 0) {
        lp_ticker_irq_handler();
        if_lp_dis_irq = 1;
    }
}

/* HAL lp ticker */

/** Initialize the low power ticker
 *
 * Initialize or re-initialize the ticker. This resets all the
 * clocking and prescaler registers, along with disabling
 * the compare interrupt.
 *
 * Pseudo Code:
 * @code
 * void lp_ticker_init()
 * {
 *     // Enable clock gate so processor can read LPTMR registers
 *     POWER_CTRL |= POWER_CTRL_LPTMR_Msk;
 *
 *     // Disable the timer and ensure it is powered down
 *     LPTMR_CTRL &= ~(LPTMR_CTRL_ENABLE_Msk | LPTMR_CTRL_COMPARE_ENABLE_Msk);
 *
 *     // Configure divisors - no division necessary
 *     LPTMR_PRESCALE = 0;
 *     LPTMR_CTRL |= LPTMR_CTRL_ENABLE_Msk;
 *
 *     // Install the interrupt handler
 *     NVIC_SetVector(LPTMR_IRQn, (uint32_t)lp_ticker_irq_handler);
 *     NVIC_EnableIRQ(LPTMR_IRQn);
 * }
 * @endcode
 */
void lp_ticker_init(void)
{
    // Initial a G-Timer
    TimerAdapter.IrqDis = 1;    // Disable Irq
    if_lp_dis_irq = 1;

    if (if_lp_ticker_init_inited == 1) {
        return;
    }

    TimerAdapter.IrqHandle.IrqFun = (IRQ_FUN) _lp_ticker_irq_handler;
    TimerAdapter.IrqHandle.IrqNum = TIMER2_7_IRQ;
    TimerAdapter.IrqHandle.Priority = 10;
    TimerAdapter.IrqHandle.Data = (u32)NULL;
    TimerAdapter.TimerId = LP_APP_TIM_ID;
    TimerAdapter.TimerIrqPriority = 0;
    TimerAdapter.TimerLoadValueUs = 1;

    TimerAdapter.TimerMode = FREE_RUN_MODE; // Countdown Free Run

    if (lp_ticker_timer_inited == 0) {
        HalTimerOp.HalTimerInit((void *) &TimerAdapter);
        lp_ticker_timer_inited = 1;
        HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);
    }

    if_lp_ticker_init_inited = 1;

    //DBG_TIMER_INFO("%s: Timer_Id=%d\n", __FUNCTION__, LP_APP_TIM_ID);
}

/** Deinitialize the lower power ticker
 *
 * Powerdown the lp ticker in preparation for sleep, powerdown, or reset.
 *
 * After calling this function no other ticker functions should be called except
 * lp_ticker_init(). Calling any function other than init after freeing is
 * undefined.
 *
 * @note This function stops the ticker from counting.
 */
void lp_ticker_free(void)
{
    HalTimerDeInit((void*) &TimerAdapter);
    if_lp_ticker_init_inited = 0;
    lp_ticker_timer_inited = 0;
}

/** Read the current tick
 *
 * If no rollover has occurred, the seconds passed since lp_ticker_init()
 * was called can be found by dividing the ticks returned by this function
 * by the frequency returned by ::lp_ticker_get_info.
 *
 * @return The current timer's counter value in ticks
 *
 * Pseudo Code:
 * @code
 * uint32_t lp_ticker_read()
 * {
 *     uint16_t count;
 *     uint16_t last_count;
 *
 *     // Loop until the same tick is read twice since this
 *     // is ripple counter on a different clock domain.
 *     count = LPTMR_COUNT;
 *     do {
 *         last_count = count;
 *         count = LPTMR_COUNT;
 *     } while (last_count != count);
 *
 *     return count;
 * }
 * @endcode
 */

static uint32_t tick_cnt;

uint32_t lp_ticker_read(void)
{
    //core_util_critical_section_enter();
    tick_cnt = HalTimerOp.HalTimerReadCount(LP_SYS_TIM_ID);
    tick_cnt = 0xffffffff - tick_cnt;
    //core_util_critical_section_exit();

    return tick_cnt;
}

uint32_t lp_TICK_TO_US(uint32_t cur_tick)
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

void lp_ticker_set_interrupt(timestamp_t timestamp) 
{
    core_util_critical_section_enter();

    if (timestamp == 0) {
        time_def = TIMER_TICK_US;
    } else {
        cur_time_us = lp_ticker_read();

        if ((uint32_t)timestamp >= cur_time_us) {
            time_def = (uint32_t)timestamp - cur_time_us;
        } else {
            time_def = 0xffffffff - cur_time_us + (uint32_t)timestamp;
        }

        time_def = lp_TICK_TO_US(time_def);

        if (time_def < TIMER_TICK_US) {
            time_def = TIMER_TICK_US;
        }
    }

    if (lp_ticker_timer_inited == 1) {
        HalTimerDeInit((void*) &TimerAdapter);
        lp_ticker_timer_inited = 0;
    }

    TimerAdapter.IrqDis = 0;    // Enable Irq
    if_lp_dis_irq = 0;

    TimerAdapter.TimerLoadValueUs = time_def;
    TimerAdapter.TimerMode = USER_DEFINED;

    HalTimerOp.HalTimerInit((void*) &TimerAdapter);
    lp_ticker_timer_inited = 1;
    HalTimerOp.HalTimerEn((u32)TimerAdapter.TimerId);

    core_util_critical_section_exit();
}

void lp_ticker_disable_interrupt(void)
{
    if_lp_dis_irq = 1;
}

void lp_ticker_clear_interrupt(void)
{
    HalTimerOp.HalTimerIrqClear((u32)TimerAdapter.TimerId);
}

void lp_ticker_fire_interrupt(void)
{
    lp_ticker_set_interrupt(0);
}

const ticker_info_t* lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        32768,      // 32KHz
        32          // 32 bit counter
    };
    return &info;
}

#endif //lp_ticker_api.c
