/***************************************************************************//**
* @file us_ticker.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
*******************************************************************************
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/

#if DEVICE_USTICKER
#include "pic32cx.h"
#include "tc.h"
#include "pmc.h"
#include "sysclk.h"
#include "us_ticker_api.h"
#include "cmsis.h"

#define USTIMER_FREQUENCY    1000000
#define USTIMER_BITS         32

#define USTIMER_ID_TC        ID_TC0_CHANNEL0
#define USTIMER_TC           TC0
#define USTIMER_TC_CHN       0
#define USTIMER_TC_Handler   TC0_CHANNEL0_Handler
#define USTIMER_TC_IRQn      TC0_CHANNEL0_IRQn

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        USTIMER_FREQUENCY,
        USTIMER_BITS
    };
    return &info;
}

static bool us_ticker_inited = false;

/** @brief	Interrupt handler for USTIMER
*/
void USTIMER_TC_Handler(void)
{
    if (tc_get_status(USTIMER_TC, USTIMER_TC_CHN) & TC_SR_CPCS) {
        /* Trigger events */
        us_ticker_irq_handler();
    }
}

/** Initialize or re-initialize the low power ticker.
*  This resets all the clocking and prescaler registers,
*  along with disabling the compare interrupt.
*/
void us_ticker_init(void)
{
    if (!us_ticker_inited) {
        tc_stop(USTIMER_TC, USTIMER_TC_CHN);
        /* Configure PMC */
        pmc_enable_periph_clk(USTIMER_ID_TC);
        /* Configure GCLK 1MHz */
        pmc_configure_generic(USTIMER_ID_TC, PMC_PCR_GCLKCSS_MCK0, PMC_PCR_GCLKDIV(199));
        pmc_enable_generic_clk(USTIMER_ID_TC);

        /* Configure Timer 1us */
        tc_init(USTIMER_TC, USTIMER_TC_CHN, TC_CMR_TCCLKS_TIMER_CLOCK1, 0);

        /* Configure and enable interrupt on RC compare */
        NVIC_ClearPendingIRQ(USTIMER_TC_IRQn);
        NVIC_EnableIRQ(USTIMER_TC_IRQn);
        tc_enable_interrupt(USTIMER_TC, USTIMER_TC_CHN, TC_IER_CPCS);

        us_ticker_inited = true;
    } else {
        tc_disable_interrupt(USTIMER_TC, USTIMER_TC_CHN, TC_IER_CPCS);
    }
}

/** Deinitialize the lower power ticker.
*/
void us_ticker_free(void)
{
    tc_stop(USTIMER_TC, USTIMER_TC_CHN);
    tc_disable_interrupt(USTIMER_TC, USTIMER_TC_CHN, TC_IER_CPCS);
    NVIC_DisableIRQ(USTIMER_TC_IRQn);
}

/** Read the current counter
*
* @return The current timer's counter value in ticks
*/
uint32_t us_ticker_read(void)
{
    return tc_read_cv(USTIMER_TC, USTIMER_TC_CHN);
}

/** Set interrupt for specified timestamp
*
* @param timestamp The time in ticks to be set
*/
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (timestamp == 0) {
        timestamp = 1;
    }

    tc_stop(USTIMER_TC, USTIMER_TC_CHN);
    tc_write_rc(USTIMER_TC, USTIMER_TC_CHN, timestamp);
    tc_enable_interrupt(USTIMER_TC, USTIMER_TC_CHN, TC_IER_CPCS);
    NVIC_ClearPendingIRQ(USTIMER_TC_IRQn);
    NVIC_EnableIRQ(USTIMER_TC_IRQn);
    tc_start(USTIMER_TC, USTIMER_TC_CHN);
}

/** Disable low power ticker interrupt
*
*/
void us_ticker_disable_interrupt(void)
{
    tc_disable_interrupt(USTIMER_TC, USTIMER_TC_CHN, TC_IER_CPCS);
}

/** Clear the low power ticker interrupt
*
*/
void us_ticker_clear_interrupt(void)
{
    tc_get_status(USTIMER_TC, USTIMER_TC_CHN);
}

void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(USTIMER_TC_IRQn);
}

#endif /* DEVICE_USTICKER */
