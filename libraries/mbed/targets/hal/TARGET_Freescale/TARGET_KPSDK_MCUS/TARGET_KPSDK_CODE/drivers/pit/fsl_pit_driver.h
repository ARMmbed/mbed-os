/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
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
 */
#ifndef __FSL_PIT_DRIVER_H__
#define __FSL_PIT_DRIVER_H__
 
#include <stdint.h>
#include <stdbool.h>
#include "fsl_pit_hal.h"
 
/*!
 * @addtogroup pit_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief PIT timer configuration structure
 *
 * Define structure PitConfig and use pit_init_channel() to make necessary
 * initializations. You may also use remaining functions for PIT configuration.
 *
 * @note the timer chain feature is not valid in all devices, please check 
 * fsl_pit_features.h for accurate setting. If it's not valid, the value set here
 * will be bypassed inside function pit_init_channel(). 
 */
typedef struct PitUserConfig {
    bool isInterruptEnabled;  /*!< Timer interrupt 0-disable/1-enable*/
    bool isTimerChained;      /*!< Chained with previous timer, 0-not/1-chained*/
    uint32_t periodUs;        /*!< Timer period in unit of microseconds*/
} pit_user_config_t;

/*! @brief PIT ISR callback function typedef */
typedef void (*pit_isr_callback_t)(void);

/*******************************************************************************
 * API
 ******************************************************************************/
 
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialize and Shutdown
 * @{
 */

/*!
 * @brief Initialize PIT module.
 * 
 * This function must be called before calling all the other PIT driver functions.
 * This function un-gates the PIT clock and enables the PIT module. The isRunInDebug
 * passed into function will affect all timer channels. 
 *
 * @param isRunInDebug Timers run or stop in debug mode.
 *        - true:  Timers continue to run in debug mode.
 *        - false: Timers stop in debug mode.
 */
void pit_init_module(bool isRunInDebug);

/*!
 * @brief Initialize PIT channel.
 * 
 * This function initialize PIT timers by channel. Pass in timer number and its
 * config structure. Timers do not start counting by default after calling this
 * function. Function pit_timer_start must be called to start timer counting. 
 * Call pit_set_timer_period_us to re-set the period.
 *
 * Here is an example demonstrating how to define a PIT channel config structure:
   @code
   pit_user_config_t pitTestInit = {
        .isInterruptEnabled = true,
        // Only takes effect when chain feature is available.
        // Otherwise, pass in arbitrary value(true/false).
        .isTimerChained = false, 
        // In unit of microseconds.
        .periodUs = 1000,
   };
   @endcode
 *
 * @param timer Timer channel number.
 * @param config PIT channel configuration structure.
 */
void pit_init_channel(uint32_t timer, const pit_user_config_t * config);

/*!
 * @brief Disable PIT module and gate control.
 *
 * This function disables all PIT interrupts and PIT clock. It then gates the
 * PIT clock control. pit_init_module must be called if you want to use PIT again.
 */
void pit_shutdown(void);

/* @} */

/*!
 * @name Timer Start and Stop 
 * @{
 */

/*!
 * @brief Start timer counting.
 *
 * After calling this function, timers load period value, count down to 0 and
 * then load the respective start value again. Each time a timer reaches 0,
 * it generates a trigger pulse and sets the timeout interrupt flag.
 *
 * @param timer Timer channel number.
 */
void pit_timer_start(uint32_t timer);

/*!
 * @brief Stop timer counting.
 *
 * This function stops every timer counting. Timers reload their periods
 * respectively after the next time they call pit_timer_start.
 *
 * @param timer Timer channel number.
 */
void pit_timer_stop(uint32_t timer);

/* @} */

/*!
 * @name Timer Period
 * @{
 */

/*!
 * @brief Set timer period in microsecond units.
 *
 * The period range depends on the frequency of PIT source clock. If the required period
 * is out of range, use the lifetime timer, if applicable. 
 * 
 * @param timer Timer channel number.
 * @param us Timer period in microseconds.
 */
void pit_set_timer_period_us(uint32_t timer, uint32_t us);

/*!
 * @brief Read current timer value in microsecond units.
 * 
 * This function returns an absolute time stamp in microsecond units.
 * One common use of this function is to measure the running time of a part of
 * code. Call this function at both the beginning and end of code; the time
 * difference between these two time stamps is the running time (Make sure the 
 * running time will not exceed the timer period). The time stamp returned is
 * up-counting.
 *
 * @param timer Timer channel number.
 * @return Current timer value in microseconds.
 */
uint32_t pit_read_timer_us(uint32_t timer);

#if FSL_FEATURE_PIT_HAS_LIFETIME_TIMER
/*!
 * @brief Set lifetime timer period.
 * 
 * Timer 1 must be chained with timer 0 before using the lifetime timer. The period
 * range is restricted by "period * pitSourceClock < max of an uint64_t integer",
 * or it may cause an overflow and be unable to set the correct period.
 *
 * @param period Lifetime timer period in microseconds.
 */
void pit_set_lifetime_timer_period_us(uint64_t us);

/*!
 * @brief Read current lifetime value in microseconds.
 *
 * This feature returns an absolute time stamp in microsecond units. The time stamp 
 * value will not exceed the timer period. The timer is up-counting.
 *
 * @return Current lifetime timer value in microseconds.
 */
uint64_t pit_read_lifetime_timer_us(void);
#endif /*FSL_FEATURE_PIT_HAS_LIFETIME_TIMER*/

/* @} */

/*!
 * @name ISR Callback Function 
 * @{
 */

/*!
 * @brief Register pit isr callback function. 
 *
 * System default ISR interfaces are already defined in fsl_pit_irq.c. Users 
 * can either edit these ISRs or use this function to register a callback
 * function. The default ISR runs the callback function if there is one
 * installed.
 *
 * @param timer    Timer channel number.
 * @param function Pointer to pit isr callback function.
 */
void pit_register_isr_callback_function(uint32_t timer, pit_isr_callback_t function);

/* @} */

#if defined(__cplusplus)
}
#endif
 
/*! @}*/
 
#endif /* __FSL_PIT_DRIVER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

