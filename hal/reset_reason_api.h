/** \addtogroup hal */
/** @{*/
/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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
 */
#ifndef MBED_RESET_REASON_API_H
#define MBED_RESET_REASON_API_H

#if DEVICE_RESET_REASON

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_reset_reason ResetReason HAL API
 * Low-level interface to the ResetReason of a target.
 *
 * This module provides a platform-independent method of checking the cause
 * of the last system reset.
 *
 * # Defined behavior
 * * The function ::hal_reset_reason_clear clears the ResetReason registers
 * for the next system boot.
 * * By the time the user calls ::hal_reset_reason_get to read the value,
 * some other part of the application may have cleared the value. Therefore,
 * though there may have been a reset reason in the registers when the system
 * started, the reason may not be available when the user comes to check it.
 * * The function ::hal_reset_reason_get_capabilities fills the given
 * `reset_reason_capabilities_t` instance.
 *
 * # Undefined behavior
 * * There is no guarantee that the function ::hal_reset_reason_get will
 * return the same value when you call it repeatedly. Store the value for later
 * use instead of calling the function repeatedly.
 * * The function ::hal_reset_reason_clear may not clear the ResetReason
 * register in time for the next system boot.
 *
 * # Notes
 * * The contents of the targets ResetReason register may be cleared by some
 * subsystem before it first gets called. This returns a ::RESET_REASON_UNKNOWN
 * value to the user. To avoid this, the user should call the ResetReason API
 * as early as possible at boot time.
 * * If the target doesn't support clearing reset registers,
 * ::hal_reset_reason_get seems to erroneously return a reset reason even after
 * clearing.
 *
 * @{
 */

/**
 * \defgroup hal_reset_reason_tests ResetReason HAL tests
 * Greentea tests for the ResetReason HAL.
 *
 * To run the ResetReason HAL tests, use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal-reset_reason
 *
 */

/** Definitions of different reset reasons
 */
typedef enum {
    RESET_REASON_POWER_ON,       /**< Set when power is initially applied to the board. The power-on-reset circuit causes a POWER_ON reset when this occurs */
    RESET_REASON_PIN_RESET,      /**< Set when a reset is triggered by the hardware pin on the board */
    RESET_REASON_BROWN_OUT,      /**< Triggered when the voltage drops below the low voltage detect (LVD) threshold; the system is held in a reset until the voltage rises above the threshold */
    RESET_REASON_SOFTWARE,       /**< Set during software reset, typically triggered by writing the SYSRESETREQ bit in the Application Interrupt and Reset Control register */
    RESET_REASON_WATCHDOG,       /**< Set when a running watchdog timer fails to be refreshed */
    RESET_REASON_LOCKUP,         /**< Set when the core is locked because of an unrecoverable exception */
    RESET_REASON_WAKE_LOW_POWER, /**< Set when waking from deep sleep mode */
    RESET_REASON_ACCESS_ERROR,   /**< Umbrella value that encompasses any access related reset */
    RESET_REASON_BOOT_ERROR,     /**< Umbrella value that encompasses any boot related reset */
    RESET_REASON_MULTIPLE,       /**< Set if multiple reset reasons are set within the board. Occurs when the reset reason registers aren't cleared between resets */
    RESET_REASON_PLATFORM,       /**< Platform specific reset reason not captured in this enum */
    RESET_REASON_UNKNOWN         /**< Unknown or unreadable reset reason **/
} reset_reason_t;

/** Reset reason capabilities of the platform
 */
typedef struct {
    uint32_t reasons; /**< Supported reset reasons. Each bit represents a corresponding reset_reason_t value.**/
} reset_reason_capabilities_t;

/** Fetch the reset reason for the last system reset.
 *
 * This function must return the contents of the system reset reason registers
 * cast to an appropriate platform independent reset reason. If multiple reset
 * reasons are set, this function should return ::RESET_REASON_MULTIPLE. If the
 * reset reason does not match any existing platform independent value, this
 * function should return ::RESET_REASON_PLATFORM. If no reset reason can be
 * determined, this function should return ::RESET_REASON_UNKNOWN.
 *
 * This function is not idempotent; there is no guarantee the system
 * reset reason will not be cleared between calls to this function altering the
 * return value between calls.
 *
 * Note: Some platforms contain reset reason registers that persist through
 * system resets. If the registers haven't been cleared before calling this
 * function, multiple reasons may be set within the registers. If multiple reset
 * reasons are detected, this function returns ::RESET_REASON_MULTIPLE.
 *
 * @return enum containing the last reset reason for the board.
 */
reset_reason_t hal_reset_reason_get(void);


/** Fetch the raw platform specific reset reason register value.
 *
 * This function must return the raw contents of the system reset reason
 * registers cast to a uint32_t value. If the platform contains reset reasons
 * that span multiple registers/addresses, the value should be concatenated into
 * the return type.
 *
 * This function is not idempotent; there is no guarantee the system
 * reset reason will not be cleared between calls to this function altering the
 * return value between calls.
 *
 * @return value containing the reset reason register for the given platform.
 *         If the platform contains reset reasons across multiple registers, they
 *         will be concatenated here.
 */
uint32_t hal_reset_reason_get_raw(void);

/** Clear the reset reason from registers.
 *
 * Reset the value of the reset status registers. The reset reason persists
 * between system resets on certain platforms, so the registers should be cleared
 * before the system resets. Failing to do so may make it difficult to determine
 * the cause of any subsequent system resets.
 */
void hal_reset_reason_clear(void);

/** Fill the given reset_reason_capabilities_t instance according to platform capabilities.
 */
void hal_reset_reason_get_capabilities(reset_reason_capabilities_t *cap);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif // DEVICE_RESET_REASON

#endif // MBED_RESET_REASON_API_H

/** @}*/
