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

#include "network-socket/CellularInterface.h"
#include <stdlib.h>
#include <string.h>


CellularInterface::CellularInterface()
    : _apn(0), _user(0), _pass(0)
{
}

CellularInterface::~CellularInterface()
{
    free(_apn);
    free(_user);
    free(_pass);
}

int CellularInterface::set_credentials(const char *apn, const char *user, const char *pass)
{
    free(_apn);
    _apn = 0;
    free(_user);
    _user = 0;
    free(_pass);
    _pass = 0;

    if (apn) {
        _apn = (char*)malloc(strlen(apn)+1);
        if (!_apn) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        strcpy(_apn, apn);
    }

    if (user) {
        _user = (char*)malloc(strlen(user)+1);
        if (!_user) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        strcpy(_user, user);
    }

    if (pass) {
        _pass = (char*)malloc(strlen(pass)+1);
        if (!_pass) {
            return NSAPI_ERROR_NO_MEMORY;
        }

        strcpy(_pass, pass);
    }

    return 0;
}

int CellularInterface::connect()
{
    return connect(_apn, _user, _pass);
}

