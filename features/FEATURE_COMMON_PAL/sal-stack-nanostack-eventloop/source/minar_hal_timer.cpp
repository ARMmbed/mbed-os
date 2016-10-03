/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

// Include before mbed.h to properly get UINT*_C()

#include "ns_types.h"

#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"

#if defined(NS_EVENTLOOP_USE_TICK_TIMER) && defined(YOTTA_CFG_MINAR)

#include "minar/minar.h"
#include "mbed-drivers/mbed.h"
#include "core-util/FunctionPointer.h"
#include "core-util/Event.h"

#define TICK_TIMER_ID   1

using minar::Scheduler;
using minar::milliseconds;
using minar::callback_handle_t;
using namespace mbed::util;

static callback_handle_t sys_timer_handle;
static void (*tick_timer_callback)(void);

void timer_callback(void const *funcArgument)
{
    (void)funcArgument;
    if (NULL != tick_timer_callback) {
        tick_timer_callback();
    }
}

// Low precision platform tick timer
int8_t platform_tick_timer_register(void (*tick_timer_cb_handler)(void))
{
    tick_timer_callback = tick_timer_cb_handler;
    return TICK_TIMER_ID;
}

int8_t platform_tick_timer_start(uint32_t period_ms)
{
    int8_t retval = -1;
    if (sys_timer_handle != NULL) {
        return 0; // Timer already started already so return success
    }
    Event e = FunctionPointer1<void, void const *>(timer_callback).bind(NULL);
    if (e != NULL) {
        sys_timer_handle = Scheduler::postCallback(e).period(milliseconds(period_ms)).getHandle();
        if (sys_timer_handle != NULL) {
            retval = 0;
        }
    }
    return retval;
}

int8_t platform_tick_timer_stop(void)
{
    int8_t retval = -1;
    if (sys_timer_handle != NULL) {
        Scheduler::cancelCallback(sys_timer_handle);
        sys_timer_handle = NULL;
        retval = 0;
    }
    return retval;
}

#endif // defined(NS_EVENTLOOP_USE_TICK_TIMER) && defined(YOTTA_CFG)
