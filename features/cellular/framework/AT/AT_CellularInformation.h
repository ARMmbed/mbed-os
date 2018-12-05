/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef AT_CELLULAR_INFORMATION_H_
#define AT_CELLULAR_INFORMATION_H_

#include "CellularInformation.h"
#include "AT_CellularBase.h"

namespace mbed {

/**
 * Class AT_CellularInformation
 *
 * Class that provides information about cellular device.
 */
class AT_CellularInformation : public CellularInformation, public AT_CellularBase {
public:
    AT_CellularInformation(ATHandler &atHandler);
    virtual ~AT_CellularInformation();

public:
    virtual nsapi_error_t get_manufacturer(char *buf, size_t buf_size);

    virtual nsapi_error_t get_model(char *buf, size_t buf_size);

    virtual nsapi_error_t get_revision(char *buf, size_t buf_size);

    virtual nsapi_error_t get_serial_number(char *buf, size_t buf_size, SerialNumberType type);

    virtual nsapi_error_t get_imsi(char *imsi, size_t buf_size);

    virtual nsapi_error_t get_iccid(char *buf, size_t buf_size);
protected:
    /** Request information text from cellular device
     *
     *  @param cmd      3gpp command string
     *  @param buf      buffer for response
     *  @param buf_size buffer size
     *  @return         on success read character count, on failure negative error code
     */
    nsapi_error_t get_info(const char *cmd, char *buf, size_t buf_size);
};

} // namespace mbed

#endif // AT_CELLULAR_INFORMATION_H_
