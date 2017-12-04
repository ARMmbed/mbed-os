/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include <mbed_assert.h>
#include "cmsis.h"
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include "ns_trace.h"

#include "eventOS_scheduler.h"

#include "ns_event_loop.h"

#define TRACE_GROUP "evlp"

static void event_loop_thread(void *arg);

static uint64_t event_thread_stk[MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_THREAD_STACK_SIZE/8];
static mbed_rtos_storage_thread_t event_thread_tcb;
static const osThreadAttr_t event_thread_attr = {
    .name = "nanostack_event_thread",
    .priority = osPriorityNormal,
    .stack_mem = &event_thread_stk[0],
    .stack_size = sizeof event_thread_stk,
    .cb_mem = &event_thread_tcb,
    .cb_size = sizeof event_thread_tcb,
};
static osThreadId_t event_thread_id;
static mbed_rtos_storage_mutex_t event_mutex;
static const osMutexAttr_t event_mutex_attr = {
  .name = "nanostack_event_mutex",
  .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
  .cb_mem = &event_mutex,
  .cb_size = sizeof event_mutex,
};
static osMutexId_t event_mutex_id;
static osThreadId_t event_mutex_owner_id = NULL;
static uint32_t owner_count = 0;

void eventOS_scheduler_mutex_wait(void)
{
    osMutexAcquire(event_mutex_id, osWaitForever);
    if (0 == owner_count) {
        event_mutex_owner_id = osThreadGetId();
    }
    owner_count++;
}

void eventOS_scheduler_mutex_release(void)
{
    owner_count--;
    if (0 == owner_count) {
        event_mutex_owner_id = NULL;
    }
    osMutexRelease(event_mutex_id);
}

uint8_t eventOS_scheduler_mutex_is_owner(void)
{
    return osThreadGetId() == event_mutex_owner_id ? 1 : 0;
}

void eventOS_scheduler_signal(void)
{
    // XXX why does signal set lock if called with irqs disabled?
    //__enable_irq();
    //tr_debug("signal %p", (void*)event_thread_id);
    osThreadFlagsSet(event_thread_id, 1);
    //tr_debug("signalled %p", (void*)event_thread_id);
}

void eventOS_scheduler_idle(void)
{
    //tr_debug("idle");
    eventOS_scheduler_mutex_release();
    osThreadFlagsWait(1, 0, osWaitForever);
    eventOS_scheduler_mutex_wait();
}

static void event_loop_thread(void *arg)
{
    (void)arg;
    eventOS_scheduler_mutex_wait();
    eventOS_scheduler_run(); //Does not return
}

void ns_event_loop_thread_create(void)
{
    event_mutex_id = osMutexNew(&event_mutex_attr);
    MBED_ASSERT(event_mutex_id != NULL);

    event_thread_id = osThreadNew(event_loop_thread, NULL, &event_thread_attr);
    MBED_ASSERT(event_thread_id != NULL);
}

void ns_event_loop_thread_start(void)
{
}
