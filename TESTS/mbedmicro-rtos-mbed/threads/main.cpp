/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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
#if defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] Threads test cases require RTOS with multithread to run
#else

#if !DEVICE_USTICKER
#error [NOT_SUPPORTED] UsTicker need to be enabled for this test.
#else

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "SynchronizedIntegral.h"
#include "LockGuard.h"

#define THREAD_STACK_SIZE 512
#if defined(__CORTEX_A9) || defined(__CORTEX_M23) || defined(__CORTEX_M33) || defined(TARGET_ARM_FM) ||  defined(TARGET_CY8CKIT_062_WIFI_BT_PSA)
#define PARALLEL_THREAD_STACK_SIZE 512
#define CHILD_THREAD_STACK_SIZE 512
#else
#define PARALLEL_THREAD_STACK_SIZE 384
#define CHILD_THREAD_STACK_SIZE 384
#endif


using namespace utest::v1;

// The counter type used accross all the tests
// It is internall ysynchronized so read
typedef SynchronizedIntegral<int> counter_t;

template<osPriority P, uint32_t S>
class ParallelThread : public Thread {
public:
    ParallelThread() : Thread(P, S) { }
};

// Tasks with different functions to test on threads
void increment(counter_t *counter)
{
    (*counter)++;
}

void increment_with_yield(counter_t *counter)
{
    ThisThread::yield();
    (*counter)++;
}

void increment_with_wait(counter_t *counter)
{
    ThisThread::sleep_for(100);
    (*counter)++;
}

void increment_with_child(counter_t *counter)
{
    Thread *child = new (std::nothrow) Thread(osPriorityNormal, CHILD_THREAD_STACK_SIZE);
    char *dummy = new (std::nothrow) char[CHILD_THREAD_STACK_SIZE];
    delete[] dummy;

    // Don't fail test due to lack of memory. Call function directly instead
    if (!child || !dummy) {
        increment(counter);
        delete child;
        return;
    }
    child->start(callback(increment, counter));
    child->join();
    delete child;
}

void increment_with_murder(counter_t *counter)
{
    {
        // take ownership of the counter mutex so it prevent the child to
        // modify counter.
        LockGuard lock(counter->internal_mutex());
        Thread *child = new (std::nothrow) Thread(osPriorityNormal, CHILD_THREAD_STACK_SIZE);
        char *dummy = new (std::nothrow) char[CHILD_THREAD_STACK_SIZE];
        delete[] dummy;

        // Don't fail test due to lack of memory.
        if (!child || !dummy) {
            delete child;
            goto end;
        }
        child->start(callback(increment, counter));
        child->terminate();
        delete child;
    }

end:
    (*counter)++;
}

void self_terminate(Thread *self)
{
    self->terminate();
    // Code should not get here
    TEST_ASSERT(0);
}

// Tests that spawn tasks in different configurations

/** Template for tests: single thread, with yield, with wait, with child, with murder

    Testing single thread
    Given single thread is started
    when the thread increments the counter
    then the final value of the counter is equal to 1

    Testing single thread with yield
    Given single thread is started
    when the thread yields and then increments the counter
    then the final value of the counter is equal to 1

    Testing single thread with wait
    Given single thread is started
    when the thread waits for 100ms and then increments the counter
    then the final value of the counter is equal to 1

    Testing single thread with child
    Given single thread is started
    when the thread spawns another thread that increments the counter
    then the final value of the counter is equal to 1

    Testing serial threads with murder
    Given single thread is started
    when the parent thread is holding a lock
        and the parent thread spawns a child thread that waits for the lock before incrementing the counter
        and the parent terminates the child before releasing the lock
        and the parent increments the counter
    then the final value of the counter is equal to 1
*/
template <void (*F)(counter_t *)>
void test_single_thread()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    counter_t counter(0);
    Thread thread(osPriorityNormal, THREAD_STACK_SIZE);
    thread.start(callback(F, &counter));
    thread.join();
    TEST_ASSERT_EQUAL(counter, 1);
}

/** Template for tests: parallel threads, with yield, with wait, with child, with murder

    Testing parallel threads
    Given multiple threads are started in parallel
    when each of the threads increments the counter
    then the final value of the counter is equal to number of threads

    Testing parallel threads with yield
    Given multiple threads are started in parallel
    when each of the threads yields and then increments the counter
    then the final value of the counter is equal to number of threads

    Testing parallel threads with wait
    Given multiple threads are started in parallel
    when each of the threads waits for 100ms and then increments the counter
    then the final value of the counter is equal to number of threads

    Testing parallel threads with child
    Given multiple threads are started in parallel
    when each of the threads spawns another thread that increments the counter
    then the final value of the counter is equal to number of parallel threads

    Testing parallel threads with murder
    Given multiple threads are started in parallel
    when the parent thread is holding a lock
        and the parent thread spawns a child thread that waits for the lock before incrementing the counter
        and the parent terminates the child before releasing the lock
        and the parent increments the counter
    then the final value of the counter is equal to number of parallel threads
*/
template <int N, void (*F)(counter_t *)>
void test_parallel_threads()
{
    char *dummy = new (std::nothrow) char[PARALLEL_THREAD_STACK_SIZE * N];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    counter_t counter(0);
    ParallelThread<osPriorityNormal, PARALLEL_THREAD_STACK_SIZE> threads[N];

    for (int i = 0; i < N; i++) {
        threads[i].start(callback(F, &counter));
    }

    for (int i = 0; i < N; i++) {
        threads[i].join();
    }

    TEST_ASSERT_EQUAL(counter, N);
}

/** Template for tests: serial threads, with yield, with wait, with child, with murder

    Testing serial threads
    Given multiple threads are started serially
    when each of the threads increments the counter
    then the final value of the counter is equal to number of threads

    Testing serial threads with yield
    Given multiple threads are started serially
    when each of the threads yields and then increments the counter
    then the final value of the counter is equal to number of threads

    Testing serial threads with wait
    Given multiple threads are started serially
    when each of the threads waits for 100ms and then increments the counter
    then the final value of the counter is equal to number of threads

    Testing serial threads with child
    Given multiple threads are started serially
    when each of the threads spawns another thread that increments the counter
    then the final value of the counter is equal to number of serial threads

    Testing serial threads with murder
    Given multiple threads are started serially
    when the parent thread is holding a lock
        and the parent thread spawns a child thread that waits for the lock before incrementing the counter
        and the parent terminates the child before releasing the lock
        and the parent increments the counter
    then the final value of the counter is equal to number of serial threads
*/
template <int N, void (*F)(counter_t *)>
void test_serial_threads()
{
    counter_t counter(0);
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    for (int i = 0; i < N; i++) {
        Thread thread(osPriorityNormal, THREAD_STACK_SIZE);
        thread.start(callback(F, &counter));
        thread.join();
    }

    TEST_ASSERT_EQUAL(counter, N);
}

/** Testing thread self terminate

    Given the thread is running
    when the thread calls @a terminate on its self
    then the thread terminates execution cleanly
 */
void test_self_terminate()
{
    Thread *thread = new (std::nothrow) Thread(osPriorityNormal, THREAD_STACK_SIZE);
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;

    // Don't fail test due to lack of memory.
    if (!thread || !dummy) {
        goto end;
    }
    thread->start(callback(self_terminate, thread));
    thread->join();

end:
    delete thread;
}

void flags_wait()
{
    uint32_t flags = ThisThread::flags_wait_all(0x1);
    TEST_ASSERT_EQUAL(0x1, flags);
}

void flags_wait_tout()
{
    uint32_t flags = ThisThread::flags_wait_all_for(0x2, 50);
    TEST_ASSERT_EQUAL(0x1, flags);
}

void flags_wait_multibit()
{
    uint32_t flags = ThisThread::flags_wait_all(0x1 | 0x2);
    TEST_ASSERT_EQUAL(0x3, flags);
}

void flags_wait_multibit_any()
{
    uint32_t flags = ThisThread::flags_wait_any(0x1 | 0x2);
    TEST_ASSERT_NOT_EQUAL(0x0, flags);
}

void flags_wait_multibit_tout()
{
    uint32_t flags = ThisThread::flags_wait_all_for(0x1 | 0x2, 50);
    TEST_ASSERT_NOT_EQUAL(0x3, flags);
}

/**
    Testing thread flags: wait
    Given two threads (A & B) are started
    when thread A executes @a flags_wait_all(0x1)
        and thread B execute @a flags_set(0x1)
    then thread A exits the wait and continues execution

    Testing thread flags: timeout
    Given two threads (A & B) are started
    when thread A executes @a flags_wait_all_for(0x1 | 0x2, 50) with a timeout of 50ms
        and thread B execute @a flags_set(0x2)
    then thread A keeps waiting for correct flags until it timeouts

    Testing thread flags: multi-bit
    Given two threads (A & B) are started
    when thread A executes @a flags_wait_all(0x1 | 0x2)
        and thread B execute @a flags_set(0x1 | 0x2)
    then thread A exits the wait and continues execution

    Testing thread flags: multi-bit any
    Given two threads (A & B) are started
    when thread A executes @a flags_wait_any(0x1 | 0x2)
        and thread B execute @a flags_set(0x1)
    then thread A exits the wait and continues execution

    Testing thread flags: multi-bit timeout
    Given two threads (A & B) are started
    when thread A executes @a flags_wait_all_for(0x1, 50) with a timeout of 50ms
        and thread B execute @a flags_set(0x2)
    then thread A keeps waiting for correct flags until it timeouts
*/
template <int S, void (*F)()>
void test_thread_flags_set()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t_wait(osPriorityNormal, THREAD_STACK_SIZE);

    t_wait.start(callback(F));

    ThisThread::yield();

    Thread::State state = t_wait.get_state();
    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, state);

    int32_t res = t_wait.flags_set(S);

    t_wait.join();
}

void flags_clear()
{
    ThisThread::yield();

    int32_t sig = ThisThread::flags_clear(0x1);
    TEST_ASSERT_EQUAL(0x1, sig);

    /* Flags cleared we should get timeout */
    uint32_t flags = ThisThread::flags_wait_all_for(0x1, 0);
    TEST_ASSERT_EQUAL(0, flags);
}

/** Testing thread flags: flags clear

    Given two threads (A & B) are started
    when thread A executes @a flags_set(0x1)
        and thread B execute @a flags_clear(0x1)
        and thread B execute @a flags_wait_all_for(0x1, 0)
    then thread B @a flags_wait_all_for return should be 0 indicating no flags set
 */
void test_thread_flags_clear()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t_wait(osPriorityNormal, THREAD_STACK_SIZE);

    t_wait.start(callback(flags_clear));

    int32_t res = t_wait.flags_set(0x1);
    TEST_ASSERT_EQUAL(0x1, res);

    t_wait.join();
}

void thread_wait_flags()
{
    ThisThread::flags_wait_all(0x1);
}

void stack_info()
{
    ThisThread::flags_wait_all(0x1);

    thread_wait_flags();

    ThisThread::flags_wait_all(0x1);
}

/** Testing thread stack info

    Given the thread is running
    when a function is called from the thread context
    then the stack usage goes up
        and the reported stack size is as requested in the constructor
        and the sum of free and used stack sizes is equal to the total stack size
    when the function returns
    then the stack usage goes down
        and the reported stack size is as requested in the constructor
        and the sum of free and used stack sizes is equal to the total stack size
 */
void test_thread_stack_info()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    t.start(callback(stack_info));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(THREAD_STACK_SIZE, t.stack_size());
    TEST_ASSERT_EQUAL(THREAD_STACK_SIZE, t.free_stack() + t.used_stack());
    uint32_t last_stack = t.used_stack();

    t.flags_set(0x1);
    ThisThread::yield();

    TEST_ASSERT_EQUAL(THREAD_STACK_SIZE, t.free_stack() + t.used_stack());
    TEST_ASSERT(last_stack <= t.used_stack());
    last_stack = t.used_stack();

    t.flags_set(0x1);
    ThisThread::yield();

    TEST_ASSERT_EQUAL(THREAD_STACK_SIZE, t.free_stack() + t.used_stack());
    TEST_ASSERT(last_stack >= t.used_stack());

    t.flags_set(0x1);

    t.join();
}

/** Testing thread wait aka delay

    Given the thread is running
    when the @a wait function is called
    then the thread sleeps for given amount of time
 */
void test_thread_wait()
{
    Timer timer;
    timer.start();

    ThisThread::sleep_for(150);

    TEST_ASSERT_UINT32_WITHIN(50000, 150000, timer.read_us());
}

/** Testing thread name

    Given a thread is started with a specified name
    when the name is queried using @a get_name
    then the returned name is as set
*/
void test_thread_name()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    const char tname[] = "Amazing thread";
    Thread t(osPriorityNormal, THREAD_STACK_SIZE, NULL, tname);
    t.start(callback(thread_wait_flags));
    TEST_ASSERT_EQUAL(strcmp(tname, t.get_name()), 0);
    t.flags_set(0x1);
    t.join();
}

void test_deleted_thread()
{
}

/** Testing thread states: deleted

    Given the thread is not started
    then its state, as reported by @a get_state, is @a Deleted
    when the thread is started and finishes executing
    then its state, as reported by @a get_state, is @a Deleted
 */
void test_deleted()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);

    TEST_ASSERT_EQUAL(Thread::Deleted, t.get_state());

    t.start(callback(test_deleted_thread));

    t.join();
    TEST_ASSERT_EQUAL(Thread::Deleted, t.get_state());
}

void test_delay_thread()
{
    ThisThread::sleep_for(50);
}

/** Testing thread states: wait delay

    Given the thread is running
    when thread calls @a wait
    then its state, as reported by @a get_state, is @a WaitingDelay
 */
void test_delay()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);

    t.start(callback(test_delay_thread));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingDelay, t.get_state());

    t.join();
    TEST_ASSERT_EQUAL(Thread::Deleted, t.get_state());
}

void test_thread_flags_thread()
{
    ThisThread::flags_wait_all(0x1);
}

/** Testing thread states: wait flags

    Given the thread is running
    when thread waits for flags
    then its state, as reported by @a get_state, is @a WaitingThreadFlag
 */
void test_thread_flags()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);

    t.start(callback(test_thread_flags_thread));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingThreadFlag, t.get_state());

    t.flags_set(0x1);
}

void test_evt_flag_thread(EventFlags *evtflg)
{
    evtflg->wait_any(0x1);
}

/** Testing thread states: wait evt flag

    Given the thread is running
    when thread waits for an event flag
    then its state, as reported by @a get_state, is @a WaitingEventFlag
 */
void test_evt_flag()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    EventFlags evtflg;

    t.start(callback(test_evt_flag_thread, &evtflg));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingEventFlag, t.get_state());

    evtflg.set(0x1);
}

void test_mutex_thread(Mutex *mutex)
{
    mutex->lock();
}

/** Testing thread states: wait mutex

    Given the thread is running
    when thread waits for a mutex
    then its state, as reported by @a get_state, is @a WaitingMutex
 */
void test_mutex()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Mutex mutex;

    mutex.lock();

    t.start(callback(test_mutex_thread, &mutex));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingMutex, t.get_state());

    mutex.unlock();
}

void test_semaphore_thread(Semaphore *sem)
{
    sem->acquire();
}

/** Testing thread states: wait semaphore

    Given the thread is running
    when thread waits for a semaphore
    then its state, as reported by @a get_state, is @a WaitingSemaphore
 */
void test_semaphore()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Semaphore sem;

    t.start(callback(test_semaphore_thread, &sem));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingSemaphore, t.get_state());

    sem.release();
}

void test_msg_get_thread(Queue<int32_t, 1> *queue)
{
    queue->get();
}

/** Testing thread states: wait message get

    Given the thread is running
    when thread tries to get a message from an empty queue
    then its state, as reported by @a get_state, is @a WaitingMessageGet
 */
void test_msg_get()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Queue<int32_t, 1> queue;

    t.start(callback(test_msg_get_thread, &queue));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingMessageGet, t.get_state());

    queue.put((int32_t *)0xE1EE7);
}

void test_msg_put_thread(Queue<int32_t, 1> *queue)
{
    queue->put((int32_t *)0xDEADBEEF, osWaitForever);

}

/** Testing thread states: wait message put

    Given the thread is running
    when thread tries to put a message into a full queue
    then its state, as reported by @a get_state, is @a WaitingMessagePut
 */
void test_msg_put()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    Queue<int32_t, 1> queue;

    queue.put((int32_t *)0xE1EE7);

    t.start(callback(test_msg_put_thread, &queue));

    ThisThread::yield();

    TEST_ASSERT_EQUAL(Thread::WaitingMessagePut, t.get_state());
    queue.get();
}

/** Utility function that places some date on the stack */
void use_some_stack()
{
    volatile uint32_t stack_filler[10] = {0xDEADBEEF};
}

/** Testing thread with external stack memory

    Given external buffer is supplied as stack to a thread
    when the thread executes
    then the supplies buffer is used as a stack
 */
void test_thread_ext_stack()
{
    char stack[512];
    Thread t(osPriorityNormal, sizeof(stack), (unsigned char *)stack);

    memset(&stack, 0, sizeof(stack));
    t.start(callback(use_some_stack));
    t.join();

    /* If buffer was used as a stack it was cleared with pattern and some data were placed in it */
    for (unsigned i = 0; i < sizeof(stack); i++) {
        if (stack[i] != 0) {
            return;
        }
    }

    TEST_FAIL_MESSAGE("External stack was not used.");
}

/** Testing thread priority operations

    Given thread running with osPriorityNormal
    when new priority is set using @a set_priority
    then priority is changed and can be retrieved using @a get_priority
 */
void test_thread_prio()
{
    char *dummy = new (std::nothrow) char[THREAD_STACK_SIZE];
    delete[] dummy;
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory to run test");

    Thread t(osPriorityNormal, THREAD_STACK_SIZE);
    t.start(callback(thread_wait_flags));

    TEST_ASSERT_EQUAL(osPriorityNormal, t.get_priority());

    t.set_priority(osPriorityHigh);

    TEST_ASSERT_EQUAL(osPriorityHigh, t.get_priority());

    t.flags_set(0x1);
    t.join();
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(25, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

#define DEFAULT_HANDLERS NULL,NULL,greentea_case_setup_handler,greentea_case_teardown_handler,greentea_case_failure_abort_handler

// Test cases. It's spelled out rather than constructed with macro because
// macros don't play nicely with the templates (extra comma).
static const case_t cases[] = {
    {"Testing single thread", test_single_thread<increment>, DEFAULT_HANDLERS},
    {"Testing parallel threads", test_parallel_threads<3, increment>, DEFAULT_HANDLERS},
    {"Testing serial threads", test_serial_threads<10, increment>, DEFAULT_HANDLERS},

    {"Testing single thread with yield", test_single_thread<increment_with_yield>, DEFAULT_HANDLERS},
    {"Testing parallel threads with yield", test_parallel_threads<3, increment_with_yield>, DEFAULT_HANDLERS},
    {"Testing serial threads with yield", test_serial_threads<10, increment_with_yield>, DEFAULT_HANDLERS},

    {"Testing single thread with wait", test_single_thread<increment_with_wait>, DEFAULT_HANDLERS},
    {"Testing parallel threads with wait", test_parallel_threads<3, increment_with_wait>, DEFAULT_HANDLERS},
    {"Testing serial threads with wait", test_serial_threads<10, increment_with_wait>, DEFAULT_HANDLERS},

    {"Testing single thread with child", test_single_thread<increment_with_child>, DEFAULT_HANDLERS},
    {"Testing parallel threads with child", test_parallel_threads<2, increment_with_child>, DEFAULT_HANDLERS},
    {"Testing serial threads with child", test_serial_threads<10, increment_with_child>, DEFAULT_HANDLERS},

    {"Testing single thread with murder", test_single_thread<increment_with_murder>, DEFAULT_HANDLERS},
    {"Testing parallel threads with murder", test_parallel_threads<2, increment_with_murder>, DEFAULT_HANDLERS},
    {"Testing serial threads with murder", test_serial_threads<10, increment_with_murder>, DEFAULT_HANDLERS},

    {"Testing thread self terminate", test_self_terminate, DEFAULT_HANDLERS},

    {"Testing thread flags: wait", test_thread_flags_set<0x1, flags_wait>, DEFAULT_HANDLERS},
    {"Testing thread flags: timeout", test_thread_flags_set<0x1, flags_wait_tout>, DEFAULT_HANDLERS},
    {"Testing thread flags: multi-bit all", test_thread_flags_set<0x3, flags_wait_multibit>, DEFAULT_HANDLERS},
    {"Testing thread flags: multi-bit all timeout", test_thread_flags_set<0x1, flags_wait_multibit_tout>, DEFAULT_HANDLERS},
    {"Testing thread flags: multi-bit any", test_thread_flags_set<0x1, flags_wait_multibit_any>, DEFAULT_HANDLERS},
    {"Testing thread flags: flags clear", test_thread_flags_clear, DEFAULT_HANDLERS},

    {"Testing thread stack info", test_thread_stack_info, DEFAULT_HANDLERS},
    {"Testing thread wait", test_thread_wait, DEFAULT_HANDLERS},
    {"Testing thread name", test_thread_name, DEFAULT_HANDLERS},

    {"Testing thread states: deleted", test_deleted, DEFAULT_HANDLERS},
    {"Testing thread states: wait delay", test_delay, DEFAULT_HANDLERS},
    {"Testing thread states: wait thread flags", test_thread_flags, DEFAULT_HANDLERS},
    {"Testing thread states: wait event flag", test_evt_flag, DEFAULT_HANDLERS},
    {"Testing thread states: wait mutex", test_mutex, DEFAULT_HANDLERS},
    {"Testing thread states: wait semaphore", test_semaphore, DEFAULT_HANDLERS},
    {"Testing thread states: wait message get", test_msg_get, DEFAULT_HANDLERS},
    {"Testing thread states: wait message put", test_msg_put, DEFAULT_HANDLERS},

    {"Testing thread with external stack memory", test_thread_ext_stack, DEFAULT_HANDLERS},
    {"Testing thread priority ops", test_thread_prio, DEFAULT_HANDLERS}

};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}

#endif // !DEVICE_USTICKER
#endif // defined(MBED_RTOS_SINGLE_THREAD) || !defined(MBED_CONF_RTOS_PRESENT)
