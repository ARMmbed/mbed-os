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

#ifndef UBLOX_N2XX_CELLULAR_SMS_H_
#define UBLOX_N2XX_CELLULAR_SMS_H_

#include "AT_CellularSMS.h"

namespace mbed {

class UBLOX_N2XX_CellularSMS : public AT_CellularSMS {

public:

    UBLOX_N2XX_CellularSMS(ATHandler &atHandler);
    virtual ~UBLOX_N2XX_CellularSMS();

    virtual nsapi_error_t set_cpms(const char *memr, const char *memw, const char *mems);

    virtual nsapi_size_or_error_t set_cscs(const char *chr_set);

    virtual nsapi_size_or_error_t get_sms(char *buf, uint16_t buf_len, char *phone_num, uint16_t phone_len,
                                          char *time_stamp, uint16_t time_len, int *buf_size);

    virtual nsapi_error_t delete_all_messages();

};

} // namespace mbed

#endif // UBLOX_N2XX_CELLULAR_SMS_H_
