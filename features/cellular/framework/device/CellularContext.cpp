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

MBED_WEAK CellularInterface *CellularInterface::get_target_default_instance()
{
    return mbed::CellularContext::get_default_instance();
}

namespace mbed {

MBED_WEAK CellularContext *CellularContext::get_default_instance()
{
    CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }

    static CellularContext *context = dev->create_context(NULL, NULL, MBED_CONF_CELLULAR_CONTROL_PLANE_OPT);
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    context->set_file_handle(static_cast<UARTSerial *>(&dev->get_file_handle()), MDMDCD, MDM_PIN_POLARITY);
#endif // #if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
#endif // #if DEVICE_SERIAL
    return context;
}

MBED_WEAK CellularContext *CellularContext::get_default_nonip_instance()
{
    // Uses default APN, uname, password from mbed_app.json
    CellularDevice *dev = CellularDevice::get_default_instance();
    if (!dev) {
        return NULL;
    }

    static CellularContext *context = dev->create_context(NULL, NULL, MBED_CONF_CELLULAR_CONTROL_PLANE_OPT, true);
#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)
#if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
    context->set_file_handle(static_cast<UARTSerial *>(&dev->get_file_handle()), MDMDCD, MDM_PIN_POLARITY);
#endif // #if defined(MDMDCD) && defined(MDM_PIN_POLARITY)
#endif // #if DEVICE_SERIAL
    return context;
}

void CellularContext::cp_data_received()
{
    _cp_netif->data_received();
}

} // namespace mbed
