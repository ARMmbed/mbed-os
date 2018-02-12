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

#ifndef CELLULAR_MULTIPLEXER_H_
#define CELLULAR_MULTIPLEXER_H_

#include "nsapi_types.h"

namespace mbed {

/**
 *  Class CellularMultiplexer
 *
 *  An abstract interface that provides a way to start multiplexer mode in modem.
 */
class CellularMultiplexer
{
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /** virtual Destructor
     */
    virtual ~CellularMultiplexer() {};

public:

    /** Starts modem multiplexer mode specified by 3GPP TS 27.010.
     *
     *  @return zero on success, negative error code on failure
     */
    virtual nsapi_error_t multiplexer_mode_start() = 0;
};

} // namespace mbed

#endif // CELLULAR_MULTIPLEXER_H_
