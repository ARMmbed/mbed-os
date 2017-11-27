/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "test_mac_pairwise_key.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "MLE/mle.h"
#include "6LoWPAN/MAC/mac_pairwise_key.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "MAC/rf_driver_storage.h"

#include <string.h>

#include "nsdynmemLIB_stub.h"
#include "protocol_core_stub.h"
#include "mac_helper_stub.h"
#include "mle_stub.h"
#include "mac_common_defines.h"

bool test_mac_pairwise_key_add()
{
    protocol_interface_info_entry_t info_entry;
    mle_neigh_table_entry_t mle_neigh_table_entry;
    arm_15_4_mac_parameters_t mac_parameters;
    uint8_t eui64[8];
    uint8_t key[16];

    memset(&info_entry, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    memset(eui64, 0, 8);
    mac_api_t api;
    memset(&api, 0, sizeof(mac_api_t));
    info_entry.mac_api = &api;
    info_entry.mac_parameters = &mac_parameters;

    protocol_core_stub.entry_ptr = NULL;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    //Register first interface!!!
    int ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }


    protocol_core_stub.entry_ptr = &info_entry;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_interface_register(0, 6, 3);
    protocol_core_stub.entry_ptr = NULL;

    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }
    protocol_core_stub.entry_ptr = &info_entry;
    nsdynmemlib_stub.returnCounter = 0;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }

    //Delete
   mac_pairwise_key_del(0, eui64);

   //mac_pairwise_key_flush_list(0);

    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    nsdynmemlib_stub.returnCounter = 1;
    mac_helper_stub.int8_value = -1;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }
    mac_helper_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;

    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != 0 ){
        return false;
    }

    eui64[0] = 1;
    mle_neigh_table_entry.attribute_index = 1;
    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != 0 ){
        return false;
    }

    eui64[0] = 2;
    mle_neigh_table_entry.attribute_index = 2;
    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != 0 ){
        return false;
    }

    //Test write same key
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != 0 ){
        return false;
    }

    //This Should Fail
    eui64[0] = 3;
    nsdynmemlib_stub.returnCounter = 1;
    mle_neigh_table_entry.attribute_index = 3;
    ret = mac_pairwise_key_add(0, 0, eui64, key);
    if( ret != -1 ){
        return false;
    }

    eui64[0] = 0;
    mle_neigh_table_entry.attribute_index = 0;
    mac_pairwise_key_del(0, eui64);
    eui64[0] = 1;
    mle_neigh_table_entry.attribute_index = 1;
    mac_pairwise_key_del(0, eui64);
    eui64[0] = 2;
    mle_neigh_table_entry.attribute_index = 2;
    mac_pairwise_key_del(0, eui64);


    mac_pairwise_key_interface_unregister(0);
    return true;
}

bool test_mac_pairwise_key_del()
{
    protocol_interface_info_entry_t info_entry;
    mle_neigh_table_entry_t mle_neigh_table_entry;
    arm_15_4_mac_parameters_t mac_parameters;
    uint8_t eui64[8];
    uint8_t key[16];

    memset(&info_entry, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    memset(eui64, 0, 8);
    mac_api_t api;
    memset(&api, 0, sizeof(mac_api_t));
    info_entry.mac_api = &api;
    info_entry.mac_parameters = &mac_parameters;


    protocol_core_stub.entry_ptr = &info_entry;
    if( -1 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }


    //Register first interface!!!
    nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_interface_register(0, 6, 3);
    protocol_core_stub.entry_ptr = NULL;
    mle_stub.mle_neigh_table_entry_ptr = NULL;

    if( -1 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    mac_helper_stub.int8_value = 0;

    protocol_core_stub.entry_ptr = &info_entry;

    if( -1 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    mac_pairwise_key_add(0, 0, eui64, key);

    //
    mle_stub.mle_neigh_table_entry_ptr = NULL;


    if( -1 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;

    mle_neigh_table_entry.attribute_index = 1;
    if( -1 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }
    mle_neigh_table_entry.attribute_index = 0;

    if( 0 != mac_pairwise_key_del(0, eui64) ){
        return false;
    }


    nsdynmemlib_stub.returnCounter = 2;
        mac_pairwise_key_interface_register(1, 6, 3);
        nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_add(1, 0, eui64, key);
    if( 0 != mac_pairwise_key_del(1, eui64) ){
        return false;
    }
    mac_pairwise_key_interface_unregister(0);
    mac_pairwise_key_interface_unregister(1);
    return true;
}

bool test_mac_pairwise_key_flush_list()
{

    protocol_core_stub.entry_ptr = NULL;
    if( -1 != mac_pairwise_key_flush_list(0) ){
        return false;
    }

    protocol_interface_info_entry_t info_entry;
    mle_neigh_table_entry_t mle_neigh_table_entry;
    arm_15_4_mac_parameters_t mac_parameters;
    uint8_t eui64[8];
    uint8_t key[16];

    memset(&info_entry, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    memset(eui64, 0, 8);
    info_entry.mac_parameters = &mac_parameters;
    info_entry.id = 1;
    nsdynmemlib_stub.returnCounter = 2;
    mac_helper_stub.int8_value = 0;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    protocol_core_stub.entry_ptr = &info_entry;

    //Register interface here
    nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_interface_register(0, 6, 3);

    if( 0 != mac_pairwise_key_flush_list(0) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_add(0, 0, eui64, key);
    if( 0 != mac_pairwise_key_flush_list(0) ){
        return false;
    }


    if( -1 != mac_pairwise_key_flush_list(1) ){
        return false;
    }

    mac_pairwise_key_interface_unregister(0);
    return true;
}

bool test_mac_pairwise_key_interface_register() {

    protocol_interface_info_entry_t info_entry;
    mle_neigh_table_entry_t mle_neigh_table_entry;
    arm_15_4_mac_parameters_t mac_parameters;
    uint8_t eui64[8];
    uint8_t key[16];

    memset(&info_entry, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    memset(eui64, 0, 8);
    info_entry.mac_parameters = &mac_parameters;
    info_entry.id = 1;
    nsdynmemlib_stub.returnCounter = 2;
    mac_helper_stub.int8_value = 0;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    protocol_core_stub.entry_ptr = NULL;



    if (mac_pairwise_key_interface_register(0, 0, 3) == 0) {
        return false;
    }

    if (mac_pairwise_key_interface_register(0, 3, 0) == 0) {
        return false;
    }

    if (mac_pairwise_key_interface_register(0, 1, 3) == 0) {
        return false;
    }

    if (mac_pairwise_key_interface_register(0, 3, 3) == 0) {
        return false;
    }
    protocol_core_stub.entry_ptr = &info_entry;
    nsdynmemlib_stub.returnCounter = 0;
    if (mac_pairwise_key_interface_register(0, 3, 3) == 0) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if (mac_pairwise_key_interface_register(0, 6, 3) == 0) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (mac_pairwise_key_interface_register(0, 6, 3) != 0) {
        return false;
    }

    if (mac_pairwise_key_interface_register(0, 3, 3) == 0) {
        return false;
    }

    mac_pairwise_key_interface_unregister(0);


    return true;
}

bool test_mac_pairwise_key_interface_unregister() {

    protocol_interface_info_entry_t info_entry;
    mle_neigh_table_entry_t mle_neigh_table_entry;
    arm_15_4_mac_parameters_t mac_parameters;
    uint8_t eui64[8];
    uint8_t key[16];

    memset(&info_entry, 0, sizeof(protocol_interface_info_entry_t));
    memset(&mac_parameters, 0, sizeof(arm_15_4_mac_parameters_t));
    memset(&mle_neigh_table_entry, 0, sizeof(mle_neigh_table_entry_t));
    memset(eui64, 0, 8);
    info_entry.mac_parameters = &mac_parameters;
    info_entry.id = 1;
    nsdynmemlib_stub.returnCounter = 2;
    mac_helper_stub.int8_value = 0;
    mle_stub.mle_neigh_table_entry_ptr = &mle_neigh_table_entry;
    protocol_core_stub.entry_ptr = NULL;


    if (mac_pairwise_key_interface_unregister(0) != -1) {
        return false;
    }
    protocol_core_stub.entry_ptr = &info_entry;
    if (mac_pairwise_key_interface_unregister(0) != -1) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    mac_pairwise_key_interface_register(0, 6, 3);
    if (mac_pairwise_key_interface_unregister(0) != 0) {
        return false;
    }

    return true;
}

