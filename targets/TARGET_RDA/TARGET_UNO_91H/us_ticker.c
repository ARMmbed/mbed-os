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
#include "us_ticker_api.h"
#include "rda_ccfg_api.h"
#include "mbed_critical.h"

#define US_TICKER_TIMER             (RDA_TIM0)
#define rTIMER0_CURVAL              (RDA_TIM0->CVAL)
#define TIMER0_PRESCALE             (8)
#define TIMER0_SHIFTBITS            (3)
#define TIMER0_LDCNT_INIT_VAL       (0xFFFFFFFF)
#define TIMER0_MAX_COUNT            (0x1FFFFFFF)

#define TIMER0_CONTROL_ENABLE       (0x01)
#define TIMER0_CONTROL_MODE         (0x02)
#define TIMER0_CONTROL_INT_MSK      (0x04)


volatile uint32_t us_ticker_clrInt = 0;

static uint32_t us_ticker_inited = 0;
uint32_t us_ticker_soft_int_flag;
static uint32_t us_ticker_timestamp;
static uint32_t us_ticker_interruptCount;

extern void rda_timer_irq_set(void);

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        us_ticker_disable_interrupt();
        return;
    }

    /* Enable apb timer clock */
    RDA_SCU->CLKGATE1 |= (0x01UL << 3);

    /* Set timer mode */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_MODE);

    /* Set period mode */
    RDA_GPIO->REVID |= (0x01UL << 25);

    /* Set timer count */
    US_TICKER_TIMER->LDCNT = TIMER0_LDCNT_INIT_VAL;

    /* Enable timer */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_ENABLE);

    /* mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);

    rda_timer_irq_set();

    /* Set us_ticker_inited true, after all settings done */
    us_ticker_inited = 1U;
    us_ticker_soft_int_flag = 0;
    us_ticker_timestamp = 0;
    us_ticker_interruptCount = TIMER0_MAX_COUNT;
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited) {
        return 0 ;
    }

    uint32_t tick_readout = 0 ;

    core_util_critical_section_enter();
    uint32_t ticker = rTIMER0_CURVAL >> TIMER0_SHIFTBITS ;

    if (us_ticker_interruptCount > ticker)
        tick_readout = (us_ticker_timestamp + us_ticker_interruptCount - ticker) % TIMER0_MAX_COUNT ;
    else
        tick_readout = (us_ticker_timestamp + TIMER0_MAX_COUNT + us_ticker_interruptCount - ticker) % TIMER0_MAX_COUNT ;
    core_util_critical_section_exit();

    return tick_readout;
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited) {
        return ;
    }

    uint32_t tmp_stamp = timestamp % TIMER0_MAX_COUNT ;

    core_util_critical_section_enter();
    us_ticker_timestamp = us_ticker_read() ;
    us_ticker_interruptCount = (tmp_stamp > us_ticker_timestamp) ? (tmp_stamp - us_ticker_timestamp):(tmp_stamp + TIMER0_MAX_COUNT - us_ticker_timestamp) ;

    /* Disable timer */
    US_TICKER_TIMER->TCTRL &= (~TIMER0_CONTROL_ENABLE);

    US_TICKER_TIMER->LDCNT = us_ticker_interruptCount << TIMER0_SHIFTBITS ;

    /* Enable timer */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_ENABLE);

    /* Unmask timer, enable an overflow int */
    US_TICKER_TIMER->TCTRL &= (~(TIMER0_CONTROL_INT_MSK));
    core_util_critical_section_exit();

    return ;
}

void us_ticker_fire_interrupt(void)
{
    if (!us_ticker_inited) {
        return ;
    }

    core_util_critical_section_enter();
    us_ticker_soft_int_flag = 1 ;
    NVIC_SetPendingIRQ(TIMER_IRQn);
    core_util_critical_section_exit();
}

void us_ticker_disable_interrupt_help(void)
{
    if (!us_ticker_inited) {
        return ;
    }

    /* Mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);
}

void us_ticker_disable_interrupt(void)
{
    if (!us_ticker_inited) {
        return ;
    }

    core_util_critical_section_enter();
    /* Mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);

    us_ticker_timestamp = us_ticker_read();
    us_ticker_interruptCount = TIMER0_MAX_COUNT;

    /* Disable timer */
    US_TICKER_TIMER->TCTRL &= (~TIMER0_CONTROL_ENABLE);

    /* Set timer count */
    US_TICKER_TIMER->LDCNT = TIMER0_LDCNT_INIT_VAL;

    /* Enable timer */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_ENABLE);

    /* mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);
    core_util_critical_section_exit();
}

void us_ticker_clear_interrupt(void)
{
    if (!us_ticker_inited) {
        return ;
    }

    us_ticker_clrInt = US_TICKER_TIMER->INTCLR;
}

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info =
    {
        5000000,      // 5MHZ
        29            // 29 bit counter
    };
    return &info;
}

void us_ticker_free(void)
{
    if (!us_ticker_inited) {
        return ;
    }

    core_util_critical_section_enter();
    us_ticker_disable_interrupt_help();
    us_ticker_clear_interrupt();

    /* Disable timer */
    US_TICKER_TIMER->TCTRL &= (~TIMER0_CONTROL_ENABLE);

    us_ticker_inited = 0;
    us_ticker_timestamp = 0 ;
    us_ticker_interruptCount = 0 ;
    core_util_critical_section_exit();

    return;
}

void us_ticker_irq_callback()
{

    us_ticker_clear_interrupt () ;

    if (us_ticker_soft_int_flag == 1) {
        us_ticker_soft_int_flag = 0 ;
        return ;
    }

    core_util_critical_section_enter();
    /* Check the flag firstly, because following hanlder can change it */
    us_ticker_disable_interrupt_help();

    /* Disable timer */
    US_TICKER_TIMER->TCTRL &= (~TIMER0_CONTROL_ENABLE);

    US_TICKER_TIMER->LDCNT = TIMER0_LDCNT_INIT_VAL ;

    /* Enable timer */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_ENABLE);

    /* mask timer, disable an overflow int */
    US_TICKER_TIMER->TCTRL |= (TIMER0_CONTROL_INT_MSK);

    core_util_critical_section_exit();
}
