/*
 * Copyright (c) 2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#include "internal/equeue_platform.h"

#if defined(EQUEUE_PLATFORM_POSIX)

#include <errno.h>

/*
 * Using global variable as a simulation of passing time. Use of sleep functions may cause problems with thread preempting, which can lead to bad timings.
 * This problem does not occur on targets.
 * This variable is only increased in equeue_sema_wait function and in functions that simulate pass of time.
 */
unsigned int equeue_global_time = 0;

// Tick operations
void equeue_tick_init(void) {}

unsigned equeue_tick(void)
{
    return equeue_global_time;
}


// Mutex operations
int equeue_mutex_create(equeue_mutex_t *m)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    return pthread_mutex_init(m, &attr);
}

void equeue_mutex_destroy(equeue_mutex_t *m)
{
    pthread_mutex_destroy(m);
}

void equeue_mutex_lock(equeue_mutex_t *m)
{
    pthread_mutex_lock(m);
}

void equeue_mutex_unlock(equeue_mutex_t *m)
{
    pthread_mutex_unlock(m);
}


// Semaphore operations
int equeue_sema_create(equeue_sema_t *s)
{
    int err = pthread_mutex_init(&s->mutex, 0);
    if (err) {
        return err;
    }

    err = pthread_cond_init(&s->cond, 0);
    if (err) {
        return err;
    }

    s->signal = false;
    return 0;
}

void equeue_sema_destroy(equeue_sema_t *s)
{
    pthread_cond_destroy(&s->cond);
    pthread_mutex_destroy(&s->mutex);
}

void equeue_sema_signal(equeue_sema_t *s)
{
    pthread_mutex_lock(&s->mutex);
    s->signal = true;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->mutex);
}

bool equeue_sema_wait(equeue_sema_t *s, int ms)
{
    pthread_mutex_lock(&s->mutex);
    if (!s->signal) {
        if (ms < 0) {
            pthread_cond_wait(&s->cond, &s->mutex);
        } else {
            for (int i = 0; i < ms; i++) {
                equeue_global_time++;
            }
            // If ms == 0 increase time so functions don't get stuck in infinite loops.
            if (ms == 0) {
                equeue_global_time++;
            }
        }
    }

    bool signal = s->signal;
    s->signal = false;
    pthread_mutex_unlock(&s->mutex);

    return signal;
}

#endif
