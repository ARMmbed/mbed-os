/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef ONBOARD_UBLOX_AT_
#define ONBOARD_UBLOX_AT_

#include "UBLOX_AT.h"

namespace mbed {

class ONBOARD_UBLOX_AT : public UBLOX_AT {
public:
    ONBOARD_UBLOX_AT(FileHandle *fh);

    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();
};

} // namespace mbed

#endif // ONBOARD_UBLOX_AT_
