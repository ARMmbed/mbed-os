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
#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "sw_mac.h"
#include "mac_api.h"
#include "mac_common_defines.h"
#include "common_functions.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"

#include "mac_security_mib_stub.h"

mac_security_mib_stub_def mac_security_mib_stub;

int8_t  mac_sec_mib_device_description_table_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return mac_security_mib_stub.int8_value;
}

int8_t  mac_sec_mib_key_description_table_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return mac_security_mib_stub.int8_value;
}

int8_t  mac_sec_mib_device_description_table_init(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t table_size)
{
    return mac_security_mib_stub.int8_value;
}

int8_t mac_sec_mib_device_description_set(uint8_t atribute_index, mlme_device_descriptor_t *device_descriptor, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return mac_security_mib_stub.int8_value;
}

int8_t mac_sec_mib_key_description_set(uint8_t atribute_index, mlme_key_descriptor_entry_t *key_descriptor, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return mac_security_mib_stub.int8_value;
}

mlme_device_descriptor_t *mac_sec_mib_device_description_get_attribute_index(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t attribute_index)
{
    return mac_security_mib_stub.device_ptr;
}

mlme_device_descriptor_t *mac_sec_mib_device_description_get(protocol_interface_rf_mac_setup_s *rf_mac_setup, const uint8_t *address, uint8_t type)
{
    return mac_security_mib_stub.device_ptr;
}

uint8_t mac_mib_device_descption_attribute_get_by_descriptor(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_device_descriptor_t *descriptor)
{
    return mac_security_mib_stub.uint8_value;
}

mlme_key_descriptor_t * mac_sec_key_description_get(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_security_t *key_source, uint8_t address_mode, uint8_t *address_ptr, uint16_t pan_id)
{
    return mac_security_mib_stub.key_ptr;
}

int8_t mac_sec_mib_init(protocol_interface_rf_mac_setup_s *rf_mac_setup,mac_description_storage_size_t *storage_sizes)
{
    return mac_security_mib_stub.int8_value;
}

void mac_sec_mib_deinit(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

mlme_key_device_descriptor_t * mac_sec_mib_key_device_description_list_update(mlme_key_descriptor_t *key_descpription_table)
{
    return mac_security_mib_stub.key_dev_update_ptr;
}

mlme_key_device_descriptor_t * mac_sec_mib_key_device_description_discover_from_list(mlme_key_descriptor_t *key_descpription_table, uint8_t device_descriptor_handle)
{
    return mac_security_mib_stub.key_dev_ptr;
}

void mac_sec_mib_device_description_blacklist(protocol_interface_rf_mac_setup_s *rf_mac_setup , uint8_t device_handle)
{
}


