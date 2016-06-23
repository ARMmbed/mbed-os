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

#ifndef COMMON_RTC_H
#define COMMON_RTC_H

#include "nrf_rtc.h"

#define RTC_COUNTER_BITS        24u

// Instance 0 is reserved for SoftDevice.
// Instance 1 is used as a common one for us_ticker, lp_ticker and (in case
// of NRF51) as an alternative tick source for RTOS.
#define COMMON_RTC_INSTANCE     NRF_RTC1

#define US_TICKER_CC_CHANNEL    0
#define OS_TICK_CC_CHANNEL      1
#define LP_TICKER_CC_CHANNEL    2

extern bool              m_common_rtc_enabled;
extern uint32_t volatile m_common_rtc_overflows;

void     common_rtc_init(void);
uint32_t common_rtc_32bit_ticks_get(void);

#endif // COMMON_RTC_H
