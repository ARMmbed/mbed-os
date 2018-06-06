/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "rtos.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#if !DEVICE_USTICKER
  #error [NOT_SUPPORTED] test not supported
#endif

using namespace utest::v1;

// Assume that tolerance is 5% of measured time.
#define DELTA(ms) (ms / 20)

// TEST_EQUEUE_SIZE was reduced below 1024B to fit this test to devices with small RAM (RAM <= 16kB)
// additionally TEST_EQUEUE_SIZE was expressed in EVENTS_EVENT_SIZE to increase readability
// (for more details about EVENTS_EVENT_SIZE see EventQueue constructor)
#define TEST_EQUEUE_SIZE (18*EVENTS_EVENT_SIZE)

// flag for called
volatile bool touched = false;

// static functions
void func5(int a0, int a1, int a2, int a3, int a4) { 
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2 | a3 | a4, 0x1f);
}

void func4(int a0, int a1, int a2, int a3) {
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2 | a3, 0xf); 
}

void func3(int a0, int a1, int a2) {
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1 | a2, 0x7);
}

void func2(int a0, int a1) {
    touched = true;
    TEST_ASSERT_EQUAL(a0 | a1, 0x3);
}

void func1(int a0) {
    touched = true;
    TEST_ASSERT_EQUAL(a0, 0x1);
}

void func0() {
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


void time_func(Timer *t, int ms) {
    TEST_ASSERT_INT_WITHIN(DELTA(ms), ms, t->read_ms());
    t->reset();
}

template <int N>
void call_in_test() {
    Timer tickers[N];

    EventQueue queue(TEST_EQUEUE_SIZE);

    for (int i = 0; i < N; i++) {
        tickers[i].start();
        queue.call_in((i+1)*100, time_func, &tickers[i], (i+1)*100);
    }

    queue.dispatch(N*100);
}

template <int N>
void call_every_test() {
    Timer tickers[N];

    EventQueue queue(TEST_EQUEUE_SIZE);

    for (int i = 0; i < N; i++) {
        tickers[i].start();
        queue.call_every((i+1)*100, time_func, &tickers[i], (i+1)*100);
    }

    queue.dispatch(N*100);
}

void allocate_failure_test() {
    EventQueue queue(TEST_EQUEUE_SIZE);
    int id;

    for (int i = 0; i < 100; i++) {
        id = queue.call((void (*)())0);
    }

    TEST_ASSERT(!id);
}

void no() {
    TEST_ASSERT(false);
}

template <int N>
void cancel_test1() {
    EventQueue queue(TEST_EQUEUE_SIZE);

    int ids[N];

    for (int i = 0; i < N; i++) {
        ids[i] = queue.call_in(1000, no);
    }

    for (int i = N-1; i >= 0; i--) {
        queue.cancel(ids[i]);
    }

    queue.dispatch(0);
}


// Testing the dynamic arguments to the event class
unsigned counter = 0;

void count5(unsigned a0, unsigned a1, unsigned a2, unsigned a3, unsigned a5) {
    counter += a0 + a1 + a2 + a3 + a5;
}

void count4(unsigned a0, unsigned a1, unsigned a2, unsigned a3) {
    counter += a0 + a1 + a2 + a3;
}

void count3(unsigned a0, unsigned a1, unsigned a2) {
    counter += a0 + a1 + a2;
}

void count2(unsigned a0, unsigned a1) {
    counter += a0 + a1;
}

void count1(unsigned a0) {
    counter += a0;
}

void count0() {
    counter += 0;
}

void event_class_test() {
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

void event_class_helper_test() {
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

void event_inference_test() {
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

void check_time_left(EventQueue* queue, int index, int expected) {
    const int event_id = timeleft_events[index];
    TEST_ASSERT_INT_WITHIN(2, expected, queue->time_left(event_id));
    touched = true;
}

void time_left(EventQueue* queue, int index) {
    const int event_id = timeleft_events[index];
    TEST_ASSERT_EQUAL(0, queue->time_left(event_id));
}

void time_left_test() {
    EventQueue queue(TEST_EQUEUE_SIZE);

    // Enque check events
    TEST_ASSERT(queue.call_in(50, check_time_left, &queue, 0, 100-50));
    TEST_ASSERT(queue.call_in(200, check_time_left, &queue, 1, 200-200));

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

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
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
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}

