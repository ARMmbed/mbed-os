/*******************************************************************************
* File Name: cyhal_clock_impl.h
*
* Description:
* CAT1 specific implementation for clocks API.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
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

#include "cyhal_hw_resources.h"

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * \addtogroup group_hal_impl_clock Clocks
 * \ingroup group_hal_impl
 * \{
 * Implementation specific interface for using the Clock driver. These items, while usable
 * within the HAL, are <b>not</b> necessarily portable between devices. The diagram below
 * shows how the clocks relate to each other. This is a superset of what is available. See
 * the device specific Data Sheet for the exact set of clocks that are available on a
 * specific device.
 *
 * \image html mxs40_clock_tree.png
 *
 * \section section_clock_snippets_impl Code snippets
 * \note Error handling code has been intentionally left out of snippets to highlight API usage.
 *
 * \subsection subsection_clock_snippet_5_impl Snippet: System initialization
 * The following snippet shows the clock driver can be used to initialize all clocks in the system.
 * \note This example is device specific.
 * \snippet hal_clock.c snippet_cyhal_clock_system_init_p6
 */

/** \cond INTERNAL */
#define SRSS_MFO_PRESENT    (SRSS_VER1P3)
/** \endcond */

/** Internal Main Oscillator: This is a fixed-frequency clock that is commonly used as a general purpose source for clocks that do not require specific frequencies or very high accuracy. This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_IMO;
/** External Clock: This is an off-chip clock (not an oscillator). This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_EXT;
/** Internal Low Speed Oscillator: This is a low accuracy fixed-frequency clock in the kilohertz range that is available in sleep, deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_ILO;
#if SRSS_ECO_PRESENT
/** External Crystal Oscillator: This is an off-chip clock source that is used when specific frequencies and/or very high accuracy is required. This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_ECO;
#endif
#if SRSS_ALTHF_PRESENT
/** Alternate High Frequency Clock: A high speed clock input provided by a subsystem outside the clocking system. This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_ALTHF;
#endif
#if SRSS_ALTLF_PRESENT
/** Alternate Low Frequency Clock: A low speed clock input provided by a subsystem outside the clocking system. This clock is the hibernate power mode. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_ALTLF;
#endif
#if SRSS_PILO_PRESENT
/** Precision ILO: An additional source that can provide a much more accurate 32.768kHz clock than ILO when periodically calibrated using a high-accuracy clock such as the ECO. This clock is stopped in the hibernate power mode. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_PILO;
#endif
#if SRSS_BACKUP_PRESENT
/** Watch Crystal Oscillator: This source is driven from an off-chip watch crystal that provides an extremely accurate source. This clock is stopped in the hibernate power mode. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_WCO;
#endif
#if SRSS_MFO_PRESENT
/** Medium Frequency Oscillator: This source produced by dividing the IMO by 4. The MFO works down to DeepSleep, and the IMO does not turn off if this clock requires it. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_MFO;
/** Medium Frequency Clock: This clock is a medium frequency, between the Low Frequency Clock (LF) and High Frequency Clock (HF). */
extern const cyhal_resource_inst_t CYHAL_CLOCK_MF;
#endif

/** Low Frequency Clock: This clock is the source for the multi-counter watchdog timers (MCWDT), and can also be a source for the RTC. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_LF;
/** Analog Pump Clock: This clock ensures precision analog performance in low voltage applications. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_PUMP;
/** Backup Clock: This clock is available to the backup domain. Typically useful if an external WCO is not available. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_BAK;
/** Fast Clock: This clock is used for the CM4 and associated AHB-Lite bus infrastructure. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_FAST;
/** Peripheral Clock: This is the source clock for any divided clock in the design. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_PERI;
/** Timer Clock: This clock is intended as a source for high-frequency timers, such as the Energy Profiler and CPU SysTick clock. This clock is stopped in the hibernate power mode. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_TIMER;
/** Slow Clock: This clock is used for the CM0+ CPU, Datawire and CRYPTO components and the associated CPUSS slow infrastructure. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_SLOW;
/** AltSysTickClk: Provides an optional external source for the CM4/CM0+ SysTick timers. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_ALT_SYS_TICK;

#if (SRSS_NUM_CLKPATH > 0)
/** Path mux selection: A clock source for high frequency clocks. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_PATHMUX[SRSS_NUM_CLKPATH];
#endif

/** Frequency-Locked Loop: This is a high-frequency clock suitable for most on-chip purposes.  It is similar to a PLL, but starts up much faster and consumes less current. This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_FLL;
#if (SRSS_NUM_PLL > 0)
/** Phase-Locked Loop: A high-frequency clock able to generate a wide range of clock frequencies making it suitable for most on-chip purposes. This clock is stopped in the deep sleep and hibernate power modes. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_PLL[SRSS_NUM_PLL];
#endif

/** High Frequency Clock: A high-frequency clock output driving specific peripherals. */
extern const cyhal_resource_inst_t CYHAL_CLOCK_HF[SRSS_NUM_HFROOT];

/** \} group_hal_impl_clock */

#if defined(__cplusplus)
}
#endif
