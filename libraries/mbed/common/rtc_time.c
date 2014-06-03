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

#include <time.h>
#include "rtc_time.h"
#include "us_ticker_api.h"

#ifdef __cplusplus
extern "C" {
#endif
#if defined (__ICCARM__)
time_t __time32(time_t *timer)
#else
time_t time(time_t *timer)
#endif

{
#if DEVICE_RTC
    if (!(rtc_isenabled())) {
        set_time(0);
    }
    time_t t = rtc_read();

#else
    time_t t = 0;
#endif

    if (timer != NULL) {
        *timer = t;
    }
    return t;
}

void set_time(time_t t) {
#if DEVICE_RTC
    rtc_init();
    rtc_write(t);
#endif
}

clock_t clock() {
    clock_t t = us_ticker_read();
    t /= 1000000 / CLOCKS_PER_SEC; // convert to processor time
    return t;
}

#ifdef __cplusplus
}
#endif
