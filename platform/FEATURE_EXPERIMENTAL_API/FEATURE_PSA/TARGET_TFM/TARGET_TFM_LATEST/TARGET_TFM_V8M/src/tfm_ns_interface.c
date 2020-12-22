/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include "tfm_api.h"
#include "tfm_ns_interface.h"
#include "cmsis_os2.h"

/**
 * \brief the ns_lock ID
 */
static osMutexId_t ns_lock_handle = NULL;

__attribute__((weak))
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    int32_t result;
    osStatus_t status;

    /* TFM request protected by NS lock */
    status = osMutexAcquire(ns_lock_handle, osWaitForever);
    if (status != osOK) {
        return (int32_t)TFM_ERROR_GENERIC;
    }

    result = fn(arg0, arg1, arg2, arg3);

    status = osMutexRelease(ns_lock_handle);
    if (status != osOK) {
        return (int32_t)TFM_ERROR_GENERIC;
    }

    return result;
}

__attribute__((weak))
enum tfm_status_e tfm_ns_interface_init(void)
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

    ns_lock_handle = osMutexNew(&attr);
    if (!ns_lock_handle) {
        return TFM_ERROR_GENERIC;
    }

    return TFM_SUCCESS;
}
