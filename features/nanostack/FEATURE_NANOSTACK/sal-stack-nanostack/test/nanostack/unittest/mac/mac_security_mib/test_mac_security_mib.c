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
#include "mac_api.h"
#include "test_mac_security_mib.h"
#include <string.h>
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "MAC/rf_driver_storage.h"

#include "nsdynmemLIB_stub.h"
#include "common_functions_stub.h"

bool test_mac_sec_mib_init()
{
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;

    int8_t ret = mac_sec_mib_init(NULL, &storage_sizes);
    if( ret != -1 ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    ret = mac_sec_mib_init(NULL, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 4;
    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 5;
    ret = mac_sec_mib_init(&setup, &storage_sizes);
    if( ret != 0 ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_deinit()
{
    mac_sec_mib_deinit(NULL);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);
    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_device_description_set()
{
    if( -1 != mac_sec_mib_device_description_set(0, NULL, NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_device_descriptor_t desc;
    memset(&desc, 0, sizeof(mlme_device_descriptor_t));
    desc.ExtAddress[0] = 1;

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    if( 0 != mac_sec_mib_device_description_set(0, &desc, &setup) ){
        return false;
    }

    mlme_device_descriptor_t desc2;
    memset(&desc2, 0, sizeof(mlme_device_descriptor_t));
    desc2.ExtAddress[0] = 2;

    if( 0 != mac_sec_mib_device_description_set(1, &desc2, &setup) ){
        return false;
    }

    mlme_device_descriptor_t desc3;
    memset(&desc3, 0, sizeof(mlme_device_descriptor_t));
    desc3.ExtAddress[0] = 3;

    if( 0 != mac_sec_mib_device_description_set(2, &desc3, &setup) ){
        return false;
    }

    mlme_device_descriptor_t desc4;
    memset(&desc4, 0, sizeof(mlme_device_descriptor_t));
    desc4.ExtAddress[0] = 4;

    mlme_key_descriptor_t *key_ptr = setup.key_description_table;
    key_ptr->KeyDeviceListEntries = 3;
    key_ptr->KeyDeviceList[0].DeviceDescriptorHandle = 0;
    key_ptr->KeyDeviceList[1].DeviceDescriptorHandle = 1;
    key_ptr->KeyDeviceList[2].DeviceDescriptorHandle = 2;

    if( 0 != mac_sec_mib_device_description_set(1, &desc4, &setup) ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_key_description_set()
{
    if( -1 != mac_sec_mib_key_description_set(0, NULL, NULL) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_key_descriptor_entry_t kdesc;
    memset(&kdesc, 0, sizeof(mlme_key_descriptor_entry_t));

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    kdesc.KeyDeviceList = NULL;
    kdesc.KeyDeviceListEntries = 1;
    if( -1 != mac_sec_mib_key_description_set(0, &kdesc, &setup) ){
        return false;
    }

    kdesc.KeyDeviceListEntries = 0;
    kdesc.KeyIdLookupList = NULL;
    kdesc.KeyIdLookupListEntries = 1;
    if( -1 != mac_sec_mib_key_description_set(0, &kdesc, &setup) ){
        return false;
    }

    kdesc.KeyIdLookupListEntries = 0;
    kdesc.KeyUsageList = NULL;
    kdesc.KeyUsageListEntries = 1;
    if( -1 != mac_sec_mib_key_description_set(0, &kdesc, &setup) ){
        return false;
    }
    kdesc.KeyUsageListEntries = 0;

    mac_sec_mib_deinit(&setup);
    nsdynmemlib_stub.returnCounter = 5;
    mac_sec_mib_init(&setup, &storage_sizes);

    mlme_key_device_descriptor_t desc;
    kdesc.KeyDeviceList = &desc;
    kdesc.KeyDeviceListEntries = 1;

    kdesc.KeyIdLookupListEntries = 1;
    mlme_key_id_lookup_descriptor_t list;
    kdesc.KeyIdLookupList = &list;

    kdesc.KeyUsageListEntries = 1;
    mlme_key_usage_descriptor_t usage;
    kdesc.KeyUsageList = &usage;

    if( 0 != mac_sec_mib_key_description_set(0, &kdesc, &setup) ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_device_description_get_attribute_index()
{
    if( mac_sec_mib_device_description_get_attribute_index(NULL, 0) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    if( !mac_sec_mib_device_description_get_attribute_index(&setup, 0) ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_device_description_get()
{
    if( mac_sec_mib_device_description_get(NULL, NULL, 0) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));

    uint8_t mac16[2];
    if( mac_sec_mib_device_description_get(&setup, &mac16, MAC_ADDR_MODE_16_BIT) ){
        return false;
    }

    uint8_t mac64[8];
    if( mac_sec_mib_device_description_get(&setup, &mac64, MAC_ADDR_MODE_64_BIT) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    mac16[0] = 0;
    mac16[1] = 0;
    if( mac_sec_mib_device_description_get(&setup, &mac16, MAC_ADDR_MODE_16_BIT) ){
        return false;
    }

    if( mac_sec_mib_device_description_get(&setup, &mac64, MAC_ADDR_MODE_64_BIT) ){
        return false;
    }

    mlme_device_descriptor_t *device_table = setup.device_description_table;
    device_table++;
    device_table->ShortAddress = 16;
    common_functions_stub.uint16_value = 16;
    if( !mac_sec_mib_device_description_get(&setup, &mac16, MAC_ADDR_MODE_16_BIT) ){
        return false;
    }
    common_functions_stub.uint16_value = 0;

    memset(&device_table->ExtAddress, 1, 8);
    memset(&mac64, 1, 8);
    if( !mac_sec_mib_device_description_get(&setup, &mac64, MAC_ADDR_MODE_64_BIT) ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_mib_device_descption_attribute_get_by_descriptor()
{
    uint8_t ret = mac_mib_device_descption_attribute_get_by_descriptor(NULL, NULL);
    if( 0xff != ret ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_device_descriptor_t desc;

    ret = mac_mib_device_descption_attribute_get_by_descriptor(&setup, &desc);
    if( 0xff != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    mlme_device_descriptor_t *device_table = setup.device_description_table;
    device_table->ShortAddress = 13;
    device_table++;

    ret = mac_mib_device_descption_attribute_get_by_descriptor(&setup, device_table);
    if( 1 != ret ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_key_description_get()
{
    uint8_t address[8];
    uint16_t pan_id = 0xface;
    memset(address, 0, 8);
    if( mac_sec_key_description_get(NULL, NULL, MAC_ADDR_MODE_16_BIT,address, pan_id) ){
        return false;
    }
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_security_t sec;
    memset(&sec, 0, sizeof(mlme_security_t));
    sec.KeyIdMode = MAC_KEY_ID_MODE_IMPLICIT;

    //64 MAC_ADDR_MODE_64_BIT
    if( mac_sec_key_description_get(&setup, &sec, MAC_ADDR_MODE_64_BIT, address, pan_id ) ){
        return false;
    }

    //MAC_ADDR_MODE_16_BIT
    if( mac_sec_key_description_get(&setup, &sec,MAC_ADDR_MODE_16_BIT,address, pan_id ) ){
        return false;
    }

    //MAC_ADDR_MODE_NONE
    if( mac_sec_key_description_get(&setup, &sec, MAC_ADDR_MODE_NONE, address, pan_id ) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 5;
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 6;
    storage_sizes.key_lookup_size = 2; //Test multiple lookup
    storage_sizes.key_usage_size = 3;
    mac_sec_mib_init(&setup, &storage_sizes);

    sec.KeyIdMode = MAC_KEY_ID_MODE_IDX;
    if( mac_sec_key_description_get(&setup, &sec, MAC_ADDR_MODE_16_BIT,address, pan_id) ){
        return false;
    }

    mlme_key_descriptor_t *key_description = setup.key_description_table;
    key_description->KeyIdLookupListEntries = 2;

    key_description->KeyIdLookupList[0].LookupDataSize = 0;
    memset(&key_description->KeyIdLookupList[0].LookupData, 0, 9);
    key_description->KeyIdLookupList[0].LookupData[4] = 7;

    key_description->KeyIdLookupList[1].LookupDataSize = 1;
    memset(&key_description->KeyIdLookupList[1].LookupData, 0, 9);
    key_description->KeyIdLookupList[1].LookupData[8] = 7;

    sec.KeyIndex = 7;

    sec.KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
    if( !mac_sec_key_description_get(&setup, &sec, MAC_ADDR_MODE_16_BIT,address, pan_id) ){
        return false;
    }

    sec.KeyIndex = 7;
    sec.KeyIdMode = MAC_KEY_ID_MODE_SRC8_IDX;
    if( !mac_sec_key_description_get(&setup, &sec, MAC_ADDR_MODE_16_BIT,address, pan_id) ){
        return false;
    }

    mac_sec_mib_deinit(&setup);

    return true;
}

bool test_mac_sec_mib_key_device_description_list_update()
{
    if( mac_sec_mib_key_device_description_list_update(NULL) ){
        return false;
    }
    mlme_key_descriptor_t desc;
    mlme_key_device_descriptor_t deviceList[2];
    memset(&desc, 0, sizeof(mlme_key_descriptor_t));
    memset(&deviceList, 0, sizeof(mlme_key_device_descriptor_t) *2);
    desc.KeyDeviceList = &deviceList;
    desc.KeyDeviceListSize = 2;
    if( !mac_sec_mib_key_device_description_list_update(&desc) ||
        1 != desc.KeyDeviceListEntries ){
        return false;
    }

    if( !mac_sec_mib_key_device_description_list_update(&desc) ||
            2 != desc.KeyDeviceListEntries ){
        return false;
    }


    return true;
}

bool test_mac_sec_mib_key_device_description_discover_from_list()
{
    if( mac_sec_mib_key_device_description_discover_from_list(NULL, 0)){
        return false;
    }
    mlme_key_descriptor_t desc;
    memset(&desc, 0, sizeof(mlme_key_descriptor_t));
    mlme_key_device_descriptor_t keydevicelist[2];
    memset(&keydevicelist, 0, sizeof(mlme_key_device_descriptor_t) *2);

    keydevicelist[0].DeviceDescriptorHandle = 8;
    keydevicelist[1].DeviceDescriptorHandle = 34;
    desc.KeyDeviceList = &keydevicelist;
    desc.KeyDeviceListEntries = 2;

    if( !mac_sec_mib_key_device_description_discover_from_list(&desc, 34)){
        return false;
    }

    return true;
}

bool test_mac_sec_mib_device_description_blacklist()
{
    mac_sec_mib_device_description_blacklist(NULL, 0);
    protocol_interface_rf_mac_setup_s setup;
    memset(&setup, 0, sizeof(protocol_interface_rf_mac_setup_s));
    mlme_key_descriptor_t desc;
    memset(&desc, 0, sizeof(mlme_key_descriptor_t));
    mlme_key_device_descriptor_t deviceList[2];
    deviceList[0].DeviceDescriptorHandle = 8;
    deviceList[1].DeviceDescriptorHandle = 34;
    desc.KeyDeviceList = &deviceList;
    desc.KeyDeviceListEntries = 2;
    setup.key_description_table = &desc;
    setup.key_description_table_size = 2;

    mac_sec_mib_device_description_blacklist(&setup, 34);
    if( !deviceList[1].Blacklisted ){
        return false;
    }

    return true;
}

