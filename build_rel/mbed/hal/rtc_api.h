
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_RTC_API_H
#define MBED_RTC_API_H

#include "device.h"

#if DEVICE_RTC

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_rtc RTC hal functions
 * @{
 */

/** Initialize the RTC peripheral
 *
 */
void rtc_init(void);

/** Deinitialize RTC
 *
 * TODO: The function is not used by rtc api in mbed-drivers.
 */
void rtc_free(void);

/** Get the RTC enable status
 *
 * @retval 0 disabled
 * @retval 1 enabled
 */
int rtc_isenabled(void);

/** Get the current time from the RTC peripheral
 *
 * @return The current time
 */
time_t rtc_read(void);

/** Set the current time to the RTC peripheral
 *
 * @param t The current time to be set
 */
void rtc_write(time_t t);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
