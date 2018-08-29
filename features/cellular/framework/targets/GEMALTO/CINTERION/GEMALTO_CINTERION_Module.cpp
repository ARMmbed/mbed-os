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

#include <string.h>

#include "AT_CellularBase.h"
#include "GEMALTO_CINTERION_Module.h"
#include "CellularLog.h"

using namespace mbed;

// unsupported features as per ELS61-E2_ATC_V01.000
static const AT_CellularBase::SupportedFeature unsupported_features_els61[] =  {
    AT_CellularBase::AT_CGSN_WITH_TYPE,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

// unsupported features as per BGS2-W_ATC_V00.100
static const AT_CellularBase::SupportedFeature unsupported_features_bgs2[] =  {
    AT_CellularBase::AT_CGSN_WITH_TYPE,
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

// unsupported features as per EMS31-US_ATC_V4.9.5
static const AT_CellularBase::SupportedFeature unsupported_features_ems31[] =  {
    AT_CellularBase::SUPPORTED_FEATURE_END_MARK
};

GEMALTO_CINTERION_Module::Model GEMALTO_CINTERION_Module::_model;

nsapi_error_t GEMALTO_CINTERION_Module::detect_model(const char *model)
{
    static const AT_CellularBase::SupportedFeature *unsupported_features;
    if (memcmp(model, "ELS61", sizeof("ELS61") - 1) == 0) {
        _model = ModelELS61;
        unsupported_features = unsupported_features_els61;
    } else if (memcmp(model, "BGS2", sizeof("BGS2") - 1) == 0) {
        _model = ModelBGS2;
        unsupported_features = unsupported_features_bgs2;
    } else if (memcmp(model, "EMS31", sizeof("EMS31") - 1) == 0) {
        _model = ModelEMS31;
        unsupported_features = unsupported_features_ems31;
    } else {
        tr_error("Cinterion model unsupported %s", model);
        return NSAPI_ERROR_UNSUPPORTED;
    }
    tr_info("Cinterion model %s (%d)", model, _model);
    AT_CellularBase::set_unsupported_features(unsupported_features);
    return NSAPI_ERROR_OK;
}

GEMALTO_CINTERION_Module::Model GEMALTO_CINTERION_Module::get_model()
{
    return _model;
}
