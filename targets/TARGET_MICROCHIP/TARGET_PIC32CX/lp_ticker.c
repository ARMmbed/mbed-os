/***************************************************************************//**
* @file lp_ticker.c
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

#if DEVICE_LPTICKER
#include "pic32cx.h"
#include "pmc.h"
#include "sysclk.h"
#include "lp_ticker_api.h"
#include "cmsis.h"

#define LPTIMER_FREQUENCY    32768
#define LPTIMER_BITS         32

#define LPTIMER_ID_TC        ID_TC0_CHANNEL1
#define LPTIMER_TC           TC0
#define LPTIMER_TC_CHN       1
#define LPTIMER_TC_Handler   TC0_CHANNEL1_Handler
#define LPTIMER_TC_IRQn      TC0_CHANNEL1_IRQn

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        LPTIMER_FREQUENCY,
        LPTIMER_BITS
    };
    return &info;
}

static bool lp_ticker_inited = false;

/** @brief	Interrupt handler for LPTIMER
*/
void LPTIMER_TC_Handler(void)
{
    if (LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_SR & TC_SR_CPCS) {
        /* Trigger events */
        lp_ticker_irq_handler();
    }
}

/** Initialize or re-initialize the low power ticker.
*  This resets all the clocking and prescaler registers,
*  along with disabling the compare interrupt.
*/
void lp_ticker_init(void)
{
    if (!lp_ticker_inited) {
        /*  Disable TC clock */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_CCR = TC_CCR_CLKDIS;

        /*  Disable interrupts. */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_IDR = 0xFFFFFFFF;

        /* Configure PMC */
        pmc_enable_periph_clk(LPTIMER_ID_TC);

        /*  Clear status register. */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_SR;

        /*  Set mode. */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK5;

        /*  Set extended mode. */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_EMR = 0;

        /* Start Timer */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

        lp_ticker_inited = true;
    } else {
        /* Disable RC Compare Interrupt */
        LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_IDR = TC_IER_CPCS;
    }
}

/** Deinitialize the lower power ticker.
*/
void lp_ticker_free(void)
{
    /*  Disable TC clock */
    LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_CCR = TC_CCR_CLKDIS;
    NVIC_DisableIRQ(LPTIMER_TC_IRQn);
}

/** Read the current counter
*
* @return The current timer's counter value in ticks
*/
uint32_t lp_ticker_read(void)
{
    return LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_CV;
}

/** Set interrupt for specified timestamp
*
* @param timestamp The time in ticks to be set
*/
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    if (timestamp == 0) {
        timestamp = 1;
    }

    LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_RC = timestamp;
    LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_IER = TC_IER_CPCS;
    NVIC_ClearPendingIRQ(LPTIMER_TC_IRQn);
    NVIC_EnableIRQ(LPTIMER_TC_IRQn);
}

/** Disable low power ticker interrupt
*
*/
void lp_ticker_disable_interrupt(void)
{
    LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_IDR = TC_IER_CPCS;
}

/** Clear the low power ticker interrupt
*
*/
void lp_ticker_clear_interrupt(void)
{
    LPTIMER_TC->TC_CHANNEL[LPTIMER_TC_CHN].TC_SR;
}

void lp_ticker_fire_interrupt(void)
{
    NVIC_EnableIRQ(LPTIMER_TC_IRQn);
    NVIC_SetPendingIRQ(LPTIMER_TC_IRQn);
}

#endif /* DEVICE_LPTICKER */
