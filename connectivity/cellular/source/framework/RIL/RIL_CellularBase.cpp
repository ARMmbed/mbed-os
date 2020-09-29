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
#include <stdio.h>
#include "RIL_CellularBase.h"
#include "CellularLog.h"
#include "ConditionVariable.h"
#include "mbed.h"
#define DEVICE_TIMEOUT 5min
namespace mbed {

RIL_CellularBase::RIL_CellularBase() : _error(0), _cond_var(_cond_mutex)
{
}

RIL_CellularBase::~RIL_CellularBase()
{
}

ril_token_t *RIL_CellularBase::lock_and_send_request(int request_id, void *data, size_t data_len,
                                                     Callback<void(ril_token_t *, RIL_Errno, void *, size_t)> callback)
{
    _cond_mutex.lock();
    ril_token_t *token = RILAdaptation::get_instance()->send_request(request_id, data, data_len, callback);
    if (token) {
        std::chrono::duration<uint32_t, std::milli> default_timeout = DEVICE_TIMEOUT;
        rtos::cv_status time_out = _cond_var.wait_for(default_timeout);
        if (time_out == rtos::cv_status::timeout) {
            _error = NSAPI_ERROR_TIMEOUT;
            RILAdaptation::get_instance()->cancel_request(token);
            tr_warning("%s timeout for request %s", __FUNCTION__, RILAdaptation::get_ril_name(request_id));
        }
    } else {
        _error = NSAPI_ERROR_DEVICE_ERROR;
    }

    return token;
}

} /* namespace mbed */
