/***************************************************************************//**
* \file cyhal_system.h
*
* \brief
* Provides a high level interface for interacting with the Cypress power
* management and system clock configuration. This interface abstracts out the
* chip specific details. If any chip specific functionality is necessary, or
* performance is critical the low level functions can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
* \addtogroup group_hal_system System (Power Management and System Clock)
* \ingroup group_hal
* \{
* High level interface for interacting with the power management
* and system clock configuration.
*
* This driver provides three categories of functionality:
* * Retrieval and adjustment of system clock frequencies.
* * Control over low power operating modes.
* * The ability to disable interrupts during a critical section, and to renable them afterwards.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_ERROR (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 0))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_INVALID_CLK_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 1))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_UNABLE_TO_SET_CLK_FREQ (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 2))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_SRC_CLK_DISABLED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 3))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_NO_VALID_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 4))

/** Flags enum of possible system reset causes */
typedef enum
{
    CYHAL_SYSTEM_RESET_NONE            = 0,      /** No cause */
    CYHAL_SYSTEM_RESET_WDT             = 1 << 0, /** A watchdog timer (WDT) reset has occurred */
    CYHAL_SYSTEM_RESET_ACTIVE_FAULT    = 1 << 1, /** The fault logging system requested a reset from its Active logic. */
    CYHAL_SYSTEM_RESET_DEEPSLEEP_FAULT = 1 << 2, /** The fault logging system requested a reset from its Deep-Sleep logic. */
    CYHAL_SYSTEM_RESET_SOFT            = 1 << 3, /** The CPU requested a system reset through it's SYSRESETREQ. */
    CYHAL_SYSTEM_RESET_HIB_WAKEUP      = 1 << 4, /** A reset has occurred due to a a wakeup from hibernate power mode. */
    CYHAL_SYSTEM_RESET_WCO_ERR         = 1 << 5, /** A reset has occurred due to a watch-crystal clock error */
    CYHAL_SYSTEM_RESET_SYS_CLK_ERR     = 1 << 6, /** A reset has occurred due to a system clock error */
} cyhal_reset_reason_t;

/** Enter a critical section
 *
 * Disables interrupts and returns a value indicating whether the interrupts were previously
 * enabled.
 *
 * @return Returns the state before entering the critical section. This value must be provided
 * to \ref cyhal_system_critical_section_exit() to properly restore the state
 */
uint32_t cyhal_system_critical_section_enter(void);

/** Exit a critical section
 *
 * Re-enables the interrupts if they were enabled before
*  cyhal_system_critical_section_enter() was called. The argument should be the value
*  returned from \ref cyhal_system_critical_section_enter().
 *
 * @param[in] oldState The state of interrupts from cyhal_system_critical_section_enter()
 */
void cyhal_system_critical_section_exit(uint32_t oldState);

/** Send the device to sleep
 *
 *
 * The processor is setup ready for sleep, and sent to sleep using __WFI(). In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs.
 *
 * @return Returns CY_RSLT_SUCCESS if the processor successfully entered and exited sleep,
 * otherwise error
 */
cy_rslt_t cyhal_system_sleep(void);

/** Send the device to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and clocks. All state
 * is still maintained.
 *
 * @return Returns CY_RSLT_SUCCESS if the processor successfully entered and exited sleep,
 * otherwise error
 */
cy_rslt_t cyhal_system_deepsleep(void);

/** Register the specified handler with the power manager to be notified when the power
 *  state changes.
 *
 * @param[in] callback The handler to notify on power transitions
 * @return The status of the register_callback request
 */
cy_rslt_t cyhal_system_register_callback(cyhal_system_callback_t *callback);

/** Removes the specified handler from the power manager so no future notification are made.
 *
 * @param[in] callback The handler to remove from receiving notifications
 * @return The status of the unregister_callback request
 */
cy_rslt_t cyhal_system_unregister_callback(cyhal_system_callback_t const *callback);

/**
 * Requests that the current operation delays for at least the specified length of time.
 * If this is running in an RTOS aware environment (-DCY_RTOS_AWARE) it will attempt to
 * have the RTOS suspend the current task so others can continue to run. If this is not
 * run under an RTOS it will then defer to the standard system delay which is likely to
 * be a busy loop.
 * If this is part of an application that is build with RTOS awareness, but the delay
 * should not depend on the RTOS for whatever reason, use cyhal_system_delay_us() with
 * the appropriate 1000x multiplier to the delay time.
 *
 * @param[in] milliseconds The number of milliseconds to delay for
 * @return Returns CY_RSLT_SUCCESS if the delay request was successful, otherwise error
 */
cy_rslt_t cyhal_system_delay_ms(uint32_t milliseconds);

/**
 * Requests that the current operation delay for at least the specified number of
 * micro-seconds. This will generally keep the processor active in a loop for the
 * specified length of time. If this is running under an RTOS, it will NOT attempt to
 * run any other RTOS tasks, however if the scheduler or a high priority interrupt
 * comes it they can take over anyway.
 *
 * @param[in] microseconds The number of micro-seconds to delay for
 */
void cyhal_system_delay_us(uint16_t microseconds);

/** Gets the specified clock's current frequency.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[out] frequency_hz The frequency the clock is currently running at
 * @return The status of the get_frequency request
 */
cy_rslt_t cyhal_system_clock_get_frequency(uint8_t clock, uint32_t *frequency_hz);

/** Sets the specified clock's frequency and enables it.
 *  This will turn on any additional clocks needed to drive this.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[in]  frequency_hz The frequency to run the clock at
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_system_clock_set_frequency(uint8_t clock, uint32_t frequency_hz);

/** Divides the clock frequency by the divider
 *
 * @param[in] clock   The clock to configure divider value for
 * @param[in] divider The divider value to divide the frequency by
 * @return The status of the set_divider request
 */
cy_rslt_t cyhal_system_clock_set_divider(cyhal_system_clock_t clock, cyhal_system_divider_t divider);

/** Gets the cause of the latest reset or resets that occured in the system.
 *
 * @return Returns an enum of flags with the cause of the last reset(s)
 */
cyhal_reset_reason_t cyhal_system_get_reset_reason(void);

/** Clears the reset cause registers */
void cyhal_system_clear_reset_reason(void);

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_SYSTEM_IMPL_HEADER
#include CYHAL_SYSTEM_IMPL_HEADER
#endif /* CYHAL_SYSTEM_IMPL_HEADER */

/** \} group_hal_system */
