/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "os_wrapper/semaphore.h"

#include "tfm_api.h"
#include "tfm_mailbox.h"
#include "tfm_multi_core_api.h"

#define MAX_SEMAPHORE_COUNT            NUM_MAILBOX_QUEUE_SLOT

static void *ns_lock_handle = NULL;

__attribute__((weak))
enum tfm_status_e tfm_ns_interface_init(void)
{
    ns_lock_handle = os_wrapper_semaphore_create(MAX_SEMAPHORE_COUNT,
                                                 MAX_SEMAPHORE_COUNT,
                                                 NULL);
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
    return os_wrapper_semaphore_acquire(ns_lock_handle,
                                        OS_WRAPPER_WAIT_FOREVER);
}

uint32_t tfm_ns_multi_core_lock_release(void)
{
    return os_wrapper_semaphore_release(ns_lock_handle);
}
