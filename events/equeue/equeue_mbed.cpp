/*
 * Implementation for the mbed library
 * https://github.com/mbedmicro/mbed
 *
 * Copyright (c) 2016 Christopher Haster
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
#include "equeue/equeue_platform.h"

#if defined(EQUEUE_PLATFORM_MBED)

#include <stdbool.h>
#include "mbed.h"


// Ticker operations
static bool equeue_tick_inited = false;
static volatile unsigned equeue_minutes = 0;
static unsigned equeue_timer[
        (sizeof(Timer)+sizeof(unsigned)-1)/sizeof(unsigned)];
static unsigned equeue_ticker[
        (sizeof(Ticker)+sizeof(unsigned)-1)/sizeof(unsigned)];

static void equeue_tick_update() {
    equeue_minutes += reinterpret_cast<Timer*>(equeue_timer)->read_ms();
    reinterpret_cast<Timer*>(equeue_timer)->reset();
}

static void equeue_tick_init() {
    MBED_STATIC_ASSERT(sizeof(equeue_timer) >= sizeof(Timer),
            "The equeue_timer buffer must fit the class Timer");
    MBED_STATIC_ASSERT(sizeof(equeue_ticker) >= sizeof(Ticker),
            "The equeue_ticker buffer must fit the class Ticker");
    new (equeue_timer) Timer;
    new (equeue_ticker) Ticker;

    equeue_minutes = 0;
    reinterpret_cast<Timer*>(equeue_timer)->start();
    reinterpret_cast<Ticker*>(equeue_ticker)
            ->attach_us(equeue_tick_update, 1000 << 16);

    equeue_tick_inited = true;
}

unsigned equeue_tick() {
    if (!equeue_tick_inited) {
        equeue_tick_init();
    }

    unsigned minutes;
    unsigned ms;

    do {
        minutes = equeue_minutes;
        ms = reinterpret_cast<Timer*>(equeue_timer)->read_ms();
    } while (minutes != equeue_minutes);

    return minutes + ms;
}


// Mutex operations
int equeue_mutex_create(equeue_mutex_t *m) { return 0; }
void equeue_mutex_destroy(equeue_mutex_t *m) { }

void equeue_mutex_lock(equeue_mutex_t *m) {
    core_util_critical_section_enter();
}

void equeue_mutex_unlock(equeue_mutex_t *m) {
    core_util_critical_section_exit();
}


// Semaphore operations
#ifdef MBED_CONF_RTOS_PRESENT

int equeue_sema_create(equeue_sema_t *s) {
    MBED_STATIC_ASSERT(sizeof(equeue_sema_t) >= sizeof(Semaphore),
            "The equeue_sema_t must fit the class Semaphore");
    new (s) Semaphore(0);
    return 0;
}

void equeue_sema_destroy(equeue_sema_t *s) {
    reinterpret_cast<Semaphore*>(s)->~Semaphore();
}

void equeue_sema_signal(equeue_sema_t *s) {
    reinterpret_cast<Semaphore*>(s)->release();
}

bool equeue_sema_wait(equeue_sema_t *s, int ms) {
    if (ms < 0) {
        ms = osWaitForever;
    }

    return (reinterpret_cast<Semaphore*>(s)->wait(ms) > 0);
}

#else

// Semaphore operations
int equeue_sema_create(equeue_sema_t *s) {
    *s = false;
    return 0;
}

void equeue_sema_destroy(equeue_sema_t *s) {
}

void equeue_sema_signal(equeue_sema_t *s) {
    *s = 1;
}

static void equeue_sema_timeout(equeue_sema_t *s) {
    *s = -1;
}

bool equeue_sema_wait(equeue_sema_t *s, int ms) {
    int signal = 0;
    Timeout timeout;
    if (ms > 0) {
        timeout.attach_us(callback(equeue_sema_timeout, s), ms*1000);
    }

    core_util_critical_section_enter();
    while (!*s) {
        sleep();
        core_util_critical_section_exit();
        core_util_critical_section_enter();
    }

    signal = *s;
    *s = false;
    core_util_critical_section_exit();

    return (signal > 0);
}

#endif

#endif
