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
#ifndef MBED_LOWPOWERTIMEOUT_H
#define MBED_LOWPOWERTIMEOUT_H

#include "platform/platform.h"

#if DEVICE_LPTICKER || defined(DOXYGEN_ONLY)

#include "hal/lp_ticker_api.h"
#include "drivers/LowPowerTicker.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \addtogroup drivers_LowPowerTimeout LowPowerTimeout class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** Low Power Timout
 *
 * @note Synchronization level: Interrupt safe
 */
class LowPowerTimeout : public LowPowerTicker, private NonCopyable<LowPowerTimeout> {
#if !defined(DOXYGEN_ONLY)
private:
    virtual void handler(void)
    {
        _function.call();
    }
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
