/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#include "arm_hal_interrupt.h"
#include "arm_hal_interrupt_private.h"
#include "cmsis_os.h"


static uint8_t sys_irq_disable_counter;

static osMutexDef(critical);
static osMutexId critical_mutex_id;

void platform_critical_init(void)
{
    critical_mutex_id = osMutexCreate(osMutex(critical));
}

void platform_enter_critical(void)
{
    osMutexWait(critical_mutex_id, osWaitForever);
    sys_irq_disable_counter++;
}

void platform_exit_critical(void)
{
    --sys_irq_disable_counter;
    osMutexRelease(critical_mutex_id);
}
