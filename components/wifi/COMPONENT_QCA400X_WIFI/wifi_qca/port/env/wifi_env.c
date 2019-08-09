/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_common.h"
#include "mbed_assert.h"
#include "mbed_critical.h"

/*TODO: check return value */
void a_free(void *addr, uint8_t id)
{
    // UNUSED_ARGUMENT(id);
    free(addr);
}

/* FIXME !! */
extern uint32_t g_totAlloc;
void *a_malloc(int32_t size, uint8_t id)
{
    void *addr;

    addr = (void *)malloc(size);

    if (addr != NULL) {
        /*FIXME: !!!*/
        g_totAlloc += size;
    }
    // UNUSED_ARGUMENT(id);

    return addr;
}

A_STATUS a_mutex_init(mutex_t *pMutex)
{
    osMutexAttr_t attr = { 0 };
    attr.name = "qca400x_mutex";
    attr.cb_mem = &pMutex->mutexMem;
    attr.cb_size = sizeof(pMutex->mutexMem);
    attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    pMutex->mutexHandler = osMutexNew(&attr);

    if (NULL == pMutex->mutexHandler) {
        return A_ERROR;
    } else {
        return A_OK;
    }
}

A_STATUS a_mutex_acquire(mutex_t *pMutex)
{
    osStatus status = osMutexAcquire(pMutex->mutexHandler, osWaitForever);

    if (status != osOK) {
        return A_ERROR;
    } else {
        return A_OK;
    }
}

A_STATUS a_mutex_release(mutex_t *pMutex)
{
    osStatus status = osMutexRelease(pMutex->mutexHandler);

    if (status != osOK) {
        return A_ERROR;
    } else {
        return A_OK;
    }
}

boolean a_is_mutex_valid(mutex_t *pMutex)
{
    // FIXME: check owner of mutex
    return true;
}

A_STATUS a_mutex_delete(mutex_t *pMutex)
{
    osMutexDelete(pMutex->mutexHandler);

    return A_OK;
}

A_STATUS a_event_delete(event_t *pEvent)
{
    MBED_ASSERT(pEvent);
    osEventFlagsDelete(pEvent->eventHandler);

    return A_OK;
}

A_STATUS a_event_init(event_t *pEvent, osa_event_clear_mode_t clearMode)
{
    osEventFlagsAttr_t attr = { 0 };
    attr.name = "qca400x_event_flags";
    attr.cb_mem = &pEvent->eventMem;
    attr.cb_size = sizeof(pEvent->eventMem);
    pEvent->eventHandler = osEventFlagsNew(&attr);

    if (pEvent->eventHandler) {
        pEvent->clearMode = clearMode;
        return A_OK;
    } else {
        return A_ERROR;
    }
}

A_STATUS a_event_clear(event_t *pEvent, uint32_t flagsToClear)
{
    MBED_ASSERT(pEvent);

    osEventFlagsClear(pEvent->eventHandler, flagsToClear);

    return A_OK;
}

A_STATUS a_event_set(event_t *pEvent, uint32_t flagsToSet)
{
    MBED_ASSERT(pEvent);

    osEventFlagsSet(pEvent->eventHandler, flagsToSet);

    return A_OK;
}

A_STATUS a_event_wait(
    event_t *pEvent, uint32_t flagsToWait, boolean waitAll, uint32_t timeout, uint32_t *setFlags)
{
    MBED_ASSERT(pEvent);
    uint32_t flagsSave;
    uint32_t clearMode;

    if (pEvent->eventHandler == NULL) {
        return A_ERROR;
    }

    clearMode = (kEventAutoClear == pEvent->clearMode) ? 0 : osFlagsNoClear;

    if (waitAll) {
        flagsSave = osEventFlagsWait(pEvent->eventHandler, flagsToWait, osFlagsWaitAll | clearMode, timeout);
    } else {
        flagsSave = osEventFlagsWait(pEvent->eventHandler, flagsToWait, osFlagsWaitAny | clearMode, timeout);
    }

    *setFlags = flagsSave & flagsToWait;

    if ((flagsSave & osFlagsError) || !(flagsSave & flagsToWait)) {
        return A_TIMEOUT; // TODO: unify with caller
    } else {
        return A_OK;

    }
}

uint32_t a_time_get_msec(void)
{
    uint32_t ticks;

    ticks = osKernelGetTickCount();

    return TICKS_TO_MSEC(ticks);
}

void OSA_EnterCritical(osa_critical_section_mode_t mode)
{
    core_util_critical_section_enter();
}

void OSA_ExitCritical(osa_critical_section_mode_t mode)
{
    core_util_critical_section_exit();
}
