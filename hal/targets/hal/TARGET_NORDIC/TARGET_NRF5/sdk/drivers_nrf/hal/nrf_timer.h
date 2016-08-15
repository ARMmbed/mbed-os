/* 
 * Copyright (c) 2014 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


/**
 * @defgroup nrf_timer_hal Timer HAL
 * @{
 * @ingroup nrf_timer
 *
 * @brief Hardware access layer for accessing the timer peripheral.
 */

#ifndef NRF_TIMER_H__
#define NRF_TIMER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "nrf.h"
#include "nrf_assert.h"


/**
 * @brief Macro for validating the correctness of the BIT_WIDTH setting.
 */
#ifdef NRF51
    /**
     * In the nRF51 Series, timer instance 0 supports all available bit widths.
     * The other two instances support only 8 and 16 bits.
     */
    #define NRF_TIMER_IS_BIT_WIDTH_VALID(p_timer, bit_width) \
        ((p_timer == NRF_TIMER0) || (bit_width <= NRF_TIMER_BIT_WIDTH_16))
#else
    /**
     * In the nRF52 Series, all timer instances support all available bit widths.
     */
    #define NRF_TIMER_IS_BIT_WIDTH_VALID(p_timer, bit_width) true
#endif

/**
 * @brief Macro for getting the number of capture/compare channels available
 *        in a given timer instance.
 */
#ifdef NRF51
    #define NRF_TIMER_CC_CHANNEL_COUNT(id)  4
#else
    #define NRF_TIMER_CC_CHANNEL_COUNT(id)  ((id) <= 2 ? 4 : 6)
#endif


/**
 * @brief Timer tasks.
 */
typedef enum
{
    /*lint -save -e30 -esym(628,__INTADDR__)*/
    NRF_TIMER_TASK_START    = offsetof(NRF_TIMER_Type, TASKS_START),      ///< Task for starting the timer.
    NRF_TIMER_TASK_STOP     = offsetof(NRF_TIMER_Type, TASKS_STOP),       ///< Task for stopping the timer.
    NRF_TIMER_TASK_COUNT    = offsetof(NRF_TIMER_Type, TASKS_COUNT),      ///< Task for incrementing the timer (in counter mode).
    NRF_TIMER_TASK_CLEAR    = offsetof(NRF_TIMER_Type, TASKS_CLEAR),      ///< Task for resetting the timer value.
    NRF_TIMER_TASK_SHUTDOWN = offsetof(NRF_TIMER_Type, TASKS_SHUTDOWN),   ///< Task for powering off the timer.
    NRF_TIMER_TASK_CAPTURE0 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[0]), ///< Task for capturing the timer value on channel 0.
    NRF_TIMER_TASK_CAPTURE1 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[1]), ///< Task for capturing the timer value on channel 1.
    NRF_TIMER_TASK_CAPTURE2 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[2]), ///< Task for capturing the timer value on channel 2.
    NRF_TIMER_TASK_CAPTURE3 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[3]), ///< Task for capturing the timer value on channel 3.
#ifdef NRF52
    NRF_TIMER_TASK_CAPTURE4 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[4]), ///< Task for capturing the timer value on channel 4.
    NRF_TIMER_TASK_CAPTURE5 = offsetof(NRF_TIMER_Type, TASKS_CAPTURE[5]), ///< Task for capturing the timer value on channel 5.
#endif
    /*lint -restore*/
} nrf_timer_task_t;

/**
 * @brief Timer events.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_TIMER_EVENT_COMPARE0 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[0]), ///< Event from compare channel 0.
    NRF_TIMER_EVENT_COMPARE1 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[1]), ///< Event from compare channel 1.
    NRF_TIMER_EVENT_COMPARE2 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[2]), ///< Event from compare channel 2.
    NRF_TIMER_EVENT_COMPARE3 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[3]), ///< Event from compare channel 3.
#ifdef NRF52
    NRF_TIMER_EVENT_COMPARE4 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[4]), ///< Event from compare channel 4.
    NRF_TIMER_EVENT_COMPARE5 = offsetof(NRF_TIMER_Type, EVENTS_COMPARE[5]), ///< Event from compare channel 5.
#endif
    /*lint -restore*/
} nrf_timer_event_t;

/**
 * @brief Types of timer shortcuts.
 */
typedef enum
{
    NRF_TIMER_SHORT_COMPARE0_STOP_MASK = TIMER_SHORTS_COMPARE0_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 0.
    NRF_TIMER_SHORT_COMPARE1_STOP_MASK = TIMER_SHORTS_COMPARE1_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 1.
    NRF_TIMER_SHORT_COMPARE2_STOP_MASK = TIMER_SHORTS_COMPARE2_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 2.
    NRF_TIMER_SHORT_COMPARE3_STOP_MASK = TIMER_SHORTS_COMPARE3_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 3.
#ifdef NRF52
    NRF_TIMER_SHORT_COMPARE4_STOP_MASK = TIMER_SHORTS_COMPARE4_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 4.
    NRF_TIMER_SHORT_COMPARE5_STOP_MASK = TIMER_SHORTS_COMPARE5_STOP_Msk,   ///< Shortcut for stopping the timer based on compare 5.
#endif
    NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK = TIMER_SHORTS_COMPARE0_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 0.
    NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK = TIMER_SHORTS_COMPARE1_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 1.
    NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK = TIMER_SHORTS_COMPARE2_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 2.
    NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK = TIMER_SHORTS_COMPARE3_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 3.
#ifdef NRF52
    NRF_TIMER_SHORT_COMPARE4_CLEAR_MASK = TIMER_SHORTS_COMPARE4_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 4.
    NRF_TIMER_SHORT_COMPARE5_CLEAR_MASK = TIMER_SHORTS_COMPARE5_CLEAR_Msk, ///< Shortcut for clearing the timer based on compare 5.
#endif
} nrf_timer_short_mask_t;

/**
 * @brief Timer modes.
 */
typedef enum
{
    NRF_TIMER_MODE_TIMER             = TIMER_MODE_MODE_Timer,           ///< Timer mode: timer.
    NRF_TIMER_MODE_COUNTER           = TIMER_MODE_MODE_Counter,         ///< Timer mode: counter.
#ifdef NRF52
    NRF_TIMER_MODE_LOW_POWER_COUNTER = TIMER_MODE_MODE_LowPowerCounter, ///< Timer mode: low-power counter.
#endif
} nrf_timer_mode_t;

/**
 * @brief Timer bit width.
 */
typedef enum
{
    NRF_TIMER_BIT_WIDTH_8  = TIMER_BITMODE_BITMODE_08Bit, ///< Timer bit width 8 bit.
    NRF_TIMER_BIT_WIDTH_16 = TIMER_BITMODE_BITMODE_16Bit, ///< Timer bit width 16 bit.
    NRF_TIMER_BIT_WIDTH_24 = TIMER_BITMODE_BITMODE_24Bit, ///< Timer bit width 24 bit.
    NRF_TIMER_BIT_WIDTH_32 = TIMER_BITMODE_BITMODE_32Bit  ///< Timer bit width 32 bit.
} nrf_timer_bit_width_t;

/**
 * @brief Timer prescalers.
 */
typedef enum
{
    NRF_TIMER_FREQ_16MHz = 0, ///< Timer frequency 16 MHz.
    NRF_TIMER_FREQ_8MHz,      ///< Timer frequency 8 MHz.
    NRF_TIMER_FREQ_4MHz,      ///< Timer frequency 4 MHz.
    NRF_TIMER_FREQ_2MHz,      ///< Timer frequency 2 MHz.
    NRF_TIMER_FREQ_1MHz,      ///< Timer frequency 1 MHz.
    NRF_TIMER_FREQ_500kHz,    ///< Timer frequency 500 kHz.
    NRF_TIMER_FREQ_250kHz,    ///< Timer frequency 250 kHz.
    NRF_TIMER_FREQ_125kHz,    ///< Timer frequency 125 kHz.
    NRF_TIMER_FREQ_62500Hz,   ///< Timer frequency 62500 Hz.
    NRF_TIMER_FREQ_31250Hz    ///< Timer frequency 31250 Hz.
} nrf_timer_frequency_t;

/**
 * @brief Timer capture/compare channels.
 */
typedef enum
{
    NRF_TIMER_CC_CHANNEL0 = 0, ///< Timer capture/compare channel 0.
    NRF_TIMER_CC_CHANNEL1,     ///< Timer capture/compare channel 1.
    NRF_TIMER_CC_CHANNEL2,     ///< Timer capture/compare channel 2.
    NRF_TIMER_CC_CHANNEL3,     ///< Timer capture/compare channel 3.
#ifdef NRF52
    NRF_TIMER_CC_CHANNEL4,     ///< Timer capture/compare channel 4.
    NRF_TIMER_CC_CHANNEL5,     ///< Timer capture/compare channel 5.
#endif
} nrf_timer_cc_channel_t;

/**
 * @brief Timer interrupts.
 */
typedef enum
{
    NRF_TIMER_INT_COMPARE0_MASK = TIMER_INTENSET_COMPARE0_Msk, ///< Timer interrupt from compare event on channel 0.
    NRF_TIMER_INT_COMPARE1_MASK = TIMER_INTENSET_COMPARE1_Msk, ///< Timer interrupt from compare event on channel 1.
    NRF_TIMER_INT_COMPARE2_MASK = TIMER_INTENSET_COMPARE2_Msk, ///< Timer interrupt from compare event on channel 2.
    NRF_TIMER_INT_COMPARE3_MASK = TIMER_INTENSET_COMPARE3_Msk, ///< Timer interrupt from compare event on channel 3.
#ifdef NRF52
    NRF_TIMER_INT_COMPARE4_MASK = TIMER_INTENSET_COMPARE4_Msk, ///< Timer interrupt from compare event on channel 4.
    NRF_TIMER_INT_COMPARE5_MASK = TIMER_INTENSET_COMPARE5_Msk, ///< Timer interrupt from compare event on channel 5.
#endif
} nrf_timer_int_mask_t;


/**
 * @brief Function for activating a specific timer task.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] task    Task to activate.
 */
__STATIC_INLINE void nrf_timer_task_trigger(NRF_TIMER_Type * p_timer,
                                            nrf_timer_task_t task);

/**
 * @brief Function for getting the address of a specific timer task register.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] task    Requested task.
 *
 * @return Address of the specified task register.
 */
__STATIC_INLINE uint32_t * nrf_timer_task_address_get(NRF_TIMER_Type * p_timer,
                                                      nrf_timer_task_t task);

/**
 * @brief Function for clearing a specific timer event.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] event   Event to clear.
 */
__STATIC_INLINE void nrf_timer_event_clear(NRF_TIMER_Type * p_timer,
                                           nrf_timer_event_t event);

/**
 * @brief Function for checking the state of a specific timer event.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] event   Event to check.
 *
 * @retval true  If the event is set.
 * @retval false If the event is not set.
 */
__STATIC_INLINE bool nrf_timer_event_check(NRF_TIMER_Type * p_timer,
                                           nrf_timer_event_t event);

/**
 * @brief Function for getting the address of a specific timer event register.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] event   Requested event.
 *
 * @return Address of the specified event register.
 */
__STATIC_INLINE uint32_t * nrf_timer_event_address_get(NRF_TIMER_Type * p_timer,
                                                       nrf_timer_event_t event);

/**
 * @brief Function for enabling specified shortcuts.
 *
 * @param[in] p_timer           Timer instance.
 * @param[in] timer_shorts_mask Shortcuts to enable.
 */
__STATIC_INLINE void nrf_timer_shorts_enable(NRF_TIMER_Type * p_timer,
                                             uint32_t timer_shorts_mask);

/**
 * @brief Function for disabling specified shortcuts.
 *
 * @param[in] p_timer           Timer instance.
 * @param[in] timer_shorts_mask Shortcuts to disable.
 */
__STATIC_INLINE void nrf_timer_shorts_disable(NRF_TIMER_Type * p_timer,
                                              uint32_t timer_shorts_mask);

/**
 * @brief Function for enabling specified interrupts.
 *
 * @param[in] p_timer        Timer instance.
 * @param[in] timer_int_mask Interrupts to enable.
 */
__STATIC_INLINE void nrf_timer_int_enable(NRF_TIMER_Type * p_timer,
                                          uint32_t timer_int_mask);

/**
 * @brief Function for disabling specified interrupts.
 *
 * @param[in] p_timer        Timer instance.
 * @param[in] timer_int_mask Interrupts to disable.
 */
__STATIC_INLINE void nrf_timer_int_disable(NRF_TIMER_Type * p_timer,
                                           uint32_t timer_int_mask);

/**
 * @brief Function for retrieving the state of a given interrupt.
 *
 * @param[in] p_timer   Timer instance.
 * @param[in] timer_int Interrupt to check.
 *
 * @retval true  If the interrupt is enabled.
 * @retval false If the interrupt is not enabled.
 */
__STATIC_INLINE bool nrf_timer_int_enable_check(NRF_TIMER_Type * p_timer,
                                                uint32_t timer_int);

/**
 * @brief Function for setting the timer mode.
 *
 * @param[in] p_timer Timer instance.
 * @param[in] mode    Timer mode.
 */
__STATIC_INLINE void nrf_timer_mode_set(NRF_TIMER_Type * p_timer,
                                        nrf_timer_mode_t mode);

/**
 * @brief Function for retrieving the timer mode.
 *
 * @param[in] p_timer Timer instance.
 *
 * @return Timer mode.
 */
__STATIC_INLINE nrf_timer_mode_t nrf_timer_mode_get(NRF_TIMER_Type * p_timer);

/**
 * @brief Function for setting the timer bit width.
 *
 * @param[in] p_timer   Timer instance.
 * @param[in] bit_width Timer bit width.
 */
__STATIC_INLINE void nrf_timer_bit_width_set(NRF_TIMER_Type * p_timer,
                                             nrf_timer_bit_width_t bit_width);

/**
 * @brief Function for retrieving the timer bit width.
 *
 * @param[in] p_timer Timer instance.
 *
 * @return Timer bit width.
 */
__STATIC_INLINE nrf_timer_bit_width_t nrf_timer_bit_width_get(NRF_TIMER_Type * p_timer);

/**
 * @brief Function for setting the timer frequency.
 *
 * @param[in] p_timer   Timer instance.
 * @param[in] frequency Timer frequency.
 */
__STATIC_INLINE void nrf_timer_frequency_set(NRF_TIMER_Type * p_timer,
                                             nrf_timer_frequency_t frequency);

/**
 * @brief Function for retrieving the timer frequency.
 *
 * @param[in] p_timer Timer instance.
 *
 * @return Timer frequency.
 */
__STATIC_INLINE nrf_timer_frequency_t nrf_timer_frequency_get(NRF_TIMER_Type * p_timer);

/**
 * @brief Function for writing the capture/compare register for a specified channel.
 *
 * @param[in] p_timer    Timer instance.
 * @param[in] cc_channel Requested capture/compare channel.
 * @param[in] cc_value   Value to write to the capture/compare register.
 */
__STATIC_INLINE void nrf_timer_cc_write(NRF_TIMER_Type * p_timer,
                                        nrf_timer_cc_channel_t cc_channel,
                                        uint32_t               cc_value);

/**
 * @brief Function for retrieving the capture/compare value for a specified channel.
 *
 * @param[in] p_timer    Timer instance.
 * @param[in] cc_channel Requested capture/compare channel.
 *
 * @return Value from the requested capture/compare register.
 */
__STATIC_INLINE uint32_t nrf_timer_cc_read(NRF_TIMER_Type * p_timer,
                                           nrf_timer_cc_channel_t cc_channel);

/**
 * @brief Function for getting a specific timer capture task.
 *
 * @param[in] channel Capture channel.
 *
 * @return Capture task.
 */
__STATIC_INLINE nrf_timer_task_t nrf_timer_capture_task_get(uint32_t channel);

/**
 * @brief Function for getting a specific timer compare event.
 *
 * @param[in] channel Compare channel.
 *
 * @return Compare event.
 */
__STATIC_INLINE nrf_timer_event_t nrf_timer_compare_event_get(uint32_t channel);

/**
 * @brief Function for getting a specific timer compare interrupt.
 *
 * @param[in] channel Compare channel.
 *
 * @return Compare interrupt.
 */
__STATIC_INLINE nrf_timer_int_mask_t nrf_timer_compare_int_get(uint32_t channel);

/**
 * @brief Function for calculating the number of timer ticks for a given time
 *        (in microseconds) and timer frequency.
 *
 * @param[in] time_us   Time in microseconds.
 * @param[in] frequency Timer frequency.
 *
 * @return Number of timer ticks.
 */
__STATIC_INLINE uint32_t nrf_timer_us_to_ticks(uint32_t time_us,
                                               nrf_timer_frequency_t frequency);

/**
 * @brief Function for calculating the number of timer ticks for a given time
 *        (in milliseconds) and timer frequency.
 *
 * @param[in] time_ms   Time in milliseconds.
 * @param[in] frequency Timer frequency.
 *
 * @return Number of timer ticks.
 */
__STATIC_INLINE uint32_t nrf_timer_ms_to_ticks(uint32_t time_ms,
                                               nrf_timer_frequency_t frequency);


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE void nrf_timer_task_trigger(NRF_TIMER_Type * p_timer,
                                            nrf_timer_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_timer + (uint32_t)task)) = 0x1UL;
}

__STATIC_INLINE uint32_t * nrf_timer_task_address_get(NRF_TIMER_Type * p_timer,
                                                      nrf_timer_task_t task)
{
    return (uint32_t *)((uint8_t *)p_timer + (uint32_t)task);
}

__STATIC_INLINE void nrf_timer_event_clear(NRF_TIMER_Type * p_timer,
                                           nrf_timer_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_timer + (uint32_t)event)) = 0x0UL;
}

__STATIC_INLINE bool nrf_timer_event_check(NRF_TIMER_Type * p_timer,
                                           nrf_timer_event_t event)
{
    return (bool)*(volatile uint32_t *)((uint8_t *)p_timer + (uint32_t)event);
}

__STATIC_INLINE uint32_t * nrf_timer_event_address_get(NRF_TIMER_Type * p_timer,
                                                       nrf_timer_event_t event)
{
    return (uint32_t *)((uint8_t *)p_timer + (uint32_t)event);
}

__STATIC_INLINE void nrf_timer_shorts_enable(NRF_TIMER_Type * p_timer,
                                             uint32_t timer_shorts_mask)
{
    p_timer->SHORTS |= timer_shorts_mask;
}

__STATIC_INLINE void nrf_timer_shorts_disable(NRF_TIMER_Type * p_timer,
                                              uint32_t timer_shorts_mask)
{
    p_timer->SHORTS &= ~(timer_shorts_mask);
}

__STATIC_INLINE void nrf_timer_int_enable(NRF_TIMER_Type * p_timer,
                                          uint32_t timer_int_mask)
{
    p_timer->INTENSET = timer_int_mask;
}

__STATIC_INLINE void nrf_timer_int_disable(NRF_TIMER_Type * p_timer,
                                           uint32_t timer_int_mask)
{
    p_timer->INTENCLR = timer_int_mask;
}

__STATIC_INLINE bool nrf_timer_int_enable_check(NRF_TIMER_Type * p_timer,
                                                uint32_t timer_int)
{
    return (bool)(p_timer->INTENSET & timer_int);
}

__STATIC_INLINE void nrf_timer_mode_set(NRF_TIMER_Type * p_timer,
                                        nrf_timer_mode_t mode)
{
    p_timer->MODE = (p_timer->MODE & ~TIMER_MODE_MODE_Msk) |
                    ((mode << TIMER_MODE_MODE_Pos) & TIMER_MODE_MODE_Msk);
}

__STATIC_INLINE nrf_timer_mode_t nrf_timer_mode_get(NRF_TIMER_Type * p_timer)
{
    return (nrf_timer_mode_t)(p_timer->MODE);
}

__STATIC_INLINE void nrf_timer_bit_width_set(NRF_TIMER_Type * p_timer,
                                             nrf_timer_bit_width_t bit_width)
{
    p_timer->BITMODE = (p_timer->BITMODE & ~TIMER_BITMODE_BITMODE_Msk) |
                       ((bit_width << TIMER_BITMODE_BITMODE_Pos) &
                            TIMER_BITMODE_BITMODE_Msk);
}

__STATIC_INLINE nrf_timer_bit_width_t nrf_timer_bit_width_get(NRF_TIMER_Type * p_timer)
{
    return (nrf_timer_bit_width_t)(p_timer->BITMODE);
}

__STATIC_INLINE void nrf_timer_frequency_set(NRF_TIMER_Type * p_timer,
                                             nrf_timer_frequency_t frequency)
{
    p_timer->PRESCALER = (p_timer->PRESCALER & ~TIMER_PRESCALER_PRESCALER_Msk) |
                         ((frequency << TIMER_PRESCALER_PRESCALER_Pos) &
                              TIMER_PRESCALER_PRESCALER_Msk);
}

__STATIC_INLINE nrf_timer_frequency_t nrf_timer_frequency_get(NRF_TIMER_Type * p_timer)
{
    return (nrf_timer_frequency_t)(p_timer->PRESCALER);
}

__STATIC_INLINE void nrf_timer_cc_write(NRF_TIMER_Type * p_timer,
                                        nrf_timer_cc_channel_t cc_channel,
                                        uint32_t               cc_value)
{
    p_timer->CC[cc_channel] = cc_value;
}

__STATIC_INLINE uint32_t nrf_timer_cc_read(NRF_TIMER_Type * p_timer,
                                           nrf_timer_cc_channel_t cc_channel)
{
    return (uint32_t)p_timer->CC[cc_channel];
}

__STATIC_INLINE nrf_timer_task_t nrf_timer_capture_task_get(uint32_t channel)
{
    return (nrf_timer_task_t)
        ((uint32_t)NRF_TIMER_TASK_CAPTURE0 + (channel * sizeof(uint32_t)));
}

__STATIC_INLINE nrf_timer_event_t nrf_timer_compare_event_get(uint32_t channel)
{
    return (nrf_timer_event_t)
        ((uint32_t)NRF_TIMER_EVENT_COMPARE0 + (channel * sizeof(uint32_t)));
}

__STATIC_INLINE nrf_timer_int_mask_t nrf_timer_compare_int_get(uint32_t channel)
{
    return (nrf_timer_int_mask_t)
        ((uint32_t)NRF_TIMER_INT_COMPARE0_MASK << channel);
}

__STATIC_INLINE uint32_t nrf_timer_us_to_ticks(uint32_t time_us,
                                               nrf_timer_frequency_t frequency)
{
    // The "frequency" parameter here is actually the prescaler value, and the
    // timer runs at the following frequency: f = 16 MHz / 2^prescaler.
    uint32_t prescaler = (uint32_t)frequency;
    ASSERT(time_us <= (UINT32_MAX / 16UL));
    return ((time_us * 16UL) >> prescaler);
}

__STATIC_INLINE uint32_t nrf_timer_ms_to_ticks(uint32_t time_ms,
                                               nrf_timer_frequency_t frequency)
{
    // The "frequency" parameter here is actually the prescaler value, and the
    // timer runs at the following frequency: f = 16000 kHz / 2^prescaler.
    uint32_t prescaler = (uint32_t)frequency;
    ASSERT(time_ms <= (UINT32_MAX / 16000UL));
    return ((time_ms * 16000UL) >> prescaler);
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

#endif // NRF_TIMER_H__

/** @} */
