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

#ifndef CELLULAR_INFORMATION_H_
#define CELLULAR_INFORMATION_H_

#include <stddef.h>
#include "nsapi_types.h"

namespace mbed {

/**
 * @addtogroup cellular
 * @{
 */

/**
 *  Class CellularInformation
 *
 *  An abstract interface that provides information about cellular device.
 */
class CellularInformation {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /** virtual Destructor
     */
    virtual ~CellularInformation() {};

public:
    /** Request manufacturer identification of cellular device
     *
     *  @param buf      manufacturer identification as zero terminated string
     *  @param buf_size max length of manufacturer identification is 2048 characters
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_manufacturer(char *buf, size_t buf_size) = 0;

    /** Request model identification of cellular device
     *
     *  @param buf      model identification as zero terminated string
     *  @param buf_size max length of model identification is 2048 characters
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_model(char *buf, size_t buf_size) = 0;

    /** Request revision identification of cellular device
     *
     *  @param buf      revision identification as zero terminated string
     *  @param buf_size max length of revision identification is 2048 characters
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t get_revision(char *buf, size_t buf_size) = 0;

    /** Request serial number identification of cellular device
     *
     *  @param buf      serial number as zero terminated string
     *  @param buf_size max length of serial number is 2048 characters
     *  @param type     serial number type to read
     *  @return         NSAPI_ERROR_OK on success
     *                  NSAPI_ERROR_UNSUPPORTED if the modem does not support SerialNumberType
     *                  NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    enum SerialNumberType {
        SN = 0, // Serial Number
        IMEI = 1, // International Mobile station Equipment Identity
        IMEISV = 2, // IMEI and Software Version number
        SVN  = 3 // Software Version Number
    };
    virtual nsapi_error_t get_serial_number(char *buf, size_t buf_size, SerialNumberType type = SN) = 0;
};

/**
 * @}
 */

} // namespace mbed

#endif // CELLULAR_INFORMATION_H_
