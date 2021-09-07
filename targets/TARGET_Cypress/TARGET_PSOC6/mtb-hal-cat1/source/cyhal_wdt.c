/***************************************************************************//**
* \file cyhal_wdt.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Watchdog Timer.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
*
********************************************************************************
* \copyright
* Copyright 2019-2021 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include <stdbool.h>
#include "cyhal_wdt.h"
#include "cyhal_wdt_impl.h"
#include "cy_wdt.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(COMPONENT_CAT1A) || defined(COMPONENT_CAT1B)
#define _cyhal_wdt_lock()   Cy_WDT_Lock()
#define _cyhal_wdt_unlock() Cy_WDT_Unlock()
#elif defined(COMPONENT_CAT2)
#define _cyhal_wdt_lock()
#define _cyhal_wdt_unlock()
#endif

static bool _cyhal_wdt_initialized = false;
static bool _cyhal_wdt_pdl_initialized = false;

static uint16_t _cyhal_wdt_initial_timeout_ms = 0;

static uint16_t _cyhal_wdt_rounded_timeout_ms = 0;
static uint32_t _cyhal_wdt_ignore_bits = 0;

// Rounds up *timeout_ms if it's outside of the valid timeout ranges (see above)
static __INLINE uint32_t _cyhal_wdt_timeout_to_ignore_bits(uint32_t *timeout_ms)
{
    for (uint32_t i = 0; i <= _CYHAL_WDT_MAX_IGNORE_BITS; i++)
    {
        if (*timeout_ms >= _cyhal_wdt_ignore_data[i].round_threshold_ms)
        {
            if (*timeout_ms < _cyhal_wdt_ignore_data[i].min_period_ms)
            {
                *timeout_ms = _cyhal_wdt_ignore_data[i].min_period_ms;
            }
            return i;
        }
    }
    return _CYHAL_WDT_MAX_IGNORE_BITS; // Should never reach this
}

static __INLINE uint16_t _cyhal_wdt_timeout_to_match(uint16_t timeout_ms, uint16_t ignore_bits)
{
    uint32_t timeout = (uint32_t)timeout_ms * CY_SYSCLK_ILO_FREQ / 1000;
    return (uint16_t)(timeout - (1UL << (17 - ignore_bits)) + Cy_WDT_GetCount());
}

/* Start API implementing */

cy_rslt_t cyhal_wdt_init(cyhal_wdt_t *obj, uint32_t timeout_ms)
{
    if (timeout_ms == 0 || timeout_ms > _CYHAL_WDT_MAX_TIMEOUT_MS)
    {
        return CY_RSLT_WDT_INVALID_TIMEOUT;
    }

    if (_cyhal_wdt_initialized)
    {
        return CY_RSLT_WDT_ALREADY_INITIALIZED;
    }

    _cyhal_wdt_initialized = true;

    cyhal_wdt_stop(obj); // Stop and unlock before doing other work

    if (!_cyhal_wdt_pdl_initialized)
    {
        Cy_WDT_ClearInterrupt();
        Cy_WDT_MaskInterrupt();
        _cyhal_wdt_pdl_initialized = true;
    }

    _cyhal_wdt_initial_timeout_ms = timeout_ms;

    _cyhal_wdt_ignore_bits = _cyhal_wdt_timeout_to_ignore_bits(&timeout_ms);
    _cyhal_wdt_rounded_timeout_ms = timeout_ms;

    Cy_WDT_SetIgnoreBits(_cyhal_wdt_ignore_bits);

    Cy_WDT_SetMatch(_cyhal_wdt_timeout_to_match(_cyhal_wdt_rounded_timeout_ms, _cyhal_wdt_ignore_bits));

    cyhal_wdt_start(obj);

    return CY_RSLT_SUCCESS;
}

void cyhal_wdt_free(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    cyhal_wdt_stop(obj);

    _cyhal_wdt_initialized = false;
}

void cyhal_wdt_kick(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);

    /* Clear to prevent reset from WDT */
    Cy_WDT_ClearWatchdog();

    _cyhal_wdt_unlock();
    Cy_WDT_SetMatch(_cyhal_wdt_timeout_to_match(_cyhal_wdt_rounded_timeout_ms, _cyhal_wdt_ignore_bits));
    _cyhal_wdt_lock();
}

void cyhal_wdt_start(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    _cyhal_wdt_unlock();
    Cy_WDT_Enable();
    _cyhal_wdt_lock();
}

void cyhal_wdt_stop(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    _cyhal_wdt_unlock();
    Cy_WDT_Disable();
}

uint32_t cyhal_wdt_get_timeout_ms(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    return _cyhal_wdt_initial_timeout_ms;
}

uint32_t cyhal_wdt_get_max_timeout_ms(void)
{
    return _CYHAL_WDT_MAX_TIMEOUT_MS;
}

bool cyhal_wdt_is_enabled(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    return Cy_WDT_IsEnabled();
}

#if defined(__cplusplus)
}
#endif
