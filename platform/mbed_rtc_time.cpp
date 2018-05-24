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
#include "hal/rtc_api.h"

#include "platform/mbed_critical.h"
#include "platform/mbed_rtc_time.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"

static SingletonPtr<PlatformMutex> _mutex;

#if DEVICE_RTC

static void (*_rtc_init)(void) = rtc_init;
static int (*_rtc_isenabled)(void) = rtc_isenabled;
static time_t (*_rtc_read)(void) = rtc_read;
static void (*_rtc_write)(time_t t) = rtc_write;

#elif DEVICE_LPTICKER

#include "drivers/LowPowerTimer.h"

static SingletonPtr<mbed::LowPowerTimer> _rtc_lp_timer;
static uint64_t _rtc_lp_base;
static bool _rtc_enabled;

static void _rtc_lpticker_init(void)
{
    _rtc_lp_timer->start();
    _rtc_enabled = true;
}

static int _rtc_lpticker_isenabled(void)
{
    return (_rtc_enabled == true);
}

static time_t _rtc_lpticker_read(void)
{
    return (uint64_t)_rtc_lp_timer->read() + _rtc_lp_base;
}

static void _rtc_lpticker_write(time_t t)
{
    _rtc_lp_base = t;
}

static void (*_rtc_init)(void) = _rtc_lpticker_init;
static int (*_rtc_isenabled)(void) = _rtc_lpticker_isenabled;
static time_t (*_rtc_read)(void) = _rtc_lpticker_read;
static void (*_rtc_write)(time_t t) = _rtc_lpticker_write;

#else /* DEVICE_LPTICKER */

static void (*_rtc_init)(void) = NULL;
static int (*_rtc_isenabled)(void) = NULL;
static time_t (*_rtc_read)(void) = NULL;
static void (*_rtc_write)(time_t t) = NULL;
#endif /* DEVICE_LPTICKER */

#ifdef __cplusplus
extern "C" {
#endif
#if defined (__ICCARM__)
time_t __time32(time_t *timer)
#else
time_t time(time_t *timer)
#endif

{
    _mutex->lock();
    if (_rtc_isenabled != NULL) {
        if (!(_rtc_isenabled())) {
            set_time(0);
        }
    }
    
    time_t t = (time_t)-1;
    if (_rtc_read != NULL) {
        t = _rtc_read();
    }

    if (timer != NULL) {
        *timer = t;
    }
    _mutex->unlock();
    return t;
}

void set_time(time_t t) {
    _mutex->lock();
    if (_rtc_init != NULL) {
        _rtc_init();
    }
    if (_rtc_write != NULL) {
        _rtc_write(t);
    }
    _mutex->unlock();
}

void attach_rtc(time_t (*read_rtc)(void), void (*write_rtc)(time_t), void (*init_rtc)(void), int (*isenabled_rtc)(void)) {
    _mutex->lock();
    _rtc_read = read_rtc;
    _rtc_write = write_rtc;
    _rtc_init = init_rtc;
    _rtc_isenabled = isenabled_rtc;
    _mutex->unlock();
}



#ifdef __cplusplus
}
#endif
