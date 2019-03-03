/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include "cmsis.h"
#include "cmsis_os2.h"

#include "tfm_api.h"
#include "tfm_ns_lock.h"

/**
 * \brief struct ns_lock_state type
 */
struct ns_lock_state
{
    bool        init;
    osMutexId_t id;
};

/**
 * \brief ns_lock status
 */
static struct ns_lock_state ns_lock = {.init=false, .id=NULL};

/**
 * \brief Mutex properties, NS lock
 */
static const osMutexAttr_t ns_lock_attrib = {
    .name = "ns_lock",
    .attr_bits = osMutexPrioInherit
};

/**
 * \brief NS world, NS lock based dispatcher
 */
uint32_t tfm_ns_lock_dispatch(veneer_fn fn,
                              uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3)
{
    uint32_t result;

    /* Check the NS lock has been initialized */
    if (ns_lock.init == false) {
        return TFM_ERROR_GENERIC;
    }

    /* TFM request protected by NS lock */
    osMutexAcquire(ns_lock.id,osWaitForever);

    result = fn(arg0, arg1, arg2, arg3);

    osMutexRelease(ns_lock.id);

    return result;
}

/**
 * \brief NS world, Init NS lock
 */
uint32_t tfm_ns_lock_init()
{
    if (ns_lock.init == false) {
        ns_lock.id = osMutexNew(&ns_lock_attrib);
        ns_lock.init = true;
        return TFM_SUCCESS;
    }
    else {
        return TFM_ERROR_GENERIC;
    }
}

bool tfm_ns_lock_get_init_state()
{
    return ns_lock.init;
}

