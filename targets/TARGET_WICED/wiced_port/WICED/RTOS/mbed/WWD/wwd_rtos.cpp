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
#include "wwd_rtos.h"
#include "wwd_constants.h"
#include "wwd_assert.h"
#include "wwd_rtos_interface.h"
#include "wiced_utilities.h"
#include "rtos.h"
#include "ticker_api.h"


extern "C" wwd_result_t host_rtos_create_thread(
        host_thread_type_t *p,
        void (*entry_function)(uint32_t),
        const char *name, void *stack,
        uint32_t stack_size,
        uint32_t priority)
{
    return host_rtos_create_thread_with_arg(
            p, entry_function, name, stack, stack_size, priority, 0);
}

extern "C" wwd_result_t host_rtos_create_thread_with_arg(
        host_thread_type_t *p,
        void (*entry_function)(uint32_t),
        const char *name, 
        void *stack,
        uint32_t stack_size,
        uint32_t priority,
        uint32_t arg)
{
    MBED_STATIC_ASSERT(sizeof(host_thread_type_t) >= sizeof(Thread),
            "host_thread_type_t must fit Thread type");
    Thread *thread = new (p) Thread(osPriorityNormal,
            stack_size, (unsigned char*)stack);
    int err = thread->start(callback(
            reinterpret_cast<void (*)(void*)>(entry_function),
            reinterpret_cast<void*>(arg)));

    return (!err) ? WWD_SUCCESS : WWD_THREAD_CREATE_FAILED;
}

extern "C" wwd_result_t host_rtos_create_configed_thread(
        host_thread_type_t *p,
        void (*entry_function)(uint32_t),
        const char *name,
        void *stack,
        uint32_t stack_size,
        uint32_t priority,
        host_rtos_thread_config_type_t *config)
{
    return host_rtos_create_thread(
            p, entry_function, name, stack, stack_size, priority);
}

extern "C" wwd_result_t host_rtos_finish_thread(host_thread_type_t *p)
{
    // NOP: Return from thread terminates thread
    return WWD_SUCCESS;
}

extern "C" wwd_result_t host_rtos_join_thread(host_thread_type_t *p)
{
    Thread *thread = reinterpret_cast<Thread*>(p);
    int err = thread->join();
    return (!err) ? WWD_SUCCESS : WWD_THREAD_CREATE_FAILED;
}

extern "C" wwd_result_t host_rtos_delete_terminated_thread(host_thread_type_t *p)
{
    Thread *thread = reinterpret_cast<Thread*>(p);
    thread->~Thread();
    return WWD_SUCCESS;
}

extern "C" wwd_result_t host_rtos_init_semaphore(
        host_semaphore_type_t *p)
{
    MBED_STATIC_ASSERT(sizeof(host_semaphore_type_t) >= sizeof(Semaphore),
            "host_semaphore_type_t must fit Semaphore type");
    new (p) Semaphore;
    return WWD_SUCCESS;
}

extern "C" wwd_result_t host_rtos_get_semaphore(
        host_semaphore_type_t *p,
        uint32_t timeout_ms,
        wiced_bool_t will_set_in_isr)
{
    if (timeout_ms == NEVER_TIMEOUT) {
        timeout_ms = osWaitForever;
    }

    Semaphore *sema = reinterpret_cast<Semaphore*>(p);
    int32_t res = sema->wait(timeout_ms);
    return (res >= 1) ? WWD_SUCCESS : WWD_TIMEOUT;
}

extern "C" wwd_result_t host_rtos_set_semaphore(
        host_semaphore_type_t *p,
        wiced_bool_t called_from_ISR)
{
    Semaphore *sema = reinterpret_cast<Semaphore*>(p);
    sema->release();
    return WWD_SUCCESS;
}

extern "C" wwd_result_t host_rtos_deinit_semaphore(
        host_semaphore_type_t *p)
{
    Semaphore *sema = reinterpret_cast<Semaphore*>(p);
    sema->~Semaphore();
    return WWD_SUCCESS;
}

extern "C" wwd_time_t host_rtos_get_time(void)
{
//    TODO
//    const ticker_data_t *ticker = get_us_ticker_data();
//    us_timestamp_t timestamp = ticker_read_us(ticker);
//    return (wwd_time_t)(timestamp / 1000);
    return us_ticker_read() / 1000;
}

extern "C" wwd_result_t host_rtos_delay_milliseconds(uint32_t num_ms)
{
    wait_ms(num_ms);
    return WWD_SUCCESS;
}

// We have to implement a direct layer to the cmsis queue since the
// mbed APIs lack a dynamic queue
#define HOST_QUEUE_OVERHEAD (10*sizeof(uint32_t))
struct host_queue {
    osMessageQueueId_t   id;
    osMessageQueueAttr_t attr;
};

extern "C" wwd_result_t host_rtos_init_queue(host_queue_type_t *p, void *buffer, uint32_t buffer_size, uint32_t message_size)
{
    MBED_STATIC_ASSERT(sizeof(host_queue_type_t) >= sizeof(struct host_queue),
            "host_queue_type_t must fit Queue type");
    MBED_ASSERT(buffer_size > HOST_QUEUE_OVERHEAD);

    // cmsis queue needs pointer overhead for each message,
    // we need to recalculate real queue size
    uint32_t real_message_size = sizeof(mbed_rtos_storage_message_t) + message_size;
    uint32_t real_queue_size = (buffer_size - sizeof(mbed_rtos_storage_msg_queue_t)) / real_message_size;
    struct host_queue *q = reinterpret_cast<struct host_queue*>(p);
    memset(buffer, 0, buffer_size);

    q->attr.cb_mem  = &((uint8_t*)buffer)[0];
    q->attr.cb_size = sizeof(mbed_rtos_storage_msg_queue_t);
    q->attr.mq_mem  = &((uint8_t*)buffer)[sizeof(mbed_rtos_storage_msg_queue_t)];
    q->attr.mq_size = buffer_size - sizeof(mbed_rtos_storage_msg_queue_t);

    q->id = osMessageQueueNew(real_queue_size, message_size, &q->attr);
    if (q->id == NULL) {
        error("Error initialising the queue object\n");
    }

    return WWD_SUCCESS;
}

extern "C" wwd_result_t host_rtos_push_to_queue(host_queue_type_t *p, void *m, uint32_t timeout_ms)
{
    struct host_queue *q = reinterpret_cast<struct host_queue*>(p);
    osStatus_t err = osMessageQueuePut(q->id, m, 0, timeout_ms);
    MBED_ASSERT(err == osOK || err == osErrorTimeout);
    return err == osOK ? WWD_SUCCESS : WWD_TIMEOUT;
}

extern "C" wwd_result_t host_rtos_pop_from_queue(host_queue_type_t *p, void *m, uint32_t timeout_ms)
{

    struct host_queue *q = reinterpret_cast<struct host_queue*>(p);

    osStatus_t err = osMessageQueueGet(q->id, m, 0, timeout_ms);
    MBED_ASSERT(err == osOK || err == osErrorTimeout);
    return err == osOK ? WWD_SUCCESS : WWD_TIMEOUT;
}

extern "C" wwd_result_t host_rtos_deinit_queue(host_queue_type_t *p)
{
    // do nothing
    return WWD_SUCCESS;
}
