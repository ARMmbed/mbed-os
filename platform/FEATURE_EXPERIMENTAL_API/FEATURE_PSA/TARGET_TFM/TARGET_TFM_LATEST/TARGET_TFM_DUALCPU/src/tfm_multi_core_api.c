/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_api.h"
#include "tfm_mailbox.h"
#include "tfm_multi_core_api.h"
#include "cmsis_os2.h"

#define MAX_SEMAPHORE_COUNT            NUM_MAILBOX_QUEUE_SLOT

static osSemaphoreId_t ns_lock_handle = NULL;

__attribute__((weak))
enum tfm_status_e tfm_ns_interface_init(void)
{
    osSemaphoreAttr_t sema_attrib = {0};

    ns_lock_handle = osSemaphoreNew(MAX_SEMAPHORE_COUNT,
                                    MAX_SEMAPHORE_COUNT,
                                    &sema_attrib);
    if (!ns_lock_handle) {
        return TFM_ERROR_GENERIC;
    }

    return TFM_SUCCESS;
}

int32_t tfm_ns_wait_for_s_cpu_ready(void)
{
    return tfm_platform_ns_wait_for_s_cpu_ready();
}

uint32_t tfm_ns_multi_core_lock_acquire(void)
{
    return osSemaphoreAcquire(ns_lock_handle, osWaitForever);
}

uint32_t tfm_ns_multi_core_lock_release(void)
{
    return osSemaphoreRelease(ns_lock_handle);
}
