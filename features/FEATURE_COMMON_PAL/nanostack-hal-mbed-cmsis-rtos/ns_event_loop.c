/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include "cmsis.h"
#include "cmsis_os2.h"
#include "ns_trace.h"

#include "eventOS_scheduler.h"

#include "ns_event_loop.h"

#define TRACE_GROUP "evlp"

static void event_loop_thread(const void *arg);

static osThreadAttr_t event_thread_attr;
static osMutexAttr_t event_mutex_attr;

static osThreadId_t event_thread_id;
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

static void event_loop_thread(const void *arg)
{
    //tr_debug("event_loop_thread create");
    osThreadFlagsWait(2, 0, osWaitForever);

    eventOS_scheduler_mutex_wait();
    tr_debug("event_loop_thread");

    // Run does not return - it calls eventOS_scheduler_idle when it's, er, idle
    eventOS_scheduler_run();
}

void ns_event_loop_thread_create(void)
{
    event_mutex_id = osMutexNew(NULL);

    event_thread_attr.priority = osPriorityNormal;
    event_thread_attr.stack_size = MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_THREAD_STACK_SIZE; // 1K should be enough - it's what the SAM4E port uses...
    event_thread_id = osThreadNew(event_loop_thread, NULL, &event_thread_attr);
}

void ns_event_loop_thread_start(void)
{
    osThreadFlagsSet(event_thread_id, 2);
}
