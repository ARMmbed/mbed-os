/* Mbed Microcontroller Library
 * Copyright (c) 2017-2019 ARM Limited
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H

#include <stdint.h>
#include <utility>
#include "rtos/mbed_rtos_types.h"
#include "rtos/Mutex.h"
#include "rtos/Semaphore.h"
#include "rtos/Kernel.h"

#include "platform/NonCopyable.h"

#if MBED_CONF_RTOS_PRESENT || defined(DOXYGEN_ONLY)

namespace rtos {
/** \addtogroup rtos-public-api */
/** @{*/

enum class cv_status {
    no_timeout,
    timeout
};

struct Waiter;
/**
 * \defgroup rtos_ConditionVariable ConditionVariable class
 * @{
 */

/** The ConditionVariable class is a synchronization primitive that allows
 *   threads to wait until a particular condition occurs.
 *
 * Use the condition variable in conjunction with a mutex to safely wait for
 * or notify waiters of condition changes to a resource accessible by multiple
 * threads.
 *
 * The thread that intends to wait on a ConditionVariable must:
 * - Acquire a lock on a mutex.
 * - Execute `wait`, `wait_for` or `wait_until`. While the thread is waiting,
 *   the mutex is unlocked.
 * - When the condition variable has been notified, or in the case of `wait_for`
 *   and `wait_until` the timeout expires, the thread is awakened.
 *
 * The thread that intends to notify a ConditionVariable must:
 * - Acquire a lock on the mutex used to construct the condition variable.
 * - Execute `notify_one` or `notify_all` on the condition variable.
 *
 * All threads waiting on the condition variable wake when
 *   `ConditionVariable::notify_all` is called.
 * At least one thread waiting on the condition variable wakes
 *   when `ConditionVariable::notify_one` is called.
 *
 * While a thread is waiting for notification of a
 *   ConditionVariable, it releases the lock held on the mutex.
 * The ConditionVariable reacquires the mutex lock before exiting the wait
 *   function.
 *
 * #### Unspecified behavior
 * - The thread that is unblocked on `ConditionVariable::notify_one` is
 *   unspecified if there are multiple waiters.
 * - When `ConditionVariable::notify_one` or `ConditionVariable::notify_all` is
 *   called and there are one or more waiters, and one or more threads
 *   attempting to acquire the condition variable's mutex, the order in which the mutex is
 *   acquired is unspecified.
 * - Spurious notifications (not triggered by the application) can occur.
 *
 * #### Undefined behavior
 * - Calling wait if the mutex is not locked by the current thread is undefined
 *   behavior.
 * - The order in which waiting threads acquire the condition variable's
 *   mutex after `ConditionVariable::notify_all` is called is undefined.
 * - The behavior of `ConditionVariable::wait` and `ConditionVariable::wait_for`
 *   is undefined if the condition variable's mutex is locked more than once by
 *   the calling thread.
 *
 * @note Synchronization level: Thread safe
 *
 * @note Bare metal profile: This class is not supported.
 *
 * Example:
 *
 * @code
 * #include "mbed.h"
 *
 * Mutex mutex;
 * ConditionVariable cv(mutex);
 *
 * // These variables are protected by locking the mutex.
 * uint32_t work_count = 0;
 * bool done = false;
 *
 * void worker_thread()
 * {
 *   // Acquire lock on mutex before accessing protected variables and waiting.
 *   mutex.lock();
 *
 *   while (done == false) {
 *     printf("Worker thread: Count: %lu\r\n", work_count);
 *
 *     // Wait for main thread to notify the condition variable.
 *     printf("Worker thread: Waiting\r\n");
 *     cv.wait();
 *   }
 *
 *   printf("Worker: Exiting\r\n");
 *
 *   // The condition variable acquires the lock when exiting the `wait` function.
 *   // Unlock mutex when exiting the thread.
 *   mutex.unlock();
 * }
 *
 * int main()
 * {
 *   Thread thread;
 *   thread.start(worker_thread);
 *
 *   for (int i = 0; i < 5; i++) {
 *     // Acquire lock on mutex before modifying variables and notifying.
 *     mutex.lock();
 *
 *     // Change count and notify waiters.
 *     work_count++;
 *     printf("Main thread: Set count to: %lu\r\n", work_count);
 *     printf("Main thread: Notifying worker thread\r\n");
 *     cv.notify_all();
 *
 *     // Mutex must be unlocked before the worker thread can acquire it.
 *     mutex.unlock();
 *
 *     ThisThread::sleep_for(1000);
 *   }
 *
 *   // Change done and notify waiters of this.
 *   mutex.lock();
 *   done = true;
 *   cv.notify_all();
 *   mutex.unlock();
 *
 *   thread.join();
 *
 *   printf("Main: Exiting\r\n");
 * }
 * @endcode
 */

class ConditionVariable : private mbed::NonCopyable<ConditionVariable> {
public:
    /** Create and initialize a ConditionVariable object.
     *
     * @note You cannot call this function from ISR context.
    */
    ConditionVariable(Mutex &mutex);

    /** Wait for a notification.
     *
     * Wait causes the current thread to block until the condition variable
     * receives a notification from another thread.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - Spurious notifications can occur, so the caller of this API
     * should check to make sure the condition the caller is waiting on has
     * been met.
     *
     * @note - The current thread releases the mutex while inside the wait
     * function and reacquires it upon exiting the function.
     *
     * Example:
     * @code
     * mutex.lock();
     *
     * while (!condition_met) {
     *     cond.wait();
     * }
     *
     * function_to_handle_condition();
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    void wait();

    /** Wait for a predicate.
     *
     * Wait causes the current thread to block until the predicate is
     * true.
     *
     * @param   pred      A function-like object such that `pred()` is convertible to bool
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - The current thread releases the mutex while inside the wait
     * function and reacquires it upon exiting the function.
     *
     * Example:
     * @code
     * extern bool data_available();
     *
     * mutex.lock();
     *
     * cond.wait(data_available);
     *
     * function_to_handle_data();
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    template <typename Predicate>
    void wait(Predicate pred)
    {
        while (!pred()) {
            wait();
        }
    }


    /** Wait for a notification until the specified time.
     *
     * Wait until causes the current thread to block until the condition
     * variable is notified, or a specific time given by millisec parameter is
     * reached.
     *
     * @param   millisec  Absolute end time referenced to `Kernel::get_ms_count()`
     * @return  `true` if a timeout occurred, `false` otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - Spurious notifications can occur, so the caller of this API
     * should check to make sure the condition the caller is waiting on has
     * been met.
     *
     * @note - The current thread releases the lock while inside the wait
     * function and reacquires it upon exiting the function.
     *
     * Example:
     * @code
     * mutex.lock();
     * uint64_t end_time = Kernel::get_ms_count() + COND_WAIT_TIMEOUT;
     *
     * while (!condition_met) {
     *     if (cond.wait_until(end_time)) {
     *         break;
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     * @deprecated Pass a chrono time_point, not an integer millisecond count. For example use `Kernel::Clock::now() + 5s`
     *             rather than `Kernel::get_ms_count() + 5000`.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono time_point, not an integer millisecond count. For example use `Kernel::Clock::now() + 5s` rather than `Kernel::get_ms_count() + 5000`.")
    bool wait_until(uint64_t millisec);

    /** Wait for a notification until the specified time.
     *
     * Wait until causes the current thread to block until the condition
     * variable is notified, or a specific time given by millisec parameter is
     * reached.
     *
     * @param   abs_time  Absolute end time referenced to `Kernel::Clock`
     * @return  `cv_status::timeout` if a timeout occurred, `cv_status::no_timeout` otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - Spurious notifications can occur, so the caller of this API
     * should check to make sure the condition the caller is waiting on has
     * been met.
     *
     * @note - The current thread releases the lock while inside the wait
     * function and reacquires it upon exiting the function.
     *
     * Example:
     * @code
     * mutex.lock();
     * Kernel::Clock::time_point end_time = Kernel::Clock::now() + 2s;
     *
     * while (!condition_met) {
     *     if (cond.wait_until(end_time) == cv_status::timeout) {
     *         break;
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    cv_status wait_until(Kernel::Clock::time_point abs_time);

    /** Wait for a predicate until the specified time.
     *
     * Wait until causes the current thread to block until the predicate is true,
     * or a specific time given by abs_time parameter is reached.
     *
     * @param   abs_time  Absolute end time referenced to `Kernel::Clock`
     * @param   pred      A function-like object such that `pred()` is convertible to bool
     * @return  The state of the predicate
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - The current thread releases the mutex while inside the wait
     * function and reacquires it upon exiting the function.
     *
     * Example:
     * @code
     * extern bool data_available();
     *
     * mutex.lock();
     *
     * if (cond.wait_until(Kernel::Clock::now() + 2s, data_available)) {
     *     function_to_handle_data();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    template <class Predicate>
    bool wait_until(Kernel::Clock::time_point abs_time, Predicate pred)
    {
        while (!pred()) {
            if (wait_until(abs_time) == cv_status::timeout) {
                return pred();
            }
        }
        return true;
    }

    /** Wait for a notification or timeout.
     *
     * `Wait for` causes the current thread to block until the condition
     * variable receives a notification from another thread, or the timeout
     * specified by the millisec parameter is reached.
     *
     * @param   millisec  Timeout value or osWaitForever in case of no timeout.
     * @return  `true` if a timeout occurred, `false` otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - Spurious notifications can occur, so the caller of this API
     * should check to make sure the condition the caller is waiting on has
     * been met.
     *
     * @note - The current thread releases the lock while inside the wait
     * function and reacquire it upon exiting the function.
     *
     * Example:
     * @code
     * mutex.lock();
     *
     * while (!condition_met) {
     *     cond.wait_for(MAX_SLEEP_TIME);
     *     if (!condition_met) {
     *         do_other_work_while_condition_false();
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     * @deprecated Pass a chrono duration, not an integer millisecond count. For example use `5s` rather than `5000`.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.0.0", "Pass a chrono duration, not an integer millisecond count. For example use `5s` rather than `5000`.")
    bool wait_for(uint32_t millisec);

    /** Wait for a notification or timeout.
     *
     * `Wait for` causes the current thread to block until the condition
     * variable receives a notification from another thread, or the timeout
     * specified by the millisec parameter is reached.
     *
     * @param   rel_time Timeout value.
     * @return  `cv_status::timeout` if a timeout occurred, `cv_status::no_timeout` otherwise.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - Spurious notifications can occur, so the caller of this API
     * should check to make sure the condition the caller is waiting on has
     * been met.
     *
     * @note - The current thread releases the lock while inside the wait
     * function and reacquire it upon exiting the function.
     *
     * Example:
     * @code
     * mutex.lock();
     *
     * while (!condition_met) {
     *     cond.wait_for(MAX_SLEEP_TIME);
     *     if (!condition_met) {
     *         do_other_work_while_condition_false();
     *     }
     * }
     *
     * if (condition_met) {
     *     function_to_handle_condition();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    cv_status wait_for(Kernel::Clock::duration_u32 rel_time);

    /** Wait for a predicate or timeout.
     *
     * `Wait for` causes the current thread to block until the predicate
     * is true, or the timeout specified by the rel_time parameter is reached.
     *
     * @param   rel_time  Timeout value.
     * @param   pred      a function-like object such that `pred()` is convertible to bool
     * @return  The state of the predicate
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex, and it must be locked exactly once.
     *
     * @note - The current thread releases the mutex while inside the wait
     * function and reacquire it upon exiting the function.
     *
     * Example:
     * @code
     * extern bool data_available();
     *
     * mutex.lock();
     *
     * if (cond.wait_for(2s, data_available)) {
     *     function_to_handle_data();
     * }
     *
     * mutex.unlock();
     * @endcode
     *
     * @note You cannot call this function from ISR context.
     */
    template <class Predicate>
    bool wait_for(Kernel::Clock::duration rel_time, Predicate pred)
    {
        return wait_until(Kernel::Clock::now() + rel_time, std::move(pred));
    }

    /** Notify one waiter on this condition variable that a condition changed.
     *
     * This function unblocks one of the threads waiting for the condition
     * variable.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex.
     *
     * @note - The thread that is unblocked on ConditionVariable::notify_one is
     * undefined if there are multiple waiters.
     *
     * @note You cannot call this function from ISR context.
     */
    void notify_one();

    /** Notify all waiters on this condition variable that a condition changed.
     *
     * This function unblocks all of the threads waiting for the condition
     * variable.
     *
     * @note - The thread calling this function must be the owner of the
     * ConditionVariable's mutex.
     *
     * @note - If there are one or more waiters and one or more threads
     * attempting to acquire the condition variable's mutex the order in which
     * the mutex is acquired is undefined.
     *
     * @note You cannot call this function from ISR context.
     */
    void notify_all();

    /** ConditionVariable destructor.
     *
     * @note You cannot call this function from ISR context.
     */
    ~ConditionVariable();

#if !defined(DOXYGEN_ONLY)
protected:
    struct Waiter {
        Waiter();
        Semaphore sem;
        Waiter *prev;
        Waiter *next;
        bool in_list;
    };

    static void _add_wait_list(Waiter **wait_list, Waiter *waiter);
    static void _remove_wait_list(Waiter **wait_list, Waiter *waiter);
    Mutex &_mutex;
    Waiter *_wait_list;
#endif // !defined(DOXYGEN_ONLY)
};

/** @}*/
/** @}*/
} // namespace rtos
#endif

#endif
