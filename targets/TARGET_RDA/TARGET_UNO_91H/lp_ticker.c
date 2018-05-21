/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "lp_ticker_api.h"

#if DEVICE_LOWPOWERTIMER

#include "us_ticker_api.h"
#include "rda_ccfg_api.h"
#include "mbed_critical.h"

#define LP_TICKER_TIMER             RDA_TIM1

#define TIMER1_CONTROL_ENABLE       (0x20)
#define TIMER1_CONTROL_MODE         (0x40)
#define TIMER1_CONTROL_INT_MSK      (0x80)

#define LP_TIMER_CLOCK_SOURCE       (32768)
#define US_TIMER_HZ                 (1000000)

static int32_t  lp_ticker_inited = 0;

int lp_ticker_soft_int_flag = 0;

#if DEVICE_SLEEP
extern unsigned int lpo_ticks_cal;
extern unsigned int lpo_tmr_flag;
extern signed int   lpo_delta_us;
#endif

extern void rda_timer_irq_set(void);

void lp_ticker_init(void)
{
    if (lp_ticker_inited)
        return;

    /* Enable apb timer clock */
    RDA_SCU->CLKGATE1 |= (1 << 3);

    /* Set timer load count */
    LP_TICKER_TIMER->LDCNT = 0xFFFFFFFF;

    /* Set timer mode */
    LP_TICKER_TIMER->TCTRL |= TIMER1_CONTROL_MODE;

    /* Enable timer */
    LP_TICKER_TIMER->TCTRL |= (TIMER1_CONTROL_ENABLE);

    rda_timer_irq_set();

    /* Set lp_ticker_inited true, after all settings done */
    lp_ticker_inited = 1;
}

uint32_t lp_ticker_read(void)
{
    if (!lp_ticker_inited)
        lp_ticker_init();

    /* Get timestamp from us_ticker */
    return us_ticker_read();
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    int32_t delta = (int32_t)(timestamp - lp_ticker_read());
    uint32_t delay_ticks = SystemCoreClock / LP_TIMER_CLOCK_SOURCE / 5;

    /* Disable timer */
    LP_TICKER_TIMER->TCTRL &= (~TIMER1_CONTROL_ENABLE);

    /* Set timer load count */
#if DEVICE_SLEEP
    /* The calibrated LPO is used as LP timer's clock source while SLEEP implemented. */
    if ((rda_ccfg_hwver() <= 4) && (0U != lpo_tmr_flag) && (0U != lpo_ticks_cal)) {
        LP_TICKER_TIMER->LDCNT = (uint32_t)(((uint64_t)delta * (uint64_t)lpo_ticks_cal) / US_TIMER_HZ) + 1;
    } else
#endif
    {
        LP_TICKER_TIMER->LDCNT = (uint32_t)(((uint64_t)delta << 15) / US_TIMER_HZ) + 1;
    }

    /* Delay for Clock Sync */
    for (;delay_ticks > 1;delay_ticks--);

    /* Enable timer */
    LP_TICKER_TIMER->TCTRL |= (TIMER1_CONTROL_ENABLE);

    /* Unmask timer, enable an overflow int */
    LP_TICKER_TIMER->TCTRL &= (~(TIMER1_CONTROL_INT_MSK));
}

void lp_ticker_fire_interrupt(void)
{
    lp_ticker_soft_int_flag = 1;
    NVIC_SetPendingIRQ(TIMER_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    /* Mask timer, disable an overflow int */
    LP_TICKER_TIMER->TCTRL |= (TIMER1_CONTROL_INT_MSK);
}

void lp_ticker_clear_interrupt(void)
{
    volatile uint32_t temp = LP_TICKER_TIMER->INTCLR;

    /* To fix compiling warning */
    temp = temp;

    /* Disable timer */
    LP_TICKER_TIMER->TCTRL &= (~TIMER1_CONTROL_ENABLE);
}

#endif /* DEVICE_LOWPOWERTIMER */
