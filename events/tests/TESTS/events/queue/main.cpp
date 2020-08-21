/* mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
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
#include "mbed_events.h"
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] test not supported
#else

using namespace utest::v1;

// Assume that tolerance is 5% of measured time.
#define DELTA(ms) (ms / 20)

// TEST_EQUEUE_SIZE was reduced below 1024B to fit this test to devices with small RAM (RAM <= 16kB)
// additionally TEST_EQUEUE_SIZE was expressed in EVENTS_EVENT_SIZE to increase readability
// (for more details about EVENTS_EVENT_SIZE see EventQueue constructor)
#define TEST_EQUEUE_SIZE (18*EVENTS_EVENT_SIZE)

// By empirical, we take 80MHz CPU/2ms delay as base tolerance for time left test.
// For higher CPU frequency, tolerance is fixed to 2ms.
// For lower CPU frequency, tolerance is inversely proportional to CPU frequency.
// E.g.:
// 100MHz: 2ms
// 80MHz: 2ms
// 64MHz: 3ms
// 48MHz: 4ms
#define ALLOWED_TIME_LEFT_TOLERANCE_MS   ((SystemCoreClock >= 80000000) ? 2 : ((80000000 * 2 + SystemCoreClock - 1) / SystemCoreClock))

// flag for called
volatile bool touched = false;

// static functions
void func5(int a0, int a1, int a2, int a3, int a4)
{
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2 | a3 | a4, 0x1f);
}

void func4(int a0, int a1, int a2, int a3)
{
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2 | a3, 0xf);
}

void func3(int a0, int a1, int a2)
{
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2, 0x7);
}

void func2(int a0, int a1)
{
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1, 0x3);
}

void func1(int a0)
{
    touched = true;
    TEST_ASSERT_EQUAL(a0, 0x1);
}

void func0()
{
    touched = true;
}

#define SIMPLE_POSTS_TEST(i, ...)                           \
void simple_posts_test##i() {                               \
    EventQueue queue(TEST_EQUEUE_SIZE);                     \
                                                            \
    touched = false;                                        \
    queue.call(func##i,##__VA_ARGS__);                      \
    queue.dispatch(0);                                      \
    TEST_ASSERT(touched);                                   \
                                                            \
    touched = false;                                        \
    queue.call_in(1, func##i,##__VA_ARGS__);                \
    queue.dispatch(2);                                      \
    TEST_ASSERT(touched);                                   \
                                                            \
    touched = false;                                        \
    queue.call_every(1, func##i,##__VA_ARGS__);             \
    queue.dispatch(2);                                      \
    TEST_ASSERT(touched);                                   \
}

SIMPLE_POSTS_TEST(5, 0x01, 0x02, 0x04, 0x08, 0x010)
SIMPLE_POSTS_TEST(4, 0x01, 0x02, 0x04, 0x08)
SIMPLE_POSTS_TEST(3, 0x01, 0x02, 0x04)
SIMPLE_POSTS_TEST(2, 0x01, 0x02)
SIMPLE_POSTS_TEST(1, 0x01)
SIMPLE_POSTS_TEST(0)


void time_func(Timer *t, int ms)
{
    TEST_ASSERT_INT_WITHIN(DELTA(ms), ms, t->read_ms());
    t->reset();
}

template <int N>
void call_in_test()
{
    Timer tickers[N];

    EventQueue queue(TEST_EQUEUE_SIZE);

    for (int i = 0; i < N; i++) {
        tickers[i].start();
        queue.call_in((i + 1) * 100, time_func, &tickers[i], (i + 1) * 100);
    }

    queue.dispatch(N * 100);
}

template <int N>
void call_every_test()
{
    Timer tickers[N];

    EventQueue queue(TEST_EQUEUE_SIZE);

    for (int i = 0; i < N; i++) {
        tickers[i].start();
        queue.call_every((i + 1) * 100, time_func, &tickers[i], (i + 1) * 100);
    }

    queue.dispatch(N * 100);
}

void allocate_failure_test()
{
    EventQueue queue(TEST_EQUEUE_SIZE);
    int id;

    for (int i = 0; i < 100; i++) {
        id = queue.call((void (*)())0);
    }

    TEST_ASSERT(!id);
}

void no()
{
    TEST_ASSERT(false);
}

template <int N>
void cancel_test1()
{
    EventQueue queue(TEST_EQUEUE_SIZE);

    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = queue.call_in(1000, no);
    }

    for (int i = N - 1; i >= 0; i--) {
        queue.cancel(ids[i]);
    }

    queue.dispatch(0);
}


// Testing the dynamic arguments to the event class
unsigned counter = 0;

void count5(unsigned a0, unsigned a1, unsigned a2, unsigned a3, unsigned a5)
{
    counter += a0 + a1 + a2 + a3 + a5;
}

void count4(unsigned a0, unsigned a1, unsigned a2, unsigned a3)
{
    counter += a0 + a1 + a2 + a3;
}

void count3(unsigned a0, unsigned a1, unsigned a2)
{
    counter += a0 + a1 + a2;
}

void count2(unsigned a0, unsigned a1)
{
    counter += a0 + a1;
}

void count1(unsigned a0)
{
    counter += a0;
}

void count0()
{
    counter += 0;
}

void event_class_test()
{
    counter = 0;
    EventQueue queue(TEST_EQUEUE_SIZE);

    Event<void(int, int, int, int, int)> e5(&queue, count5);
    Event<void(int, int, int, int)> e4(&queue, count5, 1);
    Event<void(int, int, int)> e3(&queue, count5, 1, 1);
    Event<void(int, int)> e2(&queue, count5, 1, 1, 1);
    Event<void(int)> e1(&queue, count5, 1, 1, 1, 1);
    Event<void()> e0(&queue, count5, 1, 1, 1, 1, 1);

    e5.post(1, 1, 1, 1, 1);
    e4.post(1, 1, 1, 1);
    e3.post(1, 1, 1);
    e2.post(1, 1);
    e1.post(1);
    e0.post();

    queue.dispatch(0);

    TEST_ASSERT_EQUAL(counter, 30);
}

void event_class_helper_test()
{
    counter = 0;
    EventQueue queue(TEST_EQUEUE_SIZE);

    Event<void()> e5 = queue.event(count5, 1, 1, 1, 1, 1);
    Event<void()> e4 = queue.event(count4, 1, 1, 1, 1);
    Event<void()> e3 = queue.event(count3, 1, 1, 1);
    Event<void()> e2 = queue.event(count2, 1, 1);
    Event<void()> e1 = queue.event(count1, 1);
    Event<void()> e0 = queue.event(count0);

    e5.post();
    e4.post();
    e3.post();
    e2.post();
    e1.post();
    e0.post();

    queue.dispatch(0);

    TEST_ASSERT_EQUAL(counter, 15);
}

void event_inference_test()
{
    counter = 0;
    EventQueue queue(TEST_EQUEUE_SIZE);

    queue.event(count5, 1, 1, 1, 1, 1).post();
    queue.event(count5, 1, 1, 1, 1).post(1);
    queue.event(count5, 1, 1, 1).post(1, 1);
    queue.event(count5, 1, 1).post(1, 1, 1);
    queue.event(count5, 1).post(1, 1, 1, 1);
    queue.event(count5).post(1, 1, 1, 1, 1);

    queue.event(callback(count5), 1, 1, 1, 1, 1).post();
    queue.event(callback(count5), 1, 1, 1, 1).post(1);
    queue.event(callback(count5), 1, 1, 1).post(1, 1);
    queue.event(callback(count5), 1, 1).post(1, 1, 1);
    queue.event(callback(count5), 1).post(1, 1, 1, 1);
    queue.event(callback(count5)).post(1, 1, 1, 1, 1);

    queue.dispatch(0);

    TEST_ASSERT_EQUAL(counter, 60);
}

int timeleft_events[2];

void check_time_left(EventQueue *queue, int index, int expected)
{
    const int event_id = timeleft_events[index];
    TEST_ASSERT_INT_WITHIN(ALLOWED_TIME_LEFT_TOLERANCE_MS, expected, queue->time_left(event_id));
    touched = true;
}

void time_left(EventQueue *queue, int index)
{
    const int event_id = timeleft_events[index];
    TEST_ASSERT_EQUAL(0, queue->time_left(event_id));
}

void time_left_test()
{
    EventQueue queue(TEST_EQUEUE_SIZE);

    // Enque check events
    TEST_ASSERT(queue.call_in(50, check_time_left, &queue, 0, 100 - 50));
    TEST_ASSERT(queue.call_in(200, check_time_left, &queue, 1, 200 - 200));

    // Enque events to be checked
    timeleft_events[0] = queue.call_in(100, time_left, &queue, 0);
    timeleft_events[1] = queue.call_in(200, time_left, &queue, 1);
    TEST_ASSERT(timeleft_events[0]);
    TEST_ASSERT(timeleft_events[1]);

    queue.dispatch(300);

    // Ensure check was called
    TEST_ASSERT(touched);
    touched = false;

    int id = queue.call(func0);
    TEST_ASSERT(id);
    TEST_ASSERT_EQUAL(0, queue.time_left(id));
    queue.dispatch(10);

    // Test invalid event id
    TEST_ASSERT_EQUAL(-1, queue.time_left(0));
}

void f5(int a1, int a2, int a3, int a4, int a5)
{
    touched = true;
}

class EventTest {
public:
    EventTest() : counter() {}
    void f0()
    {
        counter++;
    }
    void f1(int a)
    {
        counter += a;
    }
    void f5(int a, int b, int c, int d, int e)
    {
        counter += a + b + c + d + e;
    }
    uint32_t counter;
};

/** Test that queue executes both dynamic and user allocated events.
 *
 *  Given queue is initialized and its size is set to store three Event at max in its internal memory.
 *      When post queue allocated event.
 *      Then only three event can be posted due to queue memory size.
 *      When post user allocated evens.
 *      Then number of posted events is not limited by queue memory size.
 *      When both Event and UserAllocatedEvent are posted and queue dispatch is called.
 *      Then both types of events are executed properly.
 *
 */
void mixed_dynamic_static_events_queue_test()
{
    {
        EventQueue queue(9 * EVENTS_EVENT_SIZE);

        EventTest e1_test;
        Event<void()> e1 = queue.event(&e1_test, &EventTest::f0);
        e1.delay(10);
        e1.period(10);
        int id1 =  e1.post();
        TEST_ASSERT_NOT_EQUAL(0, id1);
        EventTest e2_test;
        Event<void()> e2 = queue.event(&e2_test, &EventTest::f1, 3);
        e2.period(10);
        int id2 = e2.post();
        TEST_ASSERT_NOT_EQUAL(0, id2);
        EventTest e3_test;
        Event<void()> e3 = queue.event(&e3_test, &EventTest::f5, 1, 2, 3, 4, 5);
        e3.period(10);
        int id3 = e3.post();
        TEST_ASSERT_NOT_EQUAL(0, id3);


        auto ue0 = make_user_allocated_event(func0);
        EventTest ue1_test;
        auto ue1 = make_user_allocated_event(&ue1_test, &EventTest::f0);
        EventTest ue2_test;
        auto ue2 = make_user_allocated_event(&ue2_test, &EventTest::f1, 3);
        EventTest ue3_test;
        auto ue3 = make_user_allocated_event(&ue3_test, &EventTest::f5, 1, 2, 3, 4, 5);
        EventTest ue4_test;
        auto ue4 = make_user_allocated_event(&ue4_test, &EventTest::f5, 1, 2, 3, 4, 5);

        touched = false;

        ue0.call_on(&queue);
        TEST_ASSERT_EQUAL(false, ue0.try_call());
        ue1.call_on(&queue);
        TEST_ASSERT_EQUAL(false, ue1.try_call());
        ue2.period(10);
        ue2.call_on(&queue);
        TEST_ASSERT_EQUAL(false, ue2.try_call());
        ue3.period(10);
        ue3.delay(50);
        ue3.call_on(&queue);
        TEST_ASSERT_EQUAL(false, ue3.try_call());
        ue4.call_on(&queue);
        ue4.cancel();
        TEST_ASSERT_EQUAL(true, ue4.try_call());
        ue4.cancel();
        e2.cancel();

        queue.dispatch(101);

        TEST_ASSERT_EQUAL(true, touched);
        TEST_ASSERT_EQUAL(1, ue1_test.counter);
        TEST_ASSERT_EQUAL(33, ue2_test.counter);
        TEST_ASSERT_EQUAL(90, ue3_test.counter);
        TEST_ASSERT_EQUAL(0, ue4_test.counter);
        TEST_ASSERT_EQUAL(10, e1_test.counter);
        TEST_ASSERT_EQUAL(0, e2_test.counter);
        TEST_ASSERT_EQUAL(165, e3_test.counter);

        // user allocated event have to be canceled(removed from the queue) before destruction
        // cancel all periodic user events
        ue2.cancel();
        ue3.cancel();
    }
}


static EventQueue g_queue(0);
static auto ue0 = g_queue.make_user_allocated_event(func0);
static EventTest test1;
static auto ue1 = make_user_allocated_event(&test1, &EventTest::f0);
static EventTest test2;
static auto ue2 = g_queue.make_user_allocated_event(&test2, &EventTest::f1, 3);
static EventTest test3;
static auto ue3 = make_user_allocated_event(&test3, &EventTest::f5, 1, 2, 3, 4, 5);
static EventTest test4;
static auto ue4 = g_queue.make_user_allocated_event(&test4, &EventTest::f5, 1, 2, 3, 4, 5);


/** Test that static queue executes user allocated events.
 *
 *  Given static queue is initialized
 *      When post user allocated evens.
 *      Then UserAllocatedEvent are posted and dispatched without any error.
 */
void static_events_queue_test()
{
    // check that no dynamic event can be posted
    Event<void()> e0 = g_queue.event(func0);
    TEST_ASSERT_EQUAL(0, e0.post());

    ue0.delay(100);
    ue0.period(200);

    ue1.delay(100);
    ue1.period(200);

    ue2.delay(100);
    ue2.period(200);

    ue3.delay(100);
    ue3.period(200);

    ue4.delay(100);
    ue4.period(200);

    ue0.call();
    TEST_ASSERT_EQUAL(false, ue0.try_call());
    ue1.call_on(&g_queue);
    TEST_ASSERT_EQUAL(false, ue1.try_call());
    ue2();
    TEST_ASSERT_EQUAL(false, ue2.try_call());
    ue3.call_on(&g_queue);
    TEST_ASSERT_EQUAL(false, ue3.try_call());
    ue4.call();
    ue4.cancel();
    ue4.cancel();
    TEST_ASSERT_EQUAL(true, ue4.try_call());
    g_queue.cancel(&ue4);
    g_queue.cancel(&ue4);

    g_queue.dispatch(400);

    TEST_ASSERT_EQUAL(2, test1.counter);
    TEST_ASSERT_EQUAL(6, test2.counter);
    TEST_ASSERT_EQUAL(30, test3.counter);
    TEST_ASSERT_EQUAL(0, test4.counter);

    ue4.delay(1);
    TEST_ASSERT_EQUAL(true, ue4.try_call());
    g_queue.dispatch(1);

    TEST_ASSERT_EQUAL(2, test1.counter);
    TEST_ASSERT_EQUAL(6, test2.counter);
    TEST_ASSERT_EQUAL(30, test3.counter);
    TEST_ASSERT_EQUAL(15, test4.counter);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(20, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

const Case cases[] = {
    Case("Testing calls with 5 args", simple_posts_test5),
    Case("Testing calls with 4 args", simple_posts_test4),
    Case("Testing calls with 3 args", simple_posts_test3),
    Case("Testing calls with 2 args", simple_posts_test2),
    Case("Testing calls with 1 args", simple_posts_test1),
    Case("Testing calls with 0 args", simple_posts_test0),

    Case("Testing call_in",    call_in_test<20>),
    Case("Testing call_every", call_every_test<20>),

    Case("Testing allocate failure", allocate_failure_test),

    Case("Testing event cancel 1", cancel_test1<20>),
    Case("Testing the event class", event_class_test),
    Case("Testing the event class helpers", event_class_helper_test),
    Case("Testing the event inference", event_inference_test),

    Case("Testing time_left", time_left_test),
    Case("Testing mixed dynamic & static events queue", mixed_dynamic_static_events_queue_test),
    Case("Testing static events queue", static_events_queue_test)

};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
