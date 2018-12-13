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
#include "AT_CellularNetwork.h"
#include "GEMALTO_CINTERION_Module.h"
#include "CellularLog.h"

using namespace mbed;

static const intptr_t cellular_properties_els61[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,  // C_EREG
    AT_CellularNetwork::RegistrationModeLAC,  // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,  // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};

static const intptr_t cellular_properties_bgs2[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeDisable, // C_EREG
    AT_CellularNetwork::RegistrationModeEnable,  // C_GREG
    AT_CellularNetwork::RegistrationModeLAC,     // C_REG
    0,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    0,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK

};

static const intptr_t cellular_properties_ems31[AT_CellularBase::PROPERTY_MAX] = {
    AT_CellularNetwork::RegistrationModeLAC,        // C_EREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_GREG
    AT_CellularNetwork::RegistrationModeDisable,    // C_REG
    1,  // AT_CGSN_WITH_TYPE
    1,  // AT_CGDATA
    1,  // AT_CGAUTH
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    0,  // PROPERTY_IPV4V6_STACK
};

GEMALTO_CINTERION_Module::Model GEMALTO_CINTERION_Module::_model;

nsapi_error_t GEMALTO_CINTERION_Module::detect_model(const char *model)
{
    static const intptr_t *cellular_properties;
    if (memcmp(model, "ELS61", sizeof("ELS61") - 1) == 0) {
        _model = ModelELS61;
        cellular_properties = cellular_properties_els61;
    } else if (memcmp(model, "BGS2", sizeof("BGS2") - 1) == 0) {
        _model = ModelBGS2;
        cellular_properties = cellular_properties_bgs2;
    } else if (memcmp(model, "EMS31", sizeof("EMS31") - 1) == 0) {
        _model = ModelEMS31;
        cellular_properties = cellular_properties_ems31;
    } else {
        tr_error("Cinterion model unsupported %s", model);
        return NSAPI_ERROR_UNSUPPORTED;
    }
    tr_info("Cinterion model %s (%d)", model, _model);
    AT_CellularBase::set_cellular_properties(cellular_properties);
    return NSAPI_ERROR_OK;
}

GEMALTO_CINTERION_Module::Model GEMALTO_CINTERION_Module::get_model()
{
    return _model;
}
