/***************************************************************************//**
* \file cyhal_wdt_impl.h
*
* \brief
* CAT1 specific implementation for WDT API.
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

#pragma once

/**
* \addtogroup group_hal_impl_wdt WDT (Watchdog Timer)
* \ingroup group_hal_impl
* \{
* The CAT1 (PSoC 6) WDT is only capable of supporting certain timeout ranges below its maximum timeout.
* As a result, any unsupported timeouts given to the HAL WDT are rounded up to the nearest supported value.
* The following table describes the unsupported ranges and the timeout values they are rounded to.
*
* <table class="doxtable">
*   <tr><th>Range (ms)</th><th>Rounded Timeout (ms)</th></tr>
*   <tr>
*     <td>3001 - 3999</td>
*     <td>4000</td>
*   </tr>
*   <tr>
*     <td>1501 - 1999</td>
*     <td>2000</td>
*   </tr>
*   <tr>
*     <td>751 - 999</td>
*     <td>1000</td>
*   </tr>
*   <tr>
*     <td>376 - 499</td>
*     <td>500</td>
*   </tr>
*   <tr>
*     <td>188 - 249</td>
*     <td>250</td>
*   </tr>
*   <tr>
*     <td>94 - 124</td>
*     <td>125</td>
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
*     <td>3 - 3</td>
*     <td>4</td>
*   </tr>
* </table>
* \} group_hal_impl_wdt
*/

#include "cyhal_wdt_impl_common.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
* \cond INTERNAL
*/

// ILO Frequency = 32768 Hz
// ILO Period = 1 / 32768 Hz = .030518 ms
// WDT Reset Period (timeout_ms) = .030518 ms * (2 * 2^(16 - ignore_bits) + match)
// ignore_bits range: 0 - 12
// match range: 0 - 2^(16 - ignore_bits)
static const _cyhal_wdt_ignore_bits_data_t _cyhal_wdt_ignore_data[] = {
    {4000, 3001}, //  0 bit(s): min period: 4000ms, max period: 6000ms, round up from 3001+ms
    {2000, 1501}, //  1 bit(s): min period: 2000ms, max period: 3000ms, round up from 1501+ms
    {1000,  751}, //  2 bit(s): min period: 1000ms, max period: 1500ms, round up from 751+ms
    { 500,  376}, //  3 bit(s): min period:  500ms, max period:  750ms, round up from 376+ms
    { 250,  188}, //  4 bit(s): min period:  250ms, max period:  375ms, round up from 188+ms
    { 125,   94}, //  5 bit(s): min period:  125ms, max period:  187ms, round up from 94+ms
    {  63,   47}, //  6 bit(s): min period:   63ms, max period:   93ms, round up from 47+ms
    {  32,   24}, //  7 bit(s): min period:   32ms, max period:   46ms, round up from 24+ms
    {  16,   12}, //  8 bit(s): min period:   16ms, max period:   23ms, round up from 12+ms
    {   8,    6}, //  9 bit(s): min period:    8ms, max period:   11ms, round up from 6+ms
    {   4,    3}, // 10 bit(s): min period:    4ms, max period:    5ms, round up from 3+ms
    {   2,    2}, // 11 bit(s): min period:    2ms, max period:    2ms, round up from 2+ms
    {   1,    1}, // 12 bit(s): min period:    1ms, max period:    1ms, round up from 1+ms
};

// (2^16 * 3) * .030518 ms
/** Maximum WDT timeout in milliseconds */
#define _CYHAL_WDT_MAX_TIMEOUT_MS 6000

/** Maximum number of ignore bits */
#define _CYHAL_WDT_MAX_IGNORE_BITS 12

/** \endcond */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
