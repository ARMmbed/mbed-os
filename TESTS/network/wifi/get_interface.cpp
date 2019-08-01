/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define WIFI 2
#if !defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || \
    MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != WIFI
#error [NOT_SUPPORTED] No network configuration found for this target.
#else
#if !defined(MBED_CONF_APP_WIFI_SECURE_SSID) && !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#else

#include "mbed.h"
#include "WiFiInterface.h"

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface) {
        interface->disconnect();
        return interface;
    }

    interface = WiFiInterface::get_default_instance();

    return interface;
}
#endif //!defined(MBED_CONF_APP_WIFI_SECURE_SSID) && !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
#endif //!defined(MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE) || MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE != WIFI
