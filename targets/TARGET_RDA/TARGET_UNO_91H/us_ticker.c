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
#include "us_ticker_api.h"
#include "rda_ccfg_api.h"
#include "mbed_critical.h"

#define US_TICKER_TIMER             (RDA_TIM0)
#define rTIMER1_CONTROL             (RDA_TIM1->TCTRL)
#define rTIMER1_LOADCNT             (RDA_TIM1->LDCNT)
#define rTIMER1_CURVAL              (RDA_TIM1->CVAL)
#define rFREE_TIMER_INITVAL         (RDA_SCU->FTMRINITVAL)
#define rFREE_TIMER_PRESCALE        (RDA_SCU->FTMRPREVAL)
#define rFREE_TIMER_TIMESTAMP       (RDA_SCU->FTMRTS)
#define rRDA_POWER_CONTROL          (RDA_SCU->PWRCTRL)

#define TIMER0_CONTROL_ENABLE       (0x01)
#define TIMER0_CONTROL_MODE         (0x02)
#define TIMER0_CONTROL_INT_MSK      (0x04)
#define TIMER1_CONTROL_ENABLE       (0x20)
#define TIMER1_CONTROL_MODE         (0x40)
#define TIMER1_CONTROL_INT_MSK      (0x80)

#define US_TIMER_CLOCK_SOURCE       (AHBBusClock >> 1)
#define US_TIMER_HZ                 (1000000)
#define TIMER1_CLOCK_SOURCE         (32768)
#define FREE_TIMER_CLOCK_SOURCE     (32768)

#define TIMER1_LDCNT_INIT_VAL       (0x08637BD0UL)
#define TIMER1_CURVAL_DELTA         (0xF79C842FUL)
#define FREE_TIMER_INIT_VAL         (0x08637BD0UL)

static int32_t  us_ticker_inited = 0;

int us_ticker_soft_int_flag = 0;

#if DEVICE_SLEEP
extern unsigned int lpo_ticks_cal;
extern unsigned int lpo_tmr_flag;
extern signed int   lpo_delta_us;
#endif

extern void rda_timer_irq_set(void);

void us_ticker_init(void)
{
    uint32_t regval;
    if (us_ticker_inited)
        return;

    /* Enable apb timer clock */
    RDA_SCU->CLKGATE1 |= (0x01UL << 3);

    /* Set timer count */
    US_TICKER_TIMER->LDCNT = 0xFFFFFFFFUL;

    /* Set timer mode */
    US_TICKER_TIMER->TCTRL |= TIMER0_CONTROL_MODE;

    /* Set free timer power */
    rda_ccfg_aontmr();

    /* Delay 300us at least */
    {
        unsigned int idx = (SystemCoreClock / US_TIMER_HZ / 4UL) * 300UL;
        regval = 0U;
        while (idx--) {
            regval += rFREE_TIMER_TIMESTAMP;
        }
    }

    /* Set free timer write_en */
    rRDA_POWER_CONTROL |= (uint32_t)(0x01UL << 8);

    /* Set init value */
#if DEVICE_SLEEP
    if ((rda_ccfg_hwver() <= 4) && (0U != lpo_tmr_flag) && (0U != lpo_ticks_cal)) {
        rFREE_TIMER_INITVAL = (uint32_t)((uint64_t)FREE_TIMER_INIT_VAL * lpo_ticks_cal / FREE_TIMER_CLOCK_SOURCE);
    } else
#endif
    {
        rFREE_TIMER_INITVAL = FREE_TIMER_INIT_VAL;
    }

    /* Enable int */
    rTIMER1_CONTROL |= (0x01UL << 4);
    rRDA_POWER_CONTROL |= (0x01UL << 18);

    /* Unmask int */
    rTIMER1_CONTROL &= ~(0x01UL << 3);

    /* Set free timer prescale */
    rFREE_TIMER_PRESCALE = 0U;

    /* Enable free timer */
    rRDA_POWER_CONTROL |= (uint32_t)(0x01UL << 17);

    /* Clr free timer write_en */
    rRDA_POWER_CONTROL &= (~(uint32_t)(1 << 8));

    rda_timer_irq_set();

    /* Set us_ticker_inited true, after all settings done */
    us_ticker_inited = 1U;
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited)
        us_ticker_init();

#if DEVICE_SLEEP
    /* The calibrated LPO is used as free timer's clock source while SLEEP implemented. */
    if ((rda_ccfg_hwver() <= 4) && (0U != lpo_tmr_flag) && (0U != lpo_ticks_cal)) {
        return (uint32_t)((((int64_t)rFREE_TIMER_TIMESTAMP) * (int64_t)US_TIMER_HZ) / (int64_t)lpo_ticks_cal + (int64_t)lpo_delta_us);
    } else
#endif
    {
        /* ">>15" gets the same result as "/FREE_TIMER_CLOCK_SOURCE" (="/32768") */
        return (uint32_t)((((uint64_t)rFREE_TIMER_TIMESTAMP) * US_TIMER_HZ) >> 15);
    }
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t ftmr_ticks = 0U;
    int32_t delta = (int32_t)(timestamp - us_ticker_read());

    /* Disable timer */
    US_TICKER_TIMER->TCTRL &= (~TIMER0_CONTROL_ENABLE);

    /* Convert the delta to the times of US_TIMER_HZ/FREE_TIMER_CLOCK_SOURCE */
    ftmr_ticks = (uint32_t)(((uint64_t)delta << 15) / US_TIMER_HZ);
    delta = (uint32_t)((((uint64_t)ftmr_ticks) * US_TIMER_HZ) >> 15);
    US_TICKER_TIMER->LDCNT = delta * (US_TIMER_CLOCK_SOURCE / US_TIMER_HZ);

    /* Enable timer */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_ENABLE);

    /* Unmask timer, enable an overflow int */
    US_TICKER_TIMER->TCTRL &= (~(TIMER0_CONTROL_INT_MSK));
}

void us_ticker_fire_interrupt(void)
{
    us_ticker_soft_int_flag = 1;
    NVIC_SetPendingIRQ(TIMER_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    /* Mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);
}

void us_ticker_clear_interrupt(void)
{
    volatile uint32_t temp = US_TICKER_TIMER->INTCLR;

    /* To fix compiling warning */
    temp = temp;
}
