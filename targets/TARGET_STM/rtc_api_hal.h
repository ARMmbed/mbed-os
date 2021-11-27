/* mbed Microcontroller Library
*******************************************************************************
* Copyright (c) 2018, STMicroelectronics
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
* 3. Neither the name of STMicroelectronics nor the names of its contributors
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

#ifndef MBED_RTC_API_HAL_H
#define MBED_RTC_API_HAL_H

#include "rtc_api.h"
#include "lp_ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif

// Possible choices of the RTC_CLOCK_SOURCE configuration set in json file
#define USE_RTC_CLK_LSE_OR_LSI 1
#define USE_RTC_CLK_LSI 2
#define USE_RTC_CLK_HSE 3

#if !((MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) || (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSI) || (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE))
#error "RTC clock configuration is invalid!"
#endif

#if (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE) && !(TARGET_STM32F2 || TARGET_STM32F4 || TARGET_STM32F7 || TARGET_STM32F1)
#error "RTC from HSE not supported for this target"
#endif

#if (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE)
#define RTC_CLOCK 1000000U
#define RTC_HSE_DIV (HSE_VALUE / RTC_CLOCK)
#if RTC_HSE_DIV > 31
#error "HSE value too high for RTC"
#endif
#elif (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_LSE_OR_LSI) && MBED_CONF_TARGET_LSE_AVAILABLE
#define RTC_CLOCK LSE_VALUE
#else
#define RTC_CLOCK LSI_VALUE
#endif

#if DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM

#if (MBED_CONF_TARGET_RTC_CLOCK_SOURCE == USE_RTC_CLK_HSE)
#error "LPTICKER is not available with HSE as RTC clock source and should be removed from the target configuration."
#endif

/* PREDIV_A : 7-bit asynchronous prescaler */
/* PREDIV_A is set to set LPTICKER frequency to RTC_CLOCK/4 */
#define PREDIV_A_VALUE 3

/** Read RTC counter with sub second precision
 *
 * @return LP ticker counter
 */
uint32_t rtc_read_lp(void);

/** Program a wake up timer event
 *
 * @param timestamp: counter to set
 */
void rtc_set_wake_up_timer(timestamp_t timestamp);

/** Call RTC Wake Up IT
 */
void rtc_fire_interrupt(void);

/** Disable the wake up timer event.
 *
 * The wake up timer use auto reload, you have to deactivate it manually.
 */
void rtc_deactivate_wake_up_timer(void);

#else /* DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM */

/* PREDIV_A : 7-bit asynchronous prescaler */
/* PREDIV_A is set to the maximum value to improve the consumption */
#define PREDIV_A_VALUE 127

#endif /* DEVICE_LPTICKER && !MBED_CONF_TARGET_LPTICKER_LPTIM */

/* PREDIV_S : 15-bit synchronous prescaler */
/* PREDIV_S is set in order to get a 1 Hz clock */
#define PREDIV_S_VALUE (RTC_CLOCK / (PREDIV_A_VALUE + 1) - 1)

/** Synchronise the RTC shadow registers.
 *
 * Must be called after a deepsleep.
 */
void rtc_synchronize(void);

#ifdef __cplusplus
}
#endif

#endif
