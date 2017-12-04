/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

namespace mbed
{
/** \addtogroup drivers */
/** ResetReason API. When the system is restarted, the reason for the restart is
 * contained in the system registers at boot time in a platform specific manner,
 * this API provides a generic method of fetching the reason for the restart.
 *
 * @ingroup drivers
 */
class ResetReason
{
public:
    /** Get the platform-independent reason code for the last system reset.
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
     * Platform specific reasons that are not covered by the reset_reason_t enum
     * will cause the ResetReason::get() function to return
     * RESET_REASON_PLATFORM. In order to get the actual reason the register
     * value must be fetched directly using this function and interpreted in a
     * platform specific manner.
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

} // namespace mbed

#endif // DEVICE_RESET_REASON
#endif // MBED_RESET_REASON_H
