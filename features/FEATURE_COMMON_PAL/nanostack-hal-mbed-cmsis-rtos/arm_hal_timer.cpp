/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

// Include before mbed.h to properly get UINT*_C()
#include "ns_types.h"
#include "mbed.h"
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"
#include <mbed_assert.h>

static osThreadId_t timer_thread_id;
static uint64_t timer_thread_stk[2048/sizeof(uint64_t)];
static osRtxThread_t timer_thread_tcb;

static Timer timer;
static Timeout timeout;
static uint32_t due;
static void (*arm_hal_callback)(void);

static void timer_thread(void *arg)
{
    (void)arg;
    for (;;) {
        osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);
        // !!! We don't do our own enter/exit critical - we rely on callback
        // doing it (ns_timer_interrupt_handler does)
        //platform_enter_critical();
        arm_hal_callback();
        //platform_exit_critical();
    }
}

// Called once at boot
void platform_timer_enable(void)
{
    static osThreadAttr_t timer_thread_attr = {0};
    timer_thread_attr.name = "pal_timer_thread";
    timer_thread_attr.stack_mem  = &timer_thread_stk[0];
    timer_thread_attr.cb_mem  = &timer_thread_tcb;
    timer_thread_attr.stack_size = sizeof(timer_thread_stk);
    timer_thread_attr.cb_size = sizeof(timer_thread_tcb);
    timer_thread_attr.priority = osPriorityRealtime;
    timer_thread_id = osThreadNew(timer_thread, NULL, &timer_thread_attr);
    MBED_ASSERT(timer_thread_id != NULL);
    timer.start();
}

// Actually cancels a timer, not the opposite of enable
void platform_timer_disable(void)
{
    timeout.detach();
}

// Not called while running, fortunately
void platform_timer_set_cb(void (*new_fp)(void))
{
    arm_hal_callback = new_fp;
}

static void timer_callback(void)
{
    due = 0;
    osThreadFlagsSet(timer_thread_id, 1);
}

// This is called from inside platform_enter_critical - IRQs can't happen
void platform_timer_start(uint16_t slots)
{
    timer.reset();
    due = slots * UINT32_C(50);
    timeout.attach_us(timer_callback, due);
}

// This is called from inside platform_enter_critical - IRQs can't happen
uint16_t platform_timer_get_remaining_slots(void)
{
    uint32_t elapsed = timer.read_us();
    if (elapsed < due) {
        return (uint16_t) ((due - elapsed) / 50);
    } else {
        return 0;
    }
}

