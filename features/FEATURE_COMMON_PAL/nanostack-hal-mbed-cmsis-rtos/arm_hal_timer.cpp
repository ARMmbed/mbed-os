/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

// Include before mbed.h to properly get UINT*_C()
#include "ns_types.h"

#include "cmsis_os.h"
#include "mbed.h"

#include "platform/arm_hal_timer.h"
#include "platform/arm_hal_interrupt.h"

static osThreadId timer_thread_id;

static Timer timer;
static Timeout timeout;
static uint32_t due;
static void (*arm_hal_callback)(void);

static void timer_thread(const void *)
{
    for (;;) {
        osSignalWait(1, osWaitForever);
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
    static osThreadDef(timer_thread, osPriorityRealtime, /*1,*/ 2*1024);
    timer_thread_id = osThreadCreate(osThread(timer_thread), NULL);
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
    osSignalSet(timer_thread_id, 1);
    //callback();
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

