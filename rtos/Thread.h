/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
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
#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include "cmsis_os.h"
#include "platform/Callback.h"
#include "platform/toolchain.h"
#include "rtos/Semaphore.h"
#include "rtos/Mutex.h"

namespace rtos {
/** \addtogroup rtos */
/** @{*/

/** The Thread class allow defining, creating, and controlling thread functions in the system.
 *
 *  Example:
 *  @code
 *  #include "mbed.h"
 *  #include "rtos.h"
 *
 *  Thread thread;
 *  DigitalOut led1(LED1);
 *  volatile bool running = true;
 *
 *  // Blink function toggles the led in a long running loop
 *  void blink(DigitalOut *led) {
 *      while (running) {
 *          *led = !*led;
 *          Thread::wait(1000);
 *      }
 *  }
 *
 *  // Spawns a thread to run blink for 5 seconds
 *  int main() {
 *      thread.start(led1, blink);
 *      Thread::wait(5000);
 *      running = false;
 *      thread.join();
 *  }
 *  @endcode
 */
class Thread {
public:
    /** Allocate a new thread without starting execution
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size      stack size (in bytes) requirements for the thread function. (default: DEFAULT_STACK_SIZE).
      @param   stack_pointer  pointer to the stack area to be used by this thread (default: NULL).
    */
    Thread(osPriority priority=osPriorityNormal,
           uint32_t stack_size=DEFAULT_STACK_SIZE,
           unsigned char *stack_pointer=NULL) {
        constructor(priority, stack_size, stack_pointer);
    }

    /** Create a new thread, and start it executing the specified function.
      @param   task           function to be executed by this thread.
      @param   argument       pointer that is passed to the thread function as start argument. (default: NULL).
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size      stack size (in bytes) requirements for the thread function. (default: DEFAULT_STACK_SIZE).
      @param   stack_pointer  pointer to the stack area to be used by this thread (default: NULL).
      @deprecated
        Thread-spawning constructors hide errors. Replaced by thread.start(task).

        @code
        Thread thread(priority, stack_size, stack_pointer);

        osStatus status = thread.start(task);
        if (status != osOK) {
            error("oh no!");
        }
        @endcode
    */
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "Thread-spawning constructors hide errors. "
        "Replaced by thread.start(task).")
    Thread(mbed::Callback<void()> task,
           osPriority priority=osPriorityNormal,
           uint32_t stack_size=DEFAULT_STACK_SIZE,
           unsigned char *stack_pointer=NULL) {
        constructor(task, priority, stack_size, stack_pointer);
    }

    /** Create a new thread, and start it executing the specified function.
      @param   obj            argument to task.
      @param   method         function to be executed by this thread.
      @param   argument       pointer that is passed to the thread function as start argument. (default: NULL).
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size      stack size (in bytes) requirements for the thread function. (default: DEFAULT_STACK_SIZE).
      @param   stack_pointer  pointer to the stack area to be used by this thread (default: NULL).
      @deprecated
        Thread-spawning constructors hide errors. Replaced by thread.start(callback(task, argument)).

        @code
        Thread thread(priority, stack_size, stack_pointer);

        osStatus status = thread.start(callback(task, argument));
        if (status != osOK) {
            error("oh no!");
        }
        @endcode
    */
    template <typename T>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "Thread-spawning constructors hide errors. "
        "Replaced by thread.start(callback(task, argument)).")
    Thread(T *argument, void (T::*task)(),
           osPriority priority=osPriorityNormal,
           uint32_t stack_size=DEFAULT_STACK_SIZE,
           unsigned char *stack_pointer=NULL) {
        constructor(mbed::callback(task, argument),
                    priority, stack_size, stack_pointer);
    }

    /** Create a new thread, and start it executing the specified function.
      @param   obj            argument to task.
      @param   method         function to be executed by this thread.
      @param   argument       pointer that is passed to the thread function as start argument. (default: NULL).
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size      stack size (in bytes) requirements for the thread function. (default: DEFAULT_STACK_SIZE).
      @param   stack_pointer  pointer to the stack area to be used by this thread (default: NULL).
      @deprecated
        Thread-spawning constructors hide errors. Replaced by thread.start(callback(task, argument)).

        @code
        Thread thread(priority, stack_size, stack_pointer);

        osStatus status = thread.start(callback(task, argument));
        if (status != osOK) {
            error("oh no!");
        }
        @endcode
    */
    template <typename T>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "Thread-spawning constructors hide errors. "
        "Replaced by thread.start(callback(task, argument)).")
    Thread(T *argument, void (*task)(T *),
           osPriority priority=osPriorityNormal,
           uint32_t stack_size=DEFAULT_STACK_SIZE,
           unsigned char *stack_pointer=NULL) {
        constructor(mbed::callback(task, argument),
                    priority, stack_size, stack_pointer);
    }

    /** Create a new thread, and start it executing the specified function.
        Provided for backwards compatibility
      @param   task           function to be executed by this thread.
      @param   argument       pointer that is passed to the thread function as start argument. (default: NULL).
      @param   priority       initial priority of the thread function. (default: osPriorityNormal).
      @param   stack_size      stack size (in bytes) requirements for the thread function. (default: DEFAULT_STACK_SIZE).
      @param   stack_pointer  pointer to the stack area to be used by this thread (default: NULL).
      @deprecated
        Thread-spawning constructors hide errors. Replaced by thread.start(callback(task, argument)).

        @code
        Thread thread(priority, stack_size, stack_pointer);

        osStatus status = thread.start(callback(task, argument));
        if (status != osOK) {
            error("oh no!");
        }
        @endcode
    */
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "Thread-spawning constructors hide errors. "
        "Replaced by thread.start(callback(task, argument)).")
    Thread(void (*task)(void const *argument), void *argument=NULL,
           osPriority priority=osPriorityNormal,
           uint32_t stack_size=DEFAULT_STACK_SIZE,
           unsigned char *stack_pointer=NULL) {
        constructor(mbed::callback((void (*)(void *))task, argument),
                    priority, stack_size, stack_pointer);
    }

    /** Starts a thread executing the specified function.
      @param   task           function to be executed by this thread.
      @return  status code that indicates the execution status of the function.
    */
    osStatus start(mbed::Callback<void()> task);

    /** Starts a thread executing the specified function.
      @param   obj            argument to task
      @param   method         function to be executed by this thread.
      @return  status code that indicates the execution status of the function.
      @deprecated
          The start function does not support cv-qualifiers. Replaced by start(callback(obj, method)).
    */
    template <typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
        "The start function does not support cv-qualifiers. "
        "Replaced by thread.start(callback(obj, method)).")
    osStatus start(T *obj, M method) {
        return start(mbed::callback(obj, method));
    }

    /** Wait for thread to terminate
      @return  status code that indicates the execution status of the function.
      @note not callable from interrupt
    */
    osStatus join();

    /** Terminate execution of a thread and remove it from Active Threads
      @return  status code that indicates the execution status of the function.
    */
    osStatus terminate();

    /** Set priority of an active thread
      @param   priority  new priority value for the thread function.
      @return  status code that indicates the execution status of the function.
    */
    osStatus set_priority(osPriority priority);

    /** Get priority of an active thread
      @return  current priority value of the thread function.
    */
    osPriority get_priority();

    /** Set the specified Signal Flags of an active thread.
      @param   signals  specifies the signal flags of the thread that should be set.
      @return  previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
    */
    int32_t signal_set(int32_t signals);

    /** Clears the specified Signal Flags of an active thread.
      @param   signals  specifies the signal flags of the thread that should be cleared.
      @return  resultant signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
    */
    int32_t signal_clr(int32_t signals);

    /** State of the Thread */
    enum State {
        Inactive,           /**< Not created or terminated */
        Ready,              /**< Ready to run */
        Running,            /**< Running */
        WaitingDelay,       /**< Waiting for a delay to occur */
        WaitingInterval,    /**< Waiting for an interval to occur */
        WaitingOr,          /**< Waiting for one event in a set to occur */
        WaitingAnd,         /**< Waiting for multiple events in a set to occur */
        WaitingSemaphore,   /**< Waiting for a semaphore event to occur */
        WaitingMailbox,     /**< Waiting for a mailbox event to occur */
        WaitingMutex,       /**< Waiting for a mutex event to occur */

        /* Not in sync with RTX below here */
        Deleted,            /**< The task has been deleted */
    };

    /** State of this Thread
      @return  the State of this Thread
    */
    State get_state();
    
    /** Get the total stack memory size for this Thread
      @return  the total stack memory size in bytes
    */
    uint32_t stack_size();
    
    /** Get the currently unused stack memory for this Thread
      @return  the currently unused stack memory in bytes
    */
    uint32_t free_stack();
    
    /** Get the currently used stack memory for this Thread
      @return  the currently used stack memory in bytes
    */
    uint32_t used_stack();
    
    /** Get the maximum stack memory usage to date for this Thread
      @return  the maximum stack memory usage to date in bytes
    */
    uint32_t max_stack();

    /** Wait for one or more Signal Flags to become signaled for the current RUNNING thread.
      @param   signals   wait until all specified signal flags set or 0 for any single signal flag.
      @param   millisec  timeout value or 0 in case of no time-out. (default: osWaitForever).
      @return  event flag information or error code.
      @note not callable from interrupt
    */
    static osEvent signal_wait(int32_t signals, uint32_t millisec=osWaitForever);

    /** Wait for a specified time period in millisec:
      @param   millisec  time delay value
      @return  status code that indicates the execution status of the function.
      @note not callable from interrupt
    */
    static osStatus wait(uint32_t millisec);

    /** Pass control to next thread that is in state READY.
      @return  status code that indicates the execution status of the function.
      @note not callable from interrupt
    */
    static osStatus yield();

    /** Get the thread id of the current running thread.
      @return  thread ID for reference by other functions or NULL in case of error.
    */
    static osThreadId gettid();
    
    /** Attach a function to be called by the RTOS idle task
      @param   fptr  pointer to the function to be called
    */
    static void attach_idle_hook(void (*fptr)(void));

    /** Attach a function to be called when a task is killed
      @param   fptr  pointer to the function to be called
    */
    static void attach_terminate_hook(void (*fptr)(osThreadId id));

    virtual ~Thread();

private:
    // Required to share definitions without
    // delegated constructors
    void constructor(osPriority priority=osPriorityNormal,
                     uint32_t stack_size=DEFAULT_STACK_SIZE,
                     unsigned char *stack_pointer=NULL);
    void constructor(mbed::Callback<void()> task,
                     osPriority priority=osPriorityNormal,
                     uint32_t stack_size=DEFAULT_STACK_SIZE,
                     unsigned char *stack_pointer=NULL);
    static void _thunk(const void * thread_ptr);

    mbed::Callback<void()> _task;
    osThreadId _tid;
    osThreadDef_t _thread_def;
    bool _dynamic_stack;
    Semaphore _join_sem;
    Mutex _mutex;
};

}
#endif

/** @}*/
