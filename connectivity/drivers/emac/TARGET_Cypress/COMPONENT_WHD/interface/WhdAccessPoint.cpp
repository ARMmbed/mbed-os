/* WHD Access Point Interface Implementation
 * Copyright (c) 2018-2019 ARM Limited
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

#include <cstdlib>
#include <utility>
#include "WhdAccessPoint.h"

WhdAccessPoint::WhdAccessPoint(nsapi_wifi_ap_t ap, whd_bss_type_t bss_type, uint8_t *ie_ptr, uint32_t ie_len) :
    WiFiAccessPoint(ap), _bss_type(bss_type)
{
    _ie_ptr = (uint8_t *)malloc(ie_len * sizeof(uint8_t));
    if (_ie_ptr != NULL) {
        _ie_len = ie_len;
        memcpy(_ie_ptr, ie_ptr, ie_len * sizeof(uint8_t));
    }
}

WhdAccessPoint &WhdAccessPoint::operator=(WhdAccessPoint &&rhs)
{
    if (this != &rhs) {
        WiFiAccessPoint::operator=(rhs);
        _bss_type = rhs._bss_type;
        _ie_ptr = rhs._ie_ptr;
        _ie_len = rhs._ie_len;
        rhs._ie_ptr = NULL;
        rhs._ie_len = 0;
    }
    return *this;
}

whd_bss_type_t WhdAccessPoint::get_bss_type() const
{
    return _bss_type;
}

uint8_t *WhdAccessPoint::get_ie_data() const
{
    return _ie_ptr;
}

uint32_t WhdAccessPoint::get_ie_len() const
{
    return _ie_len;
}

WhdAccessPoint::~WhdAccessPoint()
{
    if (_ie_ptr != NULL) {
        free(_ie_ptr);
    }
}
