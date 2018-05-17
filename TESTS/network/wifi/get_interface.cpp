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

#ifndef MBED_CONF_APP_OBJECT_CONSTRUCTION
    #error [NOT_SUPPORTED] No network interface found for this target.
#endif

#if !defined(MBED_CONF_APP_WIFI_SECURE_SSID) && !defined(MBED_CONF_APP_WIFI_UNSECURE_SSID)
    #error [NOT_SUPPORTED] Requires parameters from mbed_app.json
#endif

#include "mbed.h"
#include MBED_CONF_APP_HEADER_FILE

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface) {
        interface->disconnect();
        return interface;
    }

    interface = MBED_CONF_APP_OBJECT_CONSTRUCTION;

    return interface;
}
