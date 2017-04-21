/*
 * Profiling framework for the events library
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
#include "equeue.h"
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>


// Performance measurement utils
#define PROF_RUNS 5
#define PROF_INTERVAL 100000000

#define prof_volatile(t) __attribute__((unused)) volatile t

typedef uint64_t prof_cycle_t;

static volatile prof_cycle_t prof_start_cycle;
static volatile prof_cycle_t prof_stop_cycle;
static prof_cycle_t prof_accum_cycle;
static prof_cycle_t prof_baseline_cycle;
static prof_cycle_t prof_iterations;
static const char *prof_units;

#define prof_cycle() ({                                                     \
    uint32_t a, b;                                                          \
    __asm__ volatile ("rdtsc" : "=a" (a), "=d" (b));                        \
    ((uint64_t)b << 32) | (uint64_t)a;                                      \
})

#define prof_loop()                                                         \
    for (prof_iterations = 0;                                               \
         prof_accum_cycle < PROF_INTERVAL;                                  \
         prof_iterations++)

#define prof_start() ({                                                     \
    prof_start_cycle = prof_cycle();                                        \
})

#define prof_stop() ({                                                      \
    prof_stop_cycle = prof_cycle();                                         \
    prof_accum_cycle += prof_stop_cycle - prof_start_cycle;                 \
})

#define prof_result(value, units) ({                                        \
    prof_accum_cycle = value+prof_baseline_cycle;                           \
    prof_iterations = 1;                                                    \
    prof_units = units;                                                     \
})

#define prof_measure(func, ...) ({                                          \
    printf("%s: ...", #func);                                               \
    fflush(stdout);                                                         \
                                                                            \
    prof_units = "cycles";                                                  \
    prof_cycle_t runs[PROF_RUNS];                                           \
    for (int i = 0; i < PROF_RUNS; i++) {                                   \
        prof_accum_cycle = 0;                                               \
        prof_iterations = 0;                                                \
        func(__VA_ARGS__);                                                  \
        runs[i] = prof_accum_cycle / prof_iterations;                       \
    }                                                                       \
                                                                            \
    prof_cycle_t res = runs[0];                                             \
    for (int i = 0; i < PROF_RUNS; i++) {                                   \
        if (runs[i] < res) {                                                \
            res = runs[i];                                                  \
        }                                                                   \
    }                                                                       \
    res -= prof_baseline_cycle;                                             \
    printf("\r%s: %"PRIu64" %s", #func, res, prof_units);                   \
                                                                            \
    if (!isatty(0)) {                                                       \
        prof_cycle_t prev;                                                  \
        while (scanf("%*[^0-9]%"PRIu64, &prev) == 0);                       \
        int64_t perc = 100*((int64_t)prev - (int64_t)res) / (int64_t)prev;  \
                                                                            \
        if (perc > 10) {                                                    \
            printf(" (\e[32m%+"PRId64"%%\e[0m)", perc);                     \
        } else if (perc < -10) {                                            \
            printf(" (\e[31m%+"PRId64"%%\e[0m)", perc);                     \
        } else {                                                            \
            printf(" (%+"PRId64"%%)", perc);                                \
        }                                                                   \
    }                                                                       \
                                                                            \
    printf("\n");                                                           \
    res;                                                                    \
})

#define prof_baseline(func, ...) ({                                         \
    prof_baseline_cycle = 0;                                                \
    prof_baseline_cycle = prof_measure(func, __VA_ARGS__);                  \
})


// Various test functions
void no_func(void *eh) {
}


// Actual performance tests
void baseline_prof(void) {
    prof_loop() {
        prof_start();
        __asm__ volatile ("");
        prof_stop();
    }
}

void equeue_tick_prof(void) {
    prof_volatile(unsigned) res;
    prof_loop() {
        prof_start();
        res = equeue_tick();
        prof_stop();
    }
}

void equeue_alloc_prof(void) {
    struct equeue q;
    equeue_create(&q, 32*EQUEUE_EVENT_SIZE);

    prof_loop() {
        prof_start();
        void *e = equeue_alloc(&q, 8 * sizeof(int));
        prof_stop();

        equeue_dealloc(&q, e);
    }

    equeue_destroy(&q);
}

void equeue_alloc_many_prof(int count) {
    struct equeue q;
    equeue_create(&q, count*EQUEUE_EVENT_SIZE);

    void *es[count];

    for (int i = 0; i < count; i++) {
        es[i] = equeue_alloc(&q, (i % 4) * sizeof(int));
    }

    for (int i = 0; i < count; i++) {
        equeue_dealloc(&q, es[i]);
    }

    prof_loop() {
        prof_start();
        void *e = equeue_alloc(&q, 8 * sizeof(int));
        prof_stop();

        equeue_dealloc(&q, e);
    }

    equeue_destroy(&q);
}

void equeue_post_prof(void) {
    struct equeue q;
    equeue_create(&q, EQUEUE_EVENT_SIZE);

    prof_loop() {
        void *e = equeue_alloc(&q, 0);

        prof_start();
        int id = equeue_post(&q, no_func, e);
        prof_stop();

        equeue_cancel(&q, id);
    }

    equeue_destroy(&q);
}

void equeue_post_many_prof(int count) {
    struct equeue q;
    equeue_create(&q, count*EQUEUE_EVENT_SIZE);

    for (int i = 0; i < count-1; i++) {
        equeue_call(&q, no_func, 0);
    }

    prof_loop() {
        void *e = equeue_alloc(&q, 0);

        prof_start();
        int id = equeue_post(&q, no_func, e);
        prof_stop();

        equeue_cancel(&q, id);
    }

    equeue_destroy(&q);
}

void equeue_post_future_prof(void) {
    struct equeue q;
    equeue_create(&q, EQUEUE_EVENT_SIZE);

    prof_loop() {
        void *e = equeue_alloc(&q, 0);
        equeue_event_delay(e, 1000);

        prof_start();
        int id = equeue_post(&q, no_func, e);
        prof_stop();

        equeue_cancel(&q, id);
    }

    equeue_destroy(&q);
}

void equeue_post_future_many_prof(int count) {
    struct equeue q;
    equeue_create(&q, count*EQUEUE_EVENT_SIZE);

    for (int i = 0; i < count-1; i++) {
        equeue_call(&q, no_func, 0);
    }

    prof_loop() {
        void *e = equeue_alloc(&q, 0);
        equeue_event_delay(e, 1000);

        prof_start();
        int id = equeue_post(&q, no_func, e);
        prof_stop();

        equeue_cancel(&q, id);
    }

    equeue_destroy(&q);
}

void equeue_dispatch_prof(void) {
    struct equeue q;
    equeue_create(&q, EQUEUE_EVENT_SIZE);

    prof_loop() {
        equeue_call(&q, no_func, 0);

        prof_start();
        equeue_dispatch(&q, 0);
        prof_stop();
    }

    equeue_destroy(&q);
}

void equeue_dispatch_many_prof(int count) {
    struct equeue q;
    equeue_create(&q, count*EQUEUE_EVENT_SIZE);

    prof_loop() {
        for (int i = 0; i < count; i++) {
            equeue_call(&q, no_func, 0);
        }

        prof_start();
        equeue_dispatch(&q, 0);
        prof_stop();
    }

    equeue_destroy(&q);
}

void equeue_cancel_prof(void) {
    struct equeue q;
    equeue_create(&q, EQUEUE_EVENT_SIZE);

    prof_loop() {
        int id = equeue_call(&q, no_func, 0);

        prof_start();
        equeue_cancel(&q, id);
        prof_stop();
    }

    equeue_destroy(&q);
}

void equeue_cancel_many_prof(int count) {
    struct equeue q;
    equeue_create(&q, count*EQUEUE_EVENT_SIZE);

    for (int i = 0; i < count-1; i++) {
        equeue_call(&q, no_func, 0);
    }

    prof_loop() {
        int id = equeue_call(&q, no_func, 0);

        prof_start();
        equeue_cancel(&q, id);
        prof_stop();
    }

    equeue_destroy(&q);
}

void equeue_alloc_size_prof(void) {
    size_t size = 32*EQUEUE_EVENT_SIZE;

    struct equeue q;
    equeue_create(&q, size);
    equeue_alloc(&q, 0);

    prof_result(size - q.slab.size, "bytes");

    equeue_destroy(&q);
}

void equeue_alloc_many_size_prof(int count) {
    size_t size = count*EQUEUE_EVENT_SIZE;

    struct equeue q;
    equeue_create(&q, size);

    for (int i = 0; i < count; i++) {
        equeue_alloc(&q, (i % 4) * sizeof(int));
    }

    prof_result(size - q.slab.size, "bytes");

    equeue_destroy(&q);
}

void equeue_alloc_fragmented_size_prof(int count) {
    size_t size = count*EQUEUE_EVENT_SIZE;

    struct equeue q;
    equeue_create(&q, size);

    void *es[count];

    for (int i = 0; i < count; i++) {
        es[i] = equeue_alloc(&q, (i % 4) * sizeof(int));
    }

    for (int i = 0; i < count; i++) {
        equeue_dealloc(&q, es[i]);
    }

    for (int i = count-1; i >= 0; i--) {
        es[i] = equeue_alloc(&q, (i % 4) * sizeof(int));
    }

    for (int i = count-1; i >= 0; i--) {
        equeue_dealloc(&q, es[i]);
    }

    for (int i = 0; i < count; i++) {
        equeue_alloc(&q, (i % 4) * sizeof(int));
    }

    prof_result(size - q.slab.size, "bytes");

    equeue_destroy(&q);
}


// Entry point
int main() {
    printf("beginning profiling...\n");

    prof_baseline(baseline_prof);

    prof_measure(equeue_tick_prof);
    prof_measure(equeue_alloc_prof);
    prof_measure(equeue_post_prof);
    prof_measure(equeue_post_future_prof);
    prof_measure(equeue_dispatch_prof);
    prof_measure(equeue_cancel_prof);

    prof_measure(equeue_alloc_many_prof, 1000);
    prof_measure(equeue_post_many_prof, 1000);
    prof_measure(equeue_post_future_many_prof, 1000);
    prof_measure(equeue_dispatch_many_prof, 100);
    prof_measure(equeue_cancel_many_prof, 100);

    prof_measure(equeue_alloc_size_prof);
    prof_measure(equeue_alloc_many_size_prof, 1000);
    prof_measure(equeue_alloc_fragmented_size_prof, 1000);

    printf("done!\n");
}
