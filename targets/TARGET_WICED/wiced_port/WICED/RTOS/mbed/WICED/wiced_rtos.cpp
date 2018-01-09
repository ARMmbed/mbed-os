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
#include "wiced_result.h"
#include "wiced_utilities.h"
#include "wiced_rtos.h"
#include "wiced_time.h"
#include "wwd_rtos.h"
#include "mbed.h"


// worker threads defined here
wiced_worker_thread_t wiced_hardware_io_worker_thread;
wiced_worker_thread_t wiced_networking_worker_thread;


/** Creates and starts a new thread
 *
 * Creates and starts a new thread
 *
 * @param[out] thread     : Pointer to variable that will receive the thread handle
 * @param[in]  priority   : A priority number or WICED_DEFAULT_APP_THREAD_PRIORITY.
 * @param[in]  name       : A text name for the thread (can be null)
 * @param[in]  function   : The main thread function
 * @param[in]  stack_size : Stack size for this thread
 * @param[in]  arg        : Argument which will be passed to thread function
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_create_thread(
        wiced_thread_t *thread,
        uint8_t priority,
        const char *name,
        wiced_thread_function_t function,
        uint32_t stack_size,
        void *arg)
{
    thread->stack = malloc(stack_size);
    if (thread->stack == NULL) {
        return WICED_OUT_OF_HEAP_SPACE;
    }

    wiced_result_t result = wiced_rtos_create_thread_with_stack(
            thread,
            priority,
            name,
            function,
            thread->stack,
            stack_size,
            arg);

    if (result != WICED_WWD_SUCCESS) {
        free(thread->stack);
        thread->stack = NULL;
    }

    return (wiced_result_t)result;
}


/** Creates and starts a new thread with user provided stack
 *
 * Creates and starts a new thread with user provided stack
 *
 * @param[out] thread     : Pointer to variable that will receive the thread handle
 * @param[in]  priority   : A priority number or WICED_DEFAULT_APP_THREAD_PRIORITY.
 * @param[in]  name       : A text name for the thread (can be null)
 * @param[in]  function   : The main thread function
 * @param[in]  stack      : The stack for this thread
 * @param[in]  stack_size : Stack size for this thread
 * @param[in]  arg        : Argument which will be passed to thread function
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_create_thread_with_stack(
        wiced_thread_t *thread,
        uint8_t priority,
        const char *name,
        wiced_thread_function_t function,
        void *stack,
        uint32_t stack_size,
        void *arg)
{
    int8_t native_priority = WICED_PRIORITY_TO_NATIVE_PRIORITY(priority);

    memset(stack, 0, stack_size);

    return (wiced_result_t)host_rtos_create_thread_with_arg(
            WICED_GET_THREAD_HANDLE(thread),
            function,
            name,
            thread->stack,
            stack_size,
            native_priority,
            (wiced_thread_arg_t)arg);
}


/** Deletes a terminated thread
 *
 * @param[in] thread     : The handle of the thread to delete
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_delete_thread(wiced_thread_t *thread)
{
    wiced_result_t result = (wiced_result_t)host_rtos_finish_thread(
            WICED_GET_THREAD_HANDLE(thread));
    if (result != WICED_WWD_SUCCESS) {
        return result;
    }

    result = (wiced_result_t)host_rtos_delete_terminated_thread(
            WICED_GET_THREAD_HANDLE(thread));
    if (result != WICED_WWD_SUCCESS) {
        return result;
    }

    if (thread->stack != NULL) {
        free(thread->stack);
        thread->stack = NULL;
    }

    return result;
}

/** Forcibly wakes another thread
 *
 * Causes the specified thread to wake from suspension. This will usually
 * cause an error or timeout in that thread, since the task it was waiting on
 * is not complete.
 *
 * @param[in] thread : The handle of the other thread which will be woken
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_thread_force_awake(wiced_thread_t *t)
{
    // TODO this is probably not correct
    Thread *thread = reinterpret_cast<Thread*>(&t->handle);
    osStatus err = thread->terminate();
    return (err == osOK) ? WICED_SUCCESS : WICED_ERROR;
}


/** Checks if a thread is the current thread
 *
 * Checks if a specified thread is the currently running thread
 *
 * @param[in] thread : The handle of the other thread against which the current thread will be compared
 *
 * @return    WICED_SUCCESS : specified thread is the current thread
 * @return    WICED_ERROR   : specified thread is not currently running
 */
extern "C" wiced_result_t wiced_rtos_is_current_thread(wiced_thread_t *thread);
    // not supported, force linking error


/** Checks the stack of the current thread
 *
 * @return    WICED_SUCCESS : if the current thread stack is within limits
 * @return    WICED_ERROR   : if the current thread stack has extended beyond its limits
 */
extern "C" wiced_result_t wiced_rtos_check_stack(void)
{
    // do nothing, stack checking in rtos
    return WICED_SUCCESS;
}

/** @} */
/*****************************************************************************/
/** @addtogroup mutexes       Mutexes
 *  @ingroup rtos
 *
 * Mutex management functionss
 *
 *
 *  @{
 */
/*****************************************************************************/

/** Initialises a mutex
 *
 * Initialises a mutex
 * A mutex is different to a semaphore in that a thread that already holds
 * the lock on the mutex can request the lock again (nested) without causing
 * it to be suspended.
 *
 * @param[in] mutex : A pointer to the mutex handle to be initialised
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_init_mutex(wiced_mutex_t *p)
{
    MBED_STATIC_ASSERT(sizeof(wiced_mutex_t) >= sizeof(Mutex),
            "wiced_mutex_t must fit Mutex type");
    new (p) Mutex;
    return WICED_WWD_SUCCESS;
}


/** Obtains the lock on a mutex
 *
 * Attempts to obtain the lock on a mutex. If the lock is already held
 * by another thead, the calling thread will be suspended until
 * the mutex lock is released by the other thread.
 *
 * @param[in] mutex : A pointer to the mutex handle to be locked
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_lock_mutex(wiced_mutex_t *p)
{
    Mutex *mutex = reinterpret_cast<Mutex*>(p);
    mutex->lock();
    return WICED_WWD_SUCCESS;
}


/** Releases the lock on a mutex
 *
 * Releases a currently held lock on a mutex. If another thread
 * is waiting on the mutex lock, then it will be resumed.
 *
 * @param[in] mutex : A pointer to the mutex handle to be unlocked
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_unlock_mutex(wiced_mutex_t *p)
{
    Mutex *mutex = reinterpret_cast<Mutex*>(p);
    mutex->unlock();
    return WICED_WWD_SUCCESS;
}


/** De-initialise a mutex
 *
 * Deletes a mutex created with @ref wiced_rtos_init_mutex
 *
 * @param[in] mutex : A pointer to the mutex handle
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_deinit_mutex(wiced_mutex_t *p)
{
    Mutex *mutex = reinterpret_cast<Mutex*>(p);
    mutex->~Mutex();
    return WICED_WWD_SUCCESS;
}


/** @} */
/*****************************************************************************/
/** @addtogroup queues       Queues
 *  @ingroup rtos
 *
 * Queue management functionss
 *
 *
 *  @{
 */
/*****************************************************************************/

/** Initialises a queue
 *
 * Initialises a FIFO queue
 *
 * @param[in] queue              : A pointer to the queue handle to be initialised
 * @param[in] name               : A text string name for the queue (NULL is allowed)
 * @param[in] message_size       : Size in bytes of objects that will be held in the queue
 * @param[in] number_of_messages : Depth of the queue - i.e. max number of objects in the queue
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_init_queue(wiced_queue_t *queue, const char *name, uint32_t message_size, uint32_t number_of_messages)
{
    size_t buffer_size = (sizeof(mbed_rtos_storage_msg_queue_t)
            + number_of_messages*(sizeof(mbed_rtos_storage_message_t) + message_size));
    queue->buffer = malloc(buffer_size);
    if (!queue->buffer) {
        return WICED_OUT_OF_HEAP_SPACE;
    }

    wiced_result_t result = (wiced_result_t)host_rtos_init_queue(
            WICED_GET_QUEUE_HANDLE(queue),
            queue->buffer,
            buffer_size,
            message_size);

    if (result != WICED_WWD_SUCCESS) {
        free(queue->buffer);
        queue->buffer = NULL;
    }

    return result;
}


/** De-initialise a queue
 *
 * Deletes a queue created with @ref wiced_rtos_init_queue
 *
 * @param[in] queue : A pointer to the queue handle
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_deinit_queue(wiced_queue_t *queue)
{
    wiced_result_t result = (wiced_result_t)host_rtos_deinit_queue(
            WICED_GET_QUEUE_HANDLE(queue));
    if (result != WICED_WWD_SUCCESS) {
        return result;
    }

    if (queue->buffer != NULL) {
        free(queue->buffer);
        queue->buffer = NULL;
    }

    return result;
}


/** Check if a queue is empty
 *
 * @param[in] queue : A pointer to the queue handle
 *
 * @return    WICED_SUCCESS : queue is empty.
 * @return    WICED_ERROR   : queue is not empty.
 */
extern "C" wiced_result_t wiced_rtos_is_queue_empty(wiced_queue_t *queue);
    // not supported, force linking error


/** Check if a queue is full
 *
 * @param[in] queue : A pointer to the queue handle
 *
 * @return    WICED_SUCCESS : queue is full.
 * @return    WICED_ERROR   : queue is not full.
 */
extern "C" wiced_result_t wiced_rtos_is_queue_full(wiced_queue_t *queue);
    // not supported, force linking error


/** Get the queue occupancy
 *
 * @param[in]  queue : A pointer to the queue handle
 * @param[out] count : Pointer to integer for storing occupancy count
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_get_queue_occupancy(wiced_queue_t *queue, uint32_t *count);
    // not supported, force linking error


/** @} */
/*****************************************************************************/
/** @addtogroup rtostmr       RTOS timers
 *  @ingroup rtos
 *
 * RTOS timer management functions
 * These timers are based on the RTOS time-slice scheduling, so are not
 * highly accurate. They are also affected by high loading on the processor.
 *
 *
 *  @{
 */
/*****************************************************************************/

static void timer_thread_main(uint32_t p) {
    reinterpret_cast<EventQueue*>(p)->dispatch();
}

static EventQueue *timer_event_queue() {
    static bool timer_thread_initialized = false;
    static EventQueue timer_event_queue(TIMER_WORKER_THREAD_QUEUE_SIZE);
    static wiced_thread_t timer_thread;

    if (!timer_thread_initialized) {
        wiced_result_t result = wiced_rtos_create_thread(
                &timer_thread,
                osPriorityHigh,
                "timer queue",
                timer_thread_main,
                TIMER_WORKER_THREAD_STACK_SIZE,
                (void*)&timer_event_queue);

        MBED_ASSERT(result == WICED_SUCCESS);
        timer_thread_initialized = true;
    }

    return &timer_event_queue;
}

/** Initialises a RTOS timer
 *
 * Initialises a RTOS timer
 * Timer does not start running until @ref wiced_rtos_start_timer is called
 *
 * @param[in] timer    : A pointer to the timer handle to be initialised
 * @param[in] time_ms  : Timer period in milliseconds
 * @param[in] function : The callback handler function that is called each
 *                       time the timer expires
 * @param[in] arg      : An argument that will be passed to the callback
 *                       function
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_init_timer(wiced_timer_t *timer, uint32_t time_ms, timer_handler_t function, void *arg)
{
    // make sure event queue has been initialized
    timer_event_queue();

    timer->id = -1;
    timer->time_ms = time_ms;
    timer->function = function;
    timer->arg = arg;

    return WICED_SUCCESS;
}


/** Starts a RTOS timer running
 *
 * Starts a RTOS timer running. Timer must have been previously
 * initialised with @ref wiced_rtos_init_timer
 *
 * @param[in] timer    : A pointer to the timer handle to start
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_start_timer(wiced_timer_t *timer)
{
    timer->id = timer_event_queue()->call_in(timer->time_ms, timer->function, timer->arg);
    return timer->id ? WICED_SUCCESS : WICED_ERROR;
}


/** Stops a running RTOS timer
 *
 * Stops a running RTOS timer. Timer must have been previously
 * started with @ref wiced_rtos_start_timer
 *
 * @param[in] timer    : A pointer to the timer handle to stop
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_stop_timer(wiced_timer_t *timer)
{
    timer_event_queue()->cancel(timer->id);
    return WICED_SUCCESS;
}


/** De-initialise a RTOS timer
 *
 * Deletes a RTOS timer created with @ref wiced_rtos_init_timer
 *
 * @param[in] timer : A pointer to the RTOS timer handle
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_deinit_timer(wiced_timer_t *timer)
{
    wiced_rtos_stop_timer(timer);
    return WICED_SUCCESS;
}


/** Check if an RTOS timer is running
 *
 * @param[in] timer : A pointer to the RTOS timer handle
 *
 * @return    WICED_SUCCESS : if running.
 * @return    WICED_ERROR   : if not running
 */
extern "C" wiced_result_t wiced_rtos_is_timer_running(wiced_timer_t *timer);
    // not supported, force linking error (racey)

/** Get the current system tick time in milliseconds
 *
 * @note The time will roll over every 49.7 days
 *
 * @param[out] time : A pointer to the variable which will receive the time value
 *
 * @return @ref wiced_result_t
 */
wiced_result_t wiced_time_get_time(wiced_time_t *time)
{
    *time = timer_event_queue()->tick();
    return WICED_SUCCESS;
}

/** @} */
/*****************************************************************************/
/** @addtogroup eventflags       Event Flags
 *  @ingroup rtos
 *
 * Event flags management functions
 *
 *
 *  @{
 */
/*****************************************************************************/

/** Initialise an event flags
 *
 * @param[in] event_flags : A pointer to the event flags handle
S *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_init_event_flags(wiced_event_flags_t *event_flags);
    // not supported, force linking error

/** Wait for event flags to be set
 *
 * @param[in]  event_flags        : Pointer to the event flags handle
 * @param[in]  flags_to_wait_for  : Group of event flags (ORed bit-fields) to wait for
 * @param[out] flags_set          : Event flag(s) set
 * @param[in]  clear_set_flags    : TRUE to clear set flag, FALSE leaves flags unchanged.
 * @param[in]  wait_option        : Wait option
 * @param[in]  timeout_ms         : Timeout in milliseconds
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_wait_for_event_flags(wiced_event_flags_t *event_flags, uint32_t flags_to_wait_for, uint32_t *flags_set, wiced_bool_t clear_set_flags, wiced_event_flags_wait_option_t wait_option, uint32_t timeout_ms);
    // not supported, force linking error


/** Set event flags
 *
 * @param[in] event_flags  : Pointer to the event flags handle
 * @param[in] flags_to_set : Group of event flags (ORed bit-fields) to set
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_set_event_flags(wiced_event_flags_t *event_flags, uint32_t flags_to_set);
    // not supported, force linking error


/** De-initialise an event flags
 *
 * @param[in] event_flags : Pointer to the event flags handle
 *
 * @return    WICED_SUCCESS : on success.
 * @return    WICED_ERROR   : if an error occurred
 */
extern "C" wiced_result_t wiced_rtos_deinit_event_flags(wiced_event_flags_t *event_flags);
    // not supported, force linking error

/** @} */

