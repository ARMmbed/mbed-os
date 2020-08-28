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
#include "net_interface.h"
#include "ip6string.h"

extern "C" {
#include "ws_bbr_api.h"
}

#define TRACE_GROUP "WSBR"


WisunBorderRouter::WisunBorderRouter()
{
    // Apply mbed configuration to Wi-SUN BBR
    configure();
}

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

    apply_configuration(mesh_if_id);

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

    apply_configuration(mesh_if_id);

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

mesh_error_t WisunBorderRouter::configure()
{
#if defined(MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET) || defined(MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS)
    mesh_error_t status;
#endif

    if (_configured) {
        // Already configured
        return MESH_ERROR_NONE;
    }

    _configured = true;

#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET
    const char radius_shared_secret[] = {MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET};
#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET_LEN
    const uint16_t radius_shared_secret_len = MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET_LEN;
#else
    uint16_t radius_shared_secret_len = strlen(radius_shared_secret);
#endif
    status = set_radius_shared_secret(radius_shared_secret_len, (uint8_t *) radius_shared_secret);
    if (status != MESH_ERROR_NONE) {
        tr_error("Failed to set RADIUS shared secret!");
        return status;
    }
#endif

#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS
    const char radius_server_ipv6_addr[] = {MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS};
    status = set_radius_server_ipv6_address(radius_server_ipv6_addr);
    if (status != MESH_ERROR_NONE) {
        tr_error("Failed to set RADIUS server IPv6 address!");
        return status;
    }
#endif

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::apply_configuration(int8_t mesh_if_id)
{
    mesh_error_t status = set_bbr_radius_address();
    if (status != MESH_ERROR_NONE) {
        tr_error("Failed to apply RADIUS server IPv6 address!");
        return MESH_ERROR_PARAM;
    }
    return MESH_ERROR_NONE;
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

mesh_error_t WisunBorderRouter::info_get(ws_br_info_t *info_ptr)
{
    bbr_information_t bbr_info = {0};

    if (info_ptr == NULL) {
        return MESH_ERROR_PARAM;
    }

    int status = ws_bbr_info_get(_mesh_if_id, &bbr_info);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    info_ptr->device_count = bbr_info.devices_in_network;
    info_ptr->host_timestamp = bbr_info.timestamp;
    info_ptr->instance_id = bbr_info.instance_id;
    info_ptr->version = bbr_info.version;
    memcpy(info_ptr->rpl_dodag_id, bbr_info.dodag_id, 16);
    memcpy(info_ptr->ipv6_prefix, bbr_info.prefix, 8);
    memcpy(info_ptr->ipv6_iid, bbr_info.IID, 8);
    memcpy(info_ptr->gateway_addr, bbr_info.gateway, 16);

    return MESH_ERROR_NONE;
}

int WisunBorderRouter::routing_table_get(ws_br_route_info_t *table_ptr, uint16_t table_len)
{
    if (table_ptr == NULL) {
        return -1;
    }

    return ws_bbr_routing_table_get(_mesh_if_id, (bbr_route_info_t *)table_ptr, table_len);
}

mesh_error_t WisunBorderRouter::set_radius_server_ipv6_address(const char *address)
{
    if (address) {
        uint8_t ipv6_addr[16];
        if (!stoip6(address, strlen(address), ipv6_addr)) {
            return MESH_ERROR_PARAM;
        }
        // Stored address (returned by get) is in the format given by user of the interface
        strcpy(_radius_ipv6_addr, address);
        _radius_ipv6_addr_set = true;
    } else {
        _radius_ipv6_addr_set = false;
    }

    return set_bbr_radius_address();
}

mesh_error_t WisunBorderRouter::get_radius_server_ipv6_address(char *address)
{
    if (!_radius_ipv6_addr_set) {
        return MESH_ERROR_UNKNOWN;
    }
    strcpy(address, _radius_ipv6_addr);

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::set_bbr_radius_address(void)
{
    int status;

    if (_radius_ipv6_addr_set) {
        uint8_t ipv6_addr[16];
        if (!stoip6(_radius_ipv6_addr, strlen(_radius_ipv6_addr), ipv6_addr)) {
            return MESH_ERROR_PARAM;
        }
        status = ws_bbr_radius_address_set(_mesh_if_id, ipv6_addr);
    } else {
        status = ws_bbr_radius_address_set(_mesh_if_id, NULL);
    }
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::set_radius_shared_secret(uint16_t shared_secret_len, const uint8_t *shared_secret)
{
    if (shared_secret_len == 0 || !shared_secret) {
        return MESH_ERROR_PARAM;
    }

    int status = ws_bbr_radius_shared_secret_set(_mesh_if_id, shared_secret_len, shared_secret);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}

mesh_error_t WisunBorderRouter::get_radius_shared_secret(uint16_t *shared_secret_len, uint8_t *shared_secret)
{
    if (shared_secret_len == NULL) {
        return MESH_ERROR_PARAM;
    }

    int status = ws_bbr_radius_shared_secret_get(_mesh_if_id, shared_secret_len, shared_secret);
    if (status != 0) {
        return MESH_ERROR_UNKNOWN;
    }

    return MESH_ERROR_NONE;
}