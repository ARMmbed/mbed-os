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
#ifndef MBED_RESET_REASON_H
#define MBED_RESET_REASON_H

#ifdef DEVICE_RESET_REASON

#include "reset_reason_api.h"

namespace mbed {
/** \addtogroup drivers-public-api */
/** @{*/

/**
 * \defgroup drivers_ResetReason ResetReason class
 * @{
 */
/** A platform-independent method of checking the cause of the last system reset.
 *
 * When the system restarts, the reason for the restart is contained in
 * the system registers at boot time in a platform specific manner.
 * This API provides a generic method of fetching the reason for the restart.
 *
 */
class ResetReason {
public:
    /** Get the platform independent reason code for the last system reset.
     *
     * @return enum containing the last reset reason for the board.
     *
     *  Example:
     *  @code
     *  const reset_reason_t reason = ResetReason::get();
     *
     *  if (reason == RESET_REASON_WATCHDOG) {
     *      printf("Watchdog reset\n");
     *      rollback();
     *  }
     *  @endcode
     */
    static reset_reason_t get();

    /** Get the platform specific reason code for the last system reset.
     *
     * Platform specific reasons that are not covered by the ::reset_reason_t enum
     * will cause the ResetReason::get() function to return
     * ::RESET_REASON_PLATFORM. In order to get the actual reason the register
     * value must be fetched directly using this function and interpreted in a
     * platform specific manner.
     *
     * @return value containing the reset reason register for the given platform.
     * If the platform contains reset reasons across multiple registers they
     * will be concatenated here.
     *
     * Example:
     * @code
     * if (ResetReason::get() == RESET_REASON_PLATFORM) {
     *     const uint32_t platform_reason = ResetReason::get_raw();
     * }
     * @endcode
     */
    static uint32_t get_raw();
};

/** @}*/
/** @}*/

} // namespace mbed

#endif // DEVICE_RESET_REASON
#endif // MBED_RESET_REASON_H
