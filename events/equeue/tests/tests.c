/*
 * Testing framework for the events library
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
#include <pthread.h>


// Testing setup
static jmp_buf test_buf;
static int test_line;
static int test_failure;

#define test_assert(test) ({                                                \
    if (!(test)) {                                                          \
        test_line = __LINE__;                                               \
        longjmp(test_buf, 1);                                               \
    }                                                                       \
})

#define test_run(func, ...) ({                                              \
    printf("%s: ...", #func);                                               \
    fflush(stdout);                                                         \
                                                                            \
    if (!setjmp(test_buf)) {                                                \
        func(__VA_ARGS__);                                                  \
        printf("\r%s: \e[32mpassed\e[0m\n", #func);                         \
    } else {                                                                \
        printf("\r%s: \e[31mfailed\e[0m at line %d\n", #func, test_line);   \
        test_failure = true;                                                \
    }                                                                       \
})


// Test functions
void pass_func(void *eh) {
}

void simple_func(void *p) {
    (*(int *)p)++;
}

void sloth_func(void *p) {
    usleep(10000);
    (*(int *)p)++;
}

struct indirect {
    int *touched;
    uint8_t buffer[7];
};

void indirect_func(void *p) {
    struct indirect *i = (struct indirect*)p;
    (*i->touched)++;
}

struct timing {
    unsigned tick;
    unsigned delay;
};

void timing_func(void *p) {
    struct timing *timing = (struct timing*)p;
    unsigned tick = equeue_tick();

    unsigned t1 = timing->delay;
    unsigned t2 = tick - timing->tick;
    test_assert(t1 > t2 - 10 && t1 < t2 + 10);

    timing->tick = tick;
}

struct fragment {
    equeue_t *q;
    size_t size;
    struct timing timing;
};

void fragment_func(void *p) {
    struct fragment *fragment = (struct fragment*)p;
    timing_func(&fragment->timing);

    struct fragment *nfragment = equeue_alloc(fragment->q, fragment->size);
    test_assert(nfragment);

    *nfragment = *fragment;
    equeue_event_delay(nfragment, fragment->timing.delay);

    int id = equeue_post(nfragment->q, fragment_func, nfragment);
    test_assert(id);
}

struct cancel {
    equeue_t *q;
    int id;
};

void cancel_func(void *p) {
    struct cancel *cancel = (struct cancel *)p;
    equeue_cancel(cancel->q, cancel->id);
}

struct nest {
    equeue_t *q;
    void (*cb)(void *);
    void *data;
};

void nest_func(void *p) {
    struct nest *nest = (struct nest *)p;
    equeue_call(nest->q, nest->cb, nest->data);

    usleep(10000);
}


// Simple call tests
void simple_call_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    equeue_call(&q, simple_func, &touched);
    equeue_dispatch(&q, 0);
    test_assert(touched);

    equeue_destroy(&q);
}

void simple_call_in_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    int id = equeue_call_in(&q, 10, simple_func, &touched);
    test_assert(id);

    equeue_dispatch(&q, 15);
    test_assert(touched);

    equeue_destroy(&q);
}

void simple_call_every_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    int id = equeue_call_every(&q, 10, simple_func, &touched);
    test_assert(id);

    equeue_dispatch(&q, 15);
    test_assert(touched);

    equeue_destroy(&q);
}

void simple_post_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int touched = false;
    struct indirect *i = equeue_alloc(&q, sizeof(struct indirect));
    test_assert(i);

    i->touched = &touched;
    int id = equeue_post(&q, indirect_func, i);
    test_assert(id);

    equeue_dispatch(&q, 0);
    test_assert(*i->touched);

    equeue_destroy(&q);
}

// Misc tests
void destructor_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int touched;
    struct indirect *e;
    int ids[3];

    touched = 0;
    for (int i = 0; i < 3; i++) {
        e = equeue_alloc(&q, sizeof(struct indirect));
        test_assert(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        int id = equeue_post(&q, pass_func, e);
        test_assert(id);
    }

    equeue_dispatch(&q, 0);
    test_assert(touched == 3);

    touched = 0;
    for (int i = 0; i < 3; i++) {
        e = equeue_alloc(&q, sizeof(struct indirect));
        test_assert(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        ids[i] = equeue_post(&q, pass_func, e);
        test_assert(ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        equeue_cancel(&q, ids[i]);
    }

    equeue_dispatch(&q, 0);
    test_assert(touched == 3);

    touched = 0;
    for (int i = 0; i < 3; i++) {
        e = equeue_alloc(&q, sizeof(struct indirect));
        test_assert(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        int id = equeue_post(&q, pass_func, e);
        test_assert(id);
    }

    equeue_destroy(&q);
    test_assert(touched == 3);
}

void allocation_failure_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    void *p = equeue_alloc(&q, 4096);
    test_assert(!p);

    for (int i = 0; i < 100; i++) {
        p = equeue_alloc(&q, 0);
    }
    test_assert(!p);

    equeue_destroy(&q);
}

void cancel_test(int N) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    int *ids = malloc(N*sizeof(int));

    for (int i = 0; i < N; i++) {
        ids[i] = equeue_call(&q, simple_func, &touched);
    }

    for (int i = N-1; i >= 0; i--) {
        equeue_cancel(&q, ids[i]);
    }

    free(ids);

    equeue_dispatch(&q, 0);
    test_assert(!touched);

    equeue_destroy(&q);
}

void cancel_inflight_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;

    int id = equeue_call(&q, simple_func, &touched);
    equeue_cancel(&q, id);

    equeue_dispatch(&q, 0);
    test_assert(!touched);

    id = equeue_call(&q, simple_func, &touched);
    equeue_cancel(&q, id);

    equeue_dispatch(&q, 0);
    test_assert(!touched);

    struct cancel *cancel = equeue_alloc(&q, sizeof(struct cancel));
    test_assert(cancel);
    cancel->q = &q;
    cancel->id = 0;

    id = equeue_post(&q, cancel_func, cancel);
    test_assert(id);

    cancel->id = equeue_call(&q, simple_func, &touched);

    equeue_dispatch(&q, 0);
    test_assert(!touched);

    equeue_destroy(&q);
}

void cancel_unnecessarily_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int id = equeue_call(&q, pass_func, 0);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    id = equeue_call(&q, pass_func, 0);
    equeue_dispatch(&q, 0);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    bool touched = false;
    equeue_call(&q, simple_func, &touched);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    equeue_dispatch(&q, 0);
    test_assert(touched);

    equeue_destroy(&q);
}

void loop_protect_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    equeue_call_every(&q, 0, simple_func, &touched);

    equeue_dispatch(&q, 0);
    test_assert(touched);

    touched = false;
    equeue_call_every(&q, 1, simple_func, &touched);

    equeue_dispatch(&q, 0);
    test_assert(touched);

    equeue_destroy(&q);
}

void break_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    bool touched = false;
    equeue_call_every(&q, 0, simple_func, &touched);

    equeue_break(&q);
    equeue_dispatch(&q, -1);
    test_assert(touched);

    equeue_destroy(&q);
}

void break_no_windup_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int count = 0;
    equeue_call_every(&q, 0, simple_func, &count);

    equeue_break(&q);
    equeue_break(&q);
    equeue_dispatch(&q, -1);
    test_assert(count == 1);

    count = 0;
    equeue_dispatch(&q, 55);
    test_assert(count > 1);

    equeue_destroy(&q);
}

void period_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int count = 0;
    equeue_call_every(&q, 10, simple_func, &count);

    equeue_dispatch(&q, 55);
    test_assert(count == 5);

    equeue_destroy(&q);
}

void nested_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int touched = 0;
    struct nest *nest = equeue_alloc(&q, sizeof(struct nest));
    test_assert(nest);
    nest->q = &q;
    nest->cb = simple_func;
    nest->data = &touched;

    int id = equeue_post(&q, nest_func, nest);
    test_assert(id);

    equeue_dispatch(&q, 5);
    test_assert(touched == 0);

    equeue_dispatch(&q, 5);
    test_assert(touched == 1);

    touched = 0;
    nest = equeue_alloc(&q, sizeof(struct nest));
    test_assert(nest);
    nest->q = &q;
    nest->cb = simple_func;
    nest->data = &touched;

    id = equeue_post(&q, nest_func, nest);
    test_assert(id);

    equeue_dispatch(&q, 20);
    test_assert(touched == 1);

    equeue_destroy(&q);
}

void sloth_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int touched = 0;
    int id = equeue_call(&q, sloth_func, &touched);
    test_assert(id);

    id = equeue_call_in(&q, 5, simple_func, &touched);
    test_assert(id);

    id = equeue_call_in(&q, 15, simple_func, &touched);
    test_assert(id);

    equeue_dispatch(&q, 20);
    test_assert(touched == 3);

    equeue_destroy(&q);
}

void *multithread_thread(void *p) {
    equeue_t *q = (equeue_t *)p;
    equeue_dispatch(q, -1);
    return 0;
}

void multithread_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int touched = 0;
    equeue_call_every(&q, 1, simple_func, &touched);

    pthread_t thread;
    err = pthread_create(&thread, 0, multithread_thread, &q);
    test_assert(!err);

    usleep(10000);
    equeue_break(&q);
    err = pthread_join(thread, 0);
    test_assert(!err);

    test_assert(touched);

    equeue_destroy(&q);
}

void background_func(void *p, int ms) {
    *(unsigned *)p = ms;
}

void background_test(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    int id = equeue_call_in(&q, 20, pass_func, 0);
    test_assert(id);

    unsigned ms;
    equeue_background(&q, background_func, &ms);
    test_assert(ms == 20);

    id = equeue_call_in(&q, 10, pass_func, 0);
    test_assert(id);
    test_assert(ms == 10);

    id = equeue_call(&q, pass_func, 0);
    test_assert(id);
    test_assert(ms == 0);

    equeue_dispatch(&q, 0);
    test_assert(ms == 10);

    equeue_destroy(&q);
    test_assert(ms == -1);
}

void chain_test(void) {
    equeue_t q1;
    int err = equeue_create(&q1, 2048);
    test_assert(!err);

    equeue_t q2;
    err = equeue_create(&q2, 2048);
    test_assert(!err);

    equeue_chain(&q2, &q1);

    int touched = 0;

    int id1 = equeue_call_in(&q1, 20, simple_func, &touched);
    int id2 = equeue_call_in(&q2, 20, simple_func, &touched);
    test_assert(id1 && id2);

    id1 = equeue_call(&q1, simple_func, &touched);
    id2 = equeue_call(&q2, simple_func, &touched);
    test_assert(id1 && id2);

    id1 = equeue_call_in(&q1, 5, simple_func, &touched);
    id2 = equeue_call_in(&q2, 5, simple_func, &touched);
    test_assert(id1 && id2);

    equeue_cancel(&q1, id1);
    equeue_cancel(&q2, id2);

    id1 = equeue_call_in(&q1, 10, simple_func, &touched);
    id2 = equeue_call_in(&q2, 10, simple_func, &touched);
    test_assert(id1 && id2);

    equeue_dispatch(&q1, 30);

    test_assert(touched == 6);

    equeue_destroy(&q1);
    equeue_destroy(&q2);
}

void unchain_test(void) {
    equeue_t q1;
    int err = equeue_create(&q1, 2048);
    test_assert(!err);

    equeue_t q2;
    err = equeue_create(&q2, 2048);
    test_assert(!err);

    equeue_chain(&q2, &q1);

    int touched = 0;
    int id1 = equeue_call(&q1, simple_func, &touched);
    int id2 = equeue_call(&q2, simple_func, &touched);
    test_assert(id1 && id2);

    equeue_dispatch(&q1, 0);
    test_assert(touched == 2);

    equeue_chain(&q2, 0);
    equeue_chain(&q1, &q2);

    id1 = equeue_call(&q1, simple_func, &touched);
    id2 = equeue_call(&q2, simple_func, &touched);
    test_assert(id1 && id2);

    equeue_dispatch(&q2, 0);
    test_assert(touched == 4);

    equeue_destroy(&q1);
    equeue_destroy(&q2);
}

// Barrage tests
void simple_barrage_test(int N) {
    equeue_t q;
    int err = equeue_create(&q, N*(EQUEUE_EVENT_SIZE+sizeof(struct timing)));
    test_assert(!err);

    for (int i = 0; i < N; i++) {
        struct timing *timing = equeue_alloc(&q, sizeof(struct timing));
        test_assert(timing);

        timing->tick = equeue_tick();
        timing->delay = (i+1)*100;
        equeue_event_delay(timing, timing->delay);
        equeue_event_period(timing, timing->delay);

        int id = equeue_post(&q, timing_func, timing);
        test_assert(id);
    }

    equeue_dispatch(&q, N*100);

    equeue_destroy(&q);
}

void fragmenting_barrage_test(int N) {
    equeue_t q;
    int err = equeue_create(&q,
            2*N*(EQUEUE_EVENT_SIZE+sizeof(struct fragment)+N*sizeof(int)));
    test_assert(!err);

    for (int i = 0; i < N; i++) {
        size_t size = sizeof(struct fragment) + i*sizeof(int);
        struct fragment *fragment = equeue_alloc(&q, size);
        test_assert(fragment);

        fragment->q = &q;
        fragment->size = size;
        fragment->timing.tick = equeue_tick();
        fragment->timing.delay = (i+1)*100;
        equeue_event_delay(fragment, fragment->timing.delay);

        int id = equeue_post(&q, fragment_func, fragment);
        test_assert(id);
    }

    equeue_dispatch(&q, N*100);

    equeue_destroy(&q);
}

struct ethread {
    pthread_t thread;
    equeue_t *q;
    int ms;
};

static void *ethread_dispatch(void *p) {
    struct ethread *t = (struct ethread*)p;
    equeue_dispatch(t->q, t->ms);
    return 0;
}

void multithreaded_barrage_test(int N) {
    equeue_t q;
    int err = equeue_create(&q, N*(EQUEUE_EVENT_SIZE+sizeof(struct timing)));
    test_assert(!err);

    struct ethread t;
    t.q = &q;
    t.ms = N*100;
    err = pthread_create(&t.thread, 0, ethread_dispatch, &t);
    test_assert(!err);

    for (int i = 0; i < N; i++) {
        struct timing *timing = equeue_alloc(&q, sizeof(struct timing));
        test_assert(timing);

        timing->tick = equeue_tick();
        timing->delay = (i+1)*100;
        equeue_event_delay(timing, timing->delay);
        equeue_event_period(timing, timing->delay);

        int id = equeue_post(&q, timing_func, timing);
        test_assert(id);
    }

    err = pthread_join(t.thread, 0);
    test_assert(!err);

    equeue_destroy(&q);
}

struct count_and_queue
{
    int p;
    equeue_t* q;
};

void simple_breaker(void *p) {
    struct count_and_queue* caq = (struct count_and_queue*)p;
    equeue_break(caq->q);
    usleep(10000);
    caq->p++;
}

void break_request_cleared_on_timeout(void) {
    equeue_t q;
    int err = equeue_create(&q, 2048);
    test_assert(!err);

    struct count_and_queue pq;
    pq.p = 0;
    pq.q = &q;

    int id = equeue_call_every(&q, 10, simple_breaker, &pq);

    equeue_dispatch(&q, 10);
    test_assert(pq.p == 1);

    equeue_cancel(&q, id);

    int count = 0;
    equeue_call_every(&q, 10, simple_func, &count);

    equeue_dispatch(&q, 55);
    test_assert(count > 1);

    equeue_destroy(&q);
}

int main() {
    printf("beginning tests...\n");

    test_run(simple_call_test);
    test_run(simple_call_in_test);
    test_run(simple_call_every_test);
    test_run(simple_post_test);
    test_run(destructor_test);
    test_run(allocation_failure_test);
    test_run(cancel_test, 20);
    test_run(cancel_inflight_test);
    test_run(cancel_unnecessarily_test);
    test_run(loop_protect_test);
    test_run(break_test);
    test_run(break_no_windup_test);
    test_run(period_test);
    test_run(nested_test);
    test_run(sloth_test);
    test_run(background_test);
    test_run(chain_test);
    test_run(unchain_test);
    test_run(multithread_test);
    test_run(simple_barrage_test, 20);
    test_run(fragmenting_barrage_test, 20);
    test_run(multithreaded_barrage_test, 20);
    test_run(break_request_cleared_on_timeout);

    printf("done!\n");
    return test_failure;
}
