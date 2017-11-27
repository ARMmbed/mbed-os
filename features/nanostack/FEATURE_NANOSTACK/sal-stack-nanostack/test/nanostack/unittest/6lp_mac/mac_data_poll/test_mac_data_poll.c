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
#include "test_mac_data_poll.h"
#include <string.h>

#include "mac_data_poll.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "mac_api.h"

#include "system_timer_stub.h"
#include "protocol_6lowpan_bootstrap_stub.h"
#include "nsdynmemLIB_stub.h"
#include "event_stub.h"
#include "protocol_core_stub.h"
#include "mac_helper_stub.h"

bool visited = false;

void poll_fail(int8_t nwk_interface_id)
{
    visited = true;
}

void buffer_handler(buffer_t *a)
{

}

void mlme_req_cb(const mac_api_t* api, mlme_primitive id, const void *data)
{

}

bool test_mac_data_poll_disable()
{
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    mac_data_poll_disable(NULL);

    mac_data_poll_disable(&entry);

    entry.rfd_poll_info = &rf_ptr;
    rf_ptr.protocol_poll = 2;
    mac_data_poll_disable(&entry);
    if (rf_ptr.protocol_poll) {
        return false;
    }
    return true;
}

bool test_mac_poll_timer_trig()
{
    //void mac_poll_timer_trig(uint32_t poll_time, protocol_interface_info_entry_t *cur)
    mac_poll_timer_trig(0, NULL);
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));

    entry.lowpan_info = 64;
    system_timer_stub.int8_value = -1;

    mac_poll_timer_trig(100, &entry);

    mac_poll_timer_trig(10, &entry);

    return true;
}

bool test_mac_mlme_poll_confirm()
{
    mac_mlme_poll_confirm(NULL, NULL);
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    mlme_poll_conf_t conf;

    mac_mlme_poll_confirm(&entry, &conf);

    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;

    conf.status = MLME_SUCCESS;
    mac_mlme_poll_confirm(&entry, &conf);

    conf.status = MLME_NO_DATA;
    rf_ptr.protocol_poll = 0;
    mac_mlme_poll_confirm(&entry, &conf);

    rf_ptr.protocol_poll = 1;
    mac_mlme_poll_confirm(&entry, &conf);

    conf.status = MLME_INVALID_ADDRESS;
    mac_mlme_poll_confirm(&entry, &conf);

    visited = false;
    rf_ptr.pollFailCb = poll_fail;
    rf_ptr.nwk_parent_poll_fail = 5;
    mac_mlme_poll_confirm(&entry, &conf);
    if( !visited ){
        return false;
    }

    return true;
}

bool test_mac_data_poll_init_protocol_poll()
{

    mac_data_poll_init_protocol_poll(NULL);

    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.lowpan_info = 64;

    rf_ptr.protocol_poll = 0;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    rf_ptr.pollActive = false;
    mac_data_poll_init_protocol_poll(&entry);

    return true;
}

bool test_mac_data_poll_host_poll_time_max()
{
    if( 0 != mac_data_poll_host_poll_time_max(NULL) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s info;
    info.slow_poll_rate_seconds = 2;
    entry.rfd_poll_info = &info;
    if( 2 != mac_data_poll_host_poll_time_max(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_data_poll_host_timeout()
{
    if( 0 != mac_data_poll_host_timeout(NULL) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    rf_ptr.timeOutInSeconds = 6;

    if( 6 != mac_data_poll_host_timeout(&entry) ){
        return false;
    }
    return true;
}

bool test_mac_data_poll_protocol_poll_mode_decrement()
{
    mac_data_poll_protocol_poll_mode_decrement(NULL);
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.lowpan_info = 1;
    mac_data_poll_protocol_poll_mode_decrement(&entry);

    //Test mac_data_poll_protocol_poll_internal_cancel here -->
    entry.lowpan_info = 64;
    mac_data_poll_protocol_poll_mode_decrement(&entry);

    rf_ptr.protocol_poll = 0;
    mac_data_poll_protocol_poll_mode_decrement(&entry);

    rf_ptr.protocol_poll = 1;
    rf_ptr.pollActive = false;
    rf_ptr.nwk_app_poll_time = 1;
    mac_data_poll_protocol_poll_mode_decrement(&entry);

    rf_ptr.protocol_poll = 1;
    rf_ptr.pollActive = false;
    rf_ptr.nwk_app_poll_time = 0;
    mac_data_poll_protocol_poll_mode_decrement(&entry);
    // <-- Test mac_data_poll_protocol_poll_internal_cancel here

    return true;
}

bool test_mac_data_poll_protocol_poll_mode_disable()
{
    mac_data_poll_protocol_poll_mode_disable(NULL);
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.lowpan_info = 1;
    mac_data_poll_protocol_poll_mode_disable(&entry);

    entry.lowpan_info = 64;
    mac_data_poll_protocol_poll_mode_disable(&entry);
    return true;
}

bool test_mac_data_poll_enable_check()
{
    mac_data_poll_enable_check(NULL);

    protocol_interface_info_entry_t entry;
    protocol_core_stub.entry_ptr = &entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.lowpan_info = 1;
    mac_data_poll_enable_check(&entry);

    //Test mac_data_poll_protocol_internal here -->
    entry.lowpan_info = 64;
    rf_ptr.protocol_poll = 0;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    rf_ptr.pollActive = false;
    mac_data_poll_enable_check(&entry);

    //<-- Test mac_data_poll_protocol_internal here

    return true;
}

bool test_mac_data_poll_host_mode_get()
{
    mac_data_poll_host_mode_get(NULL, NULL);
    net_host_mode_t mode;
    protocol_interface_info_entry_t entry;

    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.rfd_poll_info->host_mode = NET_HOST_MODE_NOT_ACTIVE;

    mac_data_poll_host_mode_get(&entry, &mode);
    if( NET_HOST_MODE_NOT_ACTIVE != mode ){
        return false;
    }
    return true;
}

bool test_mac_data_poll_host_mode_set()
{
    if( -1 != mac_data_poll_host_mode_set(NULL, 0, 0) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    protocol_core_stub.entry_ptr = NULL;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t mac;
    memset(&mac, 0, sizeof(arm_15_4_mac_parameters_t));
    entry.mac_parameters = &mac;
    if( -1 != mac_data_poll_host_mode_set(&entry, 0, 0) ){
        return false;
    }
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;

    entry.lowpan_info = INTERFACE_NWK_ROUTER_DEVICE;
    if( -3 != mac_data_poll_host_mode_set(&entry, 0, 0) ){
        return false;
    }

    entry.lowpan_info = 8;

    if( -2 != mac_data_poll_host_mode_set(&entry, NET_HOST_SLOW_POLL_MODE, 0) ){
        return false;
    }

    protocol_core_stub.entry_ptr = &entry;
    rf_ptr.host_mode = NET_HOST_RX_ON_IDLE;
    if( 0 != mac_data_poll_host_mode_set(&entry, NET_HOST_SLOW_POLL_MODE, 1) ){
        return false;
    }

    protocol_6lowpan_bootstrap_stub.int_value = -1;
    rf_ptr.host_mode = NET_HOST_RX_ON_IDLE;
    if( -3 != mac_data_poll_host_mode_set(&entry, NET_HOST_SLOW_POLL_MODE, 1) ){
        return false;
    }

    rf_ptr.host_mode = NET_HOST_RX_ON_IDLE;
    protocol_6lowpan_bootstrap_stub.int_value = 0;
    if( 0 != mac_data_poll_host_mode_set(&entry, NET_HOST_FAST_POLL_MODE, 0) ){
        return false;
    }

    rf_ptr.host_mode = NET_HOST_RX_ON_IDLE;
    protocol_6lowpan_bootstrap_stub.int_value = -1;
    if( -3 != mac_data_poll_host_mode_set(&entry, NET_HOST_FAST_POLL_MODE, 0) ){
        return false;
    }

    // Test host_link_configuration rx_on_idle == true here -->
    protocol_6lowpan_bootstrap_stub.int_value = -1;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    if( -3 != mac_data_poll_host_mode_set(&entry, NET_HOST_RX_ON_IDLE, 0) ){
        return false;
    }

    mac.RxOnWhenIdle = true;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    if( -3 != mac_data_poll_host_mode_set(&entry, NET_HOST_RX_ON_IDLE, 0) ){
        return false;
    }

    thread_info_t info;
    entry.thread_info = &info;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    mac.RxOnWhenIdle = false;
    if( 0 != mac_data_poll_host_mode_set(&entry, NET_HOST_RX_ON_IDLE, 0) ){
        return false;
    }
    mac.RxOnWhenIdle = true;

    entry.thread_info = NULL;
    protocol_6lowpan_bootstrap_stub.int_value = 0;
    rf_ptr.host_mode = NET_HOST_SLOW_POLL_MODE;
    if( 0 != mac_data_poll_host_mode_set(&entry, NET_HOST_RX_ON_IDLE, 0) ){
        return false;
    }

    // <-- Test host_link_configuration here

    //default case:
    if( -1 != mac_data_poll_host_mode_set(&entry, 50, 0) ){
        return false;
    }

    return true;
}

bool test_mac_data_poll_init()
{
    mac_data_poll_init(NULL);
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    arm_15_4_mac_parameters_t mac;
    memset(&mac, 0, sizeof(arm_15_4_mac_parameters_t));
    entry.mac_parameters = &mac;
    nsdynmemlib_stub.returnCounter = 1;
    nwk_rfd_poll_setups_s *rf_ptr = ns_dyn_mem_alloc(sizeof(nwk_rfd_poll_setups_s));
    memset(rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = rf_ptr;

    entry.lowpan_info = INTERFACE_NWK_ROUTER_DEVICE;
    mac_data_poll_init(&entry);

    entry.lowpan_info = 1; //!INTERFACE_NWK_ROUTER_DEVICE
    entry.rfd_poll_info = NULL;
    event_stub.int8_value = -1;
    mac_data_poll_init(&entry);

    event_stub.int8_value = 0;
    mac_data_poll_init(&entry);

    //Test mac_data_poll_cb here -->
    if( event_stub.func_ptr ){
        arm_event_s event;
        //Test mac_data_poll_cb_run here -->
        event.event_type = 1;
        protocol_core_stub.entry_ptr = NULL;
        event_stub.func_ptr(&event);
        protocol_interface_info_entry_t entry2;
        memset(&entry2, 0, sizeof(protocol_interface_info_entry_t));
        protocol_core_stub.entry_ptr = &entry2;
        event_stub.func_ptr(&event);

        nwk_rfd_poll_setups_s rf_ptr2;
        memset(&rf_ptr2, 0, sizeof(nwk_rfd_poll_setups_s));
        entry2.rfd_poll_info = &rf_ptr2;
        mac_helper_stub.uint8_value = MAC_ADDR_MODE_NONE;
        event_stub.func_ptr(&event);

        mac_helper_stub.uint8_value = MAC_ADDR_MODE_16_BIT;
        event_stub.func_ptr(&event);

        mac_api_t api;
        memset(&api, 0, sizeof(mac_api_t) );
        entry2.mac_api = &api;
        api.mlme_req = &mlme_req_cb;
        event_stub.func_ptr(&event);
        //<-- Test mac_data_poll_cb_run here

        event.event_type = 3;
        event_stub.func_ptr(&event);
    }

    //<-- Test mac_data_poll_cb here

    nsdynmemlib_stub.returnCounter = 1;
    mac_data_poll_init(&entry);

    mac.RxOnWhenIdle = true;
    mac_data_poll_init(&entry);

    mac.RxOnWhenIdle = false;
    mac_data_poll_init(&entry);

    ns_dyn_mem_free(entry.rfd_poll_info);

    return true;
}


bool test_mac_data_poll_get_max_sleep_period()
{
    if( 0 != mac_data_poll_get_max_sleep_period(NULL) ){
        return false;
    }
    protocol_interface_info_entry_t entry;
    memset(&entry, 0, sizeof(protocol_interface_info_entry_t));
    nwk_rfd_poll_setups_s rf_ptr;
    memset(&rf_ptr, 0, sizeof(nwk_rfd_poll_setups_s));
    entry.rfd_poll_info = &rf_ptr;
    entry.if_stack_buffer_handler = &buffer_handler;
    rf_ptr.pollActive = false;
    rf_ptr.protocol_poll = 1;
    if( 300 != mac_data_poll_get_max_sleep_period(&entry) ){
        return false;
    }
    rf_ptr.protocol_poll = 0;
    rf_ptr.nwk_app_poll_time = 400;
    if( 400 != mac_data_poll_get_max_sleep_period(&entry) ){
        return false;
    }
    return true;
}
