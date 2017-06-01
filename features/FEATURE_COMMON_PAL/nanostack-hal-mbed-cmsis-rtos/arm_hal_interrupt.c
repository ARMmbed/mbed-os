/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include "arm_hal_interrupt.h"
#include "arm_hal_interrupt_private.h"
#include "cmsis_os2.h"
#include "mbed_rtos_storage.h"
#include <mbed_assert.h>

static uint8_t sys_irq_disable_counter;

static mbed_rtos_storage_mutex_t critical_mutex;
static const osMutexAttr_t critical_mutex_attr = {
  .name = "nanostack_critical_mutex",
  .attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust,
  .cb_mem = &critical_mutex,
  .cb_size = sizeof critical_mutex,
};
static osMutexId_t critical_mutex_id;

void platform_critical_init(void)
{
    critical_mutex_id = osMutexNew(&critical_mutex_attr);
    MBED_ASSERT(critical_mutex_id);
}

void platform_enter_critical(void)
{
    osMutexAcquire(critical_mutex_id, osWaitForever);
    sys_irq_disable_counter++;
}

void platform_exit_critical(void)
{
    --sys_irq_disable_counter;
    osMutexRelease(critical_mutex_id);
}
