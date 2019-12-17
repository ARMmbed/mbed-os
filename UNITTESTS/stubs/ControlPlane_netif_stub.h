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

#include "ControlPlane_netif.h"
#include <list>

namespace mbed {

class ControlPlane_netif_stub : public ControlPlane_netif {
public:
    std::list<nsapi_error_t> return_values;
    nsapi_error_t return_value;
    bool cp_data_received_called;

    ControlPlane_netif_stub()
    {
        return_value = 0;
        cp_data_received_called = false;
    }

protected:
    virtual nsapi_error_t send(const void *cpdata, nsapi_size_t cpdata_length)
    {
        if (!return_values.empty()) {
            nsapi_error_t ret = return_values.front();
            return_values.pop_front();
            return ret;
        }
        return return_value;
    };

    virtual nsapi_error_t recv(void *cpdata, nsapi_size_t cpdata_length)
    {
        if (!return_values.empty()) {
            nsapi_error_t ret = return_values.front();
            return_values.pop_front();
            return ret;
        }
        return return_value;
    };

    virtual void data_received()
    {
        cp_data_received_called = true;
    };

    virtual void attach(void (*callback)(void *), void *data) {};
};

}
