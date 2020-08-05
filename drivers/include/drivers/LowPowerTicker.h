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
#ifndef MBED_LOWPOWERTICKER_H
#define MBED_LOWPOWERTICKER_H

#include "platform/platform.h"
#include "drivers/Ticker.h"

#if defined (DEVICE_LPTICKER) || defined(DOXYGEN_ONLY)

#include "hal/lp_ticker_api.h"

namespace mbed {
/** \defgroup drivers-public-api-ticker Ticker
 * \ingroup drivers-public-api
 */

/**
 * \defgroup drivers_LowPowerTicker LowPowerTicker class
 * \ingroup drivers-public-api-ticker
 * @{
 */

/** Low Power Ticker
 *
 * @note Synchronization level: Interrupt safe
 */
class LowPowerTicker : public TickerBase {
public:
    LowPowerTicker();
};

/** @}*/

} // namespace mbed

#endif

#endif
