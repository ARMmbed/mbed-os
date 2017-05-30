/******************************************************************************
 * @file:    us_ticker.c
 * @brief:   GPT timer example
 * @version: $Revision: $
 * @date:    $Date: $
 *-----------------------------------------------------------------------------
 *
 Copyright (c) 2011-2017 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/** \addtogroup GPT_Test GPT Test
 *  Example code demonstrating use of the GPIO functions.
 *  @{
 */

#include <stdint.h>
#include <stdio.h>
#include <drivers/tmr/adi_tmr.h>
#include <drivers/pwr/adi_pwr.h>
#include <drivers/gpio/adi_gpio.h>

#define TIMER_MAX_VALUE	0xFFFF

#define MIN_TIME_LOAD 50

#define TIMER_CLK_FREQ_MHZ  26
#define TIMER_PRESCALER     16

// Prescaler value of 64 gives 2.46us/tick where as
// a prescaler value of 16 gives 0.62us/tick
#if TIMER_PRESCALER==64
#define CORE_TIME_REG_MAX 0x6800
#elif TIMER_PRESCALER==16
#define CORE_TIME_REG_MAX 0x1A000
#else
#error "CORE_TIME_REG_MAX undefined!"
#endif

typedef uint32_t timestamp_t;

typedef union {
    unsigned long long u64_latest_timer_expiry;
    uint16_t u16_latest_timer_expiry[4];
} latest_timer_expiry_t;

latest_timer_expiry_t latest_timer_expiry;

volatile uint32_t Core_Time_Tick = 0;

bool timer_present = false;

static uint8_t Capture_timer_running;

static uint16_t get_elapsed_time();
static void Tmr0_Int_Callback( void *pCBParam, uint32_t Event, void *pArg );
static void Tmr1_Int_Callback( void *pCBParam, uint32_t Event, void *pArg );

// defined in mbed_us_ticker_api.c which calls the ticker_irq_handler() routine
// defined in mbed_ticker_api.c
void us_ticker_irq_handler(void);

static int us_ticker_inited = 0;

uint32_t prev_time, current_time;

static ADI_TMR_CONFIG tmr0_config, tmr1_config;

/*---------------------------------------------------------------------------*/

static void Tmr0_Int_Callback( void *pCBParam, uint32_t Event, void *pArg )
{
    uint32_t major_ticks;

    Core_Time_Tick++;

    // CORE_TIME_REG_MAX is set to give a maximum of 2^32 ticks,
    // i.e. CORE_TIME_REG_MAX * 65536 * TIMER_PRESCALER / TIMER_CLK_FREQ_MHZ = 2^32
    // based on oscillator and the value of prescaler. Core_Time_Tick is
    // incremented once every 65536 ticks.
    if(Core_Time_Tick >= CORE_TIME_REG_MAX) {
        Core_Time_Tick = 0;
    }

    // timer_present indicates the timer has been started and running
    if(true == timer_present) {
        major_ticks = (latest_timer_expiry.u16_latest_timer_expiry[2] << 16) | \
                      latest_timer_expiry.u16_latest_timer_expiry[1];

        // check the number of major ticks (each made up of 65536 clock ticks).
        if(Core_Time_Tick == major_ticks) {
            // if the required clock ticks happens to be 0, that means we have
            // reached the required time out, so call the IRQ handler. Otherwise
            // start the remainder count up in TIMER1 by using the reload value of
            // (TIMER_MAX_VALUE - u16_latest_timer_expiry[0])
            // Capture_timer_running is set to 1 to indicate TIMER1 is running.
            if(0 == latest_timer_expiry.u16_latest_timer_expiry[0]) {
                timer_present = false;

                /* Invoke Callback Function */
                us_ticker_irq_handler();
            } else {
                Capture_timer_running = 1;

                /* Start Timer1 in Periodic mode */
                adi_tmr_Enable(ADI_TMR_DEVICE_GP1, true);
            }
        }
    }
}

static void Tmr1_Int_Callback( void *pCBParam, uint32_t Event, void *pArg )
{
    timer_present = false;

    Capture_timer_running = 0;

    /* Disable Timer1 */
    adi_tmr_Enable(ADI_TMR_DEVICE_GP1, false);

    /* Invoke Callback Function */
    us_ticker_irq_handler();
}


/*---------------------------------------------------------------------------*/
static ADI_TMR_RESULT Init_timer(ADI_TMR_DEVICE eDevice,
                                 ADI_TMR_CONFIG *pTmr_config,
                                 ADI_CALLBACK   const   pfCallback)
{
    ADI_TMR_RESULT result;

    result = adi_tmr_Init(eDevice, pfCallback, NULL, true);

    if (result == ADI_TMR_SUCCESS) {
        pTmr_config->bCountingUp = true;
        pTmr_config->bPeriodic = (eDevice == ADI_TMR_DEVICE_GP0) ? false : true;
#if TIMER_PRESCALER==64
        pTmr_config->ePrescaler = ADI_TMR_PRESCALER_64;
#elif TIMER_PRESCALER==16
        pTmr_config->ePrescaler = ADI_TMR_PRESCALER_16;
#else
#error "Invalid timer prescaler value!"
#endif
        pTmr_config->eClockSource = ADI_TMR_CLOCK_HFOSC;
        pTmr_config->nLoad = 0;
        pTmr_config->nAsyncLoad = 0;
        pTmr_config->bReloading = false;
        pTmr_config->bSyncBypass = false;

        result = adi_tmr_ConfigTimer(eDevice, *pTmr_config);
    }

    return result;
}

static void tmr_init()
{
    ADI_TMR_RESULT result;

    // Timer0 is configured as a free running count up timer
    result = Init_timer(ADI_TMR_DEVICE_GP0, &tmr0_config, Tmr0_Int_Callback);

    // Timer1 is configured as a periodic count up timer.
    if (result == ADI_TMR_SUCCESS)
        adi_tmr_Enable(ADI_TMR_DEVICE_GP0, true);

    // Timer1 is configured as a periodic count up timer.
    Init_timer(ADI_TMR_DEVICE_GP1, &tmr1_config, Tmr1_Int_Callback);
}

/*---------------------------------------------------------------------------*/
static uint32_t get_current_time(void)
{
    unsigned long long ticks;
    uint32_t current_time;

    ticks = ((unsigned long long)Core_Time_Tick << 16) + get_elapsed_time();
    current_time = (uint32_t)(ticks / TIMER_CLK_FREQ_MHZ * TIMER_PRESCALER);

    return current_time;
}
/*---------------------------------------------------------------------------*/

static uint16_t get_elapsed_time()
{
    uint16_t timer_value;

    adi_tmr_GetCurrentCount(ADI_TMR_DEVICE_GP0, &timer_value);

    return timer_value;
}

static int StopTimer()
{
    timer_present = false;

    if(Capture_timer_running) {
        adi_tmr_Enable(ADI_TMR_DEVICE_GP1, false);
        Capture_timer_running = 0;
    }

    return 0;
}

static int StartTimer(uint32_t expiry_time)
{
    uint32_t curr_time, major_ticks;

    // calculate the number of ticks the expiry_time requires. The time
    // is specified in micro seconds.
    latest_timer_expiry.u64_latest_timer_expiry = (unsigned long long)expiry_time / \
            TIMER_PRESCALER * TIMER_CLK_FREQ_MHZ;

    // if the current major tick is already the same as the required time stamp,
    // need to check the lower 16 bit of the time stamp
    major_ticks = (latest_timer_expiry.u16_latest_timer_expiry[2] << 16) | \
                  latest_timer_expiry.u16_latest_timer_expiry[1];

    if(Core_Time_Tick == major_ticks) {
        // get the current time in the timer0
        curr_time = get_current_time() & 0x0000FFFF;

        // if current time is larger or equal to the required time stamp
        // value. Note that this condition shouldn't happen, as the required
        // time stamp is in the past.
        // Here timer1 is set to count down from max value of 65536 instead
        // of triggering IRQ handler immediately.
        if(curr_time >= latest_timer_expiry.u16_latest_timer_expiry[0]) {
            timer_present = false;

            tmr1_config.nLoad = TIMER_MAX_VALUE - MIN_TIME_LOAD;
            adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmr1_config);

            Capture_timer_running = 1;

            adi_tmr_Enable(ADI_TMR_DEVICE_GP1, true);
        } else {
            // Otherwise load and count up the remainder ticks using TIMER1
            /* Start Timer1 in Periodic mode by loading TxLD Register
            with a value of (TIMER_MAX_VALUE - (u16_latest_timer_expiry[0] - curr_time))*/
            tmr1_config.nLoad = TIMER_MAX_VALUE - (latest_timer_expiry.u16_latest_timer_expiry[0] - curr_time);
            adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmr1_config);

            Capture_timer_running = 1;

            adi_tmr_Enable(ADI_TMR_DEVICE_GP1, true);
        }
    } else {
        timer_present = true;

        /*Load TxLD Register with a value of (TIMER_MAX_VALUE - u16_latest_timer_expiry[0])*/
        tmr1_config.nLoad = TIMER_MAX_VALUE - latest_timer_expiry.u16_latest_timer_expiry[0];
        adi_tmr_ConfigTimer(ADI_TMR_DEVICE_GP1, tmr1_config);
    }

    return 0;
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    us_ticker_inited = 1;
    prev_time = 0;
    current_time = 0;
    tmr_init();
}

uint32_t us_ticker_read()
{
    int diff;
    if (!us_ticker_inited) {
        us_ticker_init();
    }
    prev_time = current_time;
    current_time = get_current_time();
    diff = (current_time - prev_time);
    if (diff < 0) current_time = prev_time;
    return current_time;
}

void us_ticker_disable_interrupt(void)
{
    StopTimer();
}

void us_ticker_clear_interrupt(void)
{
    NVIC_ClearPendingIRQ(TMR1_EVT_IRQn);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    StartTimer(timestamp);
}

/*
** EOF
*/

/*@}*/

