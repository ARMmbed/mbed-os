/** \addtogroup hal */
/** @{*/
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

#if DEVICE_RESET_REASON

#include "reset_reason_api.h"

namespace mbed {
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup reset_reason reset reason functions
 * @{
 */

/** Get the platform-independent reason code for the last system reset.
 *
 *  Example:
 *  @code
 *  const reset_reason_t reason = mbed_reset_reason_get();
 *
 *  if (reason == RESET_REASON_WATCHDOG) {
 *      std::cout << "Watchdog reset" << std::endl;
 *      rollback();
 *  }
 *  @endcode
 */
reset_reason_t mbed_reset_reason_get(void);

/**@}*/
/**@}*/

} // namespace mbed

#endif // DEVICE_RESET_REASON

/**@}*/

#endif // MBED_RESET_REASON_H
