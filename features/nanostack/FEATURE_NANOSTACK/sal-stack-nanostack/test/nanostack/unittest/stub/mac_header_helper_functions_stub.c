/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "string.h"
#include "mlme.h"
#include "mac_api.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "mac_header_helper_functions_stub.h"

mac_header_helper_functions_def mac_header_helper_functions_stub;

uint8_t mac_security_mic_length_get(uint8_t security_level)
{
    return mac_header_helper_functions_stub.uint8_value;
}

uint8_t mac_header_security_aux_header_length(uint8_t security_level, uint8_t keyIdmode)
{
    return mac_header_helper_functions_stub.uint8_value;
}

uint8_t mac_header_address_length(const mac_fcf_sequence_t *fcf)
{
    return mac_header_helper_functions_stub.uint8_value;
}


void mac_header_security_parameter_set(mac_aux_security_header_t *header, const mlme_security_t *frame_setup)
{
    if( header ){
        *header = mac_header_helper_functions_stub.security_header;
    }
}

void mac_header_parse_fcf_dsn(mac_fcf_sequence_t *header, const uint8_t *ptr)
{
   *header = mac_header_helper_functions_stub.fcf;
}

void mac_header_security_components_read(mac_pre_parsed_frame_t *buffer, mlme_security_t *security_params)
{
    if (security_params) {
        security_params->KeyIdMode = mac_header_helper_functions_stub.security_header.KeyIdMode;
        security_params->KeyIndex = mac_header_helper_functions_stub.security_header.KeyIndex;
        security_params->SecurityLevel = mac_header_helper_functions_stub.security_header.securityLevel;
        memcpy(security_params->Keysource, mac_header_helper_functions_stub.security_header.Keysource, 8);

    }
}

uint16_t mac_header_get_src_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    return mac_header_helper_functions_stub.pan_src;
}

uint16_t mac_header_get_dst_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    return mac_header_helper_functions_stub.uint16_value;
}

void mac_header_get_src_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{
    if (address_ptr) {
        memcpy(address_ptr, mac_header_helper_functions_stub.src_address, 8);
    }
}

void mac_header_get_dst_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{
    if (address_ptr) {
        memcpy(address_ptr, mac_header_helper_functions_stub.dst_address, 8);
    }
}

uint16_t mcps_payload_length_from_received_frame(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint16_value;
}

uint8_t mcps_mac_header_length_from_received_frame(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_value;
}

uint8_t *mcps_mac_payload_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_ptr;
}

uint8_t *mcps_security_mic_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_ptr;
}

uint8_t *mcps_mac_security_aux_header_start_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_ptr;
}

uint8_t mcps_mac_command_frame_id_get(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_value;
}

uint32_t mcps_mac_security_frame_counter_read(const mac_pre_parsed_frame_t *buffer)
{
    return mac_header_helper_functions_stub.security_header.frameCounter;
}

uint8_t * mcps_generic_header_write(uint8_t *ptr, const mac_pre_build_frame_t *buffer)
{
    return mac_header_helper_functions_stub.uint8_ptr;
}

