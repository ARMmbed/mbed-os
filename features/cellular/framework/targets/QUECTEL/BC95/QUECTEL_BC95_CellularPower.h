/* Copyright (c) 2017 ARM Limited
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

#ifndef TELIT_HE910_CELLULAR_POWER_H_
#define TELIT_HE910_CELLULAR_POWER_H_

#include "AT_CellularPower.h"

namespace mbed {

class QUECTEL_BC95_CellularPower : public AT_CellularPower
{
public:
    QUECTEL_BC95_CellularPower(ATHandler &atHandler);
    virtual ~QUECTEL_BC95_CellularPower();

public: //from CellularPower
    /**
     * Set AT command mode.
     * @remark must be called after power on to prepare correct AT mode
     * @return blocking until success or failure
     */
    virtual nsapi_error_t set_at_mode();

    /**
   * Reset and wake-up cellular device.
   * @return zero on success
   */
  virtual nsapi_error_t reset();
};
} // namespace mbed
#endif // TELIT_HE910_CELLULAR_POWER_H_
