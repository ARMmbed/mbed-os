/* mbed Microcontroller Library
*******************************************************************************
* Copyright (c) 2016, STMicroelectronics
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

#include <stdint.h>
#include "rtc_api.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Extend rtc_api.h
 */

/** Set the given function as handler of wakeup timer event.
 *
 * @param handler    The function to set as handler
 */
void rtc_set_irq_handler(uint32_t handler);

/** Read the subsecond register.
 *
 * @return The remaining time as microseconds (0-999999)
 */
uint32_t rtc_read_subseconds(void);

/** Program a wake up timer event in delta microseconds.
 *
 * @param delta    The time to wait
 */
void rtc_set_wake_up_timer(uint32_t delta);

/** Disable the wake up timer event.
 *
 * The wake up timer use auto reload, you have to deactivate it manually.
 */
void rtc_deactivate_wake_up_timer(void);

/** Synchronise the RTC shadow registers.
 *
 * Must be called after a deepsleep.
 */
void rtc_synchronize(void);


#ifdef __cplusplus
}
#endif

#endif
