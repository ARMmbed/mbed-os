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

#include "mbed.h"

// Pick the correct driver based on mbed_app.json
#define INTERNAL            1
#define WIFI_ESP8266        2
#define X_NUCLEO_IDW01M1    3

#if MBED_CONF_APP_WIFI_DRIVER == INTERNAL

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
#define DRIVER OdinWiFiInterface

#elif TARGET_REALTEK_RTL8195AM
#include "RTWInterface.h"
#define DRIVER RTWInterface
#else
#error [NOT_SUPPORTED] Unsupported Wifi driver
#endif

#elif MBED_CONF_APP_WIFI_DRIVER == WIFI_ESP8266
#include "ESP8266Interface.h"
#define DRIVER ESP8266Interface

#elif MBED_CONF_APP_WIFI_DRIVER == X_NUCLEO_IDW01M1
#include "SpwfSAInterface.h"
#define DRIVER SpwfSAInterface
#else
#error [NOT_SUPPORTED] Unsupported Wifi driver
#endif

WiFiInterface *get_interface()
{
    static WiFiInterface *interface = NULL;

    if (interface)
        delete interface;

#if MBED_CONF_APP_WIFI_DRIVER == INTERNAL
    interface = new DRIVER();
#else
    interface = new DRIVER(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif
    return interface;
}
