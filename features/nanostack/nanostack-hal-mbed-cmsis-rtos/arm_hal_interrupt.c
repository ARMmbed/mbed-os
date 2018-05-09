/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include "arm_hal_interrupt.h"
#include "arm_hal_interrupt_private.h"

#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
#include "platform/mbed_critical.h"
#else
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#endif

#include <mbed_assert.h>


// The critical section has two alternative implementations, the default which uses mutex
// as locking primitive and the optional, which will bluntly disable interrupts
// for the critical section. The mutex version will not cause issues by delaying
// interrupts, but it has a down side that it can not be used from the interrupt
// service routines. The IRQ-safe version will do the mutual exclusion by temporarily
// disabling the interrupts, so it will have effect on interrupt latency.
#if !MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT

static uint8_t sys_irq_disable_counter;

static mbed_rtos_storage_mutex_t critical_mutex;
static const osMutexAttr_t critical_mutex_attr = {
  .name = "nanostack_critical_mutex",
  .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
  .cb_mem = &critical_mutex,
  .cb_size = sizeof critical_mutex,
};
static osMutexId_t critical_mutex_id;

#endif

void platform_critical_init(void)
{
#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    // nothing to do here
#else
    critical_mutex_id = osMutexNew(&critical_mutex_attr);
    MBED_ASSERT(critical_mutex_id);
#endif
}

void platform_enter_critical(void)
{
#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    core_util_critical_section_enter();
#else
    osMutexAcquire(critical_mutex_id, osWaitForever);
    sys_irq_disable_counter++;
#endif
}

void platform_exit_critical(void)
{
#if MBED_CONF_NANOSTACK_HAL_CRITICAL_SECTION_USABLE_FROM_INTERRUPT
    core_util_critical_section_exit();
#else
    --sys_irq_disable_counter;
    osMutexRelease(critical_mutex_id);
#endif
}
