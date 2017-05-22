/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#if DEVICE_LOWPOWERTIMER

#include "hal/lp_ticker_api.h"

namespace mbed {
/** \addtogroup drivers */
/** @{*/

/** Low Power Ticker
 *
 * @Note Synchronization level: Interrupt safe
 */
class LowPowerTicker : public Ticker {

public:
    LowPowerTicker() : Ticker(get_lp_ticker_data()) {
    }

    virtual ~LowPowerTicker() {
    }
};

} // namespace mbed

#endif

#endif

/** @}*/
