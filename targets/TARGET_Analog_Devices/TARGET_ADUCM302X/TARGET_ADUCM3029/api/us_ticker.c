/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
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

ADI_TMR_CONFIG tmrConfig, tmr2Config;

static volatile uint32_t Upper_count = 0, smallcnt = 0, largecnt = 0;

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
void GP1CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg)
{
	Upper_count++;
}


uint32_t get_current_time(void)
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
    	 * There are three main conditions protected against.
    	 * 1. TMR0 and TMR1 both run from synchronous clocks.  TMR0 runs at 26MHz and TMR1 runs at 26/256MHz
    	 * Read both timer counters, and check if the middle 8 bits match, if they don't then read the counts again
    	 * until they do.  This ensures that one or the other counters have not incremented while reading them.
    	 *
    	 * 2. CURCNT and Upper_count racing.  TMR1.CURCNT hardware might have incremented before the GP1 interrupt can occur.
    	 * This would put Upper_count one value behind the GP1 CURCNT.  How to prevent this: read GP0 then GP1
    	 * timer.  GP1 is started slightly ahead of GP0.  We read GP0 then GP1.
    	 *
    	 * 3. CURCNT and Upper_count mismatch.  This is where an interrupt occurs after CURCNT is read, which delays the reading of Upper_count.
    	 * Upper_count could then be incremented again.  This would only occur if interrupts held up the code for over 0.5seconds.
    	 */

        NVIC_DisableIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
    	tmrpend0 = NVIC_GetPendingIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
  																    // Check if there is a pending interrupt for timer 1

        __DMB();                                                    // memory barrier: read GP0 before GP1

    	tmrcnt0 = adi_tmr_registers[ADI_TMR_DEVICE_GP0]->CURCNT;    // to minimize skew, read both timers manually

    	__DMB();                                                    // memory barrier: read GP0 before GP1

    	tmrcnt1 = adi_tmr_registers[ADI_TMR_DEVICE_GP1]->CURCNT;    // read both timers manually

    	totaltmr0 = tmrcnt0;        // expand to u32 bits
    	totaltmr1 = tmrcnt1;        // expand to u32 bits

    	tmrcnt0 &= 0xff00u;
    	tmrcnt1 <<= 8;

        __DMB();

    	uc1 = *ucptr;						// Read Upper_count as late as possible to allow interrupt to happen
    	
    	tmrpend1 = NVIC_GetPendingIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
    	
    	// Check for a pending interrupt.  Only read the timer if pending state matches
        NVIC_EnableIRQ(adi_tmr_interrupt[ADI_TMR_DEVICE_GP1]);
    } while ((tmrcnt0 != tmrcnt1) || (tmrpend0 != tmrpend1));

	totaltmr1 <<= 8;                 // Timer1 runs 256x slower
	totaltmr1 += totaltmr0 & 0xffu;  // Use last 8 bits of Timer0 as it runs faster
									 // totaltmr1 now contain 24 bits of significance

	if (tmrpend0) {					 // If an interrupt is pending, then increment local copy of upper count
		uc1++;
	}

	uint64_t Uc = totaltmr1;         // expand out to 64 bits
	Uc += ((uint64_t) uc1) << 24;

	Uc *= 1290555u;                  // (1/26) << 25
	Uc >>= 25;

	return Uc;
}


static void calc_event_counts(uint32_t timestamp)
{
    uint32_t calc_time, blocks, offset;
	uint64_t aa;

    calc_time = get_current_time();
    offset = timestamp - calc_time;             // offset in useconds

    if (offset > 0xf0000000u)					// if offset is a really big number, assume that timer has already expired (i.e. negative)
    	offset = 0u;

    if (offset > 10u) {							// it takes 25us to user timer routine after interrupt.  Offset timer to account for that.
    	offset -= 10u;
    } else
    	offset = 0u;

    aa = (uint64_t) offset;
    aa *= 26u;

    blocks = aa >> 7;
    blocks++;                                   // round

	largecnt = blocks>>1;                       // communicate to event_timer() routine
}

void event_timer()
{
    if (largecnt) {
    	uint32_t cnt = largecnt;

    	if (cnt > 65535u) {
    		cnt = 0u;
    	} else
    		cnt = 65536u - cnt;

    	tmr2Config.nLoad        = cnt;
    	tmr2Config.nAsyncLoad   = cnt;
    	adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP2, &tmr2Config);
        adi_tmr_Enable(ADI_TMR_DEVICE_GP2, true);
    } else {
    	us_ticker_irq_handler();
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
void GP2CallbackFunction(void *pCBParam, uint32_t Event, void  * pArg)
{
   	if (largecnt >= 65536u) {
   		largecnt -= 65536u;
   	} else
   		largecnt = 0;

   	if (largecnt < 65536u) {
       	adi_tmr_Enable(ADI_TMR_DEVICE_GP2, false);
       	event_timer();
   	}
}


/*---------------------------------------------------------------------------*
   us_ticker HAL APIs
 *---------------------------------------------------------------------------*/
void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    us_ticker_inited = 1;

    //adi_pwr_Init();

    //adi_pwr_SetClockDivider(ADI_CLOCK_HCLK, 1u);
    //adi_pwr_SetClockDivider(ADI_CLOCK_PCLK, 1u);   // PCLK = 26MHz

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
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_1;     // TMR0 at 6.5MHz
    tmrConfig.eClockSource = ADI_TMR_CLOCK_PCLK;      // TMR source is PCLK (most examples use HFOSC)
    tmrConfig.nLoad        = 0;
    tmrConfig.nAsyncLoad   = 0;
    tmrConfig.bReloading   = false;
    tmrConfig.bSyncBypass  = true;                    // Allow x1 prescale: requires PCLK as a clk
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP0, &tmrConfig);

    /* Configure GP1 to have a period 256 times longer than GP0 */
    tmrConfig.nLoad        = 0;
    tmrConfig.nAsyncLoad   = 0;
    tmrConfig.ePrescaler   = ADI_TMR_PRESCALER_256;    // TMR1 = 6.5MHz/256
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, &tmrConfig);

    /* Configure GP2 for doing event counts */
    tmr2Config.bCountingUp  = true;
    tmr2Config.bPeriodic    = true;
    tmr2Config.ePrescaler   = ADI_TMR_PRESCALER_256;   // TMR2 at 26MHz/256
    tmr2Config.eClockSource = ADI_TMR_CLOCK_PCLK;      // TMR source is PCLK (most examples use HFOSC)
    tmr2Config.nLoad        = 0;
    tmr2Config.nAsyncLoad   = 0;
    tmr2Config.bReloading   = false;
    tmr2Config.bSyncBypass  = true;                    // Allow x1 prescale
    adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP2, &tmr2Config);


    /*------------------------- GP TIMER ENABLE ------------------------------*/

    /* Manually enable both timers to get them started at the same time
     *
     * Start Timer 0 first.  This is important as detailed in get_current_time().
     * TMR0 should tick over first, followed shortly after by TMR1.
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

    /* timestamp is when interrupt should fire.
     * We only have 16 bit timer, but we need to setup for interrupt at 32 bits.
     *
     *
     */
	calc_event_counts(timestamp);             // use timestamp to calculate largecnt to control number of timer interrupts
	event_timer();							  // uses largecnt to initiate timer interrupts
}

/*
** EOF
*/
