/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "WisunInterface.h"
#include "ns_trace.h"
#include "WisunBorderRouter.h"
#include "MeshInterfaceNanostack.h"

extern "C" {
#include "ws_bbr_api.h"
}

#define TRACE_GROUP "WSBR"

mesh_error_t WisunBorderRouter::start(NetworkInterface *mesh_if, NetworkInterface *backbone_if)
{
    if (mesh_if == NULL || backbone_if == NULL) {
        return MESH_ERROR_PARAM;
    }

    InterfaceNanostack *nano_mesh_if = reinterpret_cast<InterfaceNanostack *>(mesh_if);
    int8_t mesh_if_id = nano_mesh_if->get_interface_id();
    if (mesh_if_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }
    _mesh_if_id = mesh_if_id;

    char backbone_if_name[7] = {0};
    if (backbone_if->get_interface_name(backbone_if_name) == NULL) {
        return MESH_ERROR_UNKNOWN;
    }

    if (strlen((char *) &backbone_if_name) < 4) {
        return MESH_ERROR_UNKNOWN;
    }

    int backbone_if_id = atoi(&backbone_if_name[3]);
    if (backbone_if_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }

    int ret = ws_bbr_start(mesh_if_id, backbone_if_id);
    if (ret < 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::start(NetworkInterface *mesh_if, OnboardNetworkStack::Interface *backbone_if)
{
    InterfaceNanostack *nano_mesh_if = reinterpret_cast<InterfaceNanostack *>(mesh_if);
    int8_t mesh_if_id = nano_mesh_if->get_interface_id();
    if (mesh_if_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }
    _mesh_if_id = mesh_if_id;

    Nanostack::Interface *nano_backbone_if = static_cast<Nanostack::Interface *>(backbone_if);
    int8_t backbone_if_id = nano_backbone_if->get_interface_id();
    if (backbone_if_id < 0) {
        return MESH_ERROR_UNKNOWN;
    }

    int ret = ws_bbr_start(mesh_if_id, backbone_if_id);
    if (ret < 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

void WisunBorderRouter::stop()
{
    if (_mesh_if_id < 0) {
        return;
    }

    ws_bbr_stop(_mesh_if_id);

    _mesh_if_id = -1;
}

mesh_error_t WisunBorderRouter::set_rpl_parameters(uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant)
{
    int status = ws_bbr_rpl_parameters_set(_mesh_if_id, dio_interval_min, dio_interval_doublings, dio_redundancy_constant);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::get_rpl_parameters(uint8_t *dio_interval_min, uint8_t *dio_interval_doublings, uint8_t *dio_redundancy_constant)
{
    int status = ws_bbr_rpl_parameters_get(_mesh_if_id, dio_interval_min, dio_interval_doublings, dio_redundancy_constant);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::validate_rpl_parameters(uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant)
{
    int status = ws_bbr_rpl_parameters_validate(_mesh_if_id, dio_interval_min, dio_interval_doublings, dio_redundancy_constant);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::set_pan_configuration(uint16_t pan_id)
{
    int status = ws_bbr_pan_configuration_set(_mesh_if_id, pan_id);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::get_pan_configuration(uint16_t *pan_id)
{
    int status = ws_bbr_pan_configuration_get(_mesh_if_id, pan_id);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::validate_pan_configuration(uint16_t pan_id)
{
    int status = ws_bbr_pan_configuration_validate(_mesh_if_id, pan_id);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}
