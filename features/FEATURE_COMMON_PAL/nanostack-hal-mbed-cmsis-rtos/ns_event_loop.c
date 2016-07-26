/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include "cmsis.h"
#include "cmsis_os.h"
#include "ns_trace.h"

#include "eventOS_scheduler.h"

#include "ns_event_loop.h"

#define TRACE_GROUP "evlp"

static void event_loop_thread(const void *arg);

// 1K should be enough - it's what the SAM4E port uses...
// What happened to the instances parameter?
static osThreadDef(event_loop_thread, osPriorityNormal, /*1,*/ MBED_CONF_NANOSTACK_HAL_EVENT_LOOP_THREAD_STACK_SIZE);
static osMutexDef(event);

static osThreadId event_thread_id;
static osMutexId event_mutex_id;
static osThreadId event_mutex_owner_id = NULL;
static uint32_t owner_count = 0;

void eventOS_scheduler_mutex_wait(void)
{
    osMutexWait(event_mutex_id, osWaitForever);
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
    osSignalSet(event_thread_id, 1);
    //tr_debug("signalled %p", (void*)event_thread_id);
}

void eventOS_scheduler_idle(void)
{
    //tr_debug("idle");
    eventOS_scheduler_mutex_release();
    osSignalWait(1, osWaitForever);
    eventOS_scheduler_mutex_wait();
}

static void event_loop_thread(const void *arg)
{
    //tr_debug("event_loop_thread create");
    osSignalWait(2, osWaitForever);

    eventOS_scheduler_mutex_wait();
    tr_debug("event_loop_thread");

    // Run does not return - it calls eventOS_scheduler_idle when it's, er, idle
    eventOS_scheduler_run();
}

void ns_event_loop_thread_create(void)
{
    event_mutex_id = osMutexCreate(osMutex(event));
    event_thread_id = osThreadCreate(osThread(event_loop_thread), NULL);
}

void ns_event_loop_thread_start(void)
{
    osSignalSet(event_thread_id, 2);
}
