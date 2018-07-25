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

/* The LP Ticker performs two functions for mbed:
 *
 * 1.  Allows tracking of the passage of time.
 * 2.  Allows the system to enter the lowest power
 *     state for a given time.
 *
 * For this to work the single RTC interrupt needs
 * to perform two functions.  It needs to increment
 * an overflow counter at every 32-bit overflow without
 * otherwise affecting the system state (i.e. not waking it
 * up and not putting it to sleep) and, when requested,
 * it *also* needs to wake the system up from sleep
 * at a specific time.  Note also that the units of time
 * from an mbed perspective are useconds, whereas the RTC
 * is clocked at 32 kHz, hence there is conversion to be done.
 *
 * Since it is not possible to reset the RTC, we maintain
 * a 32-bit window on it, starting at g_last_32bit_overflow_value
 * and ending at g_next_32bit_overflow_value.  All values
 * fed back up to mbed are relative to g_last_32bit_overflow_value.
 */

#include "lp_ticker_api.h"
#include "sleep_api.h"
#include "mbed_critical.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* The maximum value of the RTC (48 bits) */
#define RTC_MAX 0x0000FFFFFFFFFFFFULL

/* RTC modulo */
#define RTC_MODULO (RTC_MAX + 1)

/* The 32-bit overflow value */
#define MODULO_32BIT 0x100000000ULL

/* Macro to increment a 64-bit RTC value x by y, with wrap */
#define INCREMENT_MOD(x, y)  (x = ((uint64_t) x + (uint64_t) y) % RTC_MODULO)

/* Macro to get MSBs from a 64-bit integer */
#define MSBS(x) ((uint32_t) ((uint64_t) (x) >> 32))

/* Macro to get LSBs from a 64-bit integer */
#define LSBS(x) ((uint32_t) (x))

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* Incremented each time the RTC goes over 32 bits */
static uint32_t g_overflow_count = 0;

/* Set when a user interrupt has been requested but an overflow
 * interrupt needs to happen first */
static bool g_user_interrupt_pending = false;

/* Set when a user interrupt is the next interrupt to happen */
static bool g_user_interrupt_set = false;

/* Initialised flag, used to protect against interrupts going
 * off before we're initialised */
static bool g_initialised = false;

/* The next overflow value to be used */
static uint64_t g_next_32bit_overflow_value;

/* The next match-compare value to be used */
static uint64_t g_next_compare_value;

/* Keep track of the previous 32-bit overflow
 * value so that we can report 32-bit time
 * correctly */
static uint64_t g_last_32bit_overflow_value;

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

static void set_interrupt_to_32bit_overflow(void);
static void set_interrupt_to_user_value(void);

/* ----------------------------------------------------------------
 * STATIC FUNCTIONS
 * ----------------------------------------------------------------*/

/* Convert a tick value (32,768 Hz) into a microsecond value */
static inline uint32_t ticksToUSeconds(uint32_t x)
{
    /* TODO: find a way to avoid the multiply by 1000000
     * Shift by 20 would introduce a 5% error, which is
     * probably too much */
    uint64_t result = ((((uint64_t) x) * 1000000) >> 15);

    if (result > 0xFFFFFFFF) {
        result = 0xFFFFFFFF;
    }

    return (uint32_t) result;
}

/* Convert a microsecond value into a tick value (32,768 Hz) */
static inline uint32_t uSecondsToTicks(uint32_t x)
{
    /* TODO: find a way to avoid the divide by 1000000
     * Shift by 20 would introduce a 5% error, which is
     * probably too much */
    return (uint32_t) ((((uint64_t) x) << 15) / 1000000);
}

/* Take g_next_32bit_overflow_value and apply it to g_next_compare_value and
 * then the chip registers
 * NOTE: the RTC interrupt should be disabled when calling this function */
static inline void set_interrupt_to_32bit_overflow()
{
    /* Load up the values */
    g_next_compare_value = g_next_32bit_overflow_value;

    /* Set up the match register values */
    RTC_IRQ_TIME_MSBS = MSBS(g_next_compare_value);
    RTC_IRQ_TIME_LSBS = LSBS(g_next_compare_value);
}

/* Take g_next_compare_value and apply it to the chip registers
 * NOTE: the RTC interrupt should be disabled when calling this function */
static inline void set_interrupt_to_user_value()
{
    g_user_interrupt_set = true;

    /* Write MSBS first, then the value is latched on LSBS write */
    RTC_IRQ_TIME_MSBS = MSBS(g_next_compare_value);
    RTC_IRQ_TIME_LSBS = LSBS(g_next_compare_value);
}

/* Get the RTC value
 * NOTE: the RTC interrupt should be disabled when calling this function */
static inline uint64_t get_rtc_value()
{
    uint64_t rtc_value;

    rtc_value = ((uint64_t) RTC_TIME_MSBS) << 32;
    rtc_value |= RTC_TIME_LSBS;

    return rtc_value;
}

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

/* RTC handler */
void IRQ0_RTC_Handler(void)
{
    /* Have seen this interrupt occurring before initialisation, so guard
     * against that */
    if (g_initialised) {
        if (g_user_interrupt_pending) {
            /* If there was a user interrupt pending, set it now */
            set_interrupt_to_user_value();

            /* Reset the pending flag */
            g_user_interrupt_pending = false;

            /* This must have been a 32-bit overflow interrupt so
             * increment the count */
            g_overflow_count++;
            g_last_32bit_overflow_value = g_next_32bit_overflow_value;
            INCREMENT_MOD(g_next_32bit_overflow_value, MODULO_32BIT);
        } else {
            if (g_user_interrupt_set) {
                /* It's a user interrupt, so wake from sleep but don't
                 * increment the overflow count as this is not an
                 * overflow interrupt */

                /* Reset the user interrupt flag and call mbed */
                g_user_interrupt_set = false;
                lp_ticker_irq_handler();
            } else {
                /* Increment the count as this was a 32-bit overflow
                 * interrupt rather than a user interrupt */
                g_overflow_count++;
                g_last_32bit_overflow_value = g_next_32bit_overflow_value;
                INCREMENT_MOD(g_next_32bit_overflow_value, MODULO_32BIT);
            }

            /* Set the next interrupt to be at the 32-bit overflow */
            set_interrupt_to_32bit_overflow();
        }
    }

    /* Clear the interrupt */
    RTC_IRQ_CLR = 0xFFFFFFFF;
}

/* ----------------------------------------------------------------
 * MBED API CALLS
 * ----------------------------------------------------------------*/

/* This will be called once at start of day to get the RTC running */
void lp_ticker_init(void)
{
    if (!g_initialised) {
        /* Reset the overflow count and the flags */
        g_overflow_count = 0;
        g_user_interrupt_pending = false;
        g_user_interrupt_set = false;

        /* Setup the next natural 32-bit overflow value */
        g_next_32bit_overflow_value = get_rtc_value();
        g_last_32bit_overflow_value = g_next_32bit_overflow_value;
        INCREMENT_MOD(g_next_32bit_overflow_value, MODULO_32BIT);

        /* Clear the interrupt */
        RTC_IRQ_CLR = 0xFFFFFFFF;

        /* Interrupt at 32-bit overflow */
        set_interrupt_to_32bit_overflow();

        /* Enable the interrupt */
        g_initialised = true;
        NVIC_EnableIRQ(RTC_IRQn);
    }
}

uint32_t lp_ticker_read(void)
{
    uint64_t rtcNow;

    /* Disable interrupts to avoid collisions */
    core_util_critical_section_enter();

    /* Just in case this is called before initialisation has been performed */
    if (!g_initialised) {
        lp_ticker_init();
    }

    /* What mbed expects here is a 32 bit timer value.  There is no
     * way to reset the RTC so, to pretend it is 32 bits, we have to
     * maintain a 32-bit window on it using the remembered overflow
     * value */
    rtcNow = get_rtc_value();

    /* Put interrupts back */
    core_util_critical_section_exit();

    return ticksToUSeconds(rtcNow - g_last_32bit_overflow_value);
}

void lp_ticker_set_interrupt(timestamp_t time)
{
    uint32_t timeNow = get_rtc_value() - g_last_32bit_overflow_value;
    uint32_t timeOffset = uSecondsToTicks(time) - timeNow;

    /* Disable interrupts to avoid collisions */
    core_util_critical_section_enter();

    g_user_interrupt_pending = false;
    g_user_interrupt_set = false;

    /* Handle time slipping into the past */
    if (timeOffset > 0xEFFFFFFF) {
        timeOffset = 100;
    }

    /* Read the current time */
    g_next_compare_value = get_rtc_value();

    /* Add the offset */
    INCREMENT_MOD(g_next_compare_value, timeOffset);

    /* We must let the normal overflow interrupt occur as
     * well as setting this interrupt so, if the value
     * of 'time' would occur after the overflow point,
     * put the change of compare-value off until afterwards. */
    /* TODO: this needs proper testing. */
    if (g_next_32bit_overflow_value > g_next_compare_value) {
        /* The easy case, no overlap */
    } else {
        /* Could be because g_next_compare_value has wrapped (around the
         * 48-bit limit of the RTC) */
        if (g_next_32bit_overflow_value - g_next_compare_value >= MODULO_32BIT) {
            /* The wrap case, we're OK */
        } else {
            /* There is an overlap, apply the value later */
            g_user_interrupt_pending = true;

            if (g_next_32bit_overflow_value == g_next_compare_value) {
                /* If they are on top of each other, bump this
                 * one forward to avoid losing the interrupt */
                INCREMENT_MOD(g_next_compare_value, 2);
            }
        }
    }

    if (!g_user_interrupt_pending) {
        /* Make the change immediately */
        set_interrupt_to_user_value();
    }

    /* Put interrupts back */
    core_util_critical_section_exit();
}

void lp_ticker_fire_interrupt(void)
{
    // user interrupt only set, this will invoke from ISR routine directly lp handler
    g_user_interrupt_pending = false;
    g_user_interrupt_set = true;
    NVIC_SetPendingIRQ(RTC_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    /* Can't disable interrupts as we need them to manage
     * overflow.  Instead, switch off the user part. */
    g_user_interrupt_pending = false;
    g_user_interrupt_set = false;
}

void lp_ticker_clear_interrupt(void)
{
    /* Can't disable interrupts as we need them to manage
     * overflow.  Instead, switch off the user part. */
    g_user_interrupt_pending = false;
    g_user_interrupt_set = false;
}

void lp_ticker_free(void)
{

}
