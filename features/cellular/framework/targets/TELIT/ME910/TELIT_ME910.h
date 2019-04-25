/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef CELLULAR_TARGETS_TELIT_ME910_TELIT_ME910_H_
#define CELLULAR_TARGETS_TELIT_ME910_TELIT_ME910_H_

#ifdef TARGET_FF_ARDUINO
#ifndef MBED_CONF_TELIT_ME910_TX
#define MBED_CONF_TELIT_ME910_TX D1
#endif
#ifndef MBED_CONF_TELIT_ME910_RX
#define MBED_CONF_TELIT_ME910_RX D0
#endif
#endif /* TARGET_FF_ARDUINO */

#include "AT_CellularDevice.h"

//the delay between sending AT commands
#define DEFAULT_DELAY_BETWEEN_AT_COMMANDS 20

namespace mbed {

class TELIT_ME910 : public AT_CellularDevice {
public:
    TELIT_ME910(FileHandle *fh);

protected: // AT_CellularDevice
    virtual uint16_t get_send_delay() const;
    virtual nsapi_error_t init();
    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();
};
} // namespace mbed
#endif /* CELLULAR_TARGETS_TELIT_ME910_TELIT_ME910_H_ */
