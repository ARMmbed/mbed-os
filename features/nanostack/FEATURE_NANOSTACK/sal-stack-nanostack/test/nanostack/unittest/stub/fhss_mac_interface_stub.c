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
#include <string.h>
#include "nsdynmemLIB.h"
#include "fhss_api.h"
#include "fhss_mac_interface_stub.h"

fhss_mac_interface_stub_def fhss_mac_interface_stub;

bool fhss_is_broadcast_channel_cb(const fhss_api_t *api)
{
    fhss_mac_interface_stub.int8_value++;
    return fhss_mac_interface_stub.channel_bool_value;
}

bool fhss_use_broadcast_queue_cb(const fhss_api_t *api, bool is_broadcast_addr, int frame_type)
{
    return fhss_mac_interface_stub.queue_bool_value;
}

int fhss_tx_handle_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t *destination_address, int frame_type, uint8_t *synch_info, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    fhss_mac_interface_stub.int8_value++;
    return fhss_mac_interface_stub.handle_value;
}

bool fhss_check_tx_conditions_cb(const fhss_api_t *api, bool is_broadcast_addr, uint8_t handle, int frame_type, uint16_t frame_length, uint8_t phy_header_length, uint8_t phy_tail_length)
{
    fhss_mac_interface_stub.int8_value++;
    if (frame_type == FHSS_SYNCH_REQUEST_FRAME) {
        return false;
    }
    return fhss_mac_interface_stub.cond_bool_value;
}

void fhss_receive_frame_cb(const fhss_api_t *api, uint16_t pan_id, uint8_t *source_address, uint32_t timestamp, uint8_t *synch_info, int frame_type)
{
    fhss_mac_interface_stub.int8_value++;
}

void fhss_data_tx_done_cb(const fhss_api_t *api, bool waiting_ack, bool tx_completed, uint8_t handle)
{
    fhss_mac_interface_stub.int8_value++;
}

bool fhss_data_tx_fail_cb(const fhss_api_t *api, uint8_t handle, int frame_type)
{
    fhss_mac_interface_stub.int8_value++;
    return true;
}

void fhss_synch_state_set_cb(const fhss_api_t *api, fhss_states fhss_state, uint16_t pan_id)
{
    fhss_mac_interface_stub.int8_value++;
}

uint32_t fhss_read_timestamp_cb(const fhss_api_t *api)
{
    fhss_mac_interface_stub.int8_value++;
    return fhss_mac_interface_stub.uint32_value;
}

uint16_t fhss_get_retry_period_cb(const fhss_api_t *api, uint8_t *destination_address, uint16_t phy_mtu)
{
    return 0;
}
int fhss_init_callbacks_cb(const fhss_api_t *api, fhss_callback_t *callbacks)
{

}
