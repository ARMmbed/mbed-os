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

MBED_WEAK CellularBase *CellularBase::get_target_default_instance()
{
    return mbed::CellularContext::get_default_instance();
}

namespace mbed {

MBED_WEAK CellularContext *CellularContext::get_default_instance()
{
    static CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }
    static CellularContext *context = dev->create_context();
#if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    context->set_file_handle(static_cast<UARTSerial *>(&dev->get_file_handle()), MDMDCD, MDM_PIN_POLARITY);
#endif // #if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    return context;
}

} // namespace mbed
