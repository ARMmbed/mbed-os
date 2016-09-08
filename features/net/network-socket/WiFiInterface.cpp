/* Socket
 * Copyright (c) 2015 ARM Limited
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

#include "network-socket/WiFiInterface.h"
#include <stdlib.h>
#include <string.h>


WiFiInterface::WiFiInterface()
    : _ssid(0), _pass(0), _security(NSAPI_SECURITY_NONE)
{
}

WiFiInterface::~WiFiInterface()
{
    free(_ssid);
    free(_pass);
}

int WiFiInterface::set_credentials(const char *ssid, const char *pass, nsapi_security_t security)
{
    free(_ssid);
    _ssid = 0;
    free(_pass);
    _pass = 0;

    if (ssid) {
        _ssid = (char*)malloc(strlen(ssid)+1);
        if (!_ssid) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        strcpy(_ssid, ssid);
    }

    if (pass) {
        _pass = (char*)malloc(strlen(pass)+1);
        if (!_pass) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        strcpy(_pass, pass);
    }

    _security = security;

    return 0;
}

int WiFiInterface::connect()
{
    if (!_ssid || !_pass) {
        return NSAPI_ERROR_PARAMETER;
    }

    return connect(_ssid, _pass, _security);
}

