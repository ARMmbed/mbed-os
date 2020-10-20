/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, MultiTech Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of MultiTech nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#ifndef __MTQN_LOW_POWER_H__
#define __MTQN_LOW_POWER_H__

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

/* mtqn_disable_systick_int
 * disable the systick interrupt
 * call this before mtqn_enter_stop_mode so systick interrupt doesn't wake up the processor
 * only necessary if RTOS is used
 */
void mtqn_disable_systick_int();

/* mtqn_enable_systick_int
 * enable the systick interrupt
 * call this after mtqn_enter_stop_mode so RTOS can function again
 * only necessary if RTOS is used
 */
void mtqn_enable_systick_int();

/* mtqn_save_gpio_state
 * save current state of all GPIOs
 * call this before mtqn_enter_stop_mode
 * to achieve the lowest possible power consumption possible all GPIO pins must be configured for
 *      analog mode with no pull resistors enabled before entering STOP mode
 * the mtqn_enter_stop_mode function does this for USBTX/RX and all internal pins
 * after calling mtqn_save_gpio_state, the user application must do the same for WAKE, GPIO*,
 *      UART1_*, I2C_*, and SPI_* pins
 * the user application should make a call to mtqn_restore_gpio_state after waking from STOP mode
 *      in order to restore GPIO functionality
 */
void mtqn_save_gpio_state();

/* mtqn_restore_gpio_state
 * restore all GPIOs to the state they were in when mtqn_save_gpio_state was called
 * call this after exiting from STOP mode
 */
void mtqn_restore_gpio_state();

/* mtqn_enter_stop_mode
 * put the processor into STOP mode
 * RAM and peripheral state is retained
 * can be woken up by a number of interrupt sources including GPIOs and internal interrupts
 * program execution resumes after this function when the device wakes up
 */
void mtqn_enter_stop_mode();

/* mtqn_enter_standby_mode
 * put the processor into STANDBY mode
 * RAM and peripheral state is lost
 * can be woken up by the RTC alarm and rising edge on WAKE pin (WAKE pin must be configured first)
 * program execution starts from the beginning of the application when the device wakes up
 */
void mtqn_enter_standby_mode();

/* mtqn_enable_standby_wake_pin
 * configure the WAKE pin as a wakeup source from standby mode
 * after this call, a rising edge on the WAKE pin will wake the processor up from standby mode
 * this function should be called immediately before mtqn_enter_standby_mode
 */
void mtqn_enable_standby_wake_pin();

/* mtqn_disnable_standby_wake_pin
 * should be called after waking up from standby mode
 */
void mtqn_disable_standby_wake_pin();

#ifdef __cplusplus
}
#endif

#endif /* __DRAGONFLY_LOW_POWER_H__ */
