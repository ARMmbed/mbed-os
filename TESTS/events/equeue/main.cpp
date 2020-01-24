/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef MBED_EXTENDED_TESTS
#error [NOT_SUPPORTED] When running on CI this test is disabled due to limiting testing time.
#else

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include "equeue.h"
#include "mbed.h"

using namespace utest::v1;

#define TEST_EQUEUE_SIZE (4*EVENTS_EVENT_SIZE)
#define TEST_THREAD_STACK_SIZE 512
#define DISPATCH_INFINITE -1

// Test functions
static void pass_func(void *eh)
{
}

static void simple_func(void *p)
{
    uint8_t *d = reinterpret_cast<uint8_t *>(p);
    if (*d < 255) {
        (*d)++;
    }
}

static void sloth_func(void *p)
{
    ThisThread::sleep_for(10);
    (*(reinterpret_cast<uint8_t *>(p)))++;
}

struct indirect {
    uint8_t *touched;
    uint8_t buffer[7];
};

static void indirect_func(void *p)
{
    struct indirect *i = reinterpret_cast<struct indirect *>(p);
    (*i->touched)++;
}

struct timing {
    unsigned tick;
    unsigned delay;
};

static void timing_func(void *p)
{
    struct timing *timing = reinterpret_cast<struct timing *>(p);
    unsigned tick = equeue_tick();

    unsigned t1 = timing->delay;
    unsigned t2 = tick - timing->tick;
    TEST_ASSERT_UINT_WITHIN(10, t2, t1);

    timing->tick = tick;
}

struct fragment {
    equeue_t *q;
    size_t size;
    struct timing timing;
};

static void fragment_func(void *p)
{
    struct fragment *fragment = reinterpret_cast<struct fragment *>(p);
    timing_func(&fragment->timing);

    struct fragment *nfragment = reinterpret_cast<struct fragment *>(equeue_alloc(fragment->q, fragment->size));
    TEST_ASSERT_NOT_NULL(nfragment);

    *nfragment = *fragment;
    equeue_event_delay(nfragment, fragment->timing.delay);

    int id = equeue_post(nfragment->q, fragment_func, nfragment);
    TEST_ASSERT_NOT_EQUAL(0, id);
}

struct cancel {
    equeue_t *q;
    int id;
};

static void cancel_func(void *p)
{
    struct cancel *ccel = reinterpret_cast<struct cancel *>(p);
    equeue_cancel(ccel->q, ccel->id);
}

struct nest {
    equeue_t *q;
    void (*cb)(void *);
    void *data;
};

static void nest_func(void *p)
{
    struct nest *nst = reinterpret_cast<struct nest *>(p);
    equeue_call(nst->q, nst->cb, nst->data);

    ThisThread::sleep_for(10);
}

static void multithread_thread(equeue_t *p)
{
    equeue_dispatch(p, DISPATCH_INFINITE);
}

static void background_func(void *p, int ms)
{
    *(reinterpret_cast<int *>(p)) = ms;
}

struct ethread {
    equeue_t *q;
    int ms;
};

static void ethread_dispatch(void *p)
{
    struct ethread *t = reinterpret_cast<struct ethread *>(p);
    equeue_dispatch(t->q, t->ms);
}

struct count_and_queue {
    int p;
    equeue_t *q;
};

static void simple_breaker(void *p)
{
    struct count_and_queue *caq = reinterpret_cast<struct count_and_queue *>(p);
    equeue_break(caq->q);
    ThisThread::sleep_for(10);
    caq->p++;
}

// Simple call tests

/** Test that equeue executes function passed by equeue_call.
 *
 *  Given queue is initialized.
 *  When the event is scheduled and after that equeue_dispatch is called.
 *  Then function passed by equeue_call is executed properly.
 */
static void test_equeue_simple_call()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    equeue_call(&q, simple_func, &touched);
    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_dispatch(&q, 10);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_destroy(&q);
}

/** Test that equeue executes function passed by equeue_call_in.
 *
 *  Given queue is initialized.
 *  When the event is scheduled and after that equeue_dispatch is called.
 *  Then function passed by equeue_call_in is executed properly.
 */
static void test_equeue_simple_call_in()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    int id = equeue_call_in(&q, 10, simple_func, &touched);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 15);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_dispatch(&q, 10);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_destroy(&q);
}

/** Test that equeue executes function passed by equeue_call_every.
 *
 *  Given queue is initialized.
 *  When the event is scheduled and after that equeue_dispatch is called.
 *  Then function passed by equeue_call_every is executed properly.
 */
static void test_equeue_simple_call_every()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    int id = equeue_call_every(&q, 10, simple_func, &touched);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 15);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_destroy(&q);
}

/** Test that equeue executes function passed by equeue_post.
 *
 *  Given queue is initialized.
 *  When the event is posted and after that equeue_dispatch is called.
 *  Then function passed by equeue_post is executed properly.
 */
static void test_equeue_simple_post()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    struct indirect *i = reinterpret_cast<struct indirect *>(equeue_alloc(&q, sizeof(struct indirect)));
    TEST_ASSERT_NOT_NULL(i);

    i->touched = &touched;
    int id = equeue_post(&q, indirect_func, i);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(1, *i->touched);

    equeue_destroy(&q);
}


// Misc tests

/** Test that equeue executes events attached to its events destructors by equeue_event_dtor.
 *
 *  Given queue is initialized.
 *  When equeue events are being destroyed by equeue_dispatch, equeue_cancel, or equeue_destroy.
 *  Then functions attached to equeue events destructors are executed properly.
 */
static void test_equeue_destructor()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    struct indirect *e;
    int ids[3];

    for (int i = 0; i < 3; i++) {
        e = reinterpret_cast<struct indirect *>(equeue_alloc(&q, sizeof(struct indirect)));
        TEST_ASSERT_NOT_NULL(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        int id = equeue_post(&q, pass_func, e);
        TEST_ASSERT_NOT_EQUAL(0, id);
    }

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(3, touched);

    touched = 0;
    for (int i = 0; i < 3; i++) {
        e = reinterpret_cast<struct indirect *>(equeue_alloc(&q, sizeof(struct indirect)));
        TEST_ASSERT_NOT_NULL(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        ids[i] = equeue_post(&q, pass_func, e);
        TEST_ASSERT_NOT_EQUAL(0, ids[i]);
    }

    for (int i = 0; i < 3; i++) {
        equeue_cancel(&q, ids[i]);
    }
    TEST_ASSERT_EQUAL_UINT8(3, touched);

    equeue_dispatch(&q, 0);

    touched = 0;
    for (int i = 0; i < 3; i++) {
        e = reinterpret_cast<struct indirect *>(equeue_alloc(&q, sizeof(struct indirect)));
        TEST_ASSERT_NOT_NULL(e);

        e->touched = &touched;
        equeue_event_dtor(e, indirect_func);
        int id = equeue_post(&q, pass_func, e);
        TEST_ASSERT_NOT_EQUAL(0, id);
    }

    equeue_destroy(&q);
    TEST_ASSERT_EQUAL_UINT8(3, touched);
}

/** Test that equeue_alloc returns 0 when equeue can not be allocated.
 *
 *  Given queue is initialized.
 *  When equeue_alloc is called and equeue can not be allocated
 *  Then function equeue_alloc returns NULL.
 */
static void test_equeue_allocation_failure()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    void *p = equeue_alloc(&q, 2 * TEST_EQUEUE_SIZE);
    TEST_ASSERT_NULL(p);

    for (int i = 0; i < 100; i++) {
        p = equeue_alloc(&q, 0);
    }
    TEST_ASSERT_NULL(p);

    equeue_destroy(&q);
}

/** Test that equeue does not execute evenets that has been canceled.
 *
 *  Given queue is initialized.
 *  When events are canceled by equeue_cancel.
 *  Then they are not executed by calling equeue_dispatch.
 */
template <int N>
static void test_equeue_cancel()
{
    equeue_t q;
    int err = equeue_create(&q, (N * EVENTS_EVENT_SIZE));
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = equeue_call(&q, simple_func, &touched);
        TEST_ASSERT_NOT_EQUAL(0, ids[i]);
    }

    for (int i = N - 1; i >= 0; i--) {
        equeue_cancel(&q, ids[i]);
    }

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT(0, touched);

    equeue_destroy(&q);
}

/** Test that events can be cancelled by function executed by equeue_dispatch.
 *
 *  Given queue is initialized.
 *  When event is cancelled by another event while dispatching.
 *  Then event that was cancelled is not being executed.
 */
static void test_equeue_cancel_inflight()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;

    int id = equeue_call(&q, simple_func, &touched);
    equeue_cancel(&q, id);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(0, touched);

    id = equeue_call(&q, simple_func, &touched);
    equeue_cancel(&q, id);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(0, touched);

    struct cancel *ccel = reinterpret_cast<struct cancel *>(equeue_alloc(&q, sizeof(struct cancel)));
    TEST_ASSERT_NOT_NULL(ccel);
    ccel->q = &q;
    ccel->id = 0;

    id = equeue_post(&q, cancel_func, ccel);
    TEST_ASSERT_NOT_EQUAL(0, id);

    ccel->id = equeue_call(&q, simple_func, &touched);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(0, touched);

    equeue_destroy(&q);
}

/** Test that unnecessary canceling events would not affect executing other events.
 *
 *  Given queue is initialized.
 *  When event is unnecessary canceled by equeue_cancel.
 *  Then other events are properly executed after calling equeue_dispatch.
 */
static void test_equeue_cancel_unnecessarily()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    int id = equeue_call(&q, pass_func, 0);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    id = equeue_call(&q, pass_func, 0);
    equeue_dispatch(&q, 0);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    uint8_t touched = 0;
    equeue_call(&q, simple_func, &touched);
    for (int i = 0; i < 5; i++) {
        equeue_cancel(&q, id);
    }

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_destroy(&q);
}

/** Test that dispatching events that have 0 ms period time would not end up in infinite loop.
 *
 *  Given queue is initialized.
 *  When events have 0 ms period time.
 *  Then dispatching would not end up in infinite loop.
 */
static void test_equeue_loop_protect()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched1 = 0;
    equeue_call_every(&q, 0, simple_func, &touched1);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);

    touched1 = 0;
    uint8_t touched2 = 0;
    equeue_call_every(&q, 1, simple_func, &touched2);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(0, touched2);

    equeue_destroy(&q);
}

/** Test that equeue_break breaks event queue out of dispatching.
 *
 *  Given queue is initialized.
 *  When equeue_break is called.
 *  Then event queue will stop dispatching after finisching current dispatching cycle.
 */
static void test_equeue_break()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched1 = 0;
    equeue_call_every(&q, 0, simple_func, &touched1);

    uint8_t touched2 = 0;
    equeue_call_every(&q, 5, simple_func, &touched2);

    equeue_break(&q);
    equeue_dispatch(&q, DISPATCH_INFINITE);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(0, touched2);

    equeue_destroy(&q);
}

/** Test that equeue_break function breaks equeue dispatching only once.
 *
 *  Given queue is initialized.
 *  When equeue_break is called several times.
 *  Then equeue is stopped only once.
 */
static void test_equeue_break_no_windup()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    equeue_call_every(&q, 0, simple_func, &touched);

    equeue_break(&q);
    equeue_break(&q);
    equeue_dispatch(&q, DISPATCH_INFINITE);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    touched = 0;
    equeue_dispatch(&q, 55);
    TEST_ASSERT(touched > 1);

    equeue_destroy(&q);
}

/** Test that function passed by equeue_call_every is being executed periodically.
 *
 *  Given queue is initialized.
 *  When function is passed by equeue_call_every with specified period.
 *  Then event is executed (dispatch time/period) times.
 */
static void test_equeue_period()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    equeue_call_every(&q, 10, simple_func, &touched);

    equeue_dispatch(&q, 55);
    TEST_ASSERT_EQUAL_UINT8(5, touched);

    equeue_destroy(&q);
}

/** Test that function added to the equeue by other function which already is in equeue executes in the next dispatch, or after the end of execution of the "mother" event.
 *
 *  Given queue is initialized.
 *  When nested function is added to enqueue.
 *  Then it is executed in the next dispatch, or after execution of "mother" function.
 */
static void test_equeue_nested()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    struct nest *nst = reinterpret_cast<struct nest *>(equeue_alloc(&q, sizeof(struct nest)));
    TEST_ASSERT_NOT_NULL(nst);
    nst->q = &q;
    nst->cb = simple_func;
    nst->data = &touched;

    int id = equeue_post(&q, nest_func, nst);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 5);
    TEST_ASSERT_EQUAL_UINT8(0, touched);

    equeue_dispatch(&q, 1);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    touched = 0;
    nst = reinterpret_cast<struct nest *>(equeue_alloc(&q, sizeof(struct nest)));
    TEST_ASSERT_NOT_NULL(nst);
    nst->q = &q;
    nst->cb = simple_func;
    nst->data = &touched;

    id = equeue_post(&q, nest_func, nst);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 20);
    TEST_ASSERT_EQUAL_UINT8(1, touched);

    equeue_destroy(&q);
}

/** Test that functions scheduled after slow function would execute according to the schedule if it is possible, if not they would execute right after sloth function.
 *
 *  Given queue is initialized.
 *  When sloth function is being called before other functions.
 *  Then if it is possible all functions start according to predefined schedule correctly.
 */
static void test_equeue_sloth()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched1 = 0;
    uint8_t touched2 = 0;
    uint8_t touched3 = 0;
    int id = equeue_call(&q, sloth_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id);

    id = equeue_call_in(&q, 5, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id);

    id = equeue_call_in(&q, 15, simple_func, &touched3);
    TEST_ASSERT_NOT_EQUAL(0, id);

    equeue_dispatch(&q, 20);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(1, touched2);
    TEST_ASSERT_EQUAL_UINT8(1, touched3);

    equeue_destroy(&q);
}

/** Test that equeue can be broken of dispatching from a different thread.
 *
 *  Given queue is initialized.
 *  When equeue starts dispatching in one thread.
 *  Then it can be stopped from another thread via equeue_break.
 */
static void test_equeue_multithread()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    equeue_call_every(&q, 1, simple_func, &touched);

    Thread t1(osPriorityNormal, TEST_THREAD_STACK_SIZE);
    t1.start(callback(multithread_thread, &q));
    ThisThread::sleep_for(10);
    equeue_break(&q);
    err = t1.join();
    TEST_ASSERT_EQUAL_INT(0, err);

    TEST_ASSERT(touched > 1);

    equeue_destroy(&q);
}

/** Test that variable referred via equeue_background shows value in ms to the next event.
 *
 *  Given queue is initialized.
 *  When variable is referred via equeue_background.
 *  Then it depicts the time in ms to the next event.
 */
static void test_equeue_background()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    int id = equeue_call_in(&q, 20, pass_func, 0);
    TEST_ASSERT_NOT_EQUAL(0, id);

    int ms;
    equeue_background(&q, background_func, &ms);
    TEST_ASSERT_EQUAL_INT(20, ms);

    id = equeue_call_in(&q, 10, pass_func, 0);
    TEST_ASSERT_NOT_EQUAL(0, id);
    TEST_ASSERT_EQUAL_INT(10, ms);

    id = equeue_call(&q, pass_func, 0);
    TEST_ASSERT_NOT_EQUAL(0, id);
    TEST_ASSERT_EQUAL_INT(0, ms);

    equeue_dispatch(&q, 0);
    TEST_ASSERT_EQUAL_INT(10, ms);

    equeue_destroy(&q);
    TEST_ASSERT_EQUAL_INT(-1, ms);
}

/** Test that when chaining two equeues, events from both equeues execute by calling dispatch only on target.
 *
 *  Given queue is initialized.
 *  When target chained equeue is dispatched.
 *  Then events from both chained equeues are executed.
 */
static void test_equeue_chain()
{
    equeue_t q1;
    int err = equeue_create(&q1, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    equeue_t q2;
    err = equeue_create(&q2, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    equeue_chain(&q2, &q1);

    uint8_t touched1 = 0;
    uint8_t touched2 = 0;

    int id1 = equeue_call_in(&q1, 20, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    int id2 = equeue_call_in(&q2, 20, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    id1 = equeue_call(&q1, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    id2 = equeue_call(&q2, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    id1 = equeue_call_in(&q1, 5, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    id2 = equeue_call_in(&q2, 5, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    equeue_cancel(&q1, id1);
    equeue_cancel(&q2, id2);

    id1 = equeue_call_in(&q1, 10, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    id2 = equeue_call_in(&q2, 10, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    equeue_dispatch(&q1, 30);

    TEST_ASSERT_EQUAL_UINT8(3, touched1);
    TEST_ASSERT_EQUAL_UINT8(3, touched2);

    equeue_destroy(&q2);
    equeue_destroy(&q1);
}

/** Test that unchaining equeues makes them work on their own.
 *
 *  Given queue is initialized.
 *  When equeue is unchained.
 *  Then it can be only dispatched by calling with reference to it.
 */
static void test_equeue_unchain()
{
    equeue_t q1;
    int err = equeue_create(&q1, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    equeue_t q2;
    err = equeue_create(&q2, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    equeue_chain(&q2, &q1);

    uint8_t touched1 = 0;
    uint8_t touched2 = 0;
    int id1 = equeue_call(&q1, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    int id2 = equeue_call(&q2, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    equeue_dispatch(&q1, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(1, touched2);

    equeue_chain(&q2, 0);

    touched1 = 0;
    touched2 = 0;

    id1 = equeue_call(&q1, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    id2 = equeue_call(&q2, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    equeue_dispatch(&q1, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(0, touched2);

    equeue_dispatch(&q2, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(1, touched2);

    equeue_chain(&q1, &q2);

    touched1 = 0;
    touched2 = 0;

    id1 = equeue_call(&q1, simple_func, &touched1);
    TEST_ASSERT_NOT_EQUAL(0, id1);
    id2 = equeue_call(&q2, simple_func, &touched2);
    TEST_ASSERT_NOT_EQUAL(0, id2);

    equeue_dispatch(&q2, 0);
    TEST_ASSERT_EQUAL_UINT8(1, touched1);
    TEST_ASSERT_EQUAL_UINT8(1, touched2);

    equeue_destroy(&q1);
    equeue_destroy(&q2);
}

// Barrage tests

/** Test that equeue keeps good time at starting events.
 *
 *  Given queue is initialized.
 *  When equeue is being dispatched.
 *  Then events happen according to the schedule with an error within a specified range.
 */
template<int N>
static void test_equeue_simple_barrage()
{
    equeue_t q;
    int err = equeue_create(&q, N * (EQUEUE_EVENT_SIZE + sizeof(struct timing)));
    TEST_ASSERT_EQUAL_INT(0, err);

    for (int i = 0; i < N; i++) {
        struct timing *timing = reinterpret_cast<struct timing *>(equeue_alloc(&q, sizeof(struct timing)));
        TEST_ASSERT_NOT_NULL(timing);

        timing->tick = equeue_tick();
        timing->delay = (i + 1) * 100;
        equeue_event_delay(timing, timing->delay);
        equeue_event_period(timing, timing->delay);

        int id = equeue_post(&q, timing_func, timing);
        TEST_ASSERT_NOT_EQUAL(0, id);
    }

    equeue_dispatch(&q, N * 100);

    equeue_destroy(&q);
}

/** Test that equeue keeps good time at starting events when events are added via functions already placed in equeue.
 *
 *  Given queue is initialized.
 *  When equeue is being dispatched and new events are added via already placed in equeue.
 *  Then events happen according to the schedule with an error within a specified range.
 */
template<int N>
static void test_equeue_fragmenting_barrage()
{
    equeue_t q;
    int err = equeue_create(&q,
                            2 * N * (EQUEUE_EVENT_SIZE + sizeof(struct fragment) + N * sizeof(int)));
    TEST_ASSERT_EQUAL_INT(0, err);

    for (int i = 0; i < N; i++) {
        size_t size = sizeof(struct fragment) + i * sizeof(int);
        struct fragment *fragment = reinterpret_cast<struct fragment *>(equeue_alloc(&q, size));
        TEST_ASSERT_NOT_NULL(fragment);

        fragment->q = &q;
        fragment->size = size;
        fragment->timing.tick = equeue_tick();
        fragment->timing.delay = (i + 1) * 100;
        equeue_event_delay(fragment, fragment->timing.delay);

        int id = equeue_post(&q, fragment_func, fragment);
        TEST_ASSERT_NOT_EQUAL(0, id);
    }

    equeue_dispatch(&q, N * 100);

    equeue_destroy(&q);
}

/** Test that equeue keeps good time at starting events even if it is working on different thread.
 *
 *  Given queue is initialized.
 *  When equeue is being dispatched on different thread.
 *  Then events happen according to the schedule with an error within a specified range.
 */
template<int N>
static void test_equeue_multithreaded_barrage()
{
    equeue_t q;
    int err = equeue_create(&q, N * (EQUEUE_EVENT_SIZE + sizeof(struct timing)));
    TEST_ASSERT_EQUAL_INT(0, err);

    struct ethread t;
    t.q = &q;
    t.ms = N * 100;

    Thread t1(osPriorityNormal, TEST_THREAD_STACK_SIZE);

    t1.start(callback(ethread_dispatch, &t));

    for (int i = 0; i < N; i++) {
        struct timing *timing = reinterpret_cast<struct timing *>(equeue_alloc(&q, sizeof(struct timing)));
        TEST_ASSERT_NOT_NULL(timing);

        timing->tick = equeue_tick();
        timing->delay = (i + 1) * 100;
        equeue_event_delay(timing, timing->delay);
        equeue_event_period(timing, timing->delay);

        int id = equeue_post(&q, timing_func, timing);
        TEST_ASSERT_NOT_EQUAL(0, id);
    }

    err = t1.join();
    TEST_ASSERT_EQUAL_INT(0, err);

    equeue_destroy(&q);
}

/** Test that break request flag is cleared when equeue stops dispatching timeouts.
 *
 *  Given queue is initialized.
 *  When equeue break request flag is called but equeue stops dispatching because of timeout.
 *  Then next equeue dispatch is not stopped.
 */
static void test_equeue_break_request_cleared_on_timeout()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    struct count_and_queue pq;
    pq.p = 0;
    pq.q = &q;

    int id = equeue_call_in(&q, 1, simple_breaker, &pq);

    equeue_dispatch(&q, 10);

    TEST_ASSERT_EQUAL_INT(1, pq.p);

    equeue_cancel(&q, id);

    uint8_t touched = 0;
    equeue_call_every(&q, 10, simple_func, &touched);

    equeue_dispatch(&q, 55);
    TEST_ASSERT_EQUAL_UINT8(5, touched);

    equeue_destroy(&q);
}

/** Test that siblings events don't have next pointers.
 *
 *  Given queue is initialized.
 *  When events are scheduled on the same time.
 *  Then they are connected via sibling pointers and siblings have their next pointer pointing to NULL.
 */
static void test_equeue_sibling()
{
    equeue_t q;
    int err = equeue_create(&q, TEST_EQUEUE_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    int id0 = equeue_call_in(&q, 1, pass_func, 0);
    int id1 = equeue_call_in(&q, 1, pass_func, 0);
    int id2 = equeue_call_in(&q, 1, pass_func, 0);

    struct equeue_event *e = q.queue;

    for (; e; e = e->next) {
        for (struct equeue_event *s = e->sibling; s; s = s->sibling) {
            TEST_ASSERT_NULL(s->next);
        }
    }
    equeue_cancel(&q, id0);
    equeue_cancel(&q, id1);
    equeue_cancel(&q, id2);
    equeue_destroy(&q);
}

struct user_allocated_event {
    struct equeue_event e;
    uint8_t touched;
};

/** Test that equeue executes user allocated events passed by equeue_post.
 *
 *  Given queue is initialized and its size is set to store one event at max in its internal memory.
 *  When post events allocated in queues internal memory (what is done by calling equeue_call).
 *  Then only one event can be posted due to queue memory size.
 *  When post user allocated events.
 *  Then number of posted events is not limited by queue memory size.
 *  When both queue allocaded and user allocated events are posted and equeue_dispatch is called.
 *  Then both types of events are executed properly.
 */
static void test_equeue_user_allocated_event_post()
{
    equeue_t q;
    int err = equeue_create(&q, EQUEUE_EVENT_SIZE);
    TEST_ASSERT_EQUAL_INT(0, err);

    uint8_t touched = 0;
    user_allocated_event e1 = { { 0, 0, 0, NULL, NULL, NULL, 0, -1, NULL, NULL }, 0 };
    user_allocated_event e2 = { { 0, 0, 0, NULL, NULL, NULL, 10,  10, NULL, NULL }, 0 };
    user_allocated_event e3 = { { 0, 0, 0, NULL, NULL, NULL, 10,  10, NULL, NULL }, 0 };
    user_allocated_event e4 = { { 0, 0, 0, NULL, NULL, NULL, 10,  10, NULL, NULL }, 0 };
    user_allocated_event e5 = { { 0, 0, 0, NULL, NULL, NULL, 0, -1, NULL, NULL }, 0 };

    TEST_ASSERT_NOT_EQUAL(0, equeue_call_every(&q, 10, simple_func, &touched));
    TEST_ASSERT_EQUAL_INT(0, equeue_call_every(&q, 10, simple_func, &touched));
    TEST_ASSERT_EQUAL_INT(0, equeue_call_every(&q, 10, simple_func, &touched));

    equeue_post_user_allocated(&q, simple_func, &e1.e);
    equeue_post_user_allocated(&q, simple_func, &e2.e);
    equeue_post_user_allocated(&q, simple_func, &e3.e);
    equeue_post_user_allocated(&q, simple_func, &e4.e);
    equeue_post_user_allocated(&q, simple_func, &e5.e);
    equeue_cancel_user_allocated(&q, &e3.e);
    equeue_cancel_user_allocated(&q, &e3.e);

    equeue_dispatch(&q, 11);

    TEST_ASSERT_EQUAL_UINT8(1, touched);
    TEST_ASSERT_EQUAL_UINT8(1, e1.touched);
    TEST_ASSERT_EQUAL_UINT8(1, e2.touched);
    TEST_ASSERT_EQUAL_UINT8(0, e3.touched);
    TEST_ASSERT_EQUAL_UINT8(1, e4.touched);
    TEST_ASSERT_EQUAL_UINT8(1, e5.touched);

    e3.e.target = 10; // set target as it's modified by equeue_call
    e3.e.period = 10; // set period as it's reset by equeue_cancel
    equeue_post_user_allocated(&q, simple_func, &e3.e);
    equeue_dispatch(&q, 101);

    TEST_ASSERT_EQUAL_UINT8(11, touched);
    TEST_ASSERT_EQUAL_UINT8(1, e1.touched);
    TEST_ASSERT_EQUAL_UINT8(11, e2.touched);
    TEST_ASSERT_EQUAL_UINT8(10, e3.touched);
    TEST_ASSERT_EQUAL_UINT8(11, e4.touched);
    TEST_ASSERT_EQUAL_UINT8(1, e5.touched);

    equeue_destroy(&q);
}

Case cases[] = {
    Case("simple call test", test_equeue_simple_call),
    Case("simple call in test", test_equeue_simple_call_in),
    Case("simple call every test", test_equeue_simple_call_every),
    Case("simple post test", test_equeue_simple_post),

    Case("destructor test", test_equeue_destructor),
    Case("allocation failure test", test_equeue_allocation_failure),
    Case("cancel test", test_equeue_cancel<20>),
    Case("cancel inflight test", test_equeue_cancel_inflight),
    Case("cancel unnecessarily test", test_equeue_cancel_unnecessarily),
    Case("loop protect test", test_equeue_loop_protect),
    Case("break test", test_equeue_break),
    Case("break no windup test", test_equeue_break_no_windup),
    Case("period test", test_equeue_period),
    Case("nested test", test_equeue_nested),
    Case("sloth test", test_equeue_sloth),

    Case("multithread test", test_equeue_multithread),

    Case("background test", test_equeue_background),
    Case("chain test", test_equeue_chain),
    Case("unchain test", test_equeue_unchain),

    Case("simple barrage test", test_equeue_simple_barrage<20>),
    Case("fragmenting barrage test", test_equeue_fragmenting_barrage<10>),
    Case("multithreaded barrage test", test_equeue_multithreaded_barrage<10>),
    Case("break request cleared on timeout test", test_equeue_break_request_cleared_on_timeout),
    Case("sibling test", test_equeue_sibling),
    Case("user allocated event test", test_equeue_user_allocated_event_post)

};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // MBED_EXTENDED_TESTS
