/*
 * Copyright (c) 2019 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdbool.h>

#include "cmsis_os2.h"
#include "rtx_os.h"

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
static osRtxMutex_t ns_lock_mutex_obj;
static const osMutexAttr_t ns_lock_attrib = {
    .name = "ns_lock",
    .attr_bits = osMutexPrioInherit,
    .cb_size = sizeof(osRtxMutex_t),
    .cb_mem = &ns_lock_mutex_obj
};

/**
 * \brief NS world, NS lock based dispatcher
 */
uint32_t tfm_ns_lock_dispatch(veneer_fn fn,
                              uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3)
{
    uint32_t result;
    osStatus_t status;

    /* Check the NS lock has been initialized */
    if (ns_lock.init == false) {
        return TFM_ERROR_GENERIC;
    }

    /* TFM request protected by NS lock */
    status = osMutexAcquire(ns_lock.id,osWaitForever);
    if (status != osOK) {
        return TFM_ERROR_GENERIC;
    }

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
        if (ns_lock.id == NULL) {
            return TFM_ERROR_GENERIC;
        }
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
