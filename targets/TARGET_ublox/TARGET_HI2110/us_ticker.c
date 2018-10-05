/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* The usecond ticker is mapped to TIMER0.  A few issues must be dealt
 * with in this driver:
 *
 * 1.  The us_ticker API must count upwards, not down.
 * 2.  The expected range/resolution is 32 bits each of 1 usecond,
 *     whereas TIMER0 runs at 48 MHz (not 1 MHz) and so actually
 *     has a range/resolution of 26 bits at 0.02 useconds.  Software
 *     has to compensate for this.
 */

#include "us_ticker_api.h"
#include "mbed_critical.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* TIMER0 clock is 48 MHz */
#define CLOCK_TICKS_PER_US 48

/* The number of clock ticks in a full-run of
 * TIMER0, scaled to represent useconds */
#define USECONDS_PER_FULL_TIMER0_RUN 89478485

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* Are we ready? */
static bool g_initialised = false;

/* Keep track of the number of useconds elapsed. */
static uint32_t g_us_overflow = 0;

/* The number of useconds to increment the by at each interrupt */
static uint32_t g_us_overflow_increment = USECONDS_PER_FULL_TIMER0_RUN;

/* Keep track of extra loops required to represent a particular time
 * as the HW timer runs faster than 1 MHz */
static uint32_t g_timer_extra_loops_required = 0;
static uint32_t g_timer_extra_loops_done = 0;

/* Keep track of any adjustment due to user interrupts . */
static uint32_t g_user_interrupt_offset = 0;

/* Flag that a user timer is running */
static bool g_user_interrupt = false;

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

static inline uint32_t divide_by_48(uint32_t x);

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

/* Perform a divide-by-48 operation.
 * This is done as a multiply-shift operation to take advantage of
 * the ARM 32 bit single-cycle multiply and avoid using division;
 * 1/48 is equivalent to 1365/2^16.  It is also done in two halves
 * to make sure that the multiplies fit into 32 bits.
 *
 * The principle is:
 *  - divide the top 16 bits by 48 using multiply-shift (=> x1),
 *  - work out the remainder of that operation and divide that by 48 (=> x1r),
 *  - divide the bottom 16 bits by 48 using multiply-shift (=> x2),
 *  - add the lot together to get the result.
 *
 * The cost is 29 instructions.
 */
static inline uint32_t divide_by_48(uint32_t x)
{
    uint32_t x1 = ((x >> 16) * 1365) >> 16;
    uint32_t x1r = ((x & 0xFFFF0000) - ((x1 * 48) << 16));
             x1r = (x1r  * 1365) >> 16;
    uint32_t x2 = ((x & 0xFFFF) * 1365) >> 16;

    return (x1 << 16) + x1r + x2;
}

/* Timer0 handler */
void IRQ1_TMR0_Handler(void)
{
    if (g_initialised) {
        /* Increment the overflow count and set the increment
         * value for next time */
        g_us_overflow += g_us_overflow_increment;
        g_us_overflow_increment = USECONDS_PER_FULL_TIMER0_RUN;

        /* Now handle the user interrupt case */
        if (g_user_interrupt) {
            if (g_timer_extra_loops_done < g_timer_extra_loops_required) {
                /* Let the timer go round again */
                g_timer_extra_loops_done++;
            } else {
                /* We've done with looping around for a user interrupt */
                g_user_interrupt = false;

                /* Call the mbed API */
                us_ticker_irq_handler();
            }
        }
    }

    NVIC_ClearPendingIRQ(Timer_IRQn);
}

/* ----------------------------------------------------------------
 * MBED API CALLS
 * ----------------------------------------------------------------*/

void us_ticker_init(void)
{
    if (!g_initialised) {
        /* Reset the globals */
        g_timer_extra_loops_done = 0;
        g_timer_extra_loops_required = 0;
        g_us_overflow = 0;
        g_us_overflow_increment = USECONDS_PER_FULL_TIMER0_RUN;
        g_user_interrupt_offset = 0;
        g_user_interrupt = false;

        /* Get the timer running (starting at what is zero,
         * once inverted), with repeat */
        NVIC_ClearPendingIRQ(Timer_IRQn);
        TIMER0_LOAD = 0xFFFFFFFF;
        TIMER0_CTRL = 0x03;
        NVIC_EnableIRQ(Timer_IRQn);

        g_initialised = true;
    }
}

uint32_t us_ticker_read()
{
    uint32_t timeValue;

    /* This can be called before initialisation has been performed */
    if (!g_initialised) {
        us_ticker_init();
    }

    /* Disable interrupts to avoid collisions */
    core_util_critical_section_enter();

    /* Get the timer value, adding the offset in case we've been moved
     * around by user activity, inverting it (as a count-up timer is
     * expected), then scaling it to useconds and finally adding the
     * usecond overflow value to make up the 32-bit usecond total */
    timeValue = divide_by_48(~(TIMER0_TIME + g_user_interrupt_offset)) + g_us_overflow;

    /* Put interrupts back */
    core_util_critical_section_exit();

    return timeValue;
}

/* NOTE: it seems to be an accepted fact that users
 * will never ask for a timeout of more than 2^31 useconds
 * and hence it's possible to do signed arithmetic
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    g_timer_extra_loops_required = 0;
    g_timer_extra_loops_done = 0;
    int32_t timeDelta;

    /* Disable interrupts to avoid collisions */
    core_util_critical_section_enter();

    /* Establish how far we're being asked to move */
    timeDelta = (int32_t) ((uint32_t) timestamp - us_ticker_read());

    if (timeDelta <= 0) {
        /* Make delta positive if it's not, it will expire pretty quickly */
        /* Note: can't just call us_ticker_irq_handler() directly as we
         * may already be in it and will overflow the stack */
        timeDelta = 1;
    }

    /* The TIMER0 clock source is greater than 1 MHz, so
     * work out how many times we have to go around
     * and what the remainder is */
    g_timer_extra_loops_required = (uint32_t) timeDelta / USECONDS_PER_FULL_TIMER0_RUN;
    timeDelta -= g_timer_extra_loops_required * USECONDS_PER_FULL_TIMER0_RUN;

    /* Next time we hit the interrupt the increment will be smaller */
    g_us_overflow_increment = (uint32_t) timeDelta;

    /* We're about to modify the timer value; work out the
     * difference so that we can compensate for it when
     * the time is read */
    timeDelta = timeDelta * CLOCK_TICKS_PER_US;
    g_user_interrupt_offset += TIMER0_TIME - timeDelta;

    /* Run for the remainder first, then we can loop for the full
     * USECONDS_PER_FULL_TIMER0_RUN afterwards */
    TIMER0_LOAD = timeDelta;

    /* A user interrupt is now running */
    g_user_interrupt = true;

    /* Put interrupts back */
    core_util_critical_section_exit();
}

void us_ticker_fire_interrupt(void)
{
    g_user_interrupt = true;
    NVIC_SetPendingIRQ(Timer_IRQn);
}

void us_ticker_disable_interrupt(void)
{
    /* Can't actually disable the interrupt here
     * as we need it to manage the timer overflow,
     * instead switch off the user interrupt part */
    g_user_interrupt = false;
    g_timer_extra_loops_required = 0;
    g_us_overflow_increment = 0;
}

void us_ticker_clear_interrupt(void)
{
    /* As above, can't clear the interrupt as it
     * may just be an overflow interrupt, instead
     * clear the variables */
    g_user_interrupt = false;
    g_timer_extra_loops_required = 0;
    g_us_overflow_increment = 0;
}

void us_ticker_free(void)
{

}
