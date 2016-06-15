/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#ifndef RTC_COMMON_H
#define RTC_COMMON_H

#include "nrf_drv_rtc.h"

#define RTC_COUNTER_BITS        24

#define LP_TICKER_CC_CHANNEL    0
#define US_TICKER_CC_CHANNEL    1

extern nrf_drv_rtc_t const m_rtc_common;
extern bool                m_rtc_common_enabled;
extern uint32_t volatile   m_rtc_common_overflows;

void     rtc_common_init(void);
uint32_t rtc_common_32bit_ticks_get(void);

#endif // RTC_COMMON_H
