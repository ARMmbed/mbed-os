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
#include "lp_ticker_api.h"
#include "mbed_assert.h"
#include "rtt.h"

static uint8_t lp_ticker_inited = 0;
extern uint8_t g_sys_init;

void RTT_Handler(void)
{
    uint32_t ul_status;

    /* Get RTT status */
    ul_status = rtt_get_status(RTT);

    /* Alarm */
    if ((ul_status & RTT_SR_ALMS) == RTT_SR_ALMS) {
        rtt_disable_interrupt(RTT, RTT_MR_ALMIEN);
        lp_ticker_irq_handler();
    }
}

void lp_ticker_init(void)
{
    if (g_sys_init == 0) {
        sysclk_init();
        g_sys_init = 1;
    }
    if (lp_ticker_inited) return;

    lp_ticker_inited = 1;

#if SAM4N || SAM4S || SAM4E || SAM4C || SAM4CP || SAM4CM || SAMV71 || SAMV70 || SAME70 || SAMS70
    rtt_sel_source(RTT, true);
#endif
    rtt_init(RTT, 33); /* Selects 32.768Khz clock with prescalar 33 */ /*1ms tick*/
}

uint32_t lp_ticker_read()
{
    if (!lp_ticker_inited)
        lp_ticker_init();
    return rtt_read_timer_value(RTT);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t cur_time;
    int32_t delta;

    cur_time = lp_ticker_read();
    delta = (int32_t)((uint32_t)timestamp - cur_time);
    if (delta < 0) {
        /* Event already occurred in past */
        lp_ticker_irq_handler();
        return;
    }
    /* Enable RTT interrupt */
    NVIC_DisableIRQ(RTT_IRQn);
    NVIC_ClearPendingIRQ(RTT_IRQn);
    NVIC_SetPriority(RTT_IRQn, 0);
    NVIC_EnableIRQ(RTT_IRQn);
    rtt_enable_interrupt(RTT, RTT_MR_RTTINCIEN);

    rtt_write_alarm_time(RTT, (uint32_t)timestamp);
}

void lp_ticker_disable_interrupt(void)
{
    rtt_disable_interrupt(RTT, RTT_MR_ALMIEN);
}

void lp_ticker_clear_interrupt(void)
{
    uint32_t ul_status;
    /* Get RTT status */
    ul_status = rtt_get_status(RTT);
}