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
* Copyright 2019-2020 Cypress Semiconductor Corporation
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

/**
* \addtogroup group_hal_psoc6_wdt WDT (Watchdog Timer)
* \ingroup group_hal_psoc6
* \{
* The PSoC 6 WDT is only capable of supporting certain timeout ranges below its maximum timeout of 6000ms.
* As a result, any unsupported timeouts given to the HAL WDT are rounded up to the nearest supported value.
* The following table describes the unsupported ranges and the timeout values they are rounded to.
* <table class="doxtable">
*   <tr><th>Range (ms)</th><th>Rounded Timeout (ms)</th></tr>
*   <tr>
*     <td>3001 - 4000</td>
*     <td>4001</td>
*   </tr>
*   <tr>
*     <td>1500 - 2000</td>
*     <td>2001</td>
*   </tr>
*   <tr>
*     <td>750 - 1000</td>
*     <td>1001</td>
*   </tr>
*   <tr>
*     <td>375 - 500</td>
*     <td>501</td>
*   </tr>
*   <tr>
*     <td>188 - 250</td>
*     <td>251</td>
*   </tr>
*   <tr>
*     <td>94 - 125</td>
*     <td>126</td>
*   </tr>
*   <tr>
*     <td>47 - 62</td>
*     <td>63</td>
*   </tr>
*   <tr>
*     <td>24 - 31</td>
*     <td>32</td>
*   </tr>
*   <tr>
*     <td>12 - 15</td>
*     <td>16</td>
*   </tr>
*   <tr>
*     <td>6 - 7</td>
*     <td>8</td>
*   </tr>
*   <tr>
*     <td>3</td>
*     <td>4</td>
*   </tr>
* </table>
* \} group_hal_psoc6_wdt
*/

#include <stdbool.h>
#include "cyhal_wdt.h"
#include "cy_wdt.h"
#include "cy_lvd.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

// ((2^16 * 2) + (2^16 - 1)) * .030518 ms
/** Maximum WDT timeout in milliseconds */
#define CYHAL_WDT_MAX_TIMEOUT_MS 6000

/** Maximum number of ignore bits */
#define CYHAL_WDT_MAX_IGNORE_BITS 12

typedef struct {
    uint32_t min_period_ms; // Minimum period in milliseconds that can be represented with this many ignored bits
    uint32_t round_threshold_ms; // Timeout threshold in milliseconds from which to round up to the minimum period
    // uint32_t pre_match_cycles; // The number of clock cycles in the first two full counter cycles (before the match value matters)
} cyhal_wdt_internal_ignore_bits_data_t;

// ILO Frequency = 32768 Hz
// ILO Period = 1 / 32768 Hz = .030518 ms
// WDT Reset Period (timeout_ms) = .030518 ms * (2 * 2^(16 - ignore_bits) + match)
// ignore_bits range: 0 - 12
// match range: 0 - (2^(16 - ignore_bits) - 1)
static const cyhal_wdt_internal_ignore_bits_data_t cyhal_wdt_internal_ignore_bits_data[] = {
    {4001, 3001}, // 0 bits:  min period: 4001ms, max period: 6000ms, round up from 3001+ms
    {2001, 1500}, // 1 bit:   min period: 2001ms, max period: 3000ms, round up from 1500+ms
    {1001, 750},  // 2 bits:  min period: 1001ms, max period: 1499ms, round up from 750+ms
    {501,  375},  // 3 bits:  min period: 501ms,  max period: 749ms,  round up from 375+ms
    {251,  188},  // 4 bits:  min period: 251ms,  max period: 374ms,  round up from 188+ms
    {126,  94},   // 5 bits:  min period: 126ms,  max period: 187ms,  round up from 94+ms
    {63,   47},   // 6 bits:  min period: 63ms,   max period: 93ms,   round up from 47+ms
    {32,   24},   // 7 bits:  min period: 32ms,   max period: 46ms,   round up from 24+ms
    {16,   12},   // 8 bits:  min period: 16ms,   max period: 23ms,   round up from 12+ms
    {8,    6},    // 9 bits:  min period: 8ms,    max period: 11ms,   round up from 6+ms
    {4,    3},    // 10 bits: min period: 4ms,    max period: 5ms,    round up from 3+ms
    {2,    2},    // 11 bits: min period: 2ms,    max period: 2ms
    {1,    1}     // 12 bits: min period: 1ms,    max period: 1ms
};

static bool cyhal_wdt_initialized = false;
static bool cyhal_wdt_pdl_initialized = false;
static uint32_t cyhal_wdt_initial_timeout_ms = 0;

static __INLINE uint32_t cyhal_wdt_timeout_to_ignore_bits(uint32_t *timeout_ms) {
    for (uint32_t i = 0; i <= CYHAL_WDT_MAX_IGNORE_BITS; i++)
    {
        if (*timeout_ms >= cyhal_wdt_internal_ignore_bits_data[i].round_threshold_ms)
        {
            if (*timeout_ms < cyhal_wdt_internal_ignore_bits_data[i].min_period_ms)
                *timeout_ms = cyhal_wdt_internal_ignore_bits_data[i].min_period_ms;
            return i;
        }
    }
    return CYHAL_WDT_MAX_IGNORE_BITS; // Should never reach this
}

static __INLINE uint32_t cyhal_wdt_timeout_to_match(uint32_t timeout_ms, uint32_t ignore_bits)
{
    // match = (timeout_ms / .030518 ms) - (2 * 2^(16 - ignore_bits))
    return (uint32_t)(timeout_ms / .030518) - (1UL << (17 - ignore_bits));
}

/* Start API implementing */

cy_rslt_t cyhal_wdt_init(cyhal_wdt_t *obj, uint32_t timeout_ms)
{
    if (timeout_ms == 0 || timeout_ms > CYHAL_WDT_MAX_TIMEOUT_MS)
    {
        return CY_RSLT_WDT_INVALID_TIMEOUT;
    }

    if (cyhal_wdt_initialized)
    {
        return CY_RSLT_WDT_ALREADY_INITIALIZED;
    }

    cyhal_wdt_initialized = true;

    if (!cyhal_wdt_pdl_initialized)
    {
        Cy_WDT_Init();
        Cy_WDT_MaskInterrupt();

        cyhal_wdt_pdl_initialized = true;
    }

    cyhal_wdt_stop(obj);

    cyhal_wdt_initial_timeout_ms = timeout_ms;
    uint32_t ignore_bits = cyhal_wdt_timeout_to_ignore_bits(&timeout_ms);
    Cy_WDT_SetIgnoreBits(ignore_bits);
    Cy_WDT_SetMatch(cyhal_wdt_timeout_to_match(timeout_ms, ignore_bits));

    cyhal_wdt_start(obj);

    return CY_RSLT_SUCCESS;
}

void cyhal_wdt_free(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    cyhal_wdt_stop(obj);

    cyhal_wdt_initialized = false;
}

void cyhal_wdt_kick(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    Cy_WDT_ClearWatchdog();
}

void cyhal_wdt_start(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    Cy_WDT_Unlock();
    Cy_WDT_Enable();
    Cy_WDT_Lock();
}

void cyhal_wdt_stop(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    Cy_WDT_Unlock();
    Cy_WDT_Disable();
}

uint32_t cyhal_wdt_get_timeout_ms(cyhal_wdt_t *obj)
{
    CY_UNUSED_PARAMETER(obj);
    return cyhal_wdt_initial_timeout_ms;
}

uint32_t cyhal_wdt_get_max_timeout_ms(void)
{
    return CYHAL_WDT_MAX_TIMEOUT_MS;
}

#if defined(__cplusplus)
}
#endif
