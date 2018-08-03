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

#include "cmsis.h"
#include <stddef.h>
#include "lp_ticker_api.h"
#include "mbed_assert.h"
#include "sleep_api.h"
#include "compiler.h"
#include "sysclk.h"
#include "tc.h"
#include "us_ticker_api.h"

uint8_t lp_ticker_inited = 0;
extern volatile uint8_t us_ticker_inited;
extern uint8_t g_sys_init;
extern volatile uint32_t overflow32bitcounter;
volatile uint16_t lp_ticker_16bit_counter;
volatile uint16_t lp_ticker_interrupt_counter;
volatile uint16_t lp_ticker_interrupt_offset;
volatile uint32_t lpoverflow32bitcounter = 0;

#define TICKER_COUNTER_lp		TC0

#define TICKER_COUNTER_CLK2     ID_TC2

#define TICKER_COUNTER_CHANNEL2 2
#define TICKER_COUNTER_IRQn2	TC2_IRQn
#define TICKER_COUNTER_Handlr2  TC2_Handler

#define OVERFLOW_16bit_VALUE    0xFFFF


void TICKER_COUNTER_Handlr2(void)
{
    uint32_t status=tc_get_status(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);
    uint32_t interrupmask=tc_get_interrupt_mask(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);

    if (((status & interrupmask)  & TC_IER_CPCS)) {
        if(lp_ticker_interrupt_counter) {
            lp_ticker_interrupt_counter--;
        } else {
            if(lp_ticker_interrupt_offset) {
                tc_stop(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);
                tc_write_rc(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2, (uint32_t)lp_ticker_interrupt_offset);
                tc_start(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);
                lp_ticker_interrupt_offset=0;
            } else {
                lp_ticker_irq_handler();
            }
        }
    }
}

void lp_ticker_init(void)
{
    if(lp_ticker_inited)
        return;
    if (!us_ticker_inited)
        us_ticker_init();
    sysclk_enable_peripheral_clock(TICKER_COUNTER_CLK2);
    tc_init(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2, TC_CMR_TCCLKS_TIMER_CLOCK4);
    lp_ticker_inited = 1;
}

uint32_t lp_ticker_read()
{
    if (!lp_ticker_inited)
        lp_ticker_init();
    return us_ticker_read();
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t cur_time;
    uint32_t delta;

    cur_time = lp_ticker_read();
    delta = timestamp - cur_time;

    uint16_t interruptat=0;

    if(delta > OVERFLOW_16bit_VALUE) {
        lp_ticker_interrupt_counter= (delta/OVERFLOW_16bit_VALUE) -1;
        lp_ticker_interrupt_offset=delta%OVERFLOW_16bit_VALUE;
        interruptat=OVERFLOW_16bit_VALUE;
    } else {
        lp_ticker_interrupt_counter=0;
        lp_ticker_interrupt_offset=0;
        interruptat=delta;
    }

    NVIC_DisableIRQ(TICKER_COUNTER_IRQn2);

    tc_write_rc(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2, (uint32_t)interruptat);

    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn2);
    NVIC_SetPriority(TICKER_COUNTER_IRQn2, 0);
    NVIC_EnableIRQ(TICKER_COUNTER_IRQn2);
    tc_enable_interrupt(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2, TC_IDR_CPCS );

    tc_start(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(TICKER_COUNTER_IRQn2);
}

void lp_ticker_disable_interrupt(void)
{
    tc_stop(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2);
    tc_disable_interrupt(TICKER_COUNTER_lp, TICKER_COUNTER_CHANNEL2, TC_IDR_CPCS);
    NVIC_DisableIRQ(TICKER_COUNTER_IRQn2);
}

void lp_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(TICKER_COUNTER_IRQn2);
}

void lp_ticker_free(void)
{

}
