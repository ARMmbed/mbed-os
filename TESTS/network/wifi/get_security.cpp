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

#include <cstring>
#include "mbed.h"
#include "nsapi_types.h"

nsapi_security get_security()
{
#if defined MBED_CONF_APP_WIFI_SECURE_PROTOCOL
    static const char *SEC_WEP = "WEP";
    static const char *SEC_WPA = "WPA";
    static const char *SEC_WPA2 = "WPA2";
    static const char *SEC_WPA_WPA2 = "WPA/WPA2";

    if (strcmp(MBED_CONF_APP_WIFI_SECURE_PROTOCOL, SEC_WEP) == 0) {
        return NSAPI_SECURITY_WEP;
    }
    if (strcmp(MBED_CONF_APP_WIFI_SECURE_PROTOCOL, SEC_WPA) == 0) {
        return NSAPI_SECURITY_WPA;
    }
    if (strcmp(MBED_CONF_APP_WIFI_SECURE_PROTOCOL, SEC_WPA2) == 0) {
        return NSAPI_SECURITY_WPA2;
    }
    if (strcmp(MBED_CONF_APP_WIFI_SECURE_PROTOCOL, SEC_WPA_WPA2) == 0) {
        return NSAPI_SECURITY_WPA_WPA2;
    }
#endif
    return NSAPI_SECURITY_NONE;
}
