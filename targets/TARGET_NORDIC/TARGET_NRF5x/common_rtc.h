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
// ["us_ticker.c" uses hard coded addresses of the 'NRF_RTC1->EVENT_COMPARE[1]'
//  register in inline assembly implementations of COMMON_RTC_IRQ_HANDLER,
//  please remember to update those in case of doing changes here]
#define COMMON_RTC_INSTANCE     NRF_RTC1
#define COMMON_RTC_IRQ_HANDLER  RTC1_IRQHandler
#define US_TICKER_CC_CHANNEL    0
#define OS_TICK_CC_CHANNEL      1
#define LP_TICKER_CC_CHANNEL    2

#define COMMON_RTC_EVENT_COMPARE(channel) \
    CONCAT_2(NRF_RTC_EVENT_COMPARE_, channel)
#define COMMON_RTC_INT_COMPARE_MASK(channel) \
    CONCAT_3(NRF_RTC_INT_COMPARE, channel, _MASK)

#define US_TICKER_EVENT     COMMON_RTC_EVENT_COMPARE(US_TICKER_CC_CHANNEL)
#define US_TICKER_INT_MASK  COMMON_RTC_INT_COMPARE_MASK(US_TICKER_CC_CHANNEL)
#define OS_TICK_EVENT       COMMON_RTC_EVENT_COMPARE(OS_TICK_CC_CHANNEL)
#define OS_TICK_INT_MASK    COMMON_RTC_INT_COMPARE_MASK(OS_TICK_CC_CHANNEL)
#define LP_TICKER_EVENT     COMMON_RTC_EVENT_COMPARE(LP_TICKER_CC_CHANNEL)
#define LP_TICKER_INT_MASK  COMMON_RTC_INT_COMPARE_MASK(LP_TICKER_CC_CHANNEL)

extern bool              m_common_rtc_enabled;
extern uint32_t volatile m_common_rtc_overflows;

void     common_rtc_init(void);
uint32_t common_rtc_32bit_ticks_get(void);
uint64_t common_rtc_64bit_us_get(void);
void     common_rtc_set_interrupt(uint32_t us_timestamp, uint32_t cc_channel,
                                  uint32_t int_mask);

#endif // COMMON_RTC_H
