/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef ONBOARD_QUECTEL_EG25_H
#define ONBOARD_QUECTEL_EG25_H

#include "QUECTEL_EC2X.h"

namespace mbed {

class ONBOARD_QUECTEL_EG25 : public QUECTEL_EC2X {
public:
    ONBOARD_QUECTEL_EG25(FileHandle *fh, PinName pwr, bool active_high, PinName rst);

    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();

private:
    uint8_t initialized;
    DigitalOut* ls;
    InterruptIn* net_status;
    void link_status();
    void press_power_button(int time_ms);
    void onboard_modem_init();
    void onboard_modem_deinit();
    void onboard_modem_power_up();
    void onboard_modem_power_down();
    void onboard_modem_reset();
};

} // namespace mbed

#endif // ONBOARD_QUECTEL_EG25_H
