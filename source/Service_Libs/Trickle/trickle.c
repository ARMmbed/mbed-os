/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
/*
 * trickle.c
 *
 * Implement a generic RFC 6206 Trickle Algorithm
 */
#include "nsconfig.h"
#include <ns_types.h>
#include <randLIB.h>

#include "ns_trace.h"

#include "Service_Libs/Trickle/trickle.h"

#define TRACE_GROUP "tric"

/* RFC 6206 Rule 2 */
void trickle_begin_interval(trickle_t *t)
{
    t->c = 0;
    if (t->I > 2) { //Take random only when t->I is bigger than 2 otherwise result will be 1
        t->t = randLIB_get_random_in_range(t->I / 2, t->I - 1);
    } else {
        t->t = 1;
    }
    t->now = 0;
}

/* RFC 6206 Rule 1 */
void trickle_start(trickle_t *t, const trickle_params_t *params)
{
    t->e = 0;
    t->I = randLIB_get_random_in_range(params->Imin, params->Imax);
    trickle_begin_interval(t);
}

uint32_t trickle_timer_max(const trickle_params_t *params, uint8_t trickle_timer_expiration)
{
    trickle_time_t time_I = params->Imin - 1;
    uint32_t total_time = time_I;
    while (trickle_timer_expiration) {
        if (time_I <= TRICKLE_TIME_MAX / 2) {
            time_I *= 2;
        } else {
            time_I = TRICKLE_TIME_MAX;
        }

        if (time_I > params->Imax) {
            time_I = params->Imax;
        }
        total_time += time_I;
        trickle_timer_expiration--;
    }
    return total_time;
}

/* We don't expose the raw reset as API; users should use "inconsistent_heard".
 * This avoids repeated resets stopping transmission by restarting the interval.
 */
static void trickle_reset_timer(trickle_t *t, const trickle_params_t *params)
{
    t->e = 0;
    t->I = params->Imin;
    trickle_begin_interval(t);

    tr_debug("trickle reset timer");
}

/* RFC 6206 Rule 3 */
void trickle_consistent_heard(trickle_t *t)
{
    if (t->c < UINT8_MAX) {
        t->c++;
    }
}

/* RFC 6206 Rule 6 */
void trickle_inconsistent_heard(trickle_t *t, const trickle_params_t *params)
{
    if (t->I != params->Imin || !trickle_running(t, params)) {
        trickle_reset_timer(t, params);
    }
}

bool trickle_running(const trickle_t *t, const trickle_params_t *params)
{
    return t->e < params->TimerExpirations;
}


/* Returns true if you should transmit now */
bool trickle_timer(trickle_t *t, const trickle_params_t *params, uint16_t ticks)
{
    if (!trickle_running(t, params)) {
        return false;
    }

    bool transmit = false;
    trickle_time_t new_time = t->now + ticks;

    /* Catch overflow */
    if (new_time < t->now) {
        new_time = TRICKLE_TIME_MAX;
    }

    /* RFC 6206 Rule 4 */
    if (t->now < t->t && new_time >= t->t) {
        /* Treat k == 0 as "infinity", as per RFC 6206 6.5 */
        if (t->c < params->k || params->k == 0) {
            transmit = true;
        }
    }

    /* RFC 6206 Rule 5 */
    t->now = new_time;
    if (new_time >= t->I) {
        if (t->I <= TRICKLE_TIME_MAX / 2) {
            t->I *= 2;
        } else {
            t->I = TRICKLE_TIME_MAX;
        }

        if (t->I > params->Imax) {
            t->I = params->Imax;
        }

        if (t->e < TRICKLE_EXPIRATIONS_INFINITE - 1) {
            t->e++;
        }
        trickle_begin_interval(t);
    }

    return transmit;
}

/* Stop the timer (by setting e to infinite) */
void trickle_stop(trickle_t *t)
{
    t->e = TRICKLE_EXPIRATIONS_INFINITE;
}
