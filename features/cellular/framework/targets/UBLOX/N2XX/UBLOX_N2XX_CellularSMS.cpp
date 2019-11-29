/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#if MBED_CONF_CELLULAR_USE_SMS

#include "UBLOX_N2XX_CellularSMS.h"

using namespace mbed;

UBLOX_N2XX_CellularSMS::UBLOX_N2XX_CellularSMS(ATHandler &atHandler, AT_CellularDevice &device) : AT_CellularSMS(atHandler, device)
{
}

UBLOX_N2XX_CellularSMS::~UBLOX_N2XX_CellularSMS()
{
}

nsapi_error_t UBLOX_N2XX_CellularSMS::set_cpms(const char *memr, const char *memw, const char *mems)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_size_or_error_t UBLOX_N2XX_CellularSMS::set_cscs(const char *chr_set)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_size_or_error_t UBLOX_N2XX_CellularSMS::get_sms(char *buf, uint16_t len, char *phone_num, uint16_t phone_len,
                                                      char *time_stamp, uint16_t time_len, int *buf_size)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

nsapi_error_t UBLOX_N2XX_CellularSMS::delete_all_messages()
{
    return NSAPI_ERROR_UNSUPPORTED;
}

#endif // MBED_CONF_CELLULAR_USE_SMS
