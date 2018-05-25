/*******************************************************************************
 * Copyright (c) 2010-2018 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <drivers/tmr/adi_tmr.h>
#include <drivers/pwr/adi_pwr.h>
#include <drivers/gpio/adi_gpio.h>

#ifndef BITM_TMR_RGB_CTL_EN
#define BITM_TMR_RGB_CTL_EN BITM_TMR_CTL_EN
#endif

typedef uint32_t timestamp_t;

// defined in mbed_us_ticker_api.c which calls the ticker_irq_handler() routine
// defined in mbed_ticker_api.c
void us_ticker_irq_handler(void);

static int us_ticker_inited = 0;

static ADI_TMR_CONFIG tmrConfig;

static volatile uint32_t Upper_count = 0, largecnt = 0;

static ADI_TMR_TypeDef * adi_tmr_registers[ADI_TMR_DEVICE_NUM] = {pADI_TMR0, pADI_TMR1, pADI_TMR2};

#if defined(__ADUCM302x__)
    static const IRQn_Type adi_tmr_interrupt[ADI_TMR_DEVICE_NUM] = {TMR0_EVT_IRQn, TMR1_EVT_IRQn, TMR2_EVT_IRQn};
#elif defined(__ADUCM4x50__)
    static const IRQn_Type adi_tmr_interrupt[ADI_TMR_DEVICE_NUM] = {TMR0_EVT_IRQn, TMR1_EVT_IRQn, TMR2_EVT_IRQn, TMR_RGB_EVT_IRQn};
#else
#error TMR is not ported for this processor
#endif


/*---------------------------------------------------------------------------*
   Local functions
 *---------------------------------------------------------------------------*/
static void GP1CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg)
{
    Upper_count++;
}


static uint32_t get_current_time(void)
{
    uint16_t tmrcnt0, tmrcnt1;
    uint32_t totaltmr0, totaltmr1;
    uint32_t uc1, tmrpend0, tmrpend1;

    do {
        volatile uint32_t *ucptr = &Upper_count;

        /*
         * Carefully coded to prevent race conditions.  Do not make changes unless you understand all the
         * implications.
         *
         * Note this function can be called with interrupts globally disabled or enabled.  It has been coded to work in both cases.
         *
         * TMR0 and TMR1 both run from the same synchronous clock. TMR0 runs at 26MHz and TMR1 runs at 26/256MHz.
         * TMR1 generates an interrupt every time it overflows its 16 bit counter.  TMR0 runs faster and provides
         * the lowest 8 bits of the current time count.  When TMR0 and TMR1 are combined, they provide 24 bits of
         * timer precision. i.e. (TMR0.CURCNT & 0xff) + (TMR1.CURCNT << 8)
         *
         * There are several race conditions protected against:
         * 1. TMR0 and TMR1 are both read at the same time, however, on rare occasions, one will have incremented before the other.
         * Therefore we read both timer counters, and check if the middle 8 bits match, if they don't then read the counts again
         * until they do.  This ensures that one or the other counters are stable with respect to each other.
         *
         * 2. TMR1.CURCNT and Upper_count racing. Prevent this by disabling the TMR1 interrupt, which stops Upper_count increment interrupt (GP1CallbackFunction).
         * Then check pending bit of TMR1 to see if we missed Upper_count interrupt, and add it manually later.
         *
         * 3. Race between the TMR1 pend, and the TMR1.CURCNT read.  Even with TMR1 interrupt disabled, the pend bit
         * may be set while TMR1.CURCNT is being read.  We don't know if the pend bit matches the TMR1 state.
         * To prevent this, the pending bit is read twice, and we see if it matches; if it doesn't, loop around again.
         *
         * Note the TMR1 interrupt is enabled on each iteration of the loop to flush out any pending TMR1 interrupt,
         * thereby clearing any TMR1 pend's.  This have no effect if this routine is called with interrupts globally disabled.
         */

        NVIC_DisableIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);     // Prevent Upper_count increment
        tmrpend0 = NVIC_GetPendingIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
                                                                    // Check if there is a pending interrupt for timer 1

        __DMB();                                                    // memory barrier: read GP0 before GP1

        tmrcnt0 = adi_tmr_registers[ADI_TMR_DEVICE_GP0]->CURCNT;    // to minimize skew, read both timers manually

        __DMB();                                                    // memory barrier: read GP0 before GP1

        tmrcnt1 = adi_tmr_registers[ADI_TMR_DEVICE_GP1]->CURCNT;    // read both timers manually

        totaltmr0 = tmrcnt0;                                        // expand to u32 bits
        totaltmr1 = tmrcnt1;                                        // expand to u32 bits

        tmrcnt0 &= 0xff00u;
        tmrcnt1 <<= 8;

        __DMB();

        uc1 = *ucptr;                                               // Read Upper_count

        tmrpend1 = NVIC_GetPendingIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
                    // Check for a pending interrupt again.  Only leave loop if they match

        NVIC_EnableIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);      // enable interrupt on every loop to allow TMR1 interrupt to run
    } while ((tmrcnt0 != tmrcnt1) || (tmrpend0 != tmrpend1));

    totaltmr1 <<= 8;                 // Timer1 runs 256x slower
    totaltmr1 += totaltmr0 & 0xffu;  // Use last 8 bits of Timer0 as it runs faster
                                     // totaltmr1 now contain 24 bits of significance

    if (tmrpend0) {                  // If an interrupt is pending, then increment local copy of upper count
        uc1++;
    }

    uint64_t Uc = totaltmr1;         // expand out to 64 bits unsigned
    Uc += ((uint64_t) uc1) << 24;    // Add on the upper count to get the full precision count

                                     // Divide Uc by 26 (26MHz converted to 1MHz) todo scale for other clock freqs

    Uc *= 1290555u;                  // Divide total(1/26) << 25
    Uc >>= 25;                       // shift back.  Fixed point avoid use of floating point divide.
                                     // Compiler does this inline using shifts and adds.

    return Uc;
}


static void calc_event_counts(uint32_t timestamp)
{
    uint32_t calc_time, blocks, offset;
    uint64_t aa;

    calc_time = get_current_time();
    offset = timestamp - calc_time; // offset in useconds

    if (offset > 0xf0000000u)       // if offset is a really big number, assume that timer has already expired (i.e. negative)
        offset = 0u;

    if (offset > 10u) {             // it takes 10us to user timer routine after interrupt.  Offset timer to account for that.
        offset -= 10u;
    } else
        offset = 0u;

    aa = (uint64_t) offset;
    aa *= 26u;                      // convert from 1MHz to 26MHz clock. todo scale for other clock freqs

    blocks = aa >> 7;
    blocks++;                       // round

    largecnt = blocks>>1;           // communicate to event_timer() routine
}

static void event_timer()
{
    if (largecnt) {
        uint32_t cnt = largecnt;

        if (cnt > 65535u) {
            cnt = 0u;
        } else {
            cnt = 65536u - cnt;
        }

        tmrConfig.nLoad        = cnt;
        tmrConfig.nAsyncLoad   = cnt;
        adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP2, &tmrConfig);
        adi_tmr_Enable(ADI_TMR_DEVICE_GP2, true);
    } else {
        tmrConfig.nLoad        = 65535u;
        tmrConfig.nAsyncLoad   = 65535u;
        adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP2, &tmrConfig);
        adi_tmr_Enable(ADI_TMR_DEVICE_GP2, true);
    }
}


/*
 * Interrupt routine for timer 2
 *
 * largecnt counts how many timer ticks should be counted to reach timer event.
 * Each interrupt happens every 65536 timer ticks, unless there are less than 65536 ticks to count.
 * In that case do the remaining timers ticks.
 *
 * largecnt is a global that is used to communicate between event_timer and the interrupt routine
 * On entry, largecnt will be any value larger than 0.
 */
static void GP2CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg)
{
    if (largecnt >= 65536u) {
        largecnt -= 65536u;
    } else {
        largecnt = 0;
    }

    if (largecnt < 65536u) {
        adi_tmr_Enable(ADI_TMR_DEVICE_GP2, false);
        if (largecnt) {
            event_timer();
        } else {
            us_ticker_irq_handler();
        }
    }
}


/*---------------------------------------------------------------------------*
   us_ticker HAL APIs
 *---------------------------------------------------------------------------*/
void us_ticker_init(void)
{
    if (us_ticker_inited) {
        // Disable ticker interrupt on reinitialization
        adi_tmr_Enable(ADI_TMR_DEVICE_GP2, false);
        return;
    }

    us_ticker_inited = 1;

    /*--------------------- GP TIMER INITIALIZATION --------------------------*/

    /* Set up GP0 callback function */
    adi_tmr_Init(ADI_TMR_DEVICE_GP0, NULL, NULL, false);

    /* Set up GP1 callback function */
    adi_tmr_Init(ADI_TMR_DEVICE_GP1, GP1CallbackFunction, NULL, true);

    /* Set up GP1 callback function */
    adi_tmr_Init(ADI_TMR_DEVICE_GP2, GP2CallbackFunction, NULL, true);

    /* Configure GP0 to run at 26MHz */
    tmrConfig.bCountingUp  = true;
    tmrConfig.bPeriodic    = true;
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_1;     // TMR0 at 26MHz
    tmrConfig.eClockSource = ADI_TMR_CLOCK_PCLK;      // TMR source is PCLK (most examples use HFOSC)
    tmrConfig.nLoad        = 0;
    tmrConfig.nAsyncLoad   = 0;
    tmrConfig.bReloading   = false;
    tmrConfig.bSyncBypass  = true;                    // Allow x1 prescale: requires PCLK as a clk
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP0, &tmrConfig);

    /* Configure GP1 to have a period 256 times longer than GP0 */
    tmrConfig.nLoad        = 0;
    tmrConfig.nAsyncLoad   = 0;
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_256;    // TMR1 = 26MHz/256
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, &tmrConfig);

    /* Configure GP2 for doing event counts */
    tmrConfig.bCountingUp  = true;
    tmrConfig.bPeriodic    = true;
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_256;   // TMR2 at 26MHz/256
    tmrConfig.eClockSource = ADI_TMR_CLOCK_PCLK;      // TMR source is PCLK (most examples use HFOSC)
    tmrConfig.nLoad        = 0;
    tmrConfig.nAsyncLoad   = 0;
    tmrConfig.bReloading   = false;
    tmrConfig.bSyncBypass  = true;                    // Allow x1 prescale
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP2, &tmrConfig);


    /*------------------------- GP TIMER ENABLE ------------------------------*/

    /* Manually enable both timers to get them started at the same time
     *
     */
    adi_tmr_registers[ADI_TMR_DEVICE_GP0]->CTL |= (uint16_t) BITM_TMR_RGB_CTL_EN;
    adi_tmr_registers[ADI_TMR_DEVICE_GP1]->CTL |= (uint16_t) BITM_TMR_RGB_CTL_EN;
}

uint32_t us_ticker_read()
{
    uint32_t curr_time;

    if (!us_ticker_inited) {
        us_ticker_init();
    }

    curr_time = get_current_time();

    return curr_time;
}

void us_ticker_disable_interrupt(void)
{
    adi_tmr_Enable(ADI_TMR_DEVICE_GP2, false);
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(TMR2_EVT_IRQn);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // if timestamp is already past, do not set interrupt
    if ((timestamp + 10) <= us_ticker_read()) return;
    /* timestamp is when interrupt should fire.
     *
     * This MUST not be called if another timer event is currently enabled.
     *
     */
    calc_event_counts(timestamp);             // use timestamp to calculate largecnt to control number of timer interrupts
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_256;   // TMR2 at 26MHz/256
    event_timer();                            // uses largecnt to initiate timer interrupts
}

/** Set pending interrupt that should be fired right away.
 *
 * The ticker should be initialized prior calling this function.
 *
 * This MUST not be called if another timer event is currently enabled.
 */
void us_ticker_fire_interrupt(void)
{
    largecnt = 1;                              // set a minimal interval so interrupt fire immediately
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_1;   // TMR2 at 26MHz/1
    event_timer();                             // enable the timer and interrupt
}


/*
** EOF
*/
