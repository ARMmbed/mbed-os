/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
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
#include "SclAccessPoint.h"

SclAccessPoint::SclAccessPoint(nsapi_wifi_ap_t ap, scl_bss_type_t bss_type, uint8_t *ie_ptr, uint32_t ie_len) :
    WiFiAccessPoint(ap), _bss_type(bss_type)
{
    _ie_ptr = (uint8_t *)malloc(ie_len * sizeof(uint8_t));
    if (_ie_ptr != NULL) {
        _ie_len = ie_len;
        memcpy(_ie_ptr, ie_ptr, ie_len);
    }
}

SclAccessPoint &SclAccessPoint::operator=(SclAccessPoint &&rhs)
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

scl_bss_type_t SclAccessPoint::get_bss_type() const
{
    return _bss_type;
}

uint8_t *SclAccessPoint::get_ie_data() const
{
    return _ie_ptr;
}

uint32_t SclAccessPoint::get_ie_len() const
{
    return _ie_len;
}

SclAccessPoint::~SclAccessPoint()
{
    if (_ie_ptr != NULL) {
        free(_ie_ptr);
    }
}
