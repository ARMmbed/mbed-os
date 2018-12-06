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
#include "CellularContext.h"

namespace mbed {
#ifdef CELLULAR_DEVICE
MBED_WEAK CellularContext *CellularContext::get_default_instance()
{
    // Uses default APN, uname, password from mbed_app.json
    static CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }
    static CellularContext *context = dev->create_context();
    return context;
}
#else
MBED_WEAK CellularContext *CellularContext::get_default_instance()
{
    return NULL;
}
#endif // CELLULAR_DEVICE

} // namespace mbed
