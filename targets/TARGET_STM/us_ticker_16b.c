/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "hal_tick.h"

// A 16-bit timer is used
#if TIM_MST_16BIT

TIM_HandleTypeDef TimMasterHandle;

volatile uint32_t SlaveCounter = 0;
volatile uint32_t oc_int_part = 0;

void us_ticker_init(void)
{
    /* NOTE: assuming that HAL tick has already been initialized! */
}

uint32_t us_ticker_read()
{
    uint16_t cntH_old, cntH, cntL;
    do {
        cntH_old = SlaveCounter;
        /* SlaveCounter needs to be checked before AND after we read the
         * current counter TIM_MST->CNT, in case it wraps around.
         * there are 2 possible cases of wrap around
         * 1) in case this function is interrupted by timer_irq_handler and
         *    the SlaveCounter is updated. In that case we will loop again.
         * 2) in case this function is called from interrupt context during
         * wrap-around condtion. That would prevent/delay the timer_irq_handler
         * from being called so we need to locally check the FLAG_UPDATE and
         * update the cntH accordingly. The SlaveCounter variable itself will
         * be updated in the interrupt handler just after ...
         */
        if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_UPDATE) == SET) {
            cntH_old += 1;
        }
        cntL = TIM_MST->CNT;
        cntH = SlaveCounter;
        if (__HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_UPDATE) == SET) {
            cntH += 1;
        }
    } while(cntH_old != cntH);
    // Glue the upper and lower part together to get a 32 bit timer
    return (uint32_t)(cntH << 16 | cntL);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    // NOTE: This function must be called with interrupts disabled to keep our
    //       timer interrupt setup atomic

    // Set new output compare value
    __HAL_TIM_SET_COMPARE(&TimMasterHandle, TIM_CHANNEL_1, timestamp & 0xFFFF);
    // Ensure the compare event starts clear
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
    // Enable IT
    __HAL_TIM_ENABLE_IT(&TimMasterHandle, TIM_IT_CC1);

    /* Set the number of timer wrap-around loops before the actual timestamp
     * is reached.  If the calculated delta time is more than halfway to the
     * next compare event, check to see if a compare event has already been
     * set, and if so, add one to the wrap-around count.  This is done to
     * ensure the correct wrap count is used in the corner cases where the
     * 16 bit counter passes the compare value during the process of
     * configuring this interrupt.
     *
     * Assumption: The time to execute this function is less than 32ms
     *             (otherwise incorrect behaviour could result)
     *
     * Consider the following corner cases:
     * 1) timestamp is 1 us in the future:
     *      oc_int_part = 0 initially
     *      oc_int_part left at 0 because ((delta - 1) & 0xFFFF) < 0x8000
     *      Compare event should happen in 1 us and us_ticker_irq_handler()
     *      called
     * 2) timestamp is 0x8000 us in the future:
     *      oc_int_part = 0 initially
     *      oc_int_part left at 0 because ((delta - 1) & 0xFFFF) < 0x8000
     *      There should be no possibility of the CC1 flag being set yet
     *      (see assumption above).  When the compare event does occur in
     *      32768 us, us_ticker_irq_handler() will be called
     * 3) timestamp is 0x8001 us in the future:
     *      oc_int_part = 0 initially
     *      ((delta - 1) & 0xFFFF) >= 0x8000 but there should be no
     *      possibility of the CC1 flag being set yet (see assumption above),
     *      so oc_int_part will be left at 0, and when the compare event
     *      does occur in 32769 us, us_ticker_irq_handler() will be called
     * 4) timestamp is 0x10000 us in the future:
     *      oc_int_part = 0 initially
     *      ((delta - 1) & 0xFFFF) >= 0x8000
     *      There are two subcases:
     *      a) The timer counter has not incremented past the compare
     *          value while setting up the interrupt.  In this case, the
     *          CC1 flag will not be set, so oc_int_part will be
     *          left at 0, and when the compare event occurs in 65536 us,
     *          us_ticker_irq_handler() will be called
     *      b) The timer counter has JUST incremented past the compare
     *          value.  In this case, the CC1 flag will be set, so
     *          oc_int_part will be incremented to 1, and the interrupt will
     *          occur immediately after this function returns, where
     *          oc_int_part will decrement to 0 without calling
     *          us_ticker_irq_handler().  Then about 65536 us later, the
     *          compare event will occur again, and us_ticker_irq_handler()
     *          will be called
     * 5) timestamp is 0x10001 us in the future:
     *      oc_int_part = 1 initially
     *      oc_int_part left at 1 because ((delta - 1) & 0xFFFF) < 0x8000
     *      CC1 flag will not be set (see assumption above).  In 1 us the
     *      compare event will cause an interrupt, where oc_int_part will be
     *      decremented to 0 without calling us_ticker_irq_handler().  Then
     *      about 65536 us later, the compare event will occur again, and
     *      us_ticker_irq_handler() will be called
     * 6) timestamp is 0x18000 us in the future:
     *      oc_int_part = 1 initially
     *      oc_int_part left at 1 because ((delta - 1) & 0xFFFF) < 0x8000
     *      There should be no possibility of the CC1 flag being set yet
     *      (see assumption above).  When the compare event does occur in
     *      32768 us, oc_int_part will be decremented to 0 without calling
     *      us_ticker_irq_handler().  Then about 65536 us later, the
     *      compare event will occur again, and us_ticker_irq_handler() will
     *      be called
     * 7) timestamp is 0x18001 us in the future:
     *      oc_int_part = 1 initially
     *      ((delta - 1) & 0xFFFF) >= 0x8000 but there should be no
     *      possibility of the CC1 flag being set yet (see assumption above),
     *      so oc_int_part will be left at 1, and when the compare event
     *      does occur in 32769 us, oc_int_part will be decremented to 0
     *      without calling us_ticker_irq_handler().  Then about 65536 us
     *      later, the compare event will occur again, and
     *      us_ticker_irq_handler() will be called
     *
     * delta - 1 is used because the timer compare event happens on the
     * counter incrementing to match the compare value, and it won't occur
     * immediately when the compare value is set to the current counter
     * value.
     */
    uint32_t current_time = us_ticker_read();
    uint32_t delta = timestamp - current_time;
    /* Note: The case of delta <= 0 is handled in MBED upper layer */
    oc_int_part = (delta - 1) >> 16;
    if ( ((delta - 1) & 0xFFFF) >= 0x8000 &&
         __HAL_TIM_GET_FLAG(&TimMasterHandle, TIM_FLAG_CC1) == SET ) {
        ++oc_int_part;
        /* NOTE: Instead of incrementing oc_int_part here, we could clear
         *       the CC1 flag, but then you'd have to wait to ensure the
         *       interrupt is knocked down before returning and reenabling
         *       interrupts.  Since this is a rare case, it's not worth it
         *       to try and optimize it, and it keeps the code simpler and
         *       safer to just do this increment instead.
         */
    }

}

void us_ticker_fire_interrupt(void)
{
    /* When firing the event, the number of 16 bits counter wrap-ups (oc_int)
     * must be re-initialized */
    oc_int_part = 0;
    HAL_TIM_GenerateEvent(&TimMasterHandle, TIM_EVENTSOURCE_CC1);
}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_disable_interrupt(void)
{
    __HAL_TIM_DISABLE_IT(&TimMasterHandle, TIM_IT_CC1);
}

/* NOTE: must be called with interrupts disabled! */
void us_ticker_clear_interrupt(void)
{
    __HAL_TIM_CLEAR_FLAG(&TimMasterHandle, TIM_FLAG_CC1);
}

#endif // TIM_MST_16BIT
