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

#if MBED_CONF_NSAPI_PRESENT

#include "ONBOARD_UBLOX_N2XX.h"
#include "UBLOX_onboard_modem_api.h"

using namespace mbed;

ONBOARD_UBLOX_N2XX::ONBOARD_UBLOX_N2XX(FileHandle *fh) : UBLOX_N2XX(fh)
{
}

nsapi_error_t ONBOARD_UBLOX_N2XX::hard_power_on()
{
    ::onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_N2XX::hard_power_off()
{
    ::onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_N2XX::soft_power_on()
{
    ::onboard_modem_power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_UBLOX_N2XX::soft_power_off()
{
    ::onboard_modem_power_down();
    return NSAPI_ERROR_OK;
}

#endif // MBED_CONF_NSAPI_PRESENT
