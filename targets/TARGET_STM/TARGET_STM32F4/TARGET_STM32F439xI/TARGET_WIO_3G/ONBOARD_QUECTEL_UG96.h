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

#ifndef ONBOARD_QUECTEL_UG96_
#define ONBOARD_QUECTEL_UG96_

#include "QUECTEL_UG96.h"

namespace mbed {

class ONBOARD_QUECTEL_UG96 : public QUECTEL_UG96 {
public:
    ONBOARD_QUECTEL_UG96(FileHandle *fh);

    virtual nsapi_error_t hard_power_on();
    virtual nsapi_error_t hard_power_off();
    virtual nsapi_error_t soft_power_on();
    virtual nsapi_error_t soft_power_off();

private:
    void press_power_button(int time_ms);

    void onboard_modem_init();

    void onboard_modem_deinit();

    void onboard_modem_power_up();

    void onboard_modem_power_down();
};

} // namespace mbed

#endif // ONBOARD_QUECTEL_UG96_
