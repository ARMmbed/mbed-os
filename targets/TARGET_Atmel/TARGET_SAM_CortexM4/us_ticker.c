/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "mbed_assert.h"
#include "compiler.h"
#include "sysclk.h"
#include "tc.h"

uint8_t us_ticker_inited = 0;
extern uint8_t g_sys_init;
volatile uint16_t us_ticker_16bit_counter;
volatile uint16_t us_ticker_interrupt_counter;
volatile uint16_t us_ticker_interrupt_offset;
volatile uint32_t overflow32bitcounter = 0;

#define TICKER_COUNTER_uS        TC1

#define TICKER_COUNTER_CLK0      ID_TC3
#define TICKER_COUNTER_CLK1      ID_TC4

#define TICKER_COUNTER_CHANNEL0  0
#define TICKER_COUNTER_IRQn0     TC3_IRQn
#define TICKER_COUNTER_Handlr0   TC3_Handler

#define TICKER_COUNTER_CHANNEL1  1
#define TICKER_COUNTER_IRQn1     TC4_IRQn
#define TICKER_COUNTER_Handlr1   TC4_Handler

#define OVERFLOW_16bit_VALUE    0xFFFF


void TICKER_COUNTER_Handlr1(void)
{
    uint32_t status=tc_get_status(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);
    uint32_t interrupmask=tc_get_interrupt_mask(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);

    if (((status & interrupmask)  & TC_IER_CPCS)) {
        if(us_ticker_interrupt_counter) {
            us_ticker_interrupt_counter--;
        } else {
            if(us_ticker_interrupt_offset) {
                us_ticker_interrupt_offset=0;
                tc_stop(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);
                tc_write_rc(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1, (uint32_t)us_ticker_interrupt_offset);
                tc_start(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);
            } else
                us_ticker_irq_handler();
        }
    }
}

void TICKER_COUNTER_Handlr0(void)
{
    uint32_t status=tc_get_status(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0);
    uint32_t interrupmask=tc_get_interrupt_mask(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0);

    if (((status & interrupmask)  & TC_IER_COVFS)) {
        us_ticker_16bit_counter++;
        if(us_ticker_16bit_counter == 0xFFFF)
            overflow32bitcounter++;
    }
}

void us_ticker_init(void)
{
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    us_ticker_16bit_counter=0;
    us_ticker_interrupt_counter=0;
    us_ticker_interrupt_offset=0;

    if (g_sys_init == 0) {
        sysclk_init();
        system_board_init();
        g_sys_init = 1;
    }

    /* Configure the PMC to enable the TC module. */
    sysclk_enable_peripheral_clock(TICKER_COUNTER_CLK0);
    sysclk_enable_peripheral_clock(TICKER_COUNTER_CLK1);

#if SAMG55
    /* Enable PCK output */
    pmc_disable_pck(PMC_PCK_3);
    pmc_switch_pck_to_mck(PMC_PCK_3, PMC_PCK_PRES_CLK_1);
    pmc_enable_pck(PMC_PCK_3);
#endif

    /* Init TC to Counter mode. */
    tc_init(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0, TC_CMR_TCCLKS_TIMER_CLOCK4);
    tc_init(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1, TC_CMR_TCCLKS_TIMER_CLOCK4);


    NVIC_DisableIRQ(TICKER_COUNTER_IRQn0);

    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn0);
    NVIC_SetPriority(TICKER_COUNTER_IRQn0, 0);
    NVIC_EnableIRQ(TICKER_COUNTER_IRQn0);
    tc_enable_interrupt(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0, TC_IER_COVFS);

    tc_start(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0);
}


uint32_t us_ticker_read()
{
    if (!us_ticker_inited)
        us_ticker_init();

    uint32_t counter_value=0;
    uint16_t tickerbefore=0;
    do {
        tickerbefore=us_ticker_16bit_counter;
        counter_value=tc_read_cv(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL0);
    } while(tickerbefore!=us_ticker_16bit_counter);

    return counter_value+(OVERFLOW_16bit_VALUE*us_ticker_16bit_counter);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t cur_time;
    int32_t delta;

    cur_time = us_ticker_read();
    delta = (int32_t)((uint32_t)timestamp - cur_time);
    if (delta < 0) {
        /* Event already occurred in past */
        us_ticker_irq_handler();
        return;
    }

    uint16_t interruptat=0;

    if(delta > OVERFLOW_16bit_VALUE) {
        us_ticker_interrupt_counter= (delta/OVERFLOW_16bit_VALUE) -1;
        us_ticker_interrupt_offset=delta%OVERFLOW_16bit_VALUE;
        interruptat=OVERFLOW_16bit_VALUE;
    } else {
        us_ticker_interrupt_counter=0;
        us_ticker_interrupt_offset=0;
        interruptat=delta;
    }

    NVIC_DisableIRQ(TICKER_COUNTER_IRQn1);

    tc_write_rc(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1, (uint32_t)interruptat);

    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn1);
    NVIC_SetPriority(TICKER_COUNTER_IRQn1, 0);
    NVIC_EnableIRQ(TICKER_COUNTER_IRQn1);
    tc_enable_interrupt(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1, TC_IDR_CPCS );

    tc_start(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(TICKER_COUNTER_IRQn1);
}

void us_ticker_disable_interrupt(void)
{
    tc_stop(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1);
    tc_disable_interrupt(TICKER_COUNTER_uS, TICKER_COUNTER_CHANNEL1, TC_IDR_CPCS);
    NVIC_DisableIRQ(TICKER_COUNTER_IRQn1);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn1);
}

void us_ticker_free(void)
{

}
