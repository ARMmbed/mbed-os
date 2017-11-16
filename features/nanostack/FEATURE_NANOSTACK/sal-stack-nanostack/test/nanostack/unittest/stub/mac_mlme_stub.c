/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "common_functions.h"

#include "mac_mlme_stub.h"

mac_mlme_stub_def mac_mlme_stub;

uint8_t mac_mlme_beacon_req_tx(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    return mac_mlme_stub.uint8_value;
}

void mac_mlme_scan_request(const mlme_scan_t *msg, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

int8_t mac_mlme_start_req(const mlme_start_t *s, struct protocol_interface_rf_mac_setup *rf_mac_setup)
{
    return mac_mlme_stub.int8_value;
}

int8_t mac_mlme_reset(protocol_interface_rf_mac_setup_s *rf_mac_setup, const mlme_reset_t *reset)
{
    return mac_mlme_stub.int8_value;
}

void mac_extended_mac_set(protocol_interface_rf_mac_setup_s *rf_mac_setup, const uint8_t *mac64)
{
}

int8_t mac_mlme_set_req(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req)
{
    return mac_mlme_stub.int8_value;
}

int8_t mac_mlme_get_req(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_get_conf_t *get_req)
{
    return mac_mlme_stub.int8_value;
}

void mac_frame_src_address_set_from_interface(uint8_t SrcAddrMode,protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t *addressPtr)
{
}

void mac_mlme_active_scan_response_timer_start(void *interface)
{
}

void mac_mlme_event_cb(void *mac_ptr)
{
}

void mac_mlme_timer_cb(int8_t timer_id, uint16_t slots)
{
}

void mac_mlme_set_active_state(protocol_interface_rf_mac_setup_s *entry, bool new_state)
{
}

void mac_mlme_data_base_deallocate(struct protocol_interface_rf_mac_setup *rf_mac)
{
}

protocol_interface_rf_mac_setup_s *mac_mlme_data_base_allocate(uint8_t *mac64, arm_device_driver_list_s *dev_driver, mac_description_storage_size_t *storage_sizes)
{
    return mac_mlme_stub.setup_ptr;
}

uint8_t mac_mlme_set_new_sqn(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return mac_mlme_stub.uint8_value;
}

uint8_t mac_mlme_set_new_beacon_sqn(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return mac_mlme_stub.uint8_value;
}

uint16_t mac_mlme_get_panid(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return mac_mlme_stub.uint16_value;
}

int mac_mlme_beacon_notify(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_beacon_ind_t *data)
{
    //Copy Beacon
    mac_mlme_stub.beacon_ind.BSN = data->BSN;
    mac_mlme_stub.beacon_ind.PANDescriptor = data->PANDescriptor;
    mac_mlme_stub.beacon_ind.PendAddrSpec = data->PendAddrSpec;
    mac_mlme_stub.beacon_ind.beacon_data_length = data->beacon_data_length;
    memcpy(mac_mlme_stub.beacon_payload_buffer,data->beacon_data, data->beacon_data_length);

    return mac_mlme_stub.int_value;
}

void mac_mlme_scan_confirmation_handle(protocol_interface_rf_mac_setup_s *rf_ptr)
{
}

void mac_mlme_mac_radio_disabled(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

void mac_mlme_mac_radio_enable(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

int8_t mac_mlme_rf_channel_change(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t new_channel)
{
    return mac_mlme_stub.int8_value;
}

void mac_mlme_poll_process_confirm(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t status)
{
    mac_mlme_stub.pollConfirm = status;
}

void mac_mlme_poll_req(protocol_interface_rf_mac_setup_s *cur, const mlme_poll_t *poll_req)
{
}

int8_t mac_mlme_beacon_tx(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    return mac_mlme_stub.int8_value;
}

int8_t mac_mlme_link_beacon_join_priority_tx_callback_set(int8_t interface_id, beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr)
{
    return mac_mlme_stub.int8_value;
}

int8_t mac_mlme_link_beacon_compare_rx_callback_set(int8_t interface_id, beacon_compare_rx_cb *beacon_compare_rx_cb_ptr)
{
    return mac_mlme_stub.int8_value;
}

int8_t mac_mlme_virtual_confirmation_handle(int8_t driver_id, const uint8_t *data_ptr, uint16_t length)
{
    return 0;
}
