/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "AT_CellularBase.h"
#include "CellularLog.h"

using namespace mbed;

AT_CellularBase::AT_CellularBase(ATHandler &at) : _at(at)
{
}

ATHandler &AT_CellularBase::get_at_handler()
{
    return _at;
}

device_err_t AT_CellularBase::get_device_error() const
{
    return _at.get_last_device_error();
}

AT_CellularBase::SupportedFeature const *AT_CellularBase::_unsupported_features;

void AT_CellularBase::set_unsupported_features(const SupportedFeature *unsupported_features)
{
    _unsupported_features = unsupported_features;
}

bool AT_CellularBase::is_supported(SupportedFeature feature)
{
    if (!_unsupported_features) {
        return true;
    }

    for (int i = 0; _unsupported_features[i] != SUPPORTED_FEATURE_END_MARK; i++) {
        if (_unsupported_features[i] == feature) {
            tr_debug("Unsupported feature (%d)", (int)feature);
            return false;
        }
    }

    return true;
}
