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

#include "device.h"
#if DEVICE_LOWPOWERTIMER

#include "rtc_api.h"
#include "rtc_api_HAL.h"
#include "lp_ticker_api.h"

void lp_ticker_init()
{
    rtc_init_real(RTC_INIT_LPTIMER);
    rtc_set_comp0_handler((uint32_t)lp_ticker_irq_handler);
}

void lp_ticker_set_interrupt(timestamp_t timestamp) {
	uint64_t timestamp_ticks;
    uint64_t current_ticks = RTC_CounterGet();
    timestamp_t current_time = ((uint64_t)(current_ticks * 1000000) / (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT));
    
    
    /* calculate offset value */
    timestamp_t offset = timestamp - current_time;
    if(offset > 0xEFFFFFFF) offset = 100;
    
    /* map offset to RTC value */
	// ticks = offset * RTC frequency div 1000000
	timestamp_ticks = ((uint64_t)offset * (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT)) / 1000000;
    timestamp_ticks += current_ticks;

	/* RTC has 24 bit resolution */
	timestamp_ticks &= 0xFFFFFF;

    /* check for RTC limitation */
    if((timestamp_ticks - RTC_CounterGet()) >= 0x800000) timestamp_ticks = RTC_CounterGet() + 2;
    
	/* Set callback */
	RTC_FreezeEnable(true);
	RTC_CompareSet(0, (uint32_t)timestamp_ticks);
	RTC_IntEnable(RTC_IF_COMP0);
	RTC_FreezeEnable(false);
}

inline void lp_ticker_disable_interrupt() {
	RTC_IntDisable(RTC_IF_COMP0);
}

inline void lp_ticker_clear_interrupt() {
	RTC_IntClear(RTC_IF_COMP0);
}

timestamp_t lp_ticker_read() {
	uint64_t ticks_temp;
	uint64_t ticks = RTC_CounterGet();

	/* ticks = counter tick value
	 * timestamp = value in microseconds
	 * timestamp = ticks * 1.000.000 / RTC frequency
	 */

	ticks_temp = (ticks * 1000000) / (LOW_ENERGY_CLOCK_FREQUENCY / RTC_CLOCKDIV_INT);
	return (timestamp_t) (ticks_temp & 0xFFFFFFFF);
}

#endif
