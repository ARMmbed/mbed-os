/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include <stdbool.h>
#include "us_ticker_api.h"
#include "cmsis.h"
#include "PeripheralNames.h"
#include "nrf_delay.h"
#include "mbed_toolchain.h"

/*
 * Note: The micro-second timer API on the nRF51 platform is implemented using
 * the RTC counter run at 32kHz (sourced from an external oscillator). This is
 * a trade-off between precision and power. Running a normal 32-bit MCU counter
 * at high frequency causes the average power consumption to rise to a few
 * hundred micro-amps, which is prohibitive for typical low-power BLE
 * applications.
 * A 32kHz clock doesn't offer the precision needed for keeping u-second time,
 * but we're assuming that this will not be a problem for the average user.
 */

#define MAX_RTC_COUNTER_VAL     0x00FFFFFF               /**< Maximum value of the RTC counter. */
#define RTC_CLOCK_FREQ          (uint32_t)(32768)
#define RTC1_IRQ_PRI            3                        /**< Priority of the RTC1 interrupt (used
                                                          *  for checking for timeouts and executing
                                                          *  timeout handlers). This must be the same
                                                          *  as APP_IRQ_PRIORITY_LOW; taken from the
                                                          *  Nordic SDK. */
#define MAX_RTC_TASKS_DELAY     47                       /**< Maximum delay until an RTC task is executed. */

#define FUZZY_RTC_TICKS          2  /* RTC COMPARE occurs when a CC register is N and the RTC
                                     * COUNTER value transitions from N-1 to N. If we're trying to
                                     * setup a callback for a time which will arrive very shortly,
                                     * there are limits to how short the callback interval may be for us
                                     * to rely upon the RTC Compare trigger. If the COUNTER is N,
                                     * writing N+2 to a CC register is guaranteed to trigger a COMPARE
                                     * event at N+2. */

#define RTC_UNITS_TO_MICROSECONDS(RTC_UNITS) (((RTC_UNITS) * (uint64_t)1000000) / RTC_CLOCK_FREQ)
#define MICROSECONDS_TO_RTC_UNITS(MICROS)    ((((uint64_t)(MICROS) * RTC_CLOCK_FREQ) + 999999) / 1000000)

static bool              us_ticker_inited = false;
static volatile uint32_t overflowCount;                   /**< The number of times the 24-bit RTC counter has overflowed. */
static volatile bool     us_ticker_callbackPending = false;
static uint32_t          us_ticker_callbackTimestamp;
static bool              os_tick_started = false;         /**< flag indicating if the os_tick has started */
/**
 * The value previously set in the capture compare register of channel 1
 */
static uint32_t          previous_tick_cc_value = 0;

/*
 RTX provide the following definitions which are used by the tick code:
   * os_trv: The number (minus 1) of clock cycle between two tick.
   * os_clockrate: Time duration between two ticks (in us).
   * OS_Tick_Handler: The function which handle a tick event.
     This function is special because it never returns.
 Those definitions are used by the code which handle the os tick.
 To allow compilation of us_ticker programs without RTOS, those symbols are
 exported from this module as weak ones.
 */
MBED_WEAK uint32_t const os_trv;
MBED_WEAK uint32_t const os_clockrate;
MBED_WEAK void OS_Tick_Handler() { }

static inline void rtc1_enableCompareInterrupt(void)
{
    NRF_RTC1->EVTENCLR = RTC_EVTEN_COMPARE0_Msk;
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk;
}

static inline void rtc1_disableCompareInterrupt(void)
{
    NRF_RTC1->INTENCLR = RTC_INTENSET_COMPARE0_Msk;
    NRF_RTC1->EVTENCLR = RTC_EVTEN_COMPARE0_Msk;
}

static inline void rtc1_enableOverflowInterrupt(void)
{
    NRF_RTC1->EVTENCLR = RTC_EVTEN_OVRFLW_Msk;
    NRF_RTC1->INTENSET = RTC_INTENSET_OVRFLW_Msk;
}

static inline void rtc1_disableOverflowInterrupt(void)
{
    NRF_RTC1->INTENCLR = RTC_INTENSET_OVRFLW_Msk;
    NRF_RTC1->EVTENCLR = RTC_EVTEN_OVRFLW_Msk;
}

static inline void invokeCallback(void)
{
    us_ticker_callbackPending = false;
    rtc1_disableCompareInterrupt();
    us_ticker_irq_handler();
}

/**
 * @brief Function for starting the RTC1 timer. The RTC timer is expected to
 * keep running--some interrupts may be disabled temporarily.
 */
static void rtc1_start()
{
    NRF_RTC1->PRESCALER = 0; /* for no pre-scaling. */

    rtc1_enableOverflowInterrupt();

    NVIC_SetPriority(RTC1_IRQn, RTC1_IRQ_PRI);
    NVIC_ClearPendingIRQ(RTC1_IRQn);
    NVIC_EnableIRQ(RTC1_IRQn);

    NRF_RTC1->TASKS_START = 1;
    nrf_delay_us(MAX_RTC_TASKS_DELAY);
}

/**
 * @brief Function for stopping the RTC1 timer. We don't expect to call this.
 */
void rtc1_stop(void)
{
    // If the os tick has been started, RTC1 shouldn't be stopped
    // In that case, us ticker and overflow interrupt are disabled.
    if (os_tick_started) {
        rtc1_disableCompareInterrupt();
        rtc1_disableOverflowInterrupt();
    } else {
        NVIC_DisableIRQ(RTC1_IRQn);
        rtc1_disableCompareInterrupt();
        rtc1_disableOverflowInterrupt();

        NRF_RTC1->TASKS_STOP = 1;
        nrf_delay_us(MAX_RTC_TASKS_DELAY);

        NRF_RTC1->TASKS_CLEAR = 1;
        nrf_delay_us(MAX_RTC_TASKS_DELAY);
    }
}

/**
 * @brief Function for returning the current value of the RTC1 counter.
 *
 * @return Current RTC1 counter as a 64-bit value with 56-bit precision (even
 *         though the underlying counter is 24-bit)
 */
static inline uint64_t rtc1_getCounter64(void)
{
    if (NRF_RTC1->EVENTS_OVRFLW) {
        overflowCount++;
        NRF_RTC1->EVENTS_OVRFLW = 0;
        NRF_RTC1->EVTENCLR      = RTC_EVTEN_OVRFLW_Msk;
    }
    return ((uint64_t)overflowCount << 24) | NRF_RTC1->COUNTER;
}

/**
 * @brief Function for returning the current value of the RTC1 counter.
 *
 * @return Current RTC1 counter as a 32-bit value (even though the underlying counter is 24-bit)
 */
static inline uint32_t rtc1_getCounter(void)
{
    return rtc1_getCounter64();
}

/**
 * @brief Function for handling the RTC1 interrupt for us ticker (capture compare channel 0 and overflow).
 *
 * @details Checks for timeouts, and executes timeout handlers for expired timers.
 */
void us_ticker_handler(void)
{
    if (NRF_RTC1->EVENTS_OVRFLW) {
        overflowCount++;
        NRF_RTC1->EVENTS_OVRFLW = 0;
        NRF_RTC1->EVTENCLR      = RTC_EVTEN_OVRFLW_Msk;
    }
    if (NRF_RTC1->EVENTS_COMPARE[0]) {
        NRF_RTC1->EVENTS_COMPARE[0] = 0;
        NRF_RTC1->EVTENCLR          = RTC_EVTEN_COMPARE0_Msk;
        if (us_ticker_callbackPending && ((int)(us_ticker_callbackTimestamp - rtc1_getCounter()) <= 0))
            invokeCallback();
    }
}

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }

    rtc1_start();
    us_ticker_inited = true;
}

uint32_t us_ticker_read()
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    /* Return a pseudo microsecond counter value. This is only as precise as the
     * 32khz low-freq clock source, but could be adequate.*/
    return RTC_UNITS_TO_MICROSECONDS(rtc1_getCounter64());
}

/**
 * Setup the us_ticker callback interrupt to go at the given timestamp.
 *
 * @Note: Only one callback is pending at any time.
 *
 * @Note: If a callback is pending, and this function is called again, the new
 * callback-time overrides the existing callback setting. It is the caller's
 * responsibility to ensure that this function is called to setup a callback for
 * the earliest timeout.
 *
 * @Note: If this function is used to setup an interrupt which is immediately
 * pending--such as for 'now' or a time in the past,--then the callback is
 * invoked a few ticks later.
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited) {
        us_ticker_init();
    }

    /*
     * The argument to this function is a 32-bit microsecond timestamp for when
     * a callback should be invoked. On the nRF51, we use an RTC timer running
     * at 32kHz to implement a low-power us-ticker. This results in a problem
     * based on the fact that 1000000 is not a multiple of 32768.
     *
     * Going from a micro-second based timestamp to a 32kHz based RTC-time is a
     * linear mapping; but this mapping doesn't preserve wraparounds--i.e. when
     * the 32-bit micro-second timestamp wraps around unfortunately the
     * underlying RTC counter doesn't. The result is that timestamp expiry
     * checks on micro-second timestamps don't yield the same result when
     * applied on the corresponding RTC timestamp values.
     *
     * One solution is to translate the incoming 32-bit timestamp into a virtual
     * 64-bit timestamp based on the knowledge of system-uptime, and then use
     * this wraparound-free 64-bit value to do a linear mapping to RTC time.
     * System uptime on an nRF is maintained using the 24-bit RTC counter. We
     * track the overflow count to extend the 24-bit hardware counter by an
     * additional 32 bits. RTC_UNITS_TO_MICROSECONDS() converts this into
     * microsecond units (in 64-bits).
     */
    const uint64_t currentTime64 = RTC_UNITS_TO_MICROSECONDS(rtc1_getCounter64());
    uint64_t timestamp64 = (currentTime64 & ~(uint64_t)0xFFFFFFFFULL) + timestamp;
    if (((uint32_t)currentTime64 > 0x80000000) && (timestamp < 0x80000000)) {
        timestamp64 += (uint64_t)0x100000000ULL;
    }
    uint32_t newCallbackTime = MICROSECONDS_TO_RTC_UNITS(timestamp64);

    /* Check for repeat setup of an existing callback. This is actually not
     * important; the following code should work even without this check. */
    if (us_ticker_callbackPending && (newCallbackTime == us_ticker_callbackTimestamp)) {
        return;
    }

    /* Check for callbacks which are immediately (or will *very* shortly become) pending.
     * Even if they are immediately pending, they are scheduled to trigger a few
     * ticks later. This keeps things simple by invoking the callback from an
     * independent interrupt context. */
    if ((int)(newCallbackTime - rtc1_getCounter()) <= (int)FUZZY_RTC_TICKS) {
        newCallbackTime = rtc1_getCounter() + FUZZY_RTC_TICKS;
    }

    NRF_RTC1->CC[0]             = newCallbackTime & MAX_RTC_COUNTER_VAL;
    us_ticker_callbackTimestamp = newCallbackTime;
    if (!us_ticker_callbackPending) {
        us_ticker_callbackPending = true;
        rtc1_enableCompareInterrupt();
    }
}

void us_ticker_disable_interrupt(void)
{
    if (us_ticker_callbackPending) {
        rtc1_disableCompareInterrupt();
        us_ticker_callbackPending = false;
    }
}

void us_ticker_clear_interrupt(void)
{
    NRF_RTC1->EVENTS_OVRFLW     = 0;
    NRF_RTC1->EVENTS_COMPARE[0] = 0;
}


#if defined (__CC_ARM)         /* ARMCC Compiler */

__asm void RTC1_IRQHandler(void)
{
    IMPORT  OS_Tick_Handler
    IMPORT  us_ticker_handler

    /**
     * Chanel 1 of RTC1 is used by RTX as a systick.
     * If the compare event on channel 1 is set, then branch to OS_Tick_Handler.
     * Otherwise, just execute us_ticker_handler.
     * This function has to be written in assembly and tagged as naked because OS_Tick_Handler
     * will never return.
     * A c function would put lr on the stack before calling OS_Tick_Handler and this value
     * would never been dequeued.
     *
     * \code
     * void RTC1_IRQHandler(void) {
         if(NRF_RTC1->EVENTS_COMPARE[1]) {
             // never return...
             OS_Tick_Handler();
         } else {
             us_ticker_handler();
         }
       }
     * \endcode
     */
    ldr r0,=0x40011144
    ldr r1, [r0, #0]
    cmp r1, #0
    beq US_TICKER_HANDLER
    bl OS_Tick_Handler
US_TICKER_HANDLER
    push {r3, lr}
    bl us_ticker_handler
    pop {r3, pc}
    nop /* padding */
}

#elif defined (__GNUC__)        /* GNU Compiler */

__attribute__((naked)) void RTC1_IRQHandler(void)
{
    /**
     * Chanel 1 of RTC1 is used by RTX as a systick.
     * If the compare event on channel 1 is set, then branch to OS_Tick_Handler.
     * Otherwise, just execute us_ticker_handler.
     * This function has to be written in assembly and tagged as naked because OS_Tick_Handler
     * will never return.
     * A c function would put lr on the stack before calling OS_Tick_Handler and this value
     * would never been dequeued.
     *
     * \code
     * void RTC1_IRQHandler(void) {
         if(NRF_RTC1->EVENTS_COMPARE[1]) {
             // never return...
             OS_Tick_Handler();
         } else {
             us_ticker_handler();
         }
       }
     * \endcode
     */
    __asm__ (
        "ldr r0,=0x40011144\n"
    	"ldr r1, [r0, #0]\n"
    	"cmp r1, #0\n"
    	"beq US_TICKER_HANDLER\n"
    	"bl OS_Tick_Handler\n"
	"US_TICKER_HANDLER:\n"
    	"push {r3, lr}\n"
    	"bl us_ticker_handler\n"
    	"pop {r3, pc}\n"
        "nop"
	);
}

#else

#error Compiler not supported.
#error Provide a definition of RTC1_IRQHandler.

/*
 * Chanel 1 of RTC1 is used by RTX as a systick.
 * If the compare event on channel 1 is set, then branch to OS_Tick_Handler.
 * Otherwise, just execute us_ticker_handler.
 * This function has to be written in assembly and tagged as naked because OS_Tick_Handler
 * will never return.
 * A c function would put lr on the stack before calling OS_Tick_Handler and this value
 * will never been dequeued. After a certain time a stack overflow will happen.
 *
 * \code
 * void RTC1_IRQHandler(void) {
     if(NRF_RTC1->EVENTS_COMPARE[1]) {
         // never return...
         OS_Tick_Handler();
     } else {
         us_ticker_handler();
     }
   }
 * \endcode
 */

#endif

/**
 * Return the next number of clock cycle needed for the next tick.
 * @note This function has been carrefuly optimized for a systick occuring every 1000us.
 */
static uint32_t get_next_tick_cc_delta() {
    uint32_t delta = 0;

    if (os_clockrate != 1000) {
        // In RTX, by default SYSTICK is is used.
        // A tick event is generated  every os_trv + 1 clock cycles of the system timer.
        delta = os_trv + 1;
    } else {
        // If the clockrate is set to 1000us then 1000 tick should happen every second.
        // Unfortunatelly, when clockrate is set to 1000, os_trv is equal to 31.
        // If (os_trv + 1) is used as the delta value between two ticks, 1000 ticks will be
        // generated in 32000 clock cycle instead of 32768 clock cycles.
        // As a result, if a user schedule an OS timer to start in 100s, the timer will start
        // instead after 97.656s
        // The code below fix this issue, a clock rate of 1000s will generate 1000 ticks in 32768
        // clock cycles.
        // The strategy is simple, for 1000 ticks:
        //   * 768 ticks will occur 33 clock cycles after the previous tick
        //   * 232 ticks will occur 32 clock cycles after the previous tick
        // By default every delta is equal to 33.
        // Every five ticks (20%, 200 delta in one second), the delta is equal to 32
        // The remaining (32) deltas equal to 32 are distributed using primes numbers.
        static uint32_t counter = 0;
        if ((counter % 5) == 0 || (counter % 31) == 0 || (counter % 139) == 0 || (counter == 503)) {
            delta = 32;
        } else {
            delta = 33;
        }
        ++counter;
        if (counter == 1000) {
            counter = 0;
        }
    }
    return delta;
}

static inline void clear_tick_interrupt() {
    NRF_RTC1->EVENTS_COMPARE[1] = 0;
    NRF_RTC1->EVTENCLR = (1 << 17);
}

/**
 * Indicate if a value is included in a range which can be wrapped.
 * @param  begin start of the range
 * @param  end   end of the range
 * @param  val   value to check
 * @return       true if the value is included in the range and false otherwise.
 */
static inline bool is_in_wrapped_range(uint32_t begin, uint32_t end, uint32_t val) {
    // regular case, begin < end
    // return true if  begin <= val < end
    if (begin < end) {
        if (begin <= val && val < end) {
            return true;
        } else {
            return false;
        }
    } else {
        // In this case end < begin because it has wrap around the limits
        // return false if end < val < begin
        if (end < val && val < begin)  {
            return false;
        } else {
            return true;
        }
    }

}

/**
 * Register the next tick.
 */
static void register_next_tick() {
    previous_tick_cc_value = NRF_RTC1->CC[1];
    uint32_t delta = get_next_tick_cc_delta();
    uint32_t new_compare_value = (previous_tick_cc_value + delta) & MAX_RTC_COUNTER_VAL;

    // Disable irq directly for few cycles,
    // Validation of the new CC value against the COUNTER,
    // Setting the new CC value and enabling CC IRQ should be an atomic operation
    // Otherwise, there is a possibility to set an invalid CC value because
    // the RTC1 keeps running.
    // This code is very short 20-38 cycles in the worst case, it shouldn't
    // disturb softdevice.
    __disable_irq();
    uint32_t current_counter = NRF_RTC1->COUNTER;

    // If an overflow occur, set the next tick in COUNTER + delta clock cycles
    if (is_in_wrapped_range(previous_tick_cc_value, new_compare_value, current_counter) == false) {
        new_compare_value = current_counter + delta;
    }
    NRF_RTC1->CC[1] = new_compare_value;

    // set the interrupt of CC channel 1 and reenable IRQs
    NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE1_Msk;
    __enable_irq();
}

/**
 * Initialize alternative hardware timer as RTX kernel timer
 * This function is directly called by RTX.
 * @note this function shouldn't be called directly.
 * @return  IRQ number of the alternative hardware timer
 */
int os_tick_init (void)
{
    // their is no need to start the LF clock, it is already started by SystemInit.
    NVIC_SetPriority(RTC1_IRQn, RTC1_IRQ_PRI);
    NVIC_ClearPendingIRQ(RTC1_IRQn);
    NVIC_EnableIRQ(RTC1_IRQn);

    NRF_RTC1->TASKS_START = 1;
    nrf_delay_us(MAX_RTC_TASKS_DELAY);

    NRF_RTC1->CC[1] = 0;
    clear_tick_interrupt();
    register_next_tick();

    os_tick_started = true;

    return RTC1_IRQn;
}

/**
 * Acknowledge the tick interrupt.
 * This function is called by the function OS_Tick_Handler of RTX.
 * @note this function shouldn't be called directly.
 */
void os_tick_irqack(void)
{
    clear_tick_interrupt();
    register_next_tick();
}

/**
 * Returns the overflow flag of the alternative hardware timer.
 * @note This function is exposed by RTX kernel.
 * @return 1 if the timer has overflowed and 0 otherwise.
 */
uint32_t os_tick_ovf(void) {
    uint32_t current_counter = NRF_RTC1->COUNTER;
    uint32_t next_tick_cc_value = NRF_RTC1->CC[1];

    return is_in_wrapped_range(previous_tick_cc_value, next_tick_cc_value, current_counter) ? 0 : 1;
}

/**
 * Return the value of the alternative hardware timer.
 * @note The documentation is not very clear about what is expected as a result,
 * is it an ascending counter, a descending one ?
 * None of this is specified.
 * The default systick is a descending counter and this function return values in
 * descending order, even if the internal counter used is an ascending one.
 * @return the value of the alternative hardware timer.
 */
uint32_t os_tick_val(void) {
    uint32_t current_counter = NRF_RTC1->COUNTER;
    uint32_t next_tick_cc_value = NRF_RTC1->CC[1];

    // do not use os_tick_ovf because its counter value can be different
    if(is_in_wrapped_range(previous_tick_cc_value, next_tick_cc_value, current_counter)) {
        if (next_tick_cc_value > previous_tick_cc_value) {
            return next_tick_cc_value - current_counter;
        } else if(current_counter <= next_tick_cc_value) {
            return next_tick_cc_value - current_counter;
        } else {
            return next_tick_cc_value + (MAX_RTC_COUNTER_VAL - current_counter);
        }
    } else {
        // use (os_trv + 1) has the base step, can be totally inacurate ...
        uint32_t clock_cycles_by_tick = os_trv + 1;

        // if current counter has wrap arround, add the limit to it.
        if (current_counter < next_tick_cc_value) {
            current_counter = current_counter + MAX_RTC_COUNTER_VAL;
        }

        return clock_cycles_by_tick - ((current_counter - next_tick_cc_value) % clock_cycles_by_tick);
    }
}
