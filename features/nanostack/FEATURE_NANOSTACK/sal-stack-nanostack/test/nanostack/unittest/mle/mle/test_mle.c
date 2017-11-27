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
#include "MLE/mle.h"

#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_event_timer.h"
#include "socket_api.h"
#include "Core/include/socket.h"
#include "nsdynmemLIB.h"
#include "ns_trace.h"
#include "string.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "Common_Protocols/udp.h"
#include "randLIB.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#include "common_functions.h"
#include "MLE/mle_tlv.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/etx/etx.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "mac_api.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"

#include "nsdynmemLIB_stub.h"
#include "common_functions_stub.h"
#include "event_stub.h"
#include "mle_tlv_stub.h"
#include "mac_helper_stub.h"
#include "protocol_core_stub.h"



static void tester_mle_entry_user_entry_remove_notify(int8_t interface_id, mle_neigh_table_entry_t *entry_ptr)
{

}

static int8_t tester_mle_entry_link_keep_alive(int8_t interface_id, const uint8_t *mac64)
{
    return 0;
}

static bool test_interface_is_active = true;

static bool tester_interface_is_active(int8_t interface_id) {
    return test_interface_is_active;
}



bool test_mle_class_deallocate()
{
    nsdynmemlib_stub.returnCounter = 0;
    if (-1 != mle_class_deallocate(0) ) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    event_stub.int8_value = 0;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(1, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (-1 != mle_class_deallocate(2) ) {
        return false;
    }

    if (0 != mle_class_deallocate(0) ) {
        return false;
    }
    if (0 != mle_class_deallocate(1) ) {
        return false;
    }

    return true;
}

bool test_mle_class_init()
{
    nsdynmemlib_stub.returnCounter = 0;
    event_stub.int8_value = -1;
    if (-2 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    event_stub.int8_value = 0;
    if (-2 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if (-2 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
            return false;
        }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
        if (0 != mle_class_init(0, 60, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
            return false;
        }

    nsdynmemlib_stub.returnCounter = 0;
    if (-2 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 0;
    if (0 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    if (0 != mle_class_init(1, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
        if (0 != mle_class_init(1, 40, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
            return false;
        }

    mle_class_deallocate(0);
    mle_class_deallocate(1);

    return true;
}


bool test_mle_class_mode_set()
{
    if (-1 !=mle_class_mode_set(0, MLE_CLASS_ROUTER)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);
    if (0 !=mle_class_mode_set(0, MLE_CLASS_ROUTER)) {
        return false;
    }
    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_set_new_key_pending()
{
    uint8_t ll64[16];
    memset(ll64, 0, sizeof(ll64));
    ll64[0] = 0xfe;
    ll64[1] = 0x80;
    mle_neigh_table_entry_t *mle_entry;

    if (-1 != mle_class_set_new_key_pending(0)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);
    if (!mle_entry) {
        return false;
    }

    if (0 !=mle_class_set_new_key_pending(0)) {
        return false;
    }

    mle_class_deallocate(0);

    return true;
}

bool test_mle_class_free_entry_count_get()
{
    uint8_t ll64[16];
    memset(ll64, 0, sizeof(ll64));
    ll64[0] = 0xfe;
    ll64[1] = 0x80;
    mle_neigh_table_entry_t *mle_entry;

    if (0 != mle_class_free_entry_count_get(0)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    if (0 != mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active)) {
        return false;
    }

    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);

    if (!mle_entry) {
        return false;
    }

    if (0 == mle_class_free_entry_count_get(0)) {
        return false;
    }

    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_get_entry_by_ll64()
{
    uint8_t ll64[16];
    memset(ll64, 0, 16);
    mle_neigh_table_entry_t *mle_entry;
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,false);
    if (mle_entry) {
        return false;
    }
    memset(ll64, 0, 16);
    ll64[0] = 0xfe;
    ll64[1] = 0x80;
    memcpy(&ll64[8], ADDR_SHORT_ADR_SUFFIC, 6);
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,false);
    if (mle_entry) {
        return false;
    }
    memset(&ll64[8], 0, 6);
    ll64[8] = 0x02;
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,false);
    if (mle_entry) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,false);
    if (mle_entry) {
        return false;
    }

    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);
    if (!mle_entry) {
        return false;
    }
    uint8_t test_tribute = mle_entry->attribute_index;
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);
    if (!mle_entry || test_tribute != mle_entry->attribute_index) {
        return false;
    }

    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,false);
    if (!mle_entry || test_tribute != mle_entry->attribute_index) {
        return false;
    }

    for (uint8_t i= 1; i< 32; i++) {
        ll64[15] = i;
        mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);
        if (!mle_entry ) {
            return false;
        }
    }

    ll64[15] = 33;
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);
    if (mle_entry ) {
        return false;
    }


    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_discover_entry_by_ll64()
{
    uint8_t ll64[16];
    memset(ll64, 0, 16);
    mle_neigh_table_entry_t *mle_entry;
    mle_entry = mle_class_discover_entry_by_ll64(0,ll64);
    if (mle_entry) {
        return false;
    }
    memset(ll64, 0, 16);
    ll64[0] = 0xfe;
    ll64[1] = 0x80;
    memcpy(&ll64[8], ADDR_SHORT_ADR_SUFFIC, 6);
    mle_entry = mle_class_discover_entry_by_ll64(0,ll64);
    if (mle_entry) {
        return false;
    }
    memset(&ll64[8], 0, 6);
    ll64[8] = 0x02;
    mle_entry = mle_class_discover_entry_by_ll64(0,ll64);
    if (mle_entry) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    mle_entry = mle_class_discover_entry_by_ll64(0,ll64);
    if (mle_entry) {
        return false;
    }
    //add address
    mle_entry = mle_class_get_entry_by_ll64(0, 0,ll64 ,true);

    uint8_t test_tribute = mle_entry->attribute_index;
    mle_entry = mle_class_discover_entry_by_ll64(0,ll64);
    if (!mle_entry || test_tribute != mle_entry->attribute_index) {
        return false;
    }

    mle_class_deallocate(0);
    return true;
}



bool test_mle_class_get_by_link_address()
{
    uint8_t mac64[8];
    uint8_t mac16[2];
    memset(mac64, 0, 8);
    memset(mac16, 0, 2);

    if (mle_class_get_by_link_address(0,mac64, ADDR_NONE)) {
        return false;
    }

    if (mle_class_get_by_link_address(0,mac64, ADDR_802_15_4_LONG)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (mle_class_get_by_link_address(0,mac64, ADDR_802_15_4_LONG)) {
        return false;
    }

    mle_class_get_entry_by_mac64(0,0,mac64, true);


    mle_neigh_table_entry_t *mle_entry = mle_class_get_by_link_address(0,mac64, ADDR_802_15_4_LONG);
    if (!mle_entry) {
        return false;
    }

    mle_entry->short_adr = 0;
    common_functions_stub.uint16_value = 0;
    mle_entry = mle_class_get_by_link_address(0,mac16, ADDR_802_15_4_SHORT);
    if (!mle_entry) {
        return false;
    }

    mle_entry->short_adr = 1;
    common_functions_stub.uint16_value = 2;
    mle_entry = mle_class_get_by_link_address(0,mac16, ADDR_802_15_4_SHORT);
    if (mle_entry) {
        return false;
    }

    common_functions_stub.uint16_value = 1;
    mle_entry = mle_class_get_by_link_address(0,mac16, ADDR_802_15_4_SHORT);
    if (!mle_entry) {
        return false;
    }
    mac64[7] = 1;
    if (mle_class_get_by_link_address(0,mac64, ADDR_802_15_4_LONG)) {
        return false;
    }
    mle_class_get_entry_by_mac64(0,0,mac64, true);

    if (!mle_class_get_by_link_address(0,mac64, ADDR_802_15_4_LONG)) {
        return false;
    }


    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_get_by_device_attribute_id()
{
    uint8_t mac64_1[8];
    uint8_t mac64_2[8];
    memset(mac64_1, 0, 8);
    memset(mac64_2, 0, 8);
    mac64_1[7] = 1;
    mac64_2[7] = 2;
    uint8_t attribute1, attribute2;
    if (mle_class_get_by_device_attribute_id(0, 0) ) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (mle_class_get_by_device_attribute_id(0, 0) ) {
        return false;
    }


    mle_neigh_table_entry_t *mle_entry = mle_class_get_entry_by_mac64(0,0,mac64_1, true);
    attribute1 = mle_entry->attribute_index;
    mle_entry = mle_class_get_entry_by_mac64(0,0,mac64_2, true);
    attribute2 = mle_entry->attribute_index;


    if (mle_class_get_by_device_attribute_id(0, 3) ) {
            return false;
        }

    mle_entry = mle_class_get_by_device_attribute_id(0, attribute1);
    if (!mle_entry || memcmp(mle_entry->mac64,mac64_1, 8 ) != 0) {
        return false;
    }

    mle_entry = mle_class_get_by_device_attribute_id(0, attribute2);
    if (!mle_entry || memcmp(mle_entry->mac64,mac64_2, 8 ) != 0) {
        return false;
    }

    if (mle_class_get_by_device_attribute_id(0, 17) ) {
        return false;
    }

    mle_class_deallocate(0);
    return true;
}

static bool tester_remove_ok = false;
static mle_neigh_table_entry_t *tester_mle_entry;
static void tester2_mle_entry_user_entry_remove_notify(int8_t interface_id, mle_neigh_table_entry_t *entry_ptr)
{
    if (tester_mle_entry == entry_ptr) {
        tester_remove_ok = true;
    }
}


bool test_mle_class_remove_entry()
{
    uint8_t mac64_1[8];
    uint8_t mac64_2[8];
    memset(mac64_1, 0, 8);
    memset(mac64_2, 0, 8);
    mac64_1[7] = 1;
    mac64_2[7] = 2;
    if (-1 != mle_class_remove_entry(0, NULL) ) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester2_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (-2 != mle_class_remove_entry(0, NULL) ) {
        return false;
    }

    mle_neigh_table_entry_t *mle_entry_1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);
    mle_neigh_table_entry_t *mle_entry_2 = mle_class_get_entry_by_mac64(0,0,mac64_2, true);
    tester_mle_entry = mle_entry_1;
    tester_remove_ok = false;

    mle_entry_2->short_adr = 1;

    if (0 != mle_class_remove_entry(0, mle_entry_1) ) {
        return false;
    }

    if (!tester_remove_ok) {
        return false;
    }

    if (-2 != mle_class_remove_entry(0, mle_entry_1) ) {
        return false;
    }

    if (mle_class_get_by_link_address(0,mac64_1, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (!mle_class_get_by_link_address(0,mac64_2, ADDR_802_15_4_LONG) ) {
        return false;
    }


    tester_mle_entry = mle_entry_2;
    tester_remove_ok = false;
    //SET interface ptr stub
    protocol_interface_info_entry_t interface_ptr;
    memset(&interface_ptr, 0, sizeof(protocol_interface_info_entry_t));

    protocol_core_stub.entry_ptr = &interface_ptr;

    if (0 != mle_class_remove_entry(0, mle_entry_2) ) {
        protocol_core_stub.entry_ptr = NULL;
        return false;
    }

    if (!tester_remove_ok) {
        protocol_core_stub.entry_ptr = NULL;
        return false;
    }

    if (-2 != mle_class_remove_entry(0, mle_entry_2) ) {
        protocol_core_stub.entry_ptr = NULL;
        return false;
    }
    protocol_core_stub.entry_ptr = NULL;
    //Test Indirect free


    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_remove_neighbour()
{
    uint8_t mac64_1[8];
        uint8_t mac64_2[8];
        uint8_t mac64_3[8];
        memset(mac64_1, 0, 8);
        memset(mac64_2, 0, 8);
        memset(mac64_3, 0, 8);
        mac64_1[7] = 1;
        mac64_2[7] = 2;
        mac64_3[7] = 3;
        uint8_t mac16[2];

    if (-1 != mle_class_remove_neighbour(0, mac64_1, ADDR_802_15_4_LONG)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester2_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    mle_neigh_table_entry_t *mle_entry_1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);
    mle_neigh_table_entry_t *mle_entry_2 = mle_class_get_entry_by_mac64(0,0,mac64_2, true);
    mle_neigh_table_entry_t *mle_entry_3 = mle_class_get_entry_by_mac64(0,0,mac64_3, true);

    mle_entry_1->short_adr = 1;
    mle_entry_2->short_adr = 2;
    mle_entry_3->short_adr = 3;

    //Delete first by MAC16 midle of device

    tester_mle_entry = mle_entry_2;
    tester_remove_ok = false;

    common_functions_stub.uint16_value = 2;
    if (0 != mle_class_remove_neighbour(0, mac16, ADDR_802_15_4_SHORT) ) {
        return false;
    }

    if (!tester_remove_ok) {
        return false;
    }

    if (-2 != mle_class_remove_neighbour(0, mac16, ADDR_802_15_4_SHORT) ) {
        return false;
    }

    if (!mle_class_get_by_link_address(0,mac64_1, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (mle_class_get_by_link_address(0,mac64_2, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (!mle_class_get_by_link_address(0,mac64_3, ADDR_802_15_4_LONG) ) {
        return false;
    }

    tester_mle_entry = mle_entry_1;
    tester_remove_ok = false;

    if (0 != mle_class_remove_neighbour(0, mac64_1, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (!tester_remove_ok) {
        return false;
    }

    if (-2 != mle_class_remove_neighbour(0, mac64_1, ADDR_802_15_4_LONG) ) {
            return false;
        }

    if (mle_class_get_by_link_address(0,mac64_1, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (!mle_class_get_by_link_address(0,mac64_3, ADDR_802_15_4_LONG) ) {
        return false;
    }

    tester_mle_entry = mle_entry_3;
    tester_remove_ok = false;

    if (0 != mle_class_remove_neighbour(0, mac64_3, ADDR_802_15_4_LONG) ) {
        return false;
    }

    if (-2 != mle_class_remove_neighbour(0, mac64_3, ADDR_802_15_4_LONG) ) {
            return false;
        }

    if (mle_class_get_by_link_address(0,mac64_3, ADDR_802_15_4_LONG) ) {
        return false;
    }


    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_active_list_get()
{
    if (mle_class_active_list_get(0) ) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester2_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);
    if (!mle_class_active_list_get(0)){
        return false;
    }
    mle_class_deallocate(0);
    return true;
}

bool test_mle_class_active_neigh_counter()
{
    uint8_t mac64_1[8];
    uint8_t mac64_2[8];
    memset(mac64_1, 0, 8);
    memset(mac64_2, 0, 8);

    mac64_1[7] = 1;
    mac64_2[7] = 2;

    if (0xffff != mle_class_active_neigh_counter(0)) {
        return false;
    }
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester2_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (0 != mle_class_active_neigh_counter(0)) {
        return false;
    }

    mle_class_get_entry_by_mac64(0,0,mac64_1, true);
    if (1 != mle_class_active_neigh_counter(0)) {
        return false;
    }
    mle_class_get_entry_by_mac64(0,0,mac64_2, true);
    if (2 != mle_class_active_neigh_counter(0)) {
        return false;
    }

    mle_class_deallocate(0);
    return true;
}

bool test_mle_refresh_entry_timeout()
{
    uint8_t mac64_1[8];
    memset(mac64_1, 0, 8);
    mac64_1[7] = 1;

    mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, false);
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester2_mle_entry_user_entry_remove_notify, &tester_mle_entry_link_keep_alive, &tester_interface_is_active);
    if (mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, false) ) {
        return false;
    }
    mle_neigh_table_entry_t * entry1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);

    if (!mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, false) ) {
        return false;
    }

    entry1->handshakeReady = true;
    entry1->timeout_rx = 20;

    entry1->ttl = 5;

    mle_neigh_table_entry_t *temp_entry = mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, false);
    if (!temp_entry || temp_entry->ttl != 20) {
        return false;
    }
    entry1->ttl = 3;
    temp_entry = mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, true);
    if (!temp_entry || temp_entry->ttl != 3) {
        return false;
    }

    entry1->ttl = 5;
    temp_entry = mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, true);
    if (!temp_entry || temp_entry->ttl != 20) {
        return false;
    }

    temp_entry->short_adr = 1;
    common_functions_stub.uint16_value = 1;

    entry1->ttl = 5;
    temp_entry = mle_refresh_entry_timeout(0, mac64_1, ADDR_802_15_4_LONG, true);
    if (!temp_entry || temp_entry->ttl != 20) {
        return false;
    }

    mle_class_deallocate(0);
    return true;
}

bool test_mle_entry_timeout_update()
{
    mle_neigh_table_entry_t temp_entry;
    memset(&temp_entry, 0, sizeof(mle_neigh_table_entry_t));


    mle_entry_timeout_update(&temp_entry, 0);
    if (temp_entry.ttl != 500 /MLE_TIMER_TICKS_SECONDS + 1) {
        return false;
    }

    mle_entry_timeout_update(&temp_entry, 86401);
    if (temp_entry.ttl != 86400 /MLE_TIMER_TICKS_SECONDS + 1) {
        return false;
    }

    mle_entry_timeout_update(&temp_entry, 200);
    if (temp_entry.ttl != 200 /MLE_TIMER_TICKS_SECONDS + 1) {
        return false;
    }

    temp_entry.ttl = 100;
    mle_entry_timeout_refresh(&temp_entry);
    if (temp_entry.ttl != 200 /MLE_TIMER_TICKS_SECONDS + 1) {
        return false;
    }

    return true;
}

static bool tester2_remove_ok = false;
static mle_neigh_table_entry_t *tester2_mle_entry;
static void tester3_mle_entry_user_entry_remove_notify(int8_t interface_id, mle_neigh_table_entry_t *entry_ptr)
{
    if (tester2_mle_entry == entry_ptr) {
        tester2_remove_ok = true;
    }
}
static int8_t keep_response = 0;
static int8_t tester2_mle_entry_link_keep_alive(int8_t interface_id, const uint8_t *mac64)
{
    return keep_response;
}

bool test_mle_event_handler()
{
    uint8_t mac64_1[8];
    memset(mac64_1, 0, 8);
    mac64_1[7] = 1;
    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester3_mle_entry_user_entry_remove_notify, &tester2_mle_entry_link_keep_alive, &tester_interface_is_active);

    mle_class_mode_set(0, MLE_CLASS_ROUTER);

    mle_neigh_table_entry_t * entry1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);

    entry1->timeout_rx = 6;
    entry1->ttl = 6;

    //Event send
    arm_event_s event = {
            .receiver = 0,
            .sender = 0,
            .event_id = 0,
            .data_ptr = NULL,
            .event_type = 1,
            .priority = 0,
    };
    eventOS_event_send(&event);

    if (entry1->ttl != 5) {
        return false;
    }

    test_interface_is_active = false;

    eventOS_event_send(&event);

    if (entry1->ttl != 5) {
        return false;
    }

    test_interface_is_active = true;

    entry1->ttl = 1;
    tester2_remove_ok = false;
    tester2_mle_entry = entry1;
    eventOS_event_send(&event);
    if (!tester2_remove_ok) {
        return false;
    }
    if (mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }

    mle_class_mode_set(0, MLE_CLASS_END_DEVICE);

    entry1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);

    entry1->ttl = 5;
    entry1->priorityFlag = true;
    eventOS_event_send(&event);

    if (entry1->ttl != 4) {
        return false;
    }

    eventOS_event_send(&event);

    if (entry1->ttl != 1) {
        return false;
    }

    eventOS_event_send(&event);

    if (entry1->ttl != 1) {
        return false;
    }

    entry1->ttl = 4;
    keep_response = -1;
    eventOS_event_send(&event);

    if (entry1->ttl != 3) {
        return false;
    }

    eventOS_event_send(&event);
    if (entry1->ttl != 2) {
            return false;
    }

    eventOS_event_send(&event);

    if (mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }

    entry1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);

    entry1->ttl = 3;
    entry1->priorityFlag = false;
    eventOS_event_send(&event);
    if (entry1->ttl != 2) {
        return false;
    }
    entry1->ttl = 1;
    eventOS_event_send(&event);
    if (mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }

    mle_class_mode_set(0, MLE_CLASS_SLEEPY_END_DEVICE);

    entry1 = mle_class_get_entry_by_mac64(0,0,mac64_1, true);

    entry1->ttl = 5;
    entry1->priorityFlag = true;
    eventOS_event_send(&event);

    if (entry1->ttl != 4) {
        return false;
    }

    entry1->ttl = 1;
    tester2_remove_ok = false;
    tester2_mle_entry = entry1;
    eventOS_event_send(&event);
    if (!tester2_remove_ok) {
        return false;
    }
    if (mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }

    event.event_type = 0;
    eventOS_event_send(&event);

    mle_class_deallocate(0);
    return true;
}

bool test_mle_neigh_entry_frame_counter_update()
{

    protocol_interface_info_entry_t entry;
    mle_neigh_table_entry_t entry_temp;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    entry.mac_api = &api;
    uint8_t tem_buf[16];

    entry.mac_parameters->SecurityEnabled = false;

    if (!mle_neigh_entry_frame_counter_update(&entry_temp,tem_buf, 16, &entry, 1)) {
        return false;
    }
    entry.mac_parameters->SecurityEnabled = true;

    mle_tlv_stub.expectedInt = 4;

    if (!mle_neigh_entry_frame_counter_update(&entry_temp,tem_buf, 16, &entry, 1)) {
        return false;
    }

    mle_tlv_stub.expectedInt = 0;

    if (mle_neigh_entry_frame_counter_update(&entry_temp,tem_buf, 16, &entry, 1)) {
        return false;
    }

    return true;
}

bool test_mle_general_write_source_address()
{
    protocol_interface_info_entry_t entry;
    mle_neigh_table_entry_t entry_temp;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    entry.mac_api = &api;
    uint8_t tem_buf[16];
    uint8_t *ptr1 = tem_buf;
    uint8_t *ptr2;

    entry.global_address_available = false;
    ptr2 = mle_general_write_source_address(ptr1, &entry);
    if (ptr2 != ptr1) {
        return false;
    }

    entry.global_address_available = true;
    mac_helper_stub.uint16_value = 0xffff;
    ptr2 = mle_general_write_source_address(ptr1, &entry);
    if (ptr2 != ptr1) {
        return false;
    }

    mac_helper_stub.uint16_value = 0;
    ptr2 = mle_general_write_source_address(ptr1, &entry);
    if (ptr2 != ptr1 + 4) {
        return false;
    }
    return true;
}

bool test_mle_general_write_link_layer_framecounter()
{
    protocol_interface_info_entry_t entry;
    mle_neigh_table_entry_t entry_temp;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t params;
    entry.mac_parameters = &params;
    mac_api_t api;
    entry.mac_api = &api;
    uint8_t tem_buf[16];
    uint8_t *ptr1 = tem_buf;
    uint8_t *ptr2;
    ptr2 = mle_general_write_link_layer_framecounter(ptr1, &entry);
    if (ptr2 != ptr1 + 6) {
            return false;
        }

    return true;
}

bool test_mle_class_list_clean()
{

    uint8_t mac64_1[8];
    memset(mac64_1, 0, 8);
    mac64_1[7] = 1;

    if (-1 != mle_class_list_clean(0)) {
        return false;
    }


    event_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 2;
    mle_class_init(0, 32, &tester3_mle_entry_user_entry_remove_notify, &tester2_mle_entry_link_keep_alive, &tester_interface_is_active);

    if (0 != mle_class_list_clean(0)) {
        return false;
    }

    mle_class_get_entry_by_mac64(0,0,mac64_1, true);
    if (!mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }

    if (0 != mle_class_list_clean(0)) {
        return false;
    }

    if (mle_class_get_entry_by_mac64(0,0,mac64_1, false)) {
        return false;
    }


    mle_class_deallocate(0);
    return true;
}

