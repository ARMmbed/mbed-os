/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#ifndef MBED_LOWPOWERTIMER_H
#define MBED_LOWPOWERTIMER_H

#include "platform/platform.h"
#include "drivers/Timer.h"
#include "platform/NonCopyable.h"

#if DEVICE_LPTICKER || defined(DOXYGEN_ONLY)

#include "hal/lp_ticker_api.h"

namespace mbed {
/**
 * \defgroup drivers_LowPowerTimer LowPowerTimer class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** Low power timer
 *
 * @note Synchronization level: Interrupt safe
 */
class LowPowerTimer : public Timer, private NonCopyable<LowPowerTimer> {

public:
    LowPowerTimer() : Timer(get_lp_ticker_data())
    {
    }

};

/** @}*/

} // namespace mbed

#endif

#endif
