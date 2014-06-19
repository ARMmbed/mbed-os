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
#include "rtc_api.h"
#include "em_cmu.h"
#include "em_rtc.h"

static int rtc_inited = 0;
time_t time_base = 0;

void RTC_IRQHandler(void) {
    RTC_IntClear(RTC_IFC_OF);
    /* RTC has overflowed (24 bits). Use time_base as software counter for upper 8 bits. */
    time_base += 1 << 24;
}

void rtc_init(void) {
    /* Start LFRCO and wait until it is stable */
    CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

    /* Route the LFRCO clock to the RTC */
    CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFRCO);
    CMU_ClockEnable(cmuClock_RTC, true);

    /* Enable clock to the interface of the low energy modules */
    CMU_ClockEnable(cmuClock_CORELE, true);

    /* Scale clock to 1 second ticks */
    CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_32768);

    RTC_Init_TypeDef init = RTC_INIT_DEFAULT;
    init.enable = 1;
    /* Don't use compare register 0 as top value */
    init.comp0Top = 0;

    /* Enable Interrupt from RTC */
    RTC_IntEnable(RTC_IEN_OF);
    NVIC_EnableIRQ(RTC_IRQn);

    /* Initialize */
    RTC_Init(&init);
    rtc_inited = 1;
}

void rtc_free(void) {
    NVIC_DisableIRQ(RTC_IRQn);
    RTC_Reset();
    CMU_ClockEnable(cmuClock_RTC, false);
    rtc_inited = 0;
}

int rtc_isenabled(void) {
    return rtc_inited;
}

time_t rtc_read(void) {
    return (time_t)(RTC_CounterGet() + time_base);
}

void rtc_write(time_t t) {
    time_base = t;
    RTC_CounterReset();
}
