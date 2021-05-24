/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "os_wrapper/thread.h"
#include "os_wrapper/mutex.h"
#include "os_wrapper/semaphore.h"

#include <string.h>
#include "cmsis_os2.h"

/* This is an example OS abstraction layer for CMSIS-RTOSv2 */

void *os_wrapper_thread_new(const char *name, int32_t stack_size,
                            os_wrapper_thread_func func, void *arg,
                            uint32_t priority)
{
    osThreadAttr_t task_attribs = {.tz_module = 1};

    /* By default, the thread starts as osThreadDetached */
    if (stack_size != OS_WRAPPER_DEFAULT_STACK_SIZE) {
        task_attribs.stack_size = stack_size;
    }
    task_attribs.name = name;
    task_attribs.priority = (osPriority_t) priority;

    return (void *)osThreadNew(func, arg, &task_attribs);
}

void *os_wrapper_semaphore_create(uint32_t max_count, uint32_t initial_count,
                                  const char *name)
{
    osSemaphoreAttr_t sema_attrib = {0};

    sema_attrib.name = name;

    return (void *)osSemaphoreNew(max_count, initial_count, &sema_attrib);
}

uint32_t os_wrapper_semaphore_acquire(void *handle, uint32_t timeout)
{
    osStatus_t status;

    status = osSemaphoreAcquire((osSemaphoreId_t)handle,
                                (timeout == OS_WRAPPER_WAIT_FOREVER) ?
                                osWaitForever : timeout);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_semaphore_release(void *handle)
{
    osStatus_t status;

    status = osSemaphoreRelease((osSemaphoreId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_semaphore_delete(void *handle)
{
    osStatus_t status;

    status = osSemaphoreDelete((osSemaphoreId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

void *os_wrapper_mutex_create(void)
{
    const osMutexAttr_t attr = {
        .name = NULL,
        .attr_bits = osMutexPrioInherit, /* Priority inheritance is recommended
                                          * to enable if it is supported.
                                          * For recursive mutex and the ability
                                          * of auto release when owner being
                                          * terminated is not required.
                                          */
        .cb_mem = NULL,
        .cb_size = 0U
    };

    return (void *)osMutexNew(&attr);
}

uint32_t os_wrapper_mutex_acquire(void *handle, uint32_t timeout)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexAcquire((osMutexId_t)handle,
                            (timeout == OS_WRAPPER_WAIT_FOREVER) ?
                             osWaitForever : timeout);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_mutex_release(void *handle)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexRelease((osMutexId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_mutex_delete(void *handle)
{
    osStatus_t status = osOK;

    if (!handle) {
        return OS_WRAPPER_ERROR;
    }

    status = osMutexDelete((osMutexId_t)handle);
    if (status != osOK) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

void *os_wrapper_thread_get_handle(void)
{
    return (void *)osThreadGetId();
}

uint32_t os_wrapper_thread_get_priority(void *handle, uint32_t *priority)
{
    osPriority_t prio;

    prio = osThreadGetPriority((osThreadId_t)handle);
    if (prio == osPriorityError) {
        return OS_WRAPPER_ERROR;
    }

    *priority = (uint32_t)prio;

    return OS_WRAPPER_SUCCESS;
}

void os_wrapper_thread_exit(void)
{
    osThreadExit();
}

uint32_t os_wrapper_thread_set_flag(void *handle, uint32_t flags)
{
    uint32_t ret;

    ret = osThreadFlagsSet((osThreadId_t)handle, flags);
    if (ret & osFlagsError) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

/*
 * According to the description of CMSIS-RTOS v2 Thread Flags,
 * osThreadFlagsSet() can be called inside Interrupt Service Routine.
 */
uint32_t os_wrapper_thread_set_flag_isr(void *handle, uint32_t flags)
{
    uint32_t ret;

    ret = osThreadFlagsSet((osThreadId_t)handle, flags);
    if (ret & osFlagsError) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_thread_wait_flag(uint32_t flags, uint32_t timeout)
{
    uint32_t ret;

    ret = osThreadFlagsWait(flags, osFlagsWaitAll,
                            (timeout == OS_WRAPPER_WAIT_FOREVER) ?
                            osWaitForever : timeout);
    if (ret & osFlagsError) {
        return OS_WRAPPER_ERROR;
    }

    return OS_WRAPPER_SUCCESS;
}

uint32_t os_wrapper_get_tick(void)
{
    return osKernelGetTickCount();
}

void *os_wrapper_msg_queue_create(size_t msg_size, uint8_t msg_count)
{
    osMessageQueueId_t mq_id;

    mq_id = osMessageQueueNew(msg_count, msg_size, NULL);

    return (void *)mq_id;
}

int32_t os_wrapper_msg_queue_send(void *mq_handle,
                                  const void *msg_ptr)
{
    osStatus_t status;

    status = osMessageQueuePut(mq_handle, msg_ptr, 0, 0);
    if (status == osOK) {
        return OS_WRAPPER_SUCCESS;
    }

    return OS_WRAPPER_ERROR;
}

int32_t os_wrapper_msg_queue_receive(void *mq_handle,
                                     void *msg_ptr)
{
    osStatus_t status;

    status = osMessageQueueGet(mq_handle, msg_ptr, NULL, osWaitForever);
    if (status == osOK) {
        return OS_WRAPPER_SUCCESS;
    }

    return OS_WRAPPER_ERROR;
}
