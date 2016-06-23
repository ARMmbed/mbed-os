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
#if defined(TARGET_MCU_NRF51822)

#include "common_rtc.h"
#include "toolchain.h"
#include "nrf_delay.h"


#define MAX_RTC_COUNTER_VAL     ((1uL << RTC_COUNTER_BITS) - 1)

/**
 * The value previously set in the capture compare register of channel 1
 */
static uint32_t previous_tick_cc_value = 0;

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


#if defined (__CC_ARM)         /* ARMCC Compiler */

__asm void RTC1_IRQHandler(void)
{
    IMPORT  OS_Tick_Handler
    IMPORT  common_rtc_irq_handler

    /**
     * Chanel 1 of RTC1 is used by RTX as a systick.
     * If the compare event on channel 1 is set, then branch to OS_Tick_Handler.
     * Otherwise, just execute common_rtc_irq_handler.
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
             common_rtc_irq_handler();
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
    bl common_rtc_irq_handler
    pop {r3, pc}
    nop /* padding */
}

#elif defined (__GNUC__)        /* GNU Compiler */

__attribute__((naked)) void RTC1_IRQHandler(void)
{
    /**
     * Chanel 1 of RTC1 is used by RTX as a systick.
     * If the compare event on channel 1 is set, then branch to OS_Tick_Handler.
     * Otherwise, just execute common_rtc_irq_handler.
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
             common_rtc_irq_handler();
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
        "bl common_rtc_irq_handler\n"
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
 * Otherwise, just execute common_rtc_irq_handler.
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
         common_rtc_irq_handler();
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
    common_rtc_init();

    NRF_RTC1->CC[1] = 0;
    clear_tick_interrupt();
    register_next_tick();

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

    return 0;
}

#endif // defined(TARGET_MCU_NRF51822)
